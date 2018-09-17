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

#ifndef __ODDRAWABLE_H__
#define __ODDRAWABLE_H__  /* { Secret} **/

#include "RxObject.h"
#include "RxOverrule.h"

class OdGiDrawableTraits;
class OdGiWorldDraw;
class OdGiViewportDraw;
class OdGsCache;
class OdDbStub;
class OdGeExtents3d;

#include "TD_PackPush.h"

/** \details
    This class is the base class for all graphical objects, both transient and persistent.
    
    \remarks
    This interface must be implemented by all graphical objects. Such objects
    are capable of vectorizing themselves within the Teigha framework.

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawable : public OdRxObject
{
protected:
  OdGiDrawable();
public:

  ODRX_DECLARE_MEMBERS(OdGiDrawable);

  enum SetAttributesFlags
  {
    kDrawableNone                         = 0, // Default flags; the drawable object uses only OdGi primitives, 
                                               // with no nested calls to draw().
    kDrawableIsAnEntity                   = 1, // Classes derived from OdDbEntity must set this flag,
                                               // which is set by the default implementation of 
                                               // OdDbEntity::subSetAttributes().
    kDrawableUsesNesting                  = 2, // The drawable uses nested calls to draw(), but makes no
                                               // calls to other OdGi primatives. 
    kDrawableIsCompoundObject             = 4, // The drawable is to be treated as a block. Valid only when combined 
                                               // with kDrawableIsAnEntity. If set, you must override 
                                               // OdDbEntity::getCompoundObjectTransform().
    kDrawableViewIndependentViewportDraw  = 8,  // Currently not supported.
    kDrawableIsInvisible                  = 16, // Object is invisible, and should not be rendered.
    kDrawableHasAttributes                = 32, // Currently not supported.
    kDrawableRegenTypeDependantGeometry   = 64, // This flag must be set for objects which is drawn differently in different regen type modes.
    kDrawableIsDimension                  = (kDrawableIsAnEntity + kDrawableIsCompoundObject + 128), // Dimension objects must set these flags
                                                // which are set by the default implementation of OdDbDimension.
    kDrawableRegenDraw                    = 256, // The drawable must be always regenerated.
    kDrawableStandardDisplaySingleLOD     = 512,   // Currently not supported.
    kDrawableShadedDisplaySingleLOD       = 1024,  // Currently not supported.
    kDrawableViewDependentViewportDraw    = 2048,  // Currently not supported.
    kDrawableBlockDependentViewportDraw   = 4096,  // Currently not supported.
    kDrawableIsExternalReference          = 8192,  // Currently not supported.
    kDrawableNotPlottable                 = 16384, // Currently not supported.

    kLastFlag                             = kDrawableNotPlottable // Marker
  };

  enum DrawableType
  {
      kGeometry              = 0, 
      kDistantLight          = 1, // this number is important!
      kPointLight            = 2, // this number is important!
      kSpotLight             = 3, // this number is important!
      kAmbientLight,
      kSolidBackground,
      kGradientBackground,
      kImageBackground,
      kGroundPlaneBackground,
      kViewport,
      kWebLight,
      kSkyBackground,
      kImageBasedLightingBackground
  };

  virtual DrawableType drawableType() const { return kGeometry; }

  /** \details
    Sets the vectorization attributes of this object, and returns its attribute flags.  

    \param pTraits [in]  Pointer to OdGiDrawableTraits object from and to which the attributes are to be set.

    \remarks
    This function is called by the vectorization framework, prior to calling worldDraw() or
    viewportDraw(), to set the attributes for an object.

    \sa
    SetAttributesFlags 
  */
  ODRX_SEALED_VIRTUAL OdUInt32 setAttributes(OdGiDrawableTraits* pTraits) const ODRX_SEALED;

  /** \details
    Creates a viewport-independent geometric representation of this object.
    
    \returns
    Returns true if and only if the geometric representation can be generated in a
    viewport-independent manner. 

    \remarks
    A return value of false indicates that viewportDraw() must be called for this object.

    The 3D GS will call this function at least once but may cache subsequent display updates.

    Use OdGsModel::onModified() to ensure that the 3D GS will call worldDraw() for next display update.
    
    The default implementation does nothing but return true. This function can be
    overridden in custom classes.

    \param pWd [in]  Pointer to the OdGiWorldDraw interface.
  */
  ODRX_SEALED_VIRTUAL bool worldDraw(OdGiWorldDraw* pWd) const ODRX_SEALED;

  /** \details
    Creates a viewport-dependent geometric representation of this object.

    \remarks
    Causes OdGiDrawable to describe its geometry to the specified OdGiViewportDraw object. 

    This function is called once per viewport.

    \remarks
    By overriding this function, it is possible to have totally different images in each viewport; a schematic
    in one viewport, a layout in another.
    
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.

    \param pVd [in]  Pointer to the OdGiViewportDraw interface.
  */
  ODRX_SEALED_VIRTUAL void viewportDraw(OdGiViewportDraw* pVd) const ODRX_SEALED;

  /** \details
    Returns true if and only if this object is persistent (stored in a database).
  */
  virtual bool isPersistent() const = 0;

  /** \details
    Returns the database ID of this object.

    \remarks
    Returns a null pointer if this object is not persistent.
  */
  virtual OdDbStub* id() const = 0;

  /** \details
    Assigns the specified OdGsCache to this object.
    
    \param pGsNode [in]  Pointer to the OdGsCache to be assigned.
  */
  virtual void setGsNode(OdGsCache* pGsNode) = 0;

  /** \details
    Returns the OdGsCache associated with this object.
  */
  virtual OdGsCache* gsNode() const = 0;

  /** \details
    Returns the setAttributes flags for the current viewportDraw. 
    
    \param pVd [in]  Pointer to OdGiViewportDraw interface.
    
    \remarks
    The default implementation of this function always returns 0.
  */
  ODRX_SEALED_VIRTUAL OdUInt32 viewportDrawLogicalFlags(OdGiViewportDraw* pVd) const ODRX_SEALED;

 /** \details
    Returns a WCS bounding box that represents the extents of this entity.

    \param extents [out]  Receives the WCS extents.

    \returns
    Returns eOk if successful or an appropriate error code if not.
    \remarks
    The default implementation of this function always returns eInvalidExtents. 
 */
  virtual OdResult getGeomExtents(OdGeExtents3d& extents) const;

 /** \details
    Returns a set of flags that represents the multi-threaded rendering support of this entity.

    \returns
    Returns 0 if the drawable can be rendered only in a single threaded mode.
    \remarks
    The default implementation of this function returns flags defined with the OdRxClass::customFlags() method.
 */
  ODRX_SEALED_VIRTUAL OdUInt32 regenSupportFlags() const ODRX_SEALED;

protected:
  virtual OdUInt32 subSetAttributes (OdGiDrawableTraits * traits) const = 0;
  virtual bool subWorldDraw (OdGiWorldDraw * wd) const = 0;
  virtual void subViewportDraw (OdGiViewportDraw * vd) const = 0;
  virtual OdUInt32 subViewportDrawLogicalFlags (OdGiViewportDraw * vd) const;
  virtual OdUInt32 subRegenSupportFlags() const;
  friend class OdGiDrawableOverrule;
};

typedef OdSmartPtr<OdGiDrawable> OdGiDrawablePtr;

/** \details
  This class overrules a subset of drawable linked operations which a drawable object specifies. 
  It is used as a base class for classes derived from OdGiDrawable to change their behavior. 
  Each default implementation calls the corresponding method of the target class.

  Corresponding C++ library: TD_Gi
  <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawableOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdGiDrawableOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdGiDrawableOverrule();

  /** \details
    Overrules the OdGiDrawable::setAttributes() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to the drawable object.
    \param traits [in]  A pointer to the graphics traits.
  */
  virtual OdUInt32 setAttributes (const OdGiDrawable* pSubject, OdGiDrawableTraits * traits);

  /** \details
    Overrules the OdGiDrawable::worldDraw() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to the drawable object.
    \param wd [in]  A pointer to the Gi world drawable object.
  */
  virtual bool worldDraw (const OdGiDrawable* pSubject, OdGiWorldDraw * wd);

  /** \details
    Overrules the OdGiDrawable::viewportDraw() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to the drawable object.
    \param vd [in]  A pointer to the Gi viewport drawable object.
  */
  virtual void viewportDraw (const OdGiDrawable* pSubject, OdGiViewportDraw * vd);

  /** \details
    Overrules the OdGiDrawable::viewportDrawLogicalFlags() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to the drawable object.
    \param vd [in]  A pointer to the Gi viewport drawable object.
  */
  virtual OdUInt32 viewportDrawLogicalFlags (const OdGiDrawable* pSubject, OdGiViewportDraw * vd);

  /** \details
    Overrules the regen support flags.

    \param pSubject [in]  A raw pointer to the drawable object.
  */
  virtual OdUInt32 regenSupportFlags(const OdGiDrawable* pSubject);
};

#include "TD_PackPop.h"

#endif // __ODDRAWABLE_H__

