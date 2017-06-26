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
\brief Inline definitions for sen::render
*/


#include "libdat/ExtentsIterator.h"
#include "libdat/RowCol.h"
#include "libdat/Spot.h"
#include "libgeo/Ray.h"
#include "libgeo/xform.h"
#include "libmodel/Part.h"


namespace sen
{
namespace render
{

namespace
{
	//! Render into the elements of a sensor imprint
	template 
		< typename ImprintType
		, typename SenseOpType
		, typename ProjOpType
		, typename ModelType
		>
	inline
	void
	renderInto
		( ImprintType * const & ptGrid
		, ModelType const & modelInRef
		, ga::Rigid const & xSenWrtRef
		, ProjOpType const & projOp
		, SenseOpType const & atomSensing
		)
	{
		ga::Rigid const xRefWrtSen(xSenWrtRef.inverse());

		dat::ExtentsIterator itRC{ptGrid->hwSize()};
		for (typename ImprintType::iterator
			iter{ptGrid->begin()} ; ptGrid->end() != iter ; ++iter, ++itRC)
		{
			// map location into which to "pull" data atoms
			dat::RowCol const rowcol = *itRC;
			dat::Spot const spot(dat::cast::Spot(rowcol));

			// cast ray forward through projector
			geo::Ray const rayInSen(projOp(spot));

			// convert ray into expression w.r.t world frame
			geo::Ray const rayInRef(geo::xform::apply(xRefWrtSen, rayInSen));

			// intersect ray with world model (e.g. here an object simulation)
			// (to get geometric location in world)
			ga::Vector const locPartInRef(modelInRef.locationFor(rayInRef));

			// get model component at this world location
			// (to get object characteristics expressed w.r.t. world)
			model::Part const partInRef(modelInRef.partFor(locPartInRef));

			// transform model components to expression w.r.t. sensor space
			model::Part const partInSen(partInRef.transformedBy(xSenWrtRef));

			// perform sensing operation
			*iter = atomSensing(partInSen, rayInSen);
		}
	}
}

template 
	< typename ImprintType
	, typename SenseOpType
	, typename ProjOpType
	, typename ModelType
	>
inline
ImprintType // currently coded to support grids
imprintFrom
	( ModelType const & modelInRef
	, ga::Rigid const & xSenWrtRef
	, ProjOpType const & projOp
	, dat::Extents const & hwSize
	)
{
	ImprintType imprint(hwSize); // allocate space
	SenseOpType const senseOp; // create a sensor instance
	renderInto(& imprint, modelInRef, xSenWrtRef, projOp, senseOp);
	return imprint;
}

}
}

