//  Copyright (c) 2000-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

/* zip.h -- IO for compress .zip files using zlib
   Version 0.15 alpha, Mar 19th, 1998,

   Copyright (C) 1998 Gilles Vollant

   This unzip package allow creates .ZIP file, compatible with PKZip 2.04g
     WinZip, InfoZip tools and compatible.
   Multi volume ZipFile (span) are not supported.
   Old compressions used by old PKZip 1.x are not supported

  For uncompress .zip file, look at unzip.h

   Condition of use and distribution are the same than zlib :

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

*/

/* for more info about .ZIP format, see
      http://www.pkware.com/products/enterprise/white_papers/appnote.html
*/

//Note: Encryption (password) support added by Jeffrey Klug, Autodesk, Inc. 6/14/2003

/* this software has been significantly altered from its original state in that
   it has been customized for the Dwf file format */

#ifndef _zip_H
#define _zip_H

#include "dwfcore/Core.h"
#include "dwfcore/String.h"
#include "dwfcore/FileDescriptor.h"
#include "dwfcore/OutputStream.h"
using namespace DWFCore;


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ZLIB_H
#if     defined(DWFCORE_BUILD_ZLIB) || defined(DWFTK_USE_DWFCORE_ZLIB)
#include "dwfcore/zlib/zlib.h"
#else
#ifdef ANDROID
#include <ZLib/zlib.h>
#else
#include <zlib.h>
#endif
#ifdef ZEXTERN
#undef ZEXTERN
#define ZEXTERN _DWFCORE_API
#endif
#endif
#endif

#ifndef DWF_RENAME_LOCAL_ZIP_FUCTIONALITY
#define DWF_RENAME_LOCAL_ZIP_FUCTIONALITY
#endif

#ifdef DWF_RENAME_LOCAL_ZIP_FUCTIONALITY
#ifndef ODA_DWF_RENAME_LOCAL_ZIP
#define ODA_DWF_RENAME_LOCAL_ZIP(a) oda_dwf_rename_local_zip_##a
#endif

#define zipOpen                             ODA_DWF_RENAME_LOCAL_ZIP(zipOpen)
#define zipOpenNewFileInZip                 ODA_DWF_RENAME_LOCAL_ZIP(zipOpenNewFileInZip)
#define zipWriteInFileInZip                 ODA_DWF_RENAME_LOCAL_ZIP(zipWriteInFileInZip)
#define zipCloseFileInZip                   ODA_DWF_RENAME_LOCAL_ZIP(zipCloseFileInZip)
#define zipClose                            ODA_DWF_RENAME_LOCAL_ZIP(zipClose)
#endif

#if !defined(DEF_MEM_LEVEL)
#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
/* default memLevel */
#endif

#if defined(STRICTZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */
typedef struct TagzipFile__ { int unused; } zipFile__;
typedef zipFile__ *zipFile;
#else
typedef voidp zipFile;
#endif

#define ZIP_OK                                  (0)
#define ZIP_ERRNO               (Z_ERRNO)
#define ZIP_PARAMERROR                  (-102)
#define ZIP_INTERNALERROR               (-104)

/* tm_zip contain date/time info */
typedef struct tm_zip_s
{
    uInt tm_sec;            /* seconds after the minute - [0,59] */
    uInt tm_min;            /* minutes after the hour - [0,59] */
    uInt tm_hour;           /* hours since midnight - [0,23] */
    uInt tm_mday;           /* day of the month - [1,31] */
    uInt tm_mon;            /* months since January - [0,11] */
    uInt tm_year;           /* years - [1980..2044] */
} tm_zip;

typedef struct
{
    tm_zip      tmz_date;       /* date in understandable format           */
    uLong       dosDate;       /* if dos_date == 0, tmu_date is used      */
    uLong       internal_fa;    /* internal file attributes        2 bytes */
    uLong       external_fa;    /* external file attributes        4 bytes */
} zip_fileinfo;

#ifndef Z_BUFSIZE
#define Z_BUFSIZE (16384)
#endif

typedef struct
{
    z_stream stream;            /* zLib stream structure for inflate */
    int  stream_initialised;    /* 1 is stream is initialised */
    uInt pos_in_buffered_data;  /* last written byte in buffered_data */

    uLong pos_local_header;     /* offset of the local header of the file
                                     currenty writing */
    char* central_header;       /* central header data for the current file */
    uLong size_centralheader;   /* size of the central header for cur file */
    uLong flag;                 /* flag of the file currently writing */

    int  method;                /* compression method of file currenty wr.*/
    Byte buffered_data[Z_BUFSIZE];/* buffer contain compressed data to be writ*/
    uLong dosDate;
    uLong crc32;
    unsigned long key[3];       /* decompression keys*/
    uLong pos_file_data;        /* offset of the beginning of the file data of the file
                                     currenty writing */
    char *header_data_block;    /* pointer to the cached data block where this file's
                                   central header record resides */
} curfile_info;

#define SIZEDATA_INDATABLOCK (4096-(4*4))

typedef struct linkedlist_datablock_internal_s
{
  struct linkedlist_datablock_internal_s* next_datablock;
  uLong  avail_in_this_block;
  uLong  filled_in_this_block;
  uLong  unused; /* for future use and alignement */
  unsigned char data[SIZEDATA_INDATABLOCK];
} linkedlist_datablock_internal;

typedef struct linkedlist_data_s
{
    linkedlist_datablock_internal* first_block;
    linkedlist_datablock_internal* last_block;
} linkedlist_data;

typedef struct zip_internal_s
{
    DWFFileDescriptor* filezip;
    bool               own_descriptor;
    linkedlist_data central_dir;/* datablock with central dir in construction*/
    int  in_opened_file_inzip;  /* 1 if a file in the zip is currently writ.*/
    curfile_info ci;            /* info on the file curretly writing */

    uLong begin_pos;            /* position of the beginning of the zipfile */
    uLong number_entry;
} zip_internal;

ZEXTERN zipFile ZEXPORT zipOpenStream OF(( DWFFileDescriptor* pDescriptor, const void* pHeader = NULL, size_t nHeaderBytes = 0));
/*
  Create a zipfile in memory.
*/

ZEXTERN zipFile ZEXPORT zipOpen OF((const DWFString& zPath, const void* pHeader = NULL, size_t nHeaderBytes = 0));
/*
  Create a zipfile.
    pathname contain on Windows NT a filename like "c:\\zlib\\zlib111.zip" or on
        an Unix computer "zlib/zlib111.zip".
    if the file pathname exist and append=1, the zip will be created at the end
        of the file. (useful if the file contain a self extractor code)
    If the zipfile cannot be opened, the return value is NULL.
    Else, the return value is a zipFile Handle, usable with other function
        of this zip package.
*/

ZEXTERN zipFile  dwfOpen OF((const DWFString& zPath, unsigned int nMajorRevision, unsigned int nMinorRevision));
/*
  Create a DWF zipfile.
*/

ZEXTERN zipFile ZEXPORT zipOpenArchive OF((zip_internal& ziinit, const void* pHeader = NULL, size_t nHeaderBytes = 0));

ZEXTERN int ZEXPORT zipBlankBuffer OF((void* buffer, uInt size_blank));

ZEXTERN int ZEXPORT zipBlankFileBytes OF((zipFile file, uLong position, uInt size_blank));

ZEXTERN int ZEXPORT zipOpenNewFileInZip OF((zipFile file,
                       const DWFString& filename,
                       const zip_fileinfo* zipfi,
                       const void* extrafield_local,
                       uInt size_extrafield_local,
                       const void* extrafield_global,
                       uInt size_extrafield_global,
                       const DWFString& comment,
                       int method,
                       int level,
                       const DWFString& password));
/*
  Open a file in the ZIP for writing.
  filename : the filename in zip (if NULL, '-' without quote will be used
  *zipfi contain supplemental information
  if extrafield_local!=NULL and size_extrafield_local>0, extrafield_local
    contains the extrafield data for the local header
  if extrafield_global!=NULL and size_extrafield_global>0, extrafield_global
    contains the extrafield data for the global header
  if comment != NULL, comment contain the comment string
  method contain the compression method (0 for store, Z_DEFLATED for deflate)
  level contain the level of compression (can be Z_DEFAULT_COMPRESSION)
  password (const char *password), is used for password encrypted files, and
  should be set to NULL for non-protected files
*/

ZEXTERN int ZEXPORT zipWriteInFileInZip OF((zipFile file,
                       const void* buf,
                       unsigned len));
/*
  Write data in the zipfile
*/

ZEXTERN int ZEXPORT zipCloseFileInZip OF((zipFile file));
/*
  Close the current file in the zipfile
*/

ZEXTERN int ZEXPORT zipClose OF((zipFile file,
                const char* global_comment = 0));
/*
  Close the zipfile
*/

#ifdef __cplusplus
}
#endif

#endif /* _zip_H */
