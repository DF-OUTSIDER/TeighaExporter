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
#if !defined WTSTRING_HEADER
#define WTSTRING_HEADER

///
///\file        dwf/whiptk/wtstring.h
///

#include "whiptk/whipcore.h"

#ifdef HAVE_CONFIG_H
#include "dwf/config.h"
#endif


class WT_File;

/// A generic string class which supports both ascii and wide characters.
class WHIPTK_API WT_String
{
    /// States used during materialization.
    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Getting_String_Type,
        Getting_String,
        Getting_String_Length,
        Getting_String_Data,
        Getting_End_Quote
    } m_stage;

    /// Flags indicating the type of the string.
    enum
    {
        Not_Yet_Scanned,
        Only_ASCII,
        Non_ASCII
    } m_string_type;

    WT_Integer32                m_length;
    WT_Unsigned_Integer16*      m_string;
    WT_Byte*                    m_ascii_string;
    WT_Byte                     m_lead_byte;


public:

    /// Static empty string
    static const    WT_String kNull;

    /// A platform-independent implementation of wcslen for 16-bit characters.
    /// Note that we return 'int', not size_t.
    static int wcslen (WT_Unsigned_Integer16 const * string);

    //@{ \name Construction / desctruction
    /// Constructs a WT_String object.
    WT_String()
    : m_stage(Eating_Initial_Whitespace)
    , m_string_type(Not_Yet_Scanned)
    , m_length(0)
    , m_string(WD_Null)
    , m_ascii_string(WD_Null)
    , m_lead_byte (0)
    {}

    /// Constructs a WT_String object.  Copy constructor.
    WT_String( WT_String const & string ) throw(WT_Result);
    /// Constructs a WT_String object from a null-terminated wide character string (i.e. Unicode.)
    WT_String( WT_Unsigned_Integer16 const * string ) throw(WT_Result);
    /// Constructs a WT_String object from a null-terminated ASCII character string.
    WT_String( WT_Byte const * string ) throw(WT_Result);
    /// Constructs a WT_String object from a null-terminated ASCII character string.
    WT_String( char const * string ) throw(WT_Result);
    /// Constructs a WT_String object from a wide character string (i.e. Unicode) buffer for the specified length.
    WT_String( int length, WT_Unsigned_Integer16 const * string ) throw(WT_Result);
    /// Constructs a WT_String object from an ASCII character string buffer for the specified length.
    WT_String( int length, WT_Byte const * string ) throw(WT_Result);
    /// Constructs a WT_String object from an ASCII character string buffer for the specified length.
    WT_String( int length, char const * string ) throw(WT_Result);
    /// Destroys a WT_String object.
    ~WT_String();
    //@}

    //@{ \name WT_Object methods
    /// Writes the WT_String to a file.
    /** \retval WT_Result::Success The operation was successful.
     */
    WT_Result serialize(
        WT_File & file, /**< The file to write to. */
        WT_Boolean force_quotes=WD_False /**< Add "quotation marks" around the string. */
        ) const;

    /// Reads a string from a file.
    /** \retval WT_Result::Success The operation was successful.
     */
    WT_Result materialize(
        WT_File & file /**< The file to read from. */
        );
    //@}

    //@{ \name Get / set methods
    /// Returns an ASCII version of the string.
    char const *                   ascii()     const ;
    /// Returns the string length (in characters.)
    int                            length()    const { return m_length; }
    /// Returns a wide character (Unicode) version of the string.
    WT_Unsigned_Integer16 const *  unicode()   const ;
    /// Wide character type cast operator - returns a wide character (Unicode) version of the string.
    operator const WT_Unsigned_Integer16 * ()  const { return unicode();}
    /// Sets the string from a wide character string (i.e. Unicode) buffer for the specified length.
    WT_Result               set( int length, WT_Unsigned_Integer16 const * string );
    /// Sets the string from a null-terminated ASCII character string buffer for the specified length.
    WT_Result               set( int length, WT_Byte const * string );
    /// Sets the string from a null-terminated ASCII character string buffer for the specified length.
    WT_Result               set( int length, char const * string );
    /// Sets the string from another (performs a copy).
    WT_Result               set( WT_String const & string );
    /// Assignment operator.  Sets the string from a null-terminated wide character string (i.e. Unicode.)
    WT_String const &  operator= ( WT_Unsigned_Integer16 const * string ) throw(WT_Result);
    /// Assignment operator.  Sets the string from a null-terminated ASCII character string.
    WT_String const &  operator= ( WT_Byte const * string ) throw(WT_Result);
    /// Assignment operator.  Sets the string from a null-terminated ASCII character string.
    WT_String const &  operator= ( char const * string ) throw(WT_Result);
    /// Assignment operator.  Sets the string from another WT_String object.
    WT_String const &  operator= ( WT_String const & string ) throw(WT_Result);
    //@}

    //@{ \name Comparison methods
    /// Equality operator.  Returns WD_True if the strings are equal.
    WT_Boolean         operator== ( char const * string )      const;
    /// Equality operator.  Returns WD_True if the strings are equal.
    WT_Boolean         operator== ( WT_String const & string ) const;
    /// Inequality operator.  Returns WD_True if the strings are NOT equal.
    WT_Boolean         operator!= ( char const * string )      const { return !(*this==string); }
    /// Inequality operator.  Returns WD_True if the strings are NOT equal.
    WT_Boolean         operator!= ( WT_String const & string ) const { return !(*this==string); }
    /// Equality method with case sensitivity toggle.  Returns WD_True if the strings are equal.
    WT_Boolean         equals(char const * string, WT_Boolean case_sensitive=WD_True ) const;
    /// Equality method with case sensitivity toggle.  Returns WD_True if the strings are equal.
    WT_Boolean         equals(WT_String const & string, WT_Boolean case_sensitive=WD_True ) const;
    /// Equality method with no case sensitivity.  Returns WD_True if the strings are equal (regardless of case.)
    WT_Boolean         equals_no_case(char const * string ) const;
    /// Equality method with no case sensitivity.  Returns WD_True if the strings are equal (regardless of case.)
    WT_Boolean         equals_no_case(WT_String const & string ) const;
    /// Returns true if the string is an ASCII string (contains no Unicode characters.)
    WT_Boolean         is_ascii() const;
    //@}

#ifdef WHIP_USE_WCHAR_STRINGS
    //@{ \name wchar_t methods
    /// Constructs a WT_String object from a wide character string (i.e. Unicode) buffer for the specified length.
    /** \note On systems where wchar_t is the same as WT_Unsigned_Integer16, this stores a
     *  simple copy of the input string. On other systems, the input string is translated
     *  to the appropriate UTF representation. */
    WT_String( wchar_t const * string ) throw(WT_Result);
    /// Sets the string from a wide character string (i.e. Unicode) buffer for the specified length.
    /** \note On systems where wchar_t is the same as WT_Unsigned_Integer16, this stores a
     *  simple copy of the input string. On other systems, the input string is translated
     *  to the appropriate UTF representation. */
    WT_Result               set( int length, wchar_t const * string );
    /// Assignment operator.  Sets the string from a null-terminated wide character string (i.e. Unicode.)
    /** \note On systems where wchar_t is the same as WT_Unsigned_Integer16, this stores a
     *  simple copy of the input string. On other systems, the input string is translated
     *  to the appropriate UTF representation. */
    WT_String const & operator= ( wchar_t const * string ) throw(WT_Result);
    //@}
#endif


    //@{ \name Static comparison methods
    /// Returns true if the string buffer is an ASCII string (contains no Unicode characters.)
    static WT_Boolean  is_ascii( int length, WT_Unsigned_Integer16 const * string );
    /// Returns true if the string buffer is an ASCII string (contains no Unicode characters.)
    static WT_Boolean  is_ascii( int length, char const * string );
    //@}

    //@{ \name Static conversion methods
    /// Creates a wide character (Unicode) string from an ASCII buffer.
    /** The low-byte of each wide character is taken from the relative ASCII character,
     *  and the high-byte is zero.
     *  \warning Client is responsible for calling delete[] on the returned pointer.
     *  \retval WT_Unsigned_Integer16* The new wide string.
     */
    static WT_Unsigned_Integer16 * to_unicode(
        int length, /**< The length of the string in characters. */
        WT_Byte const * string /**< The ASCII buffer to copy from. */
        ) throw(WT_Result);

    /// Creates a wchar_t (portable Unicode) string from a wide (Unicode) string.
    /** On systems where wchar_t is the same as WT_Unsigned_Integer16, this returns a
     *  simple copy of the input string. On other systems, the input string is translated
     *  to the appropriate wchar_t representation.
     *  \warning Client is responsible for calling delete[] on the returned pointer.
     *  \retval wchar_t* The new wide string.
     */
    static wchar_t * to_wchar(
        int length, /**< The length of the string in characters. */
        WT_Unsigned_Integer16 const * string /**< The 16 bit wide character (Unicode) buffer to copy from. */
        ) throw(WT_Result);

    /// Creates an ASCII string from a wide (Unicode) string.
    /** The low-byte of each wide character is copied to the relative ASCII character.
     *  \warning Client is responsible for calling delete[] on the returned pointer.
     *  \retval WT_Byte* The new ASCII string.
     */
    static WT_Byte *               to_ascii(
        int length, /**< The length of the string in characters. */
        WT_Unsigned_Integer16 const * string /**< The wide character (Unicode) buffer to copy from. */
        ) throw(WT_Result);

    /// Takes a hex character digit and returns the binary value (0-15).
    static WT_Byte                 to_binary(
        WT_Byte ascii_hex_digit /**< Hex character ('0'-'9', 'A'-'F') */
        );
    //@}

private:
    WT_Boolean                     restore();
    WT_Boolean                     expand();


};

#endif // WTSTRING_HEADER
