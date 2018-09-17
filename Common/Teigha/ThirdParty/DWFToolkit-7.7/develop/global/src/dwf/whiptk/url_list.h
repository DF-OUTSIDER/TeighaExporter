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

#if !defined URL_LIST_HEADER
#define URL_LIST_HEADER

///
///\file        dwf/whiptk/url_list.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

/// Describes a single-target hyperlink item for use with WT_URL_List.
/** \sa WT_URL_List \sa WT_URL */
class WHIPTK_API WT_URL_Item : public WT_Item
{
    friend class WT_URL;
    friend class WT_URL_List;
    friend class WT_Attribute_URL;
private:
    WT_String m_address;
    WT_String m_friendly_name;
    WT_Boolean m_emitted;
    WT_Integer32 m_index;
    void set_emitted_status(WT_Boolean status) { m_emitted = status; }
    void _deleteObject(void *object)
    {
        delete (WT_URL_Item*)object;
    }

public:

    //@{ \name Construction
    /// Constructs a WT_URL_Item object.
    WT_URL_Item()
        : m_index(0)
    {};
    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const * address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const * friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        set(index, address, friendly_name);
    }
    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        char const *address, /**< Machine-readable target for this hyperlink. */
        char const *friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        set(index, address, friendly_name);
    }

    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_String & address, /**< Machine-readable target for this hyperlink. */
        WT_String & friendly_name /**< Human-readable friendly name for this hyperlink. */
        );

    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        WT_Unsigned_Integer16 const *address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const *friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        set(WD_URL_Optimize, address, friendly_name);
    }

    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        char const *address, /**< Machine-readable target for this hyperlink. */
        char const *friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        set(WD_URL_Optimize, address, friendly_name);
    }

    /// Constructs a WT_URL_Item object with the given data.
    WT_URL_Item(
        WT_String & address, /**< Machine-readable target for this hyperlink. */
        WT_String & friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        m_address.set(address);
        m_friendly_name.set(friendly_name);
        m_emitted = WD_False;
    }

    /// Destructs a WT_URL_Item object
    virtual ~WT_URL_Item() {}
    //@}
public:
    /// Sets the hyperlink with the given data.
    void set(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const * address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const * friendly_name /**< Human-readable friendly name for this hyperlink. */
        );
    /// Sets the hyperlink with the given data.
    void set(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        char const *address, /**< Machine-readable target for this hyperlink. */
        char const *friendly_name /**< Human-readable friendly name for this hyperlink. */
        );

    /// Returns the achine-readable target for this hyperlink.
    WT_String    &    address()        { return m_address; }
    /// Returns the human-readable friendly name for this hyperlink.
    WT_String    &    friendly_name()  { return m_friendly_name; }
    /// Returns the unique index assigned to this hyperlink.
    WT_Integer32 &    index()          { return m_index; }
    /// Returns WD_True if this hyperlink has been serialized.
    WT_Boolean get_emitted_status()    { return m_emitted; }
};


/// A linked list of WT_URL_Item objects.
class WHIPTK_API WT_URL_List : public WT_Item_List, public WT_Object
{
	friend class WT_URL;
	friend class WT_Attribute_URL;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;

public:
    /// Constructs a WT_URL_List object.
    WT_URL_List()
    { }

    /// Constructs a WT_URL_List object.  Copy constructor.
    WT_URL_List(WT_URL_List const & list);

    /// Destroys a WT_URL_List object.
    virtual ~WT_URL_List();

public:
    /// Adds a WT_URL_Item object to the list.
    void add(WT_URL_Item &item);
    /// Returns WD_True if the URL has been emitted (serialized) already.
    /** \note As an optimization, once a URL has been serialized once, subsequent
     *  references can be referred to by index.
     */
    WT_Boolean check_whether_emitted(WT_Integer32 index) const;
    /// Returns the index of the WT_URL_Item if found (returns -1 if not found.)
    WT_Integer32  index_from_url_item(WT_URL_Item & item_in);
    /// Returns the WT_URL_Item object with the given index (returns NULL if not found.)
    WT_URL_Item * url_item_from_index(WT_Integer32 index);
    /// Sets a flag indicating that this URL has been emitted (serializaed.)
    /** \note As an optimization, once a URL has been serialized once, subsequent
     *  references can be referred to by index.
     */
    void set_as_being_emitted(WT_Integer32 index);

    // WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID        object_id() const;
    WT_Type      object_type() const;
    WT_Result    process(WT_File & file);
    virtual WT_Result    serialize (WT_File & file) const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);

    /// Assignment operator.  Assigns the given list contents to this one.
    WT_URL_List const & operator=(WT_URL_List const & url);
    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean       operator== (WT_URL_List const & list) const;
};

#endif // URL_LIST_HEADER
