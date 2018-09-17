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

#if !defined GROUP_BEGIN_HEADER
#define GROUP_BEGIN_HEADER

///
///\file        dwf/whiptk/group_begin.h
///

#include "whiptk/wtstring.h"
#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Specifies the start of a group of objects.
/** \deprecated The functionality of this object has been absorbed by the more efficient
 * WT_Object_Node rendition attribute.
 *
 * \sa WT_Object_Node
 */
class WHIPTK_API WT_Group_Begin : public WT_Object
{
private:
    WT_String m_group_path;

    enum WT_Materialize_Stage
    {
        Gathering_Group_Path,
        Eating_End_Whitespace
    } m_stage;

public:
    //@{ \name Construction
    /// Constructs a WT_Group_Begin object.
    WT_Group_Begin()
        : m_stage(Gathering_Group_Path) //Gathering_Group_Name
    { }

    /// Constructs a WT_Group_Begin object with the given group path name.
    WT_Group_Begin(WT_String path_name)
        : m_group_path(path_name)
        , m_stage(Gathering_Group_Path)
    { }
	virtual ~WT_Group_Begin()
	{}

    //@{ \name Data access methods
    /// Returns the group path name.
    WT_String const & group_path() { return m_group_path; }
    //@}

    //@{ \name WT_Object virtual methods
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        process(WT_File & file);
    WT_Result        serialize(WT_File & file) const;
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Group_Begin & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // GROUP_BEGIN_HEADER
