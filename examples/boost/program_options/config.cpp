#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <boost/optional.hpp>
#include <fstream>

#include "config.hpp"

namespace {

const unsigned MAX_NUMBER_OF_PROCESSES = 20;

} /* Anonymouse Namespace */

namespace meave { namespace examples {

Config::Config() throw()
:	pid_file_()
,	working_directory_("/var/run/me/po")
,	num_processes_(1)
,	nice_(0)
,	port_(0)
,	no_daemon_(false) {
}

bool Config::parse(const int argc, const char * const argv[]) throw() {
	// Let's make the code less verbose.
	namespace po = boost::program_options;
	using po::value;
	using po::bool_switch;

	const $::string S_USAGE = $::string("Usage: ") + argv[0] + " [options]";

	$::string config_file;

	po::options_description visible_main_options("Main options");
	visible_main_options.add_options()
	("help"       , "display the help message and exit")
	("config-file", value(&config_file)->default_value(config_file), "configuration file")
	;

	po::options_description visible_conf_file_options("Config file options");
	visible_conf_file_options.add_options()
	("port"         , value(&port_)->required(), "TCP port to listen on")
	("pid-file"     , value(&pid_file_)->required(), "PID file")
	("no-daemon"    , bool_switch(&no_daemon_)->default_value(no_daemon_), "don't daemonize")
	;

	po::options_description hidden_conf_file_options("Hidden config file options");
	hidden_conf_file_options.add_options()
	("num-processes", value(&num_processes_)->default_value(num_processes_)
				        , "number of worker processes (1-20)")
	;

	po::options_description hidden_main_options("Hidden main options");
	hidden_main_options.add_options()
	("full-help", "display the full help message and exit")
	("working-directory", value(&working_directory_)->default_value(working_directory_) // FIXME: Really default value here?
				        , "working directory with data")
	;

	po::positional_options_description parameters;
	parameters.add("working-directory", 1);

	po::options_description conf_file_options;
	conf_file_options.add(visible_conf_file_options).add(hidden_conf_file_options);

	po::options_description hidden_options;
	hidden_options.add(hidden_main_options).add(hidden_conf_file_options);

	po::options_description visible_options;
	visible_options.add(visible_main_options).add(visible_conf_file_options);

	po::options_description full_options;
	full_options.add(visible_options).add(hidden_options);

	try {
		po::variables_map option_map;
		po::parsed_options parsed_options =
			po::command_line_parser(argc, argv).options(full_options).positional(parameters).run();
		po::store(parsed_options, option_map);

		if (option_map.count("help")) {
			$::cerr << S_USAGE << "\n" << visible_options << $::endl;
			return false;
		}

		if (option_map.count("full-help")) {
			$::cerr << S_USAGE << "\n" << full_options << $::endl;
			return false;
		}

		if (option_map.count("config-file")) {
			config_file = option_map["config-file"].as<$::string>();

			if (!config_file.empty()) {
				$::ifstream config_stream(config_file.c_str());
				if (!config_stream) {
					$::cerr << "Error: Cannot open config file: `" << config_file << "' for reading." << $::endl;
					return false;
				}

				po::parsed_options opt_config_parsed_options = po::parse_config_file(config_stream, conf_file_options, false);
				po::store(opt_config_parsed_options, option_map);
			}
		}

		po::notify(option_map);
	} catch (const po::error &error) {
		$::cerr << "Error: " << error.what() << "\n" << "\n" << S_USAGE << "\n" << visible_options << $::endl;
		return false;
	}

	if (num_processes_ == 0 || num_processes_ > MAX_NUMBER_OF_PROCESSES) {
		$::cerr << "Error: Failed to configure num-processes to " << num_processes_ << $::endl;
		$::cerr << "       num-threads must be in interval <1," << MAX_NUMBER_OF_PROCESSES << ">" << $::endl;
		return false;
	}

	return true;
}

$::string Config::pid_file() const noexcept {
	return pid_file_;
}

int Config::nice() const noexcept {
	return nice_;
}

::uint16_t Config::port() const noexcept {
	return port_;
}

bool Config::no_daemon() const noexcept {
	return no_daemon_;
}

$::string Config::working_directory() const noexcept {
	return working_directory_;
}

unsigned Config::num_processes() const noexcept {
	return num_processes_;
}

$::ostream& operator<<($::ostream &o, const Config &$) {
	return o << "pid-file:`" << $.pid_file() << "'" << $::endl
		 << "working-directory:`" << $.working_directory() << "'" << $::endl
		 << "num-processes:" << $.num_processes() << $::endl
		 << "nice:" << $.nice() << $::endl
		 << "port:" << $.port() << $::endl
		 << "no-daemon:" << $.no_daemon();
}

} } /* namespace meave::examples */
