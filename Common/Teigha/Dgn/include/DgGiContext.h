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
#ifndef __DG_GICONTEXTFORDGDATABASE_H__
#define __DG_GICONTEXTFORDGDATABASE_H__

#include "GiDefaultContext.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiLinetyper.h"
#include "DgDatabase.h"
#include "DgDatabaseReactor.h"
#include "StaticRxObject.h"
#include "Ps/PlotStyles.h"
#include "DgView.h"
#include "DgLevelTableRecord.h"
#include "DgCmColor.h"
#include "Ps/DgPlotStyles.h"
#include "Ps/DgPlotStyleService.h"

#include "TD_PackPush.h"

struct OdGsClientViewInfo;

class OdGiContextForDgDatabase;

/** \details
This template class is a specialization of the OdSmartPtr class for OdGiContextForDgDatabase object pointers.
*/
typedef OdSmartPtr<OdGiContextForDgDatabase> OdGiContextForDgDatabasePtr;

/** \details
    This class defines the operations and properties that are used in the
    Teigha™ for .dgn files vectorization of an OdDgDatabase.

    Corresponding C++ library: TG_Db

    <group OdGi_Classes> 
*/
class TG_EXPORT OdGiContextForDgDatabase : public OdGiDefaultContext
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContextForDgDatabase);
  OdGiContextForDgDatabase();
  ~OdGiContextForDgDatabase();

  //to create contexts fully equipped
  static OdGiContextForDgDatabasePtr createObject( OdDgDatabasePtr dgnDatabase, OdDgViewPtr displayedView );

  // OdGiContext overridden functions
  OdDbBaseDatabase* database() const;
  OdGiDrawablePtr openDrawable(OdDbStub* drawableId);
  OdUInt32 numberOfIsolines() const;
  double commonLinetypeScale() const;
  OdDb::LineWeight defaultLineWeight() const;
  OdUInt32 lineWeightConfiguration(LineWeightStyle styleEntry) const; // #9277
  void     setLineWeightConfiguration( LineWeightStyle styleEntry, OdUInt32 uValue ); // #9277
  bool fillTtf() const;
  virtual PStyleType plotStyleType() const;
  virtual void plotStyle(int penNumber, OdPsPlotStyleData& plotStyleData) const;
  virtual void plotStyle(OdDbStub* psNameId, OdPsPlotStyleData& plotStyleData) const;
  virtual OdDbStub* getStubByMatName( const OdString&  ) const;

  // OdGiDefaultContext overridden functions
  void getDefaultTextStyle(OdGiTextStyle& textStyle);
  ODCOLORREF paletteBackground() const;
  void setPaletteBackground(ODCOLORREF paletteBackground);
  bool supportVerticalTTFText() const;

  // Control the GsModel flag of this Context Object
  bool useGsModel() const;
  void enableGsModel(bool enable);

  // Override from OdGiContext (the first one) and the method to set (the second one)
  bool isPlotGeneration() const;
  void setPlotGeneration(bool plotGeneration);

  SolidHatchAsPolygonMode hatchAsPolygon() const;
  void setHatchAsPolygon(SolidHatchAsPolygonMode mode);

  /** \details
    Loads the specified plot style table for this Context object.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
  */
  virtual bool loadPlotStyle( const OdString& strPlotStyleFile );
  virtual OdDgPsPlotStylePtr getPlotStyle();

  // The access to the DGN database
  OdDgDatabase* getDatabase() const;
  void setDatabase( OdDgDatabasePtr dgnDatabase );

  // OdDgView element should be used to precise some rendering options
  OdDgViewPtr getView( OdDg::OpenMode openMode = OdDg::kForRead ) const;
  void setView( OdDgViewPtr dgnView );

  /*
  Some tools to apply the common rules of DGN about overrides for symbologies (color, weight, style). Initially, it could operate with:
  1. kColorByLevel, etc - if set, a property should be taken from level (default properties)
  2. 'overrideByLevel' flag of views (OdDgView) - if set and an appropriate flag of the level is set (for example, 'override colors'), then this property is taken from level (overrided properties)

  Notes:
  1. All methods return false if a property can not be evaluated
  2. The level could be zero, that leads to 'can not be evaluated' if the level is required
  3. If this context does not have 'view' property initialized, its features are not used (of course)
  4. Sometimes Level Override should be ignored. For instance, If Text has background it should use a native background color
      value not override. Don't apply override in this case. 'bAllowForLevelOverrides' flag should handle just such kind of cases.
  */
  virtual bool getTrueColor( OdUInt32 colorIndex, OdDgCmEntityColor& trueColor, OdDgLevelTableRecordPtr level
                             , bool bAllowForLevelOverrides = true ) const;
  virtual bool getWeight( OdUInt32 weight, OdUInt32& resultWeight, OdDgLevelTableRecordPtr level, 
                          bool bCheckViewFlag = true, bool bAllowForLevelOverrides = true ) const;
  virtual bool getStyleEntryId( OdUInt32 styleEntryId, OdUInt32& resultStyleEntryId, OdDgLevelTableRecordPtr level, bool bAllowForLevelOverrides = true ) const;

  // Line type scale multiplier for default 8 internal line types.
  virtual double getLinetypeScaleMultiplier() const;
  virtual void   setLinetypeScaleMultiplier( double dMultiplier );

  virtual bool   getUseLineWeightMapFlag();

  //returns a context of this type if can (zero otherwise)
  static OdGiContextForDgDatabase* getContext( OdGiDrawableTraits* pDrwTraits );

private:
  class TG_EXPORT DatabaseHolder : public OdStaticRxObject<OdDgDatabaseReactor>
  {
  public:
    DatabaseHolder();
    ~DatabaseHolder();

    void setDatabase(OdDgDatabase * pDb);
    void goodbye(const OdDgDatabase* pDb);

    OdDgDatabase* m_pDb;
  } m_DbHolder;

  ODCOLORREF            m_paletteBackground;
  bool                  m_plotGeneration, m_useGsModel;
  OdDgElementId         m_dgnViewId;
  // #9277
  OdUInt32              m_uPointLineWeight;
  OdUInt32              m_uLineCapStyle;
  OdUInt32              m_uLineJoinStyle;
  double                m_dLineStyleScaleMultiplier;
};

class OdGiContextForDgDatabaseToDisplayStyleSupport;
typedef OdSmartPtr<OdGiContextForDgDatabaseToDisplayStyleSupport> OdGiContextForDgDatabaseToDisplayStyleSupportPtr;

/** \details
    This class defines the context that support dgn display styles.

    Corresponding C++ library: TG_Db

    <group OdGi_Classes> 
*/

class TG_EXPORT OdDgDisplayStyleContextData
{
  public:
    OdDgDisplayStyleContextData();
    virtual ~OdDgDisplayStyleContextData();

    virtual bool getTrueColor( OdDgDatabase* pDb, OdDgCmEntityColor& trueColor ) const;
    virtual void setColorIndex( OdUInt32 uColorIndex );

    virtual bool getLineStyle( OdUInt32& uLineStyle ) const;
    virtual void setLineStyle( OdUInt32 uLineStyle );

    virtual bool getLineWeight( OdUInt32& uLineWeight ) const;
    virtual void setLineWeight( OdUInt32  uLineWeight );

    virtual bool getTransparency( OdCmTransparency& dTransparency ) const;
    virtual void setTransparency( const OdCmTransparency& dTransparency );

    virtual bool getMaterial( OdDgElementId& idMaterial ) const;
    virtual void setMaterial( const OdDgElementId&  idMaterial );

    virtual void clearColorFlag();
    virtual void clearLineStyleFlag();
    virtual void clearLineWeightFlag();
    virtual void clearTransparencyFlag();
    virtual void clearMaterialFlag();
    virtual void clearAllFlags();

    virtual void initFromDisplayStyle( OdDgDatabase* pDb, const OdDgElementId& idDisplayStyle );

  private:
    OdUInt32          m_uDisplayStyleColor;
    OdUInt32          m_uDisplayStyleLineType;
    OdUInt32          m_uDisplayStyleLineWeight;
    OdCmTransparency  m_dDisplayStyleTransparency;
    OdDgElementId     m_idDisplayStyleMaterial;

    bool              m_bUseDisplayStyleColor;
    bool              m_bUseDisplayStyleLineType;
    bool              m_bUseDisplayStyleLineWeight;
    bool              m_bUseDisplayStyleTransparency;
    bool              m_bUseDisplayStyleMaterial;
};

class TG_EXPORT OdGiContextForDgDatabaseToDisplayStyleSupport : public OdGiContextForDgDatabase
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContextForDgDatabaseToDisplayStyleSupport);
  OdGiContextForDgDatabaseToDisplayStyleSupport();
  ~OdGiContextForDgDatabaseToDisplayStyleSupport();

  virtual void setDisplayStyle( const OdDgElementId& idDisplayStyle );

  virtual bool getDisplayStyleTransparency( OdCmTransparency& trans ) const;
  virtual bool getDisplayStyleMaterialId( OdDgElementId& idMaterial ) const;
  virtual bool getTrueColor( OdUInt32 colorIndex, OdDgCmEntityColor& trueColor, OdDgLevelTableRecordPtr level
                             , bool bAllowForLevelOverrides = true ) const;
  virtual bool getWeight( OdUInt32 weight, OdUInt32& resultWeight, OdDgLevelTableRecordPtr level, 
                          bool bCheckViewFlag = true, bool bAllowForLevelOverrides = true ) const;
  virtual bool getStyleEntryId( OdUInt32 styleEntryId, OdUInt32& resultStyleEntryId, OdDgLevelTableRecordPtr level, 
                          bool bAllowForLevelOverrides = true ) const;

protected:
    OdDgDisplayStyleContextData m_displayStyleData;
};

class OdGiContextForDgDatabaseToPlotStyleSupport;
typedef OdSmartPtr<OdGiContextForDgDatabaseToPlotStyleSupport> OdGiContextForDgDatabaseToPlotStyleSupportPtr;


/** \details
    This class defines the context that support dgn plot styles.

    Corresponding C++ library: TG_Db

    <group OdGi_Classes> 
*/

class TG_EXPORT OdGiContextForDgDatabaseToPlotStyleSupport : public OdGiContextForDgDatabaseToDisplayStyleSupport
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContextForDgDatabaseToPlotStyleSupport);
  OdGiContextForDgDatabaseToPlotStyleSupport();
  ~OdGiContextForDgDatabaseToPlotStyleSupport();

  //to create contexts fully equipped
  static OdGiContextForDgDatabaseToPlotStyleSupportPtr createObject( OdDgDatabasePtr dgnDatabase, OdDgViewPtr displayedView );
  static OdGiContextForDgDatabaseToPlotStyleSupport* getContext( OdGiDrawableTraits* pDrwTraits );

  virtual bool getTrueColor( OdUInt32 colorIndex, OdDgCmEntityColor& trueColor, OdDgLevelTableRecordPtr level
                     , bool bAllowForLevelOverrides = true 
                    ) const;

  virtual bool getWeight( OdUInt32 weight, OdUInt32& resultWeight, OdDgLevelTableRecordPtr level, 
                          bool bCheckViewFlag = true, bool bAllowForLevelOverrides = true ) const;

  virtual bool getStyleEntryId( OdUInt32 styleEntryId, OdUInt32& resultStyleEntryId, OdDgLevelTableRecordPtr level, bool bAllowForLevelOverrides = true ) const;

  virtual bool loadPlotStyle( const OdString& strPlotStyleFile );

  virtual OdDgPsPlotStylePtr getPlotStyle();

  virtual PStyleType plotStyleType() const;

  void plotStyle(int penNumber, OdPsPlotStyleData& plotStyleData) const;
  
  void plotStyle(OdDbStub* psNameId, OdPsPlotStyleData& plotStyleData) const;

  void setFrozenColor( const OdDgPlotStyleSectionOutputColor& clr );
  OdDgPlotStyleSectionOutputColor getFrozenColor() const;
  void setFrozenColorEnabledFlag( bool bSet );
  bool getFrozenColorEnabledFlag() const;

  void setFrozenLineWeight( const OdDgPlotStyleSectionOutputLineWeight& lWeight );
  OdDgPlotStyleSectionOutputLineWeight getFrozenLineWeight() const;
  void setFrozenLineWeightEnabledFlag( bool bSet );
  bool getFrozenLineWeightEnabledFlag() const;

  void setFrozenLineStyle( const OdDgPlotStyleSectionOutputLineStyle& lStyle );
  void setFrozenLineStyle( OdUInt32 uLineStyleEntryId );
  OdDgPlotStyleSectionOutputLineStyle getFrozenLineStyle() const;
  void setFrozenLineStyleEnabledFlag( bool bSet );
  bool getFrozenLineStyleEnabledFlag() const;

  void                setFrozenPSFillStyle( OdDgPs::DgFillStyle uFillStyle );
  OdDgPs::DgFillStyle getFrozenPSFillStyle() const;
  void                setFrozenPSFillStyleEnabledFlag( bool bSet );
  bool                getFrozenPSFillStyleEnabledFlag() const;

  void                setFrozenPSScreening( OdUInt8 uScreening );
  OdUInt8             getFrozenPSScreening() const;
  void                setFrozenPSScreeningEnabledFlag( bool bSet );
  bool                getFrozenPSScreeningEnabledFlag() const;

  virtual bool        getUseLineWeightMapFlag();

private:

  OdDgPsPlotStylePtr                   m_pPlotStyle;
  OdPsPlotStyleData                    m_oldPSDataByName;
  OdPsPlotStyleData                    m_oldPSDataByColorIndex;

  OdDgPlotStyleSectionOutputColor      m_freezedColor;
  OdDgPlotStyleSectionOutputLineWeight m_freezedLineWeight;
  OdDgPlotStyleSectionOutputLineStyle  m_freezedLineStyle;
  OdDgPsPlotStyleWeightData            m_lineWeightIndexReplacement;
  OdUInt32                             m_uFreezedLineStyleId;
  OdDgPs::DgFillStyle                  m_uFreezedPsFillStyle;
  OdUInt8                              m_uFreezedPsScreening;
  bool                                 m_bUseFreezedColor;
  bool                                 m_bUseFreezedLineWeight;
  bool                                 m_bUseFreezedLineStyle;
  bool                                 m_bUseFreezedPsFillStyle;
  bool                                 m_bUseFreezedScreening;

  std::map<OdString, OdUInt32>         m_mapLineStyleNameToIndex;
};

class OdGiContextForDgDatabaseToExport;
typedef OdSmartPtr<OdGiContextForDgDatabaseToExport> OdGiContextForDgDatabaseToExportPtr;

/** \details
    This class defines the context to be export in DGN.

    Corresponding C++ library: TG_Db

    <group OdGi_Classes> 
*/
class TG_EXPORT OdGiContextForDgDatabaseToExport : public OdGiContextForDgDatabaseToPlotStyleSupport
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContextForDgDatabaseToExport);
  OdGiContextForDgDatabaseToExport() {}
  ~OdGiContextForDgDatabaseToExport() {}

  //to create contexts fully equipped
  static OdGiContextForDgDatabaseToExportPtr createObject( OdDgDatabasePtr dgnDatabase, OdDgViewPtr displayedView );
  static OdGiContextForDgDatabaseToExport* getContext( OdGiDrawableTraits* pDrwTraits );

  virtual bool getWeight( OdUInt32 weight, OdUInt32& resultWeight, OdDgLevelTableRecordPtr level, bool bCheckViewFlag = true, bool bAllowForLevelOverrides = true ) const;
  void   setLineWeigthScale( double dScale );
private:
  double m_dLineWeightScale;
};


#include "TD_PackPop.h"

#endif // __DG_GICONTEXTFORDGDATABASE_H__
