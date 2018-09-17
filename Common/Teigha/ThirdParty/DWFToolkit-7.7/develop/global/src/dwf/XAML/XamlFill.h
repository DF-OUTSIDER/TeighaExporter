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

#if !defined XAML_FILL_HEADER
#define XAML_FILL_HEADER

///
///\file        XAML/XamlFill.h
///

#include "XAML/XamlCore.h"

#include "whiptk/fill.h"
#include "XAML/XamlDrawableAttributes.h"

class WT_XAML_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing if fill mode is on.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Fill : public WT_Fill
                              , public XamlDrawableAttributes::Fill::Provider
                              , public XamlDrawableAttributes::PathAttributeProvider
{
    friend class WT_XAML_Class_Factory;
    friend class WT_XAML_Rendition;

private:
    WT_XAML_File * _pSerializeFile;

protected:
    /// Constructs a WT_XAML_Fill object.  (Defaults to off.)
    /** The fill mode will be set to WD_False. */
    WT_XAML_Fill()
    : _pSerializeFile( NULL )
    { }

    /// Constructs a WT_XAML_Fill object with the given fill state.
    WT_XAML_Fill(WT_Boolean fill)
    : WT_Fill(fill)
    , _pSerializeFile( NULL )
    { }

	/// Copy Constructor
	WT_XAML_Fill(WT_Fill const& fill)
		:WT_Fill(fill)
		, _pSerializeFile( NULL )
	{ }

    virtual ~WT_XAML_Fill()
    { }

public:

    virtual WT_XAML_Fill const & operator= (WT_XAML_Fill const & other)
    {
        set( other.fill() );
        return *this;
    }

    virtual WT_XAML_Fill const & operator= (WT_Boolean const & fill)
    {
        set(fill);
        return *this;
    }

    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;

    //XamlAttributes::Fill::Provider
    WT_Result        provideFill(XamlDrawableAttributes::Fill* & rpFill);

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;

};
//@}

#endif //XAML_FILL_HEADER
