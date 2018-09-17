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

#ifndef _ODEDUSERIO_H_INCLUDED_
#define _ODEDUSERIO_H_INCLUDED_

#include "OdPlatform.h"
#include "Gi/GiDrawable.h"
#include "OdString.h"
#include "SharedPtr.h"
#include "RxObject.h"

class OdGePoint3d;

#include "TD_PackPush.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace OdEd
{
  enum CommonInputOptions
  {
    kInpDefault       = 0x00000000,
    kInpDisallowEmpty = kInpDefault,
    kInpDisallowOther = kInpDisallowEmpty,
    kInpThrowEmpty    = (long)0x80000000,
    kInpThrowOther    = 0x40000000,
    kInpNonZero       = 0x20000000, // Disallow zero input 
    kInpNonNeg        = 0x10000000  // Disallow negative input 
  };

  enum GetFilePathFlags
  {
    kGfpForOpen         = 0, // For Open.
    kGfpForSave         = 1, // For Save.
    kGfpOverwritePrompt = 2  // Enable overwrite prompt displaying when exist file selected for saving.
  };

  enum GetStringOptions
  {
    kGstDefault      = 0, // Spaces disallowed, no KeywordExceptions
    kGstNoSpaces     = 0, // Spaces disallowed
    kGstAllowSpaces  = 1, // The first space character will not terminate the input.
    kGstNoEmpty      = 2  // The first space character will not terminate the input.
  };

  enum OdDbInputOptions
  {
    kGptDefault         = 0x00000000,
    kGdsDefault         = kGptDefault,
    kGinDefault         = kGptDefault,
    kGrlDefault         = kGptDefault,
    kGanDefault         = kGptDefault,
    kGptNoLimCheck      = 0x00000001, // Ignore the LIMMIN and LIMMAX system variables
    kGptNoUCS           = 0x00000002, // Do not project result point onto UCS plane (input point in eye plane)
    kGptRubberBand      = 0x00000004,
    kGptRectFrame       = 0x00000008,
    kGptBeginDrag       = 0x00000010,
    kGptEndDrag         = 0x00000020,
    kGptNoOSnap         = 0x00000040, // Disables object snaps
    kGanFromLastPoint   = 0x00000080,
    kGdsFromLastPoint   = kGanFromLastPoint,
    kGanNoAngBase       = 0x00000100,
    kGds2d              = 0x00000200, // Restrict (getDistance) to 2D
    kGdsSignedDist      = 0x00000400,
    kGdsPerpDist        = 0x00000800, // is in perpendicular plane to UCS plane
    kGdsNoZero          = 0x00001000,
    kGinNoZero          = kGdsNoZero,
    kGrlNoZero          = kGdsNoZero,
    kGanNoZero          = kGdsNoZero,
    kGdsNoNeg           = 0x00002000,
    kGinNoNeg           = kGdsNoNeg,
    kGrlNoNeg           = kGdsNoNeg
  };

  enum SelectOptions
  {
    kSelDefault         = 0x00000000,
    kSelPickLastPoint   = 0x00000001,  // Use last entered point as first corner of selection box
    kSelSingleEntity    = 0x00000002,  // Select single entity
    kSelIgnorePickFirst = 0x00000004,  // Clear previous contents of selection set
    kSelSinglePass      = 0x00000008,  // Use single pass for selection
                                       // (no pressing Enter is required to finish selection)
    kSelAllowEmpty      = 0x00000010,  // Empty selection set is allowed
    kSelRemove          = 0x00000020,  // Remove selected entities from current selection set
    kSelLeaveHighlighted= 0x00000040,  // Leave entities highlighted after operation
    kSelAllowInactSpaces= 0x00000080,  // Allow selection of all entities not only in active layout
    
    kSelAllowObjects    = 0x00000100,  // Allow non-entity objects selection
    kSelAllowPSVP       = 0x00000200,  // Allow overall Paper Space viewport selection
    kSelAllowSubents    = 0x00000400,  // Allow subentities selection
    kSelAllowLocked     = 0x00000800,   // Allow selection of entities on locked layers

    kSelAllowSubentsAlways    = 0x00001000,  // Allow subentities selection independent on press CTRL
  };
}

class OdGsView;
class OdGsModel;

class OdCmColorBase;

//
// OdEd_Classes
//

/** \details
    This class allows commands to track object
    changes and redraw the entities while user sets parameter values.

    Corresponding C++ library: TD_Root
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdInputTracker : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdEdInputTracker);

  virtual int addDrawables(OdGsView* pView) = 0;
  virtual void removeDrawables(OdGsView* pView) = 0;
};

typedef OdSmartPtr<OdEdInputTracker> OdEdInputTrackerPtr;


/** \details
    This notification interface template allows commands to track object
    changes and redraw the entities while user sets parameter values.

    Corresponding C++ library: TD_Root
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdIntegerTracker : public OdEdInputTracker
{
public:
  ODRX_DECLARE_MEMBERS(OdEdIntegerTracker);

  virtual void setValue(int value) = 0;
};

typedef OdSmartPtr<OdEdIntegerTracker> OdEdIntegerTrackerPtr;


/** \details
    This notification interface template allows commands to track object
    changes and redraw the entities while user sets parameter values.

    Corresponding C++ library: TD_Root
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdRealTracker : public OdEdInputTracker
{
public:
  ODRX_DECLARE_MEMBERS(OdEdRealTracker);

  virtual void setValue(double value) = 0;
};

typedef OdSmartPtr<OdEdRealTracker> OdEdRealTrackerPtr;


/** \details
    This notification interface template allows commands to track object
    changes and redraw the entities while user sets parameter values.

    Corresponding C++ library: TD_Root
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdStringTracker : public OdEdInputTracker
{
protected:
  bool bShowCursor;
public:
  ODRX_DECLARE_MEMBERS(OdEdStringTracker);

  virtual void setValue(const OdString& value) = 0;

  virtual void setCursor(const bool& bShowCursor)
  {
    this->bShowCursor = bShowCursor;
  }
};

typedef OdSmartPtr<OdEdStringTracker> OdEdStringTrackerPtr;


/** \details
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdPointTracker : public OdEdInputTracker
{
public:
  ODRX_DECLARE_MEMBERS(OdEdPointTracker);

  virtual void setValue(const OdGePoint3d& value) = 0;
};

typedef OdSmartPtr<OdEdPointTracker> OdEdPointTrackerPtr;


/** \details
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdPointDefTracker : public OdEdPointTracker
{
public:
  ODRX_DECLARE_MEMBERS(OdEdPointDefTracker);

  virtual OdGePoint3d basePoint() const = 0;
};

typedef OdSmartPtr<OdEdPointDefTracker> OdEdPointDefTrackerPtr;


/** \details
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdColorTracker : public OdEdInputTracker
{
public:
  ODRX_DECLARE_MEMBERS(OdEdColorTracker);

  virtual void setValue(const OdCmColorBase& value) = 0;
};

typedef OdSmartPtr<OdEdColorTracker> OdEdColorTrackerPtr;


//
// OdEd_Exceptions
//

#define OdException OdError


/** \details
    This class will be thrown whenever the user cancels input from query function of this UserIO object.

    Corresponding C++ library: TD_Root
    <group OdEd_Exceptions>
*/
class FIRSTDLL_EXPORT OdEdException : public OdException
{
protected:
  OdEdException();
  OdEdException(OdErrorContext* pErrCtx);
};


/** \details
    This class will be thrown whenever the user cancels input from query function of this UserIO object.

    Corresponding C++ library: TD_Root
    <group OdEd_Exceptions>
*/
class FIRSTDLL_EXPORT OdEdCancel : public OdEdException
{
public:
  OdEdCancel();
  OdEdCancel(OdErrorContext* pErrCtx);
};


/** \details
    This class will be thrown whenever a keyword is input to query function of this UserIO object.

    Corresponding C++ library: TD_Root
    <group OdEd_Exceptions>
*/
class FIRSTDLL_EXPORT OdEdKeyword : public OdEdException
{
public:
  OdEdKeyword(int nKeyword, const OdString& sKeyword);

  int keywordIndex() const;
  const OdString& keyword() const;
};


/** \details

    Corresponding C++ library: TD_Root
    <group OdEd_Exceptions>
*/
class FIRSTDLL_EXPORT OdEdOtherInput : public OdEdCancel
{
public:
  OdEdOtherInput(const OdString& sInput);
  const OdString& string() const;
};


/** \details

    Corresponding C++ library: TD_Root
    <group OdEd_Exceptions>
*/
class FIRSTDLL_EXPORT OdEdEmptyInput : public OdEdCancel
{
public:
  OdEdEmptyInput();
};


/** \details
    This class implements non-geometric interactive user IO functionality for custom interactive command objects.
    
    Corresponding C++ library: TD_Root
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdUserIO : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEdUserIO);

  /** \details
    Returns true if this is manual user input not batch invocation
  */
  virtual bool interactive();

  /** \details
    Returns user input of a keyword.
 
    \param prompt [in]  Prompt output before pausing for user input.
    \param keywords [in]  Keyword list.
    \param options [in]  Bitwise OR combination of OdEd::CommonInputOptions enum.
    \param defVal [in]  Returned if user input nothing.

    \returns
    Returns the index of the entered keyword in the keyword list, or -1 it's not there.
    \remarks
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.  
    
    \remarks
    This function does not throw a KeywordException.
  */
  virtual int getKeyword(
    const OdString& prompt,
    const OdString& keywords,
    int defVal = -1,
    int options = OdEd::kInpDefault,
    OdEdIntegerTracker* pTracker = 0) = 0;

  /** \details
    Returns user input of an integer.
 
    \param prompt [in]  Prompt output before pausing for user input.
    \param pTracker [in/out] Pointer to an IO Tracker for this user input.
    \param options [in] Bitwise OR combination of OdEd::CommonInputOptions enum.
    \param defVal [in] Returned if user input nothing and OdEd::kInpReturnDefVal flag is set.
    
    \remarks
    If prompt is empty, a default prompt is used.    
    
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.  
  */
  virtual int getInt(
    const OdString& prompt,
    int options = OdEd::kInpDefault,
    int defVal = 0,
    const OdString& keywords = OdString::kEmpty,
    OdEdIntegerTracker* pTracker = 0) = 0;

  /** \details
    Returns user input of a double.
 
    \param prompt [in]  Prompt output before pausing for user input.
    \param options [in]  Bitwise OR combination of OdEd::CommonInputOptions enum.
    \param defVal [in]  Returned if user input nothing and OdEd::kInpReturnDefVal flag is set.
    \param pTracker [in]  Pointer to an IO Tracker for this user input.
    
    \remarks
    If prompt is empty, a default prompt is used.    
    
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.  
  */
  virtual double getReal(
    const OdString& prompt, 
    int options = OdEd::kInpDefault,
    double defVal = 0.0,
    const OdString& keywords = OdString::kEmpty,
    OdEdRealTracker* pTracker = 0) = 0;

  /** \details
    Returns user input of a string.
 
    \param options [in]  combination of OdEd::CommonInputOptions and OdEd::GetStringOptions enum values.
    \param prompt [in]  Prompt output before pausing for user input.
    \param pTracker [in]  Pointer to an IO Tracker for this user input.
    \param defValue [in]  Default value.
    \param keywords [in]  Keyword string.
    
    \remarks
    If prompt is empty, a default prompt is used.    
    
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.  
  */
  virtual OdString getString(
    const OdString& prompt, 
    int options = OdEd::kInpDefault, 
    const OdString& defValue = OdString::kEmpty,
    const OdString& keywords = OdString::kEmpty,
    OdEdStringTracker* pTracker = 0) = 0;
                                 
  /** \details
    Writes the specified string to the command's output.
    \param string [in]  string to write.
  */
  virtual void putString(const OdString& string) = 0;

  /** \details
    Returns user input of a color as wrapped value.

    \param prompt [in]  Message displayed to prompt the user.
    \param options [in]  Combination of OdEd::GetPointOptions enum values. 
    \param pDefaultValue [in]  Pointer to a default value to return if an empty string is received as input.
    \param keywords [in]  Keyword string.
    \param pTracker [in]  Pointer to a User IO tracker.
    
    \remarks
    This function uses the underlying OdEdBaseIO functions putString and getString for low
    level output and input.
    
    If prompt is an empty string, a default prompt string is used.  
  */
  virtual OdSharedPtr<OdCmColorBase> getCmColor(
    const OdString& prompt,
    int options = OdEd::kInpDefault,
    const OdCmColorBase* pDefaultValue = 0,
    const OdString& keywords = OdString::kEmpty,
    OdEdColorTracker* pTracker = 0) = 0;

  /** \details
    Returns user input of a filename obtained in some manner.

    \param options [in]  combination of OdEd::CommonInputOptions and OdEd::GetFilePathFlags enum values.
    \param prompt [in]  Prompt output before pausing for user input.
    \param dialogCaption [in]  Dialog caption.
    \param defExt [in]  Default file extension without "."
    \param fileName [in]  Initial filename to initialize dialog with.
    \param filter [in]  Filter pattern.
    \param pTracker [in/out] File path tracker; may be used to display file preview.
    
    \remarks
    flags must be one of the following:

    <table>
    Name                        Value   Description
    OdEd::kGfpForOpen           0       For Open.
    OdEd::kGfpForSave           1       For Save.
    OdEd::kGfpOverwritePrompt   2       Enable overwrite prompt displaying when exist file selected for saving.
    </table>

    The default implementation of this function utilizes getString().  
  */
  virtual OdString getFilePath(
      const OdString& prompt, 
      int options = OdEd::kGfpForOpen,
      const OdString& dialogCaption = OdString::kEmpty, 
      const OdString& defExt = OdString::kEmpty,
      const OdString& fileName = OdString::kEmpty,
      const OdString& filter = OdString::kEmpty,
      const OdString& keywords = OdString::kEmpty,
      OdEdStringTracker*  pTracker = 0);

  /** \details
    Writes the specified *error message* to the command's error output.
    Override it to distinguish normal output of a command from its error messages.
    \param errmsg [in]  error message to write.
  */
  virtual void putError(const OdString& errmsg) { putString(errmsg); }
};

#include "TD_PackPop.h"

#endif //#ifndef _ODEDUSERIO_H_INCLUDED_
