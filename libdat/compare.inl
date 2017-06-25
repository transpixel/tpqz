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
\brief Inline definitions for dat::compare
*/


#include <iostream>


namespace dat
{
// ======================================================================


template <typename Type>
inline
bool
nearlyEquals
	( Type const & valA
	, Type const & valB
	, Type const & tol
	)
{
	bool same(false);
	bool const okayA(isValid(valA));
	bool const okayB(isValid(valB));
	// require both to be valid
	if (okayA && okayB)
	{
		Type diff{};
		if (valA < valB)
		{
			diff = valB - valA;
		}
		else
		if (valB < valA)
		{
			diff = valA - valB;
		}
		// else exactly the same
		same = (diff <= tol);
	}
	return same;
}

template <typename Type, typename FwdIter1, typename FwdIter2>
inline
bool
nearlyEquals
	( FwdIter1 const & beg1
	, FwdIter1 const & end1
	, FwdIter2 const & beg2
	, FwdIter2 const & end2
	, Type const & tol
	)
{
	size_t const num1(std::distance(beg1, end1));
	size_t const num2(std::distance(beg2, end2));

	bool same(num1 == num2);
	if (same)
	{
		FwdIter1 it1(beg1);
		FwdIter2 it2(beg2);
		while (end1 != it1)
		{
			same &= nearlyEquals<Type>(*it1++, *it2++, tol);
			if (! same)
			{
				break;
			}
		}
	}
	return same;
}

/*
template <typename TolType, typename FwdIter1, typename FwdIter2>
inline
bool
nearlyEqualsCompound
	( FwdIter1 const & beg1
	, FwdIter1 const & end1
	, FwdIter2 const & beg2
	, FwdIter2 const & end2
	, TolType const & tol = smallValue<TolType>()
	)
{
	size_t const num1(std::distance(beg1, end1));
	size_t const num2(std::distance(beg2, end2));

	bool same(num1 == num2);
	if (same)
	{
		FwdIter1 it1(beg1);
		FwdIter2 it2(beg2);
		for ( ; end1 != it1 ; ++it1, ++it2)
		{
			same &= it1->nearlyEquals(*it2, tol);
			if (! same)
			{
				break;
			}
		}
	}
	return same;
}
*/

template <typename DataType, typename TolType>
inline
bool
nearlyEquals
	( std::vector<DataType> const & d1
	, std::vector<DataType> const & d2
	, TolType const & tol
	)
{
	// problem with some version(s?) of gcc ?
//	return nearlyEqualsCompound
//		(d1.begin(), d1.end(), d2.begin(), d2.end(), tol);

	using FwdIter = typename std::vector<DataType>::const_iterator;
	FwdIter const beg1{ d1.begin() };
	FwdIter const end1{ d1.end() };
	FwdIter const beg2{ d2.begin() };
	FwdIter const end2{ d2.end() };
	size_t const num1(std::distance(beg1, end1));
	size_t const num2(std::distance(beg2, end2));

	bool same(num1 == num2);
	if (same)
	{
		FwdIter it1(beg1);
		FwdIter it2(beg2);
		for ( ; end1 != it1 ; ++it1, ++it2)
		{
			same &= it1->nearlyEquals(*it2, tol);
			if (! same)
			{
				break;
			}
		}
	}
	return same;
}

template <typename Type, size_t Dim>
inline
bool
nearlyEquals
	( std::array<Type, Dim> const & testValue
	, std::array<Type, Dim> const & refValue
	, Type const & tol
	)
{
	return nearlyEquals<Type>
		( testValue.begin(), testValue.end()
		, refValue.begin(), refValue.end()
		, tol
		);
}

template <typename Type1, typename Type2>
inline
bool
nearlyEquals
	( std::pair<Type1, Type2> const & pairA
	, std::pair<Type1, Type2> const & pairB
	, Type1 const & tol1
	, Type2 const & tol2
	)
{
	return
		(  nearlyEquals(pairA.first, pairB.first, tol1)
		&& nearlyEquals(pairA.second, pairB.second, tol2)
		);
}

template <typename Type>
inline
bool
nearlyEquals
	( std::complex<Type> const & valA
	, std::complex<Type> const & valB
	, Type const & tol
	)
{
	return
		(  dat::nearlyEquals(valA.real(), valB.real(), tol)
		&& dat::nearlyEquals(valA.imag(), valB.imag(), tol)
		);
}

template <typename ArgType, typename DataType>
inline
bool
areSame
	( ArgType const & itemA
	, ArgType const & itemB
	)
{
	bool same{ false };
	bool const okayA{ dat::isValid<ArgType>(itemA) };
	bool const okayB{ dat::isValid<ArgType>(itemB) };
	if (okayA)
	{
		if (okayB)
		{
			// both are valid - so test for answer
			same = dat::nearlyEquals<DataType>(itemA, itemB);
		}
		// else A valid, B not - so different
	}
	else
	{
		if (! okayB)
		{
			// both are invalid - so "same"
			same = true;
		}
		// else A not valid, B valid - so different
	}
	return same;
}

template <typename Type>
inline
bool
areSame
	( Type const & itemA
	, Type const & itemB
	)
{
	return areSame<Type, Type>(itemA, itemB);
}

template <typename DatType, size_t Dim>
inline
bool
areSame
	( std::array<DatType, Dim> const & itemA
	, std::array<DatType, Dim> const & itemB
	)
{
	using ArgType = std::array<DatType, Dim>;
	return areSame<ArgType, DatType>(itemA, itemB);
}

template <typename Type>
inline
bool
areSameNoSmall
	( Type const & itemA
	, Type const & itemB
	)
{
	bool same{ false };
	bool const okayA{ dat::isValid(itemA) };
	bool const okayB{ dat::isValid(itemB) };
	if (okayA)
	{
		if (okayB)
		{
			// both are valid - so test for answer
			same = itemA.nearlyEquals(itemB);
		}
		// else A valid, B not - so different
	}
	else
	{
		if (! okayB)
		{
			// both are invalid - so "same"
			same = true;
		}
		// else A not valid, B valid - so different
	}
	return same;
}

template <typename IType, EnableIf< std::is_integral<IType> >...>
inline
bool
isEven
	( IType const & value
	)
{
	return (0u == (value % 2));
}

template <typename IType, EnableIf< std::is_integral<IType> >...>
inline
bool
isOdd
	( IType const & value
	)
{
	return (! isEven<IType>(value));
}

template <typename IType, EnableIf< std::is_integral<IType> >...>
inline
bool
isSameParity
	( IType const & value1
	, IType const & value2
	)
{
	bool const bothEven{ isEven(value1) && isEven(value2) };
	bool const bothOdd{ isOdd(value1) && isOdd(value2) };
	return (bothEven || bothOdd);
}

template <typename UType, DisableIf< std::is_signed<UType> >...>
inline
bool
canSubtract
	( UType const & big
	, UType const & small
	)
{
	return (small <= big);
}

template <typename Type>
inline
bool
isLess
	( Type const & value1
	, Type const & value2
	)
{
	return (value1 < value2);
}

template <typename Type>
inline
bool
isNotLess
	( Type const & value1
	, Type const & value2
	)
{
	return (! isLess(value1, value2));
}

// ======================================================================
}

