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

#if !defined TEXT_OPTIONS_HEADER
#define TEXT_OPTIONS_HEADER

///
///\file        dwf/whiptk/text_options.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"

/** \addtogroup groupTextOptions Text options
 *  The text option objects assist the reader application in rendering text.
 *  \sa WT_Text
 *
 *  @{
 */

// ====================================================================================
/// Text option base class for underscoring and overscoring.
/** \sa WT_Text
 *  \sa WT_Text_Option_Overscore */
class WHIPTK_API WT_Text_Option_Scoring : public WT_Option
{
public:
    typedef std::vector< WT_Unsigned_Integer16 > tPositionVector;

private:
    WT_Unsigned_Integer16     m_count;
    WT_Unsigned_Integer16 *   m_positions;

    int                       m_materialization_counter;

    enum
    {
        Starting = 1001,
        Getting_Count = 1002,
        Getting_Positions = 1003,
        Eating_Pre_Open_Paren_Whitespace = 1004,
        Getting_Open_Paren = 1005,
        Getting_Close_Parens = 1006
    } ;

protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_Text_Option_Scoring object.
    WT_Text_Option_Scoring()
        : m_count(0)
        , m_positions(WD_Null)
    {
        m_stage = WT_Text_Option_Scoring::Starting;
    }
    /// Constructs a WT_Text_Option_Scoring object with the specified ASCII opcode string (includes start parenthesis.)
    WT_Text_Option_Scoring(WT_Text_Option_Scoring const & scoring) throw(WT_Result);
    /// Destroys a WT_Text_Option_Scoring object.
    virtual ~WT_Text_Option_Scoring();

    //@}
public:
    //@{ \name Data access methods
    /// Returns the size of the array accessed through positions().
    WT_Unsigned_Integer16           count() const                                 {   return m_count;     }
    /// Returns the ASCII opcode string for the scoring subclass (includes start parenthesis.)
    virtual      char const *       opcode() const = 0;
    /// Returns the array of index positions within the string to which the scoring applies.
    WT_Unsigned_Integer16 const *   positions() const                             {   return m_positions; }
    /// Sets the array of index positions within the string to which the scoring applies.
    WT_Result  set(
        WT_Unsigned_Integer16 count, /**< Size of \a pos array. */
        WT_Unsigned_Integer16 const * pos /**< Array of index positions within the string to which the scoring applies. */
        );
    /// Sets the array of index positions within the string to which the scoring applies.
    WT_Result  set(
        const tPositionVector& rVector /**< Vector of index positions within the string to which the scoring applies. */
        );
    /// Assignment operator.  Copies the values from the given WT_Text_Option_Scoring to this one.
    WT_Text_Option_Scoring const &  operator= (WT_Text_Option_Scoring const & scoring) throw(WT_Result);
    //@}

    //@{ \name WT_Optioncode virtual methods
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};


// ====================================================================================
/// Text option class for specifying character overscoring.
/** \sa WT_Text
 *  \sa WT_Text_Option_Scoring */
class WHIPTK_API WT_Text_Option_Overscore : public WT_Text_Option_Scoring
{
public:
    //@{ \name WT_Optioncode virtual methods
    WT_ID   object_id() const;
    //@}
    virtual char const * opcode() const     {   return "(Overscore";    }
};

/// Text option class for specifying character underscoring.
/** \sa WT_Text
 *  \sa WT_Text_Option_Scoring */
class WHIPTK_API WT_Text_Option_Underscore : public WT_Text_Option_Scoring
{
public:
    //@{ \name WT_Optioncode virtual methods
    WT_ID   object_id() const;
    //@}
    virtual char const * opcode() const     {   return "(Underscore";    }
};


// ====================================================================================
/// Text option class for specifying a text bounding box.
/** \note Use of a bounding box may cause the text to be tesselated in the consuming
 *  application which could reduce performance when viewing the WHIP! data.
 *  \sa WT_Text
 *  \image html TextBounding.png "Text baseline and bounding box details"
 */
class WHIPTK_API WT_Text_Option_Bounds : public WT_Option
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Text;
private:
    WT_Logical_Point *       m_bounds;
    int                      m_materialization_counter;

    enum
    {
        Getting_Bounds = 1001
    };

protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_Text_Option_Bounds object.
    WT_Text_Option_Bounds()
        : m_bounds (WD_Null)
    { }
    /// Constructs a WT_Text_Option_Bounds object with the given bounding rectangle.
    WT_Text_Option_Bounds(
        WT_Logical_Point const * pos  /**< Bounding rectangle (left-bottom, right-bottom, right-top, left-top.) */
        ) throw(WT_Result)
	{ 
		WT_Result res = set(pos);
		if(res != WT_Result::Success)
			throw res;
	}
    /// Constructs a WT_Text_Option_Bounds object. Copy constructor
    WT_Text_Option_Bounds(WT_Text_Option_Bounds const & bounds) throw(WT_Result);
    /// Destroys a WT_Text_Option_Bounds object.
    virtual ~WT_Text_Option_Bounds();

    //@}
public:
    //@{ \name Data access methods
    /// Returns the bounding rectangle (left-bottom, right-bottom, right-top, left-top).
    WT_Logical_Point const *    bounds() const              {   return m_bounds;   }
    /// Returns the bounding box points to absolute values based on their relative position to last materialized point.
    virtual void                        de_relativize(WT_File & file);
    /// Relativizes the bounding box points to the last serialized point.
    virtual void                        relativize(WT_File & file);
    /// Applies the given transform to the position and bounding box points.
    virtual void                        transform(WT_Transform const & transform);
    /// Sets the bounding rectangle (left-bottom, right-bottom, right-top, left-top).
    WT_Result                   set(WT_Logical_Point const * bounds);
    /// Assignment operator.  Copies the values from the given WT_Text_Option_Bounds to this one.
    WT_Text_Option_Bounds const &   operator=(WT_Text_Option_Bounds const & bounds) throw(WT_Result);
    //@}

    //@{ \name WT_Optioncode virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};


// ====================================================================================

/// A reserved text option.
/** \note This class is reserved for future use (reserves a specified count of integer values.)
 * \sa WT_Text */
class WHIPTK_API WT_Text_Option_Reserved : public WT_Option
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Text;

private:
    WT_Unsigned_Integer16     m_count;
    WT_Unsigned_Integer16 *   m_values;
    int                       m_materialization_counter;

    enum
    {
        Starting = 1001,
        Getting_Count = 1002,
        Getting_Values = 1003,
        Eating_Pre_Open_Paren_Whitespace = 1004,
        Getting_Open_Paren = 1005,
        Getting_Close_Parens = 1006
    } ;

protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_Text_Option_Reserved object.
    WT_Text_Option_Reserved()
        : m_count(0)
        , m_values(WD_Null)
    {
        m_stage = WT_Text_Option_Reserved::Starting;
    }
    /// Constructs a WT_Text_Option_Reserved object. Copy constructor.
    WT_Text_Option_Reserved(WT_Text_Option_Reserved const & reserved) throw(WT_Result);
    /// Destroys a WT_Text_Option_Reserved object.
    virtual ~WT_Text_Option_Reserved();

    //@}
public:
    //@{ \name Data access methods
    WT_Unsigned_Integer16           count() const                                 {   return m_count;     }
    WT_Unsigned_Integer16 const *   values() const                                {   return m_values; }
    WT_Result                       set(WT_Unsigned_Integer16 count, WT_Unsigned_Integer16 const * values);
    WT_Text_Option_Reserved const &  operator= (WT_Text_Option_Reserved const & reserved) throw(WT_Result);
    //@}

    //@{ \name WT_Optioncode virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};

// ====================================================================================

/// Provides an ID for text option objects.
class WHIPTK_API WT_Text_Optioncode : public WT_Optioncode
{
public:
    /// Text option identifiers.
    enum
    {
        Unknown_Option  = 0,
        Overscore_Option,
        Underscore_Option,
        Bounds_Option,
        Reserved_Option
    };

    /// Returns the option ID of the text option.
    virtual int option_id_from_optioncode();
};


//@}

#endif // TEXT_OPTIONS_HEADER
