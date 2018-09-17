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

#if !defined FILETIME_HEADER
#define FILETIME_HEADER

///
///\file        dwf/whiptk/filetime.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/// BlockRef attribute.  Describes a file timestamp.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_FileTime : public WT_Object
{
private:
    WT_Unsigned_Integer32        m_low_date_time;
    WT_Unsigned_Integer32        m_high_date_time;

    enum {
        Starting,
        Getting_Low_Date_Time,
        Getting_High_Date_Time,
        Getting_Close
    }   m_stage;

public:

    // Constructors, Destructors

    WT_FileTime()
        : m_low_date_time(0) , m_high_date_time(0), m_stage(Starting)
    { }

    WT_FileTime(const WT_Unsigned_Integer32 & low_date_time, const WT_Unsigned_Integer32 & high_date_time)
        : m_low_date_time(low_date_time) , m_high_date_time(high_date_time), m_stage(Starting)
    {

    }

    virtual ~WT_FileTime()
    { }

    // Mandatory WT_Object Virtual methods

    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        serialize (WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);

    static WT_Result default_process(WT_FileTime & item, WT_File & file);


    WT_Unsigned_Integer32 const &    low_date_time() const
    {    return m_low_date_time;    }
    WT_Unsigned_Integer32 const &    high_date_time() const
    {    return m_high_date_time;    }
    void set_low_date_time(WT_Unsigned_Integer32 const low_date_time)
    {    m_low_date_time = low_date_time;    }
    void set_high_date_time(WT_Unsigned_Integer32 const high_date_time)
    {    m_high_date_time = high_date_time;    }

    WT_Boolean    operator!= (WT_FileTime const & time) const;
    WT_Boolean    operator== (WT_FileTime const & time) const;
};

#endif // FILETIME_HEADER
