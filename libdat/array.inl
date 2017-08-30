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


/*! \file
\brief Inline definitions for dat::array
*/


namespace dat
{
//======================================================================

// Operations on std::array data type

template <typename Type, size_t Dim>
std::array<Type, Dim>
nullValue
	()
{
	return std::array<Type, Dim>{{ dat::nullValue<Type>() }};
}

template <typename AType>
AType
nullArray
	()
{
	return AType{{ dat::nullValue<typename AType::value_type>() }};
}

template <typename Type>
inline
bool
isValid
	( std::array<Type, 2u> const & array2d
	)
{
	//! TODO probably could flag only [0] element, but this is safer
	return
		(  isValid(array2d[0])
		&& isValid(array2d[1])
		);
}

template <typename Type>
inline
bool
isValid
	( std::array<Type, 3u> const & array3d
	)
{
	//! TODO probably could flag only [0] element, but this is safer
	return
		(  isValid(array3d[0])
		&& isValid(array3d[1])
		&& isValid(array3d[2])
		);
}

template <typename Type, size_t Dim>
inline
bool
isValid
	( std::array<Type, Dim> const & arrayNd
	)
{
	return dat::isValid<Type>(arrayNd[0]);
}

template <typename Type, size_t Dim>
inline
std::array<Type, Dim>
operator-
	( std::array<Type, Dim> const & vals
	)
{
	std::array<Type, Dim> result;
	std::transform
		( vals.begin(), vals.end()
		, result.begin()
		, []
			(Type const & inval)
			{ return -inval; }
		);
	return result;
}

template <typename Type, size_t Dim>
inline
Type
magSq
	( std::array<Type, Dim> const & vals
	)
{
	return std::inner_product
		(vals.begin(), vals.end(), vals.begin(), static_cast<Type>(0));
}

template <typename Type, size_t Dim>
inline
Type
magnitude
	( std::array<Type, Dim> const & vals
	)
{
	return std::sqrt(magSq(vals));
}

template
	< typename FType, size_t Dim
	, EnableIf< std::is_floating_point<FType> >...
	>
inline
std::array<FType, Dim>
unit
	( std::array<FType, Dim> const & vec
	)
{
	std::array<FType, Dim> result{{ dat::nullValue<FType>() }};
	FType const mag{ magnitude(vec) };
	if (std::numeric_limits<FType>::min() < mag)
	{
		// for non-zero arguments, normalize data
		FType const inv{ static_cast<FType>(1./mag) };
		result = inv * vec;
	}
	return result;
}

template <typename Type, size_t Dim>
inline
std::array<Type, Dim>
operator+
	( std::array<Type, Dim> const & argA
	, std::array<Type, Dim> const & argB
	)
{
	std::array<Type, Dim> result;
	std::transform
		( argA.begin(), argA.end()
		, argB.begin()
		, result.begin()
		, []
			(Type const & valA, Type const & valB)
			{ return (valA + valB); }
		);
	return result;
}

template <typename Type, size_t Dim>
inline
std::array<Type, Dim>
operator-
	( std::array<Type, Dim> const & argA
	, std::array<Type, Dim> const & argB
	)
{
	std::array<Type, Dim> result;
	std::transform
		( argA.begin(), argA.end()
		, argB.begin()
		, result.begin()
		, []
			(Type const & valA, Type const & valB)
			{ return (valA - valB); }
		);
	return result;
}

template <typename Type, size_t Dim>
inline
std::array<Type, Dim>
operator*
	( Type const & lmult
	, std::array<Type, Dim> const & arg
	)
{
	std::array<Type, Dim> result;
	std::transform
		( arg.begin(), arg.end()
		, result.begin()
		, [& lmult]
			(Type const & val)
			{ return (lmult * val); }
		);
	return result;
}

template
	< typename FType
	, size_t Dim
	, EnableIf< std::is_floating_point<FType> >...
	>
inline
std::array<FType, Dim>
reciprocals
	( std::array<FType, Dim> const & arg
	)
{
	std::array<FType, Dim> result;
	std::transform
		( arg.begin(), arg.end()
		, result.begin()
		, []
			(FType const & inval)
			{ return (1. / inval); }
		);
	return result;
}

template <typename Type, size_t Dim>
inline
Type
dot
	( std::array<Type, Dim> const & argA
	, std::array<Type, Dim> const & argB
	)
{
	return std::inner_product
		( argA.begin(), argA.end()
		, argB.begin()
		, Type{}
		);
}

//======================================================================
}

