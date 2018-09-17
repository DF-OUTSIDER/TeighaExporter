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
#include "TfIO.h"

struct OdTfHandleMap : std::map<OdDbHandle, OdTfDigest>
{
  void write(OdStreamBuf* hmStream) const
  {
    OdPlatformStreamer::wrInt64(*hmStream, size());
    for (OdTfHandleMap::const_iterator i = begin(); i != end(); ++i)
    {
      OdPlatformStreamer::wrInt64(*hmStream, i->first);
      i->second.write(hmStream);
    }
  }
  void write(OdTfStorage* vfs, OdTfDigest& sha) const
  {
    OdFlatMemStreamPtr hmStream = OdFlatMemStreamManaged::createNew(sizeof(OdInt64) + size()*(20 /*sizeof (OdTfDigest)*/ + sizeof(OdInt64)));
    write(hmStream);
    vfs->write(hmStream, sha);
  }
  void read(OdStreamBuf* s)
  {
    OdInt64 nObjects = OdPlatformStreamer::rdInt64(*s);
    for (OdInt64 i = 0; i < nObjects; ++i)
    {
      OdDbHandle h((OdUInt64)OdPlatformStreamer::rdInt64(*s));
      OdTfDigest sha; sha.read(s);
      (*this)[h] = sha;
    }
  }
  void read(OdTfStorage* vfs, const OdTfDigest& hsh)
  {
    read(vfs->read(hsh));
  }
};
