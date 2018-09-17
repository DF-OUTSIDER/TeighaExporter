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



#ifndef PDF_ERROR_MESSAGE_DEFS
#define PDF_ERROR_MESSAGE_DEFS(a, b)
#endif

  PDF_ERROR_MESSAGE_DEFS(eOk                       , OD_T("No error."));
  PDF_ERROR_MESSAGE_DEFS(eInternalError            , OD_T("Unknown exception in pdf export."));
  PDF_ERROR_MESSAGE_DEFS(eEmptyInputArray          , OD_T("Array is empty."));
  PDF_ERROR_MESSAGE_DEFS(eNotSortedArray           , OD_T("Array must be sorted."));
  PDF_ERROR_MESSAGE_DEFS(eDuplicatedElement        , OD_T("Element is duplicated."));
  PDF_ERROR_MESSAGE_DEFS(eNullDocument             , OD_T("Document of OCG is null."));
  PDF_ERROR_MESSAGE_DEFS(eKeyNotFound              , OD_T("There is no requested key in the dictionary."));
  PDF_ERROR_MESSAGE_DEFS(eNullBaseName             , OD_T("Base name for the unique property is not defined."));
  PDF_ERROR_MESSAGE_DEFS(eNotImplementedYet        , OD_T("This way is not implemented yet."));

#undef PDF_ERROR_MESSAGE_DEFS
