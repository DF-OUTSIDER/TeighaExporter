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




#ifndef _ODDB_BASEHOSTAPPSERVICES_INCLUDED_
#define _ODDB_BASEHOSTAPPSERVICES_INCLUDED_


#include "TD_PackPush.h"


#include "DbRootExport.h"
#include "DbBaseDatabase.h"
#include "DbHostAppProgressMeter.h"
#include "RxObject.h"
#include "SmartPtr.h"
#include "OdFont.h"
#include "BrepBuilder/BrepBuilder.h"

class OdGsDevice;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdGsDevice object pointers.
*/
typedef OdSmartPtr<OdGsDevice> OdGsDevicePtr;

class OdDbUndoController;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbUndoController object pointers.
*/
typedef OdSmartPtr<OdDbUndoController> OdDbUndoControllerPtr;

class OdAuditInfo;
class OdTtfDescriptor;


typedef unsigned long LCID;

enum OdSDIValues
{
  kMDIEnabled             = 0,
  kSDIUserEnforced        = 1,
  kSDIAppEnforced         = 2,
  kSDIUserAndAppEnforced  = 3
};

enum ProdIdCode
{
  kProd_ACAD = 1, // Plain
  kProd_LT   = 2, // Lite
  kProd_OEM  = 3, // OEM
  kProd_OdDb = 4  // ..
};


/** \details
    This class is the base class for platform specific operations within Teigha.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class ODRX_ABSTRACT DBROOT_EXPORT OdDbBaseHostAppServices : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbBaseHostAppServices);

  OdDbBaseHostAppServices();

  enum RemapFileContext
  {
    kDrawingOpen        = 0,
    kXrefResolution     = 1,
    kRasterResolution   = 2
  };

  enum FindFileHint
  {
    kDefault              = 0,  // Any file.
    kFontFile             = 1,  // Can be either SHX or TTF file.
    kCompiledShapeFile    = 2,  // SHX file.
    kTrueTypeFontFile     = 3,  // TTF file.
    kEmbeddedImageFile    = 4,  // Image file (ISM).
    kXRefDrawing          = 5,  // Drawing template file (DWT).
    kPatternFile          = 6,  // Pattern file (PAT).
    kTXApplication        = 7,  // Teigha(R) Xtension file (TX).
    kFontMapFile          = 8,  // FontMap file (FMP).
    kUnderlayFile         = 9,  // Underlay file.
    kTextureMapFile       = 10, // Material texture file.
    kPhotometricWebFile   = 11  // Photometric web file (IES).
  };

  enum GsBitmapDeviceFlags
  {
    kUseSoftwareHLR       = (1 << 0), // Enable use of SoftwareHLR on device
    kFor2dExportRender    = (1 << 1), // Device for shaded viewport export
    kFor2dExportRenderHLR = (1 << 2), // Device for HiddenLine viewport export
    kForThumbnail         = (1 << 3)  // Device for thumbnail image generation
  };

  /** \details
    Returns the fully qualified path to the specified file.

    \param filename [in]  Name of the file to find.
    \param pDb [in]  Pointer to the database context.
    \param hint [in]  Hint that indicates the type of file that is required.

    \returns
    Returns an empty string if the file is not found.

    This function is called by Teigha when access is needed to a
    file, such as a font file, a template file, etc.

    pDb == 0 specifies that this file search is not related to a database.

    If pDb != 0, call pDb->getFilename() to determine the path to the DWG
    file associated with the database.

    The typical search order is as follows:

    1.  The filename itself.
    2.  The current directory.
    3.  The drawing directory (for shx font, image, and xref files).
    4.  The directories listed in the ACAD environment variable.
    5.  The Windows fonts directory (only for TTF fonts on Windows).

    hint must be one of the following:

    <table>
    Name                    Value     Description Extension
    kDefault                0         Any file.                         any
    kFontFile               1         Can be either SHX or TTF file.    SHX or TTF
    kCompiledShapeFile      2         SHX file.                         SHX
    kTrueTypeFontFile       3         TTF file.                         TTF
    kEmbeddedImageFile      4         Image file.                       ISM
    kXRefDrawing            5         Drawing template file.            DWT
    kPatternFile            6         Pattern file (PAT)                PAT
    kTXApplication          7         Teigha(R) Xtension file.            TX
    kFontMapFile            8         FontMap file                      FMP
    kUnderlayFile           9         Underlay file
    kTextureMapFile        10
    </table>

    \remarks
    The client application can, in fact return any non-null string, so long as
    the application can handle that string in OdDbSystemServices::createFile.
  */
  virtual OdString findFile(
    const OdString& filename,
    OdDbBaseDatabase* pDb = 0,
    FindFileHint hint = kDefault) = 0;

  /** \details
    Returns an instance of an OdDbHostAppProgressMeter.
  */
  virtual OdDbHostAppProgressMeter* newProgressMeter();

  /** \details
    Notification function called whenever Teigha no longer needs the specified
    ProgressMeter object.

    \param pProgressMeter [in]  Pointer to the ProgressMeter object no longer needed by Teigha.

    \remarks
    The default implementation of this function does nothing but return.
  */
  virtual void releaseProgressMeter(
    OdDbHostAppProgressMeter* pProgressMeter);

  /** \details
    Returns the name of the client program.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns oddbGetLibraryInfo()->getLibName()
  */
  virtual const OdString program() = 0;

  /** \details
    Returns the name of the client product.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns program().
  */
  virtual const OdString product() = 0;

  /** \details
    Returns the name of the client company.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns oddbGetLibraryInfo()->getCompanyName().
  */
  virtual const OdString companyName() = 0;

  /** \details
    Returns the name of the client product code.

    \remarks
    This function is typically used for populating "About" information.

    prodcode() returns one of the following:

    <table>
    Name          Value   Description
    kProd_ACAD    1       Plain
    kProd_LT      2       Lite
    kProd_OEM     3       OEM
    kProd_OdDb    4       ..
    </table>

    \remarks
    The default implementation of this function returns kProd_OEM, and should not be overridden.
  */
  virtual ProdIdCode prodcode();

  /** \details
    Returns the release major and minor version string of the client application.

    \remarks
    The default implementation of this function returns oddbGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString releaseMajorMinorString();

  /** \details
    Returns the release major version of the client application.

    \remarks
    The default implementation of this function returns TD_MAJOR_VERSION.
  */
  virtual int releaseMajorVersion();

  /** \details
    Returns the release minor version of the client application.

    \remarks
    The default implementation of this function returns TD_MINOR_VERSION.
  */
  virtual int releaseMinorVersion();

  /** \details
    Returns the release version string of the client application.

    \remarks
    The default implementation of this function returns oddbGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString versionString();

  /** \details
    Displays the specified warning message.

    \param message [in]  Warning message.

    \remarks
    This function calls odSystemServices()->warning(message) to display the warning.
  */
  virtual void warning(
    const OdString& message);
  
  /** \details
    Displays the specified warning message.

    \param warnVisGroup [in]  Group of warning message visibility (always visible if empty).
    \param message [in]  Warning message.

    \remarks
    This function calls odSystemServices()->warning(message) to display the warning.
  */
  virtual void warning(
    const char* warnVisGroup,
    const OdString& message);
    
  /** \details
    Displays the specified warning message.

    \param warningOb [in]  Warning object.
  */
  virtual void warning(
    OdWarning warningOb);
    
  /** \details
    Displays the specified warning message.

    \param warnVisGroup [in]  Group of warning message visibility (always visible if empty).
    \param warningOb [in]  Warning object.
  */
  virtual void warning(
    const char* warnVisGroup,
    OdWarning warningOb);

  //virtual void warning(
  //  OdWarning warningOb,
  //  OdDbObjectId objectId);

  /** \details
    Returns the error description associated with the specified error code.
    \param errorCode [in]  Error code.

    \remarks
    The default implementation of this function returns formatMessage(errorCode).
  */
  virtual OdString getErrorDescription(
    unsigned int errorCode);

  /** \details
    Returns a formatted message corresponding to the specified error code(s).
    \param errorCode [in]  Error code.
  */
  virtual OdString formatMessage(
    unsigned int errorCode,...);
#ifdef ODA_FORMAT
  /** \details
    Returns a formatted message corresponding to the specified error code(s).
    \param errorCode [in]  Error code.
  */
  virtual OdString formatMessage(unsigned int errorCode, long i, const OdString& str) { return formatMessage(errorCode, i, (void*)str.c_str()); }
#endif

  /** \details
    Returns an alternate UndoController object for Teigha.

    \remarks
    The default implementation of this function always returns a null SmartPointer.

    Each database requires at most 2 undo controllers (for undo and redo).
  */
  virtual OdDbUndoControllerPtr newUndoController();

  /** \details
  Returns an alternate stream object for undo/redo operations.

  \remarks
  The default implementation of this function always returns a null SmartPointer.
  It may return null in the user application. In this case the default memory based stream will be used.
  In fact this stream will be used for the current undo/redo step. Other steps can be controlled by
  OdDbUndoController.
  */
  virtual OdStreamBufPtr newUndoStream();

  /** \details
    Print the audit report for the specified AuditInfo object.

    \param pAuditInfo [in]  Pointer to an OdDbAuditInfo object.
    \param strLine [in]  The string to print.
    \param printDest [in]  Print destination.

    \remarks
    The default implementation of this function does nothing but return.
  */
  virtual void auditPrintReport(
    OdAuditInfo* pAuditInfo,
    const OdString& strLine,
    int printDest) const;

  /** \details
    Locates the TTF or TTC file containing the specified font description.

    \param description [in]  Font description.
    \param filename [out]  Receives the name of the TrueType font file.

    \returns
    Returns true and the filename if and only if the font file was found.

    \remarks
    The non-Windows builds of this function do nothing but return.
  */
  virtual bool ttfFileNameByDescriptor(
    const OdTtfDescriptor& description,
    OdString& filename);

  /** \details
    Returns the font file to be used when a given font file is
    not found by Teigha.

    \sa
    Font Handling
  */
  virtual OdString getAlternateFontName() const = 0;

  /** \details
    Returns the name of the font mapping file used by the getPreferableFont function.

    \sa
    Font Handling

    \remarks
    The default implementation of this function does nothing but return an empty string.
    It will be fully implemented in a future release.
  */
  virtual OdString getFontMapFileName() const = 0;

  /** \details
    Returns the preferable font name for the specified font name and type.


    \param fontName [in]  Font name.
    \param fontType [in]  Font type.

    \remarks
    This function is called as the first step in the process of resolving a font file.
    The default implementation tries to locate a font mapping file by calling
    getFontMapFileName, and substitutes the font name based on the contents of this
    file.

    fontType must be one of the following:

    <table>
    Name                    Value    Description
    kFontTypeUnknown        0        Unknown.
    kFontTypeShx            1        SHX font.
    kFontTypeTrueType       2        TrueType font.
    kFontTypeShape          3        Shape file.
    kFontTypeBig            4        BigFont file.
    </table>

    \sa
    Font Handling
  */
  virtual OdString getPreferableFont(
    const OdString& fontName,
    OdFontType fontType) = 0;

  /** \details
    Returns the font to be used when the specified font is not found.

    \param fontName [in]  Font name.
    \param fontType [in]  Font type.

    \remarks
    fontType must be one of the following:

    <table>
    Name                    Value    Description
    kFontTypeUnknown        0        Unknown.
    kFontTypeShx            1        SHX font.
    kFontTypeTrueType       2        TrueType font.
    kFontTypeShape          3        Shape file.
    kFontTypeBig            4        BigFont file.
    </table>

    The default implementation of this function calls getAlternateFontName for
    fonts that are not of type kFontTypeShape or kFontTypeBig. When they are,
    and empty string is returned.

    Client code could override this function to perform custom substitution
    for these types of fonts.

    \sa
    Font Handling
  */
  virtual OdString getSubstituteFont(
    const OdString& fontName,
    OdFontType fontType) = 0;

  /** \details
    Returns the typeface name of the TTF font which has the character.

    \param pFont [in]        Current font.
    \param unicodeChar [in]  Current symbol.
    \param pDb [in]          Pointer to the database context.

    \sa
    Font Handling

    \remarks
    The default implementation uses Windows API for getting the font.
  */
  virtual OdString getSubstituteFontByChar(
    const OdFont& pFont,
    OdChar unicodeChar, OdDbBaseDatabase *pDb) { return OdString::kEmpty; }

  /** \details
    Collects folders with TrueType fonts.

    \param aDirs [out]   Array names of folders.

    \sa
    Font Handling
  */
  virtual bool getSystemFontFolders(
    OdStringArray& aDirs);

  /** \details
    Collects TrueType font paths.

    \param res [out]      Array of full file names of TrueType fonts.
    \param sPath [in]    Array of folders paths with TrueType files.
    \param sFilter [in]  File name filter.

    \sa
    Font Handling
  */
  virtual void collectFilePathsInDirectory(OdStringArray& res, // out
    const OdString& sPath, // in
    const OdString& sFilter = L"*.*");

  /** \details
    Prompts the user for a filename, and returns their response.

    \param flags [in]  Dialog behavior.
    \param dialogCaption [in]  Dialog caption.
    \param defExt [in]  Default file extension without "."
    \param defFilename [in]  Default filename.
    \param filter [in]  Filter pattern.

    \remarks
    flags must be one of the following:

    <table>
    Name                  Value   Description
    kGfpForOpen           0       For Open.
    kGfpForSave           1       For Save.
    kGfpOverwritePrompt   2       Enable overwrite prompt displaying when exist file selected for saving.
    </table>

    This function is intended for the selection of filenames via dialog boxes,
    although other methods may be used.

    *  Returns "canceled" if the user cancels the request.
    *  Returns "unsupported" if the application does not support this user input.

    \remarks
    The default implementation of this function does nothing but return "unsupported".
  */
  virtual OdString fileDialog(
    int flags,
    const OdString& dialogCaption = OdString::kEmpty,
    const OdString& defExt = OdString::kEmpty,
    const OdString& defFilename = OdString::kEmpty,
    const OdString& filter = OdString::kEmpty);

  /** \details
    Returns a the gsBitmapDevice associated with this HostAppServices object.

    \param pViewObj [in]  OdAbstractViewPE compatible object (OdGsView, OdDbViewport or etc.).
    \param pDb [in]  Pointer to the database context.
    \param flags [in]  Bitmap device flags.
  */
  virtual OdGsDevicePtr gsBitmapDevice(
    OdRxObject* pViewObj = NULL,
    OdDbBaseDatabase* pDb = NULL,
    OdUInt32 flags = 0);

  /** \details
    Returns the path to the temporary files folder.
  */
  virtual OdString getTempPath() const;

// ODA_MT_DBIO_BEGIN
  /** \details
    MtMode controls if multi-threading is used (bit-coded)
  */
  virtual OdInt16 getMtMode() const;

  /** \details
    Returns the number of threads for required multi-threaded mode.
  */
  virtual int numThreads(OdDb::MultiThreadedMode mtMode);
// ODA_MT_DBIO_END

  /** \details
    Initialize brep builder with product specific implementation. Return eNotImplementedYet by default.

    \param brepBuilder [in]  instance of brep builder class.
    \param bbType [in] type of brep.

  */
  virtual OdResult brepBuilder(OdBrepBuilder& bbuilder, BrepType bbType);

  /** \details
  Get environment variable. If key varName doesn't exist, returns eKeyNotFound.

  */
  virtual OdResult getEnv(const OdString &varName, OdString &value);

  /** \details
  Set environment variable with new value.
  */
  virtual OdResult setEnv(const OdString &varName, const OdString &newValue);

  /** \details
  Returns user name in the requested format
  */
  virtual OdString getAppUserName(Oda::UserNameFormat unFormat = Oda::kUserId);
};

typedef OdSmartPtr<OdDbBaseHostAppServices> OdDbBaseHostAppServicesPtr;


#include "TD_PackPop.h"

#endif // _ODDB_BASEHOSTAPPSERVICES_INCLUDED_
