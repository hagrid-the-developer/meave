#pragma once

namespace meave::midi::detail {

// TODO: signed values
template <typename T>
T read_int(uint8_t const* p, const size_t len, size_t& i) {
    constexpr auto RESULT_SIZE = sizeof(T);

    if (i + RESULT_SIZE > len)
	throw Error("Cannot read integer value: Premature end of data");

    T res{};
    for (const auto end = i + RESULT_SIZE; i < end; ++i) {
	res = res << 8 | p[i];
    }
    return res;
}


// TODO: signed values
template <typename T, typename ItT>
T read_int(ItT& it, ItT end) {
    constexpr auto RESULT_SIZE = sizeof(T);

    if (it + RESULT_SIZE > end)
	throw Error("Cannot read integer value: Premature end of data");

    T res{};
    for (const auto e = it + RESULT_SIZE; it != e; ++it) {
	res = res << 8 | *it;
    }
    return res;
}

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
		const uns v = x & 0x7F;
		is_last = !(x & 0x80);
		LOG(INFO) << "Byte: " << std::hex << x << "; v: " << v << "; is_last: " << is_last << std::dec << std::endl;
		val = val << 7 | v;
	}
	return val;
}

template<typename It>
uns read_varint(It& it, const It end) {
	uns val{};
	bool is_last = false;
	uns representation_length = 0;
	for (; !is_last; ++it, ++representation_length) {
		if (it >= end)
			throw Error("Cannot parse variable length value: Premature end of data");
		if (representation_length >= 4)
			throw Error("Cannot parse variable length value: Out of bounds");
		uns x = *it;
		const uns v = x & 0x7F;
		is_last = !(x & 0x80);
		val = val << 7 | v;
	}
	return val;
}

} /* namespace meave::midi::detail */
