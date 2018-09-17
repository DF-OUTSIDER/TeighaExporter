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

#if !defined USER_FILL_PATTERN_HEADER
#define USER_FILL_PATTERN_HEADER

///
///\file        dwf/whiptk/usrfillpat.h
///

#include "whiptk/whipcore.h"
#include "whiptk/pattern_scale.h"
#include "dwfcore/CountedObject.h"
/** \addtogroup groupRenditionAttributes
 *  @{
 *  Describes the Set_Fill_Pattern attribute in Rendition to be applied to 
 *  subsequent Polygons and other filled geometry. 
 *  This attribute is  a user defined Fill based on image data. It is used 
 *  by all the drawing opcodes that are affected by the Fill attribute. 
 */

/// Attribute class for specifying a user defined fill pattern 
class WHIPTK_API WT_User_Fill_Pattern : public WT_Attribute
{
    friend class WT_Pattern_Scale;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    //@{ \Inner Class 
    /// Reference counted class Fill_Pattern consists of a user defined Bitonal Bitmap 
    /// This is used to ensure that the bitmap data is not copied around along with the 
    /// parent WT_User_Fill_Pattern class.
    class Fill_Pattern : public DWFCore::DWFCountedObject
    {
    public:
        /// Static construct method that creates this reference counted object on the heap.
        static Fill_Pattern* Construct (
            WT_Unsigned_Integer16    rows, /**< Height of the bitonal bitmap in pixels. */
            WT_Unsigned_Integer16    columns, /**< Width of the bitonal bitmap in pixels. */                    
            WT_Unsigned_Integer32    data_size = 0, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
            WT_Byte const *          data = WD_Null) /**< The raw pixel data.  */  
                      
        { return DWFCORE_ALLOC_OBJECT (Fill_Pattern(rows,columns,data_size,data)); }
        /// Returns the height of the bitonal bitmap used in this fill pattern. 
        WT_Unsigned_Integer16 rows() const
        {   return m_rows;  }
        /// Returns the width of the bitonal bitmap used in this fill pattern.
        WT_Unsigned_Integer16 columns() const
        {   return m_columns;  }
        /// Returns the size in WT_Byte's of the bitmap data 
        WT_Unsigned_Integer32 data_size() const
        {   return m_data_size; }
        /// Returns a const pointer to the data 
        const WT_Byte * data() const
        {   return m_data; }
        /// Comparison operator 
        WHIPTK_API WT_Boolean operator == ( Fill_Pattern const &  ) const;
        /// Not equal to operator
        WT_Boolean operator != ( Fill_Pattern const &  pattern)
        {   return !(operator==(pattern));  }

    protected:
        /// Protected Costructor
        WHIPTK_API Fill_Pattern( WT_Unsigned_Integer16    rows, /**< Height of the bitonal bitmap in pixels. */
                      WT_Unsigned_Integer16    columns, /**< Width of the bitonal bitmap in pixels. */                         
                      WT_Unsigned_Integer32    data_size, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
                      WT_Byte const *          data) throw() ; /**< The raw pixel data.  */  
        /// Protected Destructor is called only from the base class.
        virtual ~Fill_Pattern() throw()
        {
            if (m_data != WD_Null)
                delete[] m_data;
        }
    private:
        /// Hidden copy constructor and assignment operator 
        Fill_Pattern(Fill_Pattern const & );
        Fill_Pattern const & operator=(Fill_Pattern const & );

        /// Private Data members
        WT_Unsigned_Integer16   m_rows;
        WT_Unsigned_Integer16   m_columns;
        WT_Unsigned_Integer32   m_data_size;
        WT_Byte *               m_data;
    };
    //@}

public:
     //@{ \name Construction

    /// Constructs a WT_User_Fill_Pattern object.
    WT_User_Fill_Pattern()
    : m_pattern_num(-1)  // important: -1 means "does not exist", which is the default state
    , m_is_scale_used(WD_False)
    , m_fill_pattern(WD_Null)
    {}
    /// Constructs a WT_User_Fill_Pattern with the given ID.
    WT_User_Fill_Pattern (WT_Integer16 pattern_num)
    : m_pattern_num(pattern_num)
    , m_is_scale_used(WD_False)
    , m_fill_pattern(WD_Null)
    {}
    
    /// Constructs a WT_User_Fill_Pattern with a user defined bitonal bitmap passing in all the parameters 
    WT_User_Fill_Pattern (
            WT_Integer16    pattern_num, /**< The unique numeric identifier of the fill pattern in the graphics collection. */
            WT_Unsigned_Integer16    rows, /**< Height of the bitonal bitmap in pixels. */
            WT_Unsigned_Integer16    columns, /**< Width of the bitonal bitmap in pixels. */ 
			WT_Unsigned_Integer32    data_size, /**< The size (in bytes) of the raw image data contained in the \a data argument. */
            WT_Byte const *          data); /**< The raw pixel data that will be copied.  */  
            

    
    /// Constructs a WT_User_Fill_Pattern with a user defined bitonal bitmap using the reference counted Fill_Pattern class
    WT_User_Fill_Pattern (
            WT_Integer16    pattern_num, /**< The unique numeric identifier of the fill pattern in the graphics collection. */
            Fill_Pattern * fill_pattern
            );
 
    /// Copy Constructor for the WT_User_Fill_Pattern
    WT_User_Fill_Pattern(WT_User_Fill_Pattern const &);
    
    /// Destroys a WT_User_Fill_Pattern object.
    virtual ~WT_User_Fill_Pattern();

    //@}
public:
   
	 /// Assignment operator for the WT_User_Fill_Pattern 
    WT_User_Fill_Pattern const & operator =(WT_User_Fill_Pattern const &); 
    //@{ \name Data access methods
    /// The user defined fill pattern number. 
    WT_Integer16 const& pattern_number() const
    {   return m_pattern_num; }
    WT_Integer16& pattern_number()
    {   return m_pattern_num; }
    WT_Boolean const& is_scale_used() const
    {   return m_is_scale_used; }
    WT_Boolean& is_scale_used() 
    {   return m_is_scale_used; }
    /// Returns the a read-only reference to the pattern scale option.
    WT_Pattern_Scale const & pattern_scale() const 
    {  return m_pattern_scale; }
    /// Returns the a writable reference to the pattern scale option.
    WT_Pattern_Scale& pattern_scale() 
    {  m_is_scale_used = WD_True;  return m_pattern_scale; }
    /// Returns the reference counted Fill_Pattern object that holds onto the data
    const Fill_Pattern* fill_pattern() const
    { return m_fill_pattern; } 
    Fill_Pattern*& fill_pattern()
    {   return m_fill_pattern;  }
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
    
    //@{ \name Inspection methods
    /// Returns WD_True if the given pattern is equal to this one (compares ID and pattern scale.)
    WT_Boolean operator== (WT_User_Fill_Pattern const & pattern) const;
    /// Returns WD_True if the given pattern is NOT equal to this one (compares ID and pattern scale.)
    WT_Boolean operator!= (WT_User_Fill_Pattern const & pattern) const
    {   return !(operator==(pattern));  }
    //@}
    
    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_User_Fill_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

private:
    /// Private Data members
    WT_Integer16            m_pattern_num;
    WT_Pattern_Scale        m_pattern_scale;
    WT_Boolean              m_is_scale_used;
    Fill_Pattern*           m_fill_pattern;
    
    /// Inner struct for handling the materialization of the Pattern_Scale
    struct WHIPTK_API WT_User_Fill_Pattern_Option_Code
    : public WT_Optioncode
    {   enum WT_Fill_Pattern_Option_ID
        {   Unknown_Option, /**< Default, no options set. */
            Pattern_Scale_Option /**< The WT_Pattern_Scale option. */
        };
        /// Returns the bitmask of the active options.
        int option_id_from_optioncode();
    } m_optioncode;
};

//@}

#endif // SET_FILL_PATTERN_HEADER
