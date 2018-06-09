#include <iostream>
#include <string>
#include <glog/logging.h>
#include <fstream>
#include <string>
#include <sstream>

#include "meave/commons.hpp"
#include "parser.hpp"

namespace {

$::string read_file(char const* fn) {
	$::ifstream t(fn);
	$::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

template<typename RangeT>
class PrintingParser: public meave::midi::Parser<PrintingParser<RangeT>, RangeT> {
public:
	PrintingParser(RangeT range): meave::midi::Parser<PrintingParser<RangeT>, RangeT>(std::move(range)) {}

	void on_chunk_MThd(const uns header_size, const meave::midi::FileFormat file_format, const uns number_of_tracks, const uns delta_time_ticks_per_quarter_rate) const {
		std::cout << "MThd chunk:" <<
			     "\n\theader-size: " << header_size <<
			     "\n\tfile-format: " << file_format <<
			     "\n\tnumber-of-tracks: " << number_of_tracks <<
			     "\n\tdelta-time-ticks-per-quarter-note: " << delta_time_ticks_per_quarter_rate <<
			     "\n";
	}
};

} /* anonymous namespace */

int
main(int argc, char const* const* argv) {
	for (int i = 1; i < argc; ++i) {
		auto fn = argv[i];
		try {
			const auto s = read_file(fn);
			PrintingParser parser{ std::make_pair(std::begin(s), std::end(s)) };
			parser();
		} catch (const meave::Error& e) {
			LOG(ERROR) << "Cannot parse: " << fn << ": " << e.what();
			return 1;
		}
	}
}
