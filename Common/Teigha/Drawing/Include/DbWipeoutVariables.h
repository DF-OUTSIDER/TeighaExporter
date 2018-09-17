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




#ifndef OD_DBWIPEOUTVARIABLES_H
#define OD_DBWIPEOUTVARIABLES_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "RxDictionary.h"


#ifdef WIPEOUT_EXPORTS
#define WIPEOUT_EXPORT           OD_TOOLKIT_EXPORT
#define WIPEOUT_EXPORT_STATIC    OD_STATIC_EXPORT
#else
#define WIPEOUT_EXPORT           OD_TOOLKIT_IMPORT
#define WIPEOUT_EXPORT_STATIC    OD_STATIC_IMPORT
#endif



class OdDbWipeoutVariables;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbWipeoutVariables object pointers.
*/
typedef OdSmartPtr<OdDbWipeoutVariables> OdDbWipeoutVariablesPtr;

/** \details
    Represents the Wipeout Variables object in an OdDbDatabase instance.
    
    Corresponding C++ library: WipeOut
  
    <group OdDb_Classes>
*/
class WIPEOUT_EXPORT OdDbWipeoutVariables : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbWipeoutVariables);

  OdDbWipeoutVariables();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Returns the frame display flag for this Variables object (DXF 70).
  */
  virtual bool showFrame() const;

  /** \details
    Sets the frame display flag for this Variables object (DXF 70).
    \param showFrame [in]  Controls frame visibility
    \param bUpdateWIPEOUTFRAME [in]  Controls if WIPEOUTFRAME system variable is also updated.

    \remarks
    WIPEOUTFRAME was introduced in 2013 file format and should be used to control
    frame visiblity. bUpdateWIPEOUTFRAME argument added to keep existing code working.
  */
  virtual void setShowFrame(bool showFrame, bool bUpdateWIPEOUTFRAME = true);

  /** \details
    Opens the Wipeout Variables object in the specified database.
    \param openMode [in]  Mode in which to open the wipeout variables object.
    \param pDb [in]  Pointer to the database that contains the wipeout variables object.

    \remarks
    If the Wipeout Variables object does not exist in the specified database
    it is created.
    
    Returns a SmartPointer to the Wipeout Variables object.
  */
  static OdDbWipeoutVariablesPtr openWipeoutVariables(OdDbDatabase* pDb,
    OdDb::OpenMode openMode = OdDb::kForRead);
};


inline OdDbWipeoutVariablesPtr OdDbWipeoutVariables::openWipeoutVariables(OdDbDatabase* pDatabase, OdDb::OpenMode mode) {
  OdDbWipeoutVariablesPtr pWpVars;
  ODA_ASSERT(pDatabase);
  if (pDatabase) {
    OdDbDictionaryPtr pRootDict = pDatabase->getNamedObjectsDictionaryId().openObject();
    OdString AcDbWipeoutVariables(L"AcDbWipeoutVariables");
    pWpVars = ::odrxCastByClassName<OdDbWipeoutVariables>(pRootDict->getAt(ACAD_WIPEOUT_VARS).openObject(mode), AcDbWipeoutVariables);
    if (pWpVars.isNull() && mode == OdDb::kForWrite) {
      pRootDict->upgradeOpen();
      pWpVars = ::odrxSafeCreateObject<OdDbWipeoutVariables>(AcDbWipeoutVariables);
      pRootDict->setAt(ACAD_WIPEOUT_VARS, pWpVars);
    }
  }
  return pWpVars;
}


#include "TD_PackPop.h"

#endif //OD_DBWIPEOUTVARIABLES_H

