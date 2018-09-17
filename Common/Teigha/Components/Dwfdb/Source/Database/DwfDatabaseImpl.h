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

#ifndef _DWFDATABASEIMPL_INCLUDED_
#define _DWFDATABASEIMPL_INCLUDED_

#include "DwfDatabase.h"
#include "DwfObjectImpl.h" // #include "DwfDbItemImpl.h"
#include "DwfObjectContainer.h"
#include "SharedPtr.h"

namespace DWFCore
{
  class DWFInputStream;
};
namespace DWFToolkit
{
  class DWFPackageReader;
  class DWFSection;
};

class DwfDatabaseImpl : public OdDwfObjectImpl
{
protected:
  OdSharedPtr<class OdHandleTree> m_pHandles; friend class OdDwfDatabase;
  friend class OdDwfHostAppServices;
  OdString m_sFilePathName;
  OdString m_password; friend class DwfStreamParserW2D; friend class OdDwfModelView;
  friend class OdDwfView;
  bool m_partialLoad, 
       m_multiParseLoad2d; // true to use initial multi-parse implementation for partial 2d
#if defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
public:
#endif
  bool m_isDwfx; // _xps
#if defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
protected:
#endif 
  OdDwfObjectContainer m_idsLayouts; friend class OdDwfViewLoadContext;
  OdDwfObjectContainer m_idsLayers;
  OdDwfObjectContainer m_idsMaterials; friend class OdDwfSegment;
  OdDwfObjectId	m_CurrentLayouId;

  mutable OdDwfHostAppServices* m_pDwfHostAppSvcs;
  OdUnitsFormatterPtr m_pFormatter;

  // loads old style DWF (ver<=55) || new DFW .w2d section 
  // path = path to old DWF || materialized w2d
  // return values described in "Dwfimport.h"
  OdResult loadPackage(DWFCore::DWFInputStream& stream);
  OdResult loadW2dSingleSheet(DWFCore::DWFInputStream& stream); // old version

  // load streams
  OdResult loadW3d(DWFToolkit::DWFSection* pSection);
  OdResult loadW2d(DWFToolkit::DWFSection* pSection, DWFToolkit::DWFPackageReader* pReader);

  friend class DwfUGeomBase; //public:
  static DWFToolkit::DWFPackageReader *createReader( DWFCore::DWFInputStream& stream, const OdChar* password, OdResult& result );

public:
  ODRX_DECLARE_MEMBERS(DwfDatabaseImpl);
  DwfDatabaseImpl();
  ~DwfDatabaseImpl();

  // To prevent access error on gcc 3.4 or borland
  bool getIsDwfx(){ return m_isDwfx;}
 
  // OdDwfDatabase methods :
  virtual OdDwfHostAppServices* appServices() const;
  virtual void readFile(OdStreamBuf* pStreamBuf,
                        bool partialLoad2d = false,
                        const OdString& password = OdString::kEmpty);
  virtual void readFile(const OdString& fileName,
                        bool partialLoad2d = false,
                        const OdString& password = OdString::kEmpty);
  virtual OdString getFilename() const;
  virtual OdString getPassword() const;

  OdDwfObjectId addObject(OdDwfObjectPtr pObj, 
                          const OdDbHandle& handle = OdDbHandle());
  OdDwfObjectId getObjectId(const OdDbHandle& handle, 
                            bool createIfNotFound);

  OdDwfObjectId currentLayoutId() const;
  void setCurrentLayout(const OdString& layoutName);
  void setCurrentLayout(const OdDwfObjectId& layoutId);

  OdDwfObjectContainer& layoutIds();
  OdDwfObjectContainer& layerIds();

  static DwfDatabaseImpl* getImpl(const OdDwfDatabase* pDwfDb);
  static DwfDatabaseImpl* getImpl(const OdRxObject* pRxDb);

protected:
  OdDwfObjectId getNewObjectId(const OdDbHandle& handle = OdDbHandle());
};

typedef OdSmartPtr<DwfDatabaseImpl> DwfDatabaseImplPtr;

#endif // _DWFDATABASEIMPL_INCLUDED_
