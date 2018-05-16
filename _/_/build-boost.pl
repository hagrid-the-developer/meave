#!/usr/bin/env perl

use strict;
use warnings;

use Cwd;
use Const::Fast;
use File::Path qw(mkpath rmtree);
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);

const my $BOOST_URL => 'http://sourceforge.net/projects/boost/files/boost/1.67.0/boost_1_67_0.tar.bz2/download';
const my $BOOST_VERSION => ($BOOST_URL =~ m{boost/(\d+[.]\d+[.]\d+)/})[0];
const my $WD => getcwd;
const my $OPT => "$WD";
const my $BUILD => "$OPT/__";
const my $BOOST_BUILD => "$BUILD/boost-$BOOST_VERSION";
const my $BOOST_TAR_FILE => $BOOST_BUILD . '/' . ($BOOST_URL =~ m{/(boost_\d+_\d+_\d+[.][^/]*)/})[0];
const my $BOOST_UNTAR_FILE => ($BOOST_TAR_FILE =~ m{(^.*/boost_\d+_\d+_\d+)[.][^/]*$})[0];
const my $BOOST_PREFIX => "$OPT/boost-$BOOST_VERSION";
const my $MARK => "$OPT/_boost";

-e $BOOST_BUILD and rmtree($BOOST_BUILD, 1);
-e $BOOST_PREFIX and rmtree($BOOST_PREFIX, 1);
mkpath($BOOST_BUILD, 1) or die "Cannot create directory: \`$BOOST_BUILD'";

INFO "Downloading boost \`$BOOST_VERSION' from \`$BOOST_URL' to \`$BOOST_TAR_FILE'";
INFO "Please be patient";

system('wget', $BOOST_URL, '-O', $BOOST_TAR_FILE) == 0 or die "Cannot download boost";

const my $CMD =>
	"cd '$BOOST_BUILD' && ".
	"tar xvf '$BOOST_TAR_FILE' && ".
	"cd '$BOOST_UNTAR_FILE' && ".
	"./bootstrap.sh && ".
	"./b2 install --prefix='$BOOST_PREFIX'";
INFO "Running shell command: $CMD";
system('bash', '-c', $CMD) == 0 or die "Cannot build boost library :-(";
symlink $BOOST_PREFIX, $MARK;
