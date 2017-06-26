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

#ifndef sen_funcProjOp_INCL_
#define sen_funcProjOp_INCL_

/*! \file
\brief Declarations for sen::funcProjOp
*/


#include "libcam/PinHole.h"
#include "libdat/Extents.h"
#include "libdat/Range.h"
#include "libdat/Spot.h"
#include "libdat/SpotX.h"
#include "libgeo/Ray.h"
#include "libmath/Partition.h"

#include <vector>


namespace sen
{

//! Functors for cylindrical coordinate frame projection.
namespace cylinder
{
	struct ProjOp
	{

	private:

		math::Partition thePartZ;
		std::vector<double> theLutCos;
		std::vector<double> theLutSin;

	public:

		//! Example Projection: Cylindrical
		ProjOp
			( dat::Extents const & hwSize
			, dat::Range<double> const & zRange
			);

		//! 3D ray associated with map location
		inline
		geo::Ray
		operator()
			( dat::Spot const & mapSpot
			) const;
	};
}

//! Functors for central-point perspective.
namespace central
{
	struct ProjOp
	{
		cam::PinHole const theCam;
		dat::SpotX const theOptWrtDet;

		//! A centeral perspective camera with this principal distance
		explicit
		ProjOp
			( dat::Extents const & detSize
			);

		//! Rectilinear ray-cast
		inline
		geo::Ray
		operator()
			( dat::Spot const & spotInDet
			) const;
	};
}

}

// Inline definitions
#include "libsen/funcProjOp.inl"

#endif // sen_funcProjOp_INCL_

