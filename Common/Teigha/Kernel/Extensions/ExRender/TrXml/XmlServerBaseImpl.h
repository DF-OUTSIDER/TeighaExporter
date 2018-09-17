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
// Base implementation of XmlServer interface for GLES2 Xml device

#ifndef ODGLES2XMLSERVERBASEIMPL
#define ODGLES2XMLSERVERBASEIMPL

#include "TD_PackPush.h"

#include "XmlServer.h"
#include "TrVisUniqueId.h"
#include "DbBaseDatabase.h"

#ifndef odabs
#define odabs(X) ((X) > 0 ? (X) : -(X))
#endif

/** \details
    <group ExRender_Classes> 
*/
class OdGLES2XmlServerBaseImpl : public OdGLES2XmlServer, protected OdTrVisUniqueID::DefUIDTransformer
{
protected:
  int m_nIdent;
  bool m_bClientDependentSectionOnly;
  OdDbBaseDatabasePE *m_pDbPE;

  OdAnsiString m_sTmpBuf; // for sprintf or format on one level upper indent
  char* m_pszTmpBuf;
  int m_lenTmpBuf;
# if defined(odSprintfA)
#   define ODFMTBEG(len) \
    { \
      { \
        m_pszTmpBuf = m_sTmpBuf.getBuffer(m_lenTmpBuf = (int)len + 1); \
        odSprintfA(m_pszTmpBuf, m_lenTmpBuf,
#   define ODFMTADD() \
        ); \
      } \
    } \
    { \
      { \
        odSprintfA(m_pszTmpBuf += odStrLenA(m_pszTmpBuf), m_lenTmpBuf,
#   define ODFMTFORBEGEX(len, expr) \
    { \
      m_pszTmpBuf = m_sTmpBuf.getBuffer(m_lenTmpBuf = len * nData + 1); \
      for (OdUInt32 index = 0; index < nData; index++, m_pszTmpBuf += odStrLenA(m_pszTmpBuf)) \
      { \
        expr; \
        odSprintfA(m_pszTmpBuf, m_lenTmpBuf, 
# else
#   define ODFMTBEG(len) \
    { \
      { \
        m_sTmpBuf.format(
#   define ODFMTADD() \
        ); \
      } \
    }
    { \
      { \
        m_sTmpBuf += OdAnsiString().format(
#   define ODFMTFORBEGEX(len, expr) \
    { \
      m_sTmpBuf.empty(); \
      for (OdUInt32 index = 0; index < nData; index++) \
      { \
        expr; \
        m_sTmpBuf += OdAnsiString().format( 
# endif
#   define ODFMTFORBEG(len) ODFMTFORBEGEX(len,)
#   define ODFMTEND() \
        ); \
      } \
    }
#   define ODFLTFMT(cstrpref, val, cstrsfx) \
      ((val >= 0.0f ? val : - val) >= 0.1f && (val >= 0.0f ? val : - val) < 1e+16) ? cstrpref "%f" cstrsfx : cstrpref "%g" cstrsfx

public:
  OdGLES2XmlServerBaseImpl(const OdDbBaseDatabase *pDb = NULL)
    : m_nIdent(0)
    , m_bClientDependentSectionOnly(false)
    , m_pDbPE(NULL)
    , m_pszTmpBuf(NULL)
    , m_lenTmpBuf(0)
  {
    setDatabase(pDb);
  }
  void setClientDependentSectionOnlyFlag(bool bSet)
  { 
    m_bClientDependentSectionOnly = bSet; 
  }

  void setDatabase(const OdDbBaseDatabase *pDb = NULL)
  {
    if (pDb)
      m_pDbPE = OdDbBaseDatabasePE::cast(pDb);
    else
      m_pDbPE = NULL;
  }

  virtual void append(OdAnsiString &to, OdDbStub* from) const
  {
    if (m_pDbPE)
      OdTrVisUniqueID::DefUIDTransformer::append(to, (OdUInt64)m_pDbPE->getHandle(from));
    else
      OdTrVisUniqueID::DefUIDTransformer::append(to, from);
  }

  // should be overrode for JSON
  virtual void ident(const char* pTag, const char* pText)  = 0;
  virtual void IncreaseNesting(const char* pLevelName)
  {
    ODFMTBEG(odStrLenA(pLevelName) + 2) 
      "<%s>", pLevelName 
    ODFMTEND()
    ident(NULL, m_sTmpBuf.c_str());
    m_nIdent += 2;
  }
  virtual void DecreaseNesting(const char* pLevelName)
  {
    m_nIdent -= 2;
    ODFMTBEG(odStrLenA(pLevelName) + 3)
      "</%s>", pLevelName 
    ODFMTEND()
    ident(NULL, m_sTmpBuf.c_str());
  }

  virtual void DropId(const char* pTagName, OdUInt64 data, 
                      OdTrVisUniqueID* pId = NULL,
                      OdInt32 enAdv = -1)
  {
    if (pId && data > OdUInt64(100)) // is not an address
    {
      m_sTmpBuf = pId->transform(*this);
      ident(pTagName, m_sTmpBuf.c_str());
      return;
    }
    ODA_ASSERT_ONCE(data <= OdUInt64(100));

    OdGLES2XmlServer::DropId(pTagName, data, pId, enAdv); // DropUInt64(pTagName, data);
  }

  virtual void DropBoolean(const char* pTagName, bool data)
  {
    DropUInt32(pTagName, (data) ? 1 : 0);
  }

  virtual void DropUInt8(const char* pTagName, OdUInt8 data)
  {
    DropUInt32(pTagName, data);
  }
  virtual void DropUInt32(const char* pTagName, OdUInt32 data)
  {
    ODFMTBEG(16)
      "%u", (unsigned)data
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }
  virtual void DropUInt64(const char* pTagName, OdUInt64 data)
  {
    ODFMTBEG(32)
      PRIu64, data
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }
  virtual void DropInt32(const char* pTagName, OdInt32 data)
  {
    ODFMTBEG(10)
      "%d", (int)data
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropChars(const char* pTagName, const char *pStr)
  {
    m_sTmpBuf = pStr;
    m_sTmpBuf.replace("<", "&lt;");
    m_sTmpBuf.replace(">", "&gt;");
    m_sTmpBuf.replace("\n", "&#x0A;");
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropInts(const char* pTagName, OdUInt32 nData, const OdUInt16* pData)
  {
    ODA_ASSERT_ONCE(nData && pData);
    ODFMTFORBEG(16)
      index ? ",%d" : "%d" , pData[index]
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropFloats(const char* pTagName, OdUInt32 nData, const float *pData)
  {
    ODA_ASSERT_ONCE(nData && !(nData % 3)); // test
    if (  odStrCmpA(pTagName, "ArrayData")
        || !nData || (nData % 3)
        || (odabs(pData[0]) < 10000 && odabs(pData[1]) < 10000 && odabs(pData[2]) < 10000))
    {
      ODA_ASSERT_ONCE(nData && pData);
      ODFMTFORBEG(32)
        //index ? ",%g" : "%g" , pData[index]
        ODFLTFMT("%s", pData[index], ), index ? "," : "", pData[index]
      ODFMTEND()
      ident(pTagName, m_sTmpBuf.c_str());
      return;
    }

    float val, offs[3];
   #if !defined(__BORLANDC__) && !defined(__hpux)
    offs[0] = ceilf(pData[0]) - 1.0f; offs[1] = ceilf(pData[1]) - 1.0f; offs[2] = ceilf(pData[2]) - 1.0f; // by first
   #else
    // float-mathematics is missing in Builder
    offs[0] = pData[0]; offs[1] = pData[1]; offs[2] = pData[2];
   #endif

    DropFloat3("ArrayOffset", offs[0], offs[1], offs[2]);

    ODA_ASSERT_ONCE(nData && pData);
    ODFMTFORBEGEX(16, val = pData[index] - offs[index % 3])
      //index ? ",%g" : "%g" , pData[index] - offs[index % 3]
      ODFLTFMT("%s", val, ), index ? "," : "", val
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropFloat(const char* pTagName, float data)
  {
    ODFMTBEG(32)
      //"%g", data
      ODFLTFMT( , data, ), data
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropFloat2(const char* pTagName, float data1, float data2)
  {
    ODFMTBEG(64)
      //"%g, %g", data1, data2
      ODFLTFMT(    , data1, ), data1
    ODFMTADD()
      ODFLTFMT(", ", data2, ), data2
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }
  virtual void DropFloat3(const char* pTagName, float data1, float data2, float data3)
  {
    ODFMTBEG(96)
      //"%g, %g, %g", data1, data2, data3
      ODFLTFMT(    , data1, ), data1
    ODFMTADD()
      ODFLTFMT(", ", data2, ), data2
    ODFMTADD()
      ODFLTFMT(", ", data3, ), data3
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }
  virtual void DropFloat4(const char* pTagName, float data1, float data2, float data3, float data4)
  {
    ODFMTBEG(128)
      //"%g, %g, %g, %g", data1, data2, data3, data4
      ODFLTFMT(    , data1, ), data1
    ODFMTADD()
      ODFLTFMT(", ", data2, ), data2
    ODFMTADD()
      ODFLTFMT(", ", data3, ), data3
    ODFMTADD()
      ODFLTFMT(", ", data4, ), data4
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropUInt8RGB(const char* pTagName, OdUInt8 data1, OdUInt8 data2, OdUInt8 data3, OdUInt8 data4)
  {
    ODFMTBEG(64)
      "%u, %u, %u, %u", (unsigned)data1, (unsigned)data2, (unsigned)data3, (unsigned)data4
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropMatrix(const char* pTagName, const OdGeMatrix3d &data)
  {
    ODFMTBEG(512)
      //"%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g",
      //data.entry[0][0], data.entry[0][1], data.entry[0][2], data.entry[0][3],
      //data.entry[1][0], data.entry[1][1], data.entry[1][2], data.entry[1][3],
      //data.entry[2][0], data.entry[2][1], data.entry[2][2], data.entry[2][3],
      //data.entry[3][0], data.entry[3][1], data.entry[3][2], data.entry[3][3]
      ODFLTFMT(    , data.entry[0][0], ), (double)(float) data.entry[0][0]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[0][1], ), (double)(float) data.entry[0][1]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[0][2], ), (double)(float) data.entry[0][2]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[0][3], ), (double)(float) data.entry[0][3]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[1][0], ), (double)(float) data.entry[1][0]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[1][1], ), (double)(float) data.entry[1][1]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[1][2], ), (double)(float) data.entry[1][2]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[1][3], ), (double)(float) data.entry[1][3]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[2][0], ), (double)(float) data.entry[2][0]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[2][1], ), (double)(float) data.entry[2][1]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[2][2], ), (double)(float) data.entry[2][2]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[2][3], ), (double)(float) data.entry[2][3]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[3][0], ), (double)(float) data.entry[3][0]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[3][1], ), (double)(float) data.entry[3][1]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[3][2], ), (double)(float) data.entry[3][2]
    ODFMTADD()
      ODFLTFMT(", ", data.entry[3][3], ), (double)(float) data.entry[3][3]
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropBinaryStream(const char* pTagName, const OdUInt8 * pData, OdUInt32 nData)
  {
    ODFMTFORBEG(4)
      "%02X", pData[index]
    ODFMTEND()
    ident(pTagName, m_sTmpBuf.c_str());
  }

  virtual void DropDbStub(const char *pTagName, OdDbStub *pDbStub)
  {
    // possible via paper object for example //ODA_ASSERT_ONCE(pDbStub);
    if (!pDbStub)
      return;

    m_sTmpBuf.empty();
    append(m_sTmpBuf, pDbStub);
    ident(pTagName, m_sTmpBuf);
  }

  virtual bool UpdateClientDependentSectionOnly() const
  {
    return m_bClientDependentSectionOnly;
  }
};

#include "TD_PackPop.h"

#endif // ODGLES2XMLSERVERBASEIMPL
