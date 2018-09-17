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
#include "DgnImportLineString.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DbPolyline.h>
#include <Db3dPolyline.h>
#include <DgLineString.h>
#include <Db3dPolylineVertex.h>
#include <DbLine.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgLineString2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLineString2d* pline = (OdDgLineString2d*)e;

  bool bImportThroughDraw = false;

  for (OdUInt32 j = 0; j < pline->getVerticesCount(); ++j)
  {
    OdGePoint2d ptVertex = pline->getVertexAt(j);

    if( ptVertex.x > 1e100 || ptVertex.y > 1e100 ) // break;
    {
      bImportThroughDraw = true;
      break;
    }
  }

  if( bImportThroughDraw )
  {
    OdDgnImportPE::subImportElement( e, owner );
    return;
  }

  OdDbPolylinePtr dbPline = OdDbPolyline::createObject();
  dbPline->setDatabaseDefaults(owner->database());

  for (OdUInt32 i = 0; i < pline->getVerticesCount(); ++i)
    dbPline->addVertexAt(i, pline->getVertexAt(i));

  owner->database()->addOdDbObject(dbPline, owner->objectId());
  copyEntityProperties(pline, dbPline);
  owner->appendOdDbEntity(dbPline);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbPline->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pline->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgLineString3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLineString3d* pline = (OdDgLineString3d*)e;
  OdGePoint3dArray pts;
  pts.resize(pline->getVerticesCount());
  if (pts.size() == 0)
    return;

  bool bImportThroughDraw = false;

  for (OdUInt32 i = 0; i < pline->getVerticesCount(); ++i)
  {
    pts[i] = pline->getVertexAt(i);

    if( pts[i].x > 1e100 || pts[i].y > 1e100 || pts[i].z > 1e100 ) // break
    {
      bImportThroughDraw = true;
      break;
    }
  }

  if( bImportThroughDraw )
  {
    OdDgnImportPE::subImportElement( e, owner );
    return;
  }

  OdGePlane plane;
  OdDbEntityPtr dbEnt;
  if (pts.size() == 1)
  {
    OdDbLinePtr line = OdDbLine::createObject();
    line->setStartPoint(pts[0]);
    line->setEndPoint(pts[0]);
    dbEnt = line;
  }
  else if (pts.size() == 2)
  {
    OdDbLinePtr line = OdDbLine::createObject();
    line->setStartPoint(pts[0]);
    line->setEndPoint(pts[1]);
    dbEnt = line;
  }
  else
  {
    OdGeError res = geCalculatePlane(pts, plane);
    if (res == OdGe::kNonCoplanarGeometry)
    {
      OdDb3dPolylinePtr dbPline = OdDb3dPolyline::createObject();
      for (OdUInt32 i = 0; i < pts.size(); ++i)
      {
        OdDb3dPolylineVertexPtr v = OdDb3dPolylineVertex::createObject();
        v->setPosition(pts[i]);
        dbPline->appendVertex(v);
      }
      dbEnt = dbPline;
    }
    else
    {
      switch (res)
      {
        case OdGe::kOk:
          break;

        case OdGe::kDegenerateGeometry:
        { // Collinear
          double dQDistance = 0;
          OdGeVector3d vLine;
          for (int i = pts.size() - 1; i > 0; --i)
          {
            OdGeVector3d vC = pts[i] - pts[0];
            double dCQd = vC.lengthSqrd();
            if (dCQd > dQDistance)
            {
              dQDistance = dCQd;
              vLine = vC;
            }
          }
          if (OdNonZero(sqrt(dQDistance)) && !vLine.isPerpendicularTo(OdGeVector3d::kZAxis))
          {
            plane.set(pts[0], vLine, vLine.perpVector());
            break;
          }
        }
        // No break here
        case OdGe::kSingularPoint:
          plane.set(pts[0], OdGeVector3d::kZAxis);
          break;
        default:
          ODA_FAIL();
      }

      OdGeVector3d normal(plane.normal());
      double elevation = -plane.signedDistanceTo(OdGePoint3d::kOrigin);
      // move plane origin to the (0,0)
      plane = OdGePlane(OdGePoint3d::kOrigin + plane.normal() * elevation, normal);

      OdDbPolylinePtr dbPline = OdDbPolyline::createObject();
      dbPline->setNormal(normal);
      dbPline->setElevation(elevation);
      OdGeMatrix3d ecs(OdGeMatrix3d::worldToPlane(plane));
      dbPline->reset(true, pts.size());
      for (OdUInt32 i = 0; i < pts.size(); ++i)
      {
        OdGePoint3d p(pts[i]);
        p.transformBy(ecs);
        dbPline->setPointAt(i, p.convert2d());
      }
      dbEnt = dbPline;
    }
  }
  dbEnt->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbEnt, owner->objectId());
  copyEntityProperties(pline, dbEnt);
  owner->appendOdDbEntity(dbEnt);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbEnt->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pline->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
