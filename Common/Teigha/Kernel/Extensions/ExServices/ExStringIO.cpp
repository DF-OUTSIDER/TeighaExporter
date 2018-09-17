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

#include "StdAfx.h"
#include "RxObjectImpl.h"
#include "ExStringIO.h"
#include "ExEdInputParser.h"

OdSmartPtr<ExStringIO> ExStringIO::create(const OdString& sInput)
{
  return OdRxObjectImpl<ExStringIO>::createObject()->init(sInput);
}

OdString ExStringIO::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  if(isEof())
    throw OdEdCancel();
  const OdChar* inp = m_sInput;

  ExEdInputParser inpars;
  inpars.reset(GETBIT(options, OdEd::kGstAllowSpaces), odmin(m_sInput.getLength(), 128));
  int n = 0;
  for (; n < m_sInput.getLength(); ++n)
  {
    if (inpars.addChar(inp[n]))
      continue;
    if ((inp[n] == L' ' || inp[n] == L'\t') && inpars.isResultEmpty()) // skip leader spaces
      continue;
    if (inp[n] == L'\"') // via end of string in quotes
    {
      while (inp[n + 1] == L' ' || inp[n + 1] == L'\t')
        ++n;
      if (inp[n + 1] == L'\n')
        ++n;
    }
    ++n;
    break;
  }

  if (m_sInput.getLength() - n)
  {
    m_sInput = m_sInput.right(m_sInput.getLength() - n);
  }
  else
  {
    m_sInput.empty();
  }
  return inpars.result();
}

void ExStringIO::putString(const OdString& )
{
}

void ExStringIO::reset(const OdString& sInput) {
  m_sInput = sInput;
  m_sInput.replace(OD_T("\r\n"),OD_T("\n"));
}

const OdString& ExStringIO::getInitString() const 
{ 
  return m_sInput; 
} 

OdSmartPtr<ExStringIO> ExStringIO::init(const OdString& sInput) {
  reset(sInput);
  return this;
}
