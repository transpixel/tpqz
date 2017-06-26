//
// TODO_HEADER_NOTICE
//

#ifndef app_Progress_INCL_
#define app_Progress_INCL_

/*! \file
\brief Declarations for app::Progress
*/


#include "libdat/validity.h"

#include <string>


namespace app
{

/*! \brief Simple progress state class

\par Example
\dontinclude testapp/uProgress.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Progress
{

	size_t theMaxValue{ dat::nullValue<size_t>() };
	size_t theAtValue{ dat::nullValue<size_t>() };

public: // methods

	//! default null constructor
	Progress
		() = default;

	//! Initialization construction
	explicit
	Progress
		( size_t const & maxValue
		, size_t const & startValue = 0u
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! (Pre) Increment progress - and return modified self
	Progress const &
	operator++
		();

	//! Calls op++() numBump times - and return modified self
	Progress const &
	advance
		( size_t const & numBump
		);

	//! Amount done as fractional value [0.,1.]
	double
	fractionDone
		() const;

	//! Amount done as (nearest) percent value
	uint8_t
	percentDone
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libapp/Progress.inl"

#endif // app_Progress_INCL_


