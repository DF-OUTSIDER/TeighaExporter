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

#ifndef _DGN_IMPORTCONTEXT_INCLUDED_ 
#define _DGN_IMPORTCONTEXT_INCLUDED_

#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgElementId.h>
#include <DgHostAppServices.h>
#include <DgModel.h>
#include <DgTable.h>
#include <DgLevelMask.h>
#include <DgView.h>
#include "DgAttributeLinkage.h"
#include "DgnImportImpl.h"
#include "DbHatch.h"

class OdDgDatabase;
class OdDbDatabase;
class OdDgLevelTableRecord;
class OdDbLayerTableRecord;
class OdDgElement;
class OdDgLineStyleResource;
class OdDgModel;


/** \details
  <group OdImport_Classes> 
*/
namespace TD_DGN_IMPORT {

class DgnImporter;

// Color map to convert dgn color index to dwg color.
struct OdDgnImportColorIndexDescription
{
  OdUInt8    m_uDwgColorIndex;
  ODCOLORREF m_clrDwgTrueColor;
  bool       m_bUseTrueColor;
};

// Dgn multiline style descriptor

struct OdDgnImportMultilineDescriptor
{
  bool              m_bShowJoints;
  bool              m_bShowStartCapLine;
  bool              m_bShowStartCapInArc;
  bool              m_bShowStartCapOutArc;
  bool              m_bShowEndCapLine;
  bool              m_bShowEndCapInArc;
  bool              m_bShowEndCapOutArc;
  bool              m_bShowFill;
  bool              m_bTableMLineStyle;
  bool              m_bCenterIsZero;
  double            m_dStartCapAngle;
  double            m_dEndCapAngle;
  OdUInt32          m_uFillColor;
  OdUInt32          m_uJustification;
  OdUInt64          m_uMLineStyle;
  OdArray<double>   m_arrOffsets;
  OdArray<OdUInt32> m_arrColorIndexes;
  OdArray<OdUInt32> m_arrLineStyles;
  OdString          m_strStyleName;
};

// Dwg dimension associativity descriptor

struct OdDgnImportDimAssocDescriptor
{
  OdDgnImportDimAssocDescriptor()
  {
    bRadialDiamDimension = false;
  }

  OdDgAssocPointRootPtr point1Desc;
  OdDgAssocPointRootPtr point2Desc;
  OdDgAssocPointRootPtr point3Desc;
  OdDgAssocPointRootPtr point4Desc;
  bool                  bRadialDiamDimension;
};

// Dwg symbol hatch descriptor

struct OdDgnImportSymbolHatchDescriptor
{
  OdHatchPattern m_hatchPattern;
  double         m_dHatchScale;
  OdUInt32       m_uColorIndex;
  double         m_dPatternRowSpacing;
  double         m_dPatternColSpacing;
};

//

class OdDgnImportContext
{
public:
  // Color transform
  static void pushPalette();
  static void popPalette();
  static void setPalette( OdDgDatabase* pDb );
  static void setPalette( const ODCOLORREF* pPalette );
  static void setBackgroundColor( ODCOLORREF bgColor );
  static bool getDwgColor( OdUInt32 uDgnColorIndex, OdDgnImportColorIndexDescription& retColor );
  static bool getConvertColorIndexToTrueColorFlag();
  static void setConvertColorIndexToTrueColorFlag( bool bSet );
  // Reference attachment map
  static void addDwgModelBlockId( const OdString& strFileName, const OdDgElementId& idModel, 
                                  const OdDbObjectId& idBlockDefinition );
  static OdDbObjectId getDwgModelBlockId( const OdString& strFileName, const OdDgElementId& idModel, const OdString& strLevelMaskExt );
  static OdUInt32 getXRefNestedDepth();
  static void     pushXRef( const OdString& strFileName, const OdDgElementId& idModel, 
                            const OdDgElementId& idXRef, OdUInt32 uNestedDepth, const OdString& strLevelMaskExt );
  static void     popXRef();
  static bool     isXRefInStack( const OdString& strFileName, const OdDgElementId& idModel );
  static OdDgElementId getXRefId( OdUInt32 uIndex );
  static bool     checkNestedDepth();
  // Global line style scales
  static bool getGlobalLineScale( double& dLineScale );
  static void pushGlobalLineScale( double dLineScale );
  static void popGlobalLineScale();
  // Hatches
  static bool getImportCurvesOnlyFlag();
  static void setImportCurvesOnlyFlag( bool bSet );
  static bool getSkipPatternDrawFlag();
  static void setSkipPatternDrawFlag( bool bSet );
  // Text styles
  static bool isFontsEqual( const OdFont* pFont1, const OdFont* pFont2 );
  static OdDbObjectId createStyleForDbText( OdDbDatabase* pDb, OdDgDatabase* pDgDb, const OdGiTextStyle& textStyle);
  static OdDbObjectId getStyleForDbText(OdDbDatabase* pDb, const OdGiTextStyle& textStyle);
  static bool getConvertEmptyDataFieldsToSpacesFlag();
  static void setConvertEmptyDataFieldsToSpacesFlag( bool bSet );
  // Id Map
  static void addCellHeaderIdToPath( const OdDbObjectId& idCellHeader );
  static void removeLastCellHeaderIdFromPath();
  static OdDbObjectId getObjectId( const OdDgElementId& idDgnElement );
  static bool getObjectPath( const OdDgElementId& idDgnElement, OdDgnImportPathToDwgObject& dwgPath );
  static void addObjectPath( const OdDgElementId& idDgnElement, OdDgnImportPathToDwgObject& dwgPath );
  static void addResourceId( const OdDbObjectId& idResource );
  static void setResourceUsage( const OdDbObjectId& idResource );
  // Dimensions
  static bool isNoteDimensionCell( const OdDgElementId& idCellHeader );
  static void addNoteDimensionCell( const OdDgElementId& idCellHeader );
  static void removeNoteDimensionCell( const OdDgElementId& idCellHeader );
  //
  static void         restoreAfterImport();
  static void         prepareToImport( DgnImporter* dgnImporter, bool bCopyLevelMasks = false );
  static DgnImporter* getDgnImporter();
  static bool         getInternalImportFlag();
  static void         setInternalImportFlag( bool bSet );
  // Level Masks
  static bool isLevelMask();
  static bool checkLevelMask( OdUInt32 uLevelEntryId );
  static OdString getLevelMaskString();
  static void pushLevelMask( const OdDgLevelMaskPtr& pLevelMask, const OdDgLevelTablePtr& pLevelTable );
  static void popLevelMask();
  // Active View
  static OdDgViewPtr getActiveView();
  static void        setActiveView( const OdDgViewPtr& pView );
  // Import invisible elements
  static bool isImportInvisible();
  static void setImportInvisible( bool bSet );
  // Fast access to polygon import modes
  static OdUInt8 getShape3dImportMode();
  static OdUInt8 getShape2dImportMode();
  static OdUInt8 getObject3dImportMode();
  static OdUInt8 getEllipse3dImportMode();
  static OdUInt8 getEllipse2dImportMode();
  static OdUInt8 getClosedBSplineCurve3dImportMode();
  static OdUInt8 getClosedBSplineCurve2dImportMode();
  static void    setCurvesOnlyImportMode();
  static void    setUpdateCurvesImportMode();
  // Rsc to Shx conversion
  static OdUInt32 getRscFontCount();
  static OdString getRscFontName( OdUInt32 uIndex );
  static void     addRscFontName( const OdString& strName );
  static void     createShxFontFromRscFont( const OdString& strFontName, OdDgDatabase* pDgDb, OdDbHostAppServices* pHostAppServices, const OdString& strShxPath );
  static void     clearRscFontArray();
  // Import multilines
  static OdDbObjectId createMultilineStyle( const OdDgnImportMultilineDescriptor& desc, OdDbDatabase* pDb, OdDgDatabase* pDg );
  static OdDbObjectId getMultilineStyle( const OdDgnImportMultilineDescriptor& desc );
  static void         clearMultilineStyles();
  // Dimension association points
  static void     convertAccuracyToDwgFormat( OdDgDimTextFormat::Accuracy dgnAccur, OdUInt8& uDwgUnitsType, OdUInt8& uPresission );
  static OdUInt32 getDimAssocDescriptorCount();
  static void     addDimAssocDescriptor( const OdDbObjectId& idDwgDim, const OdDgnImportDimAssocDescriptor& dimAssocDesc );
  static void     getDimAssocDescriptor( OdUInt32 uIndex, OdDbObjectId& idDwgDim, OdDgnImportDimAssocDescriptor& dimAssocDesc );
  static void     clearDimAssocDescriptors();
  // Symbol Hatch
  static void     addSymbolHatchDesc( const OdDgElementId& idSymbol, const OdDgnImportSymbolHatchDescriptor& hatchDesc );
  static bool     getSymbolHatchDesc( const OdDgElementId& idSymbol, double dRowSpacing, double dColSpacing, OdDgnImportSymbolHatchDescriptor& hatchDesc );
  static void     clearSymbolHatchDesc();
  // Cell block index
  static void     setCellBlockNextIndex( const OdString& strCellName, OdUInt32 uIndex );
  static bool     getCellBlockNextIndex( const OdString& strCellName,  OdUInt32& uIndex );
};

}
#endif // _DGN_IMPORTCONTEXT_INCLUDED_
