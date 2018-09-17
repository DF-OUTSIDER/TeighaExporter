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

///
///\file        dwf/whiptk/matrix.h
///

#if !defined MATRIX_HEADER
#define MATRIX_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/opcode.h"

/// Simple 4x4 matrix.
class WHIPTK_API WT_Matrix
{
protected:
    double      m_elements[4][4];

public:

    //@{ \name Construction
    /// Constructs a WT_Matrix object (sets it to an identity matrix.)
    WT_Matrix()
    {   set_identity(); }

    /// Constructs a WT_Matrix object. Copy constructor.
    WT_Matrix(WT_Matrix const & xform)
    {   set(xform);     }

    /// Constructs a WT_Matrix object from an array of doubles (must be 16 doubles in the array.)
    WT_Matrix(double const * xform)
    {   set(xform);     }

	virtual ~WT_Matrix()
	{}
    //@}
public:

    //@{ \name Data access methods
    /// Returns a 16 element double array containing the matrix values.
    double const *        elements() const                          {   return &m_elements[0][0];  }
    /// Returns the value at the given row and column
    /** \exception WT_Result::Toolkit_Usage_Error One of the arguments was out of range (0 <= x <= 3).
     */
    inline double  operator() ( unsigned int row, unsigned int col ) const
    {   if( row>3 || col>3 )
            throw WT_Result::Toolkit_Usage_Error;
        return m_elements[row][col];
    }
    /// Returns a reference to the value at the given row and column (can be used to set the value).
    /** \exception WT_Result::Toolkit_Usage_Error One of the arguments was out of range (0 <= x <= 3).
     */
    inline double& operator() ( unsigned int row, unsigned int col )
    {   if( row>3 || col>3 )
            throw WT_Result::Toolkit_Usage_Error;
        return m_elements[row][col];
    }
    //@}

    //@{ \name Manipulation methods
    /// Adjoins the given matrix to this matrix and returns the result.
    /** Returns the matrix obtained by joining each row of \a result to the
     *  right of the corresponding row of this matrix.
     */
    void                  adjoin (WT_Matrix & result) const;
    /// Rotates the matrix.
    /** This method takes the matrix and applies a rotation of the first
     * quadrant of WHIP! coordinate space about that quadrant's center
     * (in other words it rotates the first quadrant around (MAX_Int/2, Max_int/2)
     */
    void                  rotate (WT_Matrix & result, WT_Integer32 rotation) const;
    /// Sets the matrix to the identity matrix (1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1).
    void                  set_identity();
    /// Sets the matrix to the array of doubles (must be 16 doubles in the array.)
    void                  set(double const * xform);
    /// Sets the matrix from the given one (copies its values.)
    void                  set(WT_Matrix const & xform);
    /// Uses this matrix to transform the given point.
    /** Transforms \a pt into \a result.
     */
    void                  transform(
        WT_Point3D const & pt, /**< The source point.*/
        WT_Point3D & result, /**< Output parameter.  The resultant transformed point. */
        double * out_w = WD_Null, /**< Output parameter.  If non-null, receives the homogeneous adjustment (projection constant) for the point. */
        double cutoff = 0.0 /**< Maximum homogeneous adjustment (projection constant) allowed for the point. */
        ) const;
    ///Adds the scaling and translation values from the given transform to the corresponding entries in the matrix.
    WT_Matrix &           operator *= (WT_Transform const & xform);
    //@}

    //@{ \name Inspection methods
    /// Returns WD_True if the matrix is an identity matrix.
    WT_Boolean            is_identity() const;
    /// Returns WD_True if the matrix is equivalent to this one.
    WT_Boolean            operator== (WT_Matrix const & matrix) const;
    /// Returns WD_True if the matrix is NOT equivalent to this one.
    WT_Boolean            operator!= (WT_Matrix const & matrix) const    {   return !(*this==matrix); }
    //@}
};

/// Simple 4x4 matrix.
class WHIPTK_API WT_Matrix_IO : public WT_Matrix
{
	friend class WT_Units;
	friend class WT_BlockRef;
	friend class WT_W2D_Class_Factory;
	friend class WT_Class_Factory;


private:

    enum
    {
        Eating_Initial_Whitespace,
        Eating_Outermost_Open_Paren,
        Getting_First_Row,
        Getting_Second_Row,
        Getting_Third_Row,
        Getting_Fourth_Row,
        Skipping_Past_Close_Paren
    }           m_stage;

    enum
    {
        Eating_Initial_Row_Whitespace,
        Eating_Open_Paren,
        Getting_First_Column,
        Getting_Second_Column,
        Getting_Third_Column,
        Getting_Fourth_Column,
        Skipping_Past_Row_Close_Paren
    }           m_row_stage;

    int         m_paren_count;
    int         m_row_paren_count;

protected:

    //@{ \name Construction
    /// Constructs a WT_Matrix_IO object (sets it to an identity matrix.)
    WT_Matrix_IO()
        : WT_Matrix()
        , m_stage (Eating_Initial_Whitespace)
        , m_row_stage (Eating_Initial_Row_Whitespace)
    { }

    /// Constructs a WT_Matrix_IO object. Copy constructor.
    WT_Matrix_IO(WT_Matrix const & xform)
        : WT_Matrix( xform )
        , m_stage (Eating_Initial_Whitespace)
        , m_row_stage (Eating_Initial_Row_Whitespace)
    { }

    /// Constructs a WT_Matrix_IO object from an array of doubles (must be 16 doubles in the array.)
    WT_Matrix_IO(double const * xform)
        : WT_Matrix( xform )
        , m_stage (Eating_Initial_Whitespace)
        , m_row_stage (Eating_Initial_Row_Whitespace)
    { }

	virtual ~WT_Matrix_IO()
	{}
    //@}
public:

    /** \name Materialization / serialization
     *  \warning These methods are typically only used by parent objects.
     *  Client code should not call these methods.
     *  @{
     */
    WT_Result             serialize(WT_File & file) const;
    WT_Result             serialize_padded(WT_File & file) const;
    WT_Result             materialize(WT_File & file);
    WT_Result             materialize_row(WT_File & file, int row);
    //@}
};

#ifdef EMCC
#undef minor
#endif

/// Simple 3x3 matrix.
class WHIPTK_API WT_Matrix2D
{
protected:
	double m_elements[3][3];

public:

    //@{ \name Construction
    /// Constructs a WT_Matrix2D_IO object (sets it to an identity matrix.)
    WT_Matrix2D()
    { set_to_identity(); }
    /// Constructs a WT_Matrix2D_IO object.  Copy constructor.
    WT_Matrix2D( const WT_Matrix2D& r )
    { set(r); }

	virtual ~WT_Matrix2D()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Computes and returns the matrix's determinant.
    double determinant() const;
    /// Returns the adjoin of this matrix.
    void get_adjoint( WT_Matrix2D& matrix) const;
    /// Returns the inverse of this matrix.
    void get_inverse ( WT_Matrix2D& result) const;
    /// Returns the minor of the matrix using the submatrix comprised of the given rows and columns.
    /** \exception WT_Result::Toolkit_Usage_Error One of the arguments was out of range (0 <= x <= 2).
     */
    double minor(unsigned int r0, unsigned int r1, unsigned int c0, unsigned int c1) const;
    /// Returns the value at the given row and column
    /** \exception WT_Result::Toolkit_Usage_Error One of the arguments was out of range (0 <= x <= 2).
     */
    inline double  operator() ( unsigned int row, unsigned int col ) const
    {   if( row>2 || col>2 )
            throw WT_Result::Toolkit_Usage_Error;
        return m_elements[row][col];
    }
    /// Returns a reference to the value at the given row and column (can be used to set the value).
    /** \exception WT_Result::Toolkit_Usage_Error One of the arguments was out of range (0 <= x <= 2).
     */
    inline double& operator() ( unsigned int row, unsigned int col )
    {   if( row>2 || col>2 )
            throw WT_Result::Toolkit_Usage_Error;
        return m_elements[row][col];
    }
    //@}

    //@{ \name Manipulation methods
    /// Sets this matrix to its adjoint.
    WT_Matrix2D& adjoin();
    /// Sets the matrix from the given one (copies its values.)
    void set( const WT_Matrix2D& matrix);
    /// Rotate the matrix by the angle (degrees) provided
    void rotate (WT_Matrix2D & result, WT_Integer32 rotation) const;
    /// Rotate the matrix by the angle (rads) provided
    void rotate (WT_Matrix2D & result, double rotation) const;
    /// Sets the matrix to the identity matrix (1 0 0 0 1 0 0 0 1).
    WT_Matrix2D& set_to_identity();

    /// Uses this matrix to transform the given point.
    /** Transforms \a pt into \a result.
     *  \retval double The homogeneous adjustment (projection) constant for the point.
     */
    double transform(
        const WT_Point2D& pt, /**< The source point.*/
        WT_Point2D& result, /**< Output parameter.  The resultant transformed point. */
        double cutoff=0.0 /**< The maximum homogeneous adjustment (projection) allowed for the point. */
        ) const;

    /// Multiplies matrix by the matrix .
    WT_Matrix2D & operator *= (const WT_Matrix2D & rMatrix);

    /// Multiplies all matrix values by the given number.
    WT_Matrix2D & operator *= (double d);
    //@}

    //@{ \name Inspection methods
    /// Returns WD_True if the matrix is equivalent to this one.
    WT_Boolean operator== ( const WT_Matrix2D& matrix ) const;
    /// Returns WD_True if the matrix is NOT equivalent to this one.
    inline WT_Boolean operator!= ( const WT_Matrix2D& matrix ) const { return !(*this==matrix); }
    //@}

    /// Static identity matrix.
    static const WT_Matrix2D kIdentity;
};


/// Simple 3x3 matrix.
class WHIPTK_API WT_Matrix2D_IO : public WT_Matrix2D
{
	friend class WT_Plot_Info;
	friend class WT_W2D_Class_Factory;
	friend class WT_Class_Factory;

protected:

    //@{ \name Construction
    /// Constructs a WT_Matrix2D_IO object (sets it to an identity matrix.)
    WT_Matrix2D_IO()
        : WT_Matrix2D()
        , m_stage(Eating_Initial_Whitespace)
    { }
    /// Constructs a WT_Matrix2D_IO object.  Copy constructor.
    WT_Matrix2D_IO( const WT_Matrix2D_IO& r )
        : WT_Matrix2D( r )
        , m_stage(Eating_Initial_Whitespace)
    { }

    /// Constructs a WT_Matrix2D_IO object.  Copy constructor.
    WT_Matrix2D_IO( const WT_Matrix2D& r )
        : WT_Matrix2D( r )
        , m_stage(Eating_Initial_Whitespace)
    { }

	virtual ~WT_Matrix2D_IO()
	{}
    //@}
public:
    /** \name Materialization / serialization
     *  \warning These methods are typically only used by parent objects.
     *  Client code should not call these methods.
     *  @{
     */
    WT_Result serialize( WT_File& file) const;
    WT_Result materialize( WT_File& file);
    //@}

private:
    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Open_Paren_0,
        Getting_Open_Paren_1,
        Getting_Element_00,
        Getting_Element_01,
        Getting_Element_02,
        Getting_Close_Paren_1,
        Getting_Open_Paren_2,
        Getting_Element_10,
        Getting_Element_11,
        Getting_Element_12,
        Getting_Close_Paren_2,
        Getting_Open_Paren_3,
        Getting_Element_20,
        Getting_Element_21,
        Getting_Element_22,
        Getting_Close_Paren_3,
        Eating_End_Whitespace
    } m_stage;
};


#endif // MATRIX_HEADER
