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
\brief Definitions for cloud::geo
*/


#include "libcloud/geo.h"

#include "libga/Aligner.h"


namespace cloud
{
namespace geo
{

::geo::Ray const &
axisInBinCloud
	()
{
	// probe height is about .606
//	constexpr double zCloudInRef{ -.765 }; // smaller: cloud to low
	// constexpr double zCloudInRef{ -.630 }; // 
	constexpr double zCloudInRef{ -.950 }; // 
//	constexpr double zCloudInRef{ -.500 }; // larger: cloud to high
//	static ga::Vector const origCloudInRef(0., 0., zCloudInRef);
	// on screen view0:
	// x: down
	// y: right
	// z: up
	static ga::Vector const origCloudInRef(-.055, -.015, zCloudInRef);
	static ::geo::Ray const axis(origCloudInRef, ga::e2);
	return axis;
}

ga::Pose const &
poseCloudWrtRef
	()
{
	// cloud data axis
	static ::geo::Ray const axisInCloud(axisInBinCloud());

	// attitude -- note convention (into:cloud, from:ref)
	// cloud : ref
	//  e1 : -e2  (-Y)
	//  e2 : -e3  (-Z)
	//  e3 : +e1  (+X)
	static ga::Vector const & cloudDown = axisInCloud.theDir;
	static ga::Vector const & refDown = -ga::e3;
	static ga::Vector const & cloudAzim = ga::e1;
	static ga::Vector const & refAzim = -ga::e2;
	// static ga::Vector const & cloudAzim = ga::e3;
	// static ga::Vector const & refAzim =  ga::e1;
	if (cloudAzim.nearlyEquals(cloudDown))
	{
		io::err() << "ERROR: cloud axis/azimth must be distinct" << std::endl;
		assert(! "check/change clound axis pair");
	}

	static ga::Aligner::ExactFit<ga::Vector> const eFit(refDown, cloudDown);
	static ga::Aligner::BestTry<ga::Vector> const  bTry(refAzim, cloudAzim);
	static ga::Aligner const align(eFit, bTry);

	static ga::Pose const att(align.pose());
	return att;
}

ga::Rigid const &
xformCloudWrtRef
	()
{
	// cloud data axis
	static ::geo::Ray const axisInCloud(axisInBinCloud());

	// offset
	static ga::Vector const orig(axisInCloud.theStart);

	// attitude -- note passive convention
	static ga::Pose const attCloudWrtRef(poseCloudWrtRef());

	static ga::Rigid xform(orig, attCloudWrtRef);
	return xform;
}

}
}

