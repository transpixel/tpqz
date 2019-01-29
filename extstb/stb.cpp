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


// ====
// ==== Read capabilities
// ====


// activate all definitions
#define STB_IMAGE_IMPLEMENTATION

// enable specific capabilities
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
// #define STBI_ONLY_BMP
// #define STBI_ONLY_PSD
// #define STBI_ONLY_TGA
// #define STBI_ONLY_GIF
// #define STBI_ONLY_HDR
// #define STBI_ONLY_PIC
// #define STBI_ONLY_PNM  // (.ppm and .pgm)

// include source code inlcuding (conditional) definitions
#include "stb_image.h"


// ====
// ==== Write capabilities
// ====

// activate all definitions
#define STB_IMAGE_WRITE_IMPLEMENTATION

// disable use of stdio.h
/*
You can define STBI_WRITE_NO_STDIO to disable the file variant of these
functions, so the library will not use stdio.h at all. However, this will
also disable HDR writing, because it requires stdio for formatted output.
*/
// #define STBI_WRITE_NO_STDIO // NOTE! bug in #defs, still uses sprintf (HDR)

// include source code with (now)activated definitions
#include "stb_image_write.h"

