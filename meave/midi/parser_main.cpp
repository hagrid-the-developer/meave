#include <cstdio>
#include <glog/logging.h>
#include <iomanip>
#include <iostream>
#include <string>
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
	template <typename It>
	static void print(It it, It e) {
		for (; it < e; ++it) {
			const uns u = uint8_t(*it);
			if (u >= 32 && u <= 126)
				putc(char(u), stdout);
			else
				printf("\\x%.2x", uns(uint8_t(u)));
		}
	}

public:
	PrintingParser(RangeT range): meave::midi::Parser<PrintingParser<RangeT>, RangeT>(std::move(range)) {}

	void on_chunk_MThd(const uns header_size, const meave::midi::FileFormat file_format, const uns number_of_tracks, const uns delta_time_ticks_per_quarter_rate) const {
		printf("MThd chunk:"
		       "\n\theader-size: %u"
		       "\n\tfile-format: %s"
		       "\n\tnumber-of-tracks: %u"
		       "\n\tdelta-time-ticks-per-quarter-note: %u"
		       "\n", header_size, to_cstr(file_format), number_of_tracks, delta_time_ticks_per_quarter_rate);
	}

	void on_chunk_MTrk(const uns length) const {
		std::cout << "MTrk chunk:" <<
			     "\n\tlength: " << length <<
			     "\n";
	}
	void on_chunk_unknown(const uns length) const {
		printf("Unknown chunk of length: %u\n", length);
	}

	template <typename It>
	void on_meta_event(const uns delta_time, const uns type, It it, It end) const {
		printf("Meta Event:"
		       "\n\t\tdelta_time: %u"
		       "\n\t\ttype: 0x%x"
		       "\n\t\tlength: %zu"
		       "\n\t\tdata: ", delta_time, type, std::distance(it, end));
		print(it, end);
		putc('\n', stdout);
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
