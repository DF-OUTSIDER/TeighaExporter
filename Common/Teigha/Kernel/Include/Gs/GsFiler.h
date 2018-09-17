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

#ifndef __OD_GS_FILER_H__
#define __OD_GS_FILER_H__

#include "TD_PackPush.h"

#include "OdStreamBuf.h"
#include "Gs/GsExport.h"
#include "OdBinaryData.h"
#include "OdVector.h"
#include "IntArray.h"
#include "UInt8Array.h"
#include "UInt32Array.h"
#include "UInt16Array.h"
#include "GsMarkerArray.h"
#include "DbStubPtrArray.h"
#include "Ge/GePoint2d.h"
#include "UInt64Array.h"

class OdGePoint2d;
class OdGeVector2d;
class OdGePoint3d;
class OdGeVector3d;
class OdGeMatrix3d;
class OdGeExtents3d;

class OdDbStub;
typedef OdRxObject OdDbBaseDatabase;

class OdGsView;
class OdGsCache;

class OdGsDCRect;
class OdGsDCRectDouble;
typedef OdArray<float, OdMemoryAllocator<float> > OdFloatArray;
typedef OdArray<OdGePoint2d, OdMemoryAllocator<OdGePoint2d> > OdGePoint2dArray;

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFiler : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGsFiler);

    OdGsFiler() { }
    virtual ~OdGsFiler() { }
  public:
    enum Section
    {
      kEOFSection = -1,      // EOF
      // V1 sections
      kHeaderSection = 0,     // File header
      kDatabaseLinkSection,   // Database linkage section
      kGsModuleSection,       // Gs Module section
      kDeviceSection,         // Gs Device section
      kClientDeviceSection,   // Client Gs Device section
      kViewSection,           // Gs View section
      kClientViewSection,     // Client Gs View section
      kModelSection,          // Gs Model section
      kClientModelSection,    // Client Gs Model section
      kNodeSection,           // Gs Node section
      kClientNodeSection,     // Client Gs Node section
      kMetafileSection,       // Gs Metafile section
      kClientMetafileSection, // Client Gs Metafile section
      kClientMaterialSection, // Client Gs Material section
      kBlockRefImplSection,   // Block reference cache implementation section
      kLinkedDeviceSection,   // Linked Gs Device section
      kRenditionSection,      // Client visual rendition section
      // V2 sections
      kRuntimeChangesSection  // Runtime Gs modifications section
    };
    enum Version
    {
      kV1 = 100,
      kV2 = 200,
      kVLast = kV2
    };
    struct SubstitutionActuator
    {
      typedef void (*SetPtrFunc)(void *pPlace, const void *pValue);
      SubstitutionActuator() { }
      virtual ~SubstitutionActuator() { }
      virtual void applySubstitution(void *pPlace, const void *pValue, SetPtrFunc pSetFunc) { pSetFunc(pPlace, pValue); }
    };
    struct Substitutor
    {
      // Register known substitution
      virtual void registerSubstitution(const void *pValue, const void *pSubstitution, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = false) = 0;
      // Register substitution request
      virtual void requestSubstitution(void *pPlace, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true) = 0;
      // Register substitution request for smart pointer
      virtual void requestSubstitution(OdBaseObjectPtr *pPlace, const void *pValue, bool bRegister = true, bool bImmediate = true) = 0;
      // Register substitution with non-default behavior
      virtual void requestSubstitution(void *pPlace, OdGsFiler::SubstitutionActuator *pActuator, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true) = 0;
      // Clear substitutions
      virtual void clearSubstitutions(const void *pValue = NULL, OdUInt32 size = 0) = 0;
      // Run substitutions
      virtual void runSubstitutions(const void *pValue = NULL, OdUInt32 size = 0, bool bClear = true) = 0;
    };
  public:
    // Attach stream and initiate read/write process
    virtual bool setStream(OdStreamBuf *pStream, bool bWrite = false) = 0;
    virtual OdStreamBufPtr getStream() const = 0;

    // Setup working database
    virtual void setDatabase(const OdDbBaseDatabase *pDb) = 0;
    virtual OdDbBaseDatabase *getDatabase() const = 0;

    // File version control
    virtual void setVersion(OdUInt32 nVersion) = 0;
    virtual OdUInt32 version() const = 0;

    // Database hash control
    virtual void wrDbHash(const OdDbBaseDatabase *pDb) = 0;
    virtual bool checkDbHash(const OdDbBaseDatabase *pDb) = 0;

    // Setup sections for writing
    virtual void setWriteSections(OdUInt64 nSections) = 0;
    virtual void setWriteSection(Section section, bool bSet) = 0;
    virtual bool isWriteSection(Section section) const = 0;

    // Setup sections for reading
    virtual void setReadSections(OdUInt64 nSections) = 0;
    virtual void setReadSection(Section section, bool bSet) = 0;
    virtual bool isReadSection(Section section) const = 0;

    // Section writing
    virtual void wrSectionBegin(Section section) = 0;
    virtual void wrSectionEnd(Section section) = 0;
    virtual void wrEOFSection() = 0;

    // Section reading
    virtual Section rdSection() const = 0;
    virtual Section curSection() const = 0;
    virtual void skipSection() const = 0;
    virtual void rdBackSection() const = 0;
    virtual bool checkEOF() const = 0;

    // Substitutions processing
    virtual Substitutor *subst() const = 0;
    virtual void makeSubstitutions(bool bClear = true) const = 0;
    virtual OdRxObjectPtr getSubstitutor() const = 0;
    virtual void setSubstitutor(OdRxObject *pSubst) = 0;

    // Arbitrary data
    virtual void setArbitraryData(const OdChar *pName, OdRxObject *pObject) = 0;
    virtual OdRxObjectPtr getArbitraryData(const OdChar *pName) const = 0;
    virtual bool hasArbitraryData(const OdChar *pName) const = 0;
    virtual void clearArbitraryData() = 0;

    // Pointers registration
    virtual void registerPtr(const void *pPtr) = 0;
    virtual void unregisterPtr(const void *pPtr) = 0;
    virtual bool isPtrRegistered(const void *pPtr) const = 0;
    virtual void clearRegisteredPtrs() = 0;

    // Process handles
    virtual void wrHandle(OdDbStub *pHandle) = 0;
    virtual OdDbStub *rdHandle() const = 0;

    // Process classes
    virtual void wrClass(OdRxObject *pObj) = 0;
    virtual OdRxObjectPtr rdClass() const = 0;

    // Process raw data
    virtual void wrRawData(const void *pData, OdUInt32 nDataSize) = 0;
    virtual void rdRawData(void *pData, OdUInt32 nDataSize) const = 0;

    // Process data primitives
    virtual void wrBool(bool bVal);
    virtual bool rdBool() const;

    virtual void wrInt(int val);
    virtual int rdInt() const;

    virtual void wrUInt(unsigned int val);
    virtual unsigned int rdUInt() const;

    virtual void wrChar(char val);
    virtual char rdChar() const;

    virtual void wrUInt8(OdUInt8 val);
    virtual OdUInt8 rdUInt8() const;

    virtual void wrInt16(OdInt16 val);
    virtual OdInt16 rdInt16() const;

    virtual void wrUInt16(OdUInt16 val);
    virtual OdUInt16 rdUInt16() const;

    virtual void wrInt32(OdInt32 val);
    virtual OdInt32 rdInt32() const;

    virtual void wrUInt32(OdUInt32 val);
    virtual OdUInt32 rdUInt32() const;

    virtual void wrInt64(OdInt64 val);
    virtual OdInt64 rdInt64() const;

    virtual void wrUInt64(OdUInt64 val);
    virtual OdUInt64 rdUInt64() const;

    virtual void wrIntPtr(OdIntPtr val);
    virtual OdIntPtr rdIntPtr() const;

    void wrPtr(const void *pPtr) { wrIntPtr((OdIntPtr)pPtr); }
    void *rdPtr() const { return (void*)rdIntPtr(); }

    virtual void wrCOLORREF(ODCOLORREF val);
    virtual ODCOLORREF rdCOLORREF() const;

    virtual void wrFloat(float val);
    virtual float rdFloat() const;

    virtual void wrDouble(double val);
    virtual double rdDouble() const;

    virtual void wrPoint2d(const OdGePoint2d &pt);
    virtual void rdPoint2d(OdGePoint2d &pt) const;

    virtual void wrVector2d(const OdGeVector2d &vec);
    virtual void rdVector2d(OdGeVector2d &vec) const;

    virtual void wrPoint3d(const OdGePoint3d &pt);
    virtual void rdPoint3d(OdGePoint3d &pt) const;

    virtual void wrVector3d(const OdGeVector3d &vec);
    virtual void rdVector3d(OdGeVector3d &vec) const;

    virtual void wrMatrix3d(const OdGeMatrix3d &mat);
    virtual void rdMatrix3d(OdGeMatrix3d &mat) const;

    virtual void wrExtents3d(const OdGeExtents3d &ext);
    virtual void rdExtents3d(OdGeExtents3d &ext) const;

    virtual void wrAnsiString(const OdAnsiString &str);
    virtual void rdAnsiString(OdAnsiString &str) const;
    OdAnsiString rdAnsiStringRet() const { OdAnsiString str; rdAnsiString(str); return str; }

    virtual void wrString(const OdString &str);
    virtual void rdString(OdString &str) const;
    OdString rdStringRet() const { OdString str; rdString(str); return str; }

    virtual void wrUInt8Array (const OdUInt8Array& arr);
    virtual void wrUInt16Array(const OdUInt16Array& arr);
    virtual void wrUInt16Array(OdUInt32 count, const OdUInt16* arr);
    virtual void wrUInt32Array(const OdUInt32Array& arr);
    virtual void wrUInt32Array(const OdUInt32Vector& arr);
    virtual void wrUInt64Array(const OdUInt64Array& arr);
    virtual void wrUInt64Array(const OdUInt64Vector& vec);
    virtual void wrUInt64Array(OdUInt32 count, const OdUInt64* arr);
    virtual void wrIntArray   (const OdIntArray& arr);
    virtual void wrFloatArray (const OdFloatArray& arr);
    virtual void wrFloatArray (OdUInt32 count, const float* arr);
    virtual void wrPoint2dArray  (const OdGePoint2dArray& arr);
    virtual void wrPoint3dArray  (const OdGePoint3dArray& arr);
    virtual void wrDbStubPtrArray(const OdDbStubPtrArray& arr);
    virtual void wrGsDCRect      (const OdGsDCRect& rc);
    virtual void wrGsDCRectDouble(const OdGsDCRectDouble& rcd);

    virtual void rdUInt8Array (OdUInt8Array& arr);
    virtual void rdUInt16Array(OdUInt16Array& arr);
    virtual void rdUInt16Array(OdUInt32 count, OdUInt16* arr);
    virtual void rdUInt32Array(OdUInt32Array& arr);
    virtual void rdUInt32Array(OdUInt32Vector& arr);
    virtual void rdUInt64Array(OdUInt64Array& arr);
    virtual void rdUInt64Array(OdUInt64Vector& vec);
    virtual void rdUInt64Array(OdUInt32 count, OdUInt64* arr);
    virtual void rdIntArray   (OdIntArray& arr);
    virtual void rdFloatArray (OdFloatArray& arr);
    virtual void rdFloatArray (OdUInt32 count, float* arr);
    virtual void rdPoint2dArray  (OdGePoint2dArray& arr);
    virtual void rdPoint3dArray  (OdGePoint3dArray& arr);
    virtual void rdDbStubPtrArray(OdDbStubPtrArray& arr);
    virtual void rdGsDCRect      (OdGsDCRect& dcrc);
    virtual void rdGsDCRectDouble(OdGsDCRectDouble& dcrcd);

    // Create filer object
    static OdSmartPtr<OdGsFiler> createObject(OdStreamBuf *pStream, bool bForWrite, const OdDbBaseDatabase *pDb, OdUInt32 nVersion = kVLast);
};

typedef OdSmartPtr<OdGsFiler> OdGsFilerPtr;

#define OdGsFiler_wrArray(filer, arry, wrFunc) \
  { \
    filer.wrUInt32(arry.size()); \
    for (OdUInt32 nVal = 0; nVal < arry.size(); nVal++) \
      filer.wrFunc(arry.getPtr()[nVal]); \
  }
#define OdGsFilerPtr_wrArray(pFiler, arry, wrFunc) \
  OdGsFiler_wrArray((*pFiler), arry, wrFunc)

#define OdGsFiler_wrArrayRaw(filer, arry, typSz) \
  { \
    filer.wrUInt32(arry.size()); \
    if (!arry.isEmpty()) \
      filer.wrRawData(arry.getPtr(), arry.size() * typSz); \
  }
#define OdGsFilerPtr_wrArrayRaw(pFiler, arry, typSz) \
  OdGsFiler_wrArrayRaw((*pFiler), arry, typSz)

#define OdGsFiler_rdArray(filer, arry, rdFunc) \
  { \
    arry.clear(); \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.setPhysicalLength(nElems); \
    for (OdUInt32 nElem = 0; nElem < nElems; nElem++) \
      arry.push_back(filer.rdFunc()); \
  }
#define OdGsFilerPtr_rdArray(pFiler, arry, rdFunc) \
  OdGsFiler_rdArray((*pFiler), arry, rdFunc)

#define OdGsFiler_rdArrayArg(filer, arry, rdFunc) \
  { \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.resize(nElems); \
    for (OdUInt32 nElem = 0; nElem < nElems; nElem++) \
      filer.rdFunc(arry[nElem]); \
  }
#define OdGsFilerPtr_rdArrayArg(pFiler, arry, rdFunc) \
  OdGsFiler_rdArrayArg((*pFiler), arry, rdFunc)

#define OdGsFiler_rdArrayRaw(filer, arry, typSz) \
  { \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.resize(nElems); \
    if (nElems) \
      filer.rdRawData(arry.asArrayPtr(), nElems * typSz); \
  }
#define OdGsFilerPtr_rdArrayRaw(pFiler, arry, typSz) \
  OdGsFiler_rdArrayRaw((*pFiler), arry, typSz)

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFilerCreatorPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsFilerCreatorPE);
public:
  virtual OdGsFilerPtr createGsFiler(OdStreamBuf *pStream, bool bForWrite, const OdDbBaseDatabase *pDb) = 0;
};

typedef OdSmartPtr<OdGsFilerCreatorPE> OdGsFilerCreatorPEPtr;

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFilerDbHashPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsFilerDbHashPE);
public:
  virtual OdBinaryData computeDbHash(const OdDbBaseDatabase *pDb) = 0;
};

typedef OdSmartPtr<OdGsFilerDbHashPE> OdGsFilerDbHashPEPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_FILER_H__
