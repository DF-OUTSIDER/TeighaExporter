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

#ifndef OD_GE_DXF_IO_H_
#define OD_GE_DXF_IO_H_

#include "Ge/GeLineSeg3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCompositeCurve3d.h"
//#include "Ge/GeLibVersion.h"

class OdDbDxfFiler;

/** \details
    This class reads information from and writes information to a DXF file.

    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdGeDxfIO
{
public:

  //static void outFields(OdDbDxfFiler* pFiler, const OdGePoint2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeVector2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeMatrix2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeScale2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePoint2dArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeVector2dArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePoint3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeVector3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeMatrix3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeScale3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePoint3dArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeVector3dArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeTol& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeInterval& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeKnotVector& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeDoubleArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdIntArray& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCurveBoundary& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePosition2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePointOnCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeLine2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeLineSeg2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeRay2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCircArc2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeEllipArc2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeExternalCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCubicSplineCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeNurbCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCompositeCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeOffsetCurve2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeSegmentChain2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePosition3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePointOnCurve3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePointOnSurface& object);
  
  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeLine3d& object);
  
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeRay3d& object);
  
  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeLineSeg3d& object);
  
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePlane& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeBoundedPlane& object);
  
  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeCircArc3d& object);
  
  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeEllipArc3d& object);
  
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCubicSplineCurve3d& object);
  
  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeNurbCurve3d& object);

  /** \details
    Writes information to a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDxfFiler* pFiler, const OdGeCompositeCurve3d& object);
  
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeOffsetCurve3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGePolyline3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeAugPolyline3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeExternalCurve3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCone& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeEllipCone& object);	// AE 03.09.2003 
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCylinder& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeEllipCylinder& object);	// AE 09.09.2003 
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeTorus& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeExternalSurface& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeOffsetSurface& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeNurbSurface& object);
  //static void outFields(OdDbDxfFiler* pFiler,const OdGeExternalBoundedSurface& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeSphere& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeBoundBlock2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeBoundBlock3d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCurveCurveInt2d& object);
  //static void outFields(OdDbDxfFiler* pFiler, const OdGeCurveCurveInt3d& object);


  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePoint2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeVector2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeMatrix2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeScale2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePoint2dArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeVector2dArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePoint3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeVector3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeMatrix3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeScale3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePoint3dArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeVector3dArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeTol& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeInterval& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeKnotVector& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeDoubleArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdIntArray& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCurveBoundary& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePosition2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePointOnCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeLine2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeLineSeg2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeRay2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCircArc2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeEllipArc2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeExternalCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCubicSplineCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeNurbCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCompositeCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeOffsetCurve2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeSegmentChain2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePosition3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePointOnCurve3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePointOnSurface& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeLine3d& object);
  
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeRay3d& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeLineSeg3d& object);
  
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePlane& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeBoundedPlane& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCircArc3d& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeEllipArc3d& object);
  
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCubicSplineCurve3d& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeNurbCurve3d& object);
  
  /** \details
    Reads information from a DXF file.

    \param pFiler [in]  Pointer to the OdDbDxfFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCompositeCurve3d& object);
  
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeOffsetCurve3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGePolyline3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeAugPolyline3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeExternalCurve3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCone& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeEllipCone& object);	// AE 03.09.2003 
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCylinder& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeEllipCylinder& object);	// AE 09.09.2003 
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeTorus& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeExternalSurface& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeOffsetSurface& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeNurbSurface& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeExternalBoundedSurface& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeSphere& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeBoundBlock2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeBoundBlock3d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCurveCurveInt2d& object);
  //static OdResult inFields(OdDbDxfFiler* pFiler, OdGeCurveCurveInt3d& object);

  //static const OdGeLibVersion  OdGeDxfIOVersion;
};

#endif // OD_GE_DXF_IO_H_

