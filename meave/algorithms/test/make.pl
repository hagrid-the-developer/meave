#!/usr/bin/env perl

my @cfiles = <*.c>;
my @cppfiles = <*.cpp>;
my @ocbinaries = map { local $_ = $_; s{[.]c}{.o}; $_ } (@cfiles);
my @ocppbinaries = map { local $_ = $_; s{[.]cpp}{.o}; $_ } (@cppfiles);
my @cbinaries = map { local $_ = $_; s{[.]c}{}; $_ } (@cfiles);
my @cppbinaries = map { local $_ = $_; s{[.]cpp}{}; $_ } (@cppfiles);

my @c = map { [$cfiles[$_], $ocbinaries[$_], $cbinaries[$_], ] } (0..$#cfiles);
my @cpp = map { [$cppfiles[$_], $ocppbinaries[$_], $cppbinaries[$_], ] } (0..$#cppfiles);

my $CFLAGS = '-D_GNU_SOURCE -I../../.. -g -Ofast';
print "all: binaries\n";
print "\n";
print "clean:\n";
print "\trm -f ".do{ join ' ', map {"'$_'"} (@cbinaries, @cppbinaries, @ocbinaries, @ocppbinaries) }."\n";
print "\n";
print "binaries: @cbinaries @cppbinaries\n\n";

for my $x(@c) {
    my ($f, $o, $b) = @$x;
    my $head_line = `gcc $CFLAGS -M '$f'`;
    die "Cannot run gcc -M '$f'" if $?;
    print "$b: $o\n";
    print "\tgcc -std=gnu99 $CFLAGS -o '$b' '$o'\n";
    chomp $head_line;
    print "$head_line\n";
    print "\tgcc -std=gnu99 $CFLAGS -o '$o' -c '$f'\n";
    print "\n";
}

for my $x(@cpp) {
    my ($f, $o, $b) = @$x;
    print "#\n# Build '$b'\n#\n";
    my $head_line = `gcc $CFLAGS -M '$f'`;
    die "Cannot run gcc -M '$f'" if $?;
    print "$b: $o\n";
    print "\tg++ -std=gnu++0x $CFLAGS -o '$b' '$o'\n";
    chomp $head_line;
    print "$head_line\n";
    print "\tg++ -std=gnu++0x $CFLAGS -o '$o' -c '$f'\n";
    print "\n";
}
