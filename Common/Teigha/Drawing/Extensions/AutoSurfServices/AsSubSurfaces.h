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

// AsSubSurfaces.h: interface for the OdAsSurface* classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSUBSURFACES_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
#define AFX_ASSUBSURFACES_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "AsSurfBodyExport.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeDoubleArray.h"

class OdDbDwgFiler;
class OdDbDxfFiler;

/** \details
  This class represents 3D AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsSurface
{
public:
  enum AsSurfType
  {
    kAsSurfUndefined    = 0,
    kAsSurfCylinder     = 1,
    kAsSurfCone         = 2,
    kAsSurfSphere       = 3,
    kAsSurfTorus        = 4,
    kAsSurfRevolve      = 5
  };

  /** \details
    Returns the type of this AsSurface object (DXF 73).  
    
    \remarks
    getType() returns one of the following:
    
    <table>
    Name                Value
    kAsSurfUndefined    0
    kAsSurfCylinder     1
    kAsSurfCone         2
    kAsSurfSphere       3
    kAsSurfTorus        4
    kAsSurfRevolve      5
    </table>
  */
  virtual AsSurfType getType() const = 0;

  /** \details
    Reads the DWG format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \remarks
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) = 0;

  /** \details
    Writes the DWG format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const = 0;

  /** \details
    Reads the DXF format data of this object from the specified filer. 
    
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \remarks
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) = 0;
  /** \details
    Writes the DXF format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const = 0;

  virtual ~OdAsSurface();
};

/** \details
  This class represents undefined AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsUndefined : public OdAsSurface
{
public:
  OdInt16 m_GripsCountU;
  OdInt16 m_GripsCountV;
  double  m_dSpan;

  OdAsUndefined();

  virtual AsSurfType getType() const { return kAsSurfUndefined; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This class represents cylindrical AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsCylinder : public OdAsSurface
{
public:
  OdGePoint3d  m_basePoint3d;
  OdGeVector3d m_axisOfSymmetry;
  OdGeVector3d m_refAxis;
  double m_dStartAngle;
  double m_dIncludedAngle;
  double m_dRadius;
  double m_dHeight;

  OdAsCylinder();

  virtual AsSurfType getType() const { return kAsSurfCylinder; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This class represents conical AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsCone : public OdAsSurface
{
public:
  OdGePoint3d  m_basePoint3d;
  OdGeVector3d m_axisOfSymmetry;
  OdGeVector3d m_refAxis;
  double m_dStartAngle;
  double m_dIncludedAngle;
  double m_dBaseRadius;
  double m_dTopRadius;
  double m_dHeight;

  OdAsCone();

  virtual AsSurfType getType() const { return kAsSurfCone; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This class represents spherical AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsSphere : public OdAsSurface
{
public:
  OdGePoint3d  m_Center3d;
  OdGeVector3d m_northAxis;
  OdGeVector3d m_refAxis;
  double m_dStartAngle;
  double m_dIncludedAngle;
  double m_dRadius;

  OdAsSphere();

  virtual AsSurfType getType() const { return kAsSurfSphere; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This class represents toroidal AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsTorus : public OdAsSurface
{
public:
  OdGePoint3d  m_Origin3d;
  OdGeVector3d m_axisOfSymmetry;
  OdGeVector3d m_refAxis;
  double m_dStartAngle;
  double m_dIncludedAngle;
  double m_dRadius;
  double m_dTubeRadius;

  OdAsTorus();

  virtual AsSurfType getType() const { return kAsSurfTorus; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This class represents revolved AutoSurf surfaces.

  Library: Source code provided. 
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsRevolve : public OdAsSurface
{
public:
  OdGePoint3d      m_AxisStartPoint;
  OdGeVector3d     m_AxisDirection;
  double           m_dStartAngle;
  double           m_dIncludedAngle;
  OdInt16          m_Unknown;
  OdGeDoubleArray  m_Knots;
  OdGePoint3dArray m_CtrlPoints;
  OdGeDoubleArray  m_Weights;

  OdAsRevolve();

  virtual AsSurfType getType() const { return kAsSurfRevolve; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

#include "TD_PackPop.h"

#endif // !defined(AFX_ASSUBSURFACES_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
