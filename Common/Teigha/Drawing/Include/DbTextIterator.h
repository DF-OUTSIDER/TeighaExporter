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

#ifndef _DbTextIterator_h_Included_
#define _DbTextIterator_h_Included_

#include "DbExport.h"
#include "OdCodePage.h"
#include "Gi/GiTextStyle.h"

class TOOLKIT_EXPORT OdDbTextIterator;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbTextIterator object pointers.
*/
typedef OdSmartPtr<OdDbTextIterator> OdDbTextIteratorPtr;

#include "TD_PackPush.h"

/** \details
    This class can can be used to parse the text received in a client override of
    OdGiConveyorGeometry::textProc.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbTextIterator : public OdBaseTextIterator
{
public:
  /** \details
    Creates an TextIterator object for the specified data.

    \param textString [in]  Text string to be parsed.
    \param length [in]  Length of string in bytes.
    \param raw [in]  If and only if true, character sequences in the form of %%c will be treated as raw text.
    \param codePageId [in]  Object ID of the Code Page of the text.
    \param pTextStyle [in]  Pointer to the TextStyle object associated with the specified text.

    \returns
    Returns a SmartPointer to the new OdDbTextIterator object.
  */
  static OdDbTextIteratorPtr createObject(const OdChar* textString, 
                                          int length, 
                                          bool raw, 
                                          OdCodePageId codePageId, 
                                          const OdGiTextStyle* pTextStyle);
};

#include "TD_PackPop.h"

#endif
