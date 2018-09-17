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




#ifndef OD_DBLAYOUT_H
#define OD_DBLAYOUT_H

#include "TD_PackPush.h"

#include "DbPlotSettings.h"
#include "DwgDeclareMembers.h"

class OdGiRasterImage;
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

class OdString;

/** \details
    This class represents Layout objects in an OdDbDatabase instance.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbLayout : public OdDbPlotSettings
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLayout);

  /** \details
      Constructor (no arguments).
  */
  OdDbLayout();

  ///////// BEGIN ODA SPECIFIC /////////////

  /** \details
    Returns the minimum limits for this Layout object (DXF 10).
  */
  const OdGePoint2d& getLIMMIN() const;

  /** \details
    Sets the minimum limits for this Layout object (DXF 10).
    
    \param limMin [in]  Minimum limits.
  */
  void setLIMMIN(
    const OdGePoint2d& limMin);

  /** \details
    Returns the maximum limits for this Layout object (DXF 11).
  */
  const OdGePoint2d& getLIMMAX() const;

  /** \details
    Sets the maximum limits for this Layout object (DXF 11).

    \param limMax [in]  Maximum limits.
  */
  void setLIMMAX(
    const OdGePoint2d& limMax);

  /** \details
    Returns the minimum extents for this Layout object (DXF 14).
  */
  const OdGePoint3d& getEXTMIN() const;

  /** \details
    Sets the minimum extents for this Layout object (DXF 14).
    
    \param extMin [in]  Minimum extents.
  */
  void setEXTMIN(
    const OdGePoint3d& extMin);

  /** \details
    Returns the maximum extents for this Layout object (DXF 15).
  */
  const OdGePoint3d& getEXTMAX() const;

  /** \details
    Sets the maximum extents for this Layout object (DXF 15).
    
    \param extMax [in]  Maximum extents.
  */
  void setEXTMAX(
    const OdGePoint3d& extMax);

  /** \details
    Returns the insertion base for this Layout object (DXF 12).
  */
  const OdGePoint3d& getINSBASE() const;

  /** \details
    Sets the insertion base for this Layout object (DXF 12).

    \param insBase [in]  Insertion base.
  */
  void setINSBASE(
    const OdGePoint3d& insBase);

  /** \details
    Returns the LIMCHECK variable for this Layout object (DXF 70, bit 0x02).
  */
  bool getLIMCHECK() const;

  /** \details
    Sets the LIMCHECK variable for this Layout object (DXF 70, bit 0x02).
    
    \param limCheck [in]  LIMCHECK variable.  
  */
  void setLIMCHECK(
    bool limCheck);

  /** \details
    Returns the PSLTSCALE variable for this Layout object (DXF 70, bit 0x01).
  */
  bool getPSLTSCALE() const;

  /** \details
    Sets the PSLTSCALE variable for this Layout object (DXF 70, bit 0x01).
      
    \param psLtScale [in]  PSLTSCALE variable.
  */
  void setPSLTSCALE(
    bool psLtScale);

  ///////// END ODA SPECIFIC /////////////

  
  /** \details
    Returns the Object ID of the PaperSpace OdDbBlockTableRecord associated with this Layout object (DXF 330).
  */
  OdDbObjectId getBlockTableRecordId() const;

  /** \details
    Associates this Layout object with the specified PaperSpace OdDbBlockTableRecord (DXF 330).
    
    \param blockTableRecordId [in]  Block table record Object Id.
  */
  virtual void setBlockTableRecordId(
    const OdDbObjectId& blockTableRecordId);

  /** \details
    Adds this Layout object to the layout dictionary in the specified database, and associates 
    this Layout object with the specified PaperSpace OdDbBlockTableRecord. 

    \param pDb [in]  Pointer to the database
    \param blockTableRecordId [in]  Block table record Object Id.
  */
  virtual void addToLayoutDict(
    OdDbDatabase* pDb, 
    OdDbObjectId blockTableRecordId);
  
  /** \details
    Returns the name of this Layout object (DXF 1).
  */
  OdString getLayoutName() const;

  /** \details
    Sets the name of this Layout object (DXF 1).
      
    \param layoutName [in]  Layout name.  
  */
  virtual void setLayoutName(const OdString& layoutName);
  
  /** \details
    Returns the tab order for this Layout object (DXF 71).
    
    \remarks
    The tab order determines the order in which layout tabs are to be displayed.
  */
  int getTabOrder() const;

  /** \details
    Sets the tab order for this Layout Object (DXF 71).
    
    \param tabOrder [in]  Tab order.

    \remarks
    The tab order determines the order in which layout tabs are to be displayed.
    
    Tab order should be sequential for all Layout objects in the database.
  */
  virtual void setTabOrder(
    int tabOrder);
  
  /** \details
    Returns true if and only if this layout tab is selected.
  */
  bool getTabSelected() const;

  /** \details
    Controls the selected status for this Layout object.
    
    \param tabSelected [in]  Controls selected status. 
    
    \remarks
    Selected Layout objects are included in selection sets for operations effecting multiple layouts.
  */
  virtual void setTabSelected(
    bool tabSelected);

  
  /** \details
    Returns an array of OdDbObjectIds of all the OdDbViewports that exist in the Paper Space layout.
    The first ID in the list will be the overall Paper Space viewport.
    If the list returned is empty, then this layout is Model Space or has never been switched to (activated).
  */
  OdDbObjectIdArray getViewportArray() const;

  /** \details
    Returns the thumbnail bitmap associated with this layout object.

    \remarks
    The thumbnail is in Windows BITMAPINFO* format.

    \param dataLength [out]  Receives the data length of the thumbnail.
  */
  const void* thumbnailBitmap(
    OdUInt32& dataLength) const;

  /** \details
    Sets the thumbnail bitmap associated with this layout object.

    \remarks
    The thumbnail is in Windows BITMAPINFO* format.

    \param dataLength [in]  Data length of the thumbnail.
    \param pBMPData [in]  Pointer to the bitmap data.
  */
  void setThumbnailBitmap(
    const void* pBMPData,
    OdUInt32 dataLength);

  /** \details
    Returns the preview image associated with the layout object.

    \remarks
    Layout previews are stored as .png images. To create layout preview Teigha requires Raster Services module supporting .png format.
  */
   OdGiRasterImagePtr getPreviewImage () const;

  /** \details
    Sets a new preview image for the layout object.

    \param image [in]  A pointer to an OdGiRasterImage object to be set as a new preivew thumbnail.

    \remarks
    Layout previews are stored as .png images. To create layout preview Teigha requires Raster Services module supporting .png format.
  */
   void setPreviewImage (
      const OdGiRasterImage* image);

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual void appendToOwner (
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Returns the Object ID of the viewport that was last active
    in this Layout object (DXF 331).
  */
  OdDbObjectId activeViewportId() const;

  /** \details
    Makes the specified viewport the active viewport of this Layout object (DXF 331).

    \param viewportId [in]  Object ID of the active viewport.      
  */
  void setActiveViewportId(
    OdDbObjectId viewportId);

  /** \details
      Returns the Object ID of the overall viewport of this Layout object.
  */
  OdDbObjectId overallVportId() const;

  OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  void subViewportDraw(
    OdGiViewportDraw* vPd) const;

  OdResult getGeomExtents(OdGeExtents3d& extents) const;

  void subClose();

  bool annoAllVisible() const;

  void setAnnoAllVisible(bool newVal);
};

// !!! SYMBOL RENAMED !!! USE activeViewportId()

#define lastActiveVportId() activeViewportId()

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbLayout object pointers.
*/
typedef OdSmartPtr<OdDbLayout> OdDbLayoutPtr;

#include "TD_PackPop.h"

#endif

