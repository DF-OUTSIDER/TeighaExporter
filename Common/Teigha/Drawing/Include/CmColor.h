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


#ifndef ODA_CM_COLOR
#define ODA_CM_COLOR

#include "OdString.h"
#include "Gi/Gi.h"
#include "CmColorBase.h"
#include "DbExport.h"

class OdCmEntityColor;
class OdDbDwgFiler;
class OdDbDxfFiler;
class OdDbAuditInfo;

#include "TD_PackPush.h"

class OdDbObject;
class OdDbFiler;

/** \details
    <group OdCm_Classes>

    This class implements Color object that represents the unnamed and named colors
    specified by the byLayer, byBlock, byColor, byACI, byPen, Foreground, byDgnIndex, or None 
    color methods.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    The OdCmEntityColor class,  the OdCmColorBase class
*/
class TOOLKIT_EXPORT OdCmColor : public OdCmColorBase
{
public:
  OdCmColor();
  OdCmColor(
    const OdCmColor& color);
  OdCmColor(
    const OdCmColorBase& color);
  OdCmColor(
    OdCmEntityColor::ColorMethod color);
  OdCmColor& operator=(
    const OdCmColor& color);
  OdCmColor& operator=(
    const OdCmColorBase& color);
  ~OdCmColor();

  /** \details
    Compares two database color objects using their integer values together with their color names
    and returns true when their values are equal, or false when their values are not equal.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
    bool operator ==(
    const OdCmColor& color) const;
  bool operator ==(
    const OdCmColorBase& color) const;

  /** \details
    Compares two database color objects using their integer values together with their color names
    and returns true when their values are not equal, or false when their values are equal.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_integer.html, Color Functionality as an Integer-value>
  */
    bool operator !=(
    const OdCmColor& color) const;
  bool operator !=(
    const OdCmColorBase& color) const;
  
  /** \details
    Returns the description string of the database color object.

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  OdString getDescription() const;

  /** \details
    Returns the explanation string of the database color object.

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
 OdString getExplanation() const;

  virtual OdCmEntityColor::ColorMethod colorMethod() const;
  virtual void setColorMethod(
    OdCmEntityColor::ColorMethod colorMethod);

  virtual bool isByColor() const;
  virtual bool isByLayer() const;
  virtual bool isByBlock() const;
  virtual bool isByACI() const;

  virtual bool isForeground() const;
  virtual bool isByDgnIndex() const;

  /** \details
    Checks whether the color method is None (invisible) for the database color object and 
    returns true if and only if the color method is set to _kNone_ or was set to _kACInone_, 
    otherwise it returns false.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
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
    Returns the OdCmEntityColor settings of the database color object.
  */
  OdCmEntityColor entityColor() const;

  /** \details
    Returns a dictionary key based on the color name and book name of the database color object.
   
    \remarks
    OdCmColor objects with color namescan be stored in the form of an OdDbColor in a dictionary.
    getDictionaryKey() returns the key for that dictionary.

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  OdString getDictionaryKey() const;
  
  /** \details
    Sets the book name and color name for the dictionary keyword.

    \returns
    Returns true if and only if successful.
    The dictionary key is the string that is the combination of the book name and color name joined by the dollar '$' symbol

    \param dictionaryKey [in]  Dictionary keyword.   

    \sa
    <link cm_color_sample_base.html, Example of Working with the Database Color>

    \sa
    <link cm_color_book.html, Color Functionality as a Book Name>
  */
  bool setNamesFromDictionaryKey(
    const OdString& dictionaryKey);

  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
  */
  void dwgIn(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  void dwgOut(
    OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF format data of this object from the specified filer. 
    
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    \param groupCodeOffset [in]  Group code offset.
  */
  void dxfIn(
    OdDbDxfFiler* pFiler, 
    int groupCodeOffset = 0);

  /** \details
    Writes the DXF format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    \param groupCodeOffset [in]  Group code offset.
  */
  void dxfOut(
    OdDbDxfFiler* pFiler, 
    int groupCodeOffset = 0) const;

  /** \details
    Perform an audit operation on this object.

    \param pAuditInfo [in]  Pointer to an AuditInfo object.
    
    \remarks
    When overriding this function for a custom class, first call OdCmColorBase::audit(pAuditInfo) 
    to validate the audit operation.
  */
  void audit(
    OdDbAuditInfo* pAuditInfo);

  /*!DOM*/
  void     dwgInAsTrueColor (
    OdDbDwgFiler* pFiler);

  /*!DOM*/
  void     dwgOutAsTrueColor(
    OdDbDwgFiler* pFiler) const;

  TOOLKIT_EXPORT_STATIC static const OdUInt16 MaxColorIndex;

private:
  enum NameFlags
  {    
    kNoNames      = 0,
    kHasColorName = 1,
    kHasBookName  = 2
  };
  OdCmEntityColor::RGBM   m_RGBM;
  OdString                m_colorName;
  OdString                m_bookName;
};

#include "TD_PackPop.h"

#endif // ODA_CM_COLOR
