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

#if !defined CODE_PAGE_HEADER
#define CODE_PAGE_HEADER

///
///\file        dwf/whiptk/code_page.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;
/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute specifying the current code page (can be to assist with displaying localized strings.)
/** \sa WT_Rendition */
class WHIPTK_API WT_Code_Page : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

    enum WT_Materialize_Stage
    {    Eating_Initial_Whitespace,
        Getting_Page,
        Eating_End_Whitespace
    }               m_stage;
    WT_Integer32    m_page_number;

public:

    /// Constructs a WT_Code_Page object.
    WT_Code_Page()
    : m_stage(Eating_Initial_Whitespace)
    , m_page_number(1252)
    {}

    /// Constructs a WT_Code_Page object with the given page number.
    WT_Code_Page( WT_Integer32 page_number )
    : m_stage(Eating_Initial_Whitespace)
    , m_page_number(page_number)
    {}

	/// Copy Constructor
	WT_Code_Page(WT_Code_Page const& code_page)
	{
		*this = code_page;
	}

	virtual ~WT_Code_Page()
	{}
public:
    /// Returns the code page number.
    inline WT_Integer32 const & number() const { return m_page_number; }
    inline WT_Integer32&        number()       { return m_page_number; }
    inline WT_Code_Page const& operator=( WT_Integer32 nNumber ) { m_page_number = nNumber; return *this; }

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Code_Page & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};
//@}

#endif // CODE_PAGE_HEADER
