//
//
// MIT License
//
// Copyright (c) 2017 Stellacore Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//

#ifndef ro_Pair_INCL_
#define ro_Pair_INCL_

/*! \file
\brief Declarations for ro::Pair
*/


#include "libga/Rigid.h"
#include "libro/ops.h"
#include "libro/ro.h"

#include <string>


namespace ro
{

// TODO -- rename methods for consistency across the board
//
// Classes: ?  Model <- Pair ?
//
// loc2w1()
// vector2w1()
// offset2w1() // linear offset
// separation? split? divide? partition? split? distinct? unique?
//
// angle2w1() // angular offset
// pose2w1() // angle as transform
//
// rigid2w1() // linear/angular offset combined as transform
//

/*! \brief Interface for a pair of orientations forming a simple stereo model

\par Example
\dontinclude testro/uPair.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Pair
{
	std::string theClassName{ "Null" };

public: // basic constructions

	//! Null constructor
	Pair
		() = default;

protected: // basic constructions

	//! Associate class name with instance
	explicit
	Pair
		( std::string const & pairName
		)
		: theClassName{ pairName }
	{ }

// VIRTUAL
public: // pure virtual methods

	//! Check if instance is valid
	virtual
	bool
	isValid
		() const = 0;

	//
	// Orientations
	//

	//! Orientation of primary member
	virtual
	ga::Rigid
	rigid1w0
		() const = 0;

	//! Orientation of secondary member
	virtual
	ga::Rigid
	rigid2w0
		() const = 0;

	//! Orientation of model w.r.t. ctor-reference frame
	virtual
	ga::Rigid
	rigid0wRef
		() const = 0;

// VIRTUAL
public: // Default virtual methods

	//
	// Information
	//

	//! Descriptive information about this instance.
	virtual
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information about this instance.
	virtual
	std::string
	infoStringDetail
		( std::string const & title = std::string()
		) const;

public: // default implementations

	//
	// Positions
	//

	//! Location of primary member
	inline
	ga::Vector
	station1w0
		() const;

	//! Location of secondary member
	inline
	ga::Vector
	station2w0
		() const;

	//! Relative offset of secondary w.r.t. primary
	inline
	ga::Vector
	baseVectorIn0
		() const;

	//! Base-Plane: GA/G3 Dual of baseVectorIn0
	inline
	ga::BiVector
	basePlaneDirIn0
		() const;

	//! Length of baseVector()
	inline
	double
	baseMag
		() const;

	//! Relative baseline of secondary w.r.t. primary expressed in prime frame
	inline
	ga::Vector
	baseVector2w1
		() const;

	//
	// Attitudes
	//

	//! Attitude of primary member
	inline
	ga::Pose
	pose1w0
		() const;

	//! Attitude of secondary member
	inline
	ga::Pose
	pose2w0
		() const;

	//! Physical pose angle of primary member
	inline
	ga::BiVector
	angle1w0
		() const;

	//! Physical pose angle of secondary member
	inline
	ga::BiVector
	angle2w0
		() const;

	//! Relative angle of secondary w.r.t. primary
	inline
	ga::BiVector
	baseAngle2w1
		() const;

	//
	// Relative Orientations
	//

	//! Association of rigidNw0() transforms
	inline
	std::pair<ga::Rigid, ga::Rigid>
	pair
		() const;

	//! Full orientation of secondary frame w.r.t. primary
	inline
	ga::Rigid
	rigid2w1
		() const;

public: // general use functions

	//! Run time identification of current objects
	std::string const &
	className
		() const
	{
		return theClassName;
	}

	//
	// Environment interaction
	//

	//! Corresponding unit directions for pnt expressed in RO-model space
	std::pair<ga::Vector, ga::Vector>
	uvDirectionsFor
		( ga::Vector const & pntIn0
		) const;

	//
	// Coplanarity relationships
	//

	//! Fully rigorous coplanarity misclosure value
	double
	tripleProductGap
		( ga::Vector const & uDirIn1
		, ga::Vector const & vDirIn2
		) const;

	//! Convenience version
	inline
	double
	tripleProductGap
		( std::pair<ga::Vector, ga::Vector> const & uvDirs
		) const;

	//! True if this and other have rigid2w1() that are nearlyEquals
	bool
	nearlyEquals
		( Pair const & other
		) const;

}; // Pair

} // ro

// Inline definitions
#include "libro/Pair.inl"

#endif // ro_Pair_INCL_

