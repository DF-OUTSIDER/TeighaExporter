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

#ifndef _DBMANAGERSUBENTDATA_INCLUDED_
#define _DBMANAGERSUBENTDATA_INCLUDED_

#include <OdaCommon.h>
#include "UInt32Array.h"
#include "DoubleArray.h"

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
enum SubentType
{ 
  kUnknown    = 0,
  kFace       = 3,
  kEdge       = 4,
  kFilletPart = 5,
  kFillet     = 6
};

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class SubentDataBase
{
public:
  SubentDataBase() {};
  virtual ~SubentDataBase() {};
  virtual OdResult init(const OdUInt32Array& arr) = 0;
};

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
namespace OdDbHistoryInternal
{

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class PerSubentData
{
  //The number ids in second part.
  //Usually it is 1 and sec number contain sec number in persubent attribute.
  //There are another case.
  //For example 5(m_lNumbIds) 1000000000 1001 1000000000 1002 1000000000 1003 1000000000 1004 1000000000 1
  OdUInt32Array m_lEntTplIdArr;

public:
  PerSubentData()
  {}

  void set(const OdUInt32* pStart, OdUInt32& iShiftArr)
  {
    OdUInt32 iNumbIds  = *(pStart + iShiftArr++);
    if(iNumbIds > 0)
    {
      iNumbIds = iNumbIds*2;
      m_lEntTplIdArr.resize(iNumbIds);
      for (OdUInt32 i = 0; i < iNumbIds; ++i)
      {
        m_lEntTplIdArr.setAt(i, *(pStart + iShiftArr++));
      }
    }
  }

  OdUInt32Array& SubentDataArr() { return m_lEntTplIdArr; }
};
typedef OdArray<PerSubentData> PerSubentDataArray;

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
struct SubentRWData
{
  OdUInt32      m_iType;
  OdUInt32      m_iMark; //0
  OdUInt32Array m_arrIdFirst;
  OdUInt32Array m_arrIdSec;
  OdDoubleArray m_arrDouble;
};
typedef OdArray<SubentRWData> SubentRWDataArray;

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class EdgeData : public SubentDataBase
{
  OdUInt32 m_iStIndex;
  PerSubentData m_mainSubEntity; //The subentity which was used in operation. (For example set color)
  PerSubentDataArray m_arrFaces;
  OdUInt32 m_ivtxIndex1, m_ivtxIndex2;  
  PerSubentData m_vtxSubEntity1, m_vtxSubEntity2; //Vertices of the edge.
  PerSubentDataArray m_arrFacesVtx1, m_arrFacesVtx2;//Arrays of faces which includes the vertex.
  OdUInt32Array m_arrFPart1, m_arrFPart2;
public:
  EdgeData() : m_ivtxIndex1(0), m_ivtxIndex2(0), m_iStIndex(0) {};
  virtual ~EdgeData() {};
  virtual OdResult init(const OdUInt32Array& arr);
};

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class FaceData : public SubentDataBase
{
  PerSubentData m_mainSubEntity; //The subentity which was used in operation. (For example set color)
  PerSubentDataArray m_arrPerSubData;
  OdUInt32Array m_arrLastPart;
public:
  FaceData() {};
  virtual ~FaceData() {};
  virtual OdResult init(const OdUInt32Array& arr);
  OdUInt32& getNFaces();
};

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class FilletSubentData
{
  SubentType m_SubentType;

  OdUInt32 m_iFSize;// The size for first block ???
  SubentDataBase* m_pSubData;
public:
  FilletSubentData() {};
  virtual ~FilletSubentData() 
  {
    if (m_pSubData)
      delete m_pSubData;
  };
  SubentType& getSubentType() {return m_SubentType;};
  OdResult parseSubData(const OdUInt32Array& arr, OdUInt32& iShiftArr);
;};
typedef OdArray<FilletSubentData> FilletSubentDataArray;

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class FilletData : public SubentDataBase
{
  FilletSubentDataArray m_dataArr;
public:
  FilletData() {};
  virtual ~FilletData() {};
  virtual OdResult init(const OdUInt32Array& arr);
};

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class FilletInternalData : public SubentDataBase
{
  PerSubentData m_mainSubEntity; //The subentity which was used in operation. (For example set color)
  PerSubentDataArray m_arrPerSubData;
  OdUInt32Array m_arrLastPart;
public:
  FilletInternalData() {};
  virtual ~FilletInternalData() {};
  virtual OdResult init(const OdUInt32Array& arr);
};

};

/** \details

\sa
TD_Db
<group OdDbSh_Classes> 
*/
struct SubentData
{
  SubentType m_SubentType; // 3 - face, 4 - edge, 6 - chamfer, fillet.
  //First elemet is history element id. (SubentColor, SubEntMaterial, Fillet, chamfer)
  //Array indexes of entities in composite entity. (First number in persubent attribute.)
  //In case face. Array faces and after it array edges.
  OdUInt32Array m_arrMgrEntityId;
  SubentDataBase* m_pSubData;

  SubentData() : m_SubentType(kUnknown), m_pSubData(0)
  {}
  ~SubentData() 
  {
    if (m_pSubData)
    {
      delete m_pSubData;
      m_pSubData   = NULL;
      m_SubentType = kUnknown;
    }
  };
};
typedef OdArray<SubentData> SubentDataArray;

#endif //_DBMANAGERSUBENTDATA_INCLUDED_

