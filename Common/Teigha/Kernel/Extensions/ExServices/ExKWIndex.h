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

#ifndef _ExKWIndexData_h_Included_
#define _ExKWIndexData_h_Included_

#include "TD_PackPush.h"

#include "OdString.h"
#include "Ed/EdUserIO.h"
#include "OdArray.h"


/** \details
    This class implements keyword index data for ExDbCommandContext objects.

	<group ExServices_Classes> 
*/
class KWIndexData
{
public:
  KWIndexData()
  {}
  OdString m_sKey;   // Key1
  OdString m_sKey2;  // Key2
  OdString m_sKword; // Keyword

  /** \details
      Returns the leftmost n characters of Key1.
  */
  OdString key1(int n) const
  {
    return m_sKey.left(odmin(n,m_sKey.getLength()));
  }
  /** \details
      Returns the leftmost n characters of Key2.
  */
  OdString key2(int n) const
  {
    return m_sKey2.left(odmin(n,m_sKey2.getLength()));
  }
  /** \details
      Returns true if and only if the specified string
      matches the beginning of Key1 or Key2.
      \remarks
      The comparison is case-insensitive. 
      Example:
      If (Key1 == "NORTH") || (Key2 == "SOUTH") then
      match("NOR") == true and match(SOUTHWEST) == false.
  */
  bool match(const OdString& str) const
  {
    return key1(str.getLength()).iCompare(str)==0 || key2(str.getLength()).iCompare(str)==0;
  }
  /** \details
      Returns the keyword for this KWIndexData object.
  */
  OdString keyword() const
  {
    return m_sKword;
  }
};

/** \details
    This class implements a keyword index for ExDbCommandContext objects.

	<group ExServices_Classes>
*/
class KWIndex : public OdArray<KWIndexData> 
{
public:
  /** \param keywords [in]  Keyword list.
  */
  KWIndex(const OdString& keywords);
  /** \details
      Returns the index of the keyword matched by the specfied input.
      \param input [in]  Input to be matched.
      \remarks
      Returns -1 if no match is found. 
  */
  int find(const OdString& input) const;
  /** \details
      Throws an exception if and only if the specified input matches a keyword.
      \param input [in]  Input to be checked.

      Throws:
      ::OdEdKeyword(keywordIndex, Keyword)
  */
  void check(const OdString& input) const;
};

#include "TD_PackPop.h"

#endif // _ExKWIndexData_h_Included_
