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




#ifndef OD_DBLEOD_H
#define OD_DBLEOD_H 1

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DbDimStyleTableRecord.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents Leader entities in an OdDbDatabase instance.

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbLeader : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLeader);

  OdDbLeader();
    
  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  virtual OdGeVector3d normal() const;
  
  /** \details
    Returns the number of vertices in this Leader entity (DXF 76).
  */
  virtual int numVertices() const;

  /** \details
    Appends the specified WCS vertex to this Leader entity.
     
    \param vertex [in]  Vertex to append.
       
    \returns
    Returns true if and only if the vertex was appended.

    The specified vertex is projected onto the plane of this Leader, parallel to its normal.

    \remarks
    If the specified vertex is inside a 2e-10 cube surrounding the
    previous vertex, it will not be appended.
  */
  virtual bool appendVertex(
    const OdGePoint3d& vertex);

  /** \details
    Removes the last vertex of this Leader entity.
  */
  virtual void removeLastVertex();

  /** \details
    Returns the WCS first vertex of this Leader entity.
  */
  virtual OdGePoint3d firstVertex() const;

  /** \details
    Returns the WCS last vertex of this Leader entity.
  */
  virtual OdGePoint3d lastVertex() const;

  /** \details
    Returns the specified WCS vertex of this Leader entity.
    \param vertexIndex [in]  Vertex index.
  */
  virtual OdGePoint3d vertexAt(
    int vertexIndex) const;

  /** \details
    Sets the specified WCS vertex of this Leader entity.

    \param vertexIndex [in]  Vertex index.
    \param vertex [in]  WCS value for the vertex.

    \returns
    Returns true if and only if the vertex was set.
  */
  virtual bool setVertexAt(
    int vertexIndex, 
    const OdGePoint3d& vertex);
  
  /** \details
    Returns true if and only if this Leader entity has an arrowhead (DXF 71).
  */
  bool isArrowHeadEnabled() const;

  /** \details
    Returns true if arrowhead is currently enabled for this leader:
  1. Arrow head is enabled (isArrowHeadEnabled() returns true).
  2. DIMASZ (arrow size) is non-zero
  3. First leader's segment length is greater or equal to (DIMASZ  DIMSCALE  2).
  */
  virtual bool hasArrowHead() const;

  /** \details
    Enables the arrowhead for this Leader entity (DXF 71).
  */
  virtual void enableArrowHead();

  /** \details
    Disables the arrowhead for this Leader entity (DXF 71).
  */
  virtual void disableArrowHead();

  /** \details
    Returns true if and only if this Leader entity has a hookline (DXF 75).
  */
  virtual bool hasHookLine() const;

  /** \details
    Returns true if the hookline is codirectional with the
    OCS X-axis of this Leader entity, or false if it is antidirectional with it.
  */
  virtual bool isHookLineOnXDir() const;

  /** \details
    Sets this Leader entity to use a spline-fit leader line (DXF 72).
  */
  virtual void setToSplineLeader();

  /** \details
    Sets this Leader to use a straight line segments (DXF 72).
  */
  virtual void setToStraightLeader();

  /** \details
    Returns true if and only if this Leader entity uses a spline-fit leader line (DXF 72).
  */
  virtual bool isSplined() const;
  
  /** \details
    Returns the dimension style (OdDbDimStyleTableRecord) used by this Leader entity (DXF 3).
  */
  virtual OdDbHardPointerId dimensionStyle() const;

  /** \details
    Sets the dimension style (OdDbDimStyleTableRecord) to used by this Leader entity (DXF 3).
    
    \param dimStyleId [in]  Object ID of the dimension style.
  */
  virtual void setDimensionStyle(
    const OdDbHardPointerId& dimStyleId);
    
  /** \details
    Sets the annotation entity of this Leader entity (DXF 340).
      
    \param annoId [in]  The Object ID of the annotation entity.
    
    \remarks
    This Leader entity is appended to the persistent reactor list of the annotation entity.
    
    The annotation entity must be one of, or a subclass of one of, the following
    
    <table>
    Name        Entity type          Description
    kMText      OdDbMText            MText 
    kFcf        OdDbFcf              Feature control frame (Tolerance)  
    kBlockRef   OdDbBlockReference   Block reference
    </table>

    Other than annoId, the parameters are usually set with evaluateLeader().      
  */
  virtual void attachAnnotation(
    OdDbObjectId annoId);

  /** \details
    Sets the annotation entity of this Leader entity (DXF 340).
      
    \param annoId [in]  The Object ID of the annotation entity.
    \param xDir [in]  WCS X-axis of the annotation entity.
    \param annotationWidth [in]  Annotation entity width.
    \param annotationHeight [in]  Annotation entity height.
    \param hookLineOnXDir [in]  True to set hookline codirectional with the
                       OCS X-axis of this annotation entity, or false to set it antidirectional with it.
    
    \remarks
    This Leader entity is appended to the persistent reactor list of the annotation entity.
    
    The annotation entity must be one of, or a subclass of one of, the following
    
    <table>
    Name        Entity type          Description
    kMText      OdDbMText            MText 
    kFcf        OdDbFcf              Feature control frame (Tolerance)  
    kBlockRef   OdDbBlockReference   Block reference
    </table>

    Other than annoId, the parameters are usually set with evaluateLeader().      
  */
  virtual void attachAnnotation(
    OdDbObjectId annoId,
    OdGeVector3d xDir,
    double annotationWidth,
    double annotationHeight,
    bool hookLineOnXDir);

  /** \details
    Removes this Leader entity from the persistent reactor list of its annotation entity,
    and sets the annotation Object ID to NULL.
  */
  virtual void detachAnnotation();

  /** \details
     Returns the annotation Object ID of the annotation entity associated with this Leader entity (DXF 340).
  */
  virtual OdDbObjectId annotationObjId() const;

  /** \details
    Returns the OCS X-axis of the annotation entity associated with this Leader entity (DXF 211).
  */
  virtual OdGeVector3d annotationXDir() const;

  /** \details
    Returns the annotation offset of this Leader entity (DXF 213).
    
    \remarks
    The annotation offset determines the final leader endpoint of this Leader entity
    
    <table>
    annoType()     Final Leader Endpoint
    kMText         annotationOffset() + OdDbMText::location() ± OdGeVector3d(dimgap(), 0, 0) 
    kFcf           annotationOffset() + OdDbFcf::location()   ± OdGeVector3d(dimgap(), 0, 0)
    kBlockRef      annotationOffset() + OdDbBlockReference::position()
    kNoAnno        lastVertex() 
    </table>
    
    \remarks
    dimgap() is subtracted if the annotation is to the right of the Leader entity (isHookLikeOnXDir() == true)
    and added if it is to the left.
  */
  virtual OdGeVector3d annotationOffset() const;

  /** \details
    Sets the annotation offset of this leader (DXF 213).
    
    \param offset [in]  Annotation offset.

    \remarks
    The annotation offset determines the final leader endpoint of this Leader entity
    
    <table>
    annoType()     Final Leader Endpoint
    kMText         annotationOffset() + OdGeVector3d(± dimgap(), 0, 0) + OdDbMText::location()
    kFcf           annotationOffset() + OdGeVector3d(± dimgap(), 0, 0) + OdDbFcf::location()
    kBlockRef      annotationOffset() + OdDbBlockReference::position()
    kNoAnno        lastVertex() 
    </table>
    
    \remarks
    dimgap() is added if isHookLikeOnXDir() is true, subtracted if it is false.
  */
  virtual void setAnnotationOffset(
    const OdGeVector3d& offset);
  
  /** \details
    The type of annotation used by a Leader entity. 
  */
  
  enum AnnoType
  { 
    kMText      = 0,   // OdDbMText
    kFcf        = 1,   // OdDbFcf
    kBlockRef   = 2,   // OdDbBlockReference.
    kNoAnno     = 3    // No annotation.
  };

  /** \details
    Returns the annotation type associated with this Leader entity (DXF 73).

    \remarks
    annoType() returns one of the following:
    
    <table>
    Name        Value    Entity type          Description
    kMText      0        OdDbMText            MText 
    kFcf        1        OdDbFcf              Feature control frame (Tolerance)  
    kBlockRef   2        OdDbBlockReference   Block reference
    kNoAnno     3        --                   No annotation
    </table>
  */
  AnnoType annoType() const;

  /** \details
      Returns the height of the annotation entity associated with this Leader entity (DXF 40).
  */
  double annoHeight() const;

  /** \details
      Returns the width of the annotation entity associated with this Leader entity (DXF 41).
  */
  double annoWidth() const;
  
  /** \details
    Returns the DIMASZ value of this object.  
    \remarks
    DIMASZ specifies the arrowhead size for dimensions. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual double dimasz() const;

  /** \details
    Returns the DIMCLRD value of this object.  
    \remarks
    DIMCLRD specifies the color of dimension lines, leader lines, and arrowheads. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual OdCmColor dimclrd() const;

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
   virtual double dimgap() const;

  /** \details
    Returns the DIMLWD value of this object.  
    \remarks
    DIMLWD specifies the lineweight for dimension lines. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual OdDb::LineWeight dimlwd() const;

  /** \details
    Returns the DIMLDRBLK value of this object.  
    \remarks
    DIMLDRBLK specifies the arrowhead block at the ends of leader lines.

    A null value specifies the default arrowhead is to be used. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual OdDbObjectId dimldrblk() const;

  /** \details
    Returns the DIMSAH value of this object.  
    \remarks
    DIMSAH controls the generation of separate arrowheads for dimension lines.

    <table>
    Value    Arrowhead Blocks
    false    DIMBLK
    true     DIMBLK1 and DIMBLK2 
    </table>
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
  virtual bool dimsah() const;

  /** \details
    Returns the DIMSCALE value of this object.  
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
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual double dimscale() const;

  /** \details
    Returns the DIMTAD value of this object.  
    \remarks
    DIMTAD specifies the dimension text vertical position.

    <table>
    Value        Description
    0            Centered about the dimension line.
    1            Above the dimension line, unless DIMTIH==1 and the dimension line is not horizontal.
    2            Side of the dimension line farthest from the defining points.
    3            JIS standard. 
    </table>
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual int dimtad() const;

  /** \details
    Returns the DIMTXSTY value of this object.  
    \remarks
    DIMTXSTY specifies the text style of the dimension text. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual OdDbObjectId dimtxsty() const;

  /** \details
    Returns the DIMTXT value of this object.  
    \remarks
    DIMTXT specifies the size of the dimension text. 
    
    \remarks
    The value from the dimension style of this object will be returned unless 
    that value has been overridden, in which case, the override value will be returned. 
    
  */
   virtual double dimtxt() const;
  
  /** \details
    Sets the DIMASZ override for this object. 
    \param val [in] New value for DIMASZ.   
    \remarks
    DIMASZ specifies the arrowhead size for dimensions. 
  */
   virtual void setDimasz(
    double val);

  /** \details
    Sets the DIMCLRD override for this object. 
    \param val [in] New value for DIMCLRD.   
    \remarks
    DIMCLRD specifies the color of dimension lines, leader lines, and arrowheads. 
  */
   virtual void setDimclrd(
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
   virtual void setDimgap(
    double val);

  /** \details
    Sets the DIMLDRBLK override for this object. 
    \param val [in] New value for DIMLDRBLK.   
    \remarks
    DIMLDRBLK specifies the arrowhead block at the ends of leader lines.

    A null value specifies the default arrowhead is to be used. 
  */
   virtual void setDimldrblk(
    OdDbObjectId val);
  
  /** \details
    Sets the DIMLDRBLK override for this object. 
    \param val [in] New value for DIMLDRBLK.   
    \remarks
    DIMLDRBLK specifies the arrowhead block at the ends of leader lines.

    A null value specifies the default arrowhead is to be used. 
  */
  virtual void setDimldrblk(
    const OdString& val);

  /** \details
    Sets the DIMLWD override for this object. 
    \param val [in] New value for DIMLWD.   
    \remarks
    DIMLWD specifies the lineweight for dimension lines. 
  */
   virtual void setDimlwd(
    OdDb::LineWeight val); 

  /** \details
    Sets the DIMSAH override for this object. 
    \param val [in] New value for DIMSAH.   
    \remarks
    DIMSAH controls the generation of separate arrowheads for dimension lines.

    <table>
    Value    Arrowhead Blocks
    false    DIMBLK
    true     DIMBLK1 and DIMBLK2 
    </table>
  */
   virtual void setDimsah(
    bool val);

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
   virtual void setDimscale(
    double val);

  /** \details
    Sets the DIMTAD override for this object. 
    \param val [in] New value for DIMTAD.  [0..3] 
    \remarks
    DIMTAD specifies the dimension text vertical position.

    <table>
    Value        Description
    0            Centered about the dimension line.
    1            Above the dimension line, unless DIMTIH==1 and the dimension line is not horizontal.
    2            Side of the dimension line farthest from the defining points.
    3            JIS standard. 
    </table>
  */
   virtual void setDimtad(
    int val);

  /** \details
    Sets the DIMTXSTY override for this object. 
    \param val [in] New value for DIMTXSTY.   
    \remarks
    DIMTXSTY specifies the text style of the dimension text. 
  */
   virtual void setDimtxsty(
    OdDbObjectId val);

  /** \details
    Sets the DIMTXT override for this object. 
    \param val [in] New value for DIMTXT.   
    \remarks
    DIMTXT specifies size of the dimension text. 
  */
   virtual void setDimtxt(
    double val);
    
  virtual OdResult subGetClassID(
    void* pClsid) const;
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const ODRX_OVERRIDE;

  ODRX_SEALED_VIRTUAL void subViewportDraw(
    OdGiViewportDraw* pVd) const ODRX_OVERRIDE;

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE; 

  virtual void subClose() ODRX_OVERRIDE;

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const ODRX_OVERRIDE;
 
  virtual bool isPlanar() const;

  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;

  virtual OdResult getStartParam(
    double& startParam) const;

  virtual OdResult getEndParam (
  double& endParam) const;

  virtual OdResult getStartPoint(
    OdGePoint3d& startPoint) const;

  virtual OdResult getEndPoint(
    OdGePoint3d& endPoint) const;

  virtual OdResult getPointAtParam(
    double param, 
    OdGePoint3d& pointOnCurve) const ODRX_OVERRIDE;

  virtual OdResult getParamAtPoint(
    const OdGePoint3d& pointOnCurve, 
    double& param) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xform,
    OdDbEntityPtr& pEnt) const ODRX_OVERRIDE;

  virtual OdResult getDistAtParam( double param, double& dist ) const;

  virtual OdResult getParamAtDist( double dist, double& param ) const;

  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector3d& firstDeriv) const;

  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector3d& secondDeriv) const;

  virtual OdResult getArea(
    double& area) const;

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  /** \details
     This function is an override for OdDbEntity::subSetDatabaseDefaults() to set 
     the dimension style of this entity to the current style for the specified database.
  */
  void subSetDatabaseDefaults(
    OdDbDatabase *pDb,
    bool doSubents);
  
  /** \details
    Updates this the geometry of this leader per its relationship to its
    annotation entity.
  */
  virtual OdResult evaluateLeader();

  
  /** \details
    Support for persistent reactor to annotation entity.
  */
  virtual void modifiedGraphics(
    const OdDbObject* pObject);
  
  virtual void erased(
    const OdDbObject* pObject, 
    bool erasing = true);

  /** \details
    Copies the dimension style settings, including overrides, of this entity into the specified
    dimension style table record.
    
    \param pRecord [out]  Receives the effective dimension style data associated with entity.
    
    \remarks
    The copied data includes the dimension style data with all applicable overrides. 
  */
  void getDimstyleData(
    OdDbDimStyleTableRecord *pRecord) const;

  /** \details
    Copies the dimension style settings, including overrides, from the specified
    dimension style table record to this entity.
    
    \param pDimstyle [in]  Pointer to a non- database -resident dimension style record.

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
    Sets the plane to contain this Leader entity.

    \param leaderPlane [in]  Leader plane.
      
    \remarks
    Any associativity of this Leader entity will be broken.
  */
  virtual void setPlane(
    const OdGePlane& leaderPlane);

  OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
    OdGsMarker gsMark, 
    const OdGePoint3d& ,
    const OdGeMatrix3d& , 
    OdDbFullSubentPathArray& subentPaths, 
    const OdDbObjectIdArray* pEntAndInsertStack ) const;
  OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
    OdGsMarkerArray& gsMarkers) const;

    enum gsMarkType {
    kArrowMark          = 3,
    kLeaderLineMark     = kArrowMark  + 1
  };
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbLeader object pointers.
*/
typedef OdSmartPtr<OdDbLeader> OdDbLeaderPtr;

#include "TD_PackPop.h"

#endif

