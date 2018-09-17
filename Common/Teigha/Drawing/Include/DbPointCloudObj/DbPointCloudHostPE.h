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


#ifndef __OD_DB_POINCLOUD_HOST__
#define __OD_DB_POINCLOUD_HOST__

#include "DbPointCloudObjExports.h"
#include "Ge/GeExtents3d.h"

class OdDbPointCloud;
class OdDbPointCloudDef;
class OdGiWorldDraw;
class OdGiViewportDraw;
class OdDbPointCloudEx;
class OdDbPointCloudDefEx;

#include "TD_PackPush.h"

#include "StringArray.h"

/** \details
    This class represents an interface of a Point Cloud Object within the Teigha vectorization framework.

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudItem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudItem);

  /** \details
    Returns extents of Point Cloud Object.
  */
  virtual OdGeExtents3d extents() const = 0;

  /** \details
    Returns total number of points in Point Cloud Object.
  */
  virtual OdInt64 pointsCount() const = 0;


  /** \details
    Creates a viewport-independent geometric representation of a Point Cloud Entity.
    
    \returns
    Returns true if and only if the geometric representation can be generated in a
    viewport-independent manner. 

    \remarks
    A return value of false indicates that viewportDrawPoints() must be called for this object.

    Transformations and clipping boundaries are taken care of outside of this function by the Teigha framework.

    The 3D GS will call this function at least once, but may cache subsequent display updates.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pWd [in]  Pointer to the OdGiWorldDraw interface.
  */
  virtual bool worldDrawPoints(const OdDbPointCloud* pEnt, OdGiWorldDraw* pWd) const = 0;

  /** \details
    Creates a viewport-dependent geometric representation of a Point Cloud Entity.

    \remarks
    This function is called once per viewport.

    Transformations and clipping boundaries are taken care of outside of this function by the Teigha framework.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pVd [in] Pointer to the OdGiViewportDraw interface.
  */
  virtual void viewportDrawPoints(const OdDbPointCloud* pEnt, OdGiViewportDraw* pWd) const = 0;

};

typedef OdSmartPtr<OdDbPointCloudItem> OdDbPointCloudItemPtr;

/** \details
    This class represents an interface of a Point Cloud Ex Object within the Teigha vectorization framework.

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExItem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExItem);

  virtual ~OdDbPointCloudExItem() {}

  /** \details
    Returns extents of Point Cloud Object.
  */
  virtual OdGeExtents3d extents() const = 0;

  /** \details
    Returns total number of points in Point Cloud Object.
  */
  virtual OdUInt64 pointsCount() const = 0;


  /** \details
    Creates a viewport-independent geometric representation of a Point Cloud Entity.
    
    \returns
    Returns true if and only if the geometric representation can be generated in a
    viewport-independent manner. 

    \remarks
    A return value of false indicates that viewportDrawPoints() must be called for this object.

    Transformations and clipping boundaries are taken care of outside of this function by the Teigha framework.

    The 3D GS will call this function at least once, but may cache subsequent display updates.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pWd [in]  Pointer to the OdGiWorldDraw interface.
  */
  virtual bool worldDrawPoints(const OdDbPointCloudEx* pEnt, OdGiWorldDraw* pWd) const = 0;

  /** \details
    Creates a viewport-dependent geometric representation of a Point Cloud Entity.

    \remarks
    This function is called once per viewport.

    \remarks
    Transformations and clipping boundaries are taken care of outside of this function by the Teigha framework.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pVd [in] Pointer to the OdGiViewportDraw interface.
  */
  virtual void viewportDrawPoints(const OdDbPointCloudEx* pEnt, OdGiViewportDraw* pWd) const = 0;  
};

typedef OdSmartPtr<OdDbPointCloudExItem> OdDbPointCloudExItemPtr;

/** \details
<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExScanItem : public OdDbPointCloudExItem
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExScanItem);

  virtual ~OdDbPointCloudExScanItem() {}
  
  /** \details
    Returns the hasRGB flag.
  */
  virtual bool hasRGB() const = 0;
  
  /** \details
    Returns the hasNormals flag.
  */
  virtual bool hasNormals() const = 0;
  
  /** \details
    Returns the hasIntensity flag.
  */
  virtual bool hasIntensity() const = 0;

  /** \details
  */
  virtual OdString getScanId() const = 0;
};

typedef OdSmartPtr<OdDbPointCloudExScanItem> OdDbPointCloudExScanItemPtr;

/** \details
<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT ScanItemIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(ScanItemIterator);

  virtual ~ScanItemIterator(){}

  virtual void start() = 0;
  virtual void step() = 0;
  virtual bool done() const = 0;

  virtual bool isLoaded() const = 0;
  virtual bool shouldBeLoaded() const = 0;
  virtual bool notFound() const = 0;

  virtual void setNotFound(bool val) = 0;

  virtual OdString getPath() const = 0;
  virtual OdString getRelativePath() const = 0;

  virtual void setItem(OdDbPointCloudExScanItemPtr pItem) = 0;
};

typedef OdSmartPtr<ScanItemIterator> ScanItemIteratorPtr;

/** \details
<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExProjItem : public OdDbPointCloudExItem
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExProjItem);

  virtual ~OdDbPointCloudExProjItem() {}

  virtual void getAllRcsFilePaths( OdStringArray &list ) const = 0;
  virtual void getAllRcsRelativeFilePaths( OdStringArray &list ) const = 0;

  virtual OdUInt64 getTotalAmountOfScanItems() const = 0;
  virtual ScanItemIteratorPtr newScanItemIterator() const = 0;

  virtual OdUInt32 getTotalRegionsCount() const = 0;
  virtual OdUInt32 getTotalScansCount() const = 0;
  virtual OdString getCoordinateSystemName() const = 0;
    
  virtual OdInt8 hasRGB() const = 0;  
  virtual OdInt8 hasNormals() const = 0;  
  virtual OdInt8 hasIntensity() const = 0;

  virtual OdString getRcsFilePath(const OdString &guid) const = 0;
  virtual OdString getRcsRelativeFilePath(const OdString &guid) const = 0;
};

typedef OdSmartPtr<OdDbPointCloudExProjItem> OdDbPointCloudExProjItemPtr;

/** \details
  This protocol extension, attached to the OdDbPointCloudDefinition class, returns
  a pointer to an object of the class implementing the OdDbPointCloudItem interface given a file path
  to a Point Cloud file.

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudHostPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudHostPE);

  OdDbPointCloudHostPE() {}

  /** \details
    Returns the OdResult status of the operation.

    \param strPath [in] File path.
    \param item [out] Pointer to object of class implementing the OdDbPointCloudItem interface for specified file.
  */
  virtual OdResult load(const OdString& strPath, OdDbPointCloudItemPtr& item) = 0;
};

typedef OdSmartPtr<OdDbPointCloudHostPE> OdDbPointCloudHostPEPtr;

/** \details
  This protocol extension, attached to the OdDbPointCloudDefinitionEx class, returns
  a pointer to an object of the class implementing the OdDbPointCloudItem interface given a file path
  to a Point Cloud file.

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExHostPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExHostPE);

  OdDbPointCloudExHostPE() {}

  /** \details
    Returns the OdResult status of the operation.

    \param strPath [in] File path.
    \param item [out] Pointer to object of class implementing the OdDbPointCloudExItem interface for specified file.
  */
  virtual OdResult loadRcs(const OdString& strPath, OdDbPointCloudExItemPtr& item) = 0;

  virtual OdResult loadRcp(const OdString& strPath, OdDbPointCloudExItemPtr& item) = 0;
};

typedef OdSmartPtr<OdDbPointCloudExHostPE> OdDbPointCloudExHostPEPtr;

static const OdChar* szPointCloudHostModuleName = OD_T("PointCloudHost");

#include "TD_PackPop.h"

#endif // __OD_DB_POINCLOUD_HOST__
