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




#ifndef OD_DBSPINDX_H
#define OD_DBSPINDX_H

#include "TD_PackPush.h"

#include "DbIndex.h"
#include "DbBlockIterator.h"

/** \details
    This class implements Spatial Index objects in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSpatialIndex : public OdDbIndex
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSpatialIndex);

  OdDbSpatialIndex();

  OdDbFilteredBlockIteratorPtr newIterator(
    const OdDbFilter* pFilter) const;
  
  void rebuildFull(
    OdDbIndexUpdateData* pIdxData);
  
  OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

protected:

  void rebuildModified(
    OdDbBlockChangeIterator* iterator);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSpatialIndex object pointers.
*/
typedef OdSmartPtr<OdDbSpatialIndex> OdDbSpatialIndexPtr;

class OdDbSpatialIndexIteratorImpl;
class OdDbSpatialFilter;


class OdDbSpatialIndexIterator;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSpatialIndexIterator object pointers.
*/
typedef OdSmartPtr<OdDbSpatialIndexIterator> OdDbSpatialIndexIteratorPtr;

/** \details
    This class implements Iterator objects that traverse queries defined by OdDbSpatialFilter objects.
    \sa
    TD_Db
    
    \remarks
    This class is used by OcDbSpatialIndex.
     
    <group OdDb_Classes>
*/
class OdDbSpatialIndexIterator : public OdDbFilteredBlockIterator
{
protected:
  OdDbSpatialIndexIterator();
public:
  ODRX_DECLARE_MEMBERS(OdDbSpatialIndexIterator);

  static OdDbSpatialIndexIteratorPtr newIterator(
    const OdDbSpatialIndex* pIndex, 
    const OdDbSpatialFilter* pFilter);
};


#include "TD_PackPop.h"

#endif // OD_DBSPINDX_H
