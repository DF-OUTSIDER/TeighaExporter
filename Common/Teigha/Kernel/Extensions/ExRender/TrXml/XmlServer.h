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
// XmlServer interface for GLES2 Xml device

#ifndef ODGLES2XMLSERVER
#define ODGLES2XMLSERVER

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"
#include "RxObject.h"
#include "TrVisIdMap.h"

class OdDbStub;
class OdTrVisUniqueID;
class OdGsFiler;
class OdGLES2XmlRendition;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2XmlServer : public OdRxObject
{
  public:
    enum OutputState
    {
      kUndefState = 0,
      kIntermediateState,     // No actual data, GsUpdate initiated
      kClientOnlyState,       // Transient metafiles
      kClientDependentState,  // Viewports and surface settings
      kShareableState,        // Metafiles and materials
      kShareableClientState   // Textures and shaders
    };
    enum IdType
    {
      kUnknownId = -1,
      kViewportId = 0,
      kMetafileId,
      kTextureId,
      kMaterialId,
      kVertexShaderId,
      kFragmentShaderId,
      kProgramId,
      kHlBranchId,
      kVisualStyleId,
      kLayerId,
      kOverlayId,

      kDisplayId = kMetafileId
    };
    enum IdSubType
    {
      kNoIdSubtype       = 0,
      kViewportIdExtents = 1
    };
    struct IdMapping
    {
      IdType m_idType;
      OdUInt64 m_resId;
      OdTrVisUniqueID* m_pId;
    };
  public:
    ODRX_HEAP_OPERATORS();

    void addRef() { }
    void release() { }

    virtual void IncreaseNesting(const char* pLevelName) = 0;
    virtual void DecreaseNesting(const char* pLevelName) = 0;
    virtual void DropBoolean(const char* pTagName, bool data) = 0;
    virtual void DropBooleanOpt(const char* pTagName, bool data) { if (data) DropBoolean(pTagName, data); }

    virtual bool DropMetafileAdded(OdGLES2XmlRendition& rndn, OdTrVisMetafileId& metafileId, const OdTrVisMetafileDef &pDef) { return false; }

    virtual void DropUInt8(const char* pTagName, OdUInt8 data) = 0;
    virtual void DropUInt16(const char* pTagName, OdUInt16 data) { DropUInt32(pTagName, data); }
    virtual void DropUInt32(const char* pTagName, OdUInt32 data) = 0;
    virtual void DropUInt64(const char* pTagName, OdUInt64 data) = 0;

    virtual void DropInt16(const char* pTagName, OdInt16 data) { DropInt32(pTagName, data); }
    virtual void DropInt32(const char* pTagName, OdInt32 data) = 0;

    virtual void DropChars(const char* pTagName, const char* pStr) = 0;
    virtual void DropString(const char *pTagName, const OdString &pStr);

    virtual void DropInts(const char* pTagName, OdUInt32 nData, const OdUInt16* pData) = 0;
    virtual void DropFloats(const char* pTagName, OdUInt32 nData, const float* pData) = 0;

    virtual void DropFloat(const char* pTagName, float data) = 0;
    virtual void DropFloat2(const char* pTagName, float data1, float data2) = 0;
    virtual void DropFloat3(const char* pTagName, float data1, float data2, float data3) = 0;
    virtual void DropFloat4(const char* pTagName, float data1, float data2, float data3, float data4) = 0;

    virtual void DropDouble(const char* pTagName, double data) { DropFloat(pTagName, (float)data); }
    virtual void DropDouble2(const char* pTagName, double data1, double data2) { DropFloat2(pTagName, (float)data1, (float)data2); }
    virtual void DropDouble3(const char* pTagName, double data1, double data2, double data3)
    { DropFloat3(pTagName, (float)data1, (float)data2, (float)data3); }
    virtual void DropDouble4(const char* pTagName, double data1, double data2, double data3, double data4)
    { DropFloat4(pTagName, (float)data1, (float)data2, (float)data3, (float)data4); }

    virtual void DropUInt8RGB(const char* pTagName, OdUInt8 data1, OdUInt8 data2, OdUInt8 data3, OdUInt8 data4) = 0;

    virtual void DropMatrix(const char* pTagName, const OdGeMatrix3d& data) = 0;

    virtual void DropBinaryStream(const char* pTagName, const OdUInt8* pData, OdUInt32 nData) = 0;

    virtual void DropId(const char* pTagName, OdUInt64 data,
                        OdTrVisUniqueID* = NULL, OdInt32 = -1) { DropUInt64(pTagName, data); }

    virtual void DropDbStub(const char* pTagName, OdDbStub* pDbStub) = 0;

    virtual void RegisterId(OdUInt64, OdTrVisUniqueID* = NULL, OdInt32 = -1) { }
    virtual void UnregisterId(OdUInt64, OdTrVisUniqueID* = NULL, OdInt32 = -1) { }

    virtual void OnStateChanged(OutputState /*newState*/) { }
    virtual bool UpdateClientDependentSectionOnly() const { return false; }
    virtual bool UpdateShareableSectionOnly() const { return false; }

    virtual void saveState(OdGsFiler * pFiler) const { }
    virtual void loadState(OdGsFiler * pFiler, OdTrVisIdMap *pIdMap) { }

    virtual void setOutput(class OdStreamBuf * buf) { }
};

inline
void OdGLES2XmlServer::DropString(const char *pTagName, const OdString &pStr)
{
  OdAnsiCharArray utf8Str(pStr.getLength() * 4 + 1, 1);
  OdCharMapper::unicodeToUtf8(pStr.c_str(), pStr.getLength(), utf8Str);
  DropChars(pTagName, utf8Str.getPtr());
}

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2XmlServerStub : public OdGLES2XmlServer
{
  public:
    virtual void IncreaseNesting(const char* /*pLevelName*/) { }
    virtual void DecreaseNesting(const char* /*pLevelName*/) { }

    virtual void DropBoolean(const char* /*pTagName*/, bool /*data*/) { }

    virtual void DropUInt8(const char* /*pTagName*/, OdUInt8 /*data*/) { }
    virtual void DropUInt32(const char* /*pTagName*/, OdUInt32 /*data*/) { }
    virtual void DropUInt64(const char* /*pTagName*/, OdUInt64 /*data*/) { }

    virtual void DropInt32(const char* /*pTagName*/, OdInt32 /*data*/) { }

    virtual void DropChars(const char* /*pTagName*/, const char* /*pStr*/) { }
    virtual void DropInts(const char* /*pTagName*/, OdUInt32 /*nData*/, const OdUInt16* /*pData*/) { }
    virtual void DropFloats(const char* /*pTagName*/, OdUInt32 /*nData*/, const float* /*pData*/) { }

    virtual void DropFloat(const char* /*pTagName*/, float /*data*/) { }
    virtual void DropFloat2(const char* /*pTagName*/, float /*data1*/, float /*data2*/) { }
    virtual void DropFloat3(const char* /*pTagName*/, float /*data1*/, float /*data2*/, float /*data3*/) { }
    virtual void DropFloat4(const char* /*pTagName*/, float /*data1*/, float /*data2*/, float /*data3*/, float /*data4*/) { }

    virtual void DropUInt8RGB(const char* /*pTagName*/, OdUInt8 /*data1*/, OdUInt8 /*data2*/, OdUInt8 /*data3*/, OdUInt8 /*data4*/) { }

    virtual void DropMatrix(const char* /*pTagName*/, const OdGeMatrix3d& /*data*/) { }

    virtual void DropBinaryStream(const char* /*pTagName*/, const OdUInt8* /*pData*/, OdUInt32 /*nData*/) { }

    virtual void DropDbStub(const char* /*pTagName*/, OdDbStub* /*pDbStub*/) { }
};

#include "TD_PackPop.h"

#endif // ODGLES2XMLSERVER
