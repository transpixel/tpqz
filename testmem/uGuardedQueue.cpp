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
\brief  This file contains test for mem::GuardedQueue
*/


#include "libmem/GuardedQueue.h"

#include "libio/stream.h"
#include "libsys/time.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>


namespace
{
	//
	// Test configuration
	//

	//! Number of requests to process
	static size_t const sNumRequests(1u * 1000u);

	//! Max simulated processing time
	static double const sMaxCrunchTime(.000005000);

	//
	// Processing configuration
	//

	//! Delay when nothing to do
	static double const sWaitDelay(.000001000);

	//! Pause calling thread to free resources for others
	void
	restAWhile
		()
	{
		long const microSleep((long)std::floor(sWaitDelay));
		std::this_thread::sleep_for(std::chrono::microseconds(microSleep));
	}

	//! Conditional output
	struct Output
	{
		bool const theEnabled;
		io::out theOut;

		Output
			( bool const & enable
			)
			: theEnabled(enable)
		{ }

		~Output
			()
		{ }

		template <typename Type>
		Output &
		operator<<
			( Type const & value
			)
		{
			if (theEnabled)
			{
				theOut << value;
			}
			return *this;
		}

		Output &
		operator<<
			( std::ostream & (*fptr)(std::ostream &)
			)
		{
			if (theEnabled)
			{
				theOut << fptr;
			}
			return *this;
		}
	};

	//! Set to true to produce output trace of operations
	static constexpr bool sDoOut{false};


//! Aribtrary delay
inline
long
arbitraryMicroDelay
	( size_t const & seed
	)
{
	// compute some arbitrary wait interval
	static double const microAmp(1.e6*sMaxCrunchTime);
	double const updown(std::cos((double)seed));
	double const delay(std::max(0., std::floor(microAmp*updown)));
	long const nmicro((long)delay);
	return nmicro;
}


//! Check for common functions
std::string
mem_GuardedQueue_test0
	()
{
	std::ostringstream oss;
	mem::GuardedQueue<double> const anEmpty;
	// enforce infoString presence
	(void)anEmpty.infoString("anEmpty");
	return oss.str();
}


//! Subsystem types
enum SubSysId
{
	  Unknown
	, SubSystemA
	, SubSystemB
	, SubSystemC
};

//! Example arbitrary command representation
struct SubSysCommand
{
	size_t theCode;
};

//! Example arbitrary data representation
struct SubSysData
{
	double theValue;
};

//! Example arbitrary subsystem action request packet
struct ActionRequest
{
	SubSysId theSubSysId;
	SubSysCommand theSubSysCommand;
	SubSysData theSubSysData;

	//! Invalid instance
	ActionRequest
		()
		: theSubSysId(SubSysId::Unknown)
		, theSubSysCommand{0u}
		, theSubSysData{0.}
	{ }

	//! Value constructor
	explicit
	ActionRequest
		( SubSysId const & subsys
		, size_t const & cmdCode
		, double const & cmdData
		)
		: theSubSysId(subsys)
		, theSubSysCommand{cmdCode}
		, theSubSysData{cmdData}
	{ }

	//! True if this instance is valid
	bool
	isValid
		() const
	{
		return (! (SubSysId::Unknown == theSubSysId));
	}

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << " ";
		}

		oss
			<< "  " << theSubSysId
			<< "  " << theSubSysCommand.theCode
			<< "  " << theSubSysData.theValue
			;

		return oss.str();
	}
};

//! Create an arbitrary ActionRequest
ActionRequest
arbitraryAction
	()
{
	ActionRequest request;

	// select a subsystem arbitrarily
	double const nanoSecs(1.e9 * sys::time::relativeNow());
	size_t const ns((size_t)std::floor(nanoSecs));
	size_t const subCount(ns % 7u);
	SubSysId subId(SubSysId::Unknown);
	if (0u < subCount)
	{
		size_t const select(subCount % 3u);
		if (0u == select)
		{
			subId = SubSysId::SubSystemA;
		}
		else
		if (1u == select)
		{
			subId = SubSysId::SubSystemB;
		}
		else
		if (2u == select)
		{
			subId = SubSysId::SubSystemC;
		}
	}

	// command and data fer grins
	size_t const cmdCode(subCount);
	double const cmdData(nanoSecs);

	// form synthetic data into action request for return
	request = ActionRequest(subId, cmdCode, cmdData);

	return request;
}


template <typename DataType>
class SourceOfRequests
{
	mem::GuardedQueue<DataType> * const theQ;

public:

	//! construct with attachement to guarded queue (on which to enqueue)
	SourceOfRequests
		( mem::GuardedQueue<DataType> * const & gQueue
		)
		: theQ(gQueue)
	{
		assert(theQ);
		assert(theQ->isActive());
	}

	//! Place items onto queue
	void
	operator()
		() const
	{
		Output(sDoOut) << "SourceOfRequests Starting\n";

		for (size_t nn(0u) ; nn < sNumRequests ; ++nn)
		{
			// simulate some artibtrary waiting
			// NOTE: make delay 3x longer for 3x processing subsystems
			long const nmicro(3u * arbitraryMicroDelay(nn));
			std::this_thread::sleep_for(std::chrono::microseconds(nmicro));

			// before enqueueing the next data item
			ActionRequest const request(arbitraryAction());

			// dont' polute the output with invalid cases, but
			if (request.isValid())
			{
				Output(sDoOut) << request.infoString("request") << '\n';
			}

			// include invalid case in testing (e.g. requests with no home)
			theQ->addItem(request);
		}

		Output(sDoOut) << "SourceOfRequests dactivating queue\n";
		// inform others : all finished generating requests
		theQ->deactivate();

		Output(sDoOut) << "SourceOfRequests exiting\n";
	}

};

template <typename DataType>
class SubSystem
{
	mem::GuardedQueue<DataType> * const theRequestQ;

protected:

	std::string theSubSysName;

public:

	//! construct with attachement to guarded queue (with which to process)
	SubSystem
		( mem::GuardedQueue<DataType> * const & queue
		, std::string const & subSysName
		)
		: theRequestQ(queue)
		, theSubSysName(subSysName)
	{
		assert(theRequestQ);
		assert(theRequestQ->isActive());
	}

	//! Noop destricture
	virtual
	~SubSystem
		()
	{ }

	//! Process a data item
	virtual
	void
	processItem
		( DataType const & dataItem
		) = 0;

	//! simulate processing items
	void
	operator()
		()
	{
		Output(sDoOut) << "Starting: " << theSubSysName << '\n';

		while (theRequestQ->isActive() || theRequestQ->hasData())
		{
			// dequeue data item
			DataType const dataItem(theRequestQ->nextDataItem());
			if (dataItem.isValid())
			{
				processItem(dataItem);
			}
			else
			{
				restAWhile();
			}
		}

		Output(sDoOut) << "SubSystem EXIT\n";
	}
};

template <typename DataType>
class FakeProcessor : public SubSystem<DataType>
{
	size_t theCount;
	size_t theCountDelta;

public:

	//! construct with attachement to guarded queue (with which to process)
	FakeProcessor
		( mem::GuardedQueue<DataType> * const & ptrQueue
		, std::string const & subSysName
		, size_t const & delaySeed
		)
		: SubSystem<DataType>(ptrQueue, subSysName)
		, theCount(0u)
		, theCountDelta(delaySeed)
	{ }

	virtual
	~FakeProcessor
		()
	{ }

	//! Fake processing by waiting a while
	virtual
	void
	processItem
		( DataType const & dataItem
		)
	{
		std::string const processor(SubSystem<DataType>::theSubSysName);
		Output(sDoOut) << dataItem.infoString(processor) << '\n';

		// simulate some arbitrary processing time
		long const nmicro(arbitraryMicroDelay(theCount));
		std::this_thread::sleep_for(std::chrono::microseconds(nmicro));
		theCount += theCountDelta;
	}

};

// for testing just fake wait and print message
// - so use same tes-code implementation for "different" subsystem activities
typedef FakeProcessor<ActionRequest> ProcessorA;
typedef FakeProcessor<ActionRequest> ProcessorB;
typedef FakeProcessor<ActionRequest> ProcessorC;


class SystemCoordinator
{
//private: // disable
//
//	//! Disable implicit copy and assignment
//	SystemCoordinator(SystemCoordinator const &) = delete;
//	SystemCoordinator & operator=(SystemCoordinator const &) = delete;

public:
	// instantiate request queues
	std::shared_ptr<mem::GuardedQueue<ActionRequest> > theMasterQ;

	// and subsystem processing queues
	std::shared_ptr<mem::GuardedQueue<ActionRequest> > theQueueA;
	std::shared_ptr<mem::GuardedQueue<ActionRequest> > theQueueB;
	std::shared_ptr<mem::GuardedQueue<ActionRequest> > theQueueC;

	// create source to add items
	SourceOfRequests<ActionRequest> theSource;

	// create consumer subsystems
	ProcessorA theSystemA;
	ProcessorB theSystemB;
	ProcessorC theSystemC;


	//! Construct input and processing Queues
	SystemCoordinator
		()
		: theMasterQ(std::make_shared<mem::GuardedQueue<ActionRequest> >())
		, theQueueA(std::make_shared<mem::GuardedQueue<ActionRequest> >())
		, theQueueB(std::make_shared<mem::GuardedQueue<ActionRequest> >())
		, theQueueC(std::make_shared<mem::GuardedQueue<ActionRequest> >())
		, theSource(theMasterQ.get())
		, theSystemA(theQueueA.get(), "proc:A--", 17u)
		, theSystemB(theQueueB.get(), "proc:-B-", 19u)
		, theSystemC(theQueueB.get(), "proc:--C", 29u)
	{ }

	//! Run operations in own thread
	void
	operator()
		()
	{
		// start processing subsystems
		std::thread threadSystemA(theSystemA);
		std::thread threadSystemB(theSystemB);
		std::thread threadSystemC(theSystemC);

		// start request source
		std::thread threadSource(theSource);

		// monitor and route requests until request source shutdown
		Output(sDoOut) << '\n';
		Output(sDoOut) << "SystemCoordinator: Start request processing\n";
		while (theMasterQ->isActive() || theMasterQ->hasData())
		{
			ActionRequest const request(theMasterQ->nextDataItem());

			// if input request is valid route request, else rest a while
			if (request.isValid())
			{
				// use request information
				SubSysId const & subId = request.theSubSysId;

				// TODO - probably should be array of subsystems
				// to route request to appropriate subsystem
				if (SubSysId::SubSystemA == subId)
				{
					// subsystem A
					theQueueA->addItem(request);
				}
				else
				if (SubSysId::SubSystemB == subId)
				{
					// subsystem B
					theQueueB->addItem(request);
				}
				else
				if (SubSysId::SubSystemC == subId)
				{
					// subsystem C
					theQueueC->addItem(request);
				}
			}
			else
			{
				// otherwise wait a while for something interesting to happen
				restAWhile();
			}

		}
		Output(sDoOut) << '\n';
		Output(sDoOut) << "SystemCoordinator: end request processing\n";

		Output(sDoOut) << "SystemCoordinator: deactivating queues\n";
		// deactive processing queues (jobs continue until queues are empty)
		theQueueA->deactivate();
		theQueueB->deactivate();
		theQueueC->deactivate();


		Output(sDoOut) << "SystemCoordinator: waiting for threads\n";
		// Ensure all jobs are complete
		threadSource.join();
		threadSystemA.join();
		threadSystemB.join();
		threadSystemC.join();

	}

	public:
};


//! Test to exercise GuardedQueue in quasi realistic environment.
std::string
mem_GuardedQueue_test1
	()
{
	std::ostringstream oss;

	// create system coordinator
	SystemCoordinator sysCoord;

	// run system controller until completed
	std::thread threadCoord(sysCoord);
	threadCoord.join();

	// when controller joins, everything's done

	return oss.str();
}


}

//! Unit test for mem::GuardedQueue
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	oss << mem_GuardedQueue_test0();
	oss << mem_GuardedQueue_test1();

	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}

