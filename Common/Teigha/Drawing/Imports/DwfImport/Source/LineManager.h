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

#ifndef _DwfLineManager_Included_
#define _DwfLineManager_Included_

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {
class DwfImporter;
/////////////////////////////////////////////////////
// Line management
//

/** \details
  This class implements the DWF line manager.
*/
class DwfLineManager
{
  DwfImporter* _importer;
  // mapping: DWF line style -> DWG line style
  typedef std::map<int, OdDbObjectId> LineStyleMap;
  LineStyleMap _lineStyles;
  static void setLinePattern(OdDbLinetypeTableRecord* pLt, const short* pattern, int length, double scale);
  static void setPredefinedPattern(OdDbLinetypeTableRecord* pLt, int patternId, double scale);
public:	
  DwfLineManager(DwfImporter* importer) : _importer(importer){}
  // get current line type & update _lineStyles mapping
  OdDbObjectId getCurrentLineStyle(WT_File& file);
  // convert DWF lineweight to DWG lineweight
  OdDb::LineWeight getCurrentLineWeight(WT_File& file);
  // create polyline entity from point set
  void addPolyline(WT_Point_Set& pl, WT_File& file);
};
}
#endif
