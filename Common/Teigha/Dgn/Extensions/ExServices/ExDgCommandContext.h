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

#ifndef _ExDg_CommandContext_h_Included_
#define _ExDg_CommandContext_h_Included_

#include "DgUserIO.h"
#include "Ed/EdFunctionIO.h"
#include "DgSSet.h"
#include "DgCommandContext.h"
//#include "DbUnitsFormatterImpl.h"
//#include "StaticRxObject.h"
#include "ExEdBaseIO.h"
//#include "ExKWIndex.h"
#include "ResBuf.h"

class OdDgUnitsFormatter;

/** \details
  <group Dg_Extension_Classes> 

  This class implements the base user I/O for custom commands.

  Library: Source code provided. 
*/
//class OdEdBaseIO : public OdRxObject
//{
//public:
//  /** Description:
//    Returns user input of a string.
// 
//    Arguments:
//    options (I) Combination of OdEd::CommonInputOptions and OdEd::GetStringOptions enum values.
//    prompt (I) Prompt output before pausing for user input.
//    pTracker (I) Pointer to an IO Tracker for this user input.
//    
//    Remarks:
//    If prompt is empty, a default *prompt* is used.    
//    
//    This function uses the underlying OdEdBaseIO function putString for low
//    level output.  
//  */
//  virtual OdString getString(const OdString& prompt, int options = OdEd::kInpDefault, OdEdStringTracker* pTracker = 0) = 0;
//
//  /** Description:
//    Writes the specified *string*.
//    Arguments:
//    string (O) Output *string*.
//  */
//  virtual void putString(const OdString& string) = 0;
//  
//  /** Description:
//    Returns user input of a point.
//
//    Arguments:
//    prompt (I) Message displayed to *prompt* the user.
//    options (I) Combination of OdEd::GetPointOptions enum values. 
//    pTracker (I) Pointer to a User IO tracker.
//
//    Remarks:
//    This function uses the underlying OdEdBaseIO functions putString and getString for low
//    level output and input.
//    
//    If *prompt* is an empty string, a default *prompt* string is used.  
//  */
//  virtual OdGePoint3d getPoint(const OdString& prompt, int options = OdEd::kInpDefault, OdEdPointTracker* pTracker = 0);
//
//  enum MouseEventFlags
//  {
//    kLeftButtonIsDown   = 0x0001,
//    kRightButtonIsDown  = 0x0002,
//    kShiftIsDown        = 0x0004,
//    kControlIsDown      = 0x0008,
//    kMiddleButtonIsDown = 0x0010
//  };
//
//  /** Description:
//  Returns combination of "MouseEventFlags" representing currently pressed functional keys
//  Currently used are:
//  Shift = 4 (MK_SHIFT)
//  Ctrl = 8 (MK_CONTROL)
//  */
//  virtual OdUInt32 getKeyState() = 0;
//};


/** \details
  <group Dg_Extension_Classes> 

  This class implements the interface for I/O and database access 
  for custom commands during their execution.
*/
class ExDgCommandContext : public OdDgCommandContext
                         , protected OdDgUserIO
                         , protected OdEdFunctionIO
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdDgCommandContext);

  OdSmartPtr<OdEdBaseIO>      m_pIoStream;
  OdDgDatabase*               m_pDb;
  OdGePoint3d                 m_LASTPOINT;
  mutable OdRxDictionaryPtr   m_pArbDataDic;

  /** \details
    Returns a SmartPointer to the arbitrary data dictionary for this CommandContext object.
    \remarks
    A dictionary is created if a dictionary does not exist.
  */
  OdRxDictionaryPtr arbDataDic() const;

  /** \details
    Returns input of a string from the StreamBuf associated with this CommandContext object.
 
    \param options [in]  Combination of OdEd::CommonInputOptions and OdEd::GetStringOptions enum values.
    \param prompt [in]  Prompt output before pausing for user input.
    \param pTracker [in]  Pointer to an IO Tracker for this user input.
    
    \remarks
    If prompt is empty, a default prompt is used.    
    
    This function uses the underlying OdEdBaseIO function putString for low
    level output.  
  */
  OdString getStringInternal(const OdString& prompt, int options = OdEd::kInpDefault, OdEdStringTracker* pTracker = 0);

  struct RealInput
  {
    double val;
  };

  /** \details
    Returns input of a point or a real from the StreamBuf associated with this CommandContext object.
 
    \param options [in]  Combination of OdEd::CommonInputOptions and OdEd::GetStringOptions enum values.
    \param prompt [in]  Prompt output before pausing for user input.
    \param pPtTracker [in]  Pointer to a Point Tracker for this user input.
    \param keywords [in]  Keyword string.
    \param realType [in]  Type of real.
    
    \remarks
    If prompt is empty, a default prompt is used.
    
    realType will be one of the following:
    
    <table>
    Name
    OdResBuf::kDxfXdDist
    OdResBuf::kRtOrient   
    </table>
    
    This function uses the underlying OdEdBaseIO functions putString and getPoint for low
    level output.  
  */
  OdGePoint3d getPointOrReal(const OdString& prompt,
                             int options,
                             const OdString& keywords,
                             OdResBuf::ValueType realType,
                             OdEdPointTracker* pPtTracker);

  double getReal(const OdString& prompt,
                 int options,
                 double defValue,
                 OdResBuf::ValueType realType,
                 const OdString& keywords,
                 OdEdRealTracker* pTracker);

  /** \details
    Returns the selection set specified by the user with a box.
    \param firstCorner [in]  First corner of the box.
  */
  OdDgSelectionSetPtr box(OdGsView* pGsView, const OdGePoint3d& firstCorner, OdDbVisualSelection::SubentSelectionMode ssm);

  /** \details
    Returns true if and only if specified string is an AutoLISP (handent) expression.
    \remarks
    In order to return true, input must be in the form
    
            (handent "<handleString>")
            
    If true is returned, the result object is set to the specified handle.
           
    \param input [in]  String to try.
  */
  virtual bool try_lisp(const OdString& input);

protected:
  

  // Leave this here--DO NOT move it out of the class definition.  Moving it 
  // outside causes an "internal compiler error" in CodeWarrior 8 & 9 on the mac.

  ExDgCommandContext()
    : m_pDb(0)
  {
  }

public:
  /** \details
    Creates an instance of this CommandContext object.
    \param pStreamBuf [in]  Pointer to the the stream buffer.
    \param pDb [in]  Pointer to the database.
    \remarks
    Returns a SmartPointer to the newly created object.
  */
  static OdDgCommandContextPtr createObject(OdEdBaseIO* pIOStream, OdDgDatabase* pDb);
  OdRxObject* baseDatabase();
  
  OdEdUserIO* userIO() { return this; }
  OdEdFunctionIO* funcIO() { return this; }

  /** \details
    Sets the parameter object for this CommandContext object.
    \param pParamObj [in]  Pointer to the parameter object.
  */
  void setParam(OdRxObject* pParamObj);
  /** \details
    Returns the parameter object for this CommandContext object.
  */
  OdRxObjectPtr param();
  /** \details
    Sets the result object for this CommandContext object.
    \param pResultObj [in]  Pointer to the result object.
  */
  void setResult(OdRxObject* pResultObj);
  /** \details
    Returns the result object for this CommandContext object.
  */
  OdRxObjectPtr result();
  
  /** \details
    Writes arbitrary data to the specified file with this CommandContext object.
    \param fileName [in] Filename.
    \param pDataObj [in] Pointer to the data object.
  */
  void setArbitraryData(const OdString& fileName, OdRxObject* pDataObj);
  /** \details
    Reads arbitrary data from the specified file with this CommandContext object.
    \param fileName [in] Filename.
    \remarks
    Returns a SmartPointer to the data object.
  */
  OdRxObjectPtr arbitraryData(const OdString& fileName) const;

  /** \details
    Returns a selection set containing pickfirst data with this CommandContext object.
  */
  OdDgSelectionSetPtr pickfirst();
  /** \details
    Sets the pickfirst selection set for this CommandContext object.
    \param pSSet [in]  Pointer to the pickfirst selection set.
  */
  void setPickfirst(OdDgSelectionSet* pSSet);

  /** \details
    Returns a selection set containing previous selection data with this CommandContext object.
  */
  OdDgSelectionSetPtr previousSelection();
  /** \details
    Sets the previous selection set for this CommandContext object.
    \param pSSet [in]  Pointer to the previous selection set.
  */
  void setPreviousSelection(OdDgSelectionSet* pSSet);

  // OdEdUserIO interface:

  virtual OdUnitsFormatter& baseFormatter();
  OdDgUnitsFormatter& formatter();

  void putString(const OdString& string);

  int getInt(const OdString& prompt,
             int options = OdEd::kInpDefault,
             int defVal = 0,
             const OdString& keywords = OdString::kEmpty,
             OdEdIntegerTracker* pTracker = 0);

  double getReal(const OdString& prompt, 
                 int options = OdEd::kInpDefault,
                 double defVal = 0.0,
                 const OdString& keywordList = OdString::kEmpty,
                 OdEdRealTracker* pTracker = 0);

  OdString getString(const OdString& prompt, 
                     int options = OdEd::kInpDefault, 
                     const OdString& pDefVal = OdString::kEmpty,
                     const OdString& keywords = OdString::kEmpty,
                     OdEdStringTracker* pTracker = 0);

  int getKeyword(const OdString& prompt,
                 const OdString& keywords,
                 int defRes = -1,
                 int options = OdEd::kInpDefault,
                 OdEdIntegerTracker* pTracker = 0);

  double getAngle(const OdString& prompt,
                  int options = OdEd::kInpDefault,
                  double defVal = 0.0,
                  const OdString& keywords = OdString::kEmpty,
                  OdEdRealTracker* pTracker = 0);

  /** \details
    Returns the LASTPOINT system variable for this CommandContext object.
  */
  OdGePoint3d getLASTPOINT() const;

  /** \details
    Sets the LASTPOINT system variable for this CommandContext object.
    \param val [in]  New value for LASTPOINT. 
  */
  void setLASTPOINT(const OdGePoint3d& val);

  /** \details
    Creates a rubberband line from the specified point to the user input.
    \param base [in] Base point.
  */
  OdEdPointDefTrackerPtr createRubberBand(const OdGePoint3d& base, OdGsModel* pModel = NULL) const;
  /** \details
    Creates a rectangular frame from the specified point to the user input.
    \param base [in] Base point.
  */
  OdEdPointDefTrackerPtr createRectFrame(const OdGePoint3d& base, OdGsModel* pModel = NULL) const;

  OdGePoint3d getPoint(const OdString& prompt,
                       int options = OdEd::kGptDefault,
                       const OdGePoint3d* pDefVal = 0,
                       const OdString& keywords = OdString::kEmpty,
                       OdEdPointTracker* = 0);

  double getDist(const OdString& prompt,
                 int options = OdEd::kInpDefault,
                 double defVal = 0.0,
                 const OdString& keywords = OdString::kEmpty,
                 OdEdRealTracker* pTracker = 0);

  OdString getFilePath(const OdString& prompt,
                       int options,
                       const OdString& dialogCaption, 
                       const OdString& defExt,
                       const OdString& fileName,
                       const OdString& filter,
                       const OdString& keywords = OdString::kEmpty,
                       OdEdStringTracker* pTracker = 0);

  OdDgCmColor getColor(const OdString& prompt,
                       int options = OdEd::kInpDefault,
                       const OdDgCmColor* pDefVal = 0,
                       const OdString& keywordList = OdString::kEmpty,
                       OdEdColorTracker* pTracker = 0);

  OdDgSelectionSetPtr select(OdGsView* pGsView, 
                             const OdString& prompt = OdString::kEmpty,
                             int options = OdEd::kSelDefault,
                             const OdDgSelectionSet* pDefVal = 0,
                             const OdString& keywords = OdString::kEmpty,
                             OdEdDgSSetTracker* pTracker = 0);
};

#endif // _ExDg_CommandContext_h_Included_
