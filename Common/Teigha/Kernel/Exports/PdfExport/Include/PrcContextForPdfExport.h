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


#ifndef _PRC_CONTEXT_FOR_PDF_EXPORT_INCLUDED_
#define _PRC_CONTEXT_FOR_PDF_EXPORT_INCLUDED_

#include "Gi/GiDrawableImpl.h"
#include "Gs/Gs.h"
#include "PdfExportDef.h"
#include "Gi/GiCommonDraw.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
  <group OdExport_Classes>
  Implements .prc context for exporting to .pdf format.
*/
class PDFEXPORT_DLL OdPrcContextForPdfExport : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdPrcContextForPdfExport);

private:
  // User-defined data container that can be used in implementations of shouldExportAsPRC function. 
  OdRxObjectPtr m_userData;

public:
  /** \details
  \param pDrawable [in] An entity that is checked whether it can/should be exported as prc object or not
  \param entity [in] A path node of the entity.
  \param pView [in] view.
  \param viewportidx [out] Index of viewport (prc file) in pdf document where the entity should be exported to
  \returns
  Returns true if entity should be exported to pdf as prc object; otherwise returns false
  */
  virtual bool shouldExportAsPRC(
    const OdGiDrawable *pDrawable,
    const OdGiPathNode *entity,
    const OdGsClientViewInfo *pView,
    OdUInt32 &viewportidx) const
  {
    return false;
  }
  /** \details
  Get extents of the entity.
      \remarks
  \param pDrawable [in] An entity whose extents will be gotten.
  \param viewportObjectId [in] viewport object id in which current entity will be drawn.
  \param pGiContext [in] OdGiContext associated with this object.
  \param cameraView [in] camera transformation.
  \param modelToWorld [in] model to world transformation.
  \param extents [out] extents of an entity.
  */
  virtual void getExtents(
    const OdGiDrawable * pDrawable,
    OdDbStub * viewportObjectId,
    OdGiContext * pGiContext,
    const OdGeMatrix3d & cameraView,
    const OdGeMatrix3d & modelToWorld,
    OdGeExtents3d & extents) const;

  /** \details
  \returns
  Returns pointer to user data (RxObject)
  */
  virtual OdRxObjectPtr& userData(void) { return m_userData; }

  /** \details
  \param userData [in] A user-defined data container.
  */
  virtual void setUserData (const OdRxObjectPtr &userData) { m_userData = userData; }

protected:
  /** \details
  Get extents of the entity.
      \remarks
  \param pDrawable [in] An entity whose extents will be gotten.
  \param viewportObjectId [in] viewport object id in which current entity will be drawn.
  \param pGiContext [in] OdGiContext associated with this object.
  \param cameraView [in] camera transformation.
  \param modelToWorld [in] model to world transformation.
  \param extents [out] extents of an entity.
  \param regenType [in] OdGiRegenType for viewport object id.
  */
  void getExtentsWithParams(
    const OdGiDrawable * pDrawable,
    OdDbStub * viewportObjectId,
    OdGiContext * pGiContext,
    const OdGeMatrix3d & cameraView,
    const OdGeMatrix3d & modelToWorld,
    OdGeExtents3d & extents,
    OdGiRegenType regenType) const;
};

typedef OdSmartPtr<OdPrcContextForPdfExport> OdPrcContextForPdfExportPtr;

typedef std::map<OdUInt32, OdStreamBufPtr> PRCStreamsMap;

/** \details
    <group OdExport_Classes>
*/
class PDFEXPORT_DLL OdPrcContextForPdfExportWrapper : public OdPrcContextForPdfExport
{
  ODRX_DECLARE_MEMBERS(OdPrcContextForPdfExportWrapper);

  private:
    OdPrcContextForPdfExportPtr m_userContext;
    PRCStreamsMap m_outStreamsPRC;

  public:
    PRCStreamsMap &getOutputPRC()
    {
      return m_outStreamsPRC;
    }

    void setOutputPRC(const PRCStreamsMap &streamsPRC)
    {
      m_outStreamsPRC = streamsPRC;
    }

    const OdPrcContextForPdfExportPtr &getUserContext() const
    {
      return m_userContext;
    }

    void setUserContext(const OdPrcContextForPdfExportPtr &userContext)
    {
      m_userContext = userContext;
    }

    virtual bool shouldExportAsPRC (const OdGiDrawable *pDrawable,
      const OdGiPathNode *entity,
      const OdGsClientViewInfo *pView,
      OdUInt32 &viewportidx) const
    {
      return m_userContext.isNull() ? OdPrcContextForPdfExport::shouldExportAsPRC(pDrawable, entity, pView, viewportidx) : m_userContext->shouldExportAsPRC(pDrawable, entity, pView, viewportidx);
    }

    virtual OdRxObjectPtr& userData(void)
    {
      return m_userContext.isNull() ? OdPrcContextForPdfExport::userData() : m_userContext->userData();
    }

    virtual void setUserData(const OdRxObjectPtr &userData)
    {
      m_userContext.isNull() ? OdPrcContextForPdfExport::setUserData(userData) : m_userContext->setUserData(userData);
    }
};

typedef OdSmartPtr<OdPrcContextForPdfExportWrapper> OdPrcContextForPdfExportWrapperPtr;

#endif
