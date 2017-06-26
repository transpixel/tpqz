// 
//  -$RCSfile: Usage.h,v $
//  $Revision: 1.5 $
//  Rev-$Name:  $
//  Mod-$Date: 2013/06/17 15:08:41 $
// 
//  Copyright (c) 2007 Stellacore Corporation. All Rights Reserved.
//
// #HEADER_NOTICE#
//

#ifndef app_Usage_INCL_
#define app_Usage_INCL_

/*! \file
\brief Declarations for app::Usage
*/

#include <string>
#include <vector>

namespace app
{

/*! \brief Argument usage summary.

If the program doesn't need arguments don't use this class.

\par Example
\dontinclude testapp/uUsage.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Usage
{

public: // enum

	enum Status { Valid, TooFew, Special, ValidationFail };

public: // function pointer

	typedef bool (*ValidFunc)(std::string const &);

public: // methods

	//! construct with arguments
	Usage
		();

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Add an argument -- will assert if optional args not empty.
	void
	addArg
		( std::string const & name
		, std::string const & desc
		, ValidFunc const & func=0
		);

	//! Add an optional argument -- must be at end.
	void
	addArgOpt
		( std::string const & name
		, std::string const & desc
		, ValidFunc const & func=0
		);

	//! Set summary
	void
	setSummary
		( std::string const & summary
		);

	//! Special argument implied to have a leading '--'
	void
	setSpecial
		( std::string const & name
		, std::string const & desc
		);

	//! Check args status
	Status
	argStatus
		( int const & argc
		, char const * const argv[]
		) const;

	//! report passed in arguments
	std::string
	argSummary
		( int const & argc
		, char const * const argv[]
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title="<program>"
		) const;

private: // data

	std::string theSummary;
		//!< program summary

	std::vector<std::pair<std::string, std::string> > theArgPairs;
		//!< required options

	std::vector<std::pair<std::string, std::string> > theArgOptPairs;
		//!< optional options

	std::vector<ValidFunc> theValidFuncs;
		//!< argument validator functions

	std::pair<std::string, std::string> theSpecialPair;
		//!< special arg begins with '--'


};

}

// Inline definitions
// #include "libapp/Usage.inl"

#endif // app_Usage_INCL_

