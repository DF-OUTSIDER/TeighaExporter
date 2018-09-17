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

#ifndef DbCenterMarkActionBody_INCLUDED_
#define DbCenterMarkActionBody_INCLUDED_ /*!DOM*/

#include "DbSmartCenterActionBody.h"
#include "DbSmartCenterObjectsParameters.h"

#include "TD_PackPush.h"
//#include "GeCircArc3d.h"

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbCenterMarkActionBody : public OdDbSmartCenterActionBody
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCenterMarkActionBody);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbCenterMarkActionBody(bool createImpObj = true);

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbCenterMarkActionBody();
  virtual void modified(const OdDbObject* pObject);

  //const OdDbSmartCenterMarkParametersPtr parameters() const;
  //OdDbSmartCenterMarkParametersPtr& parameters();

  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  //////////////////////////////////////////////////////////////////////////////////// From Arx
  /*OdDbCenterMarkObjectWrapper(OdDbCenterMarkObjectWrapper const & ) ;
  OdDbCenterMarkObjectWrapper(OdDbObjectId) ;
  OdDbCenterMarkObjectWrapper &  operator=(OdDbCenterMarkObjectWrapper const & ) ;
  void __autoclassinit2(unsigned __int64) ;*/

  //double crossHairGap(void)const;
  OdString crossHairGapExpression(void)const;
  //double crossHairLength(void)const;
  OdString crossHairLengthExpression(void)const;
  //enum OdResult getAllExtensionPoints(OdArray<OdGePoint3d,OdArrayMemCopyReallocator<OdGePoint3d> > & ) ;
  //enum OdResult getAllExtensionPoints(OdGePoint3d &, OdGePoint3d &, OdGePoint3d &, OdGePoint3d &);
  //enum OdResult getAllOvershootPoints(OdArray<OdGePoint3d,OdArrayMemCopyReallocator<OdGePoint3d> > & ) ;
  //enum OdResult getAllOvershootPoints(OdGePoint3d &, OdGePoint3d &, OdGePoint3d &, OdGePoint3d &);
  //enum OdResult getCenterPoint(OdGePoint3d &);
  //enum OdResult getCrossLengthAndGap(double &, double &)const;
  //double getExtensionLineLength(enum AcCenterMarkSubEntType)const;
  //OdDb::Visibility getExtensionLinesVisibility(void)const;
  OdResult getRefCircle(OdGeCircArc3d &)const;
  //double getRefCircleRadius(void)const;
  //bool getSubEntVisibility(enum AcCenterMarkSubEntType)const;
  //OdGeVector3d getSubentDirection(enum AcCenterMarkSubEntType)const;
  //OdGeLineSeg3d getSubentLineGeometry(enum AcCenterMarkSubEntType, bool)const;
  //double horizontalEndExtension(void)const;
  //OdGePoint3d horizontalEndExtensionPoint(void);
  double horizontalEndOvershoot(void)const;
  //OdGePoint3d horizontalEndOvershootPoint(void);
  //double horizontalStartExtension(void)const;
  //OdGePoint3d horizontalStartExtensionPoint(void);
  double horizontalStartOvershoot(void)const;
  //OdGePoint3d horizontalStartOvershootPoint(void);
  //bool isCenterCrossTooLarge(enum AcCenterMarkSubEntType)const;
  double rotation(void)const;
  //enum OdResult setAllOvershoots(double);
  //enum OdResult setCachedCircleRadius(double);
  enum OdResult setCrossHairGapExpression(OdString const &);
  enum OdResult setCrossHairLengthExpression(OdString const &);
  enum OdResult setExtensionLinesVisibility(OdDb::Visibility);
  //enum OdResult setHorizontalEndExtension(double);
  enum OdResult setHorizontalEndOvershoot(double);
  //enum OdResult setHorizontalStartExtension(double);
  enum OdResult setHorizontalStartOvershoot(double);
  enum OdResult setRotation(double);
  //enum OdResult setSubEntVisibility(enum AcCenterMarkSubEntType, bool);
  //enum OdResult setVerticalEndExtension(double);
  enum OdResult setVerticalEndOvershoot(double);
  //enum OdResult setVerticalStartExtension(double);
  enum OdResult setVerticalStartOvershoot(double);
  //void updateCachedCircleRadius(void);
  //double verticalEndExtension(void)const;
  //OdGePoint3d verticalEndExtensionPoint(void);
  double verticalEndOvershoot(void)const;
  //OdGePoint3d verticalEndOvershootPoint(void);
  //double verticalStartExtension(void)const;
  //OdGePoint3d verticalStartExtensionPoint(void);
  double verticalStartOvershoot(void)const;
  //OdGePoint3d verticalStartOvershootPoint(void);
  //static enum SmartCenterType wrapperCenterType(void);

  /** \details
  Creates associative action, action body, dependencies etc.
  */
  /** \details
  Creates associative centerLine: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPath subentPath,
    OdDbObjectId& centerMarkId,
    OdDbObjectId& actionBodyId);
};

typedef OdSmartPtr<OdDbCenterMarkActionBody> OdDbCenterMarkActionBodyPtr;

#endif

