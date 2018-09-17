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

///
///\file        dwf/whiptk/macro_index.h
///

#if !defined MACRO_INDEX_HEADER
#define MACRO_INDEX_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current macro to be used

class WHIPTK_API WT_Macro_Index : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Integer32        m_macro;

public:

    //@{ \name Constructors
    /// Constructs a WT_Macro_Index object.
    WT_Macro_Index()
        : m_macro(0)
    { }

    /// Constructs a WT_Macro_Index object using the Macro index. if the macro dosent exist
    /// it will be ignored
    WT_Macro_Index(WT_Integer32 macro)
        : m_macro(macro)
    { }
	///Copy Constructor
	WT_Macro_Index(WT_Macro_Index const& index)
	{
		*this = index;
	}

	virtual ~WT_Macro_Index()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the symbol ID.

    inline const WT_Integer32& macro() const { return m_macro; }
    inline WT_Integer32&       macro()       { return m_macro; }
    inline WT_Macro_Index const & operator=( WT_Integer32 nMacro ) { m_macro = nMacro; return *this; }
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
        WT_Macro_Index & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // MACRO_INDEX_HEADER
