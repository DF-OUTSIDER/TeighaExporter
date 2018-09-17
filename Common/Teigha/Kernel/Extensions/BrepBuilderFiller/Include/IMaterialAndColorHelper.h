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


#ifndef _IMATERIALHELPER_INCLUDED_
#define _IMATERIALHELPER_INCLUDED_

#include "OdError.h"

class OdBrEdge;
class OdBrFace;
class OdDbStub;
class OdCmEntityColor;
class OdGiMapper;


/** \details
  Helper structure for handling materials and colors.

  Corresponding C++ library: TD_BrepBuilderFiller
  <group Extension_Classes>
*/
class OdIMaterialAndColorHelper
{
public:
  virtual ~OdIMaterialAndColorHelper() {}

  /** \details
  This method get and convert face visuals information for BrepBuilder.
  Also it should return same material for similar visual information.
  \param face [in] object for getting source visual information.
  \param faceMaterial [out] face material for BrepBuilder.
  \param faceMaterialMapping [out] face material mapping for BrepBuilder. Should be ignoredd if applyFaceMaterialMapping is false.
  \param applyFaceMaterialMapping [out] flag signals to use faceMaterialMapping.
  \param faceColor [out] face color for BrepBuilder. Should be ignored if applyFaceColor is false.
  \param applyFaceColor [out] flag signals to use faceColor.
  */
  virtual OdResult getFaceVisualInfo(const OdBrFace& sourceFace,
    OdDbStub*& faceMaterial,
    OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
    OdCmEntityColor& faceColor, bool& applyFaceColor) = 0;

  /** \details
  This method get and convert edge visuals information for BrepBuilder.
  \param edge [in] object for getting source visual information.
  \param edgeColor [out] edge color for BrepBuilder. Should be ignored if applyEdgeColor is false.
  \param applyEdgeColor [out] flag signals to use edgeColor.
  */
  virtual OdResult getEdgeVisualInfo(const OdBrEdge& edge, OdCmEntityColor& edgeColor, bool& applyEdgeColor) = 0;

};

#endif //_IMATERIALHELPER_INCLUDED_
