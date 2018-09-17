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




#ifndef _ODDBVIEWPORTTABLERECORD_INCLUDED
#define _ODDBVIEWPORTTABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbAbstractViewTableRecord.h"
#include "DwgDeclareMembers.h"

class OdGsDCRect;
class OdDbViewportTable;
class OdGeExtents;
class OdGeMatrix3d;

/** \details
    This class represents tiled viewport records in the OdDbViewportTable in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbViewportTableRecord : public OdDbAbstractViewTableRecord
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbViewportTableRecord);

  OdDbViewportTableRecord();

  typedef OdDbViewportTable TableType;

/** \details
    Returns the lower left corner of this Viewport (DXF 10).
    
    \remarks
    * point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    * point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
*/
  OdGePoint2d lowerLeftCorner() const;

  /** \details
    Sets the lower-left corner of this Viewport (DXF 10).
    
    \param point [in]  Lower-left corner.
    
    \remarks
    * point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    * point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
  */
  void setLowerLeftCorner(
    const OdGePoint2d& point);

  /** \details
    Returns the upper right corner of this Viewport (DXF 11).
    
    \remarks
    * point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    * point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
  */
  OdGePoint2d upperRightCorner() const;

  /** \details
    Sets the upper right corner of this Viewport (DXF 11).

    \param point [in]  Upper-right corner.
    
    \remarks
    * point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    * point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
  */
  void setUpperRightCorner(
    const OdGePoint2d& point);

  /** \details
    Returns true if and only if this Viewport will display
    a plan view whenever the UCS for this Viewport changes.
    (DXF 71, bit 0x08).
  */
  bool ucsFollowMode() const;

  /** \details
    Controls the display of 
    a plan view in this Viewport whenever the UCS for this Viewport changes.
    (DXF 71, bit 0x08).

    \param ucsFollowMode [in]  Controls UCSFollowMode.
  */
  void setUcsFollowMode(
    bool ucsFollowMode);

  /** \details
    Returns the circle zoom percent for this Viewport (DXF 72).
    \remarks
    circleSides() returns a number in the range [1..20000].
  */
  OdUInt16 circleSides() const;

  /** \details
    Sets the circle zoom percent for this Viewport (DXF 72).
    \param circleSides [in]  circle zoom percent [1..20000].
  */
  void setCircleSides(
    OdUInt16 circleSides);

  /** \details
    Returns true if and only if this Viewport will display a UCS icon (DXF 74, bit 0x01).
  */
  bool iconEnabled() const;

  /** \details
    Controls the display of a UCS icon by this Viewport (DXF 74, bit 0x01).

    \param iconEnabled [in]  Controls the display.
  */
  void setIconEnabled(
    bool iconEnabled);

  /** \details
    Returns true if and only if this Viewport will display a UCS icon at the UCS origin (DXF 74, bit 0x02).
  */
  bool iconAtOrigin() const;

  /** \details
    Controls the display of a UCS icon at the UCS origin by this Viewport (DXF 74, bit 0x02).
    \param atOrigin [in]  Controls the display.   
  */
  void setIconAtOrigin(
    bool atOrigin);

  /** \details
    Returns true if and only if this Viewport will display a grid (DXF 76).
  */
  bool gridEnabled() const;

  /** \details
    Controls the display of a grid by this Viewport (DXF 76).
    \param gridEnabled [in]  Controls the display.
  */
  void setGridEnabled(
    bool gridEnabled);

  /** \details
    Returns the grid increments for this Viewport (DXF 15, 25).
  */
  OdGePoint2d gridIncrements() const;

  /** \details
    Sets the grid increments for this Viewport (DXF 15, 25).
    \param gridIncrements [in]  Grid increments.
  */
  void setGridIncrements(
    const OdGePoint2d& gridIncrements);

  /** \details
    Returns true and only if snap is enabled for this Viewport (DXF 75).
  */
  bool snapEnabled() const;

  /** \details
    Controls the snap for this Viewport (DXF 75).
    \param snapEnabled [in]  Controls the snap.
  */
  void setSnapEnabled(
    bool snapEnabled);

  /** \details
    Returns true and only if isometric snap is enabled for this Viewport (DXF 75).
  */
  bool isometricSnapEnabled() const;

  /** \details
    Controls the isometric snap for this Viewport (DXF 75).
    \param isometricSnapEnabled [in]  Controls the isometric snap.
  */
  void setIsometricSnapEnabled(bool isometricSnapEnabled);
  /** \details
    Returns the snap IsoPair for this Viewport (DXF 78).
    
    \remarks
    snapPair() returns one of the following:
    
    <table>
    Description      Value
    Left isoplane    0
    Top isoplane     1
    Right isoplane   2
    </table>
  */
  OdInt16 snapPair() const;
  
  
  /** \details
    Returns the snap IsoPair for this Viewport (DXF 78).
    
    \remarks
    snapPair must be one of the following:
    
    <table>
    Description      Value
    Left isoplane    0
    Top isoplane     1
    Right isoplane   2
    </table>
  */
  void setSnapPair(
    OdInt16 snapPair);


  /** \details
    Returns the snap rotation angle for this Viewport (DXF 50).
    \remarks
    All angles are expressed in radians.
  */
  double snapAngle() const;
  /** \details
    Sets the snap rotation angle for this Viewport (DXF 50).
    \param snapAngle [in]  Snap angle.
    \remarks
    All angles are expressed in radians.
  */
  void setSnapAngle(double snapAngle);
  /** \details
    Returns the snap Base for this Viewport (DXF 13, 23).
    \remarks
    All angles are expressed in radians.
  */
  OdGePoint2d snapBase() const;
  /** \details
    Sets the snap Base for this Viewport (DXF 13, 23).
    \param snapBase [in]  Snap base.
    \remarks
    All angles are expressed in radians.
  */
  void setSnapBase(
    const OdGePoint2d& snapBase);
  /** \details
    Returns the snap increments for this Viewport (DXF 14, 24).
  */
    OdGePoint2d snapIncrements() const;
  /** \details
    Sets the snap increments for this Viewport (DXF 14, 24).
    \param snapIncrements [in]  Snap increments.
  */
    void setSnapIncrements(const OdGePoint2d& snapIncrements);

  /** \details
    Returns true if and only if the UCS saved with this Viewport will become
    active whenever this Viewport is made active (DXF 65).
  */
  bool isUcsSavedWithViewport() const;

  /** \details
    Controls the UcsPerViewport for this Viewport (DXF 65).
    
    \param ucsPerViewport [in]  Controls UcsPerViewport.
    \remarks
    If and only if UcsPerViewport is true, The UCS saved with this Viewport will become
    active whenever this Viewport is made active>
  */
  void setUcsPerViewport( bool ucsPerViewport);

  /** \details
    Returns true if and only if the fast zooms are enabled for this Viewport (DXF 73).
  */
  bool fastZoomsEnabled() const;
  
  /** \details
    Controls fast zooms for this Viewport (DXF 73).
    \param fastZoomsEnabled [in]  Fast zooms enabled.
  */
  void setFastZoomsEnabled(
    bool fastZoomsEnabled);

  // New R21 grid management API

  bool isGridBoundToLimits() const;
  void setGridBoundToLimits(bool enabled);
       
  bool isGridAdaptive() const;
  void setGridAdaptive(bool enabled);
       
  bool isGridSubdivisionRestricted() const;
  void setGridSubdivisionRestricted(bool enabled);
       
  bool isGridFollow() const;
  void setGridFollow(bool enabled);

  OdInt16 gridMajor() const;
  void setGridMajor(OdInt16 value);

  
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

  /** \details
    Returns the OdGsView associated with this Viewpoint.
  */
  OdGsView* gsView() const;
  /** \details
    Sets the OdGsView associated with this Viewpoint.
    \param pGsView [in]  Pointer to the GsView.
  */
  void setGsView(OdGsView* pGsView);
  
  /** \details
      Returns the OdGiDrawable for this Viewport.
  */
  OdGiDrawable* drawable();

  OdResult subGetClassID(void* pClsid) const;

  virtual void copyFrom(const OdRxObject* pSource);

  /** \details
      Adjusts the parameters in this Viewport such that the view is zoomed 
      to the extents of the drawing.
  */
  void zoomExtents();

  void subClose();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbViewportTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbViewportTableRecord> OdDbViewportTableRecordPtr;

#include "TD_PackPop.h"

#endif // _ODDBVIEWPORTTABLERECORD_INCLUDED

