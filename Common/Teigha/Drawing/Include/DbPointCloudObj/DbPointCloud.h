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


#ifndef _OD_DB_POINTCLOUD_
#define _OD_DB_POINTCLOUD_

#include "DbPointCloudObjExports.h"
#include "DbPointCloudClipping.h"
#include "DbPointCloudIntensityStyle.h"
#include "TD_PackPush.h"

#include "DbEntity.h"

/** \details
    This class represents PointCloud entities in an OdDbDatabase instance.

    Corresponding C++ library: AcDbPointCloudObj

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloud : public OdDbEntity
{
public:

  ODDB_DECLARE_MEMBERS(OdDbPointCloud);

  /** \details
    Default constructor.
  */
  OdDbPointCloud();

  /** \details
    Destructor.
  */
  ~OdDbPointCloud() {}

  // Properties

  /** \details
    Returns the transformation matrix for this point cloud entity.
  */
  OdGeMatrix3d transformation() const;

 /** \details
    Sets the transformation matrix for this point cloud entity.

    \param transformation [in]  The transformation matrix to be applied for this point cloud entity.
 */
  OdResult setTransformation(const OdGeMatrix3d&);

  /** \details
    Gets the name of the indexed point cloud file (.pcg or .isd) for this point cloud entity stored in file.

    \param fileName [out]  The filename.
  */
  bool  getSavedFileName(OdString &) const;

  /** \details
    Sets the name of the indexed point cloud file (.pcg or .isd) for this point cloud entity stored in file.

    \param fileName [in]  The filename to be set.

    \remarks
    To be deprecated. Updating PointCloudDef object will update all Point Cloud Entities if PointCloudDefReactor is enabled.
  */
  bool  setSavedFileName(const OdString&);

  /** \details
    Gets the current file path of the indexed point cloud file (.pcg or .isd) for this point cloud entity located during run-time session (not saved in file).

    \param fileName [out]  The filename.
  */
  bool  getIndexFile(OdString &) const;

  /** \details
    Sets the current file path of the indexed point cloud file (.pcg or .isd) for this point cloud entity for current run-time session (not saved in file).

    \param fileName [in]  The filename to be set.

    \remarks
    To be deprecated. Updating PointCloudDef object will update all Point Cloud Entities if PointCloudDefReactor is enabled.
  */
  bool  setIndexFile(const OdString&);

  /** \details
    Gets the count of the source point cloud files (.las for example) for this point cloud entity.
  */
  OdUInt32 getSourceFileCount() const;

  /** \details
    Gets the name of the source point cloud file for this point cloud entity.

    \param fileName [out]  The filename.
    \param index [in]  index of the filenames array

    Returns true if name was successfully set and false if index is invalid.
  */
  bool getSourceFile(OdUInt32 index, OdString & fileName) const;

  /** \details
    Clears source files for this point cloud entity.

  */
  void clearSourceFiles();

  /** \details
    Adds the name of the source point cloud file for this point cloud entity.

    \param fileName [in]  The filename to be added.

  */
  bool addSourceFile(const OdString& fileName );


  OdString getCoordSys() const;
  void setCoordSys(const OdString&);

  /** \details
    Returns true if the point cloud entity is locked or false otherwise.
  */
  bool  locked() const;

  /** \details
    Sets the point cloud entity to be locked.
  */
  void  setLocked(bool);

  bool useEntityColor()const;
  void setUseEntityColor(bool);

  /** \details
    Returns the Object ID of the OdDbPointCloudDef object associated with this point cloud entity.
  */
  OdDbObjectId  pointCloudDefId() const;

  /** \details
    Sets the Object ID of the OdDbPointCloudDef object associated with this point cloud entity.

    \param pointCloudDefId [in]  Object ID of the point cloud to be associated with this point cloud entity.
  */
  void  setPointCloudDefId(const OdDbObjectId);

  /** \details
    Returns the Object ID of the OdDbPointCloudDefReactor object associated with this point cloud entity.

    \remarks
    Each OdDbPointCloud object must have an OdDbPointCloudDefReactor associated with the OdDbPointCloudDef object it is using.

  */
  OdDbObjectId  reactorId() const;

  /** \details
    Sets the Object ID of the OdDbPointCloudDefReactor object associated with this point cloud entity.

    \param pointCloudDefReactorId [in]  Object ID of a reactor for this point cloud entity.

    \remarks
    Each OdDbPointCloud object must have an OdDbPointCloudDefReactor associated with the OdDbPointCloudDef object it is using.
  */
  void  setReactorId(const OdDbObjectId);


  /** \details
    Returns the total number of points of this point cloud entity.
  */
  OdInt64 getPointCount();

  /** \details
    Sets the total number of points of this point cloud entity.

    \param count [in]  Number od points.

    \remarks
    To be deprecated. Updating PointCloudDef object will update all Point Cloud Entities if PointCloudDefReactor is enabled.
  */
  void setPointCount(const OdUInt64 count);


  /** \details
    Returns the extents of this OdDbPointCloud object.
  */
  OdGeExtents3d extents() const;

  /** \details
    Sets the extents of this point cloud entity.

    \param exts [in]  Extents of this Point Cloud entity.

    \remarks
    To be deprecated. Updating PointCloudDef object will update all Point Cloud Entities if PointCloudDefReactor is enabled.
  */
  void setExtents(const OdGeExtents3d & exts);

  //Intensity


  /** \details
    Returns true only if intensity is shown for this point cloud entity.
  */
  bool  showIntensity() const;

  /** \details
    Enables/disables showing intensity for this point cloud entity.

    \param intensity [in]  Value for option.
    
    \returns
    Returns eOk.
  */
  OdResult  setShowIntensity(const bool);
  
  
  //bool hasIntensity() const;


  /** \details
    Returns an intensity scheme index for this point cloud entity.
  */
  int  getIntensityScheme() const;

  /** \details
    Sets an intensity scheme for this point cloud entity.

    \param schemeIndex [in]  An intensity scheme index to be set.
    
    \returns
    Returns eOk.
  */
  OdResult  setIntensityScheme(const int);

  const OdDbPointCloudIntensityStyle& intensityStyle() const;
  OdDbPointCloudIntensityStyle* pIntensityStyle();
  
  // Clipping
  /** \details
    Returns true only if this point cloud entity is displayed as clipped.
  */
  bool  showClipping() const;

  /** \details
    Set whether this point cloud entity is displayed as clipped.

    \param clipping [in]  Value for option.

    \returns
    Returns eOk.
  */
  OdResult  setShowClipping(const bool);

  /** \details
    Returns the number of clipping boundaries (the size of the clipping boundaries array) for this point cloud entity.
  */
  int  clippingCount() const;


  /** \details
    Adds a clipping boundary to the clipping boundaries array for this point cloud entity.

    \param boundary [in]  An OdDbPointCloudClipping object to be added.

    \returns
    Returns true if the specified clipping boundary is successfully added or false if the boundary isn't valid.
  */
  bool  addClippingBoundary(OdDbPointCloudClipping const &);

  /** \details
    Removes a clipping boundary at the specified position from the clipping boundaries array for this point cloud entity.

    \param nInd [in]  An index of the OdDbPointCloudClipping object in the clipping boundaries array to be removed.

    \returns
    Returns true if the specified clipping boundary is successfully removed or false if the index is out of range.
  */
  bool  removeClippingBoundary(int);

  /** \details
    Resets all clipping boundaries for this point cloud entity.
  */
  void  resetClippingBoundary();


  /** \details
    Returns a pointer to a constant PointCloudClipping object at the specified position in the clipping boundaries array.
    
    \param nInd [in]  An index of the OdDbPointCloudClipping object.

    \remarks
    Returns NULL if the index is out of range.
  */
  OdDbPointCloudClipping const*  getConstPointCloudClipping(int) const;

  /** \details
    Returns a pointer to a PointCloudClipping object at the specified position in the clipping boundaries array.

    \param nInd [in]  An index of the OdDbPointCloudClipping object.

    \remarks
    Returns NULL if the index is out of range.
  */
  OdDbPointCloudClipping*  getPointCloudClipping(int);

  // File IO
  virtual OdResult  dwgInFields(OdDbDwgFiler *);
  virtual void  dwgOutFields(OdDbDwgFiler *) const;
  virtual OdResult  dxfInFields(OdDbDxfFiler *);
  virtual void  dxfOutFields(OdDbDxfFiler *) const;

  // Rendering
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;


  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm);

#if 0
  // Require investigation
  bool  clearSelection(const OdString&);
  bool  select(PointCloudSelectionDef const &, const OdString &);
  bool  selectNone();
  bool  selectionId(const OdString&, struct _GUID &);

  bool  hasIntensity() const;

  IPointProcess *  ProcessPoints(IPointProcessor *, OdDbExtents *);
  IPointProcess *  ProcessPoints(IPointProcessor *, OdPointCloudViewFrustum *);

  bool  closestPoint(OdGePoint3d const &, OdGePoint3d const &,unsigned long &) const;
  void  UsePointBuffer(IPointCloudDataBuffer *);

  IPointCloudEngine *  getEngine() const;
  OdResult  getNativeCloudExtent(OdDbExtents &) const;
  bool  getPointCloudFileVersion(int &,int &) const;
  void  getViewFrustum(OdGiViewport &,OdPointCloudViewFrustum &,bool) const;
  bool  hasProperty(wchar_t const *,bool &) const;
  bool  isValid() const;
  void  markDirty();
  void  onViewChanged();
  IPointCloudDataBuffer *  pointData() const;
  bool  readPoints(OdDbExtents &, unsigned long);
  bool  readPoints(OdPointCloudViewFrustum &, unsigned long);
  bool  readPoints(unsigned long);
  OdResult  setPointCloudFilter(IPointCloudFilter *);
  class IOdPcPointFilter *  setPointFilter(IOdPcPointFilter *);
  void  setVisibleSelType(enum IPointCloudDataFile::selType);
  enum IPointCloudDataFile::selType  visibleSelType() const;
#endif

};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPointCloud entity pointers.
*/
typedef OdSmartPtr<OdDbPointCloud> OdDbPointCloudPtr;

/** \details
  This method performs the whole point cloud entity creation routine: creates the OdDbPointCloud entity, appends it to the database,
  applies specified attributes, creates the OdDbPointCloudDef object (if needed), restores or creates new links to the OdDbPointCloudDef object, adds reactors.

  \param pBlockToAdd [in]  Pointer to the block table record to add the point cloud entity to.
  \param newPointCloud [out]  Recieves a pointer to the created point cloud entity.
  \param indexFile [in]  Index file path. 
  \param sourceFile [in]  Source file path.
  \param location [in]  Location of the entity.
  \param scale [in]  Scale factor.
  \param rotation [in]  Rotation angle.
  \param vAxis [in]  Rotation axis.

  \returns
  Returns eOk. 
*/
DBPOINTCLOUDOBJ_EXPORT OdResult oddbCreatePointCloudEntity(
    OdDbBlockTableRecord* pBlockToAdd,
    OdDbPointCloudPtr& newPointCloud,
    const OdString& indexFile,
    const OdString& sourceFile,
    const OdGePoint3d& location = OdGePoint3d::kOrigin,
    double             scale = 1.0,
    double             rotation = 0.0,
    const OdGeVector3d& vAxis = OdGeVector3d::kZAxis
    );

#include "TD_PackPop.h"

#endif
