#!/usr/bin/env perl

use strict;
use warnings;

use Cwd;
use Const::Fast;
use File::Path qw(mkpath rmtree);
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);

const my $GLOG_URL => 'https://github.com/google/glog/archive/v0.3.5.tar.gz';
const my $GLOG_VERSION => ($GLOG_URL =~ m{/v(\d+[.]\d+[.]\d+)[.]\D})[0];
const my $WD => getcwd;
const my $OPT => "$WD";
const my $BUILD => "$OPT/__";
const my $GLOG_BUILD => "$BUILD/glog-$GLOG_VERSION";
const my $GLOG_TAR_FILE => $GLOG_BUILD . '/' . ($GLOG_URL =~ m{/(v\d+[.]\d+[.]\d+[.]\D+)$})[0];
const my $GLOG_UNTAR_FILE => "glog-$GLOG_VERSION";
const my $GLOG_PREFIX => "$OPT/glog-$GLOG_VERSION";
const my $MARK => "$OPT/_glog";

-e $GLOG_BUILD and rmtree($GLOG_BUILD, 1);
-e $GLOG_PREFIX and rmtree($GLOG_PREFIX, 1);
mkpath($GLOG_BUILD, 1) or die "Cannot create directory: \`$GLOG_BUILD'";

INFO "Downloading glog \`$GLOG_VERSION' from \`$GLOG_URL' to \`$GLOG_TAR_FILE'";
INFO "Please be patient";

system('wget', $GLOG_URL, '-O', $GLOG_TAR_FILE) == 0 or die "Cannot download glog";

const my $CMD =>
	"cd '$GLOG_BUILD' && ".
	"tar xvf '$GLOG_TAR_FILE' && ".
	"cd '$GLOG_UNTAR_FILE' && ".
	"./configure --prefix='$GLOG_PREFIX' &&".
	"make &&".
	"make install";
INFO "Running shell command: $CMD";
system('bash', '-c', $CMD) == 0 or die "Cannot build glog library :-(";
symlink $GLOG_PREFIX, $MARK;
