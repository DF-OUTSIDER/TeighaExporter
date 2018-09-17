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

#ifndef OD_DBABSTRACTVIEWPORTDATA_H
#define OD_DBABSTRACTVIEWPORTDATA_H

#include "AbstractViewPE.h"

class OdGsView;

/** \details
    This class is the base class of Protocol Extension classes for OdDbViewport and OdDbViewportTableRecord objects.
    
    Corresponding C++ library: TD_Db
    
    \sa
    OdDbAbstractViewTableRecord

    \remarks
    Only the following objects are supported:
    
    *  OdDbViewport
    *  OdDbViewportTableRecord
    *  Derivatives of one of the above

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractViewportData : public OdAbstractViewPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAbstractViewportData);

  /** \details
    Sets the parameters for the specified viewport object according to the arguments.
	\param pViewport [in] Input pointer to the destination viewport object.
    \param pSourceView [in] Input pointer to the source viewport object.
  */
  virtual void setProps(OdRxObject* pViewport, const OdRxObject* pSourceView) const;

  // OdAbstractViewPE-inherited methods

  TD_USING(OdAbstractViewPE::setUcs);
  /** \details
    Sets the parameters for the specified view object according to the arguments.
    \param pSourceView [in] Input pointer to the source view object.
    \param pDestinationView [in] Input pointer to the destination view object.
  */
  virtual void setUcs(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;
  /** \details
    Returns true if and only if there is a view offset associated with the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool hasViewOffset(const OdRxObject* pViewport) const;
  /** \details
    Applies plot settings to the view.
	\param pDestinationView [in] Input pointer to the destination view object.
    \param pSourceView [in] Input pointer to the source view object.
    \returns Returns true if plot settings were applied successfully.
  */
  virtual bool applyPlotSettings(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;

  /** \details
    Returns compatible visual style or background object for the specified viewport.
    \param pViewport [in] Input pointer to the viewport object.
    \param pCopyObject [in] Input object ID.
    \returns Returns compatible objectID or null if no compatible objects are found.
  */
  virtual OdDbStub* compatibleCopyObject(const OdRxObject* pViewport, OdDbStub* pCopyObject) const;

  //

  /** \details
    Returns true if and only if the UCS that is associated with the specified viewport object will become active
    with activation of the viewport object.

    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isUcsSavedWithViewport(const OdRxObject* pViewport) const = 0;

  /** \details
    Controls activation of the UCS that is associated with the specified viewport object 
    when the viewport object is activated.
    
    \param pViewport [in] Input pointer to the viewport object.
    \param ucsPerViewport [in] Input activation of the UCS flag.
  */
  virtual void setUcsPerViewport( OdRxObject* pViewport, bool ucsPerViewport) const = 0;

  /** \details
    Returns true if and only if UCS follow mode is on for the specified viewport object.
    
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isUcsFollowModeOn(const OdRxObject* pViewport) const = 0;

  /** \details
    Controls UCS follow mode for the specified viewport object.
    
    \param pViewport [in] Input pointer to the viewport object.
    \param ucsFollowMode [in] Input UCS follow mode flag.
  */
  virtual void setUcsFollowModeOn(OdRxObject* pViewport, bool ucsFollowMode) const = 0;

  /** \details
    Returns the circle zoom percent for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \returns Returns a value in the range [1..20000].
  */
  virtual OdUInt16 circleSides(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets circle zoom percent for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param circleSides [in] Input circle zoom percent. [1..20000]
  */
  virtual void setCircleSides(OdRxObject* pViewport, OdUInt16 circleSides) const = 0;

  /** \details
    Returns true if and only if the grid is on for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isGridOn(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the grid for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridOn [in] Input grid usage flag.
  */
  virtual void setGridOn(OdRxObject* pViewport, bool gridOn) const = 0;

  /** \details
    Returns the grid increment for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdGeVector2d gridIncrement(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the grid increment for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridIncrement [in] Input grid increment.
  */
  virtual void setGridIncrement(OdRxObject* pViewport, const OdGeVector2d& gridIncrement) const = 0;

  /** \details
    Returns whether the grid is bound to limits for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isGridBoundToLimits(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls whether the grid is bound to limits for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridDispFlag [in] Input flag for whether the grid is bound to limits.
  */
  virtual void setGridBoundToLimits(OdRxObject* pViewport, bool gridDispFlag) const = 0;

  /** \details
    Returns the adaptive grid flag for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isGridAdaptive(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the adaptive grid flag for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridDispFlag [in] Input adaptive grid flag.
  */
  virtual void setGridAdaptive(OdRxObject* pViewport, bool gridDispFlag) const = 0;

  /** \details
    Returns wheether grid subdivision is restricted for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isGridSubdivisionRestricted(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls whether grid subdivision is restricted for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridDispFlag [in] Input flag for whether grid subdivision is restricted.
  */
  virtual void setGridSubdivisionRestricted(OdRxObject* pViewport, bool gridDispFlag) const = 0;

  /** \details
    Returns the grid follow flag for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isGridFollow(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the grid follow flag for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param gridDispFlag [in] Input grid follow flag.
  */
  virtual void setGridFollow(OdRxObject* pViewport, bool gridDispFlag) const = 0;

  /** \details
    Returns the major grid lines frequency for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdInt16 gridMajor(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the major grid lines frequency for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param nMajor [in] Input major grid lines frequency.
  */
  virtual void setGridMajor(OdRxObject* pViewport, OdInt16 nMajor) const = 0;

  /** \details
    Returns true if and only if the UCS icon is visible for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isUcsIconVisible(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the UCS icon visibility for the specified viewport object.
    
    \param pViewport [in] Input pointer to the viewport object.
    \param iconVisible [in] Input visibility flag.
  */
  virtual void setUcsIconVisible(OdRxObject* pViewport, bool iconVisible) const = 0;

  /** \details
    Returns true if and only if the UCS icon is at the UCS orgin for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isUcsIconAtOrigin(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the UCS icon display at the UCS orgin for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param atOrigin [in] Input UCS icon flag.
    
    \remarks
    If atOrigin is true, the UCS icon is at the UCS origin for this viewport object. If false, it is at the 
    corner for this viewport object.
    
  */
  virtual void setUcsIconAtOrigin(OdRxObject* pViewport, bool atOrigin) const = 0;

  /** \details
    Returns true if and only if the snap mode is on for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isSnapOn(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the snap mode for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param snapOn [in] Input snap mode flag.
  */
  virtual void setSnapOn(OdRxObject* pViewport, bool snapOn) const = 0;

  /** \details
    Returns true if and only if isometric snap style is on for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual bool isSnapIsometric(const OdRxObject* pViewport) const = 0;
  /** \details
    Controls the isometric snap style for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param snapIsometric [in] Input isometric snap style flag.
  */
  virtual void setSnapIsometric(OdRxObject* pViewport, bool snapIsometric) const = 0;

  /** \details
    Returns the UCS snap angle for the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual double snapAngle(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the UCS snap angle for the specified viewport object (DXF 50).
    \param pViewport [in] Input pointer to the viewport object.
    \param snapAngle [in] Input snap angle.
    \remarks All angles are expressed in radians.
  */
  virtual void setSnapAngle(OdRxObject* pViewport, double snapAngle) const = 0;

  /** \details
    Returns the UCS snap base point of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdGePoint2d snapBase(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the UCS snap base point of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param snapBase [in] Input snap base.
  */
  virtual void setSnapBase(OdRxObject* pViewport, const OdGePoint2d& snapBase) const = 0; 

  /** \details
    Returns the snap increment of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdGeVector2d snapIncrement(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the snap increment of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param snapIncrement [in] Input snap increment.
  */
  virtual void setSnapIncrement(OdRxObject* pViewport, const OdGeVector2d& snapIncrement) const = 0;

  /** \details
    Returns the snap IsoPair of this viewport object.
    \param pViewport [in] Input pointer to the viewport object.

    \remarks
    snapIsoPair() returns one of the following:
    
    <table>
    Value    Description
    0        Left isoplane
    1        Top isoplane
    2        Right isoplane
    </table>
  */
  virtual OdUInt16 snapIsoPair(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the snap IsoPair of this viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param snapIsoPair [in] Input snap IsoPair.
    \remarks
    snapIsoPair must be one of the following:
    
    <table>
    Value    Description
    0        Left isoplane
    1        Top isoplane
    2        Right isoplane
    </table>
  */
  virtual void setSnapIsoPair(OdRxObject* pViewport, OdUInt16 snapIsoPair) const = 0;

  /** \details
    Returns the brightness value of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual double brightness(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the brightness value to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param brightness [in] Input brightness value.
  */
  virtual void setBrightness(OdRxObject* pViewport, double brightness) const = 0;

  /** \details
    Returns the contrast value of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual double contrast(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the contrast value to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param contrast [in] Input brightness value.
  */
  virtual void setContrast(OdRxObject* pViewport, double contrast) const = 0;

  /** \details
    Returns the ambient light color of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdCmColor ambientLightColor(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the ambient light color to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param color [in] Input ambient light color.
  */
  virtual void setAmbientLightColor(OdRxObject* pViewport, const OdCmColor& color) const = 0;

  /** \details
    Returns the sun ID of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdDbStub *sunId(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the sun object to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param pSun [in] Input pointer to the sun object.
  */
  virtual OdDbStub *setSun(OdRxObject* pViewport, OdRxObject* pSun) const = 0;

  /** \details
    Returns the tone operator parameters of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param params [out] Output tone operator parameters.
  */
  virtual void toneOperatorParameters(const OdRxObject* pViewport, OdGiToneOperatorParameters &params) const = 0;
  /** \details
    Sets the tone operator parameters to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param params [in] Input tone operator parameters.
  */
  virtual void setToneOperatorParameters(OdRxObject* pViewport, const OdGiToneOperatorParameters &params) const = 0;

  /** \details
    Returns the gsView associated with the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual OdGsView* gsView(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Sets the gsView to the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param pGsView [in] Input pointer to the gsView object.
  */
  virtual void setGsView(OdRxObject* pViewport, OdGsView* pGsView) const = 0;

  /** \details
    Returns the type of the viewcube display of the specified viewport object.
    \param pViewport [in] Input pointer to the viewport object.
  */
  virtual int navvcubedisplay(const OdRxObject* pViewport) const;
  /** \details
    Sets the type of the viewcube display to the specifiedvViewport object.
    \param pViewport [in] Input pointer to the viewport object.
    \param nVal [in] Input type of the viewcube display.
  */
  virtual OdResult setNavvcubedisplay(OdRxObject* pViewport, int nVal) const;

  TD_USING(OdAbstractViewPE::setView);
  /** \details
    Applies data from one view to another view.
	\param pDestinationView [in] Input pointer to the destination view object.
    \param pSourceView [in] Input pointer to the source view object.
  */
  virtual void setView(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbAbstractViewportData object pointers.
*/
typedef OdSmartPtr<OdDbAbstractViewportData> OdDbAbstractViewportDataPtr;


//DOM-IGNORE-BEGIN
class SavePeWriteEnabler
{
  OdDbObjectPtr m_pObj;
public:
  SavePeWriteEnabler(OdRxObject* pRxObj)
  {
    if ((m_pObj = OdDbObject::cast(pRxObj)).isNull())
      return;
    if (m_pObj->isWriteEnabled())
      m_pObj = NULL;
    else
      m_pObj->upgradeOpen(); // standard way for set method impl of PE
  }
  ~SavePeWriteEnabler()
  {
    if (m_pObj.get())
      m_pObj->downgradeOpen();
  }
};
//DOM-IGNORE-END

#endif //#ifndef OD_DBABSTRACTVIEWPORTDATA_H
