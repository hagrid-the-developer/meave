#!/usr/bin/env bash

set -ex

sudo apt-get install g++ make liblocal-lib-perl python{,3}-dev libzip-dev libbz2-dev

perl -MCPAN -e'install Log::Message::Simple'

echo 'eval $(perl -I~/perl5 -Mlocal::lib)' >> ~/.bashrc
. ~/.bashrc
echo >&2 "Don't forget to run . ~/.bashrc"

perl -MCPAN -e'install Const::Fast'
perl -MCPAN -e'install Log::Log4perl'
