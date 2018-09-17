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



#ifndef _ODDGHOSTAPPLICATIONSERVICES_INCLUDED_
#define _ODDGHOSTAPPLICATIONSERVICES_INCLUDED_


#include "TD_PackPush.h"

#include "DgExport.h"
#include "DbBaseHostAppServices.h"
#include "StringArray.h"
#include "OdBinaryData.h"

class OdDgDatabase;
typedef OdSmartPtr<OdDgDatabase> OdDgDatabasePtr;


// NOTE: ODA_DG is an environment variable. It could be set as user variable among system properties
//        to specify a path or list of paths that could contain symbology resource files.

/** \details
    This class is the base class for platform specific operations within Teigha™ for .dgn files.

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgHostAppServices : public OdDbBaseHostAppServices
{
public:
  ODRX_DECLARE_MEMBERS(OdDgHostAppServices);

  OdDgHostAppServices();

  enum DatabaseDefaultContent
  {
    kDbNotInitialized   = 0,
    kDbDefaultModelIs3d = 1,
    kDbDefaultModelIs2d = 2
  };

  /** \details
    Returns the static OdRxClass description object associated with the database.

    \remarks
    Returns OdDgDatabase::desc();
  */
  virtual OdRxClass* databaseClass() const;

  /** \details
    Creates an instance of an OdDgDatabase object.

    \param defaultContent [in]  If it is not equal to kNotInitialized, the newly created database
            will be populated with the default set of objects.

    \returns
    Returns the newly created database.
    \remarks
    This HostAppServices object will be associated with the newly created database.

  */
  virtual OdDgDatabasePtr createDatabase(
    DatabaseDefaultContent defaultContent = kDbDefaultModelIs3d) const;

  /** \details
    Loads the contents of the specified DGN file file into the
    database with which this HostAppServices object is associated.

    \param fileName [in]  Name of the file from which the data are to be read.
    \param shareMode [in]  Share mode to use when opening the specified file.
    \param partialLoad [in]  Controls the partial loading of DGN files.
    \param pAuditInfo [in]  Pointer to an OdDgAuditInfo object.
    \param password [in]  Password for file.

    \returns
    Returns a SmartPointer to the database containing the contents of the file.
  */
  virtual OdDgDatabasePtr readFile(
    const OdString& fileName,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite);
    //bool partialLoad = false,
    //OdDgAuditInfo* pAuditInfo = 0,
    //const OdString& password = OdString()

  /** \details
    Returns the fully qualified path to the specified file.

    \param filename [in]  Name of the file to find.
    \param pDb [in]  Pointer to the database context.
    \param hint [in]  Hint that indicates the type of file that is required.

    \remarks
    Returns an empty string if the file is not found.

    This function is called by Teigha™ for .dgn files when access is needed to a
    file, such as a font file, a template file, etc.

    pDb == 0 specifies that this file search is not related to a database.

    If pDb != 0, call pDb->getFilename() to determine the path to the DGN
    file associated with the database.

    The typical search order is as follows:

    1.  The filename itself.
    2.  The current directory.
    3.  The drawing directory (for shx font, image, and xref files).
    4.  The Windows fonts directory (only for TTF fonts on Windows).

    hint must be one of the following:

    <table>
    Name                    Value     Description Extension
    kDefault                0         Any file                           Any
    kFontFile               1         Can be either SHX or TTF file      SHX or TTF
    kCompiledShapeFile      2         SHX file                           SHX
    kTrueTypeFontFile       3         TTF file                           TTF
    kEmbeddedImageFile      4         Image file                         ISM
    kXRefDrawing            5         External reference file
    kPatternFile            6         Pattern file                       PAT
    kTXApplication          7         Teigha Xtension Application file   TX
    kFontMapFile            8         FontMap file                       FMP
    </table>
  */
  virtual OdString findFile(
    const OdString& filename,
    OdDbBaseDatabase* pDb = 0,
    FindFileHint hint = kDefault);
/** \details
    Loads the thumbnail of the specified DGN file into binary data structure.

    \param fileName [in]  Name of the file from which the data are to be read.
    \param dibData [out] Data of thumbnail.

    \returns
    Returns true if thumbnail data was found.
*/

  virtual bool getThumbnail( const OdString& filename, OdBinaryData& dibData );

  /** \details
    Returns the name of the client program.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns oddgGetLibraryInfo()->getLibName()
  */
  virtual const OdString program();

  /** \details
    Returns the name of the client product.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns program().
  */
  virtual const OdString product();

  /** \details
    Returns the name of the client company.

    \remarks
    This function is typically used for populating "About" information.

    \remarks
    The default implementation of this function returns oddgGetLibraryInfo()->getCompanyName().
  */
  virtual const OdString companyName();

  /** \details
    Returns the release major and minor version string of the client application.

    \remarks
    The default implementation of this function returns oddgGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString releaseMajorMinorString();

  /** \details
    Returns the release version string of the client application.

    \remarks
    The default implementation of this function returns oddgGetLibraryInfo()->getLibVersion().
  */
  virtual const OdString versionString();

  /** \details
    Returns a formatted message corresponding to the specified error code(s).
    \param errorCode [in]  Error code.
  */
  virtual OdString formatMessage(
    unsigned int errorCode,...);

  /** \details
    Returns the font file to be used when a given font file is
    not found by Teigha™ for .dgn files.

    \sa
    Font Handling

    \remarks
    The default implementation of this function returns an empty string.
  */
  virtual OdString getAlternateFontName() const;

  /** \details
    Returns the name of the font mapping file used by the getPreferableFont function.

    \sa
    Font Handling

    \remarks
    The default implementation of this function returns an empty string.
  */
  virtual OdString getFontMapFileName() const;

  /** \details
    Returns the preferable font name for the specified font name and type.

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

    \sa
    Font Handling

    \remarks
    The default implementation of this function returns an empty string.
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

    \sa
    Font Handling

    \remarks
    The default implementation of this function returns an empty string.
  */
  virtual OdString getSubstituteFont(
    const OdString& fontName,
    OdFontType fontType);

  /** \details
    Returns a the bitmap device associated with this HostAppServices object.

    \param pViewObj [in]  OdAbstractViewPE compatible object (OdGsView, OdDbViewport or etc.).
    \param pDb [in]  Pointer to the database context.
    \param flags [in]  Bitmap device flags.
  */
  virtual OdGsDevicePtr gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                       OdDbBaseDatabase* pDb = NULL,
                                       OdUInt32 flags = 0);

  /** \details
    Returns the OdDbHostAppServices object used for reading .dwg files.
  */
  virtual OdDbBaseHostAppServices* getDbHostAppServices() const;

  /** \details
    Returns an alternate UndoController object for Teigha.

    \remarks
    The default implementation of this function always returns a null SmartPointer.

    Each database requires at most 2 undo controllers (for undo and redo).
  */
  virtual OdDbUndoControllerPtr newUndoController();

  /** \details
  Returns an alternate stream object for undo\redo operations.

  \remarks
  The default implementation of this function always returns a null SmartPointer.
  It may return null in the user application. In this case the default memory based stream will be used.
  In fact this stream will be used for the current undo\redo step. Other steps can be controlled by
  OdDbUndoController.
  */
  virtual OdStreamBufPtr newUndoStream();

  virtual OdResult brepBuilder(OdBrepBuilder& brepBuilder, BrepType bbType);

  /**DOM-IGNORE-BEGIN*/
  #define REGVAR_DEF(type, name, unused3, unused4, unused5)               \
  protected:                                                              \
    type  m_##name;                                                       \
  public:                                                                 \
    /** Description: Returns the ##NAME registry-resident system variable.\
      Remarks: ##remarks                                                  \
      Notes: ##notes                                                      \
      See Also: ##see_also                                                \
    */                                                                    \
    virtual type get##name() const;                                       \
    /** Description: Sets the ##NAME registry-resident system variable.   \
      Arguments:  val (I) New value for ##NAME. ##RANGE                   \
      Remarks: ##remarks                                                  \
      Notes: ##notes                                                      \
      See Also: ##see_also                                                \
    */                                                                    \
    virtual void set##name(type val);

  #define REGVAR_DEF_ARRAY(type, name, unused3, unused4, unused5)         \
  protected:                                                              \
    type  m_##name;                                                       \
  public:                                                                 \
    /** Description: Returns the ##NAME registry-resident system variable.\
      Remarks: ##remarks                                                  \
      Notes: ##notes                                                      \
      See Also: ##see_also                                                \
    */                                                                    \
    virtual void get##name(type& val) const;                              \
    /** Description: Sets the ##NAME registry-resident system variable.   \
      Arguments:  val (I) New value for ##NAME. ##RANGE                   \
      Remarks: ##remarks                                                  \
      Notes: ##notes                                                      \
      See Also: ##see_also                                                \
    */                                                                    \
    virtual void set##name(type& val);

  #include "DgRegVarDefs.h"

  #undef REGVAR_DEF
  #undef REGVAR_DEF_ARRAY
/**DOM-IGNORE-END*/
  
  void loadRscFont( const OdString& strRscFontFileName );
  void loadRscLineStyle( const OdString& strRscLineStyleFileName );
  void reloadRscFonts();
  void reloadRscLineStyles();
  void cleanupRscFontMap();
  void cleanupLineStyleDefRscMap();

};


class OdDbLibraryInfo;

/** \details
  Returns the properties of the library.

  \remarks
  This function is typically used for populating "About" information.
*/
TG_EXPORT OdDbLibraryInfo* oddgGetLibraryInfo();


#include "TD_PackPop.h"

#endif /* _ODDGHOSTAPPLICATIONSERVICES_INCLUDED_ */



