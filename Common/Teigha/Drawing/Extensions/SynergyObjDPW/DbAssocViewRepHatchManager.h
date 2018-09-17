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

#ifndef DbAssocViewRepHatchManager_INCLUDED_
#define DbAssocViewRepHatchManager_INCLUDED_ /*!DOM*/

#include "SynergyObjDPWExports.h" 
#include "DbAssocActionParam.h"

#include "TD_PackPush.h"

/** \details
  This class represents the view hatch action manager.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbAssocViewRepHatchManager : public OdDbAssocCompoundActionParam
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocViewRepHatchManager);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocViewRepHatchManager(bool createImpObj = true);

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


	bool     isEmpty() const;                    // true, if Count of DbAssocViewRepHatchActionParam > 0;
  OdResult getShowHatch(bool &) const;         // if Hatch show, return true, else false
	OdResult setShowHatch(bool,bool);            // (false/true, ?) - clear/draw Hatch


	bool checkConsistency() const;
	OdResult detectPropertyOverrides(OdDbObjectId const &,OdDbObjectId const &,OdDbObjectId const &,OdDbObjectId const &) const;
	OdDbObjectId findHatchParam(OdDbObjectId const &) const;
//	void outputBodyHatch(struct aciv::DVO::BodyHatch const &,OdDbImpViewRep const *,OdDbSectionViewStyle const *);
	void processErasedOrBrokenViewRepHatches();
	void removeObsoleteChildren();
	void setObsolete(bool) const;
	OdResult setStatusToAssocDependency(enum OdDbAssocStatus) const;
	OdResult updateHatchProperties(OdDbObjectId const &,OdDbObjectId const &,OdDbObjectId const &) const;
//	OdResult updateViewRepHatches(OdDbImpViewRep const *,OdGeMatrix3d const &);
	void viewRepHatches(OdDbObjectIdArray &hatchActionParamIds) const;
	static void viewRepHatches(OdDbObjectIdArray &, OdDbObjectIdArray const &);

private:
//	 OdResult addNewHatch(OdUInt64,OdUInt32 &,struct aciv::DVO::HatchBoundary const &,OdDbImpViewRep const *,OdDbSectionViewStyle const *,OdDbObjectIdArray const &);
	 void clear();
	 OdResult getBodyId4ParamId(OdUInt64 &,OdDbObjectId const &) const;
	 OdResult getHatchParam4Component(OdDbObjectId &,OdUInt64,bool,OdGeVector3d const &) const;
	 OdUInt32 getLastUnusedHatchIndex() const;




};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocViewRepHatchManager class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocViewRepHatchManager> OdDbAssocViewRepHatchManagerPtr;

#include "TD_PackPop.h"

#endif // DbAssocViewRepHatchManager_INCLUDED_
