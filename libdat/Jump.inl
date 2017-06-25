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
\brief Definitions for dat::Jump
*/


namespace dat
{
//======================================================================

template <typename DataType, typename FwdIter>
inline
// static
std::vector<Jump>
Jump :: fromData
	( FwdIter const & dataBeg
	, FwdIter const & dataEnd
	, DataType const & tol
	)
{
	std::vector<Jump> jumps;

	size_t const numIn((size_t)std::distance(dataBeg, dataEnd));
	jumps.reserve(numIn);
	if (dataEnd != dataBeg)
	{
		FwdIter iter(dataBeg);
		size_t ndx(0u);
		DataType prevVal(*iter++);
		while (dataEnd != iter)
		{
			DataType const & currVal = *iter;
			if (! dat::nearlyEquals(currVal, prevVal, tol))
			{
				Jump const jump(ndx, prevVal, currVal);
				jumps.push_back(jump);
				prevVal = currVal;
			}
			++ndx;
			++iter;
		}
	}

	return jumps;
}

inline
bool
Jump :: isValid
	() const
{
	return
		(  dat::isValid(theNdx)
		&& dat::isValid(theLo)
		&& dat::isValid(theHi)
		);
}

inline
double
Jump :: mean
	() const
{
	return (.5 * (theLo + theHi));
}

//======================================================================
}


