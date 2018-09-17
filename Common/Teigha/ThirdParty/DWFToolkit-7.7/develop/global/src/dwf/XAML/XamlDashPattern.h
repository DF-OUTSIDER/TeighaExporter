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

#if !defined XAML_DASHPAT_HEADER
#define XAML_DASHPAT_HEADER

///
///\file        XAML/XamlDashPattern.h
///

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a user definable line pattern.
/** Given a unique ID and a sequence of dash and space lengths, this object defines a custom line pattern.
 *
 *  \note Dash patterns override line patterns.  When finished with the dash pattern, clients
 *  must "turn off" dash patterns by setting the current dash pattern to WT_Dash_Pattern::kNull.
 *
 *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
 *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count.)  To reserve
 *  room for predefined line pattern growth in the future (which will all have positive ID's, it
 *  is suggested that dash pattern ID's begin at WT_Line_Pattern::Count+100 and increase in value
 *  from there.  The toolkit does not prevent conflicts from happening.
 *  \sa WT_Rendition
 *
 */
 
#include "XAML/XamlCore.h"

#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

class XAMLTK_API WT_XAML_Dash_Pattern : public WT_Dash_Pattern
                                      , public XamlDrawableAttributes::PathAttributeProvider
                                      , public XamlDrawableAttributes::StrokeDashArray::Provider
                                      , public XamlDrawableAttributes::StrokeDashArray::Consumer
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
    //@{ \name Construction / destruction
    /// Construct a WT_XAML_Dash_Pattern object.
    WT_XAML_Dash_Pattern()
    : WT_Dash_Pattern(),
      _pSerializeFile(NULL)
    {
    }

    /// Construct a WT_XAML_Dash_Pattern object with the given data.
    /** \exception WT_Result::Toolkit_Usage_Error The \a length parameter was not an even number.
     *  \exception WT_Result::Out_Of_Memory_Error Internal data could not be allocated.
     */
    WT_XAML_Dash_Pattern(
        /** Unique identifier for the dash pattern.
         *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
         *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count). */
        WT_Integer32 id_number,
        WT_Integer16 length, /**< Length of the \a pArray argument.  Must be even. */
        WT_Integer16 const * pArray /**< Array of values (contiguous pairs) representing pixels-on pixels-off defining the dash pattern. */
        ) throw(WT_Result)
    : WT_Dash_Pattern(id_number,length,pArray),
      _pSerializeFile(NULL)
	{
    }

    /// Construct a WT_XAML_Dash_Pattern object.  Copy constructor.
    WT_XAML_Dash_Pattern(
        WT_Dash_Pattern const & r ) throw(WT_Result)
    : WT_Dash_Pattern(r),
      _pSerializeFile(NULL)
    {
    }

    /// Destroys a WT_XAML_Dash_Pattern object.
    virtual ~WT_XAML_Dash_Pattern()
    { 
    }

    //@}

private :

    WT_XAML_File *_pSerializeFile;                          // serialized file, !=0

public:

	WT_XAML_Dash_Pattern const & operator= (WT_XAML_Dash_Pattern const & other) throw(WT_Result)
	{
		WT_Dash_Pattern::operator = (other);
		return *this;
	}
	
    //
    // from XamlDrawableAttributes::PathAttributeProvider
    //
    virtual WT_Result providePathAttributes(
        XamlDrawableAttributes::PathAttributeConsumer*,     // consumer, != 0
        WT_XAML_File& ) const;                      // current file

public:
	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;  
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

    //
    // from XamlDrawableAttributes::StrokeDashArray
    //
protected:
    virtual WT_Result provideStrokeDashArray( XamlDrawableAttributes::StrokeDashArray *& );    // memento, may be null
    virtual WT_Result consumeStrokeDashArray( XamlDrawableAttributes::StrokeDashArray::Provider * );

};
//@}

#endif //XAML_DASHPAT_HEADER
