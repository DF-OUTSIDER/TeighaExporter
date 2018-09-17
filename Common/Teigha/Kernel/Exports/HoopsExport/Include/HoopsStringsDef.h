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


///////////////////////////////////////////////////////////////////////////////
//
// HoopsStringsDef.h - Open Design Hoops Export string definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_STRINGS_DEF_INCLUDED_
#define _HOOPS_STRINGS_DEF_INCLUDED_

/** \details
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {
#define HOOPS_FONT_OPTIONS    "encoding = unicode"
#define HOOPS_FONT_STYLE      "Defined"
#define HOOPS_FONT_SET_TTF    "renderer = truetype"
#define HOOPS_FONT_SET_DEFINED "renderer = defined"
#define HOOPS_SET_TEXT_3D     "transforms = on"
#define HOOPS_SET_TEXT_2D     "transforms = off"
#define HOOPS_TEXT_DEFAULT_ALIGN "v<"
#define HOOPS_SET_BOLD        "bold = on"
#define HOOPS_SET_ITALIC      "italic = on"
#define HOOPS_SET_UNDERLINED  "underline = on"
#define HOOPS_SET_OVERLINED   "overline = on"
#define HOOPS_UNSET_BOLD      "bold = off"
#define HOOPS_UNSET_ITALIC    "italic = off"
#define HOOPS_UNSET_UNDERLINED "underline = off"
#define HOOPS_UNSET_OVERLINED  "overline = off"
#define HOOPS_TEXT_ROTATION_90 "rotation = 90"
#define HOOPS_IMAGE_VISIBILITY_OPTIONS "markers = off, light = off, shadows = off"
#define HOOPS_IMAGE_RENDERING_OPTIONS "anti-alias = off, simple shadow = off, transparency = (style = off)"
#define HOOPS_3DOBJECT_VISIBILITY_OPTIONS "markers = off"

#define PERSPECTIVE_MODE        "perspective"
#define PERSPECTIVE_STEREO_MODE "oblique perspective"
#define PARALLEL_MODE           "orthographic"
#define PARALLEL_STEREO_MODE    "oblique orthographic"
#define HOOPS_RIGHT_CS          "right"

#define L_HOOPS_LINE_ORIENTATION L"orientation = (%f,%f,%f)"

#define L_GLYPH_NAME_FORMAT   L"glyph_%u"
#define L_LTYPE_NAME_FORMAT   L"linetype_%u"
#define L_PLINE_NAME_FORMAT   L"Line_%u"
#define L_XLINE_NAME_FORMAT   L"XLine_%u"
#define L_RAY_NAME_FORMAT     L"Ray_%u"
#define L_CIRCLE_NAME_FORMAT  L"CircleArc_%u"
#define L_ELLIPSE_NAME_FORMAT L"EllipArc_%u"
#define L_NURBS_NAME_FORMAT   L"NurbsCurve_%u"
#define L_TEXT_NAME_FORMAT    L"Text_%u"
#define L_FONT_NAME_FORMAT    L"font_%u"
#define L_POLYGON_NAME_FORMAT L"Polygon_%u"
#define L_IMAGE_NAME_FORMAT   L"Image_%u"
#define L_IMAGESHELL_NAME_FORMAT L"RasterImage_%u "
#define L_TEXTURE_NAME_FORMAT L"Texture_%u "
#define L_MESH_NAME_FORMAT    L"Mesh_%u "
#define L_SHELL_NAME_FORMAT   L"Shell_%u "
#define L_LS_NAME_FORMAT      L"LightSource_%u "
#define L_VIEWPORT_NAME_FORMAT  L"ViewPort_%u "
#define L_OBJECT_NAME_FORMAT  L"Object_%u"
#define L_RGB_IMAGE_FORMAT    L"RGB, size = ( 1 oru, 1 oru ), name = %s"
#define L_RGBA_IMAGE_FORMAT   L"RGBA, size = ( 1 oru, 1 oru ), name = %s"
#define L_RGB_SIZE_IMAGE_FORMAT L"RGB, size = ( %u oru, %u oru ), name = %s"
#define L_RGBA_SIZE_IMAGE_FORMAT L"RGBA, size = ( %u oru, %u oru ), name = %s"
#define L_RASTER_TEXTURE_OPTIONS L"parameterization source = uv, source = %s"
#define L_SET_TEXTURE_FORMAT  L"faces = (ambient = %s, diffuse texture = %s)"
#define L_SET_AMBIENT_FORMAT  L"ambient = (R = %f G = %f B = %f)"
#define L_SET_DIFFUSE_FORMAT  L"diffuse color = (R = %f G = %f B = %f)"
#define L_SET_SPECULAR_FORMAT L"specular = (R = %f G = %f B = %f)"
#define L_SET_TRANSPARENT_FORMAT L"transmission = ( R = %f G = %f B = %f )"
#define L_SET_BGCOLOR_FORMAT  L"window = ( R = %f G = %f B = %f )"
#define L_SET_GLOSS_FORMAT    L"gloss = %f"
#define L_SET_DIFFUSE_TEXTURE L"diffuse texture = %s"
#define L_SET_BUMP_TEXTURE    L"bump = %s"
#define L_SET_REFLECTION_TEXTURE L"environment = %s, mirror=grey"
#define L_SET_SPECULAR_TEXTURE L"specular = %s"
#define L_MODULATE            L"modulate"
#define L_TEXTURE_SOURCE_FORMAT     L"source = %s"
#define L_TEXTURE_NATURAL     L"parameterization source = natural"
#define L_TEXTURE_SURFACE_NORMAL L"parameterization source = surface normal"
#define L_TEXTURE_CYLINDER L"parameterization source = cylinder"
#define L_TEXTURE_SPHERE  L"parameterization source = sphere"
#define L_TEXTURE_UV  L"parameterization source = uv"
#define L_TILING_CLAMP   L"tiling = clamp"
#define L_TILING_MIRROR  L"tiling = mirror"
#define L_TILING_REPEAT  L"tiling = repeat"
#define L_SET_POINTLIGHT_FORMAT L"lighting = ( R = %f G = %f B = %f )"
#define L_SPOT_LIGHT_FORMAT L"outer cone = %f degrees, inner cone = %f degrees"

#define L_FONT_ANGLE_FORMAT   L"slant = %f"
#define L_FONT_WIDTH_SCALE    L"width scale = %f"
#define L_FONT_SIZE_FORMAT    L"size = %f oru"
#define L_SET_FONT_NAME_FORMAT L"name = %s"
#define L_SET_FONT_DIR_FORMAT  L"font directory = %s"
#define HOOPS_WCS_TEXT_MODE   "wcs"
#define HOOPS_WORLD_CLIPPING  "world"
#define HOOPS_SHOW_GEOMETRY   "geometry = on"
#define HOOPS_LIGHT_ON        "lights = on"
#define HOOPS_LIGHT_OFF       "lights = off"
#define HOOPS_HIDE_GEOMETRY   "geometry = off"
#define HOOPS_EXCLUDE_BOUNDING "exclude bounding = on"

#define HOOPS_COLORSPACE    "RGB"
#define LINE_COLOR_TYPE     "lines"
#define FACE_COLOR_TYPE     "faces"
#define EDGE_COLOR_TYPE     "edges"
#define ALL_COLOR_TYPE      "geometry"
#define TEXT_COLOR_TYPE     "text"
#define VERTICES_COLOR_TYPE "vertices"

#define HOOPS_SHOW_EDGES    "edges = on"
#define HOOPS_HIDE_EDGES    "edges = off"
#define HOOPS_SHOW_FACES    "faces = on"
#define HOOPS_HIDE_FACES    "faces = off"

#define L_DASH_LINETYPE_ITEM  L"dash"
#define L_BLANK_LINETYPE_ITEM L"blank"
#define L_POINT_LINETYPE_ITEM L"1 pixel dash"
#define L_GLYPH_LINETYPE_ITEM L"%f oru %s, transformed"
#define L_FIXED_GLYPH_LINETYPE_ITEM L"%f oru %s, fixed"
#define L_3D_LINETYPE_ITEM L",transformed"
#define L_HOOPS_SOLID_LINE    L"---" 

#define DRAWING_SEGMENT  "?Picture"
#define HOOPS_IMAGE_SEGMENT "images"
#define L_DRAWING_SEGMENT  L"?Picture"
}
//////////////////////////////////////////////////////////////////////////

#endif // _HOOPS_STRINGS_DEF_INCLUDED_

