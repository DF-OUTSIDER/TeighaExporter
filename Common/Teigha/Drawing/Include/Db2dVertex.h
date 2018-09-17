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




#ifndef _OD_DB_2DVERTEX_
#define _OD_DB_2DVERTEX_

#include "TD_PackPush.h"

#include "DbVertex.h"
#include "DwgDeclareMembers.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum Vertex2dType
  {
    k2dVertex          = 0,  // Standard vertex
    k2dSplineCtlVertex = 1,  // Spline-fit or curve-fit control point
    k2dSplineFitVertex = 2,  // Spline-fit generated vertex
    k2dCurveFitVertex  = 3   // Curve-fit generated vertex
  };
}

/** \details
    This class represents OdDb2Polyline vertices in an OdDbDatabase instance.
  
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDb2dVertex: public OdDbVertex
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDb2dVertex);

  /** \remarks
    Newly created vertices are of type OdDb::k2dVertex.
  */
  OdDb2dVertex();

  /** \details
    Returns the type of this Vertex entity.  
    
    \remarks
    vertexType() returns one of the following:

    <table>
    Name                        Value   DXF 70  Description    
    OdDb::k2dVertex             0       0x00    Standard vertex.
    OdDb::k2dSplineCtlVertex    1       0x10    Spline-fit or curve-fit control point.
    OdDb::k2dSplineFitVertex    2       0x08    Spline-fit generated vertex.
    OdDb::k2dCurveFitVertex     3       0x01    Curve-fit generated vertex.
    </table>
  */
  OdDb::Vertex2dType vertexType() const;

  /** \details
    Sets the type of this Vertex entity.

    \param vertexType [in]  Vertex type.

    \remarks
    vertexType type must be one of the following:

    <table>
    Name                        Value   DXF 70  Description    
    OdDb::k2dVertex             0       0x00    Standard vertex.
    OdDb::k2dSplineCtlVertex    1       0x10    Spline-fit or curve-fit control point.
    OdDb::k2dSplineFitVertex    2       0x08    Spline-fit generated vertex.
    OdDb::k2dCurveFitVertex     3       0x01    Curve-fit generated vertex.
    </table>
  */
  void setVertexType(
    OdDb::Vertex2dType vertexType);

  /** \details
    Returns the OdDb2dPolyline OCS position of this entity (DXF 10).

    \remarks
    Each OdDb2dPolyline is described a series of OCS OdDb2dVertices and an elevation.
    
    While Z coordinates are set and retrieved by setPosition() and position() respectively,
    they are ignored by the Polyline.
    
    The elevation of the Polyline can be changed only with OdDb2dPolyline::setElevation(). 
      
  */
  OdGePoint3d position() const;

  /** \details
    Sets the OdDb2dPolyline OCS position of this entity (DXF 10).
    \param position [in]  Position.
    
    \remarks
    Each OdDb2dPolyline is described a series of OCS OdDb2dVertices and an elevation.
    
    While Z coordinates are set and retrieved by setPosition() and position() respectively,
    they are ignored by the Polyline.
    
    The elevation of the Polyline can be changed only with OdDb2dPolyline::setElevation(). 
  */
  void setPosition(
    const OdGePoint3d& position);

  /** \details
    Returns the starting width of the segment following this Vertex entity (DXF 40).
  */
  double startWidth() const;

  /** \details
    Sets the starting width of the segment following this Vertex entity (DXF 40).

    \param startWidth [in]  Starting width.
  */
  void setStartWidth(
    double startWidth);

  /** \details
    Returns the ending width of the segment following this Vertex entity (DXF 41).
  */
  double endWidth() const;

  /** \details
    Sets the ending width of the segment following this Vertex entity (DXF 41).
   
    \param endWidth [in]  Ending width.
  */
  void setEndWidth(
    double endWidth);

  /** \details
    Returns the bulge of the segment following this Vertex entity (DXF 42).
    
    \remarks
    Bulge is the tangent of 1/4 the included angle of the arc segment, measured counterclockwise.
  */
  double bulge() const;

  /** \details
    Sets the bulge of the arc segment following this Vertex entity (DXF 42).

    \param bulge [in]  Bulge.
    
    \remarks
    Bulge is the tangent of 1/4 the included angle of the arc segment, measured counterclockwise.
  */
  void setBulge(
    double bulge);

  /** \details
    Returns true if and only if the curve fit tangent direction is used by this Vertex entity.(DXF 70, bit 0x02).
  */
  bool isTangentUsed() const;

  /** \details
    Specifies the curve fit tangent direction is to be used by this Vertex entity.(DXF 70, bit 0x02).
  */
  void useTangent();

  /** \details
    Specifies the curve fit tangent direction is not to be used by this Vertex entity.(DXF 70, bit 0x02).
  */
  void ignoreTangent();

  /** \details
    Returns the curve fit tangent direction for this Vertex (DXF 50).

    \remarks
    This is the angle between the OdDb2dPolyline OCS X-axis and the tangent direction.

    \remarks
    All angles are expressed in radians.
  */
  double tangent() const;

  /** \details
    Sets the curve fit tangent direction for this Vertex (DXF 50).

    \param tangent [in]  Tangent angle.
    
    \remarks
    This is the angle between the OdDb2dPolyline OCS X-axis and the tangent direction.

    \remarks
    All angles are expressed in radians.
  */
  void setTangent(
    double tangent);

  /** \details
    Returns the *vertex identifier* of this Vertex entity.
  */
  OdInt32 vertexIdentifier() const;

  /** \details
    Sets the *vertex identifier* of this Vertex entity.

    \param suggestedId [in]  Vertex identifier.
  */
  void setVertexIdentifier(
    OdInt32 suggestedId);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDb2dVertex object pointers.
*/
typedef OdSmartPtr<OdDb2dVertex> OdDb2dVertexPtr;

#include "TD_PackPop.h"

#endif

