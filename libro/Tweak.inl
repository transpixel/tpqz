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


/*! \file
\brief Inline definitions for ro::Tweak
*/


namespace ro
{

inline
// explicit
Tweak :: Tweak
	( double const & phi1
	, double const & phi2
	, double const & alpha
	, double const & theta1
	, double const & theta2
	)
	: thePhi1{ phi1 }
	, thePhi2{ phi2 }
	, theAlpha{ alpha }
	, theTheta1{ theta1 }
	, theTheta2{ theta2 }
{
}

inline
bool
Tweak :: isValid
	() const
{
	return dat::isValid(thePhi1);
}

inline
ga::BiVector
Tweak :: phi
	() const
{
	return ga::BiVector(thePhi1, thePhi2, theAlpha);
}

inline
ga::BiVector
Tweak :: theta
	() const
{
	return ga::BiVector(theTheta1, theTheta2, -theAlpha);
}

inline
ga::Pose
Tweak :: attPhi
	() const
{
	return ga::Pose(phi());
}

inline
ga::Pose
Tweak :: attTheta
	() const
{
	return ga::Pose(theta());
}

} // ro

