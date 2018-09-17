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

#ifndef _DBSCALE_INCLUDED_
#define _DBSCALE_INCLUDED_

#include "DbObject.h"

/** \details
  <group OdDb_Classes>

  This class implements the scale object that stores information about an annotation scale as an 
  element of the database container. The database stores the scale instances in the dictionary 
  and associates the object ID with an each scale instance. The scale dictionary is accessed from 
  the database object using the getScaleListDictionaryId() method. The root drawing dictionary, 
  associates the "ACAD_SCALELIST" name with the scale dictionary.

  \remarks
  This class is intended for stand-alone applications only. TXSDK developers cannot use it.

  \sa
  TD_Db

  \sa
  <link db_scale_sample_object.html, Example of Working with the Scale Object>

  <link db_scale_sample_dictionary.html, Example of Working with the Scale Dictionary Object>

  \sa
  <link db_scale.html, Working with Scales>

  OdDbAnnotationScale class
*/
class TOOLKIT_EXPORT OdDbScale : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbScale);

  /** \details
    Builds an instance of the scale object.

    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_scale_manipulate.html, Manipulating Objects of the Scale Dictionary>
  */
  OdDbScale();


  /** \details
    Returns the internal scale name as a String value. This name is used as a comment for the ratio 
    of paper units to drawing units when the scale object is displayed in a list.
  
    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::setScaleName() method
  */
  class OdString scaleName() const;


  /** \details
    Sets the internal scale name as a String value. This name is used as a comment for the ratio 
    of paper units to drawing units when the scale object is displayed in a list. The initial value 
    is an empty string by default.
  
    \param sName [in]   Scale name as non-empty string.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::scaleName() method
  */
  void setScaleName( const OdString& sName );


  /** \details
    Returns the scale factor as a positive Double value. This factor defines the ratio in which 
    units of model space correlate to the units of paper space and indicates how many paper units 
    fit to one drawing unit.
  
    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::paperUnits(), OdDbScale::drawingUnits() methods
  */
  double scale() const;


  /** \details
    Returns the number of drawing units as a positive Double value.
  
    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::setDrawingUnits(), OdDbScale::paperUnits(), OdDbScale::scale() methods
  */
  double drawingUnits() const;


  /** \details
    Sets the number of drawing units as a positive Double value. The initial value is 1.0 by default.
  
    \param vUnits [in]   value of drawing units.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::drawingUnits(), OdDbScale::setPaperUnits(), OdDbScale::scale() methods
  */
  void setDrawingUnits( double vUnits );


  /** \details
    Returns the number of paper units as a positive Double value.
  
    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::setPaperUnits(), OdDbScale::drawingUnits(), OdDbScale::scale() methods
  */
  double paperUnits() const;


  /** \details
    Sets the number of paper units as a positive Double value. The initial value is 1.0 by default.
  
    \param vUnits [in]   value of paper units.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::paperUnits(), OdDbScale::setDrawingUnits(), OdDbScale::scale() methods
  */
  void setPaperUnits( double vUnits );


  /** \details
    Determines whether the scale object is temporary stored in the database and returns True if the  
    scale is temporary or False if the scale is fixed. 
    
    \remarks
    The temporary scale typically exists because attached XREF objects in the drawing have dependencies 
    on scales that are not directly referenced by other objects in the drawing. 

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::setIsTemporaryScale() method
  */
  bool isTemporaryScale() const;


  /** \details
    Sets the scale temporary status as a Boolean value. The initial value is False by default.

    \remarks
    The temporary scale typically exists because attached XREF objects in the drawing have  
    dependencies on scales that are not directly referenced by other objects in the drawing. 

    \param bStatus [in]   True if the scale is temporary or False if the scale is fixed.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::isTemporaryScale() method
  */
  void setIsTemporaryScale( bool bStatus );


  /** \details
    Determines whether the scale object is selected as one-to-one (1:1) by default and returns True if 
    the scale is selected (1:1) by default or False if the scale is not the default.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::setIsUnitScale() method
  */
  bool isUnitScale() const;


  /** \details
    Sets the scale default status as a Boolean value. The initial value is False by default.

    \param bStatus [in]   True if the scale is selected (1:1) by default or False if the scale is not the default.

    \sa
    <link db_scale_sample_object.html, Example of Working with the Scale Object>

    \sa
    <link db_scale.html, Working with Scales>

    OdDbScale::isUnitScale() method
  */
  void setIsUnitScale( bool bStatus );

  /*!DOM*/
  /** \details
    For internal use only.
  */
  void getXRefMangledName(OdString, OdString&, OdDbDatabase *);
  /*!DOM*/  
  /** \details
    For internal use only.
  */
  static bool nameExists(const OdString&,OdString*,OdDbDatabase *);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  virtual void appendToOwner(OdDbIdPair& idPair, OdDbObject* pOwnerObject, OdDbIdMapping& ownerIdMap) ODRX_OVERRIDE;
  virtual OdResult subErase(bool erasing) ODRX_OVERRIDE;
  virtual void applyPartialUndo(OdDbDwgFiler* pFiler, OdRxClass* pClass) ODRX_OVERRIDE;
};

/** \details
  The typified smart pointer for the scale object. This template class is a specialization of 
  the OdSmartPtr class for OdDbScale object.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbScale> OdDbScalePtr;


#endif //_DBSCALE_INCLUDED_
