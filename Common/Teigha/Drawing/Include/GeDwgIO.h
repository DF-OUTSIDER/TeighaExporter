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

#ifndef OD_GE_DWG_IO_H_
#define OD_GE_DWG_IO_H_

#include "Ge/GeLineSeg3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCompositeCurve3d.h"
//#include "Ge/GeLibVersion.h"

class OdDbDwgFiler;

/** \details
    This class reads information from and writes information to a .dwg file.

    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdGeDwgIO
{
public:

	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePoint2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeVector2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeMatrix2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeScale2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePoint2dArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeVector2dArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePoint3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeVector3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeMatrix3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeScale3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePoint3dArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeVector3dArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeTol& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeInterval& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeKnotVector& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeDoubleArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdIntArray& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCurveBoundary& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePosition2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePointOnCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeLine2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeLineSeg2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeRay2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCircArc2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeEllipArc2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeExternalCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCubicSplineCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeNurbCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCompositeCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeOffsetCurve2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeSegmentChain2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePosition3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePointOnCurve3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGePointOnSurface& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeLine3d& object);
	
  //static OdResult inFields(OdDbDwgFiler* pFiler, OdGeRay3d& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeLineSeg3d& object);
	
  //static OdResult inFields(OdDbDwgFiler* pFiler, OdGePlane& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeBoundedPlane& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCircArc3d& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeEllipArc3d& object);
	
  //static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCubicSplineCurve3d& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCompositeCurve3d& object);
  
  //static OdResult inFields(OdDbDwgFiler* pFiler, OdGeOffsetCurve3d& object);
	
  /** \details
    Reads information from a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be read.
  */
  static OdResult inFields(OdDbDwgFiler* pFiler, OdGeNurbCurve3d& object);
	
  //static OdResult inFields(OdDbDwgFiler* pFiler, OdGePolyline3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeAugPolyline3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeExternalCurve3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCone& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeEllipCone& object);	// AE 03.09.2003 
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCylinder& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeEllipCylinder& object);	// AE 09.09.2003 
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeTorus& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeExternalSurface& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeOffsetSurface& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeNurbSurface& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeExternalBoundedSurface& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeSphere& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeBoundBlock2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeBoundBlock3d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCurveCurveInt2d& object);
	//static OdResult inFields(OdDbDwgFiler* pFiler, OdGeCurveCurveInt3d& object);


	//static void outFields(OdDbDwgFiler* pFiler, const OdGePoint2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeVector2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeMatrix2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeScale2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePoint2dArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeVector2dArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePoint3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeVector3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeMatrix3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeScale3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePoint3dArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeVector3dArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeTol& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeInterval& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeKnotVector& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeDoubleArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdIntArray& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCurveBoundary& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePosition2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePointOnCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeLine2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeLineSeg2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeRay2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCircArc2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeEllipArc2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeExternalCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCubicSplineCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeNurbCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCompositeCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeOffsetCurve2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeSegmentChain2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePosition3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePointOnCurve3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePointOnSurface& object);
	  
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeLine3d& object);
	
  //static void outFields(OdDbDwgFiler* pFiler, const OdGeRay3d& object);
	
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeLineSeg3d& object);
	
  //static void outFields(OdDbDwgFiler* pFiler, const OdGePlane& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeBoundedPlane& object);
	
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeCircArc3d& object);
	
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeEllipArc3d& object);
	
  //static void outFields(OdDbDwgFiler* pFiler, const OdGeCubicSplineCurve3d& object);
	
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeNurbCurve3d& object);
	
  /** \details
    Writes information to a .dwg file.

    \param pFiler [in]  Pointer to the OdDbDwgFiler object.
    \param object [in]  OdGe object to be written.
  */
  static void outFields(OdDbDwgFiler* pFiler, const OdGeCompositeCurve3d& object);
	
  //static void outFields(OdDbDwgFiler* pFiler, const OdGeOffsetCurve3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGePolyline3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeAugPolyline3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeExternalCurve3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCone& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeEllipCone& object);	// AE 03.09.2003 
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCylinder& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeEllipCylinder& object);	// AE 09.09.2003 
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeTorus& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeExternalSurface& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeOffsetSurface& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeNurbSurface& object);
	//static void outFields(OdDbDwgFiler* pFiler,const OdGeExternalBoundedSurface& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeSphere& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeBoundBlock2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeBoundBlock3d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCurveCurveInt2d& object);
	//static void outFields(OdDbDwgFiler* pFiler, const OdGeCurveCurveInt3d& object);

	//static const OdGeLibVersion  OdGeDwgIOVersion;
};

#endif // OD_GE_DWG_IO_H_

