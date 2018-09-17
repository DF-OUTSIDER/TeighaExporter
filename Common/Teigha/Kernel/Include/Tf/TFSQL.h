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
#ifndef _OD_TFSQL_INCLUDED_
#define _OD_TFSQL_INCLUDED_


#include "RxObject.h"
#include "OdStreamBuf.h"
#include "RxIterator.h"
#include "RxFS.h"
#include "TfIO.h"
#include "DynamicLinker.h"

#ifndef OD_SQLITE3_DEFINED
typedef int (*sqlite3_callback)(void*,int,char**, char**);
#endif

// pasts static constant string and it's length to function call parameter list
#define ODSZ_CONS(str_const) str_const,(sizeof(str_const)-1)

namespace OD {
  class TFSQL;
  typedef OdSmartPtr<TFSQL> TFSQLP;

  class FIRSTDLL_EXPORT TFSQL : public OdRxObject {
  public:
    ODRX_DECLARE_MEMBERS(TFSQL);

    static TFSQLP create(const OdString& pathToData = OdString::kEmpty);
    virtual void attach(const OdString& pathToData) = 0;
    virtual OdString path() const = 0;
    virtual void close() = 0;

    class Statement;
    typedef OdSmartPtr<Statement> StatementP;

    typedef OdStreamBuf Blob;
    typedef OdSmartPtr<Blob> BlobP;

    /*   */ void beginTransaction() { exe("BEGIN;PRAGMA read_uncommitted=true;"); }
    /*   */ void endTransaction() { exe("COMMIT"); }
    /*   */ void abortTransaction() { exe("ROLLBACK"); }


    virtual BlobP open   (const char* tab, const char* col, OdInt64 row, int mode = File::Read, const char* db = "main") = 0;
    /*   */ BlobP openR  (const char* tab, const char* col, OdInt64 row, const char* db = "main") {return open(tab,col,row,File::Read              ,db);}
    /*   */ BlobP openW  (const char* tab, const char* col, OdInt64 row, const char* db = "main") {return open(tab,col,row,File::Write             ,db);}
    /*   */ BlobP openRW (const char* tab, const char* col, OdInt64 row, const char* db = "main") {return open(tab,col,row,File::Read|File::Write  ,db);}
    /*  *///void  touch  (const char* tab, const char* col, OdInt64 row, const char* db = "main") {return blob(tab,col,row,Blob::Touch             ,db);}

    class Statement : public OdRxObject {
    public:
      class Row : public OdRxObject {
      public:
        virtual bool          isNull(int i) = 0;
        virtual bool          Bool  (int i) = 0;
        virtual OdUInt8       int8  (int i) = 0;
        virtual OdUInt16      int16 (int i) = 0;
        virtual OdUInt32      int32 (int i) = 0; 
        virtual OdUInt64      int64 (int i) = 0;
        virtual double        realD (int i) = 0;
        virtual float         real  (int i) = 0;
        virtual OdString      utf16 (int i) = 0;
        virtual OdAnsiString  utf8  (int i) = 0;
        virtual const void*   blob  (int i, OdUInt64 *len = 0) = 0;
        virtual BlobP         open  (int i, File::Access acc = File::Read) = 0;
      };
      typedef OdSmartPtr<Row> RowP; 

      class RowIterator : public OdRxIterator {
      public:
        Row& row() { return (Row&)*object().get(); }
      };
      typedef OdSmartPtr<RowIterator> RowIteratorP;
      typedef RowIterator Result;
      typedef OdSmartPtr<Result> ResultP; 

      class Params : public OdRxObject {
      public:
        //virtual Params& setNull(int at) = 0;
        virtual Params& set(int at, bool      val) = 0;
        virtual Params& set(int at, OdUInt8   val) = 0;
        virtual Params& set(int at, OdUInt16  val) = 0;
        virtual Params& set(int at, OdUInt32  val) = 0;
        virtual Params& set(int at, OdUInt64  val) = 0;
        virtual Params& set(int at, double    val) = 0;
        virtual Params& set(int at, const OdString& val) = 0;
        virtual Params& set(int at, const OdAnsiString& val) = 0;
        virtual Params& set(int at, const void* val, OdUInt64 len) = 0;
        virtual Params& set(int at, OdStreamBuf* val) = 0;
        virtual ResultP exe() = 0;
      };
      typedef OdSmartPtr<Params> ParamsP;

      virtual ParamsP reset(const char* sql = 0, int len = -1) = 0;
    };
    typedef OdSmartPtr<Statement::Row> StatementRow;


    /*
    ** A function to executes one or more statements of SQL.
    **
    ** If one or more of the SQL statements are queries, then
    ** the callback function specified by the 3rd parameter is
    ** invoked once for each row of the query result.  This callback
    ** should normally return 0.  If the callback returns a non-zero
    ** value then the query is aborted, all subsequent SQL statements
    ** are skipped and the sqlite3_exec() function returns the SQLITE_ABORT.
    **
    ** The 1st parameter is an arbitrary pointer that is passed
    ** to the callback function as its first parameter.
    **
    ** The 2nd parameter to the callback function is the number of
    ** columns in the query result.  The 3rd parameter to the callback
    ** is an array of strings holding the values for each column.
    ** The 4th parameter to the callback is an array of strings holding
    ** the names of each column.
    **
    ** The callback function may be NULL, even for queries.  A NULL
    ** callback is not an error.  It just means that no callback
    ** will be invoked.
    */
    virtual void exe(const char* sql, sqlite3_callback cb, void* callbackctx) = 0;
    class SQLReadCallback : public OdRxObject { public: virtual bool on(StatementRow& row) = 0; };
    virtual void exe(const char* sql, SQLReadCallback* cb = 0) = 0;
    virtual OdUInt64 lastInsertRowId() = 0;

    virtual Statement::ParamsP prepare(const char* sql, int at = -1, StatementP* stmt = 0) = 0;
    Statement::Params& prepareOrReset(const char* sql, int len, StatementP& stmt) {
      if(stmt.isNull())
        return *prepare(sql, len, &stmt);
      return *stmt->reset();
    }
  };

  inline OD::TFSQLP TFSQL::create(const OdString& pathToData) {
    ::odrxDynamicLinker()->loadApp("TFSQLite", false);
    OD::TFSQLP strg = odrxCreateObject("TFSQLite");
    if(strg.isNull())
      throw OdError_FileNotFound("rx:TFSQLite.tx/TFSQLite");
    strg->attach(pathToData);
    return strg;
  }
}
typedef OD::TFSQL::Blob                 OdTFSQLBlob;
typedef OdSmartPtr<OD::TFSQL::Blob>     OdTFSQLBlobPtr;
typedef OD::TFSQL::Statement            OdTFSQLStatement;
typedef OdSmartPtr<OdTFSQLStatement>    OdTFSQLStatementPtr;
typedef OD::TFSQL::SQLReadCallback      OdTFSQLReadCallback;


#endif // _OD_TFSQL_INCLUDED_
