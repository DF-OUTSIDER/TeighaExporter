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

#ifndef DbAssocViewRepHatchActionParam_INCLUDED_
#define DbAssocViewRepHatchActionParam_INCLUDED_ /*!DOM*/

#include "SynergyObjDPWExports.h" 
#include "DbAssocActionParam.h"

#include "TD_PackPush.h"

/** \details
  This class represents the view hatch action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbAssocViewRepHatchActionParam : public OdDbAssocObjectActionParam
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocViewRepHatchActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocViewRepHatchActionParam(bool createImpObj = true);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  OdDbObjectId hatchId() const;

  OdUInt32 hatchIndex() const;
  void setHatchIndex(OdUInt32);

  bool isObsolete() const;
  void setObsolete(bool);

  OdGeVector3d normal() const;
  void setNormal(OdGeVector3d const &);

  OdUInt32 propertyOverrides() const;
  void setPropertyOverrides(OdUInt32);

//  virtual OdResult subErase(int);
//  OdResult applyHatchProperties(OdDbSectionViewStyle const *,double,OdDbAssocViewRepHatchActionParam const *);
//  OdResult updateHatch(struct aciv::DVO::HatchBoundary const &,OdDbSectionViewStyle const *,double,OdDbAssocViewRepHatchActionParam const *);

private:
//  OdResult applyHatchProperties(OdDbHatch *,OdDbSectionViewStyle const *,double,OdDbHatch const *) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocViewRepHatchActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocViewRepHatchActionParam> OdDbAssocViewRepHatchActionParamPtr;

#include "TD_PackPop.h"

#endif // DbAssocViewRepHatchActionParam_INCLUDED_
