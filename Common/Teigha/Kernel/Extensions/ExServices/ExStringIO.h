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

#ifndef _ExStringIO_h_Included_
#define _ExStringIO_h_Included_

#include "TD_PackPush.h"

#include "ExEdBaseIO.h"

/** \details
    This class implements base-level string I/O 
    for custom commands during their execution.
    <group ExServices_Classes> 
    Library: Source code provided.
*/
class ExStringIO : public OdEdBaseIO
{
protected:
  OdString m_sInput;
  OdSmartPtr<ExStringIO> init(const OdString& sInput);
  ExStringIO() {}
public:
  /*!DOM*/
  void reset(const OdString& sInput);
  /*!DOM*/
  const OdString& getInitString() const;

  /** \details
    Creates a new instance of this class.
    \param string [in]  String.
    \remarks
    Returns a SmartPointer to the newly created instance.
  */
  static OdSmartPtr<ExStringIO> create(const OdString& filename);

  /** \details
    Returns user input of a string.
 
    \param options [in]  Combination of OdEd::CommonInputOptions and OdEd::GetStringOptions enum values.
    \param prompt [in]  Prompt output before pausing for user input.
    \param pTracker [in]  Pointer to an IO Tracker for this user input.
    
    \remarks
    If prompt is empty, a default prompt is used.    
    
    This function uses the underlying OdEdBaseIO function putString for low
    level output.  
  */
  OdString getString(const OdString& prompt, int options = OdEd::kInpDefault, OdEdStringTracker* pTracker = 0);
  /** \details
    Writes the specified string to the data source associated with this IO User object.
    \param string [out]  Output string.
  */
  void putString(const OdString& string);
  /** \details
    Returns true if and only if the current position of the file pointer for this StringIO object is at the end of file.
  */
  bool isEof() const { return m_sInput.isEmpty(); }
  virtual OdUInt32 getKeyState(){ return 0;}
};

#include "TD_PackPop.h"

#endif //_ExStringIO_h_Included_
