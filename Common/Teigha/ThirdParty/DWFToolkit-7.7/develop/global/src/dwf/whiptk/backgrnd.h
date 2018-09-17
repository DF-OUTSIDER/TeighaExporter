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

#if !defined BACKGRND_HEADER
#define BACKGRND_HEADER

///
///\file        dwf/whiptk/backgrnd.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/color.h"

class WT_File;

/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

/// Describes the drawing background color (i.e. paper color.)
/** \deprecated This class is provided for backward compatibiility with classic %DWF
 *  files of version 0.55 and earlier.  Users desiring to set background color for
 *  W2D files should set the DWFEPlot::EPlotPaper color.  Any attempt to use this attribute
 *  in version 6.0 or greater will result in a WT_Result::Toolkit_Usage_Error during
 *  the sync() and/or serialize() calls.
 *  \sa WT_Rendering_Options
 */
class WHIPTK_API WT_Background : public WT_Attribute
{
private:
    WT_Color        m_color;
    enum
    {
        Getting_Color,
        Getting_Close_Paren
    } m_stage;

public:

    //@{ \name Construction
    /// Creates a WT_Background object.
    WT_Background()
        : m_color(0,0,0,255)
        , m_stage(Getting_Color)
    { }

    /// Creates a WT_Background object using the specified color.
    WT_Background(const WT_Color& color)
        : m_color(color)
        , m_stage(Getting_Color)
    { }

	virtual ~WT_Background()
	{}
    //@}

public:
    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    //@{ \name Get / set methods
    /// Sets the background color from a WT_Color object.
    virtual void       set(
        WT_Color const & color /**< The new color. */
        );
    /// Read-only accessor to a WT_Color object representing the background color.
    const WT_Color&   color() const
        ;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Background & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // BACKGRND_HEADER
