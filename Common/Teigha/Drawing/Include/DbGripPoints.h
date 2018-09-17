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



#ifndef _ODDBGPIPPOINTS_INCLUDED_
#define _ODDBGPIPPOINTS_INCLUDED_

#include "RxObject.h"
#include "RxModule.h"
#include "IntArray.h"
#include "DbEntity.h"
#include "Gi/GiDrawable.h"


/** \details
    This class is the Grip Points Protocol Extension class.
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbGripPointsPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGripPointsPE);
  
  /** \details
    Returns all grip points of the specified entity.

    \param pEntity [in]  Pointer to the entity.
    \param gripPoints [in/out] Receives an array of WCS grip points.

    \remarks
    Grip points are appended to the specified array.
    
    \returns 
    Returns eOk if successful, or an appropriate error code if not.
    The default implementation of this function returns eNotImplemented.
  */
  virtual OdResult getGripPoints(
    const OdDbEntity* pEntity,
    OdGePoint3dArray& gripPoints ) const;

  /** \details
    Moves the specified grip points of the specified entity.
    
    \param pEntity [in]  Pointer to the entity.
    \param indices [in]  Array of indicies.
    \param offset [in] The direction and magnitude of the grip points offset (WCS).
    
    \remarks
    Each element in gripPoints has a corresponding entry in indices, which specifies the index of 
    the grip point as returned by getGripPoints.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    The default implementation of this function returns eNotImplemented.
  */
    virtual OdResult moveGripPointsAt(
    OdDbEntity* pEntity,
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  /** \details
    Returns OdDbGripData objects for grip points of this entity.

    \param pEntity [in]  Pointer to the entity.
    \param grips [in/out] Receives an array of OdDbGripData objects.

    \remarks
    Grip points are appended to the specified array.
    When eNotImplemented is returned, the application should call the other overload of the getGripPoints() method.
    The default implementation of this function returns eNotImplemented.
  */
  virtual OdResult getGripPoints(
    const OdDbEntity* pEntity,
    OdDbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags ) const;

  /** \details
    Moves the specified grip points of this entity.
    
    \param pEntity [in]  Pointer to the entity.
    \param grips [in]  Array of OdDbGripData identifiers.
    \param offset [in] The direction and magnitude of the grip points offset (WCS).

    \remarks
    The default implementation of this function returns eNotImplemented. 
  */
    virtual OdResult moveGripPointsAt(
    OdDbEntity* pEntity,
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags );

  /** \details
    Returns all stretch points of the specified entity.

    \param pEntity [in]  Pointer to the entity.
    \param stretchPoints [in/out]  Receives an array of WCS stretch points.

    \remarks
    Stretch points are appended to the specified array.
    
    Returns eOk if successful, or an appropriate error code if not.
    The default implementation of this function returns eNotImplemented.
  */
  virtual OdResult getStretchPoints(
    const OdDbEntity* pEntity,
    OdGePoint3dArray& stretchPoints ) const;
  /** \details
    Moves the specified stretch points of this entity.
    
    \param pEntity [in]  Pointer to the entity.
    \param indices [in]  Array of indicies.
    \param offset [in] The direction and magnitude of the stretch points offset (WCS).
    
    \remarks
    Each element in stretchPoints has a corresponding entry in indices, which specifies the index of 
    the stretch point as returned by getStretchPoints.
    
    Returns eOk if successful, or an appropriate error code if not.
    The default implementation of this function returns eNotImplemented.
  */
    virtual OdResult moveStretchPointsAt(
    OdDbEntity* pEntity,
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  /** \details
    Returns all appropriate object snap points of the specified entity.
    
    \param pEntity [in]  Pointer to the entity.
    \param osnapMode [in]  The object snap mode being queried.
    \param gsSelectionMark [in]  The GS marker of the subentity being queried.
    \param pickPoint [in]  The WCS point being queried.
    \param lastPoint [in]  The WCS point picked before pickPoint.
    \param viewXform [in]  The WCS->DCS transformation matrix.
    \param ucs [in]  The WCS->UCS transformation matrix.
    \param snapPoints [in/out] Receives an array of UCS object snap points.
    
    \remarks
    Object snap points are appended to the specified array.
    
    osnapMode must be one of the following:
    
    <table>
    Name                      Value   Description 
    OdDb::kOsModeEnd          1       Endpoint
    OdDb::kOsModeMid          2       Midpoint
    OdDb::kOsModeCen          3       Center
    OdDb::kOsModeNode         4       Node
    OdDb::kOsModeQuad         5       Quadrant
    OdDb::kOsModeIntersec     6       Intersection
    OdDb::kOsModeIns          7       Insertion point
    OdDb::kOsModePerp         8       Perpendicular
    OdDb::kOsModeTan          9       Tangent
    OdDb::kOsModeNear         10      Nearest
    OdDb::kOsModeApint        11      Apparent intersection
    OdDb::kOsModePar          12      Parallel
    OdDb::kOsModeStart        13      Unknown 
    </table>
   
    The default implementation of this function returns eNotImplemented.
  */
    virtual OdResult getOsnapPoints( 
    const OdDbEntity* pEntity, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& xWorldToEye, 
    OdGePoint3dArray& snapPoints ) const;

  /** \details
  Returns all appropriate object snap points of the specified entity.

  \param pEntity [in]  Pointer to the entity.
  \param osnapMode [in]  The object snap mode being queried.
  \param gsSelectionMark [in]  The GS marker of the subentity being queried.
  \param pickPoint [in]  The WCS point being queried.
  \param lastPoint [in]  The WCS point picked before pickPoint.
  \param viewXform [in]  The WCS->DCS transformation matrix.
  \param ucs [in]  The WCS->UCS transformation matrix.
  \param snapPoints [in/out] Receives an array of UCS object snap points.
  \param insert [in] Current insertion matrix

  \remarks
  Object snap points are appended to the specified array.

  osnapMode must be one of the following:

  <table>
  Name                      Value   Description 
  OdDb::kOsModeEnd          1       Endpoint
  OdDb::kOsModeMid          2       Midpoint
  OdDb::kOsModeCen          3       Center
  OdDb::kOsModeNode         4       Node
  OdDb::kOsModeQuad         5       Quadrant
  OdDb::kOsModeIntersec     6       Intersection
  OdDb::kOsModeIns          7       Insertion point
  OdDb::kOsModePerp         8       Perpendicular
  OdDb::kOsModeTan          9       Tangent
  OdDb::kOsModeNear         10      Nearest
  OdDb::kOsModeApint        11      Apparent intersection
  OdDb::kOsModePar          12      Parallel
  OdDb::kOsModeStart        13      Unknown 
  </table>

  The default implementation of this function returns eNotImplemented.
  */
    virtual OdResult getOsnapPoints( 
    const OdDbEntity* pEntity, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& xWorldToEye, 
    OdGePoint3dArray& snapPoints,
    const OdGeMatrix3d& insert) const;

  /** \details
	Gets the grip points and supporting information about the grips for a subentity.

  \param pEntity [in]  Pointer to the entity.
	\param path [in]  The OdDbFullSubentPath to the subentity.
	\param grips [in/out] Receives an array of pointers of OdDbGripData objects, one object for each grip point in the subentity.
	\param curViewUnitSize [in] The size (in pixels) of one drawing unit in the current viewport.
	\param gripSize [in] The current grip size (in pixels).
	\param curViewDir [in] The view direction in the current viewport.
	\param bitflags [in] The bitmap of one or more flags specified by the GetGripPointsFlags enumeration.
  */

  virtual OdResult getGripPointsAtSubentPath(   const OdDbEntity* pEntity,
                        const OdDbFullSubentPath& path, OdDbGripDataPtrArray& grips,
											  const double curViewUnitSize, const int gripSize,
											  const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const;

  /** \details
	Applies a vector offset to one or more grip points exposed by subentities on the object.

  \param pEntity [in]  Pointer to the entity.
	\param paths [in]  The array of OdDbFullSubentPath objects.
	\param gripAppData [in]  The array of pointers of OdDbGripData objects.
	\param offset [in]  The vector (in WCS coordinates) indicating the direction and magnitude of the translation.
	\param bitflags [in]  The bitmap of one or more flags specified by the GetGripPointsFlags enumeration.
  */

  virtual OdResult moveGripPointsAtSubentPaths(OdDbEntity* pEntity,
                        const OdDbFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData,
												const OdGeVector3d& offset, const OdUInt32 bitflags);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbGripPointsPE object pointers.
*/
typedef OdSmartPtr<OdDbGripPointsPE> OdDbGripPointsPEPtr;

#endif //_ODDBGPIPPOINTS_INCLUDED_
