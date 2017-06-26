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

#ifndef ro_PairBaseZ_INCL_
#define ro_PairBaseZ_INCL_

/*! \file
\brief Declarations for ro::PairBaseZ
*/


#include "libro/Pair.h"
#include "libro/ArgsBaseZ.h"


namespace ro
{

/*! \brief Relative pair with: base=ga::e3(z-axis) + phi[12],alpha,theta[12]

\par Example
\dontinclude testro/uPair.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PairBaseZ : public ro::Pair
{

	ArgsBaseZ theParms;
	double theBaseMag{ dat::nullValue<double>() };
	ga::Rigid theOri0wRef;

public: // generic methods (e.g. overrides)

	//! default null constructor
	PairBaseZ
		() = default;

	//! Construct from pair of orientations
	explicit
	PairBaseZ
		( ga::Rigid const & ori1wRef
		, ga::Rigid const & ori2wRef
		);

	//! Convenience version of above
	explicit
	PairBaseZ
		( OriPair const & oriPair
		)
		: PairBaseZ(oriPair.first, oriPair.second)
	{ }

	//! Value ctor
	explicit
	PairBaseZ
		( ArgsBaseZ const & parms
		, double const & baseMag = 1.
		, ga::Rigid const & ori0wRef = ga::Rigid::identity()
		);

	//! Check if instance is valid
	virtual
	bool
	isValid
		() const;

	//! Root-Sum-Square of parmValues()
	double
	parmRMS
		() const;

	//! Orientation of primary member
	inline
	virtual
	ga::Rigid
	rigid1w0
		() const;

	//! Orientation of secondary member
	inline
	virtual
	ga::Rigid
	rigid2w0
		() const;

	//! Orientation of model frame w.r.t. reference
	inline
	virtual
	ga::Rigid
	rigid0wRef
		() const;

public:

	//! Parameter values: {p1,p2, t1,t2, a3}
	std::array<double, 5u>
	parmValues
		() const;

	//! Jacobian: tripleProductGap() w.r.t. parmValues()
	std::array<double, 5u>
	jacobianRow
		( ga::Vector const & uDir
		, ga::Vector const & vDir
		) const;

	//! Convenience version of above
	std::array<double, 5u>
	jacobianRow
		( std::pair<ga::Vector, ga::Vector> const & uvPair
		) const;

	//! Descriptive information about this instance.
	std::string
	parmString
		( std::string const & fmt = std::string("%12.6f")
		) const;

	//! Descriptive information about this instance.
	std::string
	infoStringParms
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%12.6f")
		) const;

	// virtual

	//! Detailed descriptive information.
	virtual
	std::string
	infoStringDetail
		( std::string const & title
		) const;

private:

	//! uDir rotated into base frame: g = exp(-phi) * udir * exp(phi)
	ga::Vector
	gVec
		( ga::Vector const & uDir
		) const;

	//! vDir rotated into base frame: h = exp(-theta) * vdir * exp(theta)
	ga::Vector
	hVec
		( ga::Vector const & vDir
		) const;

	//! Change in gVec() w.r.t. change in ndx-th component of phi
	ga::Vector
	gDot
		( ga::Vector const & uDir
		, size_t const & ndxPhi
		) const;

	//! Change in hVec() w.r.t. change in ndx-th component of theta
	ga::Vector
	hDot
		( ga::Vector const & vDir
		, size_t const & ndxTheta
		) const;


}; // PairBaseZ

} // ro

// Inline definitions
#include "libro/PairBaseZ.inl"

#endif // ro_PairBaseZ_INCL_

