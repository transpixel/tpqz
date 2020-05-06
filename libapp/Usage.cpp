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
\brief Definitions for app::Usage
*/

// declaration include
#include "libapp/Usage.h"

#include <cassert>
#include <iostream>
#include <sstream>

//
// constructor
//
app::Usage :: Usage
	()
	: theSummary()
	, theArgPairs()
	, theArgOptPairs()
	, theValidFuncs()
	, theSpecialPair()
{
}

//
// isValid
//
bool
app::Usage :: isValid() const
{
	bool status
		( ! theSummary.empty()
		&& ! theArgPairs.empty()
		&& ( theSpecialPair.first.empty() == theSpecialPair.second.empty() )
		&& theValidFuncs.size() == (theArgPairs.size() + theArgOptPairs.size())
		);

	if (status)
	{
		for (std::vector<std::pair<std::string, std::string> >::const_iterator
			ita(theArgPairs.begin()) ; ita != theArgPairs.end() ; ++ita)
		{
			status &= ! ita->first.empty() && ! ita->second.empty();
		}

		for (std::vector<std::pair<std::string, std::string> >::const_iterator
			ita(theArgOptPairs.begin()) ; ita != theArgOptPairs.end() ; ++ita)
		{
			status &= ! ita->first.empty() && ! ita->second.empty();
		}

	}

	return status;
}

//
// addArg
//
void
app::Usage :: addArg
	( std::string const & name
	, std::string const & desc
	, app::Usage::ValidFunc const & func
	)
{
	assert(theArgOptPairs.empty());
	theArgPairs.push_back(std::make_pair(name, desc));
	theValidFuncs.push_back(func);
}

//
// addArgOpt
//
void
app::Usage :: addArgOpt
	( std::string const & name
	, std::string const & desc
	, app::Usage::ValidFunc const & func
	)
{
	theArgOptPairs.push_back(std::make_pair(name, desc));
	theValidFuncs.push_back(func);
}

//
// setSummary
//
void
app::Usage :: setSummary
	( std::string const & summary
	)
{
	theSummary = summary;
}

//
// setSpecial
//
void
app::Usage :: setSpecial
	( std::string const & name
	, std::string const & desc
	)
{
	theSpecialPair = std::make_pair(name, desc);
}

//
// argStatus
//
app::Usage::Status
app::Usage :: argStatus
	( int const & argc
	, char const * const argv[]
	) const
{
	Status status(Valid);

	if (! theArgPairs.empty())
	{
		// check number of arguments
		std::vector<std::string> const args(argv + 1, argv + argc);
		if (args.empty())
		{
			status = TooFew;
		}
		else
		if (args.front().substr(0, 2) == "--")
		{
			status = Special;
		}
		else
		if (args.size() < theArgPairs.size())
		{
			status = TooFew;
		}

		// check status of arguments
		for ( size_t index(0)
			; index < theValidFuncs.size() && Valid == status
			; ++index )
		{
			ValidFunc const & func = theValidFuncs[index];

			if (nullptr != func)
			{
				std::string const & arg = args[index];
				if (! func(arg))
				{
					status = ValidationFail;
					std::cerr << "\nBad argument: " << 1 + index << "\n"
						<< "  arg: <";
					if (index < theArgPairs.size())
					{
						std::cerr << theArgPairs[index].first;
					}
					else
					if (index < (theArgPairs.size() + theArgOptPairs.size()))
					{
						size_t const oindex(index - theArgPairs.size());
						std::cerr << theArgOptPairs[oindex].first;
					}

					std::cerr << ">\n" << "  val: '" << arg << "'" << std::endl;
				}
			}
		}
	}

	return status;
}

//
// argSummary
//
std::string
app::Usage :: argSummary
	( int const & argc
	, char const * const argv[]
	) const
{
	std::ostringstream ostr;

	ostr << "Application: '" << argv[0] << "'" << '\n';

	int argnum(0);
	while (++argnum < argc)
	{
		ostr << "  '" << argv[argnum] << "'" << '\n';
	}

	return ostr.str();
}

//
// infoString
//
std::string
app::Usage :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;

	oss << std::endl;
	oss << "Summary:";
	oss << std::endl;
	oss << "  " << theSummary;

	oss << std::endl;
	oss << "Usage: " << title;
	for (std::vector<std::pair<std::string, std::string> >::const_iterator
		ita(theArgPairs.begin()) ; ita != theArgPairs.end() ; ++ita)
	{
		oss << " <" << ita->first << ">";
	}
	for (std::vector<std::pair<std::string, std::string> >::const_iterator
		ita(theArgOptPairs.begin()) ; ita != theArgOptPairs.end() ; ++ita)
	{
		oss << " [" << ita->first << "]";
	}

	if (! theArgPairs.empty())
	{
		oss << std::endl;
		oss << "Required Arguments:";
		for (std::vector<std::pair<std::string, std::string> >::const_iterator
			ita(theArgPairs.begin()) ; ita != theArgPairs.end() ; ++ita)
		{
			oss << std::endl;
			oss << "  <" << ita->first << "> : " << ita->second;
		}
	}

	if (! theArgOptPairs.empty())
	{
		oss << std::endl;
		oss << "Optional Arguments:";
		for (std::vector<std::pair<std::string, std::string> >::const_iterator
			ita(theArgOptPairs.begin()) ; ita != theArgOptPairs.end() ; ++ita)
		{
			oss << std::endl;
			oss << "  [" << ita->first << "] : " << ita->second;
		}
	}

	if (! theSpecialPair.first.empty() && !theSpecialPair.second.empty())
	{
		oss << std::endl;
		oss << "Or Optionally:";
		oss << std::endl;
		oss << "  --" << theSpecialPair.first << " : " << theSpecialPair.second;
	}
	oss << std::endl;

	return oss.str();
}

