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

#ifndef __ODGIXFORM_H__
#define __ODGIXFORM_H__

#include "Gi/GiConveyorNode.h"
class OdGeMatrix3d;

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiXform : public OdGiConveyorNode
{
protected:
  OdGiXform();
public:
  ODRX_DECLARE_MEMBERS(OdGiXform);

  virtual void setTransform(const OdGeMatrix3d& xMat) = 0;
  virtual void transform(OdGeMatrix3d& xMat) const = 0;

  /** \details
    Enable output as 2d points mode.

    \param bOutput2dPoints [in]  Output as 2d points mode flag.

    \remarks
    This flag indicating that when applying transformation to the input points, output points
    should be considered as 2d points, z-coordinate won't be used and does not require calculation.

    This way could be used to avoid extra arithmetic operations on doubles.

    An example of setOutput2dPoints() usage is GDI device implementation.
    The last stage of geometry conveyor when playing metafiles to device is
    OdGiXForm (metafile to device transformation). It produces points in pixel
    coordinates, does not require z coordinate to be calculated.
  */
  virtual void setOutput2dPoints(bool bOutput2dPoints) = 0;
  virtual bool output2dPoints() const = 0;
};

typedef OdSmartPtr<OdGiXform> OdGiXformPtr;

// Several Xform optimizations

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiXformOptimizer : public OdGiConveyorNode
{
protected:
  OdGiXformOptimizer();
public:
  ODRX_DECLARE_MEMBERS(OdGiXformOptimizer);

  virtual void setTransform(const OdGeMatrix3d& xMat) = 0;
  virtual OdGeMatrix3d transform() const = 0;

  virtual void sync(OdGiXform* xForm) = 0;
  virtual OdGiConveyorGeometry* redirectionGeometry() = 0;
};

typedef OdSmartPtr<OdGiXformOptimizer> OdGiXformOptimizerPtr;

class OdGeVector3d;
class OdGePoint3d;

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiTranslationXform : public OdGiXformOptimizer
{
protected:
  OdGiTranslationXform();
public:
  ODRX_DECLARE_MEMBERS(OdGiTranslationXform);

  virtual void setTranslation(const OdGeVector3d &transform) = 0;
  virtual const OdGeVector3d &translationAsVector() const = 0;

  virtual void setTranslation(const OdGePoint3d &transform) = 0;
  virtual const OdGePoint3d &translationAsPoint() const = 0;
};

typedef OdSmartPtr<OdGiTranslationXform> OdGiTranslationXformPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGIXFORM_H__
