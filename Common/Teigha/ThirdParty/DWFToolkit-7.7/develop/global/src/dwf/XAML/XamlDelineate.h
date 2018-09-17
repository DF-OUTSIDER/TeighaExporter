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
#if !defined XAML_DELINEATE_HEADER
#define XAML_DELINEATE_HEADER

///
///\file        XAML/XamlDelineate.h
///

#include "XAML/XamlCore.h"

#include "whiptk/delineate.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a delineate - a relation to a group of objects.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Delineate : public WT_Delineate
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Delineate object.
    WT_XAML_Delineate()
        : WT_Delineate()        
    { }
	
	WT_XAML_Delineate(WT_Boolean delineate_state)
		: WT_Delineate(delineate_state)
	{}

    /// Constructs a WT_Delineate object.  Copy constructor
    WT_XAML_Delineate (WT_XAML_Delineate const & delineate)
		: WT_Delineate(static_cast<WT_Delineate const &>(delineate))
	{}

	virtual ~WT_XAML_Delineate()
	{}
    //@}
public:

	/// Assignment operator.  Sets the delineate contents from another (copies its contents.)
    WT_XAML_Delineate const & operator=(WT_XAML_Delineate const & delineate)
	{
		WT_Delineate::operator = (delineate);
		return *this;
	}

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAML_DELINEATE_HEADER
