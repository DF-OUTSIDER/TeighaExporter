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
#ifndef ODGSDRAWABLEHOLDER_INC
#define ODGSDRAWABLEHOLDER_INC

#include "TD_PackPush.h"
#include "Ge/GeExtents3d.h"
#include "IntArray.h"
#include "Gi/GiDrawable.h"
#include "Gs/GsExport.h"

class OdGsNode;
class OdGsBaseModel;
class OdDbStub;

class GS_TOOLKIT_EXPORT OdGsUpdateExtents
{
  protected:
    OdGeExtents3d m_extents;
    OdDb::LineWeight m_lineweight;
  public:
    GS_STATIC_EXPORT static const OdGsUpdateExtents kInvalid;
  public:
    OdGsUpdateExtents()
      : m_lineweight(OdDb::kLnWt000)
    {
    }
    OdGsUpdateExtents(const OdGeExtents3d &extents, OdDb::LineWeight lw = OdDb::kLnWt000)
      : m_extents(extents)
      , m_lineweight(lw)
    {
    }

    const OdGeExtents3d &getExtents() const
    {
      return m_extents;
    }
    OdGeExtents3d &accessExtents()
    {
      return m_extents;
    }
    void setExtents(const OdGeExtents3d &extents)
    {
      m_extents = extents;
    }
    void resetExtents()
    {
      m_extents = OdGeExtents3d::kInvalid;
    }
    void addExtents(const OdGeExtents3d &extents)
    {
      if (extents.isValidExtents())
        m_extents.addExt(extents);
    }

    bool hasLineweight() const
    {
      return (m_lineweight > OdDb::kLnWt000);
    }
    OdDb::LineWeight getLineweight() const
    {
      return m_lineweight;
    }
    OdDb::LineWeight &accessLineweight()
    {
      return m_lineweight;
    }
    void setLineweight(OdDb::LineWeight lw)
    {
      m_lineweight = lw;
    }
    void resetLineweight()
    {
      m_lineweight = OdDb::kLnWt000;
    }
    void addLineweight(OdDb::LineWeight lw)
    {
      if (m_lineweight < lw)
        m_lineweight = lw;
    }

    void get(OdGeExtents3d &extents, OdDb::LineWeight &lw) const
    {
      extents = m_extents;
      lw = m_lineweight;
    }
    void set(const OdGeExtents3d &extents, OdDb::LineWeight lw)
    {
      m_extents = extents;
      m_lineweight = lw;
    }
    void reset()
    {
      resetExtents();
      resetLineweight();
    }
    void add(const OdGeExtents3d &extents, OdDb::LineWeight lw)
    {
      addExtents(extents);
      addLineweight(lw);
    }
    void add(const OdGsUpdateExtents &extents)
    {
      add(extents.getExtents(), extents.getLineweight());
    }

    bool isValid() const
    {
      return m_extents.isValidExtents();
    }
};

/** \details
  <group OdGs_Classes> 
    
  Corresponding C++ library: TD_Gs
*/
struct DrawableHolder
{
  DrawableHolder(): m_drawableId(0), m_pGsRoot(0)
  {
  }
  OdDbStub* m_drawableId;
  OdGiDrawablePtr m_pDrawable;
  OdSmartPtr<OdGsBaseModel> m_pGsModel;
  OdGsNode* m_pGsRoot;
  OdRxObjectPtr m_pMetafile;
  OdGsUpdateExtents m_lastExt;
};
typedef OdArray<DrawableHolder> DrawableHolderArray;

#include "TD_PackPop.h"

#endif // ODGSDRAWABLEHOLDER_INC
