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

#include "OdaCommon.h"

#include "OdConstrainedGeometry.h"
#include "OdRadiusDiameterConstraintImpl.h"
#include "DbFiler.h"

#include "DbDimension.h"
#include "DbDiametricDimension.h"
#include "DbRadialDimension.h"

OdRadiusDiameterConstraintImpl::OdRadiusDiameterConstraintImpl():OdExplicitConstraintImpl(),
                                                                 m_type(OdRadiusDiameterConstraint::kCircleRadius)
{
}

OdRadiusDiameterConstraintImpl::OdRadiusDiameterConstraintImpl(
                        OdRadiusDiameterConstraint::RadiusDiameterConstrType type):
                              OdExplicitConstraintImpl(),
                              m_type(type)
{
}

OdRadiusDiameterConstraint::RadiusDiameterConstrType OdRadiusDiameterConstraintImpl::constrType() const
{
  return m_type;
}

OdResult OdRadiusDiameterConstraintImpl::setConstrType(OdRadiusDiameterConstraint::RadiusDiameterConstrType type)
{
  m_type = type;
  return eOk;
}

OdResult OdRadiusDiameterConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;
   
  m_type = static_cast<OdRadiusDiameterConstraint::RadiusDiameterConstrType>(pFiler->rdUInt8());

  return res;
}

void OdRadiusDiameterConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdExplicitConstraintImpl::dwgOutFields(pFiler);
  pFiler->wrUInt8((OdUInt8)m_type);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdRadiusDiameterConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(280)
  m_type = static_cast<OdRadiusDiameterConstraint::RadiusDiameterConstrType>(pFiler->rdUInt8());
  return res;
}

void OdRadiusDiameterConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdExplicitConstraintImpl::dxfOutFields(pFiler);
  pFiler->wrUInt8(280, (OdUInt8)m_type);
}

void OdRadiusDiameterConstraintImpl::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  OdDbObjectId id = dimDependencyId();
  if ( id.isNull() )
    return;

  OdArray<OdConstrainedGeometry*> arrGeom;
  getConnectedGeometries(arrGeom);
  
  ODA_ASSERT(arrGeom.length() == 1);

  OdDbAssocDependencyPtr assocDepPtr = id.openObject();
  id = assocDepPtr->dependentOnObject();
  if ( id.isNull() )
    return;

  OdDbDimensionPtr dimPtr = id.openObject(OdDb::kForWrite);
  
  OdGeMatrix3d matrToPlane = matrToWrd.inverse();
  
  if ( constrType() == OdRadiusDiameterConstraint::kCircleRadius )
  {
    OdDbRadialDimensionPtr radDimPtr = OdDbRadialDimension::cast(dimPtr);

    OdGePoint3d txtPosPt = radDimPtr->textPosition();
    OdGePoint3d chordPt = radDimPtr->chordPoint();
    OdGePoint3d centerPt  = radDimPtr->center();

    txtPosPt.transformBy(matrToPlane);
    chordPt.transformBy(matrToPlane);
    centerPt.transformBy(matrToPlane);

    OdGeVector3d dirVec = chordPt - centerPt;
    dirVec.normalize();

    OdGeVector3d txtVec = txtPosPt - centerPt;

    if ( arrGeom[0]->isKindOf(OdConstrainedArc::desc()) )
    {
      OdSmartPtr<OdConstrainedArc> arcPtr = OdConstrainedArc::cast(arrGeom[0]);
      double rad = arcPtr->radius();
      dirVec *= rad;

      centerPt = arcPtr->centerPoint();
      chordPt = centerPt + dirVec;

      OdGePoint3d midPt = arcPtr->midPoint();
      OdGePoint3d sPt = arcPtr->startPoint();
      OdGePoint3d ePt = arcPtr->endPoint();

      OdGeVector3d seVec = ePt - sPt;
      OdGeVector3d smVec = midPt - sPt;
      OdGeVector3d schVec = chordPt - sPt;

      OdGeVector3d crossPrVec1 = seVec.crossProduct(smVec);
      OdGeVector3d crossPrVec2 = seVec.crossProduct(schVec);

      OdGeCircArc3d arc = arcPtr->getOdGeCircArc3d();

      OdGeVector3d chSVec;
      OdGeVector3d chEVec;
      if ( !crossPrVec1.isCodirectionalTo(crossPrVec2) )
      {
        chSVec = chordPt - sPt;
        chEVec = chordPt - ePt;
        if ( chSVec.length() <= chEVec.length() )
          chordPt = sPt;
        else
          chordPt = ePt;
      }

      double sAng = arc.paramOf(sPt);
      double eAng = arc.paramOf(ePt);
      while ( eAng < sAng )
        eAng += Oda2PI;

      double chAng = arc.paramOf(chordPt);
      while ( chAng < sAng )
        chAng += Oda2PI;

      bool lenLes40 = eAng - sAng < 4.0*OdaPI/18.0;
      double diffECHAng = eAng - chAng;
      double diffCHSAng = chAng - sAng;

      double baseDiffAng, baseAng;
      int sign;
      double ang;

      if ( diffECHAng >= diffCHSAng )
      {
        baseAng = sAng;
        baseDiffAng = diffCHSAng;
        sign = 1;
      }
      else
      {
        baseAng = eAng;
        baseDiffAng = diffECHAng;
        sign = -1;
      }

      if ( lenLes40 )
      {
        if ( eAng - sAng <= OdaPI/90 )
        {
          ang = (sAng + eAng)/2.0;
        }
        else if ( baseDiffAng < OdaPI/90 )
        {
          ang = OdaPI/90;
        }
        else
          ang = baseDiffAng;
      }
      else
      {
          if ( baseDiffAng < 4.0*OdaPI/18.0 )
          {
            ang = 4.0*OdaPI/18.0;
          }
          else
            ang = baseDiffAng;
      }
      
      chordPt = arc.evalPoint(baseAng + ang*sign);

      txtPosPt = centerPt + txtVec;

      txtPosPt.transformBy(matrToWrd);
      centerPt.transformBy(matrToWrd);
      chordPt.transformBy(matrToWrd);
    }
    else if ( arrGeom[0]->isKindOf(OdConstrainedCircle::desc()) )
    {
      OdSmartPtr<OdConstrainedCircle> circlePtr = OdConstrainedCircle::cast(arrGeom[0]);
      double rad = circlePtr->radius();
      dirVec *= rad;

      centerPt = circlePtr->centerPoint();
      centerPt.transformBy(matrToWrd);

      chordPt = centerPt + dirVec;
  
      txtPosPt = centerPt + txtVec;
    }
    else
      ODA_FAIL_ONCE();

    radDimPtr->setTextPosition(txtPosPt);
    radDimPtr->setChordPoint(chordPt);
    radDimPtr->setCenter(centerPt);
  }
  else if ( constrType() == OdRadiusDiameterConstraint::kCircleDiameter )
  {
    OdDbDiametricDimensionPtr diamDimPtr = OdDbDiametricDimension::cast(dimPtr);

    OdGePoint3d txtPosPt = diamDimPtr->textPosition();
    OdGePoint3d chordPt = diamDimPtr->chordPoint();
    OdGePoint3d farChordPt = diamDimPtr->farChordPoint();

    txtPosPt.transformBy(matrToPlane);
    chordPt.transformBy(matrToPlane);
    farChordPt.transformBy(matrToPlane);

    OdGePoint3d centerPt((chordPt.x + farChordPt.x)/2.0, (chordPt.y + farChordPt.y)/2.0, (chordPt.z + farChordPt.z)/2.0);

    OdGeVector3d dirVec = chordPt - farChordPt;
    dirVec.normalize();

    OdGeVector3d txtVec = txtPosPt - centerPt;
    if ( arrGeom[0]->isKindOf(OdConstrainedArc::desc()) )
    {
      OdSmartPtr<OdConstrainedArc> arcPtr = OdConstrainedArc::cast(arrGeom[0]);
      double rad = arcPtr ->radius();
      dirVec *= rad;

      centerPt = arcPtr->centerPoint();

      chordPt = centerPt + dirVec;
      farChordPt = centerPt - dirVec;
  
      txtPosPt = centerPt + txtVec;

      OdGeCircArc3d arc = arcPtr->getOdGeCircArc3d();

      double sAng = arc.paramOf(arcPtr->startPoint());
      double eAng = arc.paramOf(arcPtr->endPoint());
      diamDimPtr->setExtArcStartAngle(sAng);
      diamDimPtr->setExtArcEndAngle(eAng);

      txtPosPt.transformBy(matrToWrd);
      chordPt.transformBy(matrToWrd);
      farChordPt.transformBy(matrToWrd);
    }
    else if ( arrGeom[0]->isKindOf(OdConstrainedCircle::desc()) )
    {
      OdSmartPtr<OdConstrainedCircle> circlePtr = OdConstrainedCircle::cast(arrGeom[0]);
      double rad = circlePtr->radius();
      dirVec *= rad;

      centerPt = circlePtr->centerPoint();
      centerPt.transformBy(matrToWrd);

      chordPt = centerPt + dirVec;
      farChordPt = centerPt - dirVec;
  
      txtPosPt = centerPt + txtVec;
    }
    else
      ODA_FAIL_ONCE();

    diamDimPtr->setTextPosition(txtPosPt);
    diamDimPtr->setChordPoint(chordPt);
    diamDimPtr->setFarChordPoint(farChordPt);
  }
}
