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


#ifndef _ODRXSYSTEMSERVICES_INCLUDED_
#define _ODRXSYSTEMSERVICES_INCLUDED_

#include <stdarg.h>

#include "RxObject.h"
#include "OdStreamBuf.h"
#include "OdCodePage.h"
#if defined(_DEBUG) && defined(_WIN32)
#define STL_USING_VECTOR
#include "OdaSTL.h"
#endif

class OdTimeStamp;
class OdRxModule;
class OdRxDictionary;
typedef OdSmartPtr<OdRxDictionary> OdRxDictionaryPtr;

#include "TD_PackPush.h"

/** \details

    <group TD_Namespaces>
*/
namespace Oda
{
  ///////////////// FILE SYSTEM SERVICES //////////////////


  enum FileAccessMode   // Access
  {
    kFileRead     = (long)
                    0x80000000,   // Read access. Use kFileRead | kFileWrite for read/write access.
    kFileWrite    = 0x40000000,   // Write access. Use kFileRead | kFileWrite for read/write access.
    kFileTmp      = 0x20000000,   // hint to use FILE_ATTRIBUTE_TEMPORARY  for 'FlagsAndAttributes'
    kFileDelete   = 0x10000000,   // hint to use FILE_FLAG_DELETE_ON_CLOSE for 'FlagsAndAttributes'
    kNoFlushWhenClosed = 0x08000000       //do NOT call FlushFileBuffers when closing file (tremendously accelerates writing small files)
  };

  enum FileShareMode
  {
    kShareDenyReadWrite = 0x10, // deny read/write mode
    kShareDenyWrite     = 0x20, // deny write mode
    kShareDenyRead      = 0x30, // deny read mode
    kShareDenyNo        = 0x40  // deny none mode
  };


  enum FileCreationDisposition
  {
    kCreateNew        = 1,  // Creates a new file; fails if the specified file already exists.
    kCreateAlways     = 2,  // Creates a new file; overwrites any existing file.
    kOpenExisting     = 3,  // Opens the file; fails if the file does not exist.
    kOpenAlways       = 4,  // Opens the file; creates the file if it does not exist.
    kTruncateExisting = 5   // Truncates the file; fails if the file does not exist. The file must be open at least kFileWrite.
  };

  enum UserNameFormat 
  {
    kUserId   = 1, //Current user id
    kUserName = 2  //A "friendly" user name (for example, Jeff Smith).
  };
}

/** \details
    This class implements OdError objects with access to the Result codes and filenames
    of the objects related to the errors.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_FileException : public OdError
{
public:
  /** \param code [in]  Result code.
    \param filename [in]  Filename.
  */
  OdError_FileException(OdResult code, const OdString& filename, const OdString& extended = OdString::kEmpty);
  /** \details
    Returns the filename of this Error object.
  */
  OdString getFileName() const;
};

/** \details
    This class implements FileException object with a Result code of eFileNotFound,
    and the filename of the object related to the error.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_FileNotFound : public OdError_FileException
{
public:
  /** \param filename [in]  Filename.
  */
  OdError_FileNotFound(const OdString& filename);
};

/** \details
    This class implements FileException object with a Result code of eFileException,
    and the filename of the object related to the error.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_CantOpenFile : public OdError_FileException
{
public:
  /** \param filename [in]  Filename.
  */
  OdError_CantOpenFile(const OdString& filename);
};

/** \details
    This class implements FileException object with a Result code of eFileWriteError,
    and the filename of the object related to the error.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_FileWriteError : public OdError_FileException
{
public:
  /** \param filename [in]  Filename.
  */
  OdError_FileWriteError(const OdString& filename);
};

/** \details
    This class implements OdError objects with Result codes of eInvalidIndex.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_InvalidIndex : public OdError
{
public:
  OdError_InvalidIndex();
};

/** \details
    This class implements OdError objects with Result codes of eInvalidKey.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_InvalidKey: public OdError
{
public:
  OdError_InvalidKey();
};

/** \details
    This class is the base class for classes that provide platform-dependent file operations for
    Teigha.
    
    Corresponding C++ library: TD_Root
    
    <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT OdRxSystemServices : public OdRxObject
{
public:

  OdRxSystemServices()
  {
#if defined _WIN32_WCE
    srand(GetTickCount());
#else
    Od_srand((unsigned int) time(0));
#endif
  }
  ///////////////// FILE SYSTEM SERVICES //////////////////

  /** \details
    Creates and/or opens the specified file.
    \param filename [in]  Filename.
    \param accessMode [in]  Access mode.
    \param shareMode [in]  Share mode.
    \param creationDisposition [in]  Creation disposition.
    
    \returns
    Returns a SmartPointer to an OdStreamBuf object for the file. 
    \remarks
    accessMode must be a combination of one or more of the following:
    
    <table>
    Name                Value          Description
    Oda::kFileRead      0x80000000     Read access.
    Oda::kFileWrite     0x40000000     Write access.
    </table>
    
    shareMode must be one of the following:
    
    <table>
    Name                         Value       Description
    Oda::kShareDenyReadWrite     0x10        deny read/write mode
    Oda::kShareDenyWrite         0x20        deny write mode
    Oda::kShareDenyRead          0x30        deny read mode
    Oda::kShareDenyNo            0x40        deny none mode
    </table>

    Creation disposition must be one of the following:
    
    <table>
    Name                         Value     Description
    Oda::kCreateNew              1         Creates a new file; fails if the specified file already exists.
    Oda::kCreateAlways           2         Creates a new file; overwrites any existing file.
    Oda::kOpenExisting           3         Opens the file; fails if the file does not exist.
    Oda::kOpenAlways             4         Opens the file; creates the file if it does not exist.
    Oda::kTruncateExisting       5         Truncates the file; fails if the file does not exist. The file must be open at least Oda::kFileWrite.
    </table>
  */
  virtual OdStreamBufPtr createFile(
    const OdString& filename,                    
    Oda::FileAccessMode accessMode = Oda::kFileRead,
    Oda::FileShareMode  shareMode = Oda::kShareDenyNo,                          
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);

  /** \details
    Returns true if and only if the specified file can be accessed in the specified mode.

    \param filename [in]  Filename.
    \param accessMode [in]  Access mode.
    
    \remarks
    accessMode must be a combination of one or more of the following:
    
    <table>
    Name                Value          Description
    ..                  0              File exists.
    Oda::kFileRead      0x80000000     Read access.
    Oda::kFileWrite     0x40000000     Write access.
    </table>

  */
  virtual bool accessFile(const OdString& filename, int accessMode);

  /** \details
    Returns the creation time of the specified file.
    \param filename [in]  Filename.
    
    \remarks
    On most platforms, this function will be implemented with the stat function (stat::st_ctime).
  */
  virtual OdInt64 getFileCTime(const OdString& filename) = 0;

  /** \details
    Returns the modification time of the specified file.
    \param filename [in]  Filename.
    
    \remarks
    On most platforms, this function will be implemented with the stat function (stat::st_mtime).
  */
  virtual OdInt64 getFileMTime(const OdString& filename) = 0;

  /** \details
    Returns the size of the specified file.
    \param filename [in]  Filename.
    
    \remarks
    On most platforms, this function will be implemented with the stat function (stat::st_size).
  */
  virtual OdInt64 getFileSize(const OdString& filename) = 0;


  /** \details
    Returns a message string formatted using a vsprintf-style format string and argument list.
    \param formatID [in]  String resource identifier containing the format string. 
    \param argList [in]  Argument list.   
  */
  virtual OdString formatMessage(unsigned int formatId, va_list* argList= 0) = 0;

  /** \details
    Assigns a value to this String object using a printf-style format string and arguments.
    \param formatID [in]  String resource identifier containing the format string. 
    \returns
    Returns a reference to this String Object.
  */
  virtual OdString formatMsg(unsigned int formatId, ...);

  /** \details
    Returns the system code page.
  */
  virtual OdCodePageId systemCodePage() const = 0;
  
  /** \details
    Returns a string representation for newly generated Globally Unique IDentifier (GUID) for this SystemServices object.
  */
  virtual OdString createGuid();

  /** \details
    Returns a newly generated Globally Unique IDentifier (GUID) for this SystemServices object.
  */
  virtual OdGUID createOdGUID();

  /** \details
    Loads the specified physical (platform-dependent) module.
    
    \remarks
    Appends the appropriate version suffix onto the supplied module name.
    Returns the abstract (platform independent) module object,    
    which represents module functionality in Teigha.

    \param moduleFileName [in]  Module filename to load.
    \param silent [in]  If true, no load status message will be printed.
  */
  virtual OdRxModule* loadModule(const OdString& moduleFileName, bool silent);

  /** \details
    Unloads the specified physical (platform-dependent) module.

    \param pModuleObj [in]  Pointer to the module object to be unloaded.
  */
  virtual void unloadModule(OdRxModule* pModuleObj);

  /** \details
    Returns the filename associated with the specified application name.

    \param applicationName [in]  TX application name.
  */
  virtual OdString findModule(const OdString& applicationName);

  /** \details
    Displays in some manner the specified warning message.
    
    \param message [in]  Warning message.
  */
  virtual void warning(const OdString& message);
  
  /** \details
    Displays in some manner the specified warning message.
    
    \param warnVisGroup [in]  Group of warning message visibility (always visible if it is an empty).
    \param message [in]  Warning message.
  */
  virtual void warning(const char* warnVisGroup, const OdString& message);

  /** \details
    Loads the specified physical (platform-dependent) module.

    \returns
    Returns the abstract (platform independent) module object,    
    which represents module functionality in Teigha.

    \param moduleFileName [in]  Module filename to load.
    \param silent [in]  If true, no load status message will be printed.
  */
  static OdRxModule* loadModuleLib(const OdString& moduleFileName, bool silent);

  /** \details
  Inits/Uninits the modeler library (platform-dependent) module.

  \returns
  Should return eOk, if (un)initialization is implemented inside (un)initModelerLibrary.
  Or eNotImplemented if default module (un)initialization required.

  \remarks
   Used for custom initialization/uninitialization of Spatial ACIS library.
  */
  virtual OdResult initModelerLibrary(OdRxDictionary *pDict);
  virtual OdResult uninitModelerLibrary(OdRxDictionary *pDict);

  /** \details
  Get environment variable value.
  */
  virtual OdResult getEnvVar(const OdString &varName, OdString &value) = 0;

  /** \details
  Set environment variable value.
  */
  virtual OdResult setEnvVar(const OdString &varName, const OdString &newValue) = 0;

  /** \details
  Returns the path to the temporary files folder.
  */
  virtual OdString getTemporaryPath() const;
  
  /** \details
  Returns user name in the requested format
  */
  virtual OdString getUserName(Oda::UserNameFormat unFormat = Oda::kUserId);

  /** \details
  Returns the unique name for temporary file.
  */
  virtual OdString getTempFileName();

#if defined(_DEBUG) && defined(_WIN32)
  static void dontFreeLibrary(std::vector<HMODULE>* notFreedDlls) { s_notFreedDlls = notFreedDlls; }
private:
  FIRSTDLL_EXPORT_STATIC static std::vector<HMODULE>* s_notFreedDlls;
#endif
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxSystemServices object pointers.
*/
typedef OdSmartPtr<OdRxSystemServices> OdRxSystemServicesPtr;

FIRSTDLL_EXPORT OdRxSystemServices* odrxSystemServices();

#include "TD_PackPop.h"

#endif // #ifndef _ODRXSYSTEMSERVICES_INCLUDED_
