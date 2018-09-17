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
///\file        dwf/whiptk/point.h
///

#if !defined POINT_HEADER
#define POINT_HEADER

#include "dwfcore/String.h"


/// 3D point (using doubles) used in 4x4 matrix calculations.
/** \sa WT_Matrix_IO */
class WHIPTK_API WT_Point3D
{
public:
    double        m_x; /**< Magnitude of point along the X-axis. */
    double        m_y; /**< Magnitude of point along the Y-axis. */
    double        m_z; /**< Magnitude of point along the Z-axis. */

    /// Constructs a WT_Point3D object.
    WT_Point3D() {}

    /// Constructs a WT_Point3D object with the given coordinates.
    WT_Point3D(double x, double y, double z = 0.0)
        : m_x (x)
        , m_y (y)
        , m_z (z)
    { }

    /// Returns WD_True if the two points are equal.
    WT_Boolean operator== (WT_Point3D const & p3d) const
    {
        if (m_x == p3d.m_x && m_y == p3d.m_y && m_z == p3d.m_z)
            return WD_True;
        else
            return WD_False;
    }
};

/// 2D point (using doubles) used in 3x3 matrix calculations.
/** \sa WT_Matrix2D_IO */
class WHIPTK_API WT_Point2D
{
private:
	char          m_zPointString[64];
    WT_Unsigned_Integer16 _nPrecision;
    double        _x; 
    double        _y; 
public:
    double        m_x; /**< Magnitude of point along the X-axis. */
    double        m_y; /**< Magnitude of point along the Y-axis. */

    /// Constructs a WT_Point2D object.
    WT_Point2D()
        :  _nPrecision(10)
        , _x(0.0)
        , _y(0.0) 
        , m_x(0.0)
        , m_y(0.0)  
    {
        m_zPointString[0] = '\0';
    }

    /// Copy constructor.  Constructs a WT_Point2D object from another.
    WT_Point2D(WT_Point2D const &r)
        : _nPrecision(r._nPrecision)
        , _x(r._x)
        , _y(r._y)  
        , m_x(r.m_x)
        , m_y(r.m_y)  
    {
        m_zPointString[0] = '\0';
    }

    /// Constructs a WT_Point2D object with the given coordinates.
    WT_Point2D(double x, double y)
        : _nPrecision(10)
        , _x (x)
        , _y (y)
        , m_x (x)
        , m_y (y)
    { 
        m_zPointString[0] = '\0';
    }

	virtual~ WT_Point2D()
	{
	}

    /// Assignment operator implementation
    WT_Point2D const &operator= (WT_Point2D const & rPt)
    {
        _x = rPt._x;
        _y = rPt._y;
        m_x = rPt.m_x;
        m_y = rPt.m_y;
        m_zPointString[0] = '\0';
        return *this;
    }

    /// Returns WD_True if the two points are equal.
    WT_Boolean operator== (WT_Point2D const & rPt) const
    {
        if (m_x == rPt.m_x && m_y == rPt.m_y )
            return WD_True;
        else
            return WD_False;
    }

    /// Returns a string representation of the point, with the provided precision
	const char * toString( WT_Unsigned_Integer16 nPrecision )
	{
        if (m_zPointString[0] == '\0' || _x != m_x || _y != m_y || _nPrecision != nPrecision)
        {
            _x = m_x;
            _y = m_y;

            if (nPrecision > 17)
                nPrecision = 17;

            _nPrecision = nPrecision;

		    char cFormattedBuf[64];
            DWFCore::DWFString::DoubleToString( cFormattedBuf, 64, m_x, nPrecision );
            
		    DWFCORE_ASCII_STRING_COPY(m_zPointString, cFormattedBuf);
		    DWFCORE_ASCII_STRING_CONCATENATE(m_zPointString, ",");

            DWFCore::DWFString::DoubleToString( cFormattedBuf, 64, m_y, nPrecision );
		    DWFCORE_ASCII_STRING_CONCATENATE(m_zPointString, cFormattedBuf);
        }

		return m_zPointString;
	}

};

#endif // POINT_HEADER
