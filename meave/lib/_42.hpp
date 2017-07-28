#ifndef MEAVE__42_HPP
#	define MEAVE__42_HPP

#	include <glog/logging.h>

namespace meave {

template <typename F>
class _42 {
private:
	int argc_;
	char **argv_;

public:
	_42(int argc, char **argv)
	:	argc_(argc)
	,	argv_(argv)
	{
		// Initialize Google's logging library.
		google::InitGoogleLogging(argv_[0]);
	}

	int operator()() {
		return static_cast<F*>(this)->main();
	}
};

} /* namespace meave */

#endif
