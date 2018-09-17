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

#ifndef _DbBlockElement_h_Included_
#define _DbBlockElement_h_Included_

#include "DbEvalGraph.h"
#include "IntArray.h"
#include "Ge/GePoint3dArray.h"

class OdDbBlockTableRecord;
class OdDbBlockRepresentationContext;

#define ODDB_EVAL_DECLARE_MEMBERS(ClassName) \
  ODDB_DECLARE_MEMBERS(ClassName);\
  OdResult dwgInFields(OdDbDwgFiler* pFiler);\
  void dwgOutFields(OdDbDwgFiler* pFiler) const;\
  OdResult dxfInFields(OdDbDxfFiler* pFiler);\
  void dxfOutFields(OdDbDxfFiler* pFiler) const

// #define DEBUG_EVALGRAPH 1

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockElement : public OdDbEvalConnectable
{
public:
  enum HistoryCompression{kHistoryCompression0,kHistoryCompression1,kHistoryCompression2,kHistoryCompression3};
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockElement);
  //!DOM EvalExpr interface
  //!DOM virtual void adjacentNodeRemoved( const OdDbEvalNodeId& );
  virtual bool evaluate( const OdDbEvalContext* );
  //!DOM OdDbEvalConnectable interface
  virtual bool connectionAllowed(const OdString&,OdDbEvalNodeId,const OdString&) const;
  virtual bool connectTo(const OdString&,OdDbEvalNodeId,const OdString&);
  virtual bool disconnectFrom(const OdString&,OdDbEvalNodeId,const OdString&);
  virtual bool hasConnectionNamed(const OdString&) const;
  virtual bool getConnectedNames(const OdString&,unsigned long,OdStringArray&) const;
  virtual bool getConnectedObjects(const OdString&,OdDbEvalNodeIdArray&) const;
  virtual void getConnectionNames(OdStringArray&) const;
  virtual bool getConnectionType  (const OdString& name, OdResBuf::ValueType& type) const;
  virtual OdResBufPtr getConnectionValue (const OdString& name) const;
  //!DOM  
  virtual OdString name() const;
  virtual void setName( const OdString& );
  //!DOM virtual unsigned long alertState() const;
  //!DOM virtual void auditAlertState();
  //!DOM OdDbObjectId getEntity();
  //!DOM class AcRxClass * getRxEntity(void)
  virtual OdDb::MaintReleaseVer getInstanceMaintenanceVersion();
  virtual OdDb::DwgVersion  getInstanceVersion();
  virtual void getStretchPoints( OdGePoint3dArray& )const;
  virtual void moveStretchPointsAt( const OdIntArray &, const OdGeVector3d &);
  virtual HistoryCompression historyCompression() const;
  virtual bool historyRequired() const;
  //!DOM static bool isNameUnique(class OdDbEvalGraph* graph, const OdString& name, OdString* result );
  virtual bool hasInstanceData()const; // has instance data loaded
  virtual bool loadInstanceData(const OdResBuf*, bool bRequireEvaluate = false); // loads instance data from xdata chain
  virtual OdResBufPtr saveInstanceData()const; // saves instance data to xdata chain
  virtual void updateConnections();
  //!DOM virtual bool onBeginEdit( OdDbBlockTableRecord* );
  //!DOM virtual bool onEndEdit(OdDbBlockTableRecord*);
    /*!DOM 
  void OnBeginEditCancelled(class AcDbObjectId,class AcDbObjectId)
  void OnBeginEditEnded(class AcDbObjectId,class AcDbObjectId)
  void OnBeginSaveCancelled(class AcDbObjectId,class AcDbObjectId)
  void OnBeginSaveEnded(class AcDbObjectId,class AcDbObjectId)
  void OnBeginSaveStarted(class AcDbObjectId,class AcDbObjectId)
  void OnEndEditCancelled(class AcDbObjectId,class AcDbObjectId)
  void OnEndEditStarted(class AcDbObjectId,class AcDbObjectId)
  */
  virtual bool isMemberOfCurrentVisibilitySet()const;
  virtual void setMemberOfCurrentVisibilitySet(bool);
  //!DOM   enum Acad::ErrorStatus  AcDbBlockElement::sync(class AcDbBlockElementEntity *)
  //!DOM  virtual void transformDefinitionBy( const OdGeMatrix3d &);
  virtual OdResult transformBy( const OdGeMatrix3d& );
};
typedef OdSmartPtr<OdDbBlockElement>OdDbBlockElementPtr;

#endif // _DbBlockElement_h_Included_
