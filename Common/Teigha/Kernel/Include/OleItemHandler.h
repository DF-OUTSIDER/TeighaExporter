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

#ifndef _ODOLEITEMHANDLER_INCLUDED_
#define _ODOLEITEMHANDLER_INCLUDED_

#include "RxObject.h"
#include "OdErrorContext.h"
#include "Gi/GiViewportGeometry.h"
#include "RxModule.h"
#include "OdStreamBuf.h"

#include "TD_PackPush.h"

class COleClientItem;

/** \details
    This class is the base class for classes that provide platform-dependent I/O for OLE items for Teigha.

    <group Other_Classes>
*/
class DBROOT_EXPORT OdOleItemInitStream : public OdStreamBuf
{
public:
  ODRX_DECLARE_MEMBERS(OdOleItemInitStream);

  /** \details
    Returns the Object ID of the frame associated with this OdOleItemInitStream object.
  */
  virtual OdDbStub* frameId() const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdOleItemInitStream object pointers.
*/
typedef OdSmartPtr<OdOleItemInitStream> OdOleItemInitStreamPtr;

class OdOleItemHandler;
class OdDbObjectId;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdOleItemHandler object pointers.
*/
typedef OdSmartPtr<OdOleItemHandler> OdOleItemHandlerPtr;

/** \details
    This class defines the interface for custom OLE handlers.

    <group Other_Classes>
*/
class DBROOT_EXPORT OdOleItemHandler : public OdGiSelfGdiDrawable
{
public:
  ODRX_DECLARE_MEMBERS(OdOleItemHandler);

  /** \details
    Reads the binary data for the OdDbOle2Frame of this OleItemHandler object from the specified stream.
    
    \param streamBuf [in]  StreamBuf object from which the data are to be read.

    \remarks
    See MFC COleClientItem::Serialize(CArchive& ar) for details.
    Assume that ar.m_bForceFlat is TRUE.
  */
  virtual void load(OdStreamBuf& streamBuf) = 0;

  /** \details
    Writes the binary data for the OdDbOle2Frame of this OleItemHandler object to the specified stream.
    
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    
    \remarks
    See MFC COleClientItem::Serialize(CArchive& ar) for details.
    Assume that ar.m_bForceFlat is TRUE.
  */
  virtual void save(OdStreamBuf& streamBuf) const = 0;

  /** \details
    Returns the data size (in bytes) of the compound document of this OleItemHandler object.
  */
  virtual OdUInt32 getCompoundDocumentDataSize() const = 0;

  /** \details
    Writes the compound document data of this OleItemHandler object to the specified stream.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
  */
  virtual void getCompoundDocument(OdStreamBuf& streamBuf) const = 0;

  /** \details
    Reads the compound document data of this OleItemHandler object from the specified stream.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
    \param numBytes [in]  Number of bytes to be written.
  */
  virtual void setCompoundDocument(OdUInt32 numBytes, OdStreamBuf& streamBuf) = 0;

  enum Type
  {
    kUnknown     = 0,
    kLink        = 1,
    kEmbedded    = 2,
    kStatic      = 3
  };
  /** \details
    Returns the type of the OdDbOleFrame of this OdOleItemHandler.
    
    \remarks
    type() returns one of the following:
    
    <table>
    Name          Value
    kUnknown      0
    kLink         1
    kEmbedded     2
    kStatic       3
    </table>
  */
  virtual Type type() const = 0;

  enum DvAspect
  {
    kContent    = 1,
    kThumbnail  = 2,
    kIcon       = 4,
    kDocPrint   = 8
  };

  /** \details
    Returns the draw aspect of this OleItemHandler object.
    
    \remarks
    drawAspect() returns one of the following:
    
    <table>
    Name          Value
    kContent      1 
    kThumbnail    2 
    kIcon         4 
    kDocPrint     8
    </table>
  */
  virtual DvAspect drawAspect() const = 0;

  /** \details
    Returns the link name of this OleItemHandler object.
  */
  virtual OdString linkName() const = 0;

  /** \details
    Returns the link path of this OleItemHandler object.
  */
  virtual OdString linkPath() const = 0;

  /** \details
    Returns the user type of this OleItemHandler object.
  */
  virtual OdString userType() const = 0;

  /** \details
    Sets the draw aspect of this OleItemHandler object.
    \param drawAspect [in]  Draw aspect.

    \remarks
    drawAspect must be one of the following:
    
    <table>
    Name          Value
    kContent      1 
    kThumbnail    2 
    kIcon         4 
    kDocPrint     8
    </table>
  */
  virtual void setDrawAspect(DvAspect drawAspect) = 0;

  enum PlotQuality
  {
    kMonochrome   = 0,  // e.g., spreadsheet
    kLowGraphics  = 1,  // e.g., color text & pie charts
    kHighGraphics = 2,  // e.g., photograph 
    kAutomatic    = 3   // Automatic dependent on document type
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
  kAutomatic         3        Automatic dependent on document type
  </table>
  */
  virtual PlotQuality outputQuality() const = 0;

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
  kAutomatic         3        Automatic dependent on document type
  </table>
  */
  virtual void setOutputQuality(PlotQuality  quality) = 0;

  virtual COleClientItem* getOleClientItem(void) const = 0;
  /** \details
    Embeds a raster image to this OLE object.
    
    \param pImage [in] Pointer to a raster image. 
    \param pRxDb [in] Pointer to a database object.
    
    \returns Returns true if a raster image is successfully embedded to the OLE object, 
    or returns false in the other case.
  */  virtual bool embedRaster(OdSmartPtr<class OdGiRasterImage> pImage,
                           OdRxObject* pRxDb = NULL) = 0;
  
  virtual OdGiRasterImagePtr getRaster(bool bDisplayedOnly = true) const = 0;
};

#include "TD_PackPop.h"

#endif // _ODOLEITEMHANDLER_INCLUDED_
