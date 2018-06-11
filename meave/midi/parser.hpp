#pragma once

#include <boost/range.hpp>
#include <cstdint>
#include <iterator>

#include "meave/commons.hpp"
#include "meave/lib/error.hpp"
#include "meave/midi/commons.hpp"
#include "meave/midi/detail.hpp"

namespace meave::midi {

template <typename DerivedT, typename RangeT>
class Parser {
	using It = typename boost::range_iterator<RangeT>::type;

	DerivedT* $() noexcept {
		return static_cast<DerivedT*>(this);
	}
	DerivedT const* $() const noexcept {
		return static_cast<DerivedT const*>(this);
	}

	It b() const {
		return boost::begin(data_);
	}

	It e() const {
		return boost::end(data_);
	}

	RangeT data_;

	ChunkType parse_chunk_header(It& it, uint32_t& length) const {
		static constexpr char str_MThd[] = { 'M', 'T', 'h', 'd' };
		static constexpr char str_MTrk[] = { 'M', 'T', 'r', 'k' };

		if (it + 4 + 4 > e())
			throw Error("Cannot parse chunk header: Premature end of data");

		ChunkType type = CHUNK_TYPE_UNKNOWN;
		if (std::equal(std::begin(str_MThd), std::end(str_MThd), it))
			type = CHUNK_TYPE_MThd;
		else if (std::equal(std::begin(str_MTrk), std::end(str_MTrk), it))
			type = CHUNK_TYPE_MTrk;
		it += 4;

		length = detail::read_int<uint32_t>(it, e());

		return type;
	}

	void parse_chunk_MThd(It& it, const uint32_t header_size) const {
		if (header_size != 6)
			throw Error("Unexpected header size: ") << header_size;

		if (it + 2 + 2 + 2 > e())
			throw Error("Cannot parse MThd header: Premature end of data");

		const auto file_format = detail::read_int<uint16_t>(it, e());
		if (file_format > 2)
			throw Error("Invalid value of file format: ") << file_format;

		const auto number_of_tracks = detail::read_int<uint16_t>(it, e());
		const auto delta_time_ticks_per_quarter_rate = detail::read_int<uint16_t>(it, e());

		$()->on_chunk_MThd(header_size, FileFormat(file_format), number_of_tracks, delta_time_ticks_per_quarter_rate);
	}

	void parse_event(const uns delta_time, It& it) const {
		LOG(INFO) << __FUNCTION__;

	}

	void parse_running_status(const uns delta_time, It& it) const {
		LOG(INFO) << __FUNCTION__;

	}

	void parse_meta_event(const uns delta_time, It& it) const {
		LOG(INFO) << __FUNCTION__;

		assert(uint8_t(*it) == 0xFF);
		++it;
		const uns type = uint8_t(*it);
		++it;
		const auto length = detail::read_varint(it, e());
		if (it + length > e())
			throw Error("Meta event with length: %u cannot fit into the data", length);
		const auto beg = it;
		it += length;
		$()->on_meta_event(delta_time, type, beg, it);
	}

	void parse_sysex(const uns delta_time, It& it) const {
		LOG(INFO) << __FUNCTION__;

	}

	void parse_chunk_MTrk(It& it, const uint32_t length) const {
		$()->on_chunk_MTrk(length);
		for (const It end = it + length; it < end;) {
			const uns delta_time = detail::read_varint(it, e());
			const uns x = uint8_t(*it);
			LOG(INFO) << "pos: " << (it - b()) << "; x: " << std::hex << x << std::dec << "; delta_time: " << delta_time;

			if (x == 0xFF) {
				parse_meta_event(delta_time, it);
			} else if (x == 0xF0) {
				parse_sysex(delta_time, it);
			} else if (x & 0x80) {
				parse_event(delta_time, it);
			} else {
				parse_running_status(delta_time, it);
			}
		}
	}

	void parse_chunk_unknown(It& it, const uint32_t length) const {
		$()->on_chunk_unknown(length);
		it += length;
	}

public:
	Parser(RangeT range): data_{std::move(range)} {}

	void operator()() const {
		bool was_chunk_MThd{false};
		for (It it = b(); it < e(); ) {
			uint32_t length = 0;
			LOG(INFO) << "Outer pos: " << (it - b());
			const ChunkType chunk_type = parse_chunk_header(it, length);
			switch (chunk_type) {
			case CHUNK_TYPE_MThd:
				if (was_chunk_MThd)
					throw Error("Chunk MThd seen more than once");
				was_chunk_MThd = true;
				parse_chunk_MThd(it, length);
				break;
			case CHUNK_TYPE_MTrk:
				if (!was_chunk_MThd)
					throw Error("Chunk MTrk seen before MThd chunk");
				parse_chunk_MTrk(it, length);
				break;
			case CHUNK_TYPE_UNKNOWN:
				parse_chunk_unknown(it, length);
				break;
			}
		}
	}

};

} /* namespace meave::midi */
