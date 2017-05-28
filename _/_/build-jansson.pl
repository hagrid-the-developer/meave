#!/usr/bin/env perl

use strict;
use warnings;

use Cwd;
use Const::Fast;
use File::Path qw(mkpath rmtree);
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);

const my $JANSSON_URL => 'http://www.digip.org/jansson/releases/jansson-2.6.tar.bz2';
const my $JANSSON_VERSION => ($JANSSON_URL =~ m{/jansson-(\d+(:?[.]\d+)*)[.]\D})[0];
const my $WD => getcwd;
const my $OPT => "$WD";
const my $BUILD => "$OPT/__";
const my $JANSSON_BUILD => "$BUILD/jansson-$JANSSON_VERSION";
INFO "url: $JANSSON_URL; version: $JANSSON_VERSION; xyz: " . ($JANSSON_URL =~ m{/(jansson-\d+(:?[.]\d+)*[.]\D\S*)$})[0] . ";";
const my $JANSSON_TAR_FILE => $JANSSON_BUILD . '/' . ($JANSSON_URL =~ m{/(jansson-\d+(:?[.]\d+)*[.]\D\S*)$})[0];
const my $JANSSON_UNTAR_FILE => ($JANSSON_TAR_FILE =~ m{^.*/(jansson-\d+(:?[.]\d+)*)[.]\D\S*$})[0];
const my $JANSSON_PREFIX => "$OPT/jansson-$JANSSON_VERSION";
const my $MARK => "$OPT/_jansson";

-e $JANSSON_BUILD and rmtree($JANSSON_BUILD, 1);
-e $JANSSON_PREFIX and rmtree($JANSSON_PREFIX, 1);
mkpath($JANSSON_BUILD, 1) or die "Cannot create directory: \`$JANSSON_BUILD'";

INFO "Downloading jansson \`$JANSSON_VERSION' from \`$JANSSON_URL' to \`$JANSSON_TAR_FILE'";
INFO "Please be patient";

system('wget', $JANSSON_URL, '-O', $JANSSON_TAR_FILE) == 0 or die "Cannot download jansson";

const my $CMD =>
	"cd '$JANSSON_BUILD' && ".
	"tar xvf '$JANSSON_TAR_FILE' && ".
	"cd '$JANSSON_UNTAR_FILE' && ".
	"./configure --prefix='$JANSSON_PREFIX' &&".
	"make &&".
	"make install";
INFO "Running shell command: $CMD";
system('bash', '-c', $CMD) == 0 or die "Cannot build jansson library :-(";
symlink $JANSSON_PREFIX, $MARK;
