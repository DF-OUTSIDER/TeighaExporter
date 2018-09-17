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
#ifndef __DG_3D_OBJECT_H__
#define __DG_3D_OBJECT_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSurface : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgSurface );
public:

  enum Type
  {
      kSurfaceProjection              = 0 // supported
    , kSurfaceBoundedPlane            = 1
    , kSurfaceUnboundedPlane          = 2
    , kSurfaceRight_CIRCULAR_Cylinder = 3
    , kSurfaceRight_CIRCULAR_Cone     = 4
    , kSurfaceTabulatedCylinder       = 5
    , kSurfaceTabulatedCone           = 6
    , kSurfaceConvolute               = 7
    , kSurfaceRevolution              = 8 // supported
    , kSurfaceWarped                  = 9
    //kSurfaceForcedWord              = 0x7FFFFFFF,
  };

  //type of the surface
  Type getType() const;
  void setType( Type type );

  //overrides from OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual OdRxObjectPtr clone() const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgSurface > OdDgSurfacePtr;


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSolid : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgSolid );
public:

  enum Type
  {
      kSolidProjection                = 0 // supported
    , kSolidRevolution                = 1 // supported
    , kSolidBoundary                  = 2
    //kSurfaceForcedWord              = 0x7FFFFFFF,
  };

  //type of the solid
  Type getType() const;
  void setType( Type type );

  //overrides from OdDgGraphicsElement
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual OdRxObjectPtr clone() const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgSolid > OdDgSolidPtr;


/** \details
The structure simplifies creation/reading surfaces & solids.

Each surface/solid consists of boundaries (chains of opened/closed elements)
linked together by rules (sets of curves of some type).

There should be at least 2 boundaries (with, obviously, 1 set of rules between them).

MUshakov, 04032010

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDg3dObjectHelper
{
public:
  //an element (OdDgSolid or OdDgSurface) should be given to that helper
  OdDg3dObjectHelper( OdDgGraphicsElementPtr element );

  //These helpers return true if a type fits for boundaries/rules.
  //Note: surfaces/solids allow different types.
  bool isAcceptableForBoundary( OdDgElement::ElementTypes type ) const;
  bool isAcceptableForRule( OdDgElement::ElementTypes type ) const;

  //The types to declare content
  typedef OdArray< OdDgGraphicsElementPtr > OdDgGraphicsElementPtrArray;
  struct Boundary 
  {
    OdDgGraphicsElementPtrArray m_boundary;
    OdDgGraphicsElementPtrArray m_rules; //rules to the previous boundary (hint: the first boundary can not have any rules)
  };
  typedef OdArray< Boundary > EntireObject;

  //Extract the content from the element.
  //Note: This method can throw an exception if the data is not correct
  void extractInformation( EntireObject& dataToFill );

  //Some helpers to build a new object.
  //Note:
  // 1. The order of coming elements is:
  //     - all elements for the 1-st boundary
  //     - 2-st boundary
  //     - the rules between them
  //     - 3-st boundary (if exists)
  //     - its rules
  //     - etc
  // 2. All boundaries should have the same structure (the same types, order of elements)
  // 3. Their types should correlate with their purpose
  // 4. These methods can throw exceptions if element mismatch their purpose
  // 5. The class of an element changes according to the purpose
  void addToBoundary( OdDgGraphicsElementPtr element );
  void addToRule( OdDgGraphicsElementPtr element );

  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true) const;

private:

  //it is used to read the first & second boundaries (they go together)
  void splitFirstBoundaries( EntireObject& dataToFill );

  //MUshakov, 28/05/2010. Bug #6984.
  //This method uses the given information (boundary counter, sub-elements with their 'class' properties)
  //to fill the given array with correct values for 'class' property (this array is used later to parse the object).
  //This method tries to recover structure if it is damaged.
  //Note: it can throw exceptions.
  void extractClassValues( OdArray< OdDgGraphicsElement::Class >& classes );

  bool m_isSolid; //it is based on the given element, just for ease

  OdDgSurfacePtr m_surface;
  OdDgSolidPtr m_solid;

  void splitFirstBoundaries( EntireObject& dataToFill, const OdDgElementPtr& pFirstRule );
};


#include "TD_PackPop.h"

#endif // __DG_3D_OBJECT_H__
