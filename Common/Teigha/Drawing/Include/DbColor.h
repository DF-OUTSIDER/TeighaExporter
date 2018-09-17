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


#ifndef _OD_DB_COLOR_
#define _OD_DB_COLOR_


#include "DbObject.h"
#include "CmColor.h"

#include "TD_PackPush.h"

/** \details
  <group OdDb_Classes>

  This class implements the color object that stores information about a color as an element of the 
  database container. The database stores the color instances in the dictionary and associates the 
  object ID with an each color instance. The color dictionary is accessed from the database object 
  using the getColorDictionaryId() method. The root drawing dictionary, associates the "ACAD_COLOR" name 
  with the color dictionary.

  \sa
  TD_Db

  \sa
  <link db_color_sample_dictionary.html, Example of Working with the Color Dictionary Object>

  <link db_color_sample_object.html, Example of Entering and Displaying for Color Objects>

  \sa
  <link cm_concept.html, Concept of Colors>

  <link db_color_concept.html, Concept of Database Colors>

  OdCmColor class
*/
class TOOLKIT_EXPORT OdDbColor : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbColor);

  /** \details
    Builds an instance of the color object.

    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_color_manipulate.html, Manipulating Objects of the Color Dictionary>
  */
  OdDbColor();


  /** \details
    Returns the unnamed color and named color instance specified by the byLayer, byBlock, byColor, 
    byACI, byPen, Foreground, byDgnIndex, None color methods, or name in a book.
  
    \sa
    <link db_color_sample_dictionary.html, Example of Working with the Color Dictionary Object>

    <link db_color_sample_object.html, Example of Entering and Displaying for Color Objects>

    \sa
    <link db_color.html, Working with Colors>

    OdCmColor class  
  */
  const OdCmColor& cmColor() const;


  /** \details
    Sets the unnamed color and named color instance for the database color object.
    
    \param cmColor [in]   A reference to the color instance.

    \sa
    <link db_color_sample_dictionary.html, Example of Working with the Color Dictionary Object>

    <link db_color_sample_object.html, Example of Entering and Displaying for Color Objects>

    \sa
    <link db_color.html, Working with Colors>

    OdCmColor class  
  */
  void setColor(const OdCmColor& cmColor);


  /** \details
    Returns the unnamed entity color instance specified by the byLayer, byBlock, byColor, 
    byACI, byPen, Foreground, byDgnIndex, None color methods.
  
    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link db_color.html, Working with Colors>

    <link cm.html, Colors and Transparencies>

    OdCmEntityColor class  
  */
  OdCmEntityColor entityColor() const;

  
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  The typified smart pointer for the color object. This template class is a specialization of 
  the OdSmartPtr class for OdDbColor object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbColor> OdDbColorPtr;

#include "TD_PackPop.h"

#endif
