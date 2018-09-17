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

#if !defined XAML_USER_FILL_PATTERN_HEADER
#define XAML_USER_FILL_PATTERN_HEADER

///
///\file        XAML/XamlUserFillPattern.h
///

#include "XAML/XamlCore.h"

#include "whiptk/usrfillpat.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 *  Describes the Set_Fill_Pattern attribute in Rendition to be applied to 
 *  subsequent Polygons and other filled geometry. 
 *  This attribute is  a user defined Fill based on image data. It is used 
 *  by all the drawing opcodes that are affected by the Fill attribute. 
 */

/// Attribute class for specifying a user defined fill pattern 
class XAMLTK_API WT_XAML_User_Fill_Pattern :
    public WT_User_Fill_Pattern,
    public XamlDrawableAttributes::PathAttributeProvider,
    public XamlDrawableAttributes::Fill::Provider
{
    friend class WT_XAML_Class_Factory;
    friend class WT_XAML_Rendition;

private :

    WT_XAML_File *_pSerializeFile;                          // serialized file, !=0

protected:
     //@{ \name Construction

    /// Constructs a WT_XAML_User_Fill_Pattern object.
    WT_XAML_User_Fill_Pattern()
    : WT_User_Fill_Pattern(),  // important: -1 means "does not exist", which is the default state
      _pSerializeFile( NULL )
    {
    }

    /// Constructs a WT_XAML_User_Fill_Pattern with the given ID.
    WT_XAML_User_Fill_Pattern (WT_Integer16 pattern_num)
    : WT_User_Fill_Pattern(pattern_num),
      _pSerializeFile( NULL )
    {
    }
    
    /// Constructs a WT_XAML_User_Fill_Pattern with a user defined bitonal bitmap passing in all the parameters 
    WT_XAML_User_Fill_Pattern (
            WT_Integer16    pattern_num, /**< The unique numeric identifier of the fill pattern in the graphics collection. */
            WT_Unsigned_Integer16    rows, /**< Height of the bitonal bitmap in pixels. */
            WT_Unsigned_Integer16    columns, /**< Width of the bitonal bitmap in pixels. */ 
			WT_Unsigned_Integer32    data_size, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
            WT_Byte const *          data) /**< The raw pixel data that will be copied.  */  
	: WT_User_Fill_Pattern(pattern_num,rows,columns,data_size,data),
      _pSerializeFile( NULL )
	{
    }
        
    /// Constructs a WT_XAML_User_Fill_Pattern with a user defined bitonal bitmap using the reference counted Fill_Pattern class
    WT_XAML_User_Fill_Pattern (
            WT_Integer16    pattern_num, /**< The unique numeric identifier of the fill pattern in the graphics collection. */
            Fill_Pattern * fill_pattern
            )
    : WT_User_Fill_Pattern(pattern_num,fill_pattern),
      _pSerializeFile( NULL )
	{
    }

    /// Copy Constructor for the WT_User_Fill_Pattern
    WT_XAML_User_Fill_Pattern(WT_User_Fill_Pattern const &other)
    : WT_User_Fill_Pattern(other),
    _pSerializeFile( NULL)
    {
    }
    
    /// Destroys a WT_XAML_User_Fill_Pattern object.
    virtual ~WT_XAML_User_Fill_Pattern()
	{
    }

    //@}
public:

	/// Assignment operator for the WT_XAML_User_Fill_Pattern 
	WT_XAML_User_Fill_Pattern const & operator =(WT_XAML_User_Fill_Pattern const & other)
	{
		WT_User_Fill_Pattern::operator = (other);
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
    //@}
};
//@}

#endif //XAML_USER_FILL_PATTERN_HEADER
