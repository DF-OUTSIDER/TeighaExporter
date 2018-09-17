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

#ifndef _ODDWFHOSTAPPSERVICES_H_
#define _ODDWFHOSTAPPSERVICES_H_

#include "TD_PackPush.h"

#include "DbBaseHostAppServices.h"
#include "DwfdbExport.h"

class DwfDbHostAppProgressMeter : public OdDbHostAppProgressMeter
{
public:
  DwfDbHostAppProgressMeter();

  // OdDbHostAppProgressMeter overridden
  virtual void start(const OdString& displayString = OdString::kEmpty);
  virtual void stop();
  virtual void meterProgress();
  virtual void setLimit(int max);

  //Controls display of this ProgressMeter.
  void disableOutput(bool disable); 
  void setPrefix(const OdString& prefix); 

protected:
  OdString m_Prefix;
  long m_MeterLimit, m_MeterCurrent, m_MeterOld;
  bool m_disableOutput;
};

// This class is the base class for platform specific operations within Teigha.
// Library: TDwfDb

class DWFDBEXPORT OdDwfHostAppServices : public OdDbBaseHostAppServices
{
public:
  ODRX_DECLARE_MEMBERS(OdDwfHostAppServices);

  OdDwfHostAppServices();
  virtual ~OdDwfHostAppServices();

  /** \details
    Returns the fully qualified path to the specified file.

    \param filename [in]  Name of the file to find.
    \param pDb [in]  Pointer to the database context.
    \param hint [in]  Hint that indicates the type of file that is required.

    \remarks
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
    4.  The directories listed in the "ACAD" environment variable.
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

    \note
    The client application can, in fact return any non-null string, so long as
    the application can handle that string in OdDbSystemServices::createFile.
  */
  virtual OdString findFile(
    const OdString& filename,
    OdDbBaseDatabase* pDb = 0,
    FindFileHint hint = kDefault);

  /** \details
    Returns an instance of an OdDbHostAppProgressMeter.
  */
  virtual OdDbHostAppProgressMeter* newProgressMeter();

  /** \details
    Notification function called whenever Teigha no longer needs the specified
    ProgressMeter object.

    \param pProgressMeter [in]  Pointer to the ProgressMeter object no longer needed by Teigha.

    \note
    The default implementation of this function does nothing but return.
  */
  virtual void releaseProgressMeter(
    OdDbHostAppProgressMeter* pProgressMeter);

  /** \details
    Returns the static OdRxClass description object associated with the database.

    \remarks
    Returns OdDwfDatabase::desc();
  */
  virtual OdRxClass* databaseClass() const;

  /** \details
    Creates an instance of an OdDwfDatabase object.

    \param createDefault [in]  If and only if true, the newly created database
            will be populated with the default set of objects (all tables, ModelSpace and PaperSpace blocks
            etc.)
    \param measurement [in] Units of measurement for creating the database.

    \remarks
    Returns the newly created database.

    This HostAppServices object will be associated with the newly created database.

    measurement must be one of the following:

    <table>
    Name              Value   Description
    OdDb::kEnglish    0       English
    OdDb::kMetric     1       Metric
    </table>

  */
  virtual OdSmartPtr<class OdDwfDatabase> createDatabase(bool createDefault = true) const;

  /** \details
    Loads the contents of the specified StreamBuf or DWG/DXF file into the
    database with which this HostAppServices object is associated.

    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \param partialLoad [in]  Controls the partial loading of .dwg files.
    \param password [in]  Password for file.
    \param allowCPConversion [in]  If and only if true, allows code page conversion.

    \remarks
    Returns a SmartPointer to the database containing the contents of the file.
  */
  //virtual OdSmartPtr<OdDwfDatabase> readFile(
  //  OdStreamBuf* pStreamBuf,
  //  bool allowCPConversion = false,
  //  bool partialLoad2d,
  //  const OdPassword& password = OdPassword());

  /** \details
    \param filename [in]  Path of the DWG/DXF file to read.
  */
  virtual OdSmartPtr<OdDwfDatabase> readFile(
    const OdString& filename,
    bool partialLoad = true,
    const OdString& password = OdString::kEmpty);

  /** \details
    Returns the name of the client program.

    \remarks
    This function is typically used for populating "About" information.

    \note
    The default implementation of this function returns oddbGetLibraryInfo()->getLibName()
  */
  virtual const OdString program();

  /** \details
    Returns the name of the client product.

    \remarks
    This function is typically used for populating "About" information.

    \note
    The default implementation of this function returns program().
  */
  virtual const OdString product();

  /** \details
    Returns the name of the client company.

    \remarks
    This function is typically used for populating "About" information.

    \note
    The default implementation of this function returns oddbGetLibraryInfo()->getCompanyName().
  */
  virtual const OdString companyName();

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

    \note
    The default implementation of this function returns kProd_OEM, and should not be overridden.
  */
  virtual ProdIdCode prodcode();

  /** \details
    Returns the release major and minor version string of the client application.

    \note
    The default implementation of this function returns oddbGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString releaseMajorMinorString();

  /** \details
    Returns the release major version of the client application.

    \note
    The default implementation of this function returns TD_MAJOR_VERSION.
  */
  virtual int releaseMajorVersion();

  /** \details
    Returns the release minor version of the client application.

    \note
    The default implementation of this function returns TD_MINOR_VERSION.
  */
  virtual int releaseMinorVersion();

  /** \details
    Returns the release version string of the client application.

    \note
    The default implementation of this function returns oddbGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString versionString();

  /** \details
    Displays the specified warning message.

    \param warnVisGroup [in]  Group of warning message visibility (always visible if empty).
    \param message [in]  Warning message.
    \param warningOb [in]  Warning object.
    \param objectId [in]  Object ID of the object associated with the warning.

    \note
    These functions call odSystemServices()->warning(message) to display the warning.
  */
  //TD_USING(OdDbBaseHostAppServices::warning);
  ////virtual void warning(
  ////  OdWarning warningOb,
  ////  OdDbObjectId objectId) { warning(NULL, warningOb, objectId); }
  ////virtual void warning(
  ////  const char* warnVisGroup,
  ////  OdWarning warningOb,
  ////  OdDbObjectId objectId);
  //virtual void warning(
  //  const OdError& err)  { warning(NULL, err.description()); }
  //virtual void warning(
  //  const char* warnVisGroup,
  //  const OdError& err)  { warning(warnVisGroup, err.description()); }

  /** \details
    Returns the error description associated with the specified error code.
    \param errorCode [in]  Error code.

    \note
    The default implementation of this function returns formatMessage(errorCode).
  */
  //virtual OdString getErrorDescription(
  //  unsigned int errorCode);

  /** \details
    Returns a formatted message corresponding to the specified error code(s).
    \param errorCode [in]  Error code.
  */
//  virtual OdString formatMessage(
//    unsigned int errorCode,...);
//#ifdef ODA_FORMAT
//  virtual OdString formatMessage(unsigned int errorCode, long i, const OdString& str) { return formatMessage(errorCode, i, (void*)str.c_str()); }
//#endif


  /** \details
    Returns true if and only if Teigha is to do full-time CRC checking on database filing operations.

    \remarks
    The default implementation of this function always returns false.
    Override this function to return true or false as desired to control this checking.
  */
  //virtual bool doFullCRCCheck();

  /** \details
    Returns an alternate UndoController object for Teigha.

    \remarks
    The default implementation of this function always returns a null SmartPointer.

    Each database requires at most 2 undo controllers (for undo and redo).
  */
  //virtual OdDbUndoControllerPtr newUndoController();

  /** \details
    Returns the PlotSettingsValidator object associated with HostAppServices object.

    \remarks
    If no PlotSettingsValidator object is associated with this HostAppServices object, one is created and
    associated with it.
  */
  //virtual OdDbPlotSettingsValidator* plotSettingsValidator();

  /** \details
    Locates the TTF or TTC file containing the specified font description.

    \param description [in]  Font description.
    \param filename [out]  Receives the name of the TrueType font file.

    \remarks
    Returns true and the filename if and only if the font file was found.

    \note
    The non-Windows builds of this function do nothing but return.
  */
  //virtual bool ttfFileNameByDescriptor(
  //  const OdTtfDescriptor& description,
  //  OdString& filename);

  virtual OdString getAlternateFontName() const;

  /** \details
    Returns the name of the font mapping file used by the getPreferableFont function.

    \sa
    Font Handling

    \note
    The default implementation of this function does nothing but return an empty string.
    It will be fully implemented in a future release.
  */
  virtual OdString getFontMapFileName() const;

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
    OdFontType fontType);

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
    OdFontType fontType);

  /** \details
    Returns the typeface name of the TTF font which has the character.

    \param pFont [in]        Current font.
    \param unicodeChar [in]  Current symbol.
    \param pDb [in]          Pointer to the database context.

    \sa
    Font Handling

    \note
    The default implementation uses Windows API for getting the font.
  */
  //virtual OdString getSubstituteFontByChar(
  //  const OdFont& pFont,
  //  OdChar unicodeChar, OdDwfDatabase *pDb);

  /** \details
    Collects folders with TrueType fonts.

    \param aDirs [out]   Array names of folders.

    \sa
    Font Handling
  */
  //virtual bool getSystemFontFolders(
  //   OdStringArray& aDirs);

  /** \details
    Collects TrueType font paths.

    \param res [out]      Array of full file names of TrueType fonts.
    \param sPath [in]    Array of folders paths with TrueType files.
    \param sFilter [in]  File name filter.

    \sa
    Font Handling
  */
  //virtual void collectFilePathsInDirectory(OdStringArray& res, // out
  //  const OdString& sPath, // in
  //  const OdString& sFilter = L"*.*");

  //Returns the PatternManager associated with this HostAppServices object.
  //virtual OdHatchPatternManager* patternManager() = 0;

  /** \details
    Notification function called whenever a drawing file requires a *password.*

    \param dwgName [in]  Drawing file requiring the password.
    \param isXref [in]  True if and only if the drawing file is being opened as an Xref.
    \param password [out]  Password.

    \remarks
    Prompts the user for a password and returns said password.

    Returns false if the user cancels the password entry, or
    if there is no user.

    \note
    The default implementation of this function does nothing but return false.
  */
  //virtual bool getPassword(
  //  const OdString& dwgName,
  //  bool isXref,
  //  OdPassword& password);

  /* { Secret } */
  //virtual OdPwdCachePtr getPasswordCache();

  //virtual OdDbPageControllerPtr newPageController();

  /** \details
  Callback method to use with paging functionality.

  \remarks
  Return eInvalidInput in case if pDb is null.

  \note
  The default implementation of this function calls odDbPageObjects.
  */
  //virtual OdResult pageObjects(OdDwfDatabase* pDb);

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

    * Returns "canceled" if the user cancels the request.
    * Returns "unsupported" if the application does not support this user input.

    \note
    The default implementation of this function does nothing but return "unsupported".
  */
  //virtual OdString fileDialog(
  //  int flags,
  //  const OdString& dialogCaption = OdString::kEmpty,
  //  const OdString& defExt = OdString::kEmpty,
  //  const OdString& defFilename = OdString::kEmpty,
  //  const OdString& filter = OdString::kEmpty);

  /** \details
    Returns a the gsBitmapDevice associated with this HostAppServices object.

    \param pViewObj [in]  OdAbstractViewPE compatible object (OdGsView, OdDbViewport or etc.).
    \param pDb [in]  Pointer to the database context.
    \param flags [in]  Bitmap device flags.
  */
  virtual OdGsDevicePtr gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                       OdDbBaseDatabase* pDb = NULL,
                                       OdUInt32 flags = 0);

  //virtual OdDbLayoutManagerPtr layoutManager() const;

  /** \details
  Returns non-zero if the application copy is educational.
  */
  //virtual OdUInt32 educationalPlotStamp() const;

  //  Returns the OdDgHostAppServices object used for reading dgn files.
  //virtual OdDbBaseHostAppServices* getDgnHostAppServices() const;

  /** \details
    Returns thumbnail preview size. Default implementation returns sizes based on THUMBSIZE Registry variable.
  */
  //virtual void getThumbSize(unsigned int& nWidth, unsigned int& nHeight);

  /** \details
    Returns the path to the temporary files folder.
  */
  //virtual OdString getTempPath() const;

  void disableProgressMeterOutput(bool disable);

protected:
  //OdMutex                       m_LayoutManagerMutex;
  //OdMutex                       m_ValidatorMutex;
  //OdMutex                       m_PwdCacheMutex;
  //OdDbLayoutManagerPtr          m_pLayoutManager;
  //OdDbPlotSettingsValidatorPtr  m_pValidator;
  //OdPwdCachePtr                 m_pPwdCache;
  DwfDbHostAppProgressMeter m_progressMeter;
};

typedef OdSmartPtr<OdDwfHostAppServices> OdDwfHostAppServicesPtr;

#include "TD_PackPop.h"

#endif // _ODDWFHOSTAPPSERVICES_H_
