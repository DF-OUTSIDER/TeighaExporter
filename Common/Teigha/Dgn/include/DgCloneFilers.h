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


#ifndef OD_DG_CLONEFILERS_INCLUDED
#define OD_DG_CLONEFILERS_INCLUDED

#include "DgFiler.h"
#include "IdArrays.h"
#include "DgElementId.h"

class OdDgIdMapping;

class OdDgDeepCloneFiler;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDeepCloneFiler object pointers.
*/
typedef OdSmartPtr<OdDgDeepCloneFiler> OdDgDeepCloneFilerPtr;

/** \details
    This class is the base class for classes that are used for DeepClone file I/O operations utilizing .dgn formats.
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDeepCloneFiler : public OdDgFiler
{
protected:
  OdDgDeepCloneFiler() {}

public:
  ODRX_DECLARE_MEMBERS(OdDgDeepCloneFiler);

  /** \details
    Creates a DeepCloneFiler object with the specified ID Mapping.
    \param pIdMapping [in]  Pointer to the ID Mapping. 
  */
  static OdDgDeepCloneFilerPtr createObject(OdDgIdMapping* pIdMapping);

  // Protocol specific to clone filers

  /** \details
    Sets this DeepCloneFiler object to reference the Object ID that it would normally reference first.

    \remarks
    Allows multiple traversals of the ID Mapping.
  */
    virtual void start() = 0;
  /** \details
    Sets this DeepCloneFiler object to reference the Object ID following the current Object ID referenced.
    
    \param objectId [out]  Receives the next referenced Object ID.
  */
    virtual bool nextReference(OdDgElementId& objectId) = 0;

  /** \details
    Returns the ID Mapping for this DeepCloneFiler object.
  */
  virtual OdDgIdMapping& idMapping() = 0;

  /** \details
    Processes the objects in the ID Mapping of this DeepCloneFiler object.
  */
  virtual void defaultProcessReferences() = 0;
};

class OdDgWblockCloneFiler;
/** \details
   This template class is a specialization of the OdSmartPtr class for OdDbWblockCloneFiler object pointers.
*/
typedef OdSmartPtr<OdDgWblockCloneFiler> OdDgWblockCloneFilerPtr;

/** \details
   This class is the base class for classes that are used for Wblock file I/O operations utilizing .dgn format.
   Corresponding C++ library: TG_Db
   <group OdDg_Classes>
*/
class TG_EXPORT OdDgWblockCloneFiler : public OdDgDeepCloneFiler
{
protected:
 OdDgWblockCloneFiler() {}

public:
 ODRX_DECLARE_MEMBERS(OdDgWblockCloneFiler);

 /** \details
   Creates a WblockCloneFiler object with the specified ID Mapping.
   \param pIdMapping [in]  Pointer to the ID Mapping. 
 */
 static OdDgWblockCloneFilerPtr createObject(OdDgIdMapping* pIdMapping);
};

#endif //OD_DG_CLONEFILERS_INCLUDED
