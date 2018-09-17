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

#ifndef DbCenterLineActionBody_INCLUDED_
#define DbCenterLineActionBody_INCLUDED_ /*!DOM*/

#include "DbSmartCenterActionBody.h"
#include "DbSmartCenterObjectsParameters.h"

#include "TD_PackPush.h"

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT  OdDbCenterLineActionBody : public OdDbSmartCenterActionBody
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCenterLineActionBody);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbCenterLineActionBody(bool createImpObj = true);
  virtual void modified(const OdDbObject* pObject);
  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbCenterLineActionBody();

  //const OdDbSmartCenterLineParametersPtr parameters() const;
  //OdDbSmartCenterLineParametersPtr& parameters();
  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  //////////////////////////////////////////////////////////////////////////////////// From Arx
  //OdDbCenterLineObjectWrapper(OdDbCenterLineObjectWrapper const &);
  //OdDbCenterLineObjectWrapper(OdDbObjectId);
  //OdDbCenterLineObjectWrapper &  operator=(OdDbCenterLineObjectWrapper const &);
  //void __autoclassinit2(unsigned __int64);

  //OdGeVector3d cachedCenterLineDirection(void)const;
  //double cachedCenterLineLength(void)const;
  //int edgeRefDirEnding(int);
  enum OdResult edgeRefDirEndingArray(OdArray<int>  &);// , OdArrayMemCopyReallocator<int> > &);
  //double endExtension(void)const;
  double endOvershoot(void)const;
  enum OdResult getCenterLineGeometry(OdGeLineSeg3d &, bool);
  //enum OdResult getCenterPoint(OdGePoint3d &);
  //enum OdResult setCachedCenterLineDirection(OdGeVector3d const &);
  //enum OdResult setCachedCenterLineLength(double);
  //enum OdResult setEdgeRefDirEnding(int, int);
  enum OdResult setEdgeRefDirEndingArray(OdArray<int>  &);// , OdArrayMemCopyReallocator<int> > &);
  //enum OdResult setEndExtension(double);
  enum OdResult setEndOvershoot(double);
  //enum OdResult setStartExtension(double);
  enum OdResult setStartOvershoot(double);
  //double startExtension(void)const;
  double startOvershoot(void)const;
  //static enum SmartCenterType wrapperCenterType(void);

  /** \details
  Creates associative centerLine: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    OdArray<int> inputArr,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);

  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    //OdGePoint3dArray ptsSelected,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);
  /** \details
  Method is called by the corresponding method of the parent action which owns this action
  (see OdDbAssocAction::isRelevantDependencyChange() method).

  \remarks Not implemented
  */
  virtual OdResult isRelevantDependencyChangeOverride(const OdDbAssocDependency* pDependency,
    bool& isRelevantDepChange) const
  {
    isRelevantDepChange = false;
    return eOk;
    //return eNotImplemented;
  }
};

typedef OdSmartPtr<OdDbCenterLineActionBody> OdDbCenterLineActionBodyPtr;

#include "TD_PackPop.h"

#endif

