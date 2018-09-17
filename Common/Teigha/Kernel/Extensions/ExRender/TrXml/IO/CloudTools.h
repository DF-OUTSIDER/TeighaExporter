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
//
// CloudTools.h
//

#ifndef OD_CLD_TOOLS_H_
#define OD_CLD_TOOLS_H_

#include "StringArray.h"
#include "OdCharMapper.h"
#include "OdBinaryData.h"
#include "OdDToStr.h"
#include "RxVariant.h"
#include "RxVariantValue.h"
#include "CmColorBase.h"
#include "RxSysRegistry.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#ifndef _WIN32_WCE
#include <sys/stat.h>
#endif

#if !defined(ODA_WINDOWS) || defined(_WINRT)
#include <fstream> // std::ifstream & std::ofstream
#if !defined(_WINRT)
#include <unistd.h> // unlink ...
#endif
#endif

//////////////////////////////////////////////////////////////////////////

/** \details
  <group ExRender_Classes>
*/
class OdCldGuardedObject
{
  class OdCldGuardPointer* m_pWvPointer; friend class OdCldGuardPointer;
public:
  OdCldGuardedObject() 
    : m_pWvPointer(NULL)
  {
  }
  virtual ~OdCldGuardedObject();
};

// service soft wrapper with auto-clearing reference by delete

/** \details
  <group ExRender_Classes>
*/
class OdCldGuardPointer
{
protected:
  OdCldGuardedObject* m_pObj;
  OdCldGuardPointer* m_pPrev;
  OdCldGuardPointer* m_pNext;

  friend class OdCldGuardedObject;
  void remove()
  {
    if (!m_pObj)
    {
      ODA_ASSERT_ONCE(m_pPrev == this && m_pNext == this);
      return;        
    }
    ODA_ASSERT_ONCE(m_pPrev && m_pNext);

    if (m_pNext == this)
    {
      ODA_ASSERT_ONCE(m_pPrev == this)
      m_pObj->m_pWvPointer = NULL;
      m_pObj = NULL;
      return;
    }
    ODA_ASSERT_ONCE(m_pPrev != this)

    OdCldGuardPointer* pNext = m_pNext;
    OdCldGuardPointer* pPrev = m_pPrev;
    m_pNext = this;
    m_pPrev = this;
    pPrev->m_pNext = pNext; 
    pNext->m_pPrev = pPrev;

    if (m_pObj->m_pWvPointer == this)
      m_pObj->m_pWvPointer = pNext;
    m_pObj = NULL;
  }

  void append(OdCldGuardedObject* ptr)
  {
    if (m_pObj == ptr)
      return;
    remove();
    ODA_ASSERT_ONCE(!m_pObj && m_pPrev == this && m_pNext == this);
    if (!ptr)
      return;

    if (ptr->m_pWvPointer)
    {
      m_pNext = ptr->m_pWvPointer;
      m_pPrev = ptr->m_pWvPointer->m_pPrev;
      m_pNext->m_pPrev = this;
      m_pPrev->m_pNext = this; 
    }
    ptr->m_pWvPointer = this;
    m_pObj = ptr;
  }

  OdCldGuardPointer& operator=(OdCldGuardedObject* ptr)
  { 
    if (m_pObj == ptr)
      return *this;
    append(ptr);
    return *this; 
  }

  OdCldGuardPointer(OdCldGuardedObject* ptr = NULL) 
    : m_pObj(NULL)
  {
    m_pPrev = m_pNext = this;
    operator=(ptr);
  }

  virtual ~OdCldGuardPointer()
  {
    if (m_pObj) 
      remove();
  }

public:
  bool isNull() const
  { 
    return !m_pObj; 
  }

  OdCldGuardedObject* get() const
  {
    return m_pObj;
  }
};

inline OdCldGuardedObject::~OdCldGuardedObject()
{
  while (m_pWvPointer)
    m_pWvPointer->remove();
}

/** \details
  <group ExRender_Classes>
*/
template <class T>
class OdCldPointer : public OdCldGuardPointer
{
public:
  OdCldPointer(T* ptr = NULL) 
    : OdCldGuardPointer(ptr)
  { 
  }  
  OdCldPointer(const OdCldPointer<T>& ptr)
    : OdCldGuardPointer(ptr.get())
  { 
  }
  inline OdCldPointer<T> &operator=(T* ptr)
  { 
    OdCldGuardPointer::operator=(ptr);
    return *this; 
  }
  inline OdCldPointer<T> &operator=(const OdCldPointer<T>& ptr)
  { 
    return operator=(ptr.get());
  }

  T* get() const
  { 
    return (T*) const_cast<OdCldGuardedObject*>(m_pObj); 
  }
  operator T*() const
  { 
    return get();
  }
  T* operator->() const
  { 
    return get();
  }
  T& operator*() const
  { 
    return *get();
  }
};

//////////////////////////////////////////////////////////////////////////

#define OD_COPY_DISABLED(class)     \
  private:                          \
    class(const class&);            \
    class& operator=(const class&); \
  protected:

//////////////////////////////////////////////////////////////////////////

#if defined(__GNUC__) && !defined(__INTEL_COMPILER) && !defined(__ARMCC__) && !defined(__CC_ARM)
// make use of typeof-extension

/** \details
  <group ExRender_Classes>
*/
template <typename T>
class OdForeachContainer {
public:
    inline OdForeachContainer(const T& t) : c(t), brk(0), i(c.begin()), e(c.end()) { }
    const T c;
    int brk;
    typename T::const_iterator i, e;
};

#define odforeach(variable, container)                                            \
  for (OdForeachContainer<__typeof__(container)> _container_(container);          \
       !_container_.brk && _container_.i != _container_.e;                        \
       __extension__  ({ ++_container_.brk; ++_container_.i; }))                  \
    for (variable = *_container_.i;; __extension__ ({--_container_.brk; break;}))

#else

struct OdForeachContainerBase {};

/** \details
  <group ExRender_Classes>
*/
template <typename T>
class OdForeachContainer : public OdForeachContainerBase {
public:
  inline OdForeachContainer(const T& t)
    : c(t)
    , brk(0)
    , i(c.begin()), e(c.end())
  {
  }
  const T c;
  mutable int brk;
  mutable typename T::const_iterator i, e;
  inline bool condition() const 
  { 
    return (!brk++ && i != e); 
  }
};

template <typename T> inline T *foreachPointer(const T &) 
{ 
  return NULL; 
}

template <typename T> inline OdForeachContainer<T> foreachContainerNew(const T& t)
{ 
  return OdForeachContainer<T>(t); 
}

template <typename T>
inline const OdForeachContainer<T> *foreachContainer(const OdForeachContainerBase *base, const T *)
{ 
  return static_cast<const OdForeachContainer<T>* >(base); 
}

#if (defined(_MSC_VER) && _MSC_VER >= 1300 && !defined(__INTEL_COMPILER)) || defined(__sgi)
// in VC++6 and MIPSpro CC
#  define odforeach(variable,container)                                                            \
     if (false) {} else                                                                            \
       for (const OdForeachContainerBase &_container_ = foreachContainerNew(container);            \
            foreachContainer(&_container_, true ? 0 : foreachPointer(container))->condition();     \
            ++foreachContainer(&_container_, true ? 0 : foreachPointer(container))->i)             \
         for (variable = *foreachContainer(&_container_, true ? 0 : foreachPointer(container))->i; \
              foreachContainer(&_container_, true ? 0 : foreachPointer(container))->brk;           \
              --foreachContainer(&_container_, true ? 0 : foreachPointer(container))->brk)

#elif defined(__DCC__)
// VxWorks DIAB generates unresolvable symbols, if container is a function call
#  define odforeach(variable,container)                                                           \
    if (false) {} else                                                                            \
      for (const OdForeachContainerBase &_container_ = foreachContainerNew(container);            \
           foreachContainer(&_container_, (__typeof__(container) *) 0)->condition();              \
           ++foreachContainer(&_container_, (__typeof__(container) *) 0)->i)                      \
        for (variable = *foreachContainer(&_container_, (__typeof__(container) *) 0)->i;          \
             foreachContainer(&_container_, (__typeof__(container) *) 0)->brk;                    \
             --foreachContainer(&_container_, (__typeof__(container) *) 0)->brk)

#else
#  define odforeach(variable, container)                                                          \
    for (const OdForeachContainerBase &_container_ = foreachContainerNew(container);              \
         foreachContainer(&_container_, true ? 0 : foreachPointer(container))->condition();       \
         ++foreachContainer(&_container_, true ? 0 : foreachPointer(container))->i)               \
        for (variable = *foreachContainer(&_container_, true ? 0 : foreachPointer(container))->i; \
             foreachContainer(&_container_, true ? 0 : foreachPointer(container))->brk;           \
             --foreachContainer(&_container_, true ? 0 : foreachPointer(container))->brk)
#endif // #if (defined(_MSC_VER) && _MSC_VER >= 1300 && !defined(__INTEL_COMPILER)) || defined(__sgi)

#endif // if defined(__GNUC__) && !defined(__INTEL_COMPILER) && !defined(__ARMCC__) && !defined(__CC_ARM)

//////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#  define chPathDelimiter L'\\'
#  define chPathSeparator L';'
#else
#  define chPathDelimiter L'/'
#  define chPathSeparator L':'
#endif

//////////////////////////////////////////////////////////////////////////
// TODO move the next to OdPlatform.h at odmin & odmax

#define odabs(X) ((X) > 0 ? (X) : -(X))

#ifndef FLT_MAX 
#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F
#endif
#ifndef SHRT_MAX
#define SHRT_MAX 32767
#define SHRT_MIN (-32768)
#endif

//////////////////////////////////////////////////////////////////////////
// TODO move the next to DebugStuff.h at ODA_ASSERT declaration

#ifdef ODA_DIAGNOSTICS

// to declare variables for assert conditions only
#  define ODA_DEB(code) code

#  undef ODA_ASSERT_ONCE
#  define ODA_ASSERT_ONCE(exp)\
     do {\
       static bool was_here = false;\
       if (!was_here && !(exp))\
       {\
         was_here = true;\
         OdAssert(#exp, __FILE__, __LINE__);\
       }\
     } while (0)
#  undef ODA_FAIL_ONCE
#  define ODA_FAIL_ONCE()\
     do {\
       static bool was_here = false;\
       if (!was_here)\
       {\
         was_here = true;\
         OdAssert("Invalid Execution.", __FILE__, __LINE__);\
       }\
     } while (0)

#else
#  define ODA_DEB(code)
#endif

//////////////////////////////////////////////////////////////////////////
// TODO move the next into OdaDefs.h

#ifdef SETBIT
#  undef SETBIT
#endif
#define SETBIT(flags, bit, value) ((value) ? ((*(int*)&flags) |= (bit)) : ((*(int*)&flags) &= ~(bit)))

//////////////////////////////////////////////////////////////////////////
// TODO move the next as methods into class OdString

inline OdString OdString_trimmed(const OdString& str)
{
  return OdString(str).trimLeft().trimRight();
}

inline OdString OdString_toLower(const OdString& str)
{
  return OdString(str).makeLower();
}

inline OdString OdString_toUpper(const OdString& str)
{
  return OdString(str).makeUpper();
}

inline OdString& OdString_replace(OdString& str, OdChar oldChar, OdChar newChar)
{
  str.replace(oldChar, newChar);
  return str;
}

inline OdString& OdString_replace(OdString& str, const OdChar* pOld, const OdChar* pNew)
{
  str.replace(pOld, pNew);
  return str;
}

inline OdString& OdString_replace(OdString& str, int pos, int num, const OdString& sAfter)
{
  OdString str_ = str.left(pos);
  str_ += sAfter;
  str = str_ + str.mid(pos + num);
  return str;
}

inline OdString& OdString_remove(OdString& str, OdChar chRemove)
{
  str.remove(chRemove);
  return str;
}

inline OdString& OdString_remove(OdString& str, const OdChar* pRemove)
{
  str.replace(pRemove, L"");
  return str;
}

inline OdString& OdString_removeQuotes(OdString& str, OdChar chQuotes = L'\"')
{
  str.trimLeft().trimRight();

  while (!str.find(chQuotes))
  {
    if (   str.getLength() == 1
      || (!str.getLength() || str[str.getLength() - 1] != chQuotes)) // !str.endsWith(chQuotes))
    {
      ODA_FAIL_ONCE(); // syntax error: unpaired quotes
      break;
    }

    str = str.mid(1, str.getLength() - 2);
  }
  return str;
}

inline OdAnsiString& OdAnsiString_addBrackets(OdAnsiString& str, 
                                              char chBracketLeft = '(',
                                              char chBracketRight = ')',
                                              bool* pbFound = NULL)
{
  str.trimLeft().trimRight();
  if (pbFound)
    *pbFound = false;
  if (str.find(chBracketLeft))
    str = OdAnsiString(chBracketLeft, 1) + str + OdAnsiString(chBracketRight, 1);
  else if (pbFound)
    *pbFound = true;
  return str;
}

inline OdString& OdString_addBrackets(OdString& str, 
                                      OdChar chBracketLeft = L'(',
                                      OdChar chBracketRight = L')',
                                      bool* pbFound = NULL)
{
  str.trimLeft().trimRight();
  if (pbFound)
    *pbFound = false;
  if (str.find(chBracketLeft))
    str = OdString(chBracketLeft, 1) + str + OdString(chBracketRight, 1);
  else if (pbFound)
    *pbFound = true;
  return str;
}

inline OdString& OdString_removeBrackets(OdString& str, 
                                         OdChar chBracketLeft = L'(',
                                         OdChar chBracketRight = L')',
                                         bool* pbFound = NULL)
{
  str.trimLeft().trimRight();
  if (pbFound)
    *pbFound = false;

  while (!str.find(chBracketLeft))
  {
    if (   str.getLength() == 1
        || (!str.getLength() || str[str.getLength() - 1] != chBracketRight)) // !str.endsWith(chBracketRight))
    {
      ODA_FAIL_ONCE(); // syntax error: unpaired bracket
      break;
    }

    str = str.mid(1, str.getLength() - 2);
    if (pbFound)
      *pbFound = true;
  }
  return str;
}

inline OdString OdString_toCmdArg(const OdString& csPath)
{
  OdString sPath = csPath;
  if (!sPath.find(L'\\'))
    // first '\\' is special to enter data from Upper context 
    // (see #13896 Menu Draw/Circle/Center, Diameter)
    sPath.replace(L'\\', L'/'); 

  //int pos = sPath.findOneOf(L"\" \t");
  //if (pos <= 0)
  //{
  //  ODA_ASSERT_ONCE(pos < 0 || sPath[0] == L'\"');
  //  return sPath;
  //}
  //ODA_ASSERT_ONCE(sPath[pos] != L'\"'); // TODO
  //
  //will be gotten by OdEdUserIO::getFilePath via getString(pmt, OdEd::kGstAllowSpaces, ...
  //Therefore each path should be on separate line or in "..."

  sPath = L"\"" + sPath + L"\"";
  return sPath;
}

inline int OdString_reverseFind(const OdString& str, const OdChar* sfx)
{
  int idxStart = -1,
      pos;
  while ((pos = str.find(sfx, idxStart + 1)) >= 0)
    idxStart = pos;
  return idxStart;
}

inline int OdString_lengthOfNumber(const OdString& str, int posStart = 0)
{
  int pos = posStart,
      len = str.getLength();
  for (; pos < len; pos++)
  {
    if (str.getAt(pos) < L'0' || str.getAt(pos) > L'9')
      break;
  }
  return pos - posStart;
}

inline int OdString_lengthOfName(const OdString& str, int posStart = 0)
{
  int pos = posStart,
      len = str.getLength();
  for (; pos < len; pos++)
  {
    if (   (str.getAt(pos) < L'A' || str.getAt(pos) > L'Z') // !isalpha
        && (str.getAt(pos) < L'a' || str.getAt(pos) > L'z')
        && (   pos == posStart
            || str.getAt(pos) < L'0' || str.getAt(pos) > L'9')) // !isdigit
      break;
  }
  return pos - posStart;
}

inline bool OdString_isPositionInsideQuotes(const OdString& str, 
                                            int pos,
                                            OdChar chQuotes = L'\"')
{
  ODA_ASSERT_ONCE(pos >= 0);
  int start = 0,
      posStart = -1,
      posEnd = -1;
  while ((posStart = str.find(chQuotes, start)) >= 0)
  {
    if (posStart > pos)
      return false;
    ODA_ASSERT_ONCE(pos > posStart);
    start = posStart + 1; 

    while ((posEnd = str.find(chQuotes, start)) >= 0)
    {
      if (str.getAt(posEnd - 1) != L'\\')
        break;
      start = ++posEnd; 
    }
    ODA_ASSERT_ONCE(posEnd >= start); // (posEnd >= 0)
    if (posEnd < start)
      break;
    
    if (posEnd > pos)
      return true;

    start = ++posEnd;
  }

  return false;
}

inline bool OdString_isPositionInsideBrackets(const OdString& str, 
                                              int pos,
                                              OdChar chBracketLeft = L'(',
                                              OdChar chBracketRight = L')')
{
  ODA_ASSERT_ONCE(pos >= 0 && chBracketLeft != chBracketRight);
  int start = 0,
      level = 0,
      posStart = -1,
      posEnd = -1;
  while (true)
  {
    posStart = str.find(chBracketLeft, start);
    if (level > 0)
      posEnd = str.find(chBracketRight, start);
    else
      posEnd = -1;
    if (posStart < start && posEnd < start)
      break;

    if (posStart >= start && posEnd >= start) 
    {
      if (posStart < posEnd) 
        posEnd = -1;
      else
        posStart = -1;
    }

    if (posStart >= start)
    {
      if (!level && posStart > pos)
        return false;
      
      ODA_ASSERT_ONCE(pos > posStart);
      start = posStart + 1; 
      level++;
      continue;
    }
    ODA_ASSERT_ONCE(posEnd >= start);
    start = ++posEnd; 

    if (!--level && posEnd > pos)
      return true;
  }

  return false;
}

inline OdString OdString_extractFirst(OdString& sPath, 
                                      OdChar chDelim = L'/', 
                                      bool bTrimSpaces = true,
                                      bool bKeepQuotes = false,
                                      bool* pbLastDelimFound = NULL)
{
  OdString sFirst;
  int start = 0;
  int pos = -1;
  if (pbLastDelimFound)
    *pbLastDelimFound = false;

  while ((pos = sPath.find(chDelim, start)) >= 0)
  {
    if (   bKeepQuotes
        && (   OdString_isPositionInsideQuotes(sPath, pos, L'\"')
            || OdString_isPositionInsideBrackets(sPath, pos, L'[', L']')))
    {
      start = ++pos;
      continue;
    }

    if (pbLastDelimFound)
      *pbLastDelimFound = true;
    break;
  }

  if (pos < 0)
  {
    sFirst = sPath;
    sPath.empty();
    if (bTrimSpaces)
      sFirst.trimRight().trimLeft();
    return sFirst;
  }

  sFirst = sPath.left(pos);
  sPath = sPath.mid(++pos);
  if (bTrimSpaces)
  {
    sFirst.trimRight().trimLeft();
    sPath.trimRight().trimLeft();
  }
  return sFirst;
}

inline OdString OdString_last(const OdString& sPath, 
                              OdChar chDelim = L'/', 
                              bool bTrimSpaces = true)
{
  OdString sLast = sPath;
  int pos = sPath.reverseFind(chDelim);
  if (pos >= 0)
    sLast = sPath.mid(++pos);
  if (bTrimSpaces)
    sLast.trimRight().trimLeft();
  return sLast;
}

inline OdAnsiString OdString_last(const OdAnsiString& sPath, 
                                  char chDelim = '/', 
                                  bool bTrimSpaces = true)
{
  OdAnsiString sLast = sPath;
  int pos = sPath.reverseFind(chDelim);
  if (pos >= 0)
    sLast = sPath.mid(++pos);
  if (bTrimSpaces)
    sLast.trimRight().trimLeft();
  return sLast;
}

inline OdString OdString_skipTo(const OdString& sPath, const OdString& sMarker,
                                bool bWithMarker = true)
{
  int pos = sPath.find(sMarker);
  if (pos < 0)
    return OdString::kEmpty;
  if (bWithMarker)
    pos += sMarker.getLength();
  return sPath.mid(pos);
}

inline OdAnsiString OdString_skipTo(const OdAnsiString& asPath, const OdAnsiString& asMarker,
                                    bool bWithMarker = true)
{
  int pos = asPath.find(asMarker);
  if (pos < 0)
    return "";
  if (bWithMarker)
    pos += asMarker.getLength();
  return asPath.mid(pos);
}

inline OdStringArray OdString_split(const OdString& cstr, 
                                    OdChar chDelim,   // TODO: // = L' ', 
                                    bool bSkipEmpty,  // TODO: //  = true,
                                    bool bTrimSpaces, // TODO: // = true,
                                    bool bKeepQuotes) // TODO: // = false
{
  OdString sNext = cstr;
  OdStringArray res;

  if (bTrimSpaces)
    sNext.trimLeft().trimRight();

  bool bLastDelimFound = false;
  while (!sNext.isEmpty())
  {
    OdString str = OdString_extractFirst(sNext, chDelim, bTrimSpaces, bKeepQuotes, &bLastDelimFound);
    if (bTrimSpaces && chDelim != L' ')
      str.trimRight();
    if (!str.isEmpty() || !bSkipEmpty) 
      res.push_back(str);
    if (bTrimSpaces)
      sNext.trimLeft();
  }
  if (!bSkipEmpty && res.size() && bLastDelimFound)
    res.push_back(sNext);

  return res;
}

inline OdStringArray OdString_split(const OdString& cstr, 
                                    OdChar chDelim = L' ', 
                                    bool bSkipEmpty = true,
                                    bool bTrimSpaces = true)
{
  OdString sNext = cstr;
  OdStringArray res;
  int pos;

  if (bTrimSpaces)
    sNext.trimLeft().trimRight();

  while ((pos = sNext.find(chDelim)) >= 0)
  {
    OdString str = sNext.left(pos);
    if (bTrimSpaces && chDelim != L' ')
      str.trimRight();
    if (!str.isEmpty() || !bSkipEmpty) 
      res.push_back(str);

    sNext = sNext.mid(pos + 1);
    if (bTrimSpaces)
      sNext.trimLeft();
  }
  if (bTrimSpaces)
    sNext.trimRight();

  if (!sNext.isEmpty() || (!bSkipEmpty && res.size()))
    res.push_back(sNext);

  return res;
}

inline OdStringArray OdString_split(const OdString& cstr, 
                                    const OdString& sDelim, 
                                    bool bSkipEmpty = true,
                                    bool bTrimSpaces = true)
{
  OdString sNext = cstr;
  OdStringArray res;
  int pos;

  if (bTrimSpaces)
    sNext.trimLeft().trimRight();

  while ((pos = sNext.find(sDelim)) >= 0)
  {
    OdString str = sNext.left(pos);
    if (bTrimSpaces && sDelim != L" ")
      str.trimRight();
    if (!str.isEmpty() || !bSkipEmpty) 
      res.push_back(str);

    sNext = sNext.mid(pos + sDelim.getLength());
    if (bTrimSpaces)
      sNext.trimLeft();
  }
  if (bTrimSpaces)
    sNext.trimRight();

  if (!sNext.isEmpty() || (!bSkipEmpty && res.size()))
    res.push_back(sNext);

  return res;
}

// return NULL by end
inline const char* CharBufSplit_next(char*& pszBuf, // in /out 
                                     const char chDelim = ',', 
                                     bool bTrimSpaces = true)
{
  if (bTrimSpaces)
  {
    while (*pszBuf && (*pszBuf != chDelim) && (*pszBuf == ' '))
      pszBuf++;
  }
  const char* pcszRes = pszBuf;
  if (!*pcszRes)
    return NULL; // end
  for (; *pszBuf && (*pszBuf != chDelim); pszBuf++)
  {
    if (bTrimSpaces && *pszBuf == ' ')
      *pszBuf = '\0';
  }
  if (*pszBuf == chDelim || (bTrimSpaces && *pszBuf == ' '))
    *pszBuf++ = '\0';
  return pcszRes;
}

//inline OdString& OdString_jsonToPath(OdString& sStr) // in / out
//{
//  sStr.replace(L"\\n", L"\n");
//  sStr.replace(L"\\r", L"\r");
//  sStr.replace(L"\\\'", L"\'");
//  sStr.replace(L"\\\"", L"\"");
//  sStr.replace(L"\\\\", L"\\");
//  return sStr;
//}
// use client side test-command ".cmd" to reproduce a problem 
inline OdString& OdString_jsonToPath(OdString& sStr) // in / out
{
  if (sStr.find(L'\\') < 0)
    return sStr;

  OdString sBuf;
  OdChar* pOut = sBuf.getBuffer(sStr.getLength() + 1);
  for (const OdChar* pIn = sStr.c_str(); *pIn; pIn++)
  {
    OdChar ch;
    int numBackslashes = 0;
    for (; (ch = *pIn) == L'\\'; pIn++)
    {
      if (!(++numBackslashes % 2))
        *pOut++ = L'\\';
    }
    if (numBackslashes)
    {
      if (!ch)
      {
        ODA_ASSERT_ONCE(!(numBackslashes % 2));
        break;
      }
      if (numBackslashes % 2)
      {
        switch (ch)
        {
        case L'n':
          ch = L'\n'; 
          break;
        case L'r':
          ch = L'\r'; 
          break;
        default:
          ODA_FAIL_ONCE(); // TODO
        case L'\"': case L'\'':
          break;
        }
      }
    }

    *pOut++ = ch;
  }
  *pOut = L'\0';
  sBuf.releaseBuffer();
  return sStr = sBuf;
}

inline OdString& OdString_pathToJson(OdString& sStr) // in / out
{
  sStr.replace(L"\\", L"\\\\");
  sStr.replace(L"\"", L"\\\"");
  sStr.replace(L"\'", L"\\\'");
  sStr.replace(L"\r", L"\\r");
  sStr.replace(L"\n", L"\\n");
  return sStr;
}

inline OdAnsiString& OdAnsiString_pathToJson(OdAnsiString& asStr) // in / out
{
  asStr.replace("\\", "\\\\");
  asStr.replace("\"", "\\\"");
  asStr.replace("\'", "\\\'");
  asStr.replace("\r", "\\r");
  asStr.replace("\n", "\\n");
  return asStr;
}

inline OdAnsiString& OdAnsiString_toXmlAttrib(OdAnsiString& asStr) // in / out
{
  ODA_ASSERT_ONCE(asStr.findOneOf("\"&\'\r") < 0); //ODA_ASSERT_ONCE(asStr.findOneOf("\"&<>\'\n\r") < 0);
  //asStr.replace("&", "&amp;");
  asStr.replace("<", "&lt;");
  asStr.replace(">", "&gt;");
  //asStr.replace("\"", "&quot;");
  //asStr.replace("\'", "&apos;");
  asStr.replace("\n", "&#x0A;");
  //asStr.replace("\r", "&#x0D;");
  return asStr;
}

inline bool OdString_parseJsonRpc(const OdString& csJsonRpc, 
                                  OdStringArray& names, OdStringArray& values,
                                  OdString* pErrMessage = NULL)
{
  OdString sJsonRpc = csJsonRpc;
  bool isInBrackets = false;

  sJsonRpc = OdString_removeBrackets(sJsonRpc, L'{', L'}', &isInBrackets);
  if (!isInBrackets)
  {
    if (pErrMessage)
      *pErrMessage = L"OdString_parseJsonRpc: Syntax error.";
    return false;
  }

  OdStringArray pairs = OdString_split(sJsonRpc, L',', true, true, true);
  odforeach (OdString pair, pairs)
  {
    OdString sName = OdString_extractFirst(pair, L':', true, true);
    sName = OdString_jsonToPath(OdString_removeQuotes(OdString_removeQuotes(sName), L'\''));
    if (sName.isEmpty())
    {
      ODA_FAIL_ONCE(); // test
      if (pErrMessage)
        *pErrMessage = L"OdString_parseJsonRpc: Syntax error.";
      return false;
    }
    OdString sValue = OdString_removeQuotes(pair);
    sValue = OdString_jsonToPath(OdString_removeQuotes(sValue, L'\''));

    names.push_back(sName);
    values.push_back(sValue);
  }
  ODA_ASSERT_ONCE(names.size() == values.size());

  if (names.isEmpty())
  {
    if (pErrMessage)
      *pErrMessage = L"OdString_parseJsonRpc: data is absent.";
    return false;
  }

  return true;
}

inline OdString OdString_join(const OdStringArray& entries, 
                              const OdString& sDelim = L",")
{
  OdString sRes;
  odforeach (OdString sEntry, entries)
  {
    if (!sRes.isEmpty())
      sRes += sDelim;
    sRes += sEntry;
  }
  return sRes;
}

inline OdString OdString_getByKey(const OdStringArray& pairs,
                                  const OdString& sKey,
                                  const OdString& sDefValue = OdString::kEmpty,
                                  const OdString& sDelimInPair = L"=",
                                  bool bIgnoreCase = true)
{
  ODA_ASSERT_ONCE(!sKey.isEmpty());

  odforeach (OdString sPair, pairs)
  {
    OdString sName = OdString_extractFirst(sPair, L'=');
    sName = OdString_removeQuotes(sName);
    if (sPair.isEmpty() || sPair.find(L'=') >= 0)
      continue; // possible via side data in cookie 

    if (   sName != sKey
        && (!bIgnoreCase || OdString_toLower(sName) != OdString_toLower(sKey)))
      continue;

    return OdString_removeQuotes(sPair);
  }

  return sDefValue;
}

inline OdString OdString_getByKey(const OdString& sPairs, 
                                  const OdString& sKey, 
                                  const OdString& sDefValue = OdString::kEmpty,
                                  const OdString& sDelimPairs = L";",
                                  const OdString& sDelimInPair = L"=",
                                  bool bIgnoreCase = true)
{
  return OdString_getByKey(OdString_split(sPairs, sDelimPairs), sKey, sDefValue, sDelimInPair, bIgnoreCase);
}

inline bool OdString_isMatched(const OdChar* str, const OdChar* pat)
{
  switch (*pat) 
  {
  case L'\0': 
    return *str == L'\0';
  case L'*':
    return OdString_isMatched(str, pat + 1) || (*str && OdString_isMatched(str + 1, pat));
  case L'?': 
    return *str && OdString_isMatched(str + 1, pat + 1);
  }
  return *pat==*str && OdString_isMatched(str + 1, pat + 1);
}

inline bool OdString_isMatched(const OdString& sValue, const OdString& sWildCard, bool bIgnoreCase)
{
  ODA_ASSERT_ONCE(!sValue.isEmpty());
  if (sValue.isEmpty() || sWildCard.isEmpty())
    return false;

  // This function is used in CloudService on upper wrapper level
  // It is important to avoid linking with DbRoot and others on this level
  // => possible TODO for v5.0 :
  // moved odutWcMatch & odutWcMatchNoCase from DbRooot to Root module
  // and used it here (with #include "OdUtilAds.h")

  bool bRes = false;
  if (bIgnoreCase)
    bRes = OdString_isMatched(OdString_toLower(sValue).c_str(), OdString_toLower(sWildCard).c_str());
  else
    bRes = OdString_isMatched(sValue.c_str(), sWildCard.c_str());
  return bRes;
}

inline OdString OdString_toUnicode(const char* pAnsi)
{
  OdAnsiString asCmd(pAnsi);
  OdCharArray buf;
  OdCharMapper::utf8ToUnicode(asCmd.c_str(), asCmd.getLength(), buf);
  OdString sRes = OdString(buf.getPtr(), buf.size() - 1);
  // possible TODO substitutions for & ...
  return sRes;
}

inline OdAnsiString OdString_toUtf8(const OdString& sUnicode)
{
  OdAnsiCharArray dstBuf;
  int lenStr = sUnicode.getLength();
  dstBuf.reserve(lenStr * 4 + 1);
  OdCharMapper::unicodeToUtf8(sUnicode.c_str(), lenStr, dstBuf);
  OdAnsiString asRes((const char*)dstBuf.asArrayPtr(), dstBuf.length(), CP_UTF_8);
  // possible TODO substitutions for & ...
  return asRes;
}

inline bool OdString_toBool(const OdString& csValue)
{
  OdString sValue = csValue;
  bool bValue = (!sValue.isEmpty() && sValue.makeLower() != L"false" && sValue != L"off" && sValue != L"0");
  return bValue;
}

inline OdString aciColor_toString(int index)
{
  ODA_ASSERT_ONCE(index >= 0 && index <= 255);
  switch (index)
  {
  case OdCmEntityColor::kACIRed: return L"red";
  case OdCmEntityColor::kACIYellow: return L"yellow";
  case OdCmEntityColor::kACIGreen: return L"green";
  case OdCmEntityColor::kACICyan: return L"cyan";
  case OdCmEntityColor::kACIBlue: return L"blue";
  case OdCmEntityColor::kACIMagenta: return L"magenta";
  case OdCmEntityColor::kACIWhite: return L"white";
  }

  return OdString().format(L"%d", index);
}

inline OdCmEntityColor OdString_toOdColor(const OdString& sColor,
                                          bool* pIsOk = NULL)
{
  if (pIsOk)
    *pIsOk = false;

  if (sColor.isEmpty() || sColor == L"*VARIES*")
  {
    ODA_FAIL_ONCE();
    return OdCmEntityColor();
  }

  OdStringArray lstRgb = OdString_split(sColor, L',');
  if (lstRgb.size() == 3)
  {
    OdUInt8 rgb[3] = { 0, 0, 0 };
    for (int idx = 0; idx < (int) lstRgb.size(); idx++)
    {
      OdString sVal = lstRgb[idx]; 
      unsigned int iVal = 0;
      if (odSScanf(sVal, L"%d", &iVal) != 1 || iVal > 255)
      {
        ODA_FAIL_ONCE();
        return OdCmEntityColor(OdCmEntityColor::kNone); // OdCmEntityColor::kACInone
      }
      rgb[idx] = (OdUInt8) iVal;
    }
    if (pIsOk)
      *pIsOk = true;
     return OdCmEntityColor(rgb[0], rgb[1], rgb[2]);
  }

  unsigned int indexColor = 0;
  if (odSScanf(sColor, L"%d", &indexColor) == 1 && indexColor <= 255)
  {
    //kACIbyBlock       0       ByBlock.
    //kACIforeground    7       Foreground *color*.
    //kACIbyLayer       256     ByLayer. 
    //kACIRed           1       Red. 
    //kACIYellow        2       Yellow. 
    //kACIGreen         3       Green. 
    //kACICyan          4       Cyan. 
    //kACIBlue          5       Blue. 
    //kACIMagenta       6       Magenta. 
    //..                8-255   Defined by display device.
    //kACInone          257     No *color*.

    OdCmEntityColor color(OdCmEntityColor::kByACI);
    ODA_ASSERT_ONCE(   indexColor >= OdCmEntityColor::kACIRed 
                    && indexColor < OdCmEntityColor::kACInone);
    color.setColorIndex((OdInt16) indexColor);
    return color;
  }

  for (indexColor = OdCmEntityColor::kACIRed; indexColor <= OdCmEntityColor::kACIWhite; indexColor++)
  {
    if (sColor != aciColor_toString(indexColor))
      continue;

    OdCmEntityColor color(OdCmEntityColor::kByACI);
    color.setColorIndex(indexColor);
    if (pIsOk)
      *pIsOk = true;
    return color;
  }

  if (sColor == L"ByLayer")
  {
    if (pIsOk)
      *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kByLayer);
  }
  if (sColor == L"ByBlock")
  {
    if (pIsOk)
      *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kByBlock);
  }
  if (sColor == L"Foreground")
  {
    if (pIsOk)
      *pIsOk = true;
    return OdCmEntityColor(OdCmEntityColor::kForeground);
  }

  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(sColor == L"None");
  return OdCmEntityColor(OdCmEntityColor::kNone); // OdCmEntityColor::kACInone
}

inline OdString OdColor_toString(const OdCmEntityColor& color)
{
  OdString sColor;  

  switch (color.colorMethod())
  {
  case OdCmEntityColor::kByLayer:
    return L"ByLayer";
  case OdCmEntityColor::kByBlock:
    return L"ByBlock";
  case OdCmEntityColor::kForeground:
    return L"Foreground";
  case OdCmEntityColor::kNone:
    ODA_FAIL_ONCE();
    sColor = L"None";
    break;
  case OdCmEntityColor::kByACI: // index in palette "Index Color"
    sColor = aciColor_toString(color.colorIndex());
    break;
  case OdCmEntityColor::kByColor:
    sColor.format(L"%d,%d,%d", color.red(), color.green(), color.blue());
    break;
  }

  if (sColor.isEmpty())
  {
    ODA_FAIL_ONCE(); // test
    sColor = L"Custom...";
  }
  return sColor;
}

//////////////////////////////////////////////////////////////////////////

inline OdUInt32 encodeToBase64(const unsigned char* bytesToEncode, OdUInt32 length, 
                               OdAnsiString& sOut, bool inQuotes = false) 
{
  static const char* base64Dict = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  //char* res = new char[len * 8 / 6 + 2];
  char* res = sOut.getBuffer(length * 8 / 6 + 4 + (inQuotes ? 2 : 0));
  if (inQuotes)
   *res++ = '\"';

  OdUInt32 len = length,
           finalLen = 0,
           i = 0,
           j = 0;
  unsigned char charArraySrc[3];
  unsigned char charArrayFin[4];

  while (len--) {
    charArraySrc[i++] = *(bytesToEncode++);
    if (i == 3) {
      charArrayFin[0] = (charArraySrc[0] & 0xfc) >> 2;
      charArrayFin[1] = ((charArraySrc[0] & 0x03) << 4) + ((charArraySrc[1] & 0xf0) >> 4);
      charArrayFin[2] = ((charArraySrc[1] & 0x0f) << 2) + ((charArraySrc[2] & 0xc0) >> 6);
      charArrayFin[3] = charArraySrc[2] & 0x3f;

      for(i = 0; i < 4; i++)
        res[finalLen++] = base64Dict[charArrayFin[i]];
      i = 0;
    }
  }

  if (i != 0)
  {
    for(j = i; j < 3; j++)
      charArraySrc[j] = '\0';

    charArrayFin[0] = (charArraySrc[0] & 0xfc) >> 2;
    charArrayFin[1] = ((charArraySrc[0] & 0x03) << 4) + ((charArraySrc[1] & 0xf0) >> 4);
    charArrayFin[2] = ((charArraySrc[1] & 0x0f) << 2) + ((charArraySrc[2] & 0xc0) >> 6);
    charArrayFin[3] = charArraySrc[2] & 0x3f;

    for (j = 0; j < i + 1; j++)
      res[finalLen++] = base64Dict[charArrayFin[j]];

    while((i++ < 3))
      res[finalLen++] = '=';
  }
  
  ODA_ASSERT_ONCE(finalLen <= (length * 8 / 6 + 3));
  if (inQuotes)
  {
    res[finalLen] = '\"';
    res[finalLen + 1] = 0; // res[finalLen + 1] = 0;
    finalLen += 2;
  }
  else
    res[finalLen] = 0; // res[finalLen + 1] = 0;

  //sOut.releaseBuffer();
  return finalLen;
}

inline OdUInt32 decodeBase64(const OdAnsiString& stringToDecode, OdBinaryData& out)
{
  static unsigned char* decode64Dict = NULL;
  if (!decode64Dict)
  {
    static const char* base64Dict = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static OdBinaryData dataDict;
    dataDict.resize(256);
    decode64Dict = dataDict.asArrayPtr();
    for (OdUInt32 i = 0; i < 64; i++)
      decode64Dict[base64Dict[i]] = (unsigned char) i;
  }

  OdUInt32 length = stringToDecode.getLength();
  if (length % 4 != 0)
    return -1;
  const char* pToDecode = stringToDecode.c_str();

  OdUInt32 outLength = length * 3 / 4;
  outLength -= (pToDecode[length - 2] == '=') ? 2 : ((pToDecode[length - 1] == '=') ? 1 : 0);

  out.resize(outLength);
  unsigned char* data = (unsigned char*) out.asArrayPtr();

  for (OdUInt32 i = 0, j = 0; i < length;)
  {
    OdUInt32 sextet_a = (pToDecode[i] == '=') ? (0 & i++) : decode64Dict[pToDecode[i++]];
    OdUInt32 sextet_b = (pToDecode[i] == '=') ? (0 & i++) : decode64Dict[pToDecode[i++]];
    OdUInt32 sextet_c = (pToDecode[i] == '=') ? (0 & i++) : decode64Dict[pToDecode[i++]];
    OdUInt32 sextet_d = (pToDecode[i] == '=') ? (0 & i++) : decode64Dict[pToDecode[i++]];

    OdUInt32 triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

    if (j < outLength) 
      data[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < outLength) 
      data[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < outLength) 
      data[j++] = (triple >> 0 * 8) & 0xFF;
  }
  return outLength;
}

//////////////////////////////////////////////////////////////////////////

#if 0
#include "../Extensions/ExServices/ExDbCommandContext.h"
#include "../Extensions/ExServices/ExStringIO.h"
#include "Ed/EdCommandStack.h"

// attention include the next in CMakeLists.txt if you use this functions
// ${TD_ROOT}/Extensions/ExServices/ExDbCommandContext.cpp
// ${TD_ROOT}/Extensions/ExServices/ExKWIndex.cpp
// ${TD_ROOT}/Extensions/ExServices/ExStringIO.cpp

inline void OdEdCommandContext_loop(OdEdCommandContext* ctx)
{
  OdEdUserIO* io = ctx->userIO();
  OdString sCmd;
  do {
    sCmd.empty();
    try {
      sCmd = io->getString(L"Command");
    } catch (OdEdCancel&) {
      continue; // via eof
    }
    if (sCmd.isEmpty())
      break;

    try {
      ::odedRegCmds()->executeCommand(sCmd, ctx);
    } catch (OdEdCancel&) 
    {
      break; // via close command
    } 
  } while (!sCmd.isEmpty());
}

inline bool ExDbCommandContext_sendCommand(ExDbCommandContext* ctx, const OdString& sCmdWithArgs, 
                                           OdString* psError = NULL)
{
  OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(sCmdWithArgs);
  OdEdCommandContextPtr ctxClone = ctx->cloneObject(pStringIO.get());
  try {
    OdEdCommandContext_loop(ctxClone);
  }
  catch (OdError& err) {
    ODA_ASSERT_ONCE(psError);
    if (psError)
      *psError = err.description();
    return false;
  }
  catch (...)
  {
    return false;
  }
  return true;
}

inline bool odedRegCmds_sendCommand(OdRxObject* pRxDb, const OdString& sCmdWithArgs)
{
  //OdChar chDelim = L' ';
  //int pos = sCmdWithArgs.find(chDelim),
  //    posAlt = sCmdWithArgs.find(L'\n');
  //if (posAlt >= 0 && (pos < 0 || posAlt < pos)) 
  //  chDelim = L'\n';
  //OdStringArray lstArgs = OdString_split(sCmdWithArgs, chDelim, true, true, true);
  //if (!lstArgs.size())
  //  return false;
  //OdString sArgs = OdString_join(lstArgs, L"\n");
  //OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(sArgs);
  OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(sCmdWithArgs);

  OdEdCommandContextPtr pCtx = ExDbCommandContext::createObject(pStringIO, pRxDb);
  try
  {
    //OdString sCmd;
    //while (   !pStringIO->isEof() 
    //       && !(sCmd = pCtx->userIO()->getString(L"Command")).isEmpty())
    //  ::odedRegCmds()->executeCommand(sCmd, pCtx);
    OdEdCommandContext_loop(pCtx);
  }
  catch (...)
  {
    ODA_FAIL_ONCE();
  	return false;
  }
  return true;
}
#endif
//////////////////////////////////////////////////////////////////////////

inline OdInt32 OdBinaryData_find(const OdBinaryData& data, const OdBinaryData& marker)
{
  ODA_FAIL_ONCE(); // TODO
  return -1;
}

inline OdBinaryData OdBinaryData_mid(const OdBinaryData& data, OdInt32 pos, OdUInt32 len)
{
  OdBinaryData res;
  ODA_FAIL_ONCE(); // TODO
  return res;
}

inline void OdBinaryData_pushback(OdBinaryData& data, const void* pBuf, OdUInt32 len)
{
  for (OdUInt32 idx = 0; idx < len; idx++)
    data.push_back(((const OdUInt8*)pBuf)[idx]);
}

//////////////////////////////////////////////////////////////////////////

template <typename K,class V>
inline OdArray<K> map_keys(const std::map<K, V>& m)
{
  OdArray<K> keys;
  for (typename std::map<K, V>::const_iterator itr = m.begin(); itr != m.end(); itr++)
    keys.push_back(itr->first);
  return keys;
}

template <typename K,class V>
inline OdArray<V> map_values(const std::map<K, V>& m)
{
  OdArray<V> values;
  for (typename std::map<K, V>::const_iterator itr = m.begin(); itr != m.end(); itr++)
    values.push_back(itr->second);
  return values;
}

// for multimap

template <typename K,class V>
inline OdArray<K> map_keys(const std::multimap<K, V>& m)
{
  OdArray<K> keys;
  for (typename std::multimap<K, V>::const_iterator itr = m.begin(); itr != m.end(); )
  {
    K key = itr->first;
    keys.push_back(key);
    do 
      itr++; 
    while (itr != m.end() && key == itr->first);
  }
  return keys;
}

template <typename K,class V>
inline OdArray<V> map_values(const std::multimap<K, V>& m)
{
  OdArray<V> values;
  for (typename std::multimap<K, V>::const_iterator itr = m.begin(); itr != m.end(); itr++)
    values.push_back(itr->second);
  return values;
}

template <typename K,class V>
inline OdArray<V> map_values(const std::multimap<K, V>& m, const K& key)
{
  OdArray<V> values;
  for (typename std::multimap<K, V>::const_iterator itr = m.find(key); itr != m.end() && key == itr->first; itr++)
    values.push_back(itr->second);
  return values;
}

template <typename K,class V>
inline int map_count(const std::multimap<K, V>& m, const K& key)
{
  int cnt = 0;
  for (typename std::multimap<K, V>::const_iterator itr = m.find(key); itr != m.end() && key == itr->first; itr++)
    cnt++;
  return cnt;
}

//////////////////////////////////////////////////////////////////////////
// TODO move the next as operator into class OdStringArray

template <typename T>
inline OdStringArray& operator<<(OdStringArray& lst, const T& arg)
{
  lst.append(arg); // is working for arg = OdStringArray too // lst.push_back(arg);
  return lst;
}

//////////////////////////////////////////////////////////////////////////

inline OdString OdVariant_toString(const OdVariant& vValue,
                                   bool bIntAsUnsigned = false)
{
  OdString sValue;
  switch (vValue.varType())
  {
  case OdVariant::kVoid:
    ODA_FAIL_ONCE(); // test
    break;
  case OdVariant::kBool:
    sValue = vValue.getBool() ? L"true" : L"false";
    break;
  case OdVariant::kString:
    sValue = vValue.getString();
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    sValue.format(bIntAsUnsigned ? L"%u" : L"%d", vValue.getInt8());
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    sValue.format(bIntAsUnsigned ? L"%u" : L"%d", vValue.getInt16());
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    sValue.format(bIntAsUnsigned ? L"%lu" : L"%d", vValue.getInt32());
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    // bIntAsUnsigned
    sValue.format(PRIu64W, vValue.getUInt64());
    break;
  case OdVariant::kDouble:
    sValue.format(L"%g", vValue.getDouble());
    break;
  case OdVariant::kWString: // // == OdVariant::kAnsiString
    sValue = OdString(vValue.getAnsiString());
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  }
  return sValue;
}

inline bool OdVariant_toBool(const OdVariant& vValue)
{
  bool bValue = false;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    bValue = vValue.getBool();
    break;
  case OdVariant::kString:
    bValue = OdString_toBool(vValue.getString());
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    bValue = (vValue.getInt8() != 0);
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    bValue = (vValue.getInt16() != 0);
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    bValue = (vValue.getInt32() != 0);
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    bValue = (vValue.getInt64() != 0);
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    bValue = OdString_toBool(vValue.getAnsiString().c_str());
    break;
  case OdVariant::kRxObjectPtr:
    bValue = !vValue.getRxObjectPtr().isNull();
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return bValue;
}

inline int OdVariant_toInt(const OdVariant& vValue)
{
  int iValue = 0;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    iValue = vValue.getBool() ? 1 : 0;
    break;
  case OdVariant::kString:
    iValue = odStrToInt(vValue.getString());
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    iValue = vValue.getInt8();
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    iValue = vValue.getInt16();
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    iValue = vValue.getInt32();
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    iValue = (int) vValue.getInt64();
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    iValue = odStrToInt(OdString(vValue.getAnsiString()));
    break;
  case OdVariant::kRxObjectPtr:
    iValue = (long)OdPtrToInt32(vValue.getRxObjectPtr().get());
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return iValue;
}

inline double OdVariant_toDouble(const OdVariant& vValue)
{
  double dValue = 0;
  switch (vValue.varType())
  {
  case OdVariant::kBool:
    dValue = vValue.getBool() ? 1.0 : 0.0;
    break;
  case OdVariant::kString:
    dValue = odStrToD(vValue.getString());
    break;
  case OdVariant::kInt8: // == OdVariant::kUInt8
    dValue = vValue.getInt8();
    break;
  case OdVariant::kInt16: // == OdVariant::kUInt16
    dValue = vValue.getInt16();
    break;
  case OdVariant::kInt32: // == OdVariant::kUInt32
    dValue = vValue.getInt32();
    break;
  case OdVariant::kInt64: // == OdVariant::kUInt64
    dValue = (double) vValue.getInt64();
    break;
  case OdVariant::kWString: // == OdVariant::kAnsiString
    dValue = odStrToD(vValue.getAnsiString());
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    break;
  } // end switch
  return dValue;
}

inline ODCOLORREF OdVariant_toColorref(const OdVariant& vValue)
{
  OdString sValue = OdString_toLower(OdVariant_toString(vValue, true));
  unsigned int uiValue = 0;
  if (!sValue.find(L"0x"))
    //sValue = sValue.mid(2)
    odSScanf(sValue, L"0x%lx", &uiValue);
  else
    uiValue = odStrToD(sValue);
  return (ODCOLORREF) uiValue;
}

//////////////////////////////////////////////////////////////////////////

inline OdString OdRxDictionary_toString(OdRxDictionary* pProperties, const OdString& sKey, 
                                        const OdString& sDefValue,
                                        bool bIntAsUnsigned = false)
{
  if (!pProperties || !pProperties->has(sKey))
    return sDefValue;
  OdRxVariantValue vValue = (OdRxVariantValue) pProperties->getAt(sKey);
  return OdVariant_toString(*vValue.get(), bIntAsUnsigned);
}

inline bool OdRxDictionary_toBool(OdRxDictionary* pProperties, const OdString& sKey, 
                                  bool bDefValue)
{
  if (!pProperties || !pProperties->has(sKey))
    return bDefValue;
  OdRxVariantValue vValue = (OdRxVariantValue) pProperties->getAt(sKey);
  return OdVariant_toBool(*vValue.get());
}

inline int OdRxDictionary_toInt(OdRxDictionary* pProperties, const OdString& sKey, 
                                int nDefValue)
{
  if (!pProperties || !pProperties->has(sKey))
    return nDefValue;
  OdRxVariantValue vValue = (OdRxVariantValue) pProperties->getAt(sKey);
  return OdVariant_toInt(*vValue.get());
}

inline double OdRxDictionary_toDouble(OdRxDictionary* pProperties, const OdString& sKey, 
                                      double dDefValue)
{
  if (!pProperties || !pProperties->has(sKey))
    return dDefValue;
  OdRxVariantValue vValue = (OdRxVariantValue) pProperties->getAt(sKey);
  return OdVariant_toDouble(*vValue.get());
}

inline ODCOLORREF OdRxDictionary_toColorref(OdRxDictionary* pProperties, const OdString& sKey, ODCOLORREF clrDefValue)
{
  if (!pProperties || !pProperties->has(sKey))
    return clrDefValue;
  OdRxVariantValue vValue = (OdRxVariantValue) pProperties->getAt(sKey);
  return OdVariant_toColorref(*vValue.get());
}

//declared in DbHostAppServices.h //#define ODDB_HOST_APP_SERVICES L"OdDbHostAppServices" // like ODDB_FONT_SERVICES
#define ODRX_APP_VARS L"HostAppVariables" // see also ODDB_FONT_SERVICES & ODDB_HOST_APP_SERVICES 

inline OdString appVar_toString(const OdString& sKey, 
                                const OdString& sDefValue = OdString::kEmpty,
                                bool bIntAsUnsigned = false)
{
  return OdRxDictionary_toString(OdRxDictionary::cast(::odrxSysRegistry()->getAt(ODRX_APP_VARS)),
                                 OdString_toLower(sKey), sDefValue, bIntAsUnsigned);
}

inline bool appVar_toBool(const OdString& sKey, bool bDefValue)
{
  return OdRxDictionary_toBool(OdRxDictionary::cast(::odrxSysRegistry()->getAt(ODRX_APP_VARS)),
                               OdString_toLower(sKey), bDefValue);
}

inline int appVar_toInt(const OdString& sKey, int nDefValue)
{
  return OdRxDictionary_toInt(OdRxDictionary::cast(::odrxSysRegistry()->getAt(ODRX_APP_VARS)),
                              OdString_toLower(sKey), nDefValue);
}

inline double appVar_toDouble(const OdString& sKey, double dDefValue)
{
  return OdRxDictionary_toDouble(OdRxDictionary::cast(::odrxSysRegistry()->getAt(ODRX_APP_VARS)),
                                 OdString_toLower(sKey), dDefValue);
}

inline ODCOLORREF appVar_toColorref(const OdString& sKey, ODCOLORREF clrDefValue)
{
  return OdRxDictionary_toColorref(OdRxDictionary::cast(::odrxSysRegistry()->getAt(ODRX_APP_VARS)),
                                   OdString_toLower(sKey), clrDefValue);
}

// substitute variablse     *{name}  // * instead $ to solve problem under Visual Studio
inline OdString checkAppVar(const OdString& csValue, 
                            const OdString& sCommonDefValue = OdString::kEmpty)
{
  OdString sContent = csValue;
  for (int pos = 0, posStart = 0; 
       (pos = sContent.find(L"*{", posStart)) >= 0; posStart = ++pos)
  {
    int posEnd = sContent.find(L"}", pos); ODA_ASSERT_ONCE(posEnd > posStart);
    if (posEnd++ <= posStart)
      continue;
    OdString sName = sContent.mid(pos, posEnd - pos),
             sArgs = sName.mid(2, sName.getLength() - 3).c_str();
    OdStringArray lstArgs = OdString_split(sArgs, L',', false, true, true);
    int sz = lstArgs.size();
    if (!sz)
      sName.empty();
    else
      sName = lstArgs.first(), lstArgs.removeFirst(), sz--;
    ODA_ASSERT_ONCE(!sz || sz == 1); // only single arg supported now //ODA_ASSERT_ONCE(sz >= 0);
    OdString sValue = appVar_toString(sName, sz ? lstArgs.first() : sCommonDefValue);
    sContent = sContent.left(pos) + sValue + sContent.mid(posEnd);
  } // end for
  return sContent;
}

#if 0
inline void setAppVar(const OdString& sKey, const OdString& sValue)
{
  if (::odrxDynamicLinker()->loadModule(L"WebMisc.tx").isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }
  try {
    // it is better do via command to avoid crash problem via TxHost termination
    OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(L"appvar " + sKey + L" \"" + sValue + L"\"");
    OdEdCommandContext_loop(ExDbCommandContext::createObject(pStringIO));
  }
  catch (...) {
    ODA_FAIL_ONCE();
  }
}

//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <shellapi.h>
#elif defined(__linux__) 
#include <stdio.h>
#include <unistd.h>
#endif

inline OdStringArray applicationArgs()
{
  // http://stackoverflow.com/questions/160292/how-can-i-access-argc-and-argv-in-c-from-a-library-function

  OdStringArray args;
  int argc;
 #ifdef _WIN32
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//vector<wstring> args;
  if (argc && argv) 
  {
//  args.assign(argv, argv + argc);
    for (int idx = 0; idx < argc; idx++)
    {
      OdString arg = argv[idx];
      args.push_back(arg);
    }
    LocalFree(argv);
  }
 #elif defined(__linux__) && !defined(ANDROID)// TODO
  char **argv;
  char **p = &__environ[-2];
  size_t i;
  for (i = 1; i != *(size_t*)(p-1); i++)
    p--;
  argc = (int) i;
  argv = p;
  if (argc && argv) 
  {
    for (int idx = 0; idx < argc; idx++)
    {
      OdString arg = OdString_toUnicode(argv[idx]);
      args.push_back(arg);
    }
  }
 #elif defined(ANDROID)
 //TODO
 #endif
  return args;
}

inline OdString applicationPathName()
{
  OdString sPath;
 #ifdef _WIN32
    if (!::GetModuleFileName(::GetModuleHandle(0), sPath.getBuffer(MAX_PATH), MAX_PATH))
    {
      ODA_FAIL_ONCE(); // TODO
    }
    sPath.releaseBuffer();
 #elif defined(__linux__) // TODO
  // On Linux the pseudo-file /proc/self/cmdline holds the command line for the process.
  // Each argument is terminated with a 0 byte, and the final argument is followed by an additional 0 byte.
  //
  //readlink("/proc/self/exe", ... , ...);
  // http://www.linux.org.ru/forum/development/154958       
  // http://stackoverflow.com/questions/7051844/how-to-find-the-full-path-of-the-c-linux-program-from-within
  //string get_path() // only for kernel version >=2.2
  //{
  //  char arg1[20];
  //  char exepath[PATH_MAX + 1] = {0};
  //  sprintf( arg1, "/proc/%d/exe", getpid() );
  //  readlink( arg1, exepath, 1024 );
  //  return string( exepath );
  //}
  OdAnsiString asArg, asPath;
  asArg.format("/proc/%d/exe", getpid());
  readlink(asArg.c_str(), asPath.getBuffer(1025), 1024);
  asPath.releaseBuffer();
  sPath = OdString_toUnicode(asPath.c_str());
 #else 
  // is not a good way for linux
  // (ok under DEBUGGER but return ./OdaCloudServerx if executed like ./OdaCloudServerx (standard)
  OdStringArray args = applicationArgs();
  if (args.size())
    sPath = args.first();
 #endif
  return sPath;
}
#endif

//////////////////////////////////////////////////////////////////////////

/** \details
  <group ExRender_Classes>
*/
class OdFilePathName
{
  OdString m_sFilePathName;

  bool resolveUpper(OdString& path)
  {
    int pos = path.find(L"..");
    if (pos < 0)
    {
      if ((pos = path.find(L".")) >= 0)
      {
        ODA_ASSERT_ONCE(pos); // test
        OdString sPathDelimiter; sPathDelimiter += chPathDelimiter;
        path.replace(sPathDelimiter + L"." + sPathDelimiter, sPathDelimiter);
      }
      return false;
    }
    int posEnd = pos + 2;
    ODA_ASSERT_ONCE(pos > 2 && (posEnd == path.getLength() || path.getAt(posEnd) == chPathDelimiter));
    pos--;
    ODA_ASSERT_ONCE(path.getAt(pos) == chPathDelimiter);
    pos = path.reverseFind(chPathDelimiter, --pos);
    ODA_ASSERT_ONCE(pos > 0);
    if (pos < 0)
      pos = 0;
    path = path.left(pos) + path.mid(posEnd);
    return true;
  }

public:
  OdFilePathName& operator=(const OdFilePathName& from)
  {
    if (this == &from)
      return *this;
    m_sFilePathName = from.m_sFilePathName;
    return *this;
  }

  bool operator==(const OdFilePathName& with)
  {
    return m_sFilePathName == with.m_sFilePathName;
  }

  OdFilePathName(const OdString& path = OdString::kEmpty, // full or path only
                 const OdString& name = OdString::kEmpty, 
                 const OdString& suffix = OdString::kEmpty)
    : m_sFilePathName(path)
  {
    if (chPathDelimiter == L'\\')
      m_sFilePathName.replace(L'/',  L'\\');
    else
      m_sFilePathName.replace(L'\\', L'/');
    while (resolveUpper(m_sFilePathName)) {}

    if (!name.isEmpty())
    {
      if (   !m_sFilePathName.isEmpty() 
          && m_sFilePathName.find(chPathDelimiter) != (m_sFilePathName.getLength() - 1))
        m_sFilePathName += chPathDelimiter;
      m_sFilePathName += name;
    }

    if (!suffix.isEmpty())
    {
      if (suffix.find(L'.'))
        m_sFilePathName += L'.';
      m_sFilePathName += suffix;
    }
  }

  OdFilePathName(const OdFilePathName& from)
  {
    operator=(from);
  }

  OdString pathName() const
  {
    return m_sFilePathName;
  }

  OdString path() const
  {
    int pos = m_sFilePathName.reverseFind(chPathDelimiter);
    if (pos < 0)
      return OdString::kEmpty;
    return m_sFilePathName.left(pos);
  }

  OdString name() const
  {
    int pos = m_sFilePathName.reverseFind(chPathDelimiter);
    if (pos < 0)
      return m_sFilePathName;
    return m_sFilePathName.mid(++pos);
  }

  OdString baseName() const
  {
    OdString str = name();
    int pos = str.reverseFind(L'.');
    if (pos < 0)
      return str;
    return str.left(pos);
  }

  OdString suffix() const
  {
    OdString str = name();
    int pos = str.reverseFind(L'.');
    if (pos < 0)
      return OdString::kEmpty;
    return str.mid(++pos);
  }

  bool exists(OdInt64* pModificationTime = NULL) const
  {
    bool bRes = ::odrxSystemServices()->accessFile(m_sFilePathName, Oda::kFileRead);
    if (bRes && pModificationTime)
      *pModificationTime = ::odrxSystemServices()->getFileMTime(m_sFilePathName); // return (-1) for non Windows // TODO
    return bRes;
  }

  bool touch() // set current time
  {
#if defined(ODA_WINDOWS) && !defined(_WINRT)
    SYSTEMTIME st;
    ::GetSystemTime(&st); // Gets the current system time
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);  // Converts the current system time to file time format

    //OFSTRUCT of; // OpenFile is deprecated function. Use CreateFile instead.
    //HANDLE hFile = (HANDLE) ::OpenFile(OdString_toUtf8(m_sFilePathName).c_str(),
    //                                   &of, OF_READWRITE | OF_SHARE_DENY_READ);
    HANDLE hFile = ::CreateFile(m_sFilePathName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    bool bRes = (   hFile 
                 && ::SetFileTime(hFile,             // Sets last-write time of the file 
                                  (LPFILETIME) NULL, // to the converted current system time 
                                  (LPFILETIME) NULL, 
                                  &ft));
    ::CloseHandle(hFile);

    return bRes;
#else
    ODA_FAIL_ONCE(); // TODO
    return false;
#endif
  }

  bool existsDir() const
  {
    if (m_sFilePathName.isEmpty())
      return false;
#if defined(_WIN32_WCE) 
    DWORD ftyp = GetFileAttributes(m_sFilePathName.c_str());
    if (ftyp == 0xFFFFFFFF)
      return false;
    return (ftyp & FILE_ATTRIBUTE_DIRECTORY) != 0;
#elif (defined(_MSC_VER) || defined(__BORLANDC__))
    struct _stat statbuf = {0};
    if (_wstat(m_sFilePathName.c_str(), &statbuf))
      return false;
    return (statbuf.st_mode & _S_IFDIR) != 0;
#else
    struct stat statbuf = {0};
    if (stat(OdString_toUtf8(m_sFilePathName).c_str(), &statbuf))
        return false;
    return S_ISDIR(statbuf.st_mode);
#endif
  }

  OdUInt32 length() const
  {
    OdInt64 len64 = ::odrxSystemServices()->getFileSize(m_sFilePathName);
    OdUInt32 mask = 0x7fffffffL;
    ODA_ASSERT_ONCE(len64 <= OdInt64(mask));
    OdUInt32 len = len64 & mask;
    return len;
  }

  bool isWritable() const
  {
    bool bRes = ::odrxSystemServices()->accessFile(m_sFilePathName, Oda::kFileWrite);
    return bRes;
  }

  bool makeDir() const
  {
    if (existsDir())
      return true;
#if defined(ODA_WINDOWS)
    return ::CreateDirectoryW(pathName().c_str(), NULL) != 0; 
#else
    //?  #include <dir.h>
    return !mkdir(OdString_toUtf8(pathName()).c_str(), S_IRWXU | S_IRWXG | S_IRWXO); // S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
  }

  bool remove()
  {
    if (!exists())
      return true;
#if defined(ODA_WINDOWS)
    return ::DeleteFileW(pathName().c_str()) != 0; // winbase.h
    //return ::remove(OdString_toUtf8(pathName()).c_str()) != 0; // io.h
#else
    return !rmdir(OdString_toUtf8(pathName()).c_str()) == 0;
#endif
  }

  bool removeDir()
  {
    if (!existsDir())
      return true;
#if defined(ODA_WINDOWS)
    return ::RemoveDirectoryW(pathName().c_str()) != 0; // winbase.h
    //return ::RemoveDirectory(OdString_toUtf8(pathName()).c_str()) != 0; // io.h
#else
    return unlink(OdString_toUtf8(pathName()).c_str()) == 0;
#endif
  }

  bool rename(const OdString& newPathName)
  {
    ODA_ASSERT_ONCE(exists() && pathName() != newPathName);
#if defined(ODA_WINDOWS) && !defined(_WINRT)
    return ::MoveFileW(pathName().c_str(), newPathName.c_str()) != 0; // winbase.h
    //return ::rename(OdString_toUtf8(pathName()), OdString_toUtf8(newPathName)) != 0; // io.h
#else
    return ::rename(OdString_toUtf8(pathName()).c_str(), OdString_toUtf8(newPathName).c_str()) == 0;
#endif
  }

  bool copyTo(const OdString& copyPathName)
  {
    ODA_ASSERT_ONCE(exists() && pathName() != copyPathName);
    if (pathName() == copyPathName)
      return false;
#if defined(ODA_WINDOWS) && !defined(_WINRT)
    return ::CopyFileW(pathName().c_str(), copyPathName.c_str(), FALSE) != 0; // winbase.h
#else
    std::ifstream inFile(OdString_toUtf8(pathName()).c_str(), std::ios::binary);
    if (!inFile.is_open())
      return false;

    std::ofstream outFile(OdString_toUtf8(copyPathName).c_str(), std::ios::trunc | std::ios::binary);
    if (!outFile.is_open())
      return false;

    outFile << inFile.rdbuf();
    outFile.close();
    return true;
#endif
   }

  bool loadToString(OdAnsiString& sData,
                    int maxFileSize = 200 * 1024 * 1024,
                    OdString* psErrMessage = NULL) const
  {
    sData.empty();
    if (!exists())
      return false;
    OdStreamBufPtr io = ::odrxSystemServices()->createFile(pathName());
    ODA_ASSERT_ONCE(io.get());
    if (!io.get())
      return false;
    //io->seek(0, OdDb::kSeekFromStart);
    OdUInt64 len64 = io->length();
    if (len64 > OdUInt64(maxFileSize))
    {
      if (psErrMessage)
        *psErrMessage = L"Size of view file cache is too large to load.";
      return false;
    }
    OdUInt32 len = len64 & 0xffffffffL;
    if (!len)
    {
      sData.empty();
      return true;
    }
    char* pBuf = sData.getBuffer((int) len + 1);
    io->getBytes(pBuf, len);
    pBuf[len] = '\0';
    sData.releaseBuffer();
    ODA_ASSERT_ONCE(!sData.isEmpty());
    return !sData.isEmpty();
  }

  bool saveToFile(const OdAnsiString& sData,
                  int maxFileSize = 200 * 1024 * 1024,
                  OdString* psErrMessage = NULL) const
  {
    OdInt32 len = sData.getLength();
    if (len > maxFileSize - 1)
    {
      if (psErrMessage)
        *psErrMessage = L"Size of view file cache is too large to save.";
      return false;
    }

    OdStreamBufPtr io = ::odrxSystemServices()->createFile(pathName(),
                                                           Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    ODA_ASSERT_ONCE(io.get());
    if (!io.get())
      return false;

    io->putBytes(sData.c_str(), len);
    return true;
  }

  bool load(OdBinaryData& data,
            int maxFileSize = 50 * 1024 * 1024,
            OdString* psErrMessage = NULL) const
  {
    data.resize(0);
    if (!exists())
      return false;
    OdStreamBufPtr io = ::odrxSystemServices()->createFile(pathName());
    ODA_ASSERT_ONCE(io.get());
    if (!io.get())
      return false;
    //io->seek(0, OdDb::kSeekFromStart);
    OdUInt64 len64 = io->length();
    if (len64 > OdUInt64(maxFileSize))
    {
      if (psErrMessage)
        *psErrMessage = L"Size of view file cache is too large to load.";
      return false;
    }
    OdUInt32 len = len64 & 0xffffffffL;
    data.resize(len);
    io->getBytes(data.asArrayPtr(), len);
    return true;
  }

  bool save(const OdBinaryData& data,
            OdUInt32 maxFileSize = 50 * 1024 * 1024,
            OdString* psErrMessage = NULL) const
  {
    OdUInt32 len = data.size();
    if (len > maxFileSize)
    {
      if (psErrMessage)
        *psErrMessage = L"Size of data file is too large to save.";
      return false;
    }

    OdStreamBufPtr io = ::odrxSystemServices()->createFile(pathName(),
                                                           Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    ODA_ASSERT_ONCE(io.get());
    if (!io.get())
      return false;

    io->putBytes(data.asArrayPtr(), len);
    return true;
  }
};

//////////////////////////////////////////////////////////////////////////
/** \details
  <group ExRender_Classes>
*/
class OdSettings // via Registry for windows, TODO for others 
{
  OdString  m_sPath;
public:
  OdSettings(const OdString& sPath) // like L"HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.0\\ACAD-5001:409\\Profiles\\<<Unnamed Profile>>\\General"
    : m_sPath(sPath)
  {
  }

  static OdString value(const OdString& csPath, const OdString& csName,
                        const OdString& sDefaultValue = OdString::kEmpty);

  OdString value(const OdString& sSubPath,
                 const OdString& sDefaultValue = OdString::kEmpty)
  {
    return value(m_sPath, sSubPath, sDefaultValue);
  }
};

#if !defined(_MSC_VER) || defined(_WINRT)

inline OdString OdSettings::value(const OdString&, const OdString&,
                                  const OdString& sDefaultValue) // = OdString::kEmpty
{
  // TODO 
  return sDefaultValue;
}

#else

#define MAX_PATH_LENGTH 1024

inline HKEY extractHKey(OdString& sPath)
{
  HKEY hKey = HKEY_CURRENT_USER; // default if is absent

  if (sPath.find(L"HKEY_"))
    return hKey;

  OdString sHKey = OdString_extractFirst(sPath, L'\\');
  if (sHKey == L"HKEY_CLASSES_ROOT")
    hKey = HKEY_CLASSES_ROOT;
  else if (sHKey == L"HKEY_LOCAL_MACHINE")
    hKey = HKEY_LOCAL_MACHINE;
  else if (sHKey == L"HKEY_USERS")
    hKey = HKEY_USERS;
#if !defined(_WIN32_WCE)
  else if (sHKey == L"HKEY_CURRENT_CONFIG")
    hKey = HKEY_CURRENT_CONFIG;
#endif
  else 
    ODA_ASSERT_ONCE(sHKey == L"HKEY_CURRENT_USER");
    
  return hKey;
}

//instead GetRegistryString
inline OdString OdSettings::value(const OdString& csPath, const OdString& csName,
                                  const OdString& sDefaultValue) // = OdString::kEmpty
{
  ODA_ASSERT_ONCE(!csPath.isEmpty() && !csName.isEmpty());

  OdString sPath = csPath;
  bool bRes = false;
  HKEY hKey = extractHKey(sPath);
  OdString sData;
    
  if (RegOpenKeyExW(hKey, sPath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    LPBYTE pBuf = (LPBYTE) sData.getBuffer((MAX_PATH_LENGTH / 2) + 2);
    DWORD dwSize = MAX_PATH_LENGTH;
    //unsigned char data[MAX_PATH_LENGTH];
    memset(pBuf, 0x00, MAX_PATH_LENGTH + 1);

    if (RegQueryValueExW(hKey, csName.c_str(), 0, 0, pBuf, &dwSize) == ERROR_SUCCESS)
      bRes = true;
    else if (RegEnumKeyExW(hKey, 0, (LPWSTR)(unsigned short*)pBuf, &dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
      bRes = true;

    sData.releaseBuffer();
    RegCloseKey(hKey);
  }
  if (!bRes)
    return sDefaultValue;

  return sData;
}

#endif


//////////////////////////////////////////////////////////////////////////
// OdSleep

#ifndef WIN32
#include <unistd.h>
#endif

inline void OdSleep(long milliseconds)
{
#if !defined(_WINRT)
#ifdef WIN32
  Sleep(milliseconds);
#else 
#ifdef __MWERKS__
  long seconds = milliseconds / 1000;
  sleep(seconds); 
#else
  // Available on most UNIX systems.
  while (milliseconds >= 1000)
  {
    usleep(999 * 1000);  // usleep requires microseconds, with max of 999999
    milliseconds -= 999;
  }
  usleep(milliseconds * 1000);
#endif
#endif
#endif
}

//////////////////////////////////////////////////////////////////////////

#endif // OD_CLD_TOOLS_H_

