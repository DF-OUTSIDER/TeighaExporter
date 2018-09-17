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

#ifndef _TXDEFS_H_
#define _TXDEFS_H_


#include "RxDictionary.h"
#include "OdStreamBuf.h"
#include "DynamicLinker.h"


#include "OdPlatformSettings.h"
#include "StringArray.h"
#include "RxVariantValue.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdFunctionIO.h"
#include "Path.h"
#include "OdCharMapper.h"


#define TXAPP_FULLNAME "Host Application"
#define TXHOST_CURR_CMDCTX "TXHOST_CURR_CMDCTX"
#define TXHOST_SETTINGS_NAME "TxHostStorage"


namespace Oda { // shortened Teigha identifiers

  typedef   OdUInt8         UI8;
  typedef   OdUInt16        UI16;
  typedef   OdUInt32        UI32;
  typedef   OdInt32          I32;
  typedef   OdUInt64        UI64;
  typedef   OdInt64          I64;

  typedef OdError           EXCEP;
  typedef const EXCEP&      EXCEPREF;

  typedef OdAnsiString      ASTR;
  typedef const ASTR&       ASTRREF;
  typedef OdString          WSTR;
  typedef const WSTR&       WSTRREF;
  typedef const char*       ACSZ;
  typedef const OdChar*     WCSZ;

  typedef OdRxObject        RXOBJ;
  typedef OdRxObjectPtr     RXPTR;
  typedef OdStreamBuf       STRMBF;
  typedef OdStreamBufPtr    STRMBFPTR;


  typedef OD::Path PATH;
  typedef const PATH& PATHREF;


  typedef OdRxDictionary RXDIC;
  typedef OdRxDictionaryPtr RXDICPTR;
  typedef OdRxDictionaryIterator RXDICITER;
  typedef OdRxDictionaryIteratorPtr RXDICITERPTR;

  typedef OdRxVariant       RXVAR;
  typedef OdRxVariantPtr    RXVARPTR;
  typedef OdRxVariantValue  HRXVAR;
  typedef OdRxObjectPtrArray RXPTRARR;


  typedef OdEdCommandContext      CMDCTX;
  typedef OdEdCommandContextPtr   CMDCTXPTR;
  typedef OdEdCommand             CMD;
  typedef OdEdCommandPtr          CMDPTR;
  typedef OdEdUserIO              USERIO;


  class HRXDIC : public RXDICPTR {
  protected:
    inline void validate();
  public:
    HRXDIC();
    HRXDIC(const RXOBJ* dictionary);
    HRXDIC& operator=(const RXOBJ* dictionary);

    static HRXDIC create();
    static HRXDIC arbDataOpen(CMDCTX* cmdctx, WSTRREF name, bool bCreateIfNotFound = false);
    HRXDIC step(WSTRREF name, bool bCreateIfNotFound = false);
    HRXDIC walk(const PATH& path, bool bCreateIfNotFound = false);
    HRXDIC stepCr(WSTRREF name);
    HRXDIC walkCr(const PATH& path);
    RXPTR getAt(WSTRREF name);
    RXPTR putAt(WSTRREF name, RXOBJ* obj);
    RXPTR getAt(UI32 indx);
    RXPTR putAt(UI32 indx, RXOBJ* obj);
    UI32 index(WSTRREF name);
    RXPTR putAt(WSTRREF name, RXOBJ* obj, UI32& indx);
    RXPTR remove(WSTRREF alias);
  };


  enum {
    kCmdGlobal    = OdEdCommandStack::kGlobal,    // Search global (untranslated) names.
    kCmdLocal     = OdEdCommandStack::kLocal,     // Search local (translated) names.
    kCmdSpecGroup = OdEdCommandStack::kSpecifedGroup, // Search in specified group only.
    kCmdThrowUnknown = OdEdCommandStack::kThrowUnknown // Do not fire unknownCommand() event.
  };

  class HRXCMD {
    OdEdCommandPtr fn;
  public:
    HRXCMD(WSTRREF func, int lookup = kCmdGlobal|kCmdLocal|kCmdThrowUnknown, WSTRREF group = WSTR::kEmpty);
    void execute(CMDCTX* ctx);
    RXPTR callFn(CMDCTX* ctx);
    RXPTR callFn(CMDCTX* ctx, RXOBJ* param);
    bool isNull() const { return fn.isNull(); }
  };


  inline HRXCMD::HRXCMD(WSTRREF func, int lookup, WSTRREF group) {
    fn = odedRegCmds()->lookupCmd(func, lookup, group);
    if(fn.isNull() && GETBIT(lookup, kCmdThrowUnknown))
      throw OdError("unknown function " + group + "." + func);
  }

  inline void HRXCMD::execute(CMDCTX* ctx) { odedRegCmds()->executeCommand(fn, ctx); }

  inline RXPTR HRXCMD::callFn(CMDCTX* ctx) {
    fn->execute(ctx);
    OdEdFunctionIO* fnio = ctx->funcIO();
    RXPTR res = fnio->result();
    fnio->setResult(0);
    return res;
  }

  inline RXPTR HRXCMD::callFn(CMDCTX* ctx, RXOBJ* param) {
    OdEdFunctionIO* fnio = ctx->funcIO();
    fnio->setParam(param);
    fn->execute(ctx);
    RXPTR res = fnio->result();
    fnio->setParam(0);
    fnio->setResult(0);
    return res;
  }



  inline RXPTR arbVarSafe(OdEdCommandContext* ctx, const WSTR& varName) {
    OdRxObjectPtr res = ctx->arbitraryData( varName );
    if(res.isNull())
      throw OdError(OdString().format(L"Context variable \"%ls\" was not found", varName.c_str()));
    return res;
  }




  inline void HRXDIC::validate() { if(isNull()) throw EXCEP(eNullObjectPointer); }

  inline HRXDIC::HRXDIC() { }
  inline HRXDIC::HRXDIC(const RXOBJ* dictionary) : RXDICPTR(dictionary) { }
  inline HRXDIC& HRXDIC::operator=(const RXOBJ* dictionary) {
    assign(dictionary); return *this;
  }

  inline HRXDIC HRXDIC::create() {
    return (RXDIC*)RXDIC::createObject().get();
  }

  inline HRXDIC HRXDIC::arbDataOpen(CMDCTX* cmdctx, WSTRREF name, bool bCreateIfNotFound) { // open or create if not found
    RXDICPTR dic = cmdctx->arbitraryData(name);
    if(dic.isNull()) {
      dic = RXDIC::createObject();
      cmdctx->setArbitraryData(name, dic);
    }
    return HRXDIC(dic);
  }
  inline HRXDIC HRXDIC::step(WSTRREF name, bool bCreateIfNotFound) {
    validate();
    HRXDIC nextdic = getAt(name).get();
    if(nextdic.isNull() && bCreateIfNotFound) {
      nextdic = RXDIC::createObject();
      putAt(name, nextdic);
    }
    return nextdic;
  }
  inline HRXDIC HRXDIC::walk(const PATH& path, bool bCreateIfNotFound) {
    validate();
    HRXDIC nextdic = get();
    for(OdUInt32 i=0; nextdic.get() && i<path.size(); ++i)
      nextdic = nextdic.step(WSTR(path[i]), bCreateIfNotFound);
    return nextdic;
  }
  inline HRXDIC HRXDIC::stepCr(WSTRREF name) { return step(name, true); }

  inline HRXDIC HRXDIC::walkCr(const PATH& path) { return walk(path, true); }

  inline RXPTR HRXDIC::getAt(WSTRREF name) { validate(); return get()->getAt(name); }

  inline RXPTR HRXDIC::putAt(WSTRREF name, RXOBJ* obj) { validate(); return get()->putAt(name, obj); }

  inline RXPTR HRXDIC::remove(WSTRREF name) { validate(); return get()->remove(name); }

  inline RXPTR HRXDIC::getAt(UI32 indx) { validate(); return get()->getAt(indx); }

  inline RXPTR HRXDIC::putAt(UI32 indx, RXOBJ* obj) { validate(); return get()->putAt(indx, obj); }

  inline UI32 HRXDIC::index(WSTRREF name) { validate(); return get()->idAt(name); }

  inline RXPTR HRXDIC::putAt(WSTRREF name, RXOBJ* obj, UI32& indx) { validate(); return get()->putAt(name, obj, &indx); }


  inline OdString getDbCtxName(OdEdCommandContext* ctx) {
    OdRxObjectPtr db = ctx->baseDatabase();
    if (db.isNull())
      throw OdError(eNoDatabase);

    OdString dbName = db->isA()->name();
    if (dbName == "OdDbDatabase") {
      return "DwgCtx";
    }
    else if (dbName == "OdDgDatabase") {
      return "DgnCtx";
    }
    //else if (dbName == "OdDwfDatabase"){
    //  return "OdDwfDatabase";
    //}
    //else if (dbName == "OdPrcFile") { // *AutoUpdateReactor commands not implemented
    //  return "PrcCtx";
    //}
    //else if (dbName == "OdBmDatabase") // DNA: AutoUpdateReactor will not work with BIM because OdBmDatabaseReactor has no goodby() callback method
    //  return "BimCtx";
    return OdString();
  }


  inline OdString ctxByFileExtension(const OdString& sExt) {
    if (sExt.iCompare(L"dwg") == 0 ||
      sExt.iCompare(L"dxf") == 0 ||
      sExt.iCompare(L"dxb") == 0 ||
      sExt.iCompare(L"dws") == 0 ||
      sExt.iCompare(L"tvc") == 0 ||
      sExt.iCompare(L"tig") == 0 ||
      sExt.iCompare(L"dwt") == 0) {
      return OdString("DwgCtx");
    }
    HRXDIC txVars = HRXDIC(::odrxSysRegistry()).walkCr("FileFormats");
    OdRxObjectPtr var = txVars.getAt(sExt);
    if (var.isNull())
      throw OdError(eUnknownFileType);

    OdString sFmtBrdgAppName = HRXVAR(var)->getString();
    ::odrxDynamicLinker()->loadApp(sFmtBrdgAppName, false);
    return sFmtBrdgAppName;
  }


  inline OdString toUtf16(const char* pAnsi) {
    OdAnsiString asCmd(pAnsi);
    OdCharArray buf;
    OdCharMapper::utf8ToUnicode(asCmd.c_str(), asCmd.getLength(), buf);
    OdString sRes = OdString(buf.getPtr(), buf.size() - 1);
    // possible TODO substitutions for & ...
    return sRes;
  }

  inline OdAnsiString toUtf8(const OdString& sUnicode) {
    OdAnsiCharArray dstBuf;
    int lenStr = sUnicode.getLength();
    dstBuf.reserve(lenStr * 4 + 1);
    OdCharMapper::unicodeToUtf8(sUnicode.c_str(), lenStr, dstBuf);
    OdAnsiString asRes = (const char*) dstBuf.asArrayPtr();
    // possible TODO substitutions for & ...
    return asRes;
  }

} //namespace Oda



#endif //#ifndef _TXDEFS_H_
