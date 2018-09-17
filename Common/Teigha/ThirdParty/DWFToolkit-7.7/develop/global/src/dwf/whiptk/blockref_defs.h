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

#if !defined BLOCKREF_DEFS_HEADER
#define BLOCKREF_DEFS_HEADER

///
///\file        dwf/whiptk/blockref_defs.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/// BlockRef attribute. Identifies the type of block.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Block_Meaning : public WT_Object
{
public:

    enum WT_Block_Description
    {
        None      = 0x00000001,
        Seal      = 0x00000002,
        Stamp     = 0x00000004,
        Label     = 0x00000008,
        Redline   = 0x00000010,
        Reserved1 = 0x00000020,
        Reserved2 = 0x00000040
    };

private:
    WT_Block_Description   m_description;
    enum {
        Starting,
        Getting_Description,
        Getting_Close
    }   m_stage;

public:

    // Constructors, Destructors

    WT_Block_Meaning()
      : m_description(None)
      , m_stage(Starting)
    { }

    WT_Block_Meaning(WT_Block_Description description)
      : m_description(description)
      , m_stage(Starting)
    { }

    virtual ~WT_Block_Meaning()
    { }

    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        serialize (WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode,
                                                WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode,
                                                WT_File & file);
    WT_Result        process(WT_File & file);

    static WT_Result default_process(WT_Block_Meaning & item,
                                                WT_File & file);

    // Get/Set methods
    void set_block_description(
        WT_Block_Meaning::WT_Block_Description description);

        WT_Block_Meaning::WT_Block_Description  get_block_description() const;

    WT_Boolean    operator!= (WT_Block_Meaning const &
                                                    meaning) const;
    WT_Boolean    operator== (WT_Block_Meaning const &
                                                    meaning) const;
};

/// BlockRef attribute. Identifies the type of encryption, if the WHIP data is encrypted.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  Further, this opcode was primarily intended for future data encryption support that was never realized.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Encryption : public WT_Object
{
public:

    enum WT_Encryption_Description
    {
        None      = 0x00000001,
        Reserved1 = 0x00000002,
        Reserved2 = 0x00000004,
        Reserved3 = 0x00000008
    };

private:
    WT_Encryption_Description   m_description;

    enum {
        Starting,
        Getting_Description,
        Getting_Close
    }   m_stage;

public:
    // Constructors, Destructors

    WT_Encryption()
      : m_description(None)
      , m_stage(Starting)
    { }

    WT_Encryption(WT_Encryption_Description description)
      : m_description(description)
      , m_stage(Starting)
    { }

    virtual ~WT_Encryption()
    { }

    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        serialize (WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        process(WT_File & file);

    static WT_Result default_process(WT_Encryption & item,
                                                        WT_File & file);

    // Get/Set methods
    void set_encryption_description(
        WT_Encryption::WT_Encryption_Description description);
    WT_Encryption::WT_Encryption_Description
        get_encryption_description() const;

    WT_Boolean    operator!= (WT_Encryption const
                                                & encryption) const;
    WT_Boolean    operator== (WT_Encryption const
                                                & encryption) const;
};

/// BlockRef attribute.  Represents the relationship between the paper and image orientation of the graphics block that the opcode represents.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Orientation : public WT_Object
{
public:

    enum WT_Orientation_Description
    {
        Always_In_Sync       = 0x00000001,
        Always_Different     = 0x00000002,
        Decoupled            = 0x00000004
    };

private:
    WT_Orientation_Description   m_description;

    enum {
        Starting,
        Getting_Description,
        Getting_Close
    }   m_stage;

public:
    // Constructors, Destructors

    WT_Orientation()
      : m_description(Always_In_Sync)
      , m_stage(Starting)
    { }

    WT_Orientation(WT_Orientation_Description description)
      : m_description(description)
      , m_stage(Starting)
    { }

    virtual ~WT_Orientation()
    { }

    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        serialize (WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        process(WT_File & file);

    static WT_Result default_process(WT_Orientation & item,
                                                        WT_File & file);

    // Get/Set methods
    void set_orientation_description(
        WT_Orientation::WT_Orientation_Description description);
    WT_Orientation::WT_Orientation_Description
        get_orientation_description() const;

    WT_Boolean    operator!= (WT_Orientation const
                                            & orientation) const;
    WT_Boolean    operator== (WT_Orientation const
                                            & orientation) const;
};

/// BlockRef attribute.  Represents the alignment of all graphics in the graphic block, in a certain format, on the paper plot.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Alignment : public WT_Object
{
public:

    enum WT_Alignment_Description
    {
        Align_Center        = 0x00000001,
        Align_Title_Block   = 0x00000002,
        Align_Top           = 0x00000004,
        Align_Bottom        = 0x00000008,
        Align_Left          = 0x00000010,
        Align_Right         = 0x00000020,
        Align_Top_Left      = 0x00000040,
        Align_Top_Right     = 0x00000080,
        Align_Bottom_Left   = 0x00000100,
        Align_Bottom_Right  = 0x00000200,
        Align_None          = 0x00000400
    };

private:
    WT_Alignment_Description   m_description;

    enum {
        Starting,
        Getting_Description,
        Getting_Close
    }   m_stage;

public:
    // Constructors, Destructors

    WT_Alignment()
      : m_description(Align_None)
      , m_stage(Starting)
    { }

    WT_Alignment(WT_Alignment_Description description)
      : m_description(description)
      , m_stage(Starting)
    { }

    virtual ~WT_Alignment()
    { }
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        serialize (WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode,
                                                        WT_File & file);
    WT_Result        process(WT_File & file);

    static WT_Result default_process(WT_Alignment & item,
                                                        WT_File & file);

    // Get/Set methods
    void set_alignment_description(
        WT_Alignment::WT_Alignment_Description description);
    WT_Alignment::WT_Alignment_Description
        get_alignment_description() const;

    WT_Boolean    operator!= (WT_Alignment const
                                            & alignment) const;
    WT_Boolean    operator== (WT_Alignment const
                                            & alignment) const;
};


/// BlockRef attribute.  Intended to be used for password protection of a BlockRef.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Password : public WT_Object
{
private:
        WT_Byte m_string[33];
public:
        WT_Password()
        {
            memset(&m_string[0], '\0', 33);
        }
        WT_Password(char * string);
        WT_Password(WT_Unsigned_Integer16 * string);
        WT_Password(WT_Password const & info);

        virtual ~WT_Password() {}

        WT_Object::WT_ID    object_id() const;
        WT_Object::WT_Type object_type() const;
        WT_Result process(WT_File & file);
        WT_Result skip_operand(WT_Opcode const & opcode, WT_File & file);
        static WT_Result default_process(WT_Password & item, WT_File & file);
        void set(char const * string);
        void set(WT_Unsigned_Integer16 const * string);
        void set(WT_String const & string);
        WT_Byte * const     string(void) const;
        WT_Unsigned_Integer16 const  get_string_length(void);
        WT_Result serialize(WT_File & file) const;
        WT_Result materialize(WT_Opcode const & opcode, WT_File & file);
        WT_Password    operator= (WT_Password const & Password) ;
        WT_Boolean    operator!= (WT_Password const & Password) const;
        WT_Boolean    operator== (WT_Password const & Password) const;
};                                                                                                  //

/// BlockRef attribute.  Used to uniquely identify a BlockRef object.
/** \deprecated This class is only used in the context of a WT_BlockRef object, which has been deprecated.
 *  This remains in the toolkit only for backward compatability with %DWF version 00.55. */
class WHIPTK_API WT_Guid : public WT_Object, public WT_Item
{
private:
    WD_GUID  m_guid;

enum {
    Starting,
    Getting_Data1,
    Getting_Data2,
    Getting_Data3,
    Getting_Data4,
    Getting_Close
}   m_stage;

    void _deleteObject(void *object)
    {
        delete (WT_Guid*)object;
    }

public:
    WT_Guid()
        : m_guid ()
        , m_stage(Starting)
    {
        memset((void *) &m_guid, 0, sizeof(WD_GUID));
    }

    WT_Guid(const WD_GUID guid);
    WT_Guid(WT_Guid const & info);

	virtual ~WT_Guid()
	{}
public:
    WT_Object::WT_ID    object_id() const;
    WT_Object::WT_Type object_type() const;
    WT_Result process(WT_File & file);
    WT_Result skip_operand(WT_Opcode const & opcode, WT_File & file);
    static WT_Result default_process(WT_Guid & item, WT_File & file);
    void set(WD_GUID guid);
    void set(WT_Guid guid);
    WD_GUID get_guid();
    WT_Result serialize(WT_File & file) const;
    WT_Result materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Boolean operator!=(WT_Guid const & in) const  ;
    WT_Boolean operator==(WT_Guid const & in) const  ;
    WT_Guid operator=(WT_Guid const & in) const  ;

};

#endif // BLOCKREF_DEFS_HEADER
