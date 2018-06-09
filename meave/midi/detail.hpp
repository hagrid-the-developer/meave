#pragma once

namespace meave::midi::detail {

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

// TODO: signed values
template <typename T>
T read_int(uint8_t const* p, const size_t len, size_t& i) {
    constexpr auto RESULT_SIZE = sizeof(T);

    if (i + RESULT_SIZE > len)
        throw Error("Cannot read integer value: Premature end of buffer");

    T res{};
    for (const auto end = i + RESULT_SIZE; i < end; ++i) {
        res = res << 8 | p[i];
    }
    return res;
}

} /* namespace meave::midi::detail */
