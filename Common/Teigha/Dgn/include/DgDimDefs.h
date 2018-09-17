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
#ifndef __DG_DIMDEFS_H__
#define __DG_DIMDEFS_H__

#include "TD_PackPush.h"

//#include "DgGraphicsElement.h"
//#include "OdString.h"
//#include "Ge/GePoint2d.h"
//#include "Ge/GePoint3d.h"
//#include "Ge/GeQuaternion.h"


//-------------------------------------------------------------------
// OdDgDimTool
//-------------------------------------------------------------------

/** \details
    This class holds settings for the OdDgDimension class related to
  some of the tool-dependent settings.

    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimTool
{
public:
  // MKU 07/04/07 Leader - the optional terminator for the beginning of a dimension string
  enum Leader
  {
      kRadius       = 0
    , kRadiusExt1   = 1
    , kRadiusExt2   = 2
    , kDiameter     = 3
  };

  enum CustomSymbol
  {
      kCsNone         = 0
    , kCsDiameter     = 1
    , kCsRadius       = 2
    , kCsSquare       = 3
    , kCsSR           = 4
    , kCsSDiameter    = 5
  };
  enum TextType
  {
      kStandard = 0
    , kVertical = 1
    , kMixed    = 2
  };

  enum TerminatorType
  {
    kTtNone           = 0,
    kTtArrow          = 1,
    kTtStroke         = 2,
    kTtCircle         = 3,
    kTtFilledCircle   = 4
  };

public:
  OdDgDimTool()
  {}

  virtual ~OdDgDimTool()
  {}

  virtual bool getStackExtLinesFlag() const = 0;
  virtual void setStackExtLinesFlag( bool val ) = 0;

  virtual bool getLeftExtLinesPresentFlag() const = 0;
  virtual void setLeftExtLinesPresentFlag( bool val ) = 0;

  virtual bool getRightExtLinesPresentFlag() const = 0;
  virtual void setRightExtLinesPresentFlag( bool val ) = 0;

  virtual bool getCenterMarkFlag() const = 0;
  virtual void setCenterMarkFlag( bool val ) = 0;

  virtual bool getCenterMarkLeftExtendFlag() const = 0;
  virtual void setCenterMarkLeftExtendFlag( bool val ) = 0;

  virtual bool getCenterMarkRightExtendFlag() const = 0;
  virtual void setCenterMarkRightExtendFlag( bool val ) = 0;

  virtual bool getCenterMarkTopExtendFlag() const = 0;
  virtual void setCenterMarkTopExtendFlag( bool val ) = 0;

  virtual bool getCenterMarkBottomExtendFlag() const = 0;
  virtual void setCenterMarkBottomExtendFlag( bool val ) = 0;

  virtual bool getArcSymbolFlag() const = 0;
  virtual void setArcSymbolFlag( bool val ) = 0;

  virtual bool getChordAlignFlag() const = 0;
  virtual void setChordAlignFlag( bool val ) = 0;

  virtual TerminatorType getFirstTerminator() const = 0;
  virtual void setFirstTerminator( TerminatorType val ) = 0;

  virtual TerminatorType getLeftTerminator() const = 0;
  virtual void setLeftTerminator( TerminatorType val ) = 0;

  virtual TerminatorType getRightTerminator() const = 0;
  virtual void setRightTerminator( TerminatorType val ) = 0;

  virtual TerminatorType getJointTerminator() const = 0;
  virtual void setJointTerminator( TerminatorType val ) = 0;

  virtual TextType getTextType() const = 0;
  virtual void setTextType( TextType val ) = 0;

  virtual CustomSymbol getPrefix() const = 0;
  virtual void setPrefix( CustomSymbol val ) = 0;

  virtual CustomSymbol getSuffix() const = 0;
  virtual void setSuffix( CustomSymbol val ) = 0;

  virtual bool getDecrementInReverseDirectionFlag() const = 0;
  virtual void setDecrementInReverseDirectionFlag( bool val ) = 0;

  virtual bool getFreeLocationOfTxtFlag() const = 0;
  virtual void setFreeLocationOfTxtFlag( bool val ) = 0;

  virtual double getDatumValue() const = 0;
  virtual void setDatumValue( double val ) = 0;

  virtual Leader getLeader() const = 0;
  virtual void setLeader( Leader val ) = 0;

};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolSizeArrow : public OdDgDimTool
{
public:
  OdDgDimToolSizeArrow()
  {}

  virtual ~OdDgDimToolSizeArrow()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolSizeStroke : public OdDgDimTool
{
public:
  OdDgDimToolSizeStroke()
  {}
  virtual ~OdDgDimToolSizeStroke()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolSingleLocation : public OdDgDimTool
{
public:
  OdDgDimToolSingleLocation()
  {}
  virtual ~OdDgDimToolSingleLocation()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolStackedLocation : public OdDgDimTool
{
public:
  OdDgDimToolStackedLocation()
  {}

  virtual ~OdDgDimToolStackedLocation()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolAngleSize : public OdDgDimTool
{
public:
  OdDgDimToolAngleSize()
  {}
  virtual ~OdDgDimToolAngleSize()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolArcSize : public OdDgDimTool
{
public:
  OdDgDimToolArcSize()
  {}
  virtual ~OdDgDimToolArcSize()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolAngleLocation : public OdDgDimTool
{
public:
  OdDgDimToolAngleLocation()
  {}
  virtual ~OdDgDimToolAngleLocation()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolArcLocation : public OdDgDimTool
{
public:
  OdDgDimToolArcLocation()
  {}
  virtual ~OdDgDimToolArcLocation()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolAngleLines : public OdDgDimTool
{
public:
  OdDgDimToolAngleLines()
  {}
  virtual ~OdDgDimToolAngleLines()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolAngleAxis : public OdDgDimTool
{
public:
  OdDgDimToolAngleAxis()
  {}
  virtual ~OdDgDimToolAngleAxis()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolNote : public OdDgDimTool
{
public:
  OdDgDimToolNote()
  {}
  virtual ~OdDgDimToolNote()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(StackExtLinesFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ArcSymbolFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(RightTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(Prefix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(Suffix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolLabelLine : public OdDgDimTool
{
public:
  OdDgDimToolLabelLine()
  {}
  virtual ~OdDgDimToolLabelLine()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(StackExtLinesFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ArcSymbolFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(RightTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(Prefix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(Suffix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolRadius : public OdDgDimTool
{
public:
  OdDgDimToolRadius()
  {}
  virtual ~OdDgDimToolRadius()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolRadiusExtended : public OdDgDimTool
{
public:
  OdDgDimToolRadiusExtended()
  {}
  virtual ~OdDgDimToolRadiusExtended()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolDiameter : public OdDgDimTool
{
public:
  OdDgDimToolDiameter()
  {}
  virtual ~OdDgDimToolDiameter()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolDiameterExtended : public OdDgDimTool
{
public:
  OdDgDimToolDiameterExtended()
  {}
  virtual ~OdDgDimToolDiameterExtended()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolDiameterParallel : public OdDgDimTool
{
public:
  OdDgDimToolDiameterParallel()
  {
  }
  virtual ~OdDgDimToolDiameterParallel()
  {
  }

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolPerpendicular : public OdDgDimTool
{
public:
  OdDgDimToolPerpendicular()
  {}
  virtual ~OdDgDimToolPerpendicular()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolCustomLinear : public OdDgDimTool
{
public:
  OdDgDimToolCustomLinear()
  {}
  virtual ~OdDgDimToolCustomLinear()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolOrdinate : public OdDgDimTool
{
public:
  OdDgDimToolOrdinate()
  {}
  virtual ~OdDgDimToolOrdinate()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkLeftExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkRightExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkTopExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkBottomExtendFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(RightTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(Prefix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(Suffix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimToolCenter : public OdDgDimTool
{
public:
  OdDgDimToolCenter()
  {}
  virtual ~OdDgDimToolCenter()
  {}

private:

#undef TOOL_PROPERTY_NOT_SUPPORTED
#define TOOL_PROPERTY_NOT_SUPPORTED(name, type)  \
  virtual type get##name() const   \
  { return (type)(0); }            \
  virtual void set##name(type val){} ;

  TOOL_PROPERTY_NOT_SUPPORTED(StackExtLinesFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(RightExtLinesPresentFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(CenterMarkFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ArcSymbolFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(ChordAlignFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FirstTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(LeftTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(RightTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(JointTerminator, TerminatorType)
  TOOL_PROPERTY_NOT_SUPPORTED(TextType, TextType)
  TOOL_PROPERTY_NOT_SUPPORTED(Prefix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(Suffix, CustomSymbol)
  TOOL_PROPERTY_NOT_SUPPORTED(DecrementInReverseDirectionFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(FreeLocationOfTxtFlag, bool)
  TOOL_PROPERTY_NOT_SUPPORTED(DatumValue, double)
  TOOL_PROPERTY_NOT_SUPPORTED(Leader, Leader)
};

#include "TD_PackPop.h"

#endif // __DG_DIMDEFS_H__
