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

} /* namespace meave::midi::detail */
