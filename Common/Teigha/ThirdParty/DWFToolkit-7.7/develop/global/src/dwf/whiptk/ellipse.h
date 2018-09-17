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

#if !defined ELLIPSE_HEADER
#define ELLIPSE_HEADER

///
///\file        dwf/whiptk/ellipse.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"

/// A base class for filled and outline ellipse / elliptical arc / elliptical wedge objects.
class WHIPTK_API WT_Ellipse : public WT_Drawable
{
protected:
    WT_Logical_Point        m_position; /**< \brief Center point of ellipse. */
    WT_Integer32            m_major; /**< \brief Major axis radius. */
    WT_Integer32            m_minor; /**< \brief Minor axis radius. */
    WT_Unsigned_Integer16   m_start; /**< \brief Start angle (in 360/65,536ths of a degree.) */
    WT_Unsigned_Integer32   m_end; /**< \brief End angle (in 360/65,536ths of a degree.) */
    WT_Unsigned_Integer16   m_tilt; /**< \brief Rotation angle (in 360/65,536ths of a degree.) */

    /// Materialization stage.
    enum WT_Materialize_Stage
    {
        Get_Position,
        Get_Major,
        Get_Minor,
        Get_Start,
        Get_End,
        Get_Tilt,
        Get_Close_Paren,
        Completed
    }                       m_stage;

    WT_Boolean              m_transformed; /**< \brief WD_True if the ellipse has been transformed. */

protected:

    /// Constructs a WT_Ellipse object.
    WT_Ellipse ()
        : m_major(0)
        , m_minor(0)
        , m_start(0)
        , m_end(0x00010000)
        , m_tilt(0)
        , m_stage (Get_Position)
        , m_transformed (WD_False)
    {}

    /// Constructs a WT_Ellipse object with the given data.
    WT_Ellipse(
        WT_Integer32            x,  /**< Horizontal component of the center point of ellipse. */
        WT_Integer32            y,  /**< Vertical component of the center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : m_position (WT_Logical_Point (x, y))
        , m_major (major)
        , m_minor (minor)
        , m_start (start)
        , m_end (end)
        , m_tilt (tilt)
        , m_stage (Completed)
        , m_transformed (WD_False)
    {
        if (m_end <= m_start)
            m_end += 0x00010000;
    }

    /// Constructs a WT_Ellipse object with the given data.
    WT_Ellipse(
        WT_Logical_Point const &  pos,  /**< Center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : m_position (pos)
        , m_major (major)
        , m_minor (minor)
        , m_start (start)
        , m_end (end)
        , m_tilt (tilt)
        , m_stage (Completed)
        , m_transformed (WD_False)
    {
        if (m_end <= m_start)
            m_end += 0x00010000;
    }

	WT_Ellipse(const WT_Ellipse& e)
	{
		*this = e;
	}
		

    /// Destroys a WT_Ellipse object.
    virtual ~WT_Ellipse()
    { }

public:
    //@{ \name Data access methods
    /// Returns the major axis radius.
#ifdef EMCC
    const WT_Integer32& major_version() const   { return m_major; }
          WT_Integer32& major_version()         { return m_major; }

    /// Returns the minor axis radius.
    const WT_Integer32& minor_version() const   { return m_minor; }
          WT_Integer32& minor_version()         { return m_minor; }
#else
    const WT_Integer32& major() const   { return m_major; }
          WT_Integer32& major()         { return m_major; }

    /// Returns the minor axis radius.
    const WT_Integer32& minor() const   { return m_minor; }
          WT_Integer32& minor()         { return m_minor; }
#endif
    /// Returns the center point of the ellipse.
    const WT_Logical_Point& position() const { return m_position; }
          WT_Logical_Point& position()       { return m_position; }

    /// Returns the start angle (in 360/65,536ths of a degree.)
    const WT_Unsigned_Integer16& start() const { return m_start; }
          WT_Unsigned_Integer16& start()       { return m_start; }

    /// Returns the start angle (in degrees.)
    float start_degree() const
    {
        return ((float)(m_start * 360.0) / (float)0x00010000);
    }

    /// Returns the start angle (in radians.)
    float start_radian() const
    {
        return ((float)(m_start * TWO_PI) / (float)0x00010000);
    }

    /// Returns the end angle (in 360/65,536ths of a degree.)
    const WT_Unsigned_Integer32& end() const { return m_end; }
          WT_Unsigned_Integer32& end()       { return m_end; }

    /// Returns the end angle (in degrees.)
    float end_degree() const
    {
        return ((float)(m_end * 360.0) / (float)0x00010000);
    }

    /// Returns the end angle (in radians.)
    float end_radian() const
    {
        return ((float)(m_end * TWO_PI) / (float)0x00010000);
    }

/// Returns the rotation angle (in 360/65,536ths of a degree.)
    const WT_Unsigned_Integer16& tilt() const { return m_tilt; }
          WT_Unsigned_Integer16& tilt()       { return m_tilt; }

    /// Returns the rotation angle (in degrees.)
    float tilt_degree() const
    {
        return ((float)(m_tilt * 360.0) / (float)0x00010000);
    }

    /// Returns the rotation angle (in radians.)
    float tilt_radian() const
    {
        return ((float)(m_tilt * TWO_PI) / (float)0x00010000);
    }

	WT_Boolean transformed() const
	{
		return m_transformed;
	}
    //@}

    //@{ \name WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);
    virtual void update_bounds(WT_File * file);
    //@}

    /// Applies the given transform to the point set.
    void         transform(WT_Transform const & transform);

protected:
    /// Used by child classes to serialize the ellipse.
    WT_Result    serialize(WT_File & file, WT_Boolean filled) const;

};


/** \addtogroup groupDrawable Drawable objects
 *  @{
 */

/// A drawable describing a filled ellipse, or elliptical wedge.
/** \image html CircleStartEnd.png "Circular arc (major==minor) start/end angle details"
 *  \image html EllipseStartEnd.png "Elliptical arc (major!=minor) start/end angle details"
 */
class WHIPTK_API WT_Filled_Ellipse : public WT_Ellipse
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	
public:

    /// Constructs a WT_Filled_Ellipse object.
    WT_Filled_Ellipse()
    { }

    /// Constructs a WT_Filled_Ellipse object with the given data.
    WT_Filled_Ellipse (
        WT_Integer32            x,  /**< Horizontal component of the center point of ellipse. */
        WT_Integer32            y,  /**< Vertical component of the center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Ellipse (x, y, major, minor, start, end, tilt)
    { }

    /// Constructs a WT_Filled_Ellipse object with the given data.
    WT_Filled_Ellipse(
        WT_Logical_Point const &  pos,  /**< Center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Ellipse (pos, major, minor, start, end, tilt)
    { }

	virtual ~WT_Filled_Ellipse()
	{}
public:
    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize(WT_File & file) const;
    WT_Result        process(WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Filled_Ellipse & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

/// A drawable describing an unfilled ellipse, or elliptal arc.
/** \image html CircleStartEnd.png "Circular arc (major==minor) start/end angle details"
 *  \image html EllipseStartEnd.png "Elliptical arc (major!=minor) start/end angle details"
 */
class WHIPTK_API WT_Outline_Ellipse : public WT_Ellipse
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    /// Constructs a WT_Outline_Ellipse object.
    WT_Outline_Ellipse ()
    { }

    /// Constructs a WT_Outline_Ellipse object with the given data.
    WT_Outline_Ellipse (
        WT_Integer32            x,  /**< Horizontal component of the center point of ellipse. */
        WT_Integer32            y,  /**< Vertical component of the center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Ellipse (x, y, major, minor, start, end, tilt)
    { }

    /// Constructs a WT_Outline_Ellipse object with the given data.
    WT_Outline_Ellipse(
        WT_Logical_Point const &  pos,  /**< Center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Ellipse (pos, major, minor, start, end, tilt)
    { }
	virtual ~WT_Outline_Ellipse()
	{}
public:
    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize(WT_File & file) const;
    WT_Result        process(WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Outline_Ellipse & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // ELLIPSE_HEADER
