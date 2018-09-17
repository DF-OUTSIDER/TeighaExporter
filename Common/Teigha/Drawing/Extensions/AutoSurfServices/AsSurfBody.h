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

// OdAsSurfBody.h: interface for the OdAsSurfBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSURFBODY_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
#define AFX_ASSURFBODY_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "AsSurfBodyExport.h"
#include "AsSubSurfaces.h"
#include "DbBody.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbSurface.h"

/*
 Read only class	
 */

/** \details
  This class represents 3D AutoSurf entities in an OdDbDatabase instance.
  Library: Source code provided.
  <group AutoSurfServices_Classes> 
*/
class ASSURFBODYEXPORT OdAsSurfBody : public OdDbBody
{
public:
  struct AsSpline2dInfo
  {
    OdInt16         Unknown6;
    OdGeNurbCurve2d NurbCurve2d;
  };

  typedef OdArray<OdArray<AsSpline2dInfo> > AsLoop2dInfo;

private:
  OdAsSurface    *m_pAsSurface;
  int             m_nFormatVersion;
  double          m_dNormalLength;
  OdInt16         m_LinesCountU;
  OdInt16         m_LinesCountV;
  bool            m_bHasSubNurb;
  int             m_iUDegree;
  int             m_iVDegree;
  OdGeNurbSurface m_NURBSurf;
  OdInt16         m_Unknown4;
  double          m_Unknown5;
  AsLoop2dInfo    m_Loops2d;

  void clear();
  OdResult dwgInAsSurface(OdDbDwgFiler* pFiler);
  OdResult dwgInSubNurbSurface(OdDbDwgFiler* pFiler);
  OdResult dwgInTrimmingLoops(OdDbDwgFiler* pFiler);
  void dwgOutAsSurface(OdDbDwgFiler* pFiler) const;
  void dwgOutSubNurbSurface(OdDbDwgFiler* pFiler) const;
  void dwgOutTrimmingLoops(OdDbDwgFiler* pFiler) const;

  OdResBufPtr prepareResBuf4DxfOut(OdDbDxfFiler* pFiler, OdResBufPtr pResBuf) const;

public:
  ODDB_DECLARE_MEMBERS(OdAsSurfBody);

	OdAsSurfBody();
	virtual ~OdAsSurfBody();

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual void dxfOut(OdDbDxfFiler* pFiler) const;

  virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver,
                                  OdDbObjectId& replaceId,
                                  bool& exchangeXData);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \note
    This function does nothing but return eUnsupportedMethod.
  */
  virtual OdResult acisIn(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer *pTypeVer = 0);

  /** \details
    Returns the SubSurface of this SurfBody object.
  */
  virtual const OdAsSurface *getSubSurface() const
  {
    return m_pAsSurface;
  }

  /** \details
    Returns the normal length of this AsSurfBody object. (DXF 40)
  */
  virtual double  getNormalLength() const;
  /** \details
    Returns the number of lines in the U direction of this AsSurfBody object. (DXF 71)
  */
  virtual OdInt16 getLinesCountU() const;
  /** \details
    Returns the number of lines in the V direction of this AsSurfBody object. (DXF 72)
  */
  virtual OdInt16 getLinesCountV() const;
  /** \details
    Returns the SubNurbSurface of this AsSurfBody object.
    \param nurbs [out]  Receives the NurbSurface.

    Returns true if and only if there is a SubNurbSurface for this AsSurfBody object.
  */
  virtual bool    getSubNurbSurface(OdGeNurbSurface &nurbs) const;
  /** \details
    Returns the Trimming Loops of this AsSurfBody object.
    \param loops2d [out]  Receives the trimming loops.
    \remarks
    Returns true if and only if there is are trimming loops for this AsSurfBody object.
  */
  virtual bool    getTrimmingLoops(AsLoop2dInfo &loops2d) const;
};

/** \details
  This template class is an specialization of the OdSmartptr class for OdAsSurfBody object pointers. 
*/
typedef OdSmartPtr<OdAsSurfBody> OdAsSurfBodyPtr;

#include "TD_PackPop.h"

#endif // !defined(AFX_ASSURFBODY_H__335BDC0F_7675_4177_841E_EB995F14FD37__INCLUDED_)
