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




#ifndef _OD_DB_OLE2FRAME_
#define _OD_DB_OLE2FRAME_

#include "TD_PackPush.h"

#include "DbOleFrame.h"
#include "Ge/GePoint3d.h"

class OdOleItemHandler;
class COleClientItem;

/** <group OdDb_Classes>
*/
class OdRectangle3d
{
public:
  OdGePoint3d upLeft;
  OdGePoint3d upRight;
  OdGePoint3d lowLeft;
  OdGePoint3d lowRight;
};

/** \details
    This class represents OLE2 entities in an OdDbDatabase instance.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbOle2Frame : public OdDbOleFrame
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbOle2Frame);

  OdDbOle2Frame();
  
  OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  ODRX_SEALED_VIRTUAL bool subWorldDraw(
    OdGiWorldDraw* pWd) const ODRX_OVERRIDE;

  ODRX_SEALED_VIRTUAL void subViewportDraw(
    OdGiViewportDraw* pVd) const ODRX_OVERRIDE;

  ODRX_SEALED_VIRTUAL OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;
  
  /** \details
    Returns the upper-left corner of the OLE object (DXF 10).
    
    \param point3d [out]  Receives the upper-left corner.
  */
  void getLocation(OdGePoint3d& 
    point3d) const;

  /** \details
    Returns the corner points of the OLE object (DXF 10 & 11).
    
    \param rect3d [out]  Receives the corner points.
  */
  void position(
    OdRectangle3d& rect3d) const;
  
  /** \details
      Sets the corner points of the OLE object (DXF 10 & 11).
    
    \param rect3d [in]  Corner points.
  */
  void setPosition(
    const OdRectangle3d& rect3d);

  /** \details
    Returns the user type-string for the OLE object (DXF 3).
    
    Examples:  
    "Word Document"  
    "Excel Chart"  
  */
  OdString getUserType() const;
  
  enum Type
  {
      kUnknown  = 0,
      kLink     = 1,
      kEmbedded = 2,
      kStatic   = 3
  };
  /** \details
    Returns the type of this OLE object (DXF 71).  
    
    \remarks
    getType() returns one of the following:
    
    <table>
    Name         Value
    kUnknown     0
    kLink        1
    kEmbedded    2
    kStatic      3
    </table>
  */
  Type getType() const;

  /** \details
    Returns the filename and item to which this OLE object is linked.
    
    Example:
    D:/My Documents/My Workbook.xls!Sheet2!R1C1:R10C10

    \sa
    getLinkPath()
  */
  OdString getLinkName() const;
  
  /** \details
    Returns the filename to which this OLE object is linked.

    Example:
    D:/My Documents/My Workbook.xls
             
    \sa
    getLinkName()
  */
  OdString getLinkPath() const;
  
  enum PlotQuality
  {
      kMonochrome   = 0,  // e.g., spreadsheet
      kLowGraphics  = 1,  // e.g., color text & pie charts
      kHighGraphics = 2,  // e.g., photograph 
      kAutomatic    = 3   // e.g., Automatic
  };

  /** \details
    Returns the output quality for this OLE object (DXF 73).
    
    \remarks
    Controls the color depth and resolution when plotted.
    
    outputQuality() returns one of the following:

    <table>
    Name               Value    Example
    kMonochrome        0        Spreadsheet
    kLowGraphics       1        Color text & pie charts
    kHighGraphics      2        Photograph 
    kAutomatic         3        Automatic
    </table>
  */
  PlotQuality outputQuality() const;

  /** \details
    Sets the output quality for this OLE object (DXF 73).
    
    \param quality [in]  Output quality.
      
    \remarks
    Controls the color depth and resolution when plotted.

    quality must be one of the following:
    
    <table>
    Name               Value    Example
    kMonochrome        0        Spreadsheet
    kLowGraphics       1        Color text & pie charts
    kHighGraphics      2        Photograph 
    kAutomatic         3        Automatic
    </table>
  */
  void setOutputQuality(
    PlotQuality  quality);

  /** \details
    Returns the compound document data size for this OLE object.
  */
  OdUInt32 getCompoundDocumentDataSize() const;

  /** \details
    Writes the compound document data to the specified stream.
    
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
  */
  void getCompoundDocument(
    OdStreamBuf& streamBuf) const;

  /** \details
    Reads the compound document data from the specified stream.
    
    \param nSize [in]  Size of compound data.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  void setCompoundDocument(
    OdUInt32 nSize, 
    OdStreamBuf& streamBuf);

  OdResult subGetClassID(void* pClsid) const;
  
  /** \details
    Returns the Item Handler for this OLE object.
  */
  const OdOleItemHandler* itemHandler() const;

  
  /** \details
    Creates an Item Handler for this OLE object.
    
    \returns
    Returns the Item Handler.
  */
  OdOleItemHandler* getItemHandler();

  void subClose();

  /** \details
    Returns unknown field.

    \remarks
    Was not accessible in DD1.12 (OdDbOle2FrameImpl::m_UnknownFromDWG)

    \remarks
    Currently Teigha does not handle this field.
    This method will be removed or renamed in future releases.
  */
  OdInt32 unhandled_unknown0() const;

  /** \details
    Returns unknown field.

    \remarks
    Still unknown. OdDbOle2Frame::getUnknown1() in DD1.12

    \remarks
    Currently Teigha does not handle this field.
    This method will be removed or renamed in future releases.
  */
  OdUInt8 unhandled_unknown1() const;

  /** \details
    Returns unknown field.

    \remarks
    Still unknown. OdDbOle2Frame::getUnknown2() in DD1.12

    \remarks
    Currently Teigha does not handle this field.
    This method will be removed or renamed in future releases.
  */
  OdUInt8 unhandled_unknown2() const;

  /** \details
    Sets unknown fields.

    \remarks
    Currently Teigha does not handle these fields.
    This method will be removed or renamed in future releases.
  */
  void unhandled_setUnknown(OdInt32 unk0 = 0, OdUInt8 unk1 = 0x80, OdUInt8 unk2 = 0x55);

  /** \details

    \remarks
    OdDbOle2Frame::getPixelWidth() in DD1.12

    \remarks
    Currently Teigha does not handle this field.
    This method will be removed or renamed in future releases.
  */
  OdUInt16 unhandled_himetricWidth() const;

  /** \details

    \remarks
    OdDbOle2Frame::getPixelHeight() in DD1.12

    \remarks
    Currently Teigha does not handle this field.
    This method will be removed or renamed in future releases.
  */
  OdUInt16 unhandled_himetricHeight() const;

  /** \details
    Sets unhandled fields.

    \remarks
    Currently Teigha does not handle these fields.
    This method will be removed or renamed in future releases.
  */
  void unhandled_setHimetricSize(OdUInt16 w, OdUInt16 h);

  // New properties

  /** \details
    Resets current OLE XData.

    \param bInitSize [in]  Init actual OLE object size using current WCS size.
  */
  void reset(bool bInitSize = false);

  /** \details
    Returns automatically selectable by application plot quality.
  */
  bool autoOutputQuality() const;

  /** \details
    Sets plot quality to automatically selectable by application.

    \param bFlag [in]  New value.
  */
  void setAutoOutputQuality(bool bFlag);

  /** \details
    Returns rotation angle for this OLE object.
  */
  double rotation() const;

  /** \details
    Sets rotation angle for this OLE object.

    \param fAngle [in]  New value.
  */
  void setRotation(double fAngle);

  /** \details
    Returns actual width of this OLE object in WCS.
  */
  double wcsWidth() const;

  /** \details
    Sets actual width of this OLE object in WCS.

    \param fWidth [in]  New OLE object WCS width.
  */
  void setWcsWidth(double fWidth);

  /** \details
    Returns actual height of this OLE object in WCS.
  */
  double wcsHeight() const;

  /** \details
    Sets actual height of this OLE object in WCS.

    \param fHeight [in]  New OLE object WCS height.
  */
  void setWcsHeight(double fHeight);

  /** \details
    Returns percentage of the current OLE object width scale to original object width scale.
  */
  double scaleWidth() const;

  /** \details
    Sets new OLE object width scale relative to original object width.

    \param fScale [in]  New width scale in percents.
  */
  void setScaleWidth(double fScale);

  /** \details
    Returns percentage of the current OLE object height scale to original object height scale.
  */
  double scaleHeight() const;

  /** \details
    Sets new OLE object height scale relative to original object height.

    \param fScale [in]  New height scale in percents.
  */
  void setScaleHeight(double fScale);

  /** \details
    Returns lock aspect property.
  */
  bool lockAspect() const;

  /** \details
    Sets lock aspect property.

    \param bFlag [in]  New value.
  */
  void setLockAspect(bool bLockAspect);

  /** \details
    Returns pointer to MFC COleClientItem object.

    \returns
    Returns non-Null pointer only if OleItemHandler implementation is MFC-based and supports this method.
  */
  COleClientItem * getOleClientItem() const;

  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;
  void subSetDatabaseDefaults(OdDbDatabase *pDb, bool);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbOld2Frame object pointers.
*/
typedef OdSmartPtr<OdDbOle2Frame> OdDbOle2FramePtr;

#include "TD_PackPop.h"

#endif
