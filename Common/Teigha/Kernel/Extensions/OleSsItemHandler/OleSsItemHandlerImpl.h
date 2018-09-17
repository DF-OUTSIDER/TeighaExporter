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

// OdOleSsItemHandler.h: interface for the OdOleSsItemHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ODOLESSITEMHANDLERIMPL_INCLUDED_)
#define _ODOLESSITEMHANDLERIMPL_INCLUDED_

#include "TD_PackPush.h"

#include "OdBinaryData.h"
#include "OleItemHandler.h"
#include "OleItemHandlerBase.h"

/** \details
  This class portable implements an OLE handler.
  Library: Source code provided.
  <group Extension_Classes> 
*/
class OdOleSsItemHandlerImpl : public OdOleItemHandlerBase
{
  OdBinaryData m_buff;
  OdGiRasterImagePtr m_pImage;
  OdBinaryData m_wmfData; // TODO render it if m_pImage.isNull()
  OdOleItemHandler::Type m_type;
  OdString m_sUserType,
           m_sLinkPath;
public:
  ODRX_DECLARE_MEMBERS(OdOleSsItemHandlerImpl);
  /** \details
    Default constructor.
  */
  OdOleSsItemHandlerImpl();
  /** \details
    Destructor.
  */
  ~OdOleSsItemHandlerImpl();
  /** \details
    Returns the data size (in bytes) of the compound document of the OdOleItemHandler object.
  */
  OdUInt32 getCompoundDocumentDataSize() const;
  
  /** \details
    Writes the compound document data of this OdOleItemHandler object to the specified stream.
    
    \param streamBuf [out] StreamBuf object to which the data are to be written. 
  */
  void getCompoundDocument(OdStreamBuf& streamBuf) const;

  /** \details
    Reads the compound document data of this OdOleItemHandler object from the specified stream.
    
    \param nDataSize [in] Number of bytes to be written. 
    \param stream [out] StreamBuf object from which the data are to be read.
  */
  void setCompoundDocument(OdUInt32 nDataSize, OdStreamBuf& stream);

  // OdGiSelfGdiDrawable:
  /** \details
    This function is called to have a GDI self-drawable to draw itself.
    Returns false if implementation is missing or in error case.
    \param drawObj [in] Object to be drawn. 
    \param hdc [in] Handle to the device context.
    \param rect [in] Screen rectangle in device coordinates (pixels). 
  */
  bool draw(const OdGiCommonDraw& drawObj, void* hdc, const OdGsDCRect& rect) const;
  // OdGiSelfGiDrawablePE: 
  /** \details
    Returns false to create HDC and draw via OdGiSelfGdiDrawable interface.
    
    \param pDestGeom [in] Pointer to the Conveyor object. 
    \param origin [in] Drawn object's origin.
    \param u [in] Drawn object's width vector.
    \param v [in] Drawn object's height vector.
    \param rect [in] Screen rectangle in device coordinates (pixels).
    \param isExport [in] Boolean flag.
  */
  bool draw(OdGiConveyorGeometry* pDestGeom,
            const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
            const OdGsDCRect& rect, bool isExport = false);
  
  /** \details
    Returns the type of the OdDbOleFrame of this OdOleItemHandler object.
    
    \remarks
    type() returns one of the following:
    <table>
      Name          Value
      kUnknown        0
      kLink           1
      kEmbedded       2
      kStatic         3
    </table>
  */
  Type type() const;
  
  /** \details
    Returns the  user type of this OleItemHandler object.
  */
  OdString userType() const;
  
  /** \details
    Returns the link name of this OleItemHandler object.
  */
  OdString linkName() const;
  
  /** \details
    Returns the link path of this OleItemHandler object.
  */
  OdString linkPath() const;
  
  /** \details
    Returns the draw aspect of this OdOleItemHandler object.
    
    \remarks
    drawAspect() returns one of the following:
    <table>
      Name          Value
      kContent        1
      kThumbnail      2
      kIcon           4
      kDocPrint       8
    </table>
  */
  DvAspect drawAspect() const;
  /** \details
    Embeds a raster image to this OdOleItemHandler object.
    
    \param pImage [in] Path to a raster image. 
    \param pRxDb [in] Pointer to a database object.
    
    \returns Returns true if a raster image is successfully embedded to the OdOleItemHandler object, 
    or returns false in the other case.
  */
  bool embedRaster(OdGiRasterImagePtr pImage,
                   OdRxObject* pRxDb = NULL);
  //bool embedRasterFromFile(const OdString& path);

  // Returns OdGiRasterImagePtr() if there is only WMF data are present.
  OdGiRasterImagePtr getRaster(bool) const;
  // Some OLE entities contain only WMF previews (is not raster), for example, entities with Microsoft Excel® content.
  // Teigha does not have the cross-platform ability to draw WMFs now.
  bool getWmfData(OdBinaryData& data) const;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdOleSsItemHandlerImpl object pointers.
*/
typedef OdSmartPtr<OdOleSsItemHandlerImpl> OdOleSsItemHandlerImplPtr;

/** \details
  This class represents a protocol extension for OdOleItemHandler objects.
  Library: Source code provided.
  <group Extension_Classes> 
*/
class OdGiSelfGiDrawablePEImpl : public OdGiSelfGiDrawablePE
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSelfGiDrawablePEImpl);
  /** \details
    Returns false to create HDC and draw via OdGiSelfGdiDrawable interface.
    
    \param pRxObject [in] Pointer to OdRxObject object.
    \param pDestGeom [in] Pointer to the Conveyor object. 
    \param origin [in] Drawn object's origin.
    \param u [in] Drawn object's width vector.
    \param v [in] Drawn object's height vector.
    \param rect [in] Screen rectangle in device coordinates (pixels).
    \param isExport [in] Boolean flag.
  */
  virtual bool draw(OdRxObject* pRxObject, 
                    OdGiConveyorGeometry* pDestGeom,
                    const OdGePoint3d& origin,
                    const OdGeVector3d& u, const OdGeVector3d& v,
                    const OdGsDCRect& rect,
                    bool isExport = false) const;
};

#include "TD_PackPop.h"

#endif // !defined(_ODOLESSITEMHANDLERIMPL_INCLUDED_)
