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

#ifndef _TRIANGULATIONPARAMS_H_
#define _TRIANGULATIONPARAMS_H_

#include "TD_PackPush.h"

#include "OdHeap.h"

/** \details
    This container class holds ACIS triangulation (rendering) parameters. 
    <group Other_Classes>
\remarks
    Default triangulation parameters:
    
    <table>
    Parameter             Default
    maxNumGridLines       10000  
    maxFacetEdgeLength    0  
    normalTolerance       15  
    surfaceTolerance      0  
    gridAspectRatio       0  
    PointsPerEdge         0  
    BetweenKnots          2  
    bUseFacetRES          false  
    </table>    
*/
class wrTriangulationParams
{
public:
  /** \details 
  Maximum number of grid lines.*/
  OdUInt32 maxNumGridLines;
  
  /** \details
    Maximum facet edge length.
  */
  double   maxFacetEdgeLength;
  
  /** \details 
  NormalTolerance allows to specify the tessellation quality of circular surfaces (such as Cone, Torus, Sphere or Cylinder). 
  It is measured in degrees. The value of this parameter determines the quantity of mesh cells (triangles) used to represent a circular surface (360/NormalTolerance). 
  Default value is 15. It means, for an example, that surface will be tessellated with 24 mesh cells (triangles) in circular direction. 
  */
  double   normalTolerance;

  /** \details
    Distance tolerance for coincident faces.
  */
  double   surfaceTolerance;

  /** \details
    Grid aspect ratio.
  */
  double   gridAspectRatio;
  
  /** \details
    Distance between knots. This parameter contains the number of additional points between knots; for NURB surfaces it contains number of additional isolines between knots. 
  */
  OdUInt16 BetweenKnots;
  
  /** \details
    Points quantity per edge.
  */
  OdUInt32 PointsPerEdge; 
  
  /** \details
    Use Facetres System variable.
  */
  bool     bUseFacetRES;
  
  /** \details
    Fast mode is much more faster, but generates more errors.
  */
  bool     bFastMode;

  /** \details
    Use new tesselation algo for nurbs surfaces (not in a production state yet).
  */
  bool bUseTesselation;

  /** \details
    Default constructor. Sets the following default triangulation parameters:
    <table>
    Parameter             Default
    maxNumGridLines       10000  
    maxFacetEdgeLength    0  
    normalTolerance       15  
    surfaceTolerance      0  
    gridAspectRatio       0  
    PointsPerEdge         0  
    BetweenKnots          2  
    bUseFacetRES          false  
    </table>
  */
  wrTriangulationParams(bool bNewTess = false)
  {
    if (bNewTess)
    {
      maxNumGridLines = 10000;
      maxFacetEdgeLength = 100000;
      normalTolerance = 360;
      surfaceTolerance = 0; // must be overriden!
      gridAspectRatio = 0;
      BetweenKnots = 0;
      PointsPerEdge = 0;
      bUseFacetRES = true;
      bFastMode = false;
      bUseTesselation = true;
    }
    else
    {
      maxNumGridLines = 10000;
      maxFacetEdgeLength = 0;
      normalTolerance = 15;
      surfaceTolerance = 0;
      gridAspectRatio = 0;
      BetweenKnots = 2;
      PointsPerEdge = 0;
      bUseFacetRES = false;
      bFastMode = false;
      bUseTesselation = false;
    }
  }

  /** \details
      Comparing operator for wrTriangulationParams class.
      \param other [in] A wrTriangulationParams object to be compared with.
      Note that bUseFacetRES property is NOT compared.
  */
  bool operator ==(const wrTriangulationParams other) const 
  {
    return other.maxNumGridLines == maxNumGridLines
    && other.maxFacetEdgeLength == maxFacetEdgeLength
    && other.normalTolerance == normalTolerance
    && other.surfaceTolerance == surfaceTolerance
    && other.gridAspectRatio == gridAspectRatio
    && other.PointsPerEdge == PointsPerEdge
    && other.BetweenKnots == BetweenKnots;
    // bUseFacetRES, bFastMode skipped
  }
};

#include "TD_PackPop.h"

#endif // _TRIANGULATIONPARAMS_H_
