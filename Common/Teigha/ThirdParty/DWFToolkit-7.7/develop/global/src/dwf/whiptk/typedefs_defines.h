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

#if !defined TYPEDEF_DEFINES_HEADER
#define TYPEDEF_DEFINES_HEADER

#include "OdPlatformSettings.h"
///
///\file        dwf/whiptk/typedefs_defines.h
///

// basic types & definitions

/// \typedef typedefchar WT_Boolean
/// \brief Simple boolean.
typedef OdInt8 WT_Boolean;

/// \typedef typedef long WT_Integer32
/// \brief 32-bit signed integer.
typedef OdInt32 WT_Integer32;

/// \typedef typedef long WT_Integer16
/// \brief 16-bit signed integer.
typedef OdInt16 WT_Integer16;

/// \typedef typedef unsigned long WT_Unsigned_Integer32
/// \brief 32-bit unsigned integer.
typedef OdUInt32 WT_Unsigned_Integer32;

/// \typedef typedef unsigned long WT_Unsigned_Integer16
/// \brief 16-bit unsigned integer.
typedef OdUInt16 WT_Unsigned_Integer16;

/// \typedef typedef unsigned char WT_Byte
/// \brief Unsigned 8-bit character.
typedef OdUInt8 WT_Byte;

/// \typedef typedef unsigned char WT_Byte
/// \brief Index into a color map.
typedef WT_Integer32 WT_Color_Index;

/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
/// \brief A GUID structure.
class WD_GUID
{
  public:
    WD_GUID()
      : Data1(0)
      , Data2(0)
      , Data3(0)
    { Data4[0] = Data4[1] = Data4[2] = Data4[3] = Data4[4] = Data4[5] = Data4[6] = Data4[7] = 0; }

  WT_Unsigned_Integer32  Data1; /**< \brief XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx */
  WT_Unsigned_Integer16 Data2;  /**< \brief xxxxxxxx-XXXX-xxxx-xxxx-xxxxxxxxxxxx */
  WT_Unsigned_Integer16 Data3; /**< \brief xxxxxxxx-xxxx-XXXX-xxxx-xxxxxxxxxxxx */
  WT_Byte  Data4[8]; /**< \brief xxxxxxxx-xxxx-xxxx-0011-223344556677 */
};

/// \typedef typedef WD_GUID WD_UUID
/// \brief An alias for a GUID.
typedef WD_GUID WD_UUID;

#define WD_NO_COLOR_INDEX       -1
#define WD_Null                 NULL
#define WD_False                (static_cast<WT_Boolean>(0))
#define WD_True                 (static_cast<WT_Boolean>(-1))
#define WD_stack
#define WD_Opaque               255
#define WD_Clear                0

#define TWO_PI                  6.283185308
#define WD_90_DEGREES_AS_SHORT  (0x00010000 / 4)

#define WD_HEAP_LOCK
#define WD_HEAP_UNLOCK

inline WT_Boolean WD_fits_in_short(long x)
{
    return  x <= 32767 && x >= -32768;
}

// Note: use macro instead of inline since we need to return from the caller, not just the check.
//       macro is considered "safe" for side effects as its argument is only referenced once.
#define WD_CHECK(x) do {                                    \
                        WT_Result       result = (x);       \
                        if (result != WT_Result::Success)   \
                            return result;                  \
                    } while (0)                            //


inline WT_Boolean WU_is_whitespace(WT_Byte byte)
{
    switch (byte)
    {
    case (0x00):  // Null character -- ignore
    case (0x09):  // white space (Tab) -- ignore
    case (0x0A):  // white space (Line Feed) --ignore
    case (0x0D):  // white space (Carriage Return) -- ignore
    case (0x20):  // white space (space) -- ignore
        return WD_True;
    default:
      return WD_False;
    } // switch
}

#define BINARYSIZEOFGUID                                                                    \
    (WT_Integer32) (sizeof(WT_Integer32) + sizeof(WT_Unsigned_Integer16) +                  \
    sizeof(WT_Unsigned_Integer32) + sizeof(WT_Unsigned_Integer16) +                         \
    sizeof(WT_Unsigned_Integer16) + (8 * sizeof(WT_Byte)) + (2 * sizeof(WT_Byte)))          //


#define WD_MIN(first, second)    (first < second ? first : second);
#define WD_MAX(first, second)    (first > second ? first : second);

#endif // TYPEDEF_DEFINES_HEADER
