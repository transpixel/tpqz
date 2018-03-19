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
\brief Inline definitions for ga::Conform
*/


namespace ga
{

inline
// explicit
Conform :: Conform
	( double const & scale
	, Rigid const & rigid
	)
	: theScaleFwd{ scale }
	, theRigidFwd{ rigid }
{
}

inline
// explicit
Conform :: Conform
	( double const & scale
	, ga::Vector const & location
	, ga::Pose const & pose
	)
	: Conform(scale, Rigid(location, pose))
{
}

inline
// explicit
Conform :: Conform
	( double const & scale
	, ga::Vector const & location
	, ga::BiVector const & physAngle
	)
	: Conform(scale, Rigid(location, Pose(physAngle)))
{
}

inline
bool
Conform :: isValid
	() const
{
	return
		( theRigidFwd.isValid()
		, dat::isValid(theScaleFwd)
		);
}

inline
Vector
Conform :: operator()
	( Vector const & vec
	) const
{
	return theScaleFwd * theRigidFwd(vec);
}

inline
Conform
Conform :: operator*
	( Conform const & rhsFirst
	) const
{
	Conform result;
	if (isValid())
	{
		double const & alpha = rhsFirst.theScaleFwd;
		if (std::numeric_limits<double>::min() < std::abs(alpha))
		{
			double const & beta = theScaleFwd;
			Pose const & attR = rhsFirst.theRigidFwd.pose();
			Pose const & attQ = theRigidFwd.pose();
			Vector const & vecA = rhsFirst.theRigidFwd.location();
			Vector const & vecB = theRigidFwd.location();

			double const invAlpha{ 1. / alpha };
			Pose const invR{ attR.inverse() };

			double const netScale{ beta * alpha };
			Vector const netTrans{ vecA + invAlpha*invR(vecB) };
			Pose const netPose{ attQ * attR };
			result = Conform(netScale, netTrans, netPose);
		}
	}
	return result;
}

inline
Conform
Conform :: inverse
	() const
{
	Conform inv;
	if (isValid())
	{
		if (std::numeric_limits<double>::min() < std::abs(theScaleFwd))
		{
			double const invScale{ 1./theScaleFwd };
			Vector const & fwdTrans = theRigidFwd.location();
			Pose const fwdPose = theRigidFwd.pose();
			Pose const invPose{ fwdPose.inverse() };
			Vector const invTrans{ theScaleFwd * fwdPose(-fwdTrans) };
			Rigid const invRigid{ invTrans, invPose };
			inv = Conform(invScale, invRigid);
		}
	}
	return inv;
}

} // ga

