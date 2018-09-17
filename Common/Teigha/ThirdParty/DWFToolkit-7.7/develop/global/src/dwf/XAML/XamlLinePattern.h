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
#if !defined XAML_LINEPAT_HEADER
#define XAML_LINEPAT_HEADER

///
///\file        XAML/XamlLinePattern.h
///

/** \addtogroup groupRenditionAttributes
 *  @{
 */

#include "XAML/XamlCore.h"

#include "whiptk/linepat.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

class XAMLTK_API WT_XAML_Line_Pattern :
    public WT_Line_Pattern,
    public XamlDrawableAttributes::PathAttributeProvider,
    public XamlDrawableAttributes::StrokeDashArray::Provider,
    public XamlDrawableAttributes::StrokeDashOffset::Provider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
    //@{ \name Construction
    /// Constructs a WT_Line_Pattern object.
    WT_XAML_Line_Pattern()
    : WT_Line_Pattern()
    , _pSerializeFile(NULL)
    {}

    /// Constructs a WT_Line_Pattern object with the given predefined pattern ID.
    WT_XAML_Line_Pattern( WT_Pattern_ID id )
    : WT_Line_Pattern(id)
    , _pSerializeFile(NULL)
    {}

    virtual ~WT_XAML_Line_Pattern()
	{}
    //@}

private :

    WT_XAML_File *_pSerializeFile;                  // serialized file, !=0
    
    static unsigned int *_kpDwfPatterns [];           // DWF patterns
    static unsigned int *_kpIsoPatterns [];           // ISO patterns

public:

    //
    // from XamlDrawableAttributes::PathAttributeProvider
    //
    virtual WT_Result providePathAttributes(
        XamlDrawableAttributes::PathAttributeConsumer*,     // consumer, != 0
        WT_XAML_File& ) const;                      // current file

    //
    // from XamlDrawableAttributes::StrokeDashArray::Provider
    //
    virtual WT_Result provideStrokeDashArray(
        XamlDrawableAttributes::StrokeDashArray *&);    // memento, may be null

    //
    // from XamlDrawableAttributes::StrokeDashOffset::Provider
    //
    virtual WT_Result provideStrokeDashOffset(
        XamlDrawableAttributes::StrokeDashOffset *&);    // memento, may be null

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
}; 
//@}

#endif //XAML_LINEPAT_HEADER
