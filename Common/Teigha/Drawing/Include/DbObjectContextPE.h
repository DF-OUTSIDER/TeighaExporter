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

#ifndef _ODDBOBJECTCONTEXTPE_H_ICLUDED_
#define _ODDBOBJECTCONTEXTPE_H_ICLUDED_

#include "DbObjectContextInterface.h"
#include "DbObjectContextCollection.h"
#include "DbObjectContextData.h"

/** \details
<group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectContextDefaultPE : public OdDbObjectContextInterface
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextDefaultPE);
  virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual bool hasContext(const OdDbObject*,const OdDbObjectContext&) const;
  virtual OdResult removeContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual bool supportsCollection(const OdDbObject*,const OdString&) const;
};

class OdDbContextDataSubManager;

/** \details
<group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectContextPE : public OdDbObjectContextInterface
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextPE);
  virtual OdResult setDefaultContext(OdDbObject*,const OdDbObjectContext&);
  virtual OdDbObjectContextDataPtr getDefaultContextData(const OdDbObject* ,const OdString& /*,bool*/) const;
  virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject* pObj,const OdDbObjectContext& ctx, const OdDbObjectContext& def) const = 0;
  virtual bool hasContext(const OdDbObject*,const OdDbObjectContext&) const;
  virtual OdResult removeContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual bool supportsCollection(const OdDbObject*,const OdString&) const;
  virtual OdDbObjectContextDataPtr getContextData(const OdDbObject*,const OdDbObjectContext&) const;
  virtual void compose(OdDbObject* pObj);
/*  
  virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&,OdDbObjectContextData*) const;
  virtual OdDbObjectContextCollection* contextCollection(const OdDbObject*,const OdString&) const;
  virtual OdResult addCurrentContext(OdDbObject*,const OdString&,OdDbObjectContextData*) const;
  //virtual OdResult copyContextData(const OdDbObject*,OdDbObject*) const;
  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdResult deleteContextData(OdDbObject*) const;
  virtual OdResult dwgInContextData(const OdDbObject*,OdDbDwgFiler*) const;
  virtual OdResult dwgOutContextData(const OdDbObject*,OdDbDwgFiler*) const;
  OdDbContextDataSubManager* getContextDataSubManager(const OdDbObject*,const OdDbObjectContext&,bool,bool) const;
  OdDbContextDataSubManager* getContextDataSubManager(const OdDbObject*,const OdString&,bool,bool) const;
  virtual OdResult getContexts(const OdDbObject*,const OdString&,OdRxObjectPtrArray&) const;
  virtual OdDbObjectContextData*  getCurrentContextData(const OdDbObject*,const OdString&,bool) const;
  static OdDbObjectContextPE* getObjectContextPE(const OdDbObject*);
  virtual bool  hasContext(const OdDbObject*,const OdDbObjectContext&) const;
  virtual int  numContexts(const OdDbObject*,const OdString&) const;
  virtual OdResult removeAllContexts(OdDbObject*,const OdString&) const;
  virtual OdResult removeContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual OdResult removeContext(OdDbObject*,OdDbObjectContextData*) const;
  virtual OdResult replaceContext(OdDbObject*,OdDbObjectContextData*,bool) const;
  virtual OdResult saveContextData(const OdDbObject*) const;
  virtual bool  supportsCollection(const OdDbObject*,const OdString&) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);
  OdResult updateContextData(const OdDbObject*,void *) const;
  static void init();
  static void uninit();*/
};

typedef OdSmartPtr<OdDbObjectContextPE> OdDbObjectContextPEPtr;

/** \details
<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbObjectContextHatchPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextHatchPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const ODRX_OVERRIDE;
  virtual OdResult addContext(OdDbObject* pObject, const OdDbObjectContext& ctx) const ODRX_OVERRIDE;
/*
  virtual OdDbObjectContextData*  getContextData(const OdDbObject*,const OdDbObjectContext&) const;
  virtual OdDbObjectContextData*  getCurrentContextData(const OdDbObject*,const OdString&,bool) const;
  virtual bool  hasContext(const OdDbObject*,const OdDbObjectContext&) const;
  virtual OdResult removeAllContexts(OdDbObject*,const OdString&) const;
  virtual OdResult removeContext(OdDbObject*,const OdDbObjectContext&) const;
  virtual bool  supportsCollection(const OdDbObject*,const OdString&) const;
  */
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextMTextPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextMTextPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
  //virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&,OdDbObjectContextData*) const;
};

/** \details
<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbObjectContextTextPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextTextPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
};
typedef OdSmartPtr<OdDbObjectContextTextPE> OdDbObjectContextTextPEPtr;

/** \details
<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbObjectContextAttributePE : public OdDbObjectContextTextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextAttributePE);
  virtual OdResult addContext(OdDbObject* pObject, const OdDbObjectContext& ctx) const ODRX_OVERRIDE;
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextLeaderPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextLeaderPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextMLeaderPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextMLeaderPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const ODRX_OVERRIDE;
  virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&) const ODRX_OVERRIDE;
};
typedef OdSmartPtr<OdDbObjectContextMLeaderPE> OdDbObjectContextMLeaderPEPtr;

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextFcfPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextFcfPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
};

/** \details
<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbObjectContextBlkRefPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextBlkRefPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const ODRX_OVERRIDE;
  virtual OdResult setDefaultContext(OdDbObject*,const OdDbObjectContext&) ODRX_OVERRIDE;
  virtual OdResult addContext(OdDbObject*,const OdDbObjectContext&) const ODRX_OVERRIDE;
  virtual OdResult removeContext(OdDbObject*,const OdDbObjectContext&) const ODRX_OVERRIDE;
  virtual void compose(OdDbObject*);
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextDimPE : public OdDbObjectContextPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextDimPE);
  virtual OdResult addContext(OdDbObject* pObject, const OdDbObjectContext& ctx) const;
/*  virtual void *  contextDataManager(const OdDbObject*,bool) const;
  virtual OdDbObjectContextData*  createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult dwgInContextData(const OdDbObject*,OdDbDwgFiler*) const;
  virtual OdResult saveContextDataManager(OdDbObject*,void *) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextAlDimPE : public OdDbObjectContextDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextAlDimPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextAngDimPE : public OdDbObjectContextDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextAngDimPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextDmDimPE : public OdDbObjectContextDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextDmDimPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextOrdDimPE : public OdDbObjectContextDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextOrdDimPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextRaDimPE : public OdDbObjectContextDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextRaDimPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

/** \details
<group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbObjectContextRaDimLrgPE : public OdDbObjectContextRaDimPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextRaDimLrgPE);
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdDbObjectContext&, const OdDbObjectContext& def) const;
/*  virtual OdDbObjectContextData* copyExtDictContextData(const OdDbObject*,OdDbObjectContextData*) const;
  virtual OdDbObjectContextDataPtr createContextData(const OdDbObject*,const OdString&,const OdDbObjectContext*,OdDbObjectContextData*,const OdDbContextDataSubManager*) const;
  virtual OdResult synchronizeContextData(const OdDbObject*,const OdDbObjectContext&);*/
};

TOOLKIT_EXPORT bool oddbGetContextDataAndScale(class OdGiCommonDraw* pWd, const OdDbObject* pObject, OdDbAnnotScaleObjectContextDataPtr& ctx, double* scaleOut = 0, bool getDefaultScale = false);

#endif
