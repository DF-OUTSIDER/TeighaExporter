/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////




#ifndef __ODGIPOLYLINE_H__
#define __ODGIPOLYLINE_H__

class OdGeVector3d;
class OdGeLineSeg2d;
class OdGeLineSeg3d;
class OdGeCircArc2d;
class OdGeCircArc3d;

#include "TD_PackPush.h"

#include "Gi/GiExport.h"

/** \details
    This class implements the OdGi equivalent of an OdDbPolyline or "lightweight" polyline.
    \remarks
    Instances of this class are used to pass lightweight polyline data through the 
    Teigha vectorization framework.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPolyline : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiPolyline);

  enum SegType
  {
    kLine,        // Straight segment with length greater than 0.
    kArc,         // Arc segment with length greater than 0.
    kCoincident,  // Segment with 0 length.
    kPoint,       // Polyline with a single vertex.
    kEmpty        // No vertices are present.
  };

  /** \details
      Returns the number of vertices in the entity.
  */
  virtual unsigned int numVerts() const = 0;

  /** \details
      Returns true if this polyline is closed, false otherwise. 
  */
  virtual bool isClosed() const = 0;

  /** \details
      Returns the normal vector for this entity.
  */
  virtual OdGeVector3d normal() const = 0;

  /** \details
      Returns the thickness for this entity.
  */
  virtual double thickness() const = 0;

  /** \details
      Returns the constant width for this entity.  
  */
  virtual double getConstantWidth() const = 0;

  /** \details
      Returns true if the vertices in this entity contain start and end width data, false otherwise.
  */
  virtual bool hasWidth() const = 0;

  /** \details
      Returns the PLINEGEN flag for this entity. If PLINEGEN is true,
      linetype generation will be patterned across the entire polyline, rather than 
      being done for each segment individually.
  */
  virtual bool hasPlinegen() const = 0;

  /** \details
      Returns the elevation for this entity.
  */
  virtual double elevation() const = 0;

  /** \details
      Returns the segment type at the specified index.

      \sa
      SegType
  */
  virtual SegType segType(unsigned int index) const = 0;
  
  /** \details
    Returns the line segment starting at the specified index.
  */
  virtual void getLineSegAt(unsigned int index, OdGeLineSeg2d& ln) const = 0;

  /** \details
    Returns the line segment starting at the specified index.
  */
  virtual void getLineSegAt(unsigned int index, OdGeLineSeg3d& ln) const = 0;

  /** \details
      Returns the arc segment starting at the specified index.
  */
  virtual void getArcSegAt(unsigned int index, OdGeCircArc2d& arc) const = 0;

  /** \details
      Returns the arc segment starting at the specified index.
  */
  virtual void getArcSegAt(unsigned int index, OdGeCircArc3d& arc) const = 0;

  /** \details
      Returns a specified point from this entity.
  */
  virtual void getPointAt(unsigned int index, OdGePoint2d& pt) const = 0;

  /** \details
      Returns the bulge value for a specified point.
  */
  virtual double getBulgeAt(unsigned int index) const = 0;

  /** \details
      Returns the start and end widths for a specified point.
  */
  virtual void getWidthsAt(unsigned int index, double& startWidth,  double& endWidth) const = 0;

  /** \details
      Returns the original OdDbPolyline object from which this OdGiPolyline was 
      created.
  */
  virtual OdRxObjectPtr getDbPolyline() const = 0;
};

typedef OdSmartPtr<OdGiPolyline> OdGiPolylinePtr;

#include "TD_PackPop.h"

#endif  // __ODGIPOLYLINE_H__

