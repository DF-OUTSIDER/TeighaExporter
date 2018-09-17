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

#ifndef OD_DETAILSYMBOL_H
#define OD_DETAILSYMBOL_H

#include "DbViewSymbol.h"
#include "DbDetailViewStyle.h"
#include "TD_PackPush.h"

/** \details
    This class represents a Detail Symbol entity.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbDetailSymbol : public OdDbViewSymbol
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDetailSymbol);

  /** \details
    Default constructor. Creates an OdDbDetailSymbol instance.
  */
  OdDbDetailSymbol();
  ~OdDbDetailSymbol();

  /** \details
    Defines boundary types for Detail Symbol entities.
  */
  enum BoundaryType
  {
    kCircularBoundary,		// Circular boundary
    kRectangularBoundary,	// Rectangular boundary
    kCustomBoundary       // Custom boundary type
  };


  /** \details
    Defines which property of the style is overridden.
  */
  enum OverriddenProperty
  {
    kNone = 0,				// No properties are overridden
    kModelEdge = 0x1,			// Indicates the model edge type of the corresponding OdDbDetailViewStyle object has been overridden.
    kIdentifierPosition = 0x1 << 1	// Indicates the identifier placement of the corresponding OdDbDetailViewStyle object has been overridden.
  };

  /** \details
    Returns the boundary type for this OdDbDetailSymbol object as a value defined in the BoundaryType enumeration. 
  */
  BoundaryType boundaryType() const;

  /** \details
    Returns the type of the model edge as a value defined in the OdDbDetailViewStyle::ModelEdge enumeration. 
  */
  OdDbDetailViewStyle::ModelEdge modelEdgeType() const;

  /** \details
    Returns true only if the specified property is overridden.

    \param property [in]  The property value to check if it's overridden from the corresponding detail view style. 
    All applicable values are defined in the OverriddenProperty enumeration.
  */
  bool isOverriddenProperty(OdDbDetailSymbol::OverriddenProperty property) const;

  /** \details
    Returns whether the identifier of this Detail Symbol entity must be displayed.
  */
  bool displayIdentifier() const;


  /** \details
    Returns the origin of this Detail Symbol entity.
  */
  OdGePoint3d origin() const;

  /** \details
    Returns the direction of this Detail Symbol entity.
  */
  const OdGeVector3d direction() const;

  /** \details
    Returns the boundary size of this Detail Symbol entity.
  */
  const OdGeVector2d& boundarySize() const;

  /** \details
    Returns the origin of the model edge of of this Detail Symbol entity.
  */
  const OdGePoint3d& modelEdgeOrigin() const;

  /** \details
    Returns the scale parameter of the parent view.
  */
  double owningViewScale() const;

  /** \details
    Returns the scale parameter of the associated detail view.
  */
  double detailViewScale() const;

  /** \details
    Returns the model edge direction of this Detail Symbol entity.
  */
  const OdGeVector3d& modelEdgeDirection() const;

  /** \details
    Returns the identifier position of this Detail Symbol entity.
  */
  const OdGePoint3d identifierPosition() const;

  // Methods to use internal only :
  OdResult setIdentifierPosition( const OdGePoint3d& pt );

  OdResult setDetailViewScale( double viewScale );
  OdResult setOwningViewScale( double viewScale );

  OdResult setBoundaryType( OdDbDetailSymbol::BoundaryType bndType );

  OdResult setModelEdgeType( OdDbDetailViewStyle::ModelEdge modelEdgeType );
  OdResult setModelEdgeOrigin( const OdGePoint3d& pt );
  OdResult setModelEdgeDirection( const OdGeVector3d& dir );

  OdResult setDisplayIdentifier( const bool displayIdentifier );

  OdResult setOrigin( const OdGePoint3d& pt );
  OdResult setBoundarySize( const OdGeVector2d& size );

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable methods :
  virtual bool subWorldDraw(OdGiWorldDraw* pWorldDraw) const;

  virtual OdResult subTransformBy( const OdGeMatrix3d& xfm ) ODRX_OVERRIDE;
  virtual OdResult subGetGripPoints( OdGePoint3dArray& gripPoints ) const ODRX_OVERRIDE;
  virtual OdResult subMoveGripPointsAt( const OdIntArray& indices, const OdGeVector3d& offset ) ODRX_OVERRIDE;

};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDetailSymbol object pointers.
*/
typedef OdSmartPtr<OdDbDetailSymbol> OdDbDetailSymbolPtr;

#include "TD_PackPop.h"

#endif // OD_DETAILSYMBOL_H
