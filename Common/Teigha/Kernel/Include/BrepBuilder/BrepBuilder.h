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

#ifndef _BREPBUILDER_H_
#define _BREPBUILDER_H_

#include "OdaCommon.h"
#include "BrepBuilderExport.h"
#include "Ge/GeCurve3d.h"
#include "CmColorBase.h"
#include "TD_PackPush.h"

class OdDbStub;
class OdGiMapper;

/** \details
Corresponding C++ library: TD_BrepBuilder

<group OdBrepBuilder_Classes>
*/
enum BrepType
{
  //The output geometry is an open shell
  kOpenShell,
  //The output geometry is an solid
  kSolid,
  //The output geometry is an void (i.e., an inverted solid).
  kVoid
};

typedef OdUInt32 BRepBuilderGeometryId;

/** \details
Corresponding C++ library: TD_BrepBuilder

<group OdBrepBuilder_Classes>
*/
class BREPBUILDEREXPORT OdBrepBuilder
{
public:
  BREPBUILDEREXPORT_STATIC static const BRepBuilderGeometryId kDefaultShellId;

  OdBrepBuilder();
  ~OdBrepBuilder();

  enum EntityDirection
  {
    kForward,
    kReversed
  };

  //Returns type of output geometry 
  BrepType getType();

  //Sets the product-dependent implementation
  void set(const void* pFile);

  /** \details
    Add a co-edge associated to a previously added edge. 
    A co-edge represents the use of an edge on one of the edge's faces.

    \remarks:
    OdError(eInvalidInput) exception will be thrown if type of parametric curve(pParCur) is not supported by BrepBuilder implementation.
  */
  BRepBuilderGeometryId addCoedge(const BRepBuilderGeometryId& loopId, const BRepBuilderGeometryId& edgeId, EntityDirection codgeDirection = kForward, const OdGeCurve2d* pParCur = NULL);
  // Add a new edge to the geometry being built.
  BRepBuilderGeometryId addEdge(const OdGeCurve3d* pCurveForEdge);

  // Creates an empty face in the geometry being built.
  BRepBuilderGeometryId addFace(const OdGeSurface* pSurf, EntityDirection faceDirection, const BRepBuilderGeometryId& shellId = kDefaultShellId);

  // Creates an empty loop in a given face of the geometry being built.
  BRepBuilderGeometryId addLoop(const BRepBuilderGeometryId& faceId);

  // Creates a shell.
  BRepBuilderGeometryId addShell(const BRepBuilderGeometryId& complexId);

  // Creates a complex - upper topology element with gives shell
  BRepBuilderGeometryId addComplex();

  //Allow BRepBuilder to remove problematic faces (e.g., due to inaccurate edge geometry). 
  //If this option is enabled and BRepBuilder removes some faces, 
  //the output geometry's type will be OpenShell regardless of the expected type specified when the BRepBuilder was created.
  void allowRemovalOfProblematicFaces();
 
  //A validator function that checks the state of this BRepBuilder object.Returns true if this BRepBuilder object is accepting b - rep data, false otherwise.
  bool canAddGeometry();

  //Indicates that the caller has finished defining the given complex.
  void finishComplex(const BRepBuilderGeometryId& complexId);

  //Indicates that the caller has finished defining the given shell
  void finishShell(const BRepBuilderGeometryId& shellId);

  //Indicates that the caller has finished defining the given face.
  void finishFace(const BRepBuilderGeometryId& faceId);

  //Indicates that the caller has finished defining the given loop.
  void finishLoop(const BRepBuilderGeometryId& loopId);
    
  //Get the Geometry object built by this BRepBuilder. This will clear the built Geometry stored in the BRepBuilder.
  //This function will throw if this BRepBuilder hasn't completed building the b-rep. Use IsResultAvailable() to verify that this BRepBuilder contains a valid result.
  OdRxObjectPtr getResult();

  //A validator function that checks whether the surface object is of type supported as face surface by BRepBuilder.
  bool isPermittedSurfaceType(const OdGeSurface* pSurf);

  //A validator function that checks the state of this BRepBuilder object. Returns true if this BRepBuilder object has successfully built a b-rep.
  bool isResultAvailable();

  //A validator function that checks whether the edge id corresponds to an edge previously added to this BRepBuilder object.
  bool isValidEdgeId(const BRepBuilderGeometryId& edgeId);

  //A validator function that checks whether the complex id corresponds to a complex previously added to this BRepBuilder object.
  bool isValidComplexId(const BRepBuilderGeometryId& complexId);

  //A validator function that checks whether the shell id corresponds to a shell previously added to this BRepBuilder object.
  bool isValidShellId(const BRepBuilderGeometryId& shellId);

  //A validator function that checks whether the face id corresponds to a face previously added to this BRepBuilder object.
  bool isValidFaceId(const BRepBuilderGeometryId& faceId);

  //A validator function that checks whether the loop id corresponds to a loop previously added to this BRepBuilder object.
  bool isValidLoopId(const BRepBuilderGeometryId& loopId);

  //Returns 'true' if BRepBuilder removed some problematic faces from the output geometry, 'false' if not.
  //If allowRemovalOfProblematicFaces was not called to enable removal of problematic faces, this function will return 'false'. Note that if some faces were removed,
  //the output geometry's type will be OpenShell regardless of the expected type that was specified when the BRepBuilder was created.
  bool removedSomeFaces();

  //Make BRepBuilder allow edges that it would normally disallow as being too short for geometry.
  void setAllowShortEdges();

  //Sets material id to a face.
  void setFacesMaterial(const BRepBuilderGeometryId &faceId, OdDbStub &materialId);

  /** \details
  Sets material mapping for face.
  Return eNotApplicable if not supported in implementation or material mapping is part of material.
  */
  OdResult setFaceMaterialMapping(const BRepBuilderGeometryId &faceId,
    const OdGiMapper& materialMapper);

  /** \details
  Sets edge color.
  Return value:
    eNotApplicable if setEdgeColor not supported in implementation.
    eInvalidInput if color method not supported in implementation.
  */
  OdResult setEdgeColor(const BRepBuilderGeometryId& edgeId, const OdCmEntityColor& edgeColor);

  /** \details
  Sets face color.
  Return eNotApplicable if not supported in implementation or face color is part of material.
  */
  OdResult setFaceColor(const BRepBuilderGeometryId& faceId, const OdCmEntityColor& faceColor);

  //Complete construction of the geometry. The geometry will be validated and, if valid, stored in this Builder.
  OdRxObjectPtr finish();

  //Enable/disable validation in BRepBuilder
  void enableValidator(bool bEnable = true);

  // Check state
  bool isValid() const;

protected:
    void* m_pImpl;
};

#include "TD_PackPop.h"

#endif //_BREPBUILDER_H_
