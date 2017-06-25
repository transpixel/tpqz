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
\brief Inline definitions for ro::Pair
*/


namespace ro
{

//
// Positions
//

inline
ga::Vector
Pair :: station1w0
	() const
{
	return rigid1w0().location();
}

inline
ga::Vector
Pair :: station2w0
	() const
{
	return rigid2w0().location();
}

inline
ga::Vector
Pair :: baseVectorIn0
	() const
{
	return { rigid2w0().location() - rigid1w0().location() };
}

inline
ga::BiVector
Pair :: basePlaneDirIn0
	() const
{
	return { ga::unit(ga::E123 * baseVectorIn0()) };
}

inline
double
Pair :: baseMag
	() const
{
	return { ga::magnitude(baseVectorIn0()) };
}

inline
ga::Vector
Pair :: baseVector2w1
	() const
{
	ga::Pose const att1w0{ pose1w0() };
	return att1w0(baseVectorIn0());
}

//
// Attitudes
//

inline
ga::Pose
Pair :: pose1w0
	() const
{
	return rigid1w0().pose();
}

inline
ga::Pose
Pair :: pose2w0
	() const
{
	return rigid2w0().pose();
}

inline
ga::BiVector
Pair :: angle1w0
	() const
{
	return pose1w0().physicalAngle();
}

inline
ga::BiVector
Pair :: angle2w0
	() const
{
	return pose2w0().physicalAngle();
}

inline
ga::BiVector
Pair :: baseAngle2w1
	() const
{
	ga::Pose const att1wQ{ pose1w0() };
	ga::Pose const att2wQ{ pose2w0() };
	ga::Pose const attQw1{ att1wQ.inverse() };
	ga::Pose const att2w1{ att2wQ * attQw1 };
	return att2w1.physicalAngle();
}

inline
std::pair<ga::Rigid, ga::Rigid>
Pair :: pair
	() const
{
	return { rigid1w0(), rigid2w0() };
}

inline
ga::Rigid
Pair :: rigid2w1
	() const
{
	return ro::relative2w1(pair());
}


inline
double
Pair :: tripleProductGap
	( std::pair<ga::Vector, ga::Vector> const & uvDirs
	) const
{
	return tripleProductGap(uvDirs.first, uvDirs.second);
}

} // ro

