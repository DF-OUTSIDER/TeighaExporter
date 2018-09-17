//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#ifndef _DWFCORE_STRING_H
#define _DWFCORE_STRING_H

///
///\file        dwfcore/String.h
///\brief       This header contains the declaration of the DWFString class.
///

///
///\example     Strings/Strings.cpp
///             This sample program exercises some of the operations available in DWFCore::DWFString.
///
///             Also shown is some simple file I/O using the following core classes and interfaces:
///                 - DWFCore::DWFFile
///                 - DWFCore::DWFFileOutputStream
///                 - DWFCore::DWFStreamFileDescriptor
///
///             This file may contain Unicode text that may display incorrectly in the documentation
///             and/or not display/save/load correctly on certain filesystems and text editors, etc.
///


#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/Exception.h"
#include "dwfcore/Pointer.h"



#ifndef DWFCORE_STRING_PREFER_HEAP_LIMIT_IN_BYTES
///
///\ingroup     StringMacros
///\brief       Sets the heap reuse limit.
///
///             If a DWFString heap buffer \e can be reused because
///             the incoming data is smaller than the currently allocated buffer,
///             this value will determine whether or not the buffer \e will be reused
///             or be discarded because it is too big.
///
#define DWFCORE_STRING_PREFER_HEAP_LIMIT_IN_BYTES       128
#define _DWFCORE_STRING_PREFER_HEAP_LIMIT_IN_WCHARS     (DWFCORE_STRING_PREFER_HEAP_LIMIT_IN_BYTES>>(sizeof(wchar_t)>>1))
#endif



namespace DWFCore
{

//
// fwd decl
//
class DWFThreadMutex;


///
///\class           DWFString   dwfcore/String.h  "dwfcore/String.h"
///\brief           A platform-independent Unicode string class.
///\since           1.0.1
///
///                 This string class is used as an unambigious boundary object type
///                 especially where both "narrow" and "wide" character strings are
///                 used.  This class always stores Unicode data in the system default
///                 \e wchar_t character type.  This class will always assume a non-Unicode
///                 character string is encoded UTF-8.
///
///\note            This class \b does \b not perform any character set transcoding
///                 except where explicitly designated and only then the conversion
///                 will be wide Unicode to or from UTF-8.
///\note            This class has platform-specific implementations.
///\ingroup         CrossPlatformImpl
///
class DWFString : virtual public DWFCoreMemory
{

public:

    ///
    ///             Performs fast UTF-8 encoding for 16-bit UCS-2 character strings.
    ///
    ///\param       zUCS2String         The source string to encode as UTF-8.  
    ///                                 The character data type must be 16-bit UCS-2
    ///                                 (or equivalently, 16-bit UTF-16 with \b no surrogate pairs or byte-order marker.)
    ///\param       nUCS2StringBytes    The length of \a zUCS2String in bytes.
    ///\param       pUTF8Buffer         The destintation buffer into which the resultant UTF-8 encoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant UTF-8 encoded string.
    ///\param       nUTF8BufferBytes    The length of \a pUTF8Buffer in bytes.
    ///\return      The actual length (in bytes) of the UTF-8 encoded string written into \a pUTF8Buffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t EncodeUTF8( const UCS2_char_t*    zUCS2String,
                              size_t                nUCS2StringBytes,
                              ASCII_char_t*         pUTF8Buffer,
                              size_t                nUTF8BufferBytes )
        throw( DWFOverflowException );

    ///
    ///             Performs fast UTF-8 encoding for 32-bit UCS-4 character strings.
    ///
    ///\param       zUCS4String         The source string to encode as UTF-8.  
    ///                                 The character data type must be 32-bit UCS-4
    ///                                 (or equivalently, 32-bit UTF-32.)
    ///\param       nUCS4StringBytes    The length of \a zUCS4String in bytes.
    ///\param       pUTF8Buffer         The destintation buffer into which the resultant UTF-8 encoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant UTF-8 encoded string.
    ///\param       nUTF8BufferBytes    The length of \a pUTF8Buffer in bytes.
    ///\return      The actual length (in bytes) of the UTF-8 encoded string written into \a pUTF8Buffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t EncodeUTF8( const UCS4_char_t*    zUCS4String,
                              size_t                nUCS4StringBytes,
                              ASCII_char_t*         pUTF8Buffer,
                              size_t                nUTF8BufferBytes )
        throw( DWFOverflowException );


    ///
    ///             Performs fast UTF-8 decoding into 16-bit UCS-2 character strings.
    ///
    ///\param       zUTF8String         The source string encoded in UTF-8 to decode.
    ///\param       nUTF8StringBytes    The length of \a zUTF8String in bytes.
    ///\param       pUCS2Buffer         The destintation buffer into which the resultant decoded UCS-2 string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant decoded UCS-2 string.
    ///\param       nUCS2BufferBytes    The length of \a pUCS2Buffer in bytes.
    ///\return      The actual length (in bytes) of the decoded UCS-2 string written into \a pUCS2Buffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t DecodeUTF8( const ASCII_char_t*   zUTF8String,
                              size_t                nUTF8StringBytes,
                              UCS2_char_t*          pUCS2Buffer,
                              size_t                nUCS2BufferBytes )
        throw( DWFOverflowException );

    ///
    ///             Performs fast UTF-8 decoding into 32-bit UCS-4 character strings.
    ///
    ///\param       zUTF8String         The source string encoded in UTF-8 to decode.
    ///\param       nUTF8StringBytes    The length of \a zUTF8String in bytes.
    ///\param       pUCS4Buffer         The destintation buffer into which the resultant decoded UCS-4 string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant decoded UCS-4 string.
    ///\param       nUCS4BufferBytes    The length of \a pUCS4Buffer in bytes.
    ///\return      The actual length (in bytes) of the decoded UCS-4 string written into \a pUCS4Buffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t DecodeUTF8( const ASCII_char_t*   zUTF8String,
                              size_t                nUTF8StringBytes,
                              UCS4_char_t*          pUCS4Buffer,
                              size_t                nUCS4BufferBytes )
        throw( DWFOverflowException );


    ///
    ///             Performs fast XML encoding for character strings.
    ///
    ///\param       zASCIIString        The source string to encode for XML.
    ///                                 This string is processed one 8-bit character at a time.
    ///\param       nASCIIStringBytes   The length of \a zASCIIString in bytes.
    ///\param       pXMLBuffer          The destintation buffer into which the resultant XML encoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant XML encoded string.
    ///\param       nXMLBufferBytes     The length of \a pXMLBuffer in bytes.
    ///\return      The actual length (in bytes) of the XML encoded string written into \a pXMLBuffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static size_t EncodeXML( const ASCII_char_t*    zASCIIString,
                             size_t                 nASCIIStringBytes,
                             ASCII_char_t*          pXMLBuffer,
                             size_t                 nXMLBufferBytes )
        throw( DWFException );

    ///
    ///             Performs a restricted, fast XML encoding for wide character strings.
    ///
    ///             This is special method that only encodes explicit characters in the source string:
    ///
    ///             L'<' (less than)    -> L"&lt;"
    ///             L'>' (greater than) -> L"&gt;"
    ///             L'&' (ampersand)    -> L"&amp;"
    ///             L'"' (quote)        -> L"&quot;"
    ///             L''' (apostrophe)   -> L"&apos;"
    ///
    ///             All other source characters are left unchanged.  Care should be taken when using
    ///             this method since it is definitely a special-case algorithm made available
    ///             for those situations when a "reverse-encoding process" \e Unicode \e -> \e Unicode \e XML \e -> \e UTF-8 is required.
    ///
    ///\param       zWideString         The source string to encode for XML.
    ///                                 The character data type must be match the system \e wchar_t type definition.
    ///\param       nWideStringBytes    The length of \a zWideString in bytes.
    ///\param       pXMLBuffer          The destintation buffer into which the resultant "special" XML string will be written.
    ///                                 Note that this is a \e wchar_t character type string.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant string.
    ///\param       nXMLBufferBytes     The length of \a pXMLBuffer in bytes.
    ///\param       bHTMLEncodeNonASCII If true, all non-ascii characters will be html-encoded. In a perfect world, this
    ///                                 should not be necessary. However, the 6.X series of DWF toolkits was incapable
    ///                                 of correctly reading non-ascii UTF-8 characters and required that all such
    ///                                 characters be HTML-encoded. As of spring 2007, the DWF round-tripping loader
    ///                                 in AutoCAD is still using the 6.X library, so we are forced to keep outputting
    ///                                 these characters in this encoding by default.
    ///\return      The actual length (in bytes) of the "special" XML encoded string written into \a pXMLBuffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static size_t EncodeXML( const wchar_t*         zWideString,
                             size_t                 nWideStringBytes,
                             wchar_t*               pXMLBuffer,
                             size_t                 nXMLBufferBytes,
                             bool                   bHTMLEncodeNonASCII = true )
        throw( DWFException );

    ///
    ///             Performs fast XML decoding for character strings.
    ///
    ///\param       zXMLString          The source string with XML character encoding.
    ///                                 This string is processed one 8-bit character at a time.
    ///\param       nXMLStringBytes     The length of \a zXMLString in bytes.
    ///\param       pASCIIBuffer        The destintation buffer into which the resultant decoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant decoded string.
    ///\param       nASCIIBufferBytes   The length of \a pASCIIBuffer in bytes.
    ///\return      The actual length (in bytes) of the decoded string written into \a pASCIIBuffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static size_t DecodeXML( const ASCII_char_t*   zXMLString,
                             size_t                nXMLStringBytes,
                             ASCII_char_t*         pASCIIBuffer,
                             size_t                nASCIIBufferBytes )
        throw( DWFException );

    ///
    ///             Performs fast XML decoding for character strings.
    ///
    ///\param       zXMLString          The source string with XML character encoding.
    ///                                 This string is processed one wide character at a time.
    ///\param       nXMLStringBytes     The length of \a zXMLString in bytes.
    ///\param       pWideStringBuffer   The destintation buffer into which the resultant decoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant decoded string.
    ///\param       nWideStringBufferBytes   The length of \a pWideStringBuffer in bytes.
    ///\return      The actual length (in bytes) of the decoded string written into \a pWideStringBuffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static size_t DecodeXML( const wchar_t*        zXMLString,
                             size_t                nXMLStringBytes,
                             wchar_t*              pWideStringBuffer,
                             size_t                nWideStringBufferBytes )
        throw( DWFException );

    ///
    ///             Performs fast Base64 encoding.
    ///
    ///             This method produces two flavors of encoded output.
    ///             The first, as would be expected, complies with RFC 1521 (Section 5.2)
    ///             and will always bad leftover bytes with '='.
    ///             The second is a modification made for DWF where no byte padding
    ///             is used and the 64th character '/' has been replaced by '_'.
    ///             This was done to allow encoded [UUID - see DWFCore::DWFUUID::uuid(bSquash = true)] strings
    ///             to be used in DWF section path naming.
    ///
    ///\param       pRawBuffer          The source byte buffer to encode.
    ///                                 This string is processed one 8-bit character at a time.
    ///\param       nRawBufferBytes     The length of \a pRawBuffer in bytes.
    ///\param       pOutBuffer          The destintation buffer into which the resultant Base64 encoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant Base64 encoded string.
    ///\param       nOutBufferBytes     The length of \a pOutBuffer in bytes.
    ///\param       bRFC1521Compliant   If \e true, this method will produce a Base64 Content-Transfer-Encoding compliant with the specification
    ///                                 in which it has been detailed, otherwise the encoding will be optimized for DWF usage.
    ///\return      The actual length (in bytes) of the XML encoded string written into \a pOutBuffer.
    ///             This value does not include any null termination bytes.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t EncodeBase64( const void*         pRawBuffer,
                                size_t              nRawBufferBytes,
                                void*               pOutBuffer,
                                size_t              nOutBufferBytes,
                                bool                bRFC1521Compliant = true )
        throw( DWFOverflowException );


    ///
    ///             Performs fast Base64 decoding of base64 strings.
    ///
    ///             This method handles encodings that are RFC1521 Compliant, or those
    ///             that are of the DWF modified form, as produced by DWFString::EncodeBase64
    ///
    ///             All buffers are assummed to be pre-allocated.
    ///
    ///             Note that Output is not null terminated string
    ///
    ///\param       pRawBuffer          input buffer
    ///\param       nRawBufferBytes     input buffer size
    ///\param       pOutBuffer          The destintation buffer into which the resultant decoded string will be written.
    ///                                 If this buffer is NULL, the function will return the number of bytes
    ///                                 that will be required to hold the resultant decoded string.
    ///\param       nOutBufferBytes     The length of \a pOutBuffer in bytes.
    ///\param       bRFC1521Compliant indicates if input buffer was encoded to RFC1521 spec
    ///\return      size of output buffer, 0 if function fails.
    ///\throw       DWFOverflowException
    ///
    _DWFCORE_API
    static size_t DecodeBase64( const void*             pRawBuffer,
                                size_t                  nRawBufferBytes,
                                void*                   pOutBuffer,
                                size_t                  nOutBufferBytes,
                                bool                    bRFC1521Compliant = true)
        throw( DWFOverflowException );

	///
    ///             Perform locale-independent conversion from a string representation to double primitive type.
    ///
    ///             This algorithm will convert a string representation of double precision
    ///             floating-point number into a double precision native type regardless of
    ///             the character used (in the string) to represent the decimal separator.
    ///
    ///\param       zString     The string representation of the number.
    ///\return      The number.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static double StringToDouble( const char* zString )
        throw( DWFException );

    ///
    ///             Perform locale-independent conversion from a wide string representation to double primitive type.
    ///
    ///             This algorithm will convert a wide string representation of double precision
    ///             floating-point number into a double precision native type regardless of
    ///             the character used (in the string) to represent the decimal separator.
    ///
    ///\param       zString     The wide string representation of the number.
    ///\return      The number.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static double StringToDouble( const wchar_t* zString )
        throw( DWFException );

    ///
    ///             Perform locale-independent conversion from a double primitive type to wide string representation.
    ///
    ///             This algorithm will convert double value to wide string representation using sprintf
    ///             regardless of the character used (in the double) to represent the decimal separator.
    ///
    ///\param       dValue     The floating point number to be converted into String
    ///\param       nPrecision  Number of digits to be printed as a precision
    ///\param       nWidth      Number of digits to be printed before decimal point
    ///\return      The number.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static DWFString DoubleToString( double dValue,short nPrecision,short nWidth=-1)
        throw( DWFException );

    ///
    ///             Perform locale-independent conversion from a double primitive type to wide string representation.
    ///
    ///             This algorithm will convert double value to wide string representation using snwprintf
    ///             regardless of the character used (in the double) to represent the decimal separator.
    ///
    ///\param       pBuffer     The buffer in which to write the string
    ///\param       pBufferChars The length of pBuffer in characters
    ///\param       dValue      The floating point number to be converted into String
    ///\param       nPrecision  Number of digits to be printed as a precision
    ///\param       nWidth      Number of digits to be printed before decimal point
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static void DoubleToString( wchar_t* pBuffer, size_t nBufferChars, double dValue,short nPrecision,short nWidth=-1)
        throw( DWFException );

    ///
    ///             Perform locale-independent conversion from a double primitive type to ASCII string representation.
    ///
    ///             This algorithm will convert double value to wide string representation using snprintf
    ///             regardless of the character used (in the double) to represent the decimal separator.
    ///
    ///\param       pBuffer     The buffer in which to write the string
    ///\param       pBufferChars The length of pBuffer in characters
    ///\param       dValue      The floating point number to be converted into String
    ///\param       nPrecision  Number of digits to be printed as a precision
    ///\param       nWidth      Number of digits to be printed before decimal point
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static void DoubleToString( char* pBuffer, size_t nBufferChars, double dValue,short nPrecision,short nWidth=-1)
        throw( DWFException );

    ///
    ///             This function will convert any non-whitespace, non-numeric character
    ///             that occurs between two numeric characters into a decimal point (period) '.'
    ///
    ///\warning     This function will modify the original string.
    ///\note        This function expects \a zString to represent one number.
    ///
    ///\param       zString     The wide string representation of the number.
    ///\return      The repaired string (this will match \a zString.)
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static wchar_t* RepairDecimalSeparators( wchar_t* zString )
        throw( DWFException );

    ///
    ///             This function will convert any non-whitespace, non-numeric character
    ///             that occurs between two numeric characters into a decimal point (period) '.'
    ///
    ///\warning     This function will modify the original string.
    ///\note        This function expects \a zString to represent one number.
    ///
    ///\param       zString     The ASCII string representation of the number.
    ///\return      The repaired string (this will match \a zString.)
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static char* RepairDecimalSeparators( char* zString )
        throw( DWFException );

    ///
    ///             Searches for a character in a string from a given position in the string.
    ///
    ///\param       zString     The string to search.
    ///\param       cFind       The character to search for.
    ///\param       iOffset     The index (inclusive) from which to start the search.
    ///                         For reverse finds this is relative to the end of \a zString.
    ///                         This value must always be greater than or equal to zero.
    ///\param       bReverse    If \e true, the search will start from the end of \a zString.
    ///\return      The index of the first occurence of \a cFind in \a zString after \a iOffset
    ///             or \b -1 if not found.
    ///\throw       DWFInvalidArgumentException
    ///
    _DWFCORE_API
    static off_t Find( const wchar_t*   zString,
                       const wchar_t    cFind,
                       off_t            iOffset = 0,
                       bool             bReverse = false )
        throw( DWFInvalidArgumentException );

    ///
    ///             Searches for a substring in a string from a given position in the string.
    ///
    ///\param       zString     The string to search.
    ///\param       zFind       The substring to search for.
    ///\param       iOffset     The index (inclusive) from which to start the search.
    ///                         For reverse finds this is relative to the end of \a zString.
    ///                         This value must always be greater than or equal to zero.
    ///\param       bReverse    If \e true, the search will start from the end of \a zString.
    ///\return      The index of the first occurence of \a zFind in \a zString after \a iOffset
    ///             or \b -1 if not found.
    ///\throw       DWFInvalidArgumentException
    ///
    _DWFCORE_API
    static off_t Find( const wchar_t*   zString,
                       const wchar_t*   zFind,
                       off_t            iOffset = 0,
                       bool             bReverse = false )
        throw( DWFInvalidArgumentException );

    ///
    ///             Searches for the first occurence of a character in a string.
    ///
    ///\param       zString     The string to search.
    ///\param       cFind       The character to search for.
    ///\return      The index of the first occurence of \a cFind in \a zString or \b -1 if not found.
    ///\throw       None
    ///
    _DWFCORE_API
    static off_t FindFirst( const wchar_t*  zString,
                            const wchar_t   cFind )
        throw();

    ///
    ///             Searches for the last occurence of a character in a string.
    ///
    ///\param       zString     The string to search.
    ///\param       cFind       The character to search for.
    ///\return      The index of the last occurence of \a cFind in \a zString or \b -1 if not found.
    ///\throw       None
    ///
    _DWFCORE_API
    static off_t FindLast( const wchar_t*   zString,
                           const wchar_t    cFind )
        throw();

#ifndef DWFCORE_STRING_DISABLE_FIXED_ASCII

    ///
    ///             Builds a restricted, immutable DWFString with ASCII data.
    ///
    ///             This method should be used with extreme care, if at all.
    ///             Since DWFString is used as the API currency for string data,
    ///             there inevitably arises the situation where raw ASCII, and sometimes MBCS,
    ///             string data must be communicated via a DWFString object.  The very nature
    ///             of this class (and in fact its documentation) demands any ASCII data
    ///             be UTF-8 encoded.  This will corrupt any non US-ASCII wishing to stay as such.
    ///             Fixed ASCII DWFStrings are not as functional as Unicode versions.  These objects
    ///             will throw exceptions when certain methods are used.  Exercise caution when using
    ///             these objects.  It is best to restrict usage to this method as \a getUTF8() which
    ///             will return the raw ASCII data unmodified.
    ///
    ///\param       zASCIIString    The character data to store.  This data may not be modified 
    ///                             for the lifetime of the object.
    ///\return      The restricted, immutable string object.  This pointer must be deleted by the caller
    ///             using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    static DWFString* BuildFixedASCIIString( const char* zASCIIString )
        throw( DWFException );

#endif

public:

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    ~DWFString()
        throw();

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFString()
        throw();

    ///
    ///             Constructor
    ///
    ///             Build content from an ASCII character buffer.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString( const char*      pBuffer,
               size_t           nBufferBytes )
        throw( DWFException );

    ///
    ///             Constructor
    ///
    ///             Build content from an ASCII character string.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       zASCIIString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString( const char*      zASCIIString )
        throw( DWFException );

    ///
    ///             Constructor
    ///
    ///             Build content from a wide character buffer.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString( const wchar_t*   pBuffer,
               size_t           nBufferBytes )
        throw( DWFException );

    ///
    ///             Constructor
    ///
    ///             Build content from a wide character string.
    ///
    ///\param       zWideString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString( const wchar_t*   zWideString )
        throw( DWFException );

    ///
    ///             Constructor
    ///
    ///             Build an empty string with a given pre-allocation size. This will
    ///             cause the string to always have at least that many characters in
    ///             its heap buffer. As the string is used, the heap buffer may grow,
    ///             but will never be reduced in size. This will disable the memory
    ///             reduction optimization specified by \a DWFCORE_STRING_PREFER_HEAP_LIMIT_IN_BYTES.
    ///
    ///\param       nCharsHint       The number of characters to pre-allocate.
    ///\throw       DWFException
    ///\since       1.4.0
    ///
    _DWFCORE_API
    DWFString( size_t nCharsHint )
        throw( DWFException );

    ///
    ///             Copy Constructor
    ///
    ///\param       rDWFString      The string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    DWFString( const DWFString& rDWFString )
        throw( DWFException );

    ///
    ///             Assignment Operator
    ///
    ///\param       rDWFString      The string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString& operator=( const DWFString& rDWFString )
        throw( DWFException );

    ///
    ///             Set the string content from another DWFString.
    ///
    ///\param       rDWFString      The string from which to copy content.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void assign( const DWFString& rDWFString )
        throw( DWFException );

    ///
    ///             Set the string content from an ASCII character buffer.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void assign( const char*    pBuffer,
                         size_t         nBufferBytes )
        throw( DWFException );

    ///
    ///             Set the string content from an ASCII character string.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       zASCIIString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void assign( const char*    zASCIIString )
        throw( DWFException );

    ///
    ///             Build content from a wide character buffer.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void assign( const wchar_t* pBuffer,
                         size_t         nBufferBytes )
        throw( DWFException );

    ///
    ///             Build content from a wide character string.
    ///
    ///\param       zWideString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void assign( const wchar_t* zWideString )
        throw( DWFException );

    ///
    ///             Equality Operator
    ///
    ///\param       rDWFString      The string to compare.
    ///\return      \e true if the content of the two strings are equal, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator==( const DWFString& rDWFString ) const
        throw();

    ///
    ///             Equality Operator
    ///
    ///\param       zWideString     A null-terminated wide character string to compare.
    ///\return      \e true if the content of the two strings are equal, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator==( const wchar_t* zWideString ) const
        throw();

    ///
    ///             Inequality Operator
    ///
    ///\param       rDWFString      The string to compare.
    ///\return      \e true if the content of the two strings differs, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator!=( const DWFString& rDWFString ) const
        throw();

    ///
    ///             Inequality Operator
    ///
    ///\param       zWideString     A null-terminated wide character string to compare.
    ///\return      \e true if the content of the two strings differs, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator!=( const wchar_t* zWideString ) const
        throw();

    ///
    ///             Less Than Operator
    ///
    ///\param       rDWFString      The string to compare.
    ///\return      \e true if the content this object is ordered 'less than' the content of \a rDWFString, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator<( const DWFString& rDWFString ) const
        throw();

    ///
    ///             Less Than Operator
    ///
    ///\param       zWideString     A null-terminated wide character string to compare.
    ///\return      \e true if the content this object is ordered 'less than' \a zWideString, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator<( const wchar_t* zWideString ) const
        throw();

    ///
    ///             Greater Than Operator
    ///
    ///\param       rDWFString      The string to compare.
    ///\return      \e true if the content this object is ordered 'greater than' the content of \a rDWFString, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator>( const DWFString& rDWFString ) const
        throw();

    ///
    ///             Greater Than Operator
    ///
    ///\param       zWideString     A null-terminated wide character string to compare.
    ///\return      \e true if the content this object is ordered 'greater than' \a zWideString, \e false otherwise.
    ///\throw       None
    ///
    _DWFCORE_API
    bool operator>( const wchar_t* zWideString ) const
        throw();

    ///
    ///             Frees all current resources and resets the object.
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    void destroy()
        throw( DWFException );

    ///
    ///             Add content from another DWFString.
    ///
    ///\param       rDWFString      The string from which to copy content.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void append( const DWFString& rDWFString )
        throw( DWFException );

    ///
    ///             Add content from an ASCII character buffer.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void append( const char*    pBuffer,
                 size_t         nBufferBytes )
        throw( DWFException );

    ///
    ///             Add content from an ASCII character string.
    ///             This buffer is assumed to be a valid UTF-8 encoded string
    ///             and will be decoded as such and stored internally as Unicode.
    ///
    ///\param       zASCIIString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void append( const char*    zASCIIString )
        throw( DWFException );

    ///
    ///             Add content from a wide character buffer.
    ///
    ///\param       pBuffer         The buffer from which to build.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void append( const wchar_t* pBuffer,
                 size_t         nBufferBytes )
        throw( DWFException );

    ///
    ///             Add content from a wide character string.
    ///
    ///\param       zWideString    The null-terminated string from which to build.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void append( const wchar_t* zWideString )
        throw( DWFException );

    ///
    ///             Returns the content length as byte count.
    ///
    ///\return      The number of bytes in the string.
    ///
    _DWFCORE_API
    size_t bytes() const
        throw();

    ///
    ///             Returns the content length as wide character count.
    ///
    ///\return      The number of wide characters in the string.
    ///
    _DWFCORE_API
    size_t chars() const
        throw();

    ///
    ///             Returns a UTF-8 encoded representation of the content into the buffer provided.
    ///
    ///             For fixed ASCII restricted DWFString objects, this method will
    ///             return the original ASCII data, unmodified.
    ///
    ///\param       pBuffer         The buffer to hold the UTF-8 encoded string.  
    ///                             If NULL, the method will return the number of bytes required
    ///                             to hold the encoded string.
    ///\param       nBufferBytes    The number of bytes in \a pBuffer.
    ///\return      The number of bytes used in \a pBuffer by the UTF-8 encoded string.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    size_t getUTF8( char*   pBuffer,
                    size_t  nBufferBytes ) const
        throw( DWFException );

    ///
    ///             Returns a UTF-8 encoded representation of the content into
    ///             a newly allocated memory buffer.  The caller must free this
    ///             buffer with the \b DWFCORE_FREE_MEMORY macro.
    ///
    ///             For fixed ASCII restricted DWFString objects, this method will
    ///             return the original ASCII data, unmodified.
    ///
    ///\param       ppBuffer        A pointer to the newly allocated buffer containing
    ///                             the UTF-8 encoded string representation.
    ///\return      The number of bytes used in \a ppBuffer by the UTF-8 encoded string.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    size_t getUTF8( char** ppBuffer ) const
        throw( DWFException );

    ///
    ///             Cast Operator
    ///
    ///\return      A pointer to the internal content.  This pointer will be NULL if the string
    ///             has no content.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    operator const wchar_t*() const
        throw( DWFException );

    ///
    ///             Searches for a character in a string from a given position in the string.
    ///
    ///\param       cFind       The character to search for.
    ///\param       iOffset     The index (inclusive) from which to start the search.
    ///                         For reverse finds this is relative to the end of \a zString.
    ///                         This value must always be greater than or equal to zero.
    ///\param       bReverse    If \e true, the search will start from the end of the content.
    ///\return      The index of the first occurence of \a cFind after \a iOffset
    ///             or \b -1 if not found.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    off_t find( const wchar_t cFind,
                off_t         iOffset = 0,
                bool          bReverse = false ) const
        throw( DWFException );

    ///
    ///             Searches for a substring in a string from a given position in the string.
    ///
    ///\param       zFind       The substring to search for.
    ///\param       iOffset     The index (inclusive) from which to start the search.
    ///                         For reverse finds this is relative to the end of \a zString.
    ///                         This value must always be greater than or equal to zero.
    ///\param       bReverse    If \e true, the search will start from the end of the content.
    ///\return      The index of the first occurence of \a zFind after \a iOffset
    ///             or \b -1 if not found.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    off_t find( const wchar_t* zFind,
                off_t          iOffset = 0,
                bool           bReverse = false ) const
        throw( DWFException );

    ///
    ///             Searches for the first occurence of a character in this string.
    ///
    ///\param       cFind       The character to search for.
    ///\return      The index of the first occurence of \a cFind in or \b -1 if not found.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    off_t findFirst( const wchar_t cFind ) const
        throw( DWFException );

    ///
    ///             Searches for the last occurence of a character in this string.
    ///
    ///\param       cFind       The character to search for.
    ///\return      The index of the last occurence of \a cFind in or \b -1 if not found.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    off_t findLast( const wchar_t cFind ) const
        throw( DWFException );

    ///
    ///             Creates a substring.
    ///
    ///\param       iPos        The index (inclusive) of first character in the new string.
    ///\param       iLen        The number of characters to copy into the new string, or \b -1
    ///                         to copy to end of string.
    ///\return      The new substring.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFString substring( size_t iPos,
                         size_t iLen = (size_t) -1 ) const
         throw( DWFException );

private:

    void _store( const void*    pBuffer,
                 size_t         nBufferBytes,
                 bool           bDecodeUTF8 = false,
                 bool           bFixedASCII = false )
        throw( DWFException );

    void _append( const void*   pBuffer,
                  size_t        nBufferBytes,
                  bool          bDecodeUTF8 = false )
        throw( DWFException );

    _DWFCORE_API
	void _affix()
        throw( DWFException );

private:

    wchar_t*    _pHeapBuffer;
    size_t      _nBufferChars;
    size_t      _nDataChars;

#ifndef DWFCORE_STRING_DISABLE_FIXED_ASCII

    bool        _bDataIsFixedASCII;

#endif

    bool        _bRetainHeapBuffer;

	typedef struct __tBlock
	{
		struct __tBlock* _next;
		wchar_t* _buffer;
		size_t size;
	public:
        __tBlock()
            : _next( NULL )
            , _buffer( NULL )
			, size(0)
        {;}
	} _tBlock;
	_tBlock* _pHead, *_pTail;
	size_t _size;
};

//
//
//

inline
size_t
DWFString::bytes()
const
throw()
{
	//
	// Need to resolve any append fragments before examining character counts.
	//
	const_cast<DWFString*>(this)->_affix();

#ifndef DWFCORE_STRING_DISABLE_FIXED_ASCII
    if (_bDataIsFixedASCII)
    {
        return _nDataChars;
    }
    else
#endif
    {
        return (_nDataChars * sizeof(wchar_t));
    }
}

inline
size_t
DWFString::chars()
const
throw()
{
	//
	// Need to resolve any append fragments before examining character counts.
	//
	const_cast<DWFString*>(this)->_affix();

    return _nDataChars;
}

inline
DWFString::operator const wchar_t*()
const
throw( DWFException )
{
        //
        // Protect against someone calling us with a null pointer.
        //
    if(this==NULL)
    {
        return NULL;
    }

	//
	// Need to resolve any append fragments before checking stack/heap.
	//
    DWFString* pThis = const_cast<DWFString*>(this);
    pThis->_affix();

#ifndef DWFCORE_STRING_DISABLE_FIXED_ASCII
    if (_bDataIsFixedASCII)
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This method is not available for fixed ASCII strings." ); 
    }
#endif

        //
        // no data
        //
    return _pHeapBuffer;
}

class DWFStringTable
{
public:
	///
    ///         Returns the single instance of the lookup table.
    ///
	///\return  The singleton of DWFStringTable
    ///\throw   Any
    ///
	_DWFCORE_API static DWFStringTable& Instance();

    ///
    ///         Destroys all string data from the table.
    ///
    _DWFCORE_API static void Purge();

	///
    ///         Insert a string into lookup table and return an index
    ///
	///\param   zText inserted string
	///\return  The index of inserted string
    ///\throw   Any
    ///
	_DWFCORE_API const DWFString* insert( const DWFString& zText );

private:
    struct _Less
	{
		bool operator()(const DWFString* lhs, const DWFString* rhs) const
		{
			return (*lhs) < (*rhs);
		}
	};

	DWFStringTable();
  void _init();
#if defined(__BCPLUSPLUS__)
public:
#endif
	~DWFStringTable();
  void _uninit();

private:
	DWFStringTable (const DWFStringTable&);
	DWFStringTable& operator=( const DWFStringTable&);			

private:
	std::deque<DWFString> _oTable;

	typedef std::set< const DWFString*, _Less >  _tIndex;
	_tIndex              _oIndex;

    DWFThreadMutex*         _pMutex;

};

}

#endif
