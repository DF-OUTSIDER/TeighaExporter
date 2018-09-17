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

#if !defined FONT_EXTENSION_HEADER
#define FONT_EXTENSION_HEADER

///
///\file        dwf/whiptk/font_extension.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute containing additional information for a font.
/** \sa WT_Rendition \sa WT_Font
 *  \sa See the embedded fonts example provided in the sample app \em W2DTest accompanying the WHIP!
 *  Toolkit source distribution.
 */
class WHIPTK_API WT_Font_Extension : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_String   m_logfont_name;
    WT_String   m_cannonical_name;

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Gathering_LogFont_Name,
        Eating_Middle_Whitespace,
        Gathering_Cannonical_Name,
        Eating_End_Whitespace
    }   m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_Font_Extension object.
    WT_Font_Extension()
    {  m_stage = Eating_Initial_Whitespace; }

    /// Constructs a WT_Font_Extension object with the given data.
    WT_Font_Extension(
        WT_String logfont_name, /**< The font name as it would appear in a LOGFONT structure. */
        WT_String cannonical_name /**< The cannonical font name.*/
        )
    {
        m_logfont_name = logfont_name;
        m_cannonical_name = cannonical_name;
        m_stage = Eating_Initial_Whitespace;
    }

	virtual ~WT_Font_Extension()
	{}
    //@}

public:
	//@{ \name Data access methods
    /// Returns the cannonical font name.
    WT_String const& cannonical_name() const {    return m_cannonical_name;    }
    /// Returns the LOGFONT name.
    WT_String const& logfont_name() const {    return m_logfont_name;    }
    /// Sets the cannonical font name.
    void set_cannonical_name(WT_String cannonical_name) {    m_cannonical_name = cannonical_name;    }
    /// Sets the font name as it would appear in a LOGFONT structure.
    void set_logfont_name(WT_String logfont_name) {    m_logfont_name = logfont_name;    }

    WT_Font_Extension const & operator=  (WT_Font_Extension const & font)
    {
        set_cannonical_name( font.m_cannonical_name );
        set_logfont_name( font.m_logfont_name );
        return *this;
    }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Font_Extension & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};
//@}

#endif // FONT_EXTENSION_HEADER
