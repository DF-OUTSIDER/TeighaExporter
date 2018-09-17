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
#if !defined XAML_LAYER_HEADER
#define XAML_LAYER_HEADER

///
///\file        XAML/XamlLayer.h
///

#include "XAML/XamlCore.h"

#include "whiptk/layer.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a layer - a relation to a group of objects.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Layer : public WT_Layer
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Layer object.
    WT_XAML_Layer()
        : WT_Layer()        
    { }

    /// Constructs a WT_XAML_Layer object wth the given data.
    WT_XAML_Layer(
        WT_File &file, /**< The file being written. */
        WT_Integer32 layer_num, /**< The layer number. */
        char const * layer_name=NULL /**< The optional layer name. */
        )
		: WT_Layer(file,layer_num,layer_name)
	{}

    /// Constructs a WT_XAML_Layer object wth the given data.
    WT_XAML_Layer(
        WT_File &file, /**< The file being written. */
        WT_Integer32 layer_num, /**< The layer number. */
        WT_Unsigned_Integer16 const * layer_name /**< The layer name. */
        )
		: WT_Layer(file,layer_num,layer_name)
	{}
    /// Constructs a WT_Layer object.  Copy constructor
    WT_XAML_Layer (WT_XAML_Layer const & layer)
		: WT_Layer(static_cast<WT_Layer const &>(layer))
	{}

	virtual ~WT_XAML_Layer()
	{}
    //@}
public:

	/// Assignment operator.  Sets the layer contents from another (copies its contents.)
    WT_XAML_Layer const & operator=(WT_XAML_Layer const & layer)
	{
		WT_Layer::operator = (layer);
		return *this;
	}

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

};
//@}

#endif //XAML_LAYER_HEADER
