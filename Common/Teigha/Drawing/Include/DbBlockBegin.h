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




#ifndef _ODDBBLOCKBEGIN_INCLUDED_
#define _ODDBBLOCKBEGIN_INCLUDED_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DwgDeclareMembers.h"
class OdDbBlockBeginImpl;

/** \details
    This class represents BlockBegin entities in an OdDbDatabase instance.  
    
    \remarks
    OdDbBlockBegin entities are automatically created and handled by Teigha for
    each OdDbBlockTableRecord object.
     
    Client applications may add extended data or extension dictionaries to these 
    entities, but should not create or delete them.
    
    These entities have no associated geometries.
    
    \remarks
    Do not derive from this class.
    
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockBegin : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbBlockBegin);

  OdDbBlockBegin();
  /** \details
    Reads the .dwg file data of this object.
    
    \param pFiler [in] Filer object from which data are read.
    
    \returns
    Returns the filer status.
    
    This function is called by dwgIn() to allow the object to read its data.
    
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned. 
    4) Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order they were written.
    5) Return pFiler->filerStatus().
  */
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
 
  /** \details
    Writes the .dwg file data of this object.
    
    \param pFiler [in] Pointer to the filer to which data are written. 
    
    \remarks
    This function is called by dwgIn() to allow the object to write its data. 

    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dwgOutFields(pFiler). 
    3) Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the order they were written.
  */
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in] Pointer to the filer from which data are read.  
    
    \returns
    Returns the filer status. 

    This function is called by dxfIn() to allow the object to read its data. 

    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written. 
    5) Return pFiler->filerStatus().
  */
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  /** \details
    Writes the DXF data of this object. 
    
    \param pFiler [in] Pointer to the filer to which data are to be written.   
    
    \remarks
    This function is called by dxfOut() to allow the object to write its data. 

    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dxfOutFields(pFiler).
    3) Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items 
    in the order they were written.
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Reads the DXF R12 format data of this object.  
    
    \param pFiler [in] Pointer to the filer from which data are to be read.    
    
    \returns
    Returns the filer status. 

    This function is called by dxfIn() to allow the object to read its data. 

    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dxfInFields_R12(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfOutFields_R12
    (pFiler) returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in 
    the order they were written. 
    5) Return pFiler->filerStatus().
 */
  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  /** \details
    Writes the DXF data of this object.   
    
    \param pFiler [in] Pointer to the filer to which data are to be written.     
    
    \returns
    Returns the filer status. 

    This function is called by dxfOut() to allow the object to write its data. 

    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dxfOutFields(pFiler). 
    3) Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in the order they were written.
  */    
  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Always returns eInvalidExtents.
  */
  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;
  
  /** \details
    Reads the DXF format data of this object from the specified filer.    
    
    \param pFiler [in] Pointer to the filer from which the data are to be read.      
    
    \returns
    Returns the filer status. 

    This function calls this object's dxfInFields(pFiler), then loads any Xdata associated with this object.
  */
  virtual OdResult dxfIn(OdDbDxfFiler* pFiler);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbBlockBegin object pointers.
*/
typedef OdSmartPtr<OdDbBlockBegin> OdDbBlockBeginPtr;

#include "TD_PackPop.h"

#endif 
