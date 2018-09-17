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

#ifndef __EX_FP_PRECISION_FIX__
#define __EX_FP_PRECISION_FIX__

#include "Gs/GsBaseVectorizer.h"
#include "Gi/GiXform.h"
#include "Ge/GeScale3d.h"
#include "MetafileTransformStack.h"

/* Experimental fix for #8266, #8949. Partially solves problem, but still have problems with trackers, since
   them draw geometry too far from drawing origin.                                                           */
//#define EXPERIMENTAL_VIEWIMPLCHG

#ifdef EXPERIMENTAL_VIEWIMPLCHG

/** \details
  <group ExRender_Classes> 
*/
class GsViewImplHelper
{
public:
  static void getExtents(const OdGsViewImpl& view, OdGeExtents3d& ext) { view.getExtents(ext); }
};
#endif

/** \details
  <group ExRender_Classes> 
*/
class ExFPPrecisionFix
{
  protected:
    // Type of metafile coordinates
    enum MetaCoordType
    {
      kWorldCoord  = 0,
      kOffsetCoord = 1
    } m_coordType;

    // Base transform for metafiles
    OdGeMatrix3d m_outputMtx, m_w2mMtx, m_m2wMtx, m_vpMtx, m_invVpMtx, m_blocksScale;
#ifdef EXPERIMENTAL_VIEWIMPLCHG
    OdGeMatrix3d m_balanceMtx;
#endif
    OdGiXformPtr m_outputXfm;
    MetafileTransformStack m_mfTransformStack;
    OdInt32 m_nSharedTransforms;

  public:
    ExFPPrecisionFix()
      : m_coordType(kWorldCoord)
      , m_nSharedTransforms(0)
    {
    }
    ~ExFPPrecisionFix()
    {
    }

    static double computeDoubleToFloatConversionTol(double inVal)
    {
      OdUInt32 inSizeOf = sizeof(double) << 3;
      OdUInt32 outSizeOf = sizeof(float) << 3;
      double tol = 0.0;
      if (inSizeOf > outSizeOf)
      {
        if (inVal > 1.e18) inVal = 1.e18; // This is maximum which could be truncated without exception
        else if (inVal < -1.e18) inVal = -1.e18;
        // Truncation of double to float (64 to 32 IEEE)
        const OdUInt64 integerLimit = 8388608;
        //const OdUInt64 doubleLimit = 16777216; // 24bit
        // Compute significant bits
        OdUInt64 tolCtr = (OdUInt64)fabs(inVal);
        OdUInt64 maxBit = (tolCtr != 0) ? 1 : 0;
        OdUInt32 bitCnt = 0;
        do
        {
          tolCtr >>= 1;
          if (tolCtr > 0)
            maxBit <<= 1;
          bitCnt++;
        }
        while (tolCtr > 0);
        if (maxBit == 0)
        {
          //return tol; // not return zero here
          maxBit = 1;
          bitCnt = 0;
        }
        tol = 1.0;
        if (maxBit > integerLimit)
        {
          if (bitCnt - 24 > 31)
            tol = double(OdUInt32(1) << 31);
          else
            tol = double(OdUInt32(1) << (bitCnt - 24));
        }
        else if (maxBit < integerLimit)
          tol = 1.0 / double(OdUInt32(1) << (24 - bitCnt));
      }
      return tol;
    }
    static double possibleFPConversionError(const OdGeMatrix3d &xfm)
    {
      //OdGePoint3d origPt; origPt.transformBy(xfm); // Only adds origin
      OdGePoint3d origPt(xfm.getCsOrigin());
      double tol1 = computeDoubleToFloatConversionTol(origPt.x);
      double tol2 = computeDoubleToFloatConversionTol(origPt.y);
      double tol3 = computeDoubleToFloatConversionTol(origPt.z);
      return odmax(tol1, odmax(tol2, tol3));
    }
    static double possibleFPConversionError(const OdGsView *pView)
    {
      OdGeMatrix3d xMat = pView->screenMatrix() * pView->projectionMatrix() * pView->viewingMatrix();
      return possibleFPConversionError(xMat);
    }
    static double possibleFPConversionError(ExFPPrecisionFix &pThis, const OdGeMatrix3d& mtx)
    {
      return possibleFPConversionError(pThis.blocksScaleTransform() * mtx);
    }

    bool metafileTransformOverrided() const
    {
      return m_coordType != kWorldCoord;
    }
    const OdGeMatrix3d &metafileToWorldMatrix() const
    {
      if (m_coordType != kWorldCoord)
        return m_m2wMtx;
      return OdGeMatrix3d::kIdentity;
    }
    const OdGeMatrix3d &worldToMetafileMatrix() const
    {
      if (m_coordType != kWorldCoord)
        return m_w2mMtx;
      return OdGeMatrix3d::kIdentity;
    }
    const OdGeMatrix3d &metafileGenerationMatrix() const
    {
      if (m_coordType != kWorldCoord)
        return m_outputMtx;
      return OdGeMatrix3d::kIdentity;
    }
    const OdGeMatrix3d &viewportToOutputMatrix() const
    {
      return m_vpMtx;
    }
    const OdGeMatrix3d &outputToViewportMatrix() const
    {
      return m_invVpMtx;
    }
    const OdGeMatrix3d &blocksScaleTransform() const
    {
      return m_blocksScale;
    }
#ifdef EXPERIMENTAL_VIEWIMPLCHG
    double targetBalanceLength() const
    {
      return m_balanceMtx[2][3];
    }
#endif

    OdGiXformPtr &outputXform() { return m_outputXfm; }
    const OdGiXformPtr &outputXform() const { return m_outputXfm; }

    MetafileTransformStack &mfTransformStack() { return m_mfTransformStack; }
    const MetafileTransformStack &mfTransformStack() const { return m_mfTransformStack; }

    void resetOutputXform(const OdGeMatrix3d &mtx)
    {
      m_outputXfm->setTransform(mtx);
    }

    double regenCoef(OdGsViewImpl &view)
    {
      return possibleFPConversionError(view.eyeToScreenMatrix() * m_vpMtx);
    }
    struct GsViewImplToFPFixCvt { virtual ExFPPrecisionFix &cvt(OdGsViewImpl &view) const = 0; };
    template <class T> struct GsViewImplToFPFixCvtImpl : public GsViewImplToFPFixCvt
    { virtual ExFPPrecisionFix &cvt(OdGsViewImpl &view) const {
        return static_cast<ExFPPrecisionFix&>(static_cast<T&>(view)); } };
    static double getRegenCoef(OdGsDevice &device, const GsViewImplToFPFixCvt &cvt)
    {
      int nViews = device.numViews();
      double regenCoef = 0.;
      for (int nView = 0; nView < nViews; nView++)
      {
        OdGsViewImpl &view = static_cast<OdGsViewImpl&>(*device.viewAt(nView));
        ExFPPrecisionFix &fix = cvt.cvt(view);
        const double curCoef = fix.regenCoef(view);
        if (curCoef > regenCoef)
          regenCoef = curCoef;
      }
      return regenCoef;
    }

    void setupForFPVectorization(OdGsBaseVectorizer &vect, OdGiConveyorGeometry &defOutput,
                                 bool bIdentityXfm = false, bool bOutputSuppressed = false,
                                 ExFPPrecisionFix *pGrabFrom = NULL)
    {
      OdGsViewImpl &view = vect.view();

#ifdef EXPERIMENTAL_VIEWIMPLCHG
      bool bRegenerated = false;
#endif
      if (pGrabFrom)
      { // #11858 : grab settings from main vectorizer to be in sync
        m_coordType = pGrabFrom->m_coordType;
        m_outputMtx = pGrabFrom->m_outputMtx;
        m_blocksScale = pGrabFrom->m_blocksScale;
      }
      else if (vect.isViewRegenerated())
      {
        if (possibleFPConversionError(&view) >= 1.0)
        {
          m_coordType = kOffsetCoord;
          m_outputMtx = view.viewingMatrix(); // getWorldToEyeTransform(); // similar, but viewingMatrix have lower cost
          //m_outputMtx.preMultBy(OdGeMatrix3d::scaling(1.e-100/*, m_outputMtx.getCsOrigin()*/));
          //m_outputMtx.preMultBy(OdGeMatrix3d::scaling(OdGeScale3d(1.0, 1.0, /*0.0000000001*/1.e-100)/*, m_outputMtx.getCsOrigin()*/));
        }
        else
        {
          m_coordType = kWorldCoord;
        }
        m_blocksScale = view.eyeToScreenMatrix();
#ifdef EXPERIMENTAL_VIEWIMPLCHG
        bRegenerated = true;
#endif
      }

      if (m_coordType == kWorldCoord)
      {
        m_vpMtx = view.viewingMatrix(); // getWorldToEyeTransform(); // similar, but viewingMatrix have lower cost
        m_invVpMtx = m_vpMtx.inverse();
#ifdef EXPERIMENTAL_VIEWIMPLCHG
        m_balanceMtx.setToIdentity();
#endif

        if (!bOutputSuppressed)
          vect.output().setDestGeometry(defOutput);
      }
      else
      {
#ifdef EXPERIMENTAL_VIEWIMPLCHG
        m_balanceMtx.setToIdentity();
        if (!view.isPerspective())
        {
          // Calculate extents
          OdGeExtents3d sceneExt;
          GsViewImplHelper::getExtents(view, sceneExt);
          if (!sceneExt.isValidExtents())
          { // Try to extract approx extents
            if (view.userGiContext())
            {
              OdRxObjectPtr pDbPE = ::ExGsGetBaseDatabasePE(view.userGiContext()->database());
              if (!pDbPE.isNull())
              {
                OdRxObjectPtr pLayout = ::ExGsBaseDatabasePECall(pDbPE)->findLayoutByViewport(view.userGiContext()->database(), vect.viewportObjectId());
                OdRxObjectPtr pLayoutPE = ::ExGsGetBaseLayoutPE(pLayout);
                if (!pLayoutPE.isNull())
                {
                  OdGePoint3d extMin, extMax;
                  if (::ExGsBaseLayoutPECall(pLayoutPE)->getApproxExtents(pLayout, extMin, extMax))
                    sceneExt.set(extMin, extMax);
                }
              }
            }
            if (!sceneExt.isValidExtents()) // Probably empty space
              sceneExt.set(OdGePoint3d::kOrigin, OdGePoint3d::kOrigin);
          }
          double targetBalance = (sceneExt.center() - view.target()).dotProduct(view.eyeVector());
          if (computeDoubleToFloatConversionTol(targetBalance) >= 1.0)
          {
            OdGeVector3d balanceMv = view.eyeVector() * targetBalance;
            m_balanceMtx.setToTranslation(balanceMv);
            m_balanceMtx.invert();
            if (bRegenerated)
              m_outputMtx.postMultBy(m_balanceMtx);
          }
        }
#endif

        m_vpMtx = view.viewingMatrix() /*getWorldToEyeTransform()*/ * m_outputMtx.inverse(); // similar, but viewingMatrix have lower cost
        m_invVpMtx = m_vpMtx.inverse();
        m_w2mMtx = m_invVpMtx * vect.getWorldToEyeTransform();
        m_m2wMtx = vect.getEyeToWorldTransform() * m_vpMtx;

#ifdef EXPERIMENTAL_VIEWIMPLCHG
        m_vpMtx.postMultBy(m_balanceMtx);
        m_invVpMtx.preMultBy(m_balanceMtx.inverse());
        //m_vpMtx = m_invVpMtx = OdGeMatrix3d::kIdentity;
#endif

        if (!bOutputSuppressed)
        {
          if (m_outputXfm.isNull())
            m_outputXfm = OdGiXform::createObject();
          m_outputXfm->input().addSourceNode(vect.output());
          m_outputXfm->output().setDestGeometry(defOutput);
          if (!bIdentityXfm)
            m_outputXfm->setTransform(worldToMetafileMatrix());
          else
            m_outputXfm->setTransform(OdGeMatrix3d::kIdentity);
        }
      }

      if (regenCoef(view) > 1.e9)
      {
        // Since we can't do anything in this range, simply fix it to avoid crash
        view.setView(OdGePoint3d(), OdGePoint3d() + OdGeVector3d::kZAxis, OdGeVector3d::kYAxis, 1.0, 1.0);
        if (m_coordType == kOffsetCoord && !bOutputSuppressed)
          m_outputXfm->input().removeSourceNode(vect.output());
        m_coordType = kWorldCoord;
        m_invVpMtx = m_vpMtx = OdGeMatrix3d::kIdentity;
        if (!bOutputSuppressed)
          vect.output().setDestGeometry(defOutput);
        //throw OdError(eOutOfRange);
      }
    }
    void finalizeFPVectorization(OdGsBaseVectorizer &vect)
    {
      if (m_coordType != kWorldCoord)
        m_outputXfm->input().removeSourceNode(vect.output());
    }

    bool pushMetafileTransformForUpdate(OdGsBaseVectorizer &vect, const OdGeMatrix3d& mtx, OdUInt32 uFlags)
    {
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate))
      {
        if (metafileTransformOverrided())
        {
          if ((possibleFPConversionError(*this, vect.getModelToWorldTransform() * mtx) < 1.0 && !GETBIT(m_nSharedTransforms, 1)) || GETBIT(m_nSharedTransforms, 2))
          {
            m_outputXfm->setTransform(worldToMetafileMatrix());
            SETBIT(m_nSharedTransforms, 2, true);
          }
          else
          {
            m_outputXfm->setTransform(OdGeMatrix3d::kIdentity);
            SETBIT(m_nSharedTransforms, 1, true);
          }
          m_mfTransformStack.pushMetafileTransform(mtx);
          return true;
        }
        else
        {
          if (possibleFPConversionError(*this, vect.OdGiBaseVectorizer::getModelToWorldTransform() * mtx) >= 1.0)
            vect.getModelToWorldTransform(); // #13613 : to enable ModelTf and make block non-shared
        }
      }
      return false;
    }
    bool pushMetafileTransformForDisplay(OdGsBaseVectorizer & /*vect*/, const OdGeMatrix3d& mtx, OdUInt32 uFlags, OdGeMatrix3d *pOMtx = NULL)
    {
      if (!GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate))
      {
        if (metafileTransformOverrided())
        {
          OdGeMatrix3d oMtx;
          if (possibleFPConversionError(*this, mtx) < 1.0)
            oMtx = worldToMetafileMatrix() * mtx * metafileToWorldMatrix();
          else
            oMtx = worldToMetafileMatrix() * mtx;
          if (pOMtx) *pOMtx = oMtx;
          m_mfTransformStack.pushMetafileTransform(oMtx);
        }
        else
        {
          if (pOMtx) *pOMtx = mtx;
          m_mfTransformStack.pushMetafileTransform(mtx);
        }
        return true;
      }
      return false;
    }
    bool pushMetafileTransformForSelect(OdGsBaseVectorizer & /*vect*/, const OdGeMatrix3d& mtx, OdUInt32 uFlags, OdGeMatrix3d *pOMtx = NULL)
    {
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate)) return false;
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefTransform))
        m_nSharedTransforms++;
      if (metafileTransformOverrided() && GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefTransform) && (m_nSharedTransforms == 1))
      {
        const OdGeMatrix3d oMtx = worldToMetafileMatrix() * mtx;
        if (pOMtx) *pOMtx = oMtx;
        m_mfTransformStack.pushMetafileTransform(oMtx);
      }
      else
      {
        if (pOMtx) *pOMtx = mtx;
        m_mfTransformStack.pushMetafileTransform(mtx);
      }
      return true;
    }

    bool popMetafileTransformForUpdate(OdGsBaseVectorizer & /*vect*/, OdUInt32 uFlags)
    {
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate))
      {
        if (metafileTransformOverrided())
        {
          m_mfTransformStack.popMetafileTransform();
          if (!m_mfTransformStack.hasStack() || !GETBIT(m_nSharedTransforms, 1) ||
              (possibleFPConversionError(*this, m_mfTransformStack.metafileTransform()) < 1.0))
          {
            m_outputXfm->setTransform(worldToMetafileMatrix());
            if (!m_mfTransformStack.hasStack()) m_nSharedTransforms = 0;
          }
          else
          {
            m_outputXfm->setTransform(OdGeMatrix3d::kIdentity);
          }
          return true;
        }
      }
      return false;
    }
    bool popMetafileTransformForDisplay(OdGsBaseVectorizer & /*vect*/, OdUInt32 uFlags)
    {
      if (!GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate))
      {
        m_mfTransformStack.popMetafileTransform();
        return true;
      }
      return false;
    }
    bool popMetafileTransformForSelect(OdGsBaseVectorizer & /*vect*/, OdUInt32 uFlags)
    {
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefUpdate)) return false;
      if (GETBIT(uFlags, OdGsBaseVectorizer::kSharedRefTransform))
        m_nSharedTransforms--;
      m_mfTransformStack.popMetafileTransform();
      return true;
    }
};

#endif // __EX_FP_PRECISION_FIX__
