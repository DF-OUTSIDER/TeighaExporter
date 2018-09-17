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

#if !defined COLOR_HEADER
#define COLOR_HEADER

///
///\file        dwf/whiptk/color.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;
class WT_Color_Map;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the currrent color.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Color
: public WT_Attribute
{
	friend class WT_Background;
	friend class WT_Rendition;
	friend class WT_PDFBackground;
	friend class WT_W2D_Class_Factory;
	friend class WT_PDFClassFactory;
	friend class WT_Opcode;
	friend class WT_File;
	friend class WT_Color_Map;
	friend class WT_Class_Factory;
public:
    /// Indicates how searching for a color match in the map should proceed.
    enum WT_Color_Map_Mode
    {   No_Mapping,
        Search_666_Color_Cube_Exact,
        Search_666_Color_Cube_Nearest,
        Search_666_Color_Cube_Exact_Then_Color_Map_Exact,
        Search_666_Color_Cube_Exact_Then_Color_Map_Nearest
    };
public:
    /// Constructs a WT_Color object.  (Defaults to white.)
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_Color()
    : m_stage(Getting_Color)
    { set(255,255,255); }

    /// Constructs a WT_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_Color(
        WT_RGBA32 color, /**< The color. */
        WT_Color_Map_Mode m = No_Mapping /**< Optional mapping mode. */
        )
    : m_stage(Getting_Color)
    { set(color,m); }

    /// Constructs a WT_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_Color(
        int red, /**< Red coponent of RGB color. */
        int green, /**< Green coponent of RGB color. */
        int blue, /**< Blue coponent of RGB color. */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    : m_stage(Getting_Color)
    { set(red,green,blue,m); }

    /// Constructs a WT_Color object with the given color and optional mapping mode.
    /** The color index will be set to WD_NO_COLOR_INDEX. */
    WT_Color(
        int red, /**< Red coponent of RGBA color. */
        int green, /**< Green coponent of RGBA color. */
        int blue, /**< Blue coponent of RGBA color. */
        int alpha, /**< Alpha component of RGBA color (0 = transparent, 255 = opaque.) */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    : m_stage(Getting_Color)
    { set(red,green,blue,alpha,m); }

    /// Constructs a WT_Color object from the given index and corresponding color map.
    /** \exception WT_Result::Toolkit_Usage_Error The index is greater than the size of the color map.
    */
    WT_Color(
        WT_Color_Index index, /**< The color index into the \a color_map. */
        WT_Color_Map const & color_map /**< The map to search. */
        )
    : m_mode(No_Mapping)
    , m_stage(Getting_Color)
    { set(index,color_map); }

    /// Constructs a WT_Color object from another color.
    /** \exception WT_Result::Toolkit_Usage_Error The index is greater than the size of the color map.
    */
    WT_Color(
        WT_Color const& copy /**< The color to copy from. */
        )
    { set (copy); }

	virtual ~WT_Color()
	{}
public:
    //@{ \name Data access methods
    /// Returns the color index (if any), may be WD_NO_COLOR_INDEX. */
    inline const int index()      const { return m_index; }
    /// Returns the RGBA value of the color.
    inline const WT_RGBA32& rgba() const { return m_rgba; }
    /// Returns the color mapping mode.
    inline const WT_Color_Map_Mode& mode() const { return m_mode; }

    /// Sets the color with the given color, turns off mapping
    inline WT_Color const& operator=( WT_RGBA32 const &oColor )
    {   m_mode = No_Mapping;
        m_rgba = oColor;
        m_index = WD_NO_COLOR_INDEX;
        return *this; 
    }

    /// Sets the colors from the given index and corresponding color map.
    /** \exception WT_Result::Toolkit_Usage_Error The index is greater than the size of the color map.
    */
    void set(
        WT_Color_Index index, /**< The color index into the \a color_map. */
        WT_Color_Map const & color_map /**< The map to search. */
        );

    /// Sets the color with the given color and optional mapping mode.
    inline void set(
        int red, /**< Red coponent of RGBA color. */
        int green, /**< Green coponent of RGBA color. */
        int blue, /**< Blue coponent of RGBA color. */
        int alpha, /**< Alpha component of RGBA color (0 = transparent, 255 = opaque.) */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    {   set(WT_RGBA32(red,green,blue,alpha),m); }

    /// Sets the color with the given color and optional mapping mode.
    inline void set(
        int red, /**< Red coponent of RGBA color. */
        int green, /**< Green coponent of RGBA color. */
        int blue, /**< Blue coponent of RGBA color. */
        WT_Color_Map_Mode m=No_Mapping /**< Optional mapping mode. */
        )
    {   set(WT_RGBA32(red,green,blue,0xFF),m); }

    /// Sets the color with the given color and optional mapping mode.
    inline void set(
        WT_RGBA32 color, /**< The color. */
        WT_Color_Map_Mode m = No_Mapping /**< Optional mapping mode. */
        )
    {   m_mode = m;
        m_rgba = color;
        m_index = WD_NO_COLOR_INDEX;
    }
    
    /// Sets the color from another.
    inline void set(
        WT_Color const &copy
        )
    {
        m_rgba = copy.m_rgba;
        m_index = copy.m_index;
        m_mode = copy.m_mode;
        m_stage = copy.m_stage;
    }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Returns the index of the color in the color map.
    /** \retval int The index of the color in the map (or nearest match - based on the search \a mode)
     *  \retval WD_NO_COLOR_INDEX An exact match was desired, and one was not found.
     */
    static int map_to_index(
        WT_RGBA32 color,  /**< The color to search for. */
        WT_Color_Map_Mode mode, /**< The search mode. */
        WT_Color_Map const & map /**< The map to search. */
        );

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Color & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_RGBA32 m_rgba;
    WT_Color_Index m_index;
    WT_Color_Map_Mode m_mode;
    enum WT_Materialize_Stage
    {   Getting_Color,
        Getting_Close_Paren
    } m_stage;
};

//@}

#endif // COLOR_HEADER
