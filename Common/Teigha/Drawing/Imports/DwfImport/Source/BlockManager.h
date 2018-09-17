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

#ifndef _DwfBlockManager_Included_
#define _DwfBlockManager_Included_

#include "DbLayout.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {

class DwfImporter;

/** \details
  This class implements the block, group, layer management.
*/
class DwfBlockManager
{
  DwfImporter* _importer;
  // current group (WT_Object_Node block = OdDbGroup)
  OdDbGroupPtr _group;
  // mapping: WT_Object_Node id -> OdDbGroup
  typedef std::map<int, OdDbGroupPtr> GroupMap;
  GroupMap _groups;
  // current block, where data is inserted
  OdDbBlockTableRecordPtr _currentBlock;
  // mapping: DWF layer number -> DWG layer
  typedef std::map<int, OdDbObjectId> LayerMap;
  LayerMap _layerMap;
  OdDbObjectId _layout;
public:
  bool m_bPreserveColorIndices;
  std::map<ODCOLORREF, OdInt16> m_pDwgPalette;
  //
  DwfBlockManager( DwfImporter* importer );
  // set current group (add new if necessary) 
  void setCurrentGroup(int id, const OdChar* name = 0);
  // current block, where data is inserted
  OdDbBlockTableRecordPtr currentBlock()
  {
    return _currentBlock;
  }
  void setCurrentBlock(OdDbObjectId id)
  {
    _currentBlock = id.safeOpenObject(OdDb::kForWrite);
  }
  void setCurrentBlock(OdDbBlockTableRecordPtr block)
  {
    _currentBlock = block;
  }
  // maintain mapping WT_Layers to DWG layers && set CLAYER (adding new if necessary)
  void setCurrentLayer(WT_Layer& layer);
  // add entity to the current block (and possibly, group)
  void addEntity(OdDbEntity* ent, WT_File& file);
  // create hatch entity from current rendition settings
  OdDbHatchPtr addHatch(WT_File& file);
  // Add viewport to the current layout
  OdDbObjectId addViewport();
  //
  void clear();
  void setCurrentLayout( OdDbObjectId layoutId );
  OdDbLayoutPtr currentLayout(OdDb::OpenMode mode = OdDb::kForRead);
};
}
#endif
