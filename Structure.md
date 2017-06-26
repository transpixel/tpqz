# Repository Structure

The overall repo structure is flat with everything contained in
first level subdirectories. The top level directory contains a few
miscelanous package-wide tools (e.g. SConscript build directives).

## Naming Conventions

Below the top directory are subdirectories for each package component
module which are named as:

* extABC: External modules
* libABC: Modular library code
* testABC: Unit tests for library code

## Module Inventory

The following presents the modules in order of dependency with
modules in groups later in the list depending on modules earlier
in the inventory.

### System Environment Configuration

+ Eigen-v3: Eigen C++ library
+ boost-filesystem,system: used for directory/file searching (Note
easily replaced by C++14 or later)

### Build System Components

+ env: file templates and scripts related to overall build environment.
+ build: scripts to manage builds - e.g. run unit tests, generate docs.

### External Source Code Imports

+ exthalf: half precision (16-bit) library
+ extcli: command line interpreter class

### TPQZ Modules

#### Low-Level 

+ libsys: job multithreading
+ libio: basic IO and data structures
+ libmem: threadsafe queue, (memory inspection for linux only)
+ libfile: wrapper for interface to boost filesystem
+ libdat: useful data structures used through TPQZ

#### Math and Algebra

+ libmath: useful math constructs
+ libga: geometric algebra for 3D
+ libprob: simple stats and PDF/CDF operations
+ libtrans: transformation/mapping support
+ libla: linear algebra: interface libdat/ to Eigen

#### Geometric Entities and Operations

+ libgeo: geometry library (rays, cylinders, etc)
+ libcloud: simple point cloud support

#### Image Processing

+ libimg: simple basic image manipulations (uses OpenCV for I/O
and format conversion).

#### Signal Processing

+ libsig: primative signal processing operations.

#### Photogrammetry / Computer Vision

+ libcam: simple camera/image support
+ libmap: cartographic projections
+ libro: relative orientation

#### Prototype Generic Rendering

+ libmodel: environment modeling (geometry and radiometry)
+ libsen: general sensing operations


## Test Modules

NOTE: the test.../ modules utilize arbitrary mix of libraries. E.g. a
testLoLevel/ unit test might utilize libHiLevel/ library components.

