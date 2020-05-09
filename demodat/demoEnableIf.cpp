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

#include "libio/stream.h"

#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <type_traits>



template <typename Type>
using EnableIf = typename std::enable_if<Type::value>::type;


template <typename FType, EnableIf<std::is_floating_point<FType>>...>
inline
FType
someFunc
	( FType const arg
	)
{
	return arg;
}

template <typename IType, EnableIf<std::is_integral<IType>>...>
inline
IType
someFunc
	( IType const & arg
	)
{
	return arg;
}

int
main
	()
{

	io::out() << "ld: " << typeid(someFunc<long double>(27.L)).name() << '\n';
	io::out() << " d: " << typeid(someFunc<double>(27.)).name() << '\n';
	io::out() << " f: " << typeid(someFunc<float>(27.f)).name() << '\n';
	io::out() << " i: " << typeid(someFunc<int>(27)).name() << '\n';
	io::out() << " s: " << typeid(someFunc<short>(27)).name() << '\n';
	io::out() << "u8: " << typeid(someFunc<uint8_t>(27u)).name() << '\n';

	return 0;
}

