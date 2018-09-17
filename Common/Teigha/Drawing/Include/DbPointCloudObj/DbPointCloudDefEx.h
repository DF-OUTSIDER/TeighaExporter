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

#ifndef _ODDBPOINTCLOUDDEFEX_INCLUDED_
#define _ODDBPOINTCLOUDDEFEX_INCLUDED_


#include "DbPointCloudObjExports.h"
#include "StringArray.h"
#include "TD_PackPush.h"

#include "DbObject.h"

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefEx : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS( OdDbPointCloudDefEx );

  enum PropState
  {
    kNone = -1,
    kSome = 0,
    kAll = 1
  };

  enum Property
  {
    kColor = 1,
    kIntensity = 2,
    kClassification = 3,
    kNormal = 4,
    kSegmentation = 5,
    kGeoGraphic = 6
  };

  /** \details
  Default constructor. Constructs an empty OdDbPointCloudDefEx object.
  */
  OdDbPointCloudDefEx();

  /** \details
    Returns the name of the external file containing the point cloud ex data (.rcp or .rcs) for this PointCloudEx Definition object (as it is stored in file).
    
    \remarks
    This PointCloudDefEx object must be open for reading.
  */
  OdString sourceFileName() const;

  /** \details
    Sets the name of the external file containing the point cloud ex data (.rcp or .rcs) for this PointCloudEx Definition object (as it is stored in file).
    
    \returns
    Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setSourceFileName(const OdString&);

  /** \details
  Returns the path name of the actual point cloud file being currently used for this PointCloudEx Definition object.

  \remarks
  This PointCloudDef object must be open for reading.
  */
  const OdChar * activeFileName() const;


  /** \details
  Returns the path name of the actual point cloud file being currently used for this PointCloudEx Definition object.

  \returns
  Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setActiveFileName( const OdChar * pPathName );

  /** \details
    Returns the type of the source file containing the point cloud data for this PointCloudEx Definition object (as it is stored in file).

    \remarks
    This PointCloudDefEx object must be open for reading.
  */
  const OdChar * fileType() const;

  // Helper functions to access Point Cloud Ex Definition dictionary
  /** \details
    Returns PointCloudEx Dictionary ID. Creates the dictionary, if one is not already present, in the specified OdDbDatabase instance.

    \param Db [in]  The database.
  */
  static OdDbObjectId createPointCloudExDictionary(OdDbDatabase *pDb);

  /** \details
    Returns the Object ID of the point cloud dictionary in the specified OdDbDatabase instance.
    \param Db [in]  The database.  
  */
  static OdDbObjectId pointCloudExDictionary(const OdDbDatabase *pDb);

  /** \details
    
  */
  static OdDbDictionary* getPointCloudExDictionary( OdDbPointCloudDefEx *pDefEx );

  /** \details
    Returns true if and only if the point cloud file for this PointCloudEx definition object is loaded.
  */
  bool  isLoaded() const;

  /** \details

  */
  OdResult load();

  /** \details

  */
  void unload();

  /** \details
    Returns the number of OdDbPointCloudEx entities in the current drawing that are dependent upon this object.
    
    \param pbLocked [in]  If it's non-zero, then this function will set the true value at that pointer if any dependent entities reside on locked layers, or set false otherwise.
  */
  int entityCount(bool *pbLocked) const;

  /** \details
    Returns the total number of points of this OdDbPointCloudDefEx object.
  */
  OdUInt64 totalPointsCount() const;

  /** \details
    Returns the default height of this OdDbPointCloudDefEx object.
  */
  double defaultHeight() const;

  /** \details
    Returns the default length of this OdDbPointCloudDefEx object.
  */
  double defaultLength() const;

  /** \details
    Returns the default width of this OdDbPointCloudDefEx object.
  */
  double defaultWidth() const;

  /** \details
    Calculates the extents of this OdDbPointCloudDefEx object.

    \returns
    Returns eOk if the extents are valid or eInvalidExtents otherwise.
  */
  OdResult extents( OdGeExtents3d &exts ) const;

  /** \details  
  */
  OdUInt32 totalRegionsCount() const;

  /** \details  
  */
  OdUInt32 totalScansCount() const;

  /** \details  
  */
  OdString coordinateSystemName() const;

  /** \details  
  */
  OdString getRcsFilePath( const OdString &guid) const;

  /** \details  
  */
  void getAllRcsFilePaths( OdStringArray &list ) const;

  /** \details  
  */
  PropState hasProperty( Property prop ) const;

  /** \details
  NOT INPLEMENTED
  */
  OdResult createThumbnailBitmap( BITMAPINFO*& pBmpInfo, int width, int height ) const;



  // File IO
  virtual OdResult  dwgInFields( OdDbDwgFiler * );
  virtual void  dwgOutFields( OdDbDwgFiler * ) const;
  virtual OdResult  dxfInFields( OdDbDxfFiler * );
  virtual void  dxfOutFields( OdDbDxfFiler * ) const;

  static int classVersion();

};

typedef OdSmartPtr<OdDbPointCloudDefEx> OdDbPointCloudDefExPtr;

/** <group OdDbPointCloud_Classes>
*/
class  DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefReactorEx : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS( OdDbPointCloudDefReactorEx );
  OdDbPointCloudDefReactorEx( );

  static int classVersion();

  // File IO
  virtual OdResult  dwgInFields( OdDbDwgFiler * );
  virtual void  dwgOutFields( OdDbDwgFiler * ) const;
  virtual OdResult  dxfInFields( OdDbDxfFiler * );
  virtual void  dxfOutFields( OdDbDxfFiler * ) const;

  // Notifications
  virtual void erased( OdDbObject const *, bool );   // not implemented
  virtual void modified( OdDbObject const * );

  static bool isEnabled();
  static void setEnable( bool );
private:
  static bool m_bEnabled;
};

typedef OdSmartPtr<OdDbPointCloudDefReactorEx> OdDbPointCloudDefReactorExPtr;

#include "TD_PackPop.h"

#endif //_ODDBPOINTCLOUDDEFEX_INCLUDED_
