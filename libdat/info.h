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

#ifndef dat_info_INCL_
#define dat_info_INCL_

/*! \file
\brief Declarations for dat::info
*/


#include "libdat/template.h"

#include <array>
#include <complex>
#include <string>
#include <utility>


namespace dat
{
// Functions providing descriptive information about 'ordinary' types

	//! Functions to test if type has specific member functions
	namespace has
	{
		//! Produce meaningful error - for strange attempted uses (?)
		template <typename, typename Type>
		struct infoString
		{
			// scream about error
			static_assert
				( std::integral_constant<Type, false>::value
				, "Second template parameter needs to be of function type."
				);
		};

		//! Check if class has function with correct signature and return type
		template<typename ClassType, typename RetType, typename... FuncArgs>
		struct infoString<ClassType, RetType(FuncArgs...)>
		{
		private:

			//! check return type from "attempted" call function
			template<typename TmpType>
			static constexpr auto
			check
				(TmpType*) -> typename std::is_same
					< decltype
						( std::declval<TmpType>()
							.infoString( std::declval<FuncArgs>()... )
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

	//! Put title to stream with consistent field width (and trailing space)
	inline
	void
	putTitle
		( std::ostream & strm
		, std::string const & title
		, size_t const & fieldWide = 15u
		);

	//! Basic formatting for a double
	template <typename FType, EnableIf< std::is_floating_point<FType> >...>
	inline
	std::string
	infoString
		( FType const & value
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 8u
		, size_t const & fieldsPrecision = 3u
		);

	//! Integer types
	template <typename IType, EnableIf< std::is_integral<IType> >...>
	inline
	std::string
	infoString
		( IType const & arg
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 8u
		);

	//! Basic (alpha) formating for bool
	inline
	std::string
	infoString
		( bool const & value
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 6u
		);

	//! Generic pointer formatting
	inline
	std::string
	infoString
		( void * const & ptValue
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 16u
		);

	//! Trivial string formating
	inline
	std::string
	infoString
		( std::string const & value
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 6u
		);

	//! Specialization for complex string formating
	template <typename FType, EnableIf< std::is_floating_point<FType> >...>
	inline
	std::string
	infoString
		( std::complex<FType> const & value
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 8u
		, size_t const & fieldsPrecision = 3u
		);

	//! Generic collection
	template <typename FwdIter>
	inline
	std::string
	infoString
		( FwdIter const & beg
		, FwdIter const & end
		, std::string const & title = std::string()
		, size_t const & nafter = 6u //!< number digits after decimal
		, size_t const & nbefore = 3u //!< number digits in front of decimal
		, size_t const & fieldsPerLine = 10u
		);

	//! Descriptive information about arg
	template <typename Type, size_t Dim>
	inline
	std::string
	infoString
		( std::array<Type, Dim> const & arg
		, std::string const & title = std::string()
		, size_t const & nafter = 6u //!< number digits after decimal
		, size_t const & nbefore = 3u //!< number digits in front of decimal
		, size_t const & fieldsPerLine = 10u
		);

	//! Fall-through template - call member function
	template
		< typename CType
		, EnableIf
			< dat::has::infoString<CType, std::string(std::string const &)> >...
		>
	std::string
	infoString
		( CType const & arg
		, std::string const & title = std::string()
		);

	//! Info for both values in a pair
	template <typename Type1, typename Type2>
	inline
	std::string
	infoString
		( std::pair<Type1, Type2> const & value
		, std::string const & title = std::string()
		, size_t const & fieldsWide = 8u
		);

}

// Inline definitions
#include "libdat/info.inl"

#endif // dat_info_INCL_

