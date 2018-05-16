#include <iostream>
#include <string>
#include <glog/logging.h>
#include <fstream>
#include <string>
#include <sstream>

#include "meave/commons.hpp"
#include "fileview.hpp"

namespace {

$::string read_file(char const* fn) {
	$::ifstream t(fn);
	$::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

} /* anonymous namespace */

int
main(int argc, char const* const* argv) {
	for (int i = 1; i < argc; ++i) {
		auto fn = argv[i];
		try {
			const auto s = read_file(fn);
			meave::midi::FileView view{ reinterpret_cast<uint8_t const*>(s.c_str()), s.size() };
			LOG(INFO) << "File: " << fn << "; " << view;
		} catch (const meave::Error& e) {
			LOG(ERROR) << "Cannot parse: " << fn << ": " << e.what();
			return 1;
		}
	}
}
