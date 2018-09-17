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
// Optimized execution of bit flags processing using binary tree

#ifndef _EXTRVISFLAGSBINTREE_INCLUDED_
#define _EXTRVISFLAGSBINTREE_INCLUDED_

#include "TD_PackPush.h"

// 8-bit handlers

struct OdTrVisFlagsBinTreeBase8
{
  inline void bit1() {}
  inline void bit2() {}
  inline void bit3() {}
  inline void bit4() {}
  inline void bit5() {}
  inline void bit6() {}
  inline void bit7() {}
  inline void bit8() {}
};

struct OdTrVisFlagsBinTreeBreakBase8
{
  inline bool bit1() { return true; }
  inline bool bit2() { return true; }
  inline bool bit3() { return true; }
  inline bool bit4() { return true; }
  inline bool bit5() { return true; }
  inline bool bit6() { return true; }
  inline bool bit7() { return true; }
  inline bool bit8() { return true; }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeRedir8 : public RedirBase
{
  inline void bit1() { RedirBase::bit(0); }
  inline void bit2() { RedirBase::bit(1); }
  inline void bit3() { RedirBase::bit(2); }
  inline void bit4() { RedirBase::bit(3); }
  inline void bit5() { RedirBase::bit(4); }
  inline void bit6() { RedirBase::bit(5); }
  inline void bit7() { RedirBase::bit(6); }
  inline void bit8() { RedirBase::bit(7); }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeReadirBreak8 : public RedirBase
{
  inline bool bit1() { return RedirBase::bit(0); }
  inline bool bit2() { return RedirBase::bit(1); }
  inline bool bit3() { return RedirBase::bit(2); }
  inline bool bit4() { return RedirBase::bit(3); }
  inline bool bit5() { return RedirBase::bit(4); }
  inline bool bit6() { return RedirBase::bit(5); }
  inline bool bit7() { return RedirBase::bit(6); }
  inline bool bit8() { return RedirBase::bit(7); }
};

template <typename BitDo>
inline void odTrVisFlagsBinTree(OdUInt8 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x0F)
    {
      if (nMask & 0x03)
      {
        if (nMask & (1 << 0)) { bd.bit1(); }
        if (nMask & (1 << 1)) { bd.bit2(); }
      }
      if (nMask & 0x0C)
      {
        if (nMask & (1 << 2)) { bd.bit3(); }
        if (nMask & (1 << 3)) { bd.bit4(); }
      }
    }
    if (nMask & 0xF0)
    {
      if (nMask & 0x30)
      {
        if (nMask & (1 << 4)) { bd.bit5(); }
        if (nMask & (1 << 5)) { bd.bit6(); }
      }
      if (nMask & 0xC0)
      {
        if (nMask & (1 << 6)) { bd.bit7(); }
        if (nMask & (1 << 7)) { bd.bit8(); }
      }
    }
  }
}

template <typename BitDo>
inline void odTrVisFlagsBinTreeRev(OdUInt8 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xF0)
    {
      if (nMask & 0xC0)
      {
        if (nMask & (1 << 7)) { bd.bit8(); }
        if (nMask & (1 << 6)) { bd.bit7(); }
      }
      if (nMask & 0x30)
      {
        if (nMask & (1 << 5)) { bd.bit6(); }
        if (nMask & (1 << 4)) { bd.bit5(); }
      }
    }
    if (nMask & 0x0F)
    {
      if (nMask & 0x0C)
      {
        if (nMask & (1 << 3)) { bd.bit4(); }
        if (nMask & (1 << 2)) { bd.bit3(); }
      }
      if (nMask & 0x03)
      {
        if (nMask & (1 << 1)) { bd.bit2(); }
        if (nMask & (1 << 0)) { bd.bit1(); }
      }
    }
  }
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreak(OdUInt8 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x0F)
    {
      if (nMask & 0x03)
      {
        if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
        if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
      }
      if (nMask & 0x0C)
      {
        if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
        if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
      }
    }
    if (nMask & 0xF0)
    {
      if (nMask & 0x30)
      {
        if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
        if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
      }
      if (nMask & 0xC0)
      {
        if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
        if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
      }
    }
  }
  return true;
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreakRev(OdUInt8 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xF0)
    {
      if (nMask & 0xC0)
      {
        if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
        if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
      }
      if (nMask & 0x30)
      {
        if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
        if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
      }
    }
    if (nMask & 0x0F)
    {
      if (nMask & 0x0C)
      {
        if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
        if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
      }
      if (nMask & 0x03)
      {
        if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
        if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
      }
    }
  }
  return true;
}

// 16-bit handlers

struct OdTrVisFlagsBinTreeBase16 : public OdTrVisFlagsBinTreeBase8
{
  inline void bit9() {}
  inline void bit10() {}
  inline void bit11() {}
  inline void bit12() {}
  inline void bit13() {}
  inline void bit14() {}
  inline void bit15() {}
  inline void bit16() {}
};

struct OdTrVisFlagsBinTreeBreakBase16 : public OdTrVisFlagsBinTreeBreakBase8
{
  inline bool bit9() { return true; }
  inline bool bit10() { return true; }
  inline bool bit11() { return true; }
  inline bool bit12() { return true; }
  inline bool bit13() { return true; }
  inline bool bit14() { return true; }
  inline bool bit15() { return true; }
  inline bool bit16() { return true; }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeRedir16 : public OdTrVisFlagsBinTreeRedir8<RedirBase>
{
  inline void bit9() { RedirBase::bit(8); }
  inline void bit10() { RedirBase::bit(9); }
  inline void bit11() { RedirBase::bit(10); }
  inline void bit12() { RedirBase::bit(11); }
  inline void bit13() { RedirBase::bit(12); }
  inline void bit14() { RedirBase::bit(13); }
  inline void bit15() { RedirBase::bit(14); }
  inline void bit16() { RedirBase::bit(15); }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeReadirBreak16 : public OdTrVisFlagsBinTreeReadirBreak8<RedirBase>
{
  inline bool bit9() { return RedirBase::bit(8); }
  inline bool bit10() { return RedirBase::bit(9); }
  inline bool bit11() { return RedirBase::bit(10); }
  inline bool bit12() { return RedirBase::bit(11); }
  inline bool bit13() { return RedirBase::bit(12); }
  inline bool bit14() { return RedirBase::bit(13); }
  inline bool bit15() { return RedirBase::bit(14); }
  inline bool bit16() { return RedirBase::bit(15); }
};

template <typename BitDo>
inline void odTrVisFlagsBinTree(OdUInt16 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x00FF)
    {
      if (nMask & 0x000F)
      {
        if (nMask & 0x0003)
        {
          if (nMask & (1 << 0)) { bd.bit1(); }
          if (nMask & (1 << 1)) { bd.bit2(); }
        }
        if (nMask & 0x000C)
        {
          if (nMask & (1 << 2)) { bd.bit3(); }
          if (nMask & (1 << 3)) { bd.bit4(); }
        }
      }
      if (nMask & 0x00F0)
      {
        if (nMask & 0x0030)
        {
          if (nMask & (1 << 4)) { bd.bit5(); }
          if (nMask & (1 << 5)) { bd.bit6(); }
        }
        if (nMask & 0x00C0)
        {
          if (nMask & (1 << 6)) { bd.bit7(); }
          if (nMask & (1 << 7)) { bd.bit8(); }
        }
      }
    }
    if (nMask & 0xFF00)
    {
      if (nMask & 0x0F00)
      {
        if (nMask & 0x0300)
        {
          if (nMask & (1 << 8)) { bd.bit9(); }
          if (nMask & (1 << 9)) { bd.bit10(); }
        }
        if (nMask & 0x0C00)
        {
          if (nMask & (1 << 10)) { bd.bit11(); }
          if (nMask & (1 << 11)) { bd.bit12(); }
        }
      }
      if (nMask & 0xF000)
      {
        if (nMask & 0x3000)
        {
          if (nMask & (1 << 12)) { bd.bit13(); }
          if (nMask & (1 << 13)) { bd.bit14(); }
        }
        if (nMask & 0xC000)
        {
          if (nMask & (1 << 14)) { bd.bit15(); }
          if (nMask & (1 << 15)) { bd.bit16(); }
        }
      }
    }
  }
}

template <typename BitDo>
inline void odTrVisFlagsBinTreeRev(OdUInt16 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xFF00)
    {
      if (nMask & 0xF000)
      {
        if (nMask & 0xC000)
        {
          if (nMask & (1 << 15)) { bd.bit16(); }
          if (nMask & (1 << 14)) { bd.bit15(); }
        }
        if (nMask & 0x3000)
        {
          if (nMask & (1 << 13)) { bd.bit14(); }
          if (nMask & (1 << 12)) { bd.bit13(); }
        }
      }
      if (nMask & 0x0F00)
      {
        if (nMask & 0x0C00)
        {
          if (nMask & (1 << 11)) { bd.bit12(); }
          if (nMask & (1 << 10)) { bd.bit11(); }
        }
        if (nMask & 0x0300)
        {
          if (nMask & (1 << 9)) { bd.bit10(); }
          if (nMask & (1 << 8)) { bd.bit9(); }
        }
      }
    }
    if (nMask & 0x00FF)
    {
      if (nMask & 0x00F0)
      {
        if (nMask & 0x00C0)
        {
          if (nMask & (1 << 7)) { bd.bit8(); }
          if (nMask & (1 << 6)) { bd.bit7(); }
        }
        if (nMask & 0x0030)
        {
          if (nMask & (1 << 5)) { bd.bit6(); }
          if (nMask & (1 << 4)) { bd.bit5(); }
        }
      }
      if (nMask & 0x000F)
      {
        if (nMask & 0x000C)
        {
          if (nMask & (1 << 3)) { bd.bit4(); }
          if (nMask & (1 << 2)) { bd.bit3(); }
        }
        if (nMask & 0x0003)
        {
          if (nMask & (1 << 1)) { bd.bit2(); }
          if (nMask & (1 << 0)) { bd.bit1(); }
        }
      }
    }
  }
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreak(OdUInt16 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x00FF)
    {
      if (nMask & 0x000F)
      {
        if (nMask & 0x0003)
        {
          if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
          if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
        }
        if (nMask & 0x000C)
        {
          if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
          if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
        }
      }
      if (nMask & 0x00F0)
      {
        if (nMask & 0x0030)
        {
          if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
          if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
        }
        if (nMask & 0x00C0)
        {
          if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
          if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
        }
      }
    }
    if (nMask & 0xFF00)
    {
      if (nMask & 0x0F00)
      {
        if (nMask & 0x0300)
        {
          if (nMask & (1 << 8)) { if (!bd.bit9()) return false; }
          if (nMask & (1 << 9)) { if (!bd.bit10()) return false; }
        }
        if (nMask & 0x0C00)
        {
          if (nMask & (1 << 10)) { if (!bd.bit11()) return false; }
          if (nMask & (1 << 11)) { if (!bd.bit12()) return false; }
        }
      }
      if (nMask & 0xF000)
      {
        if (nMask & 0x3000)
        {
          if (nMask & (1 << 12)) { if (!bd.bit13()) return false; }
          if (nMask & (1 << 13)) { if (!bd.bit14()) return false; }
        }
        if (nMask & 0xC000)
        {
          if (nMask & (1 << 14)) { if (!bd.bit15()) return false; }
          if (nMask & (1 << 15)) { if (!bd.bit16()) return false; }
        }
      }
    }
  }
  return true;
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreakRev(OdUInt16 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xFF00)
    {
      if (nMask & 0xF000)
      {
        if (nMask & 0xC000)
        {
          if (nMask & (1 << 15)) { if (!bd.bit16()) return false; }
          if (nMask & (1 << 14)) { if (!bd.bit15()) return false; }
        }
        if (nMask & 0x3000)
        {
          if (nMask & (1 << 13)) { if (!bd.bit14()) return false; }
          if (nMask & (1 << 12)) { if (!bd.bit13()) return false; }
        }
      }
      if (nMask & 0x0F00)
      {
        if (nMask & 0x0C00)
        {
          if (nMask & (1 << 11)) { if (!bd.bit12()) return false; }
          if (nMask & (1 << 10)) { if (!bd.bit11()) return false; }
        }
        if (nMask & 0x0300)
        {
          if (nMask & (1 << 9)) { if (!bd.bit10()) return false; }
          if (nMask & (1 << 8)) { if (!bd.bit9()) return false; }
        }
      }
    }
    if (nMask & 0x00FF)
    {
      if (nMask & 0x00F0)
      {
        if (nMask & 0x00C0)
        {
          if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
          if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
        }
        if (nMask & 0x0030)
        {
          if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
          if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
        }
      }
      if (nMask & 0x000F)
      {
        if (nMask & 0x000C)
        {
          if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
          if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
        }
        if (nMask & 0x0003)
        {
          if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
          if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
        }
      }
    }
  }
  return true;
}

// 32-bit handlers

struct OdTrVisFlagsBinTreeBase32 : public OdTrVisFlagsBinTreeBase16
{
  inline void bit17() {}
  inline void bit18() {}
  inline void bit19() {}
  inline void bit20() {}
  inline void bit21() {}
  inline void bit22() {}
  inline void bit23() {}
  inline void bit24() {}
  inline void bit25() {}
  inline void bit26() {}
  inline void bit27() {}
  inline void bit28() {}
  inline void bit29() {}
  inline void bit30() {}
  inline void bit31() {}
  inline void bit32() {}
};

struct OdTrVisFlagsBinTreeBreakBase32 : public OdTrVisFlagsBinTreeBreakBase16
{
  inline bool bit17() { return true; }
  inline bool bit18() { return true; }
  inline bool bit19() { return true; }
  inline bool bit20() { return true; }
  inline bool bit21() { return true; }
  inline bool bit22() { return true; }
  inline bool bit23() { return true; }
  inline bool bit24() { return true; }
  inline bool bit25() { return true; }
  inline bool bit26() { return true; }
  inline bool bit27() { return true; }
  inline bool bit28() { return true; }
  inline bool bit29() { return true; }
  inline bool bit30() { return true; }
  inline bool bit31() { return true; }
  inline bool bit32() { return true; }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeRedir32 : public OdTrVisFlagsBinTreeRedir16<RedirBase>
{
  inline void bit17() { RedirBase::bit(16); }
  inline void bit18() { RedirBase::bit(17); }
  inline void bit19() { RedirBase::bit(18); }
  inline void bit20() { RedirBase::bit(19); }
  inline void bit21() { RedirBase::bit(20); }
  inline void bit22() { RedirBase::bit(21); }
  inline void bit23() { RedirBase::bit(22); }
  inline void bit24() { RedirBase::bit(23); }
  inline void bit25() { RedirBase::bit(24); }
  inline void bit26() { RedirBase::bit(25); }
  inline void bit27() { RedirBase::bit(26); }
  inline void bit28() { RedirBase::bit(27); }
  inline void bit29() { RedirBase::bit(28); }
  inline void bit30() { RedirBase::bit(29); }
  inline void bit31() { RedirBase::bit(30); }
  inline void bit32() { RedirBase::bit(31); }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeReadirBreak32 : public OdTrVisFlagsBinTreeReadirBreak16<RedirBase>
{
  inline bool bit17() { return RedirBase::bit(16); }
  inline bool bit18() { return RedirBase::bit(17); }
  inline bool bit19() { return RedirBase::bit(18); }
  inline bool bit20() { return RedirBase::bit(19); }
  inline bool bit21() { return RedirBase::bit(20); }
  inline bool bit22() { return RedirBase::bit(21); }
  inline bool bit23() { return RedirBase::bit(22); }
  inline bool bit24() { return RedirBase::bit(23); }
  inline bool bit25() { return RedirBase::bit(24); }
  inline bool bit26() { return RedirBase::bit(25); }
  inline bool bit27() { return RedirBase::bit(26); }
  inline bool bit28() { return RedirBase::bit(27); }
  inline bool bit29() { return RedirBase::bit(28); }
  inline bool bit30() { return RedirBase::bit(29); }
  inline bool bit31() { return RedirBase::bit(30); }
  inline bool bit32() { return RedirBase::bit(31); }
};

template <typename BitDo>
inline void odTrVisFlagsBinTree(OdUInt32 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x0000FFFF)
    {
      if (nMask & 0x000000FF)
      {
        if (nMask & 0x0000000F)
        {
          if (nMask & 0x00000003)
          {
            if (nMask & (1 << 0)) { bd.bit1(); }
            if (nMask & (1 << 1)) { bd.bit2(); }
          }
          if (nMask & 0x0000000C)
          {
            if (nMask & (1 << 2)) { bd.bit3(); }
            if (nMask & (1 << 3)) { bd.bit4(); }
          }
        }
        if (nMask & 0x000000F0)
        {
          if (nMask & 0x00000030)
          {
            if (nMask & (1 << 4)) { bd.bit5(); }
            if (nMask & (1 << 5)) { bd.bit6(); }
          }
          if (nMask & 0x000000C0)
          {
            if (nMask & (1 << 6)) { bd.bit7(); }
            if (nMask & (1 << 7)) { bd.bit8(); }
          }
        }
      }
      if (nMask & 0x0000FF00)
      {
        if (nMask & 0x00000F00)
        {
          if (nMask & 0x00000300)
          {
            if (nMask & (1 << 8)) { bd.bit9(); }
            if (nMask & (1 << 9)) { bd.bit10(); }
          }
          if (nMask & 0x00000C00)
          {
            if (nMask & (1 << 10)) { bd.bit11(); }
            if (nMask & (1 << 11)) { bd.bit12(); }
          }
        }
        if (nMask & 0x0000F000)
        {
          if (nMask & 0x00003000)
          {
            if (nMask & (1 << 12)) { bd.bit13(); }
            if (nMask & (1 << 13)) { bd.bit14(); }
          }
          if (nMask & 0x0000C000)
          {
            if (nMask & (1 << 14)) { bd.bit15(); }
            if (nMask & (1 << 15)) { bd.bit16(); }
          }
        }
      }
    }
    if (nMask & 0xFFFF0000)
    {
      if (nMask & 0x00FF0000)
      {
        if (nMask & 0x000F0000)
        {
          if (nMask & 0x00030000)
          {
            if (nMask & (1 << 16)) { bd.bit17(); }
            if (nMask & (1 << 17)) { bd.bit18(); }
          }
          if (nMask & 0x000C0000)
          {
            if (nMask & (1 << 18)) { bd.bit19(); }
            if (nMask & (1 << 19)) { bd.bit20(); }
          }
        }
        if (nMask & 0x00F00000)
        {
          if (nMask & 0x00300000)
          {
            if (nMask & (1 << 20)) { bd.bit21(); }
            if (nMask & (1 << 21)) { bd.bit22(); }
          }
          if (nMask & 0x00C00000)
          {
            if (nMask & (1 << 22)) { bd.bit23(); }
            if (nMask & (1 << 23)) { bd.bit24(); }
          }
        }
      }
      if (nMask & 0xFF000000)
      {
        if (nMask & 0x0F000000)
        {
          if (nMask & 0x03000000)
          {
            if (nMask & (1 << 24)) { bd.bit25(); }
            if (nMask & (1 << 25)) { bd.bit26(); }
          }
          if (nMask & 0x0C000000)
          {
            if (nMask & (1 << 26)) { bd.bit27(); }
            if (nMask & (1 << 27)) { bd.bit28(); }
          }
        }
        if (nMask & 0xF0000000)
        {
          if (nMask & 0x30000000)
          {
            if (nMask & (1 << 28)) { bd.bit29(); }
            if (nMask & (1 << 29)) { bd.bit30(); }
          }
          if (nMask & 0xC0000000)
          {
            if (nMask & (1 << 30)) { bd.bit31(); }
            if (nMask & (1 << 31)) { bd.bit32(); }
          }
        }
      }
    }
  }
}

template <typename BitDo>
inline void odTrVisFlagsBinTreeRev(OdUInt32 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xFFFF0000)
    {
      if (nMask & 0xFF000000)
      {
        if (nMask & 0xF0000000)
        {
          if (nMask & 0xC0000000)
          {
            if (nMask & (1 << 31)) { bd.bit32(); }
            if (nMask & (1 << 30)) { bd.bit31(); }
          }
          if (nMask & 0x30000000)
          {
            if (nMask & (1 << 29)) { bd.bit30(); }
            if (nMask & (1 << 28)) { bd.bit29(); }
          }
        }
        if (nMask & 0x0F000000)
        {
          if (nMask & 0x0C000000)
          {
            if (nMask & (1 << 27)) { bd.bit28(); }
            if (nMask & (1 << 26)) { bd.bit27(); }
          }
          if (nMask & 0x03000000)
          {
            if (nMask & (1 << 25)) { bd.bit26(); }
            if (nMask & (1 << 24)) { bd.bit25(); }
          }
        }
      }
      if (nMask & 0x00FF0000)
      {
        if (nMask & 0x00F00000)
        {
          if (nMask & 0x00C00000)
          {
            if (nMask & (1 << 23)) { bd.bit24(); }
            if (nMask & (1 << 22)) { bd.bit23(); }
          }
          if (nMask & 0x00300000)
          {
            if (nMask & (1 << 21)) { bd.bit22(); }
            if (nMask & (1 << 20)) { bd.bit21(); }
          }
        }
        if (nMask & 0x000F0000)
        {
          if (nMask & 0x000C0000)
          {
            if (nMask & (1 << 19)) { bd.bit20(); }
            if (nMask & (1 << 18)) { bd.bit19(); }
          }
          if (nMask & 0x00030000)
          {
            if (nMask & (1 << 17)) { bd.bit18(); }
            if (nMask & (1 << 16)) { bd.bit17(); }
          }
        }
      }
    }
    if (nMask & 0x0000FFFF)
    {
      if (nMask & 0x0000FF00)
      {
        if (nMask & 0x0000F000)
        {
          if (nMask & 0x0000C000)
          {
            if (nMask & (1 << 15)) { bd.bit16(); }
            if (nMask & (1 << 14)) { bd.bit15(); }
          }
          if (nMask & 0x00003000)
          {
            if (nMask & (1 << 13)) { bd.bit14(); }
            if (nMask & (1 << 12)) { bd.bit13(); }
          }
        }
        if (nMask & 0x00000F00)
        {
          if (nMask & 0x00000C00)
          {
            if (nMask & (1 << 11)) { bd.bit12(); }
            if (nMask & (1 << 10)) { bd.bit11(); }
          }
          if (nMask & 0x00000300)
          {
            if (nMask & (1 << 9)) { bd.bit10(); }
            if (nMask & (1 << 8)) { bd.bit9(); }
          }
        }
      }
      if (nMask & 0x000000FF)
      {
        if (nMask & 0x000000F0)
        {
          if (nMask & 0x000000C0)
          {
            if (nMask & (1 << 7)) { bd.bit8(); }
            if (nMask & (1 << 6)) { bd.bit7(); }
          }
          if (nMask & 0x00000030)
          {
            if (nMask & (1 << 5)) { bd.bit6(); }
            if (nMask & (1 << 4)) { bd.bit5(); }
          }
        }
        if (nMask & 0x0000000F)
        {
          if (nMask & 0x0000000C)
          {
            if (nMask & (1 << 3)) { bd.bit4(); }
            if (nMask & (1 << 2)) { bd.bit3(); }
          }
          if (nMask & 0x00000003)
          {
            if (nMask & (1 << 1)) { bd.bit2(); }
            if (nMask & (1 << 0)) { bd.bit1(); }
          }
        }
      }
    }
  }
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreak(OdUInt32 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0x0000FFFF)
    {
      if (nMask & 0x000000FF)
      {
        if (nMask & 0x0000000F)
        {
          if (nMask & 0x00000003)
          {
            if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
            if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
          }
          if (nMask & 0x0000000C)
          {
            if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
            if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
          }
        }
        if (nMask & 0x000000F0)
        {
          if (nMask & 0x00000030)
          {
            if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
            if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
          }
          if (nMask & 0x000000C0)
          {
            if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
            if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
          }
        }
      }
      if (nMask & 0x0000FF00)
      {
        if (nMask & 0x00000F00)
        {
          if (nMask & 0x00000300)
          {
            if (nMask & (1 << 8)) { if (!bd.bit9()) return false; }
            if (nMask & (1 << 9)) { if (!bd.bit10()) return false; }
          }
          if (nMask & 0x00000C00)
          {
            if (nMask & (1 << 10)) { if (!bd.bit11()) return false; }
            if (nMask & (1 << 11)) { if (!bd.bit12()) return false; }
          }
        }
        if (nMask & 0x0000F000)
        {
          if (nMask & 0x00003000)
          {
            if (nMask & (1 << 12)) { if (!bd.bit13()) return false; }
            if (nMask & (1 << 13)) { if (!bd.bit14()) return false; }
          }
          if (nMask & 0x0000C000)
          {
            if (nMask & (1 << 14)) { if (!bd.bit15()) return false; }
            if (nMask & (1 << 15)) { if (!bd.bit16()) return false; }
          }
        }
      }
    }
    if (nMask & 0xFFFF0000)
    {
      if (nMask & 0x00FF0000)
      {
        if (nMask & 0x000F0000)
        {
          if (nMask & 0x00030000)
          {
            if (nMask & (1 << 16)) { if (!bd.bit17()) return false; }
            if (nMask & (1 << 17)) { if (!bd.bit18()) return false; }
          }
          if (nMask & 0x000C0000)
          {
            if (nMask & (1 << 18)) { if (!bd.bit19()) return false; }
            if (nMask & (1 << 19)) { if (!bd.bit20()) return false; }
          }
        }
        if (nMask & 0x00F00000)
        {
          if (nMask & 0x00300000)
          {
            if (nMask & (1 << 20)) { if (!bd.bit21()) return false; }
            if (nMask & (1 << 21)) { if (!bd.bit22()) return false; }
          }
          if (nMask & 0x00C00000)
          {
            if (nMask & (1 << 22)) { if (!bd.bit23()) return false; }
            if (nMask & (1 << 23)) { if (!bd.bit24()) return false; }
          }
        }
      }
      if (nMask & 0xFF000000)
      {
        if (nMask & 0x0F000000)
        {
          if (nMask & 0x03000000)
          {
            if (nMask & (1 << 24)) { if (!bd.bit25()) return false; }
            if (nMask & (1 << 25)) { if (!bd.bit26()) return false; }
          }
          if (nMask & 0x0C000000)
          {
            if (nMask & (1 << 26)) { if (!bd.bit27()) return false; }
            if (nMask & (1 << 27)) { if (!bd.bit28()) return false; }
          }
        }
        if (nMask & 0xF0000000)
        {
          if (nMask & 0x30000000)
          {
            if (nMask & (1 << 28)) { if (!bd.bit29()) return false; }
            if (nMask & (1 << 29)) { if (!bd.bit30()) return false; }
          }
          if (nMask & 0xC0000000)
          {
            if (nMask & (1 << 30)) { if (!bd.bit31()) return false; }
            if (nMask & (1 << 31)) { if (!bd.bit32()) return false; }
          }
        }
      }
    }
  }
  return true;
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreakRev(OdUInt32 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (nMask & 0xFFFF0000)
    {
      if (nMask & 0xFF000000)
      {
        if (nMask & 0xF0000000)
        {
          if (nMask & 0xC0000000)
          {
            if (nMask & (1 << 31)) { if (!bd.bit32()) return false; }
            if (nMask & (1 << 30)) { if (!bd.bit31()) return false; }
          }
          if (nMask & 0x30000000)
          {
            if (nMask & (1 << 29)) { if (!bd.bit30()) return false; }
            if (nMask & (1 << 28)) { if (!bd.bit29()) return false; }
          }
        }
        if (nMask & 0x0F000000)
        {
          if (nMask & 0x0C000000)
          {
            if (nMask & (1 << 27)) { if (!bd.bit28()) return false; }
            if (nMask & (1 << 26)) { if (!bd.bit27()) return false; }
          }
          if (nMask & 0x03000000)
          {
            if (nMask & (1 << 25)) { if (!bd.bit26()) return false; }
            if (nMask & (1 << 24)) { if (!bd.bit25()) return false; }
          }
        }
      }
      if (nMask & 0x00FF0000)
      {
        if (nMask & 0x00F00000)
        {
          if (nMask & 0x00C00000)
          {
            if (nMask & (1 << 23)) { if (!bd.bit24()) return false; }
            if (nMask & (1 << 22)) { if (!bd.bit23()) return false; }
          }
          if (nMask & 0x00300000)
          {
            if (nMask & (1 << 21)) { if (!bd.bit22()) return false; }
            if (nMask & (1 << 20)) { if (!bd.bit21()) return false; }
          }
        }
        if (nMask & 0x000F0000)
        {
          if (nMask & 0x000C0000)
          {
            if (nMask & (1 << 19)) { if (!bd.bit20()) return false; }
            if (nMask & (1 << 18)) { if (!bd.bit19()) return false; }
          }
          if (nMask & 0x00030000)
          {
            if (nMask & (1 << 17)) { if (!bd.bit18()) return false; }
            if (nMask & (1 << 16)) { if (!bd.bit17()) return false; }
          }
        }
      }
    }
    if (nMask & 0x0000FFFF)
    {
      if (nMask & 0x0000FF00)
      {
        if (nMask & 0x0000F000)
        {
          if (nMask & 0x0000C000)
          {
            if (nMask & (1 << 15)) { if (!bd.bit16()) return false; }
            if (nMask & (1 << 14)) { if (!bd.bit15()) return false; }
          }
          if (nMask & 0x00003000)
          {
            if (nMask & (1 << 13)) { if (!bd.bit14()) return false; }
            if (nMask & (1 << 12)) { if (!bd.bit13()) return false; }
          }
        }
        if (nMask & 0x00000F00)
        {
          if (nMask & 0x00000C00)
          {
            if (nMask & (1 << 11)) { if (!bd.bit12()) return false; }
            if (nMask & (1 << 10)) { if (!bd.bit11()) return false; }
          }
          if (nMask & 0x00000300)
          {
            if (nMask & (1 << 9)) { if (!bd.bit10()) return false; }
            if (nMask & (1 << 8)) { if (!bd.bit9()) return false; }
          }
        }
      }
      if (nMask & 0x000000FF)
      {
        if (nMask & 0x000000F0)
        {
          if (nMask & 0x000000C0)
          {
            if (nMask & (1 << 7)) { if (!bd.bit8()) return false; }
            if (nMask & (1 << 6)) { if (!bd.bit7()) return false; }
          }
          if (nMask & 0x00000030)
          {
            if (nMask & (1 << 5)) { if (!bd.bit6()) return false; }
            if (nMask & (1 << 4)) { if (!bd.bit5()) return false; }
          }
        }
        if (nMask & 0x0000000F)
        {
          if (nMask & 0x0000000C)
          {
            if (nMask & (1 << 3)) { if (!bd.bit4()) return false; }
            if (nMask & (1 << 2)) { if (!bd.bit3()) return false; }
          }
          if (nMask & 0x00000003)
          {
            if (nMask & (1 << 1)) { if (!bd.bit2()) return false; }
            if (nMask & (1 << 0)) { if (!bd.bit1()) return false; }
          }
        }
      }
    }
  }
  return true;
}

// 64-bit handlers

struct OdTrVisFlagsBinTreeBase64 : public OdTrVisFlagsBinTreeBase32
{
  inline void bit33() {}
  inline void bit34() {}
  inline void bit35() {}
  inline void bit36() {}
  inline void bit37() {}
  inline void bit38() {}
  inline void bit39() {}
  inline void bit40() {}
  inline void bit41() {}
  inline void bit42() {}
  inline void bit43() {}
  inline void bit44() {}
  inline void bit45() {}
  inline void bit46() {}
  inline void bit47() {}
  inline void bit48() {}
  inline void bit49() {}
  inline void bit50() {}
  inline void bit51() {}
  inline void bit52() {}
  inline void bit53() {}
  inline void bit54() {}
  inline void bit55() {}
  inline void bit56() {}
  inline void bit57() {}
  inline void bit58() {}
  inline void bit59() {}
  inline void bit60() {}
  inline void bit61() {}
  inline void bit62() {}
  inline void bit63() {}
  inline void bit64() {}
};

struct OdTrVisFlagsBinTreeBreakBase64 : public OdTrVisFlagsBinTreeBreakBase32
{
  inline bool bit33() { return true; }
  inline bool bit34() { return true; }
  inline bool bit35() { return true; }
  inline bool bit36() { return true; }
  inline bool bit37() { return true; }
  inline bool bit38() { return true; }
  inline bool bit39() { return true; }
  inline bool bit40() { return true; }
  inline bool bit41() { return true; }
  inline bool bit42() { return true; }
  inline bool bit43() { return true; }
  inline bool bit44() { return true; }
  inline bool bit45() { return true; }
  inline bool bit46() { return true; }
  inline bool bit47() { return true; }
  inline bool bit48() { return true; }
  inline bool bit49() { return true; }
  inline bool bit50() { return true; }
  inline bool bit51() { return true; }
  inline bool bit52() { return true; }
  inline bool bit53() { return true; }
  inline bool bit54() { return true; }
  inline bool bit55() { return true; }
  inline bool bit56() { return true; }
  inline bool bit57() { return true; }
  inline bool bit58() { return true; }
  inline bool bit59() { return true; }
  inline bool bit60() { return true; }
  inline bool bit61() { return true; }
  inline bool bit62() { return true; }
  inline bool bit63() { return true; }
  inline bool bit64() { return true; }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeRedir64 : public OdTrVisFlagsBinTreeRedir32<RedirBase>
{
  inline void bit33() { RedirBase::bit(32); }
  inline void bit34() { RedirBase::bit(33); }
  inline void bit35() { RedirBase::bit(34); }
  inline void bit36() { RedirBase::bit(35); }
  inline void bit37() { RedirBase::bit(36); }
  inline void bit38() { RedirBase::bit(37); }
  inline void bit39() { RedirBase::bit(38); }
  inline void bit40() { RedirBase::bit(39); }
  inline void bit41() { RedirBase::bit(40); }
  inline void bit42() { RedirBase::bit(41); }
  inline void bit43() { RedirBase::bit(42); }
  inline void bit44() { RedirBase::bit(43); }
  inline void bit45() { RedirBase::bit(44); }
  inline void bit46() { RedirBase::bit(45); }
  inline void bit47() { RedirBase::bit(46); }
  inline void bit48() { RedirBase::bit(47); }
  inline void bit49() { RedirBase::bit(48); }
  inline void bit50() { RedirBase::bit(49); }
  inline void bit51() { RedirBase::bit(50); }
  inline void bit52() { RedirBase::bit(51); }
  inline void bit53() { RedirBase::bit(52); }
  inline void bit54() { RedirBase::bit(53); }
  inline void bit55() { RedirBase::bit(54); }
  inline void bit56() { RedirBase::bit(55); }
  inline void bit57() { RedirBase::bit(56); }
  inline void bit58() { RedirBase::bit(57); }
  inline void bit59() { RedirBase::bit(58); }
  inline void bit60() { RedirBase::bit(59); }
  inline void bit61() { RedirBase::bit(60); }
  inline void bit62() { RedirBase::bit(61); }
  inline void bit63() { RedirBase::bit(62); }
  inline void bit64() { RedirBase::bit(63); }
};

template <typename RedirBase>
struct OdTrVisFlagsBinTreeReadirBreak64 : public OdTrVisFlagsBinTreeReadirBreak32<RedirBase>
{
  inline bool bit33() { return RedirBase::bit(32); }
  inline bool bit34() { return RedirBase::bit(33); }
  inline bool bit35() { return RedirBase::bit(34); }
  inline bool bit36() { return RedirBase::bit(35); }
  inline bool bit37() { return RedirBase::bit(36); }
  inline bool bit38() { return RedirBase::bit(37); }
  inline bool bit39() { return RedirBase::bit(38); }
  inline bool bit40() { return RedirBase::bit(39); }
  inline bool bit41() { return RedirBase::bit(40); }
  inline bool bit42() { return RedirBase::bit(41); }
  inline bool bit43() { return RedirBase::bit(42); }
  inline bool bit44() { return RedirBase::bit(43); }
  inline bool bit45() { return RedirBase::bit(44); }
  inline bool bit46() { return RedirBase::bit(45); }
  inline bool bit47() { return RedirBase::bit(46); }
  inline bool bit48() { return RedirBase::bit(47); }
  inline bool bit49() { return RedirBase::bit(48); }
  inline bool bit50() { return RedirBase::bit(49); }
  inline bool bit51() { return RedirBase::bit(50); }
  inline bool bit52() { return RedirBase::bit(51); }
  inline bool bit53() { return RedirBase::bit(52); }
  inline bool bit54() { return RedirBase::bit(53); }
  inline bool bit55() { return RedirBase::bit(54); }
  inline bool bit56() { return RedirBase::bit(55); }
  inline bool bit57() { return RedirBase::bit(56); }
  inline bool bit58() { return RedirBase::bit(57); }
  inline bool bit59() { return RedirBase::bit(58); }
  inline bool bit60() { return RedirBase::bit(59); }
  inline bool bit61() { return RedirBase::bit(60); }
  inline bool bit62() { return RedirBase::bit(61); }
  inline bool bit63() { return RedirBase::bit(62); }
  inline bool bit64() { return RedirBase::bit(63); }
};

template <typename BitDo> /* internal */
struct OdTrVisFlagsBinTree64_UpperBitsWrap
{
  BitDo &outBits;
  OdTrVisFlagsBinTree64_UpperBitsWrap(BitDo &_outBits) : outBits(_outBits) { }
  inline void bit1() { outBits.bit33(); }
  inline void bit2() { outBits.bit34(); }
  inline void bit3() { outBits.bit35(); }
  inline void bit4() { outBits.bit36(); }
  inline void bit5() { outBits.bit37(); }
  inline void bit6() { outBits.bit38(); }
  inline void bit7() { outBits.bit39(); }
  inline void bit8() { outBits.bit40(); }
  inline void bit9() { outBits.bit41(); }
  inline void bit10() { outBits.bit42(); }
  inline void bit11() { outBits.bit43(); }
  inline void bit12() { outBits.bit44(); }
  inline void bit13() { outBits.bit45(); }
  inline void bit14() { outBits.bit46(); }
  inline void bit15() { outBits.bit47(); }
  inline void bit16() { outBits.bit48(); }
  inline void bit17() { outBits.bit49(); }
  inline void bit18() { outBits.bit50(); }
  inline void bit19() { outBits.bit51(); }
  inline void bit20() { outBits.bit52(); }
  inline void bit21() { outBits.bit53(); }
  inline void bit22() { outBits.bit54(); }
  inline void bit23() { outBits.bit55(); }
  inline void bit24() { outBits.bit56(); }
  inline void bit25() { outBits.bit57(); }
  inline void bit26() { outBits.bit58(); }
  inline void bit27() { outBits.bit59(); }
  inline void bit28() { outBits.bit60(); }
  inline void bit29() { outBits.bit61(); }
  inline void bit30() { outBits.bit62(); }
  inline void bit31() { outBits.bit63(); }
  inline void bit32() { outBits.bit64(); }
};

template <typename BitDo> /* internal */
struct OdTrVisFlagsBinTreeBreak64_UpperBitsWrap
{
  BitDo &outBits;
  OdTrVisFlagsBinTreeBreak64_UpperBitsWrap(BitDo &_outBits) : outBits(_outBits) { }
  inline bool bit1() { return outBits.bit33(); }
  inline bool bit2() { return outBits.bit34(); }
  inline bool bit3() { return outBits.bit35(); }
  inline bool bit4() { return outBits.bit36(); }
  inline bool bit5() { return outBits.bit37(); }
  inline bool bit6() { return outBits.bit38(); }
  inline bool bit7() { return outBits.bit39(); }
  inline bool bit8() { return outBits.bit40(); }
  inline bool bit9() { return outBits.bit41(); }
  inline bool bit10() { return outBits.bit42(); }
  inline bool bit11() { return outBits.bit43(); }
  inline bool bit12() { return outBits.bit44(); }
  inline bool bit13() { return outBits.bit45(); }
  inline bool bit14() { return outBits.bit46(); }
  inline bool bit15() { return outBits.bit47(); }
  inline bool bit16() { return outBits.bit48(); }
  inline bool bit17() { return outBits.bit49(); }
  inline bool bit18() { return outBits.bit50(); }
  inline bool bit19() { return outBits.bit51(); }
  inline bool bit20() { return outBits.bit52(); }
  inline bool bit21() { return outBits.bit53(); }
  inline bool bit22() { return outBits.bit54(); }
  inline bool bit23() { return outBits.bit55(); }
  inline bool bit24() { return outBits.bit56(); }
  inline bool bit25() { return outBits.bit57(); }
  inline bool bit26() { return outBits.bit58(); }
  inline bool bit27() { return outBits.bit59(); }
  inline bool bit28() { return outBits.bit60(); }
  inline bool bit29() { return outBits.bit61(); }
  inline bool bit30() { return outBits.bit62(); }
  inline bool bit31() { return outBits.bit63(); }
  inline bool bit32() { return outBits.bit64(); }
};

template <typename BitDo>
inline void odTrVisFlagsBinTree(OdUInt64 nMask, BitDo &bd)
{
  if (nMask)
  {
    ::odTrVisFlagsBinTree((OdUInt32)nMask, bd);
    OdTrVisFlagsBinTree64_UpperBitsWrap<BitDo> _upper_bits_wrap(bd);
    ::odTrVisFlagsBinTree((OdUInt32)(nMask >> 32), _upper_bits_wrap);
  }
}

template <typename BitDo>
inline void odTrVisFlagsBinTreeRev(OdUInt64 nMask, BitDo &bd)
{
  if (nMask)
  {
    OdTrVisFlagsBinTree64_UpperBitsWrap<BitDo> _upper_bits_wrap(bd);
    ::odTrVisFlagsBinTreeRev((OdUInt32)(nMask >> 32), _upper_bits_wrap);
    ::odTrVisFlagsBinTreeRev((OdUInt32)nMask, bd);
  }
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreak(OdUInt64 nMask, BitDo &bd)
{
  if (nMask)
  {
    if (!::odTrVisFlagsBinTreeBreak((OdUInt32)nMask, bd)) return false;
    OdTrVisFlagsBinTreeBreak64_UpperBitsWrap<BitDo> _upper_bits_wrap(bd);
    if (!::odTrVisFlagsBinTreeBreak((OdUInt32)(nMask >> 32), _upper_bits_wrap)) return false;
  }
  return true;
}

template <typename BitDo>
inline bool odTrVisFlagsBinTreeBreakRev(OdUInt64 nMask, BitDo &bd)
{
  if (nMask)
  {
    OdTrVisFlagsBinTreeBreak64_UpperBitsWrap<BitDo> _upper_bits_wrap(bd);
    if (!::odTrVisFlagsBinTreeBreakRev((OdUInt32)(nMask >> 32), _upper_bits_wrap)) return false;
    if (!::odTrVisFlagsBinTreeBreakRev((OdUInt32)nMask, bd)) return false;
  }
  return true;
}

#include "TD_PackPop.h"

#endif // _EXTRVISFLAGSBINTREE_INCLUDED_
