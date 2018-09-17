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




#ifndef OD_DBPROXY_H
#define OD_DBPROXY_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "IntArray.h"
#include "DwgDeclareMembers.h"

/** \details
    This class is the abstract base class for Proxy objects in an OdDbDatabase instance.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class DBIO_EXPORT OdDbProxyObject : public OdDbObject
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbProxyObject);

  OdDbProxyObject();

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfIn(OdDbDxfFiler* filer);

  /** \details
    Returns the edit flags for the class contained in this Proxy object.
    \remarks
    proxyFlags() returns a combination of one or more of the following:
    
    <table>
    Name                     Value    Description
    kNoOperation             0x0      None
    kEraseAllowed            0x1      erase()
    kCloningAllowed          0x80     subDeepClone(), subWblockClone()
    kAllButCloningAllowed    0x1      erase()
    kAllAllowedBits          0x81     erase(), subDeepClone(), subWblockClone()
    kMergeIgnore             0x00     Keep orignal names.
    kMergeReplace            0x100    Use clone.
    kMergeMangleName         0x200    Create anonymous name.
    </table>
  */
  int proxyFlags() const;

  /** \details
    Returns the class name of the object represented by this Proxy object.
  */
  virtual OdString originalClassName() const;

  /** \details
    Returns the DXF name of the object represented by this Proxy object.
  */
  OdString originalDxfName() const;

  /** \details
    Returns the application description of the object represented by this Proxy object.
  */
  OdString applicationDescription() const;

  /** \details
    Returns an array of the Object IDs referenced by this Proxy object.

    \param objectIds [out]  Receives an array of the reference Object IDs.
  */
  void getReferences(
    OdTypedIdsArray& objectIds) const;

  OdDb::DuplicateRecordCloning mergeStyle() const;

  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  enum
  {
    kNoOperation          = 0, 
    kEraseAllowed         = 0x1, 
    kCloningAllowed       = 0x80,
    kAllButCloningAllowed = 0x1, 
    kAllAllowedBits       = 0x81, 
    kMergeIgnore          = 0,      
    kMergeReplace         = 0x100,  
    kMergeMangleName      = 0x200,
    kDisableProxyWarning  = 0x400
  };

  /** \details
    Returns true if and only if the erase() method is allowed for this Proxy entity. 
  */
  bool eraseAllowed() const               { return GETBIT(proxyFlags(), kEraseAllowed); }
  /** \details
    Returns true if and only if all but the subDeepClone() and subWblockClone() methods are allowed for this Proxy object.
  */
  bool allButCloningAllowed() const       { return (proxyFlags() & kAllAllowedBits) == (kAllAllowedBits ^ kAllButCloningAllowed); }

  /** \details
    Returns true if and only if the subDeepClone() and subWblockClone() methods are allowed for this Proxy object.
  */
  bool cloningAllowed() const             { return GETBIT(proxyFlags(), kCloningAllowed); }

  /** \details
    Returns true if and only if all methods are allowed for this Proxy entity. 

    \remarks
    The allowed methods are as follows
    
    @untitled table
    subDeepClone()
    erase()
    subWblockClone()
  */
  bool allOperationsAllowed() const       { return (proxyFlags() & kAllAllowedBits) == kAllAllowedBits; }

  /** \details
    Returns true if and only this Proxy entity is a R13 format Proxy object. 
  */
  bool isR13FormatProxy() const           { return GETBIT(proxyFlags(), 32768); }

  virtual OdResult subErase(
    bool erasing);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbProxyObject object pointers.
*/
typedef OdSmartPtr<OdDbProxyObject> OdDbProxyObjectPtr;

#include "TD_PackPop.h"

#endif // OD_DBPROXY_H

