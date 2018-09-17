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

#if !defined USER_DATA_HEADER
#define USER_DATA_HEADER

///
///\file        dwf/whiptk/userdata.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// An object containing user-defined data.
/** This data is generally ignored by any rendering applications which are not
 *  specifically designed to recognize the user-defined data.
 */
class WHIPTK_API WT_UserData : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:

    WT_String               m_data_description;
    WT_Integer32            m_data_size;
    WT_Byte *               m_data;
    WT_Boolean              m_local_data_copy;

    enum
    {
        Starting,
        Getting_Data_Description,
        Getting_Data_Size,
        Getting_Data,
        Getting_Close
    }   m_stage;
    WT_UserData (WT_UserData const &)
      : WT_Object()
      , m_data_description()
      , m_data_size(0)
      , m_data(WD_Null)
      , m_local_data_copy(WD_False)
      , m_stage(Starting)
    {
        WD_Complain ("cannot copy WT_UserData");
    }

    WT_UserData & operator= (WT_UserData const &)
    {
        WD_Complain ("cannot assign WT_UserData");
        return *this;
    }

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_UserData object.
    WT_UserData ()
                : m_data_size(0)
                , m_data(WD_Null)
                , m_local_data_copy(WD_False)
                , m_stage(Starting)
            { }
    /// Constructs a WT_UserData object with the given description.
    WT_UserData (const WT_String & data_description)
                : m_data_description(data_description)
                , m_data_size(0)
                , m_data(WD_Null)
                , m_local_data_copy(WD_False)
                , m_stage(Starting)
            { }
    /// Constructs a WT_UserData object with the given data.
    WT_UserData(
            const WT_String &           data_description, /**< Description of the user-defined data. */
            WT_Integer32                data_size, /**< Size of the \a data array in bytes. */
            WT_Byte *                   data, /**< Byte buffer containing user-defined data. */
            WT_Boolean                  copy /**< Whether the data should be copied or if its content should be used directly from the array. */
            ) throw(WT_Result);
    /// Destroys a WT_UserData object.
    virtual ~WT_UserData()
    {
        if (m_local_data_copy)
            delete [] m_data;
    }
    //@}
public:
    //@{ \name Data access methods
    /// Returns a read-only pointer to the data buffer containing the raw user data.
    WT_Byte const * data() const
    {   return m_data;  }
    /// Returns the description of this user-defined data.
    const WT_String & data_description() const
    {   return m_data_description;  }
    WT_String&  data_description()
    {   return m_data_description;  }
    /// Returns the size of the data buffer containing the user-defined data. */
    WT_Integer32 const& data_size() const
    {   return m_data_size; }
    WT_Integer32& data_size()
    {   return m_data_size; }
    WT_Result   set_data(WT_Integer32 size,WT_Byte* data);
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
        WT_UserData & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // USER_DATA_HEADER
