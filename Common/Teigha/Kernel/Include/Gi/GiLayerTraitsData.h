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

#ifndef _ODGILAYERTRAITSDATA_INCLUDED_
#define _ODGILAYERTRAITSDATA_INCLUDED_

#include "Gi/GiNonEntityTraits.h"
#include "CmColorBase.h"
#include "GiDrawImpl.h"
#include "GiDummyGeometry.h"
class OdGiBaseVectorizer;

class OdDbStub;

#include "TD_PackPush.h"

/** \details
    This class provides an implementation of the OdGiLayerTraits::color() function.
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class OdGiLayerTraits_ : public OdGiLayerTraits
{
  virtual OdCmEntityColor trueColor() const = 0;
protected:
  OdCmEntityColor color() const { return trueColor(); }
};

/** \details
    This class provides an implementation of the OdGisubEntityTraits::color() function.
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class OdGiSubEntityTraits_ : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
{
  virtual OdCmEntityColor trueColor() const = 0;
protected:
  OdUInt16 color() const { return trueColor().colorIndex(); }
};

/** \details

    <group !!RECORDS_TD_APIRef>
*/
struct OdGiLayerTraitsData
{
  OdGiLayerTraitsData()
    : m_flags(OdGiLayerTraits::kPlottable)
    , m_lineweight(OdDb::kLnWtByLwDefault)
    , m_plotStyleNameType(OdDb::kPlotStyleNameById)
    , m_color(OdCmEntityColor::kForeground)
    , m_linetypeId(0)
    , m_plotStyleNameId(0)
    , m_materialId(0)
    , m_cmTransparency(OdUInt8(0xFF))
  {
  }
  void operator =(const OdGiLayerTraitsData& other)
  {
    m_flags             = other.m_flags;
    m_lineweight        = other.m_lineweight;
    m_plotStyleNameType = other.m_plotStyleNameType;
    m_color             = other.m_color;
    m_linetypeId        = other.m_linetypeId;
    m_plotStyleNameId   = other.m_plotStyleNameId;
    m_materialId        = other.m_materialId;
    m_cmTransparency    = other.m_cmTransparency;
  }

  OdUInt32 flags() const { return m_flags; }
  OdDb::LineWeight lineweight() const { return m_lineweight; }
  OdDbStub* linetype() const { return m_linetypeId; }
  OdDb::PlotStyleNameType plotStyleNameType() const { return m_plotStyleNameType; }
  OdDbStub* plotStyleNameId() const { return m_plotStyleNameId; }
  const OdCmEntityColor& trueColor() const { return m_color; }
  OdDbStub* materialId() const { return m_materialId; }
  OdCmTransparency transparency() const { return m_cmTransparency; }
  void setTrueColor(const OdCmEntityColor& cl) { m_color = cl; }
  void setLineweight(OdDb::LineWeight lw) { m_lineweight = lw; }
  void setLinetype(OdDbStub* id) { m_linetypeId = id; }
  void setMaterial(OdDbStub* id) { m_materialId = id; }
  void setTransparency(const OdCmTransparency& transparency) { m_cmTransparency = transparency; }

  bool isOff() const { return GETBIT(m_flags, OdGiLayerTraits::kOff); }
  void setOff(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kOff, bVal); }

  bool isPlottable() const { return GETBIT(m_flags, OdGiLayerTraits::kPlottable); }
  void setPlottable(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kPlottable, bVal); }

  bool isLocked() const { return GETBIT(m_flags, OdGiLayerTraits::kLocked); }
  void setLocked(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kLocked, bVal); }

  bool isFrozen() const { return GETBIT(m_flags, OdGiLayerTraits::kFrozen); }
  bool isByBlock() const { return GETBIT(m_flags, OdGiLayerTraits::kByBlock); }

  OdUInt32                m_flags;
  OdDb::LineWeight        m_lineweight;
  OdDb::PlotStyleNameType m_plotStyleNameType;

  OdCmEntityColor         m_color;
  OdDbStub*               m_linetypeId;
  OdDbStub*               m_plotStyleNameId;

  OdDbStub*               m_materialId;
  OdCmTransparency        m_cmTransparency;
};

/** \details
    This class implements OdGiLayerTraits, OdGiWorldDraw, and OdGIViewportDraw together 
	as a single class that uses the OdDbLayerTableRecord::subSetAttributes(), 
	OdDbLayerTableRecord::subWorldDraw(), and OdDbLayerTableRecord::subViewportDraw() functions to return 
	layer trait values and store them temporarily.
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/

class ODGI_EXPORT OdGiLayerTraitsTaker : public OdGiLayerTraits_
                                       , public OdGiDummyWorldDraw< OdGiDummyWorldGeometry< OdGiWorldDraw_ > >
                                       , public OdGiDummyViewportDraw< OdGiWrapperViewportGeometry< OdGiViewportDraw_ > >
                                       , public OdGiSubEntityTraits_
                                       , public OdGiLayerTraitsData
                                       //, public OdGiWrapperViewportGeometry< OdGiViewportGeometry >

/*class ODGI_EXPORT OdGiLayerTraitsTaker : public OdGiLayerTraits_
                                       , public OdGiWorldDraw_
                                       , public OdGiViewportDraw_
                                       , public OdGiSubEntityTraits_
                                       , public OdGiLayerTraitsData*/
{
protected:
  ODRX_USING_HEAP_OPERATORS( OdGiDummyWorldDraw< OdGiDummyWorldGeometry< OdGiWorldDraw_ > > );
private:
  const OdGiBaseVectorizer* m_pUnderlayingView;
  OdGiContext*              m_pGiContext;

public:
  OdGiLayerTraitsTaker();
  OdGiLayerTraitsTaker(const OdGiBaseVectorizer* pRefView, OdGiContext* pContext = NULL);

  void addRef();
  void release();

  void setRefView(const OdGiBaseVectorizer* pRefView);
  void reset() { static_cast<OdGiLayerTraitsData&>(*this) = OdGiLayerTraitsData(); }

  OdDb::LineWeight lineweight() const;
  OdDbStub* linetype() const;
  OdDb::PlotStyleNameType plotStyleNameType() const;
  OdDbStub* plotStyleNameId() const;
  OdDbStub* materialId() const;
  void setColor(const OdCmEntityColor& color);
  void setLineweight(OdDb::LineWeight lineweight);
  void setLinetype(OdDbStub* pLinetypeId);
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId= 0);

  bool isOff() const { return GETBIT(m_flags, kOff); }
  void setOff(bool bVal) { SETBIT(m_flags, kOff, bVal); }

  bool isPlottable() const { return GETBIT(m_flags, kPlottable); }
  void setPlottable(bool bVal) { SETBIT(m_flags, kPlottable, bVal); }

  bool isLocked() const { return GETBIT(m_flags, kLocked); }
  void setLocked(bool bVal) { SETBIT(m_flags, kLocked, bVal); }

  OdCmEntityColor trueColor() const;
  OdDbStub* layer() const;
  OdDbStub* lineType() const;
  OdGiFillType fillType() const;
  OdDb::LineWeight lineWeight() const;
  double lineTypeScale() const;
  double thickness() const;
  OdDbStub* material() const;
  const OdGiMapper* mapper() const;
  OdDbStub* visualStyle() const;
  OdCmTransparency transparency() const;
  OdUInt32 drawFlags() const;
  bool selectionGeom() const;
  ShadowFlags shadowFlags() const;
  bool sectionable() const;
  SelectionFlags selectionFlags() const;

  void setTrueColor(const OdCmEntityColor& color);
  void setColor(OdUInt16 color);
  void setLineType(OdDbStub* lineTypeId);
  void setLineWeight(OdDb::LineWeight lineWeight);
  void setMaterial(OdDbStub* pMaterialId);
  void setTransparency(const OdCmTransparency &transparency);

  OdCmEntityColor secondaryTrueColor() const;
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  const OdGiFill* fill() const;

  OdGiRegenType regenType() const;
  bool regenAbort() const;
  OdGiSubEntityTraits& subEntityTraits() const;
  OdGiGeometry& rawGeometry() const;
  bool isDragging() const;
  double deviation(const OdGiDeviationType deviationType, 
    const OdGePoint3d& pointOnCurve) const;
  OdUInt32 numberOfIsolines() const;
  OdGiContext* context() const;

  void setContext( OdGiContext* pContext);

  OdGiViewport& viewport() const;
  OdUInt32 sequenceNumber() const;
  bool isValidId(const OdUInt32 viewportId) const;
  OdDbStub* viewportObjectId() const;

  OdGeMatrix3d getModelToWorldTransform() const;
  OdGeMatrix3d getWorldToModelTransform() const;
};

inline void
OdGiLayerTraitsTaker::setRefView(const OdGiBaseVectorizer* pRefView)
{
  m_pUnderlayingView = pRefView;
}

inline void
OdGiLayerTraitsTaker::setContext( OdGiContext* pContext)
{
  m_pGiContext = pContext;
}

inline
OdGiLayerTraitsTaker::OdGiLayerTraitsTaker()
  : m_pUnderlayingView(0),
    m_pGiContext( 0 )
{
}

inline
OdGiLayerTraitsTaker::OdGiLayerTraitsTaker(const OdGiBaseVectorizer* pRefView, OdGiContext* pContext)
  : m_pUnderlayingView(pRefView),
    m_pGiContext( pContext )
{
}

#include "TD_PackPop.h"

#endif // #ifndef _ODGILAYERTRAITSDATA_INCLUDED_
