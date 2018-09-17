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

#include "OdaCommon.h"
#include "ExtWinRes.h"
#include "ExGsGLES2RSTestDialog.h"

#include <windowsx.h>

OdGLES2RSTestDialog::OdGLES2RSTestDialog()
  : m_hDlg(NULL), m_hDisplayWindow(NULL), m_hParentWindow(NULL), m_pRSManager(NULL)
{
}

OdGLES2RSTestDialog::~OdGLES2RSTestDialog()
{
  destroyDialogWindow();
}

namespace OdGLES2RSTestDialog_ns
{

#ifndef WIN64
#define OD_LONG_PTR LONG
#else
#define OD_LONG_PTR LONG_PTR
#endif

static void init(ExtBar *pExtBar)
{
  ExtBar::GroupElement *pGroup = NULL;
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeGroup)
      ExtBar::TextElement::cast(pGroup = pExtBar->createGroup(pProps[nProp].m_nLinkUIId1))->setText(pProps[nProp].m_pName);
    else
    {
      switch (pProps[nProp].m_propType)
      {
        case OdTrRndRenderSettings::kPropTypeDrawFlag:
          pExtBar->createCheckBox(pProps[nProp].m_nLinkUIId1, pGroup, ExtBar::k2d8);
        break;
        case OdTrRndRenderSettings::kPropTypeColor:
          pExtBar->createColorButton(pProps[nProp].m_nLinkUIId1, pGroup, ExtBar::k2d8);
        break;
        case OdTrRndRenderSettings::kPropTypeFloat:
        case OdTrRndRenderSettings::kPropTypeInt:
        case OdTrRndRenderSettings::kPropTypeUInt:
          pExtBar->createButton(pProps[nProp].m_nLinkUIId1, pGroup, ExtBar::k2d8);
        default: break;
      }
      pExtBar->createText(pProps[nProp].m_nLinkUIId2, pGroup, ExtBar::k6d8)->setText(pProps[nProp].m_pName);
    }
  }
}

static void grayify(HWND hDlg, ExtBar *pExtBar, bool bEnabled = false)
{
  const BOOL bSet = (bEnabled) ? TRUE : FALSE;
  ::EnableWindow(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT), bSet);
  ::EnableWindow(::GetDlgItem(hDlg, IDC_CBGLES2RSOVERRIDE), bSet);
  ::EnableWindow(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), bSet);
  // Properties
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (pProps[nProp].m_propType != OdTrRndRenderSettings::kPropTypeGroup)
      pExtBar->getElement(pProps[nProp].m_nLinkUIId1)->setDisabled(!bSet);
  }
}

static void updateRs(HWND hDlg, ExtBar *pExtBar, OdTrRndRenderSettingsManager *pManager)
{
  OdUInt32 curSel = (OdUInt32)ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT));
  { // Prepare info string
    OdUInt32 flags = pManager->rsViewportFlags(curSel);
    OdString infos;
    if (GETBIT(flags, OdTrRndRenderSettingsManager::kRSViewportVisible))
      infos += OD_T("Visible");
    if (GETBIT(flags, OdTrRndRenderSettingsManager::kRSViewportPS))
    {
      if (!infos.isEmpty()) infos += OD_T(", ");
      infos += OD_T("PS");
    }
    if (GETBIT(flags, OdTrRndRenderSettingsManager::kRSViewportOverall))
    {
      if (!infos.isEmpty()) infos += OD_T(", ");
      infos += OD_T("Overall");
    }
    if (GETBIT(flags, OdTrRndRenderSettingsManager::kRSViewportHelper))
    {
      if (!infos.isEmpty()) infos += OD_T(", ");
      infos += OD_T("Helper");
    }
    if (infos.isEmpty()) infos += OD_T(" ");
    ::SetDlgItemText(hDlg, IDC_SGLES2RSINFO, infos.c_str());
  }
  // Settings
  ::CheckDlgButton(hDlg, IDC_CBGLES2RSOVERRIDE, pManager->rsIsViewportRenderModeOverride(curSel) ? BST_CHECKED : BST_UNCHECKED);
  OdTrRndRenderSettingsPtr rs;
  pManager->rsViewportSettings(curSel, rs);
  ComboBox_SetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), (int)rs->renderMode());
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (pProps[nProp].m_propType != OdTrRndRenderSettings::kPropTypeGroup)
    {
      switch (pProps[nProp].m_propType)
      {
        case OdTrRndRenderSettings::kPropTypeDrawFlag:
          ExtBar::CheckElement::cast(pExtBar->getElement(pProps[nProp].m_nLinkUIId1))->
            setChecked(rs->drawFlag(pProps[nProp].m_nPropCode));
        break;
        case OdTrRndRenderSettings::kPropTypeColor:
          ExtBar::ColorElement::cast(pExtBar->getElement(pProps[nProp].m_nLinkUIId1))->
            setColor(rs->color((OdTrRndRenderSettings::ColorProps)pProps[nProp].m_nPropCode).m_colorVal);
        break;
        case OdTrRndRenderSettings::kPropTypeFloat:
          ExtBar::TextElement::cast(pExtBar->getElement(pProps[nProp].m_nLinkUIId1))->
            setText(OdString().format(OD_T("%.2f"), rs->value((OdTrRndRenderSettings::FloatProps)pProps[nProp].m_nPropCode)));
        break;
        case OdTrRndRenderSettings::kPropTypeInt:
          ExtBar::TextElement::cast(pExtBar->getElement(pProps[nProp].m_nLinkUIId1))->
            setText(OdString().format(OD_T("%d"), (int)rs->value((OdTrRndRenderSettings::IntegerProps)pProps[nProp].m_nPropCode)));
        break;
        case OdTrRndRenderSettings::kPropTypeUInt:
          ExtBar::TextElement::cast(pExtBar->getElement(pProps[nProp].m_nLinkUIId1))->
            setText(OdString().format(OD_T("%u"), (unsigned)rs->valueU((OdTrRndRenderSettings::IntegerProps)pProps[nProp].m_nPropCode)));
        break;
        default: break;
      }
    }
  }
}

static void update(HWND hDlg, ExtBar *pExtBar, OdTrRndRenderSettingsManager *pManager)
{
  int curSel = ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT));
  ComboBox_ResetContent(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT));
  OdUInt32 nViews = pManager->rsNumViewports();
  for (OdUInt32 nView = 0; nView < nViews; nView++)
  {
    OdTrVisViewportId vptId = pManager->rsViewportId(nView);
    ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT), OdString().format(OD_T("%016llX"), vptId).c_str());
  }
  if (!nViews)
    grayify(hDlg, pExtBar);
  else
  {
    grayify(hDlg, pExtBar, true);
    if (curSel >= (int)nViews) curSel = (int)(nViews - 1);
    else if (curSel < 0) curSel = 0;
    ComboBox_SetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT), curSel);
    updateRs(hDlg, pExtBar, pManager);
  }
}

static OdGLES2RSTestDialog *rsDlg(HWND hDlg)
{
  return (OdGLES2RSTestDialog*)(OdIntPtr)::GetWindowLongPtr(hDlg, DWLP_USER);
}

static OdTrRndRenderSettings &rsCurRs(HWND hDlg)
{
  return rsDlg(hDlg)->rsManager()->rsViewportSettings((OdUInt32)ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT)));
}

static void updateReactors(HWND hDlg, bool bReset = false)
{
  OdUInt32 curSel = (OdUInt32)ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT));
  if (bReset) curSel = 0xFFFFFFFF;
  OdGLES2RSTestDialog *pDlg = rsDlg(hDlg);
  const OdUInt32 nViews = pDlg->rsManager()->rsNumViewports();
  for (OdUInt32 nVp = 0; nVp < nViews; nVp++)
  {
    if (nVp == curSel)
      pDlg->rsManager()->rsViewportSettings(nVp).addReactor(pDlg);
    else
      pDlg->rsManager()->rsViewportSettings(nVp).removeReactor(pDlg);
  }
}

static void invalidateGraph(HWND hDlg)
{
  ::InvalidateRect(rsDlg(hDlg)->displayWindow(), NULL, TRUE);
  ::PostMessage(rsDlg(hDlg)->displayWindow(), WM_PAINT, 0, 0);
}

#define WM_RSREACT (WM_USER + 1)

static INT_PTR CALLBACK dlgClientProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_INITDIALOG:
      {
        ::SetWindowLongPtr(hDlg, DWLP_USER, (OD_LONG_PTR)lParam);
        { // Prepare render mode combo box
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("2dWireframe"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("3dWireframe"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("HiddenLine"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("FlatShaded"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("GouraudShaded"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("FlatShadedWithWireframe"));
          ComboBox_AddString(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE), OD_T("GouraudShadedWithWireframe"));
        }
        OdGLES2RSTestDialog *pDlg = rsDlg(hDlg);
        ExtBar::WinSysHandle sysHandle = { ::GetDlgItem(hDlg, IDC_SEXTBARPLACEHOLDER), NULL };
        pDlg->extBar().create(&sysHandle);
        init(pDlg->extBar().extBar());
        ExtBar::Rect placeholderSize; ::GetClientRect(::GetDlgItem(hDlg, IDC_SEXTBARPLACEHOLDER), &placeholderSize);
        pDlg->extBar().resize(placeholderSize);
        pDlg->extBar().resetCallback(pDlg);
        update(hDlg, pDlg->extBar().extBar(), pDlg->rsManager());
        updateReactors(hDlg);
      }
    break;
    case WM_DESTROY:
      {
        updateReactors(hDlg, true);
        rsDlg(hDlg)->extBar().destroy();
      }
    break;
    case WM_RSREACT:
      {
        updateRs(hDlg, rsDlg(hDlg)->extBar().extBar(), rsDlg(hDlg)->rsManager());
      }
    break;
    case WM_COMMAND:
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          switch (LOWORD(wParam))
          {
            case IDC_BGLES2RSUPDATE:
              update(hDlg, rsDlg(hDlg)->extBar().extBar(), rsDlg(hDlg)->rsManager());
              updateReactors(hDlg);
            break;
            case IDC_CBGLES2RSOVERRIDE:
              if (::IsDlgButtonChecked(hDlg, IDC_CBGLES2RSOVERRIDE))
                rsCurRs(hDlg).setRenderModeOverride(false, kRSIgnoreReactor);
              else
              {
                rsCurRs(hDlg).setRenderModeOverride(true, kRSIgnoreReactor);
                rsDlg(hDlg)->rsManager()->rsApplyViewportRenderModeOverride((OdUInt32)ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSVIEWPORT)));
              }
              invalidateGraph(hDlg);
            break;
          }
        break;
        case LBN_SELCHANGE:
          switch (LOWORD(wParam))
          {
            case IDC_CBGLES2RSVIEWPORT:
              updateRs(hDlg, rsDlg(hDlg)->extBar().extBar(), rsDlg(hDlg)->rsManager());
            break;
            case IDC_CBGLES2RSRMODE:
              rsCurRs(hDlg).setForRenderMode((OdGsView::RenderMode)ComboBox_GetCurSel(::GetDlgItem(hDlg, IDC_CBGLES2RSRMODE)));
              invalidateGraph(hDlg);
            break;
          }
        break;
      }
    default:
    return FALSE;
  }
  return TRUE;
}

static HMODULE getCurrentModule()
{
  MEMORY_BASIC_INFORMATION mbi;
  static int dummy;
  ::VirtualQuery(&dummy, &mbi, sizeof(mbi));
  return reinterpret_cast<HMODULE>(mbi.AllocationBase);
}

class ExtBarIntInplace : public ExtBarInplace
{
  protected:
    OdTrRndRenderSettings::IntegerProps m_nProp;
    ExtBarInplaceCallback *m_pCallback;
    OdUInt32 m_nVal, m_nLimMin, m_nLimMax;
    bool m_bSigned;
  public:
    ExtBarIntInplace(ExtBarInplaceCallback *pCallback = NULL)
      : m_pCallback(pCallback) { }
    void setCallback(ExtBarInplaceCallback *pCallback) { m_pCallback = pCallback; }
    void run(HWND hWnd, OdTrRndRenderSettings::IntegerProps nProp, OdUInt32 nVal,
             OdUInt32 nLimMin, OdUInt32 nLimMax, bool bSigned, HINSTANCE hInstance = NULL)
    {
      m_nProp = nProp; m_nVal = nVal; m_nLimMin = nLimMin; m_nLimMax = nLimMax; m_bSigned = bSigned;
      ExtBar::WinSysHandle sysHandle = { hWnd, hInstance };
      display(&sysHandle, screenplace(curpos(), makesize(100, 24 * 2 + 8)));
    }
    virtual void fillInplace(ExtBar *pExtBar)
    {
      if (m_bSigned)
        pExtBar->createRangeBox(100, NULL, ExtBar::k8d8, true, false)->setInt(OdInt32(m_nVal), OdInt32(m_nLimMin), OdInt32(m_nLimMax));
      else
        pExtBar->createRangeBox(100, NULL, ExtBar::k8d8, false, false)->setUInt(m_nVal, m_nLimMin, m_nLimMax);
      pExtBar->createButton(101, NULL, ExtBar::k4d8)->setText(OD_T("OK"));
      pExtBar->createButton(102, NULL, ExtBar::k4d8)->setText(OD_T("Cancel"));
    }
    virtual void elementChanged(ExtBar::Element *pElem)
    {
      const OdUInt32 nId = pElem->elementId();
      if (nId == 101)
      {
        if (m_bSigned)
          m_pCallback->intPropertyChanged(m_nProp, OdUInt32(ExtBar::RangeElement::cast(m_pExtBar->getElement(100))->getPosInt()));
        else
          m_pCallback->intPropertyChanged(m_nProp, ExtBar::RangeElement::cast(m_pExtBar->getElement(100))->getPosUInt());
      }
      if (nId == 101 || nId == 102)
        close();
    }
};

class ExtBarFPInplace : public ExtBarInplace
{
  protected:
    OdTrRndRenderSettings::FloatProps m_nProp;
    ExtBarInplaceCallback *m_pCallback;
    double m_nVal, m_nLimMin, m_nLimMax;
  public:
    ExtBarFPInplace(ExtBarInplaceCallback *pCallback = NULL)
      : m_pCallback(pCallback) { }
    void setCallback(ExtBarInplaceCallback *pCallback) { m_pCallback = pCallback; }
    void run(HWND hWnd, OdTrRndRenderSettings::FloatProps nProp, double nVal,
             double nLimMin, double nLimMax, HINSTANCE hInstance = NULL)
    {
      m_nProp = nProp; m_nVal = nVal; m_nLimMin = nLimMin; m_nLimMax = nLimMax;
      ExtBar::WinSysHandle sysHandle = { hWnd, hInstance };
      display(&sysHandle, screenplace(curpos(), makesize(100, 24 * 2 + 8)));
    }
    virtual void fillInplace(ExtBar *pExtBar)
    {
      pExtBar->createRangeBox(100, NULL, ExtBar::k8d8, true, true)->setFP(m_nVal, m_nLimMin, m_nLimMax);
      pExtBar->createButton(102, NULL, ExtBar::k4d8)->setText(OD_T("OK"));
      pExtBar->createButton(101, NULL, ExtBar::k4d8)->setText(OD_T("Cancel"));
    }
    virtual void elementChanged(ExtBar::Element *pElem)
    {
      const OdUInt32 nId = pElem->elementId();
      if (nId == 102)
        m_pCallback->fpPropertyChanged(m_nProp, ExtBar::RangeElement::cast(m_pExtBar->getElement(100))->getPosFP());
      if (nId == 101 || nId == 102)
        close();
    }
};

class ExtBarColorInplace : public ExtBarInplace
{
  protected:
    OdTrRndRenderSettings::ColorProps m_nProp;
    ExtBarInplaceCallback *m_pCallback;
    OdTrRndRenderSettings::Color m_color;
  public:
    ExtBarColorInplace(ExtBarInplaceCallback *pCallback = NULL)
      : m_pCallback(pCallback) { }
    void setCallback(ExtBarInplaceCallback *pCallback) { m_pCallback = pCallback; }
    void run(HWND hWnd, OdTrRndRenderSettings::ColorProps nProp, 
             const OdTrRndRenderSettings::Color &color, HINSTANCE hInstance = NULL)
    {
      m_nProp = nProp; m_color = color;
      ExtBar::WinSysHandle sysHandle = { hWnd, hInstance };
      display(&sysHandle, screenplace(curpos(), makesize(100, 24 * 7 + 8)));
    }
    virtual void fillInplace(ExtBar *pExtBar)
    {
      ExtBar::RadioElement *pGroup = pExtBar->createRadioGroup(100);
      pExtBar->createRadioButton(101, pGroup, ExtBar::k8d8, m_color.m_colorType == OdTrRndRenderSettings::kColorNormal)->setText(OD_T("Normal RGB"));
      pExtBar->createRadioButton(102, pGroup, ExtBar::k8d8, m_color.m_colorType == OdTrRndRenderSettings::kColorBackground)->setText(OD_T("Background"));
      pExtBar->createRadioButton(103, pGroup, ExtBar::k8d8, m_color.m_colorType == OdTrRndRenderSettings::kColorForeground)->setText(OD_T("Foreground"));
      pExtBar->createRadioButton(104, pGroup, ExtBar::k8d8, m_color.m_colorType == OdTrRndRenderSettings::kColorTint)->setText(OD_T("Tint"));
      pExtBar->createRadioButton(105, pGroup, ExtBar::k8d8, m_color.m_colorType == OdTrRndRenderSettings::kColorDesaturate)->setText(OD_T("Desaturate"));
      pExtBar->createColorButton(106)->setColor(m_color.m_colorVal);
      ExtBar::TextElement::cast(pExtBar->getElement(106))->setText(OD_T("RGB Color"));
      pExtBar->createButton(107, NULL, ExtBar::k4d8)->setText(OD_T("OK"));
      pExtBar->createButton(108, NULL, ExtBar::k4d8)->setText(OD_T("Cancel"));
    }
    virtual void elementChanged(ExtBar::Element *pElem)
    {
      const OdUInt32 nId = pElem->elementId();
      if (nId == 107)
      {
        OdTrRndRenderSettings::Color color;
        color.m_colorType = (OdTrRndRenderSettings::ColorType)(ExtBar::RadioElement::cast(m_pExtBar->getElement(101))->checkedElement() - 101);
        color.m_colorVal = m_color.m_colorVal;
        m_pCallback->colorPropertyChanged(m_nProp, color);
      }
      if (nId == 107 || nId == 108)
        close();
      if (nId == 106)
      {
        ODCOLORREF color = m_color.m_colorVal;
        if (m_pExtBar->getColor(color))
        {
          m_color.m_colorVal = color;
          ExtBar::ColorElement::cast(pElem)->setColor(color);
        }
      }
    }
};

} // OdGLES2RSTestDialog_ns

void OdGLES2RSTestDialog::elementChanged(ExtBar::Element *pElem)
{ using namespace OdGLES2RSTestDialog_ns;
  const OdTrRndRenderSettings::UIPropEntry *pEntry = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pEntry);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++, pEntry++)
  {
    if (pEntry->m_nLinkUIId1 == pElem->elementId())
    {
      switch (pEntry->m_propType)
      {
        case OdTrRndRenderSettings::kPropTypeDrawFlag:
        {
          OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
          rs.setDrawFlag(pEntry->m_nPropCode, ExtBar::CheckElement::cast(pElem)->isChecked(), kRSIgnoreReactor);
          invalidateGraph(m_hDlg);
        }
        break;
        case OdTrRndRenderSettings::kPropTypeInt:
        case OdTrRndRenderSettings::kPropTypeUInt:
        {
          OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
          ExtBarIntInplace(this).run(m_hDlg, (OdTrRndRenderSettings::IntegerProps)pEntry->m_nPropCode,
            rs.valueU((OdTrRndRenderSettings::IntegerProps)pEntry->m_nPropCode), pEntry->m_ranges.m_min.m_limUInt, pEntry->m_ranges.m_max.m_limUInt,
            pEntry->m_propType == OdTrRndRenderSettings::kPropTypeInt);
        }
        break;
        case OdTrRndRenderSettings::kPropTypeFloat:
        {
          OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
          ExtBarFPInplace(this).run(m_hDlg, (OdTrRndRenderSettings::FloatProps)pEntry->m_nPropCode,
            rs.value((OdTrRndRenderSettings::FloatProps)pEntry->m_nPropCode), pEntry->m_ranges.m_min.m_limFlt, pEntry->m_ranges.m_max.m_limFlt);
        }
        break;
        case OdTrRndRenderSettings::kPropTypeColor:
        {
          OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
          ExtBarColorInplace(this).run(m_hDlg, (OdTrRndRenderSettings::ColorProps)pEntry->m_nPropCode,
            rs.color((OdTrRndRenderSettings::ColorProps)pEntry->m_nPropCode, false));
        }
        break;
        default: break;
      }
      break;
    }
  }
}

void OdGLES2RSTestDialog::intPropertyChanged(OdTrRndRenderSettings::IntegerProps nPropSet, OdUInt32 newVal)
{ using namespace OdGLES2RSTestDialog_ns;
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if ((pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeInt || pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeUInt) &&
        pProps[nProp].m_nPropCode == nPropSet)
    {
      OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
      rs.setValueU(nPropSet, newVal, kRSIgnoreReactor);
      if (pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeInt)
        ExtBar::TextElement::cast(extBar().extBar()->getElement(pProps[nProp].m_nLinkUIId1))->
          setText(OdString().format(OD_T("%d"), (int)rs.value(nPropSet)));
      else
        ExtBar::TextElement::cast(extBar().extBar()->getElement(pProps[nProp].m_nLinkUIId1))->
          setText(OdString().format(OD_T("%u"), (unsigned)rs.valueU(nPropSet)));
      invalidateGraph(m_hDlg);
      break;
    }
  }
}

void OdGLES2RSTestDialog::fpPropertyChanged(OdTrRndRenderSettings::FloatProps nPropSet, double newVal)
{ using namespace OdGLES2RSTestDialog_ns;
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeFloat &&
        pProps[nProp].m_nPropCode == nPropSet)
    {
      OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
      rs.setValue(nPropSet, (float)newVal, kRSIgnoreReactor);
      ExtBar::TextElement::cast(extBar().extBar()->getElement(pProps[nProp].m_nLinkUIId1))->
        setText(OdString().format(OD_T("%.2f"), rs.value(nPropSet)));
      invalidateGraph(m_hDlg);
      break;
    }
  }
}

void OdGLES2RSTestDialog::colorPropertyChanged(OdTrRndRenderSettings::ColorProps nPropSet,
                                               const OdTrRndRenderSettings::Color &newVal)
{ using namespace OdGLES2RSTestDialog_ns;
  const OdTrRndRenderSettings::UIPropEntry *pProps = NULL;
  const OdUInt32 nProps = OdTrRndRenderSettings::extendedPropsInfo(&pProps);
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (pProps[nProp].m_propType == OdTrRndRenderSettings::kPropTypeColor &&
        pProps[nProp].m_nPropCode == nPropSet)
    {
      OdTrRndRenderSettings &rs = rsCurRs(m_hDlg);
      rs.setColor(nPropSet, newVal, kRSIgnoreReactor);
      ExtBar::ColorElement::cast(extBar().extBar()->getElement(pProps[nProp].m_nLinkUIId1))->setColor(newVal.m_colorVal);
      invalidateGraph(m_hDlg);
      break;
    }
  }
}

bool OdGLES2RSTestDialog::createDialogWindow(HWND hParent, HWND hDisplay, OdTrRndRenderSettingsManager *pRSManager)
{
  m_hParentWindow = hParent; m_hDisplayWindow = hDisplay;
  m_pRSManager = pRSManager;
  m_hDlg = ::CreateDialogParam(OdGLES2RSTestDialog_ns::getCurrentModule(), MAKEINTRESOURCE(IDD_GLES2RSDIALOG),
                               hParent, OdGLES2RSTestDialog_ns::dlgClientProc, (LPARAM)this);
  if (m_hDlg == NULL)
    return false;
  ::ShowWindow(m_hDlg, SW_SHOWNORMAL);
  ::UpdateWindow(m_hDlg);
  return true;
}

void OdGLES2RSTestDialog::destroyDialogWindow()
{
  if (m_hDlg != NULL)
  {
    ::DestroyWindow(m_hDlg);
    m_hDlg = NULL;
  }
}

bool OdGLES2RSTestDialog::isDialogPresent() const
{
  return m_hDlg != NULL;
}

void OdGLES2RSTestDialog::renderSettingsChanged(OdTrRndRenderSettings * /*pRSets*/)
{
  ::SendMessage(m_hDlg, WM_RSREACT, 0, 0);
}

//
