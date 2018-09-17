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
// GLES2 Xml viewer for Windows

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <algorithm>

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif // WM_MOUSEWHEEL
#ifndef WHEEL_DELTA
#define WHEEL_DELTA                     120
#endif // WHEEL_DELTA

#include "OdaCommon.h"
#include "AppClient.h"
#include "ExtBar.h"

#include "resource.h"

static TCHAR AppName[]     = _T("OdXmlGLES2View");
static TCHAR WindowTitle[] = _T("Xml GLES2 Viewer");

static TCHAR WndClassName[] = _T("PaintChldWindow");
static const int ExtBarSize = 200;

static HINSTANCE MainInstance = NULL;
static HWND MainWindow        = NULL;
static RECT MainWindowRect;
static HWND PaintWindow       = NULL;
static RECT PaintWindowRect;
static StaticExtBar ExtensionsBar;

static bool     IsCaptured = false;
static OdUInt32 CaptureFlags = 0;
static POINT    LastPt;

static bool      RegistratorInitialized = false;
static AppClient DefAppClient;
static AppClient *CurAppClient = &DefAppClient;

static void AppClient_attachAppClient(AppHandle /*appHandle*/, AppClient *appClient)
{
  if (appClient)
    CurAppClient = appClient;
  else
    CurAppClient = &DefAppClient;
}
static void AppClient_detachAppClient(AppHandle appHandle)
{
  AppClient_attachAppClient(appHandle, NULL);
}

inline AppHandle makeAppHandle(HWND hwnd)
{
  return (AppHandle)hwnd;
}
inline HWND makeHWND(AppHandle appHandle)
{
  return (HWND)appHandle;
}

class AppClientRegistrar
{
  protected:
    AppClient *m_pClient[10];
    OdUInt32 m_nClients;
    OdUInt32 m_nActive;
    OdUInt32 m_nPrevClients;

    void pushBack(AppClient *pLast)
    {
      m_pClient[m_nClients++] = pLast;
    }
    void pushFirst(AppClient *pFirst)
    {
      if (!m_nClients)
        pushBack(pFirst);
      else
      {
        for (OdUInt32 nCopy = m_nClients; nCopy > 0; nCopy--)
          m_pClient[nCopy] = m_pClient[nCopy - 1];
        m_pClient[0] = pFirst;
        m_nClients++;
      }
    }
    void pushMid(AppClient *pMid)
    {
      if (!m_nClients)
        pushBack(pMid);
      else
      {
        OdUInt32 nInc = 0;
        for (OdUInt32 nReg = 0; nReg < m_nClients; nReg++)
        {
          if (m_pClient[nReg]->isDefaultAppClient())
            nInc++;
          else
            break;
        }
        if (nInc == m_nClients)
          pushBack(pMid);
        else
        {
          for (OdUInt32 nCopy = m_nClients; nCopy > nInc; nCopy--)
            m_pClient[nCopy] = m_pClient[nCopy - 1];
          m_pClient[nInc] = pMid;
          m_nClients++;
        }
      }
    }
    void Init()
    {
      if (!RegistratorInitialized)
      {
        m_nClients = 0; m_nActive = 0; m_nPrevClients = 1;
        RegistratorInitialized = true;
      }
    }
  public:
    AppClientRegistrar() { Init(); }
    void registerClient(AppClient *pClient)
    {
      Init();
      if (!pClient->isDefaultAppClient())
      {
        if (pClient->isUtilitaryAppClient())
          pushBack(pClient);
        else
          pushMid(pClient);
      }
      else
        pushFirst(pClient);
    }
    void deregisterClient(AppClient *pClient)
    {
      for (OdUInt32 nClient = 0; nClient < m_nClients; nClient++)
      {
        if (m_pClient[nClient] == pClient)
        {
          if (nClient < (m_nClients - 1))
          {
            for (OdUInt32 nCopy = nClient; nCopy < m_nClients - 1; nCopy++)
              m_pClient[nCopy] = m_pClient[nCopy + 1];
          }
          m_nClients--;
          if (m_nActive >= m_nClients)
            m_nActive--;
          break;
        }
      }
    }

    void rebuildMenu(HWND hWnd)
    {
      HMENU hMenu = ::GetMenu(hWnd);
      for (OdUInt32 nMenu = IDC_MENU_APPCLIENT0; nMenu < IDC_MENU_APPCLIENT0 + m_nPrevClients; nMenu++)
        ::DeleteMenu(hMenu, nMenu, MF_BYCOMMAND);
      hMenu = ::GetSubMenu(hMenu, 1);
      for (OdUInt32 nClient = 0; nClient < m_nClients; nClient++)
        ::AppendMenuA(hMenu, MF_STRING, IDC_MENU_APPCLIENT0 + nClient, m_pClient[nClient]->appClientName());
      m_nPrevClients = m_nClients;
      if (m_nClients)
      {
        if (m_pClient[0]->isDefaultAppClient())
          ::SetMenuDefaultItem(hMenu, IDC_MENU_APPCLIENT0, FALSE);
        ::CheckMenuRadioItem(hMenu, IDC_MENU_APPCLIENT0, IDC_MENU_APPCLIENT0 + m_nClients, IDC_MENU_APPCLIENT0 + m_nActive, MF_BYCOMMAND);
      }
    }

    bool activateClient(HWND hWnd, UINT nClient);
    void updateClientMenu();

    void setDefaultClient()
    {
      m_nActive = 0;
      if (m_nClients)
      {
        AppClient_attachAppClient(AppClient().getCoreAppHandle(), m_pClient[0]);
        updateClientMenu();
      }
    }
} m_appClientReg;

// AppClient implementations

void AppClient::registerAppClient(AppClient *appClient)
{
  m_appClientReg.registerClient(appClient);
}
void AppClient::unregisterAppClient(AppClient *appClient)
{
  m_appClientReg.deregisterClient(appClient);
}

AppHandle AppClient::getCoreAppHandle()
{
  return ::makeAppHandle(MainWindow);
}
ExtBar &AppClient::getExtBar(AppHandle /*appHandle*/)
{
  return *ExtensionsBar.extBar();
}
void AppClient::updateExtBar(AppHandle /*appHandle*/)
{
  ExtensionsBar.update();
}
void AppClient::getSize(AppHandle /*appHandle*/, OdGsDCRect &dcRect)
{
  dcRect = OdGsDCRect(PaintWindowRect.left, PaintWindowRect.right, PaintWindowRect.bottom, PaintWindowRect.top);
}
bool AppClient::isCaptured(AppHandle /*appHandle*/, OdUInt32 *captureFlags)
{
  if (captureFlags)
    *captureFlags = CaptureFlags;
  return IsCaptured;
}
OdUInt32 AppClient::getKeyStates(AppHandle /*appHandle*/)
{
  OdUInt32 res = 0;
  if (!!(::GetAsyncKeyState(VK_CONTROL) & 0x8000)) res |= kCtrlKey;
  if (!!(::GetAsyncKeyState(VK_SHIFT)   & 0x8000)) res |= kShiftKey;
  if (!!(::GetAsyncKeyState(VK_MENU)    & 0x8000)) res |= kAltKey;
  return res;
}
void AppClient::getLastPoint(AppHandle /*appHandle*/, int &x, int &y)
{
  x = LastPt.x;
  y = LastPt.y;
}

void AppClient::capture(AppHandle appHandle, OdUInt32 captureFlags)
{
  if (!IsCaptured)
  {
    ::SetCapture(::makeHWND(appHandle));
    IsCaptured = true;
    CaptureFlags = captureFlags;
  }
}
void AppClient::releaseCapture(AppHandle /*appHandle*/)
{
  if (IsCaptured)
  {
    ::ReleaseCapture();
    IsCaptured = false;
  }
}

void AppClient::redraw(AppHandle appHandle)
{
  ::SendMessage(::makeHWND(appHandle), WM_PAINT, 0, 0);
}
void AppClient::close(AppHandle /*appHandle*/)
{
  ::PostMessage(MainWindow, WM_CLOSE, 0, 0);
}

void AppClient::displayError(AppHandle /*appHandle*/, const char *pText)
{
  ::MessageBoxA(MainWindow, pText, "GLES2 error", MB_ICONSTOP);
}
void AppClient::displayWarning(AppHandle /*appHandle*/, const char *pText)
{
  ::MessageBoxA(MainWindow, pText, "GLES2 warning", MB_ICONWARNING);
}

// Multiple files selection

struct MultiFileSel
{
  typedef OdArray<OdString> StringArray;
  typedef OdArray<int, OdMemoryAllocator<int> > SelRange;

  OdString m_directory;
  StringArray m_files;

  static MultiFileSel *getThis(HWND hDlg)
  {
    return (MultiFileSel*)(OdIntPtr)GetWindowLongPtr(hDlg, DWLP_USER);
  }

  static void resetSelection(HWND hDlg, BOOL bState = FALSE)
  {
    MultiFileSel *pThis = getThis(hDlg);
    ListBox_SelItemRange(::GetDlgItem(hDlg, IDC_LBFILES), bState, 0, pThis->m_files.size() - 1);
  }
  static int getSelection(HWND hDlg, SelRange *selItems)
  {
    const int numItems = ListBox_GetSelCount(::GetDlgItem(hDlg, IDC_LBFILES));
    if (numItems && selItems)
    {
      selItems->resize(numItems);
      ListBox_GetSelItems(::GetDlgItem(hDlg, IDC_LBFILES), numItems, selItems->asArrayPtr());
    }
    return numItems;
  }

  static void selRangeToStrings(HWND hDlg, SelRange &selItems, StringArray &out)
  {
    MultiFileSel *pThis = getThis(hDlg);
    const int *pSel = selItems.getPtr();
    for (OdUInt32 nSel = 0; nSel < selItems.size(); nSel++)
      out.push_back(pThis->m_files[pSel[nSel]]);
  }
  static void stringsToSelRange(HWND hDlg, StringArray &strings, SelRange &out)
  {
    MultiFileSel *pThis = getThis(hDlg);
    const OdString *pStr = strings.getPtr();
    for (OdUInt32 nStr = 0; nStr < strings.size(); nStr++)
    {
      StringArray::size_type findId = 0;
      if (pThis->m_files.find(pStr[nStr], findId))
        out.push_back(int(findId));
    }
  }

  static void updateSelection(HWND hDlg)
  {
    MultiFileSel *pThis = getThis(hDlg);
    if (pThis->m_files.isEmpty())
    {
      ::EnableWindow(::GetDlgItem(hDlg, IDOK), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BSORT), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BSELALL), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BUNSELALL), FALSE);
    }
    else
    {
      ::EnableWindow(::GetDlgItem(hDlg, IDOK), TRUE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BSORT), TRUE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BSELALL), TRUE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BUNSELALL), TRUE);
    }
    SelRange selItems;
    if (getSelection(hDlg, &selItems))
    {
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BDELETE), TRUE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVETOP), (!selItems.contains(0)) ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVEUP), (!selItems.contains(0)) ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVELOWER), (!selItems.contains(pThis->m_files.size() - 1)) ? TRUE : FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVEBOTTOM), (!selItems.contains(pThis->m_files.size() - 1)) ? TRUE : FALSE);
    }
    else
    {
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BDELETE), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVETOP), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVEUP), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVELOWER), FALSE);
      ::EnableWindow(::GetDlgItem(hDlg, IDC_BMOVEBOTTOM), FALSE);
    }
  }

  static INT_PTR CALLBACK dialogClientProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch (uMsg)
    {
      case WM_INITDIALOG:
        {
           ::SetWindowLongPtr(hDlg, DWLP_USER, lParam);
           updateSelection(hDlg);
        }
      break;
      case WM_DESTROY:
        {
        }
	    break;
      case WM_COMMAND:
        {
          switch (LOWORD(wParam))
          {
            case IDC_BADD:
              {
                MultiFileSel *pThis = getThis(hDlg);
                TCHAR gfs[_MAX_PATH * 16];
                ZeroMemory(&gfs, sizeof(gfs));
                { OPENFILENAME ofnTemp;
                  ZeroMemory(&ofnTemp, sizeof(OPENFILENAME));
                  ofnTemp.lStructSize = sizeof(OPENFILENAME);
                  ofnTemp.hwndOwner = hDlg;
                  ofnTemp.nFilterIndex = 1;
                  ofnTemp.lpstrFile = gfs;
                  ofnTemp.nMaxFile = sizeof(gfs) / sizeof(TCHAR);
                  OdString defExt;
                  ofnTemp.lpstrFilter = CurAppClient->supportFormats(::makeAppHandle(PaintWindow), defExt);
                  ofnTemp.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_NONETWORKBUTTON | OFN_EXPLORER;
                  ofnTemp.lpstrDefExt = defExt.c_str();
                  if (GetOpenFileName(&ofnTemp) == FALSE)
                    break;
                }
                TCHAR *pParse = gfs;
                OdString dir(pParse);
                while (*pParse) pParse++; pParse++;
                if (!*pParse)
                {
                  int last1 = dir.reverseFind('/'), last2 = dir.reverseFind('\\');
                  int last = odmax(last1, last2) + 1;
                  pParse -= (dir.getLength() - last) + 1;
                  dir = dir.left(last);
                }
                if ((dir.right(1) != _T("/")) && (dir.right(1) != _T("\\")))
                  dir += '\\';
                if (!pThis->m_directory.isEmpty() && (dir != pThis->m_directory))
                {
                  ::MessageBox(hDlg, _T("Can't select files from multiple directories."), _T("Selection Canceled"), MB_OK | MB_ICONWARNING);
                  break;
                }
                pThis->m_directory = dir;
                ::SetDlgItemText(hDlg, IDC_SDIRECTORY, dir);
                resetSelection(hDlg);
                while (*pParse)
                {
                  OdString fname(pParse);
                  if (!pThis->m_files.contains(fname))
                  {
                    pThis->m_files.push_back(fname);
                    ListBox_AddString(::GetDlgItem(hDlg, IDC_LBFILES), fname.c_str());
                    ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, pThis->m_files.size() - 1);
                  }
                  while (*pParse) pParse++; pParse++;
                }
                updateSelection(hDlg);
              }
            break;
            case IDC_BDELETE:
              {
                SelRange selItems;
                const int numItems = getSelection(hDlg, &selItems);
                if (numItems)
                {
                  MultiFileSel *pThis = getThis(hDlg);
                  while (!selItems.isEmpty())
                  {
                    pThis->m_files.removeAt(selItems.last());
                    ListBox_DeleteString(::GetDlgItem(hDlg, IDC_LBFILES), selItems.last());
                    selItems.removeLast();
                  }
                  if (pThis->m_files.isEmpty())
                  {
                    pThis->m_directory.empty();
                    ::SetDlgItemText(hDlg, IDC_SDIRECTORY, _T(""));
                  }
                  updateSelection(hDlg);
                }
              }
            break;
            case IDC_BSELALL:
              {
                resetSelection(hDlg, TRUE);
                updateSelection(hDlg);
              }
            break;
            case IDC_BUNSELALL:
              {
                resetSelection(hDlg, FALSE);
                updateSelection(hDlg);
              }
            break;
            case IDC_BMOVETOP:
              {
                SelRange selItems;
                if (getSelection(hDlg, &selItems))
                {
                  MultiFileSel *pThis = getThis(hDlg);
                  const int *pSel = selItems.getPtr();
                  for (OdUInt32 nSel = 0; nSel < selItems.size(); nSel++)
                  {
                    OdString moveStr = pThis->m_files.at(pSel[nSel]);
                    pThis->m_files.removeAt(pSel[nSel]);
                    pThis->m_files.insertAt(nSel, moveStr);
                    ListBox_DeleteString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel]);
                    ListBox_InsertString(::GetDlgItem(hDlg, IDC_LBFILES), nSel, moveStr.c_str());
                    ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, nSel);
                  }
                  updateSelection(hDlg);
                }
              }
            break;
            case IDC_BMOVEUP:
              {
                SelRange selItems;
                if (getSelection(hDlg, &selItems))
                {
                  MultiFileSel *pThis = getThis(hDlg);
                  const int *pSel = selItems.getPtr();
                  for (OdUInt32 nSel = 0; nSel < selItems.size(); nSel++)
                  {
                    OdString moveStr = pThis->m_files.at(pSel[nSel]);
                    pThis->m_files.removeAt(pSel[nSel]);
                    pThis->m_files.insertAt(pSel[nSel] - 1, moveStr);
                    ListBox_DeleteString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel]);
                    ListBox_InsertString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel] - 1, moveStr.c_str());
                    ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, pSel[nSel] - 1);
                  }
                  updateSelection(hDlg);
                }
              }
            break;
            case IDC_BMOVELOWER:
              {
                SelRange selItems;
                if (getSelection(hDlg, &selItems))
                {
                  MultiFileSel *pThis = getThis(hDlg);
                  const int *pSel = selItems.getPtr();
                  for (OdInt32 nSel = (OdInt32)selItems.size() - 1; nSel >= 0; nSel--)
                  {
                    OdString moveStr = pThis->m_files.at(pSel[nSel]);
                    pThis->m_files.removeAt(pSel[nSel]);
                    pThis->m_files.insertAt(pSel[nSel] + 1, moveStr);
                    ListBox_DeleteString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel]);
                    ListBox_InsertString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel] + 1, moveStr.c_str());
                    ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, pSel[nSel] + 1);
                  }
                  updateSelection(hDlg);
                }
              }
            break;
            case IDC_BMOVEBOTTOM:
              {
                SelRange selItems;
                if (getSelection(hDlg, &selItems))
                {
                  MultiFileSel *pThis = getThis(hDlg);
                  const int *pSel = selItems.getPtr();
                  OdUInt32 nLast = pThis->m_files.size() - 1;
                  for (OdInt32 nSel = (OdInt32)selItems.size() - 1; nSel >= 0; nSel--, nLast--)
                  {
                    OdString moveStr = pThis->m_files.at(pSel[nSel]);
                    pThis->m_files.removeAt(pSel[nSel]);
                    pThis->m_files.insertAt(nLast, moveStr);
                    ListBox_DeleteString(::GetDlgItem(hDlg, IDC_LBFILES), pSel[nSel]);
                    ListBox_InsertString(::GetDlgItem(hDlg, IDC_LBFILES), nLast, moveStr.c_str());
                    ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, nLast);
                  }
                  updateSelection(hDlg);
                }
              }
            break;
            case IDC_BSORT:
              {
                MultiFileSel *pThis = getThis(hDlg);
                if (pThis->m_files.size() > 1)
                {
                  SelRange selItems; StringArray selStrs;
                  getSelection(hDlg, &selItems);
                  selRangeToStrings(hDlg, selItems, selStrs); selItems.clear();
                  std::sort(pThis->m_files.begin(), pThis->m_files.end());
                  stringsToSelRange(hDlg, selStrs, selItems); selStrs.clear();
                  ListBox_ResetContent(::GetDlgItem(hDlg, IDC_LBFILES));
                  for (OdUInt32 nStr = 0; nStr < pThis->m_files.size(); nStr++)
                  {
                    ListBox_AddString(::GetDlgItem(hDlg, IDC_LBFILES), pThis->m_files.at(nStr).c_str());
                    if (selItems.contains(int(nStr)))
                      ListBox_SetSel(::GetDlgItem(hDlg, IDC_LBFILES), TRUE, nStr);
                  }
                }
              }
            break;
            case IDOK:
            case IDCANCEL:
              if (LOWORD(wParam) != IDOK)
              {
                MultiFileSel *pThis = getThis(hDlg);
                pThis->m_directory.empty();
                pThis->m_files.clear();
              }
              ::EndDialog(hDlg, LOWORD(wParam));
            break;
            case IDC_LBFILES:
              if (HIWORD(wParam) == LBN_SELCHANGE)
                updateSelection(hDlg);
            break;
          }
        }
      break;
      default:
      return FALSE;
    }
    return TRUE;
  }

  bool run()
  {
    ::DialogBoxParam(MainInstance, MAKEINTRESOURCE(IDD_OPENFILES), MainWindow, dialogClientProc, (LPARAM)this);
    return !m_files.empty();
  }
};

// Window handlers implementation

struct ViewerWnd : public ExtBar::Callback
{
  static HINSTANCE instance() { return MainInstance; }
  static HWND mainWindow()    { return MainWindow; }
  static HWND paintWindow()   { return PaintWindow; }

  static bool onCreate(HWND hwnd, CREATESTRUCT far* /*lpCreateStruct*/)
  {
    MainWindow = hwnd;
    m_appClientReg.setDefaultClient();
    m_appClientReg.rebuildMenu(hwnd);
    ExtBar::WinSysHandle sysHandle = { hwnd, instance() };
    ExtensionsBar.create(&sysHandle);
    createPaintWindow(true);
    return true;
  }
  static bool onChildCreate(HWND hwnd, CREATESTRUCT far* /*lpCreateStruct*/)
  {
    CurAppClient->onCreate(::makeAppHandle(hwnd));
    return true;
  }
  static void onDestroy(HWND /*hwnd*/)
  {
    destroyPaintWindow();
    ExtensionsBar.destroy();
    ::PostQuitMessage(0);
  }
  static void onChildDestroy(HWND hwnd)
  {
    CurAppClient->onDestroy(::makeAppHandle(hwnd));
  }
  static void onClose(HWND hwnd)
  {
    if (CurAppClient->canClose(::makeAppHandle(paintWindow())))
      ::DestroyWindow(hwnd);
  }

  static void onCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
  {
    switch (id)
    {
      case IDC_MENU_OPENFILE:
        {
          TCHAR fileName[MAX_PATH];
          ZeroMemory(&fileName, sizeof(fileName));
          OPENFILENAME ofn;
          ZeroMemory(&ofn, sizeof(OPENFILENAME));
          ofn.lStructSize = sizeof(OPENFILENAME);
          ofn.hwndOwner = mainWindow();
          OdString defExt;
          ofn.lpstrFilter = CurAppClient->supportFormats(::makeAppHandle(paintWindow()), defExt);
          ofn.nFilterIndex = 1;
          ofn.lpstrFile = fileName;
          ofn.nMaxFile = _MAX_PATH;
          ofn.lpstrInitialDir = NULL;
          ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
          ofn.lpstrDefExt = defExt.c_str();

          if (GetOpenFileName(&ofn) == FALSE)
            break;
          CurAppClient->onOpen(::makeAppHandle(paintWindow()), OdString(fileName));
          DefAppClient.redraw(::makeAppHandle(paintWindow()));
        }
      break;
      case IDC_MENU_OPENMULTFILES:
        {
          MultiFileSel msel;
          if (msel.run())
          {
            int rv = ::MessageBox(mainWindow(), _T("Reset currently accumulated contents?"), _T("Open Files"), MB_YESNOCANCEL | MB_ICONQUESTION);
            if (rv != IDCANCEL)
            {
              CurAppClient->onMultiOpen(::makeAppHandle(paintWindow()), msel.m_directory, msel.m_files.size(), msel.m_files.getPtr(), rv == IDYES);
              DefAppClient.redraw(::makeAppHandle(paintWindow()));
            }
          }
        }
      break;
      case IDC_MENU_QUIT:
        DefAppClient.close(::makeAppHandle(paintWindow()));
      break;
      case IDC_MENU_APPCLIENT0: case IDC_MENU_APPCLIENT1: case IDC_MENU_APPCLIENT2: case IDC_MENU_APPCLIENT3: case IDC_MENU_APPCLIENT4:
      case IDC_MENU_APPCLIENT5: case IDC_MENU_APPCLIENT6: case IDC_MENU_APPCLIENT7: case IDC_MENU_APPCLIENT8: case IDC_MENU_APPCLIENT9:
        if (m_appClientReg.activateClient(hwnd, (UINT)id))
          DefAppClient.redraw(::makeAppHandle(paintWindow()));
      break;
    }
  }

  static void onPaint(HWND hwnd)
  {
    PAINTSTRUCT PaintStruct;
    HDC dc = ::BeginPaint(hwnd, &PaintStruct);
    CurAppClient->onDraw(::makeAppHandle(hwnd));
    ::EndPaint(hwnd, &PaintStruct);
  }

  static void onSize(HWND hwnd, UINT /*mes*/, int w, int h)
  {
    ::SetRect(&MainWindowRect, 0, 0, w, h);
    ::SetRect(&PaintWindowRect, 0, 0, w - ExtBarSize, h);
    ::MoveWindow(paintWindow(), 0, 0, w - ExtBarSize, h, TRUE);
    RECT extBarRect; ::SetRect(&extBarRect, w - ExtBarSize, 0, w, h);
    ExtensionsBar.resize(extBarRect);
    CurAppClient->onSize(::makeAppHandle(paintWindow()), OdGsDCRect(PaintWindowRect.left, PaintWindowRect.right, PaintWindowRect.bottom, PaintWindowRect.top));
  }

  static void onLButtonDown(HWND /*hwnd*/, bool dbl, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onLMouseButtonDown(::makeAppHandle(paintWindow()), x, y, dbl);
  }
  static void onRButtonDown(HWND /*hwnd*/, bool dbl, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onRMouseButtonDown(::makeAppHandle(paintWindow()), x, y, dbl);
  }
  static void onMButtonDown(HWND /*hwnd*/, bool dbl, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onMMouseButtonDown(::makeAppHandle(paintWindow()), x, y, dbl);
  }

  static void onLButtonUp(HWND /*hwnd*/, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onLMouseButtonUp(::makeAppHandle(paintWindow()), x, y);
  }
  static void onRButtonUp(HWND /*hwnd*/, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onRMouseButtonUp(::makeAppHandle(paintWindow()), x, y);
  }
  static void onMButtonUp(HWND /*hwnd*/, int x, int y, UINT /*codeNotify*/)
  {
    CurAppClient->onMMouseButtonUp(::makeAppHandle(paintWindow()), x, y);
  }

  static void onMouseMove(HWND /*hwnd*/, int x, int y, UINT /*codeNotify*/)
  {
    if (IsCaptured)
      CurAppClient->onMouseMove(::makeAppHandle(paintWindow()), x - LastPt.x, y - LastPt.y);
    else
      CurAppClient->onNcdMouseMove(::makeAppHandle(paintWindow()), x - LastPt.x, y - LastPt.y);
    LastPt.x = x;
    LastPt.y = y;
  }

  static void onMouseWheel(HWND hwnd, int x, int y, int delta, UINT /*codeNotify*/)
  {
    POINT pt = { x, y };
    ScreenToClient(hwnd, &pt);
    CurAppClient->onMouseWheel(::makeAppHandle(paintWindow()), float(delta) / WHEEL_DELTA, pt.x, pt.y);
  }

  static void onKey(HWND /*hwnd*/, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
    CurAppClient->onKey(::makeAppHandle(paintWindow()), vk, fDown!=0, cRepeat, flags);
  }

  virtual void elementChanged(ExtBar::Element *pElem)
  {
    CurAppClient->onExtBar(::makeAppHandle(paintWindow()), pElem);
  }

  static LRESULT CALLBACK wndProc(HWND hWindow, UINT Message,
                                  WPARAM wParam, LPARAM lParam)
  {
    switch (Message)
    {
      HANDLE_MSG(hWindow, WM_CREATE       , onCreate);
      HANDLE_MSG(hWindow, WM_DESTROY      , onDestroy);
      HANDLE_MSG(hWindow, WM_CLOSE        , onClose);
      HANDLE_MSG(hWindow, WM_COMMAND      , onCommand);
      HANDLE_MSG(hWindow, WM_SIZE         , onSize);
      HANDLE_MSG(hWindow, WM_MOUSEWHEEL   , onMouseWheel);
      HANDLE_MSG(hWindow, WM_KEYDOWN      , onKey);
      HANDLE_MSG(hWindow, WM_KEYUP        , onKey);
      default: return ::DefWindowProc(hWindow, Message, wParam, lParam);
    }
  }

  static LRESULT CALLBACK childWndProc(HWND hWindow, UINT Message,
                                       WPARAM wParam, LPARAM lParam)
  {
    switch (Message)
    {
      HANDLE_MSG(hWindow, WM_CREATE       , onChildCreate);
      HANDLE_MSG(hWindow, WM_DESTROY      , onChildDestroy);
      HANDLE_MSG(hWindow, WM_PAINT        , onPaint);
      HANDLE_MSG(hWindow, WM_LBUTTONDOWN  , onLButtonDown);
      HANDLE_MSG(hWindow, WM_LBUTTONDBLCLK, onLButtonDown);
      HANDLE_MSG(hWindow, WM_LBUTTONUP    , onLButtonUp);
      HANDLE_MSG(hWindow, WM_RBUTTONDOWN  , onRButtonDown);
      HANDLE_MSG(hWindow, WM_RBUTTONDBLCLK, onRButtonDown);
      HANDLE_MSG(hWindow, WM_RBUTTONUP    , onRButtonUp);
      HANDLE_MSG(hWindow, WM_MBUTTONDOWN  , onMButtonDown);
      HANDLE_MSG(hWindow, WM_MBUTTONDBLCLK, onMButtonDown);
      HANDLE_MSG(hWindow, WM_MBUTTONUP    , onMButtonUp);
      HANDLE_MSG(hWindow, WM_MOUSEMOVE    , onMouseMove);
      HANDLE_MSG(hWindow, WM_MOUSEWHEEL   , onMouseWheel);
      HANDLE_MSG(hWindow, WM_KEYDOWN      , onKey);
      HANDLE_MSG(hWindow, WM_KEYUP        , onKey);
      default: return ::DefWindowProc(hWindow, Message, wParam, lParam);
    }
  }

  static void createPaintWindow(bool bFirstInit = false)
  {
    if (bFirstInit)
    {
      WNDCLASS WndClass;
      WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
      WndClass.lpfnWndProc = childWndProc;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      WndClass.hInstance = instance();
      WndClass.hIcon = NULL;
      WndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
      WndClass.hbrBackground = GetStockBrush(NULL_BRUSH);
      WndClass.lpszMenuName = NULL;
      WndClass.lpszClassName = WndClassName;
      ::RegisterClass(&WndClass);
    }
    PaintWindow = ::CreateWindow(WndClassName, WndClassName, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                                 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                 MainWindow, NULL, instance(), NULL);
  }
  static void destroyPaintWindow()
  {
    ::DestroyWindow(PaintWindow);
  }

  static bool registerWndClass(HINSTANCE hInst)
  {
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    WndClass.lpfnWndProc = wndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInst;
    WndClass.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONAPP));
    WndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = GetStockBrush(NULL_BRUSH);
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENUAPP);
    WndClass.lpszClassName = AppName;
    return (::RegisterClass(&WndClass)) ? true : false;
  }

  static HWND createWnd(HINSTANCE hInstance, int nCmdShow)
  {
    HWND hWindow = ::CreateWindow(AppName, WindowTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                  NULL, NULL, hInstance, NULL);
    if (hWindow == NULL)
      return hWindow;
    ::ShowWindow(hWindow, nCmdShow);
    ::UpdateWindow(hWindow);
    return hWindow;
  }
} ViewerWndCbk;

bool AppClientRegistrar::activateClient(HWND hWnd, UINT nClient)
{
  nClient -= IDC_MENU_APPCLIENT0;
  if (nClient != m_nActive)
  {
    AppClient *pPrev = m_pClient[m_nActive], *pNew = m_pClient[nClient];
    // Switch client
    if (!pPrev->canClose(::makeAppHandle(ViewerWnd::paintWindow())))
      return false;
    AppHandle coreHandle = AppClient().getCoreAppHandle();
    ViewerWnd::destroyPaintWindow();
    AppClient_detachAppClient(coreHandle);
    AppClient_attachAppClient(coreHandle, pNew);
    ExtensionsBar.clear();
    ViewerWnd::createPaintWindow();
    ::MoveWindow(ViewerWnd::paintWindow(), PaintWindowRect.left, PaintWindowRect.top,
                 PaintWindowRect.right - PaintWindowRect.left, PaintWindowRect.bottom - PaintWindowRect.top, TRUE);
    // Switch menu
    m_nActive = nClient;
    ::CheckMenuRadioItem(::GetMenu(hWnd), IDC_MENU_APPCLIENT0, IDC_MENU_APPCLIENT0 + m_nPrevClients, IDC_MENU_APPCLIENT0 + m_nActive, MF_BYCOMMAND);
    // Update menu
    updateClientMenu();
    return true;
  }
  return false;
}

void AppClientRegistrar::updateClientMenu()
{
  ::EnableMenuItem(::GetMenu(ViewerWnd::mainWindow()), IDC_MENU_OPENMULTFILES, MF_BYCOMMAND |
    ((CurAppClient->isSupportMultiOpen(::makeAppHandle(ViewerWnd::paintWindow()))) ? MF_ENABLED : MF_GRAYED));
}

int PASCAL _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR /*lpszCmdLine*/, int nCmdShow) {
  MSG Msg;
  MainInstance = hInstance;
  if (!hPrevInstance) {
    if (!ViewerWnd::registerWndClass(hInstance))
      return 0;
  }
  MainWindow = ViewerWnd::createWnd(hInstance, nCmdShow);
  if (!MainWindow)
    return 0;
  ExtensionsBar.resetCallback(&ViewerWndCbk);
  while (::GetMessage(&Msg, NULL, 0, 0))
  {
    if (!::IsDialogMessage(MainWindow, &Msg))
    {
      ::TranslateMessage(&Msg);
      ::DispatchMessage(&Msg);
    }
  }
  return (int)Msg.wParam;
}

//
