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
#ifndef __DG_DETAILINGSYMBOL_H__
#define __DG_DETAILINGSYMBOL_H__

#include "TD_PackPush.h"

#include "DgExport.h"
#include "DgMacros.h"
#include "RxObject.h"
#include "DgCellHeader.h"
#include "DgCommonStructs.h"
#include "Gi/GiDrawable.h"
#include "DgLinkSet.h"
#include "DgTextNode.h"

class OdDgCellHeader3d;

//=======================================================================================================
//                                         OdDgDetailingSymbol classes
//=======================================================================================================

/** \details
  This class declares the methods for managing all types of detailing symbols.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailingSymbol
{
public:

  enum Type
  {
    kUnknownSymbol   = 0,
    kSectionMarker   = 1,
    kTitleText       = 2,
    kArrowMarker     = 3,
    kDetailMarker    = 4,
    kLastType = kArrowMarker
  };

  virtual OdDgDetailingSymbol::Type getSymbolType() const = 0;

  virtual OdUInt32  getLinkNodeCount() const = 0;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex ) = 0;
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode ) = 0;
  virtual void deleteLinkNode( OdUInt32 uIndex ) = 0;

  virtual void     applyTextStyle( const OdDgElementId idTextStyle ) = 0;

  virtual bool     getAnnotationScaleFlag() const = 0;
  virtual void     setAnnotationScaleFlag( bool bOn ) = 0;

  virtual double   getAnnotationScale() const = 0;
  virtual void     setAnnotationScale( double dAnnotationScale ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDetailingSymbol object pointers.
*/
typedef OdSmartPtr<OdDgDetailingSymbol> OdDgDetailingSymbolPtr;

//======================================================================================================

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailingSymbolCellHeader2d : public OdDgCellHeader2d
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDetailingSymbolCellHeader2d);

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

private:

  virtual OdString getName() const;
  virtual void setName(const OdString& name);
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual bool loadFromLibrary( const OdDgModelPtr& pLibraryCell );
};

//======================================================================================================

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailingSymbolCellHeader3d : public OdDgCellHeader3d
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDetailingSymbolCellHeader3d);

  virtual OdRxObjectPtr clone() const;

  virtual OdResult explodeGeometry( OdRxObjectPtrArray& entitySet ) const;

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

private:

  virtual OdString getName() const;
  virtual void setName(const OdString& name);
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual bool loadFromLibrary( const OdDgModelPtr& pLibraryCell );
};

//======================================================================================================

/** \details
  This class declares the methods for managing kTitleText type of 2d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTitleText2d : public OdDgDetailingSymbolCellHeader2d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgTitleText2d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode2dPtr& pTitleText, OdDgTextNode2dPtr& pSubtitleText );

  virtual OdDgTextNode2dPtr getTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getSubTitleText(  OdDg::OpenMode uMode = OdDg::kForRead );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTittleText2d object pointers.
*/
typedef OdSmartPtr<OdDgTitleText2d> OdDgTitleText2dPtr;

/** \details
  This class declares the methods for managing kTitleText type of 3d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTitleText3d : public OdDgDetailingSymbolCellHeader3d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgTitleText3d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode3dPtr& pTitleText, OdDgTextNode3dPtr& pSubtitleText );

  virtual OdDgTextNode3dPtr getTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getSubTitleText(  OdDg::OpenMode uMode = OdDg::kForRead );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTittleText3d object pointers.
*/
typedef OdSmartPtr<OdDgTitleText3d> OdDgTitleText3dPtr;

/** \details
  This class declares the methods for managing kSectionMarker type of 2d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSectionMarker2d : public OdDgDetailingSymbolCellHeader2d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSectionMarker2d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode2dPtr& pTitleText, OdDgTextNode2dPtr& pSubtitleText, 
                         OdDgTextNode2dPtr& pReferenceText, OdDgTextNode2dPtr& pSheetText, 
                         double dBubbleSize = 1.0 );

  virtual OdDgTextNode2dPtr getTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getSubTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSectionMarker2d object pointers.
*/
typedef OdSmartPtr<OdDgSectionMarker2d> OdDgSectionMarker2dPtr;

/** \details
  This class declares the methods for managing kSectionMarker type of 3d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSectionMarker3d : public OdDgDetailingSymbolCellHeader3d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSectionMarker3d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode3dPtr& pTitleText, OdDgTextNode3dPtr& pSubtitleText, 
                         OdDgTextNode3dPtr& pReferenceText, OdDgTextNode3dPtr& pSheetText, 
                         double dBubbleSize = 1.0 );

  virtual OdDgTextNode3dPtr getTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getSubTitleText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSectionMarker3d object pointers.
*/
typedef OdSmartPtr<OdDgSectionMarker3d> OdDgSectionMarker3dPtr;

/** \details
  This class declares the methods for managing kArrowMarker type of 2d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgArrowMarker2d : public OdDgDetailingSymbolCellHeader2d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgArrowMarker2d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode2dPtr& pReferenceText, OdDgTextNode2dPtr& pSheetText, 
                         double dBubbleSize = 1.0, bool bFlipArrow = false );

  virtual OdDgTextNode2dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  virtual bool              getFlipArrowFlag() const;
  virtual void              setFlipArrowFlag( bool bSet );

  virtual OdUInt32          getLeaderPointsCount() const;
  virtual OdGePoint2d       getLeaderPoint( OdUInt32 uIndex ) const;
  virtual void              setLeaderPoint( OdUInt32 uIndex, const OdGePoint2d& ptLeader );
  virtual void              addLeaderPoint( const OdGePoint2d& ptLeader );
  virtual void              deleteLeaderPoint( OdUInt32 uIndex );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgArrowMarker2d object pointers.
*/
typedef OdSmartPtr<OdDgArrowMarker2d> OdDgArrowMarker2dPtr;

/** \details
  This class declares the methods for managing kArrowMarker type of 3d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgArrowMarker3d : public OdDgDetailingSymbolCellHeader3d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgArrowMarker3d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode3dPtr& pReferenceText, OdDgTextNode3dPtr& pSheetText, 
                         double dBubbleSize = 1.0, bool bFlipArrow = false );

  virtual OdDgTextNode3dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  virtual bool              getFlipArrowFlag() const;
  virtual void              setFlipArrowFlag( bool bSet );

  virtual OdUInt32          getLeaderPointsCount() const;
  virtual OdGePoint3d       getLeaderPoint( OdUInt32 uIndex ) const;
  virtual void              setLeaderPoint( OdUInt32 uIndex, const OdGePoint3d& ptLeader );
  virtual void              addLeaderPoint( const OdGePoint3d& ptLeader );
  virtual void              deleteLeaderPoint( OdUInt32 uIndex );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgArrowMarker3d object pointers.
*/
typedef OdSmartPtr<OdDgArrowMarker3d> OdDgArrowMarker3dPtr;

/** \details
  This class declares the methods for managing kDetailMarker type of 2d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailMarker2d : public OdDgDetailingSymbolCellHeader2d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDetailMarker2d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode2dPtr& pReferenceText, OdDgTextNode2dPtr& pSheetText, 
                         double dBubbleSize = 1.0, double dDetailAreaRadius = 1.0 );

  virtual OdDgTextNode2dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode2dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  virtual double            getDetailAreaRadius() const;
  virtual void              setDetailAreaRadius( double dRadius );

  virtual OdUInt32          getLeaderPointsCount() const;
  virtual OdGePoint2d       getLeaderPoint( OdUInt32 uIndex ) const;
  virtual void              setLeaderPoint( OdUInt32 uIndex, const OdGePoint2d& ptLeader );
  virtual void              addLeaderPoint( const OdGePoint2d& ptLeader );
  virtual void              deleteLeaderPoint( OdUInt32 uIndex );

  virtual OdGePoint2d       getBubblePoint() const;
  virtual void              setBubblePoint( const OdGePoint2d& ptBubble );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDetailMarker2d object pointers.
*/
typedef OdSmartPtr<OdDgDetailMarker2d> OdDgDetailMarker2dPtr;

/** \details
  This class declares the methods for managing kDetailMarker type of 3d detailing symbol.

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailMarker3d : public OdDgDetailingSymbolCellHeader3d, public OdDgDetailingSymbol
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDetailMarker3d);

public:

  virtual OdDgDetailingSymbol::Type getSymbolType() const;

  virtual OdUInt32  getLinkNodeCount() const;
  virtual OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  virtual void addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  virtual void deleteLinkNode( OdUInt32 uIndex );

  virtual void      applyTextStyle( const OdDgElementId idTextStyle );

  virtual bool      getAnnotationScaleFlag() const;
  virtual void      setAnnotationScaleFlag( bool bOn );

  virtual double    getAnnotationScale() const;
  virtual void      setAnnotationScale( double dAnnotationScale );

  virtual void      set( OdDgTextNode3dPtr& pReferenceText, OdDgTextNode3dPtr& pSheetText, 
                         double dBubbleSize = 1.0, double dDetailAreaRadius = 1.0 );

  virtual OdDgTextNode3dPtr getReferenceText( OdDg::OpenMode uMode = OdDg::kForRead );
  virtual OdDgTextNode3dPtr getSheetText( OdDg::OpenMode uMode = OdDg::kForRead );

  virtual double            getBubbleSize() const;
  virtual void              setBubbleSize( double dSize );

  virtual double            getDetailAreaRadius() const;
  virtual void              setDetailAreaRadius( double dRadius );

  virtual OdUInt32          getLeaderPointsCount() const;
  virtual OdGePoint3d       getLeaderPoint( OdUInt32 uIndex ) const;
  virtual void              setLeaderPoint( OdUInt32 uIndex, const OdGePoint3d& ptLeader );
  virtual void              addLeaderPoint( const OdGePoint3d& ptLeader );
  virtual void              deleteLeaderPoint( OdUInt32 uIndex );

  virtual OdGePoint3d       getBubblePoint() const;
  virtual void              setBubblePoint( const OdGePoint3d& ptBubble );

  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDetailMarker3d object pointers.
*/
typedef OdSmartPtr<OdDgDetailMarker3d> OdDgDetailMarker3dPtr;

#include "TD_PackPop.h"

#endif // __DG_DETAILINGSYMBOL_H__
