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

#ifndef ODGSBASEVECTORIZEVIEW_INC
#define ODGSBASEVECTORIZEVIEW_INC

#include "TD_PackPush.h"

#include <stdlib.h>
#include <utility>

#include "Gs/GsViewImpl.h"
#include "Ge/GeExtents3d.h"
#include "Gs/GsViewPropsDef.h"
#include "Gs/GsBaseVectorizeDevice.h"

/** \details
    This class is a pattern implementation of the OdGsView interface. 
    
    \remarks
    This class implements most of the OdGsView class.
    It creates simplified eye coordinate system (ECS) output graphics,
    which could be redirected by a client to a renderer to screen or file.
    
    \sa
    DWF/SVG exports
    
    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizeView : public OdGsViewImpl
{
public:
  OdGsBaseVectorizeView();
  ~OdGsBaseVectorizeView();

public:
  /** OdGsBaseVectorizeView methods *
  */

  /** \details
    Returns the specified OdGsView pointer 
    as a pointer to an OdGsBaseVectorizeView object.
    \param pView [in]  Pointer to an OdGsView object.
  */
  static OdGsBaseVectorizeView* safeCast(OdGsView* pView);

  /** OdGsViewImpl methods *
  */
  virtual OdGiContext* userGiContext() const;
  virtual void setUserGiContext(OdGiContext* pUserGiContext);
  virtual void update();

  virtual void select(const OdGePoint2d* aPtDc, int numPoints, OdGsSelectionReactor* pReactor,
    OdGsView::SelectionMode mode = OdGsView::kCrossing);
  TD_USING(OdGsViewImpl::select);

  virtual bool viewExtents(OdGeBoundBlock3d& extents) const;

  bool sceneDept(double& zNear, double& zFar, OdGsOverlayId nOverlay) const;
  bool sceneDept(double& zNear, double& zFar) const;

  virtual void collide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL );
  TD_USING(OdGsViewImpl::collide);
protected:

  virtual void updateGeometryByDevice();

  /** OdGsBaseVectorizeView methods *
  */
public:
  virtual OdUInt32 numVectorizers() const = 0;

protected:
  virtual OdGsBaseVectorizer* getVectorizer(bool bDisplay) = 0;
  virtual void releaseVectorizer(OdGsBaseVectorizer* pVect) = 0;

  virtual void updateGeometry();
  virtual void updateScreen();

private:
  void setVectThreadIndex(OdGsBaseVectorizer* pVect, bool bAssignIndex, int idx = -1);

  //data
protected:
  OdSmartPtr<OdGiContext> m_userGiContext;

private:
  friend class OdGsBaseVectorizeDevice;
  friend class VectorizerAutoPtr;
  friend class BaseVectScheduler;
};

inline OdGsBaseVectorizeView* OdGsBaseVectorizeView::safeCast(OdGsView* pView)
{
  return static_cast<OdGsBaseVectorizeView*>(OdSmartPtr<OdGsViewImpl>(pView).get());
}

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class VectorizerAutoPtr
{
public:
  VectorizerAutoPtr(OdGsBaseVectorizeView& view, bool bDisplay): m_view(view), m_obj(NULL)
  {
    m_obj = m_view.getVectorizer(bDisplay);
    if(m_obj && bDisplay)
        m_view.setVectThreadIndex(m_obj, true);
  }
  ~VectorizerAutoPtr()
  {
    if(m_obj)
    {
      m_view.setVectThreadIndex(m_obj, false);
      m_view.releaseVectorizer(m_obj);
    }
  }
  OdGsBaseVectorizer* operator ->()
  {
    return static_cast<OdGsBaseVectorizer*>(m_obj);
  }
  
  const OdGsBaseVectorizer* operator ->() const
  {
    return const_cast<const OdGsBaseVectorizer*>(m_obj);
  }

  const OdGsBaseVectorizer* get() const
  {
    return const_cast<const OdGsBaseVectorizer*>(m_obj);
  }
  
  OdGsBaseVectorizer* get()
  {
    return static_cast<OdGsBaseVectorizer*>(m_obj);
  }

protected:
  OdGsBaseVectorizeView& m_view;
  OdGsBaseVectorizer* m_obj;
};

#include "TD_PackPop.h"

#define IMPL_VECTORIZER_DEFAULT()\
  virtual OdUInt32 numVectorizers() const { return 1; }\
  virtual OdGsBaseVectorizer* getVectorizer(bool /*bDisplay*/)\
  { return this; }\
  virtual void releaseVectorizer(OdGsBaseVectorizer* pVect)\
  {ODA_ASSERT(pVect == (OdGsBaseVectorizer*)this); }

#define IMPL_VECTORIZER_DEFAULT_EX()\
  IMPL_VECTORIZER_DEFAULT()\
  virtual void clearLinetypeCache(){ OdGiBaseVectorizerImpl::clearLinetypeCache(); }\
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

#endif // ODGSBASEVECTORIZEVIEW_INC
