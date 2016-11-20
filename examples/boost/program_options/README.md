Example usage of boost::program_options
==========================================

This app doesn't do anything usefull, it only reads command-line options and config file, dump the configuration and exits.

| Parameter | Hidden | Command - Line | Config - File | Comment |
|-------------------------|-------------|----------------|---------------|---------------|
| --help        | N | Y | N | Print help.                                                |
| --full-help   | Y | Y | N | Print full help including hidden parameters.               |
| --config-file | N | Y | N | Config file to be used for reading of parameters.          |
| --port        | N | Y | Y | Tcp port. |
| --pid-file    | N | Y | Y | Path to pid-file. |
| --nice        | N | Y | Y | Nice value. |
| --mode        | N | Y | Y | Mode (SERVER, CLIENT, PROXY). |
| --no-daemon   | N | Y | Y | Don't daemonize (bool flag). |
| --num-processes | Y | Y | Y | Number of processes (1 - 20). |
| --working-directory | Y | Y | Y | Working directory (also as positional parameter on cmd - line). |

Main practices:

*	Use one object for storing of all arguments.
*	Default values are set in constructor.
*	Object has getter function for reading of values.
*	There is function `bool parse(const int argc, const char *argv[])`:
	* This function parses command-line and config file options. It prints help if needed. It returns true, if configuration was read successfully and false if error occured or help was printed.


Example of run:
---------------

	$ ./main --pid-file /var/lib/xyz --mode 'XCLIENT' --config-file ./conf/001.conf /opt/www
	Error: the argument ('XCLIENT') for option '--mode' is invalid

	Usage: ./main [options]

	Main options:
	  --help                display the help message and exit
	  --config-file arg     configuration file

	Config file options:
	  --port arg            TCP port to listen on
	  --pid-file arg        PID file
	  --nice arg (=0)       Nice
	  --mode arg (=SERVER)  Mode
	  --no-daemon           don't daemonize

	$ ./main --	pid-file /var/lib/xyz --mode 'CLIENT' --config-file ./conf/001.conf /opt/www
	pid-file:`/var/lib/xyz'
	working-directory:`/opt/www'
	num-processes:1
	nice:0
	port:2048
	mode:CLIENT
	no-daemon:1
	$ ./main --	pid-file /var/lib/xyz --config-file ./conf/001.conf /opt/www
	pid-file:`/var/lib/xyz'
	working-directory:`/opt/www'
	num-processes:1
	nice:0
	port:2048
	mode:SERVER
	no-daemon:1