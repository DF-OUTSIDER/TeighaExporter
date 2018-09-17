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

#ifndef __COMMON_OPENGL_PROPERTIES__
#define __COMMON_OPENGL_PROPERTIES__

#include "Gs/GsDefs.h"
#include "CommonDeviceProps.h"

#define OD_OPENGL_DEFINE_COMMON_PROPS(className, TBase, TPropsBase) \
class className : public TBase, public TPropsBase \
{ \
  public: \
    ODRX_USING_HEAP_OPERATORS(TBase); \
\
    ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(className); \
\
    className() : TBase(), TPropsBase() \
    { \
      if (GETBIT(m_flags, kDynamicSubhighlight)) \
        setDynamicSubEntHltEnabled(true); \
    } \
\
    OdRxDictionaryPtr properties() { return this; } \
    OD_COMMONDEVPROP_BlocksCacheEnabled_DevRedirect(); \
    OD_COMMONDEVPROP_DynamicSubEntHltEnabled_DevRedirect(); \
\
    void setRegenCoef(double /*val*/) { } \
\
    OdUInt32 getLineweightOptimizationProp() const \
    { \
      return ((isPointLineweightOptimizationEnabled()) ? 1 : 0) | ((isLineLineweightOptimizationEnabled()) ? 2 : 0); \
    } \
    void setLineweightOptimizationProp(OdUInt32 nVal) \
    { \
      enablePointLineweightOptimization(GETBIT(nVal, 1)); \
      enableLineLineweightOptimization (GETBIT(nVal, 2)); \
    } \
\
    bool supportPartialUpdate() const \
    { \
        return isClearScreenEnabled(); \
    } \
\
    void update(OdGsDCRect* pUpdatedRect) \
    { \
      if (!supportPartialUpdate()) \
      { \
        clear(); \
      } \
\
      TBase::update(pUpdatedRect); \
    } \
}

#define OD_OPENGL_DECLARE_COMMON_PROPS(className, TBase, TPropsBase) \
\
ODRX_DECLARE_PROPERTY2(DiscardBackFaces, className) \
ODRX_DECLARE_PROPERTY2(RegenCoef, className) \
ODRX_DECLARE_PROPERTY2(ClearScreen, className) \
ODRX_DECLARE_PROPERTY2(AlternativeHlt, className) \
ODRX_DECLARE_PROPERTY2(AlternativeHltColor, className) \
ODRX_DECLARE_PROPERTY2(UseTTFCache, className) \
ODRX_DECLARE_PROPERTY2(UseLutPalette, className) \
ODRX_DECLARE_PROPERTY2(LinesRepMode, className) \
ODRX_DECLARE_PROPERTY2(LineweightOptimization, className) \
\
ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(className); \
  ODRX_INHERIT_PROPERTIES(TPropsBase) \
  ODRX_GENERATE_PROPERTY2(DiscardBackFaces, className) \
  ODRX_GENERATE_PROPERTY2(RegenCoef, className) \
  ODRX_GENERATE_PROPERTY2(ClearScreen, className) \
  ODRX_GENERATE_PROPERTY2(AlternativeHlt, className) \
  ODRX_GENERATE_PROPERTY2(AlternativeHltColor, className) \
  ODRX_GENERATE_PROPERTY2(UseTTFCache, className) \
  ODRX_GENERATE_PROPERTY2(UseLutPalette, className) \
  ODRX_GENERATE_PROPERTY2(LinesRepMode, className) \
  ODRX_GENERATE_PROPERTY2(LineweightOptimization, className) \
ODRX_END_DYNAMIC_PROPERTY_MAP(className); \
\
ODRX_DEFINE_PROPERTY_METHODS2(DiscardBackFaces,       className, className, discardBackFaces,              setDiscardBackFaces,           getBool); \
ODRX_DEFINE_PROPERTY_METHODS2(RegenCoef,              className, className, getRegenCoef,                  setRegenCoef,                  getDouble); \
ODRX_DEFINE_PROPERTY_METHODS2(ClearScreen,            className, className, isClearScreenEnabled,          enableClearScreen,             getBool) \
ODRX_DEFINE_PROPERTY_METHODS2(AlternativeHlt,         className, className, isAlternativeHltEnabled,       enableAlternativeHlt,          getBool) \
ODRX_DEFINE_PROPERTY_METHODS2(AlternativeHltColor,    className, className, alternativeHltColor,           setAlternativeHltColor,        getUInt32) \
ODRX_DEFINE_PROPERTY_METHODS2(UseTTFCache,            className, className, isTTFCacheEnabled,             enableTTFCache,                getBool) \
ODRX_DEFINE_PROPERTY_METHODS2(UseLutPalette,          className, className, useLutPalette,                 setUseLutPalette,              getUInt32) \
ODRX_DEFINE_PROPERTY_METHODS2(LinesRepMode,           className, className, isLinesRepModeEnabled,         enableLinesRepMode,            getBool) \
ODRX_DEFINE_PROPERTY_METHODS2(LineweightOptimization, className, className, getLineweightOptimizationProp, setLineweightOptimizationProp, getUInt32)

#endif // __COMMON_OPENGL_PROPERTIES__
