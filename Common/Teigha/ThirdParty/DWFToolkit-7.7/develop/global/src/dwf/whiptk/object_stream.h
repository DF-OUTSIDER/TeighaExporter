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

///
///\file        dwf/whiptk/object_stream.h
///



#if !defined OBJECT_STREAM_HEADER
#define OBJECT_STREAM_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/object.h"


class WHIPTK_API WT_Object_Stream : public WT_Object 
{
	friend class WT_W2D_Class_Factory;
public:
    /// Constructs a WT_Object_Stream object.
    WT_Object_Stream();
    
	///Copy Constructor
	WT_Object_Stream(WT_Object_Stream const& object_stream)
	{
		*this = object_stream;
	}

    /// Destroys a WT_Object_Stream object.
    virtual ~WT_Object_Stream();

public: 
    // operations
    // Enumeration capabilities
    void                reset() const;
    WT_Boolean          get_next() const;
    const WT_Object*    object() const;

    void                set_owner( WT_Boolean bOwn ) { m_streamOwner = bOwn; }

protected:
    
	/// Returns the WT_ID for this object.
    virtual WT_ID      object_id() const { return Unknown_ID; }

    /// Returns the object type WT_Type::Option.
    WT_Type    object_type() const { return Unknown; }

	virtual WT_Result        serialize (
        WT_File & /*file*/ /**< The file being written. */
        ) const { return WT_Result::Toolkit_Usage_Error; }

	virtual WT_Result        process(WT_File & /*file*/) { return WT_Result::Toolkit_Usage_Error; }
    /// Provided to implement WT_Object, but <b>not used</b> (returns WT_Result::Toolkit_Usage_Error).
    virtual WT_Result    materialize(WT_Opcode const & /* opcode */, WT_File & /* file */) {
        return WT_Result::Toolkit_Usage_Error;
    }
    // Adds another item to the end of the definition stream
    // Note this method takes ownership of the object
    virtual WT_Result           addObject(const WT_Object* object);
    // Read/Write from file.
    virtual WT_Result           serialize_stream(WT_File&) const;
    virtual WT_Result           materialize_stream(WT_Opcode const &, WT_File&);

    // Derivations can override to keep track of opcodes as they are added
    virtual WT_Result   on_materialize(WT_Object& , WT_File&); 
    
private:
    // private methods
    void                add_to_array( const WT_Object* object);

    // private data members
    const WT_Object**   m_ppArray;
    unsigned int        m_allocated;
    unsigned int        m_elements;
    mutable int         m_current;
    WT_Boolean          m_streamOwner;

};

#endif // OBJECT_STREAM_HEADER
// End of file.
