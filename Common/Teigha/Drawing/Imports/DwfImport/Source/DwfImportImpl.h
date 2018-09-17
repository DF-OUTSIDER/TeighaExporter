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

#ifndef _DWF_7_IMPORTER_INCLUDED_
#define _DWF_7_IMPORTER_INCLUDED_

#include "../Include/DwfImport.h"
#include "DbBlockTableRecord.h"
#include "DbLinetypeTableRecord.h"
#include "DbGroup.h"
#include "DbHatch.h"
#include "DbPlotSettings.h"
#include "DbTextStyleTableRecord.h"
#include "DbDatabase.h"
#define STL_USING_MAP
#define STL_USING_SET
#define STL_USING_MEMORY
#include "OdaSTL.h"
#include "Ge/GeMatrix3d.h"
#include "Gi/GiMaterialTraitsData.h"

// MKU 07/14/05 - for converting to DWF Toolkit v7.0
//

#include "dwfcore/Core.h"
#include "dwf/Toolkit.h"
#include "whiptk/whip_toolkit.h"

//This causes strange compiler errors using gcc, so use forward declaration instead
//#include "dwf/package/EModelSection.h"

/*!DOM*/
namespace DWFToolkit { class DWFEModelSection; class DWFPackageReader;class DWFSection;}

#include "w3dtk/BStream.h"
#include "w3dtk/BOpcodeHandler.h"


#include "ExtentManager.h"
#include "FontManager.h"
#include "BlockManager.h"
#include "ImageManager.h"
#include "LineManager.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {
class DwfProperties;
class DWFImportProgressMeter;
typedef OdSmartPtr<DwfProperties> DwfPropertiesPtr;

/** \details
  This class implements the upper level import manager.
*/
class DwfImporter : public OdDwfImport
{
	// database to fill
	OdDbDatabasePtr _db;
  // 
  DwfPropertiesPtr _properties;
  OdArray<OdString> _usedLayoutNames;
  bool _xps;
  OdDbObjectId _lastMaterialId;
  OdGiMaterialTraitsTaker _lastGiData;
public:
  int _idxNextMaterial;
  std::map<OdString, OdGiMaterialTexturePtr> m_textures; // for W3D only
  DwfImporter();
  OdDbDatabase* database(){return _db;}
  OdRxDictionaryPtr properties();
  ImportResult import();
  std::map<OdString, OdString> _embeddedFontMap;
private:
	
	// load DWF (ver >= 600)
  // return values described in "Dwfimport.h"
	ImportResult loadPackage( DWFInputStream& stream, const OdChar* password);
  ImportResult getPrimaryUnits(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection);
  ImportResult load2dResource(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName, double dPaperWidth, double dPaperHeight, OdDbPlotSettings::PlotPaperUnits units, DWFImportProgressMeter pm, int layoutCount, bool bForExtents);
  ImportResult load2dResourceXps(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName, double dPaperWidth, double dPaperHeight, OdDbPlotSettings::PlotPaperUnits units, DWFImportProgressMeter pm, int layoutCount, bool bForExtents);
  ImportResult loadW3d(DWFToolkit::DWFEModelSection* pSection, DWFImportProgressMeter& pm, int layoutCount );
  void loadRasterResource(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection, const wchar_t* pRoleName);
  void loadFontResources(DWFToolkit::DWFPackageReader* pReader, DWFToolkit::DWFSection* pSection);
  // loads old style DWF (ver<=55) || new DFW .w2d section 
	// path = path to old DWF || materialized w2d
  // return values described in "Dwfimport.h"
	ImportResult loadStream( DWFInputStream* stream, WT_File& wtFile);
  // cleanup all page specific stuff (text styles etc.)
  void cleanupW2D();

  /////////////////////////////////////////////////////
  // import helpers, state flags & variables
  /////////////////////////////////////////////////////
private:
  DwfExtentManager _extent;
  DwfBlockManager _blocks;
  DwfFontManager _fonts;
  DwfImageManager	_images;
  DwfLineManager _lines;
public: 
  // Extent, scale & paper management
  DwfExtentManager& extent(){return _extent;}
  // Block, group & layer management
  DwfBlockManager& blocks(){return _blocks;}
  // font & text management
  DwfFontManager& fonts(){return _fonts;}
  // Image management
  DwfImageManager& images(){return _images;}
  // Line management
  DwfLineManager& lines(){return _lines;}
  // flag: ignore metadata while reading w2d if version >= 600
	bool _ignoreMetadata;
  bool isXPS() const { return _xps; }
  OdString currentLayoutName() const { return _usedLayoutNames.size() ? _usedLayoutNames.last() : OdString::kEmpty; }

  struct SegmentAttributes
  {
    SegmentAttributes* _parent;
    OdGeMatrix3d _matrix;
    double font_size;
    // CORE-10297 CORE-10376
    OdGiMaterialTraitsData _dataMaterial;
    mutable OdDbObjectId _idMaterial; // via all m_dataMaterial
    //
    OdCmEntityColor _color;
    int _colorGeometryMask;
    int _visibilityMask;
    int _visibility;
    OdDbObjectIdArray objects;
    OdString _name;
    OdDbObjectId _block;
    int _anonymousSegments;
    SegmentAttributes(const OdChar* name = 0, SegmentAttributes* parent = 0);
  };
  std::map<OdString,SegmentAttributes> _segments;
  SegmentAttributes* _currentSegment;
  OdResult add3dObject(OdDbEntity* ent, TKO_Geometry_Bits mask);
  OdDbObjectId checkMaterialId();
};

/*!DOM*/
namespace dwfImp
{
  // generate name, not in dictionary / table
  template<class T> OdString generateName(const OdChar* begin, const T& table, int& start_no)
  {
    OdChar name[20];
    for (;;) {
      odSprintf(name, 20, L"%ls%d", begin, start_no++);
      if (!table->has(name)) break;
    }
    return OdString(name);
  }
  // convert unicode & codepage as necessary
  OdString WTS2ODS(const WT_String& s);
  // replace with '_' any character forbidden for named object name
  OdString normalizeObjectName(const WT_String& name);
  // convert Dwf color to colorref
  OdUInt32 exchangeRBColorComponents(OdUInt32 inputColor);
}
}

#endif // _DWF_7_IMPORTER_INCLUDED_
