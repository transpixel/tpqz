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
\brief Inline definitions for math::CosTable
*/


namespace math
{
//======================================================================

template <typename Type>
inline
// static
dat::Range<double>
CosTable<Type> :: closedDomain
	()
{
	return dat::Range<double>(-1., (1. + math::eps));
}

template <typename Type>
template <typename FwdIter>
inline
// explicit
CosTable<Type> :: CosTable
	( FwdIter const & itBeg
	, FwdIter const & itEnd
	, Type const & nullValue
	)
	: theTable(itBeg, itEnd)
	, thePart(closedDomain(), theTable.size())
	, theNull{ nullValue }
{
	assert(0u == static_cast<size_t>(thePart.interpIndexFor(-1.)));
	assert(thePart.interpIndexFor( 1.) < theTable.size());
}

template <typename Type>
inline
// explicit
CosTable<Type> :: CosTable
	( size_t const & numElem
	, Type const & nullValue
	)
	: theTable(numElem, nullValue)
	, thePart(closedDomain(), theTable.size())
	, theNull{ nullValue }
{
}

template <typename Type>
inline
bool
CosTable<Type> :: isValid
	() const
{
	return thePart.isValid();
}

template <typename Type>
inline
size_t
CosTable<Type> :: size
	() const
{
	return theTable.size();
}

template <typename Type>
inline
Type const &
CosTable<Type> :: operator[]
	( size_t const & ndx
	) const
{
	if (ndx < theTable.size())
	{
		return theTable[ndx];
	}
	else
	{
		return theNull;
	}
}

template <typename Type>
inline
Type &
CosTable<Type> :: operator[]
	( size_t const & ndx
	)
{
	if (dat::isValid(ndx) && (ndx < theTable.size()))
	{
		return theTable[ndx];
	}
	else
	{
		return theNull;
	}
}

template <typename Type>
inline
size_t
CosTable<Type> :: indexForCosine
	( double const & cosValue
	) const
{
	size_t ndx(dat::nullValue<size_t>());
	if (isValid())
	{
		double const fndx(thePart.interpIndexFor(cosValue));
		if (dat::isValid(fndx))
		{
			ndx = static_cast<size_t>(fndx);
		}
	}
	return ndx;
}

template <typename Type>
inline
double
CosTable<Type> :: cosineForIndex
	( size_t const & index
	) const
{
	return thePart.interpValueFor(static_cast<double>(index));
}

template <typename Type>
inline
double
CosTable<Type> :: deltaCosine
	() const
{
	double dc{ dat::nullValue<double>() };
	if (0u < thePart.size())
	{
		double const mag{ thePart.max() - thePart.min() };
		double const den{ static_cast<double>(thePart.size() - 1u) };
		dc = mag / den;
	}
	return dc;
}

/*
template <typename Type>
inline
Type
CosTable<Type> :: operator()
	( double const & cosValue
	) const
{
	Type value(theNull);
	if (isValid())
	{
		size_t const ndx(indexForCosine(cosValue));
		if (ndx < theTable.size())
		{
			value = theTable[ndx];
		}
	}
	return value;
}
*/

template <typename Type>
inline
void
CosTable<Type> :: assignValueAt
	( Type const & tabValue
	, double const & cosValue
	)
{
	assert(-1. <= cosValue);
	assert(cosValue <= 1.);
	size_t const ndx(indexForCosine(cosValue));
	assert(dat::isValid(ndx));
	assert(ndx < theTable.size());
	theTable[ndx] = tabValue;
}

template <typename Type>
inline
std::string
CosTable<Type> :: infoString
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
		oss << dat::infoString(size(), "size()");

		oss << std::endl;
		oss << dat::infoString(theNull, "theNull");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

