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
		mean = (1./(double)numSamps) * sum;
	}
	return mean;
}

template <typename FwdIter, typename DataType = double>
inline
DataType
geometric
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	DataType mean(dat::nullValue<DataType>());
	if (beg != end)
	{
		double sumLogs{ 0. };
		double count{ 0. };
		for (FwdIter iter{beg} ; end != iter ; ++iter)
		{
			double const & value = *iter;
			if (! (0. < value))
			{
				count = 0u;
				break;
			}
			sumLogs += std::log(value);
			count += 1.;
		}
		if (0. < count)
		{
			double const aveLog{ (1./count) * sumLogs };
			mean = std::exp(aveLog);
		}
	}
	return mean;
}

template <typename FwdIter, typename DataType = double>
inline
DataType
harmonic
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	DataType mean(dat::nullValue<DataType>());
	if (beg != end)
	{
		double sumInvs{ 0. };
		double count{ 0. };
		for (FwdIter iter{beg} ; end != iter ; ++iter)
		{
			double const & value = *iter;
			if (! (0. < value))
			{
				count = 0u;
				break;
			}
			sumInvs += (1. / value);
			count += 1.;
		}
		if (0. < count)
		{
			if (0. < sumInvs)
			{
				mean = count / sumInvs;
			}
		}
	}
	return mean;
}

} // mean

} // prob

