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

#ifndef _DwfImageManager_Included
#define _DwfImageManager_Included

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT{

class DwfImporter;

/** \details
  This class implements the DWF image management.
*/
class DwfImageManager	
{
  DwfImporter* _importer;
  // mapping: DWF image id -> OdDbRasterImageDef id
  typedef std::map<int, OdDbObjectId> ImageMap;
  ImageMap _imageMap;
public:	
  DwfImageManager(DwfImporter* importer) : _importer(importer){}
    // add new image entity at the given location
    void addImage(const unsigned char* data, int dataSize, int cols, int rows, 
    int format, const WT_Color_Map*, const WT_Logical_Point& ll, const WT_Logical_Point& ur, int id, WT_File& file);
  void clear();
};
}
#endif
