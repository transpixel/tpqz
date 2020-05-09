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

#ifndef dat_validity_INCL_
#define dat_validity_INCL_

/*! \file
\brief Declarations for dat::validity
*/


#include "libdat/template.h"
#include "libdat/types.h"

#include <cmath>
#include <functional>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>


namespace dat
{
// functions to manage data validity

	//! Functions to test if type has specific member functions
	namespace has
	{
		//! Produce meaningful error - for strange attempted uses (?)
		template <typename, typename Type>
		struct null
		{
			// scream about error
			static_assert
				( std::integral_constant<Type, false>::value
				, "Second template parameter needs to be of function type."
				);
		};

		//! Check if class has function with correct signature and return type
		template<typename ClassType, typename RetType, typename... FuncArgs>
		struct null<ClassType, RetType(FuncArgs...)>
		{
		private:

			//! check return type from "attempted" call function
			template<typename TmpType>
			static constexpr auto
			check
				(TmpType*) -> typename std::is_same
					< decltype
						( std::declval<TmpType>()
							.null( std::declval<FuncArgs>()... )
						)
					, RetType
					>::type;

			//! cover cases without function
			template<typename>
			static constexpr std::false_type
			check
				(...);

			//! provide 'type' consistent with type_traits
			typedef decltype(check<ClassType>(0)) type;

		public:

			static constexpr bool value = type::value;
		};
	}

	//! Fall-through template - call member function
	template
		< typename CType
		, EnableIf
			< dat::has::null<CType, CType()> >...
		>
	inline
	CType
	nullValue
		()
	{
		return CType::null();
	}

	//
	// Data validity
	//

	template <typename FType, EnableIf< std::is_floating_point<FType> >...>
	constexpr //inline
	FType
	nullValue
		()
	{
		return std::numeric_limits<FType>::quiet_NaN();
	}

	template <typename IType, EnableIf< std::is_integral<IType> >...>
	constexpr //inline
	IType
	nullValue
		()
	{
		// what else to do for integral types?
		return std::numeric_limits<IType>::max();
	}

	template <typename FType, EnableIf< std::is_same<FType, f16_t> >...>
	constexpr //inline
	FType
	nullValue
		()
	{
		return std::numeric_limits<FType>::quiet_NaN();
	}

	template <typename AType, EnableIf< std::is_same<AType, RowCol> >...>
	constexpr //inline
	AType
	nullValue
		()
	{
		return RowCol
			{ std::numeric_limits<RowCol::value_type>::max()
			, std::numeric_limits<RowCol::value_type>::max()
			};
	}

	template <typename AType, EnableIf< std::is_same<AType, Spot> >...>
	constexpr //inline
	AType
	nullValue
		()
	{
		return Spot
			{ std::numeric_limits<Spot::value_type>::quiet_NaN()
			, std::numeric_limits<Spot::value_type>::quiet_NaN()
			};
	}


	//! Invalid float
	constexpr float badFlt
		{ std::numeric_limits<float>::quiet_NaN() };

	//! Invalid double
	constexpr double badDub
		{ std::numeric_limits<double>::quiet_NaN() };

	//! Simple functor returning null value
	template <typename OutType, typename InType>
	constexpr //inline
	std::function<OutType(InType const &)>
	nullFunc
		()
	{
		return ( [] (InType const &) { return nullValue<OutType>(); } );
	}

	//! Always true - (for compatibliity with other templates)
	constexpr //inline
	bool
	isValid
		( bool const & // value
		)
	{
		return true;
	}

	//! True if value is not one of (subnormal, infinite, NaN) zero is okay
	template <typename FType, EnableIf< std::is_floating_point<FType> >...>
	inline
	bool
	isValid
		( FType const & value
		);

	//! True if value is not one of (max possible)
	template <typename IType, EnableIf< std::is_integral<IType> >...>
	inline
	bool
	isValid
		( IType const & value
		);

	//! True if both members are valid
	template <typename Type1, typename Type2>
	inline
	bool
	isValid
		( std::pair<Type1, Type2> const & apair
		);

	//! Specialization of above (shouldn't be needed ?)
	inline
	bool
	isValid
		( f16_t const & value
		);

	//! Specialization
	inline
	bool
	isValid
		( std::string const & value
		);

	//! Functions to test if type has specific member functions
	namespace has
	{
		//! Produce meaningful error - for strange attempted uses (?)
		template <typename, typename Type>
		struct isValid
		{
			// scream about error
			static_assert
				( std::integral_constant<Type, false>::value
				, "Second template parameter needs to be of function type."
				);
		};

		//! Check if class has function with correct signature and return type
		template<typename ClassType, typename RetType, typename... FuncArgs>
		struct isValid<ClassType, RetType(FuncArgs...)>
		{
		private:

			//! check return type from "attempted" call function
			template<typename TmpType>
			static constexpr auto
			check
				(TmpType*) -> typename std::is_same
					< decltype
						( std::declval<TmpType>()
							.isValid( std::declval<FuncArgs>()... )
						)
					, RetType
					>::type;

			//! cover cases without function
			template<typename>
			static constexpr std::false_type
			check
				(...);

			//! provide 'type' consistent with type_traits
			typedef decltype(check<ClassType>(0)) type;

		public:

			static constexpr bool value = type::value;
		};
	}

	//! Fall-through template - call member function
	template
		< typename CType
		, EnableIf
			< dat::has::isValid<CType, bool()> >...
		>
	inline
	bool
	isValid
		( CType const & arg
		)
	{
		return arg.isValid();
	}

	/* TODO - has problems with e.g. Type==dat::Spot
	//! Functor for testing validity
	template <typename Type>
	struct IsValid
	{
		// True if isValid(item)
		inline
		bool
		operator()
			( Type const & item
			) const
		{
			return dat::isValid<Type>(item);
		}
	};
	*/

}

// Inline definitions
#include "libdat/validity.inl"

#endif // dat_validity_INCL_

