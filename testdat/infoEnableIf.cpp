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

#include "libdat/types.h"

#include <cstddef>
#include <utility>
#include <array>
#include <iostream>


int
main
	()
{

using namespace std;

cout << boolalpha;

cout << endl;
cout << "--- is_integral" << endl;
cout << "  long: " << is_integral<long>::value << '\n';
cout << " float: " << is_integral<float>::value << '\n';
cout << " f16_t: " << is_integral<dat::f16_t>::value << '\n';
cout << "string: " << is_integral<string>::value << '\n';
cout << "  pair: " << is_integral<pair<size_t, size_t>>::value << '\n';
cout << "array1: " << is_integral<array<int, 3u>>::value << '\n';
cout << "array2: " << is_integral<int[3u]>::value << '\n';
cout << "  Spot: " << is_integral<dat::Spot>::value << '\n';
cout << "RowCol: " << is_integral<dat::RowCol>::value << '\n';

cout << endl;
cout << "--- is_floating_point" << endl;
cout << "  long: " << is_floating_point<long>::value << '\n';
cout << " float: " << is_floating_point<float>::value << '\n';
cout << " f16_t: " << is_floating_point<dat::f16_t>::value << '\n';
cout << "string: " << is_floating_point<string>::value << '\n';
cout << "  pair: " << is_floating_point<pair<size_t, size_t>>::value << '\n';
cout << "array1: " << is_floating_point<array<int, 3u>>::value << '\n';
cout << "array2: " << is_floating_point<int[3u]>::value << '\n';
cout << "  Spot: " << is_floating_point<dat::Spot>::value << '\n';
cout << "RowCol: " << is_floating_point<dat::RowCol>::value << '\n';

cout << endl;
cout << "--- is_array" << endl;
cout << "  long: " << is_array<long>::value << '\n';
cout << " float: " << is_array<float>::value << '\n';
cout << " f16_t: " << is_array<dat::f16_t>::value << '\n';
cout << "string: " << is_array<string>::value << '\n';
cout << "  pair: " << is_array<pair<size_t, size_t>>::value << '\n';
cout << "array1: " << is_array<array<int, 3u>>::value << '\n';
cout << "array2: " << is_array<int[3u]>::value << '\n';
cout << "  Spot: " << is_array<dat::Spot>::value << '\n';
cout << "RowCol: " << is_array<dat::RowCol>::value << '\n';

cout << endl;
cout << "--- is_class" << endl;
cout << "  long: " << is_class<long>::value << '\n';
cout << " float: " << is_class<float>::value << '\n';
cout << " f16_t: " << is_class<dat::f16_t>::value << '\n';
cout << "string: " << is_class<string>::value << '\n';
cout << "  pair: " << is_class<pair<size_t, size_t>>::value << '\n';
cout << "array1: " << is_class<array<int, 3u>>::value << '\n';
cout << "array2: " << is_class<int[3u]>::value << '\n';
cout << "  Spot: " << is_class<dat::Spot>::value << '\n';
cout << "RowCol: " << is_class<dat::RowCol>::value << '\n';

}

