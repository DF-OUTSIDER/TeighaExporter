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
///\file        dwf/whiptk/overpost.h
///



#if !defined OVERPOST_HEADER
#define OVERPOST_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/object_stream.h"



class WHIPTK_API WT_Overpost : public WT_Object_Stream
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    enum WT_AcceptMode
    {
        AcceptAll,      // process all entities in the overpost group
        AcceptAllFit,   // process entities in the overpost group which fit
        AcceptFirstFit  // process first entity in the overpost group which fits
    };
public:
    // Constructors and destructor
    WT_Overpost();
    WT_Overpost( WT_AcceptMode  acceptMode,
                 WT_Boolean     renderEntities,
                 WT_Boolean     addExtents);
	
    virtual ~WT_Overpost();
public:
    // return the overpost options
    WT_AcceptMode       acceptMode() const;
    WT_AcceptMode&      acceptMode() { return m_eAcceptMode; }
    WT_Boolean          renderEntities() const;
    WT_Boolean&         renderEntities() { return m_renderEntities; }
    WT_Boolean          addExtents() const;
    WT_Boolean&         addExtents() { return m_addExtents; }

    // add drawables to this overpost
    // Note: This class takes ownership of the WT_Drawable. 
    //       The WT_Drawable should be created on the heap.
    WT_Result           add(const WT_Object&);

    /// Implementation of the WT_Object interface
    WT_Type             object_type() const ;
    WT_ID               object_id() const ;
    WT_Result           materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result           process(WT_File & file);
    WT_Result           serialize(WT_File & file) const ;

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result    default_process(WT_Overpost&, WT_File&);

protected:
    // overriden from class WT_Object_Stream
    WT_Result           on_materialize(WT_Object&, WT_File&);    
    
protected:
    // Hidden copy constructor and Assignment operator
    WT_Overpost(const WT_Overpost& );
    void operator=(const WT_Overpost&);

    static const char*  enum_to_string(WT_AcceptMode);
    static WT_Boolean   string_to_enum(const char* pStr, WT_AcceptMode&);
    static WT_Boolean   string_to_boolean(const char* pStr, WT_Boolean&);

    WT_Byte             binary_serialize_var() const ;
    WT_Result           binary_materialize_var(WT_Byte byte);

protected:
    static const char* str_AcceptMode_All;
    static const char* str_AcceptMode_AllFit;
    static const char* str_AcceptMode_FirstFit;
    static const char* str_True;
    static const char* str_False;

private:
    // Data members
    WT_AcceptMode       m_eAcceptMode;
    WT_Boolean          m_renderEntities;
    WT_Boolean          m_addExtents;
};

#endif // OVERPOST_HEADER
// End of file.
