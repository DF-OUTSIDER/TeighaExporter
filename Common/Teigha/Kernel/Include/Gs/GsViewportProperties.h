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

#ifndef __OD_GS_VIEWPORT_PROPERTIES__
#define __OD_GS_VIEWPORT_PROPERTIES__

#include "TD_PackPush.h"

#include "Gs/GsProperties.h"
#include "Gi/GiViewportTraitsData.h"
#include "Gs/GsBackgroundProperties.h"
#include "Gs/GsVisualStyleProperties.h"
#include "Gs/GsRenderSettingsProperties.h"

// Viewports is not entities or nodes (this is objects with one attributes set per scene)

// OdGsViewportProperties hierarhy:
// OdGsViewportProperties
//   OdGsBackgroundProperties
//   OdGsVisualStyleProperties
//   OdGsRenderEnvironmentProperties
//   OdGsRenderSettingsProperties

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsViewportProperties : public OdGsProperties
{
  OdGiViewportTraitsData* m_pViewportTraits;
  // Internal handlers
  OdGsBackgroundPropertiesPtr  m_pBackground;
  OdGsVisualStylePropertiesPtr m_pVisualStyle;
  OdGsRenderEnvironmentPropertiesPtr m_pRenderEnvironment;
  OdGsRenderSettingsPropertiesPtr m_pRenderSettings;
public:
  ODRX_DECLARE_MEMBERS(OdGsViewportProperties);

  OdGsViewportProperties();
  ~OdGsViewportProperties();

  OdGiViewportTraitsData* viewportTraitsData() const { return m_pViewportTraits; }

  bool isTraitsModified();

  void clearTraits();

  // OdGsProperties virtual overrides

  virtual PropertiesType propertiesType() const
  {
    return kViewport;
  }

  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType type) const;
  
  // Update attributes
  virtual void update(const OdGiDrawable* pUnderlyingDrawable,
      OdGsViewImpl* view, OdUInt32 incFlags = kAll);
  // Display attributed data
  virtual void display(OdGsBaseVectorizer& view,
      OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsViewportProperties object pointers.
*/
typedef OdSmartPtr<OdGsViewportProperties> OdGsViewportPropertiesPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_VIEWPORT_PROPERTIES__
