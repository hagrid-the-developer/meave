#!/usr/bin/env perl

use utf8;

BEGIN { binmode STDIN, ":utf8"; }

s/(.)/ord($1) < 128 ? $1 : sprintf("\\U%08x", ord($1))/ge, print while(<STDIN>);


#while (<>) {
#	s/á/\$long_a\$/g;
#
#	print;
#}
