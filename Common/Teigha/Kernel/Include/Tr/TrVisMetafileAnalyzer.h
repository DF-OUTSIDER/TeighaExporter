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
// GLES2 metafile parser and comparator

#ifndef ODTRVISMETAFILEANALYZER
#define ODTRVISMETAFILEANALYZER

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "TrVisMetafileStream.h"

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafileParser
{
  public:
    enum VarType
    {
      kTypeInvalid = -1,
      kTypeChunk   = 0,
      kTypeUInt8   = 1,
      kTypeInt8    = 2,
      kTypeUInt16  = 3,
      kTypeInt16   = 4,
      kTypeUInt32  = 5,
      kTypeInt32   = 6,
      kTypeUInt64  = 7,
      kTypeInt64   = 8,
      kTypeFloat   = 9,
      kTypeDouble  = 10,
      kTypeFVec2   = 11,
      kTypeDVec2   = 12,
      kTypeFVec3   = 13,
      kTypeDVec3   = 14,
      kTypeFVec4   = 15,
      kTypeDVec4   = 16,
      kTypeFMat3   = 17,
      kTypeDMat3   = 18,
      kTypeFMat4   = 19,
      kTypeDMat4   = 20,

      kNTypes
    };
    enum BasisVarType
    {
      kBasisUInt8  = 0,
      kBasisUInt16 = 1,
      kBasisUInt32 = 2,
      kBasisUInt64 = 3,
      kBasisFloat  = 4,
      kBasisDouble = 5,

      kNBasisTypes
    };
    struct VarState
    {
      OdTrVisMetaRecType m_chunk;
      VarType m_varType;   // Type of current variable
      OdUInt32 m_nVar;     // Number of variable under chunk
      const void * m_pVar; // Pointer onto variable

      VarState()
        : m_chunk(OdTrVisMetaRecType_NTypes)
        , m_varType(kTypeInvalid)
        , m_nVar(0)
        , m_pVar(NULL)
      { }

      OdTrVisMetaRecType asChunk() const { return (OdTrVisMetaRecType)*(const OdUInt8*)m_pVar; }
      bool isEOF() const { return m_chunk == OdTrVisMetaRecType_NTypes; }

      OdUInt8 asUInt8() const                ;
      OdInt8 asInt8() const                  ;
      OdUInt16 asUInt16() const              ;
      OdInt16 asInt16() const                ;
      OdUInt32 asUInt32() const              ;
      OdInt32 asInt32() const                ;
      OdUInt64 asUInt64() const              ;
      OdInt64 asInt64() const                ;
      float asFloat() const                  ;
      double asDouble() const                ;
      const float *asFVec() const            ;
      const double *asDVec() const           ;
      const OdGeVector2d &asDVec2() const    ;
      const OdGePoint2d &asDPoint2() const   ;
      const OdGeVector3d &asDVec3() const    ;
      const OdGePoint3d &asDPoint3() const   ;
      const OdGeMatrix3d &asDMatrix3d() const;
    };
    static struct TypeSpec
    {
      OdUInt32 m_typeSize;
      OdUInt32 m_typeNumber;
      OdUInt32 m_basisType;
    } s_typeSpec[kNTypes];
    static struct BasisFuncs
    {
      void (*m_pLRegFunc)(OdUInt32 &lReg, const VarState &varState);
      bool (*m_pCompFunc)(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    } s_basisFunc[kNBasisTypes];
    enum NextStepOp
    {
      kOpSkip    = 0, // Skip until next chunk
      kOpProcess = 1  // Process normally
    };
    enum Scheme
    {
      kSmBasic = 0,  // Basic stream scheme
      kSmStandalone, // Standalone stream scheme
      kNumSchemes    // Number of stream schemes
    };
  protected:
    static struct RecScheme
    {
      const char *m_pSpec[kNumSchemes];
    } s_schemes[OdTrVisMetaRecType_NTypes];
    struct InitialState
    {
      const OdTrVisFlatMetafileContainer *m_pDataCont;
      Scheme m_streamScheme;
      OdUInt32 m_nStreamSize;
      const OdUInt8 *m_pStreamStart;
      const OdUInt8 *m_pStreamEnd;

      InitialState()
        : m_pDataCont(NULL)
        , m_streamScheme(kSmBasic)
        , m_nStreamSize(0)
        , m_pStreamStart(NULL)
        , m_pStreamEnd(NULL)
      { }
    } m_initState;
    struct StreamMarker
    {
      const OdUInt8 *m_pStreamCur;
      const char *m_pLogicalCur;

      StreamMarker()
        : m_pStreamCur(NULL)
        , m_pLogicalCur(NULL)
      { }
      void logicalCopyIn(const StreamMarker &sm2) { m_pLogicalCur = sm2.m_pLogicalCur - 2; }
      void logicalCopyOut(StreamMarker &sm2) { sm2.m_pLogicalCur = m_pLogicalCur; }
    };
    enum LogicalFlags
    {
      kLogicInit       = (1 << 0),
      kLogicExclusive  = (1 << 1),
      kLogicLoopSkip   = (1 << 2),
      kLogicSkip       = (1 << 3),
      kLogicSkipByMask = (1 << 4)
    };
    struct CurrentState
    {
      StreamMarker m_caret;
      OdUInt32 m_logicalReg;
      OdUInt16 m_logicalFlags;
      StreamMarker m_loopMarker;
      VarState m_curVarState;

      CurrentState()
        : m_logicalReg(0)
        , m_logicalFlags(0)
      { }
    } m_curState;
    OdUInt64 m_skipMask;
  public:
    OdTrVisMetafileParser(const OdBaseMetafileContainer *pContainer = NULL, Scheme streamScheme = kSmBasic)
      : m_skipMask(0)
    {
      initialize(pContainer, streamScheme);
    }

    const OdTrVisFlatMetafileContainer *metafileContainer() const { return m_initState.m_pDataCont; }
    Scheme streamScheme() const { return m_initState.m_streamScheme; }
    OdUInt32 streamSize() const { return m_initState.m_nStreamSize; }

    const OdUInt8 *&userEntryCurProc() { return m_curState.m_caret.m_pStreamCur; }

    void initialize(const OdBaseMetafileContainer *pContainer, Scheme streamScheme = kSmBasic);
    void reset();

    void rewind();

    const VarState &nextStep(NextStepOp op = kOpProcess);
  protected:
    static void lRegFuncUInt8(OdUInt32 &lReg, const VarState &varState);
    static void lRegFuncUInt16(OdUInt32 &lReg, const VarState &varState);
    static void lRegFuncUInt32(OdUInt32 &lReg, const VarState &varState);
    static void lRegFuncStub(OdUInt32 &lReg, const VarState &varState);
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafileComparator
{
  protected:
    OdTrVisMetafileParser m_par1, m_par2;
    static bool (*s_pCompArrayFunc[OdTrVisArrayWrapper::Type_Marker + 1])(const void *pArray1, const void *pArray2, OdUInt32 nSize);
  public:
    OdTrVisMetafileComparator(const OdBaseMetafileContainer *pContainer1 = NULL, const OdBaseMetafileContainer *pContainer2 = NULL, OdTrVisMetafileParser::Scheme streamScheme = OdTrVisMetafileParser::kSmBasic)
      : m_par1(pContainer1, streamScheme)
      , m_par2(pContainer2, streamScheme)
    {
    }

    bool compare();
    bool compare(const OdBaseMetafileContainer *pContainer1, const OdBaseMetafileContainer *pContainer2, OdTrVisMetafileParser::Scheme streamScheme = OdTrVisMetafileParser::kSmBasic);
  protected:
    friend class OdTrVisMetafileParser;
    static bool compFuncUInt8(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    static bool compFuncUInt16(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    static bool compFuncUInt32(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    static bool compFuncUInt64(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    static bool compFuncFloat(const void *pStream1, const void *pStream2, OdUInt32 nVars);
    static bool compFuncDouble(const void *pStream1, const void *pStream2, OdUInt32 nVars);

    static bool compFuncArrayVertex(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayNormal(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayColor(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayTexCoord(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayDepth(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayIndex(const void *pArray1, const void *pArray2, OdUInt32 nSize);
    static bool compFuncArrayMarker(const void *pArray1, const void *pArray2, OdUInt32 nSize);
};

inline OdUInt8             OdTrVisMetafileParser::VarState::asUInt8()      const { return *(const OdUInt8*)m_pVar; }
inline OdInt8              OdTrVisMetafileParser::VarState::asInt8()       const { return *(const OdInt8*) m_pVar; }

#ifdef TD_STRICT_ALIGNMENT

template <class T> T align(const void* ptr) { T res; memcpy(&res, ptr, sizeof(T)); return res; }

inline OdUInt16            OdTrVisMetafileParser::VarState::asUInt16()     const { return align<OdUInt16>(m_pVar); }
inline OdInt16             OdTrVisMetafileParser::VarState::asInt16()      const { return align<OdInt16 >(m_pVar); }
inline OdUInt32            OdTrVisMetafileParser::VarState::asUInt32()     const { return align<OdUInt32>(m_pVar); }
inline OdInt32             OdTrVisMetafileParser::VarState::asInt32()      const { return align<OdInt32 >(m_pVar); }
inline OdUInt64            OdTrVisMetafileParser::VarState::asUInt64()     const { return align<OdUInt64>(m_pVar); }
inline OdInt64             OdTrVisMetafileParser::VarState::asInt64()      const { return align<OdInt64 >(m_pVar); }
inline float               OdTrVisMetafileParser::VarState::asFloat()      const { return align<float   >(m_pVar); }
inline double              OdTrVisMetafileParser::VarState::asDouble()     const { return align<double  >(m_pVar); }
inline const float*        OdTrVisMetafileParser::VarState::asFVec()       const { return align<const float* >(m_pVar); }
inline const double*       OdTrVisMetafileParser::VarState::asDVec()       const { return align<const double*>(m_pVar); }
inline const OdGeVector2d& OdTrVisMetafileParser::VarState::asDVec2()      const { ODA_ASSERT(OdIntPtr(m_pVar)%8); return *(const OdGeVector2d*)m_pVar; }
inline const OdGePoint2d&  OdTrVisMetafileParser::VarState::asDPoint2()    const { ODA_ASSERT(OdIntPtr(m_pVar)%8); return *(const OdGePoint2d*)m_pVar; }
inline const OdGeVector3d& OdTrVisMetafileParser::VarState::asDVec3()      const { ODA_ASSERT(OdIntPtr(m_pVar)%8); return *(const OdGeVector3d*)m_pVar; }
inline const OdGePoint3d&  OdTrVisMetafileParser::VarState::asDPoint3()    const { ODA_ASSERT(OdIntPtr(m_pVar)%8); return *(const OdGePoint3d*)m_pVar; }
inline const OdGeMatrix3d& OdTrVisMetafileParser::VarState::asDMatrix3d()  const { ODA_ASSERT(OdIntPtr(m_pVar)%8); return *(const OdGeMatrix3d*)m_pVar; }

#else

inline OdUInt16            OdTrVisMetafileParser::VarState::asUInt16()     const { return *(const OdUInt16*)m_pVar; }
inline OdInt16             OdTrVisMetafileParser::VarState::asInt16()      const { return *(const OdInt16*)m_pVar; }
inline OdUInt32            OdTrVisMetafileParser::VarState::asUInt32()     const { return *(const OdUInt32*)m_pVar; }
inline OdInt32             OdTrVisMetafileParser::VarState::asInt32()      const { return *(const OdInt32*)m_pVar; }
inline OdUInt64            OdTrVisMetafileParser::VarState::asUInt64()     const { return *(const OdUInt64*)m_pVar; }
inline OdInt64             OdTrVisMetafileParser::VarState::asInt64()      const { return *(const OdInt64*)m_pVar; }
inline float               OdTrVisMetafileParser::VarState::asFloat()      const { return *(const float*)m_pVar; }
inline double              OdTrVisMetafileParser::VarState::asDouble()     const { return *(const double*)m_pVar; }
inline const float*        OdTrVisMetafileParser::VarState::asFVec()       const { return  (const float*)m_pVar; }
inline const double*       OdTrVisMetafileParser::VarState::asDVec()       const { return  (const double*)m_pVar; }
inline const OdGeVector2d& OdTrVisMetafileParser::VarState::asDVec2()      const { return *(const OdGeVector2d*)m_pVar; }
inline const OdGePoint2d&  OdTrVisMetafileParser::VarState::asDPoint2()    const { return *(const OdGePoint2d*)m_pVar; }
inline const OdGeVector3d& OdTrVisMetafileParser::VarState::asDVec3()      const { return *(const OdGeVector3d*)m_pVar; }
inline const OdGePoint3d&  OdTrVisMetafileParser::VarState::asDPoint3()    const { return *(const OdGePoint3d*)m_pVar; }
inline const OdGeMatrix3d& OdTrVisMetafileParser::VarState::asDMatrix3d()  const { return *(const OdGeMatrix3d*)m_pVar; }

#endif

#include "TD_PackPop.h"

#endif // ODTRVISMETAFILEANALYZER
