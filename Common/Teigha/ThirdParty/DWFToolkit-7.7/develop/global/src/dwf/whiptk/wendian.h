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

#if !defined ENDIAN_HEADER
#define ENDIAN_HEADER

///
///\file        dwf/whiptk/wendian.h
///

#include "whiptk/whipcore.h"


// We could handle the endian flipping by using a "union" structure
// of a word and a character array, and then use direct subscripts rather
// than all the bitwise and shift operations.

/// Utility class to translate to the proper endian encoding.
/** Note, these static methods are essentially a no-op on a little endian system. */
class WHIPTK_API WT_Endian
{
public:

    /// Adjusts value based on the endian encoding that was specified at compile time.
    static WT_Integer16 adjust(WT_Integer16 value)
    {
#if WD_BIGENDIAN
        return (   ( (value << 8) & 0xFF00) |
                   ( (value >> 8) & 0x00FF)    );
#else
        return value;
#endif
    }

    /// Adjusts value based on the endian encoding that was specified at compile time.
    static WT_Unsigned_Integer16 adjust(WT_Unsigned_Integer16 value)
    {
#if WD_BIGENDIAN
        return (   ( (value << 8) & 0xFF00) |
                   ( (value >> 8) & 0x00FF)    );
#else
        return value;
#endif
    }

    /// Adjusts value based on the endian encoding that was specified at compile time.
    static WT_Integer32 adjust(WT_Integer32 value)
    {
#if WD_BIGENDIAN
        return (   ( (value << 24) & 0xFF000000)  |
                   ( (value <<  8) & 0x00FF0000)  |
                   ( (value >>  8) & 0x0000FF00)  |
                   ( (value >> 24) & 0x000000FF)    );
#else
        return value;
#endif
    }

    /// Adjusts value based on the endian encoding that was specified at compile time.
    static WT_Unsigned_Integer32 adjust(WT_Unsigned_Integer32 value)
    {
#if WD_BIGENDIAN
        return (   ( (value << 24) & 0xFF000000)  |
                   ( (value <<  8) & 0x00FF0000)  |
                   ( (value >>  8) & 0x0000FF00)  |
                   ( (value >> 24) & 0x000000FF)    );
#else
        return value;
#endif
    }

};

#endif // ENDIAN_HEADER
