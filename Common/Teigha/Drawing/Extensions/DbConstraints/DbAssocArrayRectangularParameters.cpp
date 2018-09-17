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

#include "OdaCommon.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbImpAssocArrayParameters.h"
#include "DbAssocArrayActionBody.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayRectangularParameters,    // ClassName
                       OdDbAssocArrayCommonParameters,         // ParentClass
                       RXIMPL_CONSTR,                          // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
                       OdDb::kDHL_1024,                        // DwgVer
                       OdDb::kMRelease6,                       // MaintVer
                       1025,                                   // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayRectangularParameters", // DWG class name
                       L"ACDBASSOCARRAYRECTANGULARPARAMETERS", // DxfName
                       L"ObjectDBX Classes",                   // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayRectangularParameters::OdDbAssocArrayRectangularParameters(double columnSpacing,
                                                                         double rowSpacing,
                                                                         double levelSpacing,
                                                                         int columnCount,
                                                                         int rowCount,
                                                                         int levelCount,
                                                                         double rowElevation,
                                                                         double axesAngle)
  : OdDbAssocArrayCommonParameters(new OdDbImpAssocArrayRectangularParameters(columnSpacing, rowSpacing, levelSpacing,
                                                                              columnCount, rowCount, levelCount,
                                                                              rowElevation, axesAngle))
{
}


OdDbAssocArrayRectangularParameters::OdDbAssocArrayRectangularParameters(OdDbImpAssocArrayRectangularParameters* pImpl)
  : OdDbAssocArrayCommonParameters(pImpl)
{
}                                                      

OdDbAssocArrayRectangularParameters::~OdDbAssocArrayRectangularParameters()
{
}

int OdDbAssocArrayRectangularParameters::columnCount(OdString& expression, 
                                                     OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>(m_pImpl)->ColumnCount( expression, evaluatorId );
}

int OdDbAssocArrayRectangularParameters::columnCount() const
{
  OdString expression, evaluatorId;
  return columnCount(expression, evaluatorId);
}

int OdDbAssocArrayRectangularParameters::columnCount(OdString& expression) const
{
  OdString evaluatorId;
  return columnCount(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::columnSpacing(OdString& expression, 
                                                          OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>(m_pImpl)->ColumnSpacing( expression, evaluatorId );
}

double OdDbAssocArrayRectangularParameters::columnSpacing() const
{
  OdString expression, evaluatorId;
  return columnSpacing(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::columnSpacing(OdString& expression) const
{
  OdString evaluatorId;
  return columnSpacing(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::axesAngle(OdString& expression, 
                                                      OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>(m_pImpl)->AxesAngle( expression, evaluatorId );
}

double OdDbAssocArrayRectangularParameters::axesAngle() const
{
  OdString expression, evaluatorId;
  return axesAngle(expression, evaluatorId);
}

double OdDbAssocArrayRectangularParameters::axesAngle(OdString& expression) const
{
  OdString evaluatorId;
  return axesAngle(expression, evaluatorId);
}

OdGeVector3d OdDbAssocArrayRectangularParameters::axisDirection(OdGeVector3d* pYAxis) const // = NULL
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>
    (m_pImpl)->axisDirection( pYAxis );
}

OdResult OdDbAssocArrayRectangularParameters::setColumnCount(int nColumns, 
                                                             const OdString& expression, // = OdString::kEmpty
                                                             const OdString& evaluatorId, // = OdString::kEmpty
                                                             OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>
          (m_pImpl)->setColumnCount( nColumns, expression, evaluatorId, pErrorMessage );

}

OdResult OdDbAssocArrayRectangularParameters::setColumnSpacing(double offset, 
                                                               const OdString& expression, // = OdString::kEmpty
                                                               const OdString& evaluatorId, // = OdString::kEmpty
                                                               OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>
    (m_pImpl)->setColumnSpacing( offset, expression, evaluatorId, pErrorMessage );
}

OdResult OdDbAssocArrayRectangularParameters::setAxesAngle(double axesAngle, 
                                                           const OdString& expression, // = OdString::kEmpty
                                                           const OdString& evaluatorId, // = OdString::kEmpty
                                                           OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>
    (m_pImpl)->setAxesAngle( axesAngle, expression, evaluatorId, pErrorMessage );
}

OdResult OdDbAssocArrayRectangularParameters::setAxisDirection(const OdGeVector3d& xAxis)
{
  return static_cast<OdDbImpAssocArrayRectangularParameters *>
    (m_pImpl)->setAxisDirection( xAxis );
}

OdResult OdDbAssocArrayRectangularParameters::getItemPosition( const OdDbItemLocator& locator,
                                                               OdGePoint3d& position,
                                                               OdGeMatrix3d& xform) const
{
  return m_pImpl->getItemPosition( locator, position, xform );
}

OdResult OdDbAssocArrayRectangularParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{  
  return m_pImpl->getItems( items );
}                                                      

