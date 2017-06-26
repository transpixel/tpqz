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
\brief Inline definition for ro::ops
*/


namespace ro
{

inline
std::pair<ga::Pose, ga::Pose>
attitudesOf
	( std::pair<ga::Rigid, ga::Rigid> const & oriPair
	)
{
	return { oriPair.first.pose(), oriPair.second.pose() };
}

inline
std::pair<ga::Rigid, ga::Rigid>
orientationsFrom
	( LocPair const & locPair
	, std::pair<ga::Pose, ga::Pose> const & attPair
	)
{
	return
		{ ga::Rigid(locPair.first, attPair.first)
		, ga::Rigid(locPair.second, attPair.second)
		};
}

template <typename Type>
inline
std::pair<Type, Type>
inverseOf
	( std::pair<Type, Type> const & pairBwA
	)
{
	return { pairBwA.first.inverse(), pairBwA.second.inverse() };
}

template <typename Type>
inline
std::pair<Type, Type>
compositeOf
	( std::pair<Type, Type> const & pairCwB
	, std::pair<Type, Type> const & pairBwA
	)
{
	return
		{ pairCwB.first*pairBwA.first, pairCwB.second*pairBwA.second };
}

template <typename Type>
inline
std::pair<Type, Type>
compositeOf
	( Type const & attCwB
	, std::pair<Type, Type> const & pairBwA
	)
{
	return { attCwB*pairBwA.first, attCwB*pairBwA.second };
}

template <typename Type>
inline
std::pair<Type, Type>
compositeOf
	( std::pair<Type, Type> const & pairCwB
	, Type const & attBwA
	)
{
	return { pairCwB.first*attBwA, pairCwB.second*attBwA };
}

template <typename Type>
inline
Type
relative2w1
	( std::pair<Type, Type> const & pair
	)
{
	Type const & x1w0 = pair.first;
	Type const & x2w0 = pair.second;
	Type const x0w1{ x1w0.inverse() };
	Type const x2w1{ x2w0 * x0w1 };
	return x2w1;
}

inline
ga::Vector
vectorBetween
	( std::pair<ga::Rigid, ga::Rigid> const & roPair
	)
{
	return { roPair.second.location() - roPair.first.location() };
}

inline
ga::Vector
directionBetween
	( std::pair<ga::Rigid, ga::Rigid> const & roPair
	)
{
	return ga::unit(vectorBetween(roPair));
}

inline
double
gapBetween
	( std::pair<ga::Rigid, ga::Rigid> const & roPair
	)
{
	return ga::magnitude(vectorBetween(roPair));
}

inline
ga::Vector
midPointOf
	( std::pair<ga::Rigid, ga::Rigid> const & roPair
	)
{
	return { .5*(roPair.first.location() + roPair.second.location()) };
}

inline
ga::Rigid
rigidModelWrtRef
	( ga::Rigid const & oriAnyWrtMod
	, ga::Rigid const & oriAnyWrtRef
	)
{
	// compute overall model orientation
	ga::Rigid const oriModWrtAny{ oriAnyWrtMod.inverse() };
	ga::Rigid const oriModWrtRef{ oriModWrtAny * oriAnyWrtRef };
	return oriModWrtRef;
}

template <typename Type>
inline
std::string
infoString
	( std::pair<Type, Type> const & aPair
	, std::string const & title
	)
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	oss << dat::infoString(aPair.first) << std::endl;
	oss << dat::infoString(aPair.second) << std::endl;
	return oss.str();
}

template <typename Type>
inline
std::string
infoStringRelative
	( std::pair<Type, Type> const & aPair
	, std::string const & title
	)
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	ga::Rigid const ori2w1{ relative2w1(aPair) };
	oss << dat::infoString(aPair.first) << std::endl;
	oss << dat::infoString(aPair.second) << std::endl;
	oss << std::setw(15u) << "---" << std::endl;
	oss << dat::infoString(ori2w1) << std::endl;
	return oss.str();
}


} // ro

