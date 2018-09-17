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

#ifndef _ExBaseIO_h_Included_
#define _ExBaseIO_h_Included_

#include "RxObject.h"
#include "Ed/EdUserIO.h"
#include "Ge/GePoint3d.h"


/** \details
    This class implements the base user I/O for custom commands.
    <group ExServices_Classes> 
    Library: Source code provided. 
*/
class OdEdBaseIO : public OdRxObject
{
public:
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
  virtual OdString getString(const OdString& prompt, int options = OdEd::kInpDefault, OdEdStringTracker* pTracker = 0) = 0;

  /** \details
    Writes the specified string to the command's output.
    \param string [in]  string to write.
  */
  virtual void putString(const OdString& string) = 0;
  
  /** \details
    Returns user input of a point.

    \param prompt [in]  Message displayed to prompt the user.
    \param options [in]  Combination of OdEd::GetPointOptions enum values. 
    \param pTracker [in]  Pointer to a User IO tracker.

    \remarks
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.
    
    If prompt is an empty string, a default prompt string is used.  
  */
  virtual OdGePoint3d getPoint(const OdString& prompt, int options = OdEd::kInpDefault, OdEdPointTracker* pTracker = 0)
  {
    throw OdEdOtherInput(getString(prompt, OdEd::kGstDefault, 0));
  }

  enum MouseEventFlags
  {
    kLeftButtonIsDown   = 0x0001,
    kRightButtonIsDown  = 0x0002,
    kShiftIsDown        = 0x0004,
    kControlIsDown      = 0x0008,
    kMiddleButtonIsDown = 0x0010
  };

  /** \details
  Returns combination of "MouseEventFlags" representing currently pressed functional keys
  Currently used are:
  Shift = 4 (MK_SHIFT)
  Ctrl = 8 (MK_CONTROL)
  */
  virtual OdUInt32 getKeyState() { return 0; }

  /** \details
    Writes the specified errmsg to the error stream.
    Override it to distinguish normal output of a command from its error messages.
    \param errmsg [in]  error message to write.
  */
  virtual void putError(const OdString& errmsg) { putString(errmsg); }

  /** \details
    Returns true if this is a manual user input not batch invocation
  */
  virtual bool interactive() const { return false; }
};

#endif // _ExBaseIO_h_Included_
