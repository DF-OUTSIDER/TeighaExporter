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

#include "StdAfx.h"
#include "OdaCommon.h"

#include "DbRasterImage.h"
#include "DbPolyline.h"
#include "DbViewport.h"
#include "DbHyperlink.h"
#include "DbDictionary.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbTextStyleTable.h"
#include "DbLinetypeTable.h"
#include "DbHostAppServices.h"

#include "RxVariantValue.h"
#include "RxRasterServices.h"
#include "OdPlatformStreamer.h"
#include "MemoryStream.h"
#include "ColorMapping.h"

#include "Gi/GiTextStyle.h"
#include "Gi/GiRasterWrappers.h"

#include "DwfImportImpl.h"
#include "DwfEventReactor.h"
#include "EmbeddedImageDef.h"
#include "DwfCallbacks.h"
#include "DbLine.h"
#include "DbSetPlotSettingsPE.h"

using namespace TD_DWF_IMPORT::dwfImp;

namespace TD_DWF_IMPORT {

static WT_Result WtStreamOpenAction(WT_File & ){ return WT_Result::Success;}
static WT_Result WtStreamCloseAction(WT_File & ){ return WT_Result::Success;}
static WT_Result WtStreamSeekAction(WT_File & file, int distance, int & amount_seeked)
{
  DWFInputStream* stream = (DWFInputStream*)file.stream_user_data();
  stream->seek(SEEK_CUR, distance);
  amount_seeked = distance;
  return WT_Result::Success;
}
static WT_Result WTStreamReadAction(WT_File & file,int bytes_desired,int & bytes_read,void * buffer)
{
  DWFInputStream* stream = (DWFInputStream*)file.stream_user_data();
  bytes_read = (int)stream->read(buffer, bytes_desired);
  return WT_Result::Success;
}

static WT_Result WTStreamTellAction(WT_File & , unsigned long*)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}

static WT_Result WTStreamWriteAction(WT_File & ,int ,void const * )
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}
static WT_Result WtStreamEndSeekAction(WT_File &)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}

OdDwfImport::ImportResult DwfImporter::loadStream(DWFInputStream* pStream, WT_File& wtFile)
{
  wtFile.set_file_mode(WT_File::File_Read);
  if (!_xps)
  {
    wtFile.set_stream_user_data(pStream);
    wtFile.set_stream_open_action(WtStreamOpenAction);
    wtFile.set_stream_close_action(WtStreamCloseAction);
    wtFile.set_stream_seek_action(WtStreamSeekAction);
    wtFile.set_stream_end_seek_action(WtStreamEndSeekAction);
    wtFile.set_stream_read_action(WTStreamReadAction);
    wtFile.set_stream_tell_action(WTStreamTellAction);
    wtFile.set_stream_write_action(WTStreamWriteAction);
  }
  wtFile.open();
  wtFile.rendition().font() = WT_Font();
  wtFile.heuristics().set_user_data(this);
  
#define CALLBACK_MACRO(class_name, class_lower) \
		wtFile.set_##class_lower##_action(DwfCallbackManager::process_##class_lower);
  MY_CALLBACK_LIST
#undef CALLBACK_MACRO
    
    // Do the actual reading.
    while (wtFile.process_next_object() == WT_Result::Success)
    {}
    wtFile.close();
    extent().setClipRect(WT_Logical_Box(0,0,0,0));
    return OdDwfImport::success;
}

DwfBlockManager::DwfBlockManager(DwfImporter* importer) : _importer(importer), m_bPreserveColorIndices(false)
{
}

OdDbObjectId DwfBlockManager::addViewport()
{
	OdDbViewportPtr	vp = OdDbViewport::createObject();
	OdDbLayoutPtr layout = _currentBlock->getLayoutId().safeOpenObject();
	OdDbBlockTableRecordPtr lBlock = layout->getBlockTableRecordId().safeOpenObject(OdDb::kForWrite);
	lBlock->appendOdDbEntity(vp);
	return vp->objectId();
}

DwfExtentManager::DwfExtentManager(DwfImporter* importer)
  : _importer(importer)
  , _clipRect(0, 0, 0, 0)
  , _useUnits(false) 
{
}

// Transform logical point to DWG point
//
OdGePoint3d DwfExtentManager::transformPoint(const WT_Logical_Point& wtPoint)
{
  if ( _useUnits )
  {
    OdGePoint3d p( wtPoint.m_x, wtPoint.m_y, 0);
    p.transformBy(_units);
    return p;
  }
  else
  {
    return OdGePoint3d( 
      (wtPoint.m_x + _scaleInfo._xOffset) * _scaleInfo._scale,
      (wtPoint.m_y + _scaleInfo._yOffset) * _scaleInfo._scale, 0);
  }
}

// convert DWF logical size to DWG size
//
double DwfExtentManager::transformSize(double size)
{
	return _scaleInfo._scale * size;
}

OdDbHatchPtr DwfBlockManager::addHatch(WT_File& file)
{
	OdDbHatchPtr pHatch = OdDbHatch::createObject();
	addEntity(pHatch, file);
	pHatch->setAssociative(false);
	pHatch->setHatchStyle(OdDbHatch::kNormal);
	switch (file.rendition().fill_pattern().pattern_id())
	{
	case WT_Fill_Pattern::Solid:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
		return pHatch;
	case WT_Fill_Pattern::Checkerboard:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SQUARE"));
		return pHatch;
	case WT_Fill_Pattern::Crosshatch:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("NET"));
		break;
	case WT_Fill_Pattern::Diamonds:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANSI37"));
		return pHatch;
	case WT_Fill_Pattern::Horizontal_Bars:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("LINE"));
		break;
	case WT_Fill_Pattern::Slant_Left:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANSI31"));
		pHatch->setPatternAngle(OdaPI2);
		break;
	case WT_Fill_Pattern::Slant_Right:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANSI31"));
		break;
	case WT_Fill_Pattern::Square_Dots:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("DOTS"));
		return pHatch;
	case WT_Fill_Pattern::Vertical_Bars:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("LINE"));
		pHatch->setPatternAngle(OdaPI2);
		break;
	case WT_Fill_Pattern::User_Defined:
		ODA_TRACE0("[DWFImport] User Defined pattern encountered");
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
		return pHatch;
	default:
		pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
		break;
	}
  double scale = file.rendition().fill_pattern().pattern_scale();
  if (scale >= 0) 
  {
	  pHatch->setPatternScale(1);
  }
	return pHatch;
}

void DwfBlockManager::addEntity(OdDbEntity* ent, WT_File& file)
{
	_currentBlock->appendOdDbEntity(ent);
  // add to group
	if (!_group.isNull()) _group->append(ent->objectId());
  // urls
	if (!file.rendition().url().url().is_empty())
	{
    OdDbEntityHyperlinkPEPtr hpe = ent;
    OdDbHyperlinkCollectionPtr urls = hpe->getHyperlinkCollection(ent);
    for (WT_URL_Item* url = (WT_URL_Item*)file.rendition().url().url().get_head(); url; url = (WT_URL_Item*)url->next())
    {
      urls->addTail( WTS2ODS(url->address()), WTS2ODS(url->friendly_name()));
    }
    hpe->setHyperlinkCollection(ent, urls);
	}
	// color
  OdCmColor color;
  if (file.rendition().color().materialized())
  {
    if ( m_bPreserveColorIndices && file.rendition().color().index() != WD_NO_COLOR_INDEX && file.rendition().color().index() < 258 )
    {
      color.setColorIndex( (OdUInt16)file.rendition().color().index() );
    }
    else
    {
      WT_RGBA32 rgb = file.rendition().color().rgba();
      if (!m_pDwgPalette.empty())
      {
        ODCOLORREF crf = ODRGB(rgb.m_rgb.r, rgb.m_rgb.g, rgb.m_rgb.b);
        std::map<ODCOLORREF, OdInt16>::const_iterator i = m_pDwgPalette.find(crf);
        if (i == m_pDwgPalette.end())
          color.setRGB(rgb.m_rgb.r, rgb.m_rgb.g, rgb.m_rgb.b);
        else
          color.setColorIndex(i->second);
      }
      else
        color.setRGB(rgb.m_rgb.r, rgb.m_rgb.g, rgb.m_rgb.b);
    }
  }
  else
  {
    color.setColorMethod(OdCmEntityColor::kByLayer);
  }
  ent->setColor(color);
  // visibility, etc
  if (!file.rendition().visibility().visible())
    ent->setVisibility(OdDb::kInvisible);
	ent->setLinetype(_importer->lines().getCurrentLineStyle(file));
	ent->setLineWeight(_importer->lines().getCurrentLineWeight(file));
  ent->setLayer(_layerMap[file.rendition().layer().layer_num()]);
}

namespace image
{
  void setFileExt(EmbeddedImageDefPtr pImageDef, int format)
  {
    static const OdChar* imageExts[] = 
    {
      OD_T(""),
        OD_T(""),
        OD_T(".bmp"),
        OD_T(".g3x"),
        OD_T(".bmp"),
        OD_T(".bmp"),
        OD_T(".bmp"),
        OD_T(".bmp"),
        OD_T(".jpg"),
        OD_T(".tif"),
        OD_T(""),
        OD_T(""),
        OD_T(".png"),
        OD_T(".tif")
    };
		ODA_ASSERT( format > 0 && format < sizeof(imageExts) / sizeof(imageExts[0]));
    pImageDef->setExtension(imageExts[format]);
  }
  
  OdStreamBufPtr createBitmap( int format, const unsigned char* data, int dataSize,int cols, int rows, const WT_Color_Map* colormap )
  {
    OdStreamBufPtr buf = OdMemoryStream::createNew();
	OdStreamBuf* pBuf = buf;

    if ( format == WT_Image::JPEG 
      || format == WT_Image::Bitonal_Mapped 
      || format == WT_Image::Group3X_Mapped 
      || format == WT_PNG_Group4_Image::Group4X_Mapped
      || format == WT_PNG_Group4_Image::Group4
      || format == WT_PNG_Group4_Image::PNG )
    {
      buf->putBytes( data, dataSize );
    }
    else
    {
      int bpp = 1; // bytes per pixel
      if ( format == WT_Image::RGB ) bpp = 3;
      else if ( format == WT_Image::RGBA ) bpp = 4;
      int lineSize = cols * bpp; //scanline in image
      int padding = ((lineSize + 3)/4)*4 - lineSize; // bmp lines must be 32bit aligned
      int paletteSize = colormap ? 4 * colormap->size() : 0; // 24bit raster have no colormap

      OdPlatformStreamer::wrInt16(*pBuf, 0x4d42);
      OdUInt32 scanLinesPos = (OdUInt32)sizeof(BITMAPFILEHEADER) + 
        (OdUInt32)sizeof(BITMAPINFOHEADER) + paletteSize;
      OdUInt32 size = scanLinesPos + (lineSize + padding) * rows;
      OdPlatformStreamer::wrInt32(*pBuf, size);
      OdPlatformStreamer::wrInt32(*pBuf, 0); // reserved
      OdPlatformStreamer::wrInt32(*pBuf, scanLinesPos); // offBits
      OdPlatformStreamer::wrInt32(*pBuf, sizeof(BITMAPINFOHEADER));
      // save BITMAPINFOHEADER
      OdPlatformStreamer::wrInt32(*pBuf, cols);
      OdPlatformStreamer::wrInt32(*pBuf, rows);
      OdPlatformStreamer::wrInt16(*pBuf, 1);
      OdPlatformStreamer::wrInt16(*pBuf, (OdInt16)(bpp * 8));
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      OdPlatformStreamer::wrInt32(*pBuf, 0);
      // palette
      if ( colormap != 0 && paletteSize != 0 )
        buf->putBytes( colormap->map(), paletteSize );
      // pixels
      for ( int i = rows - 1; i >= 0 ; i-- )
      {
        if ( bpp == 1 ) 
          buf->putBytes( data + i * lineSize, lineSize );
        else for ( int j = 0; j < cols; j++ )
        {
          buf->putBytes( data + i * lineSize + j * bpp + 2, 1 );
          buf->putBytes( data + i * lineSize + j * bpp + 1, 1 );
          buf->putBytes( data + i * lineSize + j * bpp, 1 );
          if ( bpp == 4 ) 
            buf->putBytes( data + i * lineSize + j * bpp + 3, 1 );
        }
        if ( padding ) buf->putBytes( data, padding );
      }
    }
    buf->rewind();
    return buf;
  }

  EmbeddedImageDefPtr createImageDef(OdDbDatabase* db, OdDbObjectId& imageDefId, OdStreamBufPtr buf,int cols, int rows, bool useStableNames )
  {
		// Add an image entity to the drawing.
		OdDbObjectId imageDictId = OdDbRasterImageDef::createImageDictionary(db);
		
		// Open the image dictionary.
		OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);
		
		// Add imagedef object, which is similar to a block definition in that
		// it defines the associated raster file and attributes for this image.
    EmbeddedImageDefPtr pImageDef = EmbeddedImageDef::createObject();
    pImageDef->setData( buf );

		// Add imagedef to image dictionary.
    static int index = 1;
    OdString keyName = dwfImp::generateName(OD_T("Image"), pImageDict, index);
		imageDefId = pImageDict->setAt(keyName, pImageDef);
    if (useStableNames)
      pImageDef->setName(keyName);
    // add image to reactor, to be notified of db saving
    DwfEventReactor::getReactor(db)->addImage(pImageDef);

    OdGiRasterImagePtr img;
    // try to load image (for rendering), else set placeholder
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if ( !pRasSvcs.isNull() )
      img = pRasSvcs->loadRasterImage(buf);
    
    if ( !img.isNull() )
      pImageDef->setImage(img);
    else
      pImageDef->setImage(OdGiRasterImageDesc::createObject(cols, rows));
        
    return pImageDef;
  }
}

void DwfImageManager::addImage(const unsigned char* data, int dataSize, int cols, int rows, 
	int format, const WT_Color_Map* colormap, const WT_Logical_Point& ll, const WT_Logical_Point& ur, int id, WT_File& file)
{
  if ( file.rendition().blockref().get_format() == WT_BlockRef::Thumbnail ) return;
	OdDbObjectId imageDefId;
	ImageMap::const_iterator it = _imageMap.find(id);
	if (it == _imageMap.end())
	{
    OdRxVariantValue tmp((const OdRxObjectPtr)_importer->properties()->getAt("UseStableImageNames"));
    EmbeddedImageDefPtr pImageDef = image::createImageDef(_importer->database(), imageDefId, 
      image::createBitmap(format, data, dataSize, cols, rows, colormap ), cols, rows, tmp->getBool());

    image::setFileExt(pImageDef, format);
		_imageMap[id] = imageDefId;
	}
	else imageDefId = it->second;
	// Add image entity, which references the above imagedef.  
	OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
	// Add image entity to block.
	_importer->blocks().addEntity(pImage, file);
	
	// Set remaining image attributes.
	pImage->setImageDefId(imageDefId);
	OdGePoint3d minCorner = _importer->extent().transformPoint(ll);
	OdGePoint3d maxCorner = _importer->extent().transformPoint(ur);
	pImage->setOrientation(minCorner, 
    OdGeVector3d(maxCorner.x - minCorner.x, 0, 0), 
    OdGeVector3d(0, maxCorner.y - minCorner.y, 0));
	pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
	pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);
	pImage->setBrightness(OdUInt8(50));
	pImage->setContrast(OdUInt8(50));
  bool transparent = 
    (format == WD_IMAGE_PNG_EXT_OPCODE) ||
    (format == WD_IMAGE_RGBA_EXT_OPCODE) ||
    (format == WD_IMAGE_GROUP4X_MAPPED_EXT_OPCODE) ||
    (format == WD_IMAGE_GROUP4_BITONAL_EXT_OPCODE);
  pImage->setDisplayOpt(OdDbRasterImage::kTransparent, transparent);
  // boundary clip
  OdGePoint2dArray pts;
  pts.push_back(OdGePoint2d::kOrigin);
  pts.push_back(OdGePoint2d(cols, rows));
  
  pImage->setClipBoundary(pts); // 2 points mean OdDbRasterImage::kRect boundary type
}

namespace dwfImp
{
  // https://stackoverflow.com/questions/23919515/how-to-convert-from-utf-16-to-utf-32-on-linux-with-std-library
  //inline bool is_surrogate(OdUInt16 uc) { return ((uc >= 0xD800) && (uc <= 0xDFFF)); } //{ return (uc - 0xd800) < 2048; }
  //inline bool is_high_surrogate(OdUInt16 uc) { return (uc & 0xfffffc00) == 0xd800; }
  //inline bool is_low_surrogate(OdUInt16 uc) { return (uc & 0xfffffc00) == 0xdc00; }
  //inline OdChar surrogate_to_utf32(OdUInt16 high, OdUInt16 low) { return (high << 10) + low - 0x35fdc00; }

  OdString WTS2ODS(const WT_String& s)
  {
    OdString sRes;
    //ODA_ASSERT_VAR(const wchar_t* pWIitial = sRes.c_str();)
    const WT_Unsigned_Integer16* pUtf16 = s.unicode();
    if (sizeof(OdChar) == sizeof(WT_Unsigned_Integer16))
      sRes = OdUS2W(pUtf16);
    else
    {
      // CORE-11366 Mac - Text from DWFx files is not properly transferred to DWG
      int len = s.length(),
          idx = 0;
      bool bAscii = (len > 0); // problem with DWF_Background_18621_VK // = s.is_ascii() != WD_False;
      OdChar* pDest = sRes.getBuffer(len + 1);
//#    if 1
      if (bAscii && (!pUtf16[0] || (len > 1 && !pUtf16[1])))
        bAscii = false;
      if (!bAscii)
      {
        // CORE-11366 in fact it returns wchar_t for real unicode (32 bit for mac & some linux) data instead WT_Unsigned_Integer16*
        len = (len / 2);
        const WT_Unsigned_Integer32* pUtf32 = (const WT_Unsigned_Integer32*)pUtf16;
        while (idx < len)
          *pDest++ = pUtf32[idx++];
      }
      else
        while (idx < len)
          *pDest++ = pUtf16[idx++];

//      //len = s.length(),
//      //idx = 0;
//#    else
//      OdChar* pDest = sRes.getBuffer(len + 1);
//      while (idx < len)
//      {
//        OdUInt16 uc = pUtf16[idx++];
//        if (!uc)
//        {
//          ODA_ASSERT_ONCE_X(TDWF, sizeof(UCS2_char_t) == 2 && sizeof(UCS4_char_t) == 4); // maybe only via incorrect include path of Types.h
//          uc = pUtf16[idx++]; // Linux Debian 8.5 gcc4.9dll TODO looks like incorrect vis toolkit
//        }
//        if (!is_surrogate(uc))
//          *pDest++ = uc;
//        else if (is_high_surrogate(uc) && idx < len)
//        {
//          OdUInt16 uc_ = pUtf16[idx++];
//          if (!uc_ && idx < len)
//            uc_ = pUtf16[idx++];
//          if (is_low_surrogate(uc_))
//            *pDest++ = surrogate_to_utf32(uc, uc_);
//          else
//            *pDest++ = L'?'; // ERROR
//        }
//        else 
//          *pDest++ = L'?'; // ERROR
//      }
//#    endif

      *pDest = L'\0';
      sRes.releaseBuffer();

      //ODA_ASSERT_VAR(pWIitial = sRes.c_str();)
    }
    return sRes;
  }

  OdString normalizeObjectName(const WT_String& name)
  {
    OdString s = WTS2ODS(name);
    s.replace('|','_');
    s.replace('<','_');
    s.replace('>','_');
    s.replace('/','_');
    s.replace('*','_');
    s.replace('?','_');
    s.replace('=','_');
    s.replace('\'','_');
    s.replace('\"','_');
    s.replace(':','_');
    s.replace(';','_');
    s.replace(',','_');
    return s;
  }

  OdUInt32 exchangeRBColorComponents(OdUInt32 inputColor)
  {
    OdUInt32 color = inputColor & 0xff00;
    color |= (inputColor & 0xff) << 16;
    color |= (inputColor & 0xff0000) >> 16;
    return color;
  }
}
}

TOOLKIT_EXPORT OdString odDbGenerateName (OdUInt32 i, OdDbHostAppServices* pHostApp = 0);

namespace TD_DWF_IMPORT {
void DwfBlockManager::setCurrentLayer( WT_Layer& layer )
{
	LayerMap::const_iterator ci = _layerMap.find(layer.layer_num());
	if (ci == _layerMap.end())
	{
    OdString name = dwfImp::normalizeObjectName(layer.layer_name());
    if ( name == OD_T("0") || name.isEmpty())
    {
      _layerMap[layer.layer_num()] = _importer->database()->getLayerZeroId();
    }
    else
    {
      OdDbLayerTablePtr pLayers = _importer->database()->getLayerTableId().safeOpenObject(OdDb::kForWrite);
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
      try
      {
        pLayer->setName( name );
      }
      catch (const OdError&)
      {
        name = odDbGenerateName(layer.layer_num(), _importer->database()->appServices());
        pLayer->setName( name );
      }
      
      OdDbObjectId id = pLayers->has( name ) ? pLayers->getAt( name ) : pLayers->add( pLayer );
      _importer->database()->setCLAYER( id );
      _layerMap[layer.layer_num()] = id;
    }
	}
	else _importer->database()->setCLAYER( ci->second );
}

void DwfBlockManager::setCurrentGroup(int id, const OdChar* name)
{
  if (id == -1)
  {
    _group = 0;
    return;
  }
	
  GroupMap::const_iterator i = _groups.find(id);
	if (i == _groups.end())
	{
		OdDbDictionaryPtr pGroupDic = _importer->database()->getGroupDictionaryId().safeOpenObject(OdDb::kForWrite);
		_group = OdDbGroup::createObject();
		OdString odName = name;
		// no name or such an object already exist
		if (odName.isEmpty() || pGroupDic->has(odName))
      odName = dwfImp::generateName(OD_T("Node"), pGroupDic, id);
		pGroupDic->setAt(odName, _group);
		_group->setName(odName);
		_group->setSelectable(true);
		_groups[id] = _group;
	}
	else _group = i->second;
}

void DwfFontManager::calculateAscent(TextStyle& pStyle)
{
  double currentTextHeight = _importer->extent().transformSize(pStyle._wtFont.height().height());
  // recalculating text height, because in ACAD text height == 'A' height
  pStyle._textStyle->setTextSize(100);
  OdGiTextStyle giStyle;
  giFromDbTextStyle(pStyle._textStyle, giStyle);
  pStyle._ascentRatio = ( giStyle.getFont()->getHeight() - giStyle.getFont()->getInternalLeading() ) / giStyle.getFont()->getAbove();
  currentTextHeight /= pStyle._ascentRatio;
  pStyle._textStyle->setTextSize(currentTextHeight);
}

double DwfFontManager::getTextHeight(double h)
{
  double currentTextHeight = _importer->extent().transformSize(h);
  if (_currentTextStyle != -1)
    currentTextHeight /= _fontStyles[_currentTextStyle]._ascentRatio;
  return currentTextHeight;
}

void DwfFontManager::setFontStyle(WT_Font& font)
{
	// search for existing text style
	FontStyleMap::const_iterator ci = _fontStyles.begin();
	for (; ci != _fontStyles.end(); ++ci)	
  {
    if (WTS2ODS(ci->_wtFont.font_name()) == WTS2ODS(font.font_name())
      && ci->_wtFont.style().bold() == font.style().bold()
      && ci->_wtFont.style().italic() == font.style().italic()
      && ci->_wtFont.flags() == font.flags()
      ) break;
  }
	if (ci != _fontStyles.end()) // if found - set & exit
	{
		_importer->database()->setTEXTSTYLE(ci->_textStyle->objectId());
    _currentTextStyle = int(ci - _fontStyles.begin());
		return;
	}

	OdDbTextStyleTableRecordPtr pStyle = OdDbTextStyleTableRecord::createObject();
	// Name must be set before a table object is added to a table.  The
	// isShapeFile flag must also be set (if true) before adding the object
	// to the database.
  OdDbTextStyleTablePtr pStyles = _importer->database()->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
  int index = _fontStyles.size();
	pStyle->setName(dwfImp::generateName( L"style", pStyles, index));
	// shape fonts are exported as polylines (dwf doesn't support shape fonts)
	pStyle->setIsShapeFile(false);
	pStyles->add(pStyle);
	_fontStyles.push_back(TextStyle(font, pStyle));
	_currentTextStyle = _fontStyles.size() - 1;

	// Set the remaining properties.
  OdString name = WTS2ODS(font.font_name().name()); // CORE-11366  font name problem //DWFString name = (const wchar_t*)OdUS2W(font.font_name().name().unicode());
  if (!_importer->isXPS() && _importer->_embeddedFontMap.find(name) != _importer->_embeddedFontMap.end())
    name = _importer->_embeddedFontMap[name];
  pStyle->setFont(name, // CORE-11366  font name problem // (const wchar_t*)name,
    font.style().bold() != 0, font.style().italic() != 0,
    font.charset().charset(), 
    int(font.pitch().pitch()) | int(font.family().family()));
  calculateAscent(_fontStyles.last());

  if ( !(font.flags() & FLAG_PATH_TEXT) ) // for PATH_TEXT properties are taken from text bounds
  {
    pStyle->setXScale(1./1024*font.width_scale());
    pStyle->setObliquingAngle(Oda2PI/(2<<15)*font.oblique());

    // these flags are not part of specification, they are just "used by autocad" (and our exporter)
    pStyle->setIsVertical(font.flags() & FLAG_VERTICAL);
    pStyle->setIsBackwards((font.flags() & FLAG_MIRROR_X) != 0);
    pStyle->setIsUpsideDown((font.flags() & FLAG_MIRROR_Y) != 0);
  }
  _importer->database()->setTEXTSTYLE(pStyle->objectId());
  pStyle->downgradeOpen();
}

// set Linetype dashes
//
void DwfLineManager::setLinePattern(OdDbLinetypeTableRecord* pLt, const short* pattern, int length, double scale)
{
  if (scale <= 0) return; // no pattern length => solid line
  ODA_ASSERT(length != 1);
  pLt->setNumDashes(length);
  int total_length = 0;
  int i = 0;
  for (; i < length; i++)
    total_length += pattern[i];
  pLt->setPatternLength(scale);
  for (i = 0; i < length; i++)
  {
    double dash = pattern[i] * scale / total_length;
    // gaps are negative
    if (i % 2) dash = -dash;
    pLt->setDashLengthAt(i, dash);
  }
}

// set linetype to one of the standard patterns described in WT_Line_Pattern
// WARNING: all non ISO styles are given approximately
//
void DwfLineManager::setPredefinedPattern(OdDbLinetypeTableRecord* pLt, int patternId, double scale)
{
  switch (patternId)
  {
  case WT_Line_Pattern::Solid:
    break;
  case WT_Line_Pattern::Dashed: 
    {
      static short pat[] = {47, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Dotted: 
    {
      static short pat[] = {0, 24};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Dash_Dot: 
    {
      static short pat[] = {36, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Short_Dash: 
    {
      static short pat[] = {36, 72};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Medium_Dash: 
    {
      static short pat[] = {48, 48};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Long_Dash: 
    {
      static short pat[] = {72, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Short_Dash_X2: 
    {
      static short pat[] = {36, 144};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Medium_Dash_X2: 
    {
      static short pat[] = {48, 72};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Long_Dash_X2: 
    {
      static short pat[] = {72, 24};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Medium_Long_Dash: 
    {
      static short pat[] = {64, 24};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Medium_Dash_Short_Dash_Short_Dash: 
    {
      static short pat[] = {36, 12, 24, 12, 24, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Long_Dash_Short_Dash: 
    {
      static short pat[] = {72, 12, 24, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Long_Dash_Dot_Dot: 
    {
      static short pat[] = {72, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Long_Dash_Dot: 
    {
      static short pat[] = {72, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Medium_Dash_Dot_Short_Dash_Dot: 
    {
      static short pat[] = {48, 12, 0, 12, 24, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::Sparse_Dot: 
    {
      static short pat[] = {0, 72};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dash: 
    {
      static short pat[] = {47, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dash_Space: 
    {
      static short pat[] = {47, 71};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Long_Dash_Dot: 
    {
      static short pat[] = {94, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Long_Dash_Double_Dot: 
    {
      static short pat[] = {94, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Long_Dash_Triple_Dot: 
    {
      static short pat[] = {94, 12, 0, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dot: 
    {
      static short pat[] = {0, 79};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Long_Dash_Short_Dash: 
    {
      static short pat[] = {94, 12, 24, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Long_Dash_Double_Short_Dash: 
    {
      static short pat[] = {94, 12, 24, 12, 24, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dash_Dot: 
    {
      static short pat[] = {47, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Double_Dash_Dot: 
    {
      static short pat[] = {47, 12, 47, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dash_Double_Dot: 
    {
      static short pat[] = {47, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Double_Dash_Double_Dot: 
    {
      static short pat[] = {47, 12, 47, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Dash_Triple_Dot: 
    {
      static short pat[] = {47, 12, 0, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  case WT_Line_Pattern::ISO_Double_Dash_Triple_Dot: 
    {
      static short pat[] = {47, 12, 47, 12, 0, 12, 0, 12, 0, 12};
      setLinePattern(pLt, pat, sizeof(pat)/sizeof(pat[0]), scale);
    }
    break;
  default:
    ODA_TRACE1("[DWFImport] Undefined linestyle #%d", patternId);
  }
}

OdDbObjectId DwfLineManager::getCurrentLineStyle(WT_File& file)
{
	int patternId = file.rendition().line_pattern().pattern_id();
	// dash pattern overrides line pattern
	if (file.rendition().dash_pattern() != WT_Dash_Pattern::kNull)
		patternId = file.rendition().dash_pattern().number();

  double scale = _importer->extent().transformSize(file.rendition().line_style().pattern_scale());
	// if pattern scale is not set - ignore pattern (acad patterns must scale)
	if (scale <= 0)	patternId = 1;

	LineStyleMap::const_iterator ci = _lineStyles.find(patternId);
	if (ci != _lineStyles.end()) return ci->second;
		
	OdDbLinetypeTablePtr pTable = _importer->database()->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);
	OdDbLinetypeTableRecordPtr pLt = OdDbLinetypeTableRecord::createObject();

  
  if (patternId < WT_Line_Pattern::Count) // predefined pattern
  {
    ODA_ASSERT(patternId != 0);
    // names of standard DWF line patterns
    const OdChar* wtLinePatternNames[] =
    {
      OD_T(""),            
        OD_T("CONTINUOUS"),
        OD_T("Dashed"),
        OD_T("Dotted"),
        OD_T("Dash_Dot"),
        OD_T("Short_Dash"),
        OD_T("Medium_Dash"),
        OD_T("Long_Dash"),
        OD_T("Short_Dash_X2"),
        OD_T("Medium_Dash_X2"),
        OD_T("Long_Dash_X2"),
        OD_T("Medium_Long_Dash"),
        OD_T("Medium_Dash_Short_Dash_Short_Dash"),
        OD_T("Long_Dash_Short_Dash"),
        OD_T("Long_Dash_Dot_Dot"),
        OD_T("Long_Dash_Dot"),
        OD_T("Medium_Dash_Dot_Short_Dash_Dot"),
        OD_T("Sparse_Dot"),
        OD_T("ISO_Dash"),
        OD_T("ISO_Dash_Space"),
        OD_T("ISO_Long_Dash_Dot"),
        OD_T("ISO_Long_Dash_Double_Dot"),
        OD_T("ISO_Long_Dash_Triple_Dot"),
        OD_T("ISO_Dot"),
        OD_T("ISO_Long_Dash_Short_Dash"),
        OD_T("ISO_Long_Dash_Double_Short_Dash"),
        OD_T("ISO_Dash_Dot"),
        OD_T("ISO_Double_Dash_Dot"),
        OD_T("ISO_Dash_Double_Dot"),
        OD_T("ISO_Double_Dash_Double_Dot"),
        OD_T("ISO_Dash_Triple_Dot"),
        OD_T("ISO_Double_Dash_Triple_Dot")
    };
    // check whether there is such a pattern in dwg
    OdDbObjectId oldId = pTable->getAt(wtLinePatternNames[patternId]);
    if (!oldId.isNull())
    {
      _lineStyles[patternId] = oldId;
      return oldId;
    }
    // if not - create it
    setPredefinedPattern(pLt, patternId, scale);
    pLt->setName(wtLinePatternNames[patternId]);
  }
  else // custom pattern
  {
    setLinePattern(pLt, file.rendition().dash_pattern().pattern(), 
			file.rendition().dash_pattern().length(), scale);
    static int index = 1;
		pLt->setName(dwfImp::generateName(OD_T("LineType"), pTable, index));
	}

	OdDbObjectId id = pTable->add(pLt);
	_lineStyles[patternId] = id;
	return id;
}

OdDb::LineWeight DwfLineManager::getCurrentLineWeight(WT_File& file)
{
  // DWF lineweight is an integer <= 100
	double lw = file.rendition().line_weight().weight_value();
  double w = 0.01 * OdDb::kLnWt211 * lw;
  if (w <= 0) 
    return OdDb::kLnWt000;
#if 1
  if (w >= OdDb::kLnWt211) // TODO find more correct criteria for this way
  {
    DwfExtentManager& mngExtent = _importer->extent();
    if (mngExtent._useUnits)
    {
      OdGePoint3d pt0, pt1(1.0, 0.0, 0.0);
      pt0.transformBy(mngExtent._units);
      pt1.transformBy(mngExtent._units);
      double dScale = OdGeVector3d(pt1 - pt0).length(); // *2.0;
      
      //w *= dScale * 2.0; // it gives more near (to ACAD Designe Reviw) render result if LWDISOLAY is on (1)
      // but next gives more correct printing result
      // ( N72291_Rev_A.dxf of CORE-11621 & 255038-00-00.DWF of CORE-12238)
      w = dScale * lw;

      OdDb::MeasurementValue measure = _importer->database()->getMEASUREMENT();
      if (measure == OdDb::kEnglish)
        w *= kMmPerInch;
    }
  }
#endif
  if ( w >= OdDb::kLnWt211 ) 
    return OdDb::kLnWt211;
  // DWG lineweight may only be one of the following:
	static int CeLWeightTbl[] = {  0,   5,   9,  13,  15,  18,  20,  25, 30,  35,
		40,  50,  53,  60,  70,  80,  90, 100, 106, 120, 140, 158, 200, 211};
	for (int i = 0; i < int(sizeof(CeLWeightTbl) / sizeof(CeLWeightTbl[0]) - 1); i++)
	{
		if (CeLWeightTbl[i] <= w && CeLWeightTbl[i + 1] >= w)
		{
			if (w - CeLWeightTbl[i] > CeLWeightTbl[i + 1] - w)
				return (OdDb::LineWeight) CeLWeightTbl[i + 1];
			else
				return (OdDb::LineWeight) CeLWeightTbl[i];
		}
	}
  return (OdDb::LineWeight) 0;
}

void DwfExtentManager::setClipRect(const WT_Logical_Box& clipRect)
{
  //if (!(_clipRect == clipRect))
    _clipRect = clipRect;
}

bool DwfExtentManager::isValid() const
{
  return _extent.m_max.m_x > _extent.m_min.m_x && _extent.m_max.m_y > _extent.m_min.m_y;
}

void DwfExtentManager::updateBounds(WT_Drawable& d, WT_File& file)
{
  if (file.rendition().visibility().visible() != WD_True) return;
  
  WT_Logical_Point maxpt = d.bounds(&file).maxpt();
  WT_Logical_Point minpt = d.bounds(&file).minpt();
  
//#if 0
//  if (!(_clipRect == WT_Logical_Box(0, 0, 0, 0))
//    && (minpt.m_x < _clipRect.m_min.m_x
//      || maxpt.m_x > _clipRect.m_max.m_x
//      || minpt.m_y < _clipRect.m_min.m_y
//      || maxpt.m_y > _clipRect.m_max.m_y))
//    return;
//  _extent.m_min.m_x = odmin(minpt.m_x, _extent.m_min.m_x);
//  _extent.m_min.m_y = odmin(minpt.m_y, _extent.m_min.m_y);
//  _extent.m_max.m_x = odmax(maxpt.m_x, _extent.m_max.m_x);
//  _extent.m_max.m_y = odmax(maxpt.m_y, _extent.m_max.m_y);
//#else
  if (_clipRect == WT_Logical_Box(0, 0, 0, 0) || minpt.m_x >= _clipRect.m_min.m_x)
    _extent.m_min.m_x = odmin(minpt.m_x, _extent.m_min.m_x);
  if (_clipRect == WT_Logical_Box(0, 0, 0, 0) || minpt.m_y >= _clipRect.m_min.m_y)
    _extent.m_min.m_y = odmin(minpt.m_y, _extent.m_min.m_y);
  if (_clipRect == WT_Logical_Box(0, 0, 0, 0) || maxpt.m_x <= _clipRect.m_max.m_x)
    _extent.m_max.m_x = odmax(maxpt.m_x, _extent.m_max.m_x);
  if (_clipRect == WT_Logical_Box(0, 0, 0, 0) || maxpt.m_y <= _clipRect.m_max.m_y)
    _extent.m_max.m_y = odmax(maxpt.m_y, _extent.m_max.m_y);
//#endif
}

//static 
void DwfExtentManager::adjustParerUnits(OdDbPlotSettings::PlotPaperUnits&, // units
                                        double&, // dPaperWidth
                                        double&, // dPaperHeight
                                        OdGePoint3d*) // pCenter // = NULL
{
  //if (units != OdDbPlotSettings::kInches)
  //  return;
  //// CORE-11621 Workaroud for incorrect paper size with import of N72291_Rev_A.DWF
  //// CORE-12345 [DataFiles]/dwg/ACADSamples/2010/Sheet Sets/Civil/Civil Sample Sheet Set.dwf
  //units = OdDbPlotSettings::kMillimeters;
  //dPaperWidth *= kMmPerInch;
  //dPaperHeight *= kMmPerInch;
  //if (pCenter)
  //{
  //  pCenter->x *= kMmPerInch;
  //  pCenter->y *= kMmPerInch;
  //  //pCenter->z *= kMmPerInch;
  //}
}

bool isPaperAccordant(double wPaper, double hPaper, 
                      double resPaperWidth, double resPaperHeight, // via layout->getPlotPaperSize
                      double dCoeff = 0.01, 
                      double dCoeffY = 0.0) // 0.0 means to use dCoeff
{
  if (   fabs(resPaperWidth - wPaper) < (resPaperWidth * dCoeff) 
      && fabs(resPaperHeight - hPaper) < (resPaperHeight * (dCoeffY == 0.0 ? dCoeff : dCoeffY)))
    return true;
  return false;
}

void DwfExtentManager::setPlotSettings(double wPaper, double hPaper, OdDbPlotSettings::PlotPaperUnits units)
{
  // if have not read paper from dwf - set defaults passed from outside
  bool isPaperUnitsMissing = _paperInfo._isMissing || (_paperInfo._units == OdDbPlotSettings::kPixels);
  //ODA_ASSERT_ONCE_X(TDWF, isPaperUnitsMissing == (_paperInfo._center == OdGePoint3d(0, 0, 0)));
  if (isPaperUnitsMissing)
  {
    _paperInfo._units = units;
    _paperInfo._width = wPaper;
    _paperInfo._height = hPaper;
    _paperInfo._center = OdGePoint3d(0, 0, 0); // unknown
    //will be done later via  setPaperInfo // _paperInfo._isMissing = false;
  }
  else 
  {
    adjustParerUnits(_paperInfo._units, _paperInfo._width, _paperInfo._height, &_paperInfo._center);
    units = _paperInfo._units;
    wPaper = _paperInfo._width;
    hPaper = _paperInfo._height;
  }

  OdDbLayoutPtr layout = _importer->blocks().currentLayout(OdDb::kForWrite);
  OdDbPlotSettingsValidatorPtr pPSV = _importer->database()->appServices()->plotSettingsValidator();
  OdArray<const OdChar*> deviceList;
  OdString deviceName;
  pPSV->plotDeviceList(deviceList);
  for (unsigned i = 0; i < deviceList.size(); ++i)
  {
    if (odStrICmp(deviceList[i], OD_T("DWF6 ePlot.pc3")) == 0)
    {
      ODA_FAIL_ONCE_X(TDWF); // is unreachable now via OdDbPlotSettingsValidatorImpl changes
      deviceName = deviceList[i];
      break;
    }
  }
  if (deviceName.isEmpty() && deviceList.size() > 0)
    deviceName = deviceList[0];
  ODA_ASSERT_VAR(OdResult res =)
  pPSV->setPlotCfgName(layout, deviceName);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk); ODA_ASSERT_VAR(res =)
  pPSV->setPlotRotation(layout, OdDbPlotSettings::k0degrees);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk); ODA_ASSERT_VAR(res =)
  pPSV->setClosestMediaName(layout, wPaper, hPaper, units, false);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  double resPaperWidth, resPaperHeight;
  layout->getPlotPaperSize(resPaperWidth, resPaperHeight);
  if (units == OdDbPlotSettings::kInches)
  {
    resPaperWidth /= kMmPerInch;
    resPaperHeight /= kMmPerInch;
  }
  if (!isPaperAccordant(wPaper, hPaper, resPaperWidth, resPaperHeight)) // if (!OdEqual(wPaper, resPaperWidth) && !OdEqual(hPaper, resPaperHeight))
  {
    ODA_ASSERT_VAR(res =)
    pPSV->setPlotRotation(layout, OdDbPlotSettings::k90degrees);
    ODA_ASSERT_ONCE_X(TDWF, res == eOk);
    std::swap(wPaper, hPaper);
    ODA_ASSERT_VAR(res =)
    pPSV->setClosestMediaName(layout, wPaper, hPaper, units, false);
    ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  }
  if (units == OdDbPlotSettings::kMillimeters)
    _importer->database()->setMEASUREMENT(OdDb::kMetric);
  else
  {
    ODA_ASSERT_ONCE_X(TDWF, units == OdDbPlotSettings::kInches);
    units = OdDbPlotSettings::kInches;
    _importer->database()->setMEASUREMENT(OdDb::kEnglish);
  }
  ODA_ASSERT_VAR(res =)
  pPSV->setPlotPaperUnits(layout, units);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk);

  // CORE-11621 
  layout->getPlotPaperSize(resPaperWidth, resPaperHeight);
  if (units == OdDbPlotSettings::kInches)
  {
    resPaperWidth /= kMmPerInch;
    resPaperHeight /= kMmPerInch;
  }
  if (isPaperAccordant(wPaper, hPaper, resPaperWidth, resPaperHeight)) // if (fabs(resPaperWidth - wPaper) < (resPaperWidth / 100.0) && fabs(resPaperHeight - hPaper) < (resPaperHeight / 100.0))
  {
    // prohibited for layout //res = pPSV->setPlotCentered(layout, true); 
    // (see:  assert ODA_ASSERT(!bPlotCentered); // cannot be used with kLayout) 
    OdDbSetPlotSettingsPEPtr pPsvPE = OdDbSetPlotSettingsPE::cast(layout);
    if (pPsvPE.get())
    {
      // way to avoid dash-render of non-accordant to DWF viewport bound
      ODA_ASSERT_VAR(res =)
      pPsvPE->setMargins(layout, 0, 0, 0, 0); // possible way to fix offsets too
      ODA_ASSERT_ONCE_X(TDWF, res == eOk);
     }
  }
  if (   units == OdDbPlotSettings::kInches
      && isPaperUnitsMissing) // CORE-12345 CORE-12887 CORE-12887
  {
    // CORE-11621 // fix paper size with import of N72291_Rev_A.DWF
    // CORE-12238 // 255038-00-00.DWF
    ODA_ASSERT_VAR(res =)
    pPSV->setCustomPrintScale(layout, 1.0, kMmPerInch); // numerator:denominator
    ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  }

  // MKU 06.07.2004   the thing is OdDbLayoutImpl::activated() activated drawn paper size (limmin/limmax) 
  //  by default values. To change it using actual values
  bool bMm = (units == OdDbPlotSettings::kMillimeters) ? true : false;
  bool bMetric = (_importer->database()->getMEASUREMENT() == OdDb::kEnglish) ? false : true;
  double dWidth = wPaper;
  double dHeight = hPaper;
  if (bMm && !bMetric)
  {
    dWidth /= kMmPerInch;
    dHeight /= kMmPerInch;
  }
  else if (!bMm && bMetric)
  {
    dWidth *= kMmPerInch;
    dHeight *= kMmPerInch;
  }
  layout->setLIMMIN(OdGePoint2d::kOrigin);
  layout->setLIMMAX(OdGePoint2d(dWidth, dHeight));
}

void DwfExtentManager::calculateScale()
{
  OdDbLayoutPtr layout = _importer->blocks().currentLayout();
  double paperWidth, paperHeight;
  layout->getPlotPaperSize(paperWidth, paperHeight);
  ODA_ASSERT(paperWidth > 0 && paperHeight > 0);
  switch(layout->plotRotation())
  {
  case OdDbPlotSettings::k90degrees:
  case OdDbPlotSettings::k270degrees:
    std::swap(paperWidth, paperHeight);
  default:
    break;
  }

  // paper size is always calculated in mm's
  OdDb::MeasurementValue measure = _importer->database()->getMEASUREMENT();
  if (measure == OdDb::kEnglish)
  {
    paperWidth /= kMmPerInch;
    paperHeight /= kMmPerInch;
  }

  int extHeight = _extent.maxpt().m_y - _extent.minpt().m_y;
  int extWidth = _extent.maxpt().m_x - _extent.minpt().m_x;

  if ( _useUnits )
  {
    OdGePoint3d p1(1,0,0), p2(0,0,0), p3(0,1,0);
    p1.transformBy(_units);
    p2.transformBy(_units);
    p3.transformBy(_units);

    if ( OdEqual( p1.z, p2.z ) && OdEqual( p1.z, p3.z ) )
    {
      _scaleInfo._scale = OdGeVector3d(p2-p1).length();

      OdDbObjectId idLayout = layout->objectId();
      if (idLayout != _paperInfo._idLayoutWithCheckOrigin) // CORE-12345
      {
        _paperInfo._idLayoutWithCheckOrigin = idLayout;

        // CORE-11621 paper offsets with import of N72291_Rev_A.DWF
        OdGePoint3d ptMin((double)_extent.minpt().m_x, (double)_extent.minpt().m_y, 0.0),
                    ptMax((double)_extent.maxpt().m_x, (double)_extent.maxpt().m_y, 0.0);
        ptMin.transformBy(_units);
        ptMax.transformBy(_units);
        bool isPaperUnitsMissing = _paperInfo._isMissing || (_paperInfo._units == OdDbPlotSettings::kPixels);

        // CORE-12887
        double dPaperWidth = ptMax.x - ptMin.x,
               dPaperHeight = ptMax.y - ptMin.y;
        bool bConv = (measure == OdDb::kEnglish);
        if (   bConv 
            && isPaperAccordant(dPaperWidth, dPaperHeight, paperWidth, paperHeight, _importer->isXPS() ? 0.7 : 0.5) // CORE-13304 // 0.5)
            && !isPaperAccordant(dPaperWidth / kMmPerInch, dPaperHeight / kMmPerInch, paperWidth, paperHeight, 0.5))
          bConv = false; // CORE-12887
        if (bConv && !_paperInfo._isMissing)
        {
          if (_importer->database()->getModelSpaceId() != layout->getBlockTableRecordId())
            //&& layout->getLayoutName().find(L"Model") != 0)
          {
            // CORE-11881 problem with back exported DWF
            OdDbPlotSettingsValidatorPtr pPSV = _importer->database()->appServices()->plotSettingsValidator();
            layout->upgradeOpen();
            pPSV->setCustomPrintScale(layout, 1.0, kMmPerInch); // numerator:denominator
          }
          ptMin /= kMmPerInch; // CORE-11881 problem with back exported DWF
          ptMax /= kMmPerInch;
        }

        if (   (   measure == OdDb::kEnglish && isPaperUnitsMissing
                && (bConv || !_importer->isXPS())) // xps // CORE-13304
            || bConv)
          dPaperWidth /= kMmPerInch, dPaperHeight /= kMmPerInch;
        double xOffset = layout->getLeftMargin(), // in mm
               yOffset = layout->getBottomMargin(); // in mm
        if ((measure == OdDb::kEnglish && isPaperUnitsMissing) || bConv)
          xOffset /= kMmPerInch, yOffset /= kMmPerInch;
        //OdGePoint2d ptPaperCenter((ptMax.x + ptMin.x) / 2.0, (ptMax.y + ptMin.y) / 2.0);
        ODA_ASSERT_ONCE_X(TDWF, isPaperAccordant(dPaperWidth, dPaperHeight, paperWidth, paperHeight, 0.2, 
                                                                                                     0.7)); // CORE-13304
        if (   !isPaperUnitsMissing // && measure == OdDb::kEnglish) // CORE-12345 CORE-12887
            || (   !bConv  // CORE-13304
               // CORE-13332 problem with paper centering // && _importer->isXPS() // xps
               ))  
        {
          xOffset += (dPaperWidth - paperWidth) / 2.0;
          yOffset += (dPaperHeight - paperHeight) / 2.0;
        }
        ptMin.x += xOffset;
        ptMin.y += yOffset;

        if (fabs(ptMin.x) > (paperWidth / 10000.0) || fabs(ptMin.y) > (paperHeight / 10000.0))
        {
          ODA_ASSERT_ONCE_X(TDWF, !OdZero(fabs(xOffset)) && !OdZero(fabs(yOffset))); // test
          OdDbPlotSettingsValidatorPtr pPSV = _importer->database()->appServices()->plotSettingsValidator();
          layout->upgradeOpen();

          if (measure == OdDb::kEnglish && !isPaperUnitsMissing) // CORE-12345
            ptMin *= kMmPerInch;

          xOffset = -ptMin.x;
          yOffset = -ptMin.y;
          switch (layout->plotRotation())
          {
          case OdDbPlotSettings::k90degrees: case OdDbPlotSettings::k270degrees:
            std::swap(xOffset, yOffset);
          }
          ODA_ASSERT_VAR(OdResult res = )
            pPSV->setPlotOrigin(layout, xOffset, yOffset); // fix offset on paper // CORE-11621
          ODA_ASSERT_ONCE_X(TDWF, res == eOk);
          if (!bConv && measure == OdDb::kMetric) // CORE-13332 CORE-13333 DwfImport: camera placement problem
          {
            OdDbObjectId overallVpId = layout->overallVportId();
            if (!overallVpId.isNull())
            {
              OdRxObjectPtr pVpObj = overallVpId.safeOpenObject(OdDb::kForWrite);
              OdAbstractViewPEPtr pVpPE(pVpObj);
              OdGeBoundBlock3d fullExtents;
              if (pVpPE->viewExtents(pVpObj, fullExtents))
                pVpPE->zoomExtents(pVpObj, &fullExtents);
            }
          }
        }
      }

      // for modelspace retain original coordinates (they may be in other MEASUREMENT though)
      //if ( _importer->database()->getModelSpaceId() == layout->getBlockTableRecordId() )
        return;
      // if entities fit in paper, retain paperspace coordinates
      /*
      // commented as useless via upper return 
      // (see #16930 - DWF markup coordinates are wrong - svn 159632 vkalinin  4/11/2015)
      if ( paperWidth >= _scaleInfo._scale * extWidth && paperHeight >= _scaleInfo._scale * extHeight )
      {
        centerPlot(paperWidth, paperHeight, extHeight, extWidth);
        return;
      }
      */
    }
  }
  _useUnits = false;
  _units = OdGeMatrix3d::kIdentity;
  // try to place the entities on paper, basing on extents (like viewer do)
  if (extWidth == 0)
    _scaleInfo._scale = paperHeight / extHeight;
  else if (extHeight == 0)
    _scaleInfo._scale = paperWidth / extWidth;
  else
    _scaleInfo._scale = odmin(paperWidth / extWidth, paperHeight / extHeight);

  // HACK: make margins
  _scaleInfo._scale *= 0.95;
  centerPlot(paperWidth, paperHeight, extHeight, extWidth);
}

void DwfExtentManager::centerPlot(double paperWidth, double paperHeight, int extHeight, int extWidth)
{
  if (_extent.minpt().m_x < 0 || _extent.maxpt().m_x > (paperWidth / _scaleInfo._scale))
  {
    _scaleInfo._xOffset = -_extent.minpt().m_x + (paperWidth / _scaleInfo._scale - extWidth) / 2;
    _useUnits = false;
  }
  if (_extent.minpt().m_y < 0 || _extent.maxpt().m_y > (paperHeight / _scaleInfo._scale))
  {
    _useUnits = false;
    _scaleInfo._yOffset = -_extent.minpt().m_y + (paperHeight / _scaleInfo._scale - extHeight) / 2;
  }
}

void DwfLineManager::addPolyline(WT_Point_Set& pl, WT_File& file)
{
  if (pl.count() == 0)
    return;
  DwfExtentManager& mngExtents = _importer->extent();
  if (pl.count() == 1)
  {
    OdDbLinePtr l = OdDbLine::createObject();
    OdGePoint3d p = mngExtents.transformPoint(pl.points()[0]);
    l->setStartPoint(p);
    l->setEndPoint(p);
    _importer->blocks().addEntity(l, file);
  }
  else if (pl.count() == 2)
  {
    OdDbLinePtr l = OdDbLine::createObject();
    OdGePoint3d p1 = mngExtents.transformPoint(pl.points()[0]);
    OdGePoint3d p2 = mngExtents.transformPoint(pl.points()[1]);
    l->setStartPoint(p1);
    l->setEndPoint(p2);
    _importer->blocks().addEntity(l, file);
  }
  else 
  {
    //ODA_ASSERT_VAR(static bool s_bFirst = true; if (s_bFirst))
    //  ODA_ASSERT_VAR(s_bFirst = false;)

    OdDbPolylinePtr p2dPl = OdDbPolyline::createObject();
    _importer->blocks().addEntity(p2dPl, file);
    for (int i = 0; i < pl.count(); i++)
    {
      OdGePoint3d p = mngExtents.transformPoint(pl.points()[i]);
      p2dPl->addVertexAt(i, OdGePoint2d(p.x, p.y));
    }
  }
}

void DwfImporter::cleanupW2D()
{
  _ignoreMetadata = true;
  _extent.clear();
  _fonts.clear();
  _blocks.clear();
  _images.clear();
}

void DwfExtentManager::clear()
{
  _extent.m_min.m_x = 0x7fffffff;
  _extent.m_min.m_y = 0x7fffffff;
  _extent.m_max.m_x = 0x80000000;
  _extent.m_max.m_y = 0x80000000;
  _scaleInfo._scale = 1;
  _scaleInfo._xOffset = 0;
  _scaleInfo._yOffset = 0;
  _collectBounds = true;
  // invalid case - to test uninitialized (in dwf, paper may be in in's or mm's only)
  _paperInfo._units = OdDbPlotSettings::kPixels;
  _paperInfo._isMissing = true;
  _clipRect = WT_Logical_Box(0,0,0,0);
  _useUnits = false;
}

void DwfFontManager::clear()
{
  _currentTextStyle = -1;
}

void DwfExtentManager::setPaperInfo(double wPaper, double hPaper, const OdGePoint3d& ptCenter, OdDbPlotSettings::PlotPaperUnits units,
                                    bool bKeepIsMissing) // = false
{
  _paperInfo._width = wPaper;
  _paperInfo._height = hPaper;
  _paperInfo._units = units;
  ODA_ASSERT_ONCE_X(TDWF, units != OdDbPlotSettings::kPixels);
  if (!bKeepIsMissing)
    _paperInfo._isMissing = false;

  ODA_ASSERT_ONCE_X(TDWF, OdZero(ptCenter.x - wPaper / 2.0, 1e-2) && OdZero(ptCenter.y - hPaper / 2.0, 1e-3)); // CORE-12345 TODO noq _paperInfo._center is used as flag only
  _paperInfo._center = ptCenter;
  adjustParerUnits(_paperInfo._units, _paperInfo._width, _paperInfo._height, &_paperInfo._center);
}

void DwfBlockManager::clear()
{
  _currentBlock = 0;
  _group = 0;
  _groups.clear();
}

void DwfImageManager::clear()
{
  _imageMap.clear();
}

void DwfBlockManager::setCurrentLayout(OdDbObjectId layoutId)
{
  ODA_ASSERT(!layoutId.isNull());
  _importer->database()->setCurrentLayout(layoutId);
  _layout = layoutId;
}
OdDbLayoutPtr DwfBlockManager::currentLayout(OdDb::OpenMode mode)
{
  return OdDbLayoutPtr(_layout.safeOpenObject(mode));
}
}
