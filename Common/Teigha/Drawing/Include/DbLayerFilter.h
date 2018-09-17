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




#ifndef OD_DBLYFILT_H
#define OD_DBLYFILT_H

#include "TD_PackPush.h"

#include "DbFilter.h"

/** \details
    This class implements LayerFilter objects in an OdDbDatabase instance.
    
    \sa
    TD_Db

    <group OdDb_Classes>
    
    \remarks
    A LayerFilter is a set of layers whose corresponding entities are  
    traversed during filtered block traversal. 
    
    LayerFilter objects objects may optionally be saved in the drawing database 
    for efficient Xref demand loading.
*/
class TOOLKIT_EXPORT OdDbLayerFilter : public  OdDbFilter
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLayerFilter);

  OdDbLayerFilter();

  virtual OdRxClass* indexClass() const;

  /** \details
    Returns true if and only if all the layer names in this LayerFilter object
    are present in the specified OdDbDatabase.
    
    \param pDb [in]  Pointer to the database to be queried.
      
    \remarks
    If pDb is null, the database containing this object is used.
  */
  virtual bool isValid(
    OdDbDatabase* pDb = 0) const;

  /** \details
    Adds the specified layer name to this LayerFilter object.
    
    \param layerName [in]  Layer name to be added. 
  */
  void add(
    const OdString& layerName);

  /** \details
    Removes the specified layer name from this LayerFilter object. 

    \param layerName [in]  Layer name to be removed. 
  */
  void remove(
    const OdString& layerName);

  /** \details
    Returns the layer name associated with the specified index in this LayerFilter object.
    
    \param layerIndex [in]  Index to be queried. 
  */
  OdString getAt(
    int layerIndex) const;

  /** \details
    Returns the number of layers stored in this LayerFilter object. 
  */
  int layerCount() const;

  OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  
  OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbLayerFilter object pointers.
*/
typedef OdSmartPtr<OdDbLayerFilter> OdDbLayerFilterPtr;

#include "TD_PackPop.h"

#endif // OD_DBLYFILT_H

