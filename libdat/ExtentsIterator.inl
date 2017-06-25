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
\brief Inline definitions for dat::ExtentsIterator
*/


namespace dat
{

inline
// explicit
ExtentsIterator :: ExtentsIterator
	( dat::Extents const & domainSize
	)
	: theUL{{ 0u, 0u }}
	, theDomain(domainSize)
	, theIsActive{ (! theDomain.empty()) }
{ }

inline
bool
ExtentsIterator :: isValid
	() const
{
	return theDomain.isValid();
}

inline
bool
ExtentsIterator :: isActive
	() const
{
	return (isValid() && theIsActive);
}

inline
// explicit
ExtentsIterator :: operator bool
	() const
{
	return theIsActive;
}

inline
dat::RowCol const &
ExtentsIterator :: operator*
	() const
{
	return theUL;
}

inline
ExtentsIterator &
ExtentsIterator :: operator++
	()
{
	++theUL[1];
	if (theDomain[1] <= theUL[1])
	{
		theUL[1] = 0u;
		++theUL[0];
		if (theDomain[0] <= theUL[0])
		{
			theIsActive = false;
			theUL = dat::nullValue<dat::RowCol>();
		}
	}
	return *this;
}

}

