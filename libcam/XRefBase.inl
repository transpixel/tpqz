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
\brief Inline definitions for cam::XRefBase
*/


#include "libio/LinePrefixer.h"


namespace cam
{

template <typename DatType>
inline
// explicit
XRefBase<DatType>::const_iterator :: const_iterator
	( dat::grid<DatType> const & grid
	)
	: theItemGrid(grid)
	, theIterRC(theItemGrid.hwSize())
{
	// advance to first valid data item
	skipNullItems();
}

template <typename DatType>
inline
DatType const &
XRefBase<DatType>::const_iterator :: operator*
	() const
{
	static DatType const aNull(dat::nullValue<DatType>());
	if (theIterRC)
	{
		dat::RowCol const & rowcol = *theIterRC;
		// DatType const & item = theItemGrid(rowcol);
		return theItemGrid(rowcol);
	}
	else
	{
		return aNull;
	}
}

template <typename DatType>
inline
typename XRefBase<DatType>::PAItem
XRefBase<DatType>::const_iterator :: paItem
	() const
{
	PAItem pai{};
	if (theIterRC)
	{
		dat::RowCol const & rowcol = *theIterRC;
		pai.thePntNdx = rowcol[0];
		pai.theAcqNdx = rowcol[1];
		pai.theItem = theItemGrid(rowcol);
	}
	return pai;
}

template <typename DatType>
inline
// explicit
XRefBase<DatType>::const_iterator :: operator bool
	() const
{
	return static_cast<bool>(theIterRC);
}

template <typename DatType>
inline
typename XRefBase<DatType>::const_iterator &
XRefBase<DatType>::const_iterator :: operator++
	()
{
	++theIterRC;
	skipNullItems();
	return *this;
}

template <typename DatType>
inline
void
XRefBase<DatType>::const_iterator :: skipNullItems
	()
{
	while (theIterRC && (! dat::isValid(theItemGrid(*theIterRC))))
	{
		++theIterRC;
	}
}


//======================================================================
//======================================================================
//======================================================================


template <typename DatType>
inline
// explicit
XRefBase<DatType> :: XRefBase
	( size_t const & numPnts
	, size_t const & numAcqs
	)
	: theItemGrid(numPnts, numAcqs, dat::nullValue<DatType>())
{
}

template <typename DatType>
inline
// explicit
XRefBase<DatType> :: XRefBase
	( dat::grid<DatType> const & paGrid
	)
	: theItemGrid(paGrid)
{
}

template <typename DatType>
inline
bool
XRefBase<DatType> :: isValid
	() const
{
	return theItemGrid.isValid();
}

template <typename DatType>
inline
void
XRefBase<DatType> :: clear
	()
{
	std::fill
		( theItemGrid.begin(), theItemGrid.end()
		, dat::nullValue<DatType>()
		);
}

template <typename DatType>
inline
typename XRefBase<DatType>::const_iterator
XRefBase<DatType> :: begin
	() const
{
	const_iterator const iter(theItemGrid);
	return iter;
}

template <typename DatType>
typename dat::grid<DatType>::const_iterator
inline
XRefBase<DatType> :: beginTable
	() const
{
	return theItemGrid.begin();
}

template <typename DatType>
typename dat::grid<DatType>::const_iterator
inline
XRefBase<DatType> :: endTable
	() const
{
	return theItemGrid.end();
}

template <typename DatType>
typename dat::grid<DatType>::iterator
inline
XRefBase<DatType> :: beginTable
	()
{
	return theItemGrid.begin();
}

template <typename DatType>
typename dat::grid<DatType>::iterator
inline
XRefBase<DatType> :: endTable
	()
{
	return theItemGrid.end();
}

template <typename DatType>
inline
size_t
XRefBase<DatType> :: pntCapacity
	() const
{
	return theItemGrid.high();
}

template <typename DatType>
inline
size_t
XRefBase<DatType> :: acqCapacity
	() const
{
	return theItemGrid.wide();
}

template <typename DatType>
template <typename XRefOther>
inline
bool
XRefBase<DatType> :: sameCapacityAs
	( XRefOther const & other
	) const
{
	return
		(  (pntCapacity() == other.pntCapacity())
		&& (acqCapacity() == other.acqCapacity())
		);
}

template <typename DatType>
inline
DatType &
XRefBase<DatType> :: operator()
	( PntNdx const & pntndx
	, AcqNdx const & acqndx
	)
{
	return theItemGrid(pntndx, acqndx);
}

template <typename DatType>
inline
DatType const &
XRefBase<DatType> :: operator()
	( PntNdx const & pntndx
	, AcqNdx const & acqndx
	) const
{
	return theItemGrid(pntndx, acqndx);
}

template <typename DatType>
inline
size_t
XRefBase<DatType> :: numAcqsFor
	( PntNdx const & pntndx
	) const
{
	return acqIndicesFor(pntndx).size();
}

template <typename DatType>
inline
size_t
XRefBase<DatType> :: numPntsFor
	( AcqNdx const & acqndx
	) const
{
	return pntIndicesFor(acqndx).size();
}

template <typename DatType>
inline
std::vector<PntNdx>
XRefBase<DatType> :: pntIndicesFor
	( AcqNdx const & acqndx
	) const
{
	std::vector<PntNdx> ndxs;
	ndxs.reserve(pntCapacity());
	for (size_t nn{0u} ; nn < theItemGrid.high() ; ++nn)
	{
		PntNdx const & pntndx = nn;
		DatType const & item = theItemGrid(pntndx, acqndx);
		if (dat::isValid(item))
		{
			ndxs.emplace_back(pntndx);
		}
	}
	return ndxs;
}

/*
template <typename DatType>
inline
std::vector<PntNdx>
XRefBase<DatType> :: pntIndicesCommonTo
	( AcqNdx const & acqndx1
	, AcqNdx const & acqndx2
	) const
{
	std::vector<PntNdx> boths;
	std::vector<PntNdx> const p1s{ pntIndicesFor(acqndx1) };
	std::vector<PntNdx> const p2s{ pntIndicesFor(acqndx2) };
	boths.reserve(std::max(p1s.size(), p2s.size()));
	std::set_intersection
		( p1s.begin(), p1s.end()
		, p2s.begin(), p2s.end()
		, std::inserter(boths, boths.end())
		);
	return boths;
}
*/

template <typename DatType>
inline
std::vector<typename XRefBase<DatType>::AcqOverlap>
XRefBase<DatType> :: acqPairsWithOverlap
	( size_t const & minCommonPoints
	) const
{
	std::vector<AcqOverlap> acqInfos;

	// find acquisitions with at least the minimum number of points
	using PntIndices = std::vector<PntNdx>;
	std::vector<std::pair<AcqNdx, PntIndices> > okayAcqPntNdxs;
	size_t const numAcqs{ acqCapacity() };
	okayAcqPntNdxs.reserve(numAcqs);
	for (size_t acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		std::vector<PntNdx> const pntNdxs{ pntIndicesFor(acqNdx) };
		if (minCommonPoints < pntNdxs.size())
		{
			okayAcqPntNdxs.emplace_back(std::make_pair(acqNdx, pntNdxs));
		}
	}

	// search all okayAcquisitions for pairs with enough points in common
	size_t const numOkay{ okayAcqPntNdxs.size() };
	for (size_t n1{0u} ; n1 < numOkay ; ++n1)
	{
		// access point measurements for first okay acquisition
		AcqNdx const & acq1 = okayAcqPntNdxs[n1].first;
		PntIndices const & p1s = okayAcqPntNdxs[n1].second;

		for (size_t n2{n1+1} ; n2 < numOkay ; ++n2)
		{
			// access point measurements for second okay acquisition
			AcqNdx const & acq2 = okayAcqPntNdxs[n2].first;
			PntIndices const & p2s = okayAcqPntNdxs[n2].second;

			// get point indices in common between acq1 and acq2
			std::vector<PntNdx> pntNdxs;
			std::set_intersection
				( p1s.begin(), p1s.end()
				, p2s.begin(), p2s.end()
				, std::inserter(pntNdxs, pntNdxs.end())
				);

			// if enough in common
			if (minCommonPoints <= pntNdxs.size())
			{
				// return each pair with enough overlap
				AcqOverlap const acqInfo(acq1, acq2, pntNdxs);
				acqInfos.emplace_back(acqInfo);
			}
		}
	}

	return acqInfos;
}

template <typename DatType>
inline
std::vector<AcqNdx>
XRefBase<DatType> :: acqIndicesFor
	( PntNdx const & pntndx
	) const
{
	std::vector<AcqNdx> ndxs;
	ndxs.reserve(acqCapacity());
	for (size_t nn{0u} ; nn < theItemGrid.wide() ; ++nn)
	{
		AcqNdx const & acqndx = nn;
		DatType const & item = theItemGrid(pntndx, acqndx);
		if (dat::isValid(item))
		{
			ndxs.emplace_back(acqndx);
		}
	}
	return ndxs;
}

template <typename DatType>
inline
std::vector<DatType>
XRefBase<DatType> :: validItemsForPnt
	( PntNdx const & pntndx
	) const
{
	std::vector<DatType> items;
	size_t const numAcqs{ theItemGrid.wide() };
	items.reserve(numAcqs);
	for (size_t acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		DatType const & item = theItemGrid(pntndx, acqNdx);
		if (dat::isValid(item))
		{
			items.emplace_back(item);
		}
	}
	return items;
}

template <typename DatType>
inline
std::vector<DatType>
XRefBase<DatType> :: validItemsForAcq
	( AcqNdx const & acqndx
	) const
{
	std::vector<DatType> items;
	size_t const numPnts{ theItemGrid.high() };
	items.reserve(numPnts);
	for (size_t pntNdx{0u} ; pntNdx < numPnts ; ++pntNdx)
	{
		DatType const & item = theItemGrid(pntNdx, acqndx);
		if (dat::isValid(item))
		{
			items.emplace_back(item);
		}
	}
	return items;
}

template <typename DatType>
inline
dat::grid<DatType>
XRefBase<DatType> :: itemGrid
	() const
{
	return theItemGrid;
}

template <typename DatType>
inline
std::string
XRefBase<DatType> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << "theItemGrid: " << dat::infoString(theItemGrid);
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

template <typename DatType>
inline
std::string
XRefBase<DatType> :: infoStringPntMajor
	( std::string const & title
	, bool const & showItemDetail
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << infoString(title);
	}
	size_t const numPnts{ pntCapacity() };
	for (size_t pntNdx{0u} ; pntNdx < numPnts ; ++pntNdx)
	{
		std::vector<cam::AcqNdx> const acqNdxs{ acqIndicesFor(pntNdx) };
		size_t const numAcqs{ acqNdxs.size() };
		oss << std::endl;
		oss
			<< "PntNdx,numAcqs:"
			<< " " << dat::infoString(pntNdx)
			<< " " << dat::infoString(numAcqs)
			;
		::io::LinePrefixer prefixer("...Acqs");
		for (cam::AcqNdx const acqNdx : acqNdxs)
		{
			DatType const & item = theItemGrid(pntNdx, acqNdx);
			if (dat::isValid(item))
			{
				oss << prefixer.nextFieldString(acqNdx);
				if (showItemDetail)
				{
					oss << " " << dat::infoString(item);
				}
			}
		}
	}
	return oss.str();
}

template <typename DatType>
inline
std::string
XRefBase<DatType> :: infoStringAcqMajor
	( std::string const & title
	, bool const & showItemDetail
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << infoString(title);
	}
	size_t const numAcqs{ acqCapacity() };
	for (size_t acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		std::vector<cam::PntNdx> const pntNdxs{ pntIndicesFor(acqNdx) };
		size_t const numPnts{ pntNdxs.size() };
		oss << std::endl;
		oss
			<< "AcqNdx,numPnts:"
			<< " " << dat::infoString(acqNdx)
			<< " " << dat::infoString(numPnts)
			;
		::io::LinePrefixer prefixer("...Pnts");
		for (cam::PntNdx const pntNdx : pntNdxs)
		{
			DatType const & item = theItemGrid(pntNdx, acqNdx);
			if (dat::isValid(item))
			{
				oss << prefixer.nextFieldString(pntNdx);
				if (showItemDetail)
				{
					oss << " " << dat::infoString(item);
				}
			}
		}
	}
	return oss.str();
}

template <typename DatType>
inline
dat::grid<uint8_t>
XRefBase<DatType> :: infoImage
	( uint8_t const onValue
	) const
{
	dat::grid<uint8_t> image(pntCapacity(), acqCapacity());
	std::fill(image.begin(), image.end(), 0u);
	for (const_iterator iter(begin()) ; iter ; ++iter)
	{
		PAItem const pai(iter.paItem());
		PntNdx const & pntNdx = pai.thePntNdx;
		PntNdx const & acqNdx = pai.theAcqNdx;
		assert(dat::isValid(pai.theItem));
		image(pntNdx, acqNdx) = onValue;
	}
	return image;
}



} // cam

