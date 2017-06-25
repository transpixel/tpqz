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
\brief Inline definitions for dat::Latice
*/


namespace dat
{

template <typename Type>
// explicit
Latice<Type> :: Latice
	( size_t const & high
	, size_t const & wide
	, size_t const & deep
	)
	: theHigh{ high }
	, theWide{ wide }
	, theDeep{ deep }
	, theData(theHigh * theWide * theDeep)
	, theLaySize{ theHigh * theWide }
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

template <typename Type>
inline
bool
Latice<Type> :: isValid
	() const
{
	return (! theData.empty());
}

template <typename Type>
inline
size_t
Latice<Type> :: size
	() const
{
	return (theLaySize * theDeep);
}

template <typename Type>
inline
Type const &
Latice<Type> :: operator()
	( size_t const & row
	, size_t const & col
	, size_t const & lay
	) const
{
	return theData[offsetFor(row, col, lay)];
}

template <typename Type>
inline
Type &
Latice<Type> :: operator()
	( size_t const & row
	, size_t const & col
	, size_t const & lay
	)
{
	return theData[offsetFor(row, col, lay)];
}

template <typename Type>
inline
typename Latice<Type>::const_iterator
Latice<Type> :: begin
	() const
{
	return theData.begin();
}

template <typename Type>
inline
typename Latice<Type>::const_iterator
Latice<Type> :: end
	() const
{
	return theData.end();
}

template <typename Type>
inline
typename Latice<Type>::iterator
Latice<Type> :: begin
	()
{
	return theData.begin();
}

template <typename Type>
inline
typename Latice<Type>::iterator
Latice<Type> :: end
	()
{
	return theData.end();
}

template <typename Type>
inline
std::string
Latice<Type> :: infoString
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
		oss << "high,wide,deep:"
			<< " " << dat::infoString(theHigh)
			<< " " << dat::infoString(theWide)
			<< " " << dat::infoString(theDeep)
			;
		oss << std::endl;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

template <typename Type>
inline
size_t
Latice<Type> :: offsetFor
	( size_t const & row
	, size_t const & col
	, size_t const & lay
	) const
{
	return ((lay*theLaySize) + row*theWide + col);
}

}

