//
//  Copyright (c) 2006 by Autodesk, Inc.
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
///\file        XAML/XamlPenPattern.h
///

#if !defined XAML_PENPAT_HEADER
#define XAML_PENPAT_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/penpat.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

class XAMLTK_API WT_XAML_Pen_Pattern : public WT_Pen_Pattern
{	
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_Pen_Pattern object.
    WT_XAML_Pen_Pattern ()
       : WT_Pen_Pattern(Screening_Black)
	{ }
    /// Constructs a WT_XAML_Pen_Pattern object with the given predefined pattern ID.
    WT_XAML_Pen_Pattern (WT_Pattern_ID pattern_id)
       : WT_Pen_Pattern(pattern_id)
	{ }
    /// Constructs a WT_XAML_Pen_Pattern object with the given data.
    WT_XAML_Pen_Pattern(
            WT_Pattern_ID                pattern_id, /**< The pen or face pattern ID, one of \ref WT_Pattern_ID "enum WT_Pattern_ID". */
            WT_Unsigned_Integer32        screening_percentage, /**< If specified, \a pattern_id must be one of Screening_Black, Screening_Alternate, Screening_Block, Screening_Dots, Screening_Big_Dots. */
            WT_Boolean                   colormap_flag, /**< Indicates whether the \a color_map contains valid content. */
            WT_Color_Map const *         color_map, /**< A 2-color map, indicating background and foreground colors respectively. */
            WT_Boolean                   copy) throw(WT_Result)
		: WT_Pen_Pattern(pattern_id,screening_percentage,colormap_flag,color_map,copy)
	{}
    /// Constructs a WT_XAML_Pen_Pattern object. Copy constructor.
    WT_XAML_Pen_Pattern(WT_Pen_Pattern const & pattern) throw(WT_Result)
		: WT_Pen_Pattern(pattern)
	{}

    /// Destroys a a WT_XAML_Pen_Pattern object.
    virtual ~WT_XAML_Pen_Pattern()
    { }
    //@}
public:

	/// Assignment operator. Copies the given pattern object contents to this one.
   WT_XAML_Pen_Pattern const & operator= ( const WT_XAML_Pen_Pattern & pen_pattern )
	{
		WT_Pen_Pattern::operator = (pen_pattern);
		return *this;
	}
   
   //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
};
//@}

#endif //XAML_PENPAT_HEADER
