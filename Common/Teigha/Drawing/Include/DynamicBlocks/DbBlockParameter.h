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

#ifndef _DbBlockParameter_h_Included_
#define _DbBlockParameter_h_Included_

#include "TD_PackPush.h"

#include "DbBlockElement.h"
#include "DbEvalGraph.h"
#include "Ge/GeMatrix3d.h"
#include "DbDynBlockReference.h"

class OdDbBlockGrip;
typedef OdSmartPtr<OdDbBlockGrip> OdDbBlockGripPtr;
typedef OdArray< OdDbBlockGripPtr, OdMemoryAllocator<OdDbBlockGripPtr> > OdDbBlockGripPtrArray;

/** \details

    <group !!RECORDS_tdrawings_apiref>
*/
struct DYNBLOCKS_EXPORT OdDbBlkParamPropertyDescriptor
{
  OdDbBlkParamPropertyDescriptor();
  OdString m_sName;
  OdString m_sConnection;
  OdResBuf::ValueType m_nType;
  bool m_bReadonly;
  bool m_bVisible;
  OdString m_sDescription;
  bool m_bListPresent;
  OdDbDynBlockReferenceProperty::UnitsType m_nUnitsType;
  OdResBufArray m_pAllowedValues;
};
typedef OdArray< OdDbBlkParamPropertyDescriptor > OdDbBlkParamPropertyDescriptorArray;

/** \details
    This class represents block parameters in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockParameter : public OdDbBlockElement
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockParameter);
  enum ParameterComponent 
  {
    Base,
    End,
    BaseXEndY,
    EndXBaseY,
    InvalidComponent
  };
  //virtual void removedFromGraph( OdDbEvalGraph* );
  
  virtual OdString getPropertyConnectionName( const OdString& )const;
  virtual void getPropertyDescription( OdDbBlkParamPropertyDescriptorArray& )const;
  virtual OdResBufPtr getPropertyValue( const OdString& )const;
  virtual OdResBufPtr getPropertyValue( const OdString&, const OdGeMatrix3d& )const;
  
  //virtual void gripErased( ParameterComponent );
  //virtual bool historyRequired() const;
  //static bool isPropertyLabelUnique( OdDbEvalGraph*, const OdString &, class OdString* );
  //virtual void getGrips( OdDbBlockGripPtrArray&, OdDb::OpenMode );
  //virtual int getNumberOfGrips() const;
  
  virtual ParameterComponent getComponentForGrip( OdDbEvalNodeId );
  
  //virtual void removeGrip( ParameterComponent );
  //virtual void resetGrips();
  //virtual void setNumberOfGrips( int );
  
  virtual bool setPropertyValue( const OdString&, const OdResBuf* );
  virtual bool setPropertyValue( const OdString&, const OdGeMatrix3d&, const OdResBuf* );
  virtual bool chainActions()const;
  virtual void setChainActions( bool );
  virtual bool showProperties()const;
  virtual void setShowProperties( bool );
  OdDbEvalNodeId addGrip( ParameterComponent );
};

typedef OdSmartPtr<OdDbBlockParameter>OdDbBlockParameterPtr;

#include "TD_PackPop.h"

#endif // _DbBlockParameter_h_Included_
