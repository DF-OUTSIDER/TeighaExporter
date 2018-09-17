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


#if !defined XAML_USER_HATCH_PATTERN_HEADER
#define XAML_USER_HATCH_PATTERN_HEADER

#include "dwfcore/DWFXMLSerializer.h"
///
///\file        XAML/XamlUserHatchPattern.h
///

#include "XAML/XamlCore.h"

#include "whiptk/usrhatchpat.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/// Attribute class for specifying a user defined hatch pattern 
class XAMLTK_API WT_XAML_User_Hatch_Pattern :
    public WT_User_Hatch_Pattern,
    public XamlDrawableAttributes::PathAttributeProvider,
    public XamlDrawableAttributes::Fill::Provider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

private:

    WT_XAML_File *_pSerializeFile;                          // serialized file, !=0
    unsigned int _pattern_index;                            // current pattern number

protected :

    //@{ \name Construction
    /// Default Constructor  
    WT_XAML_User_Hatch_Pattern()
        : WT_User_Hatch_Pattern()
        , _pSerializeFile( NULL )
        , _pattern_index(0)  // important: -1 means "does not exist", which is the default state
    {}
    /// Constructor using a previously defined hashpatnum. 
    /// Note: if the haspatnum is unique the DWF Viewer will
    ///       simply ignore this attribute.
    WT_XAML_User_Hatch_Pattern(WT_Integer16 hashpatnum)
        : WT_User_Hatch_Pattern(hashpatnum)
        , _pSerializeFile( NULL )
        , _pattern_index(0)
    {}  
    /// Constructor using a unique hashpatnum along with the X and Y size
    /// of the line patterns in points. 
    /// Note: If the haspatnum is not unique it will replace the previously
    ///       occuring WT_XAML_User_Hatch_Pattern atrribute for any subsequent
    ///       usage of this attribute with the hashpatnum.
    WT_XAML_User_Hatch_Pattern(WT_Integer16 hashpatnum,
                         WT_Unsigned_Integer16 xsize,
                         WT_Unsigned_Integer16 ysize)
        : WT_User_Hatch_Pattern(hashpatnum,xsize,ysize)
        , _pSerializeFile( NULL )
        , _pattern_index(0)
    {}  

    /// Copy constructor
    WT_XAML_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern)
		: WT_User_Hatch_Pattern(static_cast<WT_User_Hatch_Pattern const &>(pattern))
        , _pSerializeFile( NULL )
        , _pattern_index(0)
	{}    
   
    virtual ~WT_XAML_User_Hatch_Pattern()
	{}    
    //@}

public:
	///Assignment operator.
	WT_XAML_User_Hatch_Pattern const & operator= (WT_User_Hatch_Pattern const & pattern)
	{
		WT_User_Hatch_Pattern::operator = (pattern);
		return *this;
	}

    //
    // sets the pattern definition # to use while rendering to XAML
    //
    // this should be used by hatchable WT_XAML_xxx drawables to serialize
    // multiple <Path> since multi-hatching has to be rendered by compositing
    // together all individual patterns
    //
    // returns WT_Success is the number is valid, Internal_Error otherwise
    //
    WT_Result set_pattern_number(
        unsigned int);                              // pattern number

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

#endif //XAML_USER_HATCH_PATTERN_HEADER
