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
\brief Definitions for dat::layout
*/


#include "libdat/layout.h"

#include "libdat/compare.h"

#include <cassert>
#include <cmath>

#include "libio/stream.h" // debug
#include "libdat/info.h" // debug


namespace
{
	//! Generate values with spacing dx starting at x0
	std::vector<double>
	valuesForDelta
		( size_t const & numValues
		, double const & dx
		, double const & x0
		)
	{
		std::vector<double> values;
		if (dat::isValid(dx))
		{
			values.reserve(numValues);
			for (size_t nn{0u} ; nn < numValues ; ++nn)
			{
				double const xx{ x0 + double(nn)*dx };
				values.emplace_back(xx);
			}
		}
		return values;
	}

	//! Generate (truncted integer) indices with spacing dx starting at x0
	std::vector<size_t>
	indicesForDelta
		( size_t const & numValues
		, double const & dx
		, double const & x0
		)
	{
		std::vector<size_t> indices;
		if (dat::isValid(dx))
		{
			indices.reserve(numValues);
			for (size_t nn{0u} ; nn < numValues ; ++nn)
			{
				double const xx{ x0 + double(nn)*dx };
				size_t const ndx{ static_cast<size_t>(std::floor(xx)) };
				indices.emplace_back(ndx);
			}
		}
		return indices;
	}

	//! Form outer combination of xx and yy values
	template <typename AType, typename ElemType> // an array of two
	std::vector<AType>
	arraysFrom
		( std::vector<ElemType> const & aas
		, std::vector<ElemType> const & bbs
		)
	{
		std::vector<AType> arrays;
		arrays.reserve(aas.size() * bbs.size());
		// form combinations
		for (ElemType const & aa : aas)
		{
			for (ElemType const & bb : bbs)
			{
				arrays.emplace_back(AType{{ aa, bb }});
			}
		}
		return arrays;
	}

	//! Form outer combination of row and column values
	std::vector<dat::RowCol>
	rowcolsFrom
		( std::vector<size_t> const & rows
		, std::vector<size_t> const & cols
		)
	{
		return arraysFrom<dat::RowCol>(rows, cols);
	}

	//! Form outer combination of xx and yy values
	std::vector<dat::Spot>
	spotsFrom
		( std::vector<double> const & xxs
		, std::vector<double> const & yys
		)
	{
		return arraysFrom<dat::Spot>(xxs, yys);
	}

	//! spacing for centered values
	double
	centeredDelta
		( size_t const & max
		, size_t const & numValues
		)
	{
		double result{ dat::nullValue<double>() };
		if (numValues <= max)
		{
			result = double(max) / double(numValues);
		}
		return result;
	}

	//! start location for centered values
	double
	centeredStart
		( double const & del
		)
	{
		double result{ dat::nullValue<double>() };
		if (dat::isValid(del))
		{
			result = .5 * del;
		}
		return result;
	}

	//! spacing for expanded values
	double
	expandedDelta
		( double const & max
		, size_t const & numValues
		)
	{
		double result{ dat::nullValue<double>() };
		if (0u < numValues)
		{
			result = double(max) / double(numValues - 1u);
		}
		return result;
	}

	//! start location for expanded values
	double
	expandedStart
		( double const & del
		)
	{
		double result{ dat::nullValue<double>() };
		if (dat::isValid(del))
		{
			result = 0.;
		}
		return result;
	}
}


//======================================================================
//======================================================================
//======================================================================


namespace dat
{
namespace layout
{

//======================================================================
namespace centered
{

	std::vector<size_t>
	indices
		( size_t const & max
		, size_t const & numValues
		)
	{
		double const dx{ centeredDelta(max, numValues) };
		double const x0{ centeredStart(dx) };
		return indicesForDelta(numValues, dx, x0);
	}

	std::vector<dat::RowCol>
	rowcols
		( dat::Extents const & domain
		, dat::Extents const & hwSize
		)
	{
		// get indices in each direction
		std::vector<size_t> const rows(indices(domain[0], hwSize[0]));
		std::vector<size_t> const cols(indices(domain[1], hwSize[1]));
		// return combinations
		return rowcolsFrom(rows, cols);
	}

} // centered

namespace
{
	std::vector<double>
	shifted
		( std::vector<double> const & inVals
		, double const & offset
		)
	{
		std::vector<double> outVals;
		outVals.reserve(inVals.size());
		for (double const & inVal : inVals)
		{
			outVals.emplace_back(inVal + offset);
		}
		return outVals;
	}
}

//======================================================================
namespace expanded
{
	std::vector<double>
	values
		( size_t const & max
		, size_t const & numValues
		)
	{
		double const dx{ expandedDelta(max, numValues) };
		double const x0{ expandedStart(dx) };
		return valuesForDelta(numValues, dx, x0);
	}

	std::vector<dat::Spot>
	spots
		( dat::Area<double> const & area
		, dat::Extents const & hwSize
		)
	{
		// get indices in each direction
		std::vector<double> const x0s{values(area[0].magnitude(), hwSize[0])};
		std::vector<double> const y0s{values(area[1].magnitude(), hwSize[1])};

		std::vector<double> const xxs{ shifted(x0s, area[0].min()) };
		std::vector<double> const yys{ shifted(y0s, area[1].min()) };

		// return combinations
		return spotsFrom(xxs, yys);
	}

	std::vector<size_t>
	indices
		( size_t const & max
		, size_t const & numValues
		)
	{
		std::vector<size_t> indices;
		if (double(numValues) <= max)
		{
			double const dx{ expandedDelta(max, numValues) };
			double const x0{ expandedStart(dx) };
			indices = indicesForDelta(numValues - 1u, dx, x0);
		}
		return indices;
	}

	std::vector<dat::RowCol>
	rowcols
		( dat::Extents const & domain
		, dat::Extents const & hwSize
		)
	{
		// get indices in each direction
		std::vector<size_t> const rows(indices(domain[0], hwSize[0]));
		std::vector<size_t> const cols(indices(domain[1], hwSize[1]));
		// return combinations
		return rowcolsFrom(rows, cols);
	}


} // extreme

} // layout
} // dat

