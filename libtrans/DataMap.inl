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
\brief Inline definitions for trans::DataMap
*/


namespace trans
{
//======================================================================

template <typename FwdIter>
inline
// explicit
DataMap :: DataMap
	( FwdIter const & dataBeg
	, FwdIter const & dataEnd
	)
	: theDataValues(dataBeg, dataEnd)
	, theIndexRange(0., static_cast<double>(theDataValues.size()))
	, theDataRange(dat::Range<double>::spanning(dataBeg, dataEnd))
{
}

inline
double
DataMap :: operator()
	( double const & xUnit
	) const
{
	double outValue(dat::nullValue<double>());

	if ((0. <= xUnit) && (xUnit < 1.))
	{
		// compute (fractional) index into the array
		double const fIndex
			(math::interp::valueAtValid(xUnit, theIndexRange.pair()));

		// interpolate within data table
		double const dataValue
			(math::interp::linear<double>(fIndex, theDataValues));

		// scale output to unit range for result
		outValue
			= math::interp::fractionAtValid(dataValue, theDataRange.pair());
	}
	return outValue;
}

//======================================================================
}

