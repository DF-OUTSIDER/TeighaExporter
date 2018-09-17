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

#ifndef DbAssocViewLabelActionParam_INCLUDED_
#define DbAssocViewLabelActionParam_INCLUDED_ /*!DOM*/

#include "SynergyObjDPWExports.h" 
#include "DbAssocActionParam.h"
#include "DbModelDocViewStyle.h"

#include "TD_PackPush.h"

/** \details
  This class represents the view label action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbAssocViewLabelActionParam : public OdDbAssocObjectActionParam
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocViewLabelActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocViewLabelActionParam(bool createImpObj = true);

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

  /** \details

    flags Overrides. 
  */
  enum LabelOverridesProperty
  {
    kLabelOffsetOverriden     = 0x001,
    kAttachmentPointOverriden = 0x002,
    kTextStyleOverriden       = 0x004,
    kColorOverriden           = 0x008,
    kTextHeightOverriden      = 0x010,
    kTextFieldOverriden       = 0x020,
    kDirectionOverriden       = 0x040,
    kLabelPlacementCustomized = 0x080 
  };

  enum OdDbModelDocViewStyle::AttachmentPoint attachmentPoint() const; 
  bool isAttachmentPointOverriden() const;  
  void setAttachmentPointOverride(enum OdDbModelDocViewStyle::AttachmentPoint, bool); 
  void setAttachmentPointOverride(); 

  bool isColorOverriden() const; 
  void setColorOverride(); 

  bool isDirectionOverriden() const; 
  void setDirectionOverride(); 

  OdGeVector2d labelOffset() const;  
  void setLabelOffset(OdGeVector2d const &);  

  bool isLabelOffsetOverriden() const;  
  void setLabelOffsetOverride(OdGeVector2d const &);  
  void setLabelOffsetOverride();  

  bool isLabelPlacementCustomized() const; 
  void setLabelPlacementCustomized(bool);  

  bool isTextFieldOverriden() const; 
  void setTextFieldOverride(); 

  bool isTextHeightOverriden() const;   
  void setTextHeightOverride();  

  bool isTextStyleOverriden() const;    
  void setTextStyleOverride(); 

  OdDbObjectId viewLabelId(void) const;  

  // Not implemented
  //void setTextOverride(OdDbObjectId const &,OdDbObjectId const &,OdDbObjectId const &, bool);
  //OdResult update(enum OdDbAssocDraggingState, OdDbObjectId const &,OdDbObjectId const &,OdDbObjectId const &, bool);
  //OdResult updateLabelExtent();

private:
  //OdResult updateLabelPosition(OdDbObjectId const &,OdDbObjectId const &, OdDbObjectId const &);

//  virtual OdResult subErase(bool);

};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocViewLabelActionParam class.

  \sa
    <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocViewLabelActionParam> OdDbAssocViewLabelActionParamPtr;

#include "TD_PackPop.h"

#endif // DbAssocViewLabelActionParam_INCLUDED_
