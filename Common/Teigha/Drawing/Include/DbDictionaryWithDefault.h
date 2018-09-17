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




#ifndef OD_DBDICTDFLT_H
#define OD_DBDICTDFLT_H

#include "TD_PackPush.h"

#include "DbDictionary.h"

/** \details
    This class represents database -resident object dictionaries with 
    with default entries.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDictionaryWithDefault : public OdDbDictionary
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDictionaryWithDefault);

  OdDbDictionaryWithDefault();
  
  /** \details
      Sets the Object ID of the default object associated with this dictionary 
      (DXF 340).
  */
  void setDefaultId(
    const OdDbObjectId& newId);

  /** \details
      Returns the Object ID of the default object associated with this dictionary 
      (DXF 340).
  */
  OdDbObjectId defaultId() const;
  
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDictionaryWithDefault.
*/
typedef OdSmartPtr<OdDbDictionaryWithDefault> OdDbDictionaryWithDefaultPtr;

#include "TD_PackPop.h"

#endif

