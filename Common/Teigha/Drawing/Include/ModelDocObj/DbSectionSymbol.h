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

#ifndef OD_SECTIONSYMBOL_H
#define OD_SECTIONSYMBOL_H

#include "DbViewSymbol.h"
#include "TD_PackPush.h"

/** \details
    This class represents Section Symbol entities.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbSectionSymbol : public OdDbViewSymbol
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSectionSymbol);

  /** \details
    Default constructor. Creates an OdDbDetailSymbol instance.
  */
  OdDbSectionSymbol();
  ~OdDbSectionSymbol();

  /** \details
    Returns the number of points in the section line of this Section Symbol entity.
  */
  int sectionPointsCount() const;

  /** \details
    Gets all points in the section line of this Section Symbol entity.

    \param pts [out]  Receives section line points.
  */
  void getSectionPoints(OdGePoint3dArray& pts) const;

  /** \details
    Gets values of label offset from all vertices.

    \param offsets [out]  Receives offset vectors.
  */
  void getLabelOffsets(OdGeVector3dArray& offsets) const;

  /** \details
    Gets the section line point at the specified index. 

    \param idx [in]  Index of the section line vertex.
    \param pt [out]  Receives the point at the index idx.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
  */
  OdResult getSectionPointAt(int idx, OdGePoint3d& pt) const;

  /** \details
    Gets the section vertex bulge parameter at the specified index. 

    \param idx [in]  Index of the section symbol vertex.
    \param bulge [out]  Receives the bulge at the index idx.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
  */
  OdResult getBulgeAt(int idx, double& bulge) const;

  /** \details
    Gets the label at the specified vertex index. 

    \param idx [in]  Index of the section symbol vertex.
    \param sName [out]  Receives the string that represents label.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
  */
  OdResult getLabelNameAt(int idx, OdString& sName) const;


  /** \details
    Gets the label offset value from the specified vertex. 

    \param idx [in]  Index of the section symbol vertex.
    \param offset [out]  Receives the offset value.

    \returns
    eOk value if successful, eOutOfRange if  idx is out of range (idx > sectionPointsCount()).
  */
  OdResult getLabelOffsetAt(int idx, OdGeVector3d& offset) const;

  /** \details
    Checks whether the view direction of the section symbol is left.

    \returns
    Returns true only if the view direction is left, and false otherwise.
  */
  bool isViewDirectionLeft() const;
  void setViewDirectionLeft(bool bLeft);

  /** \details
    Checks whether the section symbol defines a half or full section.

    \returns
    Returns true only if the section symbol defines a half section cut, false for full sections.
  */
  bool isHalfSection() const;
  void setIsHalfSection(bool bHalf);

  // Methods to use internal only :

  /** \details
    Appends new point of section line.

    \param pt [in]  Point of the section line.
    \param bulge [in]  Value of bulge.

    \returns
    eOk value if successful.
  */
  OdResult addSectionPoint(const OdGePoint3d& pt, double bulge = 0);

  /** \details
    Modifies point by index.

    \param idx [in]  Index of the section symbol vertex.
    \param pt [in]  New value of point.
    \param bulge [in]  Value of bulge.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
  */
  OdResult setSectionPointAt(int idx, const OdGePoint3d& pt, double bulge = 0);

 /** \details
    Modifies label name by index.

    \param idx [in]  Index of the section symbol vertex.
    \param name [in]  Name of label.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
 */
  OdResult setLabelNameAt(int idx, const OdString & name);

 /** \details
    Modifies label offset by index.

    \param idx [in]  Index of the section symbol vertex.
    \param offset [in]  Value of label's offset.

    \returns
    eOk value if successful, eOutOfRange if idx is out of range (idx > sectionPointsCount()).
 */
  OdResult setLabelOffsetAt(int idx, const OdGeVector3d& offset);

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable methods :
  virtual bool subWorldDraw(OdGiWorldDraw* pWorldDraw) const;

  // Methods to use internal only :
  //OdResult setSectionPoints(const OdGePoint3dArray& pts);
  //OdResult setSectionPoints(const OdGePoint3dArray& pts, const OdGeDoubleArray& bulges);
  //OdResult removeSectionPointAt(int idx);
  //void clearSectionPoints();
  //OdResult setLabelNames(const OdArray<OdString>& names);
  //OdResult setLabelOffsets(const OdGeVector3dArray& offsets);
  //void resetLabelOffsets(bool allOffsets = true);
  //bool flipDirection();
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSectionSymbol object pointers.
*/
typedef OdSmartPtr<OdDbSectionSymbol> OdDbSectionSymbolPtr;

#include "TD_PackPop.h"

#endif // OD_SECTIONSYMBOL_H
