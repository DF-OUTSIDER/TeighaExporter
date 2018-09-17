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


#ifndef __OD_DB_UNDERLAY_DEFINITION__
#define __OD_DB_UNDERLAY_DEFINITION__

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DbUnderlayHost.h"

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbUnderlayDefinition : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbUnderlayDefinition);

  void setSourceFileName(const OdString& name);
  OdString getSourceFileName() const;

  OdString getActiveFileName() const;

  void setItemName(const OdString& item);
  OdString getItemName() const;

  void setUnderlayItem(const OdString& sourceFileName, const OdString& activeFileName, OdDbUnderlayItemPtr pItem);
  OdDbUnderlayItemPtr getUnderlayItem() const;

  OdResult load(const OdString& password);
  void unload();
  bool isLoaded() const;
  static OdString dictionaryKey(const OdRxClass* underlayDefinitionType);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdDbObjectId postDefinitionToDb(OdDbDatabase* pDb, const OdString& defName);
};

typedef OdSmartPtr<OdDbUnderlayDefinition> OdDbUnderlayDefinitionPtr;


// These inheritants are for registering OdDbUnderlayHostPE protocol extension

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDwfDefinition : public OdDbUnderlayDefinition
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDwfDefinition);
  
  OdDbDwfDefinition();

  bool isDWFx() const;
};

typedef OdSmartPtr<OdDbDwfDefinition> OdDbDwfDefinitionPtr;


/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDgnDefinition : public OdDbUnderlayDefinition
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDgnDefinition);

  OdDbDgnDefinition();

  // Specific DGNUnderlay functions
  bool useMasterUnits() const;
  void setUseMasterUnits(bool useMaster);

  int xrefDepth() const;
  void setXrefDepth(int depth);

  bool showRasterRef() const;
  void setShowRasterRef(bool bShow);
};

typedef OdSmartPtr<OdDbDgnDefinition> OdDbDgnDefinitionPtr;

/** \details

<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbPdfDefinition : public OdDbUnderlayDefinition
{
public:
  ODDB_DECLARE_MEMBERS(OdDbPdfDefinition);

  OdDbPdfDefinition();

  static OdRxModulePtr loadPdfUnderlayModule();
};

typedef OdSmartPtr<OdDbPdfDefinition> OdDbPdfDefinitionPtr;

/** \details

<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbNavisworksDefinition : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS( OdDbNavisworksDefinition );

  OdDbNavisworksDefinition();

  void setSourceFileName( const OdString& fname );
  OdString sourceFileName() const;

  OdString activeFileName() const;
  OdResult setActiveFileName( const OdString& fname );

  bool getDWGModelItemVisible() const;
  void setDWGModelItemVisible( bool bVisible );

  OdGeExtents3d getExtents() const;

  OdResult load();
  void unload();
  bool isLoaded() const;

  OdString fileType() const;

  int entityCount( int *pLockCount ) const;

  int fileFound() const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler );

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler ) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler );

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler ) const;


};

typedef OdSmartPtr<OdDbNavisworksDefinition> OdDbNavisworksDefinitionPtr;

#include "TD_PackPop.h"

#endif // __OD_DB_UNDERLAY_DEFINITION__
