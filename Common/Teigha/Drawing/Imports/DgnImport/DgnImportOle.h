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

#ifndef _DGN_IMPORT_OLE_INCLUDED_ 
#define _DGN_IMPORT_OLE_INCLUDED_

#include "DgnImportPE.h"
#include "DgnImportCommon.h"
#include <OdaCommon.h>
#include <DgOle2Frame.h>
#include <DbOle2Frame.h>
#include <MemoryStream.h>
#include <OleItemHandler.h>

namespace TD_DGN_IMPORT 
{

//---------------------------------------------------------------------------------------------------
struct _tmpStream : public OdStaticRxObject<OdOleItemInitStream>
{
  OdStreamBufPtr m_stream;
  OdDbStub *m_pFrameId;
  //
  virtual OdString fileName() { return m_stream->fileName(); }
  virtual bool      isEof() { return m_stream->isEof(); }
  virtual OdUInt64  tell() { return m_stream->tell(); }
  virtual OdUInt64  length() { return m_stream->length(); }
  virtual void      truncate() { m_stream->truncate(); }
  virtual void      rewind() { m_stream->rewind(); }
  virtual OdUInt64  seek(OdInt64 offset, OdDb::FilerSeekType seekType)
  { return m_stream->seek(offset, seekType); }
  virtual OdUInt8   getByte() { return m_stream->getByte(); }
  virtual void      getBytes(void* buffer, OdUInt32 numBytes)
  { m_stream->getBytes(buffer, numBytes); }
  virtual void      putByte(OdUInt8 value) { m_stream->putByte(value); }
  virtual void      putBytes(const void* buffer, OdUInt32 numBytes)
  { m_stream->putBytes(buffer, numBytes); }
  virtual void      copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart = 0, OdUInt64 sourceEnd = 0)
  { m_stream->copyDataTo(pDestination, sourceStart, sourceEnd); }
  virtual OdUInt32  getShareMode() { return m_stream->getShareMode(); }
  //
  virtual OdDbStub* frameId() const { return m_pFrameId; }
};

//---------------------------------------------------------------------------------------------------

template <class T> OdGePoint3d getOleOrigin(T*){return OdGePoint3d::kOrigin;}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getOleOrigin<OdDgOle2Frame3d>(OdDgOle2Frame3d* ole)
{
  return ole->getPosition();
}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getOleOrigin<OdDgOle2Frame2d>(OdDgOle2Frame2d* ole)
{
  OdGePoint2d p = ole->getPosition();
  return OdGePoint3d(p.x, p.y, 0);
}

//---------------------------------------------------------------------------------------------------

template <class T> double getRotation(T*){return 0;}

//---------------------------------------------------------------------------------------------------

template <> double getRotation<OdDgOle2Frame2d>(OdDgOle2Frame2d* ole2d)
{
  double dAngle = 0;

  OdGeMatrix2d matRotation2d = ole2d->getTransformation();

  OdGeVector2d vrXAxis = OdGeVector2d::kXAxis;

  vrXAxis.transformBy( matRotation2d );
  vrXAxis.normalize();

  dAngle = OdGeVector2d::kXAxis.angleTo( vrXAxis );

  return dAngle;
}

//---------------------------------------------------------------------------------------------------

template <> double getRotation<OdDgOle2Frame3d>(OdDgOle2Frame3d* ole3d)
{
  double dAngle = 0;

  OdGeMatrix3d matRotation3d = ole3d->getTransformation();

  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
  OdGeVector3d vrZAxis = OdGeVector3d::kZAxis;

  vrXAxis.transformBy( matRotation3d );
  vrXAxis.orthoProject( OdGeVector3d::kZAxis);

  if( !OdZero(vrXAxis.length()) )
  {
    vrXAxis.normalize();
    dAngle = OdGeVector3d::kXAxis.angleTo( vrXAxis );
  }

  return dAngle;
}

//---------------------------------------------------------------------------------------------------

template <class T> struct OdDgOle2FrameImportPE : OdDgnImportPE
{
  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    T* dgOle = static_cast<T*>(e);
    OdDbOle2FramePtr dbOle = OdDbOle2Frame::createObject();
    dbOle->setDatabaseDefaults(owner->database());
    owner->database()->addOdDbObject(dbOle, owner->objectId());
    owner->appendOdDbEntity(dbOle);
    _tmpStream streamBuf;
    streamBuf.m_stream = OdMemoryStream::createNew();
    streamBuf.m_pFrameId = dbOle->objectId();
    dgOle->itemHandler()->save(streamBuf);
    streamBuf.rewind();
    dbOle->getItemHandler()->load(streamBuf);
    const_cast<OdOleItemHandler*>(dbOle->itemHandler())->setDrawAspect((OdOleItemHandler::DvAspect)dgOle->getDrawAspect());

    double dAngle = getRotation(dgOle);
    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    OdGeVector3d vrYAxis = OdGeVector3d::kYAxis;
    vrXAxis = vrXAxis.rotateBy( dAngle, OdGeVector3d::kZAxis );
    vrYAxis = vrYAxis.rotateBy( dAngle, OdGeVector3d::kZAxis );

    OdRectangle3d pos;
    pos.lowLeft  = getOleOrigin(dgOle);
    pos.lowRight = pos.lowLeft;
    pos.lowRight += vrXAxis*(dgOle->getObjectSizeX() * dgOle->getXScale());
    pos.upRight  = pos.lowRight;
    pos.upRight  += vrYAxis*(dgOle->getObjectSizeY() * dgOle->getYScale());
    pos.upLeft   = pos.lowLeft;
    pos.upLeft   += vrYAxis*(dgOle->getObjectSizeY() * dgOle->getYScale());

    dbOle->setPosition(pos);
    dbOle->setRotation( dAngle );
    copyEntityProperties(dgOle, dbOle);
  }
};

//---------------------------------------------------------------------------------------------------

}
#endif // _DGN_IMPORT_OLE_INCLUDED_
