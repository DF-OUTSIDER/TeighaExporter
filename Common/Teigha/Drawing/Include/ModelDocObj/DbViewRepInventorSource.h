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

#ifndef OD_VIEWREPINVENTORSOURCE_H
#define OD_VIEWREPINVENTORSOURCE_H

#include "DbObject.h"
#include "ModelDocObjExports.h"
#include "DbViewRepSource.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepInventorSource : public OdDbViewRepSource
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepInventorSource);
  //OdDbViewRepInventorSource(const OdString&);
  OdDbViewRepInventorSource();
  virtual ~OdDbViewRepInventorSource();

  //bool isMultiBody() const;
  //bool isiFactory() const;
  //bool isiComponent() const;
  //bool isWeldment() const;
  //bool hasFlatPattern() const;
  //bool isSheetMetal() const;
  //bool isPresentation() const;
  //bool isAssembly() const;
  //bool isPart() const;
  //bool isFlagOn(unsigned long) const;
  //void setInventorSourceFlags(unsigned long);
  //unsigned long inventorSourceFlags() const;
  //void setPresentationView(class const OdString&);
  //OdString presentationView() const;
  //void setMemberName(class const OdString&);
  //OdString memberName() const;
  //void setWeldmentInstance(class const OdString&);
  //OdString weldInstance() const;
  //void setLOD(class const OdString&);
  //OdString LOD() const;
  //void setPositionalName(class const OdString&);
  //OdString positionalName() const;
  //void setPath(class const OdString&);
  OdString path() const;

  virtual enum OdDbViewRep::SourceType type() const;

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbViewRepInventorSource object pointers.
*/
typedef OdSmartPtr<OdDbViewRepInventorSource> OdDbViewRepInventorSourcePtr;


#include "TD_PackPop.h"

#endif //OD_VIEWREPINVENTORSOURCE_H
