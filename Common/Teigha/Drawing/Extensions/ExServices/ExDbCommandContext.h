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

#ifndef _ExDbCommandContext_h_Included_
#define _ExDbCommandContext_h_Included_

#include "TD_PackPush.h"

#include "DbUserIO.h"
#include "Ed/EdFunctionIO.h"
#include "DbSSet.h"
#include "DbCommandContext.h"
#include "DbUnitsFormatterImpl.h"
#include "StaticRxObject.h"
#include "ExEdBaseIO.h"
//#include "ExKWIndex.h"


/** \details
    This class implements the interface for I/O and database access 
    for custom commands during their execution.

    <group ExServices_Classes> 
    Library: Source code provided. 
*/
class ExDbCommandContext : public OdDbCommandContext
                         , protected OdDbUserIO
                         , protected OdEdFunctionIO
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdDbCommandContext);

  OdSmartPtr<OdEdBaseIO>      m_pIoStream;
  OdRxObject*                 m_pDb;
  OdGePoint3d                 m_LASTPOINT;
  mutable OdRxDictionaryPtr   m_pArbDataDic;
  ExDbCommandContext*         m_pOwner;

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
  virtual OdSelectionSetPtr box(const OdGePoint3d& firstCorner, OdDbVisualSelection::SubentSelectionMode ssm);

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

  ExDbCommandContext()
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
  static OdDbCommandContextPtr createObject(OdEdBaseIO* pIOStream, OdDbDatabase* pDb);
  static OdDbCommandContextPtr createObject(OdEdBaseIO* pIOStream, OdRxObject* pRxDb = 0);
  OdRxObject* baseDatabase();

  /*!DOM*/
  void reset(OdEdBaseIO* pIOStream, OdRxObject* pRxDb);
  /*!DOM*/
  OdEdBaseIO* baseIO();
  /*!DOM*/
  virtual OdEdCommandContextPtr cloneObject(OdEdBaseIO* pIOStream = NULL, 
                                            OdRxObject* pRxDb = NULL);
  
  OdEdUserIO* userIO() { return this; }
  OdEdFunctionIO* funcIO() { return this; }

  /** \details
    Sets the parameter object for this CommandContext object.
    \param pParamObj [in]  Pointer to the parameter object.
  */
  virtual void setParam(OdRxObject* pParamObj);
  /** \details
    Returns the parameter object for this CommandContext object.
  */
  virtual OdRxObjectPtr param();
  /** \details
    Sets the result object for this CommandContext object.
    \param pResultObj [in]  Pointer to the result object.
  */
  virtual void setResult(OdRxObject* pResultObj);
  /** \details
    Returns the result object for this CommandContext object.
  */
  virtual OdRxObjectPtr result();
  
  /** \details
    Writes arbitrary data to the specified file with this CommandContext object.
    \param fileName [in] Filename.
    \param pDataObj [in] Pointer to the data object.
  */
  virtual void setArbitraryData(const OdString& fileName, OdRxObject* pDataObj);
  /** \details
    Reads arbitrary data from the specified file with this CommandContext object.
    \param fileName [in] Filename.
    \remarks
    Returns a SmartPointer to the data object.
  */
  virtual OdRxObjectPtr arbitraryData(const OdString& fileName) const;

  /** \details
    Returns a selection set containing pickfirst data with this CommandContext object.
  */
  virtual OdSelectionSetPtr pickfirst();
  /** \details
    Sets the pickfirst selection set for this CommandContext object.
    \param pSSet [in]  Pointer to the pickfirst selection set.
  */
  virtual void setPickfirst(OdSelectionSet* pSSet);

  /** \details
    Returns a selection set containing previous selection data with this CommandContext object.
  */
  virtual OdSelectionSetPtr previousSelection();
  /** \details
    Sets the previous selection set for this CommandContext object.
    \param pSSet [in]  Pointer to the previous selection set.
  */
  virtual void setPreviousSelection(OdSelectionSet* pSSet);

  // OdEdUserIO interface:

  virtual OdUnitsFormatter& baseFormatter();
  virtual OdDbUnitsFormatter& formatter();

  virtual void putString(const OdString& string);

  virtual int getInt(const OdString& prompt,
                     int options = OdEd::kInpDefault,
                     int defVal = 0,
                     const OdString& keywords = OdString::kEmpty,
                     OdEdIntegerTracker* pTracker = 0);

  virtual double getReal(const OdString& prompt, 
                         int options = OdEd::kInpDefault,
                         double defVal = 0.0,
                         const OdString& keywordList = OdString::kEmpty,
                         OdEdRealTracker* pTracker = 0);

  virtual OdString getString(const OdString& prompt, 
                             int options = OdEd::kInpDefault, 
                             const OdString& pDefVal = OdString::kEmpty,
                             const OdString& keywords = OdString::kEmpty,
                             OdEdStringTracker* pTracker = 0);

  virtual int getKeyword(const OdString& prompt,
                         const OdString& keywords,
                         int defRes = -1,
                         int options = OdEd::kInpDefault,
                         OdEdIntegerTracker* pTracker = 0);

  virtual double getAngle(const OdString& prompt,
                          int options = OdEd::kInpDefault,
                          double defVal = 0.0,
                          const OdString& keywords = OdString::kEmpty,
                          OdEdRealTracker* pTracker = 0);

  /** \details
    Returns the LASTPOINT system variable for this CommandContext object.
  */
  virtual OdGePoint3d getLASTPOINT() const;

  /** \details
    Sets the LASTPOINT system variable for this CommandContext object.
    \param val [in]  New value for LASTPOINT. 
  */
  virtual void setLASTPOINT(const OdGePoint3d& val);

  /** \details
    Creates a rubberband line from the specified point to the user input.
    \param base [in] Base point.
  */
  virtual OdEdPointDefTrackerPtr createRubberBand(const OdGePoint3d& base,
                                                  OdGsModel* pModel = NULL) const;
  /** \details
    Creates a rectangular frame from the specified point to the user input.
    \param base [in] Base point.
  */
  virtual OdEdPointDefTrackerPtr createRectFrame(const OdGePoint3d& base,
                                                 OdGsModel* pModel = NULL) const;

  virtual OdGePoint3d getPoint(const OdString& prompt,
                               int options = OdEd::kGptDefault,
                               const OdGePoint3d* pDefVal = 0,
                               const OdString& keywords = OdString::kEmpty,
                               OdEdPointTracker* = 0);

  virtual double getDist(const OdString& prompt,
                         int options = OdEd::kInpDefault,
                         double defVal = 0.0,
                         const OdString& keywords = OdString::kEmpty,
                         OdEdRealTracker* pTracker = 0);

  virtual OdString getFilePath(const OdString& prompt,
                               int options,
                               const OdString& dialogCaption, 
                               const OdString& defExt,
                               const OdString& fileName,
                               const OdString& filter,
                               const OdString& keywords = OdString::kEmpty,
                               OdEdStringTracker* pTracker = 0);

  virtual OdSharedPtr<OdCmColorBase> getCmColor(const OdString& sPrompt,
                                                int options = OdEd::kInpDefault,
                                                const OdCmColorBase* pDefVal = 0,
                                                const OdString& sKeywords = OdString::kEmpty,
                                                OdEdColorTracker* pTracker = 0);

  virtual OdCmColor getColor(const OdString& prompt,
                             int options = OdEd::kInpDefault,
                             const OdCmColor* pDefVal = 0,
                             const OdString& keywordList = OdString::kEmpty,
                             OdEdColorTracker* pTracker = 0);

  virtual OdSelectionSetPtr select(const OdString& prompt = OdString::kEmpty,
                                   int options = OdEd::kSelDefault,
                                   const OdSelectionSet* pDefVal = 0,
                                   const OdString& keywords = OdString::kEmpty,
                                   OdSSetTracker* pTracker = 0,
                                   OdGePoint3dArray* ptsPointer = NULL);

  //void executeString(OdEdBaseIO* pIoStream, bool bEchoCmdToOutput);
protected:
  virtual void merge(OdSelectionSet* pRes,
                     int options,
                     const OdSelectionSet* pSSet,
                     OdSSetTracker* pTracker,
                     OdEdUserIO* pIO = NULL) const;
  virtual bool isPickadd() const;
  virtual void highlight(OdSelectionSet* pSSet, 
                         bool bDoIt = true) const;
  virtual void highlight(OdSelectionSetIterator* pIter, 
                         bool bDoIt = true) const;
  virtual bool interactive();
};

#include "TD_PackPop.h"

#endif // _ExDbCommandContext_h_Included_
