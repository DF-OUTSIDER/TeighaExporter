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

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT{

/*!DOM*/
#define MY_CALLBACK_LIST\
  CALLBACK_MACRO(Author,author)\
  CALLBACK_MACRO(Background,background)\
  CALLBACK_MACRO(Comments,comments)\
  CALLBACK_MACRO(Copyright,copyright)\
  CALLBACK_MACRO(Creation_Time,creation_time)\
  CALLBACK_MACRO(Creator,creator)\
  CALLBACK_MACRO(Description,description)\
  CALLBACK_MACRO(Source_Creation_Time,source_creation_time)\
  CALLBACK_MACRO(Source_Filename,source_filename)\
  CALLBACK_MACRO(Source_Modification_Time,source_modification_time)\
  CALLBACK_MACRO(Title,title)\
  CALLBACK_MACRO(Modification_Time,modification_time)\
  CALLBACK_MACRO(Keywords,keywords)\
  CALLBACK_MACRO(Subject,subject)\
  CALLBACK_MACRO(Filled_Ellipse,filled_ellipse)\
  CALLBACK_MACRO(Image,image)\
  CALLBACK_MACRO(Layer,layer)\
  CALLBACK_MACRO(Named_View,named_view)\
  CALLBACK_MACRO(View,view)\
  CALLBACK_MACRO(Units,units)\
  CALLBACK_MACRO(Object_Node,object_node)\
  CALLBACK_MACRO(Outline_Ellipse,outline_ellipse)\
  CALLBACK_MACRO(PNG_Group4_Image,png_group4_image)\
  CALLBACK_MACRO(Font,font)\
  CALLBACK_MACRO(Polygon,polygon)\
  CALLBACK_MACRO(Polymarker,polymarker)\
  CALLBACK_MACRO(Polytriangle,polytriangle)\
  CALLBACK_MACRO(Text,text)\
  CALLBACK_MACRO(Viewport,viewport)\
  CALLBACK_MACRO(Gouraud_Polyline,gouraud_polyline)\
  CALLBACK_MACRO(Gouraud_Polytriangle,gouraud_polytriangle)\
  CALLBACK_MACRO(Contour_Set,contour_set)\
  CALLBACK_MACRO(Polyline,polyline)\
  CALLBACK_MACRO(Fill_Pattern,fill_pattern)\
CALLBACK_MACRO(Plot_Info,plot_info)

//////////////////////////////////////////////////////
// Import callbacks
//

/*!DOM*/
struct DwfCallbackManager
{
  static DwfImporter* importer(WT_File& file)
  {
    return reinterpret_cast<DwfImporter*>(file.heuristics().user_data());
  }
#define CALLBACK_MACRO(class_name, class_lower) \
  static WT_Result process_##class_lower(WT_##class_name &, WT_File&);
  MY_CALLBACK_LIST
#undef CALLBACK_MACRO
};
}
