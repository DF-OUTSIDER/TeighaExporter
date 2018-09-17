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
// Extensions Bar

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commdlg.h> // for open/save dialogs
#include <commctrl.h> // for NM_CUSTOMDRAW
#include <shlwapi.h> // for DLLVERSIONINFO

#include "OdaCommon.h"
#include "OdList.h"
#include "OdVector.h"
#include "SharedPtr.h"

#include "ExtBar.h"

static TCHAR ExtBarWndClassName[] = _T("ExtBar");
static HINSTANCE ExtBarAppInstance = 0;
static int ExtBarRegistered = 0;

static const int StringSize = 24;
static const int GroupSize  = 10;
static const int ElOffset   = 2;

#ifndef WS_EX_NOACTIVATE
#define WS_EX_NOACTIVATE        0x08000000L
#endif
#ifndef WS_EX_COMPOSITED
#define WS_EX_COMPOSITED        0x02000000L
#endif
#ifndef GWL_USERDATA
#define GWL_USERDATA            GWLP_USERDATA
#endif
#ifndef NM_CUSTOMDRAW
#define NM_CUSTOMDRAW           (NM_FIRST-12)
#define CDDS_PREPAINT           0x00000001
#define CDDS_POSTPAINT          0x00000002
#define CDDS_PREERASE           0x00000003
#define CDDS_POSTERASE          0x00000004
#define CDRF_DODEFAULT          0x00000000
#define CDRF_NEWFONT            0x00000002
#define CDRF_SKIPDEFAULT        0x00000004
#define CDRF_DOERASE            0x00000008 // draw the background
#define CDRF_SKIPPOSTPAINT      0x00000100 // don't draw the focus rect
typedef struct tagNMCUSTOMDRAWINFO
{
    NMHDR hdr;
    DWORD dwDrawStage;
    HDC hdc;
    RECT rc;
    DWORD_PTR dwItemSpec;  // this is control specific, but it's how to specify an item.  valid only with CDDS_ITEM bit set
    UINT  uItemState;
    LPARAM lItemlParam;
} NMCUSTOMDRAW, *LPNMCUSTOMDRAW;
#endif

// Helpers

static BOOL IsThemed()
{ // Based onto: http://www.codeproject.com/Articles/10564/How-to-accurately-detect-if-an-application-is-them
  BOOL ret = FALSE;
  OSVERSIONINFO ovi = {0};
  ovi.dwOSVersionInfoSize = sizeof ovi;
  GetVersionEx(&ovi);
  if (((ovi.dwMajorVersion==5) && (ovi.dwMinorVersion >= 1)) || (ovi.dwMajorVersion > 5))
  {
    // Windows XP or greater detected
    typedef BOOL WINAPI ISAPPTHEMED();
    typedef BOOL WINAPI ISTHEMEACTIVE();
    ISAPPTHEMED* pISAPPTHEMED = NULL;
    ISTHEMEACTIVE* pISTHEMEACTIVE = NULL;
    HMODULE hMod = LoadLibrary(_T("uxtheme.dll"));
    if (hMod)
    {
      pISAPPTHEMED = reinterpret_cast<ISAPPTHEMED*>(GetProcAddress(hMod, "IsAppThemed"));
      pISTHEMEACTIVE = reinterpret_cast<ISTHEMEACTIVE*>(GetProcAddress(hMod, "IsThemeActive"));
      if (pISAPPTHEMED && pISTHEMEACTIVE)
      {
        if (pISAPPTHEMED() && pISTHEMEACTIVE())
        {
          typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
          DLLGETVERSION* pDLLGETVERSION = NULL;
          HMODULE hModComCtl = LoadLibrary(_T("comctl32.dll"));
          if (hModComCtl)
          {
            pDLLGETVERSION = reinterpret_cast<DLLGETVERSION*>(GetProcAddress(hModComCtl, "DllGetVersion"));
            if (pDLLGETVERSION)
            {
              DLLVERSIONINFO dvi = {0};
              dvi.cbSize = sizeof dvi;
              if (pDLLGETVERSION(&dvi) == NOERROR)
                ret = dvi.dwMajorVersion >= 6;
            }
            FreeLibrary(hModComCtl);
          }
        }
      }
      FreeLibrary(hMod);
    }
  }
  return ret;
}

class FlatScrollBarHelper
{
  protected:
    BOOL m_bSupported;
    typedef BOOL (WINAPI *PInitializeFlatSBPtr)(HWND);
    PInitializeFlatSBPtr m_pInitFunc;
    typedef HRESULT (WINAPI *PUninitializeFlatSBPtr)(HWND);
    PUninitializeFlatSBPtr m_pUninitFunc;
    typedef BOOL (WINAPI *PFlatSB_EnableScrollBarPtr)(HWND, int, UINT);
    PFlatSB_EnableScrollBarPtr m_pEnableFunc;
    typedef BOOL (WINAPI *PFlatSB_ShowScrollBarPtr)(HWND, int, BOOL);
    PFlatSB_ShowScrollBarPtr m_pShowFunc;
    typedef BOOL (WINAPI *PFlatSB_GetScrollRangePtr)(HWND, int, LPINT, LPINT);
    PFlatSB_GetScrollRangePtr m_pGetRangeFunc;
    //typedef BOOL (WINAPI *PFlatSB_GetScrollInfoPtr)(HWND, int, LPSCROLLINFO);
    //PFlatSB_GetScrollInfoPtr m_pGetInfoFunc;
    typedef int (WINAPI *PFlatSB_GetScrollPosPtr)(HWND, int);
    PFlatSB_GetScrollPosPtr m_pGetPosFunc;
    //typedef BOOL (WINAPI *PFlatSB_GetScrollPropPtr)(HWND, int, LPINT);
    //PFlatSB_GetScrollPropPtr m_pGetPropFunc;
    typedef int (WINAPI *PFlatSB_SetScrollPosPtr)(HWND, int, int, BOOL);
    PFlatSB_SetScrollPosPtr m_pSetPosFunc;
    //typedef int (WINAPI *PFlatSB_SetScrollInfoPtr)(HWND, int, LPSCROLLINFO, BOOL);
    //PFlatSB_SetScrollInfoPtr m_pSetInfoFunc;
    typedef int (WINAPI *PFlatSB_SetScrollRangePtr)(HWND, int, int, int, BOOL);
    PFlatSB_SetScrollRangePtr m_pSetRangeFunc;
    //typedef BOOL (WINAPI *PFlatSB_SetScrollPropPtr)(HWND, UINT, INT_PTR, BOOL);
    //PFlatSB_SetScrollPropPtr m_pSetPropFunc;
  public:
    FlatScrollBarHelper()
      : m_bSupported(FALSE)
    { }
    BOOL init(HWND hWnd)
    {
      if (IsThemed())
        return FALSE;
      HMODULE hModComCtl = LoadLibrary(_T("comctl32.dll"));
      if (hModComCtl)
      {
        m_pInitFunc = reinterpret_cast<PInitializeFlatSBPtr>(::GetProcAddress(hModComCtl, "InitializeFlatSB"));
        if (m_pInitFunc)
        {
          m_bSupported = m_pInitFunc(hWnd);
          if (m_bSupported)
          {
            m_pUninitFunc = reinterpret_cast<PUninitializeFlatSBPtr>(::GetProcAddress(hModComCtl, "UninitializeFlatSB"));
            m_pEnableFunc = reinterpret_cast<PFlatSB_EnableScrollBarPtr>(::GetProcAddress(hModComCtl, "FlatSB_EnableScrollBar"));
            m_pShowFunc = reinterpret_cast<PFlatSB_ShowScrollBarPtr>(::GetProcAddress(hModComCtl, "FlatSB_ShowScrollBar"));
            m_pGetRangeFunc = reinterpret_cast<PFlatSB_GetScrollRangePtr>(::GetProcAddress(hModComCtl, "FlatSB_GetScrollRange"));
            //m_pGetInfoFunc = reinterpret_cast<PFlatSB_GetScrollInfoPtr>(::GetProcAddress(hModComCtl, "FlatSB_GetScrollInfo"));
            m_pGetPosFunc = reinterpret_cast<PFlatSB_GetScrollPosPtr>(::GetProcAddress(hModComCtl, "FlatSB_GetScrollPos"));
            //m_pGetPropFunc = reinterpret_cast<PFlatSB_GetScrollPropPtr>(::GetProcAddress(hModComCtl, "FlatSB_GetScrollProp"));
            m_pSetPosFunc = reinterpret_cast<PFlatSB_SetScrollPosPtr>(::GetProcAddress(hModComCtl, "FlatSB_SetScrollPos"));
            //m_pSetInfoFunc = reinterpret_cast<PFlatSB_SetScrollInfoPtr>(::GetProcAddress(hModComCtl, "FlatSB_SetScrollInfo"));
            m_pSetRangeFunc = reinterpret_cast<PFlatSB_SetScrollRangePtr>(::GetProcAddress(hModComCtl, "FlatSB_SetScrollRange"));
            //m_pSetPropFunc = reinterpret_cast<PFlatSB_SetScrollPropPtr>(::GetProcAddress(hModComCtl, "FlatSB_SetScrollProp"));
          }
          else
            m_pInitFunc = NULL;
        }
      }
      return m_bSupported;
    }
    HRESULT uninit(HWND hWnd)
    {
      if (m_bSupported && m_pUninitFunc)
        return m_pUninitFunc(hWnd);
      return 0;
    }
    BOOL enable(HWND hWnd, UINT nFlags, UINT nArrow)
    {
      if (m_bSupported && m_pEnableFunc)
        return m_pEnableFunc(hWnd, (int)nFlags, nArrow);
      else
        return ::EnableScrollBar(hWnd, nFlags, nArrow);
    }
    BOOL show(HWND hWnd, int nBar, BOOL bShow)
    {
      if (m_bSupported && m_pShowFunc)
        return m_pShowFunc(hWnd, nBar, bShow);
      else
        return ::ShowScrollBar(hWnd, nBar, bShow);
    }
    BOOL setRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
    {
      if (m_bSupported && m_pSetRangeFunc)
        return m_pSetRangeFunc(hWnd, nBar, nMinPos, nMaxPos, bRedraw);
      else
        return ::SetScrollRange(hWnd, nBar, nMinPos, nMaxPos, bRedraw);
    }
    BOOL range(HWND hWnd, int nBar, INT *pMinPos, INT *pMaxPos)
    {
      if (m_bSupported && m_pGetRangeFunc)
        return m_pGetRangeFunc(hWnd, nBar, pMinPos, pMaxPos);
      else
        return ::GetScrollRange(hWnd, nBar, pMinPos, pMaxPos);
    }
    int setPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
    {
      if (m_bSupported && m_pSetPosFunc)
        return m_pSetPosFunc(hWnd, nBar, nPos, bRedraw);
      else
        return ::SetScrollPos(hWnd, nBar, nPos, bRedraw);
    }
    int pos(HWND hWnd, int nBar)
    {
      if (m_bSupported && m_pGetPosFunc)
        return m_pGetPosFunc(hWnd, nBar);
      else
        return ::GetScrollPos(hWnd, nBar);
    }
};

static OdString WinTextAsString(HWND hWnd)
{
  OdString buf;
  int nText = ::GetWindowTextLength(hWnd);
  if (nText)
  {
    OdChar *pBuf = buf.getBufferSetLength(nText);
    ::GetWindowText(hWnd, pBuf, nText + 1);
    buf.releaseBuffer(nText);
  }
  return buf;
}

static bool IsScrollBarVisible(HWND hWnd, bool bHorizontal = false)
{
  return (::GetWindowLong(hWnd, GWL_STYLE) & ((bHorizontal) ? WS_HSCROLL : WS_VSCROLL)) != 0;
}

class DCObjectContainer
{
  protected:
    HGDIOBJ m_dcObject;
  public:
    DCObjectContainer() : m_dcObject(NULL) { }
    ~DCObjectContainer() { reset(); }

    void set(HGDIOBJ dcObject)
    {
      if (dcObject != m_dcObject)
        reset();
      m_dcObject = dcObject;
    }
    void reset()
    {
      if (m_dcObject)
      {
        ::DeleteObject(m_dcObject);
        m_dcObject = NULL;
      }
    }
    void set(HBRUSH hBrush) { set((HGDIOBJ)hBrush); }
    HBRUSH asBrush() const { return (HBRUSH)m_dcObject; }
    void set(HPEN hPen) { set((HGDIOBJ)hPen); }
    HPEN asPen() const { return (HPEN)m_dcObject; }
    void set(HFONT hFont) { set((HGDIOBJ)hFont); }
    HFONT asFont() const { return (HFONT)m_dcObject; }
    void set(HBITMAP hBitmap) { set((HGDIOBJ)hBitmap); }
    HBITMAP asBitmap() const { return (HBITMAP)m_dcObject; }
};

class WindowSubclassHelper
{
  protected:
    HWND m_hWnd;
  public:
    WindowSubclassHelper()
      : m_hWnd(NULL)
    {}
    ~WindowSubclassHelper()
    {
      unsubclass();
    }
    bool isSubclassed() const { return m_hWnd != NULL; }
    void subclass(HWND hWnd, WNDPROC nNewProc)
    {
      unsubclass();
      m_hWnd = hWnd;
      WNDPROC oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)nNewProc);
      ::SetProp(hWnd, _T("ExtBarSubclass"), (HANDLE)oldWndProc);
    }
    void unsubclass()
    {
      if (isSubclassed())
      {
        SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)getSubclass(m_hWnd));
        ::RemoveProp(m_hWnd, _T("ExtBarSubclass"));
        m_hWnd = NULL;
      }
    }
    static WNDPROC getSubclass(HWND hWnd) { return (WNDPROC)::GetProp(hWnd, _T("ExtBarSubclass")); }
};

// Elements implementation

class ElementImplIface
{
  public:
    struct Traverser
    {
      virtual bool traverse(ExtBar::Element *pElement) = 0;
    };
  public:
    virtual HWND window() const = 0;
    virtual bool isA(HWND hWnd) const = 0;
    virtual OdUInt32 size() const = 0;
    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd) = 0;
    virtual void setVisible(bool bVisible) = 0;
    virtual bool traverse(Traverser *pTraverser) = 0;
    virtual bool answerCommand(HWND hWnd, UINT nCommand, ExtBar::Element *&pMutation, bool *bKillFocus) = 0;
    virtual bool answerNotify(HWND hWnd, UINT nCode, NMHDR *pData, ExtBar::Element *&pMutation, bool *bKillFocus) = 0;
  protected:
    void updateExtBar();
};

template <typename Iface>
class ElementImpl : public Iface, public ElementImplIface
{
  protected:
    HWND m_hWnd;
    OdUInt32 m_nId;
    OdUInt32 m_nSize;
    void *m_pSomeData;
  public:
    ElementImpl(OdUInt32 nId = 0, OdUInt32 nSize = 0)
      : m_hWnd(NULL)
      , m_nId(nId)
      , m_nSize(nSize)
      , m_pSomeData(NULL)
    { }
    ~ElementImpl()
    {
      if (m_hWnd)
        ::DestroyWindow(m_hWnd);
    }

    virtual HWND window() const { return m_hWnd; }
    virtual bool isA(HWND hWnd) const { return m_hWnd == hWnd; }
    virtual OdUInt32 size() const { return m_nSize; }
    virtual void setVisible(bool bVisible) { setVisibleImpl(m_hWnd, bVisible); }
    virtual bool traverse(Traverser *pTraverser) { return pTraverser->traverse(this); }
    virtual bool answerCommand(HWND /*hWnd*/, UINT /*nCommand*/, ExtBar::Element *& /*pMutation*/, bool * /*bKillFocus*/) { return false; }
    virtual bool answerNotify(HWND /*hWnd*/, UINT /*nCode*/, NMHDR * /*pData*/, ExtBar::Element *& /*pMutation*/, bool * /*bKillFocus*/) { return false; }

    virtual OdUInt32 elementId() const { return m_nId; }
    //virtual ElementType elementType() const = 0;
    //virtual Element *elementInterface(ElementType type) = 0;
    virtual void setDisabled(bool bDisable) { if (m_hWnd) ::EnableWindow(m_hWnd, (bDisable) ? FALSE : TRUE); }
    virtual bool isDisabled() const { if (m_hWnd && ::IsWindowEnabled(m_hWnd)) return false; return true; }
    virtual ElementImplIface *getImpl() const { return (ElementImplIface*)this; }
    virtual void attachSomeData(void *pData) { m_pSomeData = pData; }
    virtual void* getAttachedData() const { return m_pSomeData; }

    void setFont(HWND hWnd)
    {
      HWND hParent = ::GetParent(hWnd); // Get ExtBar window
      if (hParent) hParent = ::GetParent(hParent); // ExtBar owner
      HFONT hFont = NULL;
      if (hParent) hFont = (HFONT)GetWindowFont(hParent);
      if (!hFont)
        SetWindowFont(hWnd, (HFONT)::GetStockObject(DEFAULT_GUI_FONT), FALSE);
      else
        SetWindowFont(hWnd, hFont, FALSE);
    }
    void setVisibleImpl(HWND hWnd, bool bVisible)
    {
      if (hWnd && (!!::IsWindowVisible(hWnd) != bVisible))
        ::ShowWindow(hWnd, (bVisible) ? SW_SHOW : SW_HIDE);
    }
};

template <typename Iface>
class ElementImplRedir : public Iface
{
  protected:
    ExtBar::Element *m_pRedir;
  public:
    ElementImplRedir(ExtBar::Element *pRedir = NULL)
      : m_pRedir(pRedir)
    {
    }

    void setRedir(ExtBar::Element *pRedir) { m_pRedir = pRedir; }
    ElementImpl<ExtBar::Element> *element() const { return static_cast<ElementImpl<ExtBar::Element>*>(m_pRedir); }
    template <typename Iface> ElementImpl<Iface> *typized() const { return static_cast<ElementImpl<Iface>*>(m_pRedir); }

    virtual OdUInt32 elementId() const { return m_pRedir->elementId(); }
    virtual ExtBar::ElementType elementType() const { return m_pRedir->elementType(); }
    virtual ExtBar::Element *elementInterface(ExtBar::ElementType type) { return m_pRedir->elementInterface(type); }
    virtual void setDisabled(bool bDisable) { m_pRedir->setDisabled(bDisable); }
    virtual bool isDisabled() const { return m_pRedir->isDisabled(); }
    virtual ElementImplIface *getImpl() const { return m_pRedir->getImpl(); }
    virtual void attachSomeData(void *pData) { m_pRedir->attachSomeData(pData); }
    virtual void* getAttachedData() const { return m_pRedir->getAttachedData(); }
};

class TextElementDataImpl : public ElementImplRedir<ExtBar::TextElement>
{
  public:
    TextElementDataImpl(ExtBar::Element *pRedir = NULL)
      : ElementImplRedir<ExtBar::TextElement>(pRedir)
    {}

    virtual OdString getText() const
    {
      OdString buf;
      if (!element()->window())
        return buf;
      buf = ::WinTextAsString(element()->window());
      return buf;
    }
    virtual void setText(const OdString &text)
    {
      if (element()->window())
        ::SetWindowText(element()->window(), text.c_str());
    }
};

class ColorElementDataImpl : public ElementImplRedir<ExtBar::ColorElement>
{
  protected:
    COLORREF m_color;
    DCObjectContainer m_colorBrush;
  public:
    ColorElementDataImpl(ExtBar::Element *pRedir = NULL, COLORREF color = RGB(255, 255, 255))
      : ElementImplRedir<ExtBar::ColorElement>(pRedir)
      , m_color(color)
    { createBrush(); }
    ~ColorElementDataImpl()
    { }

    virtual COLORREF getColor() const
    {
      return m_color;
    }
    virtual void setColor(COLORREF color)
    {
      m_color = color & 0x00FFFFFF;
      createBrush();
      if (element()->window())
        ::InvalidateRect(element()->window(), NULL, FALSE);
    }
    void createBrush() { m_colorBrush.set(::CreateSolidBrush(m_color)); }
    HBRUSH getBrush() const { return m_colorBrush.asBrush(); }
};

#define kTextInterfaceAlias       ExtBar::kText: case ExtBar::kEditBox
#define kCheckInterfaceAlias      ExtBar::kCheckButton: case ExtBar::kCheckBox: case ExtBar::kRadioButton
#define kColorInterfaceAlias      ExtBar::kColorButton
#define kRadioGroupInterfaceAlias ExtBar::kRadioGroup
#define kListInterfaceAlias       ExtBar::kComboBox: case ExtBar::kListBox
#define kRangeInterfaceAlias      ExtBar::kRangeBox
#define kGroupInterfaceAlias      ExtBar::kGroup

class TextElementImpl : public ElementImpl<ExtBar::Element>
{
  protected:
    TextElementDataImpl m_textData;
    ColorElementDataImpl m_colorData;
    OdUInt32 m_nLines;
  public:
    TextElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, OdUInt32 nLines, COLORREF color, bool bBorder)
      : ElementImpl<ExtBar::Element>(nId, nSize), m_colorData(NULL, color), m_nLines(nLines)
    {
      m_textData.setRedir(this); m_colorData.setRedir(this);
      m_hWnd = ::CreateWindowEx((bBorder) ? WS_EX_STATICEDGE : 0, _T("STATIC"), _T(""),
        WS_CHILD | WS_VISIBLE | ((nLines <= 1) ? SS_CENTERIMAGE : 0), -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      setFont(m_hWnd);
    }

    COLORREF color() const { return m_colorData.getColor(); }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kText; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kTextInterfaceAlias:
          return &m_textData;
        case kColorInterfaceAlias:
          return &m_colorData;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset, (int)(m_nLines + nStringsAdd) * nStringHeight - ElOffset, TRUE);
      return m_nLines + nStringsAdd;
    }
};

class PlaceholderElementImpl : public ElementImpl<ExtBar::Element>
{
  public:
    PlaceholderElementImpl(OdUInt32 nSize)
      : ElementImpl<ExtBar::Element>(0, nSize)
    {
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kPlaceholder; }
    virtual Element *elementInterface(ExtBar::ElementType /*type*/) { return NULL; }

    virtual OdUInt32 enplace(OdUInt32 /*nX*/, OdUInt32 /*nY*/, OdUInt32 /*nWidth*/, OdUInt32 /*nStringHeight*/, OdUInt32 nStringsAdd) { return nStringsAdd + 1; }
};

class ButtonElementImpl : public ElementImpl<ExtBar::Element>
{
  protected:
    TextElementDataImpl m_textData;
  public:
    ButtonElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, DWORD dwStyle = WS_CHILD | WS_VISIBLE)
      : ElementImpl<ExtBar::Element>(nId, nSize)
    {
      m_textData.setRedir(this);
      m_hWnd = ::CreateWindowEx(0, _T("BUTTON"), _T(""), dwStyle, -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      setFont(m_hWnd);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kButton; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kTextInterfaceAlias:
          return &m_textData;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset, (int)(nStringsAdd + 1) * nStringHeight - ElOffset, TRUE);
      return nStringsAdd + 1;
    }
    virtual bool answerCommand(HWND /*hWnd*/, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (bKillFocus)
        *bKillFocus = true;
      return (nCommand == BN_CLICKED);
    }
};

class CheckElementDataImpl : public ElementImplRedir<ExtBar::CheckElement>
{
  public:
    CheckElementDataImpl(ExtBar::Element *pRedir = NULL)
      : ElementImplRedir<ExtBar::CheckElement>(pRedir)
    {}

    virtual bool isChecked() const
    {
      if (!element()->window())
        return false;
      return Button_GetCheck(element()->window()) == BST_CHECKED;
    }
    virtual void setChecked(bool bCheck)
    {
      if (element()->window())
        Button_SetCheck(element()->window(), (bCheck) ? BST_CHECKED : BST_UNCHECKED);
    }
};

class CheckButtonElementImpl : public ButtonElementImpl
{
  protected:
    CheckElementDataImpl m_checkData;
  public:
    CheckButtonElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE)
      : ButtonElementImpl(hOwner, nId, nSize, dwStyle)
    {
      m_checkData.setRedir(this);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kCheckButton; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kCheckInterfaceAlias:
          return &m_checkData;
      }
      return ButtonElementImpl::elementInterface(type);
    }
};

class ColorButtonElementImpl : public ButtonElementImpl
{
  protected:
    ColorElementDataImpl m_colorData;
  public:
    ColorButtonElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize)
      : ButtonElementImpl(hOwner, nId, nSize, WS_CHILD | WS_VISIBLE | ((::IsThemed()) ? 0 : BS_OWNERDRAW))
    {
      m_colorData.setRedir(this);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kColorButton; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kColorInterfaceAlias:
          return &m_colorData;
      }
      return ButtonElementImpl::elementInterface(type);
    }

    COLORREF color() const { return m_colorData.getColor(); }
    HBRUSH colorBrush() const { return m_colorData.getBrush(); }
};

class CheckBoxElementImpl : public CheckButtonElementImpl
{
  public:
    CheckBoxElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, bool bChecked, bool bLText)
      : CheckButtonElementImpl(hOwner, nId, nSize, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | ((bLText) ? (BS_LEFTTEXT | BS_RIGHT) : 0))
    {
      m_checkData.setRedir(this);
      if (bChecked)
        m_checkData.setChecked(bChecked);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kCheckBox; }
};

class RadioGroupElementImpl : public ElementImpl<ExtBar::RadioElement>
{
  protected:
    ExtBar::GroupElement *m_pGroup;
    OdVector<ExtBar::Element*> m_pButtons;
  public:
    RadioGroupElementImpl(OdUInt32 nId, ExtBar::GroupElement *pGroup)
      : ElementImpl<ExtBar::RadioElement>(nId, 0), m_pGroup(pGroup)
    { }

    ExtBar::GroupElement *group() { return m_pGroup; }
    void insert(ExtBar::Element *pButton)
    {
      if (pButton->elementType() == ExtBar::kRadioButton)
        m_pButtons.push_back(pButton);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kRadioGroup; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kRadioGroupInterfaceAlias:
          return this;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 /*nX*/, OdUInt32 /*nY*/, OdUInt32 /*nWidth*/, OdUInt32 /*nStringHeight*/, OdUInt32 /*nStringsAdd*/) { return 0; }

    virtual OdUInt32 checkedElement() const
    {
      OdUInt32 nButtons = m_pButtons.size();
      ExtBar::Element * const *pButtons = m_pButtons.asArrayPtr();
      while (nButtons)
      {
        if (ExtBar::CheckElement::cast(*pButtons)->isChecked())
          return (*pButtons)->elementId();
        nButtons--; pButtons++;
      }
      return 0;
    }
    virtual void setCheckedElement(OdUInt32 nElement)
    {
      OdUInt32 nButtons = m_pButtons.size();
      ExtBar::Element * const *pButtons = m_pButtons.asArrayPtr();
      while (nButtons)
      {
        ExtBar::CheckElement *pCheck = ExtBar::CheckElement::cast(*pButtons);
        bool bCheck = pCheck->elementId() == nElement;
        if (pCheck->isChecked() != bCheck)
          pCheck->setChecked(bCheck);
        nButtons--; pButtons++;
      }
    }
};

class RadioButtonElementImpl : public CheckButtonElementImpl
{
  protected:
    ExtBar::RadioElement *m_pGroup;
  public:
    RadioButtonElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, bool bChecked, ExtBar::RadioElement *pGroup)
      : CheckButtonElementImpl(hOwner, nId, nSize, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON), m_pGroup(pGroup)
    {
      if (bChecked)
        m_checkData.setChecked(bChecked);
      if (pGroup)
        static_cast<RadioGroupElementImpl*>(pGroup)->insert(this);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kRadioButton; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kRadioGroupInterfaceAlias:
          return m_pGroup;
      }
      return CheckButtonElementImpl::elementInterface(type);
    }

    virtual bool answerCommand(HWND hWnd, UINT nCommand, ExtBar::Element *&pMutation, bool *bKillFocus)
    {
      if (CheckButtonElementImpl::answerCommand(hWnd, nCommand, pMutation, bKillFocus))
      {
        if (m_pGroup)
        {
          m_pGroup->setCheckedElement(elementId());
          pMutation = m_pGroup;
        }
        else
          m_checkData.setChecked(!m_checkData.isChecked());
        return true;
      }
      return false;
    }
};

class ComboBoxElementImpl : public ElementImpl<ExtBar::ListElement>
{
  protected:
    OdUInt32 m_nOpenLines;
  public:
    ComboBoxElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nOpenLines)
      : ElementImpl<ExtBar::ListElement>(nId, ExtBar::k8d8), m_nOpenLines(nOpenLines)
    {
      m_hWnd = ::CreateWindowEx(0, _T("COMBOBOX"), _T(""), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT | WS_VSCROLL,
        -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      setFont(m_hWnd);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kComboBox; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kListInterfaceAlias:
          return this;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset, (int)(nStringsAdd + m_nOpenLines) * nStringHeight - ElOffset, TRUE);
      return nStringsAdd + 1;
    }
    virtual bool answerCommand(HWND /*hWnd*/, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (bKillFocus) *bKillFocus = (nCommand == CBN_SELCHANGE) || (nCommand == CBN_SELENDCANCEL);
      return (nCommand == CBN_SELCHANGE);
    }

    virtual void addString(const OdString &string)
    {
      ComboBox_AddString(window(), string.c_str());
    }
    virtual void clear()
    {
      ComboBox_ResetContent(window());
    }
    virtual OdUInt32 getNumStrings() const
    {
      return (OdUInt32)ComboBox_GetCount(window());
    }
    virtual OdString getString(OdUInt32 nString) const
    {
      OdString buf;
      int nText = ComboBox_GetLBTextLen(window(), (int)nString);
      OdChar *pBuf = buf.getBufferSetLength(nText);
      ComboBox_GetLBText(window(), (int)nString, pBuf);
      buf.releaseBuffer(nText);
      return buf;
    }
    virtual int getSelString() const
    {
      return ComboBox_GetCurSel(window());
    }
    virtual void setSelString(int nString)
    {
      ComboBox_SetCurSel(window(), nString);
    }
    virtual void delString(OdUInt32 nString)
    {
      ComboBox_DeleteString(window(), (int)nString);
    }
    virtual void insString(int nWhere, const OdString &string)
    {
      ComboBox_InsertString(window(), nWhere, string.c_str());
    }
};

class ListBoxElementImpl : public ElementImpl<ExtBar::ListElement>
{
  protected:
    OdUInt32 m_nLines;
  public:
    ListBoxElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nLines)
      : ElementImpl<ExtBar::ListElement>(nId, ExtBar::k8d8), m_nLines(nLines)
    {
      m_hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("LISTBOX"), _T(""), WS_CHILD | WS_VISIBLE | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_VSCROLL,
        -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      setFont(m_hWnd);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kListBox; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kListInterfaceAlias:
          return this;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset, (int)(m_nLines + nStringsAdd) * nStringHeight - ElOffset, TRUE);
      return m_nLines + nStringsAdd;
    }
    virtual bool answerCommand(HWND /*hWnd*/, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (bKillFocus) *bKillFocus = (nCommand == LBN_SELCHANGE) || (nCommand == LBN_SELCANCEL);
      return (nCommand == LBN_SELCHANGE);
    }

    virtual void addString(const OdString &string)
    {
      ListBox_AddString(window(), string.c_str());
    }
    virtual void clear()
    {
      ListBox_ResetContent(window());
    }
    virtual OdUInt32 getNumStrings() const
    {
      return (OdUInt32)ListBox_GetCount(window());
    }
    virtual OdString getString(OdUInt32 nString) const
    {
      OdString buf;
      int nText = ListBox_GetTextLen(window(), (int)nString);
      OdChar *pBuf = buf.getBufferSetLength(nText);
      ListBox_GetText(window(), (int)nString, pBuf);
      buf.releaseBuffer(nText);
      return buf;
    }
    virtual int getSelString() const
    {
      return ListBox_GetCurSel(window());
    }
    virtual void setSelString(int nString)
    {
      ListBox_SetCurSel(window(), nString);
    }
    virtual void delString(OdUInt32 nString)
    {
      ListBox_DeleteString(window(), (int)nString);
    }
    virtual void insString(int nWhere, const OdString &string)
    {
      ListBox_InsertString(window(), nWhere, string.c_str());
    }
};

class EditBoxElementImpl : public ElementImpl<ExtBar::Element>
{
  protected:
    TextElementDataImpl m_textData;
    WindowSubclassHelper m_subclass;
    OdUInt32 m_nLines;
    bool m_bMonitorTyping;
  protected:
    static LRESULT CALLBACK noMLEditSubProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      switch (msg)
      {
        case WM_KEYDOWN:
          switch (wParam)
          {
            case VK_RETURN:
              if ((::GetWindowLong(wnd, GWL_STYLE) & ES_WANTRETURN) == 0)
                ::SetFocus(::GetParent(wnd));
            break;
            case VK_ESCAPE:
              ::SetFocus(::GetParent(wnd));
            return 0;
          }
        default:
        return CallWindowProc(WindowSubclassHelper::getSubclass(wnd), wnd, msg, wParam, lParam);
      }
      return 0;
    }
  public:
    EditBoxElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, OdUInt32 nLines, bool bReadOnly, bool bMonitorTyping)
      : ElementImpl<ExtBar::Element>(nId, nSize), m_nLines(nLines), m_bMonitorTyping(bMonitorTyping)
    {
      m_textData.setRedir(this);
      DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE;
      if (nLines > 1) dwStyle |= ES_AUTOVSCROLL | ES_WANTRETURN;
      if (bReadOnly) dwStyle |= ES_READONLY;
      m_hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), dwStyle,
        -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      m_subclass.subclass(m_hWnd, noMLEditSubProc);
      setFont(m_hWnd);
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kEditBox; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kTextInterfaceAlias:
          return &m_textData;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset, (int)(m_nLines + nStringsAdd) * nStringHeight - ElOffset, TRUE);
      return m_nLines + nStringsAdd;
    }
    virtual bool answerCommand(HWND /*hWnd*/, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (bKillFocus)
        *bKillFocus = false;
      return (m_bMonitorTyping) ? (nCommand == EN_CHANGE) : (nCommand == EN_KILLFOCUS);
    }
};

class RangeBoxElementImpl : public ElementImpl<ExtBar::RangeElement>
{
  protected:
    HWND m_hWndRange;
    TextElementDataImpl m_textData;
    WindowSubclassHelper m_subclass;
    OdString m_prevValidData;
    template <typename Type> struct DataCont { Type m_min, m_max, m_pos, m_delta;
      DataCont(Type _min, Type _max, Type _pos, Type _delta) :
      m_min(_min), m_max(_max), m_pos(_pos), m_delta(_delta) {} };
    DataCont<OdInt32> m_dataInt;
    DataCont<OdUInt32> m_dataUInt;
    DataCont<double> m_dataFP;
    bool m_bSigned, m_bFP, m_bUpdateItl;
  protected:
    static LRESULT CALLBACK noMLEditSubProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      switch (msg)
      {
        case WM_KEYDOWN:
          switch (wParam)
          {
            case VK_RETURN:
              ::SetFocus(::GetParent(wnd));
            break;
            case VK_ESCAPE:
              ::SetFocus(::GetParent(wnd));
            return 0;
          }
        default:
        return CallWindowProc(WindowSubclassHelper::getSubclass(wnd), wnd, msg, wParam, lParam);
      }
      return 0;
    }
    static bool isValidChr(WPARAM wParam, bool *bSigned = NULL, bool *bFP = NULL)
    {
      if (wParam < ' ')
        return true;
      switch (wParam) {
        case _T('-'): if (bSigned) *bSigned = true;
        case _T('.'): if (bFP && (wParam == _T('.'))) *bFP = true;
        case _T('0'): case _T('1'): case _T('2'): case _T('3'): case _T('4'):
        case _T('5'): case _T('6'): case _T('7'): case _T('8'): case _T('9'):
        case _T('\b'):
        return true;
      }
      return false;
    }
    static LRESULT CALLBACK noMLEditSubProc_FP(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      if (msg == WM_CHAR && !isValidChr(wParam))
      { MessageBeep(0); return 0; }
      return noMLEditSubProc(wnd, msg, wParam, lParam);
    }
    static LRESULT CALLBACK noMLEditSubProc_Int(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      bool bFP = false;
      if (msg == WM_CHAR && (!isValidChr(wParam, NULL, &bFP) || bFP))
      { MessageBeep(0); return 0; }
      return noMLEditSubProc(wnd, msg, wParam, lParam);
    }
    static LRESULT CALLBACK noMLEditSubProc_UInt(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      bool bFP = false, bSigned = false;
      if (msg == WM_CHAR && (!isValidChr(wParam, &bSigned, &bFP) || bFP || bSigned))
      { MessageBeep(0); return 0; }
      return noMLEditSubProc(wnd, msg, wParam, lParam);
    }
    void updateSubclassing()
    {
      if (m_bFP)
        m_subclass.subclass(m_hWnd, noMLEditSubProc_FP);
      else if (m_bSigned)
        m_subclass.subclass(m_hWnd, noMLEditSubProc_Int);
      else
        m_subclass.subclass(m_hWnd, noMLEditSubProc_UInt);
    }

    bool validateText(const OdString &string)
    {
      const OdChar *it = string.c_str();
      const OdChar *end = it + string.getLength();
      bool decimalPoint = false;
      int minSize = 0;
      if ((string.getLength() > 0) && ((m_bFP || m_bSigned) && *it == '-'))
      {
        it++; minSize++;
      }
      while (it != end)
      {
        if (m_bFP && (*it == '.'))
        {
          if (!decimalPoint) decimalPoint = true;
          else break;
        }
        else if (!isdigit(*it))
          break;
        ++it;
      }
      return (string.getLength() > minSize) && (it == end);
    }
    void validateText()
    {
      OdString txt = m_textData.getText();
      if (txt.isEmpty())
        return;
      if (!validateText(txt))
      {
        int curPos = HIWORD(Edit_GetSel(window())) - 1;
        m_textData.setText(m_prevValidData);
        if (curPos > 0)
          Edit_SetSel(window(), curPos, curPos);
      }
      else
      {
        m_prevValidData = txt;
        revalidate();
      }
    }
    void revalidate(int *delta = NULL)
    {
      m_bUpdateItl = !delta;
      if (m_bFP)
      {
        double fpVal = (delta) ? (m_dataFP.m_pos + m_dataFP.m_delta * *delta) : odStrToF(m_textData.getText().c_str());
        setPosFP(fpVal);
      }
      else if (m_bSigned)
      {
        OdInt32 intVal = (delta) ? (m_dataInt.m_pos + m_dataInt.m_delta * *delta) : odStrToInt(m_textData.getText().c_str());
        setPosInt(intVal);
      }
      else
      {
        OdUInt32 uintVal = (delta) ? (m_dataUInt.m_pos + m_dataInt.m_delta * *delta) : odStrToUInt(m_textData.getText().c_str());
        if (delta && ((m_dataInt.m_delta * *delta) < 0 && OdUInt32(-(m_dataInt.m_delta * *delta)) > m_dataUInt.m_pos))
          uintVal = 0;
        setPosUInt(uintVal);
      }
      m_bUpdateItl = false;
    }
    void update(OdString *getter = NULL)
    {
      if (m_bUpdateItl && !getter)
        return;
      OdString text;
      if (m_bFP)
        text.format(OdString().format(OD_T("%%.%uf"), (unsigned)m_dataUInt.m_delta).c_str(), m_dataFP.m_pos);
      else if (m_bSigned)
        text.format(OD_T("%d"), (int)m_dataInt.m_pos);
      else
        text.format(OD_T("%u"), (unsigned)m_dataUInt.m_pos);
      if (getter)
        *getter = text;
      else
        m_textData.setText(m_prevValidData = text);
    }
    // Number of digits after floating point
    OdUInt32 numFPDigs(double nDig) const {
      nDig -= (int)nDig;
      if (OdZero(nDig))
        return 2;
      OdUInt32 nCmp = 0;
      while (nDig < 1.0)
        nDig *= 10, nCmp++;
      if (nCmp < 1) nCmp++;
      return nCmp;
    }
    // Converters
    OdUInt32 intToUInt(OdInt32 nVal) const { if (nVal < 0) nVal = 0; return (OdUInt32)nVal; }
    OdInt32 uintToInt(OdUInt32 nVal) const { if (nVal > INT_MAX) nVal = INT_MAX; return (OdInt32)nVal; }
    double intToDouble(OdInt32 nVal) const { return (double)nVal; }
    double uintToDouble(OdUInt32 nVal) const { return (double)nVal; }
    OdInt32 doubleToInt(double fVal) const { return (OdInt32)fVal; }
    OdUInt32 doubleToUInt(double fVal) const { if (fVal < 0.0) fVal = 0.0; return (OdUInt32)fVal; }
    OdInt32 doubleToDelta(double fVal) const { if (fVal > -1.0 && fVal < 0.0) fVal = -1.0;
                                               if (fVal < 1.0 && fVal > 0.0) fVal = 1.0; return (OdInt32)fVal; }
    // Swappers
    void swapInt() { if (m_dataInt.m_min > m_dataInt.m_max) { OdInt32 tmp = m_dataInt.m_min;
      m_dataInt.m_min = m_dataInt.m_max; m_dataInt.m_max = tmp; m_dataInt.m_delta = -m_dataInt.m_delta; } }
    void swapUInt() { if (m_dataUInt.m_min > m_dataUInt.m_max) { OdUInt32 tmp = m_dataUInt.m_min;
      m_dataUInt.m_min = m_dataUInt.m_max; m_dataUInt.m_max = tmp; m_dataInt.m_delta = -m_dataInt.m_delta; } }
    void swapFP() { if (m_dataFP.m_min > m_dataFP.m_max) { double tmp = m_dataFP.m_min;
      m_dataFP.m_min = m_dataFP.m_max; m_dataFP.m_max = tmp; m_dataFP.m_delta = -m_dataFP.m_delta; } }
    // Clampers
    void clampInt() { if (m_dataInt.m_pos < m_dataInt.m_min) m_dataInt.m_pos = m_dataInt.m_min;
                 else if (m_dataInt.m_pos > m_dataInt.m_max) m_dataInt.m_pos = m_dataInt.m_max; }
    void clampUInt() { if (m_dataUInt.m_pos < m_dataUInt.m_min) m_dataUInt.m_pos = m_dataUInt.m_min;
                  else if (m_dataUInt.m_pos > m_dataUInt.m_max) m_dataUInt.m_pos = m_dataUInt.m_max; }
    void clampFP() { if (m_dataFP.m_pos < m_dataFP.m_min) m_dataFP.m_pos = m_dataFP.m_min;
                else if (m_dataFP.m_pos > m_dataFP.m_max) m_dataFP.m_pos = m_dataFP.m_max; }
    // Propagators
    void propagateInt(bool bMinMax = true) { m_dataUInt.m_pos = intToUInt(m_dataInt.m_pos);
      if (bMinMax) { m_dataUInt.m_min = intToUInt(m_dataInt.m_min); m_dataUInt.m_max = intToUInt(m_dataInt.m_max);
      m_dataFP.m_min = intToDouble(m_dataInt.m_min); m_dataFP.m_max = intToDouble(m_dataInt.m_max); }
      m_dataFP.m_pos = intToDouble(m_dataInt.m_pos); m_dataFP.m_delta = intToDouble(m_dataInt.m_delta); }
    void propagateUInt(bool bMinMax = true) { m_dataInt.m_pos = uintToInt(m_dataUInt.m_pos);
      if (bMinMax) { m_dataInt.m_min = uintToInt(m_dataUInt.m_min); m_dataInt.m_max = uintToInt(m_dataUInt.m_max);
      m_dataFP.m_min = uintToDouble(m_dataUInt.m_min); m_dataFP.m_max = uintToDouble(m_dataUInt.m_max); }
      m_dataFP.m_pos = uintToDouble(m_dataUInt.m_pos); m_dataFP.m_delta = intToDouble(m_dataInt.m_delta); }
    void propagateDouble(bool bMinMax = true) { m_dataInt.m_pos = doubleToInt(m_dataFP.m_pos);
      if (bMinMax) { m_dataInt.m_min = doubleToInt(m_dataFP.m_min); m_dataInt.m_max = doubleToInt(m_dataFP.m_max);
      m_dataUInt.m_min = doubleToUInt(m_dataFP.m_min); m_dataUInt.m_max = doubleToUInt(m_dataFP.m_max); }
      m_dataUInt.m_pos = doubleToUInt(m_dataFP.m_pos); m_dataInt.m_delta = doubleToDelta(m_dataFP.m_delta); }
  public:
    RangeBoxElementImpl(HWND hOwner, OdUInt32 nId, OdUInt32 nSize, bool bSigned, bool bFP)
      : ElementImpl<ExtBar::RangeElement>(nId, nSize)
      , m_dataInt(INT_MIN, INT_MAX, 0, 1)
      , m_dataUInt(0, UINT_MAX, 0, 2)
      , m_dataFP(DBL_MIN, DBL_MAX, 0.0, 0.01)
      , m_bSigned(bSigned), m_bFP(bFP), m_bUpdateItl(false)
    {
      m_textData.setRedir(this);
      DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE;
      m_hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), dwStyle,
        -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      m_hWndRange = ::CreateWindowEx(0, _T("msctls_updown32"), _T(""), WS_CHILD | WS_VISIBLE,
        -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
      updateSubclassing();
      Edit_LimitText(m_hWnd, 64);
      setFont(m_hWnd);
      update();
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kRangeBox; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kTextInterfaceAlias:
          return &m_textData;
        case kRangeInterfaceAlias:
          return this;
      }
      return NULL;
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      const OdUInt32 udSize = 14;
      ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
        (int)nWidth - ElOffset - udSize, (int)(nStringsAdd + 1) * nStringHeight - ElOffset, TRUE);
      ::MoveWindow(m_hWndRange, (int)nX + ElOffset / 2 + ((int)nWidth - ElOffset - udSize), (int)nY + ElOffset / 2,
        udSize, (int)(nStringsAdd + 1) * nStringHeight - ElOffset, TRUE);
      return nStringsAdd + 1;
    }
    virtual void setVisible(bool bVisible)
    {
      setVisibleImpl(m_hWnd, bVisible);
      setVisibleImpl(m_hWndRange, bVisible);
    }
    virtual bool isA(HWND hWnd) const { return (m_hWnd == hWnd || hWnd == m_hWndRange);  }
    virtual bool answerCommand(HWND hWnd, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (hWnd != m_hWnd)
        return false;
      if (bKillFocus)
        *bKillFocus = false;
      if (nCommand == EN_CHANGE)
        validateText();
      else if (nCommand == EN_KILLFOCUS)
        update();
      //else if (nCommand == EN_UPDATE)
      //  m_prevValidData = m_textData.getText();
      return (nCommand == EN_KILLFOCUS);
    }
    virtual bool answerNotify(HWND hWnd, UINT nCode, NMHDR *pData, ExtBar::Element *&pMutation, bool *bKillFocus)
    {
      if (hWnd != m_hWndRange)
        return false;
      bool bUpdated = false;
      if (nCode == UDN_DELTAPOS)
      {
        NMUPDOWN *pCh = reinterpret_cast<NMUPDOWN*>(pData);
        revalidate(&pCh->iDelta); bUpdated = true;
        if (bKillFocus)
          *bKillFocus = true;
      }
      return bUpdated;
    }
    // RangeElementInt
    virtual OdInt32 getRangeMinInt() const { return m_dataInt.m_min; }
    virtual OdInt32 getRangeMaxInt() const { return m_dataInt.m_max; }
    virtual void setRangeInt(OdInt32 nMin, OdInt32 nMax)
    { m_dataInt.m_min = nMin; m_dataInt.m_max = nMax; swapInt(); clampInt(); propagateInt(); update(); }
    virtual OdInt32 getDeltaInt() const { return m_dataInt.m_delta; }
    virtual void setDeltaInt(OdInt32 nDelta) { m_dataInt.m_delta = nDelta; m_dataFP.m_delta = intToDouble(m_dataInt.m_delta); }
    virtual OdInt32 getPosInt() const { return m_dataInt.m_pos; }
    virtual void setPosInt(OdInt32 nPos) { m_dataInt.m_pos = nPos; clampInt(); propagateInt(false); update(); }
    virtual void setInt(OdInt32 nPos, OdInt32 nMin, OdInt32 nMax, OdInt32 nDelta)
    { setDeltaInt(nDelta); setRangeInt(nMin, nMax); setPosInt(nPos); }
    // RangeElementUInt
    virtual OdUInt32 getRangeMinUInt() const { return m_dataUInt.m_min; }
    virtual OdUInt32 getRangeMaxUInt() const { return m_dataUInt.m_max; }
    virtual void setRangeUInt(OdUInt32 nMin, OdUInt32 nMax)
    { m_dataUInt.m_min = nMin; m_dataUInt.m_max = nMax; swapUInt(); clampUInt(); propagateUInt(); update(); }
    virtual OdUInt32 getPosUInt() const { return m_dataUInt.m_pos; }
    virtual void setPosUInt(OdUInt32 nPos) { m_dataUInt.m_pos = nPos; clampUInt(); propagateUInt(false); update(); }
    virtual void setUInt(OdUInt32 nPos, OdUInt32 nMin, OdUInt32 nMax, OdInt32 nDelta)
    { setDeltaInt(nDelta); setRangeUInt(nMin, nMax); setPosUInt(nPos); }
    // RangeElementFP
    virtual double getRangeMinFP() const { return m_dataFP.m_min; }
    virtual double getRangeMaxFP() const { return m_dataFP.m_max; }
    virtual void setRangeFP(double fMin, double fMax)
    { m_dataFP.m_min = fMin; m_dataFP.m_max = fMax; swapFP(); clampFP(); propagateDouble(); update(); }
    virtual double getDeltaFP() const { return m_dataFP.m_delta; }
    virtual void setDeltaFP(double fDelta) { m_dataFP.m_delta = fDelta; m_dataInt.m_delta = doubleToDelta(m_dataFP.m_delta); m_dataUInt.m_delta = numFPDigs(m_dataFP.m_delta); }
    virtual double getPosFP() const { return m_dataFP.m_pos; }
    virtual void setPosFP(double fPos) { m_dataFP.m_pos = fPos; clampFP(); propagateDouble(false); update(); }
    virtual void setFP(double fPos, double fMin, double fMax, double fDelta)
    { setDeltaFP(fDelta); setRangeFP(fMin, fMax); setPosFP(fPos); }
    // RangeElement
    virtual bool isFP() const { return m_bFP; }
    virtual void setFP(bool bFP) { m_bFP = bFP; updateSubclassing(); update(); }
    virtual bool isSigned() const { return m_bSigned; }
    virtual void setSigned(bool bSigned) { m_bSigned = bSigned; updateSubclassing(); update(); }
};

typedef OdSharedPtr<ExtBar::Element> ElementPtr;

class GroupElementImpl : public ElementImpl<ExtBar::GroupElement>
{
  friend class ExtBarImpl;
  protected:
    struct String
    {
      typedef OdVector<ElementPtr> Elements;
      Elements m_elems;

      OdUInt32 size() const
      {
        OdUInt32 nAccum = 0;
        OdUInt32 nSize = m_elems.size();
        const ElementPtr *pData = m_elems.getPtr();
        while (nSize)
        {
          nAccum += (*pData)->getImpl()->size();
          nSize--; pData++;
        }
        return nAccum;
      }
      bool traverse(Traverser *pTraverser)
      {
        OdUInt32 nSize = m_elems.size();
        const ElementPtr *pData = m_elems.getPtr();
        while (nSize)
        {
          if (!(*pData)->getImpl()->traverse(pTraverser))
            return false;
          nSize--; pData++;
        }
        return true;
      }
    };
    typedef OdSharedPtr<String> StringPtr;
    struct Strings
    {
      typedef OdVector<StringPtr> Elements;
      Elements m_strings;
      void insert(ExtBar::Element *pElem, OdUInt32 nStrings)
      {
        String *pString = NULL;
        if (!m_strings.isEmpty() && ((ExtBar::k8d8 - m_strings.last()->size()) >= pElem->getImpl()->size()))
          pString = m_strings.last();
        if (!pString)
          pString = *m_strings.append() = new String;
        pString->m_elems.push_back(pElem);
        for (OdUInt32 ns = 1; ns < nStrings; ns++)
        {
          (*m_strings.append()) = new String;
          m_strings.last()->m_elems.push_back(new PlaceholderElementImpl(ExtBar::k8d8));
        }
      }
      bool traverse(Traverser *pTraverser)
      {
        OdUInt32 nSize = m_strings.size();
        StringPtr *pData = m_strings.asArrayPtr();
        while (nSize)
        {
          if (!(*pData)->traverse(pTraverser))
            return false;
          nSize--; pData++;
        }
        return true;
      }
    };
  protected:
    bool m_baseGroup, m_expandeable, m_expanded;
    TextElementDataImpl m_textData;
    Strings m_strings;
    HWND m_hWndExpandBox;
    WindowSubclassHelper m_subclass;
  protected:
    static LRESULT CALLBACK expGBSubProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      switch (msg)
      {
        case WM_COMMAND:
          if (HIWORD(wParam) == BN_CLICKED)
          {
            ::SetProp((HWND)lParam, _T("ButtonState"),
              HANDLE((OdIntPtr)((::GetProp((HWND)lParam, _T("ButtonState")) == HANDLE((OdIntPtr)BST_CHECKED)) ? BST_UNCHECKED : BST_CHECKED)));
            ::SendMessage(::GetParent(wnd), msg, wParam, lParam);
          }
        break;
        case WM_DRAWITEM:
        {
          LPDRAWITEMSTRUCT pDis = (LPDRAWITEMSTRUCT)lParam;
          HGDIOBJ hPrevPen = ::SelectObject(pDis->hDC, ::GetStockObject(BLACK_PEN));
          HGDIOBJ hPrevBrush = ::SelectObject(pDis->hDC, ::GetStockObject(WHITE_BRUSH));
          ::Rectangle(pDis->hDC, pDis->rcItem.left, pDis->rcItem.top, pDis->rcItem.right, pDis->rcItem.bottom);
          ::MoveToEx(pDis->hDC, pDis->rcItem.left + 2, pDis->rcItem.top + ((pDis->rcItem.bottom - pDis->rcItem.top) >> 1), NULL);
          ::LineTo(pDis->hDC, pDis->rcItem.right - 2, pDis->rcItem.top + ((pDis->rcItem.bottom - pDis->rcItem.top) >> 1));
          if (::GetProp(pDis->hwndItem, _T("ButtonState")) == HANDLE((OdIntPtr)BST_CHECKED))
          {
            ::MoveToEx(pDis->hDC, pDis->rcItem.left + ((pDis->rcItem.right - pDis->rcItem.left) >> 1), pDis->rcItem.top + 2, NULL);
            ::LineTo(pDis->hDC, pDis->rcItem.left + ((pDis->rcItem.right - pDis->rcItem.left) >> 1), pDis->rcItem.bottom - 2);
          }
          ::SelectObject(pDis->hDC, hPrevBrush); ::SelectObject(pDis->hDC, hPrevPen);
        }
        break;
        default:
          return CallWindowProc(WindowSubclassHelper::getSubclass(wnd), wnd, msg, wParam, lParam);
      }
      return 0;
    }
  public:
    GroupElementImpl(bool bLayout, HWND hOwner, OdUInt32 nId, bool bExpandeable, bool bExpanded)
      : ElementImpl<ExtBar::GroupElement>((bLayout) ? 0 : nId, (bLayout) ? 0 : ExtBar::k8d8)
      , m_baseGroup(bLayout), m_expandeable(bExpandeable), m_expanded(bExpandeable ? bExpanded : true)
      , m_hWndExpandBox(NULL)
    {
      m_textData.setRedir(this);
      if (!bLayout)
      {
        m_hWnd = ::CreateWindowEx(WS_EX_TRANSPARENT, _T("BUTTON"), _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_GROUPBOX, -20, -20, -10, -10, hOwner, NULL, ExtBarAppInstance, NULL);
        setFont(m_hWnd);
        if (bExpandeable)
        {
          m_hWndExpandBox = ::CreateWindowEx(0, _T("BUTTON"), _T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_OWNERDRAW, -20, -20, -10, -10, m_hWnd, NULL, ExtBarAppInstance, NULL);
          setFont(m_hWndExpandBox);
          ::SetProp(m_hWndExpandBox, _T("ButtonState"), HANDLE((OdIntPtr)((!bExpanded) ? BST_CHECKED : BST_UNCHECKED)));
          m_subclass.subclass(m_hWnd, expGBSubProc);
        }
      }
    }

    virtual ExtBar::ElementType elementType() const { return ExtBar::kGroup; }
    virtual Element *elementInterface(ExtBar::ElementType type)
    {
      switch (type)
      {
        case kTextInterfaceAlias:
          return &m_textData;
        case kGroupInterfaceAlias:
          return this;
      }
      return NULL;
    }

    virtual bool isLayoutGroup() const { return m_baseGroup; }
    virtual bool isExpandeable() const { return m_expandeable; }
    virtual void setExpanded(bool bExpand)
    {
      if (m_expandeable && (m_expanded != bExpand))
      {
        m_expanded = bExpand;
        updateExtBar();
      }
    }
    virtual bool isExpanded() const { return m_expanded; }
    virtual OdUInt32 getNumElements() const
    {
      struct ElemCounter : public ElementImplIface::Traverser
      {
        OdUInt32 m_nCounter;
        virtual bool traverse(ExtBar::Element * /*pElement*/)
        {
          m_nCounter++;
          return true;
        }
        ElemCounter() : m_nCounter(0) { }
      } counter;
      const_cast<GroupElementImpl*>(this)->m_strings.traverse(&counter);
      return counter.m_nCounter;
    }
    virtual Element *getElement(OdUInt32 nElement) const
    {
      struct ElemIterator : public ElementImplIface::Traverser
      {
        OdUInt32 m_nCounter;
        OdUInt32 m_nToFind;
        Element *m_pAnswer;
        virtual bool traverse(ExtBar::Element *pElement)
        {
          if (m_nCounter == m_nToFind)
          {
            m_pAnswer = pElement;
            return false;
          }
          m_nCounter++;
          return true;
        }
        ElemIterator(OdUInt32 nFind) : m_nCounter(0), m_nToFind(nFind), m_pAnswer(NULL) { }
      } iterator(nElement);
      const_cast<GroupElementImpl*>(this)->m_strings.traverse(&iterator);
      return iterator.m_pAnswer;
    }

    void insert(ExtBar::Element *pElem, OdUInt32 nStrings)
    {
      if (nStrings < 1) nStrings = 1;
      OdUInt32 nSize = pElem->getImpl()->size();
      if (nStrings > 1) nSize = ExtBar::k8d8;
      m_strings.insert(pElem, nStrings);
    }

    virtual OdUInt32 enplace(OdUInt32 nX, OdUInt32 nY, OdUInt32 nWidth, OdUInt32 nStringHeight, OdUInt32 nStringsAdd)
    {
      if (window())
      {
        ::MoveWindow(window(), (int)nX + ElOffset / 2, (int)nY + ElOffset / 2,
          (int)nWidth - ElOffset, (int)(nStringsAdd + 1) * nStringHeight - nStringHeight / 2 - ElOffset, TRUE);
        if (m_expandeable)
          ::MoveWindow(m_hWndExpandBox, (int)nWidth - 20, 1, 11, 11, TRUE);
      }
      return nStringsAdd + 1;
    }
    virtual bool isA(HWND hWnd) const { return (m_hWnd == hWnd || hWnd == m_hWndExpandBox); }
    virtual void setVisible(bool bVisible)
    {
      setVisibleImpl(m_hWnd, bVisible);
      setVisibleImpl(m_hWndExpandBox, bVisible);
    }
    virtual bool answerCommand(HWND hWnd, UINT nCommand, ExtBar::Element *& /*pMutation*/, bool *bKillFocus)
    {
      if (hWnd == m_hWndExpandBox)
      {
        if (bKillFocus)
          *bKillFocus = true;
        if (nCommand == BN_CLICKED)
          setExpanded(!isExpanded());
      }
      return false;
    }
    virtual bool traverse(Traverser *pTraverser)
    {
      if (!pTraverser->traverse(this))
        return false;
      return m_strings.traverse(pTraverser);
    }
};

inline bool isDialogBox(HWND hWnd)
{
  TCHAR className[7];
  GetClassName(hWnd, className, sizeof(className) / sizeof(TCHAR));
  return odStrCmp(className, _T("#32770")) == 0;
}
static HWND ownerDialogBox(HWND hWnd, bool bOwner = false)
{
  int nLevelsChk = 2;
  HWND hParent = hWnd;
  while (nLevelsChk)
  {
    if (!bOwner)
      hParent = ::GetParent(hParent);
    else
      bOwner = false;
    if (!hParent)
      break;
    if (isDialogBox(hParent))
      return hParent;
    nLevelsChk--;
  }
  return NULL;
}
inline COLORREF visibleColor(COLORREF color)
{
  return ((GetRValue(color) >= 128 || GetGValue(color) >= 128 || GetBValue(color) >= 128)) ? RGB(0, 0, 0) : RGB(255, 255, 255);
}

class ExtBarImpl : public ExtBar
{
  public:
    HWND m_hWnd;
    FlatScrollBarHelper m_scrollBar;
    int m_scrollPos, m_miceDragDelta;
    OdUInt32 m_nHeight;
    mutable GroupElementImpl *m_pGroup;
    ExtBar::Callback *m_pCallback;
    ExtBar::InplaceCallback *m_pInplaceCallback;

    ExtBarImpl(ExtBar::Callback *pCallback)
      : m_hWnd(NULL)
      , m_scrollPos(0)
      , m_miceDragDelta(0)
      , m_nHeight(0)
      , m_pGroup(NULL)
      , m_pCallback(pCallback)
      , m_pInplaceCallback(NULL)
    {}
    ExtBarImpl(ExtBar::InplaceCallback *pCallback)
      : m_hWnd(NULL)
      , m_scrollPos(0)
      , m_miceDragDelta(0)
      , m_nHeight(0)
      , m_pGroup(NULL)
      , m_pCallback(pCallback)
      , m_pInplaceCallback(pCallback)
    {}
    ~ExtBarImpl()
    {
      clear();
    }
  protected:
    static void onCommand(HWND hwnd, int /*id*/, HWND hwndCtl, UINT codeNotify)
    {
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      ExtBar::Element *pElem = pThis->getElement(hwndCtl);
      bool bUnfocus = false;
      if (pElem && pElem->getImpl()->answerCommand(hwndCtl, codeNotify, pElem, &bUnfocus) && pThis->m_pCallback)
        pThis->m_pCallback->elementChanged(pElem);
      if (bUnfocus)
        ::SetFocus(::GetParent(hwnd)); // Return focus to main window to unblock commands processing
    }
    static void onNotify(HWND hwnd, HWND hwndCtl, LPNMHDR pHdr)
    {
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      ExtBar::Element *pElem = pThis->getElement(hwndCtl);
      bool bUnfocus = false;
      if (pElem && pElem->getImpl()->answerNotify(hwndCtl, pHdr->code, pHdr, pElem, &bUnfocus) && pThis->m_pCallback)
        pThis->m_pCallback->elementChanged(pElem);
      if (bUnfocus)
        ::SetFocus(::GetParent(hwnd)); // Return focus to main window to unblock commands processing
    }

    static void onVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
    {
      if (hwndCtl && (hwnd != hwndCtl))
        return;
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      int nDiff = 0;
      switch (code)
      {
        case SB_LINEUP: nDiff -= 1; break;
        case SB_LINEDOWN: nDiff += 1; break;
        case SB_PAGEUP: nDiff -= 10; break;
        case SB_PAGEDOWN: nDiff += 10; break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
          nDiff = pos - pThis->m_scrollPos;
        break;
        case SB_BOTTOM:
          nDiff = (int)pThis->m_nHeight - pThis->m_scrollPos;
        break;
        case SB_TOP:
          nDiff = -pThis->m_scrollPos;
        break;
        default:
          return;
      }
      pThis->adjustScroller(nDiff);
    }
    static BOOL onSetCursor(HWND hWnd, HWND hwndCursor, UINT codeHitTest, UINT /*msg*/)
    {
      if (!IsScrollBarVisible(hWnd) || (codeHitTest != HTCLIENT) || (hwndCursor != hWnd))
      {
        //if (codeHitTest == HTVSCROLL)
          ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        return FALSE;
      }
      ::SetCursor(::LoadCursor(NULL, IDC_HAND));
      return TRUE;
    }
    static void onLButtonDown(HWND hwnd, BOOL /*fDoubleClick*/, int /*x*/, int y, UINT /*keyFlags*/)
    {
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      pThis->m_miceDragDelta = y;
      ::SetFocus(hwnd);
      // Here cursor can be reseted until capturing willn't be completed (no need reset in LButtonUp)
      ::SetCapture(hwnd);
    }
    static void onLButtonUp(HWND /*hwnd*/, int /*x*/, int /*y*/, UINT /*keyFlags*/)
    {
      ::ReleaseCapture();
    }
    static void onMouseMove(HWND hwnd, int /*x*/, int y, UINT /*keyFlags*/)
    {
      if (::GetCapture() == hwnd)
      {
        ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
        int miceDelta = pThis->m_miceDragDelta - y; pThis->m_miceDragDelta = y;
        if (miceDelta)
        {
          int minPos, maxPos, curPos = pThis->m_scrollBar.pos(hwnd, SB_VERT);
          pThis->m_scrollBar.range(hwnd, SB_VERT, &minPos, &maxPos);
          int newPos = curPos + miceDelta;
          if (newPos < minPos) newPos = minPos; else if (newPos > maxPos) newPos = maxPos;
          const int newPosDelta = newPos - curPos;
          if (newPosDelta)
            pThis->adjustScroller(newPosDelta);
        }
      }
    }

    static HBRUSH onCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type)
    {
      HBRUSH retBrush = NULL;
      HWND hOwnerDlgBox = ownerDialogBox(hwnd);
      if (hOwnerDlgBox)
        retBrush = FORWARD_WM_CTLCOLORSTATIC(hOwnerDlgBox, hdc, hwndChild, SendMessage);
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      ExtBar::Element *pElem = pThis->getElement(hwndChild);
      COLORREF color = RGB(0, 0, 0);
      if (pElem && (pElem->elementType() == ExtBar::kText))
      {
        TextElementImpl *pText = static_cast<TextElementImpl*>(pElem);
        color = pText->color();
        if (color == RGB(0, 0, 0))
          return retBrush;
      }
      else
        return retBrush;
      ::SetTextColor(hdc, color);
      if (!retBrush)
      {
        ::SetBkColor(hdc, ::GetSysColor(COLOR_BTNFACE));
        return (HBRUSH)::GetSysColorBrush(COLOR_BTNFACE);
      }
      return retBrush;
    }
    static void onDrawItem(HWND hwnd, const DRAWITEMSTRUCT *pDIs)
    {
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      ExtBar::Element *pElem = pThis->getElement(pDIs->hwndItem);
      if (pElem && (pElem->elementType() == ExtBar::kColorButton))
      {
        ColorButtonElementImpl *pBtn = static_cast<ColorButtonElementImpl*>(pElem);
        if (pDIs->itemState & ODS_DISABLED)
          ::FillRect(pDIs->hDC, &pDIs->rcItem, ::GetSysColorBrush(COLOR_3DFACE));
        else
          ::FillRect(pDIs->hDC, &pDIs->rcItem, pBtn->colorBrush());
        OdString winText = ::WinTextAsString(pDIs->hwndItem);
        if (!winText.isEmpty())
        {
          ::SetTextColor(pDIs->hDC, visibleColor(pBtn->color()));
          ::SetBkMode(pDIs->hDC, TRANSPARENT);
          RECT drawRect = pDIs->rcItem;
          if (pDIs->itemState & ODS_SELECTED)
            ::OffsetRect(&drawRect, 1, 1);
          if (pDIs->itemState & ODS_DISABLED)
          {
            SIZE outSize; GetTextExtentPoint32(pDIs->hDC, winText.c_str(), winText.getLength(), &outSize);
            RECT drawRect;
            ::SetRect(&drawRect, pDIs->rcItem.left + (pDIs->rcItem.right - pDIs->rcItem.left) / 2 - outSize.cx / 2,
              pDIs->rcItem.top + (pDIs->rcItem.bottom - pDIs->rcItem.top) / 2 - outSize.cy / 2,
              outSize.cx + 1, outSize.cy + 1);
            ::DrawState(pDIs->hDC, (HBRUSH)::GetStockObject(BLACK_BRUSH), NULL, (LPARAM)winText.c_str(), 0,
              drawRect.left, drawRect.top, drawRect.right, drawRect.bottom, DST_TEXT | DSS_DISABLED);
          }
          else
            ::DrawText(pDIs->hDC, winText.c_str(), winText.getLength(), &drawRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        }
        if (pDIs->itemState & ODS_FOCUS)
        {
          RECT drawRect = pDIs->rcItem;
          ::OffsetRect(&drawRect, 1, 1);
          ::InflateRect(&drawRect, -4, -4);
          drawRect.left -= 1; drawRect.top -= 1;
          ::DrawFocusRect(pDIs->hDC, &drawRect);
        }
        ::DrawEdge(pDIs->hDC, const_cast<RECT*>(&pDIs->rcItem), (pDIs->itemState & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);
        if (::GetFocus() == pDIs->hwndItem)
        {
          HGDIOBJ prevPen = ::SelectObject(pDIs->hDC, ::GetStockObject(BLACK_PEN));
          HGDIOBJ prevBrush = ::SelectObject(pDIs->hDC, ::GetStockObject(NULL_BRUSH));
          ::Rectangle(pDIs->hDC, pDIs->rcItem.left, pDIs->rcItem.top, pDIs->rcItem.right, pDIs->rcItem.bottom);
          ::SelectObject(pDIs->hDC, prevBrush); ::SelectObject(pDIs->hDC, prevPen);
        }
      }
    }
    static LRESULT onDrawButton(HWND hwnd, LPNMCUSTOMDRAW custDraw)
    {
      ExtBarImpl *pThis = (ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hwnd, GWL_USERDATA);
      ExtBar::Element *pElem = pThis->getElement(custDraw->hdr.hwndFrom);
      if (pElem && (pElem->elementType() == ExtBar::kColorButton))
      {
        ColorButtonElementImpl *pBtn = static_cast<ColorButtonElementImpl*>(pElem);
        if (pBtn->isDisabled())
          return CDRF_DODEFAULT;
        COLORREF textColor = visibleColor(pBtn->color());
        {
          RECT drawRect = custDraw->rc;
          ::InflateRect(&drawRect, -4, -4);
          HGDIOBJ prevPen = ::SelectObject(custDraw->hdc, ::GetStockObject((GetGValue(textColor)) ? WHITE_PEN : BLACK_PEN));
          HGDIOBJ prevBrush = ::SelectObject(custDraw->hdc, pBtn->colorBrush());
          ::Rectangle(custDraw->hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
          ::SelectObject(custDraw->hdc, prevBrush); ::SelectObject(custDraw->hdc, prevPen);
        }
        OdString winText = ::WinTextAsString(custDraw->hdr.hwndFrom);
        if (!winText.isEmpty())
        {
          SIZE dimensions = { 0 };
          ::GetTextExtentPoint32(custDraw->hdc, winText.c_str(), winText.getLength(), &dimensions);
          const int xPos = (custDraw->rc.right  - dimensions.cx) / 2;
          const int yPos = (custDraw->rc.bottom - dimensions.cy) / 2;
          ::SetBkMode(custDraw->hdc, TRANSPARENT);
          ::SetTextColor(custDraw->hdc, textColor);
          ::TextOut(custDraw->hdc, xPos, yPos, winText.c_str(), winText.getLength());
        }
        return CDRF_SKIPDEFAULT;
      }
      return CDRF_DODEFAULT;
    }

    static LRESULT CALLBACK wndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
    {
      switch (Message)
      {
        HANDLE_MSG(hWindow, WM_COMMAND, onCommand);
        HANDLE_MSG(hWindow, WM_VSCROLL, onVScroll);
        HANDLE_MSG(hWindow, WM_SETCURSOR, onSetCursor);
        HANDLE_MSG(hWindow, WM_LBUTTONDOWN, onLButtonDown);
        HANDLE_MSG(hWindow, WM_LBUTTONUP, onLButtonUp);
        HANDLE_MSG(hWindow, WM_MOUSEMOVE, onMouseMove);
        HANDLE_MSG(hWindow, WM_CTLCOLORSTATIC, onCtlColorStatic);
        HANDLE_MSG(hWindow, WM_DRAWITEM, onDrawItem);
        case WM_NOTIFY:
        {
          LPNMHDR nmhdr = reinterpret_cast<LPNMHDR>(lParam);
          if (nmhdr->code == NM_CUSTOMDRAW)
          {
            LPNMCUSTOMDRAW custDraw = reinterpret_cast<LPNMCUSTOMDRAW>(nmhdr);
            if (custDraw->dwDrawStage == CDDS_PREPAINT)
              return onDrawButton(hWindow, custDraw);
            return CDRF_DODEFAULT;
          }
          else
            onNotify(hWindow, nmhdr->hwndFrom, nmhdr);
        }
        return 0;
        default: return ::DefWindowProc(hWindow, Message, wParam, lParam);
      }
    }

    static bool registerWndClass(HINSTANCE hInst)
    {
      WNDCLASS WndClass;
      WndClass.style = CS_HREDRAW | CS_VREDRAW;
      WndClass.lpfnWndProc = wndProc;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      WndClass.hInstance = hInst;
      WndClass.hIcon = NULL;
      WndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
      WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
      WndClass.lpszMenuName = NULL;
      WndClass.lpszClassName = ExtBarWndClassName;
      return (::RegisterClass(&WndClass)) ? true : false;
    }

    bool isCreated() const { return m_hWnd != NULL; }
  public:
    void create(HWND hOwner, HINSTANCE hInst)
    {
      if (!isCreated())
      {
        if (!ExtBarRegistered++)
          registerWndClass(ExtBarAppInstance = hInst);
        if (!m_pInplaceCallback)
          m_hWnd = ::CreateWindowEx((ownerDialogBox(hOwner, true)) ? 0 : WS_EX_COMPOSITED, ExtBarWndClassName, ExtBarWndClassName,
                                    WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_DLGFRAME | WS_VSCROLL | WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                    hOwner, NULL, hInst, NULL);
        else
          m_hWnd = ::CreateWindowEx(WS_EX_NOACTIVATE, ExtBarWndClassName, ExtBarWndClassName,
                                    WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_DLGFRAME | WS_VSCROLL | WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                    hOwner, NULL, hInst, NULL);
        SetWindowLongPtr(m_hWnd, GWL_USERDATA, (OdIntPtr)this);
        m_scrollPos = 0; m_nHeight = 0;
        m_scrollBar.init(m_hWnd);
      }
    }
    void destroy()
    {
      if (isCreated())
      {
        clear();
        m_scrollBar.uninit(m_hWnd);
        ::DestroyWindow(m_hWnd);
        if (!--ExtBarRegistered)
          ::UnregisterClass(ExtBarWndClassName, ExtBarAppInstance);
        m_hWnd = NULL;
      }
    }

    void adjustScroller(int nDelta)
    {
      RECT cliRect;
      ::GetClientRect(m_hWnd, &cliRect);
      int adjHeight = (int)m_nHeight - (cliRect.bottom - cliRect.top);
      if (adjHeight < 0) adjHeight = 0;
      int adjusted = m_scrollPos + nDelta;
      if (adjusted < 0) adjusted = 0;
      if (adjusted > adjHeight) adjusted = adjHeight;
      nDelta = adjusted - m_scrollPos;
      m_scrollPos = adjusted;
      bool bEnableScroll = ((m_scrollPos != 0) || adjHeight);
      m_scrollBar.enable(m_hWnd, SB_VERT, (!bEnableScroll) ? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
      m_scrollBar.setRange(m_hWnd, SB_VERT, 0, adjHeight, FALSE);
      m_scrollBar.setPos(m_hWnd, SB_VERT, adjusted, TRUE);
      if (nDelta)
      {
        ::ScrollWindow(m_hWnd, 0, -nDelta, NULL, NULL);
        if (nDelta < 0)
        { // Compensate strange effect caused by window composition
          const LONG style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
          if (style & WS_EX_COMPOSITED)
          {
            RECT rect;
            ::SetRect(&rect, cliRect.left, -nDelta, cliRect.right, -nDelta * 2);
            ::InvalidateRect(m_hWnd, /*NULL*/&rect, FALSE);
          }
        }
      }
    }

    void resize(const RECT *rect)
    {
      if (isCreated())
      {
        if (rect)
          ::MoveWindow(m_hWnd, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, TRUE);
        else
        {
          ::UpdateWindow(m_hWnd);
          if (m_scrollPos)
            ::ScrollWindow(m_hWnd, 0, -m_scrollPos, NULL, NULL);
        }
        adjustScroller(0);
      }
    }

    struct UpdateStruct
    {
      OdUInt32 m_nInitWidth;
      OdUInt32 m_nStringLength;
      OdUInt32 m_nCurWidth;
      OdUInt32 m_nCurHeight;
    };
    void update(ExtBar::GroupElement *pGroupElem, UpdateStruct &us, bool bExpanded = true)
    {
      GroupElementImpl *pGroup = static_cast<GroupElementImpl*>(pGroupElem);
      OdUInt32 nStrings = pGroup->m_strings.m_strings.size();
      GroupElementImpl::StringPtr *pStrings = pGroup->m_strings.m_strings.asArrayPtr();
      while (nStrings)
      {
        OdUInt32 nElems = (*pStrings)->m_elems.size();
        ElementPtr *pElems = (*pStrings)->m_elems.asArrayPtr();
        while (nElems)
        {
          OdUInt32 nStringsAdd = 0;
          if ((*pElems)->elementType() == ExtBar::kGroup)
          {
            UpdateStruct usStart = us;
            us.m_nInitWidth += GroupSize / 2; us.m_nStringLength -= GroupSize; us.m_nCurWidth = us.m_nInitWidth;
            if (bExpanded)
              us.m_nCurHeight += StringSize; // Place for group itself
            update(ExtBar::GroupElement::cast(*pElems), us, bExpanded && ExtBar::GroupElement::cast(*pElems)->isExpanded());
            us.m_nInitWidth -= GroupSize / 2; us.m_nStringLength += GroupSize; us.m_nCurWidth = us.m_nInitWidth;
            nStringsAdd = (us.m_nCurHeight - usStart.m_nCurHeight) / StringSize;
          }
          OdUInt32 nWidth = (us.m_nStringLength * (*pElems)->getImpl()->size()) / 8;
          if (bExpanded)
            (*pElems)->getImpl()->enplace(us.m_nCurWidth, us.m_nCurHeight - (nStringsAdd * StringSize), nWidth, StringSize, nStringsAdd);
          (*pElems)->getImpl()->setVisible(bExpanded);
          us.m_nCurWidth += nWidth;
          nElems--; pElems++;
        }
        nStrings--; pStrings++;
        us.m_nCurWidth = us.m_nInitWidth;
        if (bExpanded)
          us.m_nCurHeight += StringSize;
      }
    }
    void clear()
    {
      if (m_pGroup)
      {
        delete m_pGroup;
        m_pGroup = NULL;
      }
      m_nHeight = 0;
      adjustScroller(-m_scrollPos);
    }

    void setCallback(ExtBar::Callback *pCallback)
    {
      m_pCallback = pCallback;
    }

    ExtBar::Element *getElement(OdUInt32 nElementId) const
    {
      struct FindElem : public ElementImplIface::Traverser
      {
        OdUInt32 m_nWho;
        ExtBar::Element *m_pAnswer;
        virtual bool traverse(ExtBar::Element *pElement)
        {
          if (pElement->elementId() == m_nWho)
          {
            m_pAnswer = pElement;
            return false;
          }
          return true;
        }
        FindElem(OdUInt32 nWho) : m_nWho(nWho), m_pAnswer(NULL) { }
      } finder(nElementId);
      getLayoutGroup()->getImpl()->traverse(&finder);
      return finder.m_pAnswer;
    }
    ExtBar::Element *getElement(HWND nElementWnd) const
    {
      struct FindElem : public ElementImplIface::Traverser
      {
        HWND m_hWho;
        ExtBar::Element *m_pAnswer;
        virtual bool traverse(ExtBar::Element *pElement)
        {
          if (pElement->getImpl()->isA(m_hWho))
          {
            m_pAnswer = pElement;
            return false;
          }
          return true;
        }
        FindElem(HWND hWho) : m_hWho(hWho), m_pAnswer(NULL) { }
      } finder(nElementWnd);
      getLayoutGroup()->getImpl()->traverse(&finder);
      return finder.m_pAnswer;
    }

    ExtBar::GroupElement *getLayoutGroup() const
    {
      if (!m_pGroup)
        m_pGroup = new GroupElementImpl(true, NULL, 0, false, true);
      return m_pGroup;
    }
    GroupElementImpl *getLayoutGroupImpl() const
    {
      return static_cast<GroupElementImpl*>(getLayoutGroup());
    }

    ExtBar::GroupElement *createGroup(OdUInt32 nElementId, bool bExpandeable, bool bExpanded)
    {
      if (isCreated())
      {
        ExtBar::GroupElement *pGroup = new GroupElementImpl(false, m_hWnd, nElementId, bExpandeable, bExpanded);
        getLayoutGroupImpl()->insert(pGroup, 1);
        return pGroup;
      }
      return NULL;
    }
    ExtBar::TextElement *createText(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size, OdUInt32 nStrings, bool bBorder, COLORREF color)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        TextElementImpl *pText = new TextElementImpl(m_hWnd, nElementId, size, nStrings, color, bBorder);
        static_cast<GroupElementImpl*>(pGroup)->insert(pText, nStrings);
        return ExtBar::TextElement::cast(pText);
      }
      return NULL;
    }
    void createPlaceholder(ExtBar::GroupElement *pGroup, ExtBar::ElementSize size)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        PlaceholderElementImpl *pPh = new PlaceholderElementImpl(size);
        static_cast<GroupElementImpl*>(pGroup)->insert(pPh, 1);
      }
    }
    ExtBar::TextElement *createButton(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        ButtonElementImpl *pButton = new ButtonElementImpl(m_hWnd, nElementId, size);
        static_cast<GroupElementImpl*>(pGroup)->insert(pButton, 1);
        return ExtBar::TextElement::cast(pButton);
      }
      return NULL;
    }
    ExtBar::TextElement *createCheckButton(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        CheckButtonElementImpl *pButton = new CheckButtonElementImpl(m_hWnd, nElementId, size);
        static_cast<GroupElementImpl*>(pGroup)->insert(pButton, 1);
        return ExtBar::TextElement::cast(pButton);
      }
      return NULL;
    }
    ExtBar::ColorElement *createColorButton(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        ColorButtonElementImpl *pButton = new ColorButtonElementImpl(m_hWnd, nElementId, size);
        static_cast<GroupElementImpl*>(pGroup)->insert(pButton, 1);
        return ExtBar::ColorElement::cast(pButton);
      }
      return NULL;
    }
    ExtBar::TextElement *createCheckBox(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size, bool bChecked, bool bLText)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        CheckBoxElementImpl *pBox = new CheckBoxElementImpl(m_hWnd, nElementId, size, bChecked, bLText);
        static_cast<GroupElementImpl*>(pGroup)->insert(pBox, 1);
        return ExtBar::TextElement::cast(pBox);
      }
      return NULL;
    }
    ExtBar::RadioElement *createRadioGroup(OdUInt32 nElementId, ExtBar::GroupElement *pGroup)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        RadioGroupElementImpl *pRGroup = new RadioGroupElementImpl(nElementId, pGroup);
        static_cast<GroupElementImpl*>(pGroup)->insert(pRGroup, 0);
        return ExtBar::RadioElement::cast(pRGroup);
      }
      return NULL;
    }
    ExtBar::TextElement *createRadioButton(OdUInt32 nElementId, ExtBar::RadioElement *pRGroup, ExtBar::ElementSize size, bool bChecked)
    {
      if (isCreated())
      {
        ExtBar::GroupElement *pGroup = NULL;
        if (pRGroup) pGroup = static_cast<RadioGroupElementImpl*>(pRGroup)->group();
        if (!pGroup) pGroup = getLayoutGroup();
        RadioButtonElementImpl *pButton = new RadioButtonElementImpl(m_hWnd, nElementId, size, bChecked, pRGroup);
        static_cast<GroupElementImpl*>(pGroup)->insert(pButton, 1);
        return ExtBar::TextElement::cast(pButton);
      }
      return NULL;
    }
    ExtBar::ListElement *createComboBox(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, OdUInt32 nOpenLines)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        ComboBoxElementImpl *pList = new ComboBoxElementImpl(m_hWnd, nElementId, nOpenLines);
        static_cast<GroupElementImpl*>(pGroup)->insert(pList, 1);
        return ExtBar::ListElement::cast(pList);
      }
      return NULL;
    }
    ExtBar::ListElement *createListBox(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, OdUInt32 nStrings)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        ListBoxElementImpl *pList = new ListBoxElementImpl(m_hWnd, nElementId, nStrings);
        static_cast<GroupElementImpl*>(pGroup)->insert(pList, nStrings);
        return ExtBar::ListElement::cast(pList);
      }
      return NULL;
    }
    ExtBar::TextElement *createEditBox(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size, OdUInt32 nStrings, bool bReadOnly, bool bMonitorTyping)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        EditBoxElementImpl *pEdit = new EditBoxElementImpl(m_hWnd, nElementId, size, nStrings, bReadOnly, bMonitorTyping);
        static_cast<GroupElementImpl*>(pGroup)->insert(pEdit, nStrings);
        return ExtBar::TextElement::cast(pEdit);
      }
      return NULL;
    }
    ExtBar::RangeElement *createRangeBox(OdUInt32 nElementId, ExtBar::GroupElement *pGroup, ExtBar::ElementSize size, bool bSigned, bool bFP)
    {
      if (isCreated())
      {
        if (!pGroup) pGroup = getLayoutGroup();
        RangeBoxElementImpl *pRange = new RangeBoxElementImpl(m_hWnd, nElementId, size, bSigned, bFP);
        static_cast<GroupElementImpl*>(pGroup)->insert(pRange, 1);
        return ExtBar::RangeElement::cast(pRange);
      }
      return NULL;
    }
    bool getColor(ODCOLORREF &color) const;
    OdString getOpenFileName(const OdChar *pFilter, const OdChar *pDefExt);
    OdString getSaveFileName(const OdChar *pFilter, const OdChar *pDefExt);
};

void ElementImplIface::updateExtBar()
{
  HWND hWnd = ::GetParent(window());
  if (hWnd)
  {
    ExtBarInterface exCal((ExtBarImpl*)(OdIntPtr)GetWindowLongPtr(hWnd, GWL_USERDATA));
    exCal.update();
  }
}

ExtBar *ExtBarManager::createExtBar(ExtBar::Callback *pCallback)
{
  return new ExtBarImpl(pCallback);
}

ExtBar *ExtBarManager::createExtBar(ExtBar::InplaceCallback *pCallback)
{
  return new ExtBarImpl(pCallback);
}

void ExtBarManager::destroyExtBar(ExtBar *pExtBar)
{
  if (pExtBar)
    delete pExtBar;
}

void ExtBarInterface::create(ExtBar::SysHandle sysHandle)
{
  if (extBar())
  { ExtBarImpl *pImpl = static_cast<ExtBarImpl*>(extBar());
    pImpl->create(sysHandle->hOwner, sysHandle->hInstance);
    if (pImpl->m_pCallback)
      pImpl->m_pCallback->fillExtBar(extBar());
  }
}

void ExtBarInterface::destroy(int nReason)
{
  if (extBar())
  { ExtBarImpl *pImpl = static_cast<ExtBarImpl*>(extBar());
    if (pImpl->m_pCallback)
      pImpl->m_pCallback->closeExtBar(extBar(), nReason);
    pImpl->destroy();
  }
}

void ExtBarInterface::resize(const ExtBar::Rect &rect)
{
  if (extBar())
  { ExtBarImpl *pImpl = static_cast<ExtBarImpl*>(extBar());
    RECT cliRect, winRect;
    ::GetWindowRect(pImpl->m_hWnd, &cliRect);
    if (((cliRect.right - cliRect.left) != (rect.right - rect.left)) ||
        ((cliRect.bottom - cliRect.top) != (rect.bottom - rect.top)))
      pImpl->resize(&rect);
    else
      ::MoveWindow(pImpl->m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    ::GetClientRect(pImpl->m_hWnd, &winRect);
    int nDiff = (rect.bottom - rect.top) - (winRect.bottom - winRect.top);
    if (((cliRect.right - cliRect.left) != (rect.right - rect.left)) ||
        (((cliRect.bottom - cliRect.top) - nDiff < (int)pImpl->m_nHeight) != ((rect.bottom - rect.top) - nDiff < (int)pImpl->m_nHeight)))
      update();
  }
}

class CheckNCallReupdateSeq
{
  protected:
    OdUInt32 m_nPrevHeight;
    OdUInt32 &m_pImpl_m_nHeight;
    bool m_bScrollBarVisible;
    ExtBarInterface *m_pExtBar;
    HWND m_hWnd;
  public:
    CheckNCallReupdateSeq(OdUInt32 &pImpl_m_nHeight, OdUInt32 nNewHeight, ExtBarInterface *pExtBar, HWND hWnd)
      : m_nPrevHeight(pImpl_m_nHeight), m_pImpl_m_nHeight(pImpl_m_nHeight), m_pExtBar(pExtBar), m_hWnd(hWnd)
    {
      m_pImpl_m_nHeight = nNewHeight;
      m_bScrollBarVisible = ::IsScrollBarVisible(hWnd);
    }
    ~CheckNCallReupdateSeq()
    {
      const bool bScrollBarVisible = ::IsScrollBarVisible(m_hWnd);
      if ((m_pImpl_m_nHeight != m_nPrevHeight) && (m_bScrollBarVisible != bScrollBarVisible))
        m_pExtBar->update();
    }
};

void ExtBarInterface::update()
{
  if (extBar())
  { ExtBarImpl *pImpl = static_cast<ExtBarImpl*>(extBar());
    RECT cliRect;
    ::GetClientRect(pImpl->m_hWnd, &cliRect);
    ExtBarImpl::UpdateStruct us;
    us.m_nInitWidth = 0;
    us.m_nStringLength = (OdUInt32)(cliRect.right - cliRect.left);
    us.m_nCurWidth = us.m_nCurHeight = 0;
    pImpl->update(pImpl->getLayoutGroup(), us);
    CheckNCallReupdateSeq _nscsync(pImpl->m_nHeight, us.m_nCurHeight, this, pImpl->m_hWnd);
    pImpl->resize(NULL);
  }
}

void ExtBarInterface::clear()
{
  if (extBar())
  {
    static_cast<ExtBarImpl*>(extBar())->clear();
    update();
  }
}

void ExtBarInterface::resetCallback(ExtBar::Callback *pCallback)
{
  if (extBar())
    static_cast<ExtBarImpl*>(extBar())->setCallback(pCallback);
}

bool ExtBarImpl::getColor(ODCOLORREF &color) const
{
  static COLORREF colors[16] =
  {
    RGB(0 , 0 , 0) , RGB(255, 0  , 0)  , RGB(255, 255, 0) , RGB(0 , 255, 0) , RGB(0  , 255, 255), RGB(0  , 0  , 255), RGB(255, 0  , 255), RGB(255, 255, 255),
    RGB(65, 65, 65), RGB(128, 128, 128), RGB(51 , 51 , 51), RGB(80, 80 , 80), RGB(105, 105, 105), RGB(130, 130, 130), RGB(190, 190, 190), RGB(128, 128, 0)
  };
  CHOOSECOLOR cc;
  ZeroMemory(&cc, sizeof(CHOOSECOLOR));
  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = m_hWnd;
  cc.hInstance = (HWND)ExtBarAppInstance;
  cc.rgbResult = RGB(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  cc.lpCustColors = colors;
  cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
  if (::ChooseColor(&cc))
  {
    color = ODRGBA(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult), ODGETALPHA(color));
    return true;
  }
  return false;
}

OdString ExtBarImpl::getOpenFileName(const OdChar *pFilter, const OdChar *pDefExt)
{
  TCHAR fileName[MAX_PATH];
  ZeroMemory(&fileName, sizeof(fileName));
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = pFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = _MAX_PATH;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
  ofn.lpstrDefExt = pDefExt;

  if (GetOpenFileName(&ofn) == FALSE)
    return OdString();
  return fileName;
}

OdString ExtBarImpl::getSaveFileName(const OdChar *pFilter, const OdChar *pDefExt)
{
  TCHAR fileName[MAX_PATH];
  ZeroMemory(&fileName, sizeof(fileName));
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = pFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = _MAX_PATH;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = pDefExt;

  if (GetSaveFileName(&ofn) == FALSE)
    return OdString();
  return fileName;
}

// ExtBarInplace

ExtBar::Point ExtBarInplace::curpos()
{
  ExtBar::Point pt;
  ::GetCursorPos(&pt);
  return pt;
}

ExtBar::Size ExtBarInplace::makesize(int w, int h)
{
  ExtBar::Size sz = { w, h };
  return sz;
}

ExtBar::Rect ExtBarInplace::screenplace(const ExtBar::Point &pt, const ExtBar::Size &sz)
{
  HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mInfo; ZeroMemory(&mInfo, sizeof(MONITORINFO));
  mInfo.cbSize = sizeof(MONITORINFO);
  ::GetMonitorInfo(hMonitor, &mInfo);
  ExtBar::Rect resRect;
  ::SetRect(&resRect, pt.x, pt.y - sz.cy, pt.x + sz.cx, pt.y);
  if (resRect.top < mInfo.rcMonitor.top || resRect.bottom > mInfo.rcMonitor.bottom)
    resRect.top += sz.cy, resRect.bottom += sz.cy;
  if (resRect.left < mInfo.rcMonitor.left || resRect.right > mInfo.rcMonitor.right)
    resRect.left -= sz.cx, resRect.right -= sz.cx;
  // @@@TODO: add solution in case if rect is too large to place it in this point
  return resRect;
}

void ExtBarInplace::fakewndproc()
{
  HWND hwndDlg = static_cast<ExtBarImpl*>(m_pExtBar)->m_hWnd;
  HWND hwndParent = ::GetParent(hwndDlg);
  // @@@TODO: implement auto-hide inplace behavior instead of modal dialog behavior.
  //          Inplace edit can be checked by window style.
  //          Inplace callback (canClose) can be used to turn auto-hide on/off.
  if (hwndParent != NULL) // @@@tmp: do modal behavior
    EnableWindow(hwndParent, FALSE);
  MSG msg;
  for (m_bDone = false; !m_bDone; ::WaitMessage())
  {
    while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
        m_bDone = true;
        PostMessage(NULL, WM_QUIT, 0, 0);
        break;
      }
      if (!::IsDialogMessage(hwndDlg, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
  if (hwndParent != NULL) // @@@tmp: close modal behavior
    EnableWindow(hwndParent, TRUE);
}

void ExtBarInplace::closeImpl()
{
  m_bDone = true;
}

//
