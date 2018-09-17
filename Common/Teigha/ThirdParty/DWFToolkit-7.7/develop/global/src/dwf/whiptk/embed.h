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

#if !defined EMBED_HEADER
#define EMBED_HEADER

///
///\file        dwf/whiptk/embed.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Allows the source file from which the WHIP! data was created to be embedded (or referenced) inside the WHIP! data.
/** This helps to facilitate smart drag-and-drop operations for those consuming applications which do so. */
class WHIPTK_API WT_Embed : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Gathering_MIME,
        Eating_Post_MIME_Whitespace,
        Gathering_Description,
        Eating_Post_Description_Whitespace,
        Gathering_Filename,
        Eating_Post_Filename_Whitespace,
        Gathering_URL,
        Eating_Trailing_Whitespace
    };

    WT_String                m_MIME_type;
    WT_String                m_MIME_subtype;
    WT_String                m_MIME_options;
    WT_String                m_description;
    WT_String                m_filename;
    WT_String                m_url;
    WT_Materialize_Stage    m_stage;
    int                        m_incarnation;

public:

    //@{ \name Construction
    /// Constructs a WT_Embed object.
    WT_Embed()
        : m_stage(Eating_Initial_Whitespace)
        , m_incarnation(-1)
    { }
    /// Constructs a WT_Embed object. Copy constructor.
    WT_Embed(WT_Embed const & embed);
    //@}
	virtual ~WT_Embed()
	{}
public:

    //@{ \name Data access methods
    /// Returns the human-readable description of the source data.
    WT_String const & description() const { return m_description;    }
    /// Returns the source data file name.
    WT_String const & filename() const { return m_filename;    }
    /// Returns the MIME options field.
    WT_String const & MIME_options() const { return m_MIME_options;    }
    /// Returns the MIME subtype of the data being embedded.
    /** If the full mime type were \em text/html, the MIME subtype would be \em html. */
    WT_String const & MIME_subtype() const { return m_MIME_subtype;    }
    /// Returns the standard MIME (Multipurpose Internet Mail Extension) specification type field of the data being embedded.
    /** If the full mime type were \em text/html, the MIME type would be \em text. */
    WT_String const & MIME_type() const  { return m_MIME_type; }
    /// Sets the human-readable description of the source data.
    WT_Result         set_description(char const * description, WT_File & file);
    /// Sets the human-readable description of the source data.
    WT_Result         set_description(WT_Unsigned_Integer16 const * description, WT_File & file);
    /// Sets the source data file name.
    WT_Result         set_filename(char const * filename, WT_File & file);
    /// Sets the source data file name.
    WT_Result         set_filename(WT_Unsigned_Integer16 const * filename, WT_File & file);
    /// Sets the MIME options field.
    WT_Result         set_MIME_options(char const * options, WT_File & file);
    /// Sets the MIME options field.
    WT_Result         set_MIME_options(WT_Unsigned_Integer16 const * options, WT_File & file);
    /// Sets the MIME subtype of the data being embedded.
    /** If the full mime type were \em text/html, the MIME subtype would be \em html. */
    WT_Result         set_MIME_subtype(char const * subtype, WT_File & file);
    /// Sets the MIME subtype of the data being embedded.
    /** If the full mime type were \em text/html, the MIME subtype would be \em html. */
    WT_Result         set_MIME_subtype(WT_Unsigned_Integer16 const * subtype, WT_File & file);
    /// Sets the standard MIME (Multipurpose Internet Mail Extension) specification type field of the data being embedded.
    /** If the full mime type were \em text/html, the MIME type would be \em text. */
    WT_Result         set_MIME_type(char const * type, WT_File & file);
    /// Sets the standard MIME (Multipurpose Internet Mail Extension) specification type field of the data being embedded.
    /** If the full mime type were \em text/html, the MIME type would be \em text. */
    WT_Result         set_MIME_type(WT_Unsigned_Integer16 const * type, WT_File & file);
    /// Sets the URL of the source data file.
    WT_Result         set_url(char const * url, WT_File & file);
    /// Sets the URL of the source data file.
    WT_Result         set_url(WT_Unsigned_Integer16 const * url, WT_File & file);
    /// Sets the full MIME type, i.e. \em text/html
    WT_Result         set_whole_MIME(char const * mime, WT_File & file);
    /// Returns the URL of the source data file.
    WT_String const & url() const { return m_url;    }
    /// Assignment operator.  Copies the contents of the given embed object to this one.
    WT_Embed const &  operator=(WT_Embed const & embed);
    /// Returns WD_True if the given object is equal to this one.
    WT_Boolean  operator== (WT_Object const & object) const;

    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Embed & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // EMBED_HEADER
