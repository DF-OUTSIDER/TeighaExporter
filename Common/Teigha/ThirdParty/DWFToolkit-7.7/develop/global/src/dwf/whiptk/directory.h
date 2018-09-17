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

#if !defined DIRECTORY_HEADER
#define DIRECTORY_HEADER

///
///\file        dwf/whiptk/directory.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/blockref.h"

/// A linked list of WT_BlockRef objects.
/** \deprecated This class is provided for backward compatibiility with %DWF 00.55
 *  files.  This object is managed by the framework and is not typically used by
 *  client code.
 */
class WHIPTK_API WT_Directory : public WT_Object, public WT_Item_List
{
public:
    /// Constructs a WT_Directory object.
    WT_Directory() : m_file_offset(0), m_optioncode(WD_Null), m_stage(Starting)
    { }

    /// Constructs a WT_Directory object.  Copy constructor.
    WT_Directory(const WT_Directory & directory)
        : WT_Object()
        , WT_Item_List()
    {
        *this = directory;
    }

    /// Destroys a WT_Directory object.
    virtual ~WT_Directory();

    /// Adds a WT_BlockRef item to the list.
    WT_Result add (WT_BlockRef &item);
    /// Returns the WT_BlockRef with the given index (returns NULL if not found.)
    WT_BlockRef * blockref_from_index(WT_Integer32 index);
    /// Returns the index of the given WT_BlockRef in the list if found (returns -1 if not found.)
    WT_Integer32  index_from_blockref(WT_BlockRef & item_in);
    /// Returns the offset of the WT_Directory object within the file.
    WT_Unsigned_Integer32 get_file_offset() {
       return m_file_offset;
    }

    // WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID        object_id() const;
    WT_Type      object_type() const;
    WT_Result    process(WT_File & file);
    WT_Result    serialize (WT_File & file) const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);

    /// Assignment operator.  Assigns the given list contents to this one.
    WT_Directory const & operator=(WT_Directory const & blockref);
    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean operator== (WT_Directory const & list) const;

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Directory & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_Unsigned_Integer32   m_file_offset;
    WT_Opcode *             m_optioncode;

    enum
    {
        Starting,
        Getting_BlockRef_Count,
        Getting_BlockRefs,
        Getting_Directory_File_Offset,
        Getting_Close
    }   m_stage;
};

#endif // DIRECTORY_HEADER
