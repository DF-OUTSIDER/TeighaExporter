//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

///
///\file        dwf/whiptk/lzdefs.h
///

#if !defined LZDEFS_HEADER
#define LZDEFS_HEADER

#define WD_LZ_HISTORY_BUFFER_SIZE               65536
#define WD_LZ_COMPRESSION_AND_OFFSET_CODE_BYTES 3

#define WD_ZLIB_COMPRESSION_OUTPUT_BUFFER_SIZE  8192
#define WD_ZLIB_DECOMPRESSION_INPUT_BUFFER_SIZE 512
#define WD_ZLIB_DEFLATE_LEVEL                   7   // Normal ZLIB default is 6 or Z_DEFAULT_COMPRESSION
#define WD_ZLIB_DEFLATE_WINDOW_BITS             15  // Max allowed is 15, default is 15 which gives 32K history buffer
#define WD_ZLIB_DEFLATE_MEM_LEVEL               9   // Max allowed is 9, default is 8

// This following define really belongs in the file PRELOAD.H, but due to some compilation
// issues it was moved here.
#define WD_PORTION_OF_PRELOAD_DATA_NOT_USED_FOR_ZLIB    (2 * 256 * 4)   /* 2 Colormaps of 256 rgba colors */

/// The compression base classes for all other specific compression engines.
/** \note Internal use only - not intended for direct use by client code. */
class WHIPTK_API WT_Compressor
{
public:

    virtual                ~WT_Compressor() { }
    virtual    WT_Result   start() = 0; // Pure virtual
    virtual    WT_Boolean  is_compression_started() = 0; // Pure virtual
    virtual    WT_Result   stop()  = 0; // Pure virtual
    virtual    WT_Result   compress(int in_size, void const * in_buf) = 0; // Pure virtual
};

/// The decompression base classes for all other specific decompression engines.
/** \note Internal use only - not intended for direct use by client code. */
class WHIPTK_API WT_DeCompressor
{
public:
	virtual                ~WT_DeCompressor() { }
    virtual    WT_Result   start() = 0; // Pure virtual
    virtual    WT_Result   decompress(int desired, int & actual, void * buffer) = 0; // Pure virtual
};


#endif // LZDEFS_HEADER
