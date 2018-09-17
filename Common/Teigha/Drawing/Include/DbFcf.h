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




#ifndef DBFCF_H
#define DBFCF_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbDimStyleTableRecord.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents feature control frames in an OdDbDatabase instance.

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbFcf : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbFcf);

  OdDbFcf();

  enum whichLine 
  {
    kAll = -1
  };
  
  /** \details
    Sets the text string of this feature control frame (DXF 1).
    
    \param fcfText [in]  Text of this feature control frame.
    
    \remarks
    The following escape sequences are used to specify the feature control frame:

    <table>
    Sequence        Meaning
    \n	            newline 
    {\\Fgdt;a}	    angularity
    {\\Fgdt;b}	    perpendicularity
    {\\Fgdt;c}	    flatness
    {\\Fgdt;d}	    profile of a surface
    {\\Fgdt;e}	    circularity
    {\\Fgdt;f}	    parallelism
    {\\Fgdt;g}	    cylindricity
    {\\Fgdt;h}	    circular runout
    {\\Fgdt;i}	    symmetry
    {\\Fgdt;j}	    positional (+)
    {\\Fgdt;k}	    profile of a line
    {\\Fgdt;l}	    least material condition
    {\\Fgdt;m}	    maximum material condition
    {\\Fgdt;n}	    diameter
    {\\Fgdt;p}	    positional(P)
    {\\Fgdt;r}	    concentricity
    {\\Fgdt;s}	    regardless of feature size
    {\\Fgdt;t}	    total runout
    {\\Fgdt;u}	    straightness
    %%v	            vertical side of frame box
    </table>
  */
  virtual void setText(
    const OdString& fcfText);
  
  /** \details
    Returns the text string of this feature control frame (DXF 1).

    \param lineNo [in]  Index of the line desired.

    \remarks
    The following escape sequences are used to specify the feature control frame:
    
    <table>
    Sequence        Meaning
    \n	            newline 
    {\\Fgdt;a}	    angularity
    {\\Fgdt;b}	    perpendicularity
    {\\Fgdt;c}	    flatness
    {\\Fgdt;d}	    profile of a surface
    {\\Fgdt;e}	    circularity
    {\\Fgdt;f}	    parallelism
    {\\Fgdt;g}	    cylindricity
    {\\Fgdt;h}	    circular runout
    {\\Fgdt;i}	    symmetry
    {\\Fgdt;j}	    positional (+)
    {\\Fgdt;k}	    profile of a line
    {\\Fgdt;l}	    least material condition
    {\\Fgdt;m}	    maximum material condition
    {\\Fgdt;n}	    diameter
    {\\Fgdt;p}	    positional (P)
    {\\Fgdt;r}	    concentricity
    {\\Fgdt;s}	    regardless of feature size
    {\\Fgdt;t}	    total runout
    {\\Fgdt;u}	    straightness
    %%v	            vertical side of frame box
    </table>
      
    \remarks
    lineNo is not currently implemented. All lines of text will be returned, separated by \n.
  */
  virtual const OdString text(
    int lineNo = kAll) const;

  /** \details
    Sets the insertion point of this feature control frame (WCS equivalent of DXF 10).

    \param insPoint [in]  Insertion point.

    \remarks
    The insertion point is the middle of the left edge of the feature control frame.
  */
  virtual void setLocation(
    const OdGePoint3d& insPoint);

  /** \details
    Returns the insertion point of this feature control frame (WCS equivalent of DXF 10).
    
    \remarks
    The insertion point is the middle of the left edge of the feature control frame.
  */
  virtual OdGePoint3d location() const;

  /** \details
    Sets the orientation vectors of this feature control frame (DXF 210 and 11).
    \param normal [in]  WCS normal to plane of feature control frame.
    \param direction [in]  WCS X-axis direction vector of feature control frame. 
  */
  virtual void setOrientation(
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction);

  /** \details
    Returns the WCS normal to the plane of this feature control frame (DXF 210).
  */
  virtual OdGeVector3d normal() const;

  /** \details
    Returns the WCS X-axis direction vector of this feature control frame (DXF 11).
  */
  virtual OdGeVector3d direction() const;

  /** \details
    Sets the dimension style (OdDbDimStyleTableRecord) to used by this feature control frame (DXF 3).
    
    \param dimStyleId [in]  Object ID of the dimension style.
  */
  virtual void setDimensionStyle(
    OdDbHardPointerId dimStyleId);

  /** \details
    Returns the dimension style (OdDbDimStyleTableRecord) used by this feature control frame (DXF 3).
  */
  virtual OdDbHardPointerId dimensionStyle() const;

  /** \details
    Returns the DIMCLRD value of this object.  
    \remarks
    DIMCLRD specifies the color of dimension lines, leader lines, frames, and arrowheads. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   OdCmColor dimclrd() const;

  /** \details
    Returns the DIMCLRT value of this object.  
    \remarks
    DIMCLRT specifies the color of dimension text. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
  */
  OdCmColor dimclrt() const;

  /** \details
    Returns the DIMGAP value of this object.  
    \remarks
    DIMGAP specifies the gap between dimension text and dimension lines.

    A negative value for DIMGAP draws a reference box round the dimension text.

    Text is placed inside a dimension line only if there will be two line segments
    each as long as abs(DIMGAP). 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   double dimgap() const;

  /** \details
    Returns the DIMSCALE value of this object.  
    \remarks
    DIMSCALE is an scale factor applied to all dimension variables that
    regulate sizes, offsets, and distances.

    DIMSCALE serves as an overall "volume control" for dimensions
    and leaders.

    <table>
    Value        Description
    0,0          A "reasonable" value is computed for model space viewports.
    > 0.0        If set to the inverse of the plot scale, plotted dimensions will be at the sizes specified by the other dimension variables. 
    </table>
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   double dimscale() const;

  /** \details
    Returns the DIMTXSTY value of this object.  
    \remarks
    DIMTXSTY specifies the text style of the dimension text. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   OdDbObjectId dimtxsty() const;

  /** \details
    Returns the DIMTXT value of this object.  
    \remarks
    DIMTXT specifies the size of the dimension text. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   double dimtxt() const;

  /** \details
    Sets the DIMCLRD override for this object. 
    \param val [in] New value for DIMCLRD.   
    \remarks
    DIMCLRD specifies the color of dimension lines, leader lines, frames, and arrowheads. 
  */
  void setDimclrd(
    const OdCmColor& val);

  /** \details
    Sets the DIMCLRT override for this object. 
    \param val [in] New value for DIMCLRT.   
    \remarks
    DIMCLRT specifies the color of dimension text. 
  */
   void setDimclrt(
    const OdCmColor& val);

  /** \details
    Sets the DIMGAP override for this object. 
    \param val [in] New value for DIMGAP.   
    \remarks
    DIMGAP specifies the gap between dimension text and dimension lines.

    A negative value for DIMGAP draws a reference box round the dimension text.

    Text is placed inside a dimension line only if there will be two line segments
    each as long as abs(DIMGAP). 
  */
   void setDimgap(
    double val);

  /** \details
    Sets the DIMSCALE override for this object. 
    \param val [in] New value for DIMSCALE.   
    \remarks
    DIMSCALE is an scale factor applied to all dimension variables that
    regulate sizes, offsets, and distances.

    DIMSCALE serves as an overall "volume control" for dimensions
    and leaders.

    <table>
    Value        Description
    0.0          A "reasonable" value is computed for model space viewports.
    > 0.0        If set to the inverse of the plot scale, plotted dimensions will be at the sizes specified by the other dimension variables. 
    </table>
  */
   void setDimscale(
    double val);

  /** \details
    Sets the DIMTXSTY override for this object. 
    \param val [in] New value for DIMTXSTY.   
    \remarks
    DIMTXSTY specifies the text style of the dimension text. 
  */
   void setDimtxsty(
    OdDbObjectId val);

  /** \details
    Sets the DIMTXT override for this object. 
    \param val [in] New value for DIMTXT.   
    \remarks
    DIMTXT specifies the size of the dimension text. 
  */
   void setDimtxt(
    double val);

  virtual OdResult subGetClassID(void* pClsid) const ODRX_OVERRIDE;

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE;
  
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  /** \details
     This function is an override for OdDbEntity::subSetDatabaseDefaults() to set 
     the dimension style of this entity to the current style for the specified database.
  */
  virtual void subSetDatabaseDefaults(OdDbDatabase *pDb, bool doSubents) ODRX_OVERRIDE;

  /** \details
    Returns the WCS bounding points of this feature control frame.
    
    \param boundingPoints [out]  Receives the bounding points.
    
    \remarks
    The points are returned as follows:
    
    <table>
    Point                Corner
    boundingPoints[0]    Top left
    boundingPoints[1]    Top right
    boundingPoints[2]    Bottom right
    boundingPoints[3]    Bottom left
    </table>
  */
  virtual void getBoundingPoints(
    OdGePoint3dArray& boundingPoints) const ;

  virtual void subClose() ODRX_OVERRIDE;

  virtual void modified(const OdDbObject* pObject) ODRX_OVERRIDE;

  /** \details
    Copies the dimension style settings, including overrides, of this entity into the specified
    dimension style table record.
    
    \param pRecord [out]  Receives the effective dimension style data.
    
    \remarks
    The copied data includes the dimension style data with all applicable overrides. 
  */
  void getDimstyleData(
    OdDbDimStyleTableRecord *pRecord) const;

  /** \details
    Copies the dimension style settings, including overrides, from the specified
    dimension style table record to this entity.
    
    \param pDimstyle [in]  Pointer to non- database -resident dimension style record.

    \remarks
    The copied data includes the dimension style with all applicable overrides. 
  */
  void setDimstyleData(
    const OdDbDimStyleTableRecord* pDimstyle);
    
  /** \details
    Copies the dimension style settings, including overrides, from the specified
    dimension style table record to this entity.
    
    \param dimstyleID [in]  Database-resident dimension style record.

    \remarks
    The copied data includes the dimension style with all applicable overrides. 
  */
  void setDimstyleData(
    OdDbObjectId dimstyleID);

  /** \details
    Appends the consecutive distinct corner points of this feature control frame to the specified array.
    
    \param ptArray [in/out] Receives corner points.
  */
  virtual void getBoundingPline(
    OdGePoint3dArray& 
    ptArray) const;
  /*
      virtual void getGripPoints(OdGePoint3dArray&, OdDbIntArray&, OdDbIntArray&) const;
      virtual void moveGripPointsAt(const OdDbIntArray&, const OdGeVector3d&);
  */
  
  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData) ODRX_OVERRIDE;

  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary) const ODRX_OVERRIDE;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbFcf object pointers.
*/
typedef OdSmartPtr<OdDbFcf> OdDbFcfPtr;

#include "TD_PackPop.h"

#endif

