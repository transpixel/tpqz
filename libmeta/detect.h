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

#ifndef meta_detect_INCL_
#define meta_detect_INCL_

/*! \file
\brief Declarations for meta::detect
*/


#include <type_traits>
//#include <typeinfo>
//#include <cstddef>


namespace meta
{

/*! \brief C++ idioms for member type/function detection.

\par Example
\dontinclude testmeta/udetect.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace detect
{
	//! Constructs with 'isFound'==true if member func is present in Type
	template <typename Type>
	class HasNull
	{
		//! Base class that [also|only] has same member name
		struct FakeBase { int null; };

		//! Has either one member (FakeBase) or ambiguous dual definitions
		struct Derived : Type, FakeBase { };

		//! Instantiatable only if second arg of type defined by first arg
		template <typename Dummy, Dummy> struct Check; // else SFINAE fail

		//! Intermediate data to test which func() is instantiated
		static constexpr unsigned long const onlySize{ 1u };
		static constexpr unsigned long const alsoSize{ 2u };
		using OnlyInBase = char[onlySize];
		using AlsoInType = char[alsoSize];

		/*! Resolves if method/func is unique (in FakeBase only)
		 *
		 * First argument to check template declares a function pointer
		 * (to member of FakeBase), second argument attempts to provide
		 * value associated with this pointer
		 */
		template <typename Dummy>
		static
		OnlyInBase &
		func
			( Check<int FakeBase:: *, &Dummy::null> const * ptCheck
			);

		//! Resolves if method/func is ambiguous (in Type and FakeBase)
		template <typename Dummy>
		static
		AlsoInType &
		func
			( ...
			);

	public:

		//! Value is set to true/false based on which func() can be resolved
		static constexpr bool const isFound
			{ (alsoSize == sizeof(func<Derived>(0))) };
	};

} // detect

} // meta

// Inline definitions
// #include "libmeta/detect.inl"

#endif // meta_detect_INCL_

