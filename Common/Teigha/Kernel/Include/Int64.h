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




#ifndef _INC_ODAINT64_38FF72C90128_INCLUDED
#define _INC_ODAINT64_38FF72C90128_INCLUDED

#ifndef EMCC
#error "If this code is used anywhere?"
#endif

#include "TD_PackPush.h"
#include "OdHeap.h"

/** \details
    This class represents signed 64-bit integers.

    <group Other_Classes>
*/
class OdInt64
{
public:
  /** \details
      to assign 0 or 1
  */
  
  inline OdInt64& operator=(const OdInt64& n);
  OdInt64& operator=(int n);
  OdInt64() : low(0), hi(0) { }
  OdInt64(int n) { hi = (n>=0 ? 0 : -1); low = n; }
  OdInt64(const OdInt64& n) { *this = n; }
  OdInt64& operator+=(const OdInt64& n);
  inline OdInt64 operator+(const OdInt64& n) const {return (OdInt64(*this)+=n);}
  inline OdInt64& operator-=(const OdInt64 n) { return (*this += -n); }
  inline OdInt64 operator-(const OdInt64& n) const {return (OdInt64(*this)-=n);}
  inline OdInt64 operator/(const OdInt32& n) const {return (OdInt64(*this)/=n);}
  OdInt64& operator/=(const OdInt32& n);
  OdUInt32 operator%(OdUInt32 n)const{return ((2<<31)%n*2*(hi%n)+(low%n))%n; }
  inline OdInt64& operator++();
  inline OdInt64 operator++(int /*dummy*/);
  inline OdInt64& operator--();
  inline bool operator>(const OdInt64& n) const;
  inline bool operator<(const OdInt64& n) const;
  inline bool operator>=(const OdInt64& n) const;
  inline bool operator<=(const OdInt64& n) const;
  inline bool operator!=(const OdInt64& n) const;
  inline bool operator==(const OdInt64& n) const;

  OdUInt32 operator & (OdUInt32 mask) const
  { return low & mask;}

  /** \details
      to compare with 0 or 1
  */
  
  inline bool operator!=(int n) const;
  /** \details
      to compare with 0 or 1
  */
  
  inline bool operator==(int n) const;

  bool operator>(int n) const { return operator>(OdInt64(n)); }
  bool operator<(int n) const { return operator<(OdInt64(n)); }

  inline OdInt64 operator-() const;
  OdInt64& operator >>=(int n);
  OdInt64& operator <<=(int n)
  {
    while(n--) { *this += *this; }
    return *this;
  }
  OdInt64 operator >>(int n) const
  {
    OdInt64 res = *this;
    return (res>>=n);
  }
  OdInt64 operator <<(int n) const
  {
    OdInt64 res = *this;
    return (res<<=n);
  }

  OdInt64 operator | (const OdInt64& n)
  {
    OdInt64 res;
    res.low = low | n.low;
    res.hi = hi | n.hi;
    return res;
  }

  OdInt64& operator |=(const OdInt64& n)
  {
    low |= n.low;
    hi |= n.hi;
    return *this;
  }

protected:
#ifdef ODA_BIGENDIAN
  OdInt32   hi;
  OdUInt32  low;
#else
  OdUInt32  low;
  OdInt32   hi;
#endif
};

/** \details
    This class represents unsigned 64-bit integers..

    <group Other_Classes>
*/
class OdUInt64 : public OdInt64
{
public :
  OdUInt64 () { }
  OdUInt64 (int n) : OdInt64(n) { }
  OdUInt64 (const OdInt64& n) : OdInt64(n) { }
  OdUInt64 (const OdUInt64& n) : OdInt64(n) { }

  bool operator > (const OdUInt64& n) const
  {
    if (hi == n.hi)
      return low > n.low;
    // else
    return OdUInt32(hi) > OdUInt32(n.hi);
  }
  bool operator < (const OdUInt64& n) const
  {
    return (!operator>(n) && operator!=(n));
  }
  bool operator>(int n) const { return operator>(OdUInt64(n)); }
  bool operator<(int n) const { return operator<(OdUInt64(n)); }

  OdUInt64 operator >>(int n) const
  {
    OdUInt64 res = *this;
    return (res>>=n);
  }
  OdUInt64& operator >>= (int n);
};

//----------------------------------------------------------
//
// OdInt64 inline methods
//
//----------------------------------------------------------
inline OdInt64& OdInt64::operator=(int n)
{
  hi = (n>=0 ? 0 : -1);
  low = OdUInt32(n);
  return *this;
}

inline OdInt64& OdInt64::operator++()
{
  return (*this += 1);
}

inline OdInt64 OdInt64::operator++(int /*dummy*/)
{
  OdInt64 t = *this;
  *this += 1;
  return t;
}

inline OdInt64& OdInt64::operator--()
{
  return (*this += -1);
}

inline bool OdInt64::operator > (const OdInt64& n) const
{
  if (hi == n.hi)
    return low > n.low;
  /** \details
      else
  */
  return hi > n.hi;
}

inline bool OdInt64::operator < (const OdInt64& n) const
{
  return (!operator>(n) && operator!=(n));
}

inline bool OdInt64::operator >= (const OdInt64& n) const
{
  return (operator>(n) || operator==(n));
}

inline bool OdInt64::operator <= (const OdInt64& n) const
{
  return (operator<(n) || operator==(n));
}

inline bool OdInt64::operator!=(int n) const
{
  return (!operator==(n));
}

inline bool OdInt64::operator!=(const OdInt64& n) const
{
  return (hi != n.hi || low != n.low);
}

inline bool OdInt64::operator==(int n) const
{
  return (hi == (n >= 0 ? 0 : -1) && low == OdUInt32(n));
}

inline bool OdInt64::operator==(const OdInt64& n) const
{
  return (hi == n.hi && low == n.low);
}

inline OdInt64 OdInt64::operator-() const
{
  OdInt64 res;
  res.hi = ~hi; res.low =~low;
  return (res += 1);
}

inline OdInt64& OdInt64::operator=(const OdInt64& n)
{
  low = n.low;
  hi = n.hi;
  return *this;
}

#include "TD_PackPop.h"

#endif /* _INC_ODAINT64_38FF72C90128_INCLUDED */
