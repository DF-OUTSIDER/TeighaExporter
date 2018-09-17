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



#ifndef OD_AVELIGHT_H
#define OD_AVELIGHT_H

#include "TD_PackPush.h"

#include "RxObject.h"
#include "DbObjectId.h"
#include "IdArrays.h"
#include "Ve/AveDefs.h"
#include "Gs/GsLight.h"

class OdGePoint3d;
class OdGeVector3d;

class OdAveLightImpl;
class OdAveLight;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdAveLight object pointers.
*/
typedef OdSmartPtr<OdAveLight> OdAveLightPtr;


/** \details
    This class represents AVE Light properties.

    \remarks
    AVE Light properties may be attached to OdDbBlockReference objects.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveLight : public OdGsLight
{
public:
  ODRX_DECLARE_MEMBERS(OdAveLight);

  /** \details
    Creates a Light object from the specified OdDbBlockReference.
    
    \param blockRefId [in]  Object ID of the block reference.
    
    \remarks
    The new Light object is associated with the specified block reference.
     
    Returns a SmartPointer to the new Light object.
  */
  static OdAveLightPtr getLight(const OdDbObjectId& blockRefId);
  /** \details
    Returns the Light name from the specified OdDbBlockReference.
    
    \param blockRefId [in]  Object ID of the block reference.
  */
  static OdString getLightName(const OdDbObjectId& blockRefId);

  /** \details
    Saves the Light properties to the OdDbBlockReference to which this Light object is associated.

    \param pDb [in]  Pointer to the database.
      
    \remarks
    If pDb is null, the database containing this Light object is used.
  */
  OdResult writeLight(OdDbDatabase* pDb = 0, const OdDbHandle& holderHandle = 0, const OdDbObjectId& ownerId = OdDbObjectId::kNull);

  /** \details
    Returns the type of this Light object.
    \remarks
    type() returns one of the following:
    
    <table>
    Name                        Value
    OdGsLight::kPointLight      0
    OdGsLight::kDistantLight    1
    OdGsLight::kSpotlight       2
    </table>

  */
  OdGsLight::LightType type() const;
  /** \details
    Sets the type of this Light object.
    \param type [in]  Light type.
    \remarks
    type must be one of the following:
    
    <table>
    Name                        Value
    OdGsLight::kPointLight      0
    OdGsLight::kDistantLight    1
    OdGsLight::kSpotlight       2
    </table>
  */
  void setType(OdGsLight::LightType type);

  /** \details
    Returns the name of this Light object.
    
  */
  OdString name() const;
  
  /** \details
    Sets the name of this Light object.
    \param name [in] Name.
    \remarks
    Names are truncated to 8 characters.
  */
  void setName(const OdString& name);

  /** \details
    Returns the intensity of this Light object.
    \remarks
    Returns a value in the range [0.0 .. 1.0].
  */
  double intensity() const;
  /** \details
    Sets the intensity of this Light object.
    \param intensity [in] Intensity. [0.0 .. 1.0]
  */
  void setIntensity(double intensity);

  /** \details
    Returns the color of this Light object.
    
    \remarks
    color() returns a 24-bit RGB color.
  */
  OdUInt32 color() const;
  
  /** \details
    Sets the color of this Light object.
    
    \param color [in]  24-bit RGB color.
  */
  void setColor(OdUInt32 color);

  /** \details
    Returns the WCS position of this Light object. 
  */
  OdGePoint3d position() const;
  
  /** \details
    Returns the WCS target of this Light object. 
  */
  OdGePoint3d target() const;

  /** \details
    Returns the WCS direction of this Light object. 
  */
  OdGeVector3d direction() const;
  
  /** \details
    Sets the WCS position of this Light object.
    
    \param position [in]  Position. 
  */
  void setPosition(const OdGePoint3d& position);
 
  /** \details
    Sets the WCS target of this Light object.
    
    \param target [in]  Target. 
  */
  void setTarget(const OdGePoint3d& target);
  /** \details
    Sets the WCS direction of this Light object.
    
    \param fromPoint [in]  From point.
    \param toPoint [in]  To point. 
  */
  void setDirection(const OdGePoint3d& fromPoint, const OdGePoint3d& toPoint);

  /** \details
    Returns the attenuation of this Light object. 
    
    \remarks
    attenuation() returns one of the following:
    
    <table>
    Name                             Value      
    OdGsLight::kAttenNone            0          
    OdGsLight::kAttenInverseLinear   1          
    OdGsLight::kAttenInverseSquare   2
    </table>
  */
  OdGsLight::LightAttenuation attenuation() const;
  
  /** \details
    Sets the attenuation of this Light object. 
    
    \param attenuation [in]  Attenuation.
    \remarks
    attenuation must be one of the following:
    
    <table>
    Name                             Value      
    OdGsLight::kAttenNone            0          
    OdGsLight::kAttenInverseLinear   1
    OdGsLight::kAttenInverseSquare   2
    </table>
  */
  void setAttenuation(OdGsLight::LightAttenuation attenuation);

  /** \details
      Specifies the cone angles for spotlight (OdAve::kSpotLight).
      Inner cone defines the brightest cone of light(hotspot, beam angle).
      Outer cone defines the full cone of light(falloff, field angle).
      These values can range from 0 to 80 degrees.
  */
  
  /** \details
    Returns the inner cone angle for this kSpotLight Light object.
    
    \remarks
    The inner cone angle is one-half the beam angle, which defines the hotspot 
    or brightest cone of light    

    innerConeAngle() has a range of [0..80].
    
    \remarks
    All angles are expressed in degrees.
  */
  double innerConeAngle() const;
  /** \details
    Returns the outer cone angle for this kSpotLight Light object.
    
    \remarks
    The outer cone angle is one-half the field angle, which defines the 
    falloff or full cone of light.
    
    outerConeAngle() has a range of [0..80].
    
    \remarks
    All angles are expressed in degrees.
  */
  double outerConeAngle() const;
  
  /** \details
    Sets the cone angles for this kSpotLight Light object.
 
    \param innerConeAngle [in]  Inner cone angle. [0..80]
    \param outerConeAngle [in]  Outer cone angle. [0..80]
    \remarks
    The inner cone angle is one-half the beam angle, which defines the hotspot or brightest cone of light    
 
    The outer cone angle is one-half the field angle, which defines the falloff or full cone of light.

    \remarks
    All angles are expressed in degrees.
  */
  void setConeAngle(double innerConeAngle, double outerConeAngle);

  /** \details
    Returns true if and only if shadows are enabled for this Light object.
  */
  bool isShadowOn() const;
  /** \details
    Controls shadows for this Light object.
    \param shadowOn [in]  Controls shadows.
  */
  void setShadowOn(bool shadowOn);

  /** \details
    Returns true if and only if volumetric/raytraced shadows are 
    enabled for this Light object.
  */
  bool isShadowVolumes() const;
  /** \details
    Controls volumetric/raytraced shadows for this Light object.
    \param shadowVolumes [in]  Controls shadows.
  */
  void setShadowVolumes(bool);

  /** \details
    Accesses the size of the shadow map for this Light object.

    \remarks
    The shadow map size is the size, in pixels, of one edge of the shadow map.
    
    \remarks
    shadowMapSize() has a domain and a range of [64..4096].     
    
    The smaller the size, the faster but less accurate the rendering.
  */
  OdInt16 shadowMapSize() const;

  /** \details
    Accesses the size of the shadow map for this Light object.
    
    \param mapSize [in]  Shadow map size. [64..4096]
    
    \remarks
    The shadow map size is the size, in pixels, of one edge of the shadow map.
    
    \remarks
    shadowMapSize() has a domain and a range of [64..4096].     
    
    The smaller the size, the faster but less accurate the rendering.
  */
  void shadowMapSize(OdInt16 mapSize);

  /** \details
    Returns the softness (fuzziness) of shadow-mapped shadows for this Light object.
    
    \remarks
    shadowSoftness() has a range of [1..10].
    
    Shadow softness represents the number of shadow-edge pixels 
    blended into the image.
  */
  double shadowSoftness() const;
  /** \details
    Sets the softness (fuzziness) of shadow-mapped shadows for this Light object.
    
    \param shadowSoftness [in]  Shadow softness. [1..10]
    
    \remarks
    Shadow softness represents the number of shadow-edge pixels 
    blended into the image.
    
    Values in the range [2..4] typically give the best results.
  */
  void setShadowSoftness(double shadowSoftness);

  /** \details
    Returns the shadow bounding objects for this Light object.
    \param objectIds [out]  Receives the array of Object IDs. 
    
  */
  void shadowBoundingObjects(OdDbObjectIdArray& objectIds) const;
  /** \details
    Sets the shadow bounding objects for this Light object.
    \param objectIds [in]  Array of Object IDs. 
  */
  void setShadowBoundingObjects(const OdDbObjectIdArray& objectIds);
  /** \details
    Adds the specified OdDbDatabase object to the shadow bounding objects for this Light object.
    \param objectId [in]  Object ID to add. 
  */
  void addShadowBoundingObject(const OdDbObjectId& objectId);

  /** \details
    Returns the Object ID of the OdDbBlockReference associated with this Light object.
  */
  OdDbObjectId holderId() const;

protected:
  /** \details
      Constructors
  */
  OdAveLight();
  OdAveLight(OdAveLightImpl* pImpl);
  
  OdAveLightImpl* m_pImpl;
};

/** \details
  Returns an Iterator object that can be 
  used to traverse the OdAveLight objects in the specified database.
  
  \param pDb [in]  Pointer to the database.
*/
AVE_EXPORT OdRxIteratorPtr oddbGetLights(OdDbDatabase* pDb);

#include "TD_PackPop.h"

#endif // OD_AVELIGHT_H

