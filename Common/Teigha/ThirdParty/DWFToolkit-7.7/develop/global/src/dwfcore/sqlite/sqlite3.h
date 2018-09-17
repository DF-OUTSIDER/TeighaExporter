/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This header file defines the interface that the SQLite library
** presents to client programs.
**
** @(#) $Id: //DWF/Development/Components/Internal/DWF Core/v1.7/develop/global/src/dwfcore/sqlite/sqlite3.h#1 $
*/
#ifndef _SQLITE3_H_
#define _SQLITE3_H_
#include <stdarg.h>     /* Needed for the definition of va_list */

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

/*
** The version of the SQLite library.
*/
#ifdef SQLITE_VERSION
# undef SQLITE_VERSION
#endif
#define SQLITE_VERSION         "3.3.17"

/*
** The format of the version string is "X.Y.Z<trailing string>", where
** X is the major version number, Y is the minor version number and Z
** is the release number. The trailing string is often "alpha" or "beta".
** For example "3.1.1beta".
**
** The SQLITE_VERSION_NUMBER is an integer with the value 
** (X*100000 + Y*1000 + Z). For example, for version "3.1.1beta", 
** SQLITE_VERSION_NUMBER is set to 3001001. To detect if they are using 
** version 3.1.1 or greater at compile time, programs may use the test 
** (SQLITE_VERSION_NUMBER>=3001001).
*/
#ifdef SQLITE_VERSION_NUMBER
# undef SQLITE_VERSION_NUMBER
#endif
#define SQLITE_VERSION_NUMBER 3003017

/*
** The version string is also compiled into the library so that a program
** can check to make sure that the lib*.a file and the *.h file are from
** the same version.  The sqlite3_libversion() function returns a pointer
** to the sqlite3_version variable - useful in DLLs which cannot access
** global variables.
*/
extern const char sqlite3_version[];
const char *sqlite3_libversion(void);

/*
** Return the value of the SQLITE_VERSION_NUMBER macro when the
** library was compiled.
*/
int sqlite3_libversion_number(void);

/*
** Each open sqlite database is represented by an instance of the
** following opaque structure.
*/
typedef struct sqlite3 sqlite3;


/*
** Some compilers do not support the "long long" datatype.  So we have
** to do a typedef that for 64-bit integers that depends on what compiler
** is being used.
*/
#ifdef SQLITE_INT64_TYPE
  typedef SQLITE_INT64_TYPE sqlite_int64;
  typedef unsigned SQLITE_INT64_TYPE sqlite_uint64;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
  typedef __int64 sqlite_int64;
  typedef unsigned __int64 sqlite_uint64;
#else
  typedef long long int sqlite_int64;
  typedef unsigned long long int sqlite_uint64;
#endif

/*
** If compiling for a processor that lacks floating point support,
** substitute integer for floating-point
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# define double sqlite_int64
#endif

/*
** A function to close the database.
**
** Call this function with a pointer to a structure that was previously
** returned from sqlite3_open() and the corresponding database will by closed.
**
** All SQL statements prepared using sqlite3_prepare() or
** sqlite3_prepare16() must be deallocated using sqlite3_finalize() before
** this routine is called. Otherwise, SQLITE_BUSY is returned and the
** database connection remains open.
*/
int sqlite3_close(sqlite3 *);

/*
** The type for a callback function.
*/
typedef int (*sqlite3_callback)(void*,int,char**, char**);

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
**
** If an error occurs while parsing or evaluating the SQL (but
** not while executing the callback) then an appropriate error
** message is written into memory obtained from malloc() and
** *errmsg is made to point to that message.  The calling function
** is responsible for freeing the memory that holds the error
** message.   Use sqlite3_free() for this.  If errmsg==NULL,
** then no error message is ever written.
**
** The return value is is SQLITE_OK if there are no errors and
** some other return code if there is an error.  The particular
** return value depends on the type of error. 
**
** If the query could not be executed because a database file is
** locked or busy, then this function returns SQLITE_BUSY.  (This
** behavior can be modified somewhat using the sqlite3_busy_handler()
** and sqlite3_busy_timeout() functions below.)
*/
int sqlite3_exec(
  sqlite3*,                     /* An open database */
  const char *sql,              /* SQL to be executed */
  sqlite3_callback,             /* Callback function */
  void *,                       /* 1st argument to callback function */
  char **errmsg                 /* Error msg written here */
);

/*
** Return values for sqlite3_exec() and sqlite3_step()
*/
#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* NOT USED. Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* NOT USED. Table or record not found */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* NOT USED. Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* NOT USED. Too much data for one row */
#define SQLITE_CONSTRAINT  19   /* Abort due to contraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
/* end-of-error-codes */

/*
** Using the sqlite3_extended_result_codes() API, you can cause
** SQLite to return result codes with additional information in
** their upper bits.  The lower 8 bits will be the same as the
** primary result codes above.  But the upper bits might contain
** more specific error information.
**
** To extract the primary result code from an extended result code,
** simply mask off the lower 8 bits.
**
**        primary = extended & 0xff;
**
** New result error codes may be added from time to time.  Software
** that uses the extended result codes should plan accordingly and be
** sure to always hHandle new unknown codes gracefully.
**
** The SQLITE_OK result code will never be extended.  It will always
** be exactly zero.
**
** The extended result codes always have the primary result code
** as a prefix.  Primary result codes only contain a single "_"
** character.  Extended result codes contain two or more "_" characters.
*/
#define SQLITE_IOERR_READ          (SQLITE_IOERR | (1<<8))
#define SQLITE_IOERR_SHORT_READ    (SQLITE_IOERR | (2<<8))
#define SQLITE_IOERR_WRITE         (SQLITE_IOERR | (3<<8))
#define SQLITE_IOERR_FSYNC         (SQLITE_IOERR | (4<<8))
#define SQLITE_IOERR_DIR_FSYNC     (SQLITE_IOERR | (5<<8))
#define SQLITE_IOERR_TRUNCATE      (SQLITE_IOERR | (6<<8))
#define SQLITE_IOERR_FSTAT         (SQLITE_IOERR | (7<<8))
#define SQLITE_IOERR_UNLOCK        (SQLITE_IOERR | (8<<8))
#define SQLITE_IOERR_RDLOCK        (SQLITE_IOERR | (9<<8))
#define SQLITE_IOERR_DELETE        (SQLITE_IOERR | (10<<8))

/*
** Enable or disable the extended result codes.
*/
int sqlite3_extended_result_codes(sqlite3*, int onoff);

/*
** Each entry in an SQLite table has a unique integer key.  (The key is
** the value of the INTEGER PRIMARY KEY column if there is such a column,
** otherwise the key is generated automatically.  The unique key is always
** available as the ROWID, OID, or _ROWID_ column.)  The following routine
** returns the integer key of the most recent insert in the database.
*/
sqlite_int64 sqlite3_last_insert_rowid(sqlite3*);

/*
** This function returns the number of database rows that were changed
** (or inserted or deleted) by the most recent SQL statement.  Only
** changes that are directly specified by the INSERT, UPDATE, or
** DELETE statement are counted.  Auxiliary changes caused by
** triggers are not counted.   Within the body of a trigger, however,
** the sqlite3_changes() API can be called to find the number of
** changes in the most recently completed INSERT, UPDATE, or DELETE
** statement within the body of the trigger.
**
** All changes are counted, even if they were later undone by a
** ROLLBACK or ABORT.  Except, changes associated with creating and
** dropping tables are not counted.
**
** If a callback invokes sqlite3_exec() or sqlite3_step() recursively,
** then the changes in the inner, recursive call are counted together
** with the changes in the outer call.
**
** SQLite implements the command "DELETE FROM table" without a WHERE clause
** by dropping and recreating the table.  (This is much faster than going
** through and deleting individual elements form the table.)  Because of
** this optimization, the change count for "DELETE FROM table" will be
** zero regardless of the number of elements that were originally in the
** table. To get an accurate count of the number of rows deleted, use
** "DELETE FROM table WHERE 1" instead.
*/
int sqlite3_changes(sqlite3*);

/*
** This function returns the number of database rows that have been
** modified by INSERT, UPDATE or DELETE statements since the database hHandle
** was opened. This includes UPDATE, INSERT and DELETE statements executed
** as part of trigger programs. All changes are counted as soon as the
** statement that makes them is completed (when the statement hHandle is
** passed to sqlite3_reset() or sqlite_finalise()).
**
** SQLite implements the command "DELETE FROM table" without a WHERE clause
** by dropping and recreating the table.  (This is much faster than going
** through and deleting individual elements form the table.)  Because of
** this optimization, the change count for "DELETE FROM table" will be
** zero regardless of the number of elements that were originally in the
** table. To get an accurate count of the number of rows deleted, use
** "DELETE FROM table WHERE 1" instead.
*/
int sqlite3_total_changes(sqlite3*);

/* This function causes any pending database operation to abort and
** return at its earliest opportunity.  This routine is typically
** called in response to a user action such as pressing "Cancel"
** or Ctrl-C where the user wants a long query operation to halt
** immediately.
**
** It is safe to call this routine from a different thread that the
** thread that is currently running the database operation. 
*/
void sqlite3_interrupt(sqlite3*);


/* These functions return true if the given input string comprises
** one or more complete SQL statements. For the sqlite3_complete() call,
** the parameter must be a nul-terminated UTF-8 string. For
** sqlite3_complete16(), a nul-terminated machine byte order UTF-16 string
** is required.
**
** This routine is useful for command-line input to see of the user has
** entered a complete statement of SQL or if the current statement needs
** to be continued on the next line.  The algorithm is simple.  If the 
** last token other than spaces and comments is a semicolon, then return 
** true.  Actually, the algorithm is a little more complicated than that
** in order to deal with triggers, but the basic idea is the same:  the
** statement is not complete unless it ends in a semicolon.
*/
int sqlite3_complete(const char *sql);
int sqlite3_complete16(const void *sql);

/*
** This routine identifies a callback function that is invoked
** whenever an attempt is made to open a database table that is
** currently locked by another process or thread.  If the busy callback
** is NULL, then sqlite3_exec() returns SQLITE_BUSY immediately if
** it finds a locked table.  If the busy callback is not NULL, then
** sqlite3_exec() invokes the callback with two arguments.  The
** first argument to the handler is a copy of the void* pointer which
** is the third argument to this routine.  The second argument to
** the handler is the number of times that the busy handler has
** been invoked for this locking event.  If the
** busy callback returns 0, then sqlite3_exec() immediately returns
** SQLITE_BUSY.  If the callback returns non-zero, then sqlite3_exec()
** tries to open the table again and the cycle repeats.
**
** The presence of a busy handler does not guarantee that
** it will be invoked when there is lock contention.
** If SQLite determines that invoking the busy handler could result in
** a deadlock, it will return SQLITE_BUSY instead.
** Consider a scenario where one process is holding a read lock that
** it is trying to promote to a reserved lock and
** a second process is holding a reserved lock that it is trying
** to promote to an exclusive lock.  The first process cannot proceed
** because it is blocked by the second and the second process cannot
** proceed because it is blocked by the first.  If both processes
** invoke the busy handlers, neither will make any progress.  Therefore,
** SQLite returns SQLITE_BUSY for the first process, hoping that this
** will induce the first process to release its read lock and allow
** the second process to proceed.
**
** The default busy callback is NULL.
**
** Sqlite is re-entrant, so the busy handler may start a new query. 
** (It is not clear why anyone would every want to do this, but it
** is allowed, in theory.)  But the busy handler may not close the
** database.  Closing the database from a busy handler will delete 
** data structures out from under the executing query and will 
** probably result in a coredump.
*/
int sqlite3_busy_handler(sqlite3*, int(*)(void*,int), void*);

/*
** This routine sets a busy handler that sleeps for a while when a
** table is locked.  The handler will sleep multiple times until 
** at least "ms" milleseconds of sleeping have been done.  After
** "ms" milleseconds of sleeping, the handler returns 0 which
** causes sqlite3_exec() to return SQLITE_BUSY.
**
** Calling this routine with an argument less than or equal to zero
** turns off all busy handlers.
*/
int sqlite3_busy_timeout(sqlite3*, int ms);

/*
** This next routine is really just a wrapper around sqlite3_exec().
** Instead of invoking a user-supplied callback for each row of the
** result, this routine remembers each row of the result in memory
** obtained from malloc(), then returns all of the result after the
** query has finished. 
**
** As an example, suppose the query result where this table:
**
**        Name        | Age
**        -----------------------
**        Alice       | 43
**        Bob         | 28
**        Cindy       | 21
**
** If the 3rd argument were &azResult then after the function returns
** azResult will contain the following data:
**
**        azResult[0] = "Name";
**        azResult[1] = "Age";
**        azResult[2] = "Alice";
**        azResult[3] = "43";
**        azResult[4] = "Bob";
**        azResult[5] = "28";
**        azResult[6] = "Cindy";
**        azResult[7] = "21";
**
** Notice that there is an extra row of data containing the column
** headers.  But the *nrow return value is still 3.  *ncolumn is
** set to 2.  In general, the number of values inserted into azResult
** will be ((*nrow) + 1)*(*ncolumn).
**
** After the calling function has finished using the result, it should 
** pass the result data pointer to sqlite3_free_table() in order to 
** release the memory that was malloc-ed.  Because of the way the 
** malloc() happens, the calling function must not try to call 
** free() directly.  Only sqlite3_free_table() is able to release 
** the memory properly and safely.
**
** The return value of this routine is the same as from sqlite3_exec().
*/
int sqlite3_get_table(
  sqlite3*,               /* An open database */
  const char *sql,       /* SQL to be executed */
  char ***resultp,       /* Result written to a char *[]  that this points to */
  int *nrow,             /* Number of result rows written here */
  int *ncolumn,          /* Number of result columns written here */
  char **errmsg          /* Error msg written here */
);

/*
** Call this routine to free the memory that sqlite3_get_table() allocated.
*/
void sqlite3_free_table(char **result);

/*
** The following routines are variants of the "sprintf()" from the
** standard C library.  The resulting string is written into memory
** obtained from malloc() so that there is never a possiblity of buffer
** overflow.  These routines also implement some additional formatting
** options that are useful for constructing SQL statements.
**
** The strings returned by these routines should be freed by calling
** sqlite3_free().
**
** All of the usual printf formatting options apply.  In addition, there
** is a "%q" option.  %q works like %s in that it substitutes a null-terminated
** string from the argument list.  But %q also doubles every '\'' character.
** %q is designed for use inside a string literal.  By doubling each '\''
** character it escapes that character and allows it to be inserted into
** the string.
**
** For example, so some string variable contains text as follows:
**
**      char *zText = "It's a happy day!";
**
** We can use this text in an SQL statement as follows:
**
**      char *z = sqlite3_mprintf("INSERT INTO TABLES('%q')", zText);
**      sqlite3_exec(db, z, callback1, 0, 0);
**      sqlite3_free(z);
**
** Because the %q format string is used, the '\'' character in zText
** is escaped and the SQL generated is as follows:
**
**      INSERT INTO table1 VALUES('It''s a happy day!')
**
** This is correct.  Had we used %s instead of %q, the generated SQL
** would have looked like this:
**
**      INSERT INTO table1 VALUES('It's a happy day!');
**
** This second example is an SQL syntax error.  As a general rule you
** should always use %q instead of %s when inserting text into a string 
** literal.
*/
char *sqlite3_mprintf(const char*,...);
char *sqlite3_vmprintf(const char*, va_list);
char *sqlite3_snprintf(int,char*,const char*, ...);

/*
** SQLite uses its own memory allocator.  On many installations, this
** memory allocator is identical to the standard malloc()/realloc()/free()
** and can be used interchangable.  On others, the implementations are
** different.  For maximum portability, it is best not to mix calls
** to the standard malloc/realloc/free with the sqlite versions.
*/
void *sqlite3_malloc(int);
void *sqlite3_realloc(void*, int);
void sqlite3_free(void*);

#ifndef SQLITE_OMIT_AUTHORIZATION
/*
** This routine registers a callback with the SQLite library.  The
** callback is invoked (at compile-time, not at run-time) for each
** attempt to access a column of a table in the database.  The callback
** returns SQLITE_OK if access is allowed, SQLITE_DENY if the entire
** SQL statement should be aborted with an error and SQLITE_IGNORE
** if the column should be treated as a NULL value.
*/
int sqlite3_set_authorizer(
  sqlite3*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);
#endif

/*
** The second parameter to the access authorization function above will
** be one of the values below.  These values signify what kind of operation
** is to be authorized.  The 3rd and 4th parameters to the authorization
** function will be parameters or NULL depending on which of the following
** codes is used as the second parameter.  The 5th parameter is the name
** of the database ("main", "temp", etc.) if applicable.  The 6th parameter
** is the name of the inner-most trigger or view that is responsible for
** the access attempt or NULL if this access attempt is directly from 
** input SQL code.
**
**                                          Arg-3           Arg-4
*/
#define SQLITE_COPY                  0   /* Table Name      File Name       */
#define SQLITE_CREATE_INDEX          1   /* Index Name      Table Name      */
#define SQLITE_CREATE_TABLE          2   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
#define SQLITE_CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_TEMP_VIEW      6   /* View Name       NULL            */
#define SQLITE_CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_VIEW           8   /* View Name       NULL            */
#define SQLITE_DELETE                9   /* Table Name      NULL            */
#define SQLITE_DROP_INDEX           10   /* Index Name      Table Name      */
#define SQLITE_DROP_TABLE           11   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
#define SQLITE_DROP_TEMP_TABLE      13   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
#define SQLITE_DROP_TEMP_VIEW       15   /* View Name       NULL            */
#define SQLITE_DROP_TRIGGER         16   /* Trigger Name    Table Name      */
#define SQLITE_DROP_VIEW            17   /* View Name       NULL            */
#define SQLITE_INSERT               18   /* Table Name      NULL            */
#define SQLITE_PRAGMA               19   /* Pragma Name     1st arg or NULL */
#define SQLITE_READ                 20   /* Table Name      Column Name     */
#define SQLITE_SELECT               21   /* NULL            NULL            */
#define SQLITE_TRANSACTION          22   /* NULL            NULL            */
#define SQLITE_UPDATE               23   /* Table Name      Column Name     */
#define SQLITE_ATTACH               24   /* Filename        NULL            */
#define SQLITE_DETACH               25   /* Database Name   NULL            */
#define SQLITE_ALTER_TABLE          26   /* Database Name   Table Name      */
#define SQLITE_REINDEX              27   /* Index Name      NULL            */
#define SQLITE_ANALYZE              28   /* Table Name      NULL            */
#define SQLITE_CREATE_VTABLE        29   /* Table Name      Module Name     */
#define SQLITE_DROP_VTABLE          30   /* Table Name      Module Name     */
#define SQLITE_FUNCTION             31   /* Function Name   NULL            */

/*
** The return value of the authorization function should be one of the
** following constants:
*/
/* #define SQLITE_OK  0   // Allow access (This is actually defined above) */
#define SQLITE_DENY   1   /* Abort the SQL statement with an error */
#define SQLITE_IGNORE 2   /* Don't allow access, but don't generate an error */

/*
** Register a function for tracing SQL command evaluation.  The function
** registered by sqlite3_trace() is invoked at the first sqlite3_step()
** for the evaluation of an SQL statement.  The function registered by
** sqlite3_profile() runs at the end of each SQL statement and includes
** information on how long that statement ran.
**
** The sqlite3_profile() API is currently considered experimental and
** is subject to change.
*/
void *sqlite3_trace(sqlite3*, void(*xTrace)(void*,const char*), void*);
void *sqlite3_profile(sqlite3*,
   void(*xProfile)(void*,const char*,sqlite_uint64), void*);

/*
** This routine configures a callback function - the progress callback - that
** is invoked periodically during long running calls to sqlite3_exec(),
** sqlite3_step() and sqlite3_get_table(). An example use for this API is to 
** keep a GUI updated during a large query.
**
** The progress callback is invoked once for every N virtual machine opcodes,
** where N is the second argument to this function. The progress callback
** itself is identified by the third argument to this function. The fourth
** argument to this function is a void pointer passed to the progress callback
** function each time it is invoked.
**
** If a call to sqlite3_exec(), sqlite3_step() or sqlite3_get_table() results 
** in less than N opcodes being executed, then the progress callback is not
** invoked.
** 
** To remove the progress callback altogether, pass NULL as the third
** argument to this function.
**
** If the progress callback returns a result other than 0, then the current 
** query is immediately terminated and any database changes rolled back. If the
** query was part of a larger transaction, then the transaction is not rolled
** back and remains active. The sqlite3_exec() call returns SQLITE_ABORT. 
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
void sqlite3_progress_handler(sqlite3*, int, int(*)(void*), void*);

/*
** Register a callback function to be invoked whenever a new transaction
** is committed.  The pArg argument is passed through to the callback.
** callback.  If the callback function returns non-zero, then the commit
** is converted into a rollback.
**
** If another function was previously registered, its pArg value is returned.
** Otherwise NULL is returned.
**
** Registering a NULL function disables the callback.
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
void *sqlite3_commit_hook(sqlite3*, int(*)(void*), void*);

/*
** Open the sqlite database file "filename".  The "filename" is UTF-8
** encoded for sqlite3_open() and UTF-16 encoded in the native byte order
** for sqlite3_open16().  An sqlite3* hHandle is returned in *ppDb, even
** if an error occurs. If the database is opened (or created) successfully,
** then SQLITE_OK is returned. Otherwise an error code is returned. The
** sqlite3_errmsg() or sqlite3_errmsg16()  routines can be used to obtain
** an English language description of the error.
**
** If the database file does not exist, then a new database is created.
** The encoding for the database is UTF-8 if sqlite3_open() is called and
** UTF-16 if sqlite3_open16 is used.
**
** Whether or not an error occurs when it is opened, resources associated
** with the sqlite3* hHandle should be released by passing it to
** sqlite3_close() when it is no longer required.
*/
int sqlite3_open(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db hHandle */
);
int sqlite3_open16(
  const void *filename,   /* Database filename (UTF-16) */
  sqlite3 **ppDb          /* OUT: SQLite db hHandle */
);

/*
** Return the error code for the most recent sqlite3_* API call associated
** with sqlite3 hHandle 'db'. SQLITE_OK is returned if the most recent 
** API call was successful.
**
** Calls to many sqlite3_* functions set the error code and string returned
** by sqlite3_errcode(), sqlite3_errmsg() and sqlite3_errmsg16()
** (overwriting the previous values). Note that calls to sqlite3_errcode(),
** sqlite3_errmsg() and sqlite3_errmsg16() themselves do not affect the
** results of future invocations.
**
** Assuming no other intervening sqlite3_* API calls are made, the error
** code returned by this function is associated with the same error as
** the strings  returned by sqlite3_errmsg() and sqlite3_errmsg16().
*/
int sqlite3_errcode(sqlite3 *db);

/*
** Return a pointer to a UTF-8 encoded string describing in english the
** error condition for the most recent sqlite3_* API call. The returned
** string is always terminated by an 0x00 byte.
**
** The string "not an error" is returned when the most recent API call was
** successful.
*/
const char *sqlite3_errmsg(sqlite3*);

/*
** Return a pointer to a UTF-16 native byte order encoded string describing
** in english the error condition for the most recent sqlite3_* API call.
** The returned string is always terminated by a pair of 0x00 bytes.
**
** The string "not an error" is returned when the most recent API call was
** successful.
*/
const void *sqlite3_errmsg16(sqlite3*);

/*
** An instance of the following opaque structure is used to represent
** a compiled SQL statment.
*/
typedef struct sqlite3_stmt sqlite3_stmt;

/*
** To execute an SQL query, it must first be compiled into a byte-code
** program using one of the following routines. The only difference between
** them is that the second argument, specifying the SQL statement to
** compile, is assumed to be encoded in UTF-8 for the sqlite3_prepare()
** function and UTF-16 for sqlite3_prepare16().
**
** The first parameter "db" is an SQLite database hHandle. The second
** parameter "zSql" is the statement to be compiled, encoded as either
** UTF-8 or UTF-16 (see above). If the next parameter, "nBytes", is less
** than zero, then zSql is read up to the first nul terminator.  If
** "nBytes" is not less than zero, then it is the length of the string zSql
** in bytes (not characters).
**
** *pzTail is made to point to the first byte past the end of the first
** SQL statement in zSql.  This routine only compiles the first statement
** in zSql, so *pzTail is left pointing to what remains uncompiled.
**
** *ppStmt is left pointing to a compiled SQL statement that can be
** executed using sqlite3_step().  Or if there is an error, *ppStmt may be
** set to NULL.  If the input text contained no SQL (if the input is and
** empty string or a comment) then *ppStmt is set to NULL.
**
** On success, SQLITE_OK is returned.  Otherwise an error code is returned.
*/
int sqlite3_prepare(
  sqlite3 *db,            /* Database hHandle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nBytes,             /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement hHandle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare16(
  sqlite3 *db,            /* Database hHandle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nBytes,             /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement hHandle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);

/*
** Newer versions of the prepare API work just like the legacy versions
** but with one exception:  The a copy of the SQL text is saved in the
** sqlite3_stmt structure that is returned.  If this copy exists, it
** modifieds the behavior of sqlite3_step() slightly.  First, sqlite3_step()
** will no longer return an SQLITE_SCHEMA error but will instead automatically
** rerun the compiler to rebuild the prepared statement.  Secondly, 
** sqlite3_step() now turns a full result code - the result code that
** use used to have to call sqlite3_reset() to get.
*/
int sqlite3_prepare_v2(
  sqlite3 *db,            /* Database hHandle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nBytes,             /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement hHandle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare16_v2(
  sqlite3 *db,            /* Database hHandle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nBytes,             /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement hHandle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);

/*
** Pointers to the following two opaque structures are used to communicate
** with the implementations of user-defined functions.
*/
typedef struct sqlite3_context sqlite3_context;
typedef struct Mem sqlite3_value;

/*
** In the SQL strings input to sqlite3_prepare() and sqlite3_prepare16(),
** one or more literals can be replace by parameters "?" or "?NNN" or 
** ":AAA" or "@AAA" or "$VVV" where NNN is a integer, AAA is an identifer,
** and VVV is a variable name according  to the syntax rules of the
** TCL programming language.  The value of these parameters (also called
** "host parameter names") can be set using the routines listed below.
**
** In every case, the first argument is a pointer to the sqlite3_stmt
** structure returned from sqlite3_prepare().  The second argument is the
** index of the host parameter name.  The first host parameter as an index 
** of 1.  For named host parameters (":AAA" or "$VVV") you can use 
** sqlite3_bind_parameter_index() to get the correct index value given
** the parameter name.  If the same named parameter occurs more than
** once, it is assigned the same index each time.
**
** The fifth argument to sqlite3_bind_blob(), sqlite3_bind_text(), and
** sqlite3_bind_text16() is a destructor used to dispose of the BLOB or
** text after SQLite has finished with it.  If the fifth argument is the
** special value SQLITE_STATIC, then the library assumes that the information
** is in static, unmanaged space and does not need to be freed.  If the
** fifth argument has the value SQLITE_TRANSIENT, then SQLite makes its
** own private copy of the data before the sqlite3_bind_* routine returns.
**
** The sqlite3_bind_* routine must be called before sqlite3_step() and after
** an sqlite3_prepare() or sqlite3_reset().  Bindings persist across
** multiple calls to sqlite3_reset() and sqlite3_step().  Unbound parameters 
** are interpreted as NULL.
*/
int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
int sqlite3_bind_double(sqlite3_stmt*, int, double);
int sqlite3_bind_int(sqlite3_stmt*, int, int);
int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite_int64);
int sqlite3_bind_null(sqlite3_stmt*, int);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);

/*
** Return the number of host parameters in a compiled SQL statement.  This
** routine was added to support DBD::SQLite.
*/
int sqlite3_bind_parameter_count(sqlite3_stmt*);

/*
** Return the name of the i-th name parameter.  Ordinary parameters "?" are
** nameless and a NULL is returned.  For parameters of the form :AAA or
** $VVV the complete text of the parameter name is returned, including
** the initial ":" or "$".  NULL is returned if the index is out of range.
*/
const char *sqlite3_bind_parameter_name(sqlite3_stmt*, int);

/*
** Return the index of a parameter with the given name.  The name
** must match exactly.  If no parameter with the given name is found,
** return 0.
*/
int sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);

/*
** Set all the parameters in the compiled SQL statement to NULL.
*/
int sqlite3_clear_bindings(sqlite3_stmt*);

/*
** Return the number of columns in the result set returned by the compiled
** SQL statement. This routine returns 0 if pStmt is an SQL statement
** that does not return data (for example an UPDATE).
*/
int sqlite3_column_count(sqlite3_stmt *pStmt);

/*
** The first parameter is a compiled SQL statement. This function returns
** the column heading for the Nth column of that statement, where N is the
** second function parameter.  The string returned is UTF-8 for
** sqlite3_column_name() and UTF-16 for sqlite3_column_name16().
*/
const char *sqlite3_column_name(sqlite3_stmt*,int);
const void *sqlite3_column_name16(sqlite3_stmt*,int);

/*
** The first argument to the following calls is a compiled SQL statement.
** These functions return information about the Nth column returned by 
** the statement, where N is the second function argument.
**
** If the Nth column returned by the statement is not a column value,
** then all of the functions return NULL. Otherwise, the return the 
** name of the attached database, table and column that the expression
** extracts a value from.
**
** As with all other SQLite APIs, those postfixed with "16" return UTF-16
** encoded strings, the other functions return UTF-8. The memory containing
** the returned strings is valid until the statement hHandle is finalized().
**
** These APIs are only available if the library was compiled with the 
** SQLITE_ENABLE_COLUMN_METADATA preprocessor symbol defined.
*/
const char *sqlite3_column_database_name(sqlite3_stmt*,int);
const void *sqlite3_column_database_name16(sqlite3_stmt*,int);
const char *sqlite3_column_table_name(sqlite3_stmt*,int);
const void *sqlite3_column_table_name16(sqlite3_stmt*,int);
const char *sqlite3_column_origin_name(sqlite3_stmt*,int);
const void *sqlite3_column_origin_name16(sqlite3_stmt*,int);

/*
** The first parameter is a compiled SQL statement. If this statement
** is a SELECT statement, the Nth column of the returned result set 
** of the SELECT is a table column then the declared type of the table
** column is returned. If the Nth column of the result set is not at table
** column, then a NULL pointer is returned. The returned string is always
** UTF-8 encoded. For example, in the database schema:
**
** CREATE TABLE t1(c1 VARIANT);
**
** And the following statement compiled:
**
** SELECT c1 + 1, c1 FROM t1;
**
** Then this routine would return the string "VARIANT" for the second
** result column (i==1), and a NULL pointer for the first result column
** (i==0).
*/
const char *sqlite3_column_decltype(sqlite3_stmt *, int i);

/*
** The first parameter is a compiled SQL statement. If this statement
** is a SELECT statement, the Nth column of the returned result set 
** of the SELECT is a table column then the declared type of the table
** column is returned. If the Nth column of the result set is not at table
** column, then a NULL pointer is returned. The returned string is always
** UTF-16 encoded. For example, in the database schema:
**
** CREATE TABLE t1(c1 INTEGER);
**
** And the following statement compiled:
**
** SELECT c1 + 1, c1 FROM t1;
**
** Then this routine would return the string "INTEGER" for the second
** result column (i==1), and a NULL pointer for the first result column
** (i==0).
*/
const void *sqlite3_column_decltype16(sqlite3_stmt*,int);

/* 
** After an SQL query has been compiled with a call to either
** sqlite3_prepare() or sqlite3_prepare16(), then this function must be
** called one or more times to execute the statement.
**
** The return value will be either SQLITE_BUSY, SQLITE_DONE, 
** SQLITE_ROW, SQLITE_ERROR, or SQLITE_MISUSE.
**
** SQLITE_BUSY means that the database engine attempted to open
** a locked database and there is no busy callback registered.
** Call sqlite3_step() again to retry the open.
**
** SQLITE_DONE means that the statement has finished executing
** successfully.  sqlite3_step() should not be called again on this virtual
** machine.
**
** If the SQL statement being executed returns any data, then 
** SQLITE_ROW is returned each time a new row of data is ready
** for processing by the caller. The values may be accessed using
** the sqlite3_column_*() functions described below. sqlite3_step()
** is called again to retrieve the next row of data.
** 
** SQLITE_ERROR means that a run-time error (such as a constraint
** violation) has occurred.  sqlite3_step() should not be called again on
** the VM. More information may be found by calling sqlite3_errmsg().
**
** SQLITE_MISUSE means that the this routine was called inappropriately.
** Perhaps it was called on a virtual machine that had already been
** finalized or on one that had previously returned SQLITE_ERROR or
** SQLITE_DONE.  Or it could be the case the the same database connection
** is being used simulataneously by two or more threads.
*/
int sqlite3_step(sqlite3_stmt*);

/*
** Return the number of values in the current row of the result set.
**
** After a call to sqlite3_step() that returns SQLITE_ROW, this routine
** will return the same value as the sqlite3_column_count() function.
** After sqlite3_step() has returned an SQLITE_DONE, SQLITE_BUSY or
** error code, or before sqlite3_step() has been called on a 
** compiled SQL statement, this routine returns zero.
*/
int sqlite3_data_count(sqlite3_stmt *pStmt);

/*
** Values are stored in the database in one of the following fundamental
** types.
*/
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
/* #define SQLITE_TEXT  3  // See below */
#define SQLITE_BLOB     4
#define SQLITE_NULL     5

/*
** SQLite version 2 defines SQLITE_TEXT differently.  To allow both
** version 2 and version 3 to be included, undefine them both if a
** conflict is seen.  Define SQLITE3_TEXT to be the version 3 value.
*/
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

/*
** The next group of routines returns information about the information
** in a single column of the current result row of a query.  In every
** case the first parameter is a pointer to the SQL statement that is being
** executed (the sqlite_stmt* that was returned from sqlite3_prepare()) and
** the second argument is the index of the column for which information 
** should be returned.  iCol is zero-indexed.  The left-most column as an
** index of 0.
**
** If the SQL statement is not currently point to a valid row, or if the
** the colulmn index is out of range, the result is undefined.
**
** These routines attempt to convert the value where appropriate.  For
** example, if the internal representation is FLOAT and a text result
** is requested, sprintf() is used internally to do the conversion
** automatically.  The following table details the conversions that
** are applied:
**
**    Internal Type    Requested Type     Conversion
**    -------------    --------------    --------------------------
**       NULL             INTEGER         Result is 0
**       NULL             FLOAT           Result is 0.0
**       NULL             TEXT            Result is an empty string
**       NULL             BLOB            Result is a zero-length BLOB
**       INTEGER          FLOAT           Convert from integer to float
**       INTEGER          TEXT            ASCII rendering of the integer
**       INTEGER          BLOB            Same as for INTEGER->TEXT
**       FLOAT            INTEGER         Convert from float to integer
**       FLOAT            TEXT            ASCII rendering of the float
**       FLOAT            BLOB            Same as FLOAT->TEXT
**       TEXT             INTEGER         Use atoi()
**       TEXT             FLOAT           Use atof()
**       TEXT             BLOB            No change
**       BLOB             INTEGER         Convert to TEXT then use atoi()
**       BLOB             FLOAT           Convert to TEXT then use atof()
**       BLOB             TEXT            Add a \000 terminator if needed
**
** The following access routines are provided:
**
** _type()     Return the datatype of the result.  This is one of
**             SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB,
**             or SQLITE_NULL.
** _blob()     Return the value of a BLOB.
** _bytes()    Return the number of bytes in a BLOB value or the number
**             of bytes in a TEXT value represented as UTF-8.  The \000
**             terminator is included in the byte count for TEXT values.
** _bytes16()  Return the number of bytes in a BLOB value or the number
**             of bytes in a TEXT value represented as UTF-16.  The \u0000
**             terminator is included in the byte count for TEXT values.
** _double()   Return a FLOAT value.
** _int()      Return an INTEGER value in the host computer's native
**             integer representation.  This might be either a 32- or 64-bit
**             integer depending on the host.
** _int64()    Return an INTEGER value as a 64-bit signed integer.
** _text()     Return the value as UTF-8 text.
** _text16()   Return the value as UTF-16 text.
*/
const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);
sqlite_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
int sqlite3_column_type(sqlite3_stmt*, int iCol);
int sqlite3_column_numeric_type(sqlite3_stmt*, int iCol);
sqlite3_value *sqlite3_column_value(sqlite3_stmt*, int iCol);

/*
** The sqlite3_finalize() function is called to delete a compiled
** SQL statement obtained by a previous call to sqlite3_prepare()
** or sqlite3_prepare16(). If the statement was executed successfully, or
** not executed at all, then SQLITE_OK is returned. If execution of the
** statement failed then an error code is returned. 
**
** This routine can be called at any point during the execution of the
** virtual machine.  If the virtual machine has not completed execution
** when this routine is called, that is like encountering an error or
** an interrupt.  (See sqlite3_interrupt().)  Incomplete updates may be
** rolled back and transactions cancelled,  depending on the circumstances,
** and the result code returned will be SQLITE_ABORT.
*/
int sqlite3_finalize(sqlite3_stmt *pStmt);

/*
** The sqlite3_reset() function is called to reset a compiled SQL
** statement obtained by a previous call to sqlite3_prepare() or
** sqlite3_prepare16() back to it's initial state, ready to be re-executed.
** Any SQL statement variables that had values bound to them using
** the sqlite3_bind_*() API retain their values.
*/
int sqlite3_reset(sqlite3_stmt *pStmt);

/*
** The following two functions are used to add user functions or aggregates
** implemented in C to the SQL langauge interpreted by SQLite. The
** difference only between the two is that the second parameter, the
** name of the (scalar) function or aggregate, is encoded in UTF-8 for
** sqlite3_create_function() and UTF-16 for sqlite3_create_function16().
**
** The first argument is the database hHandle that the new function or
** aggregate is to be added to. If a single program uses more than one
** database hHandle internally, then user functions or aggregates must 
** be added individually to each database hHandle with which they will be
** used.
**
** The third parameter is the number of arguments that the function or
** aggregate takes. If this parameter is negative, then the function or
** aggregate may take any number of arguments.
**
** The fourth parameter is one of SQLITE_UTF* values defined below,
** indicating the encoding that the function is most likely to hHandle
** values in.  This does not change the behaviour of the programming
** interface. However, if two versions of the same function are registered
** with different encoding values, SQLite invokes the version likely to
** minimize conversions between text encodings.
**
** The seventh, eighth and ninth parameters, xFunc, xStep and xFinal, are
** pointers to user implemented C functions that implement the user
** function or aggregate. A scalar function requires an implementation of
** the xFunc callback only, NULL pointers should be passed as the xStep
** and xFinal parameters. An aggregate function requires an implementation
** of xStep and xFinal, but NULL should be passed for xFunc. To delete an
** existing user function or aggregate, pass NULL for all three function
** callback. Specifying an inconstent set of callback values, such as an
** xFunc and an xFinal, or an xStep but no xFinal, SQLITE_ERROR is
** returned.
*/
int sqlite3_create_function(
  sqlite3 *,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void*,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
int sqlite3_create_function16(
  sqlite3*,
  const void *zFunctionName,
  int nArg,
  int eTextRep,
  void*,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);

/*
** This function is deprecated.  Do not use it.  It continues to exist
** so as not to break legacy code.  But new code should avoid using it.
*/
int sqlite3_aggregate_count(sqlite3_context*);

/*
** The next group of routines returns information about parameters to
** a user-defined function.  Function implementations use these routines
** to access their parameters.  These routines are the same as the
** sqlite3_column_* routines except that these routines take a single
** sqlite3_value* pointer instead of an sqlite3_stmt* and an integer
** column number.
*/
const void *sqlite3_value_blob(sqlite3_value*);
int sqlite3_value_bytes(sqlite3_value*);
int sqlite3_value_bytes16(sqlite3_value*);
double sqlite3_value_double(sqlite3_value*);
int sqlite3_value_int(sqlite3_value*);
sqlite_int64 sqlite3_value_int64(sqlite3_value*);
const unsigned char *sqlite3_value_text(sqlite3_value*);
const void *sqlite3_value_text16(sqlite3_value*);
const void *sqlite3_value_text16le(sqlite3_value*);
const void *sqlite3_value_text16be(sqlite3_value*);
int sqlite3_value_type(sqlite3_value*);
int sqlite3_value_numeric_type(sqlite3_value*);

/*
** Aggregate functions use the following routine to allocate
** a structure for storing their state.  The first time this routine
** is called for a particular aggregate, a new structure of size nBytes
** is allocated, zeroed, and returned.  On subsequent calls (for the
** same aggregate instance) the same buffer is returned.  The implementation
** of the aggregate can use the returned buffer to accumulate data.
**
** The buffer allocated is freed automatically by SQLite.
*/
void *sqlite3_aggregate_context(sqlite3_context*, int nBytes);

/*
** The pUserData parameter to the sqlite3_create_function()
** routine used to register user functions is available to
** the implementation of the function using this call.
*/
void *sqlite3_user_data(sqlite3_context*);

/*
** The following two functions may be used by scalar user functions to
** associate meta-data with argument values. If the same value is passed to
** multiple invocations of the user-function during query execution, under
** some circumstances the associated meta-data may be preserved. This may
** be used, for example, to add a regular-expression matching scalar
** function. The compiled version of the regular expression is stored as
** meta-data associated with the SQL value passed as the regular expression
** pattern.
**
** Calling sqlite3_get_auxdata() returns a pointer to the meta data
** associated with the Nth argument value to the current user function
** call, where N is the second parameter. If no meta-data has been set for
** that value, then a NULL pointer is returned.
**
** The sqlite3_set_auxdata() is used to associate meta data with a user
** function argument. The third parameter is a pointer to the meta data
** to be associated with the Nth user function argument value. The fourth
** parameter specifies a 'delete function' that will be called on the meta
** data pointer to release it when it is no longer required. If the delete
** function pointer is NULL, it is not invoked.
**
** In practice, meta-data is preserved between function calls for
** expressions that are constant at compile time. This includes literal
** values and SQL variables.
*/
void *sqlite3_get_auxdata(sqlite3_context*, int);
void sqlite3_set_auxdata(sqlite3_context*, int, void*, void (*)(void*));


/*
** These are special value for the destructor that is passed in as the
** final argument to routines like sqlite3_result_blob().  If the destructor
** argument is SQLITE_STATIC, it means that the content pointer is constant
** and will never change.  It does not need to be destroyed.  The 
** SQLITE_TRANSIENT value means that the content will likely change in
** the near future and that SQLite should make its own private copy of
** the content before returning.
**
** The typedef is necessary to work around problems in certain
** C++ compilers.  See ticket #2191.
*/
typedef void (*sqlite3_destructor_type)(void*);
#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

/*
** User-defined functions invoke the following routines in order to
** set their return value.
*/
void sqlite3_result_blob(sqlite3_context*, const void*, int, void(*)(void*));
void sqlite3_result_double(sqlite3_context*, double);
void sqlite3_result_error(sqlite3_context*, const char*, int);
void sqlite3_result_error16(sqlite3_context*, const void*, int);
void sqlite3_result_int(sqlite3_context*, int);
void sqlite3_result_int64(sqlite3_context*, sqlite_int64);
void sqlite3_result_null(sqlite3_context*);
void sqlite3_result_text(sqlite3_context*, const char*, int, void(*)(void*));
void sqlite3_result_text16(sqlite3_context*, const void*, int, void(*)(void*));
void sqlite3_result_text16le(sqlite3_context*, const void*, int,void(*)(void*));
void sqlite3_result_text16be(sqlite3_context*, const void*, int,void(*)(void*));
void sqlite3_result_value(sqlite3_context*, sqlite3_value*);

/*
** These are the allowed values for the eTextRep argument to
** sqlite3_create_collation and sqlite3_create_function.
*/
#define SQLITE_UTF8           1
#define SQLITE_UTF16LE        2
#define SQLITE_UTF16BE        3
#define SQLITE_UTF16          4    /* Use native byte order */
#define SQLITE_ANY            5    /* sqlite3_create_function only */
#define SQLITE_UTF16_ALIGNED  8    /* sqlite3_create_collation only */

/*
** These two functions are used to add new collation sequences to the
** sqlite3 hHandle specified as the first argument. 
**
** The name of the new collation sequence is specified as a UTF-8 string
** for sqlite3_create_collation() and a UTF-16 string for
** sqlite3_create_collation16(). In both cases the name is passed as the
** second function argument.
**
** The third argument must be one of the constants SQLITE_UTF8,
** SQLITE_UTF16LE or SQLITE_UTF16BE, indicating that the user-supplied
** routine expects to be passed pointers to strings encoded using UTF-8,
** UTF-16 little-endian or UTF-16 big-endian respectively.
**
** A pointer to the user supplied routine must be passed as the fifth
** argument. If it is NULL, this is the same as deleting the collation
** sequence (so that SQLite cannot call it anymore). Each time the user
** supplied function is invoked, it is passed a copy of the void* passed as
** the fourth argument to sqlite3_create_collation() or
** sqlite3_create_collation16() as its first parameter.
**
** The remaining arguments to the user-supplied routine are two strings,
** each represented by a [length, data] pair and encoded in the encoding
** that was passed as the third argument when the collation sequence was
** registered. The user routine should return negative, zero or positive if
** the first string is less than, equal to, or greater than the second
** string. i.e. (STRING1 - STRING2).
*/
int sqlite3_create_collation(
  sqlite3*, 
  const char *zName, 
  int eTextRep, 
  void*,
  int(*xCompare)(void*,int,const void*,int,const void*)
);
int sqlite3_create_collation16(
  sqlite3*, 
  const char *zName, 
  int eTextRep, 
  void*,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

/*
** To avoid having to register all collation sequences before a database
** can be used, a single callback function may be registered with the
** database hHandle to be called whenever an undefined collation sequence is
** required.
**
** If the function is registered using the sqlite3_collation_needed() API,
** then it is passed the names of undefined collation sequences as strings
** encoded in UTF-8. If sqlite3_collation_needed16() is used, the names
** are passed as UTF-16 in machine native byte order. A call to either
** function replaces any existing callback.
**
** When the user-function is invoked, the first argument passed is a copy
** of the second argument to sqlite3_collation_needed() or
** sqlite3_collation_needed16(). The second argument is the database
** hHandle. The third argument is one of SQLITE_UTF8, SQLITE_UTF16BE or
** SQLITE_UTF16LE, indicating the most desirable form of the collation
** sequence function required. The fourth parameter is the name of the
** required collation sequence.
**
** The collation sequence is returned to SQLite by a collation-needed
** callback using the sqlite3_create_collation() or
** sqlite3_create_collation16() APIs, described above.
*/
int sqlite3_collation_needed(
  sqlite3*, 
  void*, 
  void(*)(void*,sqlite3*,int eTextRep,const char*)
);
int sqlite3_collation_needed16(
  sqlite3*, 
  void*,
  void(*)(void*,sqlite3*,int eTextRep,const void*)
);

/*
** Specify the key for an encrypted database.  This routine should be
** called right after sqlite3_open().
**
** The code to implement this API is not available in the public release
** of SQLite.
*/
int sqlite3_key(
  sqlite3 *db,                   /* Database to be rekeyed */
  const void *pKey, int nKey     /* The key */
);

/*
** Change the key on an open database.  If the current database is not
** encrypted, this routine will encrypt it.  If pNew==0 or nNew==0, the
** database is decrypted.
**
** The code to implement this API is not available in the public release
** of SQLite.
*/
int sqlite3_rekey(
  sqlite3 *db,                   /* Database to be rekeyed */
  const void *pKey, int nKey     /* The new key */
);

/*
** Sleep for a little while. The second parameter is the number of
** miliseconds to sleep for. 
**
** If the operating system does not support sleep requests with 
** milisecond time resolution, then the time will be rounded up to 
** the nearest second. The number of miliseconds of sleep actually 
** requested from the operating system is returned.
*/
int sqlite3_sleep(int);

/*
** Return TRUE (non-zero) if the statement supplied as an argument needs
** to be recompiled.  A statement needs to be recompiled whenever the
** execution environment changes in a way that would alter the program
** that sqlite3_prepare() generates.  For example, if new functions or
** collating sequences are registered or if an authorizer function is
** added or changed.
**
*/
int sqlite3_expired(sqlite3_stmt*);

/*
** Move all bindings from the first prepared statement over to the second.
** This routine is useful, for example, if the first prepared statement
** fails with an SQLITE_SCHEMA error.  The same SQL can be prepared into
** the second prepared statement then all of the bindings transfered over
** to the second statement before the first statement is finalized.
*/
int sqlite3_transfer_bindings(sqlite3_stmt*, sqlite3_stmt*);

/*
** If the following global variable is made to point to a
** string which is the name of a directory, then all temporary files
** created by SQLite will be placed in that directory.  If this variable
** is NULL pointer, then SQLite does a search for an appropriate temporary
** file directory.
**
** Once sqlite3_open() has been called, changing this variable will invalidate
** the current temporary database, if any.
*/
extern char *sqlite3_temp_directory;

/*
** This function is called to recover from a malloc() failure that occured
** within the SQLite library. Normally, after a single malloc() fails the 
** library refuses to function (all major calls return SQLITE_NOMEM).
** This function restores the library state so that it can be used again.
**
** All existing statements (sqlite3_stmt pointers) must be finalized or
** reset before this call is made. Otherwise, SQLITE_BUSY is returned.
** If any in-memory databases are in use, either as a main or TEMP
** database, SQLITE_ERROR is returned. In either of these cases, the 
** library is not reset and remains unusable.
**
** This function is *not* threadsafe. Calling this from within a threaded
** application when threads other than the caller have used SQLite is
** dangerous and will almost certainly result in malfunctions.
**
** This functionality can be omitted from a build by defining the 
** SQLITE_OMIT_GLOBALRECOVER at compile time.
*/
int sqlite3_global_recover(void);

/*
** Test to see whether or not the database connection is in autocommit
** mode.  Return TRUE if it is and FALSE if not.  Autocommit mode is on
** by default.  Autocommit is disabled by a BEGIN statement and reenabled
** by the next COMMIT or ROLLBACK.
*/
int sqlite3_get_autocommit(sqlite3*);

/*
** Return the sqlite3* database hHandle to which the prepared statement given
** in the argument belongs.  This is the same database hHandle that was
** the first argument to the sqlite3_prepare() that was used to create
** the statement in the first place.
*/
sqlite3 *sqlite3_db_handle(sqlite3_stmt*);

/*
** Register a callback function with the database connection identified by the 
** first argument to be invoked whenever a row is updated, inserted or deleted.
** Any callback set by a previous call to this function for the same 
** database connection is overridden.
**
** The second argument is a pointer to the function to invoke when a 
** row is updated, inserted or deleted. The first argument to the callback is
** a copy of the third argument to sqlite3_update_hook. The second callback 
** argument is one of SQLITE_INSERT, SQLITE_DELETE or SQLITE_UPDATE, depending
** on the operation that caused the callback to be invoked. The third and 
** fourth arguments to the callback contain pointers to the database and 
** table name containing the affected row. The final callback parameter is 
** the rowid of the row. In the case of an update, this is the rowid after 
** the update takes place.
**
** The update hook is not invoked when internal system tables are
** modified (i.e. sqlite_master and sqlite_sequence).
**
** If another function was previously registered, its pArg value is returned.
** Otherwise NULL is returned.
*/
void *sqlite3_update_hook(
  sqlite3*, 
  void(*)(void *,int ,char const *,char const *,sqlite_int64),
  void*
);

/*
** Register a callback to be invoked whenever a transaction is rolled
** back. 
**
** The new callback function overrides any existing rollback-hook
** callback. If there was an existing callback, then it's pArg value 
** (the third argument to sqlite3_rollback_hook() when it was registered) 
** is returned. Otherwise, NULL is returned.
**
** For the purposes of this API, a transaction is said to have been 
** rolled back if an explicit "ROLLBACK" statement is executed, or
** an error or constraint causes an implicit rollback to occur. The 
** callback is not invoked if a transaction is automatically rolled
** back because the database connection is closed.
*/
void *sqlite3_rollback_hook(sqlite3*, void(*)(void *), void*);

/*
** This function is only available if the library is compiled without
** the SQLITE_OMIT_SHARED_CACHE macro defined. It is used to enable or
** disable (if the argument is true or false, respectively) the 
** "shared pager" feature.
*/
int sqlite3_enable_shared_cache(int);

/*
** Attempt to free N bytes of heap memory by deallocating non-essential
** memory allocations held by the database library (example: memory 
** used to cache database pages to improve performance).
**
** This function is not a part of standard builds.  It is only created
** if SQLite is compiled with the SQLITE_ENABLE_MEMORY_MANAGEMENT macro.
*/
int sqlite3_release_memory(int);

/*
** Place a "soft" limit on the amount of heap memory that may be allocated by
** SQLite within the current thread. If an internal allocation is requested 
** that would exceed the specified limit, sqlite3_release_memory() is invoked
** one or more times to free up some space before the allocation is made.
**
** The limit is called "soft", because if sqlite3_release_memory() cannot free
** sufficient memory to prevent the limit from being exceeded, the memory is
** allocated anyway and the current operation proceeds.
**
** This function is only available if the library was compiled with the 
** SQLITE_ENABLE_MEMORY_MANAGEMENT option set.
** memory-management has been enabled.
*/
void sqlite3_soft_heap_limit(int);

/*
** This routine makes sure that all thread-local storage has been
** deallocated for the current thread.
**
** This routine is not technically necessary.  All thread-local storage
** will be automatically deallocated once memory-management and
** shared-cache are disabled and the soft heap limit has been set
** to zero.  This routine is provided as a convenience for users who
** want to make absolutely sure they have not forgotten something
** prior to killing off a thread.
*/
void sqlite3_thread_cleanup(void);

/*
** Return meta information about a specific column of a specific database
** table accessible using the connection hHandle passed as the first function 
** argument.
**
** The column is identified by the second, third and fourth parameters to 
** this function. The second parameter is either the name of the database
** (i.e. "main", "temp" or an attached database) containing the specified
** table or NULL. If it is NULL, then all attached databases are searched
** for the table using the same algorithm as the database engine uses to 
** resolve unqualified table references.
**
** The third and fourth parameters to this function are the table and column 
** name of the desired column, respectively. Neither of these parameters 
** may be NULL.
**
** Meta information is returned by writing to the memory locations passed as
** the 5th and subsequent parameters to this function. Any of these 
** arguments may be NULL, in which case the corresponding element of meta 
** information is ommitted.
**
** Parameter     Output Type      Description
** -----------------------------------
**
**   5th         const char*      Data type
**   6th         const char*      Name of the default collation sequence 
**   7th         int              True if the column has a NOT NULL constraint
**   8th         int              True if the column is part of the PRIMARY KEY
**   9th         int              True if the column is AUTOINCREMENT
**
**
** The memory pointed to by the character pointers returned for the 
** declaration type and collation sequence is valid only until the next 
** call to any sqlite API function.
**
** If the specified table is actually a view, then an error is returned.
**
** If the specified column is "rowid", "oid" or "_rowid_" and an 
** INTEGER PRIMARY KEY column has been explicitly declared, then the output 
** parameters are set for the explicitly declared column. If there is no
** explicitly declared IPK column, then the output parameters are set as 
** follows:
**
**     data type: "INTEGER"
**     collation sequence: "BINARY"
**     not null: 0
**     primary key: 1
**     auto increment: 0
**
** This function may load one or more schemas from database files. If an
** error occurs during this process, or if the requested table or column
** cannot be found, an SQLITE error code is returned and an error message
** left in the database hHandle (to be retrieved using sqlite3_errmsg()).
**
** This API is only available if the library was compiled with the
** SQLITE_ENABLE_COLUMN_METADATA preprocessor symbol defined.
*/
int sqlite3_table_column_metadata(
  sqlite3 *db,                /* Connection hHandle */
  const char *zDbName,        /* Database name or NULL */
  const char *zTableName,     /* Table name */
  const char *zColumnName,    /* Column name */
  char const **pzDataType,    /* OUTPUT: Declared data type */
  char const **pzCollSeq,     /* OUTPUT: Collation sequence name */
  int *pNotNull,              /* OUTPUT: True if NOT NULL constraint exists */
  int *pPrimaryKey,           /* OUTPUT: True if column part of PK */
  int *pAutoinc               /* OUTPUT: True if colums is auto-increment */
);

/*
****** EXPERIMENTAL - subject to change without notice **************
**
** Attempt to load an SQLite extension library contained in the file
** zFile.  The entry point is zProc.  zProc may be 0 in which case the
** name of the entry point defaults to "sqlite3_extension_init".
**
** Return SQLITE_OK on success and SQLITE_ERROR if something goes wrong.
**
** If an error occurs and pzErrMsg is not 0, then fill *pzErrMsg with 
** error message text.  The calling function should free this memory
** by calling sqlite3_free().
**
** Extension loading must be enabled using sqlite3_enable_load_extension()
** prior to calling this API or an error will be returned.
**
****** EXPERIMENTAL - subject to change without notice **************
*/
int sqlite3_load_extension(
  sqlite3 *db,          /* Load the extension into this database connection */
  const char *zFile,    /* Name of the shared library containing extension */
  const char *zProc,    /* Entry point.  Derived from zFile if 0 */
  char **pzErrMsg       /* Put error message here if not 0 */
);

/*
** So as not to open security holes in older applications that are
** unprepared to deal with extension load, and as a means of disabling
** extension loading while executing user-entered SQL, the following
** API is provided to turn the extension loading mechanism on and
** off.  It is off by default.  See ticket #1863.
**
** Call this routine with onoff==1 to turn extension loading on
** and call it with onoff==0 to turn it back off again.
*/
int sqlite3_enable_load_extension(sqlite3 *db, int onoff);

/*
****** EXPERIMENTAL - subject to change without notice **************
**
** Register an extension entry point that is automatically invoked
** whenever a new database connection is opened.
**
** This API can be invoked at program startup in order to register
** one or more statically linked extensions that will be available
** to all new database connections.
**
** Duplicate extensions are detected so calling this routine multiple
** times with the same extension is harmless.
**
** This routine stores a pointer to the extension in an array
** that is obtained from malloc().  If you run a memory leak
** checker on your program and it reports a leak because of this
** array, then invoke sqlite3_automatic_extension_reset() prior
** to shutdown to free the memory.
**
** Automatic extensions apply across all threads.
*/
int sqlite3_auto_extension(void *xEntryPoint);


/*
****** EXPERIMENTAL - subject to change without notice **************
**
** Disable all previously registered automatic extensions.  This
** routine undoes the effect of all prior sqlite3_automatic_extension()
** calls.
**
** This call disabled automatic extensions in all threads.
*/
void sqlite3_reset_auto_extension(void);


/*
****** EXPERIMENTAL - subject to change without notice **************
**
** The interface to the virtual-table mechanism is currently considered
** to be experimental.  The interface might change in incompatible ways.
** If this is a problem for you, do not use the interface at this time.
**
** When the virtual-table mechanism stablizes, we will declare the
** interface fixed, support it indefinitely, and remove this comment.
*/

/*
** Structures used by the virtual table interface
*/
typedef struct sqlite3_vtab sqlite3_vtab;
typedef struct sqlite3_index_info sqlite3_index_info;
typedef struct sqlite3_vtab_cursor sqlite3_vtab_cursor;
typedef struct sqlite3_module sqlite3_module;

/*
** A module is a class of virtual tables.  Each module is defined
** by an instance of the following structure.  This structure consists
** mostly of methods for the module.
*/
struct sqlite3_module {
  int iVersion;
  int (*xCreate)(sqlite3*, void *pAux,
               int argc, const char *const*argv,
               sqlite3_vtab **ppVTab, char**);
  int (*xConnect)(sqlite3*, void *pAux,
               int argc, const char *const*argv,
               sqlite3_vtab **ppVTab, char**);
  int (*xBestIndex)(sqlite3_vtab *pVTab, sqlite3_index_info*);
  int (*xDisconnect)(sqlite3_vtab *pVTab);
  int (*xDestroy)(sqlite3_vtab *pVTab);
  int (*xOpen)(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
  int (*xClose)(sqlite3_vtab_cursor*);
  int (*xFilter)(sqlite3_vtab_cursor*, int idxNum, const char *idxStr,
                int argc, sqlite3_value **argv);
  int (*xNext)(sqlite3_vtab_cursor*);
  int (*xEof)(sqlite3_vtab_cursor*);
  int (*xColumn)(sqlite3_vtab_cursor*, sqlite3_context*, int);
  int (*xRowid)(sqlite3_vtab_cursor*, sqlite_int64 *pRowid);
  int (*xUpdate)(sqlite3_vtab *, int, sqlite3_value **, sqlite_int64 *);
  int (*xBegin)(sqlite3_vtab *pVTab);
  int (*xSync)(sqlite3_vtab *pVTab);
  int (*xCommit)(sqlite3_vtab *pVTab);
  int (*xRollback)(sqlite3_vtab *pVTab);
  int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
                       void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                       void **ppArg);
};

/*
** The sqlite3_index_info structure and its substructures is used to
** pass information into and receive the reply from the xBestIndex
** method of an sqlite3_module.  The fields under **Inputs** are the
** inputs to xBestIndex and are read-only.  xBestIndex inserts its
** results into the **Outputs** fields.
**
** The aConstraint[] array records WHERE clause constraints of the
** form:
**
**         column OP expr
**
** Where OP is =, <, <=, >, or >=.  The particular operator is stored
** in aConstraint[].op.  The index of the column is stored in 
** aConstraint[].iColumn.  aConstraint[].usable is TRUE if the
** expr on the right-hand side can be evaluated (and thus the constraint
** is usable) and false if it cannot.
**
** The optimizer automatically inverts terms of the form "expr OP column"
** and makes other simplificatinos to the WHERE clause in an attempt to
** get as many WHERE clause terms into the form shown above as possible.
** The aConstraint[] array only reports WHERE clause terms in the correct
** form that refer to the particular virtual table being queried.
**
** Information about the ORDER BY clause is stored in aOrderBy[].
** Each term of aOrderBy records a column of the ORDER BY clause.
**
** The xBestIndex method must fill aConstraintUsage[] with information
** about what parameters to pass to xFilter.  If argvIndex>0 then
** the right-hand side of the corresponding aConstraint[] is evaluated
** and becomes the argvIndex-th entry in argv.  If aConstraintUsage[].omit
** is true, then the constraint is assumed to be fully handled by the
** virtual table and is not checked again by SQLite.
**
** The idxNum and idxPtr values are recorded and passed into xFilter.
** sqlite3_free() is used to free idxPtr if needToFreeIdxPtr is true.
**
** The orderByConsumed means that output from xFilter will occur in
** the correct order to satisfy the ORDER BY clause so that no separate
** sorting step is required.
**
** The estimatedCost value is an estimate of the cost of doing the
** particular lookup.  A full scan of a table with N entries should have
** a cost of N.  A binary search of a table of N entries should have a
** cost of approximately log(N).
*/
struct sqlite3_index_info {
  /* Inputs */
  int nConstraint;     /* Number of entries in aConstraint */
  const struct sqlite3_index_constraint {
     int iColumn;              /* Column on left-hand side of constraint */
     unsigned char op;         /* Constraint operator */
     unsigned char usable;     /* True if this constraint is usable */
     int iTermOffset;          /* Used internally - xBestIndex should ignore */
  } *aConstraint;      /* Table of WHERE clause constraints */
  int nOrderBy;        /* Number of terms in the ORDER BY clause */
  const struct sqlite3_index_orderby {
     int iColumn;              /* Column number */
     unsigned char desc;       /* True for DESC.  False for ASC. */
  } *aOrderBy;         /* The ORDER BY clause */

  /* Outputs */
  struct sqlite3_index_constraint_usage {
    int argvIndex;           /* if >0, constraint is part of argv to xFilter */
    unsigned char omit;      /* Do not code a test for this constraint */
  } * aConstraintUsage;
  int idxNum;                /* Number used to identify the index */
  char *idxStr;              /* String, possibly obtained from sqlite3_malloc */
  int needToFreeIdxStr;      /* Free idxStr using sqlite3_free() if true */
  int orderByConsumed;       /* True if output is already ordered */
  double estimatedCost;      /* Estimated cost of using this index */
};
#define SQLITE_INDEX_CONSTRAINT_EQ    2
#define SQLITE_INDEX_CONSTRAINT_GT    4
#define SQLITE_INDEX_CONSTRAINT_LE    8
#define SQLITE_INDEX_CONSTRAINT_LT    16
#define SQLITE_INDEX_CONSTRAINT_GE    32
#define SQLITE_INDEX_CONSTRAINT_MATCH 64

/*
** This routine is used to register a new module name with an SQLite
** connection.  Module names must be registered before creating new
** virtual tables on the module, or before using preexisting virtual
** tables of the module.
*/
int sqlite3_create_module(
  sqlite3 *db,               /* SQLite connection to register module with */
  const char *zName,         /* Name of the module */
  const sqlite3_module *,    /* Methods for the module */
  void *                     /* Client data for xCreate/xConnect */
);

/*
** Every module implementation uses a subclass of the following structure
** to describe a particular instance of the module.  Each subclass will
** be taylored to the specific needs of the module implementation.   The
** purpose of this superclass is to define certain fields that are common
** to all module implementations.
**
** Virtual tables methods can set an error message by assigning a
** string obtained from sqlite3_mprintf() to zErrMsg.  The method should
** take care that any prior string is freed by a call to sqlite3_free()
** prior to assigning a new string to zErrMsg.  After the error message
** is delivered up to the client application, the string will be automatically
** freed by sqlite3_free() and the zErrMsg field will be zeroed.  Note
** that sqlite3_mprintf() and sqlite3_free() are used on the zErrMsg field
** since virtual tables are commonly implemented in loadable extensions which
** do not have access to sqlite3MPrintf() or sqlite3Free().
*/
struct sqlite3_vtab {
  const sqlite3_module *pModule;  /* The module for this virtual table */
  int nRef;                       /* Used internally */
  char *zErrMsg;                  /* Error message from sqlite3_mprintf() */
  /* Virtual table implementations will typically add additional fields */
};

/* Every module implementation uses a subclass of the following structure
** to describe cursors that point into the virtual table and are used
** to loop through the virtual table.  Cursors are created using the
** xOpen method of the module.  Each module implementation will define
** the content of a cursor structure to suit its own needs.
**
** This superclass exists in order to define fields of the cursor that
** are common to all implementations.
*/
struct sqlite3_vtab_cursor {
  sqlite3_vtab *pVtab;      /* Virtual table of this cursor */
  /* Virtual table implementations will typically add additional fields */
};

/*
** The xCreate and xConnect methods of a module use the following API
** to declare the format (the names and datatypes of the columns) of
** the virtual tables they implement.
*/
int sqlite3_declare_vtab(sqlite3*, const char *zCreateTable);

/*
** Virtual tables can provide alternative implementations of functions
** using the xFindFunction method.  But global versions of those functions
** must exist in order to be overloaded.
**
** This API makes sure a global version of a function with a particular
** name and number of parameters exists.  If no such function exists
** before this API is called, a new function is created.  The implementation
** of the new function always causes an exception to be thrown.  So
** the new function is not good for anything by itself.  Its only
** purpose is to be a place-holder function that can be overloaded
** by virtual tables.
**
** This API should be considered part of the virtual table interface,
** which is experimental and subject to change.
*/
int sqlite3_overload_function(sqlite3*, const char *zFuncName, int nArg);

/*
** The interface to the virtual-table mechanism defined above (back up
** to a comment remarkably similar to this one) is currently considered
** to be experimental.  The interface might change in incompatible ways.
** If this is a problem for you, do not use the interface at this time.
**
** When the virtual-table mechanism stablizes, we will declare the
** interface fixed, support it indefinitely, and remove this comment.
**
****** EXPERIMENTAL - subject to change without notice **************
*/

/*
** Undo the hack that converts floating point types to integer for
** builds on processors without floating point support.
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# undef double
#endif

#ifndef _SQLITEINT_H_
#define _SQLITEINT_H_

#if defined(SQLITE_TCL) || defined(TCLSH)
# include <tcl.h>
#endif

/*
** Many people are failing to set -DNDEBUG=1 when compiling SQLite.
** Setting NDEBUG makes the code smaller and run faster.  So the following
** lines are added to automatically set NDEBUG unless the -DSQLITE_DEBUG=1
** option is set.  Thus NDEBUG becomes an opt-in rather than an opt-out
** feature.
*/
#if !defined(NDEBUG) && !defined(SQLITE_DEBUG) 
# define NDEBUG 1
#endif

/*
** These #defines should enable >2GB file support on Posix if the
** underlying operating system supports it.  If the OS lacks
** large file support, or if the OS is windows, these should be no-ops.
**
** Large file support can be disabled using the -DSQLITE_DISABLE_LFS switch
** on the compiler command line.  This is necessary if you are compiling
** on a recent machine (ex: RedHat 7.2) but you want your code to work
** on an older machine (ex: RedHat 6.0).  If you compile on RedHat 7.2
** without this option, LFS is enable.  But LFS does not exist in the kernel
** in RedHat 6.0, so the code won't work.  Hence, for maximum binary
** portability you should omit LFS.
**
** Similar is true for MacOS.  LFS is only supported on MacOS 9 and later.
*/
#ifndef SQLITE_DISABLE_LFS
# define _LARGE_FILE       1
# ifndef _FILE_OFFSET_BITS
#   define _FILE_OFFSET_BITS 64
# endif
# define _LARGEFILE_SOURCE 1
#endif

/************** Include hash.h in the middle of sqliteInt.h ******************/
/************** Begin file hash.h ********************************************/
/*
** 2001 September 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This is the header file for the generic hash-table implemenation
** used in SQLite.
**
** $Id: //DWF/Development/Components/Internal/DWF Core/v1.7/develop/global/src/dwfcore/sqlite/sqlite3.h#1 $
*/
#ifndef _SQLITE_HASH_H_
#define _SQLITE_HASH_H_

/* Forward declarations of structures. */
typedef struct Hash Hash;
typedef struct HashElem HashElem;

/* A complete hash table is an instance of the following structure.
** The internals of this structure are intended to be opaque -- client
** code should not attempt to access or modify the fields of this structure
** directly.  Change this structure only by using the routines below.
** However, many of the "procedures" and "functions" for modifying and
** accessing this structure are really macros, so we can't really make
** this structure opaque.
*/
struct Hash {
  char keyClass;          /* SQLITE_HASH_INT, _POINTER, _STRING, _BINARY */
  char copyKey;           /* True if copy of key made on insert */
  int count;              /* Number of entries in this table */
  HashElem *first;        /* The first element of the array */
  void *(*xMalloc)(int);  /* malloc() function to use */
  void (*xFree)(void *);  /* free() function to use */
  int htsize;             /* Number of buckets in the hash table */
  struct _ht {            /* the hash table */
    int count;               /* Number of entries with this hash */
    HashElem *chain;         /* Pointer to first entry with this hash */
  } *ht;
};

/* Each element in the hash table is an instance of the following 
** structure.  All elements are stored on a single doubly-linked list.
**
** Again, this structure is intended to be opaque, but it can't really
** be opaque because it is used by macros.
*/
struct HashElem {
  HashElem *next, *prev;   /* Next and previous elements in the table */
  void *data;              /* Data associated with this element */
  void *pKey; int nKey;    /* Key associated with this element */
};

/*
** There are 4 different modes of operation for a hash table:
**
**   SQLITE_HASH_INT         nKey is used as the key and pKey is ignored.
**
**   SQLITE_HASH_POINTER     pKey is used as the key and nKey is ignored.
**
**   SQLITE_HASH_STRING      pKey points to a string that is nKey bytes long
**                           (including the null-terminator, if any).  Case
**                           is ignored in comparisons.
**
**   SQLITE_HASH_BINARY      pKey points to binary data nKey bytes long. 
**                           memcmp() is used to compare keys.
**
** A copy of the key is made for SQLITE_HASH_STRING and SQLITE_HASH_BINARY
** if the copyKey parameter to HashInit is 1.  
*/
/* #define SQLITE_HASH_INT       1 // NOT USED */
/* #define SQLITE_HASH_POINTER   2 // NOT USED */
#define SQLITE_HASH_STRING    3
#define SQLITE_HASH_BINARY    4

/*
** Access routines.  To delete, insert a NULL pointer.
*/
void sqlite3HashInit(Hash*, int keytype, int copyKey);
void *sqlite3HashInsert(Hash*, const void *pKey, int nKey, void *pData);
void *sqlite3HashFind(const Hash*, const void *pKey, int nKey);
void sqlite3HashClear(Hash*);

/*
** Macros for looping over all elements of a hash table.  The idiom is
** like this:
**
**   Hash h;
**   HashElem *p;
**   ...
**   for(p=sqliteHashFirst(&h); p; p=sqliteHashNext(p)){
**     SomeStructure *pData = sqliteHashData(p);
**     // do something with pData
**   }
*/
#define sqliteHashFirst(H)  ((H)->first)
#define sqliteHashNext(E)   ((E)->next)
#define sqliteHashData(E)   ((E)->data)
#define sqliteHashKey(E)    ((E)->pKey)
#define sqliteHashKeysize(E) ((E)->nKey)

/*
** Number of entries in a hash table
*/
#define sqliteHashCount(H)  ((H)->count)

#endif /* _SQLITE_HASH_H_ */

/************** End of hash.h ************************************************/
/************** Continuing where we left off in sqliteInt.h ******************/
/************** Include parse.h in the middle of sqliteInt.h *****************/
/************** Begin file parse.h *******************************************/
#define TK_SEMI                            1
#define TK_EXPLAIN                         2
#define TK_QUERY                           3
#define TK_PLAN                            4
#define TK_BEGIN                           5
#define TK_TRANSACTION                     6
#define TK_DEFERRED                        7
#define TK_IMMEDIATE                       8
#define TK_EXCLUSIVE                       9
#define TK_COMMIT                         10
#define TK_END                            11
#define TK_ROLLBACK                       12
#define TK_CREATE                         13
#define TK_TABLE                          14
#define TK_IF                             15
#define TK_NOT                            16
#define TK_EXISTS                         17
#define TK_TEMP                           18
#define TK_LP                             19
#define TK_RP                             20
#define TK_AS                             21
#define TK_COMMA                          22
#define TK_ID                             23
#define TK_ABORT                          24
#define TK_AFTER                          25
#define TK_ANALYZE                        26
#define TK_ASC                            27
#define TK_ATTACH                         28
#define TK_BEFORE                         29
#define TK_CASCADE                        30
#define TK_CAST                           31
#define TK_CONFLICT                       32
#define TK_DATABASE                       33
#define TK_DESC                           34
#define TK_DETACH                         35
#define TK_EACH                           36
#define TK_FAIL                           37
#define TK_FOR                            38
#define TK_IGNORE                         39
#define TK_INITIALLY                      40
#define TK_INSTEAD                        41
#define TK_LIKE_KW                        42
#define TK_MATCH                          43
#define TK_KEY                            44
#define TK_OF                             45
#define TK_OFFSET                         46
#define TK_PRAGMA                         47
#define TK_RAISE                          48
#define TK_REPLACE                        49
#define TK_RESTRICT                       50
#define TK_ROW                            51
#define TK_TRIGGER                        52
#define TK_VACUUM                         53
#define TK_VIEW                           54
#define TK_VIRTUAL                        55
#define TK_REINDEX                        56
#define TK_RENAME                         57
#define TK_CTIME_KW                       58
#define TK_ANY                            59
#define TK_OR                             60
#define TK_AND                            61
#define TK_IS                             62
#define TK_BETWEEN                        63
#define TK_IN                             64
#define TK_ISNULL                         65
#define TK_NOTNULL                        66
#define TK_NE                             67
#define TK_EQ                             68
#define TK_GT                             69
#define TK_LE                             70
#define TK_LT                             71
#define TK_GE                             72
#define TK_ESCAPE                         73
#define TK_BITAND                         74
#define TK_BITOR                          75
#define TK_LSHIFT                         76
#define TK_RSHIFT                         77
#define TK_PLUS                           78
#define TK_MINUS                          79
#define TK_STAR                           80
#define TK_SLASH                          81
#define TK_REM                            82
#define TK_CONCAT                         83
#define TK_COLLATE                        84
#define TK_UMINUS                         85
#define TK_UPLUS                          86
#define TK_BITNOT                         87
#define TK_STRING                         88
#define TK_JOIN_KW                        89
#define TK_CONSTRAINT                     90
#define TK_DEFAULT                        91
#define TK_NULL                           92
#define TK_PRIMARY                        93
#define TK_UNIQUE                         94
#define TK_CHECK                          95
#define TK_REFERENCES                     96
#define TK_AUTOINCR                       97
#define TK_ON                             98
#define TK_DELETE                         99
#define TK_UPDATE                         100
#define TK_INSERT                         101
#define TK_SET                            102
#define TK_DEFERRABLE                     103
#define TK_FOREIGN                        104
#define TK_DROP                           105
#define TK_UNION                          106
#define TK_ALL                            107
#define TK_EXCEPT                         108
#define TK_INTERSECT                      109
#define TK_SELECT                         110
#define TK_DISTINCT                       111
#define TK_DOT                            112
#define TK_FROM                           113
#define TK_JOIN                           114
#define TK_USING                          115
#define TK_ORDER                          116
#define TK_BY                             117
#define TK_GROUP                          118
#define TK_HAVING                         119
#define TK_LIMIT                          120
#define TK_WHERE                          121
#define TK_INTO                           122
#define TK_VALUES                         123
#define TK_INTEGER                        124
#define TK_FLOAT                          125
#define TK_BLOB                           126
#define TK_REGISTER                       127
#define TK_VARIABLE                       128
#define TK_CASE                           129
#define TK_WHEN                           130
#define TK_THEN                           131
#define TK_ELSE                           132
#define TK_INDEX                          133
#define TK_ALTER                          134
#define TK_TO                             135
#define TK_ADD                            136
#define TK_COLUMNKW                       137
#define TK_TO_TEXT                        138
#define TK_TO_BLOB                        139
#define TK_TO_NUMERIC                     140
#define TK_TO_INT                         141
#define TK_TO_REAL                        142
#define TK_END_OF_FILE                    143
#define TK_ILLEGAL                        144
#define TK_SPACE                          145
#define TK_UNCLOSED_STRING                146
#define TK_COMMENT                        147
#define TK_FUNCTION                       148
#define TK_COLUMN                         149
#define TK_AGG_FUNCTION                   150
#define TK_AGG_COLUMN                     151
#define TK_CONST_FUNC                     152

/************** End of parse.h ***********************************************/
/************** Continuing where we left off in sqliteInt.h ******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

/*
** If compiling for a processor that lacks floating point support,
** substitute integer for floating-point
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# define double sqlite_int64
# define LONGDOUBLE_TYPE sqlite_int64
# ifndef SQLITE_BIG_DBL
#   define SQLITE_BIG_DBL (0x7fffffffffffffff)
# endif
# define SQLITE_OMIT_DATETIME_FUNCS 1
# define SQLITE_OMIT_TRACE 1
#endif
#ifndef SQLITE_BIG_DBL
# define SQLITE_BIG_DBL (1e99)
#endif

/*
** The maximum number of in-memory pages to use for the main database
** table and for temporary tables. Internally, the MAX_PAGES and 
** TEMP_PAGES macros are used. To override the default values at
** compilation time, the SQLITE_DEFAULT_CACHE_SIZE and 
** SQLITE_DEFAULT_TEMP_CACHE_SIZE macros should be set.
*/
#ifdef SQLITE_DEFAULT_CACHE_SIZE
# define MAX_PAGES SQLITE_DEFAULT_CACHE_SIZE
#else
# define MAX_PAGES   2000
#endif
#ifdef SQLITE_DEFAULT_TEMP_CACHE_SIZE
# define TEMP_PAGES SQLITE_DEFAULT_TEMP_CACHE_SIZE
#else
# define TEMP_PAGES   500
#endif

/*
** OMIT_TEMPDB is set to 1 if SQLITE_OMIT_TEMPDB is defined, or 0
** afterward. Having this macro allows us to cause the C compiler 
** to omit code used by TEMP tables without messy #ifndef statements.
*/
#ifdef SQLITE_OMIT_TEMPDB
#define OMIT_TEMPDB 1
#else
#define OMIT_TEMPDB 0
#endif

/*
** If the following macro is set to 1, then NULL values are considered
** distinct when determining whether or not two entries are the same
** in a UNIQUE index.  This is the way PostgreSQL, Oracle, DB2, MySQL,
** OCELOT, and Firebird all work.  The SQL92 spec explicitly says this
** is the way things are suppose to work.
**
** If the following macro is set to 0, the NULLs are indistinct for
** a UNIQUE index.  In this mode, you can only have a single NULL entry
** for a column declared UNIQUE.  This is the way Informix and SQL Server
** work.
*/
#define NULL_DISTINCT_FOR_UNIQUE 1

/*
** The maximum number of attached databases.  This must be at least 2
** in order to support the main database file (0) and the file used to
** hold temporary tables (1).  And it must be less than 32 because
** we use a bitmask of databases with a u32 in places (for example
** the Parse.cookieMask field).
*/
#define MAX_ATTACHED 10

/*
** The maximum value of a ?nnn wildcard that the parser will accept.
*/
#define SQLITE_MAX_VARIABLE_NUMBER 999

/*
** The "file format" number is an integer that is incremented whenever
** the VDBE-level file format changes.  The following macros define the
** the default file format for new databases and the maximum file format
** that the library can read.
*/
#define SQLITE_MAX_FILE_FORMAT 4
#ifndef SQLITE_DEFAULT_FILE_FORMAT
# define SQLITE_DEFAULT_FILE_FORMAT 1
#endif

/*
** Provide a default value for TEMP_STORE in case it is not specified
** on the command-line
*/
#ifndef TEMP_STORE
# define TEMP_STORE 1
#endif

/*
** GCC does not define the offsetof() macro so we'll have to do it
** ourselves.
*/
#ifndef offsetof
#define offsetof(STRUCTURE,FIELD) ((int)((char*)&((STRUCTURE*)0)->FIELD))
#endif

/*
** Check to see if this machine uses EBCDIC.  (Yes, believe it or
** not, there are still machines out there that use EBCDIC.)
*/
#if 'A' == '\301'
# define SQLITE_EBCDIC 1
#else
# define SQLITE_ASCII 1
#endif

/*
** Integers of known sizes.  These typedefs might change for architectures
** where the sizes very.  Preprocessor macros are available so that the
** types can be conveniently redefined at compile-type.  Like this:
**
**         cc '-DUINTPTR_TYPE=long long int' ...
*/
#ifndef UINT32_TYPE
# define UINT32_TYPE unsigned int
#endif
#ifndef UINT16_TYPE
# define UINT16_TYPE unsigned short int
#endif
#ifndef INT16_TYPE
# define INT16_TYPE short int
#endif
#ifndef UINT8_TYPE
# define UINT8_TYPE unsigned char
#endif
#ifndef INT8_TYPE
# define INT8_TYPE signed char
#endif
#ifndef LONGDOUBLE_TYPE
# define LONGDOUBLE_TYPE long double
#endif
typedef sqlite_int64 i64;          /* 8-byte signed integer */
typedef sqlite_uint64 u64;         /* 8-byte unsigned integer */
typedef UINT32_TYPE u32;           /* 4-byte unsigned integer */
typedef UINT16_TYPE u16;           /* 2-byte unsigned integer */
typedef INT16_TYPE i16;            /* 2-byte signed integer */
typedef UINT8_TYPE u8;             /* 1-byte unsigned integer */
typedef UINT8_TYPE i8;             /* 1-byte signed integer */

/*
** Macros to determine whether the machine is big or little endian,
** evaluated at runtime.
*/
extern const int sqlite3one;
#if defined(i386) || defined(__i386__) || defined(_M_IX86)
# define SQLITE_BIGENDIAN    0
# define SQLITE_LITTLEENDIAN 1
# define SQLITE_UTF16NATIVE  SQLITE_UTF16LE
#else
# define SQLITE_BIGENDIAN    (*(char *)(&sqlite3one)==0)
# define SQLITE_LITTLEENDIAN (*(char *)(&sqlite3one)==1)
# define SQLITE_UTF16NATIVE (SQLITE_BIGENDIAN?SQLITE_UTF16BE:SQLITE_UTF16LE)
#endif

/*
** An instance of the following structure is used to store the busy-handler
** callback for a given sqlite handle. 
**
** The sqlite.busyHandler member of the sqlite struct contains the busy
** callback for the database handle. Each pager opened via the sqlite
** handle is passed a pointer to sqlite.busyHandler. The busy-handler
** callback is currently invoked only from within pager.c.
*/
typedef struct BusyHandler BusyHandler;
struct BusyHandler {
  int (*xFunc)(void *,int);  /* The busy callback */
  void *pArg;                /* First arg to busy callback */
  int nBusy;                 /* Incremented with each busy call */
};

/*
** Defer sourcing vdbe.h and btree.h until after the "u8" and 
** "BusyHandler typedefs.
*/
/************** Include vdbe.h in the middle of sqliteInt.h ******************/
/************** Begin file vdbe.h ********************************************/
/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Header file for the Virtual DataBase Engine (VDBE)
**
** This header defines the interface to the virtual database engine
** or VDBE.  The VDBE implements an abstract machine that runs a
** simple program to access and modify the underlying database.
**
** $Id: //DWF/Development/Components/Internal/DWF Core/v1.7/develop/global/src/dwfcore/sqlite/sqlite3.h#1 $
*/
#ifndef _SQLITE_VDBE_H_
#define _SQLITE_VDBE_H_

/*
** A single VDBE is an opaque structure named "Vdbe".  Only routines
** in the source file sqliteVdbe.c are allowed to see the insides
** of this structure.
*/
typedef struct Vdbe Vdbe;

/*
** A single instruction of the virtual machine has an opcode
** and as many as three operands.  The instruction is recorded
** as an instance of the following structure:
*/
struct VdbeOp {
  u8 opcode;          /* What operation to perform */
  int p1;             /* First operand */
  int p2;             /* Second parameter (often the jump destination) */
  char *p3;           /* Third parameter */
  int p3type;         /* One of the P3_xxx constants defined below */
#ifdef VDBE_PROFILE
  int cnt;            /* Number of times this instruction was executed */
  long long cycles;   /* Total time spend executing this instruction */
#endif
};
typedef struct VdbeOp VdbeOp;

/*
** A smaller version of VdbeOp used for the VdbeAddOpList() function because
** it takes up less space.
*/
struct VdbeOpList {
  u8 opcode;          /* What operation to perform */
  signed char p1;     /* First operand */
  short int p2;       /* Second parameter (often the jump destination) */
  char *p3;           /* Third parameter */
};
typedef struct VdbeOpList VdbeOpList;

/*
** Allowed values of VdbeOp.p3type
*/
#define P3_NOTUSED    0   /* The P3 parameter is not used */
#define P3_DYNAMIC  (-1)  /* Pointer to a string obtained from sqliteMalloc() */
#define P3_STATIC   (-2)  /* Pointer to a static string */
#define P3_COLLSEQ  (-4)  /* P3 is a pointer to a CollSeq structure */
#define P3_FUNCDEF  (-5)  /* P3 is a pointer to a FuncDef structure */
#define P3_KEYINFO  (-6)  /* P3 is a pointer to a KeyInfo structure */
#define P3_VDBEFUNC (-7)  /* P3 is a pointer to a VdbeFunc structure */
#define P3_MEM      (-8)  /* P3 is a pointer to a Mem*    structure */
#define P3_TRANSIENT (-9) /* P3 is a pointer to a transient string */
#define P3_VTAB     (-10) /* P3 is a pointer to an sqlite3_vtab structure */
#define P3_MPRINTF  (-11) /* P3 is a string obtained from sqlite3_mprintf() */

/* When adding a P3 argument using P3_KEYINFO, a copy of the KeyInfo structure
** is made.  That copy is freed when the Vdbe is finalized.  But if the
** argument is P3_KEYINFO_HANDOFF, the passed in pointer is used.  It still
** gets freed when the Vdbe is finalized so it still should be obtained
** from a single sqliteMalloc().  But no copy is made and the calling
** function should *not* try to free the KeyInfo.
*/
#define P3_KEYINFO_HANDOFF (-9)

/*
** The Vdbe.aColName array contains 5n Mem structures, where n is the 
** number of columns of data returned by the statement.
*/
#define COLNAME_NAME     0
#define COLNAME_DECLTYPE 1
#define COLNAME_DATABASE 2
#define COLNAME_TABLE    3
#define COLNAME_COLUMN   4
#define COLNAME_N        5      /* Number of COLNAME_xxx symbols */

/*
** The following macro converts a relative address in the p2 field
** of a VdbeOp structure into a negative number so that 
** sqlite3VdbeAddOpList() knows that the address is relative.  Calling
** the macro again restores the address.
*/
#define ADDR(X)  (-1-(X))

/*
** The makefile scans the vdbe.c source file and creates the "opcodes.h"
** header file that defines a number for each opcode used by the VDBE.
*/
/************** Include opcodes.h in the middle of vdbe.h ********************/
/************** Begin file opcodes.h *****************************************/
/* Automatically generated.  Do not edit */
/* See the mkopcodeh.awk script for details */
#define OP_MemLoad                              1
#define OP_VNext                                2
#define OP_HexBlob                            126   /* same as TK_BLOB     */
#define OP_Column                               3
#define OP_SetCookie                            4
#define OP_IfMemPos                             5
#define OP_Real                               125   /* same as TK_FLOAT    */
#define OP_Sequence                             6
#define OP_MoveGt                               7
#define OP_Ge                                  72   /* same as TK_GE       */
#define OP_RowKey                               8
#define OP_Eq                                  68   /* same as TK_EQ       */
#define OP_OpenWrite                            9
#define OP_NotNull                             66   /* same as TK_NOTNULL  */
#define OP_If                                  10
#define OP_ToInt                              141   /* same as TK_TO_INT   */
#define OP_String8                             88   /* same as TK_STRING   */
#define OP_Pop                                 11
#define OP_VRowid                              12
#define OP_CollSeq                             13
#define OP_OpenRead                            14
#define OP_Expire                              15
#define OP_AutoCommit                          17
#define OP_Gt                                  69   /* same as TK_GT       */
#define OP_IntegrityCk                         18
#define OP_Sort                                19
#define OP_Function                            20
#define OP_And                                 61   /* same as TK_AND      */
#define OP_Subtract                            79   /* same as TK_MINUS    */
#define OP_Noop                                21
#define OP_Return                              22
#define OP_Remainder                           82   /* same as TK_REM      */
#define OP_NewRowid                            23
#define OP_Multiply                            80   /* same as TK_STAR     */
#define OP_IfMemNeg                            24
#define OP_Variable                            25
#define OP_String                              26
#define OP_RealAffinity                        27
#define OP_ParseSchema                         28
#define OP_VOpen                               29
#define OP_Close                               30
#define OP_CreateIndex                         31
#define OP_IsUnique                            32
#define OP_NotFound                            33
#define OP_Int64                               34
#define OP_MustBeInt                           35
#define OP_Halt                                36
#define OP_Rowid                               37
#define OP_IdxLT                               38
#define OP_AddImm                              39
#define OP_Statement                           40
#define OP_RowData                             41
#define OP_MemMax                              42
#define OP_Push                                43
#define OP_Or                                  60   /* same as TK_OR       */
#define OP_NotExists                           44
#define OP_MemIncr                             45
#define OP_Gosub                               46
#define OP_Divide                              81   /* same as TK_SLASH    */
#define OP_Integer                             47
#define OP_ToNumeric                          140   /* same as TK_TO_NUMERIC*/
#define OP_MemInt                              48
#define OP_Prev                                49
#define OP_Concat                              83   /* same as TK_CONCAT   */
#define OP_BitAnd                              74   /* same as TK_BITAND   */
#define OP_VColumn                             50
#define OP_CreateTable                         51
#define OP_Last                                52
#define OP_IsNull                              65   /* same as TK_ISNULL   */
#define OP_IdxRowid                            53
#define OP_MakeIdxRec                          54
#define OP_ShiftRight                          77   /* same as TK_RSHIFT   */
#define OP_ResetCount                          55
#define OP_FifoWrite                           56
#define OP_Callback                            57
#define OP_ContextPush                         58
#define OP_DropTrigger                         59
#define OP_DropIndex                           62
#define OP_IdxGE                               63
#define OP_IdxDelete                           64
#define OP_Vacuum                              73
#define OP_MoveLe                              84
#define OP_IfNot                               86
#define OP_DropTable                           89
#define OP_MakeRecord                          90
#define OP_ToBlob                             139   /* same as TK_TO_BLOB  */
#define OP_Delete                              91
#define OP_AggFinal                            92
#define OP_ShiftLeft                           76   /* same as TK_LSHIFT   */
#define OP_Dup                                 93
#define OP_Goto                                94
#define OP_TableLock                           95
#define OP_FifoRead                            96
#define OP_Clear                               97
#define OP_IdxGT                               98
#define OP_MoveLt                              99
#define OP_Le                                  70   /* same as TK_LE       */
#define OP_VerifyCookie                       100
#define OP_AggStep                            101
#define OP_Pull                               102
#define OP_ToText                             138   /* same as TK_TO_TEXT  */
#define OP_Not                                 16   /* same as TK_NOT      */
#define OP_ToReal                             142   /* same as TK_TO_REAL  */
#define OP_SetNumColumns                      103
#define OP_AbsValue                           104
#define OP_Transaction                        105
#define OP_VFilter                            106
#define OP_Negative                            85   /* same as TK_UMINUS   */
#define OP_Ne                                  67   /* same as TK_NE       */
#define OP_VDestroy                           107
#define OP_ContextPop                         108
#define OP_BitOr                               75   /* same as TK_BITOR    */
#define OP_Next                               109
#define OP_IdxInsert                          110
#define OP_Distinct                           111
#define OP_Lt                                  71   /* same as TK_LT       */
#define OP_Insert                             112
#define OP_Destroy                            113
#define OP_ReadCookie                         114
#define OP_ForceInt                           115
#define OP_LoadAnalysis                       116
#define OP_Explain                            117
#define OP_IfMemZero                          118
#define OP_OpenPseudo                         119
#define OP_OpenEphemeral                      120
#define OP_Null                               121
#define OP_Blob                               122
#define OP_Add                                 78   /* same as TK_PLUS     */
#define OP_MemStore                           123
#define OP_Rewind                             124
#define OP_MoveGe                             127
#define OP_VBegin                             128
#define OP_VUpdate                            129
#define OP_BitNot                              87   /* same as TK_BITNOT   */
#define OP_VCreate                            130
#define OP_MemMove                            131
#define OP_MemNull                            132
#define OP_Found                              133
#define OP_NullRow                            134

/* The following opcode values are never used */
#define OP_NotUsed_135                        135
#define OP_NotUsed_136                        136
#define OP_NotUsed_137                        137

/* Opcodes that are guaranteed to never push a value onto the stack
** contain a 1 their corresponding position of the following mask
** set.  See the opcodeNoPush() function in vdbeaux.c  */
#define NOPUSH_MASK_0 0xeeb4
#define NOPUSH_MASK_1 0x796b
#define NOPUSH_MASK_2 0x7ddb
#define NOPUSH_MASK_3 0xff92
#define NOPUSH_MASK_4 0xffff
#define NOPUSH_MASK_5 0xdaf7
#define NOPUSH_MASK_6 0xfefe
#define NOPUSH_MASK_7 0x99d9
#define NOPUSH_MASK_8 0x7c67
#define NOPUSH_MASK_9 0x0000

/************** End of opcodes.h *********************************************/
/************** Continuing where we left off in vdbe.h ***********************/

/*
** Prototypes for the VDBE interface.  See comments on the implementation
** for a description of what each of these routines does.
*/
Vdbe *sqlite3VdbeCreate(sqlite3*);
void sqlite3VdbeCreateCallback(Vdbe*, int*);
int sqlite3VdbeAddOp(Vdbe*,int,int,int);
int sqlite3VdbeOp3(Vdbe*,int,int,int,const char *zP3,int);
int sqlite3VdbeAddOpList(Vdbe*, int nOp, VdbeOpList const *aOp);
void sqlite3VdbeChangeP1(Vdbe*, int addr, int P1);
void sqlite3VdbeChangeP2(Vdbe*, int addr, int P2);
void sqlite3VdbeJumpHere(Vdbe*, int addr);
void sqlite3VdbeChangeToNoop(Vdbe*, int addr, int N);
void sqlite3VdbeChangeP3(Vdbe*, int addr, const char *zP1, int N);
VdbeOp *sqlite3VdbeGetOp(Vdbe*, int);
int sqlite3VdbeMakeLabel(Vdbe*);
void sqlite3VdbeDelete(Vdbe*);
void sqlite3VdbeMakeReady(Vdbe*,int,int,int,int);
int sqlite3VdbeFinalize(Vdbe*);
void sqlite3VdbeResolveLabel(Vdbe*, int);
int sqlite3VdbeCurrentAddr(Vdbe*);
void sqlite3VdbeTrace(Vdbe*,FILE*);
void sqlite3VdbeResetStepResult(Vdbe*);
int sqlite3VdbeReset(Vdbe*);
int sqliteVdbeSetVariables(Vdbe*,int,const char**);
void sqlite3VdbeSetNumCols(Vdbe*,int);
int sqlite3VdbeSetColName(Vdbe*, int, int, const char *, int);
void sqlite3VdbeCountChanges(Vdbe*);
sqlite3 *sqlite3VdbeDb(Vdbe*);
void sqlite3VdbeSetSql(Vdbe*, const char *z, int n);
const char *sqlite3VdbeGetSql(Vdbe*);
void sqlite3VdbeSwap(Vdbe*,Vdbe*);

#ifndef NDEBUG
  void sqlite3VdbeComment(Vdbe*, const char*, ...);
# define VdbeComment(X)  sqlite3VdbeComment X
#else
# define VdbeComment(X)
#endif

#endif

/************** End of vdbe.h ************************************************/
/************** Continuing where we left off in sqliteInt.h ******************/
/************** Include btree.h in the middle of sqliteInt.h *****************/
/************** Begin file btree.h *******************************************/
/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This header file defines the interface that the sqlite B-Tree file
** subsystem.  See comments in the source code for a detailed description
** of what each interface routine does.
**
** @(#) $Id: //DWF/Development/Components/Internal/DWF Core/v1.7/develop/global/src/dwfcore/sqlite/sqlite3.h#1 $
*/
#ifndef _BTREE_H_
#define _BTREE_H_

/* TODO: This definition is just included so other modules compile. It
** needs to be revisited.
*/
#define SQLITE_N_BTREE_META 10

/*
** If defined as non-zero, auto-vacuum is enabled by default. Otherwise
** it must be turned on for each database using "PRAGMA auto_vacuum = 1".
*/
#ifndef SQLITE_DEFAULT_AUTOVACUUM
  #define SQLITE_DEFAULT_AUTOVACUUM 0
#endif

/*
** Forward declarations of structure
*/
typedef struct Btree Btree;
typedef struct BtCursor BtCursor;
typedef struct BtShared BtShared;


int sqlite3BtreeOpen(
  const char *zFilename,   /* Name of database file to open */
  sqlite3 *db,             /* Associated database connection */
  Btree **,                /* Return open Btree* here */
  int flags                /* Flags */
);

/* The flags parameter to sqlite3BtreeOpen can be the bitwise or of the
** following values.
**
** NOTE:  These values must match the corresponding PAGER_ values in
** pager.h.
*/
#define BTREE_OMIT_JOURNAL  1  /* Do not use journal.  No argument */
#define BTREE_NO_READLOCK   2  /* Omit readlocks on readonly files */
#define BTREE_MEMORY        4  /* In-memory DB.  No argument */

int sqlite3BtreeClose(Btree*);
int sqlite3BtreeSetBusyHandler(Btree*,BusyHandler*);
int sqlite3BtreeSetCacheSize(Btree*,int);
int sqlite3BtreeSetSafetyLevel(Btree*,int,int);
int sqlite3BtreeSyncDisabled(Btree*);
int sqlite3BtreeSetPageSize(Btree*,int,int);
int sqlite3BtreeGetPageSize(Btree*);
int sqlite3BtreeGetReserve(Btree*);
int sqlite3BtreeSetAutoVacuum(Btree *, int);
int sqlite3BtreeGetAutoVacuum(Btree *);
int sqlite3BtreeBeginTrans(Btree*,int);
int sqlite3BtreeCommitPhaseOne(Btree*, const char *zMaster);
int sqlite3BtreeCommitPhaseTwo(Btree*);
int sqlite3BtreeCommit(Btree*);
int sqlite3BtreeRollback(Btree*);
int sqlite3BtreeBeginStmt(Btree*);
int sqlite3BtreeCommitStmt(Btree*);
int sqlite3BtreeRollbackStmt(Btree*);
int sqlite3BtreeCreateTable(Btree*, int*, int flags);
int sqlite3BtreeIsInTrans(Btree*);
int sqlite3BtreeIsInStmt(Btree*);
int sqlite3BtreeIsInReadTrans(Btree*);
void *sqlite3BtreeSchema(Btree *, int, void(*)(void *));
int sqlite3BtreeSchemaLocked(Btree *);
int sqlite3BtreeLockTable(Btree *, int, u8);

const char *sqlite3BtreeGetFilename(Btree *);
const char *sqlite3BtreeGetDirname(Btree *);
const char *sqlite3BtreeGetJournalname(Btree *);
int sqlite3BtreeCopyFile(Btree *, Btree *);

/* The flags parameter to sqlite3BtreeCreateTable can be the bitwise OR
** of the following flags:
*/
#define BTREE_INTKEY     1    /* Table has only 64-bit signed integer keys */
#define BTREE_ZERODATA   2    /* Table has keys only - no data */
#define BTREE_LEAFDATA   4    /* Data stored in leaves only.  Implies INTKEY */

int sqlite3BtreeDropTable(Btree*, int, int*);
int sqlite3BtreeClearTable(Btree*, int);
int sqlite3BtreeGetMeta(Btree*, int idx, u32 *pValue);
int sqlite3BtreeUpdateMeta(Btree*, int idx, u32 value);

int sqlite3BtreeCursor(
  Btree*,                              /* BTree containing table to open */
  int iTable,                          /* Index of root page */
  int wrFlag,                          /* 1 for writing.  0 for read-only */
  int(*)(void*,int,const void*,int,const void*),  /* Key comparison function */
  void*,                               /* First argument to compare function */
  BtCursor **ppCursor                  /* Returned cursor */
);

void sqlite3BtreeSetCompare(
  BtCursor *,
  int(*)(void*,int,const void*,int,const void*),
  void*
);

int sqlite3BtreeCloseCursor(BtCursor*);
int sqlite3BtreeMoveto(BtCursor*,const void *pKey,i64 nKey,int bias,int *pRes);
int sqlite3BtreeDelete(BtCursor*);
int sqlite3BtreeInsert(BtCursor*, const void *pKey, i64 nKey,
                                  const void *pData, int nData, int bias);
int sqlite3BtreeFirst(BtCursor*, int *pRes);
int sqlite3BtreeLast(BtCursor*, int *pRes);
int sqlite3BtreeNext(BtCursor*, int *pRes);
int sqlite3BtreeEof(BtCursor*);
int sqlite3BtreeFlags(BtCursor*);
int sqlite3BtreePrevious(BtCursor*, int *pRes);
int sqlite3BtreeKeySize(BtCursor*, i64 *pSize);
int sqlite3BtreeKey(BtCursor*, u32 offset, u32 amt, void*);
const void *sqlite3BtreeKeyFetch(BtCursor*, int *pAmt);
const void *sqlite3BtreeDataFetch(BtCursor*, int *pAmt);
int sqlite3BtreeDataSize(BtCursor*, u32 *pSize);
int sqlite3BtreeData(BtCursor*, u32 offset, u32 amt, void*);

char *sqlite3BtreeIntegrityCheck(Btree*, int *aRoot, int nRoot, int, int*);
struct Pager *sqlite3BtreePager(Btree*);


#ifdef SQLITE_TEST
int sqlite3BtreeCursorInfo(BtCursor*, int*, int);
void sqlite3BtreeCursorList(Btree*);
#endif

#ifdef SQLITE_DEBUG
int sqlite3BtreePageDump(Btree*, int, int recursive);
#else
#define sqlite3BtreePageDump(X,Y,Z) SQLITE_OK
#endif

#endif /* _BTREE_H_ */

#endif /* _SQLITEINT_H_ */

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
#endif
