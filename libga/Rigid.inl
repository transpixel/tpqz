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


/*! \file
\brief Inline definitions for ga::Rigid
*/

namespace ga
{
//======================================================================

inline
// static
Rigid const &
Rigid :: identity
	()
{
static Rigid const ident{vZero, Pose{bZero}};
	return ident;
}

inline
bool
Rigid :: isValid
	() const
{
	return (theTrans.isValid() && thePose.isValid());
}

inline
Vector const &
Rigid :: location
	() const
{
	return theTrans;
}

inline
Pose const &
Rigid :: pose
	() const
{
	return thePose;
}

inline
BiVector
Rigid :: physicalAngle
	() const
{
	return thePose.physicalAngle();
}

inline
Vector
Rigid :: operator()
	( Vector const & vec
	) const
{
	return thePose(vec - theTrans);
}

inline
Vector
Rigid :: transformedDirection
	( Vector const & vec
	) const
{
	return thePose(vec);
}

//======================================================================
}

