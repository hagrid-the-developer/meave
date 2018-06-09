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

	DerivedT& that() noexcept {
		return static_cast<DerivedT&>(*this);
	}
	DerivedT const& that() const noexcept {
		return static_cast<DerivedT const&>(*this);
	}

	It b() const {
		return boost::begin(data_);
	}

	It e() const {
		return boost::end(data_);
	}

	RangeT data_;

	bool parse_chunk_MThd(It& it) const {
		static constexpr char str_MThd[] = { 'M', 'T', 'h', 'd' };

		if (it + std::size(str_MThd) > e())
			return false;
		if (!std::equal(std::begin(str_MThd), std::end(str_MThd), it))
			return false;
		if (it + std::size(str_MThd) + 4 + 2 + 2 + 2 > e())
			throw Error("Cannot parse MThd header: Premature end of data");
		it += std::size(str_MThd);

		const auto header_size = detail::read_int<uint32_t>(it, e());
		if (header_size != 6)
			throw Error("Unexpected header size: ") << header_size;

		const auto file_format = detail::read_int<uint16_t>(it, e());
		if (file_format > 2)
			throw Error("Invalid value of file format: ") << file_format;

		const auto number_of_tracks = detail::read_int<uint16_t>(it, e());
		const auto delta_time_ticks_per_quarter_rate = detail::read_int<uint16_t>(it, e());

		that().on_chunk_MThd(header_size, FileFormat(file_format), number_of_tracks, delta_time_ticks_per_quarter_rate);

		return true;
	}

public:
	Parser(RangeT range): data_{std::move(range)} {}
	void operator()() const {
		It it = b();
		if (!parse_chunk_MThd(it))
			throw Error("Cannot find MThd chunk");
	}

};

} /* namespace meave::midi */
