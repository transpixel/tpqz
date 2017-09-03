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
\brief Inline definitions for dat::QuantumFrac
*/


namespace dat
{

inline
// explicit
QuantumFrac :: QuantumFrac
	( Type const & value
	, quantum::Splitter<long, Type> const & quantFunc
	)
	: theQuant(quantFunc)
{
	std::pair<long, Type> const qf{ quantFunc(value) };
	theFloor = qf.first;
	theResid = qf.second;
}

inline
// explicit
QuantumFrac :: QuantumFrac
	( long const & floor
	, Type const & resid
	, quantum::Splitter<long, Type> const & quantFunc
	)
	: theQuant(quantFunc)
	, theFloor{ floor }
	, theResid{ resid }
{
}

inline
bool
QuantumFrac :: isValid
	() const
{
	return dat::isValid(theResid);
}

inline
QuantumFrac::Type const &
QuantumFrac :: delta
	() const
{
	return theQuant.theDelta;
}

} // dat

