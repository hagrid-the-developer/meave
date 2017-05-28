#!/usr/bin/env perl

use strict;
use warnings;

use Cwd;
use Const::Fast;
use File::Path qw(mkpath rmtree);
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);

const my $CPUNUM => int(`cat /proc/cpuinfo | grep '^processor' | wc -l`);
const my $HWLOC_URL => 'https://github.com/open-mpi/hwloc.git';
const my $HWLOC_VERSION => '1.11.3';
const my $WD => getcwd;
const my $OPT => "$WD";
const my $BUILD => "$OPT/__";
const my $HWLOC_BUILD => "$BUILD/hwloc-$HWLOC_VERSION";
const my $HWLOC_PREFIX => "$OPT/hwloc-$HWLOC_VERSION";
const my $MARK => "$OPT/_hwloc";

-e $HWLOC_BUILD and rmtree($HWLOC_BUILD, 1);
-e $HWLOC_PREFIX and rmtree($HWLOC_PREFIX, 1);
mkpath($HWLOC_BUILD, 1) or die "Cannot create directory: \`$HWLOC_BUILD'";

INFO "Cloning hwloc \`$HWLOC_VERSION' from \`$HWLOC_URL' to \`$HWLOC_BUILD'";
INFO "Please be patient";

system('git', 'clone', $HWLOC_URL, $HWLOC_BUILD) == 0 or die "Cannot clone hwloc";

const my $CMD =>
	"cd '$HWLOC_BUILD' && ".
	"git checkout 'tags/hwloc-$HWLOC_VERSION' && ".
	"./autogen.sh && ".
	"./configure --prefix='$HWLOC_PREFIX' && ".
	"make -j$CPUNUM && ".
	"make install";
INFO "Running shell command: $CMD";
system('bash', '-c', $CMD) == 0 or die "Cannot build hwloc library :-(";
symlink $HWLOC_PREFIX, $MARK;
