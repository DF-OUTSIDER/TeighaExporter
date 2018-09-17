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

#ifndef _OD_RxFS_INCLUDED_
#define _OD_RxFS_INCLUDED_

#include "RxSystemServices.h"
#include "RxSysRegistry.h"
#include "Tf/TfIO.h"
#include "DynamicLinker.h"


class OdRxDictionary;

namespace OD {
  inline OdAnsiString toUTF8(const OdString& str) { return OdAnsiString(str, CP_UTF_8); }
  inline OdAnsiString toUTF8(const char* sz) { return OdAnsiString(sz, CP_UTF_8); }
  inline OdAnsiString toUTF8(const char* sz, int len) { return OdAnsiString(sz, len, CP_UTF_8); }

  class FilePx;
  typedef OdSmartPtr<FilePx>  FilePxP;
  typedef OdRxClass           Class;
  typedef OdRxClassPtr        ClassP;
  typedef OdRxDictionary      Dictionary;
  typedef OdSmartPtr<Dictionary> DictionaryP;

  class Stream : public OdStreamBuf {
    typedef OdStreamBuf Super;
  public:
    using Super::seek;
    Stream& seek(OdUInt64 pos);
    OdUInt64 size();
    inline OdAnsiString toStr();
    inline void putStr(const OdString& str);
    inline void putStr(const OdAnsiString& str);
  };
  typedef OdSmartPtr<Stream> StreamP;

  namespace File {
    enum Access {
      Read     = Oda::kFileRead,  // Read access. Use kFileRead | kFileWrite for read/write access.
      Write    = Oda::kFileWrite, // Write access. Use kFileRead | kFileWrite for read/write access.
      Tmp      = Oda::kFileTmp,   // hint to use FILE_ATTRIBUTE_TEMPORARY  for 'FlagsAndAttributes'
      Delete   = Oda::kFileDelete,// hint to use FILE_FLAG_DELETE_ON_CLOSE for 'FlagsAndAttributes'
      Test     = 1, // Do not open just check existence
      IfExists = 2  // Supresses OdError_FileNotFound() exception; open() will return NULL instead
    };
    enum Share {
      DenyReadWrite = Oda::kShareDenyReadWrite,
      DenyWrite     = Oda::kShareDenyWrite,
      DenyRead      = Oda::kShareDenyRead,
      DenyNo        = Oda::kShareDenyNo
    };
    enum Creation {
      CreateNew        = Oda::kCreateNew       ,  // Creates a new file; fails if the specified file already exists.
      CreateAlways     = Oda::kCreateAlways    ,  // Creates a new file; overwrites any existing file.
      OpenExisting     = Oda::kOpenExisting    ,  // Opens the file; fails if the file does not exist.
      OpenAlways       = Oda::kOpenAlways      ,  // Opens the file; creates the file if it does not exist.
      TruncateExisting = Oda::kTruncateExisting   // Truncates the file; fails if the file does not exist. The file must be open at least kFileWrite.
    };

    typedef OdUInt64 Id;
    typedef OdTfDigest Hash;

    typedef OdSmartPtr<Stream> ReadBufP;

    class WriteBuf : public Stream {
    public:
      virtual Id endWrite(Hash* hash = 0) = 0;
    };
    typedef OdSmartPtr<WriteBuf> WriteBufP;

    inline bool access(const OdString& path, int acc = Read);
    inline StreamP open(const OdString& path, int acc = Read, Creation dispos = OpenExisting, Share sh = DenyNo);

    inline OdAnsiString toStr(const OdString& path);

    inline void putStr(const OdString& path, const OdString& str);
    inline void putStr(const OdString& path, const OdAnsiString& str);
  };

  class RxFS;
  typedef OdSmartPtr<RxFS> RxFSP;
  class FIRSTDLL_EXPORT ODRX_ABSTRACT RxFS : public OdRxObject {
  public:
    ODRX_DECLARE_MEMBERS(RxFS);

    static RxFSP create(const OdString& pathToData = OdString::kEmpty);
    virtual void attach(const OdString& pathToData) = 0;

    StreamP open(const OdString& path,
      int access = File::Read,
      File::Share sh = File::DenyReadWrite,
      File::Creation dispos = File::OpenExisting
      );

    virtual File::Id fileId(const File::Hash& hsh) = 0;
    virtual File::Id fileId(const OdString& path) = 0;
    /**/        bool exists(const OdString& path, File::Hash& outHash);
    virtual bool fileHash(File::Id fid, File::Hash& outHash) = 0;
    virtual void mount(File::Id fid, const OdString& path) = 0;
    /**/    void unmount(File::Id fid) { mount(fid, OdString::kEmpty); }

    virtual File::ReadBufP openR(File::Id file) = 0;

    File::ReadBufP openR(const OdString& path) {
      File::Id fid = fileId(path);
      if(!fid)
        throw OdError_FileNotFound(path);
      return openR(fid);
    }

    File::ReadBufP openR(const File::Hash& hsh) {
      File::Id fid = fileId(hsh);
      if(!fid)
        throw OdError(eKeyNotFound);
      return openR(fid);
    }
    virtual File::WriteBufP openW(const OdString& path = OdString::kEmpty, OdInt64 fixedLength = -1) = 0;
    virtual File::Id put(Stream* data, File::Hash* out, const OdString& mountPath = OdString::kEmpty) = 0;

    void put(const OdString& path, const char* msg, OdUInt64 len = -1, File::Creation dispos = File::CreateAlways);

    static RxFSP byMountPath(const OdString& storageMountPath) {
      return File::open(storageMountPath, File::Read|File::Write, File::OpenExisting);
    }

  };

  /** \details
  This class is the base class for classes that provide platform-dependent file operations for
  Teigha.

  Library: TD_Root

  <group OdRx_Classes>
  */
  class FIRSTDLL_EXPORT ODRX_ABSTRACT FilePx : public OdRxObject {
  public:
    ODRX_DECLARE_MEMBERS(FilePx);

    ///////////////// FILE SYSTEM SERVICES //////////////////

    static OdRxObjectPtr mount(OdRxObject* fs, const OdString& mountPath = OdString::kEmpty);
    static OdRxObjectPtr unmount(const OdString& mountPath);

    /** \details
    Creates and/or opens the specified file.
    \param obj [in]  VFS object (file/directory) with this protocol extension.
    \param path [in]  path in VFS to resource.
    \param accessMode [in]  Access mode.
    \param shareMode [in]  Share mode.
    \param creationDisposition [in]  Creation disposition.

    \remarks
    Returns a SmartPointer to an OdStreamBuf object for the file. 

    accessMode must be a combination of one or more of the following:

    <table>
    Name                Value          Description
    OD::File::Read      0x80000000     Read access.
    OD::File::Write     0x40000000     Write access.
    OD::File::Delete    0x10000000     Delete on close.
    OD::File::IsDir     0x08000000     Is directory.
    </table>

    shareMode must be one of the following:

    <table>
    Name                      Value       Description
    OD::File::DenyReadWrite   0x10        deny read/write mode
    OD::File::DenyWrite       0x20        deny write mode
    OD::File::DenyRead        0x30        deny read mode
    OD::File::DenyNo          0x40        deny none mode
    </table>

    Creation disposition must be one of the following:

    <table>
    Name                          Value     Description
    OD::Filer::CreateNew          1         Creates a new file; fails if the specified file already exists.
    OD::Filer::CreateAlways       2         Creates a new file; overwrites any existing file.
    OD::Filer::OpenExisting       3         Opens the file; fails if the file does not exist.
    OD::Filer::OpenAlways         4         Opens the file; creates the file if it does not exist.
    OD::Filer::TruncateExisting   5         Truncates the file; fails if the file does not exist. The file must be open at least Oda::kFileWrite.
    </table>
    */

    virtual OdRxObjectPtr open(OdRxObject* obj, const char* path,
      File::Access accessMode = File::Read,
      File::Share shareMode = File::DenyNo,
      File::Creation disposition = File::OpenExisting) = 0;
  };




  inline Stream& Stream::seek(OdUInt64 pos) { ((OdStreamBuf*)this)->seek(pos, OdDb::kSeekFromStart); return *this; }

  inline OdUInt64 Stream::size() { return length(); }

  inline OdAnsiString Stream::toStr() {
    OdAnsiString res;
    OdUInt64 len = length();
    rewind();
    getBytes(res.getBuffer(len), len);
    res.releaseBuffer(len);
    return res;
  }
  inline void Stream::putStr(const OdString& str) { putStr(toUTF8(str)); }
  inline void Stream::putStr(const OdAnsiString& str) { putBytes(str.c_str(), str.getLength()); }


  inline bool File::access(const OdString& path, int acc) {
    return ::odrxSystemServices()->accessFile(path, acc);
}

  inline StreamP File::open(const OdString& path, int access, Creation dispos, Share sh) {
    return ::odrxSystemServices()->createFile(path,
      Oda::FileAccessMode(access), Oda::FileShareMode(sh), Oda::FileCreationDisposition(dispos));
  }

  inline OdAnsiString File::toStr(const OdString& path) {
    if(!access(path, File::Read))
      return "";
    return open(path, File::Read, File::OpenExisting, File::DenyReadWrite)->toStr();
  }
  inline void File::putStr(const OdString& path, const OdString& str) { putStr(path, toUTF8(str)); }

  inline void File::putStr(const OdString& path, const OdAnsiString& str) {
    open(path, File::Write, File::OpenAlways)->putStr(str);
  }

  inline OD::RxFSP RxFS::create(const OdString& pathToData) {
    ::odrxDynamicLinker()->loadApp("RxFS", false);
    OD::RxFSP fs = odrxCreateObject("RxFS");
    if(fs.isNull())
      throw OdError("Class RxFS is not found");
    fs->attach(pathToData);
    return fs;
  }

  inline StreamP RxFS::open(const OdString& path, int access, File::Share sh, File::Creation dispos) {
//#   pragma MARKMESSAGE(dna: todo: take file creation disposition into account)
    if(GETBIT(access, File::Write))
      return openW(path);
    return openR(path);
  }

  inline void RxFS::put(const OdString& path, const char* msg, OdUInt64 len, File::Creation dispos)
  { (StreamP(open(path, File::Write, File::DenyReadWrite, dispos)))->putBytes(msg, len); }

  inline bool RxFS::exists(const OdString& path, File::Hash& outHash) {
    File::Id fid = fileId(path);
    if(fid)
      return fileHash(fid, outHash);
    return false;
  }
}


#endif //#define _OD_RxFS_INCLUDED_
