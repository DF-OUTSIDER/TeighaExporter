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


#ifndef _ODDBLINETYPETABLERECORD_INCLUDED
#define _ODDBLINETYPETABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTableRecord.h"
#include "DwgDeclareMembers.h"

class OdGeVector2d;

/** \details
  <group OdDb_Classes>

  This class implements the linetype record object, which represents a linetype in the database. 
  This class inherits the base functionality of named records. 

  \sa
  TD_Db

  \sa
  <link db_linetype_sample_table.html, Example of Working with the Linetype Table Object>

  <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

  <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

  \sa
  <link db_linetype.html, Working with Linetypes>

  OdDbLinetypeTable class
*/
class TOOLKIT_EXPORT OdDbLinetypeTableRecord : public OdDbSymbolTableRecord
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLinetypeTableRecord);

  /** \details
    Builds an instance of the linetype record object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_linetype_add_name.html, Adding and Naming Linetypes>
  */
  OdDbLinetypeTableRecord();


  /** \details
    Returns the simple ASCII representation of the linetype (DXF 3). It is a comment or series 
    of underscores, dots, dashes, and spaces to show a representation of the linetype.

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::setComments() method
  */
  const OdString comments() const;


  /** \details
    Sets the simple ASCII representation or comment for the linetype record object (DXF 3).
    It can be a comment or series of underscores, dots, dashes, and spaces to show a representation 
    of the linetype. The initial value is an empty string by default.

    \param sComment [in]  Comment as an arbitrary string up to 47 characters.

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::comments() method
  */
  void setComments( const OdString& sComment );


  /** \details
    Determines whether or not the alignment is scaled to fit (DXF 72) and returns True if the 
    alignment is scaled to fit or False if the alignment is not scaled to fit.
    
    \remarks
    When DXF 72 code contains the letter 'S', the alignment is scaled to fit (True). 
    When DXF 72 code contains the letter 'A', the alignment is not scaled to fit (False).

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::setIsScaledToFit() method
  */
  bool isScaledToFit() const;


  /** \details
    Sets the alignment scaling to fit for the linetype (DXF 72).  
    
    \param bScaleToFit [in]  True - to set scaled to fit or False - to set no scaled to fit. 

    \remarks
    When DXF 72 code contains the letter 'S', the alignment is scaled to fit (True). 
    When DXF 72 code contains the letter 'A', the alignment is not scaled to fit (False).

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::isScaledToFit() method
  */
  void setIsScaledToFit( bool bScaleToFit );


  /** \details
    Returns the total length of the linetype pattern in drawing units (DXF 40). This length is 
    a sum of all dashes forming the linetype pattern.
    
    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::setPatternLength() method
  */
  double patternLength() const;


  /** \details
    Sets the total length of the linetype pattern in drawing units (DXF 40). This length must  
    specify the sum of all dashes forming the linetype pattern. If the pattern length is set 
    to zero, the linetype record object automatically recalculates the pattern length. The 
    initial value is zero by default.
    
    \param vLength [in]  Pattern length as a Double value.
    
    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_properties.html, Specific Properties of Linetypes>

    OdDbLinetypeTableRecord::patternLength() method
  */
  void setPatternLength( double vLength );


  /** \details
    Returns the number of dashes in the linetype (DXF 73). If the number of dashes is a zero, 
    the linetype defines a solid line. 

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setNumDashes() method
  */
  int numDashes() const;


  /** \details
    Sets the number of dashes for the linetype (DXF 73). If the number of dashes is set to zero, 
    the linetype defines a solid line. If the linetype pattern contains dashes, it must contain 
    at least two of them. The initial value is zero by default.

    \param numberDashes [in]  Number of dashes [0,2..12].

    \sa
    <link db_linetype_sample_record.html, Example of Working with the Linetype Record Object>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::numDashes() method
  */
  void setNumDashes( int numberDashes );


  /** \details
    Returns the length of the linetype dash at the specified index in drawing units (DXF 49). 
    If the length value is positive, the dash is drawn. If a length value is negative, the dash 
    is not drawn (appears as a space). If the length value is zero, a point is drawn (appears 
    as a dot).
   
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    
    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setDashLengthAt() method
  */
  double dashLengthAt( int dashIndex ) const;


  /** \details
    Sets the length of the dash at the specified index in drawing units (DXF 49). A positive
    value specifies the dash which is drawn (appears as a line). A negative value specifies 
    the dash which is not drawn (appears as a space). A zero values specifies the dash which
    is a point (appears as a dot). The initial value is zero by default.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param dashLength [in]  Dash length as a Double value.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::dashLengthAt() method
  */
  void setDashLengthAt( int dashIndex, double dashLength );


  /** \details
    Returns the Object ID associated with an instance of the text style record object 
    (OdDbTextStyleTableRecord class) which specifies the characteristics of the shape 
    inclusion inserted in the linetype dash at the specified index (DXF 340).  
      
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \remarks
    Returns OdDb::kNull if there is no shape or text at the specified index.
    
    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeStyleAt() method
  */
  OdDbObjectId shapeStyleAt( int dashIndex ) const;


  /** \details
    Sets the Object ID associated with an instance of the text style record object 
    (OdDbTextStyleTableRecord class) for the linetype dash at the specified index as the
    text style which specifies the characteristics of the shape inclusion inserted in 
    in this linetype dash (DXF 340). The initial value is OdDb::kNull by default.
      
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param idTextStyle [in]  Object ID of the text style record object to be set.

    \remarks
    When a dash does not have an inclusion, the text style property is not applicable. 
    When the shape code is zero and the text string is an empty, the dash does not contain 
    an inclusion and a text style is not used. When either the shape code is non-zero or 
    the text string is non-empty, the text style property is applicable and must store the 
    Object ID of an existing text style.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeStyleAt() method
  */
  void setShapeStyleAt( int dashIndex, OdDbObjectId idTextStyle );


  /** \details
    Returns the code of the shape included in the dash at the specified index as an inclusion  
    inside the linetype pattern (DXF 75). When the shape code is zero, the dash does not contain  
    a shape inclusion. 

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeNumberAt() method
  */
  OdUInt16 shapeNumberAt( int dashIndex ) const;


  /** \details
    Sets the code of the shape as an inclusion in the dash at the specified index inside  
    the linetype pattern (DXF 75). A zero value removes the shape inclusion. The initial
    value is zero by default.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param shapeNumber [in]  Shape code to be set as an Integer value.

    \remarks
    Applying a shape inclusion requires the availability of the .shx file which must be 
    previously loaded and the anonymous text style that defines the shape parameters. 

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeNumberAt() method
  */
  void setShapeNumberAt( int dashIndex, OdUInt16 shapeNumber );


  /** \details
    Returns the text string included in the dash at the specified index as an inclusion  
    inside the linetype pattern (DXF 9). When the string is empty, the dash does not   
    contain a text inclusion.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setTextAt() method
  */
  OdString textAt(int dashIndex) const;


  /** \details
    Sets the text string as an inclusion in the dash at the specified index inside the  
    linetype pattern (DXF 9). An empty string removes the text inclusion. The initial
    value is an empty string by default.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param textString [in]  Text string to be set.

    \remarks
    Applying a text inclusion requires the availability of a text style that defines the 
    appearance and parameters of the text inside the dash.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::textAt() method
  */
  void setTextAt( int dashIndex, const OdString& textString );


  /** \details
    Returns the inclusion offset inside the dash at the specified index (DXF 44, 45). This
    offset defines the two-dimensional displacement to the insertion point of an inclusion 
    relative to the end of the current dash that contains the text or shape as an inclusion. 
    This method returns the offset as an instance of the two-dimensional vector object.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeOffsetAt() method
  */
  OdGeVector2d shapeOffsetAt( int dashIndex ) const;


  /** \details
    Sets the inclusion offset inside the dash at the specified index (DXF 44, 45). This
    offset specifies the two-dimensional displacement to the insertion point of an inclusion 
    relative to the end of the current dash that contains the text or shape.
    
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param shapeOffset [in]  Offset as an instance of the two-dimensional vector object.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeOffsetAt() method
  */
  void setShapeOffsetAt( int dashIndex, const OdGeVector2d& shapeOffset );


  /** \details
    Returns the factor by which the text or shape scales within the dash at the specified
    index (DXF 46). If the scale factor is less than 1.0, the inclusion is condensed. If 
    the scale factor is greater than 1.0, the inclusion is expanded. If the scale factor 
    equals 1.0, the height of the associated text style defines the inclusion size.
      
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeScaleAt() method
  */
  double shapeScaleAt( int dashIndex ) const;


  /** \details
    Sets the factor by which the text or shape scales within the dash at the specified
    index (DXF 46). A value less than 1.0 condenses the inclusion. A value greater than 
    1.0 expandes the inclusion. A value 1.0 defines inclusion size specified the associated 
    text style.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param shapeScale [in]  Scale factor as a positive non-zero Double value.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeScaleAt() method
  */
  void setShapeScaleAt( int dashIndex, double shapeScale );


  /** \details
    Returns the angle on which the inclusion (shape or text) is rotated within the dash  
    at the specified index (DXF 50) counterclockwise relative to the direction vector of  
    the linetype in the range -2PI to 2PI radians. If the angle is positive, the inclusion 
    is rotated counterclockwise. If the angle is negative, the inclusion is rotated clockwise.
    If the angle is zero, the direction of the linetype and inclusion are the same.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeRotationAt() method
  */
  double shapeRotationAt( int dashIndex ) const;


  /** \details
    Sets the angle on which the inclusion (shape or text) is rotated within the dash  
    at the specified index (DXF 50) counterclockwise relative to the direction vector of  
    the linetype in the range -2PI to 2PI radians. A positive angle rotates the inclusion 
    counterclockwise. A negative angle rotates the inclusion clockwise. A zero angle indicates 
    that the direction of the linetype and inclusion are the same. If an absolute angle value 
    is greater than 2PI, it converts to the range ±2PI. The initial value is zero by default.
      
    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param shapeRotation [in]  Rotation angle as a Double value in radians.
    
    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeRotationAt() method
  */
  void setShapeRotationAt( int dashIndex, double shapeRotation );


  /** \details
    Determines whether the inclusion (text or shape) is UCS-oriented inside the dash at the 
    specified index (DXF 74, bit 0x01) and returns True if the inclusion is oriented relative 
    to the current UCS or False if the inclusion is oriented relative to the line within which 
    it is embedded. 

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeIsUcsOrientedAt() method
  */
  bool shapeIsUcsOrientedAt( int dashIndex ) const;


  /** \details
    Sets the UCS-orientation of the inclusion (text or shape) inside the dash at the specified 
    index (DXF 74, bit 0x01) either relative to the current UCS (byUCS) or relative to the line  
    within which it is embedded (byLine). The initial value is False (byLine) by default.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param isUcsOriented [in]  True - to orient the inclusion byUCS, or False - to orient the inclusion 
                      byLine.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeIsUcsOrientedAt() method
  */
  void setShapeIsUcsOrientedAt( int dashIndex, bool isUcsOriented );

  /** \details
    Determines whether the inclusion (text or shape) is Upright-oriented inside the dash at the 
    specified index (DXF 74, bit 0x08) and returns True if the inclusion is oriented upright.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::setShapeIsUprightAt() method
  */
  bool shapeIsUprightAt( int dashIndex ) const;


  /** \details
    Sets the Upright orientation of the inclusion (text or shape) inside the dash at the specified 
    index (DXF 74, bit 0x08). The initial value is False by default.

    \param dashIndex [in]  Dash index as an Integer value in range [0 <= dashIndex < numDashes].
    \param isUpright [in]  True - to orient the inclusion Upright.

    \sa
    <link db_linetype_sample_dash.html, Example of Working with the Linetype Dash>

    \sa
    <link db_linetype_pattern.html, Working with the Linetype Pattern>

    OdDbLinetypeTableRecord::shapeIsUprightAt() method
  */
  void setShapeIsUprightAt( int dashIndex, bool isUpright );

  virtual OdResult dwgInFields( OdDbDwgFiler* pFiler );

  virtual void dwgOutFields( OdDbDwgFiler* pFiler ) const;

  virtual OdResult dxfInFields( OdDbDxfFiler* pFiler );

  virtual void dxfOutFields( OdDbDxfFiler* pFiler ) const;

  virtual OdResult dxfInFields_R12( OdDbDxfFiler* pFiler );

  virtual void dxfOutFields_R12( OdDbDxfFiler* pFiler ) const;

  virtual OdResult subGetClassID( void* pClsid ) const;

  virtual OdGiDrawable* drawable();

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
  
  virtual OdResult subErase( bool erasing );
};

/** \details
  The typified smart pointer for the linetype record object. This template class is 
  specialization of the OdSmartPtr class for the OdDbLinetypeTableRecord object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbLinetypeTableRecord> OdDbLinetypeTableRecordPtr;

#include "TD_PackPop.h"

#endif // _ODDBLINETYPETABLERECORD_INCLUDED

