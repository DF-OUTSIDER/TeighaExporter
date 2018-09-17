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
///\file        dwf/whiptk/marksymb.h
///

#if !defined MARKER_SYMBOL_HEADER
#define MARKER_SYMBOL_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current marker symbol
/** \deprecated The marker symbol is no longer supported.  This remains in the toolkit
 *  only for backward compatability with previous %DWF file versions.
 *
 *  \sa WT_Rendition
 */
class WHIPTK_API WT_Marker_Symbol : public WT_Attribute
{
private:
    WT_Integer32        m_symbol;

public:
    //@{ \name Constructors
    /// Constructs a WT_Marker_Symbol object.
    WT_Marker_Symbol()
        : m_symbol(0)
    { }

    /// Constructs a WT_Marker_Symbol object using the predefined symbol ID.
    WT_Marker_Symbol(WT_Integer32 marker_symbol)
        : m_symbol(marker_symbol)
    { }

	virtual ~WT_Marker_Symbol()
	{}
    //@}

    //@{ \name Data access methods
    /// Returns the symbol ID.
    WT_Integer32    symbol() const;
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
        WT_Marker_Symbol & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // MARKER_SYMBOL_HEADER
