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
\brief Inline definitions for math::Extreme
*/


#include "libio/stream.h"
#include "libdat/info.h"

#include <sstream>


namespace math
{

template <typename FwdIter>
inline
Extreme<FwdIter> :: Extreme
	()
	: theExVal{ dat::nullValue<ValueType>() }
	, theExIter{}
{
}

template <typename FwdIter>
template <typename CompFunc>
inline
// explicit
Extreme<FwdIter> :: Extreme
	( FwdIter const & beg
	, FwdIter const & end
	, CompFunc const & comp
	)
	: theExVal{ dat::nullValue<ValueType>() }
	, theExIter{ end }
{
	for (FwdIter iter{beg} ; end != iter ; ++iter)
	{
		ValueType const value(*iter);
		if (shouldUpdate(value, comp))
		{
			theExVal = value;
			theExIter = iter;
		}
	}
}

template <typename FwdIter>
inline
bool
Extreme<FwdIter> :: isValid
	() const
{
	return dat::isValid(theExVal);
}

template <typename FwdIter>
inline
std::string
Extreme<FwdIter> :: infoString
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
		oss << dat::infoString(theExVal, "theExVal");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

template <typename FwdIter>
template <typename CompFunc>
inline
bool
Extreme<FwdIter> :: shouldUpdate
	( ValueType const & elem
	, CompFunc const & comp
	)
{
	bool update{ false };
	if (dat::isValid(elem))
	{
		if (dat::isValid(theExVal))
		{
			// already initialized, perform conditional update
			update = comp(elem, theExVal);
		}
		else
		{
			// perform initialization update
			update = true;
		}
	}
	return update;
}

}

