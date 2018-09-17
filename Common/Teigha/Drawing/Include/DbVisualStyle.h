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

#ifndef OD_DBVISUALSTYLE_H
#define OD_DBVISUALSTYLE_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "Gi/GiVisualStyle.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  // Legacy (shademode) styles
  const wchar_t kszVSFlat[]                 = L"Flat";
  const wchar_t kszVSFlatWithEdges[]        = L"FlatWithEdges";
  const wchar_t kszVSGouraud[]              = L"Gouraud";
  const wchar_t kszVSGouraudWithEdges[]     = L"GouraudWithEdges";

  // Visual Styles
  const wchar_t kszVS2DWireframe[]   = L"2dWireframe";
  const wchar_t kszVS3DWireframe[]   = L"Wireframe"; // Was "3dWireframe" before (#11263)
  const wchar_t kszVSBasic[]         = L"Basic";
  const wchar_t kszVSHidden[]        = L"Hidden";    // Was "3D Hidden" before   (#11263)
  const wchar_t kszVSRealistic[]     = L"Realistic";
  const wchar_t kszVSConceptual[]    = L"Conceptual";
  const wchar_t kszVSCustom[]        = L"Custom";
  const wchar_t kszVSShadesOfGray[]  = L"Shades of Gray";
  const wchar_t kszVSSketchy[]       = L"Sketchy";
  const wchar_t kszVSXRay[]          = L"X-Ray";

  // AirMax Visual Styles
  const wchar_t kszVSShadedWithEdges[]      = L"Shaded with edges";
  const wchar_t kszVSShaded[]               = L"Shaded";

  // Highlight Visual Styles (internal use only)
  const wchar_t kszVSDim[]           = L"Dim";
  const wchar_t kszVSBrighten[]      = L"Brighten";
  const wchar_t kszVSThicken[]       = L"Thicken";
  const wchar_t kszVSTransparent[]   = L"Transparent";
  const wchar_t kszVSLinePattern[]   = L"Linepattern";
  const wchar_t kszVSFacePattern[]   = L"Facepattern";
  const wchar_t kszVSColorChange[]   = L"ColorChange";

  // Edge Style Override Visual Styles (internal use only)
  const wchar_t kszVSJitterOff[]     = L"JitterOff";
  const wchar_t kszVSOverhangOff[]   = L"OverhangOff";
  const wchar_t kszVSEdgeColorOff[]  = L"EdgeColorOff";
}
/** \details
    This class represents a visual style (a collection of properties) in an OdDbDatabase instance.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbVisualStyle : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbVisualStyle);

  /** \details
      Default constructor.
  */
  OdDbVisualStyle();

  /** \details
      Sets the description for the visual style.

      \param pDescription [in]  Reference to a string that contains the description for this visual style to set.
  */
  void setDescription(const OdString& pDescription);

  /** \details
      Returns the description for the visual style as a string.
  */
  const OdString description () const;

  /** \details
      Sets the type of the visual style. All available types are defined in the OdGiVisualStyle::Type enumeration.  

      \param type [in]  The visual style type to set.
  */
  void setType(OdGiVisualStyle::Type type);

  /** \details
      Returns the type of the visual style. All available types are defined in the OdGiVisualStyle::Type enumeration.  
  */
  OdGiVisualStyle::Type type() const;

  /** \details
      Sets the face style of the visual style.  

      \param faceStyle [in]  Reference to the OdGiFaceStyle object to set.
  */
  void setFaceStyle(const OdGiFaceStyle& faceStyle);

  /** \details
      Returns a reference to a face style of the visual style.  
  */
  const OdGiFaceStyle& faceStyle() const;

  /** \details
      Sets the edge style of the visual style.

      \param edgeStyle [in]  Reference to the OdGiEdgeStyle object to set.
  */
  void setEdgeStyle(const OdGiEdgeStyle& edgeStyle);

  /** \details
      Returns a reference to an edge style of the visual style.  
  */
  const OdGiEdgeStyle& edgeStyle() const;

  /** \details
      Sets the display style of the visual style.

      \param displayStyle [in]  Reference to the OdGiDisplayStyle object to set.
  */
  void setDisplayStyle(const OdGiDisplayStyle& displayStyle);

  /** \details
      Returns a reference to a display style of the visual style.  
  */
  const OdGiDisplayStyle& displayStyle() const;

  /** \details
      Determines whether the visual style is for internal use only.

      \returns
      true - if the visual style is for internal use only.
      false - otherwise.
  */
  bool isInternalUseOnly() const;

  /** \details
      Sets a flag that indicates the visual style is for internal use only.

      \param bInternalUseOnly [in]  Boolean value to set.
  */
  void setInternalUseOnly(bool bInternalUseOnly);

  /** \details
      Creates a face style of the visual style.

      \returns
      A smart pointer to the created OdGiFaceStyle object.
  */
  static OdGiFaceStylePtr createFaceStyle();

  /** \details
      Creates an edge style of the visual style.

      \returns
      A smart pointer to the created OdGiEdgeStyle object.
  */
  static OdGiEdgeStylePtr createEdgeStyle();

  /** \details
      Creates a display style of the visual style.

      \returns
      A smart pointer to the created OdGiDisplayStyle object.
  */
  static OdGiDisplayStylePtr createDisplayStyle();

  /** \details
      Creates a OdGiVisualStyle instance.

      \returns
      A smart pointer to the created OdGiVisualStyle object.
  */
  static OdGiVisualStylePtr createVisualStyle();

  /** \details
      Sets a property of the visual style.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param pVal [in]  Pointer to the OdGiVariant property value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, const OdGiVariant *pVal,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);
  /** \details
      Sets an integer property of the visual style.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param nVal [in]  Integer property value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, OdInt32 nVal,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);
  
  /** \details
      Sets a boolean property of the visual style.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param bVal [in]  Boolean property value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, bool bVal,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);
  
  /** \details
      Sets a double property of the visual style.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param dVal [in]  Boolean property value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, double dVal,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
      Sets a color property of the visual style with given RGB component values.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param red [in]  Red component value to set.
      \param green [in]  Green component value to set.
      \param blue [in]  Blue component value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, double red, double green, double blue,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);
  
  /** \details
      Sets a color property of the visual style by the given OdCmColor value.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param pColor [in]  Pointer to the OdCmColor value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, const OdCmColor *pColor,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
      Sets a string property of the visual style.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param strVal [in]  Reference to the OdString value to set.
      \param op [in]  OdGiVisualStyleOperations::Operation to use for setting the property. 
  */
  void setTrait(OdGiVisualStyleProperties::Property prop, const OdString &strVal,
                OdGiVisualStyleOperations::Operation op = OdGiVisualStyleOperations::kSet);

  /** \details
      Gets a property of the visual style and an operation that is currently used for this property.

      \param prop [in]  OdGiVisualStyleProperties::Property to set.
      \param op [out]  Output OdGiVisualStyleOperations::Operation that is currently in effect for this property. 

      \returns
      Returns a smart pointer to the OdGiVariant value of the property.
  */
  OdGiVariantPtr trait(OdGiVisualStyleProperties::Property prop, OdGiVisualStyleOperations::Operation *pOp = NULL) const;

  /** \details
      Sets a property flag for the visual style (for properties that are bitfield enumerations).

      \param flagProp [in]  OdGiVisualStyleProperties::Property to set.
      \param flagVal [in]  Flag value to set. 
      \param bEnable [in]  Boolean value that defines if the flag is enabled. Use true to enable the flag, false to disable.
  */
  void setTraitFlag(OdGiVisualStyleProperties::Property flagProp, OdUInt32 flagVal, bool bEnable = true);

  /** \details
      Gets a property flag from the visual style (for properties that are bitfield enumerations).

      \param flagProp [in]  OdGiVisualStyleProperties::Property to get.
      \param flagVal [in]  Flag value to get. 
      
      \returns
      true - if the flag is enabled.
      false - otherwise.
  */
  bool traitFlag(OdGiVisualStyleProperties::Property flagProp, OdUInt32 flagVal) const;

  /** \details
      Gets a name (key) of the OdGiVisualStyle object in the dictionary.

      \param value [out]  A reference to a string value to get the name.
  */
  void name(OdString &value) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdGiDrawable* drawable();

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbVisualStyle object pointers.
*/
typedef OdSmartPtr<OdDbVisualStyle> OdDbVisualStylePtr;

#include "TD_PackPop.h"

#endif // OD_DBVISUALSTYLE_H
