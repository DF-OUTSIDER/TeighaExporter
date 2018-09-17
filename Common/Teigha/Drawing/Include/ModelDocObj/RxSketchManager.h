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
#ifndef ODRXSKETCHMANAGER_H_
#define ODRXSKETCHMANAGER_H_

#include "RxObject.h"


#include "ModelDocObjExports.h"
#include "TD_PackPush.h"

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxSketchManager : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdRxSketchManager );
  OdRxSketchManager();
  virtual ~OdRxSketchManager();

  virtual OdResult dwgInFields( OdDbDwgFiler* pFiler );
  virtual void dwgOutFields( OdDbDwgFiler* pFiler ) const;
  virtual OdResult dxfInFields( OdDbDxfFiler* pFiler );
  virtual void dxfOutFields( OdDbDxfFiler* pFiler ) const;

  
  //virtual OdResult applyPartialUndo( AcDbDwgFiler *, unsigned short );
  void clearSketchObjects();
  OdDbObjectId constructionLineType()const;
  void deleteSketch( OdRxSketchObject const *pSketchObj );
  OdResult enterSketchMode( OdRxSketchObject *pSketchObj );
  OdResult exitSketchMode();
  OdRxSketchObject *  getActiveSketch();
  OdRxSketchObject *  getSketchFromTag( unsigned short tag );
  OdRxSymbolSketch *  getSymbolSketchFromSymbol( OdDbObjectId const &symbolId );
  OdRxSymbolSketch *  getSymbolSketchFromViewRep( OdDbObjectId const &viewRepId );
  bool hasSketchObjects()const;
  bool isDirty()const;
  bool isInSketchMode()const;
  OdDbObjectId const owningViewRep()const;  
  void setIsDirty( bool bDirty );
  OdDbObjectId const sketchBlockReference()const;
  void transformSketches( OdGeMatrix3d const &matrix );
  void updateConstraintGroupWorkPlanes( OdGePlane const &plane );

protected:

  class OdRxSketchManagerImpl* m_pImpl;
};

typedef OdSmartPtr<OdRxSketchManager> OdRxSketchManagerPtr;
#endif
