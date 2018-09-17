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

#include <OdaCommon.h>
#include "DgnImportDimAssoc.h"
#include <DbLine.h>
#include <DbPolyline.h>
#include <DbRegion.h>
#include <DbSpline.h>
#include <DbArc.h>
#include <DbCircle.h>
#include <DbEllipse.h>
#include <DbSpline.h>
#include <DbMline.h>
#include <DbText.h>
#include <DbMText.h>
#include <DbBlockReference.h>
#include <DbRotatedDimension.h>
#include <Db3PointAngularDimension.h>
#include <DbArcDimension.h>
#include <DbRadialDimension.h>
#include <DbDiametricDimension.h>
#include <DbOrdinateDimension.h>
#include "DgArc.h"
#include "DgEllipse.h"

using namespace TD_DGN_IMPORT;

namespace TD_DGN_IMPORT 
{

//-----------------------------------------------------------------------------------------------------------

  bool addLinearPointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                    const OdDgLinearAssociationPtr& pDgnAssoc, 
                                      const OdDbDimAssoc::PointType& ptType,
                                        OdDgDatabase* pDg,
                                          OdArray<OdDbObjectPtr>& arrObjects
                                )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnLinearElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnLinearElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnLinearElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    double dParam = pDgnAssoc->getNumerator() * 1.0 / pDgnAssoc->getDivisor();

    OdDb::OsnapMode curMode = OdDb::kOsModeNear;

    if( OdZero(dParam - 0.5, 1e-6) )
    {
      curMode = OdDb::kOsModeMid;
    }
    else if( OdZero(dParam) )
    {
      curMode = OdDb::kOsModeStart;
    }
    else if( OdZero(dParam - 1.0) )
    {
      curMode = OdDb::kOsModeEnd;
    }

    if( pDbEnt->isKindOf(OdDbLine::desc()) )
    {
      OdDbLinePtr pLine = pDbEnt;

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(pLine->startPoint());
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());
      pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }
    else if( pDbEnt->isKindOf(OdDbPolyline::desc()))
    {
      OdDbPolylinePtr pPolyline = pDbEnt;

      OdUInt32 uStartIndex = pDgnAssoc->getVertexNumber();

      if( !idPath.m_bExists )
      {
        uStartIndex += idPath.m_uStartPt;
      }

      OdGePoint3d ptVertex;
      pPolyline->getPointAt(uStartIndex, ptVertex);

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptVertex);
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());
      pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
      pointRef->mainEntity().subentId().setIndex( uStartIndex + 1 );

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

//-----------------------------------------------------------------------------------------------------------

  bool addProjectrionPointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                         const OdDgProjectionAssociationPtr& pDgnAssoc, 
                                           const OdDbDimAssoc::PointType& ptType,
                                             OdDgDatabase* pDg,
                                               OdArray<OdDbObjectPtr>& arrObjects
                                )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnLinearElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnLinearElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnLinearElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    double dParam = pDgnAssoc->getFraction();

    OdDb::OsnapMode curMode = OdDb::kOsModeNear;

    if( OdZero(dParam - 0.5, 1e-6) )
    {
      curMode = OdDb::kOsModeMid;
    }
    else if( OdZero(dParam) )
    {
      curMode = OdDb::kOsModeStart;
    }
    else if( OdZero(dParam - 1.0) )
    {
      curMode = OdDb::kOsModeEnd;
    }

    if( pDbEnt->isKindOf(OdDbLine::desc()) )
    {
      OdDbLinePtr pLine = pDbEnt;

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(pLine->startPoint());
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());
      pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }
    else if( pDbEnt->isKindOf(OdDbPolyline::desc()))
    {
      OdDbPolylinePtr pPolyline = pDbEnt;

      OdUInt32 uStartIndex = pDgnAssoc->getVertexNumber();

      if( !idPath.m_bExists )
      {
        uStartIndex += idPath.m_uStartPt;
      }

      OdGePoint3d ptStartVertex;
      pPolyline->getPointAt(uStartIndex, ptStartVertex);

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptStartVertex);
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());
      pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
      pointRef->mainEntity().subentId().setIndex( uStartIndex + 1 );

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool addArcPointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                 const OdDgArcAssociationPtr& pDgnAssoc, 
                                   const OdDbDimAssoc::PointType& ptType,
                                     OdDgDatabase* pDg,
                                       OdArray<OdDbObjectPtr>& arrObjects
                              )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnArcElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnArcElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnArcElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    OdDgArcAssociation::OdDgArcAssociationKeyPoint uKey = pDgnAssoc->getKeyPoint();
    double dAngle = pDgnAssoc->getAngle();

    bool bSnapPointDefined = false;

    OdGePoint3d ptDgnAnglePt;

    OdDgElementPtr pElm = idDgnArcElm.openObject(OdDg::kForRead);

    double        dSweepAngle = Oda2PI;
    double        dStartAngle = 0;
    OdGePoint3d   ptCenter;
    OdGeVector3d  vrMajor3d;
    OdGeVector3d  vrMinor3d;
    OdGeVector3d  vrNormal;

    bool bInitData = false;

    if( pElm->isKindOf( OdDgArc3d::desc()) )
    {
      OdDgArc3dPtr pArc = pElm;

      dSweepAngle = pArc->getSweepAngle();
      dStartAngle = pArc->getStartAngle();

      ptCenter  = pArc->getOrigin();
      vrMajor3d = OdGeVector3d::kXAxis;
      vrMinor3d = OdGeVector3d::kYAxis;
      vrMajor3d.transformBy( pArc->getRotation().getMatrix() );
      vrMinor3d.transformBy( pArc->getRotation().getMatrix() );

      vrMajor3d.normalize();
      vrMajor3d.normalize();

      vrMajor3d *= pArc->getPrimaryAxis();
      vrMinor3d *= pArc->getSecondaryAxis();
      vrNormal = pArc->getNormal();

      bInitData = true;
    }
    else if( pElm->isKindOf( OdDgArc2d::desc()) )
    {
      OdDgArc2dPtr pArc = pElm;

      dSweepAngle = pArc->getSweepAngle();
      dStartAngle = pArc->getStartAngle();

      OdGePoint2d ptCenter2d  = pArc->getOrigin();
      ptCenter.set( ptCenter2d.x, ptCenter2d.y, 0 );

      OdGeVector2d vrMajor2d = OdGeVector2d::kXAxis;
      OdGeVector2d vrMinor2d = OdGeVector2d::kYAxis;
      vrMajor2d.rotateBy( pArc->getRotationAngle() );
      vrMinor2d.rotateBy( pArc->getRotationAngle() );

      vrMajor2d *= pArc->getPrimaryAxis();
      vrMinor2d *= pArc->getSecondaryAxis();

      vrMajor3d.set( vrMajor2d.x, vrMajor2d.y, 0 );
      vrMinor3d.set( vrMinor2d.x, vrMinor2d.y, 0 );

      vrNormal = OdGeVector3d::kZAxis;

      bInitData = true;
    }
    else if( pElm->isKindOf( OdDgEllipse3d::desc()) )
    {
      OdDgEllipse3dPtr pEllipse = pElm;

      dSweepAngle = Oda2PI;
      dStartAngle = 0;

      pEllipse->getOrigin( ptCenter );
      vrMajor3d = OdGeVector3d::kXAxis;
      vrMinor3d = OdGeVector3d::kYAxis;
      vrMajor3d.transformBy( pEllipse->getRotation().getMatrix() );
      vrMinor3d.transformBy( pEllipse->getRotation().getMatrix() );

      vrMajor3d.normalize();
      vrMajor3d.normalize();

      vrMajor3d *= pEllipse->getPrimaryAxis();
      vrMinor3d *= pEllipse->getSecondaryAxis();

      vrNormal = OdGeVector3d::kZAxis;
      
      vrNormal.transformBy(pEllipse->getRotation().getMatrix());
      vrNormal.normalize();

      bInitData = true;
    }
    else if( pElm->isKindOf( OdDgEllipse2d::desc()) )
    {
      OdDgEllipse2dPtr pEllipse = pElm;

      dSweepAngle = Oda2PI;
      dStartAngle = 0;

      OdGePoint2d ptCenter2d  = pEllipse->getOrigin();
      ptCenter.set( ptCenter2d.x, ptCenter2d.y, 0 );

      OdGeVector2d vrMajor2d = OdGeVector2d::kXAxis;
      OdGeVector2d vrMinor2d = OdGeVector2d::kYAxis;
      vrMajor2d.rotateBy( pEllipse->getRotationAngle() );
      vrMinor2d.rotateBy( pEllipse->getRotationAngle() );

      vrMajor2d *= pEllipse->getPrimaryAxis();
      vrMinor2d *= pEllipse->getSecondaryAxis();

      vrMajor3d.set( vrMajor2d.x, vrMajor2d.y, 0 );
      vrMinor3d.set( vrMinor2d.x, vrMinor2d.y, 0 );

      vrNormal = OdGeVector3d::kZAxis;

      bInitData = true;
    }

    if( bInitData )
    {
      if( uKey == OdDgArcAssociation::kStartPoint )
      {
        ptDgnAnglePt = ptCenter + vrMajor3d * cos(dStartAngle) + vrMinor3d * sin(dStartAngle);
        bSnapPointDefined = true;
      }
      else if( uKey == OdDgArcAssociation::kEndPoint )
      {
        ptDgnAnglePt = ptCenter + vrMajor3d * cos(dStartAngle + dSweepAngle) + vrMinor3d * sin(dStartAngle + dSweepAngle);
        bSnapPointDefined = true;
      }
      else if( uKey == OdDgArcAssociation::kCenterPoint )
      {
        ptDgnAnglePt = ptCenter;
        bSnapPointDefined = true;
      }
      else // uKey == OdDgArcAssociation::kAnglePoint
      {
        ptDgnAnglePt = ptCenter + vrMajor3d * cos(dAngle) + vrMinor3d * sin(dAngle);
        bSnapPointDefined = true;
      }
    }

    if( bSnapPointDefined )
    {
      double dStartParam = 0;
      double dEndParam   = Oda2PI;
      double dSnapParam  = 0;
      bool   bPolyline   = false;
      OdUInt32 uArcSegmentIndex = 0;
      OdResult resStartParam = eInvalidIndex;
      OdResult resEndParam = eInvalidIndex;
      OdResult resAngleParam = eInvalidIndex;
      bool bForbidMiddleSnap = false;

      if( curPath.size() > 1 )
      {
        OdGeMatrix3d matTransform;

        for( OdUInt32 k = curPath.size() - 1; k > 0; k-- )
        {
          OdDbObjectId idBlockRef = curPath[k-1];

          OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast( idBlockRef.openObject(OdDb::kForRead));

          if( !pBlockRef.isNull() )
          {
            matTransform = matTransform * pBlockRef->blockTransform();
          }
        }

        OdGeMatrix3d matTransformInv = matTransform;
        matTransformInv = matTransformInv.invert();

        ptDgnAnglePt.transformBy( matTransformInv );
      }

      if( pDbEnt->isKindOf(OdDbCircle::desc()) )
      {
        OdDbCirclePtr pCircle = pDbEnt;

        if( uKey != OdDgArcAssociation::kCenterPoint )
        {
          resStartParam = eOk;
          resEndParam   = eOk;
          resAngleParam = pCircle->getParamAtPoint( ptDgnAnglePt, dSnapParam );
          bForbidMiddleSnap = true;
        }
      }
      else if( pDbEnt->isKindOf(OdDbEllipse::desc()) )
      {
        OdDbEllipsePtr pEllipse = pDbEnt;

        if( uKey != OdDgArcAssociation::kCenterPoint )
        {
          OdGePoint3d ptStart;
          OdGePoint3d ptEnd;
          pEllipse->getStartPoint( ptStart );
          pEllipse->getEndPoint( ptEnd );

          resStartParam = pEllipse->getParamAtPoint( ptStart, dStartParam );
          resEndParam   = pEllipse->getParamAtPoint( ptEnd, dEndParam );
          resAngleParam = pEllipse->getParamAtPoint( ptDgnAnglePt, dSnapParam );

          if( OdZero(dEndParam - dStartParam) )
          {
            dEndParam += Oda2PI;
            bForbidMiddleSnap = true;
          }
        }
      }
      else if( pDbEnt->isKindOf(OdDbArc::desc()) )
      {
        OdDbArcPtr pArc = pDbEnt;

        if( uKey != OdDgArcAssociation::kCenterPoint )
        {
          OdGePoint3d ptStart;
          OdGePoint3d ptEnd;
          pArc->getStartPoint( ptStart );
          pArc->getEndPoint( ptEnd );

          resStartParam = pArc->getParamAtPoint( ptStart, dStartParam );
          resEndParam   = pArc->getParamAtPoint( ptEnd, dEndParam );
          resAngleParam = pArc->getParamAtPoint( ptDgnAnglePt, dSnapParam );

          if( OdZero(dEndParam - dStartParam) )
          {
            dEndParam += Oda2PI;
            bForbidMiddleSnap = true;
          }
        }
      }
      else if( pDbEnt->isKindOf(OdDbPolyline::desc()) )
      {
        bPolyline = true;

        OdDbPolylinePtr pPolyline = pDbEnt;

        uArcSegmentIndex = 0;

        if( !idPath.m_bExists )
        {
          uArcSegmentIndex += idPath.m_uStartPt;
        }

        resStartParam = eInvalidIndex;
        resEndParam   = eInvalidIndex;

        if( uArcSegmentIndex < pPolyline->numVerts() )
        {
          double dBulge = pPolyline->getBulgeAt( uArcSegmentIndex );

          if( OdZero(dBulge) )
          {
            uKey = OdDgArcAssociation::kAnglePoint;
          }
          else if( uKey != OdDgArcAssociation::kCenterPoint )
          {
            if( uArcSegmentIndex < pPolyline->numVerts() - 1 )
            {
              OdDbFullSubentPathArray subEntPath;
              OdDbObjectIdArray tmpArr;

              pPolyline->subGetSubentPathsAtGsMarker( OdDb::kEdgeSubentType, uArcSegmentIndex + 1,  
                                                      ptDgnAnglePt, OdGeMatrix3d::kIdentity, subEntPath, &tmpArr );

              OdDbEntityPtr pEnt = pPolyline->subentPtr( subEntPath[0] );

              if( pEnt->isKindOf(OdDbArc::desc()) )
              {
                OdDbArcPtr pArc = pEnt;

                OdGePoint3d ptStart;
                OdGePoint3d ptEnd;
                pArc->getStartPoint( ptStart );
                pArc->getEndPoint( ptEnd );

                resStartParam = pArc->getParamAtPoint( ptStart, dStartParam );
                resEndParam   = pArc->getParamAtPoint( ptEnd, dEndParam );
                resAngleParam = pArc->getParamAtPoint( ptDgnAnglePt, dSnapParam );

                if( OdZero(dEndParam - dStartParam) )
                {
                  dEndParam += Oda2PI;
                  bForbidMiddleSnap = true;
                }
              }
              else if( pEnt->isKindOf(OdDbCircle::desc()))
              {
                OdDbArcPtr pCircle = pEnt;

                resStartParam = eOk;
                resEndParam   = eOk;
                resAngleParam = pCircle->getParamAtPoint( ptDgnAnglePt, dSnapParam );
                bForbidMiddleSnap = true;
              }
            }
          }
        }
      }

      if( (uKey == OdDgArcAssociation::kCenterPoint) || ( (resStartParam == eOk) && (resEndParam == eOk) && (resAngleParam == eOk) ) )
      {
        OdDb::OsnapMode curMode = OdDb::kOsModeNear;

        if( uKey == OdDgArcAssociation::kCenterPoint )
        {
          curMode = OdDb::kOsModeCen;
        }
        else
        {
          double dParam = (dSnapParam - dStartParam) / ( dEndParam - dStartParam );

          if( OdZero(dParam, 1e-6) )
          {
            curMode = OdDb::kOsModeStart;
          }
          else if( OdZero(dParam - 1.0, 1e-6) )
          {
            curMode = OdDb::kOsModeEnd;
          }
          else if( !bForbidMiddleSnap && OdZero(dParam - 0.5, 1e-6) )
          {
            curMode = OdDb::kOsModeMid;
          }
        }

        OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
        pointRef->setPoint(ptDgnAnglePt);
        pointRef->setOsnapType(curMode);
        pointRef->setNearPointParam(dSnapParam);

        if( !idDgnSharedCellElm.isNull() )
        {
          pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
        }

        pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());

        if( bPolyline )
        {
          pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
          pointRef->mainEntity().subentId().setIndex( uArcSegmentIndex + 1 );
        }
        else
        {
          pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);
        }

        pDimAssoc->setPointRef(ptType, pointRef);

        OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

        for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
        {
          OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

          if( !arrObjects.contains( pObj ) )
          {
            arrObjects.push_back( pObj );
          }
        }

        bRet = true;
      }
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool addBSplineCurvePointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                          const OdDgBSplineCurveAssociationPtr& pDgnAssoc, 
                                            const OdDbDimAssoc::PointType& ptType, 
                                              OdDgDatabase* pDg, 
                                                OdArray<OdDbObjectPtr>& arrObjects 
                                      )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnLinearElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnLinearElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnLinearElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    double dParam = pDgnAssoc->getParam();

    OdDb::OsnapMode curMode = OdDb::kOsModeNear;

    if( OdZero(dParam - 0.5, 1e-6) )
    {
      curMode = OdDb::kOsModeMid;
    }
    else if( OdZero(dParam) )
    {
      curMode = OdDb::kOsModeStart;
    }
    else if( OdZero(dParam - 1.0) )
    {
      curMode = OdDb::kOsModeEnd;
    }

    OdGePoint3d ptSnap;
    OdUInt32    uPolylineVertex = 0;

    bool bAllowSnap = false;

    if( pDbEnt->isKindOf(OdDbSpline::desc()) )
    {
      OdDbSplinePtr pSpline = pDbEnt;

      double dStartParam = 0;
      double dEndParam = 1;

      pSpline->getStartParam( dStartParam );
      pSpline->getEndParam( dEndParam );

      dParam = dStartParam + (dEndParam - dStartParam) * dParam;

      OdResult res = pSpline->getPointAtParam( dParam, ptSnap );

      if( res == eOk )
      {
        bAllowSnap = true;
      }
    }
    else if( pDbEnt->isKindOf(OdDbPolyline::desc()))
    {
      OdDbPolylinePtr pPolyline = pDbEnt;

      if( !idPath.m_bExists )
      {
        if( curMode == OdDb::kOsModeStart )
        {
          uPolylineVertex += idPath.m_uStartPt;
          pPolyline->getPointAt(uPolylineVertex, ptSnap);

          bAllowSnap = true;
        }
        else if( curMode == OdDb::kOsModeEnd )
        {
          uPolylineVertex += (idPath.m_uEndPt - 1);
          pPolyline->getPointAt(uPolylineVertex + 1, ptSnap);

          bAllowSnap = true;
        }
      }
    }

    if( bAllowSnap )
    {
      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptSnap);
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());

      if( pDbEnt->isKindOf( OdDbPolyline::desc() ) )
      {
        pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
        pointRef->mainEntity().subentId().setIndex( uPolylineVertex + 1 );
      }
      else
      {
        pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);
      }

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool addOriginPointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                    const OdDgOriginAssociationPtr& pDgnAssoc, 
                                      const OdDbDimAssoc::PointType& ptType, 
                                        OdDgDatabase* pDg, 
                                          OdArray<OdDbObjectPtr>& arrObjects 
                                )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnLinearElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnLinearElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnLinearElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    OdDb::OsnapMode curMode = OdDb::kOsModeIns;

    OdGePoint3d ptSnap;

    bool bAllowSnap = false;

    if( pDbEnt->isKindOf(OdDbText::desc()) )
    {
      if( pDgnAssoc->getTextOriginOption() == OdDgOriginAssociation::kInsertionPoint )
      {
        OdDbTextPtr pText = pDbEnt;

        ptSnap = pText->position();

        bAllowSnap = true;
      }
    }
    else if( pDbEnt->isKindOf(OdDbMText::desc()))
    {
      if( pDgnAssoc->getTextOriginOption() == OdDgOriginAssociation::kInsertionPoint )
      {
        OdDgElementPtr pElm = idDgnLinearElm.openObject(OdDg::kForRead);

        if( !pElm.isNull() && pElm->getElementType() == OdDgElement::kTypeText )
        {
          OdDbMTextPtr pMText = pDbEnt;

          ptSnap = pMText->location();

          bAllowSnap = true;
        }
      }
    }
    else if( pDbEnt->isKindOf(OdDbBlockReference::desc()) )
    {
      OdDbBlockReferencePtr pBlockRef = pDbEnt;

      ptSnap  = pBlockRef->position();
      bAllowSnap = true;
    }

    if( bAllowSnap )
    {
      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptSnap);
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(0.0);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());
      pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool addIntersect2PointAssociation( OdDbDimAssocPtr& pDimAssoc, 
                                        const OdDgIntersect2AssociationPtr& pDgnAssoc, 
                                          const OdDbDimAssoc::PointType& ptType, 
                                            OdDgDatabase* pDg,
                                              const OdGePoint3d& ptDim,
                                                OdArray<OdDbObjectPtr>& arrObjects 
                                    )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() )
    {
      return bRet;
    }

    OdDgElementId idDgnElm1 = pDg->getElementId( OdDbHandle(pDgnAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm1;

    if( idDgnElm1.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm1 = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm1.isNull() )
      {
        return bRet;
      }
    }

    OdDgElementId idDgnElm2 = pDg->getElementId( OdDbHandle(pDgnAssoc->getElement2Id()) );
    OdDgElementId idDgnSharedCellElm2;

    if( idDgnElm2.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachment2Id() != 0 )
    {
      idDgnSharedCellElm2 = pDg->getElementId( OdDbHandle(pDgnAssoc->getRefAttachment2Id()) );

      if( idDgnSharedCellElm2.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath1;
    OdDgnImportPathToDwgObject idPath2;
    OdDgnImportPathToDwgObject sharedCellPath1;
    OdDgnImportPathToDwgObject sharedCellPath2;

    if( !OdDgnImportContext::getObjectPath( idDgnElm1, idPath1 ) )
    {
      return bRet;
    }

    if( !OdDgnImportContext::getObjectPath( idDgnElm2, idPath2 ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm1.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm1, sharedCellPath1 ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm2.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm2, sharedCellPath2 ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath1;
    OdDbObjectIdArray curPath2;
    idPath1.m_idPath.objectIds( curPath1 );
    idPath2.m_idPath.objectIds( curPath2 );

    OdDbEntityPtr pDbEnt1;
    OdDbEntityPtr pDbEnt2;

    if( curPath1.size() > 0 && !curPath1[ curPath1.size() - 1 ].isNull() )
    {
      pDbEnt1 = curPath1[ curPath1.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( curPath2.size() > 0 && !curPath2[ curPath2.size() - 1 ].isNull() )
    {
      pDbEnt2 = curPath2[ curPath2.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt1.isNull() || pDbEnt1->isErased() || !pDbEnt1->isKindOf( OdDbCurve::desc() ) )
    {
      return bRet;
    }

    if( pDbEnt2.isNull() || pDbEnt2->isErased() || !pDbEnt2->isKindOf( OdDbCurve::desc() ) )
    {
      return bRet;
    }

    bool bAllowAssoc = true;

    OdUInt32 uPolyline1Index = 0;
    OdUInt32 uPolyline2Index = 0;
    bool     bPolyline1      = false;
    bool     bPolyline2      = false;

    if( pDbEnt1->isKindOf( OdDbPolyline::desc() ) )
    {
      uPolyline1Index = pDgnAssoc->getSegment1();

      if( !idPath1.m_bExists )
      {
        uPolyline1Index += idPath1.m_uStartPt;
      }

      OdDbPolylinePtr pPolyline = pDbEnt1;

      OdDgElementPtr pDgnElm1 = idDgnElm1.openObject( OdDg::kForRead );

      if( (pDgnElm1->getElementType() == OdDgElement::kTypeCurve) || 
          (pDgnElm1->getElementType() == OdDgElement::kTypeBSplineCurve)
        )
      {
        bAllowAssoc = false;
      }
      else if( pDgnElm1->getElementType() == OdDgElement::kTypeArc )
      {
        if( pPolyline->numVerts() > uPolyline1Index )
        {
          if( OdZero( pPolyline->getBulgeAt(uPolyline1Index)) )
          {
            bAllowAssoc = false;
          }
        }
        else
        {
          bAllowAssoc = false;
        }
      }

      if( bAllowAssoc )
      {
        OdDbFullSubentPathArray subEntPath;
        OdDbObjectIdArray tmpArr;

        pPolyline->subGetSubentPathsAtGsMarker( OdDb::kEdgeSubentType, uPolyline1Index + 1,  
          ptDim, OdGeMatrix3d::kIdentity, subEntPath, &tmpArr );

        pDbEnt1 = pPolyline->subentPtr( subEntPath[0] );
      }

      bPolyline1 = true;
    }

    if( pDbEnt2->isKindOf( OdDbPolyline::desc() ) )
    {
      uPolyline2Index = pDgnAssoc->getSegment2();

      if( !idPath2.m_bExists )
      {
        uPolyline2Index += idPath2.m_uStartPt;
      }

      OdDgElementPtr pDgnElm2 = idDgnElm2.openObject( OdDg::kForRead );

      if( (pDgnElm2->getElementType() == OdDgElement::kTypeCurve) || 
          (pDgnElm2->getElementType() == OdDgElement::kTypeBSplineCurve)
        )
      {
        bAllowAssoc = false;
      }
      else if( pDgnElm2->getElementType() == OdDgElement::kTypeArc )
      {
        OdDbPolylinePtr pPolyline = pDbEnt2;

        if( pPolyline->numVerts() > uPolyline2Index )
        {
          if( OdZero( pPolyline->getBulgeAt(uPolyline2Index)) )
          {
            bAllowAssoc = false;
          }
        }
        else
        {
          bAllowAssoc = false;
        }
      }

      bPolyline2 = true;
    }

    if( bAllowAssoc )
    {
      OdGePoint3d ptDgnAnglePt = ptDim;

      if( curPath1.size() > 1 )
      {
        OdGeMatrix3d matTransform;

        for( OdUInt32 k = curPath1.size() - 1; k > 0; k-- )
        {
          OdDbObjectId idBlockRef = curPath1[k-1];

          OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast( idBlockRef.openObject(OdDb::kForRead));

          if( !pBlockRef.isNull() )
          {
            matTransform = matTransform * pBlockRef->blockTransform();
          }
        }

        OdGeMatrix3d matTransformInv = matTransform;
        matTransformInv = matTransformInv.invert();

        ptDgnAnglePt.transformBy( matTransformInv );
      }

      if( !idDgnSharedCellElm1.isNull() && sharedCellPath1.m_idPath.objectIds().size() > 0 )
      {
        OdGeMatrix3d matTransform;

        for( OdInt32 k = sharedCellPath1.m_idPath.objectIds().size() - 1; k >= 0; k-- )
        {
          OdDbObjectId idBlockRef = sharedCellPath1.m_idPath.objectIds()[k];

          OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast( idBlockRef.openObject(OdDb::kForRead));

          if( !pBlockRef.isNull() )
          {
            matTransform = matTransform * pBlockRef->blockTransform();
          }
        }

        OdGeMatrix3d matTransformInv = matTransform;
        matTransformInv = matTransformInv.invert();

        ptDgnAnglePt.transformBy( matTransformInv );
      }

      OdDbCurvePtr pCurve = pDbEnt1;

      double dParam = 0;

      pCurve->getParamAtPoint( ptDgnAnglePt, dParam );

      if( pCurve->isKindOf( OdDbLine::desc() ) )
      {
        OdDbLinePtr pLine = pCurve;

        dParam = dParam / (pLine->startPoint().distanceTo(pLine->endPoint()));
      }

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptDim);
      pointRef->setOsnapType(OdDb::kOsModeIntersec);
      pointRef->setNearPointParam( dParam );

      if( !idDgnSharedCellElm1.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath1.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath1.m_idPath.objectIds());

      if( bPolyline1 )
      {
        pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
        pointRef->mainEntity().subentId().setIndex( uPolyline1Index + 1 );
      }
      else
      {
        pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);
      }

      if( !idDgnSharedCellElm2.isNull() )
      {
        pointRef->intersectEntity().objectIds().append( sharedCellPath2.m_idPath.objectIds() );
      }

      pointRef->intersectEntity().objectIds().append(idPath2.m_idPath.objectIds());

      if( bPolyline2 )
      {
        pointRef->intersectEntity().subentId().setType(OdDb::kEdgeSubentType);
        pointRef->intersectEntity().subentId().setIndex( uPolyline2Index + 1 );
      }
      else
      {
        pointRef->intersectEntity().subentId().setType(OdDb::kVertexSubentType);
      }

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      arrTotalElements = pointRef->intersectEntity().objectIds();

      for( OdUInt32 j = 0; j < arrTotalElements.size(); j++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[j].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool assocDimPointToArc(  OdDbDimAssocPtr& pDimAssoc, 
                              const OdDgAssocPointRootPtr& pDgnAssoc, 
                                const OdDbDimAssoc::PointType& ptType, 
                                  OdDgDatabase* pDg,
                                    const OdGePoint3d& ptDim,
                                      OdArray<OdDbObjectPtr>& arrObjects  
    )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() || pDimAssoc.isNull() || !pDg )
    {
      return bRet;
    }

    if( pDgnAssoc->getType() != OdDgAssocPointRoot::kArcAssociation )
    {
      return bRet;
    }

    OdDgArcAssociationPtr pArcAssoc = pDgnAssoc;

    OdDgElementId idDgnArcElm = pDg->getElementId( OdDbHandle(pArcAssoc->getElementId()) );
    OdDgElementId idDgnSharedCellElm;

    if( idDgnArcElm.isNull() )
    {
      return bRet;
    }

    if( pDgnAssoc->getRefAttachmentId() != 0 )
    {
      idDgnSharedCellElm = pDg->getElementId( OdDbHandle(pArcAssoc->getRefAttachmentId()) );

      if( idDgnSharedCellElm.isNull() )
      {
        return bRet;
      }
    }

    OdDgnImportPathToDwgObject idPath;
    OdDgnImportPathToDwgObject sharedCellPath;

    if( !OdDgnImportContext::getObjectPath( idDgnArcElm, idPath ) )
    {
      return bRet;
    }

    if( !idDgnSharedCellElm.isNull() && !OdDgnImportContext::getObjectPath( idDgnSharedCellElm, sharedCellPath ) )
    {
      return bRet;
    }

    OdDbObjectIdArray curPath;
    idPath.m_idPath.objectIds( curPath );

    OdDbEntityPtr pDbEnt;

    if( curPath.size() > 0 && !curPath[ curPath.size() - 1 ].isNull() )
    {
      pDbEnt = curPath[ curPath.size() - 1 ].openObject(OdDb::kForRead);
    }

    if( pDbEnt.isNull() || pDbEnt->isErased() )
    {
      return bRet;
    }

    OdGePoint3d ptDgnAnglePt = ptDim;

    if( curPath.size() > 1 )
    {
      OdGeMatrix3d matTransform;

      for( OdUInt32 k = curPath.size() - 1; k > 0; k-- )
      {
        OdDbObjectId idBlockRef = curPath[k-1];

        OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast( idBlockRef.openObject(OdDb::kForRead));

        if( !pBlockRef.isNull() )
        {
          matTransform = matTransform * pBlockRef->blockTransform();
        }
      }

      OdGeMatrix3d matTransformInv = matTransform;
      matTransformInv = matTransformInv.invert();

      ptDgnAnglePt.transformBy( matTransformInv );
    }

    if( !idDgnSharedCellElm.isNull() && sharedCellPath.m_idPath.objectIds().size() > 0 )
    {
      OdGeMatrix3d matTransform;

      for( OdInt32 k = sharedCellPath.m_idPath.objectIds().size() - 1; k >= 0; k-- )
      {
        OdDbObjectId idBlockRef = sharedCellPath.m_idPath.objectIds()[k];

        OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast( idBlockRef.openObject(OdDb::kForRead));

        if( !pBlockRef.isNull() )
        {
          matTransform = matTransform * pBlockRef->blockTransform();
        }
      }

      OdGeMatrix3d matTransformInv = matTransform;
      matTransformInv = matTransformInv.invert();

      ptDgnAnglePt.transformBy( matTransformInv );
    }

    double dStartParam = 0;
    double dEndParam   = Oda2PI;
    double dSnapParam  = 0;
    bool   bPolyline   = false;
    OdUInt32 uArcSegmentIndex = 0;
    OdResult resStartParam = eInvalidIndex;
    OdResult resEndParam = eInvalidIndex;
    OdResult resAngleParam = eInvalidIndex;
    bool bForbidMiddleSnap = false;

    if( pDbEnt->isKindOf(OdDbCircle::desc()) )
    {
      OdDbCirclePtr pCircle = pDbEnt;

      resStartParam = eOk;
      resEndParam   = eOk;
      resAngleParam = pCircle->getParamAtPoint( ptDgnAnglePt, dSnapParam );
      bForbidMiddleSnap = true;
    }
    else if( pDbEnt->isKindOf(OdDbEllipse::desc()) )
    {
      OdDbEllipsePtr pEllipse = pDbEnt;

      OdGePoint3d ptStart;
      OdGePoint3d ptEnd;
      pEllipse->getStartPoint( ptStart );
      pEllipse->getEndPoint( ptEnd );

      resStartParam = pEllipse->getParamAtPoint( ptStart, dStartParam );
      resEndParam   = pEllipse->getParamAtPoint( ptEnd, dEndParam );
      resAngleParam = pEllipse->getParamAtPoint( ptDgnAnglePt, dSnapParam );

      if( OdZero(dEndParam - dStartParam) )
      {
        dEndParam += Oda2PI;
        bForbidMiddleSnap = true;
      }
    }
    else if( pDbEnt->isKindOf(OdDbArc::desc()) )
    {
      OdDbArcPtr pArc = pDbEnt;

      OdGePoint3d ptStart;
      OdGePoint3d ptEnd;
      pArc->getStartPoint( ptStart );
      pArc->getEndPoint( ptEnd );

      resStartParam = pArc->getParamAtPoint( ptStart, dStartParam );
      resEndParam   = pArc->getParamAtPoint( ptEnd, dEndParam );
      resAngleParam = pArc->getParamAtPoint( ptDgnAnglePt, dSnapParam );

      if( OdZero(dEndParam - dStartParam) )
      {
        dEndParam += Oda2PI;
        bForbidMiddleSnap = true;
      }
    }
    else if( pDbEnt->isKindOf(OdDbPolyline::desc()) )
    {
      bPolyline = true;

      OdDbPolylinePtr pPolyline = pDbEnt;

      uArcSegmentIndex = 0;

      if( !idPath.m_bExists )
      {
        uArcSegmentIndex += idPath.m_uStartPt;
      }

      resStartParam = eInvalidIndex;
      resEndParam   = eInvalidIndex;

      if( uArcSegmentIndex < pPolyline->numVerts() )
      {
        double dBulge = pPolyline->getBulgeAt( uArcSegmentIndex );

        if( !OdZero(dBulge) )
        {
          if( uArcSegmentIndex < pPolyline->numVerts() - 1 )
          {
            OdDbFullSubentPathArray subEntPath;
            OdDbObjectIdArray tmpArr;

            pPolyline->subGetSubentPathsAtGsMarker( OdDb::kEdgeSubentType, uArcSegmentIndex + 1,  
              ptDgnAnglePt, OdGeMatrix3d::kIdentity, subEntPath, &tmpArr );

            OdDbEntityPtr pEnt = pPolyline->subentPtr( subEntPath[0] );

            if( pEnt->isKindOf(OdDbArc::desc()) )
            {
              OdDbArcPtr pArc = pEnt;

              OdGePoint3d ptStart;
              OdGePoint3d ptEnd;
              pArc->getStartPoint( ptStart );
              pArc->getEndPoint( ptEnd );

              resStartParam = pArc->getParamAtPoint( ptStart, dStartParam );
              resEndParam   = pArc->getParamAtPoint( ptEnd, dEndParam );
              resAngleParam = pArc->getParamAtPoint( ptDgnAnglePt, dSnapParam );

              if( OdZero(dEndParam - dStartParam) )
              {
                dEndParam += Oda2PI;
                bForbidMiddleSnap = true;
              }
            }
            else if( pEnt->isKindOf(OdDbCircle::desc()))
            {
              OdDbArcPtr pCircle = pEnt;

              resStartParam = eOk;
              resEndParam   = eOk;
              resAngleParam = pCircle->getParamAtPoint( ptDgnAnglePt, dSnapParam );
              bForbidMiddleSnap = true;
            }
          }
        }
      }
    }

    if( (resStartParam == eOk) && (resEndParam == eOk) && (resAngleParam == eOk) )
    {
      OdDb::OsnapMode curMode = OdDb::kOsModeNear;

      double dParam = (dSnapParam - dStartParam) / ( dEndParam - dStartParam );

      if( OdZero(dParam, 1e-6) )
      {
        curMode = OdDb::kOsModeStart;
      }
      else if( OdZero(dParam - 1.0, 1e-6) )
      {
        curMode = OdDb::kOsModeEnd;
      }
      else if( !bForbidMiddleSnap && OdZero(dParam - 0.5, 1e-6) )
      {
        curMode = OdDb::kOsModeMid;
      }

      OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
      pointRef->setPoint(ptDgnAnglePt);
      pointRef->setOsnapType(curMode);
      pointRef->setNearPointParam(dSnapParam);

      if( !idDgnSharedCellElm.isNull() )
      {
        pointRef->mainEntity().objectIds().append( sharedCellPath.m_idPath.objectIds() );
      }

      pointRef->mainEntity().objectIds().append(idPath.m_idPath.objectIds());

      if( bPolyline )
      {
        pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
        pointRef->mainEntity().subentId().setIndex( uArcSegmentIndex + 1 );
      }
      else
      {
        pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);
      }

      pDimAssoc->setPointRef(ptType, pointRef);

      OdDbObjectIdArray arrTotalElements = pointRef->mainEntity().objectIds();

      for( OdUInt32 i = 0; i < arrTotalElements.size(); i++ )
      {
        OdDbObjectPtr pObj = arrTotalElements[i].openObject(OdDb::kForWrite);

        if( !arrObjects.contains( pObj ) )
        {
          arrObjects.push_back( pObj );
        }
      }

      bRet = true;
    }

    return bRet;
  }

  //-----------------------------------------------------------------------------------------------------------

  bool addDimAssocPoint(  OdDbDimAssocPtr& pDimAssoc, 
                            const OdDgAssocPointRootPtr& pDgnAssoc, 
                              const OdDbDimAssoc::PointType& ptType, 
                                OdDgDatabase* pDg,
                                  const OdGePoint3d& ptDim,
                                    OdArray<OdDbObjectPtr>& arrObjects 
                       )
  {
    bool bRet = false;

    if( pDgnAssoc.isNull() || pDimAssoc.isNull() || !pDg )
    {
      return bRet;
    }

    switch( pDgnAssoc->getType() )
    {
      case OdDgAssocPointRoot::kLinearAssociation :
      {
        bRet = addLinearPointAssociation( pDimAssoc,pDgnAssoc, ptType, pDg, arrObjects );
      } break;

      case OdDgAssocPointRoot::kProjectionAssociation :
      {
        bRet = addProjectrionPointAssociation( pDimAssoc, pDgnAssoc, ptType, pDg, arrObjects );
      } break;

      case OdDgAssocPointRoot::kArcAssociation :
      {
        bRet = addArcPointAssociation( pDimAssoc, pDgnAssoc, ptType, pDg, arrObjects );
      } break;

      case OdDgAssocPointRoot::kBSplineCurveAssociation :
      {
        bRet = addBSplineCurvePointAssociation( pDimAssoc, pDgnAssoc, ptType, pDg, arrObjects );
      } break;

      case OdDgAssocPointRoot::kOriginAssociation :
      {
        bRet = addOriginPointAssociation( pDimAssoc, pDgnAssoc, ptType, pDg, arrObjects );
      } break;

      case OdDgAssocPointRoot::kIntersect2Association :
      {
        bRet = addIntersect2PointAssociation( pDimAssoc, pDgnAssoc, ptType, pDg, ptDim, arrObjects );
      } break;
    }
    
    return bRet;
  }

//-----------------------------------------------------------------------------------------------------------

  void createAngularDimensionAssoc( OdDbDimensionPtr& pDim, 
                                      const OdDgnImportDimAssocDescriptor& assocDiscr, 
                                        OdDgDatabase* pDg )
  {
    OdGePoint3d ptXLine1;
    OdGePoint3d ptXLine2;
    OdGePoint3d ptCenter;

    bool bSetAssoc = false;

    if( pDim->isKindOf( OdDb3PointAngularDimension::desc() ) )
    {
      OdDb3PointAngularDimensionPtr pDbDim = pDim;
      ptXLine1 = pDbDim->xLine1Point();
      ptXLine2 = pDbDim->xLine2Point();
      ptCenter = pDbDim->centerPoint();
      bSetAssoc = true;
    }
    else if( pDim->isKindOf( OdDbArcDimension::desc() ) )
    {
      OdDbArcDimensionPtr pDbDim = pDim;
      ptXLine1 = pDbDim->xLine1Point();
      ptXLine2 = pDbDim->xLine2Point();
      ptCenter = pDbDim->centerPoint();
      bSetAssoc = true;
    }

    if( bSetAssoc )
    {
      OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::createObject();
      pDimAssoc->setRotatedDimType(OdDbDimAssoc::kUnknown);

      OdArray<OdDbObjectPtr> arrObjectsToAddReactor;

      bool bPoint3Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point3Desc, OdDbDimAssoc::kOverrideCenterPoint, pDg, 
                                            ptCenter, arrObjectsToAddReactor );
      bool bPoint1Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kXline1Point, pDg, 
                                            ptXLine1, arrObjectsToAddReactor );
      bool bPoint2Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point2Desc, OdDbDimAssoc::kXline2Point, pDg, 
                                            ptXLine2, arrObjectsToAddReactor );

      if( bPoint1Assoc || bPoint2Assoc || bPoint3Assoc )
      {
        OdDbObjectId dimAssId;
        pDim->createExtensionDictionary();
        pDimAssoc->post( pDim->objectId(), dimAssId );

        for( OdUInt32 i  = 0; i < arrObjectsToAddReactor.size(); i++ )
        {
          arrObjectsToAddReactor[i]->addPersistentReactor(dimAssId);
        }
      }
    }
  }

//-----------------------------------------------------------------------------------------------------------

  void createRadialDimensionAssoc( OdDbDimensionPtr& pDim, 
                                     const OdDgnImportDimAssocDescriptor& assocDiscr, 
                                       OdDgDatabase* pDg )
  {
    OdGePoint3d ptDim1;
    OdGePoint3d ptDim2;

    bool bPoint1Assoc = false;
    bool bPoint2Assoc = false;

    OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::createObject();
    pDimAssoc->setRotatedDimType(OdDbDimAssoc::kUnknown);

    OdArray<OdDbObjectPtr> arrObjectsToAddReactor;

    if( pDim->isKindOf( OdDbRadialDimension::desc() ) )
    {
      OdDbRadialDimensionPtr pDbDim = pDim;
      ptDim1 = pDbDim->center();
      ptDim2 = pDbDim->chordPoint();

      bPoint1Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kCenterPoint, pDg, 
                                       ptDim1, arrObjectsToAddReactor );
      bPoint2Assoc = assocDimPointToArc( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kChordPoint, pDg, 
                                       ptDim2, arrObjectsToAddReactor );
    }
    else if( pDim->isKindOf( OdDbDiametricDimension::desc() ) )
    {
      OdDbDiametricDimensionPtr pDbDim = pDim;

      ptDim1 = pDbDim->chordPoint();
      ptDim2 = pDbDim->farChordPoint();

      bPoint1Assoc = assocDimPointToArc( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kChordPoint, pDg, 
                                         ptDim1, arrObjectsToAddReactor );
      bPoint2Assoc = assocDimPointToArc( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kFarChordPoint, pDg, 
                                         ptDim2, arrObjectsToAddReactor );
    }

    if( bPoint1Assoc || bPoint2Assoc )
    {
      OdDbObjectId dimAssId;
      pDim->createExtensionDictionary();
      pDimAssoc->post( pDim->objectId(), dimAssId );

      for( OdUInt32 i  = 0; i < arrObjectsToAddReactor.size(); i++ )
      {
        arrObjectsToAddReactor[i]->addPersistentReactor(dimAssId);
      }
    }
  }

//-----------------------------------------------------------------------------------------------------------

  void createOrdinateDimensionAssoc( OdDbDimensionPtr& pDim, 
                                       const OdDgnImportDimAssocDescriptor& assocDiscr, 
                                         OdDgDatabase* pDg )
  {
    OdDbOrdinateDimensionPtr pDbDim = OdDbOrdinateDimension::cast( pDim.get() );

    if( pDbDim.isNull() )
    {
      return;
    }

    bool bPoint1Assoc = false;
    bool bPoint2Assoc = false;

    OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::createObject();
    pDimAssoc->setRotatedDimType(OdDbDimAssoc::kUnknown);

    OdArray<OdDbObjectPtr> arrObjectsToAddReactor;

    bPoint1Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kOriginPoint, pDg, 
        pDbDim->origin(), arrObjectsToAddReactor );
    bPoint2Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point2Desc, OdDbDimAssoc::kDefiningPoint, pDg, 
        pDbDim->definingPoint(), arrObjectsToAddReactor );

    if( bPoint1Assoc || bPoint2Assoc )
    {
      OdDbObjectId dimAssId;
      pDim->createExtensionDictionary();
      pDimAssoc->post( pDim->objectId(), dimAssId );

      for( OdUInt32 i  = 0; i < arrObjectsToAddReactor.size(); i++ )
      {
        arrObjectsToAddReactor[i]->addPersistentReactor(dimAssId);
      }
    }
  }

//-----------------------------------------------------------------------------------------------------------

  void createLinearDimensionAssoc( OdDbDimensionPtr& pDim, 
                                     const OdDgnImportDimAssocDescriptor& assocDiscr, 
                                       OdDgDatabase* pDg )
  {
    OdDbRotatedDimensionPtr pLinearDim = OdDbRotatedDimension::cast( pDim.get() );

    if( pLinearDim.isNull() )
    {
      return;
    }

    OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::createObject();
    pDimAssoc->setRotatedDimType(OdDbDimAssoc::kUnknown);

    OdArray<OdDbObjectPtr> arrObjectsToAddReactor;

    bool bPoint1Assoc = false;
    bool bPoint2Assoc = false;

    if( !assocDiscr.bRadialDiamDimension )
    {
      bPoint1Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kXline1Point, pDg, 
                                       pLinearDim->xLine1Point(), arrObjectsToAddReactor );
      bPoint2Assoc = addDimAssocPoint( pDimAssoc, assocDiscr.point2Desc, OdDbDimAssoc::kXline2Point, pDg, 
                                       pLinearDim->xLine2Point(), arrObjectsToAddReactor );
    }
    else
    {
      bPoint1Assoc = assocDimPointToArc( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kXline1Point, pDg, 
                                         pLinearDim->xLine1Point(), arrObjectsToAddReactor );

      bPoint1Assoc = assocDimPointToArc( pDimAssoc, assocDiscr.point1Desc, OdDbDimAssoc::kXline2Point, pDg, 
                                         pLinearDim->xLine2Point(), arrObjectsToAddReactor );
    }

    if( bPoint1Assoc || bPoint2Assoc )
    {
      OdDbObjectId dimAssId;
      pDim->createExtensionDictionary();
      pDimAssoc->post( pDim->objectId(), dimAssId );

      for( OdUInt32 i  = 0; i < arrObjectsToAddReactor.size(); i++ )
      {
        arrObjectsToAddReactor[i]->addPersistentReactor(dimAssId);
      }
    }
  }

//-----------------------------------------------------------------------------------------------------------

}
