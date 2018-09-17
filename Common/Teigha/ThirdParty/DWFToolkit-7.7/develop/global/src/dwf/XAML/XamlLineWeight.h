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
///\file        dwf/XAML/XamlLineWeight.h
///

#if !defined XAML_LWEIGHT_HEADER
#define XAML_LWEIGHT_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/lweight.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current line weight to be applied to subsequent lines and arcs.
/** \sa WT_Rendition
 *
 */

class XAMLTK_API WT_XAML_Line_Weight : public WT_Line_Weight
									 , public XamlDrawableAttributes::StrokeThickness::Provider
									 , public XamlDrawableAttributes::PathAttributeProvider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;
private:
	WT_XAML_File *_pSerializeFile;                          // serialized file, !=0

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Line_Weight object.
    WT_XAML_Line_Weight()
    : WT_Line_Weight(0)
	, _pSerializeFile(NULL)
    {}
    /// Constructs a WT_XAML_Line_Weight object with the given line weight.
    WT_XAML_Line_Weight(
        WT_Integer32 weight /**< The line weight, in drawing units (0 indicates a 1-pixel line.  Negative values are undefined.) */
        )
    : WT_Line_Weight(weight)
	, _pSerializeFile(NULL)
    {}

    /// Constructs a WT_XAML_Line_Weight object with the given line weight.
    WT_XAML_Line_Weight(
        WT_Line_Weight const &weight /**< The line weight */
        )
    : WT_Line_Weight(weight)
	, _pSerializeFile(NULL)
    {}

	virtual ~WT_XAML_Line_Weight()
	{}
    //@}
public:
	/// Assignment operator.  Sets the line weight to the given value.
    WT_XAML_Line_Weight& operator=(WT_XAML_Line_Weight const & other) 
	{ 
		WT_Line_Weight::operator = (other); 
		return *this; 
	}

    virtual WT_Result parseAttributeList(XamlXML::tAttributeMap& rMap, WT_File& rFile);

    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;

    //XamlDrawableAttributes::StrokeThickness::Provider
    virtual WT_Result provideStrokeThickness(XamlDrawableAttributes::StrokeThickness* & rpStrokeThickness);

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	
};
//@}

#endif //XAML_LWEIGHT_HEADER

