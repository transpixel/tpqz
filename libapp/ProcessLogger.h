//
// TODO_HEADER_NOTICE
//

#ifndef app_ProcessLogger_INCL_
#define app_ProcessLogger_INCL_

/*! \file
\brief Declarations for app::ProcessLogger
*/


#include "libmem/query.h"
#include "libapp/Timer.h"

#include <memory>
#include <string>
#include <sstream>


namespace app
{

//! \brief Record various process resource usage information.
class ProcessLogger
{

	app::Timer theTimer;
	std::ostringstream theOssMem;
	mem::query::Reporter theMemReporter;

public: // static methods

	//! Mark message to logger if pter is not null
	static
	void
	mark
		( ProcessLogger * const & ptLog
		, std::string const & msg
		);

public: // methods

	//! Construct to monitor specified items
	explicit
	ProcessLogger
		( std::vector<std::string> const memKeys
			= std::vector<std::string>{ "VmSize", "VmRSS" }
		);

	//! Make note of current memory use
	void
	markMem
		( std::string const & msg
		);

	//! Begin a new timing interval
	void
	markTime
		( std::string const & msg
		);

	//! Make a note of current process state
	void
	mark
		( std::string const & msg
		);

	//! Functor form of mark()
	void
	operator()
		( std::string const & msg
		);

	//! Description of memory log
	std::string
	infoStringMem
		( std::string const & title = std::string()
		) const;

	//! Description of timer log
	std::string
	infoStringTime
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libapp/ProcessLogger.inl"

#endif // app_ProcessLogger_INCL_

