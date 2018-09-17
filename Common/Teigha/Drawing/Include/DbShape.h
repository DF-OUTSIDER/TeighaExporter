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




#ifndef _OD_DB_SHAPE_
#define _OD_DB_SHAPE_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DwgDeclareMembers.h"


/** \details
    <group OdDb_Classes>

    This class represents Shape entities in an OdDbDatabase instance.
    
    \remarks
    A Shape entity is a single character of an SHX font. It is specified by name() and/or shapeNumber() and styleId(). 
  
    Corresponding C++ library: TD_Db

    \sa
    <link db_shape.html, Working with Shapes>

    OdDbMText, OdDbText classes
*/
class DBENT_EXPORT OdDbShape : public OdDbEntity
{
public:

  /*
    OdDbShape(const OdGePoint3d& position,
      double size,
      const OdChar* name,
      double rotation = 0,
      double widthFactor = 0);
  */

  DWGMAP_DECLARE_MEMBERS(OdDbShape);

  OdDbShape();
  /** \details
    Returns the position of this entity (WCS equivalent of DXF 10).
  */
  OdGePoint3d position() const;
  
  /** \details
    Sets the position of this entity (WCS equivalent of DXF 10).

    \param position [in]  Position.
  */
  void setPosition(
    const OdGePoint3d& position);

  /** \details
    Returns the size of this Shape entity (DXF 40).
  */
  double size() const;

  /** \details
    Sets the size of this Shape entity (DXF 40).
    \param size [in]  Shape size.
  */
  void setSize(
    double size);

  /** \details
    Returns the name of this Shape entity (DXF 2).
    \remarks
    The name is not part of a Shape entity, but is derived from shapeNumber() and styleId().
  */
  OdString name() const;

  /** \details
    Sets the name of this Shape entity (DXF 2).
    
    \remarks
    The name is not part of a Shape entity, but is derived from shapeNumber() and shapeIndex().
    It is more efficient to use setShapeNumber() and setStyleId() directly.
  */
  OdResult setName(
    const OdString& name);

  /** \details
    Returns the rotation angle of this Shape entity (DXF 50).
    \remarks
    All angles are expressed in radians.
  */
  double rotation() const;
  
  /** \details
    Sets the rotation angle of this Shape entity (DXF 50).
    \param rotation [in]  Rotation angle.
    \remarks
    All angles are expressed in radians.
  */
  void setRotation(
    double rotation);

  /** \details
    Returns the relative X scale factor (width factor) for this Shape entity (DXF 41).
  */
  double widthFactor() const;

  /** \details
    Sets the relative X scale factor (width factor) for this Shape entity (DXF 41).
    
    \param widthFactor [in]  Width factor.
  */
  void setWidthFactor(
    double widthFactor);

 /** \details
    Returns the oblique angle of this Shape entity (DXF 51).

    \remarks
    oblique() has a range of ±1.48335 radians (±85°).
    
    Oblique angles are measured clockwise from the vertical.

    \remarks
    All angles are expressed in radians.  
 */
  double oblique() const;
  
  /** \details
    Sets the oblique angle of this Shape entity (DXF 51).
    \param oblique [in]  Oblique angle.
    
    \remarks
    oblique() has a range of ±1.48335 radians (±85°).
    
    Oblique angles are measured clockwise from the vertical.
     
    \remarks
    All angles are expressed in radians.  
  */
  void setOblique(
    double oblique);

  /** \details
    Returns the thickness of this entity (DXF 39).
    
    \remarks
    Thickness is the extrusion length along the normal.
  */
  double thickness() const;
  
  /** \details
    Sets the thickness of this entity (DXF 39).
    \param thickness [in]  Thickness.
    \remarks
    Thickness is the extrusion length along the normal.
  */
  void setThickness(
    double thickness);

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;
  
  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).
    \param normal [in]  Normal.
  */
  void setNormal(
    const OdGeVector3d& normal);

  bool isPlanar() const;

  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;

  /** \details
    Returns the shape number of this Shape entity.
    \remarks
    A Shape entity is a single character of an SHX font. 
    This function returns the code of that character.  
  */
  OdInt16 shapeNumber() const;

  /** \details
    Sets the shape number of this Shape entity.
    \remarks
    A Shape entity is a single character of an SHX font. 
    This function sets the code of that character.
    \param shapeNumber [in]  Shape number.  
  */
  void setShapeNumber(
    OdInt16 shapeNumber);

  /** \details
    Returns the Object ID of the OdDbTextStyleTableRecord containing
    the SHX font file for this Shape entity.

    \remarks
    A Shape entity is a single character of an SHX font. This function
    returns a reference to that font.
  */
  OdDbObjectId styleId() const;

  /** \details
    Sets the Object ID of the OdDbTextStyleTableRecord containing
    the SHX font file for this Shape entity.

    \remarks
    A Shape entity is a single character of an SHX font. This function
    sets a reference to that font.
    \param styleId [in]  Style Object ID.  
  */
  OdResult setStyleId(
    OdDbObjectId styleId);

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  void dxfOut(
    OdDbDxfFiler* pFiler) const;

  OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  OdGeMatrix3d getEcs( ) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbShape object pointers.
*/
typedef OdSmartPtr<OdDbShape> OdDbShapePtr;

#include "TD_PackPop.h"

#endif

