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

#if !defined COLORMAP_HEADER
#define COLORMAP_HEADER

///
///\file        dwf/whiptk/colormap.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/wversion.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the currrent color map.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Color_Map : public WT_Attribute
{
    friend class WT_Image;
    friend class WT_Pen_Pattern;
    friend class WT_PNG_Group4_Image;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	friend class WT_File_Heuristics;
	friend class WT_DWF_Header;
	friend class WT_Color;
	friend class WT_PDFPen_Pattern;

private:
    WT_Integer32    m_size;
    int                m_incarnation;
    enum
    {
        Getting_Count,
        Getting_Colors,
        Getting_Close_Brace
    }               m_stage;
    WT_RGBA32 *        m_map;

    WT_Result        serialize_just_colors (WT_File & file) const;
    WT_Result        materialize_just_colors(WT_Opcode const & opcode, WT_Boolean use_binary, WT_Boolean get_close_brace, WT_File & file);

public:

    /// Constructs a WT_Color_Map object for the optional specific file revision.  Can be used a default constructor.
    WT_Color_Map(
        int file_revision_being_used = REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED /**< The file decimal revision for which to create the map. */
        ) throw(WT_Result);
    /// Constructs a WT_Color_Map object for the given size and map, for the given file.
    WT_Color_Map(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGBA32 const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        ) throw(WT_Result);
    /// Constructs a WT_Color_Map object for the given size and map, for the given file.
    WT_Color_Map(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGB const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        ) throw(WT_Result);
    /// Constructs a WT_Color_Map object.  Copy constructor.
    WT_Color_Map(WT_Color_Map const & cmap) throw(WT_Result);
    /// Destroys a WT_Color_Map object.
    virtual ~WT_Color_Map();

public:
    //@{ \name Data access methods

    /// Clears the color map.
    void      clear();
    /// Returns the index of the closest matching color in the map to the given color, or WD_NO_COLOR_INDEX if map is empty.
    int  closest_index(WT_Color const & desired) const;

    /// Returns the index of the matching color in the map to the given color, or WD_NO_COLOR_INDEX if not found.
    int  exact_index(WT_Color const & desired) const;

    /// Returns the index of the matching color in the map to the given color, or -1 if not found.
    int exact_index(WT_RGBA32 const & rgba) const;

    /// Returns the raw map colors.
    WT_RGBA32 * map() const
    {    return m_map;   }

    /// Returns the color at the given index.
    /** \warning This could overflow the array.  Clients should call size() to
     *  determine the map size before calling this method.
     */
    WT_RGBA32 map(WT_Byte index) const
    {
        WD_Assert (index < m_size);

        if (index < m_size)
            return m_map[index];
        else
            return WT_RGBA32(0,0,0,255);
    }

    /// Sets the map with the given size and map, for the given file.
    WT_Result    set(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGBA32 const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        );
    /// Sets the map with the given size and map, for the given file.
    WT_Result     set(
        int count, /**< Size of the color array in the \a map argument. */
        WT_RGB const * map, /**< An array of colors. */
        WT_File & file /**< The current file (used for its incarnation value.) */
        );

    /// Returns the size of the color map.
    int size() const
    {   return m_size;  }

    /// Assignment operator.  Assigns the given attribute contents to this one.
    WT_Color_Map const & operator=(WT_Color_Map const & cmap) throw(WT_Result);

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

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Color_Map & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // COLORMAP_HEADER
