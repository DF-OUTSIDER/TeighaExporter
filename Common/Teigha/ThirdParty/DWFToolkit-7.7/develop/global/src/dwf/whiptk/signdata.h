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


#if !defined SIGNDATA_HEADER
#define SIGNDATA_HEADER

///
///\file        dwf/whiptk/signdata.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"
#include "whiptk/guid_list.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Holds raw digital signature data related to a collection of BlockRefs
/** \deprecated The low-level specification of digital signature for the WHIP!
 *  format was never completed and it was decided that digital signature information
 *  is better served in the %DWF package format rather than in the 2D channel file.
 *  Further, the WT_SignData opcode related specifically to a collection of WT_BlockRef
 *  objects, which have also been has been deprecated.  This remains in the toolkit
 *  only for backward compatability with %DWF version 00.55.
 */
class WHIPTK_API WT_SignData : public WT_Object
{
private:

    WT_Guid_List            m_block_guid_list;
    WT_Integer32            m_data_size;
    WT_Byte *               m_data;
    WT_Boolean              m_local_data_copy;

    enum
    {
        Starting,
        Getting_Block_List_Flag,
        Getting_Block_List,
        Getting_Data_Size,
        Getting_Data,
        Getting_Close
    }   m_stage;

public:

    WT_SignData ()
    : m_block_guid_list()
    , m_data_size(0)
    , m_data(WD_Null)
    , m_local_data_copy(WD_False)
    , m_stage(Starting)
    { }

    WT_SignData (
        WT_Guid_List block_guid_list
        )
    : m_data_size(0)
    , m_data(WD_Null)
    , m_local_data_copy(WD_False)
    , m_stage(Starting)
    {
        m_block_guid_list = block_guid_list;
    }

    WT_SignData(
            WT_Guid_List                block_guid_list,
            WT_Integer32                data_size,
            WT_Byte *                   data,
            WT_Boolean                  copy) throw(WT_Result);

    WT_SignData(const WT_SignData & signdata) throw(WT_Result)
    : WT_Object() 
    {
        *this = signdata;
    }

    virtual ~WT_SignData()
    {
        if (m_local_data_copy)
            delete [] m_data;
    }

    WT_BlockRef_List get_blockref_list(
        WT_Guid_List & guid_list, WT_File & file);

    WT_Integer32 data_size() const
    {   return m_data_size; }

    WT_Byte const * data() const
    {   return m_data;  }

    WT_Guid_List const block_guid_list() const
    {   return m_block_guid_list;  }

    WT_SignData const & operator=(WT_SignData const & signdata) throw(WT_Result)
	{
        m_block_guid_list = signdata.block_guid_list();
        m_data_size = signdata.data_size();
        m_data = WD_Null;
        if(m_data_size) {
            m_local_data_copy = WD_True;
            m_data = new WT_Byte[m_data_size];
            if(!m_data)
                throw WT_Result::Out_Of_Memory_Error;
            memcpy((void *) &m_data, signdata.m_data,
                sizeof(WT_Byte) * m_data_size);
        }

        return *this;
    }

    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        serialize(WT_File & file) const;
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);

    static WT_Result default_process( WT_SignData & item, WT_File & file );

};

//@}

#endif // SIGNDATA_HEADER
