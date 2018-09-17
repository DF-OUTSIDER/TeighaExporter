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

#ifndef _ODSPAMODELER_INCLUDED_
#define _ODSPAMODELER_INCLUDED_

#include "TD_PackPush.h"

#include "SpaModelerExport.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"

class SPAParameters;

/** \details
    Corresponding C++ library: ModelerGeometry
    <group Other_Classes>
*/
class SPAMODELER_EXPORT SpaModeler : public OdRxModule
{
  bool                    m_bLocalInit;
protected:
  void initApp();
  void uninitApp();

  SpaModeler();
public:
  virtual OdResult setSPAParameters(const SPAParameters &pParameters);
  virtual void getSPAParameters(SPAParameters &pParameters);

  virtual void setSPAtolerances(const double &resabs, const double &resfit);
  virtual void getSPAtolerances(double &resabs, double &resfit) const;
  virtual double getSPAresnor() const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxProtocolReactorManager object pointers.
*/

typedef OdSmartPtr<SpaModeler> SpaModelerPtr;

#include "TD_PackPop.h"

#endif //_ODSPAMODELER_INCLUDED_
