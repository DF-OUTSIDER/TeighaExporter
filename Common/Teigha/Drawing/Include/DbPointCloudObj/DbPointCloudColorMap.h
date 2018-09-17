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

#ifndef _OD_DB_POINTCLOUDCOLORMAP_
#define _OD_DB_POINTCLOUDCOLORMAP_

#include "DbPointCloudObjExports.h"
#include "DbObject.h"
#include "StringArray.h"

#include "TD_PackPush.h"

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdPoinCloudColorSchemeCollectionReactor
{
public: 

  virtual void PoinCloudColorSchemeCollectionChanged( class OdDbPointCloudColorMap* coloMap ) = 0;

  virtual ~OdPoinCloudColorSchemeCollectionReactor();
};

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudColorRamp
{
  friend class OdDbPointCloudColorMap;

public:

  OdDbPointCloudColorRamp();

  OdDbPointCloudColorRamp( const OdDbPointCloudColorRamp& source );

  OdDbPointCloudColorRamp& operator = (const OdDbPointCloudColorRamp& src);

  ~OdDbPointCloudColorRamp();

  OdInt32 numColors() const;

  OdResult setNumColors( int count );

  OdCmEntityColor color( int index ) const;

  void setColor( int index, OdCmEntityColor color ) const;

  bool visibility( int index ) const;

  void setVisibility( int index, bool visibility );

  void setFrom( const OdDbPointCloudColorRamp *source );

  const OdString name() const;

  void setName( const OdString &name );


private:

  // File IO
  OdResult  dwgInFields( OdDbDwgFiler *pFiler );
  void  dwgOutFields( OdDbDwgFiler *pFiler ) const;
  OdResult  dxfInFields( OdDbDxfFiler *pFiler );
  void  dxfOutFields( OdDbDxfFiler *pFiler ) const;

  class OdDbPointCloudColorRampImpl *pImpl;
};

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudClassificationColorRamp
{
  friend class OdDbPointCloudColorMap;

public:

  OdDbPointCloudClassificationColorRamp();

  OdDbPointCloudClassificationColorRamp( const OdDbPointCloudClassificationColorRamp& source );

  OdDbPointCloudClassificationColorRamp& operator = (const OdDbPointCloudClassificationColorRamp& src);

  ~OdDbPointCloudClassificationColorRamp();

  OdInt32 numColors() const;

  OdResult setNumColors( int count );

  OdCmEntityColor color( int index ) const;

  void setColor( int index, OdCmEntityColor color ) const;

  bool visibility( int index ) const;

  void setVisibility( int index, bool visibility );

  void setFrom( const OdDbPointCloudClassificationColorRamp *source );

  const OdString name() const;

  void setName( const OdString &name );

private:

  // File IO
  OdResult  dwgInFields( OdDbDwgFiler *pFiler );
  void  dwgOutFields( OdDbDwgFiler *pFiler ) const;
  OdResult  dxfInFields( OdDbDxfFiler *pFiler );
  void  dxfOutFields( OdDbDxfFiler *pFiler ) const;

  class OdDbPointCloudClassificationColorRampImpl *pImpl;
};

/** \details
This class represents PointCloudColorMap objects in an OdDbDatabase instance.

Corresponding C++ library: AcDbPointCloudObj

<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudColorMap : public OdDbObject
{
public:

  ODDB_DECLARE_MEMBERS( OdDbPointCloudColorMap );

  OdDbPointCloudColorMap();

  virtual ~OdDbPointCloudColorMap(){};

  static OdResult getColorMap( OdDbObjectId& id, OdDbDatabase *pDb, bool bCreateIfNone = true );

  void addColorSchemeCollectionReactor( OdPoinCloudColorSchemeCollectionReactor *pReactor );

  void removeColorSchemeCollectionReactor( OdPoinCloudColorSchemeCollectionReactor *pReactor );

  void notifyColorSchemeCollectionReactor();

  const OdString defaultIntensityColorScheme() const;

  void setDefaultIntensityColorScheme( const OdString guid );

  const OdString defaultElevationColorScheme() const;

  void setDefaultElevationColorScheme( const OdString guid );

  const OdString defaultClassificationColorScheme() const;

  void setDefaultClassificationColorScheme( const OdString guid );

  OdStringArray colorSchemeGUIDs() const;

  bool colorScheme( const OdChar *GUID, OdDbPointCloudColorRamp& target ) const;

  bool hasColorScheme( const OdChar *GUID ) const;

  bool setColorScheme( const OdChar *GUID, const OdDbPointCloudColorRamp& target );

  bool deleteColorScheme( const OdChar *GUID );

  OdStringArray classificationSchemeGUIDs() const;

  bool classificationScheme( const OdChar *GUID, OdDbPointCloudClassificationColorRamp& target ) const;

  bool hasClassificationScheme( const OdChar *GUID ) const;

  bool setClassificationScheme( const OdChar *GUID, const OdDbPointCloudClassificationColorRamp& target );

  bool deleteClassificationScheme( const OdChar *GUID );

  /** \details
  NOT IMPLEMENTED
  */
  void getColorSchemeInUse( OdStringArray &GUIDs );

  /** \details
  NOT IMPLEMENTED
  */
  void getClassificationColorSchemeInUse( OdStringArray &GUIDs );

  // File IO
  virtual OdResult dwgInFields( OdDbDwgFiler *pFiler );
  virtual void dwgOutFields( OdDbDwgFiler *pFiler ) const;
  virtual OdResult dxfInFields( OdDbDxfFiler *pFiler );
  virtual void dxfOutFields( OdDbDxfFiler *pFiler ) const;
};


typedef OdSmartPtr<OdDbPointCloudColorMap> OdDbPointCloudColorMapPtr;
#include "TD_PackPop.h"

#endif //_OD_DB_POINTCLOUDCOLORMAP_
