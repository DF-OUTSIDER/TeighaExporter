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
// GLES2 information string wrapper

#ifndef ODTRVISINFOSTRING
#define ODTRVISINFOSTRING

#include "TD_PackPush.h"

#include "OdAlloc.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisInfoString
{
  struct InfoString
  {
    char *m_pInfoString;
    int m_nCounter;
  } *m_pBuffer;
  private:
    int calcLen(const char *pData) const
    {
      if (!pData)
        return 0;
      int len = 0;
      while (pData[len] != 0)
        len++;
      return len;
    }
    int find(const char *pStr, char findChr, int startIdx = 0) const
    {
      int idx = startIdx;
      while (pStr[idx] != 0)
      {
        if (pStr[idx] == findChr)
          return idx;
        idx++;
      }
      return -1;
    }
    int find(const char *pStr, const char *pSubstr, int startIdx = 0) const
    {
      int idx = startIdx, substrIdx;
      while (pStr[idx] != 0)
      {
        substrIdx = 0;
        while (pSubstr[substrIdx] != 0)
        {
          if (pSubstr[substrIdx] != pStr[idx + substrIdx])
            break;
          substrIdx++;
        }
        if (pSubstr[substrIdx] == 0)
          return idx;
        idx++;
      }
      return -1;
    }
    void erase(InfoString *pBuffer)
    {
      if (pBuffer->m_pInfoString)
        ::odrxFree(pBuffer->m_pInfoString);
      ::odrxFree(pBuffer);
    }
    bool dereference(const InfoString *pExclusive = NULL)
    {
      if (m_pBuffer != pExclusive)
      {
        if (m_pBuffer == NULL)
          return true;
        if (--m_pBuffer->m_nCounter == 0)
          erase(m_pBuffer);
        m_pBuffer = NULL;
        return true;
      }
      return false;
    }
    void doSetNewBuf(const char *pData)
    {
      dereference();
      InfoString *newBuf = (InfoString*)::odrxAlloc(sizeof(InfoString));
      int strLen = calcLen(pData);
      if (strLen)
      {
        newBuf->m_pInfoString = (char*)::odrxAlloc(strLen + 1);
        for (int nChar = 0; nChar < strLen; nChar++)
          newBuf->m_pInfoString[nChar] = pData[nChar];
        newBuf->m_pInfoString[strLen] = 0;
      }
      else
        newBuf->m_pInfoString = NULL;
      newBuf->m_nCounter = 1;
      m_pBuffer = newBuf;
    }
    void doSetCopyBuf(InfoString *pBuf)
    {
      if (dereference(pBuf) && pBuf)
      {
        m_pBuffer = pBuf;
        pBuf->m_nCounter++;
      }
    }
  public:
    OdTrVisInfoString()
      : m_pBuffer(NULL)
    { }
    OdTrVisInfoString(const char *pStr)
      : m_pBuffer(NULL)
    { doSetNewBuf(pStr); }
    OdTrVisInfoString(const OdTrVisInfoString &is)
    { doSetCopyBuf(is.m_pBuffer); }

    ~OdTrVisInfoString()
    { dereference(); }

    void set(const char *pStr) { doSetNewBuf(pStr); }
    void set(const OdTrVisInfoString &is) { doSetCopyBuf(is.m_pBuffer); }

    const char *get() const { return (m_pBuffer) ? m_pBuffer->m_pInfoString : NULL; }
    operator const char *() const { return get(); }

    OdTrVisInfoString& operator =(const OdTrVisInfoString &is) { set(is); return *this; }
    OdTrVisInfoString& operator =(const char *pStr) { set(pStr); return *this; }

    bool checkExtension(const char *pExtension) const;

    static bool checkExtensionExternal(const char *pExtensions, const char *pExtension);
};

inline bool OdTrVisInfoString::checkExtension(const char *pExtension) const
{
  const char *pExtensions = get();
  if (!pExtension || !pExtensions)
    return false;
  int extLen = calcLen(pExtension);
  if (!extLen || (find(pExtension, ' ') != -1))
    return false;
  int extsLen = calcLen(pExtensions);
  if (!extsLen)
    return false;
  int start = 0, cur, end;
  for (;;)
  {
    cur = find(pExtensions, pExtension, start);
    if (cur == -1)
      break;
    end = extLen + cur;
    if (cur == 0 || pExtensions[cur - 1] == ' ')
    {
      if (end == extsLen || pExtensions[end] == ' ')
        return true;
    }
    start = end;
  }
  return false;
}

inline bool OdTrVisInfoString::checkExtensionExternal(const char *pExtensions, const char *pExtension)
{
  InfoString tmpBuf = { NULL, 2 }; (*(const void**)&tmpBuf.m_pInfoString) = pExtensions;
  OdTrVisInfoString tmpString; tmpString.m_pBuffer = &tmpBuf;
  return tmpString.checkExtension(pExtension);
}

#include "TD_PackPop.h"

#endif // ODTRVISINFOSTRING
