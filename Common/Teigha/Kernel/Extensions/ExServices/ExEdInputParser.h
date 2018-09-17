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

#ifndef _EXEDINPUTPARSER_H_INCLUDED_
#define _EXEDINPUTPARSER_H_INCLUDED_

#include "TD_PackPush.h"

/** \details
    This class implements InputParser objects.
    \remarks
    Characters will be appended to the result buffer of an InputParser object until either
    
    * A '\r' or '\n' character is encountered. 
    * A ' ' or '\t' charactered is encountered outside a quoted or parenthesized string, and allowSpaces is false.
          
    <group ExServices_Classes> 
    Library: Source code provided.
*/
class ExEdInputParser
{
  OdString  m_res;
  int       m_nParenthesis;
  bool      m_bQuote;
  bool      m_bAllowSpaces;
public:
  ExEdInputParser()
    : m_nParenthesis(0)
    , m_bQuote(false)
  {
    reset(false);
  }

  ~ExEdInputParser()
  {
  }

  /** \details
    Resets the input parser.
    \param allowSpaces [in]  If true, the result will not be terminated by a ' ' or '\t' character.
    \param reserve [in]  Initial size of buffer (ignored).
  */
  void reset(bool allowSpaces, int reserve = 128)
  {
    m_res.empty();
    m_bAllowSpaces = allowSpaces;
  }

  /** \details
    Appends the specified character to the result buffer of this InputParser object.
    
    \remarks
    Returns true if and only if the character has been appended.

    Characters will be appended to the result buffer of an InputParser object until either 
    
    * A '\r' or '\n' character is encountered. 
    * A ' ' or '\t' charactered is encountered outside a quoted or parenthesized string, and allowSpaces is false.

    \param ch [in]  Character to append.
  */
  bool addChar(OdChar ch)
  {
    switch(ch)
    {
    case L'(':
      if(!m_bQuote)
        ++m_nParenthesis;
      break;

    case L')':
      if(!m_bQuote)
        --m_nParenthesis;
      break;

    case L'\"':
      if ((m_bQuote = !m_bQuote) && m_bAllowSpaces && m_res.isEmpty())
        m_bAllowSpaces = false; // #12992
      if (!m_bQuote && !m_res.isEmpty() && m_res.getAt(0) == L'\"')
      {
        m_res = m_res.mid(1); // remove double quotes from result
        return false;
      }
      break;

    case L' ':
    case L'\t':
      if(m_bQuote || m_nParenthesis || m_bAllowSpaces)
      {
        m_res += ch;
        return true;
      }
      //no break
    case L'\r':
    case L'\n':
      return false;
    }
    m_res += ch;
    return true;
  }
  /** \details
    Removes the last character from the result buffer of this InputParser object.
    \remarks
    Returns true if and only if the result buffer is not empty.
  */
  bool eraseChar()
  {
    if(!m_res.isEmpty())
    {
      OdChar ch = m_res.right(1)[0];
      m_res.deleteChars(m_res.getLength()-1, 1);
      switch(ch)
      {
      case L')':
        if(!m_bQuote)
          ++m_nParenthesis;
        break;
        
      case L'(':
        if(!m_bQuote)
          --m_nParenthesis;
        break;
        
      case L'\"':
        m_bQuote = !m_bQuote;
        break;
      }
    }
    return !m_res.isEmpty();
  }
  /** \details
    Returns the result buffer of this InputParser object.
  */
  const OdString& result() const
  {
    if (m_res.getLength() > 1 && m_res[0] == L'^')
    {
      // Support of some macro special symbols (^C ^J ^M) for Cloud.
      // It is more convenient than using current Acad way
      // like  (setq doc (vla-get-ActiveDocument (vlax-get-acad-object))) (vla-SendCommand doc (chr 10))
      switch (m_res[1])
      {
      case L'C':
        throw OdEdCancel();
      case L'J': case L'M': 
        return OdString::kEmpty;
      }
    }
    return m_res;
  }
  /** \details
    Returns true if result buffer of this InputParser object is empty.
  */
  bool isResultEmpty() const
  {
    return m_res.isEmpty();
  }
};

#include "TD_PackPop.h"

#endif //_EXEDINPUTPARSER_H_INCLUDED_
