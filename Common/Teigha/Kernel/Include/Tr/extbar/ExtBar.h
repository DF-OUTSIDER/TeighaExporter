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

#ifndef OD_TR_EXTBAR_H_
#define OD_TR_EXTBAR_H_

#include "TD_PackPush.h"

#include "OdString.h"
#include <limits.h>
#include <float.h>

class ElementImplIface;

class ExtBar
{
  public:
    // System definitions
#ifdef WIN32
    struct WinSysHandle
    {
      HWND hOwner;
      HINSTANCE hInstance;
    };
    typedef WinSysHandle *SysHandle;
    typedef RECT Rect;
    typedef SIZE Size;
    typedef POINT Point;
#else
    typedef void *SysHandle;
    struct Rect
    {
      long left;
      long top;
      long right;
      long bottom;
    };
    struct Size
    {
      long cx;
      long cy;
    };
    struct Point
    {
      long x;
      long y;
    };
#endif
    // Element types
    enum ElementType
    {
      kGroup = 0,
      kText,        // interfaces: [kText | kEditBox] | kColorButton
      kButton,      // interfaces: [kText | kEditBox]
      kCheckButton, // interfaces: [kText | kEditBox] | [kCheckButton | kCheckBox | kRadioButton]
      kColorButton, // interfaces: [kText | kEditBox] | kColorButton
      kCheckBox,    // interfaces: [kText | kEditBox] | [kCheckButton | kCheckBox | kRadioButton]
      kRadioGroup,  // interfaces: kRadioGroup
      kRadioButton, // interfaces: [kText | kEditBox] | [kCheckButton | kCheckBox | kRadioButton] | kRadioGroup
      kPlaceholder, // interfaces: NULL
      kComboBox,    // interfaces: [kComboBox | kListBox]
      kListBox,     // interfaces: [kComboBox | kListBox]
      kEditBox,     // interfaces: [kText | kEditBox]
      kRangeBox     // interfaces: [kText | kEditBox] | kRangeBox
    };
    // Element sizes
    enum ElementSize
    {
      k8d8 = 8,
      k6d8 = 6,
      k4d8 = 4,
      k2d8 = 2,
      k1d8 = 1
    };
    // Element interfaces
    struct Element
    {
      virtual ~Element() {}
      virtual OdUInt32 elementId() const = 0;
      virtual ElementType elementType() const = 0;
      virtual Element *elementInterface(ElementType type) = 0;
      virtual void setDisabled(bool bDisable) = 0;
      virtual bool isDisabled() const = 0;
      virtual ElementImplIface *getImpl() const = 0;
      virtual void attachSomeData(void *pData) = 0;
      virtual void* getAttachedData() const = 0;
    };
    struct TextElement : public Element
    {
      virtual OdString getText() const = 0;
      virtual void setText(const OdString &text) = 0;
      static TextElement *cast(Element *pIface) { return static_cast<TextElement*>(pIface->elementInterface(kText)); }
    };
    struct GroupElement : public Element
    {
      virtual bool isLayoutGroup() const = 0;
      virtual bool isExpandeable() const = 0;
      virtual void setExpanded(bool bExpand) = 0;
      virtual bool isExpanded() const = 0;
      virtual OdUInt32 getNumElements() const = 0;
      virtual Element *getElement(OdUInt32 nElement) const = 0;
      static GroupElement *cast(Element *pIface) { return static_cast<GroupElement*>(pIface->elementInterface(kGroup)); }
    };
    struct CheckElement : public Element
    {
      virtual bool isChecked() const = 0;
      virtual void setChecked(bool bCheck) = 0;
      static CheckElement *cast(Element *pIface) { return static_cast<CheckElement*>(pIface->elementInterface(kCheckBox)); }
    };
    struct RadioElement : public Element
    {
      virtual OdUInt32 checkedElement() const = 0;
      virtual void setCheckedElement(OdUInt32 nElement) = 0;
      static RadioElement *cast(Element *pIface) { return static_cast<RadioElement*>(pIface->elementInterface(kRadioGroup)); }
    };
    struct ListElement : public Element
    {
      virtual void addString(const OdString &string) = 0;
      virtual void clear() = 0;
      virtual OdUInt32 getNumStrings() const = 0;
      virtual OdString getString(OdUInt32 nString) const = 0;
      virtual int getSelString() const = 0;
      virtual void setSelString(int nString) = 0;
      virtual void delString(OdUInt32 nString) = 0;
      virtual void insString(int nWhere, const OdString &string) = 0;
      static ListElement *cast(Element *pIface) { return static_cast<ListElement*>(pIface->elementInterface(kListBox)); }
    };
    struct ColorElement : public Element
    {
      virtual ODCOLORREF getColor() const = 0;
      virtual void setColor(ODCOLORREF color) = 0;
      static ColorElement *cast(Element *pIface) { return static_cast<ColorElement*>(pIface->elementInterface(kColorButton)); }
    };
    struct RangeElement : public Element
    {
      virtual OdInt32 getRangeMinInt() const = 0;
      virtual OdInt32 getRangeMaxInt() const = 0;
      virtual void setRangeInt(OdInt32 nMin, OdInt32 nMax) = 0;
      virtual OdInt32 getDeltaInt() const = 0;
      virtual void setDeltaInt(OdInt32 nDelta) = 0;
      virtual OdInt32 getPosInt() const = 0;
      virtual void setPosInt(OdInt32 nPos) = 0;
      virtual void setInt(OdInt32 nPos = 0, OdInt32 nMin = INT_MIN, OdInt32 nMax = INT_MAX, OdInt32 nDelta = 1) = 0;

      virtual OdUInt32 getRangeMinUInt() const = 0;
      virtual OdUInt32 getRangeMaxUInt() const = 0;
      virtual void setRangeUInt(OdUInt32 nMin, OdUInt32 nMax) = 0;
      virtual OdUInt32 getPosUInt() const = 0;
      virtual void setPosUInt(OdUInt32 nPos) = 0;
      virtual void setUInt(OdUInt32 nPos = 0, OdUInt32 nMin = 0, OdUInt32 nMax = UINT_MAX, OdInt32 nDelta = 1) = 0;

      virtual double getRangeMinFP() const = 0;
      virtual double getRangeMaxFP() const = 0;
      virtual void setRangeFP(double fMin, double fMax) = 0;
      virtual double getDeltaFP() const = 0;
      virtual void setDeltaFP(double fDelta) = 0;
      virtual double getPosFP() const = 0;
      virtual void setPosFP(double fPos) = 0;
      virtual void setFP(double fPos = 0.0, double fMin = DBL_MIN, double fMax = DBL_MAX, double fDelta = 0.01) = 0;

      virtual bool isFP() const = 0;
      virtual void setFP(bool bFP) = 0;
      virtual bool isSigned() const = 0;
      virtual void setSigned(bool bSigned) = 0;
      static RangeElement *cast(Element *pIface) { return static_cast<RangeElement*>(pIface->elementInterface(kRangeBox)); }
    };
    // Main callback function
    struct Callback
    {
      virtual void fillExtBar(ExtBar * /*pExtBar*/) {}
      virtual void closeExtBar(ExtBar * /*pExtBar*/, int /*nReason*/) {}
      virtual void elementChanged(Element *pElem) = 0;
    };
    // Inplace callback function
    struct InplaceCallback : public Callback
    {
      virtual bool canCloseInplace(ExtBar * /*pInplace*/) { return true; }
      virtual void inplaceClosed(ExtBar * /*pInplace*/) { }
    };
  public:
    ExtBar() {}
    ~ExtBar() {}

    virtual GroupElement *createGroup(OdUInt32 nElementId, bool bExpandeable = true, bool bExpanded = true) = 0;
    virtual TextElement *createText(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8, OdUInt32 nStrings = 1, bool bBorder = false, ODCOLORREF color = ODRGB(0, 0, 0)) = 0;
    virtual TextElement *createButton(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8) = 0;
    virtual TextElement *createCheckButton(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8) = 0;
    virtual ColorElement *createColorButton(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8) = 0;
    virtual TextElement *createCheckBox(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8, bool bChecked = false, bool bLText = false) = 0;
    virtual RadioElement *createRadioGroup(OdUInt32 nElementId, GroupElement *pGroup = NULL) = 0;
    virtual TextElement *createRadioButton(OdUInt32 nElementId, RadioElement *pGroup, ElementSize size = k8d8, bool bChecked = false) = 0;
    virtual void createPlaceholder(GroupElement *pGroup = NULL, ElementSize size = k8d8) = 0;
    virtual ListElement *createComboBox(OdUInt32 nElementId, GroupElement *pGroup = NULL, OdUInt32 nOpenLines = 4) = 0;
    virtual ListElement *createListBox(OdUInt32 nElementId, GroupElement *pGroup = NULL, OdUInt32 nStrings = 4) = 0;
    virtual TextElement *createEditBox(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8, OdUInt32 nStrings = 1, bool bReadOnly = false, bool bMonitorTyping = false) = 0;
    virtual RangeElement *createRangeBox(OdUInt32 nElementId, GroupElement *pGroup = NULL, ElementSize size = k8d8, bool bSigned = false, bool bFP = false) = 0;

    virtual Element *getElement(OdUInt32 nElementId) const = 0;
    virtual GroupElement *getLayoutGroup() const = 0;

    virtual bool getColor(ODCOLORREF &color) const = 0;
    virtual OdString getOpenFileName(const OdChar *pFilter, const OdChar *pDefExt) = 0;
    virtual OdString getSaveFileName(const OdChar *pFilter, const OdChar *pDefExt) = 0;
};

// This is host interfaces. Non-abstract. Shouldn't be called from client code.

class ExtBarManager
{
  public:
    static ExtBar *createExtBar(ExtBar::Callback *pCallback = NULL);
    static ExtBar *createExtBar(ExtBar::InplaceCallback *pCallback);
    static void destroyExtBar(ExtBar *pExtBar);
};

class ExtBarInterface
{
  protected:
    ExtBar *m_pExtBar;
  public:
    ExtBarInterface(ExtBar *pExtBar = NULL) : m_pExtBar(pExtBar) {}
    ~ExtBarInterface() { }

    void create(ExtBar::SysHandle sysHandle);
    void destroy(int nReason = 0);
    void resize(const ExtBar::Rect &rect);

    void update();
    void clear();

    void resetCallback(ExtBar::Callback *pCallback);
    ExtBar *extBar() { return m_pExtBar; }
};

class StaticExtBar : public ExtBarInterface, protected ExtBarManager
{
  public:
    StaticExtBar(ExtBar::Callback *pCallback = NULL) : ExtBarInterface(createExtBar(pCallback)) { }
    ~StaticExtBar() { destroyExtBar(m_pExtBar); }
};

class ExtBarInplace : protected ExtBarInterface, protected ExtBar::InplaceCallback
{
  protected:
    bool m_bDone;
  protected:
    virtual void inplaceClosed(ExtBar * /*pInplace*/) { close(); }
    void fakewndproc();
    void closeImpl();
  public:
    ExtBarInplace() : m_bDone(true) { }
    ~ExtBarInplace() { close(); }

    static ExtBar::Point curpos();
    static ExtBar::Size makesize(int w, int h);
    static ExtBar::Rect screenplace(const ExtBar::Point &pt, const ExtBar::Size &sz);

    void display(ExtBar::SysHandle sysHandle, const ExtBar::Rect &rect)
    {
      if (m_pExtBar)
        close();
      m_pExtBar = ExtBarManager::createExtBar(this);
      create(sysHandle);
      fillInplace(m_pExtBar);
      resize(rect);
      fakewndproc();
    }
    void close(int nReason = 0)
    {
      if (m_pExtBar)
      {
        closeImpl();
        destroy(nReason);
        ExtBarManager::destroyExtBar(m_pExtBar);
        m_pExtBar = NULL;
      }
    }
    bool isClosed() const { return !m_pExtBar; }

    virtual void fillInplace(ExtBar * /*pExtBar*/)
    {
      // Do your own initialization here
    }
};

#include "TD_PackPop.h"

#endif // OD_TR_EXTBAR_H_
