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

#if !defined UNKNOWN_HEADER
#define UNKNOWN_HEADER

///
///\file        dwf/whiptk/unknown.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/// An internal class used for unknown objects.
/** Note, the data buffer contained in the object is allocated and populated in the materialize() method. */
class WHIPTK_API WT_Unknown : public WT_Object
{
    friend class WT_W2D_Class_Factory;
    friend class WT_Opcode;

private:
    WT_Byte *pass_thru_bytes;
    WT_Unsigned_Integer16 total_opcode_size;

    WT_Unknown (WT_Unknown const &)
      : WT_Object()
      , pass_thru_bytes()
      , total_opcode_size()
    {
        WD_Complain ("cannot copy WT_Unknown");
    }  // prohibited

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Unknown object.
    WT_Unknown()
    {
        pass_thru_bytes = WD_Null;
        total_opcode_size = 0;
    }
    /// Destroys a WT_Unknown object.
    virtual ~WT_Unknown()
    {
        if(pass_thru_bytes != WD_Null)
            delete []pass_thru_bytes;
        pass_thru_bytes = WD_Null;
        total_opcode_size = 0;
    }
public:
    //@{ \name Data access methods
    /// Returns the length of the unknown data, used to pass through to serialization.
    WT_Unsigned_Integer16 const& get_pass_thru_byte_length() const { return total_opcode_size; }
    WT_Unsigned_Integer16& get_pass_thru_byte_length() { return total_opcode_size; }
    WT_Byte* get_pass_thru_bytes(void) { return pass_thru_bytes; }
    WT_Result   set_pass_thru_bytes(WT_Unsigned_Integer16 size,WT_Byte* data);

    /// If data is contained in this object, this method deletes the data.
    void release_pass_thru_bytes() {
        if(pass_thru_bytes != WD_Null)
            delete []pass_thru_bytes;
        pass_thru_bytes = WD_Null;
        total_opcode_size = 0;
    }

    /// Assignment operator.  Copies the data from the given unknown object to this one.
    inline WT_Unknown const & operator= (WT_Unknown const & unknown_object) throw(WT_Result)
    {
        WT_Unknown *punknown;
        punknown = (WT_Unknown *) &unknown_object;
        pass_thru_bytes = new WT_Byte[punknown->get_pass_thru_byte_length()];
        if(!pass_thru_bytes)
            throw WT_Result::Out_Of_Memory_Error;
        memcpy(pass_thru_bytes, punknown->get_pass_thru_bytes(),
            punknown->get_pass_thru_byte_length());

        return *this;
    }
    //@}

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
        WT_Unknown & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

#endif // UNKNOWN_HEADER
