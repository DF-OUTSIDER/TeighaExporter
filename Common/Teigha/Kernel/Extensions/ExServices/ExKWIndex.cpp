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
#include "ExKWIndex.h"

#include <math.h>

KWIndex::KWIndex(const OdString& kwList)
{
  clear();
  int i = 0;
  const OdChar* szKwList = kwList;
  while(szKwList && szKwList[i])
  {
    while(szKwList[i]==' ') // trim
      ++i;
    if(!szKwList[i])
      break;
    if(szKwList[i]=='_')
    {
      ++i;
      break;
    }

    KWIndexData& data = *append();
    int j;
    for(j = i; szKwList[j]!=' ' && szKwList[j]!=',' && szKwList[j]!='\0'; ++j)
    { }
    data.m_sKword =
    data.m_sKey = OdString(szKwList + i, j-i);

    i=j;
    if(szKwList[i]==',')
    {
      ++i;
      while(szKwList[i]==' ') // trim
        ++i;

      int k;
      for (k = i; szKwList[k]!=' ' && szKwList[k]!=',' && szKwList[k]!='\0'; ++k)
      { }
      data.m_sKey2 = OdString(szKwList + i, k-i);
    }
    else
    {
      data.m_sKey2.empty();
    }
  }
  unsigned int kwi = 0;
  while(szKwList && szKwList[i] && kwi < size())
  {
    while(szKwList[i]==' ') // trim
      ++i;
    if(!szKwList[i])
      break;

    KWIndexData& data = at(kwi++);
    int j;
    for(j = i; szKwList[j]!=' ' && szKwList[j]!=',' && szKwList[j]!='\0'; ++j)
    { }
    data.m_sKword = OdString(szKwList + i, j-i);
    i=j;
    if(szKwList[i]==',')
    {
      ++i;
      while(szKwList[i]==' ') // trim
        ++i;
      int k;
      for(k = i; szKwList[k]!=' ' && szKwList[k]!=',' && szKwList[k]!='\0'; ++k)
      { }
      i=k;
    }
  }
}

int KWIndex::find(const OdString& input) const
{
  if(input.isEmpty())
    return -1;

  for(unsigned int i=0; i<size(); ++i)
  {
    const KWIndexData& data = at(i);
    if(data.match(input))
      return i;
  }
  return -1;
}

void KWIndex::check(const OdString& input) const
{
  int n = find(input);
  if(n > -1)
    throw ::OdEdKeyword(n, at(n).keyword());
}
