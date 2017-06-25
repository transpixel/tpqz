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
\brief Inline definitions for dat::IndexIterator
*/



namespace dat
{

inline
IndexIterator :: IndexIterator
	()
	: theBegEnd{ dat::nullValue<size_t>(), dat::nullValue<size_t>() }
	, theAt{ theBegEnd.second }
{ }

inline
// explicit
IndexIterator :: IndexIterator
	( IndexPair const & indexBegEnd
	)
	: theBegEnd{ indexBegEnd }
	, theAt{ theBegEnd.first }
{ }

inline
// explicit
IndexIterator :: operator bool
	() const
{
	return dat::isValid(theAt);
}

inline
size_t const &
IndexIterator :: operator*
	() const
{
	return theAt;
}

inline
IndexIterator &
IndexIterator :: operator++
	()
{
	if (*this)
	{
		++theAt;
		if (theAt == theBegEnd.second)
		{
			theAt = dat::nullValue<size_t>();
		}
	}
	return *this;
}

inline
bool
IndexIterator :: isValid
	() const
{
	return ((bool)(*this));
}

}

