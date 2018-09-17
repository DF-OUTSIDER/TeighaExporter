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

#if !defined EMBEDDED_FONT_HEADER
#define EMBEDDED_FONT_HEADER

///
///\file        dwf/whiptk/embedded_font.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/object.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Contains an embedded font as defined and/or created by the Microsoft OpenType Font Embedding SDK.
/** \note The data in this object is only briefly documented here as it is necessary for WHIP! authors
 *  creating embedded fonts to licence the Microsoft OpenType Font Embedding SDK from Microsoft.
 *  The documentation therein explains in detail how to create embedded fonts.  The structures, variables
 *  and enumerations in this object track very closely with the embedding SDK and thus
 *  copious detail is herein omitted.
 *
 *  <b> An example of creating embedded fonts is provided in the sample app \em W2DTest accompanying the WHIP!
 *  Toolkit source distribution.  It is too lengthy to be included here in this API doc. </b>
 * 
 *  \deprecated Clients should embed font subsets in the DWF package, rather than in a W2D stream
 */
class WHIPTK_API WT_Embedded_Font : public WT_Object
{
    friend class WT_W2D_Class_Factory;
    friend class WT_Opcode;

public:
    /// Font embedding request values.
    enum WT_Request_Format
    {
        /// The font structure contains the full character set, non-compressed. This is the default behavior.
        Raw                           = 0x00000001,
        /// The font structure is a subset containing only the glyphs indicated. The character codes are denoted as 16-bit Unicode values.
        Subset                        = 0x00000002,
        /// The font structure is compressed.
        Compressed                    = 0x00000004,
        /// In some cases, a client will want to avoid embedding simulated fonts, especially if the normal instance of the typeface is being embedded. If this flag is set and the font in the DC is simulated, TTEmbedFont( ) will fail, generating the error E_FONTVARIATIONSIMULATED.
        Fail_If_Variations_Simulated  = 0x00000008,
        /// Embed EUDC font. If there is typeface EUDC embed it; otherwise embed system EUDC.
        Eudc                          = 0x00000010,
        /// The validity of font file should be confirmed before embedding.
        Validation_Tests              = 0x00000020,
        /// Not used.
        Web_Object                    = 0x00000040,
        /// Font data in the embedded object was encrypted when compression was being performed.
        Encrypt_Data                  = 0x00000080
    };

    /// Font embedding character set values.
    enum WT_Character_Set
    {
        /// Unicode character set, requiring 16-bit character encoding.
        Unicode                       = 0x00000001,
        /// Symbol character set, requiring 16-bit character encoding.
        Symbol                        = 0x00000002,
        /// Indicates that subset values passed are to be interpreted as glyph id’s (rather than unicode values).
        Glyphidx                      = 0x00000003
    };

    /// The license granted to the font embedding app.
    enum WT_Privilege
    {
        /// Preview and Print Embedding.
        PreviewPrint                  = 0x00000001,
        /// Editable Embedding.
        Editable                      = 0x00000002,
        /// Installable Embedding.
        Installable                   = 0x00000003,
        /// Restricted License Embedding.
        Non_Embedding                 = 0x00000004
    };

private:

    WT_Unsigned_Integer32   m_request;
    WT_Byte                 m_privilege;
    WT_Byte                 m_character_set_type;
    WT_Integer32            m_font_type_face_name_length;
    WT_Byte *               m_font_type_face_name_string;
    WT_Integer32            m_font_logfont_name_length;
    WT_Byte *               m_font_logfont_name_string;
    WT_Integer32            m_data_size;
    WT_Byte *               m_data;

protected:
    WT_Boolean              m_local_data_copy;
private:

    enum
    {
        Starting,
        Getting_Request_Type,
        Getting_Privilege_Type,
        Getting_Character_Set_Type,
        Getting_Font_Type_Face_Name_Length,
        Getting_Font_Type_Face_Name_String,
        Getting_Font_LogFont_Name_Length,
        Getting_Font_LogFont_Name_String,
        Getting_Pre_Data_Size_Whitespace,
        Getting_Pre_Data_Size_Open_Paren,
        Getting_Data_Size,
        Getting_Data,
        Getting_Close
    }   m_stage;

    WT_Embedded_Font (WT_Embedded_Font const &)
    : WT_Object()
    , m_request(Raw)
    , m_privilege(0)
    , m_character_set_type(0)
    , m_font_type_face_name_length(0)
    , m_font_type_face_name_string(WD_Null)
    , m_font_logfont_name_length(0)
    , m_font_logfont_name_string(WD_Null)
    , m_data_size(0)
    , m_data(WD_Null)
    , m_local_data_copy(WD_False)
    , m_stage(Starting)
    {
        WD_Complain ("cannot copy WT_Embedded_Font");
    } // prohibited

    WT_Embedded_Font operator= (WT_Embedded_Font const &)
    {
        WD_Complain ("cannot assign WT_Embedded_Font");
        return *this;
    } // prohibited

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Embedded_Font object.
    WT_Embedded_Font ()
    : m_request(Raw)
    , m_privilege(0)
    , m_character_set_type(0)
    , m_font_type_face_name_length(0)
    , m_font_type_face_name_string(WD_Null)
    , m_font_logfont_name_length(0)
    , m_font_logfont_name_string(WD_Null)
    , m_data_size(0)
    , m_data(WD_Null)
    , m_local_data_copy(WD_False)
    , m_stage(Starting)
    { }

    /// Constructs a WT_Embedded_Font object with the given data.
    WT_Embedded_Font (
        int request_type,
        int privilege_type,
        int character_set_type)
    : m_request(request_type)
    , m_privilege(static_cast<WT_Byte>(privilege_type))
    , m_character_set_type(static_cast<WT_Byte>(character_set_type))
    , m_font_type_face_name_length(0)
    , m_font_type_face_name_string(WD_Null)
    , m_font_logfont_name_length(0)
    , m_font_logfont_name_string(WD_Null)
    , m_data_size(0)
    , m_data(WD_Null)
    , m_local_data_copy(WD_False)
    , m_stage(Starting)
    { }

    /// Constructs a WT_Embedded_Font object with the given data.
    WT_Embedded_Font(
        /// The embedding request value (a combination of zero or more values in \ref WT_Request_Format "enum WT_Request_Format".)
        int                            request_type,
        /// The embedding privilege value (one of the values in \ref WT_Privilege "enum WT_Privilege".)
        int                            privilege_type,
        /// Sets the character code value (one of the values in \ref WT_Character_Set "enum WT_Character_Set".)
        int                            character_set_type,
        /// The size of the embedded font data in bytes.
        WT_Integer32                   data_size,
        /// A buffer containing the embedded font data.
        WT_Byte *                      data,
        /// Whether the data should be copied or if its content should be used directly from the array.
        WT_Boolean                     copy
        ) throw(WT_Result)
    : m_request(request_type)
    , m_privilege(static_cast<WT_Byte>(privilege_type))
    , m_character_set_type(static_cast<WT_Byte>(character_set_type))
    , m_font_type_face_name_length(0)
    , m_font_type_face_name_string(WD_Null)
    , m_font_logfont_name_length(0)
    , m_font_logfont_name_string(WD_Null)
    , m_data_size(data_size)
    , m_data(WD_Null)
    , m_local_data_copy(copy)
    , m_stage(Starting)
    {
        if (copy)
        {
            if (m_local_data_copy)
            {
                m_data = new WT_Byte[data_size];

                if (m_data)
                    memcpy(m_data, data, data_size);
                else
                    throw WT_Result::Out_Of_Memory_Error;

            }
        }  else {
            m_data = data;
        }
    }

    /// Constructs a WT_Embedded_Font object with the given data.
    WT_Embedded_Font(
        /// The embedding request value (a combination of zero or more values in \ref WT_Request_Format "enum WT_Request_Format".)
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
    : m_request(request_type)
    , m_privilege(static_cast<WT_Byte>(privilege_type))
    , m_character_set_type(static_cast<WT_Byte>(character_set_type))
    , m_font_type_face_name_length(font_type_face_name_length)
    , m_font_type_face_name_string(WD_Null)
    , m_font_logfont_name_length(font_logfont_name_length)
    , m_font_logfont_name_string(WD_Null)
    , m_data_size(data_size)
    , m_data(WD_Null)
    , m_local_data_copy(copy)
    , m_stage(Starting)
    {
        if (copy)
        {
            if (m_local_data_copy)
            {
                m_data = new WT_Byte[data_size];

                if (m_data)
                    memcpy(m_data, data, data_size);
                else
                    throw WT_Result::Out_Of_Memory_Error;

                m_font_type_face_name_string =
                    new WT_Byte[font_type_face_name_length];

                m_font_logfont_name_string =
                    new WT_Byte[font_logfont_name_length];

                if(m_font_type_face_name_string)
                    strncpy((char *)m_font_type_face_name_string
                    , (const char *)font_type_face_name_string
                    , font_type_face_name_length);
                else
                    throw WT_Result::Out_Of_Memory_Error;

                if(m_font_logfont_name_string)
                    strncpy((char *)m_font_logfont_name_string
                    , (const char *)font_logfont_name_string
                    , font_logfont_name_length);
                else
                    throw WT_Result::Out_Of_Memory_Error;

            }
        }  else {
            m_data = data;
            m_font_type_face_name_string = font_type_face_name_string;
            m_font_logfont_name_string = font_logfont_name_string;
        }
    }

    /// Destroys a WT_Embedded_Font object.
    virtual ~WT_Embedded_Font()
    {
        if (m_local_data_copy) {
            delete [] m_data;
            delete [] m_font_type_face_name_string;
            delete [] m_font_logfont_name_string;
        }
    }
    //@}

public:

    //@{ \name Data access methods
    /// Returns the character set type (one of the values in \ref WT_Character_Set "enum WT_Character_Set".)
    const WT_Byte& character_set_type() const { return m_character_set_type;  }
          WT_Byte& character_set_type()       { return m_character_set_type;  }

    /// Returns a read-only pointer to the raw font data.
    WT_Byte * const & data() const { return m_data;  }
    WT_Byte *       & data()       { return m_data;  }

    /// Returns the size (in bytes) of the raw font data.
    const WT_Integer32& data_size() const { return m_data_size; }
          WT_Integer32& data_size()       { return m_data_size; }

    /// The length of the cannonical font name string.
    const WT_Integer32& font_type_face_name_length() const { return m_font_type_face_name_length; }
          WT_Integer32& font_type_face_name_length()       { return m_font_type_face_name_length; }

    /// A buffer holding the cannonical font name string (ASCII).
    WT_Byte * const & font_type_face_name_string() const { return m_font_type_face_name_string;  }
    WT_Byte *       & font_type_face_name_string()       { return m_font_type_face_name_string;  }

    /// The length of the logfont name string.
    const WT_Integer32& font_logfont_name_length() const { return m_font_logfont_name_length; }
          WT_Integer32& font_logfont_name_length()       { return m_font_logfont_name_length; }

    /// A buffer holding the logfont name string (ASCII).
    WT_Byte * const & font_logfont_name_string() const { return m_font_logfont_name_string;  }
    WT_Byte *       & font_logfont_name_string()       { return m_font_logfont_name_string;  }

    /// The embedding privilege value (one of the values in \ref WT_Privilege "enum WT_Privilege".)
    const WT_Byte& privilege() const { return m_privilege;  }
          WT_Byte& privilege()       { return m_privilege;  }

    /// The embedding request value (a combination of zero or more values in \ref WT_Request_Format "enum WT_Request_Format".)
    const WT_Unsigned_Integer32& request_type() const { return m_request;  }
          WT_Unsigned_Integer32& request_type()       { return m_request;  }
    //@}

    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Embedded_Font & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // EMBEDDED_FONT_HEADER
