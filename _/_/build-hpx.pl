#!/usr/bin/env perl

use strict;
use warnings;

use Cwd;
use Const::Fast;
use File::Path qw(mkpath rmtree);
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);

const my $CPUNUM => int(`cat /proc/cpuinfo | grep '^processor' | wc -l`);
const my $HPX_URL => 'https://github.com/STEllAR-GROUP/hpx.git';
const my $HPX_VERSION => '0.9.99';
const my $WD => getcwd;
const my $OPT => "$WD";
const my $BUILD => "$OPT/__";
const my $HPX_BUILD => "$BUILD/hpx-$HPX_VERSION";
const my $HPX_PREFIX => "$OPT/hpx-$HPX_VERSION";
const my $MARK => "$OPT/_hpx";

-e $HPX_BUILD and rmtree($HPX_BUILD, 1);
-e $HPX_PREFIX and rmtree($HPX_PREFIX, 1);
mkpath($HPX_BUILD, 1) or die "Cannot create directory: \`$HPX_BUILD'";

INFO "Cloning hpx \`$HPX_VERSION' from \`$HPX_URL' to \`$HPX_BUILD'";
INFO "Please be patient";

system('git', 'clone', $HPX_URL, $HPX_BUILD) == 0 or die "Cannot clone hpx";

const my $CMD =>
	"cd '$HPX_BUILD' && ".
	"git checkout 'tags/$HPX_VERSION' && ".
	"mkdir build && cd build && ".
	"cmake -DBOOST_ROOT=$OPT/_boost ".
	      "-DHWLOC_ROOT=$OPT/_hwloc ".
	      "-DHPX_WITH_MALLOC=system ".
	      "-DCMAKE_INSTALL_PREFIX=$HPX_PREFIX ..".
	      "&& ".
	"make -j$CPUNUM && ".
	"make install";
INFO "Running shell command: $CMD";
system('bash', '-c', $CMD) == 0 or die "Cannot build hpx library :-(";
symlink $HPX_PREFIX, $MARK;
