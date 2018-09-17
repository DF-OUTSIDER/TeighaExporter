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

///
///\file        XAML/XamlPNGGroup4Image.h
///

#if !defined XAML_PNGGROUP4IMAGE_HEADER
#define XAML_PNGGROUP4IMAGE_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/pnggroup4image.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing an image (PNG and bitonal).
/** <b> An example of creating all of the different image types is provided in the sample app
 *  \em W2DTest accompanying the WHIP! Toolkit source distribution.  It is too lengthy to be
 *  included here in this API doc. </b>
 */
class XAMLTK_API WT_XAML_PNG_Group4_Image : public WT_PNG_Group4_Image 
{
	friend class WT_XAML_Class_Factory;

protected:
	//Do we want to have these copy constructors??
	//commenting for now
	//WT_PNG_Group4_Image (WT_PNG_Group4_Image const &)
 //     : WT_PNG_Group4_Image ()
 //   { } // prohibited

 //   WT_PNG_Group4_Image & operator= (WT_PNG_Group4_Image const &)
 //   {
 //       WD_Complain ("cannot assign WT_PNG_Group4_Image");
 //       return *this;
 //   } // prohibited

    /// Constructs a WT_PNG_Group4_Image object.
    WT_XAML_PNG_Group4_Image ()
        : WT_PNG_Group4_Image()
    { }



    /// Constructs a WT_XAML_PNG_Group4_Image  object.
    /// Constructs a WT_XAML_PNG_Group4_Image  object of the given type (one of \ref WT_PNG_Group4_Image_Format "enum WT_PNG_Group4_Image_Format".)
    WT_XAML_PNG_Group4_Image (int image_type)
       : WT_PNG_Group4_Image(image_type)
    { }


    /// Constructs a WT_XAML_PNG_Group4_Image  object with the given data.
    WT_XAML_PNG_Group4_Image(
            WT_Unsigned_Integer16       rows, /**< Height of the image in pixels. */
            WT_Unsigned_Integer16       columns, /**< Width of the image in pixels. */
            WT_PNG_Group4_Image_Format  format, /**< The image format (one of \ref WT_PNG_Group4_Image_Format "enum WT_PNG_Group4_Image_Format".)*/
            WT_Integer32                identifier, /**< The unique numeric identifier of the image in the graphics collection. */
            WT_Color_Map const *        color_map, /**< The color map the image should use (if any.) */
            WT_Integer32                data_size, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
            WT_Byte *                   data, /**< The raw pixel data. */
            WT_Logical_Point const &    min_corner, /**< The lower-left corner of the image. */
            WT_Logical_Point const &    max_corner, /**< The upper-right corner of the image. */
            WT_Boolean                  copy, /**< Whether the pixel data should be copied or if it should be used directly from the buffer. */
            WT_Integer32                dpi = -1 /**< Scanned resolution for the image. */
            ) throw(WT_Result)
			: WT_PNG_Group4_Image(rows,columns,format,identifier,color_map,data_size,
								  data,min_corner,max_corner,copy,dpi)
	{}

	WT_XAML_PNG_Group4_Image & operator= (WT_XAML_PNG_Group4_Image const &)
    {
        WD_Complain ("cannot assign WT_XAML_PNG_Group4_Image");
        return *this;
    } // prohibited

    /// Destroys a WT_XAML_PNG_Group4_Image object.
    virtual ~WT_XAML_PNG_Group4_Image()
    {}
public:

	//@{ \name WT_Object virtual methods
    WT_Result        serialize(WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
	//@}
};
//@}

#endif //XAML_PNGGROUP4IMAGE_HEADER
