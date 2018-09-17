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



#ifndef OD_AVEMATERIAL_H
#define OD_AVEMATERIAL_H

#include "TD_PackPush.h"

#include "Ve/AveDefs.h"
#include "RxObject.h"
#include "DbEntity.h"
#include "IdArrays.h"

class OdDbObjectId;
class OdDbDatabase;
class OdAveMaterialImpl;
class OdAveMaterial;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdAveMaterial object pointers.
*/
typedef OdSmartPtr<OdAveMaterial> OdAveMaterialPtr;

/** \details
    This class represents AVE Material properties for shaded entities.

    \remarks
    AVE Material properties may be attached to OdDbEntity and
    OdDbLayerTableRecord objects, and to color indicies (ACIs).
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveMaterial : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdAveMaterial);

  /** \details
    Creates a Material object from the specified OdDbBlockReference.
    
    \param blockRefId [in]  Object ID of the block reference.
    
    \remarks
    The new Material object is attached to the specified block reference.
     
    Returns a SmartPointer to the new Material object.
  */
  static OdAveMaterialPtr getMaterial(const OdDbObjectId& blockRefId);

  /** \details
    Returns the Material name from the specified OdDbBlockReference.
    
    \param blockRefId [in]  Object ID of the block reference.
  */
  static OdString getMaterialName(const OdDbObjectId& blockRefId);

  /** \details
    Saves the Material properties to the OdDbBlockReference to which this Material object is associated.

    \param pDb [in]  Pointer to the database.
      
    \remarks
    If pDb is null, the database containing this Material object is used.
  */
  OdResult writeMaterial(OdDbDatabase* pDb = NULL, const OdDbHandle& holderHandle = 0);

  /** \details
    Attaches this Material object to the specified OdDbEntity.
    \param entityId [in]  Object ID of the entity.
    \returns
    Returns true if and only if successful.
  */
  bool attachToObject(OdDbObjectId entityId);

  /** \details
    Attaches this Material object to the specified OdDbLayerTableRecord.
    \param layerId [in]  Object ID of the layer table record.
    \returns
    Returns true if and only if successful.
  */
  bool attachToLayer(OdDbObjectId layerId);

  /** \details
    Attaches this Material object to the specified color index (ACI).

    \param colorIndex [in]  Color index. [1..255]
    
    \returns
    Returns true if and only if successful.

    \remarks
    The attachment is made in the database of the holder of this Material Object.
    \sa
    OdCmEntityColor
  */
  bool attachToACI(OdUInt16 colorIndex);
  
  /** \details
    Detaches this Material object from the specified OdDbEntity.
    \param entityId [in]  Object ID of the entity.
  */
  static void detachFromObject(OdDbObjectId entityId);

  /** \details
    Detaches this Material object from the specified OdDbLayerTableRecord.
    \param layerId [in]  Object ID of the layer table record.
  */
  static void detachFromLayer(OdDbObjectId layerId);

  /** \details
    Detaches this Material object from the specified color index (ACI) for the specified database.

    \param colorIndex [in]  Color index. [1..255]
    \param pDb [in]  Pointer to the database for the ACI.
      
    \remarks
    \sa
    OdCmEntityColor
  */
  static void detachFromACI(OdDbDatabase* pDb, OdUInt16 colorIndex);
  
  /** \details
    Returns the object ID of the OdDbBlockReference associated with the material to which 
    the specified OdDbEntity or OdDbLayerTableRecord is attached.

    \param sourceId [in]  Object ID of the entity or layer table record.
    \param blockRefId [out]  Object ID of OdDbBlockReference associated with the material.

    \returns
    Returns true if and only if a material is attached.
  */
  static bool getAttachedMaterial(const OdDbObjectId& sourceId, OdDbObjectId& blockRefId);
  
  /** \details
    Returns the object ID of the OdDbBlockReference associated with the material to which 
    the specified color index (ACI) is attached.

    \param colorIndex [in]  Color index. [1..255]
    \param blockRefId [out]  Object ID of OdDbBlockReference associated with the material.
    \param pDb [in]  Pointer to the database of the ACI.
    \returns
    Returns true if and only if a material is attached.
  */
  static bool getAttachedMaterial(OdDbDatabase* pDb, OdUInt16 colorIndex, OdDbObjectId& blockRefId);

  /** \details
    Returns the ACI Material Map for the specified OdDbDatabase. 

    \param blockRefIds [out]  Receives an array[255] of block reference object IDs.
    \param pDb [in]  Pointer to the database for the ACI.
    
    \remarks
    The Object ID for ACI==n is in blockRefIds[n-1],
    
    \remarks
    Returns an empty array if not successful.
    
    blockRefIds are the object IDs of the OdDbBlockReferences associated with the materials.
  */
  static void getAciMaterialMap(OdDbDatabase* pDb, OdDbObjectIdArray& blockRefIds);

  /** \details
    Returns the type of this Material object.
    \remarks
    type() returns one of the following:
    
    <table>
    Name          Value
    kStandard     0
    kGranite      1
    kMarble       2
    kWood         3
    </table>

  */
  virtual OdAve::MaterialType type();

  /** \details
    Returns true if and only if any of the specified Material attributes
    are used in this Material object.
    \param attributes [in]  Material attributes.
    \remarks
    attributes must be a combination of one or more of the following:

    <table>
    Name              Value
    kAmbient          0x1  
    kDiffuse          0x2  
    kSpecular         0x4  
    kTransparency     0x8  
    kRoughness        0x20 
    kBumpMap          0x40 
    kReflectionProp   0x100
    kRefraction       0x200
    </table>
    
  */
  bool isAttribUsed(OdAve::MaterialAttrib attributes);

  /** \details
    Returns the name of this Material object.
    
  */
  OdString name() const;
  /** \details
    Returns the description of this Material object.
  */
  OdString description() const;

  /** \details
    Sets the name of this Material object.
    \param name [in]  Name.
    
    \remarks
    Names are truncated to 16 characters.
  */
  void setName(const OdString& name);

  /** \details
    Sets the description of this Material object.
    \param description [in]  Description.
  */
  void setDescription(const OdString& description);

  /** \details
    Returns the ambient (shadow) factor of this Material object.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].

  */
  double ambientFactor() const;

  /** \details
    Returns the ambient (shadow) color of this Material object.
    
    \remarks
    ambientColor() returns a 24-bit RGB color.

  */
  OdUInt32 ambientColor() const;
  /** \details
    Returns true if and only if the ambient (shadow) color for this Material object
    is the current entity color, CECOLOR.
  */
  bool ambientColorCurrent() const;

  /** \details
    Returns true if and only if the ambient lock flag has been set for this Material object.
  */
  bool ambientLock() const;

  /** \details
    Sets the ambient (shadow) factor of this Material object.
    
    \param ambientFactor [in]  Ambient factor. [0.0 .. 1.0]
  */
  void setAmbientFactor(double ambientFactor);

  /** \details
    Sets the ambient (shadow) color of this Material object.
    
    \param ambientColor [in]  24-bit RGB color.
  */
  void setAmbientColor(OdUInt32 ambientColor);
  /** \details
    Controls the use of the current entity color, CECOLOR,
    as the ambient (shadow) color of this Material object.
    \param currentColor [in]  Controls the use of CECOLOR.
  */
  void setAmbientColorCurrent(bool currentColor);
  /** \details
    Controls the ambient lock flag of this Material object.
    \param ambientLock [in]  Controls ambient lock.
  */
  void setAmbientLock(bool ambientLock);

 
  /** \details
    Returns the diffuse (main) factor of this Material object.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].

  */
  double diffuseFactor() const;

  /** \details
    Returns the diffuse (main) color of this Material object.
    
    \remarks
    diffuseColor() returns a 24-bit RGB color.
  */
  OdUInt32 diffuseColor() const;
  
  /** \details
    Returns true if and only if the diffuse (main) color for this Material object
    is the current entity color, CECOLOR.

  */
  bool diffuseColorCurrent() const;
  
  
  /** \details
    Returns a reference to the diffuse (main) bitmap for this Material object.

  */
  OdAveBitmap& diffuseBitmap() const;

  /** \details
    Sets the diffuse (main) factor of this Material object.
    

    diffuseFactor () Diffuse factor. [0.0 .. 1.0]

  */
  void setDiffuseFactor(double diffuseFactor);
  /** \details
    Sets the diffuse (main) color of this Material object.
    
    \param diffuseColor [in]  24-bit RGB color.
  */
  void setDiffuseColor(OdUInt32 diffuseColor);

  /** \details
    Controls the use of the current entity color, CECOLOR,
    as the diffuse (main) color of this Material object.
    \param currentColor [in]  Controls the use of CECOLOR.

  */
  void setDiffuseColorCurrent(bool currentColor);

  /** \details
    Returns the specular (reflection) factor of this Material object.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].

  */
  double specularFactor() const;
 
  /** \details
    Returns the specular (reflection) color of this Material object.
    
    \remarks
    specularColor() returns a 24-bit RGB color.

  */
  OdUInt32 specularColor() const;
  /** \details
    Returns true if and only if the specular (reflection) color for this Material object
    is the current entity color, CECOLOR.

  */
  bool specularColorCurrent() const;
  /** \details
    Returns true if and only if specular reflections are to be mirrored for this Material object.

  */
  bool specularMirror() const;
  
  /** \details
    Returns true if and only if the specular lock flag has been set for this Material object.

  */
  bool specularLock() const;
  
  /** \details
    Returns a reference to, or a copy of, the specular (reflection) bitmap of this Material object.
  */
  OdAveBitmap& specularBitmap();
  const OdAveBitmap& specularBitmap() const;


  /** \details
    Sets the specular (reflection) factor of this Material object.
    
    \param specularFactor [in]  Specular factor. [0.0 .. 1.0]

  */
  void setSpecularFactor(double specularFactor);
  /** \details
    Sets the specular (reflection) color of this Material object.
    
    \param specularColor [in]  24-bit RGB color.

  */
  void setSpecularColor(OdUInt32 specularColor);

  /** \details
    Controls the use of the current entity color, CECOLOR,
    as the specular (reflection) color of this Material object.
    \param currentColor [in]  Controls the use of CECOLOR.

  */
  void setSpecularColorCurrent(bool colorCurrrent);
  /** \details
    Controls the mirroring of specular reflections for this Material object.
    \param mirror [in]  Controls mirroring.

  */
  void setSpecularMirror(bool mirror);

  /** \details
    Controls the specular lock flag of this Material object.
    \param specularLock [in]  Controls specular lock

    \remarks
    The specular component is viewpoint dependent, and most apparent when the entity is highlighted.
  */
  void setSpecularLock(bool specularLock);

  /** \details
    Returns the roughness of this Material.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].
    \remarks    
    Roughness controls the size of the material's reflective highlight.
    The higher the level of roughness, the greater the highlight. 
  */
  double roughness() const;
  /** \details
    Sets the roughness of this Material object.
    
    \param roughness [in]  Roughness. [0.0 .. 1.0]
    
    \remarks
    Roughness controls the size of the material's reflective highlight.
    The higher the level of roughness, the greater the highlight. 
  */
  void setRoughness(double);

  /** \details
    Returns the transparency of this Material object.
    
    \remarks
    <table>
    Value      Renders
    0.0        Opaque.
    1.0        Totally transparent.
    </table>
    
  */
  double transparency() const;
  /** \details
    Returns a reference to the transparency bitmap of this Material object.
  */
  OdAveBitmap& transparencyBitmap() const;
  /** \details
    Returns the transparency of this Material object.
    
    \param transparency [in]  Transparency. [0.0 .. 1.0]
    \returns
    Returns a value in the r1ange [0.0 .. 1.0].
    
    <table>
    Value      Renders
    0.0        Opaque.
    1.0        Totally transparent.
    </table>
  */
  void setTransparency(double transparency);

  /** \details
    Returns the refraction index of this Material object.
    
  */
  double refraction() const;

  /** \details
    Sets the refraction index of this Material object.
    
    \param refraction [in]  Refraction.
    
  */
  void setRefraction(double refraction) const;

  /** \details
    Returns a reference to, or a copy of, the bump bitmap of this Material object.
    
    \remarks
    The bump bitmap may be used as an opacity map for this Material object.
  */
  OdAveBitmap& bumpMap();
  const OdAveBitmap& bumpMap() const;

  /** \details
    Returns the Object ID of the OdDbBlockReference associated with this Material object.
  */
  OdDbObjectId holderId() const;

  /** \details
  Returns the handle of the DbMaterial to which has been converted AveMaterial in A2008.
  */
  OdString handleDbMaterial() const;

  /** \details
  Sets the DbMaterial handle of this material in A2008.
  */
  void setHandleDbMaterial(const OdString& sDesc);

  ~OdAveMaterial();
protected:
  OdAveMaterial();
  OdAveMaterial(OdAveMaterialImpl* pImpl);
  
  OdAveMaterialImpl* m_pImpl;
  friend class OdAveMaterialImpl;
};

/** \details
    This class represents AVE Granite Material properties for shaded entities.
    
    \remarks
    AVE Granite Materials consist of a random distribution of four colors.
    Corresponding C++ library: TD_Ave

    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveGraniteMaterial : public OdAveMaterial
{
public:
  ODRX_DECLARE_MEMBERS(OdAveGraniteMaterial);

  /** \details
    Defines the color index for each of the four colors
    that are present in a Granite Material object.
  */
  enum GraniteColor
  {
    kFirst    = 0,
    kSecond   = 1,
    kThird    = 2,
    kFourth   = 3
  };

  virtual OdAve::MaterialType type();

  /** \details
    Returns the color factor for the specified granite color of this Granite Material object.
   
    \param graniteColor [in]  Granite color.
    
    \returns
    Returns a value in the range [0.0 .. 1.0]. 
    \remarks
    There are four colors associated with a Granite Material object. 
    The color factors determine the relative amounts of the four granite colors.
    
    graniteColor must be one of the following:
    
    <table>
    Name      Value
    kFirst    0
    kSecond   1
    kThird    2
    kFourth   3
    </table>
    
  */
  double colorFactor(GraniteColor graniteColor);

  /** \details
    Returns the color for the specified granite color of this Granite Material object.
   
    \param graniteColor [in]  Granite color.
    
    \remarks
    color() returns a 24-bit RGB color.
    
    graniteColor must be one of the following:
    
    <table>
    Name      Value
    kFirst    0
    kSecond   1
    kThird    2
    kFourth   3
    </table>
  */
  OdUInt32 color(GraniteColor graniteColor);

  /** \details
    Sets the color factor for the specified granite color of this Granite Material object.
   
    \param graniteColor [in]  Granite color.
    \param colorFactor [in]  Color factor. [0.0 .. 1.0]
    
    \remarks
    There are four colors associated with a Granite Material object. 
    The color factors determine the relative amounts of the four granite colors.

    graniteColor must be one of the following:
    
    <table>
    Name      Value
    kFirst    0
    kSecond   1
    kThird    2
    kFourth   3
    </table>
    
    There are four colors associated with a Granite Material object. 
    To use less than four materials, make two or more of them the same,
    or set one or more of them to 0.
  */
  void setColorFactor(GraniteColor graniteColor, double colorFactor);

  /** \details
    Sets the color for the specified granite color for this Granite Material object.
   
    \param graniteColor [in]  Granite color.
    \param color [in]  24-bit RGB color.
    
    \remarks
    graniteColor must be one of the following:
    
    <table>
    Name      Value
    kFirst    0
    kSecond   1
    kThird    2
    kFourth   3
    </table>
    
    There are four colors associated with a Granite Material object. 
    To use less than four materials, make two or more of them the same,
    or set one or more of them to 0.
  */
  void setColor(GraniteColor graniteColor, OdUInt32 color);

  /** \details
    Returns the sharpness of this Material object.

    \returns
    Returns a value in the range [0.0 .. 1.0]

    <table>
    Value      Renders
    0.0        Colors are melded.
    1.0        Colors are discrete.
    </table>
  */
  double sharpness();
  /** \details
    Sets the sharpness of this Granite Material object.

    \param sharpness [in]  Sharpness. [0.0 .. 1.0]
 
    \remarks
    <table>
    Value      Renders
    0.0        Colors are melded.
    1.0        Colors are discrete.
    </table>
  */
  void setSharpness(double sharpness);

  /** \details
    Returns the scale of this Granite Material object.
    
    \remarks
    The scale is with respect to the objects to which this Granite Material object
    are attached. Larger values result in a coarser texture with more graininess.
  */
  double scale();
  /** \details
    Sets the scale of this Granite Material object.
    
    \param scale [in]  Scale.
    
    \remarks
    The scale is with respect to the objects to which this Granite Material object
    are attached. Larger values result in a coarser texture with more graininess.
  */
  void setScale(double scale);

protected:
  OdAveGraniteMaterial();
  OdAveGraniteMaterial(OdAveMaterialImpl* pImpl);
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdAveGraniteMaterial object pointers.
*/
typedef OdSmartPtr<OdAveGraniteMaterial> OdAveGraniteMaterialPtr;

/** \details
    This class represents AVE Marble Material properties for shaded entities.
    
    \remarks
    AVE Marble Materials consist of a stone color and a vein color. 
    Corresponding C++ library: TD_Ave

    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveMarbleMaterial : public OdAveMaterial
{
public:
  ODRX_DECLARE_MEMBERS(OdAveMarbleMaterial);

  virtual OdAve::MaterialType type();

  /** \details
    Accesses the stone color for this Marble Material object.

    \remarks
    stoneColor() returns a 24-bit RGB color.
  */
  OdUInt32 stoneColor();
  /** \details
    Accesses the vein color for this Marble Material object.
   
    \returns
    Returns a 24-bit RGB color.
  */
  OdUInt32 veinColor();
  
  /** \details
    Accesses the stone color for this Marble Material object.

    \param color [in]  24-bit RGB color.
  */
  void stoneColor(OdUInt32 color);
  
  /** \details
    Accesses the vein color for this Marble Material object.
    
    \param color [in]  24-bit RGB color.
  */
  void veinColor(OdUInt32 color);

  /** \details
    Returns the vein turbulance of this Marble Material object. 
    
    \remarks
    Higher values result in more swirling of the veins,
    and take longer to render. Values in the range [1..10] are recommended.
  */
  OdUInt32 turbulence();
  /** \details
    Sets the vein turbulance of this Marble Material object. 
    \param turbulence [in]  Turbulence.
    \remarks
    Higher values result in more swirling of the veins,
    and take longer to render. Values in the range [1..10] are recommended.
  */
  void setTurbulence(OdUInt32 turbulence);

  /** \details
    Returns the sharpness of this Marble Material object.

    \returns
    Returns a value in the range [0.0 .. 1.0]
 
    <table>
    Value      Renders
    0.0        Colors are melded.
    1.0        Colors are discrete.
    </table>
  */
  double sharpness();
  /** \details
    Sets the sharpness of this Marble Material object.

    \param sharpness [in]  Sharpness. [0.0 .. 1.0]
 
    <table>
    Value      Renders
    0.0        Colors are melded.
    1.0        Colors are discrete.
    </table>
  */
  void setSharpness(double sharpness);


  /** \details
    Returns the scale of this Marble Material object.
    
    \remarks
    The scale is with respect to the objects to which this Marble Material object
    are attached. Smaller values result in less veins.
  */
  double scale();
  
  /** \details
    Sets the scale of this Marble Material object.
        
    \param scale [in]  Scale.
    
    \remarks
    The scale is with respect to the objects to which this Marble Material object
    are attached. Smaller values result in less veins.
  */
  void setScale(double scale);

protected:
  OdAveMarbleMaterial();
  OdAveMarbleMaterial(OdAveMaterialImpl* pImpl);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdAveMarbleMaterial object pointers.
*/
typedef OdSmartPtr<OdAveMarbleMaterial> OdAveMarbleMaterialPtr;

/** \details
    This class represents Wood Material properties for shaded entities.
    \remarks
    Wood materials consist of a bicolor woodgrain pattern.
    
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveWoodMaterial : public OdAveMaterial
{
public:
  ODRX_DECLARE_MEMBERS(OdAveWoodMaterial);

  virtual OdAve::MaterialType type();

  /** \details
    Returns the light color of this Material object.
    \remarks
    lightColor() returns a 24-bit RGB color.
  */
  OdUInt32 lightColor();
  /** \details
    Returns the dark color of this Wood Material object.
    \remarks
    darkColor() returns a 24-bit RGB color.
  */
  OdUInt32 darkColor();
  /** \details
    Sets the light color of this Wood Material object.
    \param lightColor [in]  24-bit RGB color.
  */
  void setLightColor(OdUInt32 lightColor);
  /** \details
    Sets the dark color of this Wood Material object.
    \param darkColor [in]  24-bit RGB color.
  */
  void setDarkColor(OdUInt32 darkColor);

  /** \details
    Returns the mixture of light vs. dark rings in this Wood Material object.
    \returns
    Returns a value in the range [0.0 .. 1.0].
    
    <table>
    Value      Renders
    0.0        Mostly dark.
    1.0        Mostly light.
    </table>
    
  */
  double lightDark();
  /** \details
    Sets the mixture of light vs. dark rings in this Wood Material object.
    \param lightDark [in]  Light/Dark mixture.  [0.0 .. 1.0]
    
    <table>
    Value      Renders
    0.0        Mostly dark.
    1.0        Mostly light.
    </table>
  */
  void setLightDark(double lightDark);

  /** \details
    Returns the ring density of this Wood Material object.
    
    \remarks
    The ring density is with respect to the objects to which this Wood Material object
    are attached. Smaller values result in a coarser texture with less rings.
  */
  double ringDensity();
  /** \details
    Sets the ring density of this Wood Material object.
    \param ringDensity [in]  Ring Density. 
    \remarks
    The ring density is with respect to the objects to which this Wood Material object
    are attached. Smaller values result in a coarser texture with less rings.
  */
  void setRingDensity(double ringDensity);

  /** \details
    Returns the ring width variation for this Wood Material object.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].

    <table>
    Value      Renders
    0.0        Rings are uniform.
    1.0        Rings have greatest variation.
    </table>

  */
  double ringWidth();
  /** \details
    Sets the ring width parameter for this Wood Material object.
    
    \param ringWidth [in]  Ring width parameter. [0.0 .. 1.0]
    \remarks

    <table>
    Value      Renders
    0.0        Rings are uniform.
    1.0        Rings have greatest variation.
    </table>
  */
  void setRingWidth(double ringWidth);

  /** \details
    Returns the ring shape parameters for this Wood Material object.
    
    \returns
    Returns a value in the range [0.0 .. 1.0].

    <table>
    Value      Renders
    0.0        Circular rings.
    1.0        Most irregular rings.
    </table>
  */
   double ringShape();
  /** \details
    Sets the ring shape parameters for this Wood Material object.
    
    \param ringShape [in]  Ring shape parameter. [0.0 .. 1.0]
    \remarks
    
    <table>
    Value      Renders
    0.0        Circular rings.
    1.0        Most irregular rings.F
    </table>
  */
   void setRingShape(double ringShape) ;

  /** \details
    Returns the scale of this Wood Material object.
    
    \remarks
    The scale is with respect to the objects to which this Wood Material object
    are attached. Smaller values result in a finer texture with more rings.
  */
  double scale();
  
  /** \details
    Sets the scale of this Wood Material object.
        
    \param scale [in]  Scale.
    
    \remarks
    The scale is with respect to the objects to which this Wood Material object
    are attached. Smaller values result in a finer texture with rings.
  */
  void setScale(double scale);

protected:
  OdAveWoodMaterial();
  OdAveWoodMaterial(OdAveMaterialImpl* pImpl);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdAveWoodMaterial object pointers.
*/
typedef OdSmartPtr<OdAveWoodMaterial> OdAveWoodMaterialPtr;

/** \details
  Returns an Iterator object that can be 
  used to traverse the OdAveMaterial objects in the specified database or material library.
  
  \param pDb [in]  Pointer to the database.
  \param pStreamBuf [in]  Pointer to the stream buffer.
*/
AVE_EXPORT OdRxIteratorPtr oddbGetMaterials(OdDbDatabase* pDb);

AVE_EXPORT void oddbGetMaterialIds(OdDbDatabase* pDb, OdDbObjectIdArray& ids);

AVE_EXPORT OdRxIteratorPtr oddbGetMaterials(OdStreamBuf* pStreamBuf);

#include "TD_PackPop.h"

#endif // OD_AVEMATERIAL_H

