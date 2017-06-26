//
// TODO_HEADER_NOTICE
//


/*! \file
\brief Definitions for app::ProcessLogger
*/


#include "libapp/ProcessLogger.h"


namespace app
{

// static
void
ProcessLogger :: mark
	( ProcessLogger * const & ptLog
	, std::string const & msg
	)
{
	if (ptLog)
	{
		ptLog->mark(msg);
	}
}


// explicit
ProcessLogger :: ProcessLogger
	( std::vector<std::string> const memKeys
	)
	: theTimer{}
	, theOssMem{}
	, theMemReporter(memKeys)
{
}

void
ProcessLogger :: markMem
	( std::string const & msg
	)
{
	theMemReporter(msg);
}

void
ProcessLogger :: markTime
	( std::string const & msg
	)
{
	theTimer.start(msg);
}

void
ProcessLogger :: mark
	( std::string const & msg
	)
{
	markMem(msg);
	markTime(msg);
}

void
ProcessLogger :: operator()
	( std::string const & msg
	)
{
	mark(msg);
}

std::string
ProcessLogger :: infoStringMem
	( std::string const & title
	) const
{
	return theMemReporter.infoString(title);
}

std::string
ProcessLogger :: infoStringTime
	( std::string const & title
	) const
{
	return theTimer.infoString(title);
}
}

