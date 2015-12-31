#ifndef MEAVE_DAEMONIZE_HPP_INCLUDED
#	define MEAVE_DAEMONIZE_HPP_INCLUDED

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

namespace meave {

void daemonize(const $::string &out_file, const $::string &chdir_path throw(Error) {
	// Fork the process and have the parent exit. If the process was started
	// from a shell, this returns control to the user. Forking a new process is
	// also a prerequisite for the subsequent call to setsid().
	if (const ::pid_t pid = fork())	{
		if (pid > 0) {
			// We're in the parent process and need to exit.
			//
			// When the exit() function is used, the program terminates without
			// invoking local variables' destructors. Only global variables are
			// destroyed.
			::exit(0);
		} else {
			throw Error("First fork failed: %m");
		}
	}

	// Make the process a new session leader. This detaches it from the
	// terminal.
	if (::setsid() == -1) {
		throw Error("Call to setsid failed: %m");
	}

	// A process inherits its working directory from its parent. This could be
	// on a mounted filesystem, which means that the running daemon would
	// prevent this filesystem from being unmounted. Changing to the root
	// directory avoids this problem, but the root directory is not writable,
	// so we can't write a core file there. Making this configurable
	// fixes both problems.
	if (::chdir(chdir_path.c_str()) == -1) {
		throw Error("Cannot chdir to %s: %m", chdir_path.c_str());
	}

	// The file mode creation mask is also inherited from the parent process.
	// We don't want to restrict the permissions on files created by the
	// daemon, so the mask is cleared.
	::umask(0);

	// A second fork ensures the process cannot acquire a controlling terminal.
	if (const ::pid_t pid = fork()) {
		if (pid > 0) {
			::exit(0);
		} else {
			throw Error("Second fork failed: %m");
		}
	}

	// Close the standard streams. This decouples the daemon from the terminal
	//   that started it.
	::close(0);
	::close(1);
	::close(2);

	// We don't want the daemon to have any standard input.
	const int input_descriptor = ::open("/dev/null", O_RDONLY);
	switch (input_descriptor) {
		case -1:
			throw Error("Unable to open /dev/null: %m");
		case 0:
			break;
		default:
			::close(input_descriptor);
			throw Error("Cannot redirect standard input to /dev/null");
	}

	// Send standard output to a log file.
	const char* output = out_file.c_str();
	const int flags = O_WRONLY | O_CREAT | O_APPEND;
	const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	const int output_descriptor = ::open(output, flags, mode);
	switch (output_descriptor) {
		case -1:
			throw Error("Unable to open output file: %s: %m", output);
		case 1:
			break;
		default:
			::close(output_descriptor);
			throw Error("Cannot redirect standard output to: %s", out_file);
	}

	// Also send standard error to the same log file.
	const int error_descriptor = ::dup(1);
	switch (error_descriptor) {
		case -1:
			Error("Unable to dup output descriptor to error descriptor: %m");
		case 2:
			break;
		default:
			::close(error_descriptor);
			throw Error("Cannot dup output descriptor to standard error descriptor");
	}
}

} /* namespace meave */

#endif /* MEAVE_DAEMONIZE_HPP_INCLUDED */
