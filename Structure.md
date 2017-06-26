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

* Low-Level 

--- group

: libsys: job multithreading
: libio: basic IO and data structures
: libmem: threadsafe queue, (memory inspection for linux only)
: libfile: interface to boost filesystem
: libdat: useful data structures used through elsewhere

--- group

* Math and Algebra

: libmath: useful math constructs
: libga: geometric algebra for 3D use
: libprob: simple stats and PDF/CDF operations
: libtrans: transformation/mapping support
: libla: linear algebra: interface libdat/ to Eigen

--- group

* Geometric Entities and Operations

: libgeo: geometry library (rays, cylinders, etc)

* Image Processing

: libimg: simple basic image manipulations (uses OpenCV for I/O and format conversion).

* Signal Processing

: libsig: primative signal processing operations.

--- group

* Photogrammetry / Computer Vision

: libcam: simple camera/image support
: libmap: cartographic projections
: libro: relative orientation

--- group

* Prototype Generic Rendering

: libmodel: environment modeling (geometry and radiometry)
: libsen: general sensing operations

*** Test Modules ***

NOTE: the test/ modules utilize arbitrary mix of libraries. E.g. a
testLoLevel/ unit test might utilize libHiLevel/ library components.

