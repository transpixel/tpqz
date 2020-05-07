//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief Inline definitions for geo::si
*/


namespace geo
{
namespace si
{

//	struct Obs
//	{

template <typename GeoType>
inline
double
Obs<GeoType> :: obsWeight
	( double const & nomRange
	) const
{
	double const sigComposite
		{ math::hypot((nomRange*theDirSigma), theStaSigma) };
	assert(math::eps < sigComposite);
	return (1. / sigComposite);
}

template <typename GeoType>
inline
std::string
Obs<GeoType> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << dat::infoString(theGeoItem, title);
	oss << std::endl;
	oss << "   Direction Uncertainty: ";
	oss << dat::infoString(theDirSigma);
	oss << std::endl;
	oss << "    Location Uncertainty: ";
	oss << dat::infoString(theStaSigma);
	return oss.str();
}

//	}; // ObsRay

template <typename Container>
inline
Dyadic
planeDyadicFor
	( Container const & dir
	)
{
	double const & d1 = dir[0];
	double const & d2 = dir[1];
	double const & d3 = dir[2];
	double const d12{ d1*d2 };
	double const d23{ d2*d3 };
	double const d31{ d3*d1 };
	return
		{ d1*d1, d12, d31
		, d12, d2*d2, d23
		, d31, d23, d3*d3
		};
}

template <typename Container>
inline
Dyadic
rayDyadicFor
	( Container const & dir
	)
{
	double const & d1 = dir[0];
	double const & d2 = dir[1];
	double const & d3 = dir[2];
	double const d12{ -d1*d2 };
	double const d23{ -d2*d3 };
	double const d31{ -d3*d1 };
	return
		{ (1.-d1*d1), d12, d31
		, d12, (1.-d2*d2), d23
		, d31, d23, (1.-d3*d3)
		};
}


namespace priv
{
	template <typename FwdIter>
	std::string
	infoMatrix
		( FwdIter const & beg
		, std::string const & title
		, size_t const & numCol = 1u
		)
	{
		std::ostringstream oss;
		FwdIter iter{beg};
		oss << title;
		for (size_t row{0u} ; row < 3u ; ++row)
		{
			oss << "\n";
			for (size_t col{0u} ; col < numCol ; ++col)
			{
				oss << dat::infoString(*iter);
				++iter;
			}
		}
		return oss.str();
	}
}

inline
void
PointSystem :: addWeightedDyadic
	( double const & weightSq
	, Dyadic const & obsDyadic
	)
{
	std::transform
		( std::begin(theNormCo), std::end(theNormCo)
		, std::begin(obsDyadic)
		, std::begin(theNormCo)
		, [&weightSq]
			(double const & norm, double const & qq)
			{ return weightSq*qq + norm; }
		);
	/*
	io::out() << priv::infoMatrix(obsDyadic.begin(), "obsDyadic", 3u) << "\n";
	io::out() << priv::infoMatrix(theNormCo.begin(), "theNormCo", 3u) << "\n";
	io::out() << std::endl;
	*/
}

inline
void
PointSystem :: addWeightedRhs
	( double const & weightSq
	, Dyadic const & obsDyadic
	, ga::Vector const & vec
	)
{
	double const & v0 = vec[0];
	double const & v1 = vec[1];
	double const & v2 = vec[2];
	// combined tensor/vec contraction and weighted accumulation
	theNormRhs[0]
		+= (weightSq * (obsDyadic[0]*v0 + obsDyadic[1]*v1 + obsDyadic[2]*v2));
	theNormRhs[1]
		+= (weightSq * (obsDyadic[3]*v0 + obsDyadic[4]*v1 + obsDyadic[5]*v2));
	theNormRhs[2]
		+= (weightSq * (obsDyadic[6]*v0 + obsDyadic[7]*v1 + obsDyadic[8]*v2));
}

inline
void
PointSystem :: addWeightedRay
	( WRay const & wray
	)
{
	double const weightSq{ math::sq(wray.first) };
	geo::Ray const & ray = wray.second;
	Dyadic const obsDyadic{ rayDyadicFor(ray.theDir) };
	addWeightedDyadic(weightSq, obsDyadic);
	addWeightedRhs(weightSq, obsDyadic, ray.theStart);
	++theNumRays;
}

inline
void
PointSystem :: addWeightedPlane
	( WPlane const & wplane
	)
{
	double const weightSq{ math::sq(wplane.first) };
	geo::Plane const & plane = wplane.second;
	Dyadic const obsDyadic{ planeDyadicFor(plane.unitNormal()) };
	addWeightedDyadic(weightSq, obsDyadic);
	addWeightedRhs(weightSq, obsDyadic, plane.origin());
	++theNumPlanes;
}

} // si

} // geo

