#!/usr/bin/perl

use strict;
use warnings;

use IO::Compress::Gzip qw(gzip $GzipError);
use IO::Uncompress::Gunzip qw(gunzip $GunzipError);


# First divide large file to N smaller files and then run these scripts
#   in parallel over the smaller files...

sub get_file_name {
	my($orig_file_name, $key) = @_;
	(my$file_name = $orig_file_name) =~ s{^([^.]+)[.](.+)$}{$1_$key.$2};

	return $file_name;
}

sub get_key {
	if ((my$key) = $_[0] =~ m{^(\S+)}) {
		return $key;
	}
	die "Cannot determine key for: \`$_[0]'";
}

sub get_line_reader {
	my$input_file_name = shift;

	my($act_file, $act_file_key, $act_file_name);
	
	open my$in, '<', $input_file_name or die "Cannot open \`$input_file_name' for reading";
	if ($input_file_name =~ /[.]gz$/) {
		$in = new IO::Uncompress::Gunzip $in or die "Cannot uncompress \`$input_file_name': $GunzipError";
	}

	return sub {

		while(defined(my$line = <$in>)) {
			my$key = get_key $line;
			if (!defined($act_file_key) || $key ne $act_file_key) {
				$act_file && close $act_file;
				$act_file_name = get_file_name($input_file_name, ($act_file_key = $key));
				$act_file = new IO::Compress::Gzip $act_file_name or die "Cannot open file: \`$act_file_name' for writing: $GzipError";
			}
			print $act_file $line;
		}
	}
}

my$divider = get_line_reader('results_011.csv.bak.gz');
$divider->();
