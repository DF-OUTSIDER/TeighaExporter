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
// GLES2 device metafile definition

#ifndef ODTRVISMETAFILEDEF
#define ODTRVISMETAFILEDEF

#include "TD_PackPush.h"

#include "UInt8Array.h"
#include "Ge/GeExtents3d.h"
#include "SharedPtr.h"
#include "TPtr.h"

#include "TrVisDefs.h"
#include "TrVisMetafileStream.h"

// Forward declaration
class OdTrRndSgExpand;

/** \details
  This template class is a specialization of the OdSharedPtr class for OdTrVisFlatMetafileContainer object pointers. 
*/
typedef OdSharedPtr<OdTrVisFlatMetafileContainer> OdTrVisFlatMetafileContainerPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMetafileContainer : public OdTrVisFlatMetafileContainer
{
  enum MetafileDefFlags
  {
    kMfVisible     = (1 << 0), // Visibility flag (false for invisible metafiles).
    kMfHighlighted = (1 << 1), // Highlighting flag (always false for nested metafiles).
    kMfTemporary   = (1 << 2), // True for transient drawables, false for persistent drawables.
    kMfNested      = (1 << 3), // Marks nested drawables (true for drawables inside blocks, false for top-level always).
    kMfBlockRef    = (1 << 4), // Marks block reference container metafile.
    kMfUtilitary   = (1 << 5), // Marks utilitary metafiles (TTF characters cache for example).
    kMfSectionable = (1 << 6), // Marks sectionable metafiles.
    kMfComposite   = (1 << 7), // Marks composite metafiles (containing data for multipass rendering).
    kMfXRef        = (1 << 8), // Marks external reference metafiles.
    kMfRefEditFade = (1 << 9)  // Fade metafile flag (true if metafile faded onto vectorizer side).
  };

  OdUInt16           m_mfFlags;   // Metafile bit flags
  OdUInt16           m_nRefs;     // Reference counter
  OdTrRndSgExpand   *m_pSgData;   // Runtime scene graph rendering data
  OdDbStub          *m_pOwnerId;  // Metafile owner database Id
  OdTrVisHlBranchId  m_pHlBranch; // Highlight branch associated with metafile
  OdTrVisLayerId     m_pLayer;    // Layer Id associated with metafile

  OdTrVisMetafileContainer() : m_nRefs(0), m_pSgData(NULL) {}

  // Reset metafile flags
  void resetMetafileFlags(OdUInt16 mfFlags = 0) { m_mfFlags = mfFlags; }

  // Visiblity flag
  bool isMetafileVisible() const { return GETBIT(m_mfFlags, kMfVisible); }
  void setMetafileVisible(bool bSet) { SETBIT(m_mfFlags, kMfVisible, bSet); }

  // Highlighting flag
  bool isMetafileHighlighted() const { return GETBIT(m_mfFlags, kMfHighlighted); }
  void setMetafileHighlighted(bool bSet) { SETBIT(m_mfFlags, kMfHighlighted, bSet); }

  // Temporary drawable flag
  bool isTemporaryMetafile() const { return GETBIT(m_mfFlags, kMfTemporary); }
  void setTemporaryMetafile(bool bSet) { SETBIT(m_mfFlags, kMfTemporary, bSet); }

  // Nested metafile flag
  bool isNestedMetafile() const { return GETBIT(m_mfFlags, kMfNested); }
  void setNestedMetafile(bool bSet) { SETBIT(m_mfFlags, kMfNested, bSet); }

  // Block reference flag
  bool isBlockRefMetafile() const { return GETBIT(m_mfFlags, kMfBlockRef); }
  void setBlockRefMetafile(bool bSet) { SETBIT(m_mfFlags, kMfBlockRef, bSet); }

  // Utilitary metafile flag
  bool isUtilitaryMetafile() const { return GETBIT(m_mfFlags, kMfUtilitary); }
  void setUtilitaryMetafile(bool bSet) { SETBIT(m_mfFlags, kMfUtilitary, bSet); }

  // Sectionable metafile flag
  bool isSectionableMetafile() const { return GETBIT(m_mfFlags, kMfSectionable); }
  void setSectionableMetafile(bool bSet) { SETBIT(m_mfFlags, kMfSectionable, bSet); }

  // Composite metafile flag
  bool isCompositeMetafile() const { return GETBIT(m_mfFlags, kMfComposite); }
  void setCompositeMetafile(bool bSet) { SETBIT(m_mfFlags, kMfComposite, bSet); }

  // XRef metafile flag
  bool isXRefMetafile() const { return GETBIT(m_mfFlags, kMfXRef); }
  void setXRefMetafile(bool bSet) { SETBIT(m_mfFlags, kMfXRef, bSet); }

  // RefEditFade metafile flag
  bool isMetafileFaded() const { return GETBIT(m_mfFlags, kMfRefEditFade); }
  void setMetafileFaded(bool bSet) { SETBIT(m_mfFlags, kMfRefEditFade, bSet); }

  // Scene-Graph connection
  bool hasSceneGraphExpand() const { return !!m_pSgData; }
  void setSceneGraphExpand(OdTrRndSgExpand *pExpand) { m_pSgData = pExpand; }
  void resetSceneGraphExpand() { m_pSgData = NULL; }
  OdTrRndSgExpand *sceneGraphExpand() const { return m_pSgData; }

  // Calculate size of underlying stream
  OdUInt64 calcSize() const
  { OdUInt64 nRes = size(); // Include size of stream
    const OdUInt32 nArrays = arrayElementsSize(); // Include size of arrays
    for (OdUInt32 nArray = 0; nArray < nArrays; nArray++)
      nRes += arrayElement((OdTrVisArrayId)nArray).m_uSize + sizeof(OdTrVisArrayWrapper);
    return nRes + sizeof(OdTrVisMetafileContainer); // Include size of data structure itself
  }

  void setCloned(OdTrVisMetafileContainer *pSrc)
  {
    resetMetafileFlags(pSrc->m_mfFlags);
    m_pOwnerId  = pSrc->m_pOwnerId;
    m_pHlBranch = pSrc->m_pHlBranch;
    m_pLayer    = pSrc->m_pLayer;
  }
  void setDefault()
  {
    resetMetafileFlags();
    m_pOwnerId  = NULL;
    m_pHlBranch = kTrVisNullId;
    m_pLayer    = kTrVisNullId;
  }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisMfContainerRelease
{
  public:
    static void addRef(OdTrVisMetafileContainer* pObj)  { if (pObj) { pObj->m_nRefs++; } }
    static void release(OdTrVisMetafileContainer* pObj) { if (pObj) { pObj->m_nRefs--; if (!pObj->m_nRefs) delete pObj; } }
};

/** \details
  This template class is a specialization of the TPtr class for OdTrVisMetafileContainer object pointers. 
*/
typedef TPtr<OdTrVisMetafileContainer, OdTrVisMfContainerRelease> OdTrVisMetafileContainerPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMetafileDef
{
  // Metafile stream and related flags
  OdTrVisMetafileContainerPtr m_pMetafile;
  // Metafile extents
  OdGeExtents3d m_extents;
  // Generator viewport Id
  OdTrVisViewportId m_generatorId;

  void setDefault()
  {
    m_pMetafile = NULL;
    m_extents = OdGeExtents3d();
    m_generatorId = OdTrVisViewportId(0);
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISMETAFILEDEF
