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



#ifndef _INC_DDBRTRAVERSER_3F82DF7201C5_INCLUDED
#define _INC_DDBRTRAVERSER_3F82DF7201C5_INCLUDED

#include "Br/BrExport.h"
#include "Br/BrEnums.h"
#include "RxObject.h"

#include "TD_PackPush.h"
#include "SharedPtr.h"
#include "DbBaseSubentId.h"

/** \details
    This class is the interface class for BREP traversers.
    
    \sa
    TD_Br
    
    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrTraverser
{
public:

  /** \details
    Returns true if and only if the full topological adjacency list has been traversed with the next() function.
  */
  bool done() const;

  /** \details
    Sets this Traverser object to reference the next object in the topological adjacency list.

    \remarks
    Adjacency lists are circular, ordered lists.
    
    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus next();

  /** \details
      Sets this Traverser object to reference the object that it would normally return first.
  */
  OdBrErrorStatus restart();

  virtual ~OdBrTraverser();

  /** \details
    Returns true if and only if the specified Traverser object is equivalent to this Traverser object.
    
    \remarks
    Two OdBrTraverser objects are equivalent if and only if they non-null, have the
    same topological adjacency list owner, and have the same list position.
    
    \param pOtherTraverser [in]  Pointer to any OdBrTraverser object.
  */
  bool isEqualTo(const OdBrTraverser* pOtherTraverser) const;

  /** \details
    Returns true if and only if this Traverser object is empty. 
  */
  bool isNull() const;


  // Validation
  OdBrErrorStatus setValidationLevel(const BrValidationLevel& level = *(BrValidationLevel*)NULL);

  OdBrErrorStatus getValidationLevel(BrValidationLevel& level) const;

protected:
  OdRxObjectPtr m_pImp;
  OdSharedPtr<OdDbStubPtrArray> m_pFSubentPath;
  bool m_bIsValidate; //The enum has only 2 values.

  OdBrTraverser();

  OdBrTraverser(const OdBrTraverser& source);

  friend class OdBrTraverserInternals;
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRTRAVERSER_3F82DF7201C5_INCLUDED */

