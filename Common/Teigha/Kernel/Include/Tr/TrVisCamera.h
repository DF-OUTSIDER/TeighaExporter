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
// GLES2 camera

#ifndef ODTRVISCAMERA
#define ODTRVISCAMERA

#include <set>

#include "TrVisDefs.h"
#include "TrVisExtentsDef.h"
#include "TrVisViewportDef.h"

struct OdTrVisSurfaceDef;

class OdTrVisCamera
{
public:
  OdTrVisCamera();
  ~OdTrVisCamera();

  const OdTrVisViewParamsDef& viewParams(bool bUpdated = true) const { if (bUpdated && isModified()) update(); return m_viewParams; }
  void setViewParams( const OdTrVisViewParamsDef& viewParams, bool bSetModified = false );

  bool isModified() const { return GETBIT( m_flags, kCameraModified ); }
  // @@@TODO: eliminate this method usage. cameras must be linked with modified objects and must be updated automatically.
  void markModified() { setModified(); }

  void setParentCamera( OdTrVisCamera* );
  OdTrVisCamera* parentCamera() const { return m_pParentCamera; }

  void setOverlayOwnerCamera( OdTrVisCamera* );
  OdTrVisCamera* overlayOwnerCamera() const { return m_pOverlayOwnerCamera; }
  void removeOverlayCamera( OdTrVisCamera* );

  void setSiblingOwnerCamera( OdTrVisCamera* );
  OdTrVisCamera* siblingOwnerCamera() const { return m_pSiblingOwnerCamera; }
  void removeSiblingCamera( OdTrVisCamera* );

  void setContext(OdTrVisSurfaceDef *pSurface, OdTrVisViewportDef *pViewport, OdTrVisExtentsDef *pExtents)
  { m_pSurfaceDef = pSurface; m_pViewportDef = pViewport; m_pExtentsDef = pExtents; }

  // Produce panning operation
  virtual void pan(int nX, int nY);
  // Produce simple zooming operation
  virtual void zoom(float fDelta);
  // Zooming around point
  virtual void zoom(int nX, int nY, float fDelta);
  // Orbit
  enum OrbitType
  {
    kOrbitPerp = 0,
    kOrbitHorizontal,
    kOrbitVertical,
    kOrbitEye
  };
  virtual void orbit(int nX, int nY, OrbitType type = kOrbitPerp, float angle = 0.0f);
  // Zoom extents
  virtual void zoomExtents(float coef = 1.03f);

  // Low-level transformation routines
  virtual void translate(const OdGeVector3d &offset);
  virtual void rotate(double xAngle, double yAngle, double zAngle, bool bLocalCS = true);

  // Recalculate matrices
  virtual void update() const;

  //Copy view params
  virtual void copyViewParams( const OdTrVisCamera* );

  //OdTrVisViewParamsDef accessors

  const OdGePoint3d &viewPosition() const { return m_viewParams.m_viewPosition; }
  void setViewPosition( const OdGePoint3d& pt )
  {
    m_viewParams.m_viewPosition = pt;
    setModified();
  }

  const OdGeVector3d &viewBasisX() const { return m_viewParams.m_viewBasis[0]; }
  void setViewBasisX(const OdGeVector3d &xBasis)
  {
    m_viewParams.m_viewBasis[0] = xBasis;
    setModified();
  }
  const OdGeVector3d &viewBasisY() const { return m_viewParams.m_viewBasis[1]; }
  void setViewBasisY(const OdGeVector3d &yBasis)
  {
    m_viewParams.m_viewBasis[1] = yBasis;
    setModified();
  }
  const OdGeVector3d &viewBasisZ() const { return m_viewParams.m_viewBasis[2]; }
  void setViewBasisZ(const OdGeVector3d &zBasis)
  {
    m_viewParams.m_viewBasis[2] = zBasis;
    setModified();
  }

  bool isPerspectiveProjection() const { return m_viewParams.m_bPerspectiveProjection; }
  void setPerspectiveProjection( bool p )
  {
    m_viewParams.m_bPerspectiveProjection = p;
    setModified();
  }

  double fieldWidth() const { return m_viewParams.m_fieldWidth; }
  void setFieldWidth( double w )
  {
    m_viewParams.m_fieldWidth = w;
    setModified();
  }

  double fieldHeight() const { return m_viewParams.m_fieldHeight; }
  void setFieldHeight( double h )
  {
    m_viewParams.m_fieldHeight = h;
    setModified();
  }

  double lensLength() const { return m_viewParams.m_lensLength; }
  void setLensLength( double l )
  {
    m_viewParams.m_lensLength = l;
    setModified();
  }

  const OdGePoint3d &viewTarget() const { return m_viewParams.m_viewTarget; }
  void setViewTarget( const OdGePoint3d& pt )
  {
    m_viewParams.m_viewTarget = pt;
    setModified();
  }

  const OdTrVisViewClipped &frontClip() const { return m_viewParams.m_frontClip; }
  void setFrontClip( const OdTrVisViewClipped& cl )
  {
    m_viewParams.m_frontClip = cl;
    setModified();
  }

  const OdTrVisViewClipped &backClip() const { return m_viewParams.m_backClip; }
  void setBackClip( const OdTrVisViewClipped& cl )
  {
    m_viewParams.m_backClip = cl;
    setModified();
  }

  bool hasViewClip() const { return backClip().enabled() || frontClip().enabled(); }

  // Matrix accessors

  const OdGeMatrix3d &UnXForm() const { if (isModified()) update(); return m_UnXForm; }

  const OdGeMatrix3d &metafileMatrix() const
  {
    return m_viewParams.m_metafileMatrix;
  }
  bool hasMetafileMatrix() const
  {
    return !m_viewParams.m_metafileMatrix.isEqualTo(OdGeMatrix3d::kIdentity);
  }
  void setMetafileMatrix(const OdGeMatrix3d& m)
  {
    m_viewParams.m_metafileMatrix = m;
    setModified();
  }

  // Computed matrix accessors

  const OdGeMatrix3d &screenMatrix() const
  {
    if (isModified()) update();
    return m_viewParams.m_screenMatrix;
  }

  const OdGeMatrix3d &viewingMatrix() const
  {
    if (isModified()) update();
    return m_viewParams.m_viewingMatrix;
  }

  const OdGeMatrix3d &projectionMatrix() const
  {
    if (isModified()) update();
    return m_viewParams.m_projectionMatrix;
  }

  const OdGeMatrix3d &correctionMatrix() const
  {
    if (isModified()) update();
    return m_viewParams.m_correctionMatrix;
  }

  const OdGeMatrix3d &outputMatrix() const
  {
    if (isModified()) update();
    return m_viewParams.m_outputMatrix;
  }

  // Extended data accessors

  double projectionScreenDepth() const
  {
    return projectionMatrix()(2, 3);
  }
  double projectionFocalLengthReciprocal() const
  {
    return projectionMatrix()(3, 2);
  }
  double projectionFocalLength() const
  {
    return -1.0 / projectionFocalLengthReciprocal();
  }
  bool projectionPerspective() const
  {
    return !OdZero(projectionFocalLengthReciprocal());
  }

  long screenWidth() const;
  long screenHeight() const;

  // Extended viewport calculations

  bool viewportRect(OdGsDCRect &vpRect, bool bClipped = false) const;
  void applyMetafileXform(bool bSetModified = false) const;

  // Temporary

  bool cameraCumulative() const { return GETBIT( m_flags, kCameraCumulative ); }
  void setCameraCumulative( bool bSet ){ SETBIT( m_flags, kCameraCumulative, bSet ); }

protected:
  enum CameraFlags
  {
    kCameraModified = 1,
    kCameraCumulative = 2 //Viewport parameters override was based onto basic viewport parameters
  };
  mutable OdTrVisViewParamsDef m_viewParams;
  mutable OdGeMatrix3d         m_UnXForm;
  mutable OdUInt8              m_flags;

  OdTrVisCamera* m_pParentCamera;
  std::set< OdTrVisCamera* > m_childCameras;

  OdTrVisSurfaceDef  *m_pSurfaceDef;
  OdTrVisViewportDef *m_pViewportDef;
  OdTrVisExtentsDef  *m_pExtentsDef;

  OdTrVisCamera* m_pOverlayOwnerCamera;
  std::set< OdTrVisCamera* > m_overlayCameras;

  OdTrVisCamera* m_pSiblingOwnerCamera;
  std::set< OdTrVisCamera* > m_siblingCameras;

protected: //helpers
  static double computeAspectRatio(const OdGsDCRectDouble& dcRect);
  double computeFieldWidth(double aspectRatio) const;
  double computeFieldHeight(double aspectRatio) const;
  void projectedExtents(OdGePoint2d &extMin, OdGePoint2d &extMax, const OdGeExtents3d &extents) const;
  static double computeFocalLength(double lensLength, double fieldWidth, double fieldHeight);
  static void computeSceneDepth(const OdGeMatrix3d &ecsToWcs, const OdGeExtents3d &sceneExtents, double &dNear, double &dFar);
  OdGeMatrix3d computeProjectionMatrix(const OdGsDCRectDouble &dcRect, const OdTrVisViewportDef* sets, const OdGeExtents3d &sceneExtents) const;
  static OdGeMatrix3d computeCorrectionMatrix(const OdGsDCRectDouble &dcRect, double screenWidth, double screenHeight);

protected:
  void preComputeViewportDefMatrices() const;
  void setChildCamera( OdTrVisCamera* );
  void removeChildCamera( OdTrVisCamera* );

  void setOverlayCamera( OdTrVisCamera* );

  void setSiblingCamera( OdTrVisCamera* );

  OdTrVisCamera* sibling()
  {
    if( m_siblingCameras.empty() ) return NULL;
    return *(m_siblingCameras.begin());
  }

  void setModified(bool modified = true) const;

  const OdGeExtents3d &extents() const
  {
    if (!m_pExtentsDef) return OdGeExtents3d::kInvalid;
    return m_pExtentsDef->preferRealExtents();
  }
};

#endif
