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

#if !defined XAML_COLOR_HEADER
#define XAML_COLOR_HEADER

///
///\file        XAML/XamlColor.h
///

#include "XAML/XamlCore.h"

#include "whiptk/color.h"
#include "XAML/XamlXML.h"
#include "XAML/XamlDrawableAttributes.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the currrent color.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Color : public WT_Color
                               , public XamlDrawableAttributes::Stroke::Provider
                               , public XamlDrawableAttributes::Fill::Provider
                               , public XamlDrawableAttributes::Stroke::Consumer
                               , public XamlDrawableAttributes::Fill::Consumer
                               , public XamlDrawableAttributes::PathAttributeProvider
                               , public XamlDrawableAttributes::GlyphsAttributeProvider
{
    friend class WT_XAML_Class_Factory;
    friend class WT_XAML_Rendition;

protected:
    /// Constructs a WT_XAML_Color object.  (Defaults to white.)
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_XAML_Color()
    : WT_Color()
    , _pSerializeFile(NULL)
    { }

    /// Constructs a WT_XAML_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_XAML_Color(
        WT_RGBA32 color, /**< The color. */
        WT_Color_Map_Mode m = No_Mapping /**< Optional mapping mode. */
        )
    : WT_Color(color,m)
    , _pSerializeFile(NULL)
    { }

    /// Constructs a WT_XAML_Color object with the given color.
    WT_XAML_Color(
        WT_Color const & color /**< The color. */
        )
    : WT_Color(color)
    , _pSerializeFile(NULL)
    { }


    /// Constructs a WT_XAML_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_XAML_Color(
        int red, /**< Red coponent of RGB color. */
        int green, /**< Green coponent of RGB color. */
        int blue, /**< Blue coponent of RGB color. */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    : WT_Color(red,green,blue,m)
    , _pSerializeFile(NULL)
    {  }

    /// Constructs a WT_XAML_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_XAML_Color(
        int red, /**< Red coponent of RGBA color. */
        int green, /**< Green coponent of RGBA color. */
        int blue, /**< Blue coponent of RGBA color. */
        int alpha, /**< Alpha component of RGBA color (0 = transparent, 255 = opaque.) */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    : WT_Color(red,green,blue,alpha,m)
    , _pSerializeFile(NULL)
    { }

    /// Constructs a WT_XAML_Color object from the given index and corresponding color map.
    /** \exception WT_Result::Toolkit_Usage_Error The index is greater than the size of the color map.
    */
    WT_XAML_Color(
        WT_Color_Index index, /**< The color index into the \a color_map. */
        WT_Color_Map const & color_map /**< The map to search. */
        )
    : WT_Color(index,color_map)
    , _pSerializeFile(NULL)
    { }

    virtual ~WT_XAML_Color()
    {}
public:

    WT_XAML_Color const & operator= (WT_XAML_Color const & other)
    {
        set(other);
        return *this;
    }

    virtual WT_Result   parseAttributeList(XamlXML::tAttributeMap& rMap, WT_File& rFile);

    // Override WT_Attribute virtual method
    virtual WT_Result   serialize (WT_File & /*file*/) const;
    virtual WT_Result   sync_index(WT_XAML_File & /*file*/);

protected:
    virtual WT_Result provideStroke(XamlDrawableAttributes::Stroke* & rpStroke);
    virtual WT_Result provideFill(XamlDrawableAttributes::Fill*& rpFill);
    virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;
    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;
    virtual WT_Result consumeStroke(XamlDrawableAttributes::Stroke::Provider* );
    virtual WT_Result consumeFill(XamlDrawableAttributes::Fill::Provider* );

private:
    WT_XAML_File *_pSerializeFile;
};
//@}

#endif //XAML_COLOR_HEADER
