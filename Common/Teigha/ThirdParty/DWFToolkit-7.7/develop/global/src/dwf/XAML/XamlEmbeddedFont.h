//
//  Copyright (c) 2006 by Autodesk, Inc.
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

#if !defined XAML_EMBEDDED_FONT_HEADER
#define XAML_EMBEDDED_FONT_HEADER

///
///\file        XAML/XamlEmbeddedFont.h
///

#include "XAML/XamlCore.h"

#include "whiptk/embedded_font.h"


/** \addtogroup groupRenditionAttributes
*  @{
*/

/// Rendition attribute describing the current text font.
/** \sa WT_Rendition \sa WT_Text \sa \ref groupFontOptions "Font options"
*
*
*  \sa See the embedded fonts example provided in the sample app \em W2DTest accompanying the WHIP!
*  Toolkit source distribution.
*/
class XAMLTK_API WT_XAML_Embedded_Font 
    : public WT_Embedded_Font
{
    friend class WT_XAML_Class_Factory;
    friend class WT_XAML_Rendition;

protected:
    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_Embedded_Font object.
    WT_XAML_Embedded_Font ()
        : WT_Embedded_Font()
    { }

    /// Constructs a WT_XAML_Embedded_Font object with the given data.
    WT_XAML_Embedded_Font(
        /// The embedding request value (a combination of zero or more values in \ref WT_Embedded_Font::WT_Request_Format "enum WT_Request_Format".)
        int                            request_type,
        /// The embedding privilege value (one of the values in \ref WT_Privilege "enum WT_Privilege".)
        int                            privilege_type,
        /// Sets the character code value (one of the values in \ref WT_Character_Set "enum WT_Character_Set".)
        int                            character_set_type,
        /// The size of the embedded font data in bytes.
        WT_Integer32                   data_size,
        /// A buffer containing the embedded font data.
        WT_Byte *                      data,
        /// The length of the cannonical font name string specified by the \a font_type_face_name_string buffer.
        WT_Integer32                   font_type_face_name_length,
        /// A buffer holding the cannonical font name string (ASCII).
        WT_Byte *                      font_type_face_name_string,
        /// The length of the logfont name string specified by the \a font_logfont_name_string buffer.
        WT_Integer32                   font_logfont_name_length,
        /// A buffer holding the logfont name string (ASCII).
        WT_Byte *                      font_logfont_name_string,
        /// Whether the data should be copied or if its content should be used directly from the array.
        WT_Boolean                     copy
        ) throw(WT_Result)
        : WT_Embedded_Font( request_type, privilege_type, character_set_type, data_size, data, 
                            font_type_face_name_length, font_type_face_name_string,
                            font_logfont_name_length, font_logfont_name_string, copy )
    {
    }

    /// Destroys a WT_XAML_Embedded_Font object.
    virtual ~WT_XAML_Embedded_Font()
    {
    }

private:
    /// Assignment operator.  Copies the given font settings to this one.
    WT_XAML_Embedded_Font const & operator=  (WT_XAML_Embedded_Font const &)
    {
        WD_Complain( "Cannot assign this object!" );
        return *this;
    }

public:

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result serialize (WT_File & file) const;
    //@}

    WT_Result parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

};
//@}

#endif //XAML_EMBEDDED_FONT_HEADER
