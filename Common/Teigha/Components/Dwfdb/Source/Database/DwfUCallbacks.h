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

#ifndef DWFUCALLBACKS_H 
#define DWFUCALLBACKS_H 

namespace DWFToolkit
{
  class DWFFile;
};
class WT_Result;
class WT_File;
class WT_Units;
/*CALLBACK_MACRO(Alignment,alignment)\
  CALLBACK_MACRO(Attribute_URL,attribute_url)\
  CALLBACK_MACRO(Author,author)\
  CALLBACK_MACRO(Background,background)\
  CALLBACK_MACRO(Block_Meaning,block_meaning)\
  CALLBACK_MACRO(BlockRef,blockref)\
  CALLBACK_MACRO(Code_Page,code_page)\
  CALLBACK_MACRO(Color,color)\
  CALLBACK_MACRO(Color_Map,color_map)\
  CALLBACK_MACRO(Comments,comments)\
  CALLBACK_MACRO(Compressed_Data_Moniker,compressed_data_moniker)\
  CALLBACK_MACRO(Contour_Set,contour_set)\
  CALLBACK_MACRO(Contrast_Color,contrast_color)\
  CALLBACK_MACRO(Copyright,copyright)\
  CALLBACK_MACRO(Creation_Time,creation_time)\
  CALLBACK_MACRO(Creator,creator)\
  CALLBACK_MACRO(Dash_Pattern,dash_pattern)\
  CALLBACK_MACRO(Delineate,delineate)\
  CALLBACK_MACRO(Description,description)\
  CALLBACK_MACRO(Directory,directory)\
  CALLBACK_MACRO(WT_Drawing_Info,drawing_info)\
  CALLBACK_MACRO(WT_DWF_Header,dwf_header)\
  CALLBACK_MACRO(Embed,embed)\
  CALLBACK_MACRO(Embedded_Font,embedded_font)\
  CALLBACK_MACRO(Encryption,encryption)\
  CALLBACK_MACRO(End_Of_DWF,end_of_dwf)\
  CALLBACK_MACRO(FileTime,filetime)\
  CALLBACK_MACRO(Fill,fill)\
  CALLBACK_MACRO(Fill_Pattern,fill_pattern)\
  CALLBACK_MACRO(Filled_Ellipse,filled_ellipse)\
  CALLBACK_MACRO(Font,font)\
  CALLBACK_MACRO(Font_Extension,font_extension)\
  CALLBACK_MACRO(Gouraud_Polyline,gouraud_polyline)\
  CALLBACK_MACRO(Gouraud_Polytriangle,gouraud_polytriangle)\
  CALLBACK_MACRO(Group_Begin,group_begin)\
  CALLBACK_MACRO(Group_End,group_end)\
  CALLBACK_MACRO(Guid,guid)\
  CALLBACK_MACRO(Guid_List,guid_list)\
  CALLBACK_MACRO(Image,image)\
  CALLBACK_MACRO(Inked_Area,inked_area)\
  CALLBACK_MACRO(Keywords,keywords)\
  CALLBACK_MACRO(Layer,layer)\
  CALLBACK_MACRO(Line_Pattern,line_pattern)\
  CALLBACK_MACRO(Line_Style,line_style)\
  CALLBACK_MACRO(Line_Weight,line_weight)\
  CALLBACK_MACRO(Macro_Definition,macro_definition)\
  CALLBACK_MACRO(Macro_Draw,macro_draw)\
  CALLBACK_MACRO(Macro_Scale,macro_scale)\
  CALLBACK_MACRO(Macro_Index,macro_index)\
  CALLBACK_MACRO(Marker_Size,marker_size)\
  CALLBACK_MACRO(Marker_Symbol,marker_symbol)\
  CALLBACK_MACRO(Merge_Control,merge_control)\
  CALLBACK_MACRO(Modification_Time,modification_time)\
  CALLBACK_MACRO(Named_View,named_view)\
  CALLBACK_MACRO(Named_View_List,named_view_list)\
  CALLBACK_MACRO(Object_Node,object_node)\
  CALLBACK_MACRO(Orientation,orientation)\
  CALLBACK_MACRO(Origin,origin)\
  CALLBACK_MACRO(Outline_Ellipse,outline_ellipse)\
  CALLBACK_MACRO(Overpost,overpost)\
  CALLBACK_MACRO(Password,password)\
  CALLBACK_MACRO(Pen_Pattern,pen_pattern)\
  CALLBACK_MACRO(PenPat_Options,penpat_options)\
  CALLBACK_MACRO(Plot_Info,plot_info)\
  CALLBACK_MACRO(Plot_Optimized,plot_optimized)\
  CALLBACK_MACRO(PNG_Group4_Image,png_group4_image)\
  CALLBACK_MACRO(Polygon,polygon)\
  CALLBACK_MACRO(Polyline,polyline)\
  CALLBACK_MACRO(Polymarker,polymarker)\
  CALLBACK_MACRO(Polytriangle,polytriangle)\
  CALLBACK_MACRO(Projection,projection)\
  CALLBACK_MACRO(SignData,signdata)\
  CALLBACK_MACRO(Source_Creation_Time,source_creation_time)\
  CALLBACK_MACRO(Source_Filename,source_filename)\
  CALLBACK_MACRO(Source_Modification_Time,source_modification_time)\
  CALLBACK_MACRO(Subject,subject)\
  CALLBACK_MACRO(Text,text)\
  CALLBACK_MACRO(Text_Background,text_background)\
  CALLBACK_MACRO(Text_HAlign,text_halign)\
  CALLBACK_MACRO(Text_VAlign,text_valign)\
  CALLBACK_MACRO(Title,title)\
  CALLBACK_MACRO(Trusted_Font_List,trusted_font_list)\
  CALLBACK_MACRO(Units,units)\
  CALLBACK_MACRO(Unknown,unknown)\
  CALLBACK_MACRO(URL,url)\
  CALLBACK_MACRO(UserData,userdata)\
  CALLBACK_MACRO(User_Fill_Pattern,user_fill_pattern)\
  CALLBACK_MACRO(User_Hatch_Pattern,user_hatch_pattern)\
  CALLBACK_MACRO(View,view)\
  CALLBACK_MACRO(Viewport,viewport)\
  CALLBACK_MACRO(Visibility,visibility)*/
#define MY_CALLBACK_LIST_2\
  CALLBACK_MACRO(Background,background)\
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

struct DwfDbBaseCallbackManager
{
  // dwf stream loading functionality 
  static WT_Result WtStreamOpenAction(WT_File& file);
  static WT_Result WtStreamCloseAction(WT_File& file);
  static WT_Result WtStreamSeekAction(WT_File& file, int distance, int & amount_seeked);
  static WT_Result WTStreamReadAction(WT_File& file,int bytes_desired,int & bytes_read,void * buffer);
  static WT_Result WTStreamTellAction(WT_File& file, unsigned long*);
  static WT_Result WTStreamWriteAction(WT_File& file,int ,void const * );
  static WT_Result WtStreamEndSeekAction(WT_File& file);
  
#define CALLBACK_MACRO(class_name, class_lower) \
  static WT_Result process_##class_lower(WT_##class_name &, WT_File&);
  MY_CALLBACK_LIST_2
#undef CALLBACK_MACRO
};

struct DwfDbCallbackManager : DwfDbBaseCallbackManager
{
};

struct DwfDbExtentsCallbackManager //TODO : DwfDbBaseCallbackManager
{
#define CALLBACK_MACRO(class_name, class_lower) \
  static WT_Result process_##class_lower(WT_##class_name &, WT_File&);
  MY_CALLBACK_LIST_2
#undef CALLBACK_MACRO

  static void updateBounds(WT_Drawable& d, WT_File& file);
};

#define MY_CALLBACK_LIST_UNITS\
  CALLBACK_MACRO(Units,units)

struct DwfDbUnitsCallbackManager
{
  static OdDb::UnitsValue convertUnits(const WT_Units& units);

#define CALLBACK_MACRO(class_name, class_lower) \
  static WT_Result process_##class_lower(WT_##class_name &, WT_File&);
  MY_CALLBACK_LIST_UNITS
#undef CALLBACK_MACRO

  static OdDb::UnitsValue& getUnits(WT_File& file);
};

#define MY_CALLBACK_LIST_LAYERS\
  CALLBACK_MACRO(Layer,layer)

struct DwfDbLayersCallbackManager
{
  static OdDb::UnitsValue convertUnits(const WT_Units& units);

#define CALLBACK_MACRO(class_name, class_lower) \
  static WT_Result process_##class_lower(WT_##class_name &, WT_File&);
  MY_CALLBACK_LIST_LAYERS
#undef CALLBACK_MACRO
};

#endif// DWFUCALLBACKS_H 
