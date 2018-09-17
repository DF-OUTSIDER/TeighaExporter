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

#ifndef __ODGIVISUALSTYLEDATA_H__
#define __ODGIVISUALSTYLEDATA_H__

#include "Gi/GiVisualStyle.h"
#include "StaticRxObject.h"

#include "TD_PackPush.h"

// OdGiVisualStyleDataContainer

/** \details
  <group OdGi_Classes>

  This class defines the container for visual style data.

  Corresponding C++ library: TD_Gi
*/
class OdGiVisualStyleDataContainer : public OdStaticRxObject<OdGiVisualStyle>
{
  public:
    /** \details
      <group OdGi_Classes>

      OdCmEntityColor wrapper for OdCmColorBase interface.
    */
    struct OdCmColorBaseAdapt : public OdCmColorBase
    {
      OdGiVariant *m_pVar;

      OdCmColorBaseAdapt() : m_pVar(NULL) { }

      /** \details
        Sets the variant data storage associated with this object.
          
        \param pBase [in]  Pointer to the variant data storage object with which this color wrapper is associated.
      */
      void setBase(OdGiVariant *pVar) { m_pVar = pVar; }

      /** \details
        Returns the pointer to the variant data storage object with which this color wrapper is associated.
      */
      OdGiVariant *base() { return m_pVar; }

      /** \details
        Returns the entity color object.
      */
      OdCmEntityColor &entColor() { return const_cast<OdCmEntityColor&>(m_pVar->asColor()); }
      
      /** \details
        Returns the entity color object.
      */
      const OdCmEntityColor &entColor() const { return m_pVar->asColor(); }

      /** \details
        Returns the color method of the entity color object as a value of the ColorMethod enumeration.
        
        \remarks
        The color method can be one of the following:
     
        <table>
        Name            Value    Description
        _kByLayer_      0xC0     Color is specified by the layer object on which the object resides.
        _kByBlock_      0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_      0xC2     Color is specified by an RGB-value.
        _kByACI_        0xC3     Color is specified by an index (ACI) of a color palette.
        _kByPen_        0xC4     Color is specified by an index of a pen color table.
        _kForeground_   0xC5     Color is the foreground.
        _kByDgnIndex_   0xC7     Color is specified by an index of a .dgn color table.
        _kNone_         0xC8     Color is absent (object is clear).
        </table>
      */
      OdCmEntityColor::ColorMethod colorMethod() const { return entColor().colorMethod(); }
      
      /** \details
        Sets the color method for the entity color object. 
        
        \param colorMethod [in]  Color method as the ColorMethod enumeration. 
        
        \remarks
        The color method can be one of the following:
     
        <table>
        Name            Value    Description
        _kByLayer_      0xC0     Color is specified by the layer object on which the object resides.
        _kByBlock_      0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_      0xC2     Color is specified by an RGB-value.
        _kByACI_        0xC3     Color is specified by an index (ACI) of a color palette.
        _kByPen_        0xC4     Color is specified by an index of a pen color table.
        _kForeground_   0xC5     Color is the foreground.
        _kByDgnIndex_   0xC7     Color is specified by an index of a .dgn color table.
        _kNone_         0xC8     Color is absent (object is clear).
        </table>
      */
      void setColorMethod(OdCmEntityColor::ColorMethod colorMethod) { entColor().setColorMethod(colorMethod); }
      
      /** \details
        Checks whether the color method is byColor for the entity color object and returns true if 
        and only if the color method is set to _kByColor_, otherwise it returns false.
      */
      bool isByColor() const { return entColor().isByColor(); }
      
      /** \details
        Checks whether the color method is byLayer for the entity color object and returns true if 
        and only if the color method is set to _kByLayer_ or _kACIbyLayer_, otherwise it returns false.
      */
      bool isByLayer() const { return entColor().isByLayer(); }
      
      /** \details
        Checks whether the color method is byBlock for the entity color object and returns true if 
        and only if the color method is set to _kByBlock_ or _kACIbyBlock_, otherwise it returns false.
      */
      bool isByBlock() const { return entColor().isByBlock(); }
      
      /** \details
        Checks whether the color method is byACI for the entity color object and returns true if 
        and only if the color method is set to _kByACI_, otherwise it returns false.
        
        \remarks
        This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).
      */
      bool isByACI() const { return entColor().isByACI(); }
      
      /** \details
        Checks whether the color method is Foreground for the entity color object and returns true if 
        and only if the color method is set to _kForeground_ or _kACIforeground_, otherwise it returns false.
      */
      bool isForeground() const { return entColor().isForeground(); }
      
      /** \details
        Checks whether the color method is byDgnIndex for the entity color object and returns true if 
        and only if the color method is set to _kByDgnIndex_, otherwise it returns false.
      */
      bool isByDgnIndex() const { return entColor().isByDgnIndex(); }
      
      /** \details
        Returns the packed 32-bit integer value that stores the color method and 
        color components of the entity color object.
      */
      OdUInt32 color() const { return entColor().color(); }
      
      /** \details
        Sets the color method and color components for the entity color object as an integer value. 
        
        \param color [in]  A packed 32-bit integer value that specifies the color method and color components. 
      */
      void setColor(OdUInt32 color) { entColor().setColor(color); }
      
      /** \details
        Sets the red, green, and blue color components and the color method to byColor for the entity color object.  
        
        \param red [in]  Red component as an integer value in the range 0 to 255.
        \param green [in]  Green component as an integer value in the range 0 to 255.
        \param blue [in]  Blue component as an integer value in the range 0 to 255. 
      */
      void setRGB(OdUInt8 red, OdUInt8 green, OdUInt8 blue) { entColor().setRGB(red, green, blue); }
      
      /** \details
        Sets the red color component for the entity color object.
        
        \param red [in]  Red component as an integer value in the range 0 to 255.
      */
      void setRed(OdUInt8 red) { entColor().setRed(red); }
      
      /** \details
        Sets the green color component for the entity color object.
        
        \param green [in]  Green component as an integer value in the range 0 to 255. 
      */
      void setGreen(OdUInt8 green) { entColor().setGreen(green); }
      
      /** \details
        Sets the blue color component for the entity color object.
        
        \param blue [in]  Blue component as an integer value in the range 0 to 255. 
      */
      void setBlue(OdUInt8 blue) { entColor().setBlue(blue); }
      
      /** \details
        Returns the red color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 red() const { return entColor().red(); }
      
      /** \details
        Returns the green color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 green() const { return entColor().green(); }
      
      /** \details
        Returns the blue color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 blue() const { return entColor().blue(); }
      
      /** \details
        Returns the color index (ACI) of the entity color object.
        
        \remarks
        The color index can be one of the following:
     
        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red. 
        _kACIYellow_        2       Yellow. 
        _kACIGreen_         3       Green. 
        _kACICyan_          4       Cyan. 
        _kACIBlue_          5       Blue. 
        _kACIMagenta_       6       Magenta. 
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer. 
        _kACInone_          257     Sets the color method to None.
        </table>
      */
      OdUInt16 colorIndex() const { return entColor().colorIndex(); }
      
      /** \details
        Sets the color index (ACI) of a color palette and sets the color method to byACI for the entity color object.  
        
        \param colorIndex [in]  An integer value that is the index of the color in a palette.
        
        \remarks
        The color index can be one of the following: 
     
        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red. 
        _kACIYellow_        2       Yellow. 
        _kACIGreen_         3       Green. 
        _kACICyan_          4       Cyan. 
        _kACIBlue_          5       Blue. 
        _kACIMagenta_       6       Magenta. 
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer. 
        _kACInone_          257     Sets the color method to None.
        </table>
        
        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.
      */
      void setColorIndex(OdUInt16 colorIndex) { entColor().setColorIndex(colorIndex); }
#if 0
      OdCmEntityColor m_entColor;

      OdCmColorBaseAdapt() : m_entColor() { }
      OdCmColorBaseAdapt(OdUInt8 red, OdUInt8 green, OdUInt8 blue) : m_entColor(red, green, blue) { }
      OdCmColorBaseAdapt(const OdCmEntityColor &cColor) : m_entColor(cColor) { }
      OdCmColorBaseAdapt(const OdCmColorBase &cColor) { m_entColor.setColor(cColor.color()); }
      OdCmColorBaseAdapt(OdCmEntityColor::ColorMethod cm) : m_entColor(cm) { }

      /** \details
        Returns the color method of the entity color object as a value of the ColorMethod enumeration.
        
        \remarks
        The color method can be one of the following:
     
        <table>
        Name            Value    Description
        _kByLayer_      0xC0     Color is specified by the layer object on which the object resides.
        _kByBlock_      0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_      0xC2     Color is specified by an RGB-value.
        _kByACI_        0xC3     Color is specified by an index (ACI) of a color palette.
        _kByPen_        0xC4     Color is specified by an index of a pen color table.
        _kForeground_   0xC5     Color is the foreground.
        _kByDgnIndex_   0xC7     Color is specified by an index of a .dgn color table.
        _kNone_         0xC8     Color is absent (object is clear).
        </table>
      */
      OdCmEntityColor::ColorMethod colorMethod() const { return m_entColor.colorMethod(); }
      
      /** \details
        Sets the color method for the entity color object. 
        
        \param colorMethod [in]  Color method as the ColorMethod enumeration. 
        
        \remarks
        The color method can be one of the following:
     
        <table>
        Name            Value    Description
        _kByLayer_      0xC0     Color is specified by the layer object on which the object resides.
        _kByBlock_      0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_      0xC2     Color is specified by an RGB-value.
        _kByACI_        0xC3     Color is specified by an index (ACI) of a color palette.
        _kByPen_        0xC4     Color is specified by an index of a pen color table.
        _kForeground_   0xC5     Color is the foreground.
        _kByDgnIndex_   0xC7     Color is specified by an index of a .dgn color table.
        _kNone_         0xC8     Color is absent (object is clear).
        </table>
      */
      void setColorMethod(OdCmEntityColor::ColorMethod colorMethod) { m_entColor.setColorMethod(colorMethod); }
      
      /** \details
        Checks whether the color method is byColor for the entity color object and returns true if 
        and only if the color method is set to _kByColor_, otherwise it returns false.
      */
      bool isByColor() const { return m_entColor.isByColor(); }
      
      /** \details
        Checks whether the color method is byLayer for the entity color object and returns true if 
        and only if the color method is set to _kByLayer_ or _kACIbyLayer_, otherwise it returns false.
      */
      bool isByLayer() const { return m_entColor.isByLayer(); }
      
      /** \details
        Checks whether the color method is byBlock for the entity color object and returns true if 
        and only if the color method is set to _kByBlock_ or _kACIbyBlock_, otherwise it returns false.
      */
      bool isByBlock() const { return m_entColor.isByBlock(); }
      
      /** \details
        Checks whether the color method is byACI for the entity color object and returns true if 
        and only if the color method is set to _kByACI_, otherwise it returns false.
        
        \remarks
        This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).
      */
      bool isByACI() const { return m_entColor.isByACI(); }
      
      /** \details
        Checks whether the color method is Foreground for the entity color object and returns true if 
        and only if the color method is set to _kForeground_ or _kACIforeground_, otherwise it returns false.
      */
      bool isForeground() const { return m_entColor.isForeground(); }
      
      /** \details
        Checks whether the color method is byDgnIndex for the entity color object and returns true if 
        and only if the color method is set to _kByDgnIndex_, otherwise it returns false.
      */
      bool isByDgnIndex() const { return m_entColor.isByDgnIndex(); }
      
      /** \details
        Returns the packed 32-bit integer value that stores the color method and 
        color components of the entity color object.
      */
      OdUInt32 color() const { return m_entColor.color(); }
      
      /** \details
        Sets the color method and color components for the entity color object as an integer value. 
        
        \param color [in]  A packed 32-bit integer value that specifies the color method and color components. 
      */
      void setColor(OdUInt32 color) { m_entColor.setColor(color); }
      
      /** \details
        Sets the red, green, and blue color components and the color method to byColor for the entity color object.  
        
        \param red [in]  Red component as an integer value in the range 0 to 255.
        \param green [in]  Green component as an integer value in the range 0 to 255.
        \param blue [in]  Blue component as an integer value in the range 0 to 255. 
      */
      void setRGB(OdUInt8 red, OdUInt8 green, OdUInt8 blue) { m_entColor.setRGB(red, green, blue); }
      
      /** \details
        Sets the red color component for the entity color object.
        
        \param red [in]  Red component as an integer value in the range 0 to 255.
      */
      void setRed(OdUInt8 red) { m_entColor.setRed(red); }
      
      /** \details
        Sets the green color component for the entity color object.
        
        \param green [in]  Green component as an integer value in the range 0 to 255. 
      */
      void setGreen(OdUInt8 green) { m_entColor.setGreen(green); }
      
      /** \details
        Sets the blue color component for the entity color object.
        
        \param blue [in]  Blue component as an integer value in the range 0 to 255. 
      */
      void setBlue(OdUInt8 blue) { m_entColor.setBlue(blue); }
      
      /** \details
        Returns the red color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 red() const { return m_entColor.red(); }
      
      /** \details
        Returns the green color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 green() const { return m_entColor.green(); }
      
      /** \details
        Returns the blue color component of the entity color object as a value in the range 0 to 255.
      */
      OdUInt8 blue() const { return m_entColor.blue(); }
      
      /** \details
        Returns the color index (ACI) of the entity color object.
        
        \remarks
        The color index can be one of the following:
     
        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red. 
        _kACIYellow_        2       Yellow. 
        _kACIGreen_         3       Green. 
        _kACICyan_          4       Cyan. 
        _kACIBlue_          5       Blue. 
        _kACIMagenta_       6       Magenta. 
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer. 
        _kACInone_          257     Sets the color method to None.
        </table>
      */
      OdUInt16 colorIndex() const { return m_entColor.colorIndex(); }
      
      /** \details
        Sets the color index (ACI) of a color palette and sets the color method to byACI for the entity color object.  
        
        \param colorIndex [in]  An integer value that is the index of the color in a palette.
        
        \remarks
        The color index can be one of the following: 
     
        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red. 
        _kACIYellow_        2       Yellow. 
        _kACIGreen_         3       Green. 
        _kACICyan_          4       Cyan. 
        _kACIBlue_          5       Blue. 
        _kACIMagenta_       6       Magenta. 
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer. 
        _kACInone_          257     Sets the color method to None.
        </table>
        
        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.
      */
      void setColorIndex(OdUInt16 colorIndex) { m_entColor.setColorIndex(colorIndex); }
#endif
      // Unnecessary methods
      bool setNames(const OdString& /*colorName*/, const OdString& /*bookName*/ = OdString::kEmpty) { return false; }
      OdString colorName() const { return OdString::kEmpty; }
      OdString bookName() const { return OdString::kEmpty; }
      OdString colorNameForDisplay() const { return OdString::kEmpty; }
    };
    
    // Style subclasses
    
    /** \details
      <group OdGi_Classes>

      This class defines the container for face style data.
    */
    class OdGiFaceStyleDataContainer : public OdStaticRxObject<OdGiFaceStyle>
    {
      protected:
        OdGiVisualStyle           *m_pBase;
        mutable OdCmColorBaseAdapt m_cmMonoColor;
      public:
        OdGiFaceStyleDataContainer()
          : m_pBase(NULL)
        {
        }
        ~OdGiFaceStyleDataContainer()
        {
        }
        
        /** \details
          Sets the visual style associated with this object.
          
          \param pBase [in]  Pointer to the visual style object with which this face style data is associated.
        */
        void setBase(OdGiVisualStyle *pBase)
        {
          m_pBase = pBase;
          m_cmMonoColor.setBase(m_pBase->trait(OdGiVisualStyleProperties::kFaceMonoColor).get());
        }
        
        /** \details
          Returns the pointer to the visual style object with which this face style data is associated.
        */
        OdGiVisualStyle *base() const
        {
          return m_pBase;
        }

        /** \details
          Sets the lighting model to the given value. All valid values are described in the LightingModel enumeration.
          
          \param lightingModel [in]  Lighting model value to set. 
        */
        virtual void setLightingModel(LightingModel lightingModel)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceLightingModel, (OdInt32)lightingModel);
        }
        
        /** \details
          Returns the currently used lighting model of the face style.
        */
        virtual LightingModel lightingModel() const
        {
          return (LightingModel)m_pBase->trait(OdGiVisualStyleProperties::kFaceLightingModel)->asInt();
        }

        /** \details
          Sets the lighting quality of faces to the given value. All valid values are described in the LightingQuality enumeration.
          
          \param lightingQuality [in]  Lighting quality value to set. 
        */
        virtual void setLightingQuality(LightingQuality lightingQuality)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceLightingQuality, (OdInt32)lightingQuality);
        }
        
        /** \details
          Returns the currently used lighting quality of the face style.
        */
        virtual LightingQuality lightingQuality() const
        {
          return (LightingQuality)m_pBase->trait(OdGiVisualStyleProperties::kFaceLightingQuality)->asInt();
        }

        /** \details
          Sets the face color mode of the face style. All valid values are described in the FaceColorMode enumeration.
          
          \param mode [in]  Face color mode value to set.  
        */
        virtual void setFaceColorMode(FaceColorMode mode)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceColorMode, (OdInt32)mode);
        }
        
        /** \details
          Returns the currently used face color mode value.
        */
        virtual FaceColorMode faceColorMode() const
        {
          return (FaceColorMode)m_pBase->trait(OdGiVisualStyleProperties::kFaceColorMode)->asInt();
        }

        /** \details
          Sets a collection of bitflags representing the face modifiers in effect. 
          
          \param nModifiers [in]  Unsigned long value that represents a collection of bitwise flags defined in the FaceModifier enumeration. 
        */
        virtual void setFaceModifiers(unsigned long nModifiers)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceModifiers, (OdInt32)nModifiers);
        }
        
        /** \details
          Enables or disables the specified face modifier flag.  
          
          \param flag [in]  Flag to enable or disable. 
          \param bEnable [in]  Value that indicates whether to enable or disable the specified flag.            
        */
        virtual void setFaceModifierFlag(FaceModifier flag, bool bEnable)
        {
          m_pBase->setTraitFlag(OdGiVisualStyleProperties::kFaceModifiers, flag, bEnable);
        }
        
        /** \details
          Returns the collection of currently used face modifier flags.
        */
        virtual unsigned long faceModifiers() const
        {
          return (unsigned long)m_pBase->trait(OdGiVisualStyleProperties::kFaceModifiers)->asInt();
        }
        
        /** \details
          Returns whether the specified face modifier flag is enabled.  
          
          \param flag [in]  Flag to be checked whether it's enabled or disabled.           
        */
        virtual bool isFaceModifierFlagSet(FaceModifier flag) const
        {
          return m_pBase->traitFlag(OdGiVisualStyleProperties::kFaceModifiers, flag);
        }

        /** \details
          Sets the opacity level of the faces.   
          
          \param nLevel [in]  Opacity level to set. Must be in the range from 0.0 (faces are fully transparent) to 1.0 (no transparency).  
          \param bEnableModifier [in]  Value that indicates whether to enable or disable the opacity.          
        */
        virtual void setOpacityLevel(double nLevel, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceOpacity, nLevel);
          if (bEnableModifier)
            setFaceModifierFlag(kOpacity, bEnableModifier);
        }
        
        /** \details
          Returns whether the opacity level is applied to faces.
        */
        virtual double opacityLevel() const
        {
          return m_pBase->trait(OdGiVisualStyleProperties::kFaceOpacity)->asDouble();
        }

        /** \details
          Sets the amount of specular highlight (indicating reflection and shininess values) of a face's material.   
          
          \param nAmount [in]  Amount of specular highlight of a face's material to set.
          \param bEnableModifier [in]  Value that indicates whether to enable or disable the specular highlight.        
        */
        virtual void setSpecularAmount(double nAmount, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceSpecular, nAmount);
          if (bEnableModifier)
            setFaceModifierFlag(kSpecular, bEnableModifier);
        }
        
        /** \details
          Returns the amount of specular highlight applied to a face's material.
        */
        virtual double specularAmount() const
        {
          return m_pBase->trait(OdGiVisualStyleProperties::kFaceSpecular)->asDouble();
        }

        /** \details
          Sets the color that will be applied to produce a monochromatic effect on the faces.   
          
          \param color [in]  Reference to an OdCmColorBase value to set.  
          \param bEnableMode [in]  Value that indicates whether to enable or disable the mono color mode.        
        */
        virtual void setMonoColor(const OdCmColorBase& color, bool bEnableMode)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kFaceMonoColor, color);
          if (bEnableMode)
            setFaceColorMode(kMono);
        }
        
        /** \details
          Returns a color that is applied to produce a monochromatic effect on the faces.
        */
        virtual const OdCmColorBase& monoColor() const
        {
          return m_cmMonoColor;
        }
        
        /** \details
          Returns a color that is applied to produce a monochromatic effect on the faces.
        */
        virtual OdCmColorBase& monoColor()
        {
          return m_cmMonoColor;
        }
    };
    
    /** \details
      <group OdGi_Classes>

      This class defines the container for edge style data.
    */
    class OdGiEdgeStyleDataContainer : public OdStaticRxObject<OdGiEdgeStyle>
    {
      protected:
        OdGiVisualStyle           *m_pBase;
        mutable OdCmColorBaseAdapt m_cmIntColor;
        mutable OdCmColorBaseAdapt m_cmObColor;
        mutable OdCmColorBaseAdapt m_cmEdgeColor;
        mutable OdCmColorBaseAdapt m_cmSilColor;
        EdgeStyleApply             m_esApply;
      public:
        OdGiEdgeStyleDataContainer()
          : m_pBase(NULL)
          , m_esApply(kDefault)
        {
        }
        ~OdGiEdgeStyleDataContainer()
        {
        }
        
        /** \details
          Sets the visual style associated with this object.
          
          \param pBase [in]  Pointer to the visual style object with which this edge style data is associated.
        */
        void setBase(OdGiVisualStyle *pBase)
        {
          m_pBase = pBase;
          m_cmIntColor.setBase(m_pBase->trait(OdGiVisualStyleProperties::kEdgeIntersectionColor).get());
          m_cmObColor.setBase(m_pBase->trait(OdGiVisualStyleProperties::kEdgeObscuredColor).get());
          m_cmEdgeColor.setBase(m_pBase->trait(OdGiVisualStyleProperties::kEdgeColor).get());
          m_cmSilColor.setBase(m_pBase->trait(OdGiVisualStyleProperties::kEdgeSilhouetteColor).get());
        }
        
        /** \details
          Returns the pointer to the visual style object with which this edge style data is associated.
        */
        OdGiVisualStyle *base() const
        {
          return m_pBase;
        }

        /** \details
          Sets a specified edge model value for this object.
          
          \param model [in]  Edge model value to set. All available values are defined in the EdgeModel enum.
        */
        virtual void setEdgeModel(EdgeModel model)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeModel, (OdInt32)model);
        }
        
        /** \details
          Returns the currently used edge model value for this object. All model values 
          are defined in the EdgeModel enumeration.
        */
        virtual EdgeModel edgeModel() const
        {
          return (EdgeModel)m_pBase->trait(OdGiVisualStyleProperties::kEdgeModel)->asInt();
        }

        /** \details
          Sets a combination of values from the EdgeStyle enumeration as an edge style value for this object. 
          
          \param nStyles [in]  A combination of bit flags from the EdgeStyle enumeration indicating desired edge styles.
        */
        virtual void setEdgeStyles(unsigned long nStyles)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeStyles, (OdInt32)nStyles);
        }
        
        /** \details
          Enables or disables the given edge style flag.  
          
          \param flag [in]  A flag to enable or disable.
          \param bEnable [in]  A boolean value indicating whether the given flag is to be enabled or disabled.
        */
        virtual void setEdgeStyleFlag(EdgeStyle flag, bool bEnable)
        {
          m_pBase->setTraitFlag(OdGiVisualStyleProperties::kEdgeStyles, flag, bEnable);
        }
        
        /** \details
          Returns an unsigned long value that represents a collection of edge style flags in effect.
        */
        virtual unsigned long edgeStyles() const
        {
          return (unsigned long)m_pBase->trait(OdGiVisualStyleProperties::kEdgeStyles)->asInt();
        }
        
        /** \details
          Returns whether the specified face modifier flag is enabled.  
          
          \param flag [in]  Flag to be checked whether it's enabled or disabled.
        */
        virtual bool isEdgeStyleFlagSet(EdgeStyle flag) const
        {
          return m_pBase->traitFlag(OdGiVisualStyleProperties::kEdgeStyles, flag);
        }

        /** \details
          Sets the color of intersection edges.   
          
          \param color [in]  Intersection edge color to set. 
        */
        virtual void setIntersectionColor(const OdCmColorBase& color)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeIntersectionColor, color);
        }
        
        /** \details
          Returns the color of intersection edges.
        */
        virtual const OdCmColorBase& intersectionColor() const
        {
          return m_cmIntColor;
        }
        
        /** \details
          Returns the color of intersection edges.
        */
        virtual OdCmColorBase& intersectionColor()
        {
          return m_cmIntColor;
        }

        /** \details
          Sets the color of obscured edges.    
          
          \param color [in]  Obscured edge color to set. 
        */
        virtual void setObscuredColor(const OdCmColorBase& color)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeObscuredColor, color);
        }
        
        /** \details
          Returns the color of obscured edges.
        */
        virtual const OdCmColorBase& obscuredColor() const
        {
          return m_cmObColor;
        }
        
        /** \details
          Returns the color of obscured edges.
        */
        virtual OdCmColorBase& obscuredColor()
        {
          return m_cmObColor;
        }

        /** \details
          Sets the linetype of obscured edges.     
          
          \param ltype [in]  Obscured edge linetype to set.  
        */
        virtual void setObscuredLinetype(LineType ltype)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeObscuredLinePattern, (OdInt32)ltype);
        }
         
        /** \details
          Returns the linetype of obscured edges.
        */
        virtual LineType obscuredLinetype() const
        {
          return (LineType)m_pBase->trait(OdGiVisualStyleProperties::kEdgeObscuredLinePattern)->asInt();
        }

        /** \details
          Sets the linetype of intersection edges.      
          
          \param ltype [in]  Intersection edge linetype to set.  
        */
        virtual void setIntersectionLinetype(LineType ltype)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeIntersectionLinePattern, (OdInt32)ltype);
        }
        
        /** \details
          Returns the linetype of obscured edges.
        */
        virtual LineType intersectionLinetype() const
        {
          return (LineType)m_pBase->trait(OdGiVisualStyleProperties::kEdgeIntersectionLinePattern)->asInt();
        }

        /** \details
          Sets the angle threshold, above which a facet edge representing adjoining facets will be displayed.       
          
          \param nAngle [in]  Threshold angle to set.  
        */
        virtual void setCreaseAngle(double nAngle)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeCreaseAngle, nAngle);
        }
        
        /** \details
          Returns the double value that is the angle threshold (in degrees) above which a facet edge 
          representing adjoining facets will be displayed.
        */
        virtual double creaseAngle() const
        {
          return m_pBase->trait(OdGiVisualStyleProperties::kEdgeCreaseAngle)->asDouble();
        }

        /** \details
          Sets a collection of bitflags representing the edge modifiers in effect.       
          
          \param nModifiers [in]  Unsigned long value that represents a collection of bitwise flags defined in the EdgeModifier enumeration.  
        */
        virtual void setEdgeModifiers(unsigned long nModifiers)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeModifiers, (OdInt32)nModifiers);
        }
        
        /** \details
          Enables or disables the specified edge modifier flag.        
          
          \param flag [in]  Flag to enable or disable. 
          \param bEnable [in]  Value that indicates whether to enable or disable the specified flag.  
        */
        virtual void setEdgeModifierFlag(EdgeModifier flag, bool bEnable)
        {
          m_pBase->setTraitFlag(OdGiVisualStyleProperties::kEdgeModifiers, flag, bEnable);
        }
        
        /** \details
          Returns the collection of currently used edge modifier flags.
        */
        virtual unsigned long edgeModifiers() const
        {
          return (unsigned long)m_pBase->trait(OdGiVisualStyleProperties::kEdgeModifiers)->asInt();
        }
        
        /** \details
          Returns whether the specified edge modifier flag is enabled.         
          
          \param flag [in]  Flag to be checked whether it's enabled or disabled.   
        */
        virtual bool isEdgeModifierFlagSet(EdgeModifier flag) const
        {
          return m_pBase->traitFlag(OdGiVisualStyleProperties::kEdgeModifiers, flag);
        }

        /** \details
          Sets the color of edges.        
          
          \param color [in]  Color value to set.
          \param bEnableModifier [in]  Value that indicates whether to enable or disable the color.  
        */
        virtual void setEdgeColor(const OdCmColorBase& color, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeColor, color);
        }
        
        /** \details
          Returns the color of edges.
        */
        virtual const OdCmColorBase& edgeColor() const
        {
          return m_cmEdgeColor;
        }
        
        /** \details
          Returns the color of edges.
        */
        virtual OdCmColorBase& edgeColor()
        {
          return m_cmEdgeColor;
        }

        /** \details
          Sets the opacity level of edges.        
          
          \param nLevel [in]  Opacity level to set. Must be in the range from 0.0 (edges are fully transparent) to 1.0 (no transparency). 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable the opacity. 
        */
        virtual void setOpacityLevel(double nLevel, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeOpacity, nLevel);
          if (bEnableModifier)
            setEdgeModifierFlag(kOpacity, bEnableModifier);
        }
        
        /** \details
          Returns the opacity level applied to edges.
        */
        virtual double opacityLevel() const
        {
          return m_pBase->trait(OdGiVisualStyleProperties::kEdgeOpacity)->asDouble();
        }

        /** \details
          Sets the edge width in pixels.      
          
          \param nWidth [in]  The width to set. 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable the edge width. 
        */
        virtual void setEdgeWidth(int nWidth, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeWidth, (OdInt32)nWidth);
          if (bEnableModifier)
            setEdgeModifierFlag(kWidth, bEnableModifier);
        }
        
        /** \details
          Returns the edge width in pixels.
        */
        virtual int edgeWidth() const
        {
          return (int)m_pBase->trait(OdGiVisualStyleProperties::kEdgeWidth)->asInt();
        }

        /** \details
          Sets the amount (in pixels) that edges should overhang their start and end points.       
          
          \param nAmount [in]  The overhang amount. 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable edge overhang.  
        */
        virtual void setOverhangAmount(int nAmount, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeOverhang, (OdInt32)nAmount);
          if (bEnableModifier)
            setEdgeModifierFlag(kOverhang, bEnableModifier);
        }
        
        /** \details
          Returns the edge overhang in pixels.
        */
        virtual int overhangAmount() const
        {
          return (int)m_pBase->trait(OdGiVisualStyleProperties::kEdgeOverhang)->asInt();
        }

        /** \details
          Sets the amount of jitter effect to be applied to jittered edges.       
          
          \param amount [in]  The jitter amount to set. All applicable values are defined in the JitterAmount enumeration. 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable jitter effect.   
        */
        virtual void setJitterAmount(JitterAmount amount, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeJitterAmount, (OdInt32)amount);
          if (bEnableModifier)
            setEdgeModifierFlag(kJitter, bEnableModifier);
        }
        
        /** \details
          Returns the amount of jitter effect as a JitterAmount value.
        */
        virtual JitterAmount jitterAmount() const
        {
          return (JitterAmount)m_pBase->trait(OdGiVisualStyleProperties::kEdgeJitterAmount)->asInt();
        }

        /** \details
          Sets the amount of wiggle effect to be applied to edges.        
          
          \param amount [in]  The wiggle amount to set. All applicable values are defined in the WiggleAmount enumeration. 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable wiggle effect.   
        */
        virtual void setWiggleAmount(WiggleAmount amount, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeWiggleAmount, (OdInt32)amount);
          if (bEnableModifier)
            setEdgeModifierFlag(kWiggle, bEnableModifier);
        }
        
        /** \details
          Returns the amount of wiggle effect as a WiggleAmount value.
        */
        virtual WiggleAmount wiggleAmount() const
        {
          return (WiggleAmount)m_pBase->trait(OdGiVisualStyleProperties::kEdgeWiggleAmount)->asInt();
        }

        /** \details
          Sets the color of silhouette edges.        
          
          \param color [in]  Reference to an OdCmColorBase value to set. 
        */
        virtual void setSilhouetteColor(const OdCmColorBase& color)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeSilhouetteColor, color);
        }
        
        /** \details
          Returns the silhouette color.
        */
        virtual const OdCmColorBase& silhouetteColor() const
        {
          return m_cmSilColor;
        }
        
        /** \details
          Returns the silhouette color.
        */
        virtual OdCmColorBase& silhouetteColor()
        {
          return m_cmSilColor;
        }

        /** \details
          Sets the width of silhouette edges.        
          
          \param nWidth [in]  Width of silhouette edges (in pixels) to set. 
        */
        virtual void setSilhouetteWidth(short nWidth)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeSilhouetteWidth, (OdInt32)nWidth);
        }
        
        /** \details
          Returns the width of silhouette edges in pixels.
        */
        virtual short silhouetteWidth() const
        {
          return (short)m_pBase->trait(OdGiVisualStyleProperties::kEdgeSilhouetteWidth)->asInt();
        }

        /** \details
          Sets the amount of halo gap.        
          
          \param nHaloGap [in]  The halo gap amount to set. Value must be from 0 to 100. 
          \param bEnableModifier [in]  Value that indicates whether to enable or disable halo gap.   
        */
        virtual void setHaloGap(int nHaloGap, bool bEnableModifier)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeHaloGap, (OdInt32)nHaloGap);
          if (bEnableModifier)
            setEdgeModifierFlag(kHaloGap, bEnableModifier);
        }
        
        /** \details
          Returns the halo gap amount.
        */
        virtual int haloGap() const
        {
          return (int)m_pBase->trait(OdGiVisualStyleProperties::kEdgeHaloGap)->asInt();
        }

        /** \details
          Sets the number of isolines.         
          
          \param nIsolines [in]  The number of isolines to set. Value must be between 0 and 2047.
        */
        virtual void setIsolines(unsigned short nIsolines)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeIsolines, (OdInt32)nIsolines);
        }
        
        /** \details
          Returns the number of isolines.
        */
        virtual unsigned short isolines() const
        {
          return (unsigned short)m_pBase->trait(OdGiVisualStyleProperties::kEdgeIsolines)->asInt();
        }

        /** \details
          Sets whether hide precision should be enabled.          
          
          \param bHidePrecision [in]  Value that indicates whether to enable or disable hide precision. 
        */
        virtual void setHidePrecision(bool bHidePrecision)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kEdgeHidePrecision, bHidePrecision);
        }
        
        /** \details
          Returns whether hide precision is enabled or disabled.
        */
        virtual bool hidePrecision() const
        {
          return (unsigned short)m_pBase->trait(OdGiVisualStyleProperties::kEdgeHidePrecision)->asBoolean();
        }

        /** \details
          Sets the type of geometry to which the edge style should be applied.         
          
          \param apply [in]  Value that represents the type of geometry to which the edge styles should be applied.
        */
        virtual void setEdgeStyleApply(EdgeStyleApply apply)
        {
          m_esApply = apply;
        }
        
        /** \details
          Returns the type of geometry to which the edge style will be applied as an EdgeStyleApply value.
        */
        virtual EdgeStyleApply edgeStyleApply() const
        {
          return m_esApply;
        }
    };
    
    /** \details
      <group OdGi_Classes>

      This class defines the container for display style data.
    */
    class OdGiDisplayStyleDataContainer : public OdStaticRxObject<OdGiDisplayStyle>
    {
      protected:
        OdGiVisualStyle           *m_pBase;
      public:
        OdGiDisplayStyleDataContainer()
          : m_pBase(NULL)
        {
        }
        ~OdGiDisplayStyleDataContainer()
        {
        }

        /** \details
          Sets the visual style associated with this object.
          
          \param pBase [in]  Pointer to the visual style object with which this display style data is associated.
        */
        void setBase(OdGiVisualStyle *pBase)
        {
          m_pBase = pBase;
        }
        
        /** \details
          Returns the visual style associated with this object.
        */
        OdGiVisualStyle *base() const
        {
          return m_pBase;
        }

        /** \details
          Sets a collection of bitflags representing the display settings. 
          
          \param nSettings [in]  Unsigned long value that represents a collection of bitwise flags defined in the DisplaySettings enumeration. 
        */
        virtual void setDisplaySettings(unsigned long nSettings)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kDisplayStyles, (OdInt32)nSettings);
        }
        
        /** \details
          Enables or disables the specified display settings flag.  
          
          \param flag [in]  Flag to enable or disable.   
          \param bEnable [in]  Value that indicates whether to enable or disable the specified flag. 
        */
        virtual void setDisplaySettingsFlag(DisplaySettings flag, bool bEnable)
        {
          m_pBase->setTraitFlag(OdGiVisualStyleProperties::kDisplayStyles, flag, bEnable);
        }
        
        /** \details
          Returns the collection of currently used display setting flags.
        */
        virtual unsigned long displaySettings() const
        {
          return (unsigned long)m_pBase->trait(OdGiVisualStyleProperties::kDisplayStyles)->asInt();
        }
        
        /** \details
          Returns whether the specified display settings flag is enabled. 
          
          \param flag [in]  Flag to be checked whether it's enabled or disabled. 
        */
        virtual bool isDisplaySettingsFlagSet(DisplaySettings flag) const
        {
          return m_pBase->traitFlag(OdGiVisualStyleProperties::kDisplayStyles, flag);
        }

        /** \details
          Sets the brightness value.  
          
          \param value [in]  Brightness level to set.
        */
        virtual void setBrightness(double value)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kDisplayBrightness, value);
        }
        
        /** \details
          Returns the brightness level.
        */
        virtual double brightness() const
        {
          return m_pBase->trait(OdGiVisualStyleProperties::kDisplayBrightness)->asDouble();
        }

        /** \details
          Sets the shadow type.  
          
          \param type [in]  Shadow type to set. All available types are defined in the ShadowType enumeration. 
        */
        virtual void setShadowType(ShadowType type)
        {
          m_pBase->setTrait(OdGiVisualStyleProperties::kDisplayShadowType, (OdInt32)type);
        }
        
        /** \details
          Returns the currently used shadow type.
        */
        virtual ShadowType shadowType() const
        {
          return (ShadowType)m_pBase->trait(OdGiVisualStyleProperties::kDisplayShadowType)->asInt();
        }
    };
  protected:
    OdGiFaceStyleDataContainer    m_faceStyle;
    OdGiEdgeStyleDataContainer    m_edgeStyle;
    OdGiDisplayStyleDataContainer m_displayStyle;
    Type                          m_type;
    OdStaticRxObject<OdGiVariant> m_props[OdGiVisualStyleProperties::kPropertyCount];
    Operation                     m_ops[OdGiVisualStyleProperties::kPropertyCount];
  public:
    OdGiVisualStyleDataContainer()
      : m_faceStyle()
      , m_edgeStyle()
      , m_displayStyle()
      , m_type(kCustom)
    {
      // Setup redirections
      m_faceStyle.setBase(this);
      m_edgeStyle.setBase(this);
      m_displayStyle.setBase(this);
      // Face properties
      m_props[OdGiVisualStyleProperties::kFaceLightingModel].set((OdInt32)OdGiVisualStyleProperties::kPhong);
      m_props[OdGiVisualStyleProperties::kFaceLightingQuality].set((OdInt32)OdGiVisualStyleProperties::kPerVertexLighting);
      m_props[OdGiVisualStyleProperties::kFaceColorMode].set((OdInt32)OdGiVisualStyleProperties::kNoColorMode);
      m_props[OdGiVisualStyleProperties::kFaceModifiers].set((OdInt32)OdGiVisualStyleProperties::kNoFaceModifiers);
      m_props[OdGiVisualStyleProperties::kFaceOpacity].set(0.6);
      m_props[OdGiVisualStyleProperties::kFaceSpecular].set(30.0);
      m_props[OdGiVisualStyleProperties::kFaceMonoColor].set(OdCmEntityColor(255, 255, 255));
      // Edge properties
      m_props[OdGiVisualStyleProperties::kEdgeModel].set((OdInt32)OdGiVisualStyleProperties::kIsolines);
      m_props[OdGiVisualStyleProperties::kEdgeStyles].set((OdInt32)OdGiVisualStyleProperties::kObscuredFlag);
      m_props[OdGiVisualStyleProperties::kEdgeIntersectionColor].set(OdCmEntityColor(OdCmEntityColor::kForeground));
      m_props[OdGiVisualStyleProperties::kEdgeObscuredColor].set(OdCmEntityColor(OdCmEntityColor::kNone));
      m_props[OdGiVisualStyleProperties::kEdgeObscuredLinePattern].set((OdInt32)OdGiVisualStyleProperties::kSolid);
      m_props[OdGiVisualStyleProperties::kEdgeIntersectionLinePattern].set((OdInt32)OdGiVisualStyleProperties::kSolid);
      m_props[OdGiVisualStyleProperties::kEdgeCreaseAngle].set(1.0);
      m_props[OdGiVisualStyleProperties::kEdgeModifiers].set((OdInt32)OdGiVisualStyleProperties::kEdgeColorFlag);
      m_props[OdGiVisualStyleProperties::kEdgeColor].set(OdCmEntityColor(OdCmEntityColor::kForeground));
      m_props[OdGiVisualStyleProperties::kEdgeOpacity].set(1.0);
      m_props[OdGiVisualStyleProperties::kEdgeWidth].set((OdInt32)1);
      m_props[OdGiVisualStyleProperties::kEdgeOverhang].set((OdInt32)6);
      m_props[OdGiVisualStyleProperties::kEdgeJitterAmount].set((OdInt32)OdGiVisualStyleProperties::kJitterMedium);
      m_props[OdGiVisualStyleProperties::kEdgeSilhouetteColor].set(OdCmEntityColor(OdCmEntityColor::kForeground));
      m_props[OdGiVisualStyleProperties::kEdgeSilhouetteWidth].set((OdInt32)5);
      m_props[OdGiVisualStyleProperties::kEdgeHaloGap].set((OdInt32)0);
      m_props[OdGiVisualStyleProperties::kEdgeIsolines].set((OdInt32)0);
      m_props[OdGiVisualStyleProperties::kEdgeHidePrecision].set(false);
      // Display properties
      m_props[OdGiVisualStyleProperties::kDisplayStyles].set((OdInt32)OdGiVisualStyleProperties::kBackgroundsFlag);
      m_props[OdGiVisualStyleProperties::kDisplayBrightness].set(0.0);
      m_props[OdGiVisualStyleProperties::kDisplayShadowType].set((OdInt32)OdGiVisualStyleProperties::kShadowsNone);
      // New in AC2011, 2013
      m_props[OdGiVisualStyleProperties::kUseDrawOrder].set(false);
      m_props[OdGiVisualStyleProperties::kViewportTransparency].set(true);
      m_props[OdGiVisualStyleProperties::kLightingEnabled].set(true);
      m_props[OdGiVisualStyleProperties::kPosterizeEffect].set(false);
      m_props[OdGiVisualStyleProperties::kMonoEffect].set(false);
      // New in 2013
      m_props[OdGiVisualStyleProperties::kBlurEffect].set(false);
      m_props[OdGiVisualStyleProperties::kPencilEffect].set(false);
      m_props[OdGiVisualStyleProperties::kBloomEffect].set(false);
      m_props[OdGiVisualStyleProperties::kPastelEffect].set(false);
      m_props[OdGiVisualStyleProperties::kBlurAmount].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kPencilAngle].set(0.0);
      m_props[OdGiVisualStyleProperties::kPencilScale].set(1.0);
      m_props[OdGiVisualStyleProperties::kPencilPattern].set((OdInt32)0);
      m_props[OdGiVisualStyleProperties::kPencilColor].set(OdCmEntityColor(0, 0, 0));
      m_props[OdGiVisualStyleProperties::kBloomThreshold].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kBloomRadius].set((OdInt32)3);
      m_props[OdGiVisualStyleProperties::kTintColor].set(OdCmEntityColor(0, 0, 255));
      m_props[OdGiVisualStyleProperties::kFaceAdjustment].set(false);
      m_props[OdGiVisualStyleProperties::kPostContrast].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kPostBrightness].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kPostPower].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kTintEffect].set(false);
      m_props[OdGiVisualStyleProperties::kBloomIntensity].set((OdInt32)50);
      m_props[OdGiVisualStyleProperties::kColor].set(OdCmEntityColor(OdCmEntityColor::kByLayer));
      m_props[OdGiVisualStyleProperties::kTransparency].set(1.0);
      m_props[OdGiVisualStyleProperties::kEdgeWiggleAmount].set((OdInt32)OdGiVisualStyleProperties::kWiggleMedium);
      m_props[OdGiVisualStyleProperties::kEdgeTexturePath].set(OdString(OD_T("strokes_ogs.tif")));
      m_props[OdGiVisualStyleProperties::kDepthOfField].set(false);
      m_props[OdGiVisualStyleProperties::kFocusDistance].set(1.0);
      m_props[OdGiVisualStyleProperties::kFocusWidth].set(1.0);
    }
    ~OdGiVisualStyleDataContainer()
    {
    }

    /** \details
      Returns a reference to OdGiFaceStyle of this visual style.
    */
    virtual OdGiFaceStyle& faceStyle()
    {
      return m_faceStyle;
    }
    
    /** \details
      Returns a reference to OdGiEdgeStyle of this visual style.
    */
    virtual OdGiEdgeStyle& edgeStyle()
    {
      return m_edgeStyle;
    }
    
    /** \details
      Returns a reference to OdGiDisplayStyle of this visual style.
    */
    virtual OdGiDisplayStyle& displayStyle()
    {
      return m_displayStyle;
    }

    /** \details
      Returns a reference to OdGiFaceStyle of this visual style.
    */
    virtual const OdGiFaceStyle& faceStyle() const
    {
      return m_faceStyle;
    }
    
    /** \details
      Returns a reference to OdGiEdgeStyle of this visual style.
    */
    virtual const OdGiEdgeStyle& edgeStyle() const
    {
      return m_edgeStyle;
    }
    
    /** \details
      Returns a reference to OdGiDisplayStyle of this visual style.
    */
    virtual const OdGiDisplayStyle& displayStyle() const
    {
      return m_displayStyle;
    }

    /** \details
      Copies all properties of OdGiFaceStyle into this visual style.  
      
      \param style [in]  Collection of face properties. 
    */
    virtual void setFaceStyle(const OdGiFaceStyle& style)
    {
      m_faceStyle.set(style);
    }
    
    /** \details
      Copies all properties of OdGiEdgeStyle into this visual style.  
      
      \param style [in]  Collection of edge properties.  
    */
    virtual void setEdgeStyle(const OdGiEdgeStyle& style)
    {
      m_edgeStyle.set(style);
    }
    
    /** \details
      Copies all properties of OdGiDisplayStyle into this visual style.  
      
      \param style [in]  Collection of display properties.  
    */
    virtual void setDisplayStyle(const OdGiDisplayStyle& style)
    {
      m_displayStyle.set(style);
    }

    // New interface

    /** \details
      Sets the given visual style type. Multiple properties are modified to change the visual style into the specified type.   
      
      \param type [in]  Type of visual style to set. All available types are defined in the Type enumeration. 
      
      \returns
      Returns true if successful, otherwise false.
    */
    virtual bool setType(Type type)
    {
      try
      {
        configureForType(type);
      }
      catch (const OdError &)
      {
        return false;
      }
      m_type = type;
      return true;
    }
    
    /** \details
      Returns the current visual style type.
    */
    virtual Type type() const
    {
      return m_type;
    }

    /** \details
      Sets the operation associated with the given property.    
      
      \param prop [in]  Property for which the operation is to be set.
      \param op [in]  Operation to use for setting the property.  
      
      \returns
      Returns true if successful, otherwise false.
    */
    virtual bool setTrait(Property prop, Operation op)
    {
      if ((prop > OdGiVisualStyleProperties::kInvalidProperty) && (prop < OdGiVisualStyleProperties::kPropertyCount))
      {
        m_ops[prop] = op;
        return true;
      }
      return false;
    }
    
    /** \details
      Sets a property of the visual style.    
      
      \param prop [in]  Property to set.
      \param pVal [in]  Pointer to the OdGiVariant property value to set. 
      \param op [in]  Operation to use for setting the property.  
      
      \returns
      Returns true if successful, otherwise false.
    */
    virtual bool setTrait(Property prop, const OdGiVariant *pVal,
                          Operation op = OdGiVisualStyleOperations::kSet)
    {
      if ((prop > OdGiVisualStyleProperties::kInvalidProperty) && (prop < OdGiVisualStyleProperties::kPropertyCount) &&
          pVal && (pVal->type() == propertyType(prop)))
      {
        static_cast<OdGiVariant&>(m_props[prop]) = *pVal;
        m_ops[prop] = op;
        return true;
      }
      return false;
    }

    /** \details
      Gets a property of the visual style.     
      
      \param prop [in]  Property to get.
      \param pOp [in]  Pointer to the operation used for the property. If NULL, nothing is returned. 
      
      \returns
      Returns an OdGiVariantPtr for the property value if successful, otherwise an empty OdGiVariantPtr object.
    */
    virtual OdGiVariantPtr trait(Property prop, Operation *pOp = NULL) const
    {
      if ((prop > OdGiVisualStyleProperties::kInvalidProperty) && (prop < OdGiVisualStyleProperties::kPropertyCount))
      {
        if (pOp)
          *pOp = m_ops[prop];
        return OdGiVariantPtr(m_props + prop);
      }
      return OdGiVariant::createObject();
    }
    
    /** \details
      Gets the operation associated with the given property of the visual style.  
      
      \param prop [in]  Property to get.
      
      \returns
      Returns the operation associated with the specified property if successful, otherwise the kInvalidOperation value.
    */
    virtual Operation operation(Property prop) const
    {
      if ((prop > OdGiVisualStyleProperties::kInvalidProperty) && (prop < OdGiVisualStyleProperties::kPropertyCount))
      {
        return m_ops[prop];
      }
      return OdGiVisualStyleOperations::kInvalidOperation;
    }
};

#include "TD_PackPop.h"

#endif //__ODGIVISUALSTYLEDATA_H__
