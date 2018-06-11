#pragma once

namespace meave::midi {

enum FileFormat {
      FILE_FORMAT_SINGLE_TRACK = 0
    , FILE_FORMAT_MULTIPLE_TRACKS_SYNC = 1
    , FILE_FORMAT_MULTIPLE_TRACKS_ASYNC = 2
};
char const* to_cstr(const FileFormat $) {
    switch($)
    {
    case FILE_FORMAT_SINGLE_TRACK:
	return "SINGLE_TRACK";
    case FILE_FORMAT_MULTIPLE_TRACKS_SYNC:
	return "MULTIPLE_TRACKS_SYNC";
    case FILE_FORMAT_MULTIPLE_TRACKS_ASYNC:
	return "MULTIPLE_TRACKS_ASYNC";
    }
    __builtin_unreachable();
}

$::ostream& operator<<($::ostream& o, const FileFormat $)
{
	return o << to_cstr($);
}

enum ChunkType {
	  CHUNK_TYPE_MThd
	, CHUNK_TYPE_MTrk
	, CHUNK_TYPE_UNKNOWN
};

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

} /* meave::midi */
