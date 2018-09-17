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

#ifndef __DG_DGCMCOLOR_H__
#define __DG_DGCMCOLOR_H__

#include "DgExport.h"
#include "OdString.h"
#include "CmColorBase.h"
#include "DgGraphicsElement.h"

#include "TD_PackPush.h"


/** \details
    This class implements specifics of dgn color rendition.

	Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCmEntityColor: public OdCmEntityColor
{
public:

  enum DgColorMethod
  {
    kMinimum    = 0,
    kMaximum    = 65536,  // 255,
    kByCell     = OdDg::kColorByCell,
    kByLevel    = OdDg::kColorByLevel
  };

  OdDgCmEntityColor()
    : OdCmEntityColor()
  {
  }
  OdDgCmEntityColor(
    const OdCmEntityColor & color)
    : OdCmEntityColor(color)
  { 
  }
  OdDgCmEntityColor(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue)
    : OdCmEntityColor(red, green, blue)
  {
  }

  OdDgCmEntityColor& operator =(
    const OdCmEntityColor& color)
  {
    *((OdCmEntityColor*)this) = color;
    return *this;
  }

  bool operator ==(
    const OdCmEntityColor& color) const
  {
    return OdCmEntityColor::operator == (color);
  }

  bool operator ==(
    const OdDgCmEntityColor& color) const
  {
    return *((OdCmEntityColor*)this) == *((OdCmEntityColor*)&color);
  }

  bool operator !=(
    const OdCmEntityColor& color) const
  {
    return OdCmEntityColor::operator != (color);
  }

  bool operator !=(
    const OdDgCmEntityColor& color) const
  {
    return *((OdCmEntityColor*)this) != *((OdCmEntityColor*)&color);
  }

  void setColorIndex( OdInt16 colorIndex );

  static void setColorIndex(OdUInt32* rgbm, OdInt16 colorIndex);
};

/** \details
    This class implements Color objects, which represent the colors of OdDgElement objects, including RGB and ACI.

	Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCmColor : public OdCmColorBase
{
public:
  OdDgCmColor();
  OdDgCmColor(
    const OdDgCmColor& color);
  OdDgCmColor(
    const OdCmColorBase& color);
  OdDgCmColor(
    OdCmEntityColor::ColorMethod color);
  OdDgCmColor& operator=(
    const OdDgCmColor& color);
  OdDgCmColor& operator=(
    const OdCmColorBase& color);
  ~OdDgCmColor();

  bool operator ==(
    const OdDgCmColor& color) const;
  bool operator !=(
    const OdDgCmColor& color) const;
  bool operator ==(
    const OdCmColorBase& color) const;
  bool operator !=(
    const OdCmColorBase& color) const;

  virtual OdCmEntityColor::ColorMethod colorMethod() const;
  virtual void setColorMethod(
    OdCmEntityColor::ColorMethod colorMethod);

  virtual bool isByColor() const;
  virtual bool isByLayer() const;
  virtual bool isByBlock() const;
  virtual bool isByACI() const;
  virtual bool isByDgnIndex() const;

  virtual bool isForeground() const;

  /** \details
    Returns true if and only if the color method for this object is kNone or the
    color method is kByACI and the ACI is kACInone.
  */
  bool isNone() const;

  virtual OdUInt32 color() const;
  virtual void setColor(
    OdUInt32 color);

  virtual void setRGB(
    OdUInt8 red, 
    OdUInt8 green, 
    OdUInt8 blue);
  virtual void setRed(
    OdUInt8 red);
  virtual void setGreen(
    OdUInt8 green);
  virtual void setBlue(
    OdUInt8 blue);
  virtual OdUInt8 red() const;
  virtual OdUInt8 green() const;
  virtual OdUInt8 blue() const;

  virtual OdUInt16 colorIndex() const;
  virtual void setColorIndex(
    OdUInt16 colorIndex);

  virtual bool setNames(
    const OdString& colorName,
    const OdString& bookName = OdString::kEmpty);
  virtual OdString colorName() const;
  virtual OdString bookName() const;
  virtual OdString colorNameForDisplay() const;

  /** \details
    Returns the OdDgCmEntityColor settings of this object.
  */
  OdDgCmEntityColor entityColor() const;

private:
  enum NameFlags
  {    
    kNoNames            = 0x00,
    kHasColorName       = 0x01,
    kHasBookName        = 0x02
  };
  OdCmEntityColor::RGBM   m_RGBM;
  OdString                m_colorName;
  OdString                m_bookName;
};


#include "TD_PackPop.h"

#endif // __DG_DGCMCOLOR_H__
