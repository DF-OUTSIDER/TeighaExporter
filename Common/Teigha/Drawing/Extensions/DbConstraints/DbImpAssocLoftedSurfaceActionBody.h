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
#ifndef DbImpAssocLoftedSurfaceActionBody_INCLUDED_
#define DbImpAssocLoftedSurfaceActionBody_INCLUDED_ /*!DOM*/

#include "DbImpAssocExtrudedSurfaceActionBody.h"
#include "TD_PackPush.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on AnnotationActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

class DBCONSTRAINTS_EXPORT OdDbImpAssocLoftedSurfaceActionBody : public OdDbImpAssocExtrudedSurfaceActionBody// OdDbImpAssocAnnotationActionBody
{
public:
  OdDbImpAssocLoftedSurfaceActionBody();
  ~OdDbImpAssocLoftedSurfaceActionBody();
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
	OdDbAssocLoftParametersPtr parameters() const;
  OdDbAssocLoftParametersPtr& parameters();
  ////////////////////////////////////////////////////////////////////// for parent classes
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);

  /// <summary>
  /// <para> Get continuity of a specific of the resulting surface </para>
  /// <para>
  OdResult getContinuity(OdDbImpAssocLoftParameters::ProfileType type, int &continuity, OdDbAssocActionPtr pAction, OdString& expression  /*= OdString()*/,
    OdString& evaluatorId /*= OdString()*/) const;

  /// <summary>
  /// <para> Set continuity of a specific of the resulting surface </para>
  /// <para>
  OdResult setContinuity(OdDbImpAssocLoftParameters::ProfileType type, int continuity, OdDbAssocActionPtr pAction, const OdString& expression /* = OdString()*/,
    const OdString& evaluatorId /*= OdString()*/);

  /// <summary>
  /// <para> Get bulge factor of a specific of the resulting surface </para>
  /// <para>
  OdResult getBulge(OdDbImpAssocLoftParameters::ProfileType type, double &bulge, OdDbAssocActionPtr pAction, OdString& expression /*= OdString()*/,
    OdString& evaluatorId /*= OdString()*/) const;

  /// <summary>
  /// <para> Set bulge of a specific of the resulting surface </para>
  /// <para>
  OdResult setBulge(OdDbImpAssocLoftParameters::ProfileType type, double bulge, OdDbAssocActionPtr pAction, const OdString& expression/* = OdString()*/,
    const OdString& evaluatorId /*= OdString()*/);

  /// <summary> 
  /// Create an instance of OdDbAssocLoftedSurfaceActionBody
  /// </summary>
  static OdResult createInstance(
		const OdDbFullSubentPathArray crossSectionsArray,
		const OdDbFullSubentPathArray guideCurves,// TODO
		const OdDbFullSubentPath pathCurve,				// TODO
		const OdDbLoftOptions& loftOptions,				// TODO
		//const OdArray<int>& continuityArray,
		//const OdArray<double>& bulgeArray,
		//bool bEnabled,
	  OdDbObjectId& actionBodyId);

  virtual OdStringArray compareWith(OdDbImpAssocActionBody *paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

protected:
  OdDbAssocLoftParametersPtr m_pParams;
  short m_continuity;
  double m_bulge;
private:
  OdUInt32 m_unknownInt32;
};

#include "TD_PackPop.h"

#endif
