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

#ifndef OD_VIEWREPIMAGE_H
#define OD_VIEWREPIMAGE_H

#include "DbEntity.h"
#include "ModelDocObjExports.h"
#include "Gi/GiImage.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiRasterWrappers.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepImage : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepImage);

  OdDbViewRepImage();
  virtual ~OdDbViewRepImage();


	OdGePoint3d const &  centerPoint() const;
	void setCenterPoint(OdGePoint3d const & );

	double getHeight() const;
	void setHeight(double);

	double getWidth() const;
	void setWidth(double);

	OdDbObjectId ownerViewportId() const;
	void setOwnerViewportId(const OdDbObjectId);

  /*
	AcGeMatrix3d getModelCoordinate() const;
	void setModelCoordinate(AcGeMatrix3d const & );

	unsigned char* getRawImageBytes() const;
	void setRawImageBytes(unsigned char*);

	virtual void saveAs(AcGiWorldDraw*,enum AcDb::SaveType);

	void setImageData(AcGiImageBGRA32 const&);
	void setImageData(unsigned char*);

	virtual enum Acad::ErrorStatus subTransformBy(AcGeMatrix3d const&);
	virtual void subViewportDraw(AcGiViewportDraw*);
   */

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepImage object pointers.
*/
typedef OdSmartPtr<OdDbViewRepImage> OdDbViewRepImagePtr;

#include "TD_PackPop.h"

#endif // OD_VIEWREPIMAGE_H
