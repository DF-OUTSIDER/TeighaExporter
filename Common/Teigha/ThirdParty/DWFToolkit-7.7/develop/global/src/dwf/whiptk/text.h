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

#if !defined TEXT_HEADER
#define TEXT_HEADER

///
///\file        dwf/whiptk/text.h
///

#include "whiptk/text_options.h"
#include "whiptk/drawable.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing text.
/** \sa WT_Font \sa \ref groupTextOptions "Text options"
 *
 *  Working with international text is not as straightforward as it may seem. It may help to
 *  know a little about character encoding and display before using international text in WHIP!
 *  data. These notes are provided as a brief overview of supporting international text.
 *
 *  <b>Using International Character Sets</b>
 *
 *  Text characters are written to a file as either single-byte characters, double-byte characters,
 *  or both single and double-byte character sets. The method used to encode characters usually
 *  depends on the written local language of the operating system.  English has a relatively small
 *  character set of only 256 written characters. The 256 English characters are commonly referred
 *  to as the ASCII character set. ASCII characters can each be represented in one byte (8 bits) of
 *  computer memory. ASCII has a limited number of characters, but most written languages have many
 *  more characters than 256. As a result, international languages (other than English) must be
 *  represented using two byte (16 bits) character sets.
 *
 *  \note Although computer applications could simply represent all languages using two bytes of
 *  memory per character, this is not done for the English character set since the characters would
 *  consume twice as much memory as is needed using ASCII , and resulting file sizes would be
 *  unnecessarily large.
 *
 *  Using international character sets is complicated by the fact that there are two different ways
 *  to encode multiple bytes of computer memory for text: Unicode and Multi-Byte Character Set (MBCS):
 *
 *  \li Unicode is a map of characters in which each character corresponds to a unique two byte value.
 *  The Unicode character map contains characters from most of the world's languages.  Unicode character
 *  values are always two bytes and there is only one mapping such that a given number always maps to the
 *  same character on every computer using Unicode.
 *  \li Multi-Byte Character Sets (MBCS) are code pages, or maps, between written characters and either
 *  one byte or two byte numbers. As a result, a string with several MBCS characters can have both single
 *  and double-byte characters. Unlike Unicode, one number is not unique to one character. When using MBCS,
 *  a given number might correspond to a Chinese character when using a Chinese character set, or it might
 *  correspond to a Japanese character when using a Japanese character set.
 *
 *  Another difference between MBCS and Unicode is that different MBCS platforms (such as Unix, Microsoft
 *  Windows, or Macintosh) may have different character sets for the same written language. For example,
 *  when using MBCS a given number might map to a Japanese character on Japanese Windows, but the same
 *  number may map to another character on English Windows.
 *
 *  <b>Operating Systems and International Characters</b>
 *
 *  Another consideration in supporting international characters is that different operating systems
 *  support different character encoding methods. For example, Windows NT/2000/XP fully support both Unicode
 *  and MBCS, but Windows9x fully supports MBCS, and only partially supports Unicode. To maximize the
 *  efficiency with which WHIP! operates, and to ensure the smallest possible file size, the strategy for
 *  WHIP! data is:
 *  \li Text strings containing only ASCII characters are stored in WHIP! files as ASCII.  By using one
 *  byte instead of two, file size remains small.
 *  \li Text strings containing multi-byte characters are stored in WHIP! files as Unicode. This is the
 *  most flexible and universal approach for ensuring that WHIP! data works now and in the future. This
 *  also enables WHIP! data to be used on non-Microsoft operating systems.
 *
 *  \image html TextBounding.png "Text baseline and bounding box details"
 *
 */
class WHIPTK_API WT_Text : public WT_Drawable
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Logical_Point                    m_position;
    WT_String                           m_string;
    WT_Text_Option_Bounds               m_option_bounds;
    WT_Text_Option_Overscore            m_option_overscore;
    WT_Text_Option_Underscore           m_option_underscore;
    WT_Text_Option_Reserved   m_option_reserved;

    /// Materialization stage.
    enum
    {
        Getting_Started,
        Getting_Position,
        Getting_String,
        Getting_Overscore,
        Getting_Underscore,
        Getting_Bounds,
        Getting_Reserved,
        Getting_Next_Optioncode,
        Materializing_Option,
        Skipping_Last_Paren,
        Completed,
        // Obsolete forms of DrawText use the following:
        Getting_Width_Scale,
        Getting_Spacing,
        Getting_Flags,
        Getting_Oblique_Angle,
        Getting_Rotation,
        Getting_Height,
        Getting_BBox_Deltas,
        Getting_Str_Length,
        Getting_Msg
    }               m_stage;

    WT_Boolean          m_transformed;
    WT_Boolean          m_relativized;
    WT_Text_Optioncode  m_optioncode;

    WT_Font *               m_obsolete_font_holder;
    WT_Integer32            m_obsolete_length_holder;
    WT_Unsigned_Integer16 * m_obsolete_msg_holder;

public:
    /// Constructs a WT_Text object.
    WT_Text();

    /// Overrides default copy constructor
    WT_Text (WT_Text const &);
  

    /// Constructs a WT_Text object with the given data.
    WT_Text(
        WT_Logical_Point const &        position, /**< Insertion point at the baseline of the text (lower left point.) */
        WT_String const &               string, /**< Text string. */
        WT_Logical_Point const *        bbox, /**< Bounding box into which text must fit (the lower left point should equal the \a position argument.) */
        WT_Unsigned_Integer16           overscore_count, /**< The number of overscore position indices in the following array argument. */
        WT_Unsigned_Integer16 const *   overscore_pos, /**< An array containing the position indicies of characters in the string which should receive overscores. */
        WT_Unsigned_Integer16           underscore_count, /**< The number of underscore position indices in the following array argument. */
        WT_Unsigned_Integer16 const *   underscore_pos /**< An array containing the position indicies of characters in the string which should receive underscores. */
        );

    /// Constructs a WT_Text object with the given data.
    WT_Text(
        WT_Logical_Point const &        position, /**< Insertion point at the baseline of the text (lower left point.) */
        WT_String const &               string /**< Text string. */
        );

    /// Destroys a WT_Text object.
    virtual ~WT_Text();
public:

	 /// Overrides default assignment operator
    WT_Text const & operator= (WT_Text const &);

    //@{ \name Data access methods

    /// Returns the string bounding box option object.
    WT_Text_Option_Bounds const &       bounds()     const  {   return m_option_bounds;      }
    WT_Text_Option_Bounds       &       bounds()            {   return m_option_bounds;      }

    /// Returns the string overscore option object.
    WT_Text_Option_Overscore const &    overscore()  const  {   return m_option_overscore;   }
    WT_Text_Option_Overscore       &    overscore()         {   return m_option_overscore;   }

    /// Returns the insertion point at the baseline of the text (lower left point.)
    WT_Logical_Point const &            position()   const  {   return m_position;         }
    WT_Logical_Point       &            position()          {   return m_position;         }

    /// Returns the text string.
    WT_String const &                   string()     const  {   return m_string;           }
    WT_String       &                   string()            {   return m_string;           }

    /// Returns the string underscore option object.
    WT_Text_Option_Underscore const &   underscore() const  {   return m_option_underscore;  }
    WT_Text_Option_Underscore       &   underscore()        {   return m_option_underscore;  }
    //@}

    //@{ \name Manipulation methods
    /// Returns the position and bounding box points to absolute values based on their relative position to last materialized point.
    virtual void        de_relativize(WT_File & file);
    /// Relativizes the position and bounding box points to the last serialized point.
    virtual void        relativize(WT_File & file);
    /// Applies the given transform to the position and bounding box points.
    virtual void        transform(WT_Transform const & transform);
    //@}

    //@{ \name WT_Object virtual methods
    WT_ID              object_id() const;
    WT_Result          materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result          process(WT_File & file);
    virtual WT_Result  serialize(WT_File & file) const;
    WT_Result          skip_operand(WT_Opcode const & opcode, WT_File & file);
    virtual void       update_bounds(WT_File * file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Text & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_Result          materialize_obsolete_form(WT_Opcode const & opcode, WT_File & file);

};
//@}

#endif // TEXT_HEADER
