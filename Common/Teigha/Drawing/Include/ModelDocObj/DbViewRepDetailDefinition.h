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

#ifndef OD_VIEWREPDETAILDEFINITION_H
#define OD_VIEWREPDETAILDEFINITION_H

#include "DbObject.h"
#include "ModelDocObjExports.h"
#include "DbViewRepCutDefinition.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepDetailDefinition : public OdDbViewRepCutDefinition
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepDetailDefinition);
  OdDbViewRepDetailDefinition();
  virtual ~OdDbViewRepDetailDefinition();

  enum FenceType
  {
    FT_RECTANGULAR = 1,
    FT_CIRCULAR = 2
  };

  enum EdgeType
  {
    ET_SMOOTH = 0,
    ET_JAGGED = 1,
    ET_SMOOTH_WITH_BORDER = 2,
    ET_SMOOTH_WITH_CONNECTION_LINE = 6,
  };

  void setDisplayConnectLine(bool);
  bool displayConnectLine() const;

  void setDisplayFullBoundary(bool);
  bool displayFullBoundary() const;

  void setJagged(bool);
  bool jagged() const;

  void setEdgeType(EdgeType);
  EdgeType edgeType() const;

  void setBoundaryType(FenceType);
  FenceType boundaryType() const;

  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepDetailDefinition object pointers.
*/
typedef OdSmartPtr<OdDbViewRepDetailDefinition> OdDbViewRepDetailDefinitionPtr;

#include "TD_PackPop.h"

#endif // OD_VIEWREPDETAILDEFINITION_H
