** Repository Structure **


The overall structure is very a "flat" one. The top level directory contains a few miscelanous package-wide tools (e.g. SConscript build directives).

*** Naming ***

Below the top directory are subdirectories for each package component module which are named as:

* extABC: External modules
* libABC: Modular library code
* testABC: Unit tests for library code

*** Grouping/Dependencies ***

The dependency grouping of the modules includes:

* System Environment Configuration

: Eigen: Linear algebra
: boost-filesystem: directory/file searching
: boost-system:

* External

: exthalf: half precision library
: extcli: These stand by themselves

* Lowlevel

: libio: basic IO and data structures
: libdat: useful data structures used through elsewhere

* System Utilities

: libmem: threadsafe queue, (memory inspection for linux only)
: libsys: job control multithreading
: libfile: interface to boost filesystem

* Math and Geometry

: lib

NOTE: the test/ modules utilize arbitrary mix of libraries. E.g. a
testLoLevel/ unit test might utilize libHiLevel/ library components.

