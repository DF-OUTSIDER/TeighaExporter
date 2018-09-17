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

#ifndef _ODGI_CONTEXTFORDBDATABASE_INCLUDED_
#define _ODGI_CONTEXTFORDBDATABASE_INCLUDED_

#include "GiDefaultContext.h"
#include "DbExport.h"
#include "OdPlatform.h"
#include "StaticRxObject.h"
#include "DbDatabase.h"
#include "DbDatabaseReactor.h"
#include "Ps/PlotStyles.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiLinetyper.h"

#include "TD_PackPush.h"

struct OdGsClientViewInfo;
class OdGsView;
class OdGiContextualColorsImpl;

/** \details
    This class defines the operations and properties that are used in the
    Teigha vectorization of an OdDbDatabase.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdGiContextForDbDatabase : public OdGiDefaultContext
{
private:
  enum
  {
    kPlotGeneration                  = 1,
    kUseGsModel                      = 2,
    kOdGiZeroTextNormals             = 4,
    kDisableLayoutHelperLinkReactors = 8,
    kTTFPolyDraw                     = 16,
    kTrackDbDestroy                  = 32,
    kManageContextualColors          = 64,
    kKeepPSLayoutHelperView          = 128,
    kErasePSLayoutHelperView         = 256,
    kConstantModelSpaceLineweights   = 512,
    kForceDisplaySilhouettes         = 1024,
    kLastFlag                        = kForceDisplaySilhouettes
  };

  OdUInt32              m_fflags;

  class TOOLKIT_EXPORT DatabaseHolder : public OdStaticRxObject<OdDbDatabaseReactor>
  {
  public:
    DatabaseHolder();
    ~DatabaseHolder();

    OdDbDatabase* m_pDb;

    void setDatabase( bool bWasTrackDbDestroy, OdDbDatabase* pDb, bool bTrackDbDestroy );

    void goodbye(const OdDbDatabase* pDb);
  }
  m_DbHolder;

protected:
  ODCOLORREF            m_paletteBackground;
  OdPsPlotStyleTablePtr m_pPlotStyleTable;

  OdGiContextForDbDatabase();

public:

  ODRX_DECLARE_MEMBERS(OdGiContextForDbDatabase);

  ~OdGiContextForDbDatabase();

  // OdGiContext overridden functions

  virtual OdDbBaseDatabase* database() const;
  virtual OdGiDrawablePtr openDrawable(OdDbStub* drawableId);
  virtual OdUInt32 numberOfIsolines() const;
  virtual double commonLinetypeScale() const;
  virtual OdDb::LineWeight defaultLineWeight() const;
  virtual bool quickTextMode() const;
  virtual OdUInt32 textQuality() const;
  virtual ImageQuality imageQuality() const;
  virtual OdUInt32 fadingIntensityPercentage(FadingType fadingType) const;
  virtual bool ttfPolyDraw() const;
  virtual OdUInt32 displaySilhouettes() const;

  /** \details
    Sets the database to be vectorized.

    \param pDb [in]  Pointer to the database.
    \param bTrackDbDestroy [in]  if to the database is not NULL then
    this parameter specifies if implementation should track database lifetime
    (by adding reactor to the database and processing OdDbDatabaseReactor::goodbye() event)
    and to clear pointer to the database if it will be destroyed. If you sure that lifetime of this object
    is shorter then lifetime of the database you may set this parameter to false
    to turn off unnecessary database lifetime tracking.
  */
  void setDatabase( OdDbDatabase* pDb, bool bTrackDbDestroy = true );

  /** \details
    Returns the database that is currently being vectorized.
  */
  OdDbDatabase* getDatabase() const;

  /** \details
    Returns the OdGsClientViewInfo for the specified viewport.
    \param vpId [in]  Object ID of the viewport.
    \param viewInfo [out]  Receives the client view info.
  */
  virtual void fillGsClientViewInfo(const OdDbObjectId& vpId, OdGsClientViewInfo& viewInfo);

  /** \details
  Returns the OdDbStub for the specified handle id.
  \param objectId [in]  Object ID of the object.
  Returns the pointer to OdDbStub.
  */
  virtual OdDbStub* getStubByID( OdUInt64 objectId ) const;

  virtual OdDbStub* getStubByMatName(const OdString& strMatName) const;

  virtual void getDefaultTextStyle(OdGiTextStyle& textStyle);

  unsigned int circleZoomPercent(OdDbStub* viewportId) const;

  virtual OdIntPtr drawableFilterFunctionId(OdDbStub* viewportId) const;
  virtual OdUInt32 drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags);

  /** \details
    Returns true if and only the GsModel flag of this Context Object is set..
  */
  bool useGsModel() const;

  /** \details
    Controls the GsModel flag of this Context Object.
    \param enable [in]  Controls the Gs Model.
  */
  void enableGsModel(bool enable);

  /** \details
    Returns true if and only the PlotGeneration flag of this Context Object is set.
  */
  bool isPlotGeneration() const;

  /** \details
    Controls the PlotGeneration flag of this Context Object.
    \param plotGeneration [in]  Controls plot generation.
  */
  void setPlotGeneration(bool plotGeneration);

  /** \details
    Returns true if and only the kOdGiZeroTextNormals flag of this Context Object is set.
  */
  virtual bool isZeroTextNormals() const;

  /** \details
    Controls the ZeroTextNormals flag of this Context Object.
    \param bZeroTextNormals [in]  Controls text normals generation.
  */
  void setZeroTextNormals(bool bZeroTextNormals);

  /** \details
    Returns true if and only the kDisableLayoutHelperLinkReactors flag of this Context Object is set.
  */
  bool isLayoutHelperLinkReactorsDisabled() const;

  /** \details
    Controls the kDisableLayoutHelperLinkReactors flag of this Context Object.
    \param bDisableLinkReactors [in]  Controls layout link reactors flag.
  */
  void disableLayoutHelperLinkReactors(bool bDisableLinkReactors);

  /** \details
    Returns true if and only the kManageContextualColors flag of this Context Object is set.
  */
  bool isContextualColorsManagementEnabled() const;

  /** \details
    Controls the kManageContextualColors flag of this Context Object.
    \param bEnable [in]  Controls contextual colors management flag.
  */
  void enableContextualColorsManagement(bool bEnable);

  /** \details
    Update view contextual colors after view changes.
    \param pView [in]  View pointer for update contextual colors.
  */
  virtual void updateContextualColors(OdGsView *pView);

  /** \details
    Initialize contextual colors container by contextual colors values.
    \param pCtxColors [in]  Pointer to contextual colors container.
  */
  virtual void fillContextualColors(OdGiContextualColorsImpl *pCtxColors);

  /** \details
    Enables TrueType text contours info output.
    \param bPolyDraw [in]  Controls kTTFPolyDraw flag of this Context Object.
  */
  void setTtfPolyDrawMode(bool bPolyDraw);

  /** \details
    Returns true if and only if the kKeepPSLayoutHelperView flag of this Context Object is set.
  */
  bool isKeepPSLayoutHelperViewEnabled() const;

  /** \details
    Controls the kKeepPSLayoutHelperView flag of this Context Object.
    \param bEnable [in]  Controls PaperSpace layout helper view management flag.
  */
  void enableKeepPSLayoutHelperView(bool bEnable);

  /** \details
    Returns true if and only if the kErasePSLayoutHelperView flag of this Context Object is set.
  */
  bool erasePSLayoutHelperView() const;

  /** \details
    Controls the kErasePSLayoutHelperView flag of this Context Object.
    \param bEnable [in]  Controls PaperSpace layout helper view management flag.
  */
  void setErasePSLayoutHelperView(bool bOn);

  /** \details
    Returns current behavior of ModelSpace lineweights.
  */
  bool isConstantModelSpaceLineweightsEnabled() const;

  /** \details
    Controls behavior of ModelSpace lineweights.
    \param bEnable [in]  Enable contant ModelSpace lineweights.
  */
  void enableConstantModelSpaceLineweights(bool bEnable);

  /** \details
    Returns current state of display silhouettes forcing mode.
  */
  bool isForceDisplaySilhouettesEnabled() const;

  /** \details
    Sets current state of display silhouettes forcing mode.
    \param bOn [in]  Enable or disable display silhouettes forcing.
  */
  void setForceDisplaySilhouettes(bool bOn);

  /** \details
    Returns true if and only if filling of TrueType fonts is enabled.
  */
  bool fillTtf() const;

  /** \details
    Returns true if and only if shell/mesh geometry primitives should be filled during this vectorization.
  */
  bool fillMode() const;

  /** \details
    Returns the palette background color for this Context object.
  */
  ODCOLORREF paletteBackground() const { return m_paletteBackground; }
  /** \details
    Sets the palette background color for this Context object.
    \param paletteBackground [in]  Palette background.
  */
  void setPaletteBackground(ODCOLORREF paletteBackground) { m_paletteBackground = paletteBackground; }

  /** \details
    Loads the specified plot style table for this Context object.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
  */
  void loadPlotStyleTable(OdStreamBuf* pStreamBuf);
  
  /** \details
    Returns the plot style type for this Context object.
  */
  virtual PStyleType plotStyleType() const;

  /** \details
    Returns the plot style data for the specified pen number.
    \param penNumber [in]  Pen number.
    \param plotStyleData [out]  Receives the plot style data.
  */
  virtual void plotStyle(int penNumber, OdPsPlotStyleData& plotStyleData) const;

  /** \details
    Returns the plot style data for the specified object Id.
    
    \param psNameId [in]  Object id of the plot style name.
    \param plotStyleData [out]  Receives the plot style data.
  */
  virtual void plotStyle(OdDbStub* psNameId, OdPsPlotStyleData& plotStyleData) const;

  /** \details
    Returns an interface for access section geometry functionality.
  */
  virtual OdGiSectionGeometryManagerPtr getSectionGeometryManager();

  /** \details
    Controls rendering anti-aliasing.
  */
  virtual OdUInt32 antiAliasingMode() const;

  /** \details
    Controls Xref properties overriding.
  */
  virtual bool xrefPropertiesOverride() const;

  /** \details
  Returns SolidHatchAsPolygonMode set for this Context Object.
  */
  SolidHatchAsPolygonMode hatchAsPolygon() const;

  /** \details
  Controls SolidHatchAsPolygonMode of this Context Object.
  \param bOn [in]  Controls hatch as polygon generation.
  */
  void setHatchAsPolygon(SolidHatchAsPolygonMode mode);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiContextForDbDatabase object pointers.
*/
typedef OdSmartPtr<OdGiContextForDbDatabase> OdGiContextForDbDatabasePtr;


#include "TD_PackPop.h"

#endif //_ODGI_CONTEXTFORDBDATABASE_INCLUDED_
