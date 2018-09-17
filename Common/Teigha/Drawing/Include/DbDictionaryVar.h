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




#ifndef OD_DBDICTIONARYVAR_H
#define OD_DBDICTIONARYVAR_H

#include "TD_PackPush.h"

#include "DbObject.h"

/** \details
    This class represents Dictionary Variable objects in an OdDbDatabase instance.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDictionaryVar : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDictionaryVar);

  OdDbDictionaryVar();

  /** \details
    Returns the schema number of this Dictionary Variable object (DXF 280).
  */
  OdInt16 getSchema() const;

  /** \details
    Sets the schema number of this Dictionary Variable object (DXF 280).
    \param schemaNumber [in]  Schema number.
  */
  void setSchema(OdInt16 schemaNumber);

  /** \details
      Returns the value of this Dictionary Variable object  (DXF 1).
  */
  OdString value() const;
  
  /** \details
    Returns the value of this Dictionary Variable object as a string.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(OdString& val) const { val = value(); return true; }

  /** \details
    Returns the value of this Dictionary Variable object as an int.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(int& val) const;

  /** \details
    Returns the value of this Dictionary Variable object as a bool.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(bool& val) const;
  
  /** \details
    Returns the value of this Dictionary Variable object as a double.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(double& val) const;
  
  /** \details
    Returns the value of this Dictionary Variable object as an OdGePoint2d.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(OdGePoint2d& val) const;
  
  /** \details
    Returns the value of this Dictionary Variable object as an OdGePoint3d.

    \param val [out]  Receives the value of this object.
  */
  bool valueAs(OdGePoint3d& val) const;

  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(const OdString& val);
  
  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(int val);
  
  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(bool val);
  
  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(double val);
 
  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(const OdGePoint2d& val);
  
  /** \details
    Sets the value of this Dictionary Variable object  (DXF 1).
    \param val [in]  Value for this object.
  */
  void setValue(const OdGePoint3d& val);

  virtual OdResult dxfIn(OdDbDxfFiler* pFiler);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

};
 
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDictionaryVar object pointers.
*/
typedef OdSmartPtr<OdDbDictionaryVar> OdDbDictionaryVarPtr;

#include "TD_PackPop.h"

#endif  // OD_DBDICTIONARYVAR_H

