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

///
///\file        dwf/whiptk/penpat.h
///

#if !defined PENPAT_HEADER
#define PENPAT_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/attribute.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current pen pattern.
/** This specifies the pen set or the pen pattern type to be used by a
 *  rendering application before rendering WHIP! drawables. The WHIP! Toolkit
 *  defines 105 face patterns and 5 screening pen sets, the latter specified
 *  with an associated screening percentage value.  Screening percentage
 *  values are not applied to the face patterns.
 *
 *  The pen pattern attribute should be applied to wide and thick lines, filled solids,
 *  texts, polylines and polytriangles.  In the absence of a color map, rendering
 *  applications viewers should choose foreground and background colors from the rendition
 *  to represent an opaque pattern.
 *
 *  \sa WT_Rendition
 */
class WHIPTK_API WT_Pen_Pattern : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    /// Predefined screening pen sets and pen patterns.
    enum WT_Pattern_ID
    {
        Illegal,    /**< Not a pattern.*/

        //Individual Screening Percentage Patterns
        Screening_Black,      /**< Screening pen set, use with a screening percentage.*/
        Screening_Alternate,  /**< Screening pen set, use with a screening percentage.*/
        Screening_Block,      /**< Screening pen set, use with a screening percentage.*/
        Screening_Dots,       /**< Screening pen set, use with a screening percentage.*/
        Screening_Big_Dots,   /**< Screening pen set, use with a screening percentage.*/

        //Non-Individual Screening Patterns (Face Patterns)
        Dots_Big,
        Dots_Medium,
        Dots_Small,
        Slant_Left_32x32,
        Slant_Right_32x32,
        Screen_15,
        Screen_25,
        Screen_20,
        Screen_75,
        Screen_50,
        Screen_Thin_50,
        Screen_Hatched_50,
        Trellis,
        ZigZag,
        Diagonal,
        Triangle,
        Triangle_More,
        Bricks,
        Bricks_Big,
        Squares,
        Squares_3D,
        Diamond_Plaid,
        Ziggurat,
        Diagonal_Thatch,
        Zipper,
        Slants,
        Slants_More,
        Diags,
        Diags_More,
        Marks,
        Marks_More,
        Diamonds_Thick,
        Diamonds_Thin,

        Screening_Black_0,
        Screening_Black_10,
        Screening_Black_15,
        Screening_Black_20,
        Screening_Black_35,
        Screening_Black_40,
        Screening_Black_45,
        Screening_Black_50,
        Screening_Black_55,
        Screening_Black_60,
        Screening_Black_65,
        Screening_Black_70,
        Screening_Black_75,
        Screening_Black_80,
        Screening_Black_85,
        Screening_Black_90,
        Screening_Black_100,

        Screening_Alternate_5,
        Screening_Alternate_10,
        Screening_Alternate_15,
        Screening_Alternate_20,
        Screening_Alternate_25,
        Screening_Alternate_35,
        Screening_Alternate_40,
        Screening_Alternate_45,
        Screening_Alternate_50,
        Screening_Alternate_65,
        Screening_Alternate_70,
        Screening_Alternate_80,
        Screening_Alternate_85,
        Screening_Alternate_90,
        Screening_Alternate_95,

        Screening_Block_10,
        Screening_Block_20,
        Screening_Block_25,
        Screening_Block_30,
        Screening_Block_40,
        Screening_Block_50,
        Screening_Block_60,
        Screening_Block_65,
        Screening_Block_70,
        Screening_Block_75,
        Screening_Block_80,
        Screening_Block_85,
        Screening_Block_90,
        Screening_Block_95,

        Screening_Dots_5,
        Screening_Dots_10,
        Screening_Dots_15,
        Screening_Dots_20,
        Screening_Dots_25,
        Screening_Dots_30,
        Screening_Dots_40,
        Screening_Dots_50,
        Screening_Dots_60,
        Screening_Dots_70,
        Screening_Dots_75,
        Screening_Dots_85,
        Screening_Dots_90,
        Screening_Dots_95,

        Screening_Big_Dots_5,
        Screening_Big_Dots_10,
        Screening_Big_Dots_20,
        Screening_Big_Dots_30,
        Screening_Big_Dots_40,
        Screening_Big_Dots_50,
        Screening_Big_Dots_60,
        Screening_Big_Dots_65,
        Screening_Big_Dots_70,
        Screening_Big_Dots_75,
        Screening_Big_Dots_80,
        Screening_Big_Dots_85,
        Screening_Big_Dots_90,
        Screening_Big_Dots_95,

        Count,            /**< Not a pattern.  Should be the number of patterns + 1. */
        Undefined = -1    /**< Not a pattern.*/
    };

protected:

    WT_Pattern_ID           m_id;
    WT_Unsigned_Integer32   m_screening_percentage;
    WT_Color_Map *          m_color_map;
    WT_Boolean              m_local_color_map_copy;

    enum
    {
        Starting,
        Getting_Pattern_Id,
        Getting_Screening_Percentage,
        Getting_Transparency_ColorMap_Flag,
        Getting_Color_Map_Opcode,
        Getting_Color_Map,
        Getting_Pre_Data_Size_Whitespace,
        Getting_Pre_Data_Size_Open_Paren,
        Getting_Close
    }                       m_stage;

    WT_Opcode               m_colormap_opcode;
    
public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Pen_Pattern object.
    WT_Pen_Pattern ()
                : m_id(Screening_Black)
                , m_screening_percentage(0)
                , m_color_map(WD_Null)
                , m_local_color_map_copy(WD_False)
                , m_stage(Starting)
            { }
    /// Constructs a WT_Pen_Pattern object with the given predefined pattern ID.
    WT_Pen_Pattern (WT_Pattern_ID pattern_id)
                : m_id(pattern_id)
                , m_screening_percentage(0)
                , m_color_map(WD_Null)
                , m_local_color_map_copy(WD_False)
                , m_stage(Starting)
            { }
    /// Constructs a WT_Pen_Pattern object with the given data.
    WT_Pen_Pattern(
            WT_Pattern_ID                pattern_id, /**< The pen or face pattern ID, one of \ref WT_Pattern_ID "enum WT_Pattern_ID". */
            WT_Unsigned_Integer32        screening_percentage, /**< If specified, \a pattern_id must be one of Screening_Black, Screening_Alternate, Screening_Block, Screening_Dots, Screening_Big_Dots. */
            WT_Boolean                   colormap_flag, /**< Indicates whether the \a color_map contains valid content. */
            WT_Color_Map const *         color_map, /**< A 2-color map, indicating background and foreground colors respectively. */
            WT_Boolean                   copy) throw(WT_Result);
    /// Constructs a WT_Pen_Pattern object. Copy constructor.
    WT_Pen_Pattern(WT_Pen_Pattern const & pattern) throw(WT_Result);

    /// Destroys a a WT_Pen_Pattern object.
    virtual ~WT_Pen_Pattern()
    {
        if (m_local_color_map_copy && m_color_map!=NULL)
            delete [] m_color_map;
    }
    //@}
public:
    //@{ \name Data access methods
    WT_Boolean              is_local_color_map_copy() const {   return m_local_color_map_copy;  }

    /// Returns the color map used for this pen pattern (may be NULL.)
    WT_Color_Map const *    color_map() const {   return m_color_map;     }
    /// Returns WD_True if the color map has valid content
    WT_Pattern_ID           pen_pattern() const {   return m_id;  }
    /// Returns the screening percentage, if using a pen set (as opposed to a face pattern.)
    WT_Unsigned_Integer32   screening_percentage() const {   return m_screening_percentage;  }
    /// Sets the pen pattern to a predefined pattern ID, one of \ref WT_Pattern_ID "enum WT_Pattern_ID".
    void                    set_pen_pattern(WT_Pattern_ID pattern_id) {   m_id = pattern_id;  }
    /// Sets the screening percentage, if using a pen set (as opposed to a face pattern.)
    void                    set_screening_percentage (const WT_Unsigned_Integer32 screening_percentage) {   m_screening_percentage = screening_percentage;  }
    /// Assignment operator. Sets the pen pattern ID to a predefined pattern ID, one of \ref WT_Pattern_ID "enum WT_Pattern_ID".
    virtual inline WT_Pen_Pattern&  operator= ( WT_Pattern_ID const & id )  {  m_id = id;  return *this;  }
    /// Assignment operator. Copies the given pattern object contents to this one.
    virtual WT_Pen_Pattern&         operator= ( const WT_Pen_Pattern& pen_pattern ) throw(WT_Result);
    /// Returns WD_True if the given pattern is NOT equal to this one.
    WT_Boolean              operator!= ( WT_Pen_Pattern const & pattern ) const;
    /// Returns WD_True if the given pattern ID is equal to this one.
    inline WT_Boolean       operator == ( WT_Pattern_ID const & id ) const { return id == m_id; }
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
        WT_Pen_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // PENPAT_HEADER
