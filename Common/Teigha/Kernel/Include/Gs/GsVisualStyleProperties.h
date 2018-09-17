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

#ifndef __OD_GS_VISUALSTYLE_PROPERTIES__
#define __OD_GS_VISUALSTYLE_PROPERTIES__

#include "TD_PackPush.h"

#include "Gs/GsProperties.h"
#include "Gi/GiVisualStyle.h"
#include "Gi/GiVisualStyleTraitsData.h"

// VisualStyles is not entities or nodes
// VisualStyles can be dynamicly modified outside viewport's:
//   OdGsPropertiesPtr vs = props->propertiesForType(kVisualStyle);
//   vs->update(newVS, view, kVisualStyle);
// Then may be putted to view:
//   vs->display(this->view, NULL, kVisualStyle);
// Or accesed directly from viewport (on onTraitsModified for example):
//   OdGsVisualStylePropertiesPtr pVS = props->propertiesForType(kVisualStyle);
//   OdGiVisualStyleTraitsData* pVSData = pVS->visualStyleTraitsData();
//   pVSData->(useOfData());

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsVisualStyleProperties : public OdGsProperties
{
  OdGiVisualStyleTraitsData* m_pVisualStyleTraits;

public:
  ODRX_DECLARE_MEMBERS(OdGsVisualStyleProperties);

  OdGsVisualStyleProperties();
  ~OdGsVisualStyleProperties();

  OdGiVisualStyleTraitsData* visualStyleTraitsData() const { return m_pVisualStyleTraits; }

  bool isTraitsModified();

  void clearTraits();

  // OdGsProperties virtual overrides
  
  virtual PropertiesType propertiesType() const
  {
    return kVisualStyle;
  }

  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType type) const;

  virtual void update(const OdGiDrawable* pUnderlyingDrawable,
      OdGsViewImpl* view, OdUInt32 incFlags = kAll);

  virtual void display(OdGsBaseVectorizer& view,
      OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsVisualStyleProperties object pointers.
*/
typedef OdSmartPtr<OdGsVisualStyleProperties> OdGsVisualStylePropertiesPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_VISUALSTYLE_PROPERTIES__
