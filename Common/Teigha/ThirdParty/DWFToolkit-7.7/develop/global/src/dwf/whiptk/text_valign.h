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

#if !defined TEXT_VALIGN_HEADER
#define TEXT_VALIGN_HEADER

///
///\file        dwf/whiptk/text_valign.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

// Attribute class for specifying text vertical alignment
class WHIPTK_API WT_Text_VAlign : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    enum WT_VAlign
    {
        Descentline = 0,   // descentline aligned with insertion point
        Baseline    = 1,   // baseline aligned with insertion point
        Halfline    = 2,   // halfline aligned with insertion point
        Capline     = 3,   // capline aligned with insertion point
        Ascentline  = 4    // ascentline aligned with insertion point
    };

public:
    // constructs a WT_Text_VAlign object
    WT_Text_VAlign();

    // constructs a WT_Text_VAlign object with the given alignment
    WT_Text_VAlign(WT_VAlign vAlign);

	///Copy Constructor
	WT_Text_VAlign(WT_Text_VAlign const& vAlign)
	{
		*this = vAlign;
	}

    // destructor
    virtual ~WT_Text_VAlign();

public:
    // returns the vertical alignment
    inline const WT_VAlign& vAlign() const { return m_eAlignment; }
    inline WT_VAlign&       vAlign() { return m_eAlignment; }
    inline WT_Text_VAlign const& operator=( WT_VAlign& eAlign ) { m_eAlignment = eAlign; return *this; }

    // WT_Attribute virtual methods
    WT_Boolean operator==(WT_Attribute const & attrib) const;

    // WT_Object virtual methods
    WT_Result      materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID          object_id() const;
    WT_Result      process(WT_File & file);
    WT_Result      serialize(WT_File & file) const;
    WT_Result      skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result      sync(WT_File &) const;
    // provide a default action handler for this object
    static WT_Result default_process(
        WT_Text_VAlign & item, // The object to process.
        WT_File & file         // The file being read.
        );

protected:
    // static methods
    static const char*     enum_to_string(WT_VAlign eVAlign);
    static WT_Boolean      string_to_enum(const char* pStr, WT_VAlign& outEnum);

private:
    // data members
    WT_VAlign m_eAlignment;

private:
    // const char strings
    static const char* str_VAlign_Descentline;
    static const char* str_VAlign_Baseline;
    static const char* str_VAlign_Halfline;
    static const char* str_VAlign_Capline;
    static const char* str_VAlign_Ascentline;
};


#endif //TEXT_VALIGN_HEADER
// endif
