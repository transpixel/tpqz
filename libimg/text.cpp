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
\brief Definitions for img::text
*/


#include "libimg/text.h"

#include "opencv.hpp"


namespace
{
	//! Draw processing options into image
	void
	writeOptionsInto
		( cv::Mat * const & ptImage
		, std::vector<std::string> const & infoLines
		, size_t const & textHigh
		)
	{
		constexpr int fontType{ cv::FONT_HERSHEY_PLAIN };
		constexpr int fontThick{ 1u };
		constexpr size_t fontHigh{ 16u };
		double const fontScale{ double(textHigh) / double(fontHigh) };

		double const yHigh{ fontScale * double(fontHigh) };
		int const yDelta{ int(yHigh) + 1 };

		cv::Scalar const color{0, 255, 255};

		int const xPos(yDelta);
		int yPos(yDelta);
		for (size_t nn(0u) ; nn < infoLines.size() ; ++nn)
		{
			std::string const & infoLine = infoLines[nn];
			cv::putText
				( *ptImage
				, infoLine
				, cv::Point(xPos, yPos)
				, fontType
				, fontScale
				, color
				, fontThick
				);
			yPos += yDelta;
		}
	}
}

namespace img
{
namespace text
{

//======================================================================
	
void
putInto
	( dat::grid<std::array<uint8_t, 3u> > * const & ptGrid
	, std::vector<std::string> const & textLines
	, size_t const & textHigh
	)
{
	if (ptGrid->isValid())
	{
		// construct opencv header for data
		unsigned char * const ptData((unsigned char * const)ptGrid->begin());
		int const ihigh{ static_cast<int>(ptGrid->high()) };
		int const iwide{ static_cast<int>(ptGrid->wide()) };
		constexpr int cvTypeCode{ CV_8UC3 };
		cv::Mat cvm(ihigh, iwide , cvTypeCode , ptData);
		writeOptionsInto(& cvm, textLines, textHigh);
	}
}

//======================================================================
}
}

