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

#ifndef DbAssocDimDependencyBody_INCLUDED_
#define DbAssocDimDependencyBody_INCLUDED_ /*!DOM*/

#include "DbAssocDimDependencyBodyBase.h"

#include "TD_PackPush.h"

/** \details
  This is the abstract base class that is used for deriving custom dependency body classes 
  and implementing the custom dependency functionality.

  Library: Source code provided.
  <group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocDimDependencyBody : public OdDbAssocDimDependencyBodyBase
{
public:
    ODRX_DECLARE_MEMBERS(OdDbAssocDimDependencyBody);

    /** \details
      Constructor creates an instance of this class.
    */
    OdDbAssocDimDependencyBody();

    /** \details
      Destructor destroys an instance of this class.
    */
    virtual ~OdDbAssocDimDependencyBody();

    /** \details
      Gets the entity text. Redefines the pure method of the base class.
    */
    virtual OdString getEntityTextOverride() const;

    /** \details
      Sets the entity text. Redefines the pure method of the base class.
    */
    virtual OdResult setEntityTextOverride(const OdString& newText);

    /** \details
      Gets the entity measurement. Redefines the pure method of the base class.
    */
    virtual double getEntityMeasurementOverride() const;

    /** \details
      Checks whether the entity attachment is changed. Redefines the pure method of the base class.
    */
    virtual bool isEntityAttachmentChangedOverride() const;

    /** \details
      Updates the entity position, size, orientation. Redefines the pure method of the base class.
    */
    virtual OdResult updateDependentOnObjectOverride();

    /** \details
      Pseudo constructor that creates a new associated dependency owning a new associated dimension 
      dependency body and makes the depended-on object to be the provided this dimension.
    */
    static OdResult
                createAndPostToDatabase(const OdDbObjectId& dimId,
                                        OdDbObjectId&       dimDepId,
                                        OdDbObjectId&       dimDepBodyId);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocDimDependencyBody class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocDimDependencyBody> OdDbAssocDimDependencyBodyPtr;

#include "TD_PackPop.h"

#endif
