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

#if !defined TEXT_HALIGN_HEADER
#define TEXT_HALIGN_HEADER

///
///\file        dwf/whiptk/text_halign.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

// Attribute class for specifying text horizontal alignment
class WHIPTK_API WT_Text_HAlign : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    enum WT_HAlign
    {
        Left    = 0,    // left-aligned
        Center  = 1,    // centered horizontally
        Right   = 2     // right-aligned
    };

public:
    // constructs a WT_Text_HAlign object
    WT_Text_HAlign();

    // constructs a WT_Text_HAlign object with the given alignment
    WT_Text_HAlign(WT_HAlign hAlign);

	///Copy Constructor
	WT_Text_HAlign(WT_Text_HAlign const& hAlign)
	{
		*this = hAlign;
	}

    // destructor
    virtual ~WT_Text_HAlign();

public:
    // returns the horizontal alignment
    inline const WT_HAlign& hAlign() const { return m_eAlignment; }
    inline WT_HAlign&       hAlign()       { return m_eAlignment; }
    inline WT_Text_HAlign const& operator=( WT_HAlign& eAlign ) { m_eAlignment = eAlign; return *this; }

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
        WT_Text_HAlign & item, // The object to process.
        WT_File & file         // The file being read.
        );
protected:
     // static methods
    static const char*     enum_to_string(WT_HAlign eVAlign);
    static WT_Boolean      string_to_enum(const char* pStr, WT_HAlign& outEnum);

private:
    // data members
    WT_HAlign m_eAlignment;

private:
    // const char strings
    static const char* str_HAlign_Left;
    static const char* str_HAlign_Right;
    static const char* str_HAlign_Center;

};


#endif //TEXT_HALIGN_HEADER
// endif
