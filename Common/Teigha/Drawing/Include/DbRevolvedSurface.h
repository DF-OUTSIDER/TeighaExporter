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


#ifndef _DbRevolvedSurface_h_Included_
#define _DbRevolvedSurface_h_Included_

#include "TD_PackPush.h"
#include "DbSurface.h"
#include "DbRevolveOptions.h"

/** \details
  This class implements a three-dimensional revolved surface entity.
  
  \sa
  TD_Db
 
  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbRevolvedSurface: public OdDbSurface
{
public:
  OdDbRevolvedSurface ();
  ODDB_DECLARE_MEMBERS(OdDbRevolvedSurface);

  /** \details
  Creates a revolved surface from a specified curve, region, or planar surface and an axis of revolution.
  
  Axis of revolution is defined with a point (axisPnt) and vector (axisDir).
  pRevEnt will be revolved counter clockwise about the axisDir vector.
  
  If startAngle is equal to 0, rotation starts from current position of pRevEnt.
  
  \param pRevEnt        [in] A pointer to the planar curve, region, or planar surface that is to be revolved.
  \param axisPnt        [in] A point of the revolution axis.
  \param axisDir        [in] A revolution axis' direction represented with an OdGeVector object.
  \param revAngle       [in] An angle of rotation (radians).
  \param startAngle     [in] A start angle of rotation (radians).
  \param revolveOptions [in] Revolving options.
  \param pSat           [in] A pointer to an output stream buffer.
  
  \returns  Returns eOk if surface was successfully created. 
  If specified data is not valid, returns eInvalidInput.
  */
  virtual OdResult createRevolvedSurface ( OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt, const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions, const OdStreamBuf *pSat = 0 );

  /** \details
  Returns a pointer to the entity that is to be revolved.
  Revolved entity can be a planar curve, region, or planar surface.
  */
  OdDbEntity*   getRevolveEntity () const;

  /** \details
  Returns a point on the revolution axis.
  The returned point and the axis direction define the axis of revolution. 
  */
  OdGePoint3d getAxisPnt () const;
  
  /** \details
  Sets the axis point. 

  \param pnt [in] An axis point. The revolving entity revolves around this axis point.
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setAxisPnt( const OdGePoint3d& pnt );
  
  /** \details
  Returns a vector representing the axis of revolution.
  */
  OdGeVector3d getAxisVec () const;
  
  /** \details
  Sets the axis vector for revolution.
  The axis of revolution is defined by the axis point and axis vector. 
  
  \param vec [in]  A new axis vector.
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setAxisVec( const OdGeVector3d& vec );

  /** \details
  Returns the revolution angle (in radians).
  */
  double getRevolveAngle () const;
  
  /** \details
  Sets the angle of revolution.
  
  \param ang [in]  A new value of revolution angle (in radians).
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setRevolveAngle ( double ang );
  
  /** \details
  Returns the start revolution angle (in radians). 
  */
  double getStartAngle () const;
  
  /** \details
  Sets the start revolution angle.
  
  \param startAng [in]  A new value of start revolution angle (in radians).
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setStartAngle (double startAng);

  /** \details
  Returns current revolve options.
  Revolve options are stored in the passed OdDbRevolveOptions object.  
  
  \param revolveOptions [out]  A passed-in parameter for returning current revolve options.
  */
  void getRevolveOptions ( OdDbRevolveOptions& revolveOptions ) const;
  
  /** \details
  Sets the revolve options for the surface.
  
  \param revolveOptions [in]  An OdDbRevolveOptions object with the new values of revolution options to be set.
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setRevolveOptions ( const OdDbRevolveOptions& revolveOptions );

  /** \details
  Sets all the data needed for the revolved surface.

  \param axisPnt        [in] A new axis point to be set.
  \param axisVec        [in] A new axis vector to be set.
  \param revAngle       [in] A new value of revolution angle.
  \param revolveOptions [in] A new revolution options object to be set.
  
  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  void setRevolve ( const OdGePoint3d& axisPnt, const OdGeVector3d& axisVec,
    double revAngle, const OdDbRevolveOptions& revolveOptions );

  OdResult subTransformBy(
    const OdGeMatrix3d& xfm );

  // OdDbObject methods

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual bool isDependent() const { return false; }
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbRevolvedSurface object pointers.
*/
typedef OdSmartPtr<OdDbRevolvedSurface> OdDbRevolvedSurfacePtr;

#include "TD_PackPop.h"
#endif //_DbRevolvedSurface_h_Included_
