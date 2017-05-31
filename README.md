
Copyright (c) 2017 Stellacore Corporation. Refer to LICENSE file.

# TPQZ (libtpqz) "Tip-Que-Zee"

Stand-alone C++ code constructs for developing techy engineering-type software.


## Keywords
C++, Library, Toolkit, Utilities, Geometric Algebra, 3D Transformations


## Questions and Help?

Please be encouraged to:
* Browse the information in this README file
* Browse the doxygen generated project info at <:TODO>
* Chat, email, wiki... at <:TODO>


## What Is It?

TPQZ is a library of stand-alone C++ code constructs (functions, template,
and classes) perhaps useful(+) for developing techy
engineering-type software subsystems and applications.

The original code began in support of software systems involved in
3D geometry, surveying, computer vision, photogrammetry and
remote sensing operations.

Developing code for those applications required a fair number
of engineering-specific and math-specific code operations.
For example math::atan2 that avoids the ambiguity of std::atan2()
(for which both -pi and +pi can be returned) and other practical-world
things like that.

(+) at least the code was useful in some specific actual proprietary
projects - outside of that, caveat consumptor.


### What's in the Name?

Short version: Everything needs a name, should be unique-ish, and here it is.

Longer version: 
TPQZ is a compositie of the acronym "tpq" from TransPixel-Quantum with
the "z" added to make it unique and easily pronouncable "Tip-Que-Zee".

TransPixel is a (not-yet open source) software code base supporting high
precision measurement from digital imaging systems. TransPixel's "Quantum"
component provides a shared generic foundation which comprises
various C++ utility constructs and which has been factored into TPQZ.


## What's TPQZ Good For?

With certainty, it's useful as a code toolbox for its specific original
proprietary purpose. Outside of that, and with varying degrees of 
probable success, it may also be useful for other similar
technical software development activities.

 * provides a few capabilities which extend standard C++ (without the
overhead of a large framework like boost) and a number of capabilities
which are particularly useful to mathematical operations common in much
practical engineering work.

* Example capabilities
	* libdat - useful generic data structures and functions (e.g. grid<>, nullValue<>)
	* libmath - useful simple math extensions (e.g. interpolation, quadratic equation roots)
	* libga - geometric algebra support for 3D space (e.g. simple support for Scalars, Vectors, BiVectors and TriVectors and common operations involving them)
	* TODO... add descriptions lib{base,dat,file,ga,geo,img,io.la,math,mem,prob,sig,sys,trans}

## Getting Started

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

The initial internal and generated documentation/comments are rather
sparse. Therefore please do not hesitate to contact <:TODO> with questions
and/or suggestions to update the documentation.


### What's Needed?

#### Environment:

The code is intended to be environment agnostic, but is currently (2017.05)
only exercised on linux/gcc environment.

#### Dependencies:

Required
* C++11 (or later) compiler
* SCons for build system (which requires python)

Optional
* Doxygen for producing html docs from source code
* bash (or equivalent) for a few supporting scripts


## Can I Contribute?

Sure!
TODO - Establish options - for now,
* contact <:TODO>



