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




#ifndef _ODBINARYDATA_INCLUDED_
#define _ODBINARYDATA_INCLUDED_

#include "TD_PackPush.h"

#include "OdArray.h"

class OdDbDwgFiler;

/** \details
    This class is a specialization of the OdArray class for OdUInt8 (byte) objects.
    Corresponding C++ library: TD_Db
    <group Other_Classes>
*/
class OdBinaryData : public OdArray<OdUInt8, OdMemoryAllocator<OdUInt8> >
{
};


/** \details
    This Class is a specialization of the OdBinaryData class for BitBinary data.

    <group Other_Classes>
*/
class OdBitBinaryData : public OdBinaryData
{
  OdUInt64 m_nBitSize;
public:
  OdBitBinaryData() : m_nBitSize(0) {}

  /** \details
    Returns the size in bits of this BitBinaryData object.
  */
  OdUInt64 getBitSize() const { return m_nBitSize; }

  /** \details
    Resizes this BitBinaryData object to the specified number of bits.
    
    \param bitSize [in]  Bit size.
  */
  void setBitSize(OdUInt64 value)
  {
    m_nBitSize = value;
    OdUInt32 nBytes = (OdUInt32)((m_nBitSize+7)/8);
    ODA_ASSERT(nBytes == (m_nBitSize+7)/8);
    resize(nBytes);
  }

  /** \details
    Sets the size in bits of this BitBinaryData object to be 8 * its current size in bytes.
    
    \param bitSize [in]  Bit size.
  */
  void updateBitSize() {setBitSize( size() * 8 );}
};

#include "TD_PackPop.h"

#endif //_ODBINARYDATA_INCLUDED_

