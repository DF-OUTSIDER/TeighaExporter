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

#ifndef __DUMMY_
#define __DUMMY_ CLASSDEF
#endif
#ifndef CLASSDF2
#define CLASSDF2 CLASSDEF
#endif

__DUMMY_( 0x00,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
CLASSDF2( 0x01,  Text,                  Entity,                 vAC13,    kMRelease0, TEXT,           ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x02,  Attribute,             Text,                   vAC13,    kMRelease0, ATTRIB,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x03,  AttributeDefinition,   Text,                   vAC13,    kMRelease0, ATTDEF,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x04,  BlockBegin,            Entity,                 vAC13,    kMRelease0, BLOCK,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x05,  BlockEnd,              Entity,                 vAC13,    kMRelease0, ENDBLK,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x06,  SequenceEnd,           Entity,                 vAC13,    kMRelease0, SEQEND,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x07,  BlockReference,        Entity,                 vAC13,    kMRelease0, INSERT,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x08,  MInsertBlock,          BlockReference,         vAC13,    kMRelease0, INSERT,         ObjectDBX Classes, 0x0000, 0x0183 )
__DUMMY_( 0x09,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
CLASSDF2( 0x0A,  2dVertex,              Vertex,                 vAC13,    kMRelease0, VERTEX,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x0B,  3dPolylineVertex,      Vertex,                 vAC13,    kMRelease0, VERTEX,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x0C,  PolygonMeshVertex,     Vertex,                 vAC13,    kMRelease0, VERTEX,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x0D,  PolyFaceMeshVertex,    Vertex,                 vAC13,    kMRelease0, VERTEX,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x0E,  FaceRecord,            Entity,                 vAC13,    kMRelease0, VERTEX,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x0F,  2dPolyline,            Curve,                  vAC13,    kMRelease0, POLYLINE,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x10,  3dPolyline,            Curve,                  vAC13,    kMRelease0, POLYLINE,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x11,  Arc,                   Curve,                  vAC13,    kMRelease0, ARC,            ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x12,  Circle,                Curve,                  vAC13,    kMRelease0, CIRCLE,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x13,  Line,                  Curve,                  vAC13,    kMRelease0, LINE,           ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x14,  OrdinateDimension,     Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x15,  RotatedDimension,      Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x16,  AlignedDimension,      Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
// fix for BCB 2006 bug
//#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 1408)
//__DUMMY_( 0x17,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
//#else
CLASSDF2( 0x17,  3PointAngularDimension,Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
//#endif
CLASSDF2( 0x18,  2LineAngularDimension, Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x19,  RadialDimension,       Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1A,  DiametricDimension,    Dimension,              vAC13,    kMRelease0, DIMENSION,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1B,  Point,                 Entity,                 vAC13,    kMRelease0, POINT,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1C,  Face,                  Entity,                 vAC13,    kMRelease0, 3DFACE,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1D,  PolyFaceMesh,          Entity,                 vAC13,    kMRelease0, POLYLINE,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1E,  PolygonMesh,           Entity,                 vAC13,    kMRelease0, POLYLINE,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x1F,  Solid,                 Entity,                 vAC13,    kMRelease0, SOLID,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x20,  Trace,                 Entity,                 vAC13,    kMRelease0, TRACE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x21,  Shape,                 Entity,                 vAC13,    kMRelease0, SHAPE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x22,  Viewport,              Entity,                 vAC13,    kMRelease0, VIEWPORT,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x23,  Ellipse,               Curve,                  vAC13,    kMRelease0, ELLIPSE,        ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x24,  Spline,                Curve,                  vAC13,    kMRelease0, SPLINE,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x25,  Region,                Entity,                 vAC13,    kMRelease0, REGION,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x26,  3dSolid,               Entity,                 vAC13,    kMRelease0, 3DSOLID,        ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x27,  Body,                  Entity,                 vAC13,    kMRelease0, BODY,           ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x28,  Ray,                   Curve,                  vAC13,    kMRelease0, RAY,            ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x29,  Xline,                 Curve,                  vAC13,    kMRelease0, XLINE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x2A,  Dictionary,            Object,                 vAC13,    kMRelease0, DICTIONARY,     ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x2B,  OleFrame,              Frame,                  vAC13,    kMRelease0, OLEFRAME,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x2C,  MText,                 Entity,                 vAC13,    kMRelease0, MTEXT,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x2D,  Leader,                Curve,                  vAC13,    kMRelease0, LEADER,         ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x2E,  Fcf,                   Entity,                 vAC13,    kMRelease0, TOLERANCE,      ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x2F,  Mline,                 Entity,                 vAC13,    kMRelease0, MLINE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x30,  BlockTable,            SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x31,  BlockTableRecord,      SymbolTableRecord,      vAC13,    kMRelease0, BLOCK_RECORD,   ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x32,  LayerTable,            SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x33,  LayerTableRecord,      SymbolTableRecord,      vAC13,    kMRelease0, LAYER,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x34,  TextStyleTable,        SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x35,  TextStyleTableRecord,  SymbolTableRecord,      vAC13,    kMRelease0, STYLE,          ObjectDBX Classes, 0x0000, 0x0183 )
//CLASSD( 0x36,  FontTable,             SymbolTable,            vAC13,    kMRelease0, FONT,           ObjectDBX Classes, 0x0000, 0x0183 )
//CLASSD( 0x37,  FontTableRecord,       SymbolTableRecord,      vAC13,    kMRelease0, FONT,           ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x38,  LinetypeTable,         SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x39,  LinetypeTableRecord,   SymbolTableRecord,      vAC13,    kMRelease0, LTYPE,          ObjectDBX Classes, 0x0000, 0x0183 )
__DUMMY_( 0x3A,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
__DUMMY_( 0x3B,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
CLASSDEF( 0x3C,  ViewTable,             SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x3D,  ViewTableRecord,       AbstractViewTableRecord,vAC13,    kMRelease0, VIEW,           ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x3E,  UCSTable,              SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x3F,  UCSTableRecord,        SymbolTableRecord,      vAC13,    kMRelease0, UCS,            ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x40,  ViewportTable,         SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x41,  ViewportTableRecord,   AbstractViewTableRecord,vAC13,    kMRelease0, VPORT,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x42,  RegAppTable,           SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x43,  RegAppTableRecord,     SymbolTableRecord,      vAC13,    kMRelease0, APPID,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x44,  DimStyleTable,         SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x45,  DimStyleTableRecord,   SymbolTableRecord,      vAC13,    kMRelease0, DIMSTYLE,       ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x46,  VXTable,               SymbolTable,            vAC13,    kMRelease0, TABLE,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDEF( 0x47,  VXTableRecord,         SymbolTableRecord,      vAC13,    kMRelease0, VX_TABLE_RECORD,ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x48,  Group,                 Object,                 vAC13,    kMRelease0, GROUP,          ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x49,  MlineStyle,            Object,                 vAC13,    kMRelease0, MLINESTYLE,     ObjectDBX Classes, 0x0000, 0x0103 ) // MTLoading is not supported
CLASSDF2( 0x4A,  Ole2Frame,             OleFrame,               vAC14,    kMRelease0, OLE2FRAME,      ObjectDBX Classes, 0x8000, 0x0103 ) // MTLoading is not supported
__DUMMY_( 0x4B,  Dummy,                 Object,                 vAC09,    kMRelease0, DUMMY,          DUMMY,             0x0000, 0x0183 )
CLASSDEF( 0x4C,  LongTransaction,       Object,                 kDHL_1500,kMRelease42,LONG_TRANSACTION,ObjectDBX Classes,0x0000, 0x0183 )
CLASSDF2( 0x4D,  Polyline,              Curve,                  vAC14beta,kMRelease0, LWPOLYLINE,     ObjectDBX Classes, 0x8000, 0x0183 )
CLASSDF2( 0x4E,  Hatch,                 Entity,                 vAC14beta,kMRelease0, HATCH,          ObjectDBX Classes, 0x8000, 0x0183 )
CLASSDEF( 0x4F,  Xrecord,               Object,                 vAC14,    kMRelease0, XRECORD,        ObjectDBX Classes, 0x8000, 0x0183 )
CLASSDEF( 0x50,  PlaceHolder,           Object,                 kDHL_1500,kMRelease42,ACDBPLACEHOLDER,ObjectDBX Classes, 0x0000, 0x0183 )
CLASSDF2( 0x51,  VbaProject,            Object,                 vAC13,    kMRelease0, VBA_PROJECT,    ObjectDBX Classes, 0x0000, 0x0103 )
CLASSDEF( 0x52,  Layout,                PlotSettings,           kDHL_1500,kMRelease42,LAYOUT,         ObjectDBX Classes, 0x0000, 0x0103 ) // MTLoading is not supported
