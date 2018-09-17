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




#ifndef OD_DBFILTER_H
#define OD_DBFILTER_H

#include "DbObject.h"

class OdRxClass;

/** \details
    The class is the base class for all OdDb Filter objects.

    \sa
    TD_Db

    \remarks
    This class defines a query and supplies a key to the 
    OdDbCompositeFilteredBlockIterator object. The index corresponding
    to this key obtained through indexClass().

    \sa
    * OdDbIndexFilterManager
    * OdDbIndex
    * OdDbFilteredBlockIterator
    * OdDbCompositeFilteredBlockIterator    
     
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbFilter : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbFilter);

  OdDbFilter();
  
  /** \details
    Returns the class descriptor of the OdDbIndex for this Filter object.
    
    \remarks
    This function is used by the init() method of the OdDbCompositeFilteredBlockIterator object.
  */
  virtual OdRxClass* indexClass() const = 0;

  OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  
  void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbFilter object pointers.
*/
typedef OdSmartPtr<OdDbFilter> OdDbFilterPtr;

#endif // OD_DBFILTER_H

