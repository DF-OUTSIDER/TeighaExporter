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
// Maps GsModule with RenderMode

#ifndef EXGSCOMPOSITEMODULEFACTORY
#define EXGSCOMPOSITEMODULEFACTORY

#include "TD_PackPush.h"

#include "ExGsCompositeModuleProperty.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsCompositeModuleFactory
{
  protected:
    ExGsCompositeModuleProperty m_map[OdGsView::kNone];
  public:
    ExGsCompositeModuleFactory();
    ~ExGsCompositeModuleFactory();

    void setModuleForState(OdGsView::RenderMode mode, const OdGsModule *pGsModule, OdUInt32 flags = 0);
    const ExGsCompositeModuleProperty * const directPropsPtr() const { return m_map; }

    const OdGsModule *moduleForState(OdGsView::RenderMode mode) const;
    OdGsModule *moduleForState(OdGsView::RenderMode mode);
    OdUInt32 flagsForState(OdGsView::RenderMode mode) const;

    void setModuleForAllStates(const OdGsModule *pGsModule, OdUInt32 flags = 0);
    void resetModules();

    bool isSimilarModules(bool bCheckFlags = false) const;

    bool isValid() const;
    bool validate();

    // @@@ test
    void fillByDefault();
};

#include "TD_PackPop.h"

#endif // EXGSCOMPOSITEMODULEFACTORY
