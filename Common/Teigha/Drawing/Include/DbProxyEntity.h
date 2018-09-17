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




#ifndef _OD_DB_PROXY_ENTITY_
#define _OD_DB_PROXY_ENTITY_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "IdArrays.h"
#include "DwgDeclareMembers.h"

/** \details
    This class is the abstract base class for Proxy entities derived 
    from OdDbEntity in an OdDbDatabase instance.
    
    \sa
    TD_Db
    
    <group OdDb_Classes>
    
    \remarks
    Proxy entities hold surrogate data for custom Teigha objects
    when the parent application is not loaded, and allow Read-Only access
    to data contained therein. 
    
    Whenever the parent application is loaded, the Proxy entities revert
    to their custom objects.
*/
class DBIO_EXPORT OdDbProxyEntity : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbProxyEntity);

  OdDbProxyEntity();

  enum GraphicsMetafileType
  {
    kNoMetafile   = 0, // No bounding box or graphics (R13 drawing files only)
    kBoundingBox  = 1, // Bounding Box
    kFullGraphics = 2  // Full Graphics
  };

  /** \details
    Returns the GraphicsMetafileType for this Proxy entity;
    
    \remarks
    graphicsMetafileType() returns one of the following
    
    <table>
    Name            Value     Description
    kNoMetafile     0         No bounding box or graphics (R13 drawing files only)
    kBoundingBox    1         Bounding Box
    kFullGraphics   2         Full Graphics
    </table>
  */
  virtual OdDbProxyEntity::GraphicsMetafileType graphicsMetafileType() const;

  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

 /* Replace OdRxObjectPtrArray */
 
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  /*
     virtual bool subGetGeomExtents(OdGeExtents3d& extents) const;
  */

  enum
  {
    kNoOperation                = 0x000,
    kEraseAllowed               = 0x001,
    kTransformAllowed           = 0x002,
    kColorChangeAllowed         = 0x004,
    kLayerChangeAllowed         = 0x008,
    kLinetypeChangeAllowed      = 0x010,
    kLinetypeScaleChangeAllowed = 0x020,
    kVisibilityChangeAllowed    = 0x040,
    kCloningAllowed             = 0x080,
    kLineWeightChangeAllowed    = 0x100,
    kPlotStyleNameChangeAllowed = 0x200,
    kDisableProxyWarning        = 0x400,
    kMaterialChangeAllowed      = 0x800,
    kAllButCloningAllowed       = 0xB7F,
    kAllAllowedBits             = 0xBFF
  };

  /** \details
    Returns the edit flags settings for the class associated with this Proxy entity.
    
    \remarks
    These bits determine the allowable operations on this Proxy entity.
    
    proxyFlags() returns a combination of one or more of the following:
    
    <table>
    Name                          Value       Methods enabled       
    kNoOperation                  0x0         None
    kEraseAllowed                 0x1         erase()
    kTransformAllowed             0x2         transformBy()
    kColorChangeAllowed           0x4         setColor(), setColorIndex()
    kLayerChangeAllowed           0x8         setLayer() 
    kLinetypeChangeAllowed        0x10        setLinetype()
    kLinetypeScaleChangeAllowed   0x20        setLinetypeScale()
    kVisibilityChangeAllowed      0x40        setVisibility()
    kCloningAllowed               0x80        subDeepClone(), subWblockClone()
    kLineWeightChangeAllowed      0x100       setLineWeight()
    kPlotStyleNameChangeAllowed   0x200       setPlotStyleName()
    kDisableProxyWarning          0x400
    kMaterialChangeAllowed        0x800       setMaterial()
    kAllButCloningAllowed         0xB7F       All of the above but cloning
    kAllAllowedBits               0xBFF       All of the above
    </table>
  */
  virtual int proxyFlags() const;

  /** \details
    Returns true if and only if the erase() method is allowed for this Proxy entity. 
  */
  bool eraseAllowed() const               { return GETBIT(proxyFlags(), kEraseAllowed); }
  /** \details
    Returns true if and only if the transformBy() method is allowed for this Proxy entity. 
  */
  bool transformAllowed() const           { return GETBIT(proxyFlags(), kTransformAllowed); }
  /** \details
    Returns true if and only if the setColor() and setColorIndex() methods are allowed for this Proxy entity. 
  */
  bool colorChangeAllowed() const         { return GETBIT(proxyFlags(), kColorChangeAllowed); }
  /** \details
    Returns true if and only if the setLayer() method is allowed for this Proxy entity. 
  */
  bool layerChangeAllowed() const         { return GETBIT(proxyFlags(), kLayerChangeAllowed); }
  /** \details
    Returns true if and only if the setLinetype() method is allowed for this Proxy entity. 
  */
  bool linetypeChangeAllowed() const      { return GETBIT(proxyFlags(), kLinetypeChangeAllowed); }
  /** \details
    Returns true if and only if the setLinetypeScale() method is allowed for this Proxy entity. 
  */
  bool linetypeScaleChangeAllowed() const { return GETBIT(proxyFlags(), kLinetypeScaleChangeAllowed); }
  /** \details
    Returns true if and only if the setVisibility() method is allowed for this Proxy entity. 
  */
  bool visibilityChangeAllowed() const    { return GETBIT(proxyFlags(), kVisibilityChangeAllowed); }
  /** \details
    Returns true if and only if the setLineWeight() method is allowed for this Proxy entity. 
  */
  bool lineWeightChangeAllowed() const    { return GETBIT(proxyFlags(), kLineWeightChangeAllowed); }
  /** \details
    Returns true if and only if the setPlotStyleName() method is allowed for this Proxy entity. 
  */
  bool plotStyleNameChangeAllowed() const { return GETBIT(proxyFlags(), kPlotStyleNameChangeAllowed); }
  /** \details
    Returns true if and only if the setMaterial() method is allowed for this Proxy entity. 
  */
  bool materialChangeAllowed() const    { return GETBIT(proxyFlags(), kMaterialChangeAllowed); }
  /** \details
    Returns true if and only the subDeepClone() and subWblockClone() methods are allowed for this Proxy entity. 
  */
  bool cloningAllowed() const             { return GETBIT(proxyFlags(), kCloningAllowed); }
  /** \details
    Returns true if and only if all but the subDeepClone() and subWblockClone() methods are allowed for this Proxy entity.
    
    \remarks
    The allowed methods are as follows
    
    @untitled table
    erase()
    setColor()
    setColorIndex()
    setLayer() 
    setLinetype()
    setLinetypeScale()
    setLineWeight()
    setMaterial()
    setPlotStyleName()
    setVisibility()
    transformBy()
  */
  bool allButCloningAllowed() const       { return (proxyFlags() & kAllAllowedBits) == (kAllAllowedBits ^ kAllButCloningAllowed); }
  /** \details
    Returns true if and only if all methods are allowed for this Proxy entity. 

    \remarks
    The allowed methods are as follows
    
    @untitled table
    subDeepClone()
    erase()
    setColor()
    setColorIndex()
    setLayer() 
    setLinetype()
    setLinetypeScale()
    setLineWeight()
    setMaterial()
    setPlotStyleName()
    setVisibility()
    transformBy()
    subWblockClone()
  */
  bool allOperationsAllowed() const       { return (proxyFlags() & kAllAllowedBits) == kAllAllowedBits; }

  /** \details
    Returns true if and only this Proxy entity is a R13 format Proxy entity. 
  */
  bool isR13FormatProxy() const           { return GETBIT(proxyFlags(), 0x8000); }

  /** \details
    Returns the class name of the entity represented by this Proxy entity.
  */
  virtual OdString originalClassName() const;

  /** \details
    Returns the DXF name of the entity represented by this Proxy entity.
  */
  virtual OdString originalDxfName() const;

  /** \details
    Returns the application description of the entity represented by this Proxy entity.
  */
  virtual OdString applicationDescription() const;

  /** \details
    Returns an array of the Object IDs referenced by this Proxy entity.

    \param objectIds [out]  Receives an array of the reference Object IDs.
  */
  virtual void getReferences(
    OdTypedIdsArray& objectIds) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  /** \details
      Sets the properties for this entity taking into account the proxy flags.
  */
  
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setColor(
    const OdCmColor &color, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setColorIndex(
    OdUInt16 colorIndex, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setColorId(
    OdDbObjectId colorId, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setPlotStyleName(
    const OdString& plotStyleName, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setPlotStyleName(
    OdDb::PlotStyleNameType plotStyleNameType, 
    OdDbObjectId newId = OdDbObjectId::kNull, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLayer(
    const OdString& layerName, 
    bool doSubents = true,
    bool allowHiddenLayer = false);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLayer(
    OdDbObjectId layerId, 
    bool doSubents = true,
    bool allowHiddenLayer = false);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLinetype(
    const OdString& linetypeName, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLinetype(
    OdDbObjectId linetypeID, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLinetypeScale(
    double linetypeScale, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult subSetVisibility(
    OdDb::Visibility visibility, 
    bool doSubents = true);
  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setLineWeight(
    OdDb::LineWeight lineWeight, 
    bool doSubents = true);

  /** \details
    This function honors proxyFlags().
  */
  virtual OdResult setMaterial(
    const OdString& materialName,
    bool doSubents = true);
  virtual OdResult setMaterial(
    OdDbObjectId materialID,
    bool doSubents = true);

  virtual OdResult subErase(
    bool erasing);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbProxyEntity object pointers.
*/
typedef OdSmartPtr<OdDbProxyEntity> OdDbProxyEntityPtr;

#include "TD_PackPop.h"

#endif
