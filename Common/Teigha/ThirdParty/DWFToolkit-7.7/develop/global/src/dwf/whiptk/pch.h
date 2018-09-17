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
///\file        dwf/whiptk/pch.h
///

#ifndef _WHIPTK_PCH_H
#define _WHIPTK_PCH_H

#include <stdio.h>
#include <math.h>

#include "whiptk/allclass.h"
#include "whiptk/whipcore.h"
#include "whiptk/wendian.h"
#include "whiptk/wtstring.h"
#include "whiptk/matrix.h"

#include "whiptk/list.h"
#include "whiptk/url_list.h"

#include "whiptk/compdata.h"
#include "whiptk/lz77comp.h"
#include "whiptk/lz77decp.h"
#include "whiptk/lzdefs.h"
#include "whiptk/fifo.h"
#include "whiptk/object.h"
#include "whiptk/attribute.h"

#include "whiptk/informational.h"
#include "whiptk/timestamp.h"

#include "whiptk/code_page.h"
#include "whiptk/color.h"
#include "whiptk/colormap.h"
#include "whiptk/contour_set.h"
#include "whiptk/dashpat.h"
#include "whiptk/backgrnd.h"
#include "whiptk/embed.h"
#include "whiptk/embedded_font.h"
#include "whiptk/fill.h"
#include "whiptk/fillpat.h"
#include "whiptk/font_options.h"
#include "whiptk/font.h"
#include "whiptk/linepat.h"
#include "whiptk/linestyle.h"
#include "whiptk/lweight.h"

#include "whiptk/projection.h"
#include "whiptk/url.h"
#include "whiptk/view.h"
#include "whiptk/viewport_options.h"
#include "whiptk/viewport.h"
#include "whiptk/visible.h"
#include "whiptk/layer.h"
#include "whiptk/layer_list.h"
#include "whiptk/inked_area.h"
#include "whiptk/named_view_list.h"
#include "whiptk/named_view.h"
#include "whiptk/trusted_font_list.h"
#include "whiptk/units.h"

#include "whiptk/rendition.h"

#include "whiptk/drawable.h"

#include "whiptk/pointset.h"
#include "whiptk/gouraud_pointset.h"

#include "whiptk/ellipse.h"
#include "whiptk/image.h"
#include "whiptk/pnggroup4image.h"
#include "whiptk/gouraud_polyline.h"
#include "whiptk/gouraud_polytri.h"
#include "whiptk/origin.h"
#include "whiptk/polyline.h"
#include "whiptk/polytri.h"
#include "whiptk/polygon.h"
#include "whiptk/polymark.h"
#include "whiptk/text.h"
#include "whiptk/merge_control.h"
#include "whiptk/plot_optimized.h"
#include "whiptk/object_node.h"
#include "whiptk/object_node_list.h"
#include "whiptk/group_begin.h"
#include "whiptk/group_end.h"
#include "whiptk/blockref_defs.h"
#include "whiptk/blockref.h"
#include "whiptk/directory.h"
#include "whiptk/filetime.h"

#include "whiptk/dwfhead.h"
#include "whiptk/endofdwf.h"
#include "whiptk/unknown.h"

#include "whiptk/heuristics.h"
#include "whiptk/file.h"
#include "whiptk/guid_list.h"
#include "whiptk/macro_definition.h"
#include "whiptk/macro_draw.h"
#include "whiptk/macro_scale.h"
#include "whiptk/macro_index.h"
#include "whiptk/opcode.h"
#include "whiptk/userdata.h"
#include "whiptk/penpat.h"
#include "whiptk/signdata.h"
#include "whiptk/blockref_list.h"
#include "whiptk/font_extension.h"
#include "whiptk/penpat_options.h"

#include "whiptk/object_stream.h"

#include "whiptk/text_halign.h"
#include "whiptk/text_valign.h"
#include "whiptk/text_background.h"

#include "whiptk/delineate.h"
#include "whiptk/overpost.h"
#include "whiptk/usrhatchpat.h"
#include "whiptk/fileext.h"

#ifdef ANDROID_WCHAR
#include <wchar_.h>
#define Dwf_wcslen Citrus::wcslen
#else
#define Dwf_wcslen ::wcslen
#endif

#endif

