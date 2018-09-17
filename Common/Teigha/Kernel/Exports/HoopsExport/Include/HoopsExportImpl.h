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


///////////////////////////////////////////////////////////////////////////////
//
//  HoopsExportImpl.h : definition of the HoopsExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef HOOPSEXPORTIMPL_DEFINED
#define HOOPSEXPORTIMPL_DEFINED
#include "OdaCommon.h"
#include "Gs/Gs.h"
#include "Gi/GiRasterImage.h"
#include "Ge/GeMatrix3d.h"
#include "Gi/GiMaterial.h"

#include "HoopsExport.h"
#include "hc.h"

#define STL_USING_MAP
#define STL_USING_STRING
#include "OdaSTL.h"


/** \details
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {

/** \details
  This structure implements the hoops font type pair.
*/
struct HoopsFontTypePair
{
  bool          bRaw;
  OdCodePageId  idCodePage;
  OdFont*       pFont;
  OdFont*       pBigFont;
  OdString      strHoopsFontName;
  double        dScaleFactor;
};

/** \details
  This structure implements the hoops linetype pair.
*/
struct HoopsLineTypePair
{
  OdDbStub* idDwgLineType;
  OdString  strHoopsLineType;
  double    dScale;
  double    dLength;
  bool      bShapes;
};

/** \details
  This structure implements the hoops texture image pair.
*/
struct HoopsTextureImagePair
{
  OdString strFileName;
  OdString strHoopsName;
  double   dFade;
};

/** \details
  This structure implements the hoops image pair.
*/
struct HoopsImagePair
{
  const OdGiRasterImage* pImage;
  bool             bTransparency;
  double           dBrightness;
  double           dContrast;
  double           dFade;
  OdString         strHoopsImageName;
  OdUInt32         iImageHeight;
  OdUInt32         iImageWidth;
  OdUInt32         iBitPerPixel;
};

/** \details
  This structure implements the hoops glyph pair.
*/
struct HoopsGlyphsPair
{
  OdDbStub* idGlyphDwg;
  OdString  strHoopsGlyphName;
  double    dXOffset;
  double    dYOffset;
  double    dLength;
  double    dRealLength;
  double    dAngle;
  double    dScale;
  OdString  strText;
  OdInt16   iNumber;
};

/** \details
  This structure implements the hoops material information.
*/
struct HoopsMaterialInfo
{
  OdDbStub*           pMaterialId;
  bool                bSetMaterial;
  bool                bTTFProcessing;
  ODCOLORREF          clrAmbient;
  ODCOLORREF          clrDiffuse;
  ODCOLORREF          clrSpecular;
  bool                bUseAmbient;
  bool                bUseDiffuse;
  bool                bUseSpecular;
  OdString            strTextureFilename;
  OdGeMatrix3d        matTextureTransform;
  OdGiMapper::Tiling  uTiling;
  OdGiMapper::Projection iTextureProjection;
  OdInt16             iTextureType;
  bool                bTwoSides;
  double              dGlossFactor;
  double              dTranslucence;
  double              dTextureBrightness;
};

/** \details
  This class implements the hoops export.
*/
class CHoopsExportImpl
{
public:
    // Constructor/destructor

    CHoopsExportImpl();
    virtual ~CHoopsExportImpl();

    // Implementation
    HoopsResultEx init(const HoopsExportParams& params );
    HoopsResultEx run();
    HoopsResultEx exportFile();

    const HoopsExportParams& getParams() const
    {
      return m_Params;
    }

private:

    void FreeHoopsModel();

private:

    HoopsExportParams           m_Params;
    OdGsDevicePtr               m_pDevice;
    OdArray<HoopsLineTypePair>  arrLineTypePairs;
    OdArray<HoopsGlyphsPair>    arrGlyphsPairs;
    OdArray<HoopsFontTypePair>  arrFontPairs;
    OdArray<HoopsImagePair>     arrImagePairs;
    OdArray<HoopsTextureImagePair> arrTexturePairs;
    OdUInt32                    m_iShapeCount;
    OdUInt32                    m_iLTypeCount;
    HC_KEY                      m_iCurKey;
    HC_KEY                      m_iRealStartKey;
    OdArray<HC_KEY>             m_lViewKeyArr;

    OdUInt32                    m_iLineCount;
    OdUInt32                    m_iXLineCount;
    OdUInt32                    m_iRayCount;
    OdUInt32                    m_iCircleCount;
    OdUInt32                    m_iEllipseCount;
    OdUInt32                    m_iNurbsCount;
    OdUInt32                    m_iTextCount;
    OdUInt32                    m_iFontStyleCount;
    OdUInt32                    m_iPolygonCount;
    OdUInt32                    m_iImageCount;
    OdUInt32                    m_iImageShellCount;
    OdUInt32                    m_iTextureCount;
    OdUInt32                    m_iMeshCount;
    OdUInt32                    m_iShellCount;
    OdUInt32                    m_iLSCount;
    OdUInt32                    m_iObjectCount;
    OdUInt32                    m_iViewportCount;

friend class OdHoopsExportDevice;
friend class OdHoopsExportView;
};
}
#endif 
