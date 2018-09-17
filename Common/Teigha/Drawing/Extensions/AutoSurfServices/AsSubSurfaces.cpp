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

#include "StdAfx.h"
#include "AsSubSurfaces.h"
#include "DbFiler.h"

#define dxfRead1(code, member, rdFunc) case code: member = pFiler->rdFunc(); break;
#define dxfRead2(code, member, rdFunc) case code: pFiler->rdFunc(member); break;

OdAsUndefined::OdAsUndefined()
  : m_GripsCountU(0)
  , m_GripsCountV(0)
  , m_dSpan(0.)
{
}

OdResult OdAsUndefined::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_GripsCountU = pFiler->rdInt16();  // Grips Count U // 74
  m_GripsCountV = pFiler->rdInt16();  // Grips Count V // 75
  m_dSpan       = pFiler->rdDouble(); // Span          // 41

  return eOk;
}

void OdAsUndefined::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt16(m_GripsCountU); // Grips Count U // 74
  pFiler->wrInt16(m_GripsCountV); // Grips Count V // 75
  pFiler->wrDouble(m_dSpan);      // Span          // 41
}

OdResult OdAsUndefined::dxfInFields(OdDbDxfFiler* pFiler)
{
  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead1(74, m_GripsCountU, rdInt16 ); // Grips Count U // 74
      dxfRead1(75, m_GripsCountV, rdInt16 ); // Grips Count V // 75
      dxfRead1(41, m_dSpan,       rdDouble); // Span          // 41
      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsUndefined::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16 (73, (OdInt16)getType()); // undef  has 73 here
  pFiler->wrInt16 (74,  m_GripsCountU    ); // Grips Count U // 74
  pFiler->wrInt16 (75,  m_GripsCountV    ); // Grips Count V // 75
  pFiler->wrDouble(41, m_dSpan           ); // Span          // 41
}

OdAsCylinder::OdAsCylinder()
  : m_dStartAngle(0.)
  , m_dIncludedAngle(0.)
  , m_dRadius(0.)
  , m_dHeight(0.)
{
}

OdResult OdAsCylinder::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_basePoint3d    = pFiler->rdPoint3d();  // Base Point    // 10
  m_axisOfSymmetry = pFiler->rdVector3d(); // Extrusion     // 11
  m_refAxis        = pFiler->rdVector3d(); // Vector ?      // 12
  m_dStartAngle    = pFiler->rdDouble();   // Start angle   // 50
  m_dIncludedAngle = pFiler->rdDouble();   // Included angle// 51
  m_dRadius        = pFiler->rdDouble();   // Radius        // 42
  m_dHeight        = pFiler->rdDouble();   // Height        // 44

  return eOk;
}

void OdAsCylinder::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrPoint3d(m_basePoint3d);     // Base Point    // 10
  pFiler->wrVector3d(m_axisOfSymmetry); // Extrusion     // 11
  pFiler->wrVector3d(m_refAxis);        // Vector ?      // 12
  pFiler->wrDouble(m_dStartAngle);      // Start angle   // 50
  pFiler->wrDouble(m_dIncludedAngle);   // Included angle// 51
  pFiler->wrDouble(m_dRadius);          // Radius        // 42
  pFiler->wrDouble(m_dHeight);          // Height        // 44
}

OdResult OdAsCylinder::dxfInFields(OdDbDxfFiler* pFiler)
{
  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead2(10, m_basePoint3d,    rdPoint3d ); // Base Point    // 10
      dxfRead2(11, m_axisOfSymmetry, rdVector3d); // Extrusion     // 11
      dxfRead2(12, m_refAxis,        rdVector3d); // Vector ?      // 12
      dxfRead1(50, m_dStartAngle,    rdDouble  ); // Start angle   // 50
      dxfRead1(51, m_dIncludedAngle, rdDouble  ); // Included angle// 5
      dxfRead1(42, m_dRadius,        rdDouble  ); // Radius        // 42
      dxfRead1(44, m_dHeight,        rdDouble  ); // Height        // 44
      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsCylinder::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16   (72, (OdInt16)getType());
  pFiler->wrPoint3d (10, m_basePoint3d);     // Base Point    // 10
  pFiler->wrVector3d(11, m_axisOfSymmetry);  // Extrusion     // 11
  pFiler->wrVector3d(12, m_refAxis);         // Vector ?      // 12
  pFiler->wrDouble  (50, m_dStartAngle);     // Start angle   // 50
  pFiler->wrDouble  (51, m_dIncludedAngle);  // Included angle// 51
  pFiler->wrDouble  (42, m_dRadius);         // Radius        // 42
  pFiler->wrDouble  (44, m_dHeight);         // Height        // 44
}

OdAsCone::OdAsCone()
  : m_dStartAngle(0.)
  , m_dIncludedAngle(0.)
  , m_dBaseRadius(0.)
  , m_dTopRadius(0.)
  , m_dHeight(0.)
{
}

OdResult OdAsCone::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_basePoint3d    = pFiler->rdPoint3d();  // Base Point    // 10
  m_axisOfSymmetry = pFiler->rdVector3d(); // Extrusion     // 11
  m_refAxis        = pFiler->rdVector3d(); // Vector ?      // 12
  m_dStartAngle    = pFiler->rdDouble();   // Start angle   // 50
  m_dIncludedAngle = pFiler->rdDouble();   // Included angle// 51
  m_dBaseRadius    = pFiler->rdDouble();   // Base Radius   // 42
  m_dTopRadius     = pFiler->rdDouble();   // Top Radius    // 43
  m_dHeight        = pFiler->rdDouble();   // Height        // 44

  return eOk;
}

void OdAsCone::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrPoint3d(m_basePoint3d);     // Base Point    // 10
  pFiler->wrVector3d(m_axisOfSymmetry); // Extrusion     // 11
  pFiler->wrVector3d(m_refAxis);        // Vector ?      // 12
  pFiler->wrDouble(m_dStartAngle);      // Start angle   // 50
  pFiler->wrDouble(m_dIncludedAngle);   // Included angle// 51
  pFiler->wrDouble(m_dBaseRadius);      // Base Radius   // 42
  pFiler->wrDouble(m_dTopRadius);       // Top Radius    // 43
  pFiler->wrDouble(m_dHeight);          // Height        // 44
}

OdResult OdAsCone::dxfInFields(OdDbDxfFiler* pFiler)
{
  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead2(10, m_basePoint3d   , rdPoint3d ); // Base Point    // 10
      dxfRead2(11, m_axisOfSymmetry, rdVector3d); // Extrusion     // 11
      dxfRead2(12, m_refAxis       , rdVector3d); // Vector ?      // 12
      dxfRead1(50, m_dStartAngle   , rdDouble  ); // Start angle   // 50
      dxfRead1(51, m_dIncludedAngle, rdDouble  ); // Included angle// 51
      dxfRead1(42, m_dBaseRadius   , rdDouble  ); // Base Radius   // 42
      dxfRead1(43, m_dTopRadius    , rdDouble  ); // Top Radius    // 43
      dxfRead1(44, m_dHeight       , rdDouble  ); // Height        // 44
      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsCone::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16   (72, (OdInt16)getType());
  pFiler->wrPoint3d (10, m_basePoint3d   ); // Base Point    // 10
  pFiler->wrVector3d(11, m_axisOfSymmetry); // Extrusion     // 11
  pFiler->wrVector3d(12, m_refAxis       ); // Vector ?      // 12
  pFiler->wrDouble  (50, m_dStartAngle   ); // Start angle   // 50
  pFiler->wrDouble  (51, m_dIncludedAngle); // Included angle// 51
  pFiler->wrDouble  (42, m_dBaseRadius   ); // Base Radius   // 42
  pFiler->wrDouble  (43, m_dTopRadius    ); // Top Radius    // 43
  pFiler->wrDouble  (44, m_dHeight       ); // Height        // 44
}

OdAsSphere::OdAsSphere()
  : m_dStartAngle(0.)
  , m_dIncludedAngle(0.)
  , m_dRadius(0.)
{
}

OdResult OdAsSphere::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_Center3d       = pFiler->rdPoint3d();  // Base Point    // 10
  m_northAxis      = pFiler->rdVector3d(); // Extrusion     // 11
  m_refAxis        = pFiler->rdVector3d(); // Vector ?      // 12
  m_dStartAngle    = pFiler->rdDouble();   // Start angle   // 50
  m_dIncludedAngle = pFiler->rdDouble();   // Included angle// 51
  m_dRadius        = pFiler->rdDouble();   // Radius        // 42

  return eOk;
}

void OdAsSphere::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrPoint3d(m_Center3d);        // Base Point    // 10
  pFiler->wrVector3d(m_northAxis);      // Extrusion     // 11
  pFiler->wrVector3d(m_refAxis);        // Vector ?      // 12
  pFiler->wrDouble(m_dStartAngle);      // Start angle   // 50
  pFiler->wrDouble(m_dIncludedAngle);   // Included angle// 51
  pFiler->wrDouble(m_dRadius);          // Radius        // 42
}

OdResult OdAsSphere::dxfInFields(OdDbDxfFiler* pFiler)
{
  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead2(10, m_Center3d       , rdPoint3d);  // Base Point    // 10
      dxfRead2(11, m_northAxis      , rdVector3d); // Extrusion     // 11
      dxfRead2(12, m_refAxis        , rdVector3d); // Vector ?      // 12
      dxfRead1(50, m_dStartAngle    , rdDouble);   // Start angle   // 50
      dxfRead1(51, m_dIncludedAngle , rdDouble);   // Included angle// 51
      dxfRead1(42, m_dRadius        , rdDouble);   // Radius        // 42
      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsSphere::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16   (72, (OdInt16)getType());
  pFiler->wrPoint3d (10, m_Center3d      ); // Base Point    // 10
  pFiler->wrVector3d(11, m_northAxis     ); // Extrusion     // 11
  pFiler->wrVector3d(12, m_refAxis       ); // Vector ?      // 12
  pFiler->wrDouble  (50, m_dStartAngle   ); // Start angle   // 50
  pFiler->wrDouble  (51, m_dIncludedAngle); // Included angle// 51
  pFiler->wrDouble  (42, m_dRadius       ); // Radius        // 42
}


OdAsTorus::OdAsTorus()
  : m_dStartAngle(0.)
  , m_dIncludedAngle(0.)
  , m_dRadius(0.)
  , m_dTubeRadius(0.)
{
}

OdResult OdAsTorus::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_Origin3d       = pFiler->rdPoint3d();  // Base Point    // 10
  m_axisOfSymmetry = pFiler->rdVector3d(); // Extrusion     // 11
  m_refAxis        = pFiler->rdVector3d(); // Vector ?      // 12
  m_dStartAngle    = pFiler->rdDouble();   // Start angle   // 50
  m_dIncludedAngle = pFiler->rdDouble();   // Included angle// 51
  m_dRadius        = pFiler->rdDouble();   // Radius        // 42
  m_dTubeRadius    = pFiler->rdDouble();   // Tube Radius   // 43

  return eOk;
}

void OdAsTorus::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrPoint3d(m_Origin3d);        // Base Point    // 10
  pFiler->wrVector3d(m_axisOfSymmetry); // Extrusion     // 11
  pFiler->wrVector3d(m_refAxis);        // Vector ?      // 12
  pFiler->wrDouble(m_dStartAngle);      // Start angle   // 50
  pFiler->wrDouble(m_dIncludedAngle);   // Included angle// 51
  pFiler->wrDouble(m_dRadius);          // Radius        // 42
  pFiler->wrDouble(m_dTubeRadius);      // Tube Radius   // 43
}
      
OdResult OdAsTorus::dxfInFields(OdDbDxfFiler* pFiler)
{
  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead2(10, m_Origin3d       , rdPoint3d ); // Base Point    // 10
      dxfRead2(11, m_axisOfSymmetry , rdVector3d); // Extrusion     // 11
      dxfRead2(12, m_refAxis        , rdVector3d); // Vector ?      // 12
      dxfRead1(50, m_dStartAngle    , rdDouble  ); // Start angle   // 50
      dxfRead1(51, m_dIncludedAngle , rdDouble  ); // Included angle// 51
      dxfRead1(42, m_dRadius        , rdDouble  ); // Radius        // 42
      dxfRead1(43, m_dTubeRadius    , rdDouble  ); // Tube Radius   // 43
      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsTorus::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16   (72, (OdInt16)getType());
  pFiler->wrPoint3d (10, m_Origin3d      ); // Base Point    // 10
  pFiler->wrVector3d(11, m_axisOfSymmetry); // Extrusion     // 11
  pFiler->wrVector3d(12, m_refAxis       ); // Vector ?      // 12
  pFiler->wrDouble  (50, m_dStartAngle   ); // Start angle   // 50
  pFiler->wrDouble  (51, m_dIncludedAngle); // Included angle// 51
  pFiler->wrDouble  (42, m_dRadius       ); // Radius        // 42
  pFiler->wrDouble  (43, m_dTubeRadius   ); // Tube Radius   // 43
}



OdAsRevolve::OdAsRevolve()
  : m_dStartAngle(0.)
  , m_dIncludedAngle(0.)
  , m_Unknown(0)
{
}

OdResult OdAsRevolve::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_AxisStartPoint = pFiler->rdPoint3d();  // Axis Base Point             10
  m_AxisDirection  = pFiler->rdVector3d(); // Axis Direction (normalized) 11
  m_dStartAngle    = pFiler->rdDouble();   // Start angle      50
  m_dIncludedAngle = pFiler->rdDouble();   // Included angle   51
  m_Unknown        = pFiler->rdInt16();    // UnkCount         75
  ODA_ASSERT(m_Unknown == 3);
  bool bWeights    = pFiler->rdBool();     // 76
  int nKnotsCnt    = pFiler->rdInt16();    // 77
  
  m_Knots.resize(nKnotsCnt);

  int i;
  for(i = 0; i < nKnotsCnt; ++i)
  {
    m_Knots[i] = pFiler->rdDouble();       // 45
  }

  int nCtrlPtsCnt  = pFiler->rdInt16();    // 78

  m_CtrlPoints.resize(nCtrlPtsCnt);

  for(i = 0; i < nCtrlPtsCnt; ++i)
  {
    m_CtrlPoints[i] = pFiler->rdPoint3d(); // 13
  }

  if (bWeights)
  {
    m_Weights.resize(nCtrlPtsCnt);
    for(i = 0; i < nCtrlPtsCnt; ++i)
    {
      m_Weights[i] = pFiler->rdDouble();  // 46
    }
  }
  else
  {
    m_Weights.clear();
  }

  return eOk;
}

void OdAsRevolve::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrPoint3d(m_AxisStartPoint);  // Axis Base Point             10
  pFiler->wrVector3d(m_AxisDirection); // Axis Direction (normalized) 11

  pFiler->wrDouble(m_dStartAngle);   // Start angle      50
  pFiler->wrDouble(m_dIncludedAngle);   // Included angle   51

  pFiler->wrInt16(m_Unknown);  // UnkCount         75

  pFiler->wrBool(!m_Weights.isEmpty());  //76

  OdInt16 nKnotsCnt = (OdInt16)m_Knots.size(); 
  pFiler->wrInt16(nKnotsCnt);  //77
  
  int i;
  for(i = 0; i < nKnotsCnt; ++i)
  {
    pFiler->wrDouble(m_Knots[i]); //45
  }

  OdInt16 nCtrlPtsCnt = (OdInt16)m_CtrlPoints.size();
  pFiler->wrInt16(nCtrlPtsCnt); //78

  for(i = 0; i < nCtrlPtsCnt; ++i)
  {
    pFiler->wrPoint3d(m_CtrlPoints[i]); //13
  }

  if (!m_Weights.isEmpty())
  {
    int nWeights = m_Weights.size();
    for(i = 0; i < nWeights; ++i)
    {
      pFiler->wrDouble(m_Weights[i]);//46
    }
  }
}

OdResult OdAsRevolve::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdInt16 bWeights = 0;
  int nKnotsCnt = 0;
  int nCtrlPtsCnt = 0;
  int nKnotsIndx = 0;
  int nCtrlPtsIndx = 0;
  int nWeightsIndx = 0;
  
  m_CtrlPoints.clear();
  m_Knots.clear();
  m_Weights.clear();

  int   nCode;
  while (!pFiler->atEOF())
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      dxfRead2(10, m_AxisStartPoint , rdPoint3d ); // Axis Base Point             10
      dxfRead2(11, m_AxisDirection  , rdVector3d); // Axis Direction (normalized) 11
      dxfRead1(50, m_dStartAngle    , rdDouble  ); // Start angle                 50
      dxfRead1(51, m_dIncludedAngle , rdDouble  ); // Included angle              51
      dxfRead1(75, m_Unknown        , rdInt16   ); // UnkCount                    75
      dxfRead1(76, bWeights         , rdInt16   ); //                             76

      case 77:
        nKnotsCnt = pFiler->rdInt16();   // 77
        m_Knots.resize(nKnotsCnt);
        nKnotsIndx = 0;
      break;
      case 78:
        nCtrlPtsCnt = pFiler->rdInt16(); // 78
        m_CtrlPoints.resize(nCtrlPtsCnt);
        nCtrlPtsIndx = 0;
      break;

      case 45:
        if (nKnotsIndx < nKnotsCnt)
        {
          m_Knots[nKnotsIndx++] = pFiler->rdDouble(); // 45
        }
        else
        {
          ODA_ASSERT(0);
          pFiler->rdDouble();
        }
      break;

      case 13:
        if (nCtrlPtsIndx < nCtrlPtsCnt)
        {
          pFiler->rdPoint3d( m_CtrlPoints[nCtrlPtsIndx++] ); // 13
        }
        else
        {
          ODA_ASSERT(0);
          pFiler->rdDouble(); // 13
        }
      break;

      case 46:
        ODA_ASSERT(bWeights);
        if (m_Weights.isEmpty())
        {
          m_Weights.resize(nCtrlPtsCnt);
        }

        m_Weights[nWeightsIndx++] = pFiler->rdDouble(); // 46
      break;

      default:
        ODA_ASSERT(false);
        break;
    }
  }
  return eOk;
}

void OdAsRevolve::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrInt16   (72, (OdInt16)getType());
  pFiler->wrPoint3d (10, m_AxisStartPoint);// Axis Base Point             10
  pFiler->wrVector3d(11, m_AxisDirection); // Axis Direction (normalized) 11

  pFiler->wrDouble(50, m_dStartAngle);     // Start angle      50
  pFiler->wrDouble(51, m_dIncludedAngle);  // Included angle   51

  pFiler->wrInt16(75, m_Unknown);          // UnkCount         75

  pFiler->wrInt16(76, m_Weights.isEmpty() ? 0 : 1);//76

  OdInt16 nKnotsCnt = (OdInt16)m_Knots.size(); 
  pFiler->wrInt16(77, nKnotsCnt);          //77
  
  int i;
  for(i = 0; i < nKnotsCnt; ++i)
  {
    pFiler->wrDouble(45, m_Knots[i]);     //45
  }

  OdInt16 nCtrlPtsCnt = (OdInt16)m_CtrlPoints.size();
  pFiler->wrInt16(78, nCtrlPtsCnt);       //78

  for(i = 0; i < nCtrlPtsCnt; ++i)
  {
    pFiler->wrPoint3d(13, m_CtrlPoints[i]);//13
  }

  if (!m_Weights.isEmpty())
  {
    int nWeights = m_Weights.size();
    for(i = 0; i < nWeights; ++i)
    {
      pFiler->wrDouble(46, m_Weights[i]);  //46
    }
  }
}

OdAsSurface::~OdAsSurface()
{
}

#undef dxfRead1
#undef dxfRead2

