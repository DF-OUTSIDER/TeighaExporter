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
///\file        dwf/whiptk/opcode_defs.h
///

#if !defined OPCODE_DEFS_HEADER
#define OPCODE_DEFS_HEADER

#define WD_MAX_OPCODE_TOKEN_SIZE         40
#define WD_EXTENDED_BINARY_OPCODE_SIZE    2
#define WD_EXTENDED_BINARY_OFFSET_SIZE    4

#define WD_MAX_DWF_COUNT_VALUE           (256 + 65535)

#define WD_COLOR_MAP_EXT_OPCODE                 0x0001

#define WD_IMAGE_BITONAL_MAPPED_EXT_OPCODE      0x0002
#define WD_IMAGE_GROUP3X_MAPPED_EXT_OPCODE      0x0003
#define WD_IMAGE_INDEXED_EXT_OPCODE             0x0004
#define WD_IMAGE_MAPPED_EXT_OPCODE              0x0005
#define WD_IMAGE_RGB_EXT_OPCODE                 0x0006
#define WD_IMAGE_RGBA_EXT_OPCODE                0x0007
#define WD_IMAGE_JPEG_EXT_OPCODE                0x0008
#define WD_IMAGE_GROUP4_BITONAL_EXT_OPCODE      0x0009
#define WD_IMAGE_PNG_EXT_OPCODE                 0x000C
#define WD_IMAGE_GROUP4X_MAPPED_EXT_OPCODE      0x000D

#define WD_LZ_COMPRESSION_EXT_OPCODE            0x0010
#define WD_ZLIB_COMPRESSION_EXT_OPCODE          0x0011

#define WD_GRAPHIC_HDR_EXT_OPCODE               0x0012
#define WD_OVERLAY_HDR_EXT_OPCODE               0x0013
#define WD_REDLINE_HDR_EXT_OPCODE               0x0014
#define WD_THUMBNAIL_EXT_OPCODE                 0x0015
#define WD_PREVIEW_EXT_OPCODE                   0x0016
#define WD_OVERLAY_THUMBNAIL_EXT_OPCODE         0x0017
#define WD_OVERLAY_PREVIEW_EXT_OPCODE           0x0018
#define WD_FONT_EXT_OPCODE                      0x0019
#define WD_GRAPHICS_EXT_OPCODE                  0x0020
#define WD_OVERLAY_EXT_OPCODE                   0x0021
#define WD_REDLINE_EXT_OPCODE                   0x0022
#define WD_USER_EXT_OPCODE                      0x0023
#define WD_NULL_EXT_OPCODE                      0x0024
#define WD_GLOBAL_SHEET_EXT_OPCODE              0x0025
#define WD_GLOBAL_EXT_OPCODE                    0x0026
#define WD_SIGNATURE_EXT_OPCODE                 0x0027

#define WD_LZ_COMPRESSION_EXT_OPCODE_OBSOLETE   0x0123

#define WD_URL_Optimize                         -255

// -----------------

#define WD_SBBO_BEZIER_16R                      0x02 // Ctrl-B
#define WD_SBBO_SET_COLOR_RGBA                  0x03 // Ctrl-C
#define WD_SBBO_SET_FONT                        0x06 // Ctrl-F
#define WD_SBBO_DRAW_GOURAUD_POLYTRIANGLE_16R   0x07 // Ctrl-G
#define WD_SBBO_DRAW_CONTOUR_SET_16R            0x0B // Ctrl-K
#define WD_SBBO_DRAW_LINE_16R                   0x0C // Ctrl-L
#define WD_SBBO_OBJECT_NODE_AUTO                0x0E // Ctrl-N
#define WD_SBBO_DRAW_POLYLINE_POLYGON_16R       0x10 // Ctrl-P
#define WD_SBBO_DRAW_GOURAUD_POLYLINE_16R       0x11 // Ctrl-Q
#define WD_SBBO_DRAW_CIRCLE_16R                 0x12 // Ctrl-R
#define WD_SBBO_DRAW_POLYTRIANGLE_16R           0x14 // Ctrl-T
#define WD_SBBO_SET_LINE_WEIGHT                 0x17 // Ctrl-W
#define WD_SBBO_DRAW_TEXT_COMPLEX               0x18 // Ctrl-X

#define WD_SBAO_BEZIER_32R                      'B'  // 0x42
#define WD_SBAO_SET_COLOR_INDEXED               'C'  // 0x43
#define WD_SBAO_DRAW_ELLIPSE                    'E'  // 0x45
#define WD_SBAO_SET_FILL_ON                     'F'  // 0x46
#define WD_SBAO_SET_MACRO_INDEX                 'G'  // 0x47  This replaces Marker_Symbol in file versions => 601
#define WD_SBAO_SET_MARKER_SYMBOL               'G'  // 0x47  This is used only in file where version < 601
#define WD_SBAO_DRAW_LINE                       'L'  // 0x4C
#define WD_SBAO_DRAW_MACRO_DRAW                 'M'  // 0x4D  // Used in file versions greater then 6.01 
#define WD_SBAO_DRAW_POLYMARKER                 'M'  // 0x4D  // Deprecated 
#define WD_SBBO_OBJECT_NODE_32                  'N'  // 0x4E
#define WD_SBBO_SET_ORIGIN_32                   'O'  // 0x4F
#define WD_SBAO_DRAW_POLYLINE_POLYGON           'P'  // 0x50
#define WD_SBAO_DRAW_CIRCLE                     'R'  // 0x52
#define WD_SBAO_SET_MACRO_SCALE                 'S'  // 0x53 
#define WD_SBA0_SET_MARKER_SIZE                 'S'  // 0x53 Deprecate : left in for compatibility  
#define WD_SBAO_DRAW_POLYTRIANGLE               'T'  // 0x54
#define WD_SBAO_SET_VISIBILITY_ON               'V'  // 0x56

#define WD_SBBO_BEZIER_32                       'b'  // 0x62
#define WD_SBBO_SET_COLOR_INDEXED               'c'  // 0x63
#define WD_SBBO_DRAW_ELLIPSE_32R                'e'  // 0x65
#define WD_SBAO_SET_FILL_OFF                    'f'  // 0x66
#define WD_SBBO_DRAW_GOURAUD_POLYTRIANGLE_32R   'g'  // 0x67
#define WD_SBBO_DRAW_CONTOUR_SET_32R            'k'  // 0x6B
#define WD_SBBO_DRAW_LINE_32R                   'l'  // 0x6C
#define WD_SBBO_DRAW_MACRO_DRAW_32R             'm'  // 0x6D  
#define WD_SBBO_DRAW_POLYMARKER_32R             'm'  // 0x6D // Deprecate : left in for compatibility  
#define WD_SBBO_OBJECT_NODE_16                  'n'  // 0x6E
#define WD_SBBO_DRAW_POLYLINE_POLYGON_32R       'p'  // 0x70
#define WD_SBBO_DRAW_GOURAUD_POLYLINE_32R       'q'  // 0x71
#define WD_SBBO_DRAW_CIRCLE_32R                 'r'  // 0x72
#define WD_SBBO_SET_MACRO_SCALE                 's'  // 0x73 
#define WD_SBBO_SET_MARKER_SIZE                 's'  // 0x73 // Deprecate : left in for compatibility  
#define WD_SBBO_DRAW_POLYTRIANGLE_32R           't'  // 0x74
#define WD_SBAO_SET_VISIBILITY_OFF              'v'  // 0x76
#define WD_SBBO_DRAW_TEXT_BASIC                 'x'  // 0x78

#define WD_SBBO_DRAW_MACRO_DRAW_16R             0x8D // This SHOULD have been 0x0D, but we are stuck now with 0x8D 
#define WD_SBBO_DRAW_POLYMARKER_16R             0x8D // Deprecate : left in for compatibility  
#define WD_SBBO_DRAW_CIRCULAR_ARC_32R           0x92 // XTENDED-Ctrl-R
#define WD_SBBO_SET_LAYER                       0xAC // High-order-bit + Ctrl-L
#define WD_SBBO_SET_LINE_PATTERN                0xCC // Dbl-extnd + Ctrl-L

#define WD_EXAO_DEFINE_AUTHOR                   256
#define WD_EXAO_SET_BACKGROUND                  257
#define WD_EXAO_DRAW_CIRCLE                     258
#define WD_EXAO_DRAW_CONTOUR                    259
#define WD_EXAO_SET_COLOR                       260
#define WD_EXAO_SET_COLOR_MAP                   261
#define WD_EXAO_DEFINE_COMMENTS                 262
#define WD_EXAO_DEFINE_COPYRIGHT                263
#define WD_EXAO_DEFINE_CREATOR                  264
#define WD_EXAO_DEFINE_CREATION_TIME            265
#define WD_EXAO_SET_CODE_PAGE                   266
#define WD_EXAO_SET_DASH_PATTERN                267
#define WD_EXAO_DEFINE_DWF_HEADER               268
#define WD_EXAO_DEFINE_DESCRIPTION              269
#define WD_EXAO_DRAW_ELLIPSE                    270
#define WD_EXAO_DEFINE_EMBED                    271
#define WD_EXAO_DEFINE_END_OF_DWF               272
#define WD_EXAO_SET_FONT                        273
#define WD_EXAO_DRAW_IMAGE                      274
#define WD_EXAO_DEFINE_KEYWORDS                 275
#define WD_EXAO_SET_LAYER                       276
#define WD_EXAO_SET_LINE_PATTERN                277
#define WD_EXAO_SET_LINE_WEIGHT                 278
#define WD_EXAO_SET_LINE_STYLE                  279
#define WD_EXAO_DEFINE_MODIFICATION_TIME        280
#define WD_EXAO_DEFINE_NAMED_VIEW               281
#define WD_EXAO_DEFINE_PLOT_INFO                282
#define WD_EXAO_SET_PROJECTION                  283
#define WD_EXAO_DEFINE_SOURCE_CREATION_TIME     284
#define WD_EXAO_DEFINE_SOURCE_MODIFICATION_TIME 285
#define WD_EXAO_SOURCE_FILENAME                 286
#define WD_EXAO_DRAW_TEXT                       287
#define WD_EXAO_SET_URL                         288
#define WD_EXAO_DEFINE_UNITS                    289
#define WD_EXAO_SET_VIEWPORT                    290
#define WD_EXAO_SET_VIEW                        291
#define WD_EXAO_UNKNOWN                         292

#define WD_EXBO_SET_COLOR_MAP                   293
#define WD_EXBO_DRAW_IMAGE_BITONAL_MAPPED       294
#define WD_EXBO_DRAW_IMAGE_GROUP3X_MAPPED       295
#define WD_EXBO_DRAW_IMAGE_INDEXED              296
#define WD_EXBO_DRAW_IMAGE_MAPPED               297
#define WD_EXBO_DRAW_IMAGE_RGB                  298
#define WD_EXBO_DRAW_IMAGE_RGBA                 299
#define WD_EXBO_DRAW_IMAGE_JPEG                 300
#define WD_EXBO_ADSK_COMPRESSION                301
#define WD_EXBO_UNKNOWN                         302

#define WD_EXAO_DEFINE_TITLE                    303
#define WD_EXAO_DEFINE_SUBJECT                  304

#define WD_EXBO_DRAW_IMAGE_PNG                  305
#define WD_EXBO_DRAW_IMAGE_GROUP4               306
#define WD_EXAO_DRAW_PNG_GROUP4_IMAGE           307
#define WD_EXAO_SET_MERGE_CONTROL               308
#define WD_EXAO_SET_OPAQUE                      309
#define WD_EXAO_SET_MERGE                       310
#define WD_EXAO_SET_TRANSPARENT                 311
#define WD_EXAO_SET_OPTIMIZED_FOR_PLOTTING      312
#define WD_EXAO_SET_GROUP_BEGIN                 313
#define WD_EXAO_SET_GROUP_END                   314
#define WD_EXAO_SET_FILL_PATTERN                315
#define WD_EXAO_SET_INKED_AREA                  316
#define WD_EXBO_DRAW_IMAGE_GROUP4X_MAPPED       317
#define WD_EXBO_EMBEDDED_FONT                   318
#define WD_EXAO_EMBEDDED_FONT                   319
#define WD_EXAO_TRUSTED_FONT_LIST               320
#define WD_EXAO_BLOCK_MEANING                   321
#define WD_EXBO_BLOCK_MEANING                   322
#define WD_EXBO_ENCRYPTION                      323
#define WD_EXAO_ENCRYPTION                      324
#define WD_EXBO_ORIENTATION                     325
#define WD_EXAO_ORIENTATION                     326
#define WD_EXBO_ALIGNMENT                       327
#define WD_EXAO_ALIGNMENT                       328
#define WD_EXAO_PASSWORD                        329
#define WD_EXBO_GUID                            330
#define WD_EXBO_PASSWORD                        331
#define WD_EXAO_GUID                            332
#define WD_EXBO_FILETIME                        333
#define WD_EXAO_FILETIME                        334
#define WD_EXBO_GRAPHICS_HDR                    335
#define WD_EXBO_OVERLAY_HDR                     336
#define WD_EXBO_REDLINE_HDR                     337
#define WD_EXBO_THUMBNAIL                       338
#define WD_EXBO_PREVIEW                         339
#define WD_EXBO_OVERLAY_PREVIEW                 340
#define WD_EXBO_FONT                            341
#define WD_EXBO_GRAPHICS                        342
#define WD_EXBO_OVERLAY                         343
#define WD_EXBO_REDLINE                         344
#define WD_EXBO_USER                            345
#define WD_EXBO_NULL                            346
#define WD_EXBO_GLOBAL_SHEET                    347
#define WD_EXBO_GLOBAL                          348
#define WD_EXBO_SIGNATURE                       349
#define WD_EXBO_BLOCKREF                        350
#define WD_EXAO_BLOCKREF                        351
#define WD_EXBO_DIRECTORY                       352
#define WD_EXAO_DIRECTORY                       353
#define WD_EXBO_USERDATA                        354
#define WD_EXAO_USERDATA                        355
#define WD_EXBO_PEN_PATTERN                     356
#define WD_EXAO_PEN_PATTERN                     357
#define WD_EXBO_SIGNDATA                        358
#define WD_EXAO_SIGNDATA                        359
#define WD_EXBO_GUID_LIST                       360
#define WD_EXAO_GUID_LIST                       361
#define WD_EXAO_SET_FONT_EXTENSION              362
#define WD_EXAO_PENPAT_OPTIONS                  363
#define WD_EXAO_GOURAUD_POLYTRIANGLE            364
#define WD_EXAO_DRAWING_INFO                    365
#define WD_EXAO_OBJECT_NODE                     366
#define WD_EXAO_GOURAUD_POLYLINE                367
#define WD_EXAO_BEZIER                          368
#define WD_EXBO_MACRO_DEFINITION                369
#define WD_EXAO_MACRO_DEFINITION                370
#define WD_EXBO_TEXT_HALIGN                     371
#define WD_EXAO_TEXT_HALIGN                     372
#define WD_EXBO_TEXT_VALIGN                     373
#define WD_EXAO_TEXT_VALIGN                     374
#define WD_EXBO_TEXT_BACKGROUND                 375
#define WD_EXAO_TEXT_BACKGROUND                 376
#define WD_EXBO_OVERPOST                        377        
#define WD_EXAO_OVERPOST                        378 
#define WD_EXAO_DELINEATE                       379
#define WD_EXBO_DELINEATE                       380
#define WD_EXAO_SET_USER_FILL_PATTERN           381
#define WD_EXBO_SET_USER_FILL_PATTERN           382
#define WD_EXAO_SET_USER_HATCH_PATTERN          383
#define WD_EXBO_SET_USER_HATCH_PATTERN          384
#define WD_EXAO_SET_CONTRAST_COLOR              385
#define WD_EXBO_SET_CONTRAST_COLOR              386
#define WD_EXAO_ATTRIBUTE_URL                   387

#define WD_MAX_OPCODE_ID                        387

#endif // OPCODE_DEFS_HEADER
