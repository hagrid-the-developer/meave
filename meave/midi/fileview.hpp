#pragma once

#include <boost/endian/conversion.hpp>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <type_traits>

#include "meave/commons.hpp"
#include "meave/lib/error.hpp"

namespace meave::midi {

namespace detail {

namespace be = boost::endian;

enum FileFormat {
	  FILE_FORMAT_SINGLE_TRACK = 0
	, FILE_FORMAT_MULTIPLE_TRACKS_SYNC = 1
	, FILE_FORMAT_MULTIPLE_TRACKS_ASYNC = 2
};
$::ostream& operator<<($::ostream& o, const FileFormat $)
{
	switch($)
	{
	case FILE_FORMAT_SINGLE_TRACK:
		return o << "SINGLE_TRACK";
	case FILE_FORMAT_MULTIPLE_TRACKS_SYNC:
		return o << "MULTIPLE_TRACKS_SYNC";
	case FILE_FORMAT_MULTIPLE_TRACKS_ASYNC:
		return o << "MULTIPLE_TRACKS_ASYNC";
	}
	__builtin_unreachable();
}

enum Status {
	  STATUS_NOTE_OFF             = 0x80
	, STATUS_NOTE_ON              = 0x90
	, STATUS_POLY_KEYPRESS        = 0xA0
	, STATUS_CONTROLLER_CHANGE    = 0xB0
	, STATUS_PROGRAM_CHANGE       = 0xC0
	, STATUS_CHANNEL_PRESSURE     = 0xD0
	, STATUS_PITCH_BEND           = 0xE0
	, STATUS_SYS_EXMESSAGE        = 0xF0
};

class Header {
	uint8_t str_MThd_[4];    // MThd
	uint32_t header_size_;   // 0x00 0x00 0x00 0x06
	uint16_t file_format_;
	uint16_t number_of_tracks_;
	uint16_t delte_time_ticks_per_quarter_note_;

public:
	static constexpr const char* name() {
		return "Header";
	}

	uint32_t header_size() const noexcept {
		return be::big_to_native(header_size_);
	}

	FileFormat file_format() const noexcept {
		return FileFormat(file_format_);
	}

	uint16_t number_of_tracks() const noexcept {
		return be::big_to_native(number_of_tracks_);
	}

	uint16_t delte_time_ticks_per_quarter_note() const noexcept {
		return be::big_to_native(delte_time_ticks_per_quarter_note_);
	}

	void check() const {
		if (memcmp(str_MThd_, "MThd", 4))
			throw Error("Header doesn't contain MThd");
		if (header_size() != 6)
			throw Error("Wrong header size: %u", uns(header_size()));
		if (0 <= file_format_ && file_format_ <= 2)
			throw Error("Improper value for FileFormat");
	}
}__attribute__((packed));
static_assert(std::is_pod<Header>::value);

std::ostream& operator<<(std::ostream& o, const Header& hdr)
{
	return o << "number-of-tracks: " << hdr.number_of_tracks() <<
		    ", file-format: " << hdr.file_format() <<
		    ", delte-time-ticks-per-quarter-note: " << hdr.delte_time_ticks_per_quarter_note();
}

class ChunkHeader {
	uint8_t str_MTrk_[4];
	uint32_t length_;
public:
	static constexpr const char* name() {
		return "ChunkHeader";
	}

	uint32_t length() const noexcept {
		return be::big_to_native(length_);
	}

	void check() const {
		if (memcmp(str_MTrk_, "MTrk", 4))
			throw Error("ChunkHeader doesn't contain MTrk");
	}
}__attribute__((packed));
static_assert(std::is_pod<ChunkHeader>::value);

std::ostream& operator<<(std::ostream& o, const ChunkHeader& chh)
{
	return	o << "length: " << chh.length();
}

class Event {
	uint8_t status_channel_;
	uint8_t data_bytes_[2];
public:
	static constexpr const char* name() {
		return "Event";
	}

	uns channel() const noexcept {
		return status_channel_ >> 8;
	}

	uns status() const noexcept {
		return status_channel_ & 0xF;
	}

	uns data_byte_0() const noexcept {
		return data_bytes_[0];
	}

	uns data_byte_1() const noexcept {
		return data_bytes_[1];
	}
}__attribute__((packed));
static_assert(std::is_pod<Event>::value);

std::ostream& operator<<(std::ostream& o, const Event& e)
{
	return	o << "channel: " << e.channel() <<
		     std::hex << ", status: " << e.status() <<
		     ", data: " << e.data_byte_0() << ", " << e.data_byte_1() << std::dec;
}

class SysEx {
	uint8_t f7_;
	uint8_t manufacturer_;
	uint8_t bytes_[1]; // At least one byte -- the end
public:
	static SysEx const *get(uint8_t const* bytes, const size_t len, size_t& index) {
		if (index + sizeof(SysEx) >= len)
			return nullptr;

		SysEx const* p = reinterpret_cast<SysEx const*>(&bytes[index]);
		if (p->f7_ != 0xF7)
			return nullptr;

		// FIXME: Not all SysEx messages ends with 0xF7
		auto last = static_cast<uint8_t const*>( ::memchr(p->bytes_, 0xF7, len - 2) );
		if (!last)
			throw Error("Cannot parse SysEx: Premature end of buffer");
		index = last - bytes;

		return p;
	}

	uint8_t manufacturer() const noexcept {
		return manufacturer_;
	}
};

uns read_varint(uint8_t const* p, const size_t len, size_t& i) {
	uns val = 0;
	bool is_last = false;
	uns representation_length = 0;
	for (; !is_last; ++i, ++representation_length) {
		if (i >= len)
			throw Error("Cannot parse variable length value: Premature end of buffer");
		if (representation_length >= 4)
			throw Error("Cannot parse variable length value: Out of bounds");
		uns x = p[i];
		const uns v = x & ~(1U << 7);
		is_last = !(x & (1U << 7));
		LOG(INFO) << "Byte: " << std::hex << x << "; v: " << v << "; is_last: " << is_last << std::dec << std::endl;
		val = val << 7 | v;
	}
	return val;
}

class Metaevent {
	uint8_t ff_;
	uint8_t type_;
	uint8_t length_data_[1]; // At least one byte
public:
	static Metaevent const *get(uint8_t const* bytes, const size_t len, size_t& index) {
		if (index + sizeof(Metaevent) >= len)
			return nullptr;

		Metaevent const* p = reinterpret_cast<Metaevent const*>(&bytes[index]);
		if (p->ff_ != 0xFF)
			return nullptr;

		index += 2;
		const auto data_len = read_varint(bytes, len, index);
		index += data_len;

		return p;
	}

	uns type() const noexcept {
		return type_;
	}

	uns length() const noexcept {
		size_t i = 0;
		return read_varint(length_data_, ~size_t(0), i);
	}

	uint8_t const* data() const noexcept {
		size_t i = 0;
		read_varint(length_data_, ~size_t(0), i);
		return &length_data_[i];
	}
}__attribute__((packed));

std::ostream& operator<<(std::ostream& o, const Metaevent& me)
{
	o << "type: " << me.type() <<
	     ", length: " << me.length() <<
	     ", data: ";
	for (auto p = me.data(), e = me.data() + me.length(); p < e; ++p) {
		const uns u = *p;
		if (u >= 32 && u <= 126)
			o << char(u);
		else
			o << "\\x" << std::hex << std::setw(2) << u << std::dec;
	}
	return o;
}

uns read_delta_time(uint8_t const* bytes, const size_t len, size_t& i) {
	return read_varint(bytes, len, i);
}

} /* namespace detail */

class FileView {
public:
	FileView(uint8_t const* data, const size_t len)
	:	data_{data}
	,	len_{len} {
	}

	$::ostream& print($::ostream& o) const
	{
		size_t idx{};

		auto hdr = read<detail::Header>(idx);
		hdr.check();
		o << "\n\t" << hdr.name() << ": " << hdr;

		for (uns track_id = 0; track_id < hdr.number_of_tracks(); ++track_id) {
			const auto chunk_header = read<detail::ChunkHeader>(idx);
			chunk_header.check();
			o << "\n\t" << chunk_header.name() << ": " << chunk_header;

			for (uns end = idx + chunk_header.length(); idx < end;) {
				const auto dt = detail::read_delta_time(data_, len_, idx);
				o << "\n\t\t" << "Deltatime: " << dt;

				if (auto metaevent = detail::Metaevent::get(data_, len_, idx)) {
					o << "\n\t\tMetaEvent: " << *metaevent;
				} else {
					auto e = read<detail::Event>(idx);
					o << "\n\t\tEvent: " << e;
				}
			}
		}
	
		return o;	
	}

	friend inline $::ostream& operator<<($::ostream& o, const FileView& $) {
		return $.print(o);
	}

private:
	template <typename T>
	T const& read(size_t& index) const {
		if (index + sizeof(T) > len_)
			throw Error("Cannot read %s: premature end of midi data at byte: %zu", T::name(), index);
		T const* p = reinterpret_cast<T const*>(&data_[index]);
		index += sizeof(T);
		return *p;
	}

	uint8_t const* data_;
	size_t len_;
};

} /* namespace meave::midi */