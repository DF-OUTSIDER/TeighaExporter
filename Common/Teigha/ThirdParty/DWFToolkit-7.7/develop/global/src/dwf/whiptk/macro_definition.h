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
///\file        dwf/whiptk/macro_definition.h
///

#if !defined MACRO_DEFINITION_HEADER
#define MACRO_DEFINITION_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/object_stream.h"
#include "whiptk/fill.h"


class WHIPTK_API WT_Macro_Definition : public WT_Object_Stream
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    /// Constructors and destructor
    // Default constructor
    WT_Macro_Definition()
        : m_index (0)
        , m_scale_units (1)
        , m_bFill(WD_False)
        , m_cached_fill (WD_False)
    {}

    WT_Macro_Definition( WT_Unsigned_Integer16 index,
                         WT_Integer32 scale_units = 1 );
 
    virtual ~WT_Macro_Definition();

public:   
    /// Operations
    /// The index under which this macro is listed
    const WT_Unsigned_Integer16& index() const  { return m_index; }
          WT_Unsigned_Integer16& index()        { return m_index; }

    /// the scale units for this macro
    const WT_Integer32& scale_units() const     { return m_scale_units; }
          WT_Integer32& scale_units()           { return m_scale_units; }
    
    /// the extents of the drawables held by this macro_definition
    virtual WT_Logical_Box  bounds();
    
    /// Add a drawable to this macro.
    /// Note: it must be created on the heap as this method will take 
    /// ownership 
    WT_Result       add(const WT_Drawable&);
    WT_Result       add(const WT_Attribute&);
   
    /// Implementation of the WT_Object interface
    WT_Type         object_type() const ;
    WT_ID           object_id() const ;
    WT_Result       materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result       process(WT_File & file);
    WT_Result       serialize(WT_File & file) const ;

    /// Provides a default action handler for this attribute 
    /// (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static  WT_Result   default_process( WT_Macro_Definition& ,WT_File&);

protected:
    // overriden from class WT_Object_Stream
    WT_Result       on_materialize(WT_Object&, WT_File&);

private:
    // Hidden Copy constructor and Assignment operator
    WT_Macro_Definition(const WT_Macro_Definition&);
    void operator= (const WT_Macro_Definition&);
    
    // Data Members
    WT_Unsigned_Integer16   m_index;
    WT_Integer32            m_scale_units;
    
    // Non-persisted state variables
    WT_Boolean              m_bFill;
    WT_Fill                 m_cached_fill;
};

#endif // MACRO_DEFINITION_HEADER
// End of file.
