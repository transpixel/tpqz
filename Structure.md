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

: Eigen-v3: Eigen C++ library
: boost-filesystem,system: directory/file searching (Note easily replaced by C++14 or later)

* External

: exthalf: half precision library
: extcli: These stand by themselves

* Lowlevel

: libio: basic IO and data structures
: libdat: useful data structures used through elsewhere

* System Utilities

: libmem: threadsafe queue, (memory inspection for linux only)
: libsys: job multithreading
: libfile: interface to boost filesystem

* Math and Algebra

: libmath: useful math constructs
: libga: geometric algebra for 3D use
: libprob: simple stats and PDF/CDF operations
: libtrans: transformation/mapping support

* Geometric Entities and Operations

: libgeo: geometry library (rays, cylinders, etc)

* Signal Processing

: libsig: primative signal processing operations

* Numerical Operations

: libla: interface libdat structs to Eigen


NOTE: the test/ modules utilize arbitrary mix of libraries. E.g. a
testLoLevel/ unit test might utilize libHiLevel/ library components.

