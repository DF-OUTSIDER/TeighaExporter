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
// Application client abtraction layer

#ifndef OD_XML_GLES2VIEW_APPCLIENT_H_
#define OD_XML_GLES2VIEW_APPCLIENT_H_

#include "TD_PackPush.h"

#include "Ge/GePoint2d.h"
#include "Gs/GsDefs.h"

#include "ExtBar.h"

typedef void* AppHandle;
/** <group ExRender_Classes>
*/
class AppClient
{
  public:
    enum KeyState
    {
      kShiftKey = 1,
      kCtrlKey  = 2,
      kAltKey   = 4
    };
    //enum CaptureFlags
    //{
      // @@@TODO:
    //};
  public:
    AppClient() { }
    virtual ~AppClient() { }

    static void registerAppClient(AppClient *appClient);
    static void unregisterAppClient(AppClient *appClient);

    // Access to core AppClient
    AppHandle getCoreAppHandle();
    ExtBar &getExtBar(AppHandle appHandle);
    void updateExtBar(AppHandle appHandle);
    void getSize(AppHandle appHandle, OdGsDCRect &dcRect);
    bool isCaptured(AppHandle appHandle, OdUInt32 *captureFlags = NULL);
    OdUInt32 getKeyStates(AppHandle appHandle);
    void getLastPoint(AppHandle appHandle, int &x, int &y);

    // Capturing ability
    void capture(AppHandle appHandle, OdUInt32 captureFlags = 0);
    void releaseCapture(AppHandle appHandle);

    // Force redraw
    void redraw(AppHandle appHandle);
    // Force App close
    void close(AppHandle appHandle);

    // Display errors
    void displayError(AppHandle appHandle, const char *pText);
    void displayWarning(AppHandle appHandle, const char *pText);

    // Client specifications
    virtual const char *appClientName() const { return "Unknown"; }
    virtual bool isDefaultAppClient() const { return false; }
    virtual bool isUtilitaryAppClient() const { return false; }

    // Overrideable callbacks
    virtual void onCreate(AppHandle /*appHandle*/) { }
    virtual void onDestroy(AppHandle /*appHandle*/) { }
    virtual bool canClose(AppHandle /*appHandle*/) { return true; }

    virtual const OdChar *supportFormats(AppHandle /*appHandle*/, OdString & /*defExt*/) { return NULL; }
    virtual void onOpen(AppHandle /*appHandle*/, const OdString & /*fileName*/) { }

    virtual bool isSupportMultiOpen(AppHandle /*appHandle*/) const { return false; }
    virtual void onMultiOpen(AppHandle /*appHandle*/, const OdString & /*directory*/, OdUInt32 /*nFiles*/, const OdString * /*pFiles*/, bool /*bResetContents*/) { }

    virtual void onDraw(AppHandle /*appHandle*/) { }
    virtual void onSize(AppHandle /*appHandle*/, const OdGsDCRect &/*size*/) { }

    virtual void onLMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool /*bDoubleClick*/) { }
    virtual void onLMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/) { }

    virtual void onRMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool /*bDoubleClick*/) { }
    virtual void onRMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/) { }

    virtual void onMMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool /*bDoubleClick*/) { }
    virtual void onMMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/) { }

    virtual void onMouseMove(AppHandle /*appHandle*/, int /*dx*/, int /*dy*/) { }
    virtual void onNcdMouseMove(AppHandle /*appHandle*/, int /*dx*/, int /*dy*/) { }

    virtual void onMouseWheel(AppHandle /*appHandle*/, float /*delta*/, int /*x*/, int /*y*/) { }
    virtual void onKey(AppHandle /*appHandle*/, OdUInt32 /*charCode*/, bool /*down*/, int /*cRepeat*/, OdUInt32 /*flags*/) { }

    virtual void onExtBar(AppHandle /*appHandle*/, ExtBar::Element * /*pElem*/) { }
};

#include "TD_PackPop.h"

#endif // OD_XML_GLES2VIEW_APPCLIENT_H_
