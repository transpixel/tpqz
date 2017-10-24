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
\brief Declarations for namespace sig
*/

/*! \brief Declarations and Definitions for libsig.

\section Overview General Concept:

Signal processing operations useful for photogrammetric image processing.

\subsection Terminology

The concept of "window" is utilized in many contexts as is the concept of
a filter's "response". To avoid such ambiguity, this library defines a
number unique names to various filter entity and operations concepts.
Although non-standard and perhaps unusual, this terminology support class
and function names (and argument polymorphism) which are unambiguous.

Terminology for this module includes:

\par Inputs

\arg _Signal_: Data being processed (e.g. an edge response 'image')
\arg _Elem_: Quantum of the input signal (e.g. a raster element, pixel)
\arg _Hood_: Neighborhood of other Elem adjacent to a specific Elem

\par Positions

\arg _Location_: dat::RowCol: identify Elem in Full - classic (row,col) indexing
\arg _Spot_: dat::Spot: Location within 2D signal with sub-Elem precision.

\par Regions

\arg _Full_: Entirity of input data (e.g. a reference grid or a target grid)
\arg _Domain_: Active portion of Full (e.g. overlap between Ref/Tgt grids)
\arg _Hunk_: Region over which filter integrates input Elems
\arg _Move_: Region over which Hunk moves during filter response generation
\arg _Ward_: Region sufficient for combined geometry of Hunk position over Move

\par Responses

\arg _Ping_: Fundamental response function (e.g. a Squared Difference, etc)
\arg _Note_: Value obtained accumulation of multiple Pings over extent of a Hunk
\arg _Tune_: Collection of Notes obtained as Hunk translates over Move region
\arg _Peak_: Prominent aspect within a Tune (e.g. locally max response)

\par Associations

\arg _Clan_: a collection of (potentially) corresponding entities. E.g.
collection of [Ward | Tune | Peak | Spot ] etc.

\par Actions

\arg _Crop_: Extraction of all Elem for a Hunk or Ward
\arg _Join_: Digestion of Clan to produce a specific result (e.g. match)
\arg _Post_: Creation of a Ping from multiple Elem values
\arg _Emit_: Creation of a Note from multiple Ping values
\arg _Comp_: Creation of a Tune by composition of multiple Note values

*/
namespace sig
{
}

