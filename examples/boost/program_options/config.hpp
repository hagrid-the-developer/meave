#ifndef MEAVE_EXAMPLES_PROGRAM_OPTIONS_CONFIG_H
#	define MEAVE_EXAMPLES_PROGRAM_OPTIONS_CONFIG_H

#	include <boost/program_options.hpp>
#	include <meave/commons.hpp>
#	include <ostream>

namespace meave { namespace examples {

class Config
{
public:
  Config() noexcept;

  bool parse(const int argc, const char * const argv[]) noexcept;

  $::string pid_file() const noexcept;
  int nice() const noexcept;
  ::uint16_t port() const noexcept;
  bool no_daemon() const noexcept;
  $::string working_directory() const noexcept;
  unsigned num_processes() const noexcept;

private:
  $::string pid_file_;
  $::string working_directory_;
  unsigned num_processes_;
  int nice_;
  ::uint16_t port_;
  bool no_daemon_;
};

$::ostream& operator<<($::ostream &o, const Config&$);

} } /* namespace meave::examples */

#endif // MEAVE_EXAMPLES_PROGRAM_OPTIONS_CONFIG_H
