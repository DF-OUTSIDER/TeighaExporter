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

#if !defined TEXT_BACKGROUND_HEADER
#define TEXT_BACKGROUND_HEADER

///
///\file        dwf/whiptk/text_background.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

// Attribute class for specifying text background
class WHIPTK_API WT_Text_Background : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    enum eBackground
    {
        None    = 0,   // no text background
        Ghosted = 1,   // ghosted text background
        Solid   = 2    // solid text background
    };

public:
    // constructs a WT_Text_Background object
    WT_Text_Background();

    // constructs a WT_Text_Background object with the given background style and color
    WT_Text_Background(WT_Text_Background::eBackground background, 
                       WT_Integer32 offset);

	//Copy Constructor
	WT_Text_Background(WT_Text_Background const& backgrnd)
	{
		*this = backgrnd;
	}

    // destructor
    virtual ~WT_Text_Background();

public:
    // returns the background style
    inline const eBackground& background() const { return m_eBackground; }
    inline eBackground&       background()       { return m_eBackground; };

    // returns the offset
    inline const WT_Integer32& offset() const { return m_offset; }
    inline WT_Integer32&       offset()       { return m_offset; }

    // WT_Attribute virtual methods
    WT_Boolean operator==(WT_Attribute const & attrib) const;

    // WT_Object virtual methods
    WT_Result      materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID          object_id() const;
    WT_Result      process(WT_File & file);
    WT_Result      serialize(WT_File & file) const;
    WT_Result      skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result      sync(WT_File & file) const;
    // provide a default action handler for this object
    static WT_Result default_process(
        WT_Text_Background & item, // The object to process.
        WT_File & file             // The file being read.
        );
protected:
    // static methods 
    static const char*     enum_to_string(eBackground background);
    static WT_Boolean      string_to_enum(const char* pStr, eBackground& outBackground);

private:
    // Data members 
    eBackground m_eBackground;
    WT_Integer32 m_offset;

private:
    static const char* str_Background_None;
    static const char* str_Background_Ghosted;
    static const char* str_Background_Solid;
};


#endif //TEXT_BACKGROUND_HEADER
//endif
