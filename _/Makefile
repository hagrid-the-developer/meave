all: boost glog jansson hwloc hpx
.PHONY: boost glog jansson hwloc hpx

boost: _boost
_boost:
	_/build-boost.pl

glog: _glog
_glog:
	_/build-glog.pl

jansson: _jansson
_jansson:
	_/build-jansson.pl

hwloc: _hwloc
_hwloc:
	_/build-hwloc.pl

hpx: _hpx
_hpx: boost hwloc
	_/build-hpx.pl
