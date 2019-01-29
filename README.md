
# TPQZ (libtpqz) "Tip-Q-Zee"

Stand-alone C++ code constructs for developing techy engineering-type software.

## External Component Licenses

All original work in TPQZ is covered by the unrestrictive MIT License
which provides for use in commercial products and other for-profit
activities.

* TPQZ: MIT License: Copyright (c) 2017 Stellacore Corporation.

All integrated 3rd party code (as external directory modules) are
consistent with the TPQZ licensing. Specifically

* exthalf:
   MIT License: Copyright (c) 2012-2013 Christian Rau
<rauy@users.sourceforge.net>

* extcli:
   MIT License: Copyright (c) 2015 - 2016 Florian Rappl
https://github.com/FlorianRappl/CmdParser

* extstb:
   MIT License: Copyright (c) 2017 Sean Barrett
http://nothings.org/stb

* boost:
   Boost License:
   * Website: http://www.boost.org/

* Eigen-v3:
   Mozilla Public License (MPL2): Guennebaud and Beno, Jacob and others
  * Website: http://eigen.tuxfamily.org/index.php?title=Main_Page

* OpenCV: BSD License: Many [Copyright Holders](https://github.com/opencv/opencv/blob/master/LICENSE) Including:
Copyright (C) 2000-2016, Intel Corporation,,
Copyright (C) 2009-2011, Willow Garage Inc.,,
Copyright (C) 2009-2016, NVIDIA Corporation,,
Copyright (C) 2010-2013, Advanced Micro Devices, Inc.,,
Copyright (C) 2015-2016, OpenCV Foundation,,
Copyright (C) 2015-2016, Itseez Inc.,,
  * Website: http://opencv.org/

## Keywords
C++, Library, Toolkit, Utilities, Geometric Algebra, 3D Transformations


## Questions and Help?

Please be encouraged to:
* Browse the information in this README file
* Browse the doxygen generated project info at <:TODO>
* Chat, email, wiki... at <:TODO>


## What Is It?

TransPixel-QuantumZ or "TPQZ" is a library of stand-alone C++ code
constructs (functions, template,
and classes) perhaps useful(+) for developing techy
engineering-type software subsystems and applications.

(+) at least the code was useful in some specific actual proprietary
projects - outside of that, caveat consumptor.

The original code began in support of software systems involved in
3D geometry, surveying, computer vision, photogrammetry and
remote sensing operations.

Developing code for those applications required a fair number
of engineering-specific and math-specific code operations.
For example math::atan2 that avoids the ambiguity of std::atan2()
for which both -pi and +pi can be returned -and other practical-world
things like that.


### What's in the Name?

Short version: Everything needs a name, should be unique-ish, and here it is.

Longer version: 
TPQZ is a compositie of the acronym "tpq" from TransPixel-Quantum with
the "z" added to make it unique and easily pronouncable "Tip-Q-Zee".

TransPixel is a (not-yet open source) software code base supporting high
precision measurement from digital imaging systems. TransPixel's "Quantum"
component provides a shared generic foundation which comprises
various C++ utility constructs and which has been factored into TPQZ.


## What's TPQZ Good For?

With certainty, it's useful as a code toolbox for its specific original
proprietary purpose. Outside of that, and with varying degrees of 
probable success, it may also be useful for other similar
technical software development activities - or perhaps not so much.

 * provides a few general capabilities which extend standard C++ (without the
overhead of a larger framework like boost) and a number of capabilities
which are particularly useful to mathematical operations common in much
practical engineering work.

* Example capabilities
	* libdat - useful generic data structures and functions (e.g. grid<>, nullValue<>)
	* libmath - useful simple math extensions (e.g. interpolation, quadratic equation roots)
	* libga - geometric algebra support for 3D space (e.g. simple support for Scalars, Vectors, BiVectors and TriVectors and common operations involving them)
	* TODO... add descriptions lib{base,dat,file,ga,geo,img,io.la,math,mem,prob,sig,sys,trans}


## Getting Started

The [organizational structure of this repo](Structure.md) is
described in the top-level file Structure.md

For an initial quicklook, browse the overview documentation here <:TODO>

For a more detailed experience, download the source code and run a few
unit tests. E.g.

* download the source code
* from top level directory, build via
	$ cd <top-level-dirTODO>
	$ scons
* run unit tests via
	$ cd <top>/build
	$ sh RunUnitTests.sh
* explore interesting concepts
	* e.g. identify by test/class name (TODO inventory list?)
	* read class/namespace/func comment (e.g. in code or via doxygen)
	* if useage/args not clear, then read the corresponding
	unit test code for concrete example of usage.

The initial internal and generated documentation/comments
are rather sparse. Therefore please do not hesitate to contact
[info@transpixel.org](mailto:info@transpixel.org) with questions
and/or suggestions to update the documentation.


### What's Needed?

There are likely many changes/extensions required or useful for
TPQZ to be used in a new context.

#### Environment:

The code is intended to be environment agnostic, but is currently (2017.05)
only exercised on linux/gcc environment.

There are a few items of dormant code that are linux only. Such code
items are here for convenience only during development activities -
and are not dependencies of the library.

#### Dependencies:

Required
* C++11 (or later) compiler (C++14 or later to remove use of boost)
* SCons for build system (which requires python)

Special Requirements
* boost::filesystem
	* boost::system
* boost::graph
	* boost::serialization
	* boost::property_map

Optional
* Doxygen for producing html docs from source code
* bash (or equivalent) for a few supporting scripts


## Can I Contribute?

Sure!
For now,
* contact [info@transpixel.org](mailto:info@transpixel.org)



