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
// GLES2 metafile inverse converter

#ifndef ODTRVISMETAFILEPLAYBACK
#define ODTRVISMETAFILEPLAYBACK

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "TrVisMetafileStream.h"
#include "Gi/GiConveyorGeometry.h"
#include "MetafileTransformStack.h"

class OdTrVisTtfFontsCacheClient;

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafilePlaybackCallback
{
  public:
    virtual void metafilePlayMetafile(OdTrVisDisplayId /*metafileId*/) { }
    virtual void metafilePushTransform(const OdGeMatrix3d & /*mTransform*/) { }
    virtual void metafilePopTransform() { }
    virtual const OdTrVisTtfFontsCacheClient *metafileTtfFontsCache() { return NULL; }

    virtual void metafileProcessUserEntry(OdUInt32 /*dataLength*/, const OdUInt8 * /*pData*/) { }

    virtual bool metafileSelectMarker(OdTrVisGsMarker /*gsMarker*/, const OdBaseMetafileContainer * /*pContainer*/) { return true; }

    virtual bool metafilePointProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafileLineProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafileTriangleProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafileGeomVisibility(OdTrVisGeomType /*geomType*/) { return true; }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafilePlayback
{
  public:
    enum ConfFlags
    {
      kIgnoreSelGeom    = (1 << 0), // Ignore selection-only geometry (can't be set with kIgnoreNonSelGeom)
      kIgnoreNonSelGeom = (1 << 1), // Ignore geometry which can't be selected (can't be set with kIgnoreSelGeom)
      kClientStream     = (1 << 2)  // Client-side metafile stream
    };
  protected:
    // Player->Owner communication
    OdTrVisMetafilePlaybackCallback *m_pCallback;
    // Interprocess context for nested play() calls
    struct InprocRegisters
    {
      // Current selection markers
      const OdTrVisSelMark *pMarks;
      // Number of selection markers
      OdUInt32 nMarks;
      // Processing flags
      bool bSuppressGeom;
      bool bSelGeometry;

      // Default initializer
      InprocRegisters()
        : pMarks(NULL), nMarks(0), bSuppressGeom(false), bSelGeometry(false)
      { }
    } m_inprocContext;
    // Configuration flags
    OdUInt32 m_configFlags;
  public:
    OdTrVisMetafilePlayback(OdTrVisMetafilePlaybackCallback *pCallback = NULL, OdUInt32 configFlags = kIgnoreNonSelGeom)
      : m_pCallback(pCallback)
      , m_configFlags(configFlags)
    {
    }
    virtual ~OdTrVisMetafilePlayback()
    {
    }

    inline void setCallback(OdTrVisMetafilePlaybackCallback *pCallback)
    {
      m_pCallback = pCallback;
    }
    inline OdTrVisMetafilePlaybackCallback *callback() const
    {
      return m_pCallback;
    }

    inline void setConfigFlags(OdUInt32 configFlags)
    {
      m_configFlags = configFlags;
    }
    inline OdUInt32 configFlags() const
    {
      return m_configFlags;
    }

    bool play(const OdBaseMetafileContainer *pContainer, bool bCheckMarks = true);
  protected:
    void annihilateSelectionMarkers(InprocRegisters &inprocCtx, OdInt32 first, OdInt32 count);
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafileConveyorPlayback : public OdTrVisMetafilePlayback, protected OdTrVisMetafilePlaybackCallback
{
  protected:
    // Conveyor communication interfaces
    OdGiConveyorGeometry *m_pGeom;
    OdGiConveyorContext *m_pCtx;
    // Actual readdress callback
    OdTrVisMetafilePlaybackCallback *m_pActualCallback;
    // Callback redirection
    virtual void metafilePlayMetafile(OdTrVisDisplayId metafileId);
    virtual void metafilePushTransform(const OdGeMatrix3d &mtx);
    virtual void metafilePopTransform();
    virtual const OdTrVisTtfFontsCacheClient *metafileTtfFontsCache();
    virtual void metafileProcessUserEntry(OdUInt32 dataLength, const OdUInt8 *pData);
    virtual bool metafileSelectMarker(OdTrVisGsMarker gsMarker, const OdBaseMetafileContainer *pContainer);
    // Internal callback implementations
    virtual bool metafilePointProc(const OdGePoint3d *pVertex);
    virtual bool metafileLineProc(const OdGePoint3d *pVertex);
    virtual bool metafileTriangleProc(const OdGePoint3d *pVertex);
    virtual bool metafileGeomVisibility(OdTrVisGeomType geomType);
  public:
    OdTrVisMetafileConveyorPlayback(OdTrVisMetafilePlaybackCallback *pCallback = NULL)
      : OdTrVisMetafilePlayback(NULL)
      , m_pActualCallback(pCallback)
    {
      OdTrVisMetafilePlayback::setCallback(this);
    }
    virtual ~OdTrVisMetafileConveyorPlayback()
    {
    }

    inline void setCallback(OdTrVisMetafilePlaybackCallback *pCallback)
    {
      m_pActualCallback = pCallback;
    }
    inline OdTrVisMetafilePlaybackCallback *callback() const
    {
      return m_pActualCallback;
    }

    void play(const OdBaseMetafileContainer *pContainer, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, bool bCheckMarks = true);
};

#include "TD_PackPop.h"

#endif // ODTRVISMETAFILEPLAYBACK
