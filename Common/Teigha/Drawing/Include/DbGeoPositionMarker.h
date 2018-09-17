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

#ifndef _ODDBGEOPOSITIONMARKER_INCLUDED_
#define _ODDBGEOPOSITIONMARKER_INCLUDED_

#include "TD_PackPush.h"
#include "DbEntity.h"
#include "OdString.h"
#include "DbMText.h"

/** \details
<group OdDb_Classes>

Class represents a geographical location aware marker object with a label. 
*/
class DBENT_EXPORT OdDbGeoPositionMarker : public OdDbEntity
{
public:
  /** \details
  Enumerator representing alignment type of label text.
  */
  enum OdTextAlignmentType {
    /** Left-justified label text.*/
    kLeftAlignment              = 0,
    /** Label text center alignment.*/
    kCenterAlignment            = 1,
    /** Right-justified label text.*/
    kRightAlignment             = 2
  };

  ODDB_DECLARE_MEMBERS(OdDbGeoPositionMarker);
  /** \details
  Creates an instance of the OdDbGeoPositionMarker class.
  
  \remarks
  Use the static pseudo-constructor createObject() instead of it.
  */ 
  OdDbGeoPositionMarker();
  
  virtual ~OdDbGeoPositionMarker();
  
  /** \details 
  Returns the current value of the insertion point of the position marker.
  */
  OdGePoint3d       position() const;
  
  /** \details 
  Sets a new value of the insertion point of the position marker.
  
  \param position [in] New insertion point coordinates.
  
  \returns Returns eOk if the new value of the insertion point was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setPosition(const OdGePoint3d &position);
  
  /** \details 
  Returns the current radius value of the position marker.
  */
  double            radius() const; 
  
  /** \details 
  Sets a new radius value of the position marker.
  
  \param radius [in] A new radius value to be set.
  
  \returns Returns eOk if the new radius value was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setRadius(double radius);
  
  /** \details 
  Returns the current position marker text label value.
  */
  OdString          text() const;
  
  /** \details 
  Sets a new text label value of the position marker.
  
  \param text [in] A new text label value to be set.
  
  \returns Returns eOk if the new text label value was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setText(const OdString& text);
  
  /** \details 
  Returns the copy of the current position marker multiline text label value.
  */  
  OdDbMTextPtr      mtext() const;
  
  /** \details 
  Sets a new multiline text label value of the position marker as a copy of the input parameter's referenced value.
  
  \param pMText [in] A new multiline text label value to be set, which is represented as a pointer to an OdDbMText.
  
  \returns Returns eOk if the new multiline text label value was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setMText(const OdDbMText * pMText);
  
  /** \details 
  Returns the current value of the multiline text label visibility flag.
  The visibility flag indicates that the multiline text label is visible (if true).
  */  
  bool              mtextVisible() const;
  
  /** \details 
  Sets a new value of the  multiline text label visibility flag.
  The visibility flag indicates that the multiline text label is visible (if true).
  
  \param visible [in] A new flag value to be set.
  
  \returns Returns eOk if the new value of the multiline text label visibility flag was successfully set or returns an appropriate error code in the other case.
  */  
  OdResult          setMTextVisible(bool visible);
  
  /** \details 
  Returns the current value of the landing gap of the position marker.
  */  
  double            landingGap() const;
  
  /** \details 
  Sets a new value of the landing gap of the position marker.
  
  \param landingGap [in] A new landing gap value to be set.
  
  \returns Returns eOk if the new value of the landing gap was successfully set or returns an appropriate error code in the other case.
  */ 
  OdResult          setLandingGap(double landingGap);

  /** \details 
  Returns the current value of the label text frame enabling flag.
  The frame enabling flag indicates that the label text has a frame (if true).
  */  
  bool              enableFrameText() const;
  
  /** \details 
  Sets a new value of the label text frame enabling flag.
  The frame enabling flag indicates that the label text has a frame (if true).
  
  \param enableFrameText [in] A new flag value to be set.
  
  \returns Returns eOk if the new value of the label text frame enabling flag was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setEnableFrameText(bool enableFrameText);
  
  /** \details 
  Returns the current value of the label text alignment type.
  */ 
  OdTextAlignmentType textAlignmentType() const;
  
  /** \details 
  Sets a new value of the label text alignment type.
    
  \param textAlignmentType [in] A new alignment type value to be set represented as a value of the OdTextAlignmentType enumeration.
  
  \returns Returns eOk if the new value of the label text alignment type was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setTextAlignmentType(OdTextAlignmentType textAlignmentType);
 
  /** \details 
  Returns the current value of the position marker notes as an OdString.
  */ 
  OdString          notes() const;
  
  /** \details 
  Sets a new value of position marker notes.
    
  \param notes [in] A new notes value to be set.
  
  \returns Returns eOk if the new value of position marker notes was successfully set or returns an appropriate error code in the other case.
  */  
  OdResult          setNotes(const OdString& notes);
  
  /** \details 
  Returns the current value of the insertion point of the position marker.
  */  
  OdGePoint3d       geoPosition() const;
  
  /** \details 
  Sets a new value of the geographical location of the position marker.
    
  \param position [in] A new value of the geographical location coordinates (where x, y, z are longitude, latitude and alleviation) that should be set.
  
  \returns Returns eOk if the new value of the geographical location of the position marker was successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setGeoPosition(const OdGePoint3d &position);

  /** \details 
  Returns the current value of the geographical location of the position marker.
  
  \param lat [out] A reference to a double variable to store the latitude value.
  \param lon [out] A reference to a double variable to store the longitude value.
  \param alt [out] A reference to a double variable to store the alleviation value.
  
  \returns Returns eOk if the new values of the geographical location parameters of the position marker were successfully set into output variables or returns an appropriate error code in the other case.
  */  
  OdResult          latLonAlt(double &lat, double &lon, double &alt) const;
  
  /** \details 
  Sets a new value of the geographical location of the position marker.
  
  \param lat [in] A double value of the latitude.
  \param lon [in] A double value of the longitude.
  \param alt [in] A double value of the alleviation.
  
  \returns Returns eOk if the new values of the geographical location parameters of the position marker were successfully set or returns an appropriate error code in the other case.
  */
  OdResult          setLatLonAlt(double lat, double lon, double alt);
  
  /** \details 
  Returns the current value of the normal of the position marker as an OdGeVector3d object.
  */
  OdGeVector3d      normal() const;
  
  /** \details 
  Returns the current value of the label text style of the position marker as an OdDbObjectId.
  */
  OdDbObjectId      textStyle() const;

  virtual OdResult  dwgInFields(OdDbDwgFiler* pFiler);
  virtual void      dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult  dxfInFields(OdDbDxfFiler* pFiler);
  virtual void      dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual bool      subWorldDraw(OdGiWorldDraw* pWorldDraw) const;
  virtual void      subViewportDraw(OdGiViewportDraw* pViewportDraw) const;

  virtual OdResult  subTransformBy(const OdGeMatrix3d& xform);
  virtual void      subClose();

  virtual OdResult  subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual void subSetDatabaseDefaults(OdDbDatabase* pDb, bool doSubents);
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbGeoPositionMarker object pointers.
*/
typedef OdSmartPtr<OdDbGeoPositionMarker> OdDbGeoPositionMarkerPtr;

#endif
