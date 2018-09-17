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

#ifndef _ODDB_BASEDATABASE_IMPL_H_
#define _ODDB_BASEDATABASE_IMPL_H_

#include "RxObjectImpl.h"
#include "DbBaseDatabase.h"

#include "TD_PackPush.h"

//----------------------------------------------------------
// OdBaseDatabasePEImpl

class DBROOT_EXPORT OdBaseDatabasePEImpl : public OdDbBaseDatabasePE
{
public:
  ODRX_DECLARE_MEMBERS(OdBaseDatabasePEImpl);

  //virtual OdDbBaseHostAppServices* appServices(OdDbBaseDatabase* pDb) const;
  //virtual OdString getFilename(OdDbBaseDatabase* pDb) const;
  //virtual OdGiDefaultContextPtr createGiContext(OdDbBaseDatabase* pDb) const;
  virtual void putNamedViewInfo(OdDbBaseDatabase* pDb, OdDbStub* layoutInfo, 
                                OdStringArray& names, OdGePoint3dArray& points);
  virtual OdBaseTextIteratorPtr createTextIterator(OdDbBaseDatabase* db, const OdChar* textString,
                                                   int length, bool raw, const OdGiTextStyle* pTextStyle) const;
  //virtual OdGsDevicePtr setupActiveLayoutViews(OdGsDevice* pDevice, OdGiDefaultContext* pGiCtx);
  virtual OdGsDevicePtr setupLayoutView(OdGsDevice* pDevice, OdGiDefaultContext* pDgnContext,
                                        OdDbStub* objectId);
  virtual void setupPalette(OdGsDevice* device, OdGiDefaultContext* giContext, 
                            OdDbStub* layoutId = 0, ODCOLORREF palBg = ODRGBA(0,0,0,0));
  virtual OdDbStub* getNextViewForActiveLayout(OdGiDefaultContext* pDgnContext, OdDbStub* objectId);
  virtual void loadPlotstyleTableForActiveLayout(OdGiDefaultContext* pTdContext, OdDbBaseDatabase* db);

  virtual void startTransaction(OdDbBaseDatabase* pDb);
  virtual void abortTransaction(OdDbBaseDatabase* pDb);

  virtual void applyLayoutSettings(OdGsDCRect& clipBox, OdGsDevice* pDevice, OdDbBaseDatabase* db, OdUInt32 extentsFlags = kZeroFlags, OdUInt32 dpi = 72);
  virtual void zoomToExtents(const OdGsDCRect& outputRect, OdGsDevice* pDevice, OdDbBaseDatabase* db, OdGeBoundBlock3d& plotExtents,
                             OdUInt32 extentsFlags = kZeroFlags, OdDbStub* objectId = NULL);
  virtual OdRxIteratorPtr layers(OdDbBaseDatabase* db) const;
  virtual OdRxIteratorPtr visualStyles(OdDbBaseDatabase* db) const;
  virtual OdDbStub* getVisualStyleId(OdDbBaseDatabase* db, const OdString& name) const;
  //virtual void setCurrentLayout(OdDbBaseDatabase* db, const OdString& name);
  //virtual OdRxIteratorPtr layouts(OdDbBaseDatabase* db) const;

  //virtual OdDbStub* findLayoutIdByViewport(OdDbBaseDatabase* db, OdDbStub* pViewportId);
  //virtual OdDbStub* getFirstLayoutId(OdDbBaseDatabase* db);

  // returns the first visible view Id associated with model named name
  virtual OdDbStub* findLayoutNamed(OdDbBaseDatabase* db, const OdString& name);
  virtual OdTimeStamp getCreationTime(OdDbBaseDatabase* db);
  virtual OdTimeStamp getUpdateTime(OdDbBaseDatabase* db);
  virtual OdString getFingerPrintGuid(OdDbBaseDatabase* db);
  virtual OdString getVersionGuid(OdDbBaseDatabase* db);
  //virtual OdDbBaseViewTableIteratorPEPtr getViewTableIterator(OdDbBaseDatabase* db);
  virtual int getUnits(OdDbBaseDatabase* db);
  virtual OdUnitsFormatter* baseFormatter(OdDbBaseDatabase* db);
  //virtual OdDbBaseViewTablePEPtr getViewTable(OdDbBaseDatabase* db);
  virtual OdSmartPtr<class OdDbBaseViewportTablePE> getViewportTable(OdDbBaseDatabase* db);
  //virtual OdString getLayoutName(OdDbStub* elementId);
  //virtual OdDbBaseLayoutIteratorPEPtr newLayoutIteratorPE(OdDbBaseDatabasePE* db);
  virtual int getMeasurement(OdDbBaseDatabase* db);
  virtual bool getLineTypeById( OdDbBaseDatabase* db, OdDbStub* pLTypeId, OdGiLinetype& LType );
  virtual bool getTextStyleById( OdDbBaseDatabase* db, OdDbStub* idStyle, OdGiTextStyle& shapeInfo );  

  virtual OdDbStub* getId(const OdRxObject* obj) const;
  virtual OdDbStub *getObject(OdDbBaseDatabase* db, OdUInt64 handle);
  virtual OdCodePageId getCodePage(OdDbBaseDatabase* db);
  virtual OdDbStub* getModelBlockId(OdDbBaseDatabase* pDb);
  virtual OdDbStub* getPaperBlockId(OdDbBaseDatabase* pDb);
  //virtual OdDbStub* currentLayoutId(OdDbBaseDatabase* pDb);
  virtual OdDbStub* xrefBlockId(OdDbBaseDatabase* pDb);
  virtual void setMultiThreadedRender(OdDbBaseDatabase* pDb, bool bOn);
  virtual bool isAProxy(OdRxObject* pDrw);
  //virtual OdRxObjectPtr openObject(OdDbStub* pId);
  virtual bool getAnnoScaleSet(OdDbStub* drawableId, OdGiAnnoScaleSet& res);
  virtual OdDbStub* getCurrentLongTransation(const OdDbBaseDatabase* pDb);
  virtual OdRxObjectPtr addDatabaseUnloadReactor(OdDbBaseDatabase* pDb, OdRxObject* pPrevReactor, DatabaseUnloadReactor* pReactorRedirect);
  virtual void removeDatabaseUnloadReactor(OdDbBaseDatabase* pDb, OdRxObject* pReactor);
  virtual OdResult createMaterial(OdDbStub*& material, OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb,
    const OdGiMaterialTraits* pMaterialTraits, const OdGiMapper* pMaterialMapper = 0, const OdCmEntityColor* pMaterialColor = 0);
};

//----------------------------------------------------------
// OdBaseLayerPEImpl

class DBROOT_EXPORT OdBaseLayerPEImpl : public OdDbBaseLayerPE
{
public:
  ODRX_DECLARE_MEMBERS(OdBaseLayerPEImpl);

  OdString name(const OdRxObject* obj) const;
  virtual bool isOff(const OdRxObject*) const;
  virtual bool isFrozen(const OdRxObject*) const;
  virtual void setIsOff(OdRxObject*, bool) const;
  virtual void setIsFrozen(OdRxObject*, bool) const;
  virtual bool getLineType(OdRxObject* obj, OdGiLinetype& LType) const;
};

//----------------------------------------------------------
// OdBaseVisualStylePEImpl

class DBROOT_EXPORT OdBaseVisualStylePEImpl : public OdDbBaseVisualStylePE
{
public:
  ODRX_DECLARE_MEMBERS(OdBaseVisualStylePEImpl);

  virtual OdString name(const OdRxObject* obj) const;
  virtual bool isInternal(const OdRxObject* obj) const;
};

//----------------------------------------------------------
// OdBaseLayoutPEImpl

class DBROOT_EXPORT OdBaseLayoutPEImpl : public OdDbBaseLayoutPE
{
public:
  ODRX_DECLARE_MEMBERS(OdBaseLayoutPEImpl);

  //virtual OdString name(const OdRxObject*);
  //virtual bool isModelLayout(const OdRxObject*);
  virtual bool printLineweights(const OdRxObject*);
  virtual bool showPlotStyles(const OdRxObject*);
  virtual OdString paperName(const OdRxObject*);
  virtual void getPaperSize(const OdRxObject*, double& paperWidth, double& paperHeight) const;
  virtual OdDbBaseLayoutPE::PlotRotation plotRotation(const OdRxObject*);
  virtual double getTopMargin(const OdRxObject*);
  virtual double getRightMargin(const OdRxObject*);
  virtual double getBottomMargin(const OdRxObject*);
  virtual double getLeftMargin(const OdRxObject*);
  virtual bool isOverallVPortErased(const OdRxObject*);
  virtual OdResult getGeomExtents(const OdRxObject*, OdGeExtents3d& ext);

  virtual bool useStandardScale(const OdRxObject*);
  virtual void getStdScale(const OdRxObject*, double& scale);
  virtual void getCustomPrintScale(const OdRxObject*, double& numerator, double& denominator);
  virtual int plotType(const OdRxObject*);
  virtual OdString getPlotViewName(const OdRxObject*);
  virtual void getPlotWindowArea(const OdRxObject*, double& xmin, double& ymin, double& xmax, double& ymax);
  virtual void getPlotOrigin(const OdRxObject*, double& x, double& y);
  virtual void getPlotPaperSize(const OdRxObject*, double& paperWidth, double& paperHeight);
  virtual int plotPaperUnits(const OdRxObject*);

  //virtual OdDbStub* getBlockId(const OdRxObject*) const;
  virtual bool scalePSLinetypes(const OdRxObject*) const;
  virtual bool getApproxExtents(const OdRxObject*, OdGePoint3d& extMin, OdGePoint3d& extMax) const;
};

template<class ClassIterator>
struct ObjectIteratorWrapper : OdRxIterator
{
  OdSmartPtr<ClassIterator> iter;

  virtual bool done() const 
  {
    return iter.isNull() || iter->done(); 
  }
  virtual bool next() 
  { 
    iter->step(); 
    return !iter->done();
  }
  virtual OdRxObjectPtr object() const 
  {
    OdRxObjectPtr pObject = iter->item().openObject();
    return pObject;
  }
  static OdRxIteratorPtr createObject()
  {
    OdRxIteratorPtr pIterator = OdRxObjectImpl<ObjectIteratorWrapper>::createObject();
    return pIterator;
  }
};


// OdBaseVisualStylePEImpl

class DBROOT_EXPORT OdBaseMaterialPEImpl : public OdDbBaseMaterialPE
{
public:
  ODRX_DECLARE_MEMBERS(OdBaseMaterialPEImpl);

  virtual OdString name(const OdRxObject* obj) const;
  virtual bool isDefault(const OdRxObject* obj) const;
};

// possible TODO Inherit OdCmColor and others from next class

class DBROOT_EXPORT OdCmColorBaseImpl : public OdCmColorBase
{
protected:
  OdCmEntityColor::RGBM m_RGBM;
  OdString m_colorName;
  OdString m_bookName;
public:
  ODRX_HEAP_OPERATORS();
  OdCmColorBaseImpl();
  virtual OdCmEntityColor::ColorMethod colorMethod() const;
  virtual void setColorMethod(OdCmEntityColor::ColorMethod colorMethod);
  virtual bool isByColor() const;
  virtual bool isByLayer() const;
  virtual bool isByBlock() const;
  virtual bool isByACI() const;
  virtual bool isForeground() const;
  virtual bool isByDgnIndex() const;
  virtual bool isNone() const;
  virtual OdUInt32 color() const;
  virtual void setColor(OdUInt32 color);
  virtual void setRGB(OdUInt8 red, OdUInt8 green, OdUInt8 blue);
  virtual void setRed(OdUInt8 red);
  virtual void setGreen(OdUInt8 green);
  virtual void setBlue(OdUInt8 blue);
  virtual OdUInt8 red() const;
  virtual OdUInt8 green() const;
  virtual OdUInt8 blue() const;
  virtual OdUInt16 colorIndex() const;
  virtual void setColorIndex(OdUInt16 colorIndex);
  virtual bool  setNames(const OdString& colorName, const OdString& bookName = OdString::kEmpty);
  virtual OdString colorName() const;
  virtual OdString bookName() const;
  virtual OdString colorNameForDisplay() const;
};

#include "TD_PackPop.h"

#endif //_ODDB_BASEDATABASE_IMPL_H_
