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

#if !defined DASHPAT_HEADER
#define DASHPAT_HEADER

///
///\file        dwf/whiptk/dashpat.h
///

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a user definable line pattern.
/** Given a unique ID and a sequence of dash and space lengths, this object defines a custom line pattern.
 *
 *  \note Dash patterns override line patterns.  When finished with the dash pattern, clients
 *  must "turn off" dash patterns by setting the current dash pattern to WT_Dash_Pattern::kNull.
 *
 *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
 *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count.)  To reserve
 *  room for predefined line pattern growth in the future (which will all have positive ID's, it
 *  is suggested that dash pattern ID's begin at WT_Line_Pattern::Count+100 and increase in value
 *  from there.  The toolkit does not prevent conflicts from happening.
 *  \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Dash_Pattern : public WT_Attribute, public WT_Item
{
	friend class WT_Rendition;
	friend class WT_PDFRendition;
	friend class WT_Class_Factory;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	friend class WT_Dash_Pattern_List;

private:
    WT_Integer16* m_data;
    WT_Integer16  m_size;
    WT_Integer16  m_allocated;
    WT_Integer32  m_number;
    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Number,
        Checking_For_Attribute_End,
        Getting_Size,
        Getting_Data,
        Getting_Separator,
        Eating_End_Whitespace
    } m_stage;

    void _deleteObject(void *object)
    {
        delete (WT_Dash_Pattern*)object;
    }

public:

    /// Default null dash pattern.
    /** \note When finished with the dash pattern, clients must "turn off" dash patterns by
     *  setting the current dash pattern to WT_Dash_Pattern::kNull.
     */
    static const WT_Dash_Pattern kNull;

public:
    //@{ \name Construction / destruction
    /// Construct a WT_Dash_Pattern object.
    WT_Dash_Pattern()
    : m_data(0)
    , m_size(0)
    , m_allocated(0)
    , m_number(-1)
    , m_stage(Eating_Initial_Whitespace)
    {}

    /// Construct a WT_Dash_Pattern object with the given data.
    /** \exception WT_Result::Toolkit_Usage_Error The \a length parameter was not an even number.
     *  \exception WT_Result::Out_Of_Memory_Error Internal data could not be allocated.
     */
    WT_Dash_Pattern(
        /** Unique identifier for the dash pattern.
         *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
         *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count). */
        WT_Integer32 id_number,
        WT_Integer16 length, /**< Length of the \a pArray argument.  Must be even. */
        WT_Integer16 const * pArray /**< Array of values (contiguous pairs) representing pixels-on pixels-off defining the dash pattern. */
        ) throw(WT_Result)
    : m_data(0)
    , m_size(0)
    , m_allocated(0)
    , m_number(-1)
    , m_stage(Eating_Initial_Whitespace)
    { 
		WT_Result res = set(id_number,length,pArray);	
		if(res != WT_Result::Success)
			throw res;
	}

    /// Construct a WT_Dash_Pattern object.  Copy constructor.
    WT_Dash_Pattern(
        WT_Dash_Pattern const & r ) throw(WT_Result)
    : WT_Attribute()
    , WT_Item()
    , m_data(0)
    , m_size(0)
    , m_allocated(0)
    , m_number(-1)
    , m_stage(Eating_Initial_Whitespace)
    { 		
		WT_Result res = set(r);	
		if(res != WT_Result::Success)
			throw res;
	}

    /// Destroys a WT_Dash_Pattern object.
    virtual ~WT_Dash_Pattern()
    { set(0,0,0); }

    //@}
public:
    //@{ \name Data access methods
    /// Returns the length of the dash pattern definition value array.
    inline WT_Integer16 length()  const { return m_size; }
    /// Returns the read-only id of the dash pattern.
    inline WT_Integer32 number()  const { return m_number; }
    /// Returns a writable reference to the id of the dash pattern.
    inline WT_Integer32& number() { return m_number; }
    /// Returns a read-only pointer to the array of values (contiguous pairs) representing pixels-on pixels-off defining the dash pattern.
    inline WT_Integer16 const * pattern() const { return m_data; }
    /// Copies the given dash pattern to this one.
    WT_Result set( WT_Dash_Pattern const & pattern);
    /// Sets the dash pattern with the given data.
    WT_Result set(
        /** Unique identifier for the dash pattern.
         *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
         *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count.) */
        WT_Integer32 id_number,
        WT_Integer16 length=0, /**< Length of the \a pArray argument.  Must be even. */
        WT_Integer16 const * pArray=0 /**< Array of values (contiguous pairs) representing pixels-on pixels-off defining the dash pattern. */
        );
    /// Sets the pattern data with the given data (retains the existing index number.)
    inline void set_pattern(
        WT_Integer16 length, /**< Length of the \a pArray argument.  Must be even. */
        WT_Integer16 const * pArray /**< Array of values (contiguous pairs) representing pixels-on pixels-off defining the dash pattern. */
        ) { set(m_number, length, pArray); }
    /// Assignment operator.  Copies the given dash pattern to this one.
    virtual inline WT_Dash_Pattern& operator= ( WT_Dash_Pattern const & r ) throw(WT_Result)
	{
		WT_Result res = set(r);		
		if(res == WT_Result::Success)
			return *this;
		else
			throw res;		
	}
    /// Assignment operator.  Sets the dash pattern id.
    virtual inline WT_Dash_Pattern& operator=(
        /** Unique identifier for the dash pattern.
         *  \warning The unique ID must not conflict with the predefined line pattern definitions or the
         *  static dash pattern kNull (therefore should be >= WT_Line_Pattern::Count.) */
        WT_Integer32 id_number
        ) throw(WT_Result)
	{ 
		WT_Result res = set(id_number);
		if(res == WT_Result::Success)
			return *this;
		else
			throw res;	 
		
				
	}

    /// Index operator. Returns a writable reference to a dash pattern definition value array entry.
    WT_Integer16& operator[](
        WT_Integer16 index /**< The index into the dash pattern definition array. */
        );
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Dash_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_Result serialize_pattern_definition( WT_File& ) const;

};

//@}

#endif // DASHPAT_HEADER
