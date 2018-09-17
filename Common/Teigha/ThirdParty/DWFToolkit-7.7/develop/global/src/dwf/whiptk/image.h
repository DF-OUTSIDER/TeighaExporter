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

#if !defined IMAGE_HEADER
#define IMAGE_HEADER

///
///\file        dwf/whiptk/image.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"
#include "whiptk/colormap.h"

#define WD_EXPECTED_GROUP3X_FACTOR  0.2

/// Used by the WT_Image class for image encoding.
class WT_Huffman
{
public:

    /// Bitonal value enumeration.
    enum Bitonal_Value
    {
        Background = 0, /**< White */
        Foreground = 1  /**< Black */
    };

    /// Raster encoding type.
    enum Raster_Encoding
    {
        Group3 = 0x00,
        Group3X = 0x01,
        Literal = 0x02,
        Reserved = 0x03
    };

    /// The run length for this datum.
    int run;

    /// The huffman code and its bit length.
    struct Code
    {
        WT_Byte    value;
        WT_Byte    length;
    }           code[2];
};

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing an image (bitonal, color mapped, RGB, RGBA, JPEG.)
/** <b> An example of creating all of the different image types is provided in the sample app
 *  \em W2DTest accompanying the WHIP! Toolkit source distribution.  It is too lengthy to be
 *  included here in this API doc. </b>
 */
class WHIPTK_API WT_Image : public WT_Drawable
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    /// The format of the image contained in the WT_Image object.
    enum WT_Image_Format
    {
        Bitonal_Mapped      = WD_IMAGE_BITONAL_MAPPED_EXT_OPCODE,
        Group3X_Mapped      = WD_IMAGE_GROUP3X_MAPPED_EXT_OPCODE,
        Indexed             = WD_IMAGE_INDEXED_EXT_OPCODE,
        Mapped              = WD_IMAGE_MAPPED_EXT_OPCODE,
        RGB                 = WD_IMAGE_RGB_EXT_OPCODE,
        RGBA                = WD_IMAGE_RGBA_EXT_OPCODE,
        JPEG                = WD_IMAGE_JPEG_EXT_OPCODE
    };

public:

    WT_Unsigned_Integer16   m_rows;
    WT_Unsigned_Integer16   m_columns;
    WT_Byte                 m_format;
    WT_Integer32            m_identifier;
    WT_Integer32            m_dpi;

    WT_Color_Map *          m_color_map;
    WT_Integer32            m_data_size;
    WT_Byte *               m_data;

    WT_Logical_Point        m_min_corner;
    WT_Logical_Point        m_max_corner;

    WT_Boolean              m_transformed;
    WT_Boolean              m_relativized;
    WT_Boolean              m_local_data_copy;
    WT_Boolean              m_local_color_map_copy;

    // Scratchpad for conversion method use
    WT_Byte *               m_source_data;
    int                     m_src_byte_position;
    int                     m_src_bits_used;
    int                     m_dst_bits_used;
    int                     m_data_allocated;

    enum
    {
        Starting,
        Getting_Columns,
        Getting_Col_Row_Comma,
        Getting_Rows,
        Getting_Min_Corner,
        Getting_Max_Corner,
        Getting_Format,
        Getting_Identifier,
        Getting_Color_Map_Opcode,
        Getting_Color_Map,
        Getting_Pre_Data_Size_Whitespace,
        Getting_Pre_Data_Size_Open_Paren,
        Getting_Data_Size,
        Getting_Data,
        Getting_Close
    }                       m_stage;

    WT_Opcode               m_colormap_opcode;

    WT_Image (WT_Image const &)
      : WT_Drawable()
      , m_rows()
      , m_columns()
      , m_format()
      , m_identifier()
      , m_dpi()
      , m_color_map()
      , m_data_size()
      , m_data()
      , m_min_corner()
      , m_max_corner()
      , m_transformed()
      , m_relativized()
      , m_local_data_copy()
      , m_local_color_map_copy()
      , m_source_data()
      , m_src_byte_position()
      , m_src_bits_used()
      , m_dst_bits_used()
      , m_data_allocated()
      , m_stage()
      , m_colormap_opcode()
    {
        WD_Complain ("cannot copy WT_Image");
    } // prohibited

    WT_Image & operator= (WT_Image const &)
    {
        WD_Complain ("cannot assign WT_Image");
        return *this;
    } // prohibited

    inline WT_Result        add_code (WT_Huffman::Code const & code);
    inline int              get_next_bit();
    inline WT_Result        output_group3x_code(int run_color, int run_length, WT_Boolean skip_trailing_zero);

public:

    // Constructors, Destructors

    /// Constructs a WT_Image object.
    WT_Image () 
                : m_rows(0)
                , m_columns(0)
                , m_format(RGBA)
                , m_identifier(0)
                , m_dpi(-1)
                , m_color_map(WD_Null)
                , m_data_size(0)
                , m_data(WD_Null)
                , m_min_corner(0,0)
                , m_max_corner(0,0)
                , m_transformed(WD_False)
                , m_relativized(WD_False)
                , m_local_data_copy(WD_False)
                , m_local_color_map_copy(WD_False)
                , m_stage(Starting)
            { }

    /// Constructs a WT_Image object of the given type (one of \ref WT_Image_Format "enum WT_Image_Format".)
    WT_Image (int image_type) 
                : m_rows(0)
                , m_columns(0)
                , m_format(static_cast<WT_Byte>(image_type))
                , m_identifier(0)
                , m_dpi(-1)
                , m_color_map(WD_Null)
                , m_data_size(0)
                , m_data(WD_Null)
                , m_min_corner(0,0)
                , m_max_corner(0,0)
                , m_transformed(WD_False)
                , m_relativized(WD_False)
                , m_local_data_copy(WD_False)
                , m_local_color_map_copy(WD_False)
                , m_stage(Getting_Columns)
            { }


    /// Constructs a WT_Image object with the given data.
    WT_Image(
            WT_Unsigned_Integer16       rows, /**< Height of the image in pixels. */
            WT_Unsigned_Integer16       columns, /**< Width of the image in pixels. */
            WT_Image_Format             format, /**< The image format (one of \ref WT_Image_Format "enum WT_Image_Format".)*/
            WT_Integer32                identifier, /**< The unique numeric identifier of the image in the graphics collection. */
            WT_Color_Map const *        color_map, /**< The color map the image should use (if any.) */
            WT_Integer32                data_size, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
            WT_Byte *                   data, /**< The raw pixel data. */
            WT_Logical_Point const &    min_corner, /**< The lower-left corner of the image. */
            WT_Logical_Point const &    max_corner, /**< The upper-right corner of the image. */
            WT_Boolean                  copy, /**< Whether the pixel data should be copied or if it should be used directly from the buffer. */
            WT_Integer32                dpi = -1 /**< Scanned resolution for the image. */
            ) throw(WT_Result);

    /// Destroys a WT_Image object.
    virtual ~WT_Image()
    {
        if (m_local_data_copy)
            delete [] m_data;

        if (m_local_color_map_copy)
            delete m_color_map;
    }

public:
    //@{ \name Data access methods
    /// Returns the color map associated with this image (if any.)
    WT_Color_Map const * color_map() const
    {   return m_color_map;     }
    /// Returns the width of the image in pixels.
    WT_Unsigned_Integer16 columns() const
    {   return m_columns;  }
    WT_Unsigned_Integer16& columns()
    {   return m_columns;   }
    /// Returns the raw pixel data.
    WT_Byte const * data() const
    {   return m_data;  }
    /// Returns the size (in bytes) of the raw pixel data available through the data() method.
    WT_Integer32 data_size() const
    {   return m_data_size; }
    /// Returns the WT_Image_Format value for this image.
    WT_Byte format() const
    {   return m_format;  }
    WT_Byte& format() 
    {   return m_format; }
    /// Returns the unique image number
    WT_Integer32 identifier() const
    {   return m_identifier;    }
    WT_Integer32 const& dpi() const
    {   return m_dpi;   }
    WT_Integer32& dpi()
    {   return m_dpi;   }
    /// The upper-right corner of the image.
    WT_Logical_Point const & max_corner() const
    {   return m_max_corner;    }
    WT_Logical_Point& max_corner() 
    {   return m_max_corner;    }
    /// The lower-left corner of the image.
    WT_Logical_Point const & min_corner() const
    {   return m_min_corner;    }
    WT_Logical_Point& min_corner()
    {   return m_min_corner;    }
    /// The height of the image in pixels.
    WT_Unsigned_Integer16 rows() const
    {   return m_rows;  }
    WT_Unsigned_Integer16& rows()
    {   return m_rows;   }

    WT_Result set(WT_Color_Map const* pColorMap);
    //@}

    //@{ \name Image conversion methods
    /// Converts the current bitonal image to a Group3X_Mapped image.
    /** \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Toolkit_Usage_Error The image is not bitonal.
     *  \retval WT_Result::Corrupt_File_Error There is no image data.
     */
    WT_Result      convert_bitonal_to_group_3X ();
    /// Converts the current Group3X_Mapped image to a bitonal image.
    /** \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Toolkit_Usage_Error The image is not Group3X_Mapped.
     */
    WT_Result      convert_group_3X_to_bitonal();
    /// Converts the current Group3X_Mapped image to a Mapped image.
    /** \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Toolkit_Usage_Error The image is not Group3X_Mapped.
     */
    WT_Result      convert_group_3X_to_mapped();
    //@}

    //@{ \name Manipulation methods
    /// Returns the min and max corner points to absolute values based on their relative positions to last materialized point.
    virtual void        de_relativize(WT_File & file);
    /// Relativizes the min and max corner points to the last serialized point.
    virtual void        relativize(WT_File & file);
    /// Applies the given transform to the min and max corner points.
    virtual void           transform(WT_Transform const & transform);
    //@}

    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize(WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    virtual void     update_bounds(WT_File * file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Image & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // IMAGE_HEADER
