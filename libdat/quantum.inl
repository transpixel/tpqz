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
\brief Inline definitions for dat::quantum
*/


namespace dat
{

namespace quantum
{

namespace priv
{
	//! For float types, base/residual values no corrections needed
	template
		< typename BaseType, typename FracType
		, EnableIf< std::is_floating_point<FracType> >...
		>
	inline
	void
	adjustBaseFrac
		( BaseType & // base
		, FracType & // resid
		, FracType const & // delta
		)
	{
		return;
	}

	//! For integer types, correct base/residual values
	template
		< typename BaseType, typename FracType
		, EnableIf< std::is_integral<FracType> >...
		>
	inline
	void
	adjustBaseFrac
		( BaseType & base
		, FracType & resid
		, FracType const & delta
		)
	{
		// required for integer values
		if (resid < FracType{ 0 })
		{
			base -= BaseType{ 1 };
			resid = resid + delta;
		}
	}
}


template <typename BaseType, typename FracType>
inline
std::pair<BaseType, FracType>
Splitter<BaseType, FracType> :: operator()
	( FracType const & value
	) const
{
	/*
	FracType const ratio{ value / theDelta };
	BaseType lBase{ static_cast<BaseType>(std::floor(ratio)) };
	FracType const dBase{ static_cast<FracType>(lBase) };
	FracType const start{ dBase*theDelta };
	FracType resid{ value - start };
	io::out() << dat::infoString(value, "value") << std::endl;
	io::out() << dat::infoString(theDelta, "theDelta") << std::endl;
	io::out() << dat::infoString(ratio, "ratio") << std::endl;
	io::out() << dat::infoString(lBase, "lBase") << std::endl;
	io::out() << dat::infoString(dBase, "dBase") << std::endl;
	io::out() << dat::infoString(start, "start") << std::endl;
	io::out() << dat::infoString(resid, "resid") << std::endl;
	*/

	BaseType lBase
		{ static_cast<BaseType>(std::floor(value / theDelta)) };
	FracType resid
		{ value - (static_cast<FracType>(lBase))*theDelta };

	// adjustment required for integer values
	// (since division rounds toward zero)
	priv::adjustBaseFrac<BaseType, FracType>(lBase, resid, theDelta);

	return { lBase, resid };
}


} // quantum

} // dat

