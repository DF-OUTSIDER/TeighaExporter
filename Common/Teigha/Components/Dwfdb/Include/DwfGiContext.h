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

#ifndef __DWF_GICONTEXTFORDwfDatabase_H__
#define __DWF_GICONTEXTFORDwfDatabase_H__

#include "GiDefaultContext.h"
#include "OdPlatform.h"
#include "StaticRxObject.h"
#include "DwfDatabase.h"
#include "DwfObjectReactor.h"
#include "DwfdbExport.h"

#include "TD_PackPush.h"

struct OdGsClientViewInfo;

class DWFDBEXPORT OdGiContextForDwfDatabase : public OdGiDefaultContext
{
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
    kLastFlag                        = kErasePSLayoutHelperView
  };

  OdUInt32              m_fflags;

  class DWFDBEXPORT DatabaseHolder : public OdStaticRxObject<OdDwfDatabaseReactor>
  {
  public:
    DatabaseHolder();
    ~DatabaseHolder();

    void setDatabase(OdDwfDatabase * pDb);
    void goodbye(const OdDwfDatabase* pDb);

    OdDwfDatabase* m_pDb;
  } m_DbHolder;

protected:
  ODCOLORREF m_paletteBackground;
public:
  ODRX_DECLARE_MEMBERS(OdGiContextForDwfDatabase);
  OdGiContextForDwfDatabase()
    : m_fflags(0)
    , m_paletteBackground(ODRGB(0,0,0)) 
  {
  }
  ~OdGiContextForDwfDatabase() {}
  
  virtual OdDbBaseDatabase* database() const;
  virtual bool isPlotGeneration() const;
  virtual void setPlotGeneration(bool plotGeneration);
  virtual void setPaletteBackground(ODCOLORREF paletteBackground);
  virtual OdGiDrawablePtr openDrawable(OdDbStub* drawableId);
  virtual ODCOLORREF paletteBackground() const;

  void setDatabase( OdDwfDatabase* pDwfDatabase );

  /** Description:
    Returns the *database* that is currently being vectorized.
  */
  OdDwfDatabase* getDatabase() const;

  /** Description:
    Returns the OdGsClientViewInfo for the specified viewport.
    Arguments:
    vpId (I) Object ID of the viewport.
    viewInfo (O) Receives the client view info.
  */
  virtual void fillGsClientViewInfo(const OdDwfObjectId& vpId, OdGsClientViewInfo& viewInfo);

  virtual OdIntPtr drawableFilterFunctionId(OdDbStub* viewportId) const;
  virtual OdUInt32 drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags);

  /** Description:
    Returns true if and only the GsModel flag of this Context Object is set..
  */
  bool useGsModel() const;

  /** Description:
    Controls the GsModel flag of this Context Object.
    Arguments:
    enable (I) Controls the Gs Model.
  */
  void enableGsModel(bool enable);

  OdUInt32 lineWeightConfiguration(LineWeightStyle styleEntry) const;

  virtual bool ttfPolyDraw() const;
  void setTtfPolyDrawMode(bool bPolyDraw);
};

typedef OdSmartPtr<OdGiContextForDwfDatabase> OdGiContextForDwfDatabasePtr;

#include "TD_PackPop.h"

#endif // __DWF_GICONTEXTFORDwfDatabase_H__
