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


#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
#if _MSC_VER <= 1200
// compiler stack overflows if these warning is enabled
#pragma warning ( disable : 4786)
#endif
#endif

// DWF headers should come first, otherwise gcc gives strange compile errors
#include "package/Section.h"
#include "package/EPlotSection.h"
#include "package/Manifest.h"
#include "package/GraphicResource.h"
#include "package/reader/PackageReader.h"
#include "dwf/package/EModelSection.h"

#include "StdAfx.h"
#include "OdaCommon.h"
#include "../Include/DwfImport.h"
#include "DwfImportImpl.h"
#include "DwfCallbacks.h"
#include "RxDispatchImpl.h"
#include "RxVariantValue.h"
#include "DbDictionary.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"
#include "DwfEventReactor.h"
#include "ProgressMonitor.h"
#include "OdRound.h"

#include "dwf/dwfx/Constants.h"
#include "dwf/XAML/XamlFile.h"
#include "dwf/XAML/XamlObjectFactory.h"
#include "dwfcore/MIME.h"
#include "EmbeddedImageDef.h"
#include "DbRasterImage.h"
#if defined(_MSC_VER)
#include "dwf/publisher/win32/EmbeddedFontImpl.h"
#endif

using namespace DWFCore;
using namespace DWFToolkit;

namespace TD_DWF_IMPORT {
#define DATA_BUFFER_BYTES   4096
// enables w3d dumping
// #define _DEBUG_DUMP


class DwfProperties : public OdRxDispatchImpl<>
{
  OdString _password;
  OdString _path;
  double _paperWidth, _paperHeight;
  OdDbDatabasePtr _db;
  OdUInt32 _background;
  bool _preserveColorIndices;
  OdInt32 _layoutNumber;
  bool _importW3d;
  OdStreamBufPtr _stream;
  bool _useStableImageNames;
  bool _processGradients;
  bool _modelToLayout;
  OdIntPtr _palette;
public:
  DwfProperties() : _paperWidth( 297 ), _paperHeight( 210 ),_background(0xffffffff), 
    _preserveColorIndices(true), 
    _layoutNumber(-1), 
    _importW3d(true), 
    _useStableImageNames(true), 
    _processGradients(true), 
    _palette(0),
    _modelToLayout(false)
  {}
  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP( DwfProperties );
  static OdRxDictionaryPtr createObject();

  OdUInt32 get_Background() const { return _background; }
  void put_Background( OdUInt32 background ){ _background = background; }
  OdString get_Password() const { return _password; }
  void put_Password( OdString pass ){ _password = pass; }
  double get_PaperWidth() const { return _paperWidth; }
  void put_PaperWidth( double w ){ _paperWidth = w; }
  double get_PaperHeight() const { return _paperHeight; }
  void put_PaperHeight( double h ){ _paperHeight = h; }
  OdRxObjectPtr get_Database() const { return _db.get(); }
  void put_Database( OdRxObject* obj ){ _db = obj; }
  OdRxObjectPtr get_Stream() const {return _stream.get();}
  void put_Stream(OdRxObject* obj ){_stream = obj;}
  OdString get_DwfPath() const { return _path; }
  void put_DwfPath( OdString path ){ _path = path; }
  bool get_PreserveColorIndices() const { return _preserveColorIndices; }
  void put_PreserveColorIndices(bool b){  _preserveColorIndices = b; }
  OdInt32 get_LayoutNumber()const{return _layoutNumber;}
  void put_LayoutNumber(OdInt32 n){ _layoutNumber = n; }
  bool get_ImportW3d() const { return _importW3d; }
  void put_ImportW3d(bool b){  _importW3d = b; }
  bool get_UseStableImageNames()const{ return _useStableImageNames; }
  void put_UseStableImageNames(bool b){ _useStableImageNames = b; }
  bool get_ProcessGradients()const{ return _processGradients; }
  void put_ProcessGradients(bool b){ _processGradients = b; }
  OdIntPtr get_DwgPalette(){ return _palette; }
  void put_DwgPalette(OdIntPtr p){ _palette = p; }
  bool get_ModelToLayout()const { return _modelToLayout; }
  void put_ModelToLayout(bool b) { _modelToLayout = b; }
};

typedef OdSmartPtr<DwfProperties> DwfPropertiesPtr;

OdRxDictionaryPtr DwfProperties::createObject()
{
  return OdRxObjectImpl<DwfProperties, OdRxDictionary>::createObject();
}


ODRX_DECLARE_PROPERTY(DwfPath)
ODRX_DECLARE_PROPERTY(Password)
ODRX_DECLARE_PROPERTY(PaperWidth)
ODRX_DECLARE_PROPERTY(PaperHeight)
ODRX_DECLARE_PROPERTY(Database)
ODRX_DECLARE_PROPERTY(Background)
ODRX_DECLARE_PROPERTY(PreserveColorIndices)
ODRX_DECLARE_PROPERTY(LayoutNumber)
ODRX_DECLARE_PROPERTY(ImportW3d)
ODRX_DECLARE_PROPERTY(Stream)
ODRX_DECLARE_PROPERTY(UseStableImageNames)
ODRX_DECLARE_PROPERTY(ProcessGradients)
ODRX_DECLARE_PROPERTY(DwgPalette)
ODRX_DECLARE_PROPERTY(ModelToLayout)


ODRX_DEFINE_PROPERTY(DwfPath, DwfProperties, getString)
ODRX_DEFINE_PROPERTY(Password, DwfProperties, getString)
ODRX_DEFINE_PROPERTY(PaperWidth, DwfProperties, getDouble)
ODRX_DEFINE_PROPERTY(PaperHeight, DwfProperties, getDouble)
ODRX_DEFINE_PROPERTY(Background, DwfProperties, getInt32)
ODRX_DEFINE_PROPERTY(PreserveColorIndices, DwfProperties, getBool)
ODRX_DEFINE_PROPERTY(LayoutNumber, DwfProperties, getInt32)
ODRX_DEFINE_PROPERTY(ImportW3d, DwfProperties, getBool)
ODRX_DEFINE_PROPERTY_OBJECT(Database, DwfProperties,  get_Database, put_Database, OdDbDatabase)
ODRX_DEFINE_PROPERTY_OBJECT(Stream, DwfProperties,  get_Stream, put_Stream, OdStreamBuf)
ODRX_DEFINE_PROPERTY(UseStableImageNames, DwfProperties, getBool)
ODRX_DEFINE_PROPERTY(ProcessGradients, DwfProperties, getBool)
ODRX_DEFINE_PROPERTY(DwgPalette, DwfProperties, getIntPtr)
ODRX_DEFINE_PROPERTY(ModelToLayout, DwfProperties, getBool)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP( DwfProperties );
  ODRX_GENERATE_PROPERTY( DwfPath )
  ODRX_GENERATE_PROPERTY( Password )
  ODRX_GENERATE_PROPERTY( PaperWidth )
  ODRX_GENERATE_PROPERTY( PaperHeight )
  ODRX_GENERATE_PROPERTY( Database )
  ODRX_GENERATE_PROPERTY( Background )
  ODRX_GENERATE_PROPERTY( PreserveColorIndices )
  ODRX_GENERATE_PROPERTY( LayoutNumber )
  ODRX_GENERATE_PROPERTY( ImportW3d )
  ODRX_GENERATE_PROPERTY( Stream )
  ODRX_GENERATE_PROPERTY( UseStableImageNames )
  ODRX_GENERATE_PROPERTY( ProcessGradients )
  ODRX_GENERATE_PROPERTY( DwgPalette )
  ODRX_GENERATE_PROPERTY( ModelToLayout )
ODRX_END_DYNAMIC_PROPERTY_MAP(DwfProperties);

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

DwfImporter::DwfImporter() : 
  _properties( DwfProperties::createObject() ),
  _xps(false),
  _extent(this),
  _blocks(this),
  _fonts(this),
  _images(this),
  _lines(this),
  _ignoreMetadata(true),
  _currentSegment(0),
  _idxNextMaterial(0)
{
  cleanupW2D();
}

OdRxDictionaryPtr DwfImporter::properties() { return _properties; }

namespace dwfImp
{
  // version of DWF (6.00 = 600, 5.5 = 0.55 = 55, etc)
  //
  DWFPackageReader::tePackageType DWFVersion(OdStreamBuf* file)
  {
    unsigned char buf[12];
    file->getBytes(buf, 12);
    file->seek(-12, OdDb::kSeekFromCurrent);
    DWFPackageReader::tPackageInfo rInfo;
    DWFPackageReader::GetPackageInfo( buf, rInfo );
    return rInfo.eType;
  }
}

class DWFInputStreamWrapper : public DWFInputStream
{
  OdStreamBuf* m_pStream;
  DWFImportProgressMeter* m_pProgress;
public:
  DWFInputStreamWrapper(OdStreamBuf* s) : m_pStream(s), m_pProgress(0){}
  void setProgressMeter(DWFImportProgressMeter* pm){ m_pProgress = pm; }
  virtual size_t available() const throw( DWFException )
  {
    return size_t(m_pStream->length() - m_pStream->tell());
  }
  virtual size_t read( void*  pBuffer, size_t nBytesToRead ) throw( DWFException )
  {
    if (nBytesToRead > (m_pStream->length() - m_pStream->tell()))
    {
      nBytesToRead = size_t(m_pStream->length() - m_pStream->tell());
    }
    if (m_pProgress)
    {
      int cur = OdRoundToLong((100.0 * (OdInt64(m_pStream->tell())+nBytesToRead)) / OdInt64(m_pStream->length()));
      while (m_pProgress->current() < cur)
        m_pProgress->step();
    }
    ODA_ASSERT(nBytesToRead < INT_MAX);
    m_pStream->getBytes(pBuffer, (OdUInt32)nBytesToRead);
    return nBytesToRead;
  }
  virtual off_t seek( int eOrigin, off_t nOffset ) throw( DWFException )
  {
    switch(eOrigin)
    {
    case SEEK_CUR:
      return (off_t)m_pStream->seek(nOffset, OdDb::kSeekFromCurrent);
    case SEEK_SET:
      return (off_t)m_pStream->seek(nOffset, OdDb::kSeekFromStart);
    case SEEK_END:
      return (off_t)m_pStream->seek(nOffset, OdDb::kSeekFromEnd);
    }
    return 0;
  }
};

OdDwfImport::ImportResult DwfImporter::import()
{
  _db = _properties->get_Database();
  if ( _db.isNull() ) return OdDwfImport::bad_database;

  _db->closeInput();  // some problems (with viewport) in case that db was open partially  // MKU 10.06.2004
  _db->setSysVar(L"IMAGEFRAME", OdResBuf::newRb(OdResBuf::kRtInt16, (OdInt16)0));

  _ignoreMetadata = true;
  _blocks.m_bPreserveColorIndices = _properties->get_PreserveColorIndices();
  if (ODCOLORREF* pPal = (ODCOLORREF*)_properties->get_DwgPalette())
  {
    OdInt16 index = 1;
    for (ODCOLORREF* pc = pPal + 1; pc < pPal + 256; ++pc)
      _blocks.m_pDwgPalette[*pc] = index++;
  }
  OdStreamBufPtr stream = _properties->get_Stream();
  if (stream.isNull())
   stream = odrxSystemServices()->createFile(_properties->get_DwfPath());
  if (stream.isNull())
    return OdDwfImport::bad_file;
  
  switch ( dwfImp::DWFVersion( stream ) )
	{
	case DWFPackageReader::eDWFPackage:           // DWF 6 (and higher) zip package format.
	case DWFPackageReader::eDWFPackageEncrypted:  // Password protected DWF 6 (and higher) zip package format.
    {
      DWFInputStreamWrapper w(stream);
		  return loadPackage( w, _properties->get_Password());
    }
	case DWFPackageReader::eDWFStream:            // Legacy DWF 0.55 (or lower) data and graphics stream.
	case DWFPackageReader::eW2DStream:            // 2D graphics stream/file resource.
		{
			// single sheet is imported into the active layout
      _blocks.setCurrentBlock(
        _db->getTILEMODE() ? _db->getModelSpaceId() : _db->getPaperSpaceId() );
      _blocks.setCurrentLayout( _blocks.currentBlock()->getLayoutId() );
      DWFInputStreamWrapper w(stream);
      _extent._collectBounds = true;
      OdDwfImport::ImportResult res = OdDwfImport::success;
      {
        WT_File wtFile;
        res = loadStream(&w, wtFile);
      }
      if (res == OdDwfImport::success)
      {
        stream->rewind();
        DWFImportProgressMeter pm(_db);
        w.setProgressMeter(&pm);
        _extent.setPlotSettings(_properties->get_PaperWidth(), _properties->get_PaperHeight(), OdDbPlotSettings::kMillimeters);
        _extent.calculateScale();
        _ignoreMetadata = false;
        _extent._collectBounds = false;
        WT_File wtFile;
        res = loadStream(&w, wtFile);
      }
      cleanupW2D();
      return res;
		}
		break;
  case DWFPackageReader::eZIPFile:              // Try to interpret that as DWFx
  case DWFPackageReader::eDWFXPackage:
  case DWFPackageReader::eDWFXPackageEncrypted:
    {
      DWFInputStreamWrapper w(stream);
      _xps = true;
      return loadPackage( w, _properties->get_Password());
    }
    break;
  case DWFPackageReader::eUnknown:              // Unknown format.
		break;
	}
  return OdDwfImport::bad_file;
}



static DWFPackageReader *createReader( DWFInputStream& stream, const OdChar* password,
                                    OdDwfImport::ImportResult& result )
{
  try
  {
    // Allocate a new reader for this DWF file
    //
    DWFString zPassword( (const wchar_t*)password );
    DWFPackageReader* pReader = DWFCORE_ALLOC_OBJECT( DWFPackageReader(stream, zPassword) );
    if (pReader == NULL)
      result = OdDwfImport::fail;
    return pReader;
  }
  catch (DWFException& ex)
  {
    DWFString zType( ex.type() );
    //if (DWFCORE_COMPARE_WIDE_STRINGS(ex.type(), DWFIOException) == 0)
    if (zType == L"DWFIOException")
    {
      result = OdDwfImport::bad_file;
    }
    else if (zType == L"DWFInvalidPasswordException")
    {
      result = OdDwfImport::bad_password;
    }
    else
    {
      result = OdDwfImport::fail;
    }
  }
  return 0;
}

static OdString getLayoutName( OdArray<OdString>& usedNames, DWFSection* pSection )
{
  const DWFProperty* pProperty = 0;
  if ((pProperty = pSection->findProperty(L"Layout Name", L"")) != 0 ||
    (pProperty = pSection->findProperty(L"Layout Name", L"AutoCAD Drawing")) != 0)
  {
    OdString s((const wchar_t *)pProperty->value());
    if (s.isEmpty())
    {
        s = (const wchar_t *)pSection->title();
    }
    // try found name first
    if (usedNames.contains(s, 0))
      s = OdString((const wchar_t *)pSection->name());
    // try section name
    if (usedNames.contains(s, 0))
      s = OdString::kEmpty;
    return s;
  }
  return OdString::kEmpty;
}

static void setupLayout(OdArray<OdString>& usedNames, OdString& name, OdDbDatabase* db, DwfBlockManager& blocks,
                        bool bModelToLayout = false)
{
  OdDbDictionaryPtr pLayoutDict = db->getLayoutDictionaryId().safeOpenObject(OdDb::kForWrite);
  if (bModelToLayout && name == L"Model") // CORE-10319
  {
    static int index = 1;
    name = dwfImp::generateName(L"Model", pLayoutDict, index);
  }
  if ( !name.isEmpty() && pLayoutDict->has( name ) && !usedNames.contains(name))
  {
    OdDbLayoutPtr layout = pLayoutDict->getAt( name ).safeOpenObject();
    blocks.setCurrentLayout( layout->objectId() );
    blocks.setCurrentBlock( layout->getBlockTableRecordId() );
  }
  else
  {
    OdDbObjectId rec;
    if ( name.isEmpty() || usedNames.contains(name) ) 
    {
      static int index = 1;
      name = dwfImp::generateName( OD_T("Layout"), pLayoutDict, index );
    }
    blocks.setCurrentLayout( db->createLayout( name, &rec ) );
    blocks.setCurrentBlock( rec );
  }
  usedNames.append( name );
}

class Access : public WT_OpcResourceMaterializer
             , public WT_OpcResourceSerializer
{
private:
  static const size_t _DEF_BUFFER_SIZE;
  DWFToolkit::DWFResourceContainer&   _rResources;
  DWFToolkit::DWFResource&            _rFixedPage;
public:
  Access( DWFToolkit::DWFResource& rRes, DWFToolkit::DWFResourceContainer& rCont) : _rResources( rCont ) ,_rFixedPage( rRes ){}
  virtual ~Access(){}

  WT_Result getPartInputStream(const DWFCore::DWFString& rzPath, DWFCore::DWFInputStream** ppStream)
  {
    if ( ppStream == NULL )
      return WT_Result::Toolkit_Usage_Error;
    if ( (rzPath.substring(0, 2) != /* NOXLATE */ L"./") &&
      (rzPath.findFirst(/* NOXLATE */L'/') != -1) )
    {
      // Can't handle non-relative or non-local paths.
      // i.e.) path must be relative and local
      // Is this the right error?
      return WT_Result::Unsupported_DWF_Extension_Error;
    }

    DWFCore::DWFString zSearch( (rzPath.substring(0, 2) == /* NOXLATE */ L"./") ? 
      rzPath.substring(2) : rzPath ); 
    DWFToolkit::DWFResource *pRes = _rResources.findResourceByHREF( zSearch );
    WT_Result res = WT_Result::File_Inconsistency_Warning;
    if (!pRes )
    {
      //
      // Try again.
      //
      DWFCore::DWFPointer< DWFToolkit::DWFResourceContainer::ResourceKVIterator >
        pIt( _rResources.getResourcesByHREF(), false );
      if ( !pIt.isNull() && pIt->valid() )
      {
        //
        // Use the key from the iterator to infer the full-path href.
        //
        DWFCore::DWFString zKey( pIt->key() );
        off_t pos( zKey.findLast( /*NOXLATE*/ L'/' ) );
        if ( pos >= 0 )
        {
          DWFCore::DWFString zFullPath( zKey.substring( 0, pos + 1 ) );
          //
          // Append the local part taking in to consideration that it might
          // start with the "current directory" specifier.
          //
          zFullPath.append( zSearch );
          pRes = _rResources.findResourceByHREF( zFullPath );
        }
      }
    }	

    if ( pRes )
    {
      try
      {
        *ppStream = pRes->getInputStream();
        res = WT_Result::Success;
      }
      catch ( DWFException& )
      {
        res = WT_Result::Internal_Error;
      }
    }

    return res;
  }

  WT_Result getPartOutputStream(const DWFCore::DWFString&, DWFCore::DWFString&, DWFCore::DWFOutputStream**)
  {
    return WT_Result::Success;
  }
private:
  Access& operator =(Access&){return *this;};
};

OdDwfImport::ImportResult DwfImporter::load2dResourceXps( DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName, double dPaperWidth, double dPaperHeight, OdDbPlotSettings::PlotPaperUnits units, DWFImportProgressMeter pm, int layoutCount, bool bForExtents)
{
  _ignoreMetadata = false;
  _extent._collectBounds = bForExtents;
  OdDwfImport::ImportResult res = OdDwfImport::success;
  if (DWFToolkit::DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole( pRoleName ))
  {
    for (; piResources->valid(); piResources->next())
    {
      if (DWFGraphicResource* pW2D = static_cast<DWFGraphicResource*>(piResources->get()))
      {
        WT_XAML_Class_Factory XAMLFactory;
        WT_File* pFile = XAMLFactory.Create_File();
        WT_XAML_File* pXAMLFile = static_cast<WT_XAML_File*>(pFile);

        DWFToolkit::DWFResource* pExtRes = NULL;
        DWFCore::DWFPointer< DWFToolkit::DWFResourceRelationship::tList::ConstIterator > pRelIt( (*pW2D).getRelationships(), false );
        for ( ; !pRelIt.isNull() && pRelIt->valid(); pRelIt->next() )
        {
          if ( pRelIt->get()->type() == DWFToolkit::DWFXXML::kzRelationship_Graphics2dExtensionResource )
          {
            pExtRes = pSection->findResourceByObjectID( pRelIt->get()->resourceID() );
            break;
          }
        }

        if ( pExtRes )
        {
          pXAMLFile->w2xStreamIn() = pExtRes->getInputStream();
        }

        WT_Matrix2D tranf;
        tranf(0,0) = pW2D->transform()[0*4+0];
        tranf(0,1) = pW2D->transform()[0*4+1];
        tranf(1,0) = pW2D->transform()[1*4+0];
        tranf(1,1) = pW2D->transform()[1*4+1];
        // skip z coordinate
        // copy translation
        tranf(2,0) = pW2D->transform()[3*4+0];
        tranf(2,1) = pW2D->transform()[3*4+1];
        pXAMLFile->definePageLayout(tranf, dPaperWidth, dPaperHeight, 1);

        DWFInputStream* pTempInputStream = pW2D->getInputStream();
        DWFBufferInputStream* pInputStream = DWFCORE_ALLOC_OBJECT( DWFCore::DWFBufferInputStream( pTempInputStream, false ) );
        pXAMLFile->xamlStreamIn() = pInputStream;
        Access *pAccess = DWFCORE_ALLOC_OBJECT( Access(*pW2D, *pSection) );
        pXAMLFile->opcResourceMaterializer() = pAccess;

        if (!bForExtents)
        {
          _extent.setPlotSettings( dPaperWidth, dPaperHeight, units);
          _extent.calculateScale();
        }
        loadStream(pInputStream, *pXAMLFile);

        DWFCORE_FREE_OBJECT(pAccess);
        pXAMLFile->close();
        XAMLFactory.Destroy(pXAMLFile);
        DWFCORE_FREE_OBJECT(pTempInputStream);
      }
    }
    DWFCORE_FREE_OBJECT( piResources );
  }
  return res;
}

namespace image{
  EmbeddedImageDefPtr createImageDef(OdDbDatabase* db, OdDbObjectId& imageDefId, OdStreamBufPtr buf,int cols, int rows, bool useStableNames );
}

void DwfImporter::loadFontResources(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection)
{
#if defined(_MSC_VER)
  if (DWFToolkit::DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole( DWFXML::kzRole_Font))
  {
    for (; piResources->valid(); piResources->next())
    {
      if (DWFFontResource* pFontRes = dynamic_cast<DWFFontResource*>(piResources->get()))
      {
        DWFEmbeddedFontImpl_Win32* ef = new DWFEmbeddedFontImpl_Win32(*pFontRes);
        ef->load(false);

        OdString name = (const wchar_t*) OdUS2W(pFontRes->logfontName()),
                 loadedName = (const wchar_t*) OdUS2W(ef->loadedFontName());
        _embeddedFontMap[name] = loadedName;
        DWFCORE_FREE_OBJECT(ef);
      }
    }
    DWFCORE_FREE_OBJECT( piResources );
  }
#endif
}

void DwfImporter::loadRasterResource(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName)
{
  if (DWFToolkit::DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole( pRoleName ))
  {
    for (; piResources->valid(); piResources->next())
    {
      if (DWFImageResource* pRasterRes = dynamic_cast<DWFImageResource*>(piResources->get()))
      {
        OdStreamBufPtr data = OdMemoryStream::createNew();
        {
          DWFInputStream* pStream = pReader->extract( pRasterRes->href(), false );
          while(pStream->available())
          {
            char buf[1024];
            data->putBytes(buf, (OdUInt32)pStream->read(buf, 1024));
          }
          DWFCORE_FREE_OBJECT(pStream);
          data->rewind();
        }
        OdDbObjectId imageDefId;
        OdGePoint2d minpt(pRasterRes->extents()[0], pRasterRes->extents()[1]);
        OdGePoint2d maxpt(pRasterRes->extents()[2], pRasterRes->extents()[3]);
        double width = fabs(maxpt.x - minpt.x);
        double height = fabs(maxpt.y - minpt.y);
        EmbeddedImageDefPtr pImageDef = image::createImageDef(database(), imageDefId, data, (int)width, (int)height, false);
        OdString name = (const wchar_t*)pRasterRes->href();
        int slash = name.reverseFind('\\');
        if (slash != -1)
          name = name.mid(slash + 1);
        pImageDef->setName(name);
        OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
        blocks().currentBlock()->appendOdDbEntity(pImage);
        pImage->setImageDefId(imageDefId);
        OdGeMatrix3d transform;
        for (int i = 0; i < 4; ++i)
          for (int j = 0; j < 4; ++j)
            transform[i][j] = pRasterRes->transform()[i*4+j];
        OdGeVector3d u, v, w;
        OdGePoint3d org;
        transform.getCoordSystem(org, u, v, w);
        u *= width;
        v *= height;
        pImage->setOrientation(org, u, v);
        pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
        pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);
        pImage->setBrightness(OdUInt8(50));
        pImage->setContrast(OdUInt8(50));
        OdGePoint2dArray pts;
        pts.push_back(OdGePoint2d::kOrigin);
        pts.push_back(OdGePoint2d(width, height));
        pImage->setClipBoundary(pts); // 2 points mean OdDbRasterImage::kRect boundary type
      }
    }
    DWFCORE_FREE_OBJECT( piResources );
  }
}

OdDwfImport::ImportResult DwfImporter::load2dResource( DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName, double dPaperWidth, double dPaperHeight, OdDbPlotSettings::PlotPaperUnits units, DWFImportProgressMeter pm, int layoutCount, bool bForExtents)
{
  OdDwfImport::ImportResult res = OdDwfImport::success;
  if (DWFToolkit::DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole( pRoleName ))
  {
    for (; piResources->valid(); piResources->next())
    {
      if (DWFGraphicResource* pW2D = static_cast<DWFGraphicResource*>(piResources->get()))
      {
        if (pW2D->mime() == DWFCore::DWFMIME::kzMIMEType_W2D_S)
        {
          res = OdDwfImport::encrypted_file;
          break;
        }
        _extent._units = _extent._unitsPrimary;
        // apply global resource transform, so that all resources will get into the same coordinate space
        OdGeMatrix3d global;
        for (int i = 0; i < 4; ++i)
        {
          for (int j = 0; j < 4; ++j)
            global[i][j] = pW2D->transform()[j * 4 + i];
        }
        if (!global.isSingular())
          _extent._units.postMultBy(global);
        
        if (bForExtents)
        {
          DWFInputStream* pW2DStream = pReader->extract(pW2D->href(), false);
          _extent._collectBounds = true;
          {
            WT_File wtFile;
            res = loadStream(pW2DStream, wtFile);
          }
          DWFCORE_FREE_OBJECT( pW2DStream );
          if (res != OdDwfImport::success)
            break;
        }
        else
        {
          DWFInputStream* pW2DStream = pReader->extract(pW2D->href(), false);
          DWFMonitoredInputStream pMonitor(pW2DStream, true);
          ProgressMonitor m(pW2DStream->available(), pm, 100./layoutCount);
          pMonitor.attach(&m, false);
          _ignoreMetadata = false;
          _extent._collectBounds = false;
          _extent.calculateScale();
          {
            WT_File wtFile;
            res = loadStream(pW2DStream, wtFile);
          }
          if (res != OdDwfImport::success)
            break;
        }
      }
    }
    DWFCORE_FREE_OBJECT( piResources );
  }
  return res;
}

OdDwfImport::ImportResult DwfImporter::getPrimaryUnits(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection)
{
  ImportResult retval = OdDwfImport::success;
  if (DWFToolkit::DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole(DWFXML::kzRole_Graphics2d))
  {
    for (; piResources->valid(); piResources->next())
    {
      if (DWFGraphicResource* pW2D = static_cast<DWFGraphicResource*>(piResources->get()))
      {
        if (pW2D->mime() == DWFCore::DWFMIME::kzMIMEType_W2D_S)
        {
          retval = OdDwfImport::encrypted_file;
          break;
        }
        DWFInputStream* pW2DStream = pReader->extract(pW2D->href(), false);
        _extent._collectBounds = true;
        {
          WT_File wtFile;
          retval = loadStream(pW2DStream, wtFile);
        }
        DWFCORE_FREE_OBJECT(pW2DStream);
        if (_extent._useUnits)
        {
          OdGeMatrix3d global;
          for (int i = 0; i < 4; ++i)
          {
            for (int j = 0; j < 4; ++j)
              global[i][j] = pW2D->transform()[j * 4 + i];
          }
          OdGeMatrix3d inv;
          if (   global.inverse(inv, OdGeContext::gTol.equalPoint())
              || global.inverse(inv, 1.175494351e-38)) // FLT_MIN)) // CORE-11621 import problem with N72291_Rev_A.DWF
            _extent._units.postMultBy(inv);
          else
          {
            ODA_FAIL_ONCE_X(TDWF);
          }
          _extent._unitsPrimary = _extent._units;
        }
        break;
      }
    }
    DWFCORE_FREE_OBJECT(piResources);
  }
  return retval;
}

#define CHECK_RESULT(v) retval = v; if (retval != OdDwfImport::success) break

OdDwfImport::ImportResult DwfImporter::loadPackage( DWFInputStream& stream, const OdChar* password )
{
  ImportResult retval = OdDwfImport::success;
  DWFPackageReader* pReader = createReader( stream, password, retval );
  if (!pReader) return OdDwfImport::fail;
  
  // parse the manifest
  //
  DWFManifest& rManifest = pReader->getManifest();

  // process all non-global section content
  //
  if (DWFManifest::SectionIterator* piSections = rManifest.getSections())
  {
    int layoutCount = 0;
    for (; piSections->valid(); piSections->next())
    {
      DWFSection* pSection = piSections->get();
      if (pSection && (pSection->type() == DWFString(_DWF_FORMAT_EPLOT_TYPE_STRING) || pSection->type() == DWFString(_DWF_FORMAT_EMODEL_TYPE_STRING)))
        ++layoutCount;
    }
    piSections->reset();
    
    int nLayout = 0;
    int nLayoutRequired = OdRxVariantValue((OdRxObject*)properties()->getAt(OD_T("LayoutNumber")))->getInt32();
    if (nLayoutRequired > layoutCount)
      return OdDwfImport::fail;
    if (nLayoutRequired != -1)
      layoutCount = 1;

    DWFImportProgressMeter pm(_db);

    for (; piSections->valid(); piSections->next())
    {
      DWFSection* pSection = piSections->get();
      // Find DWFEPlotSection
      if (pSection && (pSection->type() == DWFString(_DWF_FORMAT_EPLOT_TYPE_STRING)))
      {
        if (nLayoutRequired != -1 && nLayout++ != nLayoutRequired)
          continue;

        DWFEPlotSection* pEPlotSection = dynamic_cast<DWFEPlotSection*>(pSection);

        if (!pEPlotSection)
          return OdDwfImport::fail;

        // there may be invalid attributes in descriptor
        try { pSection->readDescriptor(); } catch (const DWFException&){}

        DWFPaper* pPaper = pEPlotSection->paper();

        ODCOLORREF color( 0x00ffffff );
        OdDbPlotSettings::PlotPaperUnits units( OdDbPlotSettings::kMillimeters );
        double dPaperWidth = 215.89999389648;
        double dPaperHeight = 279.39999389648;

        if ( pPaper )
        {
          color = pPaper->color();
          units = (pPaper->units() == DWFPaper::eMillimeters)
                    ? OdDbPlotSettings::kMillimeters
                    : OdDbPlotSettings::kInches;
          dPaperWidth = pPaper->width();
          dPaperHeight = pPaper->height();
          _extent.adjustParerUnits(units, dPaperWidth, dPaperHeight);
        }

        properties()->putAt( OD_T("Background"), OdRxVariantValue(dwfImp::exchangeRBColorComponents((OdInt32)ODRGB(ODGETRED(color),ODGETGREEN(color),ODGETBLUE(color)))));

        OdString sLayoutName = getLayoutName(_usedLayoutNames, pSection);
        //if (sLayoutName.isEmpty())
        //{ 
        //  // Layout name by GUID in name
        //  sLayoutName = (const wchar_t *)pSection->name(); // CORE-8033
        //  OdString sPref = (const wchar_t *)pSection->type();
        //  sPref += L'_';
        //  if (!sLayoutName.find(sPref))
        //    sLayoutName = sLayoutName.mid(sPref.getLength());
        //}

        bool bModelToLayout = false;
        if (properties()->has(L"ModelToLayout"))
          bModelToLayout = OdRxVariantValue(properties()->getAt(L"ModelToLayout").get())->getBool();
        setupLayout(_usedLayoutNames, sLayoutName, _db, _blocks, bModelToLayout);

        loadRasterResource(pReader, pSection, DWFXML::kzRole_RasterOverlay);
        if (_xps)
        {
          CHECK_RESULT(load2dResourceXps(pReader, pSection, DWFXML::kzRole_Graphics2d, dPaperWidth, dPaperHeight, units, pm, layoutCount, true));
          CHECK_RESULT(load2dResourceXps(pReader, pSection, DWFXML::kzRole_Graphics2dOverlay, dPaperWidth, dPaperHeight, units, pm, layoutCount, true));
          CHECK_RESULT(load2dResourceXps(pReader, pSection, DWFXML::kzRole_Graphics2d, dPaperWidth, dPaperHeight, units, pm, layoutCount, false));
          CHECK_RESULT(load2dResourceXps(pReader, pSection, DWFXML::kzRole_Graphics2dOverlay, dPaperWidth, dPaperHeight, units, pm, layoutCount, false));
        }
        else
        {
          if (pPaper && units == OdDbPlotSettings::kInches)
            // to check validation data in DwfCallbackManager::process_plot_info by next
            _extent.setPaperInfo(dPaperWidth, dPaperHeight, OdGePoint3d(dPaperWidth / 2.0, dPaperHeight / 2.0, 0.0), units, // CORE-10319
                                 true); // for problem with paper size of CORE-11621
	        loadFontResources(pReader, pSection);
          CHECK_RESULT(getPrimaryUnits(pReader, pSection));
          CHECK_RESULT(load2dResource(pReader, pSection, DWFXML::kzRole_Graphics2dOverlay, dPaperWidth, dPaperHeight, units, pm, layoutCount, true));
          _extent.setPlotSettings(dPaperWidth, dPaperHeight, units);
          CHECK_RESULT(load2dResource(pReader, pSection, DWFXML::kzRole_Graphics2d, dPaperWidth, dPaperHeight, units, pm, layoutCount, false));
          CHECK_RESULT(load2dResource(pReader, pSection, DWFXML::kzRole_Graphics2dOverlay, dPaperWidth, dPaperHeight, units, pm, layoutCount, false));
          CHECK_RESULT(load2dResource(pReader, pSection, DWFXML::kzRole_Graphics2dMarkup, dPaperWidth, dPaperHeight, units, pm, layoutCount, false));
        }
        
        cleanupW2D();
      }
      else if (pSection && (pSection->type() == DWFString(_DWF_FORMAT_EMODEL_TYPE_STRING)))
      {
		    if (OdRxVariantValue((OdRxObject*)properties()->getAt(OD_T("ImportW3d")))->getBool())
        {
          CHECK_RESULT(loadW3d(static_cast<DWFEModelSection*>(pSection), pm, layoutCount));
        }
      }
    }
    DWFCORE_FREE_OBJECT( piSections );
  }

  DWFCORE_FREE_OBJECT(pReader);
  return retval;
}

// DWF-wide metadata has no correspondence in DWG ??
//
//void DwfImporter::ImportSumminfo(EPlotDwfReader* reader);
//{
//  OdDbDatabaseSummaryInfoPtr summ = oddbGetSummaryInfo(_db);
//  DWFManifest::ManifestRef manifest = reader->GetManifest();
//  summ->addCustomSummaryInfo(SummValue, OdString(v.string().ascii()));
//}

class DwfImportCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("IMPORTS"); }
  const OdString globalName() const { return OD_T("dwfIn"); };
  void execute(OdEdCommandContext* pCmdCtx)
  {
    OdSmartPtr<OdEdUserIO> pIO = pCmdCtx->userIO();
	OdString path;
    try
    {
		path = pIO->getFilePath( OD_T("Enter file name:"), OdEd::kGfpForOpen, OD_T("Dwf file to load"),
			OD_T("dwf"), OdString::kEmpty, OD_T("Design Web Format (*.dwf)|*.dwf|DWF 2d sheet (*.w2d)|*.w2d|DWFx (*.dwfx)|*.dwfx||"));
		if ( path.isEmpty() )
			return;
	}
	catch( const OdEdCancel )
	{
		return;
	}

    OdString password = pIO->getString(OD_T("password <>:"));
    OdDwfImportPtr importer = createImporter();
    OdRxDictionaryPtr pProps = importer->properties();
		// Target database
    pProps->putAt( OD_T("Database"), OdDbCommandContextPtr(pCmdCtx)->database() );
		// Source DWF file
    pProps->putAt( OD_T("DwfPath"), OdRxVariantValue(path) );
		// DWF may be password protected
    pProps->putAt( OD_T("Password"), OdRxVariantValue(password) );
		// very old DWF file may contain no paper information
		// in that case the next 2 parameters are used for layout initialization
    pProps->putAt( OD_T("PaperWidth"),
      OdRxVariantValue( pIO->getReal(OD_T("Paper width <297>:"), OdEd::kInpDefault, 297. ) ) );
    pProps->putAt( OD_T("PaperHeight"),
      OdRxVariantValue( pIO->getReal(OD_T("Paper height <210>:"), OdEd::kInpDefault, 210. ) ) );
		// palette of the DWF may be different from the palette of the DWG. 
		// If the color of the entity in DWF is set by index, and the palettes are different, 
		// imported file will look wrong.
    pProps->putAt( OD_T("PreserveColorIndices"),
      OdRxVariantValue( pIO->getKeyword(OD_T("Preserve color indices [Yes/No] <Yes>:"), OD_T("Yes No"), 0 ) == 0 ) );

    try
    {
			// DWF may contain many sheets. You may select which one to import
      pProps->putAt(OD_T("LayoutNumber"), OdRxVariantValue(
        (OdInt32)pIO->getInt(OD_T("Select layout to import <All>:"), 0, 0, OD_T("All"))));
    }
    catch (const OdEdKeyword&)
    {
    }
		// importing HSF works well enough with files exported from Acad, 
		// but fails sometimes on the files produced by Inventor
    pProps->putAt( OD_T("ImportW3d"), OdRxVariantValue(
      pIO->getKeyword(OD_T("Import 3d DWF (HSF) model? [Yes/No] <No>:"), OD_T("Yes No"), 1)==0));

    importer->import();
    OdRxVariantValue backGround = (OdRxObject*)pProps->getAt(OD_T("Background"));
    OdString s;
		// DWF background value is returned from import
    s.format(OD_T("Background = %x"), backGround->getInt32());
    pIO->putString( s );
  }
};

class Dwf7ImportModule : public OdDwfImportModule
{
  OdStaticRxObject<DwfImportCommand> m_Command;
public:
  virtual void initApp()
  {
#ifndef TD_NOCOMMANDS
    odedRegCmds()->addCommand( &m_Command );
#endif
    DwfEventReactor::setModule(this);
  }
  virtual void uninitApp()
  {
#ifndef TD_NOCOMMANDS
    odedRegCmds()->removeCmd( m_Command.groupName(), m_Command.globalName() );
#endif
    DwfEventReactor::setModule(0);
  }
  
    virtual OdDwfImportPtr create ()
  {
      return OdRxObjectImpl<DwfImporter>::createObject();
  }
  
};
}
using namespace TD_DWF_IMPORT;
ODRX_DEFINE_DYNAMIC_MODULE(Dwf7ImportModule)

