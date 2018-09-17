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

#ifndef BITFLAGS_INCLUDED
#define BITFLAGS_INCLUDED

// A set of macros to define bit flags inside a class
// Usage sample:
/*
#define FLAG_DEF(F, V) CMV(F)

  FLAG_DEFS_BEG
    ODDBOBJECT_FLAGS
  FLAG_DEFS_END(OdUInt16, m_objectFlags)

#undef FLAG_DEF


FLAG_DEFS_BEG - starts enum ( enum { )
  
CMV(name) macro defines inside enum symbols:
  - name_C - number of bit (0, 1, 2, ...)
  - name_V - mask (1, 2, 4, 8, ...)

FLAG_DEFS_END(type, member_name)
- describes member to store the flags (char, short, long)
- defines void setFlags(int mask, int values) function
- in _DEBUG build checks number of bits to match type specified.
*/

#define kOdDbIdAllBits         0xFFFFFFFF


#define FLAG_DEFS_BEG   enum {


#define CMV(name) \
  name##_C, \
  name##_M = (1 << name##_C), \
  name##_V = name##_C,


#ifdef _DEBUG
// Define the macro with check for number of bits
#define FLAG_DEFS_END(type, name) \
  name##_last_bit}; \
  type name;  \
  type getFlags(int mask) \
  { \
    return (type)(name & mask); \
  } \
  void setFlags(int mask, int values) \
  { \
    name = (type)((name & ~mask) | (values & mask));  \
  } \
  static void name##_size_check_fn()  \
  {\
    static char name##_size_check[(sizeof(type) * 8 >= name##_last_bit)];\
  }

#else
// Define the macro without check for number of bits
#define FLAG_DEFS_END(type, name) \
    name##_last_bit}; \
    type name;  \
    type getFlags(int mask) \
    { \
      return (type)(name & mask); \
    } \
    void setFlags(int mask, int values) \
    { \
      name = (type)((name & ~mask) | (values & mask));  \
    } \

#endif  // _DEBUG

// Define object flags
//
#define FLAG_DEF1(name, val) \
  name##_C, \
  name##_M = (1 << name##_C), \
  name##_V = name##_C,
#define DEFINE_FLAGS(Type, Flags, FLAG_DEFS) \
  FLAG_DEFS_BEG \
    FLAG_DEFS(FLAG_DEF1) \
  FLAG_DEFS_END(Type, Flags)

// Define default value for object flags
//
#define FLAG_DEF2(name, val) | ((val) ? name##_M : 0)
#define DEFINE_FLAGS_VALUE(Name, FLAG_DEFS) \
  enum { \
    Name = 0 \
    FLAG_DEFS(FLAG_DEF2) \
  };

// Define mask for object flags
//
#define FLAG_DEF3(name, val) | (name##_M)
#define DEFINE_FLAGS_MASK(Name, FLAG_DEFS) \
  enum { \
    Name = 0 \
    FLAG_DEFS(FLAG_DEF3) \
  };

// Define accessors for object flags
//
#define FLAG_DEF4(name, v) \
  bool is##name() const     { return GETBIT(m_objectFlags, name##_M);} \
  void set##name(bool val)  { SETBIT(m_objectFlags, name##_M, val);}
#define DEFINE_FLAGS_ACCESSORS(FLAG_DEFS) \
  FLAG_DEFS(FLAG_DEF4)


// The template class defines a set of bits.
// 
template<typename T>
class OdBitSet
{
public:
  typedef T flag_type;

  OdBitSet() : m_flags(0) {}

  T flags() const { return m_flags; }
  T flags(T mask) const { return (m_flags & mask); }
  void setFlags(T flags, T mask = kOdDbIdAllBits) { m_flags = (m_flags & (~mask)) | (flags & mask); }

private:
  T m_flags;
};

#endif  // BITFLAGS_INCLUDED
