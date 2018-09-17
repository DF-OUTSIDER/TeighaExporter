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

#ifndef ODDB_ANNOTATIONSCALE_H_INCLUDED
#define ODDB_ANNOTATIONSCALE_H_INCLUDED

#include "DbObjectContext.h"
#include "TD_PackPush.h"

class OdDbAnnotationScaleImpl;
class OdDbAnnotationScaleViewImpl;

////////////////////////////////////////////////////////////////////////
// class OdDbAnnotationScale
////////////////////////////////////////////////////////////////////////

/** \details
    This class defines an annotation scale as a context object.
    
    \remarks
    The context is the current viewport scale which can determine the properties
	of annotations in the viewport. You can get objects of this class using the 
	OdDbAnnotationScale context collection. An annotation scale is the ratio of paper 
	units to drawing units, for example, 1 mm to 1 meter for a scale of 0.01.
       
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbAnnotationScale : public OdDbObjectContext
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAnnotationScale);

  /** \details
    Returns the name of the annotation scale object.
  
	\remarks
	Because the name of the application scale object can change, access 
	the annotation scale object using its ID, which is unique and
	does not change.
  */
  virtual OdString getName() const;
 
  /** \details
    Sets the name of the annotation scale object.
  
	\param name [in]  Name of the annotation scale object.

	\remarks
	Because the name of the application scale object can change, access 
	the annotation scale object using its ID, which is unique and
	does not change.
  */
  virtual OdResult setName (const OdString& name);

 /** \details
    Returns the ID of the annotation scale.

	\remarks
	The ID of the annotation scale is unique and does not change.
 */
  virtual OdIntPtr uniqueIdentifier () const;

  /** \details
    Returns the name of the context collection (OdDbContextCollection) in which 
	the annotation scale is stored.
  */
  virtual OdString collectionName () const;

   /** \details
    Copies the contents of the specified annotation scale into this object when possible.
   */
  virtual void copyFrom(const OdRxObject* pSource);

   /** \details
    Returns the paper units value of the annotation scale.
 
	\remarks
	An annotation scale is the ratio of paper units to drawing units. If
	an annotation scale is 1 mm to 1 meter, the paper units value
	is 1.0.
   */
    virtual double getPaperUnits() const;
  
   /** \details
    Sets the paper units value of the annotation scale.
  
	\param dPaperUnits [in]  The paper units value for the annotation scale must be greater
	than zero.
 
	\remarks
	An annotation scale is the ratio of paper units to drawing units. If
	an annotation scale is 1 mm to 1 meter, the paper units value
	is 1.0.
   */
    virtual OdResult setPaperUnits (double dPaperUnits);

  /** \details
    Returns the drawing units value of the annotation scale.
 
	\remarks
	An annotation scale is the ratio of paper units to drawing units. If
	an annotation scale is 1 mm to 1 meter, the drawing units value
	is 100.0.
  */
    virtual double getDrawingUnits () const;

  /** \details
    Sets the drawing units value of the annotation scale.
  
	\param dDrawingUnits [in]  The drawing units value for the annotation scale must
	be greater than zero.

	\remarks
	An annotation scale is the ratio of paper units to drawing units. If
	an annotation scale is 1 mm to 1 meter, the drawing units value
	is 100.0.
  */
    virtual OdResult setDrawingUnits (double dDrawingUnits);

  /** \details
    Returns the annotation scale factor.
  
	\remarks
	An annotation scale is the ratio of paper units to drawing units. If
	an annotation scale is 1 mm to 1 meter, the scale factor is 0.01.
  */
    virtual OdResult getScale (double& dScale) const;

  /** \details
    Returns whether annotation scales are stored temporarily in the drawing.

	\remarks
	Temporary annotation scales are not saved with drawings. They exist
	when an Xref object with its own annotation scale is attached to a drawing.
	If the Xref object is detached from the drawing, its corresponding temporary
	annotation scale is removed.
  */
    virtual bool getIsTemporaryScale() const;

   /** \details
    Returns whether a specified ID matches the annotation scale ID.
  
	\param id [in]  The ID to compare with the annotation scale ID.
 
	\remarks
	This function is typically used if an Xref object that has its own annotation
	scale is attached to a drawing. Because the annotation scale used by the Xref
	object may be the same as the annotation scale used by the source drawing,
	this function is used to determine which annotation scale is being referenced.
	When referencing an annotation scale that you want saved in the drawing, do not
	reference the annotation scale of the Xref object since it is temporary and
	will be removed if the Xref object is detached or removed from the drawing.
   */
    virtual bool matchScaleId(OdIntPtr id) const;
protected:
  OdDbAnnotationScale(OdDbAnnotationScaleImpl*);
  OdDbAnnotationScaleImpl* m_pImpl;
  friend class OdDbAnnotationScaleImpl;
};

/** \details
    Teigha internal use only.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbAnnotationScaleView : public OdDbAnnotationScale
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAnnotationScaleView);
  OdString collectionName() const;
protected:
  OdDbAnnotationScaleView(OdDbAnnotationScaleViewImpl*);
  friend class OdDbAnnotationScaleViewImpl;
};

#include "TD_PackPop.h"

typedef OdSmartPtr<OdDbAnnotationScale>OdDbAnnotationScalePtr;

#endif //ODDB_ANNOTATIONSCALE_H_INCLUDED
