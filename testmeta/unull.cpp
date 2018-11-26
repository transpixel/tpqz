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
\brief  This file contains unit test for dat::null
*/


//#include "libdat/null.h"

//#include "libdat/info.h"
//#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>

#include <limits>
#include <type_traits>
#include <typeinfo>


namespace dat // template logic (logic.h?)
{
	//! Alias template that becomes void if parameter pack is missing?
	template <typename ... Ts>
	using isVoid = void;

	//! Template with (::value == false) if second argument is void
	template <typename Type, typename = void>
	struct HasMemberType : std::false_type
	{
	};

	//! Template with (::value == true) if second argument is not void
	template <typename Type>
	struct HasMemberType
		<Type, isVoid<typename Type::memberType> > : std::true_type
	{
	};
}

namespace dat
{
	template <typename Type>
	class DetectNull
	{
		//! Base class that [also|only] has same member name
		struct FakeBase { int null; };

		//! Has either one member (FakeBase) or ambiguous dual definitions
		struct Derived : Type, FakeBase { };

		//! Instantiatable only if second arg of type defined by first arg
		template <typename Dummy, Dummy> struct Check; // else SFINAE fail

		//! Intermediate data to test which func() is instantiated
		static constexpr size_t const onlySize{ 1u };
		static constexpr size_t const alsoSize{ 2u };
		using OnlyInBase = char[onlySize];
		using AlsoInType = char[alsoSize];

		//! Resolves if 'null' method is unique (in FakeBase only)
		template <typename Dummy>
		static
		OnlyInBase &
		func
			( Check<int FakeBase::*, &Dummy::null> *
			);

		//! Resolves if 'null' method is ambiguous (in Type and FakeBase)
		template <typename Dummy>
		static
		AlsoInType &
		func
			( ...
			);

	public:

		//! Value is set to true/false based on which func() can be resolved
		static constexpr bool const value
			{ (alsoSize == sizeof(func<Derived>(0))) };
	};

}

namespace aType
{
	struct CustomWithType
	{
		using memberType = float;
	};

	struct CustomSansType
	{
	};
}

namespace aFunc
{
	struct CustomSansAll
	{
	};

	struct CustomWithNull
	{
		//! Some static function
		static
		CustomWithNull
		null
			()
		{
			return {};
		}
	};

	/*
	struct CustomWithFoo
	{
		//! Some member method
		CustomWithFoo
		foo
			() const
		{
			return {};
		}
	};
	*/
}

namespace
{

//! Check presence of type members
std::string
dat_null_test0
	()
{
	std::ostringstream oss;

//	io::out() << "type of dub: " << typeid(double).name() << std::endl;

	// POD does not have type
	bool const expInt{ false };
	bool const gotInt{ dat::HasMemberType<int>::value };
	if (! (gotInt == expInt))
	{
		oss << "Failure of HasMemberType<int> test" << std::endl;
	}

	// Custom data does note have type
	bool const expSans{ false };
	bool const gotSans{ dat::HasMemberType<aType::CustomSansType>::value };
	if (! (gotSans == expSans))
	{
		oss << "Failure of HasMemberType<Sans> test" << std::endl;
	}

	// Custom data with type defined
	bool const expWith{ true };
	bool const gotWith{ dat::HasMemberType<aType::CustomWithType>::value };
	if (! (gotWith == expWith))
	{
		oss << "Failure of HasMemberType<With> test" << std::endl;
	}

	return oss.str();
}


//! Check presence of member functions
std::string
dat_null_test1
	()
{
	std::ostringstream oss;

	bool const expSans{ false };
	bool const gotSans{ dat::DetectNull<aFunc::CustomSansAll>::value };
	if (! (gotSans == expSans))
	{
		oss << "Failure of DetectNull<Sans> test" << std::endl;
	}

	bool const expWith{ true };
	bool const gotWith{ dat::DetectNull<aFunc::CustomWithNull>::value };
	if (! (gotWith == expWith))
	{
		oss << "Failure of DetectNull<With> test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::null
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_null_test0();
	oss << dat_null_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
