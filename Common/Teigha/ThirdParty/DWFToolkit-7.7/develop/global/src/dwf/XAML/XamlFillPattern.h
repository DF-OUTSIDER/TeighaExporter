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

#if !defined XAML_FILLPAT_HEADER
#define XAML_FILLPAT_HEADER

///
///\file        XAML/XamlFillPattern.h
///

#include "XAML/XamlCore.h"

#include "whiptk/fillpat.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current fill pattern.
/** \sa WT_Rendition
 *  \image html FillPatterns.png "Suggested rendering of fill patterns"
 *
 */
class XAMLTK_API WT_XAML_Fill_Pattern :
    public WT_Fill_Pattern,
    public XamlDrawableAttributes::PathAttributeProvider,
    public XamlDrawableAttributes::Fill::Provider

{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;
	
private :

    WT_XAML_File *_pSerializeFile;                          // serialized file, !=0

protected:
    //@{ \name Construction

    /// Constructs a WT_XAML_Fill_Pattern object.
    WT_XAML_Fill_Pattern()
    : WT_Fill_Pattern(), 
      _pSerializeFile( NULL )
   {
   }

    /// Constructs a WT_XAML_Fill_Pattern with the given ID.
    WT_XAML_Fill_Pattern( WT_Pattern_ID n )
    : WT_Fill_Pattern(n),
      _pSerializeFile( NULL )
    {
    }

	virtual ~WT_XAML_Fill_Pattern()
	{
    }

public:

	///Assignment operator.
	WT_XAML_Fill_Pattern const & operator= (WT_XAML_Fill_Pattern const & other)
	{
		WT_Fill_Pattern::operator = (other);
		return *this;
	}

    //
    // from XamlDrawableAttributes::PathAttributeProvider
    //
    virtual WT_Result providePathAttributes(
        XamlDrawableAttributes::PathAttributeConsumer *,    // consumer, != 0
        WT_XAML_File &) const;                      // current file

    //
    // from XamlDrawableAttributes::Fill::Provider
    //
    virtual WT_Result provideFill(
        XamlDrawableAttributes::Fill *&);               // memento, may be null

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;   
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAML_FILLPAT_HEADER
