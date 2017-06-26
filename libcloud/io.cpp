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
\brief Definitions for cloud::io
*/


#include "libcloud/io.h"

#include "libcloud/cast.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <cassert>
#include <fstream>
#include <limits>


namespace cloud
{
namespace io
{

	//! Open stream and verify data size
	bool
	verifyStream
		( std::ifstream & ifs
		, std::string const & fpath
		, std::streamsize const & bytesPerRec
		, size_t * const & ptNumPnts
		)
	{
		bool okay{ false };

		// open at end of file
		ifs.open
			( fpath
			, std::ios_base::binary | std::ios_base::ate
			);

		// check size
		if (ifs.good())
		{
			// determine number of records
			std::streamsize const allBytes{ (std::streamsize)ifs.tellg() };
			std::streamsize const numRecs{ allBytes / bytesPerRec };
			std::streamsize const expBytes{ numRecs * bytesPerRec };
			if (allBytes == expBytes)
			{
				assert(ptNumPnts);
				*ptNumPnts = (size_t)numRecs;

				// return to start of file
				ifs.seekg(0);
				okay = ifs.good();
			}
			else
			{
				// incongruent filesize
				::io::err()
					<< "Failure: cloud::io::verifyStream: bad file size:"
					<< ' ' << dat::infoString(+expBytes, "expBytes")
					<< ' ' << dat::infoString(+allBytes, "allBytes")
					<< std::endl;
				ifs.close();
			}
		}
		else
		{
			// bad file open
			::io::err()
				<< "Failure: cloud::io::verifyStream: can't open file:"
				<< ' ' << fpath
				<< std::endl;
		}

		return okay;
	}

	//! Load entire stream as binary data
	bool
	fillBinary
		( std::istream & istrm
		, std::vector<RecordBin> * const & ptRecs
		, size_t const & numPnts
		, std::streamsize const & bytesPerRec
		)
	{
		bool okay{ false };
		assert(ptRecs);
		ptRecs->resize(numPnts);

		// read data
		std::streamsize const expBytes
			{ (std::streamsize)numPnts * bytesPerRec };
		istrm.read((char*)ptRecs->data(), expBytes);

		// check for error
		std::streamsize const gotBytes{ istrm.gcount() };
		if (gotBytes == expBytes)
		{
			okay = true;
		}

		return okay;
	}

	//! Load stream contents as packed data
	bool
	fillFixed
		( std::istream & istrm
		, std::vector<FixedPoint> * const & ptPnts
		, size_t const & numPnts
		, std::streamsize const & bytesPerRec
		)
	{
		bool okay{ false };
		assert(ptPnts);
		ptPnts->reserve(numPnts);

		// read data
		constexpr size_t numPerRead{ 1024u };
		std::vector<RecordBin> buf(numPerRead);
		while (ptPnts->size() < numPnts)
		{
			// read one point (could perhaps do block read, but...
			// ... presumably stream should handle that reasonably well
			size_t const numLeft{ numPnts - ptPnts->size() };
			size_t const numToRead{ std::min(numPerRead, numLeft) };
			std::streamsize const expBytes
				{ (std::streamsize)numToRead * bytesPerRec };
			istrm.read((char*)(buf.data()), expBytes);

			// check for error
			std::streamsize const gotBytes{ istrm.gcount() };
			if (gotBytes == expBytes)
			{
				size_t const numRecs{ (size_t)(gotBytes / bytesPerRec) };
				for (size_t nBuf{ 0u } ; nBuf < numRecs ; ++nBuf)
				{
					float const & xx = buf[nBuf][0];
					float const & yy = buf[nBuf][1];
					float const & zz = buf[nBuf][2];
					ptPnts->emplace_back(cast::FixedPoint(xx, yy, zz));
				}
			}
			else
			{
				break;
			}
		}

		// check if everything was loaded
		okay = (numPnts == ptPnts->size());

		return okay;
	}


std::vector<RecordBin>
loadAsBinary
	( std::string const & fpath
	)
{
	std::vector<RecordBin> recs;

	// number of bytes per file input data record
	constexpr std::streamsize bytesPerRec{ sizeof(RecordBin) };

	// access file and seek to end
	std::ifstream ifs;
	size_t numPnts{};
	bool const okayStream{ verifyStream(ifs, fpath, bytesPerRec, &numPnts) };

	// read data from a valid stream
	if (okayStream)
	{
		// perform actual load operation(s)
		bool const okayLoad{ fillBinary(ifs, &recs, numPnts, bytesPerRec) };

		// check for error
		if (! okayLoad)
		{
			recs.clear();
		}
	}

	return recs;
}

std::vector<FixedPoint>
loadAsFixed
	( std::string const & fpath
	)
{
	std::vector<FixedPoint> pnts;

	// number of bytes per file input data record
	constexpr std::streamsize bytesPerRec{ sizeof(RecordBin) };

	// access file and seek to end
	std::ifstream ifs;
	size_t numPnts{};
	bool const okayStream{ verifyStream(ifs, fpath, bytesPerRec, &numPnts) };

	// read data from a valid stream
	if (okayStream)
	{
		// perform actual load operation(s)
		bool const okayLoad{ fillFixed(ifs, &pnts, numPnts, bytesPerRec) };

		// check for error
		if (! okayLoad)
		{
			pnts.clear();
		}
	}

	return pnts;
}

bool
saveAsAscii
	( std::ostream & ostrm
	, std::vector<FixedPoint> const & fpnts
	, std::string const & fmt
	)
{
	return saveFixedPointAsAscii(ostrm, fpnts.begin(), fpnts.end(), fmt);
}


}
}

