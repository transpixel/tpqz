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

#ifndef cam_XRefBase_INCL_
#define cam_XRefBase_INCL_

/*! \file
\brief Declarations for cam::XRefBase
*/


#include "libcam/cam.h"

#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/validity.h"

#include "libdat/info.h" // todo
#include "libio/stream.h" // todo

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>


namespace cam
{

/*! \brief Collection of values related to both points and acquisitions.

Essentially this is like a database table that holds DatType items
with equal priority keys for point identification (PntKey) and sensor
acquisition indentification (AcqKey).

\par Example
\dontinclude testcam/uXRefSpots.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename DatType>
class XRefBase
{

	dat::grid<DatType> theItemGrid{};

public: // structure (subclasses)

	struct PAItem
	{
		PntNdx thePntNdx{ dat::nullValue<PntNdx>() };
		AcqNdx theAcqNdx{ dat::nullValue<AcqNdx>() };
		DatType theItem = dat::nullValue<DatType>();
	};

	//! ReadOnly access to valid data items
	struct const_iterator
	{
		dat::grid<DatType> const & theItemGrid{};
		dat::ExtentsIterator theIterRC{};

		//! Construct an invalid iterator
		const_iterator
			() = default;

		//! Construct and advance to first valid data items (if any)
		inline
		explicit
		const_iterator
			( dat::grid<DatType> const & grid
			);

		//! Item associated with current iterator state
		inline
		DatType const &
		operator*
			() const;

		//! Item data with pnt and acq context
		inline
		PAItem
		paItem
			() const;

		//! True if end of collection has not yet been encountered
		inline
		explicit
		operator bool
			() const;

		//! Advance to next valid data item (if any)
		inline
		const_iterator &
		operator++
			();

	private:

		//! Advance iterator over null values (or to end)
		inline
		void
		skipNullItems
			();
	};

public: // methods

	//! default null constructor
	inline
	XRefBase
		() = default;

	//! Construct with specified size
	inline
	explicit
	XRefBase
		( size_t const & numPnts
		, size_t const & numAcqs
		);

	//! Construct from valid values within grid
	inline
	explicit
	XRefBase
		( dat::grid<DatType> const & paGrid //!< Rows:PntNdx, Cols:AcqNdx
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Remove all measurements from table
	inline
	void
	clear
		();

	//! Iterator to beginning of collection
	inline
	const_iterator
	begin
		() const;

	//! Read-Only access to table items.
	typename dat::grid<DatType>::const_iterator
	inline
	beginTable
		() const;

	//! Read-Only access to table items.
	typename dat::grid<DatType>::const_iterator
	inline
	endTable
		() const;

	//! Write/Read access to table items.
	typename dat::grid<DatType>::iterator
	inline
	beginTable
		();

	//! Write/Read access to table items.
	typename dat::grid<DatType>::iterator
	inline
	endTable
		();

	//! Capacity for number of points stored
	inline
	size_t
	pntCapacity
		() const;

	//! Capacity for number of points stored
	inline
	size_t
	acqCapacity
		() const;

	//! True if this instance has the same capacities as other
	template <typename XRefOther>
	inline
	bool
	sameCapacityAs
		( XRefOther const & other
		) const;

	//! Write access to data entity
	inline
	DatType &
	operator()
		( PntNdx const & pntndx
		, AcqNdx const & acqndx
		);

	//! ReadOnly access to data entity
	inline
	DatType const &
	operator()
		( PntNdx const & pntndx
		, AcqNdx const & acqndx
		) const;

	//! Number of acquisitions in which this point has a valid measurement
	inline
	size_t
	numAcqsFor
		( PntNdx const & pntndx
		) const;

	//! Number of valid point measurements in this acquisition
	inline
	size_t
	numPntsFor
		( AcqNdx const & acqndx
		) const;

	//! Indices for valid point measurements in this acquisition
	inline
	std::vector<PntNdx>
	pntIndicesFor
		( AcqNdx const & acqndx
		) const;

//	//! Point indices which are in both acquisitions 1 and 2
//	inline
//	std::vector<PntNdx>
//	pntIndicesCommonTo
//		( AcqNdx const & acqndx1
//		, AcqNdx const & acqndx2
//		) const;

	//! Helper struct for managing points in common between two acquisitions
	struct AcqOverlap
	{
		AcqNdx theAcqNdx1{ dat::nullValue<AcqNdx>() };
		AcqNdx theAcqNdx2{ dat::nullValue<AcqNdx>() };
		std::vector<PntNdx> thePntNdxs{};

		AcqOverlap
			() = default;

		explicit
		AcqOverlap
			( AcqNdx const & acq1
			, AcqNdx const & acq2
			, std::vector<PntNdx> const & pntNdxs
			)
			: theAcqNdx1{ acq1 }
			, theAcqNdx2{ acq2 }
			, thePntNdxs{ pntNdxs }
		{ }
	};

	//! Information about acquitision pairs with minCommonPoints in common
	inline
	std::vector<AcqOverlap>
	acqPairsWithOverlap
		( size_t const & minCommonPoints
		) const;

	//! Indices for acquisitions in which this point has a valid measurement
	inline
	std::vector<AcqNdx>
	acqIndicesFor
		( PntNdx const & pntndx
		) const;

	//! All/only the "isValid" data items associated with pntNdx
	inline
	std::vector<DatType>
	validItemsForPnt
		( PntNdx const & pntndx
		) const;

	//! All/only the "isValid" data items associated with acqNdx
	inline
	std::vector<DatType>
	validItemsForAcq
		( AcqNdx const & acqndx
		) const;

	//! Measurements in raster format (can be wasteful!)
	inline
	dat::grid<DatType>
	itemGrid
		() const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = {}
		) const;

	//! Description of active indices in pointNdx major order
	inline
	std::string
	infoStringPntMajor
		( std::string const & title = {}
		);

	//! Description of active indices in acquisitionNdx major order
	inline
	std::string
	infoStringAcqMajor
		( std::string const & title = {}
		);

	//! Representation of dataGrid() in image-friendly layout
	inline
	dat::grid<uint8_t>
	infoImage
		( uint8_t const onValue = 100u
		) const;

}; // XRefBase

} // cam

// Inline definitions
#include "libcam/XRefBase.inl"

#endif // cam_XRefBase_INCL_

