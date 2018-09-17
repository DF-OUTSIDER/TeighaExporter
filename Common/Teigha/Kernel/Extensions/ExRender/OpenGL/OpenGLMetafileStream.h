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

#ifndef _OPENGLMETAFILESTREAM_INCLUDED_
#define _OPENGLMETAFILESTREAM_INCLUDED_

#include "MetafileStreamBase.h"
#include "PseudoGLDefs.h"

/** \details

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMetafileInputStream : public OdBaseMetafileInputStream
{
  public:
    OdOpenGLMetafileInputStream() : OdBaseMetafileInputStream() { }
    OdOpenGLMetafileInputStream(OdBaseMetafileContainerIOBase *pIO) : OdBaseMetafileInputStream(pIO) { }

    // Read OpenGL types
#define READ_TYPE_DEF(type) \
  type rVal; \
  read(&rVal, sizeof(type)); \
  return rVal

    OdGLenum readGLenum() const { READ_TYPE_DEF(OdGLenum); }
    OdGLboolean readGLboolean() const { READ_TYPE_DEF(OdGLboolean); }
    OdGLbyte readGLbyte() const { READ_TYPE_DEF(OdGLbyte); }
    OdGLshort readGLshort() const { READ_TYPE_DEF(OdGLshort); }
    OdGLint readGLint() const { READ_TYPE_DEF(OdGLint); }
    OdGLubyte readGLubyte() const { READ_TYPE_DEF(OdGLubyte); }
    OdGLushort readGLushort() const { READ_TYPE_DEF(OdGLushort); }
    OdGLuint readGLuint() const { READ_TYPE_DEF(OdGLuint); }
    OdGLfloat readGLfloat() const { READ_TYPE_DEF(OdGLfloat); }
    OdGLdouble readGLdouble() const { READ_TYPE_DEF(OdGLdouble); }
#undef READ_TYPE_DEF

    // Read OpenGL arrays
#define READ_ARRAY_DEF(type) \
  read(pVal, nSize * sizeof(type))

    void readGLenumArray(OdGLenum *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLenum); }
    void readGLbooleanArray(OdGLboolean *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLboolean); }
    void readGLbyteArray(OdGLbyte *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLbyte); }
    void readGLshortArray(OdGLshort *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLshort); }
    void readGLintArray(OdGLint *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLint); }
    void readGLubyteArray(OdGLubyte *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLubyte); }
    void readGLushortArray(OdGLushort *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLushort); }
    void readGLuintArray(OdGLuint *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLuint); }
    void readGLfloatArray(OdGLfloat *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLfloat); }
    void readGLdoubleArray(OdGLdouble *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGLdouble); }
#undef READ_ARRAY_DEF
};

/** \details

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMetafileOutputStream : public OdBaseMetafileOutputStream
{
  public:
    OdOpenGLMetafileOutputStream() : OdBaseMetafileOutputStream() { }
    OdOpenGLMetafileOutputStream(OdBaseMetafileContainerIOBase *pIO) : OdBaseMetafileOutputStream(pIO) { }

    // Write OpenGL types
#define WRITE_TYPE_DEF(type) \
  write(&value, sizeof(type))

    void writeGLenum(OdGLenum value) { WRITE_TYPE_DEF(OdGLenum); }
    void writeGLboolean(OdGLboolean value) { WRITE_TYPE_DEF(OdGLboolean); }
    void writeGLbyte(OdGLbyte value) { WRITE_TYPE_DEF(OdGLbyte); }
    void writeGLshort(OdGLshort value) { WRITE_TYPE_DEF(OdGLshort); }
    void writeGLint(OdGLint value) { WRITE_TYPE_DEF(OdGLint); }
    void writeGLubyte(OdGLubyte value) { WRITE_TYPE_DEF(OdGLubyte); }
    void writeGLushort(OdGLushort value) { WRITE_TYPE_DEF(OdGLushort); }
    void writeGLuint(OdGLuint value) { WRITE_TYPE_DEF(OdGLuint); }
    void writeGLfloat(OdGLfloat value) { WRITE_TYPE_DEF(OdGLfloat); }
    void writeGLdouble(OdGLdouble value) { WRITE_TYPE_DEF(OdGLdouble); }
#undef WRITE_TYPE_DEF

    // Write OpenGL arrays
#define WRITE_ARRAY_DEF(type) \
  write(pVal, nSize * sizeof(type))

    void writeGLenumArray(const OdGLenum *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLenum); }
    void writeGLbooleanArray(const OdGLboolean *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLboolean); }
    void writeGLbyteArray(const OdGLbyte *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLbyte); }
    void writeGLshortArray(const OdGLshort *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLshort); }
    void writeGLintArray(const OdGLint *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLint); }
    void writeGLubyteArray(const OdGLubyte *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLubyte); }
    void writeGLushortArray(const OdGLushort *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLushort); }
    void writeGLuintArray(const OdGLuint *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLuint); }
    void writeGLfloatArray(const OdGLfloat *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLfloat); }
    void writeGLdoubleArray(const OdGLdouble *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGLdouble); }
#undef WRITE_ARRAY_DEF
};

#endif // _OPENGLMETAFILESTREAM_INCLUDED_
