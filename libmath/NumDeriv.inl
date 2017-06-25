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
\brief Inline declarations for math::NumDeriv
*/


namespace math
{

template <typename OutType>
inline
NumDeriv<OutType> :: NumDeriv
	( double const & eps
	)
	: theDelta{ std::sqrt(eps) }
	, theInvRun{ 1. / (2.*theDelta) }
{ }

template <typename OutType>
inline
bool
NumDeriv<OutType> :: isValid
	() const
{
	return
		(  dat::isValid(theDelta)
		&& dat::isValid(theInvRun)
		);
}

template <typename OutType>
template <typename ArgType, typename Func>
inline
OutType
NumDeriv<OutType> :: derivative
	( ArgType const & val
	, ArgType const & del
	, Func const & func
	) const
{
	OutType const pos{ func(val + del) };
	OutType const neg{ func(val - del) };
	OutType const rise{ pos - neg };
	return (theInvRun * rise);
}

template <typename OutType>
inline
double
NumDeriv<OutType> :: tolerance
	() const
{
	return theDelta;
}

template <typename OutType>
template <typename ArgType>
inline
ArgType
NumDeriv<OutType> :: deltaInDirOf
	( ArgType const & dir
	) const
{
	return (theDelta * dir);
}

template <typename OutType>
template <typename ArgType>
inline
std::vector<ArgType>
NumDeriv<OutType> :: deltasFor
	( std::vector<ArgType> const & dirs
	) const
{
	std::vector<ArgType> deltas;
	deltas.reserve(dirs.size());
	for (ArgType const & dir : dirs)
	{
		deltas.emplace_back(deltaInDirOf<ArgType>(dir));
	}
	return deltas;
}

template <typename OutType>
inline
std::string
NumDeriv<OutType> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // math

