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



// OdBrEnums.h -- declaration of enumerated types used in the
//                OdBr B-Rep and traverser classes.

#pragma warning ( disable : 4482 )  

#ifndef ODBRENUMS_H
#define ODBRENUMS_H
#include "OdError.h"
#include "BrExport.h"
#include "Gi/Gi.h"

/** \details
  Represents the B-rep error status.
*/
enum OdBrErrorStatus
{ 
  odbrOK                     = ::eOk,                 // The operation is finished successfully.
  odbrWrongObjectType        = eWrongObjectType,      // The type of returned object is not supported as a B-rep.
  odbrInvalidObject          = eUnrecoverableErrors,  // The OdBr* object is not initialized or is invalid.
  odbrUnsuitableTopology     = 0XBC5,                 
  odbrMissingGeometry        = eDegenerateGeometry,   // Represents the partially or fully missing geometry.
  odbrInvalidInput           = eInvalidInput,         // The input arguments point to an invalid object.
  odbrDegenerateTopology     = 0XBCC,                 // The subentity does not map to the topology.
  odbrUninitialisedObject    = 0xBCD,                 // The OdBr* object is not initialized.
  odbrOutOfMemory            = eOutOfMemory,          // The memory for the object could not be allocated.
  odbrBrepChanged            = 0xBC0,                 // The object has been modified since this OdBr* object was last set.
  odbrNotImplementedYet      = ::eNotImplementedYet,  // Returned if the function is not implemented yet.
  odbrNullObjectId           = eNullObjectId,         // Returned when the subentity path does not point to an object.
  odbrNotApplicable          = eNotApplicable,        // Represents the situation when the inherited function is not applicable to this subclass. 
  odbrWrongSubentityType     = eWrongSubentityType,   // The subentity type does not match the subclass.
  odbrNullSubentityId        = eInvalidIndex,         // Returned when the subentity path does not point to a subentity.
  odbrNullObjectPointer      = eUnrecoverableErrors,  // Returned when the function implementation is missing.
  odbrObjectIdMismatch       = eWrongDatabase,        // The traverser list owner and list position do not point to the same object.
  odbrTopologyMismatch       = eWrongDatabase,        // Returned when the traverser list position cannot be set because the subentity is not connected to the list owner.
  odbrUnsuitableGeometry     = eAmbiguousOutput,      // The geometry of OdGe object is unsuitable for this function.
  odbrMissingSubentity       = eNotInDatabase,        // The topology does not map to a subentity.
  odbrAmbiguousOutput        = eAmbiguousOutput,      // The result is ambiguous.

  odbrUnrecoverableErrors    = eUnrecoverableErrors,
  odbrMissingTopology        = odbrDegenerateTopology,
  odbrWrongDatabase          = eWrongDatabase,
  odbrNotInDatabase          = eNotInDatabase,
  odbrDegenerateGeometry     = eDegenerateGeometry
};

/** \details
  Represents the type of a loop.
*/
enum BrLoopType{
  odbrLoopUnclassified = 0,   // The loop type is ambiguous or cannot be determined at this time. 
  odbrLoopExterior     = 1,   // The loop is on a peripheral boundary.
  odbrLoopInterior     = 2,   // The loop represents a hole in the interior of a face.
  odbrLoopWinding      = 3,   // The loop is winding on a conical surface.
};

/** \details
  Represents the shell type.
*/
enum BrShellType{ 
  odbrShellUnclassified = 0,  // The shell type cannot be determined at this time.  
  odbrShellExterior     = 1,  // The shell is on a peripheral boundary of region or B-rep.
  odbrShellInterior     = 2   // The shell is empty in the interior of a region or B-rep.
};

/** \details
  Represents the validation level.
*/
enum BrValidationLevel { 
  odbrFullValidation    = 0,
  odbrNoValidation      = 1
};

/** \details
  Represents the element shape criteria.
*/
enum Element2dShape { 
  kDefault           = 0,
  kAllPolygons       = 1,
  kAllQuadrilaterals = 2,
  kAllTriangles      = 3
};

/** \details
  Represents bit flags for BrEntity.
*/
enum BrEntityFlags {
  kNoFlags         = 0,
  kDoubleSide      = 0x001, //If it is DoubleSide then not SingleSide and vice versa
  kVisible         = 0x002, //Visible should be set by default. kNoFlags is default/drawings version.
  kInvisible       = 0x004, 
  kHighlight       = 0x008,  
  kSelectionIgnore = 0x010  //It cannot be selected
};

/** \details
  This class is a specialization of the OdError class for OdBr errors.  
  Corresponding C++ library: TD_Br
  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrException : public OdError
{
public:
  /** \details
    Constructor.
  
    \param errorStatus [in]  Error status.
  */
  OdBrException(OdBrErrorStatus errorStatus);
  
  /** \details
    Returns the error status associated with this Exception object.
  */
  OdBrErrorStatus getErrorStatus() const;
};

#define BR_THROW(n) throw OdBrException(n)

#endif // ODBRENUMS_H
