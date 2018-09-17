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

#if !defined XAML_VIEWPORT_HEADER
#define XAML_VIEWPORT_HEADER

///
///\file        XAML/XamlViewport.h
///

#include "XAML/XamlCore.h"

#include "whiptk/viewport.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute defining the current viewport receiving the subsequent graphics.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Viewport : public WT_Viewport
                                  , public XamlDrawableAttributes::CanvasAttributeProvider
                                  , public XamlDrawableAttributes::Clip::Provider
                                  , public XamlDrawableAttributes::Clip::Consumer
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

private :

    WT_XAML_File *_pSerializeFile;

    //
    // materialization specific : when creating a materialized viewport,
    // we need to assign a new incarnation # to the inner contour set -
    // otherwise an incorrect clip will be specified and most likely we
    // won't see parts of the drawing in the viewer
    //
    int _nContourIncarnation;

protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_Viewport object.
    WT_XAML_Viewport()
    : WT_Viewport(),
      _pSerializeFile(NULL),
      _nContourIncarnation(-1)
	{
    }

    /// Constructs a WT_XAML_Viewport object. Copy constructor.
    WT_XAML_Viewport( WT_XAML_Viewport const & vport) throw(WT_Result)
    : WT_Viewport(static_cast<WT_Viewport const &> (vport)),
      _pSerializeFile(NULL),
      _nContourIncarnation(-1)
    {
    }

    /// Constructs a WT_XAML_Viewport object with the given data.
    WT_XAML_Viewport(
        WT_File &                   file, /**< The file being written. */
        char const *                name, /**< The viewport name. */
        int                         num_points, /**< The number of points in the viewport. */
        WT_Logical_Point const *    points, /**< An array of points defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
	: WT_Viewport(file,name,num_points,points,copy),
      _pSerializeFile(NULL),
      _nContourIncarnation(-1)
	{
    }

    /// Constructs a WT_XAML_Viewport object with the given data.
    WT_XAML_Viewport(
        WT_File &                   file, /**< The file being written. */
        WT_Unsigned_Integer16 const * name, /**< The viewport name. */
        int                         num_points, /**< The number of points in the viewport. */
        WT_Logical_Point const *    points, /**< An array of points defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
	: WT_Viewport(file,name,num_points,points,copy),
      _pSerializeFile(NULL),
      _nContourIncarnation(-1)
	{
    }

    /// Constructs a WT_XAML_Viewport object with the given data.
    WT_XAML_Viewport(
        WT_File &                   file, /**< The file being written. */
        WT_String const &           name, /**< The viewport name. */
        WT_Contour_Set const &      boundary, /**< A contour set defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
	: WT_Viewport(file,name,boundary,copy),
      _pSerializeFile(NULL),
      _nContourIncarnation(-1)
    {
    }

    /// Destroys a WT_XAML_Viewport object.
    virtual ~WT_XAML_Viewport()
	{
    }

    //@}

public:

	/// Assignment operator.  Copies the settings from the given viewport to this one.
	WT_XAML_Viewport &  operator=(WT_XAML_Viewport const & vport)
	{
		WT_Viewport::operator= (vport);
		return *this;
	}

    //
    // from XamlDrawableAttributes::CanvasAttributeProvider
    //
    virtual WT_Result provideCanvasAttributes(
        XamlDrawableAttributes::CanvasAttributeConsumer *,    // consumer, not null
        WT_XAML_File &) const;                              // file

    //
    // from XamlDrawableAttributes::Clip::Provider
    //
    virtual WT_Result provideClip( 
        XamlDrawableAttributes::Clip *&);                   // memento, may be null

    //
    // from XamlDrawableAttributes::Clip::consumeClip
    //
    /*! \fn virtual WT_Result consumeClip( XamlDrawableAttributes::Clip::Provider* p)
    \brief Consumes a clip
    */
    virtual WT_Result consumeClip( XamlDrawableAttributes::Clip::Provider* p /**< Clip provider */);


	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
};
//@}

#endif //XAML_VIEWPORT_HEADER

