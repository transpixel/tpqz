
# TPQZ (libtpqz)

Stand-alone C++ code constructs for developing techy engineering-type software.


## Keywords
C++, Library, Toolkit, Utilities, Geometric Algebra, 3D Transformations

## Questions and Help?

Please be encouraged to:
* Browse the information in this README file
* Browse the doxygen generated project info at <:TODO>
* Chat, email, wiki... at <:TODO>


## What Is It?

A library of stand-alone C++ code constructs (functions, template,
and classes) perhaps useful(+) for developing techy
engineering-type software subsystems and applications.

The original code began in support of software systems involved in
3D geometry, computer vision, photogrammetry and remote sensing operations.

Developing code for these kinds of applications required a fair number
of engineering- and math- specific code operations. For example math::atan2
that avoids the ambiguity of std::atan2() (for which both -pi and +pi can be
returned). 

(+) at least the code was useful in an actual proprietary project - outside
of that, caveat consumptor.


### What's in the Name?

Short version: Needs something, should be unique-ish, and there it is.

Longer quasi-creative version:

TPQZ is a compositie of the acronym "tpq" from TransPixel-Quantum with
the "z" added to make it unique and easily pronouncable "Tip-Que-Zee".

TransPixel is a (not-yet open source) software code base supporting high
precision measurement from digital imaging systems. The Quantum component
of this provides a shared generic foundation comprising various C++ utility
constructs and which has been factored into TPQZ.


## What's It Good For?

With certainty, it's useful as a code toolbox for it's original
proprietary purpose. With varying degrees of probability it may
also be useful for other activities.

* General capabilities
	* libdat - useful generic data structures and functions (e.g. grid<>, nullValue<>)
	* libmath - useful simple math extensions (e.g. interpolation, quadratic equation roots)
	* libga - geometric algebra support for 3D space (e.g. simple support for Scalars, Vectors, BiVectors and TriVectors and common operations involving them)
	* libio - (very) minimal - a few functions (e.g. thread safe output)
	* libsys - (very) minimal - a few functions (e.g. thread pool)
	* TODO... add descriptions

* A low overhead option for a few useful C++ extensions e.g.
	* many templates for simplicity
	* templatized 'null value propagation' (to avoid exceptions)

* High

 * provides a few 'extended' capabilities without much overhead (such as from
boost or other larger libraries)

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

#### What Environment?

The code is intended to be environment agnostic, but is currently (2017.05)
only exercised on linux/gcc environment.

#### What Dependencies?

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



