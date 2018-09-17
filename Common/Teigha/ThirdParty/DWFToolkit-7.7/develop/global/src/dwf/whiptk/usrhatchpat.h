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


#if !defined USER_HATCH_PATTERN_HEADER
#define USER_HATCH_PATTERN_HEADER

///
///\file        dwf/whiptk/usrhatchpat.h
///

#include "whiptk/whipcore.h"
#include "dwfcore/CountedObject.h"
#include "dwfcore/STL.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 *  Describes the User_Hatch_Pattern attribute in the file Rendition to be applied to 
 *  subsequent Polygons and other filled geometry. 
 *  This attribute is a user defined collection of Parrallel Line sets that is used 
 *  by all the drawing opcodes that are affected by the Fill attribute. 
 *  The attribute itself can be used in various ways. 
 *  a) Only by defining a previously defined Hash pattern num - 
 *     This allows the reuse of a Hatch Pattern attribute that was defined earlier in the file 
 *     by specifing the same Hash pattern number it used.
 *  b) By Defining a unique Hash pattern number along with the X and Y size in points
 *     (i.e. pixel size on the screen) of how often the defined pattern repeats.
 *     Note: The Line set simply consists of a defualt straight line pattern.
 *  c) By Defining a unique Hash pattern number, X, Y values and any given number of 
 *     reference counted Hatch_Pattern objects. 
 *  Its important to note that the usage of this class differs from other "heavy"
 *  attributes such as WT_Line_Pattern. Each custom parralel line specification is constructed
 *  by using the reference counted inner class, Hatch_Pattern. The use of reference counting
 *  makes the copying of the attribute a relatively cheap affair. 
 *  However, it is important that access to the reference counted object be properly controlled 
 *  by setting the reference count correctly or using a suitable smart pointer. 
 *  DWFCore provides one such "smart pointer" in the guise of the 
 *  DWFCore::DWFCountedObjectPointer.  
 */


/// Attribute class for specifying a user defined hatch pattern 
class WHIPTK_API WT_User_Hatch_Pattern : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    //@{ \Inner Class 
    /// Reference counted class Hatch_Pattern defines a parrallel line set along with a optional
    /// dashing pattern that is used by the WT_User_Hatch_Pattern class to define a custom
    /// Hatch pattern.
    /// Construction of this class is carried out by using the static Construct method. This
    /// method returns a pointer to the new Hatch_Pattern class created on the heap. 
    /// Note : Since this class is reference counted it is important to increment the reference 
    /// count on aquiring the pointer and decrementing it before disposing of it.
    /// The Constructor and destructor for this class is protected and can only be called when 
    /// the reference count is zero.
    class Hatch_Pattern : public DWFCore::DWFCountedObject
    {
    public:
        /// Static construct method that creates this reference counted object on the heap.
        static Hatch_Pattern* Construct(const double & x,  
                                        const double & y,  
                                        const double & angle, 
                                        const double & spacing,
                                        const double & skew = 0, /**< optional skew value  */
                                        WT_Unsigned_Integer32 data_size = 0, /**< optional  */
                                        const double * data = NULL /**< optional  */
                                        )
        {
            return DWFCORE_ALLOC_OBJECT (Hatch_Pattern(x,y,angle,spacing,skew,data_size,data));
        }
        
    public:
        
        double const & x() const { return m_x; }

        double const & y() const { return m_y; }

        double const & angle() const { return m_angle; }

        double const & spacing() const { return m_spacing; }

        double const & skew() const { return m_skew; }
        
        WT_Unsigned_Integer32 data_size() const { return m_data_size; }

        double const * data() const { return m_data; } 

        WHIPTK_API WT_Boolean operator == ( Hatch_Pattern const & pattern ) const;

        WT_Boolean operator != ( Hatch_Pattern const & pattern )
        {
            return ! (operator==(pattern));
        }
    protected:
        WHIPTK_API Hatch_Pattern(const double & x, 
                      const double & y,
                      const double & angle,
                      const double & spacing,
                      const double & skew,
                      WT_Unsigned_Integer32 patsize,
                      const double * patdata
                      )throw();

        virtual ~Hatch_Pattern() throw()
        { 
            if (m_data != WD_Null)
                delete [] m_data;
        }

    private:
        /// Hidden copy constructor and assignment operator 
        Hatch_Pattern( Hatch_Pattern const &);
        Hatch_Pattern const & operator=(Hatch_Pattern const &);
        double m_x;
        double m_y;
        double m_angle;
        double m_spacing;
        double m_skew;
        WT_Unsigned_Integer32 m_data_size;
        double * m_data;
    };
     //@}
// data Members for WT_User_Hatch_Pattern()
private:
    WT_Integer16 m_hashpatnum;
    WT_Unsigned_Integer16 m_xsize;
    WT_Unsigned_Integer16 m_ysize;
    std::vector<Hatch_Pattern*> m_patterns;
public:
    //@{ \name Construction
    /// Default Constructor  
    WT_User_Hatch_Pattern()
        : m_hashpatnum(-1)  // important: -1 means "does not exist", which is the default state
        , m_xsize(0)
        , m_ysize(0)
    {;}
    /// Constructor using a previously defined hashpatnum. 
    /// Note: if the haspatnum is unique the DWF Viewer will
    ///       simply ignore this attribute.
    WT_User_Hatch_Pattern(WT_Integer16 hashpatnum)
        : m_hashpatnum(hashpatnum)
        , m_xsize(0)
        , m_ysize(0)
    {;}  
    /// Constructor using a unique hashpatnum along with the X and Y size
    /// of the line patterns in points. 
    /// Note: If the haspatnum is not unique it will replace the previously
    ///       occuring WT_User_Hatch_Pattern atrribute for any subsequent
    ///       usage of this attribute with the hashpatnum.
    WT_User_Hatch_Pattern(WT_Integer16 hashpatnum,
                         WT_Unsigned_Integer16 xsize,
                         WT_Unsigned_Integer16 ysize)
        : m_hashpatnum(hashpatnum)
        , m_xsize(xsize)
        , m_ysize(ysize)
    {;}  

    /// Copy constructor
    WT_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern);
       
   
    virtual ~WT_User_Hatch_Pattern();

    
    //@}
public:
    /// Accessor method for the hash pattern number.
    WT_Integer16 const& pattern_number() const {return m_hashpatnum;}
    WT_Integer16& pattern_number() {return m_hashpatnum;}


    /// Width of the pattern in points  
    WT_Unsigned_Integer16 const& xsize() const {return m_xsize;}
    WT_Unsigned_Integer16& xsize() {return m_xsize;}
    
    /// Height of the pattern in points
    WT_Unsigned_Integer16 const& ysize() const {return m_ysize;}
    WT_Unsigned_Integer16& ysize() {return m_ysize;}

    /// Adds the reference counted inner class Hatch_Pattern to this 
    /// attribute. 
    WT_Integer16 add_pattern(Hatch_Pattern & pattern);
    
    /// returns the number of reference counted Hatch_Pattern objects held
    /// by this attribute 
    WT_Unsigned_Integer32 pattern_count() const { 
            return (WT_Unsigned_Integer32)m_patterns.size();
    }
    
    /// returns a reference counted Hatch_Pattern pointer based on the index argument.
    /// Will return Null if the index is not valid.
    const Hatch_Pattern * pattern(WT_Unsigned_Integer32 index) const;

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}
    
	WT_User_Hatch_Pattern const & operator= (WT_User_Hatch_Pattern const & pattern);

    //@{ \name Inspection methods
    /// Returns WD_True if the given pattern is equal to this one (compares ID and pattern scale.)
    WT_Boolean operator== (WT_User_Hatch_Pattern const & pattern) const;
    /// Returns WD_True if the given pattern is NOT equal to this one (compares ID and pattern scale.)
    WT_Boolean operator!= (WT_User_Hatch_Pattern const & pattern) const;
    //@}
    
    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_User_Hatch_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_Result serialize_ascii     (WT_File & file) const;
    WT_Result serialize_binary    (WT_File & file) const; 
    WT_Result materialize_ascii   (WT_Opcode const & opcode, WT_File & file);
    WT_Result materialize_binary  (WT_Opcode const & opcode, WT_File & file);

};

//@}

#endif // USER_HATCH_PATTERN_HEADER
