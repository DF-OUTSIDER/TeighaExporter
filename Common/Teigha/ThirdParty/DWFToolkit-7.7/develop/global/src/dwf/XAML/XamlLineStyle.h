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

#if !defined XAML_LINESTYLE_HEADER
#define XAML_LINESTYLE_HEADER

///
///\file        XAML/XamlLineStyle.h
///

#include "XAML/XamlCore.h"

#include "whiptk/linestyle.h"

/** \addtogroup groupLineStyleOptions Line style options
*  The line style option objects assist the reader application in rendering thick line
*  and dash end and start caps, as well as line join styles.
*  \sa WT_Line_Style.
*
*  @{
*/
//@}

#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/// Describes the attributes which affect rendered lines, polylines, and arcs.
/** \ingroup groupRenditionAttributes
 *  \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Line_Style : public WT_Line_Style
									, public XamlDrawableAttributes::PathAttributeProvider
									, public XamlDrawableAttributes::StrokeEndLineCap::Provider
									, public XamlDrawableAttributes::StrokeStartLineCap::Provider
									, public XamlDrawableAttributes::StrokeDashCap::Provider
									, public XamlDrawableAttributes::StrokeLineJoin::Provider
									, public XamlDrawableAttributes::StrokeMiterLimit::Provider
									, public XamlDrawableAttributes::StrokeEndLineCap::Consumer
									, public XamlDrawableAttributes::StrokeStartLineCap::Consumer
									, public XamlDrawableAttributes::StrokeDashCap::Consumer
									, public XamlDrawableAttributes::StrokeLineJoin::Consumer
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
    //@{ \name Construction
    /// Constructs a WT_XAML_Line_Style object.
    WT_XAML_Line_Style()
    : WT_Line_Style()
    , _pSerializeFile(NULL)
    {}

    /// Constructs a WT_XAML_Line_Style object from the given style
    WT_XAML_Line_Style( const WT_Line_Style &style) /**< The style */
    : WT_Line_Style( style )
    , _pSerializeFile(NULL)
    {}

	virtual ~WT_XAML_Line_Style()
	{}
    //@}
public:

    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;
    virtual WT_Result provideStrokeEndLineCap(XamlDrawableAttributes::StrokeEndLineCap* & rpStrokeEndLineCap);
    virtual WT_Result provideStrokeStartLineCap(XamlDrawableAttributes::StrokeStartLineCap* & rpStrokeStartLineCap);
    virtual WT_Result provideStrokeDashCap(XamlDrawableAttributes::StrokeDashCap* & rpStrokeStartLineCap);
    virtual WT_Result provideStrokeLineJoin(XamlDrawableAttributes::StrokeLineJoin* & rpStrokeLineJoin);
    virtual WT_Result provideStrokeMiterLimit(XamlDrawableAttributes::StrokeMiterLimit* & rpStrokeMiterLimit);

    virtual WT_Result consumeStrokeEndLineCap(XamlDrawableAttributes::StrokeEndLineCap::Provider *pProvider);
    virtual WT_Result consumeStrokeStartLineCap(XamlDrawableAttributes::StrokeStartLineCap::Provider *pProvider);
    virtual WT_Result consumeStrokeDashCap(XamlDrawableAttributes::StrokeDashCap::Provider *pProvider);
    virtual WT_Result consumeStrokeLineJoin(XamlDrawableAttributes::StrokeLineJoin::Provider *pProvider);

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
private:
	private :

    WT_XAML_File *_pSerializeFile;                          // serialized file, !=0
};

#endif //XAML_LINESTYLE_HEADER
