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

#ifndef _ODDBPOINTCLOUDDEF_INCLUDED_
#define _ODDBPOINTCLOUDDEF_INCLUDED_ /* { Secret} */

#include "DbPointCloudObjExports.h"
#include "TD_PackPush.h"

#include "DbObject.h"

/** \details
    This class defines Point Cloud Definition objects in an OdDbDatabase instance.

    \sa
    AcDbPointCloudObj

    \remarks
    Point Cloud Definitions (OdDbPointCloudDef objects) work with Point Cloud (OdDbPointCloud) entities
    in much the same way that Raster Image Definitions (OdDbRasterImageDef objects) work with Image References
    (OdDbRasterImage entities).

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDef : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbPointCloudDef);

  /** \details
    Default constructor. Constructs an empty OdDbPointCloudDef object.
  */
  OdDbPointCloudDef();

  // Properties
  /** \details
    Returns the name of the external file containing the point cloud data (.pcg or .isd) for this PointCloud Definition object (as it is stored in file).
    
    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString sourceFileName() const;

  /** \details
    Sets the name of the external file containing the point cloud data (.pcg or .isd) for this PointCloud Definition object (as it is stored in file).
    
    \returns
    Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setSourceFileName(const OdString&);

  /** \details
    Returns the type of the source file containing the point cloud data for this PointCloud Definition object (as it is stored in file).

    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString  fileType(void) const;


  /** \details
    Returns the path name of the actual point cloud file being currently used for this PointCloud Definition object.

    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString activeFileName() const;


  /** \details
    Returns the path name of the actual point cloud file being currently used for this PointCloud Definition object.

    \returns
    Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setActiveFileName(const OdString&);

  
  /** \details
    Returns true if and only if the point cloud file for this PointCloud definition object is loaded.
  */
  bool  isLoaded() const;


  /** \details
    Currently not implemented.
  */
  OdResult load(bool bModifyDatabase);

  /** \details
    Currently not implemented.
  */
  void unload(bool bModifyDatabase);

  /** \details
    Returns the number of OdDbPointCloud entities in the current drawing that are dependent upon this object.
    
    \param pbLocked [in]  If it's non-zero, then this function will set the true value at that pointer if any dependent entities reside on locked layers, or set false otherwise.
  */
  int entityCount(bool *) const;

  /** \details
    Returns the total number of points of this OdDbPointCloudDef object.
  */
  OdUInt64 totalPointsCount() const;

  /** \details
    Returns the default height of this OdDbPointCloudDef object.
  */
  double defaultHeight() const;

  /** \details
    Returns the default length of this OdDbPointCloudDef object.
  */
  double defaultLength() const;

  /** \details
    Returns the default width of this OdDbPointCloudDef object.
  */
  double defaultWidth() const;


  /** \details
    Calculates the extents of this OdDbPointCloudDef object.

    \returns
    Returns eOk if the extents are valid or eInvalidExtents otherwise.
  */
  OdResult  getNativeCloudExtent(OdGeExtents3d &) const;

  /** \details
    Sets the extents for this OdDbPointCloudDef object.

    \param exts [in]  Extents to be set.
  */
  void setExtents(const OdGeExtents3d &);

  // File IO

  virtual OdResult  dwgInFields(OdDbDwgFiler *);
  virtual void  dwgOutFields(OdDbDwgFiler *) const;
  virtual OdResult  dxfInFields(OdDbDxfFiler *);
  virtual void  dxfOutFields(OdDbDxfFiler *) const;

  static int classVersion();

  // Helper functions to access Point Cloud Definition dictionary
  /** \details
    Returns Pointcloud Dictionary ID. Creates the dictionary, if one is not already present, in the specified OdDbDatabase instance.

    \param Db [in]  The database.
  */
  static OdDbObjectId createPointCloudDictionary(OdDbDatabase&);

  /** \details
    Returns the Object ID of the point cloud dictionary in the specified OdDbDatabase instance.
    \param Db [in]  The database.  
  */
  static OdDbObjectId pointCloudDictionary(const OdDbDatabase&);

#if 0
  OdResult  createThumbnailBitmap(struct tagBITMAPINFO * &,int,int) const;
  bool  getPointCloudFileVersion(int &,int &) const;
  bool  hasProperty(const OdString&,bool &) const;
  bool  saveToVersion(const OdString&,int);
#endif

// OdDbObject overridden methods
  virtual OdResult subErase(bool erasing) ODRX_OVERRIDE;
  virtual void subClose() ODRX_OVERRIDE;
  virtual void subHandOverTo(OdDbObject* pNewObj) ODRX_OVERRIDE;
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary) const ODRX_OVERRIDE;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPointCloudDef object pointers.
*/
typedef OdSmartPtr<OdDbPointCloudDef> OdDbPointCloudDefPtr;


/** \details
  This class represents the point cloud definition reactor.
 
  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class  DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefReactor : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbPointCloudDefReactor);
	OdDbPointCloudDefReactor();

	static int classVersion();

  // File IO
  virtual OdResult  dwgInFields(OdDbDwgFiler *);
  virtual void  dwgOutFields(OdDbDwgFiler *) const;
  virtual OdResult  dxfInFields(OdDbDxfFiler *);
  virtual void  dxfOutFields(OdDbDxfFiler *) const;

  // Notifications
  virtual void erased(OdDbObject const * , bool) ;   // not implemented
	virtual void modified(OdDbObject const * ) ;

	static bool isEnabled();
	static void setEnable(bool);
private:
  static bool m_bEnabled;
};
typedef OdSmartPtr<OdDbPointCloudDefReactor> OdDbPointCloudDefReactorPtr;


#include "TD_PackPop.h"

#endif
