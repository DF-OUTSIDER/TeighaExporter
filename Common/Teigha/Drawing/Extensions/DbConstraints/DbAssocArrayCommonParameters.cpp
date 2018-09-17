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
#include "DbAssocArrayCommonParameters.h"
#include "DbImpAssocArrayParameters.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocArrayCommonParameters, OdDbAssocArrayParameters, L"AcDbAssocArrayCommonParameters")

//protected:
OdDbAssocArrayCommonParameters::OdDbAssocArrayCommonParameters(OdDbImpAssocArrayCommonParameters* pImpl)
  : OdDbAssocArrayParameters(pImpl)
{
}                                                      

OdDbAssocArrayCommonParameters::~OdDbAssocArrayCommonParameters()
{
}                                                      

int OdDbAssocArrayCommonParameters::levelCount(OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> (m_pImpl)->levelCount( expression, evaluatorId );
}                                                      

int OdDbAssocArrayCommonParameters::levelCount() const
{
  OdString expression, evaluatorId;
  return levelCount(expression, evaluatorId);
}

int OdDbAssocArrayCommonParameters::levelCount(OdString& expression) const
{
  OdString evaluatorId;
  return levelCount(expression, evaluatorId);
}

double OdDbAssocArrayCommonParameters::levelSpacing(OdString& expression, 
                                                    OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> (m_pImpl)->levelSpacing( expression, evaluatorId );
}                                                      

double OdDbAssocArrayCommonParameters::levelSpacing() const
{
  OdString expression, evaluatorId;
  return levelSpacing(expression, evaluatorId);
}

double OdDbAssocArrayCommonParameters::levelSpacing(OdString& expression) const
{
  OdString evaluatorId;
  return levelSpacing(expression, evaluatorId);
}

int OdDbAssocArrayCommonParameters::rowCount(OdString& expression, 
                                             OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> (m_pImpl)->rowCount( expression, evaluatorId );
}                                                      

int OdDbAssocArrayCommonParameters::rowCount() const
{
  OdString expression, evaluatorId;
  return rowCount(expression, evaluatorId);
}    

int OdDbAssocArrayCommonParameters::rowCount(OdString& expression) const
{
  OdString evaluatorId;
  return rowCount(expression, evaluatorId);
}

double OdDbAssocArrayCommonParameters::rowElevation(OdString& expression, 
                                                    OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> (m_pImpl)->rowElevation( expression, evaluatorId );
}                                                      

double OdDbAssocArrayCommonParameters::rowElevation() const
{
  OdString expression, evaluatorId;
  return rowElevation(expression, evaluatorId);
}      

double OdDbAssocArrayCommonParameters::rowElevation(OdString& expression) const
{
  OdString evaluatorId;
  return rowElevation(expression, evaluatorId);
}

double OdDbAssocArrayCommonParameters::rowSpacing(OdString& expression, 
                                                  OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> (m_pImpl)->rowSpacing( expression, evaluatorId );
}                                                      

double OdDbAssocArrayCommonParameters::rowSpacing() const
{
  OdString expression, evaluatorId;
  return rowSpacing(expression, evaluatorId);
}

double OdDbAssocArrayCommonParameters::rowSpacing(OdString& expression) const
{
  OdString evaluatorId;
  return rowSpacing(expression, evaluatorId);
}  

OdResult OdDbAssocArrayCommonParameters::setLevelCount(int nLevels, 
                                                       const OdString& expression, // = OdString::kEmpty
                                                       const OdString& evaluatorId, // = OdString::kEmpty
                                                       OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
          (m_pImpl)->setLevelCount( nLevels, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayCommonParameters::setLevelSpacing(double offset, 
                                                         const OdString& expression, //  = OdString::kEmpty
                                                         const OdString& evaluatorId, // = OdString::kEmpty
                                                         OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
          (m_pImpl)->setLevelSpacing( offset, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayCommonParameters::setRowCount(int nRows, 
                                                     const OdString& expression, // = OdString::kEmpty
                                                     const OdString& evaluatorId, // = OdString::kEmpty
                                                     OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->setRowCount( nRows, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayCommonParameters::setRowElevation(double elevation, 
                                                         const OdString& expression, // = OdString::kEmpty
                                                         const OdString& evaluatorId, // = OdString::kEmpty
                                                         OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->setRowElevation( elevation, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayCommonParameters::setRowSpacing(double offset, 
                                                       const OdString& expression, // = OdString::kEmpty
                                                       const OdString& evaluatorId, // = OdString::kEmpty
                                                       OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->setRowSpacing( offset, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayCommonParameters::getBasePlane(OdDbVertexRef& basePoint,
                                                      OdGeVector3d& normal, 
                                                      OdDbFaceRef* pFace) const // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->getBasePlane( basePoint, normal, pFace );
}                                                      

OdResult OdDbAssocArrayCommonParameters::setBasePlane(const OdDbVertexRef& basePoint,
                                                      const OdGeVector3d& normal,
                                                      const OdDbFaceRef* pFace) // = NULL
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->setBasePlane( basePoint, normal, pFace );
}                                                      

OdDbItemLocator OdDbAssocArrayCommonParameters::getCanonicalForm(const OdDbItemLocator& locator) const
{
  return static_cast<OdDbImpAssocArrayCommonParameters *> 
    (m_pImpl)->getCanonicalForm( locator );
}                                                      
