//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
///
///\file        dwf/XAML/pch.h
///

#ifndef _XAML_PCH_H
#define _XAML_PCH_H

//Win2K or above
#ifdef WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500
#endif
#include "whiptk/whip_toolkit.h"
#include "whiptk/pch.h"

#include "XAML/allclass.h"
#include "XAML/XamlAttribute_Url.h"
#include "XAML/XamlCode_Page.h"
#include "XAML/XamlColor.h"
#include "XAML/XamlColorMap.h"
#include "XAML/XamlContour_Set.h"
#include "XAML/XamlContrastColor.h"
#include "XAML/XamlDelineate.h"
#include "XAML/XamlDashPattern.h"
#include "XAML/XamlDwfHeader.h"
#include "XAML/XamlEllipse.h"
#include "XAML/XamlEmbed.h"
#include "XAML/XamlEmbeddedFont.h"
#include "XAML/XamlEndOfDwf.h"
#include "XAML/XamlFillPattern.h"
#include "XAML/XamlGouraud_Polyline.h"
#include "XAML/XamlGouraud_Polytriangle.h"
#include "XAML/XamlFont.h"
#include "XAML/XamlFontExtension.h"
#include "XAML/XamlImage.h"
#include "XAML/XamlLayer.h"
#include "XAML/XamlLinePattern.h"
#include "XAML/XamlLineStyle.h"
#include "XAML/XamlLineWeight.h"
#include "XAML/XamlMacro_Definition.h"
#include "XAML/XamlMacro_Draw.h"
#include "XAML/XamlMacro_Index.h"
#include "XAML/XamlMacro_Scale.h"
#include "XAML/XamlMerge_Control.h"
#include "XAML/XamlNamed_View.h"
#include "XAML/XamlNamed_View_List.h"
#include "XAML/XamlObject.h"
#include "XAML/XamlObject_Node.h"
#include "XAML/XamlOverpost.h"
#include "XAML/XamlPenPattern.h"
#include "XAML/XamlPNGGroup4Image.h"
#include "XAML/XamlPointset.h"
#include "XAML/XamlPolygon.h"
#include "XAML/XamlPolyline.h"
#include "XAML/XamlPolymarker.h"
#include "XAML/XamlPolytriangle.h"
#include "XAML/XamlProjection.h"
#include "XAML/XamlRendition.h"
#include "XAML/XamlText.h"
#include "XAML/XamlText_Background.h"
#include "XAML/XamlText_HAlign.h"
#include "XAML/XamlText_VAlign.h"
#include "XAML/xamlunits.h"
#include "XAML/XamlUnknown.h"
#include "XAML/XamlURL.h"
#include "XAML/XamlUserData.h"
#include "XAML/XamlUserFillPattern.h"
#include "XAML/XamlUserHatchPattern.h"
#include "XAML/XamlView.h"
#include "XAML/XamlViewport.h"
#include "XAML/XamlVisible.h"
#include "XAML/XamlDrawable.h"
#include "XAML/XamlAttribute.h"
#include "XAML/XamlGraphicsObject.h"
#include "XAML/XamlPathGeometry.h"
#include "XAML/XamlArcSegment.h"
#include "XAML/XamlPolyLineSegment.h"
#include "XAML/XamlPath.h"
#include "XAML/XamlGlyphs.h"
#include "XAML/XamlCanvas.h"
#include "XAML/XamlFile.h"
#include "XAML/XamlClassFactory.h"
#include "XAML/XamlRenditionPath.h"
#include "XAML/XamlXML.h"
#include "XAML/XamlObjectFactory.h"
#include "XAML/MemoryBuffer.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef ANDROID_WCHAR
#include <stdlib_.h>
#include <wchar_.h>
#define Dwf_atof Citrus::atof
#define Dwf_abs Citrus::abs
#define Dwf_labs Citrus::labs
#define Dwf_llabs Citrus::llabs
#define Dwf_srand Citrus::srand
#define Dwf_rand Citrus::rand
#else
#define Dwf_atof atof
#define Dwf_abs abs
#define Dwf_labs labs
#define Dwf_llabs llabs
#define Dwf_srand srand
#define Dwf_rand rand
#endif
#endif //_XAML_PCH_H

