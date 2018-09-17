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

#ifndef OdDbImpAssocSurfacesParameters_INCLUDED_
#define OdDbImpAssocSurfacesParameters_INCLUDED_ /*!DOM*/
#include "DbEntity.h"
#include "DbAssocArrayItem.h"
#include "DbImpAssocArrayParameters.h"
#include "DbAssocSurfacesParameters.h"

#include "TD_PackPush.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBCONSTRAINTS_EXPORT OdDbImpAssocLoftParameters : public OdDbImpAssocArrayParameters
{
  public:
    /// <summary>
    /// The type of a profile in creating a lofted surface.
    /// </summary>
    ///
    enum ProfileType {
      /// <summary>
      /// First profile of cross section, can be a point or curve/subentity edge
      /// </summary>
      ///
      kStartCrossSection = 0x01,

      /// <summary>
      /// Last profile of cross section, can be a point or curve/subentity edge
      /// </summary>
      ///
      kEndCrossSection = 0x02,

      /// <summary>
      /// First profile of guide curves
      /// </summary>
      ///
      kStartGuide = 0x04,

      /// <summary>
      /// Last profile of guide curves
      /// </summary>
      ///
      kEndGuide = 0x08
    };

    /** \details
    Constructor creates an instance of this class.
    */
    OdDbImpAssocLoftParameters(
      short continuity = 1,
      double bulge = 0.5
      );
  
    /** \details
    Destructor destroys an instance of this class.
    */
    virtual ~OdDbImpAssocLoftParameters() {};

    /// <summary>
    /// <para> Get continuity of a specific profile of the resulting lofted surface </para>
    /// <para>
    /// the same parameter can be accessed through class OdDbAssocAction by calling method 
    /// getValueParam with name "kContinuityParamName" and corresponding index as following: 
    /// </para>
    /// <para> kStartCrossSection  : "0". </para>
    /// <para> kEndCrossSection : number of cross sections - 1. </para>
    /// <para> kStartGuide  : number of cross sections. </para>
    /// <para> kEndGuide : number of cross sections + number of guide curves - 1. </para>
    /// </summary>
    /// <param name="type">  Specify which profile to get continuity. </param>
    /// <param name="continuity">  Returned continuity, can be 0(G0), 1(G1), 2(G2). </param>
    /// <param name="expression">  Expression of the parameter, empty string if no expression is being used. </param>
    /// <param name="evaluatorId"> The id of the expression evaluator that is used to parse the expression, empty string if no expression is being used </param>
    /// <returns> OdResult. </returns>
    ///
    //OdResult getContinuity(ProfileType type, int &continuity, OdString& expression  /*= OdString()*/,
    //  OdString& evaluatorId /*= OdString()*/) const;

    /// <summary>
    /// <para> Set continuity of a specific profile of the resulting lofted surface </para>
    /// <para>
    /// the same parameter can be set through class OdDbAssocAction by calling method 
    /// setValueParam with name "kContinuityParamName" and corresponding index as following: 
    /// </para>
    /// <para> kStartCrossSection  : "0". </para>
    /// <para> kEndCrossSection : number of cross sections - 1. </para>
    /// <para> kStartGuide  : number of cross sections. </para>
    /// <para> kEndGuide : number of cross sections + number of guide curves - 1. </para>
    /// </summary>
    /// <param name="type">  Specify which profile to set continuity. </param>
    /// <param name="continuity">  New continuity value, can be 0(G0), 1(G1), 2(G2). </param>
    /// <param name="expression">  Expression of the parameter, if the value is controlled by other parameters</param>
    /// <param name="evaluatorId"> The id of the expression evaluator that is used to parse the expression, if an expression is used. </param>
    /// <returns> OdResult. </returns>
    ///
    OdResult setContinuity(ProfileType type, short continuity, const OdString& expression /* = OdString()*/,
      const OdString& evaluatorId /*= OdString()*/, OdString& pErrorMessage);

    /// <summary>
    /// <para> Get bulge factor of a specific profile of the resulting lofted surface </para>
    /// <para>
    /// the same parameter can be accessed through class OdDbAssocAction by calling method 
    /// getValueParam with name "kBulgeParamName" and corresponding index as following: 
    /// </para>
    /// <para> kStartCrossSection  : "0". </para>
    /// <para> kEndCrossSection : number of cross sections - 1. </para>
    /// <para> kStartGuide  : number of cross sections. </para>
    /// <para> kEndGuide : number of cross sections + number of guide curves - 1. </para>
    /// </summary>
    /// <param name="type">  Specify which profile to get bulge. </param>
    /// <param name="bulge">  Returned bulge. </param>
    /// <param name="expression">  Expression of the parameter, empty string if no expression is being used. </param>
    /// <param name="evaluatorId"> The id of the expression evaluator that is used to parse the expression, empty string if no expression is being used </param>
    /// <returns> OdResult. </returns>
    ///
    //OdResult getBulge(ProfileType type, double &bulge, OdString& expression /*= OdString()*/,
    //  OdString& evaluatorId /*= OdString()*/) const;

    /// <summary>
    /// <para> Set bulge factor of a specific profile of the resulting lofted surface </para>
    /// <para>
    /// the same parameter can be set through class OdDbAssocAction by calling method 
    /// setValueParam with name "kBulgeParamName" and corresponding index as following: 
    /// </para>
    /// <para> kStartCrossSection  : "0". </para>
    /// <para> kEndCrossSection : number of cross sections - 1. </para>
    /// <para> kStartGuide  : number of cross sections. </para>
    /// <para> kEndGuide : number of cross sections + number of guide curves - 1. </para>
    /// </summary>
    /// <param name="type">  Specify which profile to set bulge. </param>
    /// <param name="bulge">  New bulge value. </param>
    /// <param name="expression">  Expression of the parameter, if the value is controlled by other parameters</param>
    /// <param name="evaluatorId"> The id of the expression evaluator that is used to parse the expression, if an expression is used. </param>
    /// <returns> OdResult. </returns>
    ///
    OdResult setBulge(ProfileType type, double bulge, const OdString& expression/* = OdString()*/,
      const OdString& evaluatorId /*= OdString()*/, OdString& pErrorMessage);

    /** \details
    Gets an array of items.
    */
    virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items);
    virtual OdGeMatrix3d CalculateDefaultMatrix(const OdDbItemLocator &locator);
    virtual OdDbItemLocator getCanonicalForm(const OdDbItemLocator& locator) const;
    OdResult setOwner(class OdDbAssocActionBody* pOwner);
    void setInputVariables(const OdDbObjectIdArray& sourceEntites, short continuity, double bulge);

protected:
    virtual OdResult GetItemByLocator(const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem) const;
    class OdDbAssocActionBody* m_pOwner;
};

#include "TD_PackPop.h"

#endif
#pragma once
