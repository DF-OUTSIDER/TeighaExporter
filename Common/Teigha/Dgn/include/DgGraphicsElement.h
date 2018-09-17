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
#ifndef __DG_GRAPHICSELEMENT_H__
#define __DG_GRAPHICSELEMENT_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "Ge/GePoint3dArray.h"
#include "CmColorBase.h"
#include "DgGradientFill.h"

class OdDgCmColor;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
namespace OdDg
{
  // The special values  for the color, the line style and the weight.
  //  This is the equivalent for ACAD's ByBlock and ByLayer properties (and stand for special values of DGN files).

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum ColorIndexConstants
  {
      kColorByLevel      = 0xffffffff
    , kColorByCell       = 0xfffffffe
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineWeightConstants
  {
      kLineWeightByLevel = 0xffffffff
    , kLineWeightByCell  = 0xfffffffe
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum LineStyleConstants
  {
      kLineStyleByLevel  = 0x7fffffff
    , kLineStyleByCell   = 0x7ffffffe
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum OdDgIntersectType
  {
    kOnBothOperands    = 0,
    kExtendThis        = 1,
    kExtendArg         = 2,
    kExtendBoth        = 3
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum OdDgMaterialType
  {
    kLevelOverride   = 0,
    kByLevelAssigned = 1,
    kElementAssigned = 2
  };

  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum OdDgElementFillType
  {
    kNoFill       = 0,
    kFillColor    = 1,
    kGradientFill = 2
  };
}

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgGraphicsElement : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgGraphicsElement);
public:

  // element class: primary stands for most commonly used entities 
  // (like geometry primitives)
  enum Class
  {
      kClassPrimary             = 0
    , kClassPatternComponent    = 1
    , kClassConstruction        = 2
    , kClassDimension           = 3
    , kClassPrimaryRule         = 4
    , kClassLinearPatterned     = 5
    , kClassConstructionRule    = 6
  };

  /** \details
    Operate with color indices.
    See OdDg::ColorIndexConstants for some special values.
    See OdDgColorTable for the methods to translate indices to RGB values and back
  */
  virtual OdUInt32 getColorIndex() const;
  virtual void setColorIndex( OdUInt32 index );

  /**
    \details
    This ones are similar to get/setColorIndex() but use color tables to find/create indexed colors.
    \remarks 
    They generate eNotApplicable if an element is not connected to any database (that should have color tables) or the color index does not correspond to color tables.
  */
  virtual ODCOLORREF getColor() const;
  virtual void setColor( ODCOLORREF color );
  virtual void setColor( const OdDgCmColor& color, bool doSubents );

  /** \details
    The method returns the entry id of the graphics group.
  */
  virtual OdUInt32 getGraphicsGroupEntryId() const;

  /** \details
    The method changes the graphics group entry id of the current element.
  */
  virtual void setGraphicsGroupEntryId( OdUInt32 value );

  /** \details
    The method returns the element class property. See <B>Class</B> enumeration ocumentation
    for more details. 
  */
  virtual Class getClass() const;

  /** \details
  The method changes the element class property. See <B>Class</B> enumeration ocumentation
  for more details. 
  */
  virtual void setClass( Class newClass );

  /** \details
  */
  virtual OdUInt32 getLevelEntryId() const;
  virtual void setLevelEntryId(OdUInt32 index);

  virtual OdDgElementId getLevelId() const;
  virtual void setLevelId(const OdDgElementId& levelId, bool doSubents = true);

  /** \details
    Operate with line styles. See OdDg::LineStyleConstants for some special values.
  */
  virtual OdInt32 getLineStyleEntryId() const;
  virtual void setLineStyleEntryId( OdInt32 );

  virtual OdDgElementId getLineStyleId() const;
  virtual void setLineStyleId(const OdDgElementId& levelId);

  /** \details
    Operate with line weights. See OdDg::LineWeightConstants for some special values.
  */
  virtual OdUInt32 getLineWeight() const;
  virtual void setLineWeight( OdUInt32 );

  /** \details
  Operate with thickness.
  */

  virtual double getThickness( ) const;
  virtual void setThickness( double dThickness );

  /** \details
  Operate with materials.
  */
  virtual bool isMaterialPropertyAvailble() const;
  virtual OdDgElementId getMaterial() const;
  virtual void setMaterial( const OdDgElementId& idMaterial );
  virtual void setMaterial( const OdString& strMaterialName );

  /** \details
  Operate with element fills.
  */
  virtual bool getElementFillFlag() const;
  virtual OdDg::OdDgElementFillType getFillType() const;
  virtual OdUInt32 getFillColorIndex() const;
  virtual OdDgGradientFill getGradientFill() const;
  virtual void setFillColor( OdUInt32 uColor, bool bSetFillFlag = true );
  virtual void setGradientFill( const OdDgGradientFill& gradFill, bool bSetFillFlag = true );

  /** \details
  Operate with display order.
  */

  virtual OdInt32 getPriority( ) const;
  virtual void setPriority( OdInt32 iPriority );

  /** \details
    Sets the bit flag indicating the element's geometry is changed.

    \param graphicsModified [in]  New value.
  */
  void recordGraphicsModified(bool graphicsModified = true);

  //
  // OdGiDrawable overridden
  //
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  //it should be called by any object that could have common auxiliary drawable properties (e.c., patterns)
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  /** \details
    Returns the visibility status of this graphic element.
  */
  virtual bool getInvisibleFlag() const;

  /** \details
    Sets the visibility status of this graphic element.

    \param visibility [in]  Visibility status.
  */
  virtual void setInvisibleFlag(bool visibility);

  virtual bool get3dFormatFlag() const;

  virtual bool getViewIndependentFlag() const;
  virtual void setViewIndependentFlag(bool flag);

  virtual bool getNonPlanarFlag() const;
  virtual void setNonPlanarFlag(bool flag);

  virtual bool getNotSnappableFlag() const;
  virtual void setNotSnappableFlag(bool flag);

  /** \details
    The H-bit means the following:
      *	It indicates whether the element is a solid or a hole for closed element types as shape, complex shape, 
        ellipse, cone, B-spline surface header, and closed B-spline curve header:
          * false is Solid
          * true is Hole
      *	For a cell header if the H-bit is 
          * false = Header for a cell
          * true = Header for an orphan cell
      *	For a a line:
          * false = Line segment
          * true = Infinite-length line
      *	For a point string element:
          * false = Continuous
          * true = Disjoint
    The H-bit has no meaning in other elements.
  */
  virtual bool getHbitFlag() const;
  virtual void setHbitFlag(bool flag);

  /** \details
    Applies the default properties of the specified database to this entity.

    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed

    \remarks
    If pDb is null, the database containing this entity is used.

    The following properties are set

    * color
    * layer
    * linetype
    * linetype scale
    * lineweight
    * plotstyle
    * visibility
  */
  void setDatabaseDefaults(
    OdDgDatabase* pDb = 0,
    bool doSubents = false);

  /** \details
    Called by setDatabaseDefaults() after the values are set.

    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed

    \remarks
    If pDb is null, the database containing this entity is used.

    This function allows custom classes to inspect and modify the values set by setDatabaseDefaults.

    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.

    \remarks
    This function is not intended to be called by the user.
  */
  virtual void subSetDatabaseDefaults(
    OdDgDatabase* pDb,
    bool doSubents);

  /** \details
    Returns the transparency setting of this entity.
  */
  OdCmTransparency getTransparency() const;

  /** \details
    Sets the transparency setting of this entity, and returns eOk if successful.

    \param transparency [in]  OdCmTransparency object be assigned.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                  this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    If you override this function, you must call OdDbEntity::setTransparency(), passing to it the calling arguments.
  */
  virtual OdResult setTransparency(
    const OdCmTransparency& transparency,
    bool doSubents = true);

    /** \details
    Intersects this element with other 3d curve element if possible.

    \param pEnt   [in] Pointer to other 3d curve element
    \param inType [in] Determines type of intersection to be calculated
  */

  virtual OdResult intersectWith(const OdDgGraphicsElement* pElement, OdDg::OdDgIntersectType intType,
                                   OdGePoint3dArray& points) const;

  virtual OdResult intersectWith(const OdDgGraphicsElement* pElement, OdDg::OdDgIntersectType intType,
                                    const OdGePlane& projPlane, OdGePoint3dArray& points ) const;

  virtual OdResult subIntersectWith(const OdDgGraphicsElement* pEnt, OdDg::OdDgIntersectType intType,
                                      OdGePoint3dArray& points) const;

  virtual OdResult subIntersectWith(const OdDgGraphicsElement* pEnt, OdDg::OdDgIntersectType intType,
                                      const OdGePlane& projPlane, OdGePoint3dArray& points ) const;

  virtual void list() const;

  virtual void subList() const;

protected:

  virtual bool subGetInvisibleFlag() const;
  virtual void subSetInvisibleFlag(bool visibility);

  friend class OdDgVisibilityOverrule;
};

typedef OdSmartPtr<OdDgGraphicsElement> OdDgGraphicsElementPtr;

/** \details

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgGraphicsElementPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgGraphicsElementPE);
  long        m_references;     //used by each object separately
public:

  OdDgGraphicsElementPE();
  ~OdDgGraphicsElementPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    This method returns the extents of an element as it was saved in file before.
    
    \remarks
    1. All extents are given in UOR measure system.
    2. These values are not recalculated if their element is changed.
    3. It can be used as a fast alternative of getGeomExtents() when somebody relies on these saved values.
  */
  virtual OdResult getRange(const OdDgGraphicsElement* obj, OdGeExtents3d& extents) const;
};

typedef OdSmartPtr<OdDgGraphicsElementPE> OdDgGraphicsElementPEPtr;

/** \details

    <group OdDg_Classes>
*/

class OdDgDisplayStyleTableRecord;
typedef OdSmartPtr<OdDgDisplayStyleTableRecord> OdDgDisplayStyleTableRecordPtr;

class TG_EXPORT OdDgDisplayStyleGraphicsElementPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleGraphicsElementPE);
  long        m_references;     //used by each object separately
public:

  OdDgDisplayStyleGraphicsElementPE();
  ~OdDgDisplayStyleGraphicsElementPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    This method allows to get display style, that associated with graphics element.
  */
  virtual OdDgDisplayStyleTableRecordPtr getDispalyStyle(const OdDgGraphicsElement* obj, OdDg::OpenMode uOpenMode = OdDg::kForRead );

  /** \details
    This method allows to associate display style with graphics element.
  */
  virtual OdResult setDispalyStyle(OdDgGraphicsElement* obj, const OdDgDisplayStyleTableRecordPtr& pDisplayStyle );
};

typedef OdSmartPtr<OdDgDisplayStyleGraphicsElementPE> OdDgDisplayStyleGraphicsElementPEPtr;

#include "TD_PackPop.h"

#endif // __DG_GRAPHICSELEMENT_H__
