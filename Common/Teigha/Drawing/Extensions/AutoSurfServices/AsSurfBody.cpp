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

// OdAsSurfBody.cpp: implementation of the OdAsSurfBody class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AsSurfBody.h"
#include "DbFiler.h"
#include "Gi/GiWorldDraw.h"
#include "Ge/GeNurbCurve3d.h"
#include "Br/BrBrepFaceTraverser.h"
#include "DbProxyEntity.h"
#include "MemoryStream.h"
#include "DbDatabase.h"
#include "GeometryFromProxy.h"
#include "DbProxyEntity.h"

TOOLKIT_EXPORT OdRxDictionaryIteratorPtr odDbGetDrawingClasses(OdDbDatabase& database);

ODRX_DEFINE_MEMBERS_EX(OdAsSurfBody
                  , OdDbBody
                  , DBOBJECT_CONSTR
                  , 0
                  , 0
                  , 0
                  , OD_T("AcAsSurfBody")
                  , OD_T("ASSURFACE")
                  , OD_T("AutoSurfServices"), OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware);


#define dxfRead1(code, member, rdFunc) case code: member = pFiler->rdFunc(); break;
#define dxfRead2(code, member, rdFunc) case code: pFiler->rdFunc(member); break;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OdAsSurfBody::OdAsSurfBody()
  : m_pAsSurface(NULL)
  , m_nFormatVersion(1)
  , m_dNormalLength(0.)
  , m_LinesCountU(0)
  , m_LinesCountV(0)
  , m_bHasSubNurb(false)
  , m_Unknown4(0)
  , m_Unknown5(0)
  , m_iUDegree(0)
  , m_iVDegree(0)
{
}

OdAsSurfBody::~OdAsSurfBody()
{
  clear();
}

void OdAsSurfBody::clear()
{
  if (m_pAsSurface)
    delete m_pAsSurface;
  m_pAsSurface = NULL;

  m_Loops2d.clear();

  m_bHasSubNurb = false;
  m_nFormatVersion = 2;

}

void OdAsSurfBody::dwgOutAsSurface(OdDbDwgFiler* pFiler) const
{
  if (m_pAsSurface)
  {
    OdAsSurface::AsSurfType cm = m_pAsSurface->getType();
    pFiler->wrInt16(OdInt16(cm));

    m_pAsSurface->dwgOutFields(pFiler);
  }
}

static OdAsSurface *getAsSurfaceByType(OdAsSurface::AsSurfType cm)
{
  switch (cm)
  {
    case OdAsSurface::kAsSurfUndefined:
        return new OdAsUndefined();
    case OdAsSurface::kAsSurfCylinder:
        return new OdAsCylinder();
    case OdAsSurface::kAsSurfCone:
        return new OdAsCone();
    case OdAsSurface::kAsSurfSphere:
        return new OdAsSphere();
    case OdAsSurface::kAsSurfTorus:
        return new OdAsTorus();
    case OdAsSurface::kAsSurfRevolve:
        return new OdAsRevolve();
  }
  return NULL;
}

OdResult OdAsSurfBody::dwgInAsSurface(OdDbDwgFiler* pFiler)
{
  OdAsSurface::AsSurfType cm = (OdAsSurface::AsSurfType)pFiler->rdInt16(); // dxf 72 (cone, cyl, sp, tor ) 73 (undef)

  m_pAsSurface = getAsSurfaceByType(cm);
  if (!m_pAsSurface)
    return eMakeMeProxy;

  return m_pAsSurface->dwgInFields(pFiler);
}

void OdAsSurfBody::dwgOutSubNurbSurface(OdDbDwgFiler* pFiler) const
{
  int degreeInU; 
  int degreeInV;
  int propsInU; 
  int propsInV;
  int numControlPointsInU;
  int numControlPointsInV;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray weights;
  OdGeKnotVector uKnots;
  OdGeKnotVector vKnots;

  m_NURBSurf.getDefinition (degreeInU, degreeInV, propsInU, propsInV,
    numControlPointsInU, numControlPointsInV, controlPoints, weights, uKnots, vKnots);

  bool hasWeights = !weights.isEmpty();
  pFiler->wrInt16(hasWeights);
  
  pFiler->wrInt16( (OdInt16)m_iUDegree );
  pFiler->wrInt16( (OdInt16)m_iVDegree );

  
  OdInt16 ctrlPtInU = (OdInt16)m_NURBSurf.numControlPointsInU();
  OdInt16 ctrlPtInV = (OdInt16)m_NURBSurf.numControlPointsInV();
  pFiler->wrInt16( ctrlPtInU );
  pFiler->wrInt16( ctrlPtInV );

  int numCtrlPts = ctrlPtInU*ctrlPtInV;

  // Knots Counts
  OdInt16 numKnotsU = (OdInt16)uKnots.length();
  OdInt16 numKnotsV = (OdInt16)vKnots.length();
  pFiler->wrInt16(numKnotsU);
  pFiler->wrInt16(numKnotsV);

  int i;
  for (i = 0; i < numCtrlPts; ++i)
  {
    pFiler->wrPoint3d( hasWeights ? (controlPoints[i]*weights[i]) : controlPoints[i] );
    if (hasWeights)
      pFiler->wrDouble(weights[i]);
  }

  // U Knots
  for (i = 0; i < numKnotsU; ++i)
  {
    pFiler->wrDouble( uKnots[i] );
  }
  // V Knots
  for (i = 0; i < numKnotsV; ++i)
  {
    pFiler->wrDouble( vKnots[i] );
  }
}

OdResult OdAsSurfBody::dwgInSubNurbSurface(OdDbDwgFiler* pFiler)
{
    // Are weights present?
  OdInt16 Unknown2 = pFiler->rdInt16(); ODA_ASSERT(Unknown2 == 1 || Unknown2 == 0);
  bool hasWeights = (Unknown2 & 1) != 0;

  // seems to be UV surface properties, possible
  // 1 - closed   ????
  // 2 - rational ????
  // 4 - poles at both ???
  //That object sat will be a surface.(spline-surface)
  //This is degrees of BS3_Surface.
  m_iUDegree = pFiler->rdInt16(); //ODA_ASSERT(m_iUDegree ==1 || m_iUDegree==2 ||m_iUDegree==3 ||m_iUDegree==4);
  //m_iUDegree &= 1;
  m_iVDegree = pFiler->rdInt16(); //ODA_ASSERT(m_iVDegree ==1 || m_iVDegree==2 ||m_iVDegree==3 ||m_iVDegree==4 ||m_iVDegree==6);
  //m_iVDegree &= 1;

  // Control Points Counts
  OdInt16 ctrlPtInU = pFiler->rdInt16();
  OdInt16 ctrlPtInV = pFiler->rdInt16();
  int numCtrlPts = ctrlPtInU*ctrlPtInV;

  // Knots Counts
  OdInt16 numKnotsU = pFiler->rdInt16();
  OdInt16 numKnotsV = pFiler->rdInt16();

  // Control Points (X,Y,Z,W):
	OdGePoint3dArray controlPoints;
	OdGeDoubleArray weights;
  controlPoints.resize(numCtrlPts);
  
  if (hasWeights)
    weights.resize(numCtrlPts);

  int vc, i;

  for (vc = 0; vc < numCtrlPts; ++vc)
  {
    OdGePoint3d pnt = pFiler->rdPoint3d();
    if (hasWeights)
    {
      double d = pFiler->rdDouble();
      weights.setAt(vc, d);
      pnt /= d;
    }
    controlPoints.setAt(vc, pnt);
  }

  // U Knots
	OdGeKnotVector uKnots;
  uKnots.setLogicalLength(numKnotsU);
  for (i = 0; i < numKnotsU; ++i)
  {
    uKnots[i] = pFiler->rdDouble();
  }
  // V Knots
	OdGeKnotVector vKnots;
  vKnots.setLogicalLength(numKnotsV);
  for (i = 0; i < numKnotsV; ++i)
  {
    vKnots[i] = pFiler->rdDouble();
  }

  int degreeU = numKnotsU - ctrlPtInU - 1;
  int degreeV = numKnotsV - ctrlPtInV - 1;
  m_NURBSurf.set(degreeU, degreeV, 0, 0, ctrlPtInU, ctrlPtInV,
           controlPoints, weights, uKnots, vKnots);

  return eOk;
}

void OdAsSurfBody::dwgOutTrimmingLoops(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt16(m_Unknown4);
  pFiler->wrDouble(m_Unknown5);

  OdInt16 nLoops = (OdInt16)m_Loops2d.size();
  pFiler->wrInt16(nLoops);

  int i;
  for (i = 0; i < nLoops; ++i)
  {
    OdInt16 nSplines = (OdInt16)m_Loops2d[i].size();
    pFiler->wrInt16(nSplines);

    for (int j = 0; j < nSplines; ++j)
    {
      pFiler->wrInt16( m_Loops2d[i][j].Unknown6 );
    
      int degree;
      bool rational;
      bool periodic;
      OdGeKnotVector knots;
      OdGePoint2dArray controlPoints;
      OdGeDoubleArray weights;
      m_Loops2d[i][j].NurbCurve2d.getDefinitionData ( degree, rational,periodic, knots, controlPoints, weights);
      
      pFiler->wrInt16((OdInt16)(degree+1));
      
      OdInt16 bWeightPresent = !weights.isEmpty();
      pFiler->wrInt16(bWeightPresent);

      int nCounter1 = controlPoints.size();
      int nCounter2 = knots.length();

      pFiler->wrInt16((OdInt16)nCounter1); 
      pFiler->wrInt16((OdInt16)nCounter2); 

      int k;
      for (k = 0; k < nCounter1; ++k) // 2d cntrl points
      {
        if (m_nFormatVersion == 1)
        {
          pFiler->wrDouble(controlPoints[k].x);
          pFiler->wrDouble(controlPoints[k].y);
        }
        else
        {
          pFiler->wrPoint2d(controlPoints[k]);
        }
        if (bWeightPresent)
        {
          pFiler->wrDouble(weights[k]);
        }
      }

      for (k = 0; k < nCounter2; ++k) // knots ????
      {
        pFiler->wrDouble(knots[k]);
      }
    }
  }
}

OdResult OdAsSurfBody::dwgInTrimmingLoops(OdDbDwgFiler* pFiler)
{
  m_Unknown4 = pFiler->rdInt16(); ODA_ASSERT(m_Unknown4 == 1 || m_Unknown4 == -1);
  m_Unknown5 = pFiler->rdDouble();

  OdInt16 nLoops = pFiler->rdInt16();
  m_Loops2d.resize(nLoops);

  int i;
  for (i = 0; i < nLoops; ++i)
  {
    OdInt16 nSplines = pFiler->rdInt16();
    m_Loops2d[i].resize(nSplines);
    for (int j = 0; j < nSplines; ++j)
    {
      m_Loops2d[i][j].Unknown6 = pFiler->rdInt16(); ODA_ASSERT(m_Loops2d[i][j].Unknown6 == -1 || m_Loops2d[i][j].Unknown6 == 3 || m_Loops2d[i][j].Unknown6 == 1 || m_Loops2d[i][j].Unknown6 == 2 || m_Loops2d[i][j].Unknown6 == 0 || m_Loops2d[i][j].Unknown6 == 9 || m_Loops2d[i][j].Unknown6 == 8 || m_Loops2d[i][j].Unknown6 == 6 || m_Loops2d[i][j].Unknown6 == 7);
      
      OdInt16 Degree = pFiler->rdInt16();
      OdInt16 bWeightPresent = pFiler->rdInt16(); ODA_ASSERT(bWeightPresent == 0 || bWeightPresent == 1); // weight ???

      int nCounter1 = pFiler->rdInt16(); 
      int nCounter2 = pFiler->rdInt16(); ODA_ASSERT(Degree == nCounter2-nCounter1);
      int k;

      OdGePoint2dArray controlPoints;
      controlPoints.resize(nCounter1);
      
      OdGeDoubleArray weights;
      if (bWeightPresent)
        weights.resize(nCounter1);

      for (k = 0; k < nCounter1; ++k) // 2d cntrl points ???
      {
        OdGePoint2d p2d;
        if (m_nFormatVersion == 1)
        {
          controlPoints[k].x = pFiler->rdDouble();
          controlPoints[k].y = pFiler->rdDouble();
        }
        else
        {
          controlPoints[k] = pFiler->rdPoint2d();
        }
        if (bWeightPresent)
        {
          weights[k] = pFiler->rdDouble();
        }
      }

      OdGeKnotVector knots;
      knots.setLogicalLength(nCounter2);
      for (k = 0; k < nCounter2; ++k) // knots ????
      {
        knots[k] = pFiler->rdDouble();
      }

      m_Loops2d[i][j].NurbCurve2d.set(Degree-1, knots, controlPoints, weights);
    }
  }

  return eOk;
}

OdResult OdAsSurfBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();

  if (pFiler->dwgVersion() < OdDb::vAC15)
    return eMakeMeProxy;

  OdResult res = OdDbBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  clear();

  m_nFormatVersion = pFiler->rdInt16(); ODA_ASSERT(m_nFormatVersion == 1 || m_nFormatVersion == 2);
  //ODA_ASSERT(m_nFormatVersion == 2); // is it 70 = 0 (m_nFormatVersion == 2)
  if (m_nFormatVersion != 2)
    return eMakeMeProxy;

  m_dNormalLength = pFiler->rdDouble(); // Normal Length  //dxf 40
  m_LinesCountU   = pFiler->rdInt16();  // Lines Count U  //dxf 71
  m_LinesCountV   = pFiler->rdInt16();  // Lines Count V  //dxf 72

  // read sub surface
  res = dwgInAsSurface(pFiler);
  if (res != eOk)
    return res;

  m_bHasSubNurb = pFiler->rdBool();
  if (m_bHasSubNurb)
  {
    res = dwgInSubNurbSurface(pFiler);
    if (res != eOk)
      return res;

    res = dwgInTrimmingLoops(pFiler);
    if (res != eOk)
      return res;
  }

  return eOk;
}

void OdAsSurfBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();

  OdDbBody::dwgOutFields(pFiler);

  pFiler->wrInt16 ((OdInt16)m_nFormatVersion);
  pFiler->wrDouble(m_dNormalLength );
  pFiler->wrInt16 (m_LinesCountU   );
  pFiler->wrInt16 (m_LinesCountV   );

  // write sub surface
  dwgOutAsSurface(pFiler);

  pFiler->wrBool(m_bHasSubNurb);
  if (m_bHasSubNurb)
  {
    dwgOutSubNurbSurface(pFiler);
    dwgOutTrimmingLoops(pFiler);
  }
}

OdResult OdAsSurfBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbBody::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  if (!pFiler->atSubclassData(desc()->name()))
  {
    ODA_ASSERT_ONCE(pFiler->dwgVersion() <= OdDb::vAC21); // TODO
    return eBadDxfSequence;
  }

  clear();

  int   nCode = 0;
  while (!pFiler->atEOF() && nCode != -1)
  {
    nCode = pFiler->nextItem();
    switch (nCode)
    {
      case 70:
      {
        OdInt16 tmp = pFiler->rdInt16(); // TODO = 1,2 ??? in dxf always = 0
        ODA_ASSERT(tmp == 0);
        if (tmp != 0)
          return eMakeMeProxy;
        break;
      }
      dxfRead1(40, m_dNormalLength, rdDouble); // Normal Length  //dxf 40
      dxfRead1(71, m_LinesCountU  , rdInt16 ); // Lines Count U  //dxf 71
      case 72:
      {
        m_LinesCountV = pFiler->rdInt16();     // Lines Count V  //dxf 72
        nCode = -1; //stop loading
        break;
      }
      default:
        return eMakeMeProxy;
    }
  }

  nCode = pFiler->nextItem();
  if (nCode != 72 && nCode != 73)
    return eMakeMeProxy;
  
  OdAsSurface::AsSurfType surfType = (OdAsSurface::AsSurfType)pFiler->rdInt16();
  m_pAsSurface = getAsSurfaceByType(surfType);
  if (!m_pAsSurface)
    return eMakeMeProxy;

  res = m_pAsSurface->dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return eOk;
}

static OdUInt32 getClassId(OdString className, OdDbDatabase &pDb)
{
  OdRxDictionaryIteratorPtr pIter = odDbGetDrawingClasses(pDb);
      
  while(!pIter->done())
  {
    if (pIter->getKey().iCompare(className) == 0)
    {
      return pIter->id();
    }
    pIter->next();
  }

  ODA_FAIL();
  return 0;
}

OdResBufPtr OdAsSurfBody::prepareResBuf4DxfOut(OdDbDxfFiler* pFiler, OdResBufPtr pResBuf) const
{
  if (pResBuf.isNull())
    return pResBuf;

  OdResBufPtr pCurRes = pResBuf;
  
  int stop = 0;
  const int stopAtPos = 3;

  while (stop < stopAtPos && !pCurRes.isNull())
  {
    if (pCurRes->restype() == 5 && pCurRes->getHandle().ascii() == OD_T("0"))
    {
      OdDbHandle h = handle();
      pCurRes->setHandle(h);
      ++stop;
    }
    else if (pCurRes->restype() == 1)
    {
      OdString className = pCurRes->getString();
      pCurRes->setRestype(91);
      pCurRes->setInt32( getClassId(className, *pFiler->database()) + 500);
      
      ++stop;
    }
    else if (pCurRes->restype() == 330 && pCurRes->getHandle().ascii() == OD_T("0"))
    {
      OdDbObjectId ownerID = ownerId();
      const OdDbHandle &h = ownerID.getHandle();
      pCurRes->setHandle(h);

      OdDbObjectId PaperId = database()->getPaperSpaceId();
      OdDbObjectId ModelId = database()->getModelSpaceId();

      if (ownerID == PaperId)
      {
        OdResBufPtr pNewRB = OdResBuf::newRb(67, OdInt16(1));
        pCurRes->insert(pNewRB);
      }
      else
      {
        for(OdDbObjectId ownerId = OdDbEntity::ownerId(); ; )
        {
          if (ownerId==PaperId)
          {
            OdResBufPtr pNewRB = OdResBuf::newRb(67, OdInt16(1));
            pCurRes->insert(pNewRB);
            break;
          }
          else if (ownerId==ModelId)
          {
            break;
          }
          OdDbObjectPtr pResObj = ownerId.openObject(OdDb::kForRead);
          if (!pResObj.isNull())
          {
            ownerId = pResObj->ownerId();
          }
          else
          {
            break;
          }
        };
      }

      ++stop;
    }

    pCurRes = pCurRes->next();
  }
  ODA_ASSERT(stop >= stopAtPos);


  return pResBuf;
}

OdDbObjectPtr OdAsSurfBody::decomposeForSave( OdDb::DwgVersion ver,
                                              OdDbObjectId& replaceId,
                                              bool& exchangeXData)
{
  exchangeXData = false;
  replaceId = NULL;
  
  //MDT has been discontinued after 2009. That object will be opened and saved as proxy object.

  //There is no AsSurfBody enablers for version > 2009.
  //The enabler for AsSurfBody prohibits to resave or modify file.
  if (ver <= OdDb::vAC12)
  {
    return OdDbBody::decomposeForSave(ver, replaceId, exchangeXData);
  } 
  else if (ver <= OdDb::vAC14 || ver > OdDb::vAC21) 
  {
    OdDbEntityPtr pEnt(this);
    // Note: starting with R27 ACIS data goes to DS section
    OdDbProxyEntityPtr pPrObject = odEntityToProxy(*pEnt, OdDb::vAC21,OdDb::kMRelease25);
    return pPrObject;
  }

  // Ok. Yes. We have dwgOutFields implemented, but there is no information about "saveTo" file type here.
  // We cannot separate .dwg or .dxf here.
  // So it is better to remove "old" dxf to proxy code from dxfOut and save entity as proxy for both .dwg and .dxf.
  OdDbEntityPtr pEnt(this);
  OdDbProxyEntityPtr pPrObject = odEntityToProxy(*pEnt, ver, OdDb::kMRelease0);
  return pPrObject;

}

void OdAsSurfBody::dxfOut(OdDbDxfFiler* pFiler) const
{
  // should newer be here. Acad save this entity with degenerate dxf output. We will save it as proxy.
  throw OdError(eNotImplemented);

  assertReadEnabled();

  bool bMakeProxy = true;
  if (!isNull())
  {
    OdBrBrep br;
    ((OdAsSurfBody*)this)->brep(br);
    if (!br.isNull())
    {
      OdBrBrepFaceTraverser bft;
      bMakeProxy = bft.setBrep(br) != odbrOK;
    }
  }

  if (bMakeProxy)
  {
    // Create proxy from this with null object handle and null owner (non DBRO)
    OdDbEntityPtr pEnt(this);
    OdDbProxyEntityPtr pPrObject = odEntityToProxy(*pEnt);

    OdResBufPtr pPrevResBuf = ::oddbEntGet( pPrObject );

    // set proxy handle to this->handle and owner to this->owner
    OdResBufPtr pResBuf = prepareResBuf4DxfOut(pFiler, pPrevResBuf );
    if (!pResBuf.isNull())
    {
      pFiler->writeRb(pResBuf);
    }
  }
  else
  {
    OdDbBody::dxfOut(pFiler);
  }
}

void OdAsSurfBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  // here should be conversion from subNurb to sat.
  OdDbBody::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());

  pFiler->wrInt16 (70, 0              ); // TODO : why = 0 in dxf ?
  pFiler->wrDouble(40, m_dNormalLength);
  pFiler->wrInt16 (71, m_LinesCountU  );
  pFiler->wrInt16 (72, m_LinesCountV  );

  m_pAsSurface->dxfOutFields(pFiler);
}

bool OdAsSurfBody::subWorldDraw(OdGiWorldDraw* pWd) const
{
  if (!m_bHasSubNurb)
    return OdDbBody::subWorldDraw(pWd);

  // TODO: draw normal with m_NormalLenght.

  // draw trimming loops
  OdGePoint2dArray pointArray;
  OdGePoint3dArray pointArray3d;
  OdUInt16 l, s, p;
  for(l = 0; l < m_Loops2d.size(); ++l)
  {
    for(s = 0; s < m_Loops2d[l].size(); ++s)
    {
      const OdGeNurbCurve2d &tmp = m_Loops2d[l][s].NurbCurve2d;
      tmp.getSamplePoints(40, pointArray);

      int n = pointArray.size();
      pointArray3d.resize(n);

      for(p=0; p<n; ++p)
      {
        pointArray3d[p] = m_NURBSurf.evalPoint(pointArray[p]);
      }
      pWd->geometry().polyline(n, pointArray3d.asArrayPtr());
    }
  }

  // draw isolines
/*
  OdGeInterval intrvlX, intrvlY;
  m_NURBSurf.getEnvelope(intrvlX, intrvlY);

    if (m_LinesCountU > 0)
    {
      OdGeNurbCurve3d nurb3d;
  
      double dU = double(intrvlX.length() / (m_LinesCountU + 1));
  
      double u = intrvlX.lowerBound() + dU;
      while (u < intrvlX.upperBound())
      {
        m_NURBSurf.computeUIsoLine(u, nurb3d);
        pWd->geometry().nurbs(nurb3d);
        u += dU;
      }
    }
    
    if (m_LinesCountV > 0)
    {
      OdGeNurbCurve3d nurb3d;
  
      double dV = double(intrvlY.length() / (m_LinesCountV + 1));
  
      double v = intrvlY.lowerBound() + dV;
      while(v < intrvlY.upperBound())
      {
        m_NURBSurf.computeVIsoLine(v, nurb3d);
        pWd->geometry().nurbs(nurb3d);
        v += dV;
      }
    }*/
  

  return true;
}

double  OdAsSurfBody::getNormalLength() const
{
  return m_dNormalLength;
}

OdInt16 OdAsSurfBody::getLinesCountU() const
{
  return m_LinesCountU;
}

OdInt16 OdAsSurfBody::getLinesCountV() const
{
  return m_LinesCountV;
}

bool    OdAsSurfBody::getSubNurbSurface(OdGeNurbSurface &nurbs) const
{
  if (!m_bHasSubNurb)
    return false;

  nurbs = m_NURBSurf;
  return true;
}

bool    OdAsSurfBody::getTrimmingLoops(AsLoop2dInfo &Loops2d) const
{
  if (!m_bHasSubNurb)
    return false;

  Loops2d = m_Loops2d;
  return true;
}

OdResult OdAsSurfBody::acisIn(OdStreamBuf* , AfTypeVer *)
{
  return eUnsupportedMethod;
}
#undef dxfRead1
#undef dxfRead2

