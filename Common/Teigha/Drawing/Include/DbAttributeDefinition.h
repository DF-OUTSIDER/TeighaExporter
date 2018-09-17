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




#ifndef _OD_DB_ATTRDEF_
#define _OD_DB_ATTRDEF_

#include "TD_PackPush.h"

#include "DbText.h"
#include "DbMText.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents attribute defintion (Attdef) entities in an OdDbDatabase instance.
  
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbAttributeDefinition : public OdDbText
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDbAttributeDefinition);
  OdDbAttributeDefinition();

  /** \details
    Returns the prompt string of this Attdef entity (DXF 3).
  */
  const OdString prompt() const;

  /** \details
    Sets the prompt string of this Attdef entity (DXF 3).
    \param promptString [in]  Prompt string
  */
  void setPrompt(
    const OdString& promptString);

  /** \details
    Returns the tag string of this Attdef entity (DXF 2).
  */
  const OdString tag() const;

  /** \details
    Sets the tag string of this Attdef entity (DXF 2).
    \param tagString [in]  Tag string.  
  */
  void setTag(
    const OdString& tagString);

  /** \details
    Returns true if and only if this Attdef entity is invisible (DXF 70, bit 0x01).
  */
  bool isInvisible() const;

  /** \details
    Controls the invisibility of this Attdef entity (DXF 70, bit 0x01).

    \param invisible [in]  Controls invisibility.
  */
  void setInvisible(
    bool invisible);

  /** \details
    Returns true if and only if this Attdef entity is constant (DXF 70, bit 0x02).
  */
  virtual bool isConstant() const;

  /** \details
    Controls the constant status of this Attdef entity (DXF 70, bit 0x02).
    \param constant [in]  Controls constant status.
  */
  void setConstant(bool constant);

  /** \details
    Returns true if and only if this Attdef entity is verifiable (DXF 70, bit 0x04).
  */
  bool isVerifiable() const;

  /** \details
    Controls the verifiable status of this Attdef entity (DXF 70, bit 0x04).
    \param verifiable [in]  Controls verifiable status.
  */
  void setVerifiable(bool);

  /** \details
    Returns true if and only if this Attdef entity is preset (DXF 70, bit 0x08).
  */
  bool isPreset() const;

  /** \details
    Controls the preset status of this Attdef entity (DXF 70, bit 0x08).
    \param preset [in]  Controls preset status.
  */
  void setPreset(bool);

  /** \details
    Returns the field length of this Attdef entity (DXF 73).
  */
  OdUInt16 fieldLength() const;

  /** \details
    Sets the field length of this Attdef entity (DXF 73).
    \param fieldLength [in]  Field length.
  */
  void setFieldLength(
    OdUInt16 fieldLength);

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;
  void subViewportDraw(
    OdGiViewportDraw* pVd) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  void appendToOwner(
    OdDbIdPair& Idpair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  bool lockPositionInBlock() const;
  void setLockPositionInBlock(bool bValue);
  // multiline attribute definition support
  bool isMTextAttributeDefinition() const;
  OdDbMTextPtr getMTextAttributeDefinition() const;
  void setMTextAttributeDefinition(OdDbMText*);
  void convertIntoMTextAttributeDefinition (bool val = true);
  void updateMTextAttributeDefinition();
  virtual void subClose() ODRX_OVERRIDE;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbAttributeDefinition object pointers.
*/
typedef OdSmartPtr<OdDbAttributeDefinition> OdDbAttributeDefinitionPtr;

#include "TD_PackPop.h"

#endif

