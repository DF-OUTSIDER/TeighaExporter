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

#ifndef __ODGSMODEL_H_INCLUDED_
#define __ODGSMODEL_H_INCLUDED_

#include "Gs/Gs.h"

class OdGiPathNode;

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class FIRSTDLL_EXPORT ODRX_ABSTRACT OdGsCache : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsCache);

  /** \details
    Returns pointer to the Model object associated with this Cache object.
  */
  virtual OdGsModel* model() const = 0;

  /** \details
    Reset pointer to the underlying GiDrawable object associated with this Cache object.
  */
  virtual void setDrawableNull() = 0;

  /** \details
    Returns the extents of the underlying GiDrawable object(s) associated with this Cache object.
    
    \param extents [out]  Receives the extents.
    
    \returns
    Returns true if and only if the GiDrawable object(s) have extents.
  */
  virtual bool extents(OdGeExtents3d& extents) const = 0;
};

typedef OdGiDrawablePtr (*OdGiOpenDrawableFn)(OdDbStub* id);

class OdGsModelReactor;

/** \details
    The class represents collections of drawable objects in the Teigha framework.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class FIRSTDLL_EXPORT ODRX_ABSTRACT OdGsModel : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsModel);

  /** \details
    Specifies the function called for drawable object open by this Model object.
    
    \param openDrawableFn [in]  Function to be called.
  */
  virtual void setOpenDrawableFn(OdGiOpenDrawableFn openDrawableFn) = 0;

  /** \details
    Notification function called whenever a drawable object is added to this Model object.

    \param pAdded [in]  Pointer to the added Drawable object.
    \param pParent [in]  Pointer to the parent of the added Drawable object.
  */
  virtual void onAdded(OdGiDrawable* pAdded, OdGiDrawable* pParent) = 0;

  /** \details
    Notification function called whenever a drawable object is added to this Model object.
    
    \param pAdded [in]  Pointer to the added Drawable object.
    \param parentID [in]  Object ID of the parent of the added Drawable object.
  */
  virtual void onAdded(OdGiDrawable* pAdded, OdDbStub* parentID) = 0;
  
  /** \details
    Notification function called whenever a drawable object is modified in this Model object.

    \param pModified [in]  Pointer to the modified Drawable object.
    \param pParent [in]  Pointer to the parent of the modified Drawable object.
  */
  virtual void onModified(OdGiDrawable* pModified, OdGiDrawable* pParent) = 0;

  /** \details
    Notification function called whenever a drawable object is modified in this Model object.

    \param pModified [in]  Pointer to the modified Drawable object.
    \param parentID [in]  Object ID of the parent of the modified Drawable object.
  */
  virtual void onModified(OdGiDrawable* pModified, OdDbStub* parentID) = 0;
  
  /** \details
    Notification function called whenever a drawable object is erased from this Model object.

    \param pErased [in]  Pointer to the erased Drawable object.
    \param pParent [in]  Pointer to the parent of the erased Drawable object.
  */
  virtual void onErased(OdGiDrawable* pErased, OdGiDrawable* pParent) = 0;

  /** \details
    Notification function called whenever a drawable object is erased from this Model object.

    \param pErased [in]  Pointer to the erased Drawable object.
    \param parentID [in]  Object ID of the parent of the erased Drawable object.
  */
  virtual void onErased(OdGiDrawable* pErased, OdDbStub* parentID) = 0;

  /** \details
    Notification function called whenever a drawable object is unerased in this Model object.

    \param pUnerased [in]  Pointer to the unerased Drawable object.
    \param pParent [in]  Pointer to the parent of the unerased Drawable object.
  */
  virtual void onUnerased(OdGiDrawable* pUnerased, OdGiDrawable* pParent);

  /** \details
    Notification function called whenever a drawable object is unerased in this Model object.

    \param pUnerased [in]  Pointer to the unerased Drawable object.
    \param parentID [in]  Object ID of the parent of the unerased Drawable object.
  */ 
  virtual void onUnerased(OdGiDrawable* pUnerased, OdDbStub* parentID);

  // Invalidation Hint

  /** \details
    The InvalidationHint enumeration specifies how the Graphic System should process 
    the invalidate() method.
  */
  enum InvalidationHint
  {
    kInvalidateIsolines       = 0,  // Invalidate isolines
    kInvalidateViewportCache  = 1,  // Invalidate viewport cache
    kInvalidateAll            = 2,  // Invalidate all caches
    kInvalidateMaterials      = 3,  // Invalidate materials
    kInvalidateLinetypes      = 4   // Invalidate all linetype caches
  };

  /** \details
    Invalidates the specified cached data contained in this Model object.
    
    \param hint [in]  Invalidation hint.
    
    \remarks
    Hint must be one of the following:
    
    <table>
    Name                        Value
    kInvalidateIsolines         0
    kInvalidateViewportCache    1
    kInvalidateAll              2
    kInvalidateMaterials        3
    kInvalidateLinetypes        4
    </table>
  */
  virtual void invalidate(InvalidationHint hint) = 0;

  /** \details
    Invalidates cached data associated with the specified View object.
    
    \param pView [in]  Pointer to the OdGsView object for which data is to be invalidated.
  */
  virtual void invalidate(OdGsView* pView) = 0;

  /** \details
    Invalidates cached data that visible on the device.
    
    \param pView [in]  Pointer to the OdGsDevice object for which data is to be invalidated.
  */
  virtual void invalidateVisible( OdGsDevice* pDevice ) = 0;

  /** \details
    Transforms this Model object by applying the specified transformation matrix to the model contents.
    
    \param OdGeMatrix3d [in]  Transformation matrix.
  */
  virtual void setTransform(const OdGeMatrix3d&) = 0;

  /** \details
    Obtains the current transformation matrix of this Model object.
  */
  virtual OdGeMatrix3d transform() const = 0;

  /** \details
    Highlights the specified entity in this Model object.
    
    \param path [in]  Path to the entity for highlighting.
    \param bDoIt [in]  Flag that indicates whether highlighting is enabled (true value) or disabled (false value).
    \param pView [in]  Pointer to the view where the specified entity should be highlighted.
  */
  virtual void highlight(const OdGiPathNode& path, bool bDoIt = true, const  OdGsView* pView = 0) = 0;

  /** \details
    Specifies how the model's geometry should be rendered by the Graphic System.
  */
  enum RenderType
  {
    kMinRenderType = -3, // Number of RenderType's in negative values range
    kUserBg1       = -3, // User-defined background rendering type without Z-buffer
    kUserBg2       = -2, // User-defined background rendering type with own Z-buffer
    kUserBg3       = -1, // User-defined background rendering type with Z-buffer
    kMain          =  0, // Use main Z-buffer
    kSprite,             // Use alternate Z-buffer (for sprites)
    kDirect,             // Render on device directly
    kHighlight,          // Render on device directly (skipping frame buffer and Z-buffer)
    kHighlightSelection, // Render on device directly using highlighting style (skipping frame buffer and Z-buffer)
    kDirectTopmost,      // Render on top of all other render types without Z-buffer
    kContrast,           // Render with contrast style
    kCount,              // Number of primary predefined RenderType's
    kUserFg1 = kCount,   // User-defined foreground rendering type with Z-buffer
    kUserFg2,            // User-defined foreground rendering type with own Z-buffer
    kUserFg3,            // Second user-defined foreground rendering type without Z-buffer
    kMaxRenderType,      // Number of RenderType's in positive values range
    kNumRenderTypes = kMaxRenderType - kMinRenderType // Exact number of RenderType's including predefined and user-defined RenderType's
  };

  /** \details
    Sets the render type for this Model object.
    
    \param renderType [in]  Render type.
  */
  virtual void setRenderType(RenderType renderType) = 0;

  /** \details
    Returns the render type of this Model object.
  */
  virtual RenderType renderType() const = 0;

  //virtual bool addSceneGraphRoot(OdGiDrawable* pRoot) = 0;
  //virtual bool eraseSceneGraphRoot(OdGiDrawable* pRoot) = 0;

  /** \details
    Sets the specified rendering mode override for this Model object.
    
    \param mode [in]  Render mode override.

    \remarks
    Rendering mode specified as OdGsView::kNone cancels currently set rendering mode override.
  */
  virtual void setRenderModeOverride(OdGsView::RenderMode mode = OdGsView::kNone) = 0;

  /** \details
    Enables this model's geometry to ignore the view's front and back clipping planes (if any) and viewport 3d clipping (if set).
    If the method's input value is false, the view's clipping planes and viewport 3d clipping are allowed by this Model object.
    
    \param bEnable [in]  Boolean flag which indicates that the model ignores the view's clipping planes and viewport 3d clipping (true value).
  */
  virtual void setViewClippingOverride(bool bEnable) = 0;

  /** \details
    Sets the background of this Model object.
    
    \param backgroundId [in]  OdDbBackground object ID.
  */
  virtual void setBackground(OdDbStub *backgroundId) = 0;

  /** \details
    Returns the OdDbBackground object ID if the background was set to this Model object.
  */
  virtual OdDbStub *background() const = 0;

  /** \details
    Sets the visual style for this Model object. This method is used to direct views 
    to display the model in the specified visual style. By default, models are displayed 
    in the visual style assigned to the view.
    
    \param visualStyleId [in]  Visual style object ID used by objects in this model
    
    \remarks
    Specify the OdDbObjectId::kNull as an input value to clear the model's visual style
    and use the view visual style for displaying the model's objects.
  */
  virtual void setVisualStyle(OdDbStub *visualStyleId) = 0;

  /** \details
    Returns the visual style of this Model object. 
  */  
  virtual OdDbStub *visualStyle() const = 0;

  /** \details
    Sets the visual style for this Model object. This method is used to direct views 
    to display the model in the specified visual style. By default, models are displayed 
    in the visual style assigned to the view.
    
    \param visualStyle [in]  Visual style object
  */
  virtual void setVisualStyle(const OdGiVisualStyle &visualStyle) = 0;

  /** \details
    Returns the visual style of this Model object. 
    
    \param visualStyle [out]  Visual style
    
    \returns Returns true if the visual style is set for the model, false otherwise.
  */  
  virtual bool visualStyle(OdGiVisualStyle &visualStyle) const = 0;

  /** \details
    Adds a model reactor for this Model object. 
    
    \param pReactor [in]  Model reactor
  */ 
  virtual void addModelReactor(OdGsModelReactor *pReactor) = 0;

  /** \details
    Removes the specified model reactor from this Model object. 
    
    \param pReactor [in]  Pointer to the model reactor that should be removed
  */ 
  virtual void removeModelReactor(OdGsModelReactor *pReactor) = 0;

  /** \details
    Enables the currently specified sectioning in the views displaying this Model object.
    
    \param bEnable [in]  Boolean flag which indicates whether sectioning should be enabled
  */
  virtual void setEnableSectioning(bool bEnable) = 0;

  /** \details
    Returns true if the sectioning is active in the views displaying this Model object.
    Returns false in the other case.
  */
  virtual bool isSectioningEnabled() const = 0;

  /** \details
    Sets the active sectioning for this Model object. After specifying the sectioning, 
    it should be enabled by the setEnableSectioning() method. 
    
    \param points [in]  Array of co-planar points specifying a sectioning region
    \param upVector [in]  Vector that specifies the orientation of the plane that contains points
    
    \remarks
    If the last point of the points array is equal to the first point of this array, the sectioning region is closed.
    The upVector is used to specify the clipped and unclipped sides of the sectioning. The unclipped side 
    is determined by the direction of the cross product of the first line segment defined by the point array, and the upVector.
  */
  virtual bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector) = 0;

  /** \details
    Sets the active sectioning for this Model object. The sectioning is capped vertically using 
    the specified top and bottom heights. After specifying the sectioning, it should be enabled 
    by the setEnableSectioning() method. 
    
    \param points [in]  Array of co-planar points specifying a sectioning region
    \param upVector [in]  Vector that specifies the orientation of the plane that contains points
    \param dTop [in]  Specifies the top capping height of the sectioning
    \param dBottom [in]  Specifies the bottom capping height of the sectioning
    
    \returns Returns true if the specified sectioning is valid. Returns false 
    if the specified sectioning is invalid or malformed.
    
    \remarks 
    If the last point of the points array is equal to the first point of this array, 
    the sectioning region is closed. The upVector is used to specify the clipped and unclipped 
    sides of the sectioning. The unclipped side is determined by the direction of the
    cross product of the first line segment defined by the point array, and the upVector.
  */
  virtual bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector,
                             double dTop, double dBottom) = 0;

  /** \details
    Sets the visual style for the objects on the clipped side of the sectioning. 
   
    \param visualStyleId [in]  Visual style object ID. Null by default.
    
    \remarks 
    If the input parameter is a null object ID, the clipped side should not be drawn.
  */                           
  virtual void setSectioningVisualStyle(OdDbStub *visualStyleId) = 0;

  /** \details
    Enables or disables linetype display in any OdGsView object that views this Model object.  
    
    \param bEnable [in]  Boolean flag which enables (true value) or disables (false value) linetype display.
  */
  virtual void setEnableLinetypes(bool bEnable) = 0;

  /** \details
    Returns true if the linetype display is enabled or returns false in the other case. 
  */
  virtual bool isLinetypesEnabled() const = 0;

  /** \details
    Enables or disables the ability to select this Model object. If the input value 
    is true, the Model object is selectable. If the input value is false, 
    the Model object is unselectable.
    
    \param bEnable [in]  Boolean flag which indicates whether this Model object is selectable
  */
  virtual void setSelectable(bool bEnable) = 0;

  /** \details
    Checks whether this Model object is selected. If the returning value is true, 
    the Model object is selectable. If the returning value is false, 
    the Model object is not selectable.
  */
  virtual bool isSelectable() const = 0;

  /** \details
    Enables or disables lights in blocks.
    
    \param bEnable [in]  Boolean flag which enables (true value) or disables (false value) lights in blocks.
  */
  virtual void setEnableLightsInBlocks(bool bEnable) = 0;

  /** \details
    Returns true if lights in blocks are enabled or returns false in the other case. 
  */
  virtual bool isLightsInBlocksEnabled() const = 0;
};

/** \details
    GsModel reactor.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class OdGsModelReactor
{
  public:
    OdGsModelReactor() { }
    virtual ~OdGsModelReactor() { }

    //virtual bool onSceneGraphRootAdded(OdGsModel *pModel, OdGiDrawable *pAdded) { return true; }
    //virtual bool onSceneGraphRootErased(OdGsModel *pModel, OdGiDrawable *pErased) { return true; }

    virtual bool onAdded(OdGsModel *pModel, OdGiDrawable *pAdded, OdGiDrawable *pParent) { return true; }
    virtual bool onAdded(OdGsModel *pModel, OdGiDrawable *pAdded, OdDbStub *parentID) { return true; }

    virtual bool onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdGiDrawable *pParent) { return true; }
    virtual bool onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdDbStub *parentID) { return true; }

    virtual bool onModified(OdGsModel *pModel, OdGiDrawable *pModified, OdGiDrawable *pParent) { return true; }
    virtual bool onModified(OdGsModel *pModel, OdGiDrawable *pModified, OdDbStub *parentID) { return true; }

    virtual bool onUnerased(OdGsModel *pModel, OdGiDrawable *pUnerased, OdGiDrawable *pParent) { return onAdded(pModel, pUnerased, pParent); }
    virtual bool onUnerased(OdGsModel *pModel, OdGiDrawable *pUnerased, OdDbStub *parentID) { return onAdded(pModel, pUnerased, parentID); }
};

#include "TD_PackPop.h"

#endif // __ODGSMODEL_H_INCLUDED_
