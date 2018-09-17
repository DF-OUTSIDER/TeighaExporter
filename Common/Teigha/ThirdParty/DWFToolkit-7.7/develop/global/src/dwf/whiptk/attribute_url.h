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

#if !defined ATTRIBUTE_URL_HEADER
#define ATTRIBUTE_URL_HEADER

///
///\file        dwf/whiptk/attribute_url.h
///


#include "whiptk/url.h"

class WT_File;
class WT_URL_Item;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the a single or multiple-target hyperlink to be applied to a subsequent attribute of a given type.
/** \note This attribute will not be written until after the subsequent dependent attribute type is written.
 *  \sa WT_Rendition
 */
class WHIPTK_API WT_Attribute_URL : public WT_Attribute
{
    friend class WT_Rendition;
	friend class WT_Opcode;
	friend class WT_W2D_Class_Factory;

protected:

    int                          m_incarnation;
    WT_Integer32                 m_current_index;
    WT_String                    m_current_address;
    WT_String                    m_current_friendly_name;
    WT_URL_List                  m_url_list;
    WT_Integer32                 m_count;
    WT_Integer32                 m_attribute_id;

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,

        Gathering_String,
        Eating_End_Whitespace,
        Skipping_Last_Paren,
		Reading_AttributeID,

        Getting_Next_Block,
        Getting_Assigned_Index,
        Getting_Address,
        Getting_Friendly_Name,
        Getting_Closing_Block_Paren,
        Reading_Index,
        Completed

    }  m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_Attribute_URL object.
    WT_Attribute_URL()
        : m_incarnation(-1)
        , m_current_index(0)
        , m_count(0)
        , m_attribute_id(-1)
        , m_stage(Eating_Initial_Whitespace)
    { }

    /// Constructs a WT_Attribute_URL object with the given dependent attribute.
    WT_Attribute_URL(WT_Attribute &attribute);

    /// Constructs a WT_Attribute_URL object with the given dependent attribute and initial URL item.
    WT_Attribute_URL(WT_Attribute &attribute, WT_URL_Item &item);

    /// Constructs a WT_Attribute_URL object with the given initial data.
    WT_Attribute_URL(
        WT_Attribute &attribute, /**< Attribute to which this object's URLs apply. */
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const * address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const * friendly_name /**< Human-readable friendly name for this hyperlink. */
        );

	/// Copy Constructor
	WT_Attribute_URL(WT_Attribute_URL const& url)
	{
		*this = url;
	}

	virtual ~WT_Attribute_URL()
	{}
    //@}

public:
    //@{ \name Data access methods

    /// Returns the ID of the attributes to which the contained URLs apply
    WT_Integer32 const& attribute_id(void) const;

    inline WT_Integer32& attribute_id() { return m_attribute_id; }

    /// Adds the specified URL item to the list of current URLs.
    /** \warning Clients must call flush_rendition() before AND after adding urls to this object.
     *  To avoid this, call add_url_optimized() instead.
     *  \deprecated Clients should always use add_url_optimized().
     */
    void add_url(
        WT_URL_Item & item
        )
    {
        WD_Assert((item.index() != WD_URL_Optimize)); // user should use add_url_optimized!
        m_url_list.add(item);
    };

    /// Adds the specified URL data to the list of current URLs.
    /** \warning Clients must call flush_rendition() before AND after adding urls to this object.
     *  To avoid this, call add_url_optimized() instead.
     *  \note Clients should always use add_url_optimized().
     */
    void add_url(
        WT_Integer32 index,
        WT_Unsigned_Integer16 const *address,
        WT_Unsigned_Integer16 const *friendly_name
        )
    {
        set(index, address, friendly_name);
    }

    /// Adds the specified URL data to the list of current URLs.
    /** \warning Clients must call flush_rendition() before AND after adding urls to this object.
     *  To avoid this, call add_url_optimized() instead.
     *  \note Clients should always use add_url_optimized().
     */
    void add_url(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_String & address, /**< Machine-readable target for this hyperlink. */
        WT_String & friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
    {
        WT_URL_Item item(index, address, friendly_name);
        add_url(item);
    }

    /// Adds the specified URL item to the list of current URLs.
    /** If the URL item has already been fully serialized once, only its idenfitier will
     *  be outputted when serialized subsequently. */
    void add_url_optimized(
        WT_URL_Item & item, /**< The URL to add. */
        WT_File & file /**< The file being written. */
        );

    /// Adds the specified URL item to the list of current URLs.
    /** If the URL item has already been fully serialized once, only its idenfitier will
     *  be outputted when serialized for subsequent graphics. */
    void add_url_optimized(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const *address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const *friendly_name, /**< Human-readable friendly name for this hyperlink. */
        WT_File & file /**< The file being written. */
        )
    {
        WT_URL_Item item(index, address, friendly_name);
        add_url_optimized(item, file);
    }

    /// Adds the specified URL item to the list of current URLs.
    /** If the URL item has already been fully serialized once, only its idenfitier will
     *  be outputted when serialized for subsequent graphics. */
    void add_url_optimized(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_String & address, /**< Machine-readable target for this hyperlink. */
        WT_String & friendly_name, /**< Human-readable friendly name for this hyperlink. */
        WT_File & file /**< The file being written. */
        )
    {
        WT_URL_Item item(index, address, friendly_name);
        add_url_optimized(item, file);
    }

    /// Clears the list of current URLs.
    virtual void clear() { 
        m_url_list.remove_all(); 
        m_attribute_id = -1; 
        m_count = 0;
    }

    /// Updates the incarnation value so that the URLs will serialized when needed.
    /** \warning Clients must call flush_rendition() before AND after adding urls to this object.
     *  To avoid this, call add_url_optimized() instead of add_url().
     *  \note Clients should always use add_url_optimized(), and if they do so, they
     *  need not use flush_rendition().
     */
    void flush_rendition(WT_File & file) ;

    /// Adds the specified URL data to the list of current URLs.
    void set(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const * address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const * friendly_name /**< Human-readable friendly name for this hyperlink. */
        );

    /// Returns the list of current URLs.
    WT_URL_List const & url()
    {    return m_url_list;    }

    /// Assignment operator.  Copies the contents of the given WT_Attribute_URL object to this one.
    virtual WT_Attribute_URL const & operator=(WT_Attribute_URL const & url);
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
        WT_Attribute_URL & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // URL_HEADER
