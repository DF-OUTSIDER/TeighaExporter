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
// GLES2 device render settings test dialog

#ifndef ODGLES2RSTESTDIALOG
#define ODGLES2RSTESTDIALOG

#include "TD_PackPush.h"

#include "../TrRndRenderSettings.h"
#include "ExtBar.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct ExtBarInplaceCallback
{
  virtual void intPropertyChanged(OdTrRndRenderSettings::IntegerProps nProp, OdUInt32 newVal) = 0;
  virtual void fpPropertyChanged(OdTrRndRenderSettings::FloatProps nProp, double newVal) = 0;
  virtual void colorPropertyChanged(OdTrRndRenderSettings::ColorProps nProp,
                                    const OdTrRndRenderSettings::Color &newVal) = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2RSTestDialog : public OdTrRndRenderSettingsReactor, public ExtBar::Callback,
                            public ExtBarInplaceCallback
{
  protected:
    HWND m_hDlg;
    HWND m_hDisplayWindow;
    HWND m_hParentWindow;
    OdTrRndRenderSettingsManager *m_pRSManager;
    StaticExtBar m_extBar;
  public:
    OdGLES2RSTestDialog();
    ~OdGLES2RSTestDialog();

    bool createDialogWindow(HWND hParent, HWND hDisplay, OdTrRndRenderSettingsManager *pRSManager);
    void destroyDialogWindow();

    bool isDialogPresent() const;

    HWND displayWindow() { return m_hDisplayWindow; }
    OdTrRndRenderSettingsManager *rsManager() { return m_pRSManager; }
    ExtBarInterface &extBar() { return m_extBar; }
  protected:
    virtual void renderSettingsChanged(OdTrRndRenderSettings *pRSets);
    virtual void elementChanged(ExtBar::Element *pElem);
    virtual void intPropertyChanged(OdTrRndRenderSettings::IntegerProps nProp, OdUInt32 newVal);
    virtual void fpPropertyChanged(OdTrRndRenderSettings::FloatProps nProp, double newVal);
    virtual void colorPropertyChanged(OdTrRndRenderSettings::ColorProps nProp,
                                      const OdTrRndRenderSettings::Color &newVal);
};

#include "TD_PackPop.h"

#endif // ODGLES2RSTESTDIALOG
