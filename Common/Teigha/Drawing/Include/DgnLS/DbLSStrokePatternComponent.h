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

#ifndef ODDGNLS_DBLSSTROKEPATTERNCOMPONENT_H
#define ODDGNLS_DBLSSTROKEPATTERNCOMPONENT_H

#include "DbLSComponent.h"
#include "DbLSStroke.h"

#include "TD_PackPush.h"

/** \details
    Dgn Line Style Stroke Pattern Component.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSStrokePatternComponent : public OdDbLSComponent
{
  public:
    enum PhaseMode
    {
      kLSPhaseDistance = 0, // Distance phase mode
      kLSPhaseFraction = 1, // Fraction phase mode (auto phase)
      kLSPhaseCentered = 2  // Centered phase mode (stretch phase mode)
    };
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSStrokePatternComponent);

    OdDbLSStrokePatternComponent();

    /** \details
      Returns phase for fraction phase mode.

      \remarks
      Null for non-fraction phase modes.
    */
    double autoPhase() const;
    /** \details
      Sets phase for fraction phase mode.

      \param fPhase [in]  New phase value to set.
    */
    void setAutoPhase(double fPhase);

    /** \details
      Returns true if iteration limit mode is enabled.
    */
    bool hasIterationLimit() const;
    /** \details
      Sets iteration limit mode.

      \param bSet [in]  New mode to set.
    */
    void setHasIterationLimit(bool bSet);

    /** \details
      Inserts new stroke into this stroke pattern component.

      \param fLength [in]  Stroke length.
      \param fStartWidth [in]  Stroke start width.
      \param fEndWidth [in]  Stroke end width.
      \param widthMode [in]  Stroke width mode.
      \param capMode [in]  Stroke caps type.
      \returns
      Returns pointer to newly added stroke which could be used for additional modifications.
    */
    OdDbLSStroke *insertStroke(double fLength = 1.0, double fStartWidth = 0.0, double fEndWidth = 0.0,
      OdDbLSStroke::WidthMode widthMode = OdDbLSStroke::kLSNoWidth, OdDbLSStroke::CapMode capMode = OdDbLSStroke::kLSCapsClosed);
    /** \details
      Inserts new stroke into this stroke pattern component as copy of existing stroke.

      \param pStroke [in]  Stroke for clone.
      \returns
      Returns pointer to newly added stroke which could be used for additional modifications.
    */
    OdDbLSStroke *insertStroke(const OdDbLSStroke *pStroke);
    /** \details
      Returns stroke pointer from this stroke pattern component.

      \param nStroke [in]  Stroke index.
    */
    OdDbLSStroke *stroke(OdUInt32 nStroke) const;

    /** \details
      Removes existing stroke from this stroke pattern component.

      \param nStroke [in]  Stroke index.
    */
    void deleteStroke(OdUInt32 nStroke);

    /** \details
      Returns count of strokes in this stroke pattern component.
    */
    OdUInt32 numberStrokes() const;

    /** \details
      Returns true if single segment mode is enabled.
    */
    bool isSingleSegment() const;
    /** \details
      Enables single segment mode.

      \param bSet [in]  New mode to set.
    */
    void setIsSingleSegment(bool bSet);

    /** \details
      Returns iteration limit.
    */
    OdInt32 iterationLimit() const;
    /** \details
      Sets iteration limit.

      \param nLimit [in]  New limit to set.
    */
    void setIterationLimit(OdInt32 nLimit);

    /** \details
      Returns phase for distance phase mode.

      \remarks
      Null for non-distance phase modes.
    */
    double phase() const;
    /** \details
      Sets phase for distance phase mode.

      \param fPhase [in]  New phase value to set.
    */
    void setPhase(double fPhase);

    /** \details
      Returns current phase mode.
    */
    PhaseMode phaseMode() const;
    /** \details
      Resets current phase mode.

      \param mode [in]  New phase mode to set.
    */
    void setPhaseMode(PhaseMode mode);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSStrokePatternComponent object pointers.
*/
typedef OdSmartPtr<OdDbLSStrokePatternComponent> OdDbLSStrokePatternComponentPtr;

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSSTROKEPATTERNCOMPONENT_H
