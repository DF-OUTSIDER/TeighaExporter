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




#ifndef OD_RTEXT_H
#define OD_RTEXT_H

#include "TD_PackPush.h"

#ifdef RTEXT_EXPORTS
#define RTEXT_EXPORT           OD_TOOLKIT_EXPORT
#define RTEXT_EXPORT_STATIC    OD_STATIC_EXPORT
#else
#define RTEXT_EXPORT           OD_TOOLKIT_IMPORT
#define RTEXT_EXPORT_STATIC    OD_STATIC_IMPORT
#endif

#include "DbEntity.h"
class OdDbEntityImpl;

/** \details
    This class represents Reactive text (RText) entities in an OdDbDatabase instance.
    Corresponding C++ library: RText
    
    \remarks
    RText entities can display either the contents of an external text file or the evaluation of a DIESEL expression. 

    <group Other_Classes>
*/
class RTEXT_EXPORT RText : public OdDbEntity
{
public:
  ODRX_DECLARE_MEMBERS(RText);

  /** \details
    Returns the WCS normal to the plane of this RText entity.
  */
  OdGeVector3d normal() const;
  
  /** \details
    Sets the WCS normal to the plane of this RText entity.
    \param normal [in]  Normal.
  */
  void         setNormal(const OdGeVector3d &normal);

  /** \details
    Returns the WCS insertion point of this RText entity.
  */
  OdGePoint3d getPoint() const;

  /** \details
    Sets the WCS insertion point of this RText entity.

    \param point [in]  Insertion point.
  */
  void        setPoint(const OdGePoint3d &point);

  /** \details
    Returns the rotation angle of this RText entity.
    \remarks
    All angles are expressed in radians.
  */
  double getRotAngle() const;
  /** \details
    Sets the rotation angle of this RText entity.
    \param rotAngle [in]  Rotation angle.
    \remarks
    All angles are expressed in radians.
  */
  void   setRotAngle(double rotAngle);

  /** \details
    Returns the height of this RText entity.
  */
  double getHeight() const;
  /** \details
    Sets the height of this RText entity.
    \param height [in]  Height.
  */
  void   setHeight(double height);

  /** \details
    Returns true if and only if the RText entity is to be treated an as DIESEL expression.
  */
  bool isStringExpression() const;
  /** \details
    Controls the treatment of this RText entity as an DIESEL expression.
    \param isExpression [in]  Controls treatment as a DIESAL expression.
  */
  void setToExpression(bool isExpression);

  /** \details
    Returns true if and only if MText inline sequences are evaluated for this RText entity.
  */
  bool enabledMTextSequences() const;
  /** \details
    Controls the evaluation of MText inline sequences for this RText entity.
    \param enable [in]  Enables MText sequences.
  */
  void enableMTextSequences(bool enable);
  /** \details
    Returns a copy of the string contents of this RText entity.
    \remarks
    If !isStringExpression(), the string contents specify the name of the file to be displayed.
  */
  OdString getStringContents() const;
  /** \details
    Sets the string contents of this RText entity.
    \param stringContents [in]  String contents.
    \remarks
    If !isStringExpression(), the string contents specify the name of the file to be displayed.
  */
  void     setStringContents(const OdString &stringContents);

  /** \details
    Returns the object ID of the TextStyle of this RText entity.
    
    \remarks
    If 
  */
  OdDbObjectId textStyleId() const;
  /** \details
    Returns the name of the TextStyle of this RText entity.
  */
  OdString textStyleName() const;
  /** \details
    Sets the TextStyle of this RText entity.
    \param textStyleId [in]  Object ID of TextStyle.
  */
  void setTextStyle(OdDbObjectId textStyleId);
  /** \details
    Sets the TextStyle of this RText entity.
    \param name [in]  Name of TextStyle.
  */
  void setTextStyle(const OdString& name);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm);

  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  void subClose();

protected:
  RText();
  RText(OdDbEntityImpl* pImpl);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for RText object pointers.
*/
typedef OdSmartPtr<RText> OdRTextPtr;

#include "TD_PackPop.h"

#endif  // OD_RTEXT_H

