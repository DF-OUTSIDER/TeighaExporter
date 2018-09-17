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

#if !defined XAML_COLORMAP_HEADER
#define XAML_COLORMAP_HEADER

///
///\file        XAML/XamlColorMap.h
///

#include "XAML/XamlCore.h"

#include "whiptk/colormap.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the currrent color map.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Color_Map : public WT_Color_Map
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    /// Constructs a WT_XAML_Color_Map object for the optional specific file revision.  Can be used a default constructor.
    WT_XAML_Color_Map(
        int file_revision_being_used = REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED /**< The file decimal revision for which to create the map. */
        ) throw(WT_Result)
		:WT_Color_Map(file_revision_being_used)
	{ }
	/// Constructs a WT_XAML_Color_Map object for the given size and map, for the given file.
    WT_XAML_Color_Map(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGBA32 const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        ) throw(WT_Result)
		:WT_Color_Map(count,map,file)
	{}

    /// Constructs a WT_XAML_Color_Map object for the given size and map, for the given file.
    WT_XAML_Color_Map(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGB const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        ) throw(WT_Result)
		:WT_Color_Map(count,map,file)
	{}
    /// Constructs a WT_XAML_Color_Map object.  Copy constructor.
    WT_XAML_Color_Map(WT_Color_Map const & cmap) throw(WT_Result)
		:WT_Color_Map(cmap)
	{}

    /// Destroys a WT_Color_Map object.
    virtual ~WT_XAML_Color_Map()
	{}
public:
	//@{ \name WT_Object and WT_Attribute virtual methods
	WT_Result        serialize (WT_File & file) const;
    WT_Result        serializeRawColorMap(WT_XAML_File & file) const;
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    
};
//@}

#endif //XAML_COLORMAP_HEADER
