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

#ifndef ODDGNLS_DBLSOBJECT_H
#define ODDGNLS_DBLSOBJECT_H

#include "DbObject.h"

#include "OdDgnLSExport.h"

#include "TD_PackPush.h"

/* DgnLS classes hierarchy:
   - OdDbLSObject
   ---- OdDbLSDefinition
   ---- OdDbLSComponent
   -------- OdDbLSCompoundComponent
   -------- OdDbLSPointComponent
   -------- OdDbLSStrokePatternComponent
   ------------ OdDbLSInternalComponent
   -------- OdDbLSSymbolComponent
*/

/** \details
    Basic object for all DgnLS objects.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSObject : public OdDbObject
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSObject);

    /** \details
      Teigha applications typically will not use this constructor, insofar as 
      this class is a base class.
    */
    OdDbLSObject();

    virtual OdResult dwgInFields(OdDbDwgFiler *pFiler);
    virtual void dwgOutFields(OdDbDwgFiler *pFiler) const;
    virtual OdResult dxfInFields(OdDbDxfFiler *pFiler);
    virtual void dxfOutFields(OdDbDxfFiler *pFiler) const;

#ifdef ODA_DIAGNOSTICS
    /** \details
      Debug data output for entire objects hierarchy.
    */
    void trace() const;
#endif

    /** \details
      Returns component unique identifier.

      \remarks
      16 bytes of data.
    */
    virtual const OdUInt8 *componentUID() const = 0;

    /** \details
      Sets component unique identifier.

      \param pUID [in]  Input unique identifier represented as 16 bytes of data.
    */
    void setComponentUID(const OdUInt8 *pUID);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSObject object pointers.
*/
typedef OdSmartPtr<OdDbLSObject> OdDbLSObjectPtr;

// Inlines section

inline void OdDbLSObject::setComponentUID(const OdUInt8 *pUID)
{
  assertWriteEnabled();
  OdUInt8 *pUIDOut = const_cast<OdUInt8*>(componentUID());
  for (int i = 0; i < 16; i++)
    pUIDOut[i] = pUID[i];
}

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSOBJECT_H
