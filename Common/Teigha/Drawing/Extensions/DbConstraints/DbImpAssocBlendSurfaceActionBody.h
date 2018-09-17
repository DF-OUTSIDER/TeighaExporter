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
#ifndef DbImpAssocBlendSurfaceActionBody_INCLUDED_
#define DbImpAssocBlendSurfaceActionBody_INCLUDED_ /*!DOM*/

#include "DbImpAssocExtrudedSurfaceActionBody.h"
#include "DbBlendOptions.h"

#include "TD_PackPush.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on OdDbImpAssocPathBasedSurfaceActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

class DBCONSTRAINTS_EXPORT OdDbImpAssocBlendSurfaceActionBody : public OdDbImpAssocPathBasedSurfaceActionBody
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocBlendSurfaceActionBody();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocBlendSurfaceActionBody();

  /** \details
  Reads the .dwg file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
  Writes the .dwg file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
  Reads the .dxf file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
  Writes the .dxf file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  //const OdDbSmartCenterMarkParametersPtr parameters() const;
  //OdDbSmartCenterMarkParametersPtr& parameters();

  //virtual void composeForLoad(OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);

  /** \details
  Creates associative blend: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    OdGePoint3dArray ptsSelected,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);
  ///// <summary><para> 
  ///// Get continuity of start profile
  ///// </para><para>
  OdInt16  startEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;

	///// <summary><para> 
  ///// Set continuity of start profile
  ///// </para><para>
  OdResult  setStartEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 value, const OdString& expression, const OdString& evaluatorId);

  ///// <summary><para> 
  ///// Get continuity of end profile
  ///// </para><para>
  OdInt16  endEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdArray<OdInt16>  arrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdResult  setArrayEdgeContinuity(OdDbAssocActionPtr pAction, OdArray<OdInt16>& values, OdString& expression, OdString& evaluatorId);

  ///// <summary><para> 
  ///// Set continuity of end profile
  ///// </para><para>
  OdResult  setEndEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 value, const OdString& expression, const OdString& evaluatorId);

  ///// <summary><para> 
  ///// Get bulge factor of end profile
  ///// </para><para>
  double  startEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdArray<double>  arrayEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdResult  setArrayEdgeBulge(OdDbAssocActionPtr pAction, OdArray<double>& values, OdString& expression, OdString& evaluatorId);

  ///// <summary><para> 
  ///// Set bulge factor of start profile
  OdResult  setStartEdgeBulge(OdDbAssocActionPtr pAction, double value, const OdString& expression,	const OdString& evaluatorId);
 
  ///// <summary><para> 
  ///// Get bulge factor of end profile
  ///// </para><para>
  double  endEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;

  ///// <summary><para> 
  ///// Set bulge factor of end profile
  ///// </para><para>
  OdResult  setEndEdgeBulge(OdDbAssocActionPtr pAction, double value, const OdString& expression, const OdString& evaluatorId);

 
  /// <summary> 
  /// Get blend options used for blend surface 
  OdResult  getBlendOptions(OdDbBlendOptions& blendOptions) const
  {
    blendOptions = m_pOptions;

    return eOk;
  }

  /// <summary> 
  /// Set blend options for blend surface
  /// </summary>
  OdResult  setBlendOptions(const OdDbBlendOptions& blendOptions);

 private:
  OdUInt32 m_unknownInt32;
  bool m_bFlag0;
  bool m_bFlag1;
  bool m_bFlag2;
  bool m_bFlag3;
  bool m_bFlag4;
  OdUInt16 m_unknownInt16_1;
  OdUInt16 m_unknownInt16_2;
  OdDbBlendOptions m_pOptions;
};

#include "TD_PackPop.h"

#endif
