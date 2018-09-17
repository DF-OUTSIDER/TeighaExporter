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
#ifndef __DG_SHAREDCELLDEFINITION_H__
#define __DG_SHAREDCELLDEFINITION_H__


#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"
#include "Ge/GeExtents3d.h"
#include "DgModel.h"


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSharedCellDefinition : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSharedCellDefinition);

public:
  
  //name
  OdString getName() const;
  void setName( OdString const& name );

  //description (empty string is returned if there is not any descriptions)
  OdString getDescription() const;
  void setDescription( OdString const& description );
  
  //All internal elements are shifted so their zero point goes to that origin.
  //When a symbol is dragged during drawing, the cursor is linked to the point '-origin'.
  OdGePoint3d getOrigin() const;
  void setOrigin( OdGePoint3d const& origin );

  //the dimension (2d/3d) of the definition can not be detected automatically,
  //therefore it should be set manually
  void set3dFormatFlag( bool value );

  //support for its complexity
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  // OdGiDrawable overridden
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  virtual bool loadFromLibrary( const OdDgModelPtr& pLibraryCell );
};

typedef OdSmartPtr<OdDgSharedCellDefinition> OdDgSharedCellDefinitionPtr;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSharedCellDefinitionTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSharedCellDefinitionTable);
public:
  /** \details
    Adds the specified shared cell definition to this collection.

    \param pModel [in]  Pointer to the object to add.
  */
  void add(OdDgSharedCellDefinitionPtr pModel);

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  /** \details
    Looks for the description with given name and returns it if find it successfully
  */
  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;

  virtual bool has( const OdDgElementId& elementId) const;
};

typedef OdSmartPtr<OdDgSharedCellDefinitionTable> OdDgSharedCellDefinitionTablePtr;


#include "TD_PackPop.h"

#endif // __DG_SHAREDCELLDEFINITION_H__
