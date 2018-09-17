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
// ModelerTools.h : auxilary functionality of geometry modeler

// Copyright © Menhirs NV. All rights reserved.

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

#ifndef _OD_MODELERTOOLS_INCLUDED_
#define _OD_MODELERTOOLS_INCLUDED_

//#include "BricsysChanges.h"
#include "ModelerGeometry.h"

#include "TD_PackPush.h"

typedef void (*MainHistStreamFunc)(void*);


/** \details
    This class defines the interface for classes that
    provide the services for ACIS SAT and SAB data.

    Corresponding C++ library: ModelerGeometry
    <group Other_Classes>
*/
class DBENT_EXPORT OdModelerTools: public OdRxObject
{
protected:
    OdModelerTools() {}
public:  
    ODRX_DECLARE_MEMBERS(OdModelerTools);

    /** \details
      Delete the modeler bulletins.
    */
    virtual OdResult deleteModelerBulletins() = 0;

    /** \details
      Should be called from the main stream to start ACIS thread-safety mode.
      Prepares separate history streams for each thread, turns the thread-safety mode on.
      Returns false if no modeler geometry exist.
    */
    virtual void beginThreadSafetyMode(unsigned nThreads, const unsigned* aThreads) = 0;

    /** \details
      Should be called from the main stream to end ACIS thread-safety mode.
      Merges separate history streams into the main history, turns the thread-safety mode off.
    */
    virtual void endThreadSafetyMode(unsigned nThreads, const unsigned* aThreads) = 0;

    /** \details
      Should be called from a thread function to start modeler in this particular thread.
      Substitutes default history stream by unique one for the given thread.
    */
    virtual bool startThread() = 0;

    /** \details
      Should be called from a thread function to stop modeler in this particular thread.
      Restores default history stream for the given thread.
    */
    virtual bool stopThread() = 0;

    /** \details
      Checks if the modeler is started in this particular thread.
    */
    virtual bool isThreadStarted() const = 0;

    /** \details
      Checks topology and geometry correctness of given entity.
      Results are returned as text in parameter 'report'.
    */
    virtual OdResult check3dSolid(const OdDb3dSolidPtr&, OdString& report) = 0;

    /** \details
    Executes given function when history stream is set to a main one
    */
    virtual void executeInMainHistoryStream(MainHistStreamFunc func, void* data) = 0;
};


/** \details
  The typified smart pointer for the /modeler tools/ object. This template 
  class is specialization of the OdSmartPtr class for the OdModelerTools class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdModelerTools> OdModelerToolsPtr;

#include "TD_PackPop.h"

#endif //_OD_MODELERTOOLS_INCLUDED_
