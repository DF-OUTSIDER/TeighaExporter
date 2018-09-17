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
// GLES2 device visual style definition

#ifndef ODTRVISVISUALSTYLE
#define ODTRVISVISUALSTYLE

#include "TD_PackPush.h"

#include "Gi/GiVisualStyle.h"
#include "OdVector.h"

#include "TrVisRenderMode.h"

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisVisualStyleProperty
{
  protected:
    struct DataType
    {
      OdUInt32 m_type     : 4;
      OdUInt32 m_set      : 1;
      OdUInt32 m_alloc    : 1;
      OdUInt32 m_modified : 1;
      OdUInt32 m_reserve  : 1;
      OdUInt32 m_num      : 8;

      DataType()
        : m_type(0), m_set(0), m_alloc(0), m_modified(0), m_reserve(0), m_num(0)
      { }

      bool isPropSet() const { return m_alloc == 1; }
      void setPropSet(bool bSet = true) { m_alloc = (bSet) ? 1 : 0; }

      bool isInherit() const { return m_set != 1; }
      void setInherit(bool bSet) { m_set = (bSet) ? 0 : 1; }

      bool isModified() const { return m_modified == 1; }
      void setModified(bool bSet) { m_modified = (bSet) ? 1 : 0; }

      OdGiVariant::VariantType getType() const { return (OdGiVariant::VariantType)m_type; }
      void setType(OdGiVariant::VariantType type) { m_type = (OdUInt32)type; }

      OdUInt32 getPropNumber() const { return m_num; }
      void setPropNumber(OdUInt32 num) { m_num = num; }
    } m_type;
    union TypeData
    {
      bool m_boolVal;
      OdInt32 m_intVal;
      OdUInt32 m_uintVal;
      double m_dblVal;
      OdChar *m_ptrVal;

      TypeData()
        : m_ptrVal(NULL)
      { }

      bool getBool() const { return m_boolVal; }
      void setBool(bool bVal) { m_boolVal = bVal; }

      OdInt32 getInt() const { return m_intVal; }
      void setInt(OdInt32 iVal) { m_intVal = iVal; }

      OdUInt32 getUInt() const { return m_uintVal; }
      void setUInt(OdUInt32 uVal) { m_uintVal = uVal; }

      double getDouble() const { return m_dblVal; }
      void setDouble(double dVal) { m_dblVal = dVal; }

      const OdChar *getString() const { return m_ptrVal; }
      void setString(const OdChar *pStr)
      {
        if (pStr)
        {
          OdUInt32 nLen = lclk(pStr);
          OdChar *pNew = new OdChar[nLen + 1];
          cstr(pNew, pStr, nLen); pNew[nLen] = 0;
          m_ptrVal = pNew;
        } else m_ptrVal = NULL;
      }
      void resetString()
      {
        if (m_ptrVal) { delete []m_ptrVal; m_ptrVal = NULL; }
      }
      bool cmpString(const OdChar *pStr) const
      {
        if (!m_ptrVal != !m_ptrVal) return false;
        if (m_ptrVal)
        {
          const OdChar *pStrLeft = m_ptrVal;
          while (*pStrLeft && *pStr)
          {
            if (*pStrLeft != *pStr)
              return false;
            pStrLeft++; pStr++;
          }
          return *pStrLeft == *pStr;
        }
        return true;
      }
      protected:
        OdUInt32 lclk(const OdChar *pStr) const
        {
          if (!pStr) return 0;
          OdUInt32 nLen = 0;
          while (pStr[nLen]) nLen++;
          return nLen;
        }
        void cstr(OdChar *pStr, const OdChar *pStr_, OdUInt32 nLen) const
        {
          for (OdUInt32 i = 0; i < nLen; i++)
            pStr[i] = pStr_[i];
        }
    } m_data;
  public:
    enum ColorType
    {
      kColorNone       = 0,
      kColorForeground = 1,
      kColorBackground = 2,

      kColorRGB        = 255
    };
  protected:
    void blockExecution() const { throw OdError(eInvalidInput); }
    bool isValidType(OdGiVariant::VariantType type) const { return (type > OdGiVariant::kUndefined) && (type < OdGiVariant::kTable); }
    bool hasString() const { return m_type.isPropSet() && (m_type.getType() == OdGiVariant::kString); }
    void resetString() { if (hasString()) m_data.resetString(); }
    void setType(OdGiVariant::VariantType type)
    {
      if (!isValidType(type))
        blockExecution();
      if (!m_type.isPropSet())
      {
        m_type.setType(type);
        m_type.setPropSet();
      }
      else if (type != m_type.getType())
        blockExecution();
    }
    void checkType(const OdGiVariant::VariantType *pTypes, OdUInt32 nTypes) const
    {
      if (!m_type.isPropSet())
        blockExecution();
      for (OdUInt32 nType = 0; nType < nTypes; nType++)
      {
        if (m_type.getType() == pTypes[nType])
          return;
      }
      blockExecution();
    }
    void checkType(OdGiVariant::VariantType type) const { checkType(&type, 1); }
  public:
    OdTrVisVisualStyleProperty() { }
    OdTrVisVisualStyleProperty(const OdTrVisVisualStyleProperty &from) { copyFrom(from, from.operation()); }
    ~OdTrVisVisualStyleProperty() { resetString(); }

    OdTrVisVisualStyleProperty &operator =(const OdTrVisVisualStyleProperty &from)
    { reset(); copyFrom(from, from.operation()); return *this; }

    void setPropertyNumber(OdUInt32 propNum) { m_type.setPropNumber(propNum); }
    OdUInt32 propertyNumber() const { return m_type.getPropNumber(); }

    OdGiVariant::VariantType type() const { return (OdGiVariant::VariantType)m_type.getType(); }

    void set(bool bVal, bool bSet = true)
    {
      setType(OdGiVariant::kBoolean);
      m_data.setBool(bVal);
      m_type.setInherit(!bSet);
    }
    void set(OdInt32 iVal, bool bSet = true)
    {
      setType(OdGiVariant::kInt);
      m_data.setInt(iVal);
      m_type.setInherit(!bSet);
    }
    void set(OdUInt32 uVal, bool bSet = true, bool bAsColor = false)
    {
      setType((!bAsColor) ? OdGiVariant::kInt : OdGiVariant::kColor);
      m_data.setUInt(uVal);
      m_type.setInherit(!bSet);
    }
    void set(ODCOLORREF cVal, ColorType type, bool bSet = true)
    {
      setType(OdGiVariant::kColor);
      m_data.setUInt(ODRGBA(ODGETRED(cVal), ODGETGREEN(cVal), ODGETBLUE(cVal), type));
      m_type.setInherit(!bSet);
    }
    void set(double dVal, bool bSet = true)
    {
      setType(OdGiVariant::kDouble);
      m_data.setDouble(dVal);
      m_type.setInherit(!bSet);
    }
    void set(const OdChar *sVal, bool bSet = true)
    {
      resetString();
      setType(OdGiVariant::kString);
      m_data.setString(sVal);
      m_type.setInherit(!bSet);
    }
    void set(const OdString &sVal, bool bSet = true) { set(sVal.c_str(), bSet); }
    void set(const char *sVal, bool bSet = true) { set(OdString(sVal), bSet); }

    bool getBool() const
    {
      checkType(OdGiVariant::kBoolean);
      return m_data.getBool();
    }
    OdUInt8 getBoolAsInt() const { return (getBool()) ? 1 : 0; }
    OdInt32 getInt() const
    {
      checkType(OdGiVariant::kInt);
      return m_data.getInt();
    }
    OdUInt32 getUInt() const
    {
      const OdGiVariant::VariantType validTypes[] = { OdGiVariant::kInt, OdGiVariant::kColor };
      checkType(validTypes, sizeof(validTypes) / sizeof(OdGiVariant::VariantType));
      return m_data.getUInt();
    }
    ODCOLORREF getColor(ColorType *pType = NULL) const
    {
      checkType(OdGiVariant::kColor);
      if (pType) *pType = (ColorType)ODGETALPHA(m_data.getUInt());
      return ODRGBA(ODGETRED(m_data.getUInt()), ODGETGREEN(m_data.getUInt()), ODGETBLUE(m_data.getUInt()), 255);
    }
    ColorType getColorType() const
    {
      checkType(OdGiVariant::kColor);
      return (ColorType)ODGETALPHA(m_data.getUInt());
    }
    double getDouble() const
    {
      checkType(OdGiVariant::kDouble);
      return m_data.getDouble();
    }
    const OdChar *getString() const
    {
      checkType(OdGiVariant::kString);
      return m_data.getString();
    }

    void setOperation(bool bSet) { m_type.setInherit(!bSet); }
    bool operation() const { return !m_type.isInherit(); }

    void setFlag(OdUInt32 nFlag, bool bSet)
    {
      OdUInt32 flags = 0;
      if (m_type.isPropSet()) flags = getUInt();
      SETBIT(flags, nFlag, bSet);
      set(flags);
    }
    bool getFlag(OdUInt32 nFlag) const
    { return GETBIT(getUInt(), nFlag); }

    void setModified(bool bSet) { m_type.setModified(bSet); }
    bool isModified() const { return m_type.isModified(); }

    void copyFrom(const OdTrVisVisualStyleProperty &from, bool bSet = true)
    {
      resetString();
      setType(from.m_type.getType());
      switch (from.m_type.getType())
      {
        case OdGiVariant::kBoolean: m_data.setBool(from.m_data.getBool()); break;
        case OdGiVariant::kInt: case OdGiVariant::kColor: m_data.setUInt(from.m_data.getUInt()); break;
        case OdGiVariant::kDouble: m_data.setDouble(from.m_data.getDouble()); break;
        case OdGiVariant::kString: m_data.setString(from.m_data.getString()); break;
        case OdGiVariant::kTable: case OdGiVariant::kUndefined: ODA_FAIL(); break;
      }
      m_type.setInherit(!bSet);
      m_type.setModified(from.isModified());
      m_type.setPropNumber(from.propertyNumber());
    }

    void reset()
    {
      resetString();
      m_type.setType(OdGiVariant::kUndefined);
      m_type.setPropSet(false);
      m_type.setInherit(true);
      m_type.setPropNumber(0);
      m_data.m_ptrVal = NULL;
    }

    bool compare(const OdTrVisVisualStyleProperty &with, bool bCmpVal = true, bool bCmpOp = false, bool bCmpNum = false) const
    {
      if ((m_type.isPropSet() != with.m_type.isPropSet()) || (m_type.getType() != with.m_type.getType()))
        return false;
      if (bCmpVal)
      {
        switch (m_type.getType())
        {
          case OdGiVariant::kBoolean: if (m_data.getBool() != with.m_data.getBool()) return false; break;
          case OdGiVariant::kInt: case OdGiVariant::kColor: if (m_data.getUInt() != with.m_data.getUInt()) return false; break;
          case OdGiVariant::kDouble: if (!OdEqual(m_data.getDouble(), with.m_data.getDouble())) return false; break;
          case OdGiVariant::kString: if (!m_data.cmpString(with.m_data.getString())) return false; break;
          case OdGiVariant::kTable: case OdGiVariant::kUndefined: ODA_FAIL(); break;
        }
      }
      if (bCmpOp && (m_type.isInherit() != with.m_type.isInherit()))
        return false;
      if (bCmpNum && (m_type.getPropNumber() != with.m_type.getPropNumber()))
        return false;
      return true;
    }
    bool operator ==(const OdTrVisVisualStyleProperty &with) const { return compare(with); }
    bool operator !=(const OdTrVisVisualStyleProperty &with) const { return !compare(with); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisVisualStyle
{
  protected:
    typedef OdVector<OdTrVisVisualStyleProperty> PropsCont;
  protected:
    OdGiVisualStyle::Type m_type;
    OdString              m_name;
    bool                  m_bInternal;
    bool                  m_bTemporary;
    mutable PropsCont     m_props;
  protected:
    void checkLength(OdUInt32 nProp) const
    {
      if (m_props.size() <= nProp)
        m_props.resize(nProp + 1);
    }
    void copyType(const OdTrVisVisualStyle &from)
    {
      m_type = from.m_type;
      m_name = from.m_name;
      m_bInternal = from.m_bInternal;
      m_bTemporary = from.m_bTemporary;
    }
    struct PropCheck
    {
        enum ModifierType { kNoMod, kLwdMod, kJitterMod };
        static void lwd_mod(int& curExtension) { curExtension = (curExtension + 1) / 2; }
        static void jitter_mod(int& curExtension)
        {
          switch (curExtension)
          {
            case OdGiVisualStyleProperties::kJitterLow:    curExtension = 4; break;
            case OdGiVisualStyleProperties::kJitterMedium: curExtension = 6; break;
            case OdGiVisualStyleProperties::kJitterHigh:   curExtension = 8; break;
          }
        }
        static bool run(const OdTrVisVisualStyle &pVS, OdGiVisualStyleProperties::Property EdgeFlags, OdUInt32 EdgeFlag,
                        OdGiVisualStyleProperties::Property PropName, int minLimit, ModifierType modType, int &maxExtension)
        {
          if (pVS.property(EdgeFlags).getFlag(EdgeFlag))
          {
            int curExtension = (int)pVS.property(PropName).getInt();
            if (curExtension > minLimit)
            {
              switch (modType)
              {
                case kLwdMod:    lwd_mod(curExtension);    break;
                case kJitterMod: jitter_mod(curExtension); break;
                default:                                   break;
              }
              if (curExtension > maxExtension)
                maxExtension = curExtension;
              return true;
            }
          }
          return false;
        }
    };
  public:
    OdTrVisVisualStyle()
      : m_type(OdGiVisualStyle::kEmptyStyle)
      , m_name()
      , m_bInternal(false)
      , m_bTemporary(false)
      , m_props(0, 1)
    { }
    OdTrVisVisualStyle(const OdTrVisVisualStyle &from)
      : m_type(OdGiVisualStyle::kEmptyStyle)
      , m_name()
      , m_bInternal(false)
      , m_bTemporary(false)
      , m_props(0, 1)
    { copyFrom(from); }
    ~OdTrVisVisualStyle() { }

    void copyFrom(const OdTrVisVisualStyle &from, bool bCopyType = true, bool bCopyDiff = false)
    {
      if (bCopyType)
        copyType(from);
      OdUInt32 nProps = from.m_props.size();
      m_props.resize(nProps);
      for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
      {
        if (!bCopyDiff || (m_props[nProp] != from.m_props[nProp]))
          m_props[nProp] = from.m_props[nProp];
      }
    }
    void copyModified(const OdTrVisVisualStyle &from, bool bCopyType = true)
    {
      if (bCopyType)
        copyType(from);
      OdUInt32 nProps = from.m_props.size();
      m_props.resize(nProps);
      for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
      {
        if (from.m_props[nProp].isModified())
        {
          m_props[nProp] = from.m_props[nProp];
          m_props[nProp].setModified(false);
        }
      }
    }

    OdTrVisVisualStyle &operator =(const OdTrVisVisualStyle &from) { copyFrom(from); return *this; }

    void setType(OdGiVisualStyle::Type tpy) { m_type = tpy; }
    OdGiVisualStyle::Type type() const { return m_type; }

    void setName(const OdString &name) { m_name = name; }
    const OdString &name() const { return m_name; }

    void setInternal(bool bSet) { m_bInternal = bSet; }
    bool isInternal() const { return m_bInternal; }

    void setTemporary(bool bSet) { m_bTemporary = bSet; }
    bool isTemporary() const { return m_bTemporary; }

    void setPropertiesNumber(OdUInt32 nProps) { m_props.resize(nProps); }
    OdUInt32 numProperties() const { return m_props.size(); }

    void clear() { m_props.setPhysicalLength(0); m_type = OdGiVisualStyle::kEmptyStyle; }

    OdTrVisVisualStyleProperty &property(OdGiVisualStyleProperties::Property prop) { checkLength(prop); return m_props[prop]; }
    const OdTrVisVisualStyleProperty &property(OdGiVisualStyleProperties::Property prop) const { checkLength(prop); return m_props[prop]; }

    bool compare(const OdTrVisVisualStyle &with, bool bCmpType = false, bool bCmpProps = true) const
    {
      if (bCmpType && ((m_type != with.m_type) ||
                       (m_name != with.m_name) ||
                       (m_bInternal != with.m_bInternal) ||
                       (m_bTemporary != with.m_bTemporary)))
        return false;
      if (bCmpProps)
      {
        if (m_props.size() != with.m_props.size())
          return false;
        const OdUInt32 nProps = m_props.size();
        for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
        {
          if (m_props[nProp] != with.m_props[nProp])
            return false;
        }
      }
      return true;
    }
    bool operator ==(const OdTrVisVisualStyle &with) const { return compare(with); }
    bool operator !=(const OdTrVisVisualStyle &with) const { return !compare(with); }

    void resetModified(bool bSet = false)
    {
      const OdUInt32 nProps = m_props.size();
      OdTrVisVisualStyleProperty *pProps = m_props.asArrayPtr();
      for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
        pProps[nProp].setModified(bSet);
    }
    OdUInt32 countModified() const
    {
      OdUInt32 nModified = 0;
      const OdUInt32 nProps = m_props.size();
      const OdTrVisVisualStyleProperty *pProps = m_props.getPtr();
      for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
      {
        if (pProps[nProp].isModified())
          nModified++;
      }
      return nModified;
    }

    bool resolveInheritance(const OdTrVisVisualStyle &owner, OdUInt32 *nResolved = NULL, OdUInt32 *nUnresolved = NULL);
    OdUInt32 countUnresolved() const;
    bool backSubstitution(const OdTrVisVisualStyle &child, OdUInt32 *nSubstituted = NULL, OdUInt32 *nUnresolved = NULL);

    void computeRenderMode(OdTrVisRenderMode &pRM, bool bBestMatch = false) const;
    int computeExtentsEnlargement() const;
};

// Inlines section

inline
bool OdTrVisVisualStyle::resolveInheritance(const OdTrVisVisualStyle &owner, OdUInt32 *nResolved, OdUInt32 *nUnresolved)
{
  if (nResolved) *nResolved = 0; if (nUnresolved) *nUnresolved = 0;
  if (m_props.size() > owner.m_props.size())
    return false;
  const OdUInt32 nProps = m_props.size();
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    OdTrVisVisualStyleProperty &prop = m_props[nProp];
    if (!prop.operation())
    {
      if (owner.m_props[nProp].operation())
      {
        prop = owner.m_props[nProp];
        if (nResolved) (*nResolved)++;
      }
      else if (nUnresolved)
        (*nUnresolved)++;
    }
  }
  return true;
}

inline
OdUInt32 OdTrVisVisualStyle::countUnresolved() const
{
  OdUInt32 nUnresolved = 0;
  const OdUInt32 nProps = m_props.size();
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    if (!m_props[nProp].operation())
      nUnresolved++;
  }
  return nUnresolved;
}

inline
bool OdTrVisVisualStyle::backSubstitution(const OdTrVisVisualStyle &child, OdUInt32 *nSubstituted, OdUInt32 *nUnresolved)
{
  const OdUInt32 nProps = odmax(m_props.size(), child.m_props.size());
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    OdTrVisVisualStyleProperty &prop = child.m_props[nProp];
    if (prop.operation())
    {
      m_props[nProp] = prop;
      if (nSubstituted) (*nSubstituted)++;
    }
    else if (nUnresolved && !m_props[nProp].operation())
      (*nUnresolved)++;
  }
  return true;
}

inline
void OdTrVisVisualStyle::computeRenderMode(OdTrVisRenderMode &pRM, bool bBestMatch) const
{
  pRM.setDepthBufferEnabled(!property(OdGiVisualStyleProperties::kUseDrawOrder).getBool());
  //pRM.setStencilBufferEnabled(property(OdGiVisualStyleProperties::kFaceColorMode).getInt() == OdGiVisualStyleProperties::kBackgroundColor);
  pRM.setStencilBufferEnabled(property(OdGiVisualStyleProperties::kFaceLightingModel).getInt() == OdGiVisualStyleProperties::kConstant);
  pRM.setPolygonOffsetEnabled(property(OdGiVisualStyleProperties::kEdgeModel).getInt() != OdGiVisualStyleProperties::kNoEdges);
  // There are 4 places where lighting can be enabled/disable. Which is actually used by Acad?
  //pRM.setLightingEnabled(property(OdGiVisualStyleProperties::kFaceLightingModel).getInt() != OdGiVisualStyleProperties::kInvisible);
  //pRM.setLightingEnabled(property(OdGiVisualStyleProperties::kLightingEnabled).getBool());
  pRM.setLightingEnabled(property(OdGiVisualStyleProperties::kFaceLightingModel).getInt() > OdGiVisualStyleProperties::kConstant);
  pRM.setFaceNormalsEnabled(property(OdGiVisualStyleProperties::kFaceLightingQuality).getInt() <= OdGiVisualStyleProperties::kPerFaceLighting);
  pRM.calibrate();
  if (bBestMatch)
  {
    if (pRM.is_sh() && property(OdGiVisualStyleProperties::kEdgeStyles).getFlag(OdGiVisualStyleProperties::kSilhouetteFlag) &&
                      (property(OdGiVisualStyleProperties::kEdgeSilhouetteWidth).getInt() > 0))
      pRM.setPolygonOffsetEnabled(true); // #CORE-11059
  }
}

inline
int OdTrVisVisualStyle::computeExtentsEnlargement() const
{
  int maxExtension = 0;
  
  // Edges width flag
  PropCheck::run(*this, OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeWidthFlag, OdGiVisualStyleProperties::kEdgeWidth, 1, PropCheck::kLwdMod, maxExtension);
  // Edges overhang
  PropCheck::run(*this, OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeOverhangFlag, OdGiVisualStyleProperties::kEdgeOverhang, 0, PropCheck::kNoMod, maxExtension);
  // Jitter effect
  PropCheck::run(*this, OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeJitterFlag, OdGiVisualStyleProperties::kEdgeJitterAmount, 0, PropCheck::kJitterMod, maxExtension);
  // Halo gap
  if (!PropCheck::run(*this, OdGiVisualStyleProperties::kEdgeModifiers, OdGiVisualStyleProperties::kEdgeHaloGapFlag, OdGiVisualStyleProperties::kEdgeHaloGap, 0, PropCheck::kNoMod, maxExtension))
    // Silhouette
    PropCheck::run(*this, OdGiVisualStyleProperties::kEdgeStyles, OdGiVisualStyleProperties::kSilhouetteFlag, OdGiVisualStyleProperties::kEdgeSilhouetteWidth, 1, PropCheck::kLwdMod, maxExtension);
  return maxExtension;
}

#include "TD_PackPop.h"

#endif // ODTRVISVISUALSTYLE
