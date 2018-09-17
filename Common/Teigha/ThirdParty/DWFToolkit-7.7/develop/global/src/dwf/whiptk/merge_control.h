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
///\file        dwf/whiptk/merge_control.h
///

#if !defined MERGE_CONTROL_HEADER
#define MERGE_CONTROL_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute suggesting the rendering behavior of overlapping objects.
/** If it is set, the merge control indicates to a rendering application
 *  that a raster operation should to be performed on drawables in order to render
 *  the corresponding pixel colors, whenever the drawables overlap.
 *  \sa WT_Rendition
 *  \image html MergeControl.png "Suggested rendering of merged geometry"
 */
class WHIPTK_API WT_Merge_Control : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_PDFMerge_Control;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    /// Merge operation to perform when objects overlap.
    enum WT_Merge_Format
    {
        Opaque      =  WD_EXAO_SET_OPAQUE, /**< Overlapping geometry completely obscures underlying geometry. */
        Merge       =  WD_EXAO_SET_MERGE, /**< Overlapping geometry pixel colors should mask merge (where the resultant pixel color is "mostly the top color.")*/
        Transparent =  WD_EXAO_SET_TRANSPARENT /**< Overlapping geometry pixel colors should exclusive-or. */
    };

private:
    WT_Merge_Format   m_merge;

public:

    //@{ \name Construction
    /// Constructs a WT_Merge_Control object.
    WT_Merge_Control()
    {     m_merge = Opaque;     }

    /// Constructs a WT_Merge_Control object with the given format.
    WT_Merge_Control(WT_Merge_Format merge)
    {   m_merge = merge;    }

	///Copy Constuctor
	WT_Merge_Control(WT_Merge_Control const& merge_control)
	{
		*this = merge_control;
	}

	virtual ~WT_Merge_Control()
	{}
    //@}
public:
    //@{ \name Data access methods
    WT_Merge_Control::WT_Merge_Format  get_merge_control() const;
    void                               set_merge_control(WT_Merge_Control::WT_Merge_Format merge);
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
        WT_Merge_Control & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // MERGE_CONTROL_HEADER
