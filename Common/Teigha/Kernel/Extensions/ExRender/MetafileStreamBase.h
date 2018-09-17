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

#ifndef _METAFILESTREAMBASE_INCLUDED_
#define _METAFILESTREAMBASE_INCLUDED_

#include "TD_PackPush.h"

#include "OdaCommon.h"
#include "OdAlloc.h"
#include "OdString.h"
#include "OdArray.h"
#include "CmColorBase.h"
#include "OdCharMapper.h"

#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeQuaternion.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GeMatrix3d.h"

class OdBaseMetafileContainerReader;
class OdBaseMetafileContainerWriter;

/** \details
  This class is a base memory container for the metafile contents interface.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileContainer
{
  public:
    // Size of entire allocated memory
    virtual OdUInt32 size() const = 0;
    // Pointer to memory for position 
    virtual const void *memoryPtr(OdUInt32 pos) const = 0;
    // Allocates additional memory
    virtual void inflateBuffer(OdUInt32 size, bool lockPage = true) = 0;
    // Write data to memory
    virtual void writeTo(OdUInt32 pos, const void *data, OdUInt32 size) = 0;
    // Read data from memory
    virtual void readFrom(OdUInt32 pos, void *data, OdUInt32 size) const = 0;
    // Truncates allocations to use optimal memory size (call after all writing operations completed)
    virtual void optimizeMemory(OdUInt32 uSize) = 0;

    virtual OdBaseMetafileContainerReader *createReader() const = 0;
    virtual OdBaseMetafileContainerWriter *createWriter() const = 0;
};

/** \details
  This class is a flat memory container for metafile contents.

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdFlatMetafileContainer : public OdBaseMetafileContainer
{
  friend class OdFlatMetafileContainerWriter;
  friend class OdFlatMetafileContainerReader;
  protected:
    OdUInt8 *m_pData;
    OdUInt32 m_uLength;
  public:
    OdFlatMetafileContainer() : m_pData(NULL), m_uLength(0) { }
    ~OdFlatMetafileContainer()
    {
      clearFlatContainer();
    }

    void clearFlatContainer()
    {
      if (m_pData)
      {
        ::odrxFree(m_pData);
        m_pData = NULL;
      }
      m_uLength = 0;
    }

    OdUInt32 size() const
    {
      return m_uLength;
    }

    const void *memoryPtr(OdUInt32 pos) const
    {
      return m_pData + pos;
    }

    void inflateBuffer(OdUInt32 size, bool /*lockPage*/ = true)
    {
      if (m_pData)
        m_pData = (OdUInt8*)::odrxRealloc(m_pData, m_uLength + size, m_uLength);
      else
        m_pData = (OdUInt8*)::odrxAlloc(size);
      if (m_pData == NULL)
        throw OdError(eOutOfMemory);
      m_uLength += size;
    }

    void writeTo(OdUInt32 pos, const void *data, OdUInt32 size)
    {
      ::memcpy(m_pData + pos, data, size);
    }

    void readFrom(OdUInt32 pos, void *data, OdUInt32 size) const
    {
      ::memcpy(data, m_pData + pos, size);
    }

    void optimizeMemory(OdUInt32 uSize)
    {
      if (m_uLength != uSize)
      {
        m_pData = (OdUInt8*)::odrxRealloc(m_pData, uSize, m_uLength);
        m_uLength = uSize;
      }
    }

    OdBaseMetafileContainerReader *createReader() const;
    OdBaseMetafileContainerWriter *createWriter() const;
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileContainerIOBase
{
  protected:
    OdBaseMetafileContainer *m_pContainer;
  public:
    OdBaseMetafileContainerIOBase() : m_pContainer(NULL) { }
    OdBaseMetafileContainerIOBase(OdBaseMetafileContainer *pContainer) : m_pContainer(pContainer) { }
    virtual ~OdBaseMetafileContainerIOBase() { } // Mac warn

    virtual void setContainer(OdBaseMetafileContainer *pContainer)
    {
      m_pContainer = pContainer;
    }
    virtual OdBaseMetafileContainer *container() const
    {
      return m_pContainer;
    }

    virtual OdBaseMetafileContainer *createContainer() const = 0;

    // Size of entire allocated memory
    virtual OdUInt32 size() const = 0;
    // Current caret position
    virtual OdUInt32 position() const = 0;
    // Reset current position
    virtual void seek(OdUInt32 delta, bool forward = true) = 0;
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileContainerWriter : public /*virtual*/ OdBaseMetafileContainerIOBase
{
  public:
    OdBaseMetafileContainerWriter() : OdBaseMetafileContainerIOBase() { }
    OdBaseMetafileContainerWriter(OdBaseMetafileContainer *pContainer) : OdBaseMetafileContainerIOBase(pContainer) { }
    // Reserves memory for write operation
    virtual void reserve(OdUInt32 size, bool moveCaret = true, bool lockPage = true) = 0;
    // Write data to memory
    virtual void write(const void *data, OdUInt32 size, bool lockPage = true) = 0;
    // Truncates allocations to use optimal memory size (call after all writing operations completed)
    virtual void completeWriting() = 0;
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileContainerReader : public /*virtual*/ OdBaseMetafileContainerIOBase
{
  public:
    OdBaseMetafileContainerReader() : OdBaseMetafileContainerIOBase() { }
    OdBaseMetafileContainerReader(OdBaseMetafileContainer *pContainer) : OdBaseMetafileContainerIOBase(pContainer) { }
    // Current pointer for caret
    virtual const void *memoryPtr() const = 0;
    // Read data from memory
    virtual void read(void *data, OdUInt32 size) const = 0;
    // Test is caret at the end of data
    virtual bool isEof() const = 0;
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdFlatMetafileContainerWriter : public OdBaseMetafileContainerWriter
{
  protected:
    OdUInt32 m_uCaret;
    OdUInt32 m_uLength;
    //
    OdUInt32 m_uInitSize;
    OdUInt32 m_uGrowSize;
  public:
#define OD_FLATMETAFILECONTAINERWRITER_DEF_INITSIZE 256
#define OD_FLATMETAFILECONTAINERWRITER_DEF_GROWSIZE 64
    OdFlatMetafileContainerWriter(OdUInt32 uInitSize = OD_FLATMETAFILECONTAINERWRITER_DEF_INITSIZE, OdUInt32 uGrowSize = OD_FLATMETAFILECONTAINERWRITER_DEF_GROWSIZE) : OdBaseMetafileContainerWriter(), m_uCaret(0), m_uLength(0), m_uInitSize(uInitSize), m_uGrowSize(uGrowSize) { }
    OdFlatMetafileContainerWriter(OdBaseMetafileContainer *pContainer, OdUInt32 uInitSize = OD_FLATMETAFILECONTAINERWRITER_DEF_INITSIZE, OdUInt32 uGrowSize = OD_FLATMETAFILECONTAINERWRITER_DEF_GROWSIZE) : OdBaseMetafileContainerWriter(pContainer), m_uCaret(0), m_uInitSize(uInitSize), m_uGrowSize(uGrowSize) { m_uLength = pContainer->size(); }
    
    void setInitSize(OdUInt32 uInitSize) { m_uInitSize = uInitSize; }
    OdUInt32 initSize() const { return m_uInitSize; }
    void setGrowSize(OdUInt32 uGrowSize) { m_uGrowSize = uGrowSize; }
    OdUInt32 growSize() const { return m_uGrowSize; }

    void setContainer(OdBaseMetafileContainer *pContainer)
    {
      OdBaseMetafileContainerIOBase::setContainer(pContainer);
      if (pContainer)
        m_uLength = pContainer->size();
      m_uCaret = 0;
    }

    OdBaseMetafileContainer *createContainer() const
    {
      return new OdFlatMetafileContainer();
    }

    OdFlatMetafileContainer *flatContainer() const { return static_cast<OdFlatMetafileContainer*>(container()); }

    OdUInt32 size() const
    {
      return m_uLength;
    }
    OdUInt32 position() const
    {
      return m_uCaret;
    }
    
    void seek(OdUInt32 delta, bool forward = true)
    {
      if (delta)
      {
        if (forward)
        {
          if (m_uCaret + delta < m_uLength)
            m_uCaret += delta;
          else
            m_uCaret = m_uLength;
        }
        else
        {
          if (delta < m_uCaret)
            m_uCaret -= delta;
          else
            m_uCaret = 0;
        }
      }
    }

    void reserve(OdUInt32 size, bool moveCaret, bool lockPage = true)
    {
      if (size + m_uCaret > m_uLength)
      {
        OdUInt32 newLen = m_uCaret + size;
        if (newLen > flatContainer()->size())
        {
          OdUInt32 swapSize = m_uInitSize;
          if (newLen > swapSize)
            swapSize = ((newLen - m_uInitSize) / m_uGrowSize + 1) * m_uGrowSize + m_uInitSize;
          flatContainer()->inflateBuffer(swapSize - flatContainer()->size(), lockPage);
        }
        m_uLength = m_uCaret + size;
      }
      if (moveCaret)
        m_uCaret += size;
    }
    void write(const void *data, OdUInt32 size, bool lockPage = true)
    {
      if (size)
      {
        reserve(size, false, lockPage);
        flatContainer()->writeTo(m_uCaret, data, size);
        m_uCaret += size;
      }
    }
    void completeWriting()
    {
      flatContainer()->optimizeMemory(m_uLength);
    }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdFlatMetafileContainerReader : public OdBaseMetafileContainerReader
{
  protected:
    mutable OdUInt32 m_uCaret;
  public:
    OdFlatMetafileContainerReader() : OdBaseMetafileContainerReader(), m_uCaret(0) { }
    OdFlatMetafileContainerReader(OdBaseMetafileContainer *pContainer) : OdBaseMetafileContainerReader(pContainer), m_uCaret(0) { }

    void setContainer(OdBaseMetafileContainer *pContainer)
    {
      OdBaseMetafileContainerIOBase::setContainer(pContainer);
      m_uCaret = 0;
    }

    OdBaseMetafileContainer *createContainer() const
    {
      return new OdFlatMetafileContainer();
    }

    OdFlatMetafileContainer *flatContainer() const { return static_cast<OdFlatMetafileContainer*>(container()); }

    OdUInt32 size() const
    {
      return flatContainer()->size();
    }
    OdUInt32 position() const
    {
      return m_uCaret;
    }

    void seek(OdUInt32 delta, bool forward = true)
    {
      if (delta)
      {
        if (forward)
        {
          if (m_uCaret + delta < flatContainer()->size())
            m_uCaret += delta;
          else
            m_uCaret = flatContainer()->size();
        }
        else
        {
          if (delta < m_uCaret)
            m_uCaret -= delta;
          else
            m_uCaret = 0;
        }
      }
    }

    const void *memoryPtr() const
    {
      return flatContainer()->memoryPtr(m_uCaret);
    }
    void read(void *data, OdUInt32 size) const
    {
      flatContainer()->readFrom(m_uCaret, data, size);
      m_uCaret += size;
    }
    bool isEof() const
    {
      return m_uCaret >= size();
    }
};

inline OdBaseMetafileContainerReader *OdFlatMetafileContainer::createReader() const
{
  return new OdFlatMetafileContainerReader();
}

inline OdBaseMetafileContainerWriter *OdFlatMetafileContainer::createWriter() const
{
  return new OdFlatMetafileContainerWriter();
}

// @@@TODO: implement effective paged memory container for metafile contents instead of flat container, which
//          can be unsafe on some platforms with big amount of data.

/** \details
  Base paged memory container for metafile contents.
*/
/*class OdPagedMetafileContainer
{
  struct Page
  {
    Page *m_pNextPage;
    OdUInt32 m_uPageLength;
    OdUInt8 *m_pData;
  };
  Page *m_pPage;
};*/

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileIOStream
{
  protected:
    OdBaseMetafileContainerIOBase *m_pIO;
  public:
    OdBaseMetafileIOStream() : m_pIO(NULL) { }
    OdBaseMetafileIOStream(OdBaseMetafileContainerIOBase *pIO) : m_pIO(pIO) { }

    virtual void setContainerIO(OdBaseMetafileContainerIOBase *pIO)
    {
      m_pIO = pIO;
    }
    virtual OdBaseMetafileContainerIOBase *containerIO() const
    {
      return m_pIO;
    }

    virtual OdUInt32 size() const
    {
      return m_pIO->size();
    }
    virtual OdUInt32 position() const
    {
      return m_pIO->position();
    }

    virtual void seek(OdUInt32 delta, bool forward = true)
    {
      m_pIO->seek(delta, forward);
    }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileInputStream : public /*virtual*/ OdBaseMetafileIOStream
{
  public:
    OdBaseMetafileInputStream() : OdBaseMetafileIOStream() { }
    OdBaseMetafileInputStream(OdBaseMetafileContainerIOBase *pIO) : OdBaseMetafileIOStream(pIO) { }

    OdBaseMetafileContainerReader *containerReader() const { return static_cast<OdBaseMetafileContainerReader*>(m_pIO); }

    // Binary level
    virtual const void *memoryPtr() const
    {
      return containerReader()->memoryPtr();
    }
    virtual void read(void *data, OdUInt32 size) const
    {
      containerReader()->read(data, size);
    }
    virtual bool isEof() const
    {
      return containerReader()->isEof();
    }
    
    // Read DD types
#define READ_TYPE_DEF(type) \
  type rVal; \
  read(&rVal, sizeof(type)); \
  return rVal

    OdInt8 readInt8() const { READ_TYPE_DEF(OdInt8); }
    OdUInt8 readUInt8() const { READ_TYPE_DEF(OdUInt8); }
    OdInt16 readInt16() const { READ_TYPE_DEF(OdInt16); }
    OdUInt16 readUInt16() const { READ_TYPE_DEF(OdUInt16); }
    OdInt32 readInt32() const { READ_TYPE_DEF(OdInt32); }
    OdUInt32 readUInt32() const { READ_TYPE_DEF(OdUInt32); }
    OdInt64 readInt64() const { READ_TYPE_DEF(OdInt64); }
    OdUInt64 readUInt64() const { READ_TYPE_DEF(OdUInt64); }
    float readFloat() const { READ_TYPE_DEF(float); }
    double readDouble() const { READ_TYPE_DEF(double); }
    bool readBool() const { READ_TYPE_DEF(bool); }
    OdCmEntityColor readCmEntityColor() const { READ_TYPE_DEF(OdCmEntityColor); }
    OdGePoint2d readPoint2d() const { READ_TYPE_DEF(OdGePoint2d); }
    OdGePoint3d readPoint3d() const { READ_TYPE_DEF(OdGePoint3d); }
    OdGeVector2d readVector2d() const { READ_TYPE_DEF(OdGeVector2d); }
    OdGeVector3d readVector3d() const { READ_TYPE_DEF(OdGeVector3d); }
    OdGeQuaternion readQuaternion() const { READ_TYPE_DEF(OdGeQuaternion); }
    OdGeMatrix2d readMatrix2d() const { READ_TYPE_DEF(OdGeMatrix2d); }
    OdGeMatrix3d readMatrix3d() const { READ_TYPE_DEF(OdGeMatrix3d); }
    ODCOLORREF readColorRef() const { READ_TYPE_DEF(ODCOLORREF); }
    void *readPointer() const { READ_TYPE_DEF(void*); }
    OdGsMarker readGsMarker() const { READ_TYPE_DEF(OdGsMarker); }
#undef READ_TYPE_DEF

    // Read DD arrays
#define READ_ARRAY_DEF(type) \
  read(pVal, nSize * sizeof(type))

    void readInt8Array(OdInt8 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdInt8); }
    void readUInt8Array(OdUInt8 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdUInt8); }
    void readInt16Array(OdInt16 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdInt16); }
    void readUInt16Array(OdUInt16 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdUInt16); }
    void readInt32Array(OdInt32 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdInt32); }
    void readUInt32Array(OdUInt32 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdUInt32); }
    void readInt64Array(OdInt64 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdInt64); }
    void readUInt64Array(OdUInt64 *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdUInt64); }
    void readFloatArray(float *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(float); }
    void readDoubleArray(double *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(double); }
    void readBoolArray(bool *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(bool); }
    void readCmEntityColorArray(OdCmEntityColor *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdCmEntityColor); }
    void readPoint2dArray(OdGePoint2d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGePoint2d); }
    void readPoint3dArray(OdGePoint3d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGePoint3d); }
    void readVector2dArray(OdGeVector2d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGeVector2d); }
    void readVector3dArray(OdGeVector3d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGeVector3d); }
    void readQuaternionArray(OdGeQuaternion *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGeQuaternion); }
    void readMatrix2dArray(OdGeMatrix2d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGeMatrix2d); }
    void readMatrix3dArray(OdGeMatrix3d *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGeMatrix3d); }
    void readColorRefArray(ODCOLORREF *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(ODCOLORREF); }
    void readGsMarkerArray(OdGsMarker *pVal, OdUInt32 nSize) const { READ_ARRAY_DEF(OdGsMarker); }
#undef READ_ARRAY_DEF

    OdString readString() const
    {
      OdString rStr;
      OdUInt32 uSize = readUInt32();
      if (uSize)
      {
        OdCharArray chars;
        chars.resize(uSize + 1);
        chars.at(uSize) = 0;
        read(chars.asArrayPtr(), uSize * sizeof(OdChar));
        rStr = chars.asArrayPtr();
      }
      return rStr;
    }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdBaseMetafileOutputStream : public /*virtual*/ OdBaseMetafileIOStream
{
  public:
    OdBaseMetafileOutputStream() : OdBaseMetafileIOStream() { }
    OdBaseMetafileOutputStream(OdBaseMetafileContainerIOBase *pIO) : OdBaseMetafileIOStream(pIO) { }

    OdBaseMetafileContainerWriter *containerWriter() const { return static_cast<OdBaseMetafileContainerWriter*>(m_pIO); }

    // Binary level
    virtual void reserve(OdUInt32 size, bool moveCaret = true, bool lockPage = true)
    {
      containerWriter()->reserve(size, moveCaret, lockPage);
    }
    virtual void write(const void *data, OdUInt32 size, bool lockPage = true)
    {
      containerWriter()->write(data, size, lockPage);
    }
    virtual void completeWriting()
    {
      containerWriter()->completeWriting();
    }

    // Write DD types
#define WRITE_TYPE_DEF(type) \
  write(&value, sizeof(type))

    void writeInt8(OdInt8 value) { WRITE_TYPE_DEF(OdInt8); }
    void writeUInt8(OdUInt8 value) { WRITE_TYPE_DEF(OdUInt8); }
    void writeInt16(OdInt16 value) { WRITE_TYPE_DEF(OdInt16); }
    void writeUInt16(OdUInt16 value) { WRITE_TYPE_DEF(OdUInt16); }
    void writeInt32(OdInt32 value) { WRITE_TYPE_DEF(OdInt32); }
    void writeUInt32(OdUInt32 value) { WRITE_TYPE_DEF(OdUInt32); }
    void writeInt64(OdInt64 value) { WRITE_TYPE_DEF(OdInt64); }
    void writeUInt64(OdUInt64 value) { WRITE_TYPE_DEF(OdUInt64); }
    void writeFloat(float value) { WRITE_TYPE_DEF(float); }
    void writeDouble(double value) { WRITE_TYPE_DEF(double); }
    void writeBool(bool value) { WRITE_TYPE_DEF(bool); }
    void writeCmEntityColor(const OdCmEntityColor &value) { WRITE_TYPE_DEF(OdCmEntityColor); }
    void writePoint2d(const OdGePoint2d &value) { WRITE_TYPE_DEF(OdGePoint2d); }
    void writePoint3d(const OdGePoint3d &value) { WRITE_TYPE_DEF(OdGePoint3d); }
    void writeVector2d(const OdGeVector2d &value) { WRITE_TYPE_DEF(OdGeVector2d); }
    void writeVector3d(const OdGeVector3d &value) { WRITE_TYPE_DEF(OdGeVector3d); }
    void writeQuaternion(const OdGeQuaternion &value) { WRITE_TYPE_DEF(OdGeQuaternion); }
    void writeMatrix2d(const OdGeMatrix2d &value) { WRITE_TYPE_DEF(OdGeMatrix2d); }
    void writeMatrix3d(const OdGeMatrix3d &value) { WRITE_TYPE_DEF(OdGeMatrix3d); }
    void writeColorRef(ODCOLORREF value) { WRITE_TYPE_DEF(ODCOLORREF); }
    void writePointer(const void *value) { WRITE_TYPE_DEF(void*); }
    void writeGsMarker(OdGsMarker value) { WRITE_TYPE_DEF(OdGsMarker); }
#undef WRITE_TYPE_DEF

    // Write DD arrays
#define WRITE_ARRAY_DEF(type) \
  write(pVal, nSize * sizeof(type))

    void writeInt8Array(const OdInt8 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdInt8); }
    void writeUInt8Array(const OdUInt8 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdUInt8); }
    void writeInt16Array(const OdInt16 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdInt16); }
    void writeUInt16Array(const OdUInt16 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdUInt16); }
    void writeInt32Array(const OdInt32 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdInt32); }
    void writeUInt32Array(const OdUInt32 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdUInt32); }
    void writeInt64Array(const OdInt64 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdInt64); }
    void writeUInt64Array(const OdUInt64 *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdUInt64); }
    void writeFloatArray(const float *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(float); }
    void writeDoubleArray(const double *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(double); }
    void writeBoolArray(const bool *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(bool); }
    void writeCmEntityColorArray(const OdCmEntityColor *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdCmEntityColor); }
    void writePoint2dArray(const OdGePoint2d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGePoint2d); }
    void writePoint3dArray(const OdGePoint3d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGePoint3d); }
    void writeVector2dArray(const OdGeVector2d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGeVector2d); }
    void writeVector3dArray(const OdGeVector3d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGeVector3d); }
    void writeQuaternionArray(const OdGeQuaternion *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGeQuaternion); }
    void writeMatrix2dArray(const OdGeMatrix2d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGeMatrix2d); }
    void writeMatrix3dArray(const OdGeMatrix3d *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGeMatrix3d); }
    void writeColorRefArray(const ODCOLORREF *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(ODCOLORREF); }
    void writeGsMarkerArray(const OdGsMarker *pVal, OdUInt32 nSize) { WRITE_ARRAY_DEF(OdGsMarker); }
#undef WRITE_ARRAY_DEF

    void writeString(const OdChar *pChars, OdUInt32 nChars)
    {
      writeUInt32(nChars);
      if (nChars)
        write(pChars, nChars * sizeof(OdChar));
    }
    void writeString(const OdString &str)
    {
      writeString(str.c_str(), (OdUInt32)str.getLength());
    }
};

#include "TD_PackPop.h"

#endif // _METAFILESTREAMBASE_INCLUDED_
