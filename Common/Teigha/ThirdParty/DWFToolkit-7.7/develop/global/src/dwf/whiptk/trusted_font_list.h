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

#if !defined TRUSTED_FONT_LIST_HEADER
#define TRUSTED_FONT_LIST_HEADER

///
///\file        dwf/whiptk/trusted_font_list.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_Trusted_Font_Item;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// A linked list of WT_Trusted_Font_Item objects.
/** \deprecated  This contained all "trusted" non-standard fonts.  Starting with WHIP! version 00.55,
 *  WHIP! data authors have the flexibility of embedding subsetted font information using WT_Embedded_Font
 *  or assume the presence of certain "trusted" fonts using this object.  While good information for an
 *  authoring app (tells the app which utilized fonts should be embedded), this information is pretty
 *  useless in the WHIP! data.  Use of this object is discouraged. This remains in the toolkit only for
 *  backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Trusted_Font_List : public WT_Object, public WT_Item_List
{
public:
    /// Constructs a WT_Trusted_Font_Item object.
    WT_Trusted_Font_List()
    { }

    /// Destroys a WT_Trusted_Font_Item object.
    virtual ~WT_Trusted_Font_List();

    /// Adds a WT_Trusted_Font_Item item to the list.
    void add (WT_Trusted_Font_Item &item);
    /// Returns the index of the given WT_Trusted_Font_Item in the list if found (returns -1 if not found.)
    WT_Integer32 index_from_trusted_font_item(WT_Trusted_Font_Item & item_in);
    /// Returns the WT_Trusted_Font_Item with the given index (returns NULL if not found.)
    WT_Trusted_Font_Item * trusted_font_item_from_index(WT_Integer32 index);

    //WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID        object_id() const;
    WT_Type      object_type() const;
    WT_Result    process(WT_File & file);
    WT_Result    serialize (WT_File & file) const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);

    /// Assignment operator.  Assigns the given list contents to this one.
    WT_Trusted_Font_List const & operator=(WT_Trusted_Font_List const & trusted_font);
    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean operator== (WT_Trusted_Font_List const & list) const;

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Trusted_Font_List & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );


};

/// A single trusted font contained in a WT_Trusted_Font_List object.
class WHIPTK_API WT_Trusted_Font_Item : public WT_Item
{
private:
    WT_String m_font_name;
    WT_Integer32 m_index;
    void _deleteObject(void *object)
    {
        delete (WT_Trusted_Font_Item*)object;
    }


public:

    //@{ \name Construction
    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item()
        : m_index(0)
    {}

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        WT_Integer32 index, /**< The unique identifier for the font item. */
        WT_Unsigned_Integer16 const *font_name /**< The font name. */
        )
    { set(index, font_name); }

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        WT_Integer32 index, /**< The unique identifier for the font item. */
        char const *font_name /**< The font name. */
        )
    { set(index, font_name); }

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        WT_Integer32 index, /**< The unique identifier for the font item. */
        WT_String & font_name_in /**< The font name. */
        );

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        WT_Unsigned_Integer16 const *font_name /**< The font name. */
        )
    { set(WD_URL_Optimize, font_name); }

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        char const *font_name /**< The font name. */
        )
    { set(WD_URL_Optimize, font_name); }

    /// Creates a WT_Trusted_Font_Item object.
    WT_Trusted_Font_Item(
        WT_String & font_name_in /**< The font name. */
        )
    { m_index = WD_URL_Optimize; m_font_name.set(font_name_in); }

	virtual ~WT_Trusted_Font_Item()
	{}
    //@}

    //@{ \name Data access members
    /// Returns the font name.
    WT_String    &    font_name()        { return m_font_name; }
    /// Returns the unique font index.
    WT_Integer32      index()          { return m_index; }
    /// Sets the font information from the given data.
    void set(
        WT_Integer32 index, /**< The unique identifier for the font item. */
        WT_Unsigned_Integer16 const *font_name /**< The font name. */
        );
    /// Sets the font information from the given data.
    void set(
        WT_Integer32 index, /**< The unique identifier for the font item. */
        char const *font_name /**< The font name. */
        );
    //@}
};

//@}

#endif // TRUSTED_FONT_LIST_HEADER
