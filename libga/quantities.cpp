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


/*! \file
\brief Inline definitions for ga::quantities
*/


#include "libga/quantities.h"

#include "libio/sprintf.h"

#include <iomanip>
#include <sstream>


namespace ga
{
	//! title (string) field width
	static size_t const sTW(15u);

	//! value (float) field width -- sign, before, decimal, after
	static size_t const sVP(6u);
	static size_t const sVW(1u + 4u + 1u + sVP);

	//! Configure stream for numeric output
	void
	setNumeric
		( std::ostream & oss
		)
	{
		oss << std::fixed;
		oss.width(sVW);
		oss.precision(sVP);
	}

	//! Format title component of output
	std::string
	infoTitle
		( std::string const title
		)
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << std::setw(sTW) << title << " ";
		}
		return oss.str();
	}

//======================================================================

//
//----------------------------------------------------------------------
//

std::string
Scalar :: infoString
	( std::string const & title
	, bool const & showGrade
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "s:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss << std::setw(sVW) << theValue;
	return oss.str();
}

//
//----------------------------------------------------------------------
//

std::string
Vector :: infoString
	( std::string const & title
	, bool const & showGrade
	, bool const & showMag
	) const
{
	return infoStringFmt(title, "%9.3f", showGrade, showMag);
}

std::string
Vector :: infoStringFmt
	( std::string const & title
	, std::string const & fmt
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "v:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss
		<< io::sprintf(fmt, theValues[0])
		<< " "
		<< io::sprintf(fmt, theValues[1])
		<< " "
		<< io::sprintf(fmt, theValues[2])
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< io::sprintf(fmt, Vector::magnitude())
			;
	}
	return oss.str();
}

//
//----------------------------------------------------------------------
//

std::string
BiVector :: infoString
	( std::string const & title
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "B:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss
		<< std::setw(sVW) << theValues[0]
		<< " "
		<< std::setw(sVW) << theValues[1]
		<< " "
		<< std::setw(sVW) << theValues[2]
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< std::setw(sVW) << BiVector::magnitude()
			;
	}
	return oss.str();
}

std::string
BiVector :: infoStringFmt
	( std::string const & title
	, std::string const & fmt
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "B:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss
		<< io::sprintf(fmt, theValues[0])
		<< " "
		<< io::sprintf(fmt, theValues[1])
		<< " "
		<< io::sprintf(fmt, theValues[2])
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< io::sprintf(fmt, BiVector::magnitude())
			;
	}
	return oss.str();
}
//
//----------------------------------------------------------------------
//

std::string
TriVector :: infoString
	( std::string const & title
	, bool const & showGrade
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "T:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss << std::setw(sVW) << theValue;
	return oss.str();
}

//
//======================================================================
//

std::string
Spinor :: infoString
	( std::string const & title
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "S:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss
		<< std::setw(sVW) << theS.theValue
		<< " "
		<< std::setw(sVW) << theB.theValues[0]
		<< " "
		<< std::setw(sVW) << theB.theValues[1]
		<< " "
		<< std::setw(sVW) << theB.theValues[2]
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< std::setw(sVW) << Spinor::magnitude()
			;
	}
	return oss.str();
}

std::string
Spinor :: infoStringFmt
	( std::string const & title
	, std::string const & fmt
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "S:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss << io::sprintf(fmt, theS.theValue)
		<< " "
		<< io::sprintf(fmt, theB.theValues[0])
		<< " "
		<< io::sprintf(fmt, theB.theValues[1])
		<< " "
		<< io::sprintf(fmt, theB.theValues[2])
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< std::setw(sVW) << Spinor::magnitude()
			;
	}
	return oss.str();
}


//
//======================================================================
//

std::string
ImSpinor :: infoString
	( std::string const & title
	, bool const & showGrade
	, bool const & showMag
	) const
{
	std::ostringstream oss;
	if (showGrade)
	{
		oss << "I:";
	}
	setNumeric(oss);
	oss << infoTitle(title);
	oss
		<< std::setw(sVW) << theV.theValues[0]
		<< " "
		<< std::setw(sVW) << theV.theValues[1]
		<< " "
		<< std::setw(sVW) << theV.theValues[2]
		<< " "
		<< std::setw(sVW) << theT.theValue
		;
	if (showMag)
	{
		oss << " " << "  mag:"
			<< " "
			<< std::setw(sVW) << ImSpinor::magnitude()
			;
	}
	return oss.str();
}

//======================================================================

extern Zero const zero{};
extern One const one{};

extern Scalar const e0{1.};
extern Vector const e1{1., 0., 0.};
extern Vector const e2{0., 1., 0.};
extern Vector const e3{0., 0., 1.};
extern BiVector const E23{1., 0., 0.};
extern BiVector const E31{0., 1., 0.};
extern BiVector const E12{0., 0., 1.};
extern TriVector const E123{1.};
extern std::array<Vector, 3u> const ei{{ e1, e2, e3 }};
extern std::array<BiVector, 3u> const Eij{{ E23, E31, E12 }};

extern Scalar const sZero(zero);
extern Vector const vZero(zero);
extern BiVector const bZero(zero);
extern TriVector const tZero(zero);

//======================================================================
}

