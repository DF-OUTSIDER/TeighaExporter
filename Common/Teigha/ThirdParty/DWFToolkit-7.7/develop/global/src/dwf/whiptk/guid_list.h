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

#if !defined GUID_LIST_HEADER
#define GUID_LIST_HEADER

///
///\file        dwf/whiptk/guid_list.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

/// A linked list of WT_Guid objects.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 */
class WHIPTK_API WT_Guid_List : public WT_Object, public WT_Item_List
{
private:
    enum
    {
        Starting,
        Getting_No_Of_Guids,
        Getting_Guid,
        Getting_Close,
        Eating_End_Whitespace
    }   m_stage;

public:

    /// Constructs a WT_Guid_List object.
    WT_Guid_List()
    { m_stage = Starting;}

    /// Constructs a WT_Guid_List object.  Copy constructor.
    WT_Guid_List(const WT_Guid_List & list)
        : WT_Object()
        , WT_Item_List() {
        m_stage = Starting;
        *this = list;
    }

    /// Destroys a WT_Guid_List object.
    virtual ~WT_Guid_List();
public:
    //@{ \name Data access methods
    /// Adds a WT_Guid item to the list.
    void add (WT_Guid &item);
    /// Returns the total number of bytes which will be serialized for this object if written in binary form.
    WT_Unsigned_Integer16    get_total_opcode_binary_size();
    /// Assignment operator.  Assigns the given list contents to this one.
    WT_Guid_List const & operator=(WT_Guid_List const & list);
    //@}

    //@{ \name WT_Object virtual methods
    WT_Result materialize (WT_Opcode const & opcode, WT_File & file);
    WT_ID     object_id() const;
    WT_Type   object_type() const;
    WT_Result process(WT_File & file);
    WT_Result serialize(WT_File & file) const;
    WT_Result skip_operand (WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean operator== (WT_Guid_List const & list) const;

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Guid_List & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

#endif // GUID_LIST_HEADER
