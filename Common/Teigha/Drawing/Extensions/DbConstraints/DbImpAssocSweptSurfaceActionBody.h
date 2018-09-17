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
#ifndef DbImpAssocSweptSurfaceActionBody_INCLUDED_
#define DbImpAssocSweptSurfaceActionBody_INCLUDED_ /*!DOM*/

#include "DbImpAssocExtrudedSurfaceActionBody.h"

#include "TD_PackPush.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on AnnotationActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

class DBCONSTRAINTS_EXPORT OdDbImpAssocSweptSurfaceActionBody : public OdDbImpAssocExtrudedSurfaceActionBody// OdDbImpAssocAnnotationActionBody
{
public:
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
  ////////////////////////////////////////////////////////////////////// from Arx for parent classes
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);
  /// <summary><para> 
  /// Set scale factor of the resulting surface
  /// </para><para>
  OdResult setScaleFactor(OdDbAssocActionPtr pAct, OdDouble scale, const OdString& expression = OdString(),
    const OdString& evaluatorId = OdString());

  /// <summary><para> 
  /// Get scale factor of the resulting swept surface 
	/// </para><para>
  OdDouble scaleFactor(const OdDbAssocActionPtr pAct, OdString& expression , OdString& evaluatorId ) const;

  /// <summary><para> 
  /// Set align angle of the resulting surface 
  /// </para><para>
  OdResult setAlignAngle(OdDbAssocActionPtr pAct, OdDouble angle, const OdString& expression ,
    const OdString& evaluatorId );

  /// <summary><para> 
  /// Get align angle of the resulting urfac
  /// </para><para>
  OdDouble alignAngle(const OdDbAssocActionPtr pAct, OdString& expression , OdString& evaluatorId ) const;

  /// <summary><para> 
  /// Set twist angle of the resulting surface
  /// </para><para>
  OdResult setTwistAngle(OdDbAssocActionPtr pAct, OdDouble angle, const OdString& expression ,
    const OdString& evaluatorId);

  /// <summary><para> 
  /// Get twist angle of the resulting surface
  OdDouble twistAngle(const OdDbAssocActionPtr pAct, OdString& expression , OdString& evaluatorId ) const;

private:
  OdUInt32 m_unknownInt32;
};
#include "TD_PackPop.h"

#endif
