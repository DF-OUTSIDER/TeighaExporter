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
///\file        dwf/whiptk/plot_optimized.h
///

#if !defined PLOT_OPTIMIZED_HEADER
#define PLOT_OPTIMIZED_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

/// Indicates whether the output was intended for plotting or for viewing.
/** This gives the WHIP! authors the ability to specify whether the graphics have been optimized for
 *  display or plotting. Rendering applications should honor this setting and take full advantage of
 *  the nature of the WHIP! data while plotting or displaying the drawables. When the data is optimized
 *  for plotting, the WHIP! data author should store enough data to allow full fidelity plots at the
 *  selected resolution and color depth. When the data is optimized for display, allow for much deeper
 *  zooms.
 *  \sa WT_Rendering_Options */
class WHIPTK_API WT_Plot_Optimized : public WT_Attribute
{
	friend class WT_Rendering_Options;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Boolean        m_plot_optimized;

public:

    //@{ \name Construction
    /// Constructs a WT_Plot_Optimized object.
    WT_Plot_Optimized()
        : m_plot_optimized(WD_False)
    { }
    /// Constructs a WT_Plot_Optimized object with the given state.
    WT_Plot_Optimized(WT_Boolean plot_optimized)
        : m_plot_optimized(plot_optimized)
    { }
	
	///Copy Constructor
	WT_Plot_Optimized(WT_Plot_Optimized const& other)
	{
		*this = other;
	}
	
	virtual ~WT_Plot_Optimized()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns true if the data has been optimized for plotting.
    WT_Boolean get_plot_optimized() const;
    /// Sets whether the data has been optimized for plotting.
    void       set_plot_optimized(WT_Boolean const & plot_optimized);
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
        WT_Plot_Optimized & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}
#endif // PLOT_OPTIMIZED_HEADER
