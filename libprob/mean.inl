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
\brief Inline definitions for prob::mean
*/


namespace prob
{
namespace mean
{

template <typename FwdIter, typename DataType>
inline
DataType
arithmetic
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	DataType mean(dat::nullValue<DataType>());
	if (beg != end)
	{
		DataType const sum
			(std::accumulate(beg, end, static_cast<DataType>(0)));
		size_t const numSamps(std::distance(beg, end));
		mean = (1./static_cast<double>(numSamps)) * sum;
	}
	return mean;
}

namespace priv
{
	//! Generic sample accumulation and normalization
	template
		< typename FwdIter
		, typename FuncFwd
		, typename FuncInv
		, typename DataType = double
		>
	inline
	DataType
	meanFor
		( FwdIter const & beg
		, FwdIter const & end
		, FuncFwd const & fwdFunc
		, FuncInv const & invFunc
		)
	{
		DataType mean(dat::nullValue<DataType>());
		if (beg != end)
		{
			double sumFwd{ 0u };
			size_t count{ 0u };
			bool hitAnyZeros{ false };
			for (FwdIter iter{beg} ; end != iter ; ++iter)
			{
				double const & value = *iter;
				if (! (0. < value))
				{
					if (value < 0.)
					{
						count = 0u;
						break;
					}
					else // == 0.
					{
						hitAnyZeros = true;
					}
				}
				sumFwd += fwdFunc(value);
				++count;
			}
			if (0u < count)
			{
				if (hitAnyZeros)
				{
					mean = 0.;
				}
				else // all samples were positive
				{
					double const aveFwd
						{ (1./static_cast<double>(count)) * sumFwd };
					mean = invFunc(aveFwd);
				}
			}
		}
		return mean;
	}

	//! Functions that define the geometric mean
	namespace geo
	{
		//! Forward function is logarithm
		inline
		double
		funcFwd
			( double const & arg
			)
		{
			return std::log(arg);
		}

		//! Inverse function is exponential
		inline
		double
		funcInv
			( double const & arg
			)
		{
			return std::exp(arg);
		}
	}

	//! Functions that define the harmonic mean
	namespace har
	{
		//! Reciprocal is both forward and inverse function
		inline
		double
		funcRecip
			( double const & arg
			)
		{
			return { 1. / arg };
		}
	}

}

template <typename FwdIter, typename DataType = double>
inline
DataType
geometric
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	return priv::meanFor(beg, end, priv::geo::funcFwd, priv::geo::funcInv);
}

template <typename FwdIter, typename DataType = double>
inline
DataType
harmonic
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	return priv::meanFor(beg, end, priv::har::funcRecip, priv::har::funcRecip);
}

} // mean

} // prob

