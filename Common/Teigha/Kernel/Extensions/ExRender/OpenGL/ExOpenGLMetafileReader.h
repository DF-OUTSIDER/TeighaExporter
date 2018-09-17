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

#ifndef _EXOPENGLMETAFILEREADER_INCLUDED_
#define _EXOPENGLMETAFILEREADER_INCLUDED_

#include "TD_PackPush.h"

#include "ExOpenGLMetafileStream.h"
#include "Gi/GiConveyorGeometry.h"
#include "../ExColorConverterCallback.h"
#include "GsOpenGLInclude.h"

// OpenGL metafile stream reader

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMetafileReaderCallback
{
  public:
    struct RasterImageEntry
    {
      OdOpenGLAlphaTestType bEnableAlphaTest;
      GLint glFormat;
      GLsizei pxWidth;
      GLsizei pxHeight;
      GLint glFilter;
      GLint glEnvMode;
      OdUInt32 lutSize;
      OdGeQuaternion glSPlane;
      OdGeQuaternion glTPlane;
      bool bBitonal;
    };
    struct RasterImageEntrySet : public RasterImageEntry
    {
      const void *pTexture;
      OdUInt32 bufSize;
      const void *pPalTexture;
    };
  public:
    virtual void metafileProcessMaterial(OdRxObject * /*pMetafile*/) { }

    virtual void metafileProcessInitTexture(const OdOpenGLMetafileReaderCallback::RasterImageEntrySet &/*dataSet*/, OdRxObject * /*pData*/ = NULL) { }
    virtual void metafileProcessUninitTexture() { }

    virtual void metafileProcessUserEntry(const OdOpenGLFlatMetafileContainer * /*pContainer*/, const OdUInt8 *& /*pMemPtr*/) { }

    virtual void metafileLineStipple(OdOpenGLLinePatt::Entry /*entry*/, OdUInt8 /*state*/) { }
    virtual void metafileFillStipple(OdOpenGLFillPatt::Entry /*entry*/, OdUInt8 /*state*/) { }
    virtual void metafileLineweightStyle(bool /*bByDefault*/,
      OdPs::LineEndStyle /*lesStyle*/ = OdPs::kLesRound, OdPs::LineJoinStyle /*ljsStyle*/ = OdPs::kLjsRound) { }

    virtual const OdOpenGLLinePatt *metafileGetLinePattPtr() const { return NULL; }
    virtual const OdOpenGLFillPatt *metafileGetFillPattPtr() const { return NULL; }
    virtual const OdOpenGLLwdStyle *metafileGetLwdStylePtr() const { return NULL; }

    virtual bool metafileForceAlternativeHlt() const { return false; }

    virtual bool metafileCheckMarker(OdOpenGLGsMarker /*gsMarker*/, bool bPrevState) { return bPrevState; }
    virtual void metafileSelectMarker(OdOpenGLGsMarker /*gsMarker*/) { }

#ifdef OD_OGL_ES
    virtual void metafilePushStencilAttrib() { }
    virtual void metafilePopStencilAttrib() { }
#endif

    virtual bool metafileProcessLineweight(const OdOpenGLLwdSetting & /*lwd*/) { return false; }
    virtual bool metafilePointsLineweight() const { return false; } // For Dgn-style
    virtual bool metafileDrawExtendedPointLineweight() const { return true; } // Returns true for extended points lineweight
    virtual bool metafileDrawExtendedLineLineweight() const { return true; } // Returns true for extended lines lineweight
    virtual void metafileDrawExtendedLine(const OdGePoint3d * /*origin*/, const OdUInt8 * /*color*/ = NULL) { }
    virtual void metafileDrawExtendedPoint(const OdGePoint3d * /*origin*/, const OdUInt8 * /*color*/ = NULL) { }
    virtual void metafileDrawExtendedLineStrip(const OdGePoint3d * /*pPoints*/, OdUInt32 /*nPoints*/, const OdUInt8 * /*color*/ = NULL) { }
    virtual void metafileGetBackgroundColor(ODCOLORREF & /*bkgndColor*/) { }
    virtual OdColorConverterCallback *metafileColorConverter() { return NULL; }

    virtual void metafileTtfText(OdUInt64 /*fontKeyA*/, OdUInt64 /*fontKeyB*/, const OdUInt32 * /*pText*/, OdUInt32 /*nText*/,
                                 const OdGeMatrix3d & /*baseTransform*/, const OdGeVector3d & /*stepDir*/) { }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMetafileReader //: public OdOpenGLMetafileInputStream
{
  protected:
    static GLint gDefArrayString[OdOpenGLMetaRecArrayType_NTypes];
    static GLenum gGLArrayString[OdOpenGLMetaRecArrayType_NTypes];
    typedef void (*glPointerProto)(GLint, GLenum, GLsizei, const GLvoid *);
    static glPointerProto gGLArrayProto[OdOpenGLMetaRecArrayType_NTypes];
    static GLint gGLArrayNVals[OdOpenGLMetaRecArrayType_NTypes];
    static GLenum gGLArrayType[OdOpenGLMetaRecArrayType_NTypes];
    static GLenum gGLAttributeString[OdOpenGLAttribute_NTypes];
    // Client functions pointers
    //typedef void (OdOpenGLMetafileReader::*glReadFunc)(const OdUInt8 *&);
    //static glReadFunc gReadFunc[OdOpenGLMetaRecType_NTypes];
  protected:
    struct PrevState
    {
      bool bOptAttrib[OdOpenGLAttribute_NTypes];
      GLenum eCullMode;
      const OdDbStub *pMaterial;
      const OdOpenGLLinePatt *pLnPatt;
      const OdOpenGLFillPatt *pFillPatt;
      const OdOpenGLLwdStyle *pLwdStyle;
      OdColorConverterCallback *m_pColorConverter;
      bool m_bProcessBackgroundColors;
      bool bForceAlternativeHlt;
      bool bNoColorModState;

      void setDefaults();
      void restoreOption(OdOpenGLAttribute attrib);
    };
    PrevState prevState;    
    friend struct OdOpenGLMetafileReader::PrevState;
    // Renderer callback
    OdOpenGLMetafileReaderCallback *m_pCallback;
    // Temporary buffer
    OdGePoint3dArray m_buffer;
    OdUInt8Array m_clrBuffer;
    // Interprocess context flags
    enum InprocCtxFlags
    {
      kSuppressGeom   = 1,
      kLineweightProc = 2,
      kLinesAntiAlias = 4,
      kHighlightInit  = 8
    };
    // Interprocess context for nested play() calls
    struct InprocRegisters
    {
      // Current selection markers
      const OdOpenGLSelMark *pMarks;
      // Number of selection markers
      OdUInt32 nMarks;
      // Processing flags
      OdUInt8 bFlags;

      // Default initializer
      InprocRegisters()
        : pMarks(NULL), nMarks(0), bFlags(0)
      { }
    } m_inprocContext;
  public:
    OdOpenGLMetafileReader(OdOpenGLMetafileReaderCallback *pCallback = NULL) : m_pCallback(pCallback) //: OdOpenGLMetafileInputStream()
    {
      //setContainerIO(new OdOpenGLFlatMetafileContainerReader());
      setCallback_Init();
    }
    virtual ~OdOpenGLMetafileReader()
    {
      //if (containerIO())
      //  delete containerIO();
    }

    inline void beginPlaying()
    {
      prevState.setDefaults();
    }

    inline void restoreOption(OdOpenGLAttribute attrib)
    {
      prevState.restoreOption(attrib);
    }
    inline void setOption(OdOpenGLAttribute attrib, bool bValue)
    {
      prevState.bOptAttrib[attrib] = bValue;
    }
    inline void setCullMode(GLenum value)
    {
      prevState.eCullMode = value;
    }

    inline void setCallback(OdOpenGLMetafileReaderCallback *pCallback)
    {
      m_pCallback = pCallback;
      setCallback_Init();
    }
    inline OdOpenGLMetafileReaderCallback *callback() const
    {
      return m_pCallback;
    }

    inline void setCallback_Init()
    {
      if (m_pCallback)
      {
        prevState.pLnPatt = m_pCallback->metafileGetLinePattPtr();
        prevState.pFillPatt = m_pCallback->metafileGetFillPattPtr();
        prevState.pLwdStyle = m_pCallback->metafileGetLwdStylePtr();
        prevState.bForceAlternativeHlt = m_pCallback->metafileForceAlternativeHlt();
        prevState.m_pColorConverter = m_pCallback->metafileColorConverter();
        if (prevState.m_pColorConverter) prevState.m_bProcessBackgroundColors = prevState.m_pColorConverter->convertBackgroundColors();
      }
      else
      {
        prevState.pLnPatt = NULL;
        prevState.pFillPatt = NULL;
        prevState.pLwdStyle = NULL;
        prevState.bForceAlternativeHlt = false;
        prevState.m_pColorConverter = NULL;
      }
    }

    inline OdGePoint3dArray &getTmpBuffer() { return m_buffer; }

    inline void enableLinesAntiAliasing(bool bSet) { SETBIT(m_inprocContext.bFlags, kLinesAntiAlias, bSet); }
    inline bool isLinesAntiAliasingEnabled() const { return GETBIT(m_inprocContext.bFlags, kLinesAntiAlias); }

    // Reading function wrappers (@@@TODO: test other platforms. May be pointers to member functions is not portable)
    //void readRecType_Empty(const OdUInt8 *&/*pMemPtr*/)
    //{
    //  // Skip
    //}
    ////

    void play(const OdBaseMetafileContainer *pContainer, bool bHighlighted, bool bCheckMark);

    void play4Select(const OdBaseMetafileContainer *pContainer, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, bool bCheckMark = true);

  protected:
    void drawArrayAntiAlias(GLenum mode, GLint first, GLsizei count, const GLvoid *pArray = NULL);
    void drawArrayAsIs(GLenum mode, GLint first, GLsizei count, const GLvoid *pArray, bool bPoint, bool bLine);
    void drawArrayLwd(GLenum mode, GLint first, GLsizei count, bool bHighlighted, const GLvoid *pArray = NULL);
    void drawElementsAntiAlias(GLenum mode, GLsizei count, GLenum type, const GLvoid *pIds);
    void drawElementsAsIs(GLenum mode, GLsizei count, GLenum type, const GLvoid *pIds, const GLvoid *pArray, bool bPoint, bool bLine);
    void drawElementsLwd(GLenum mode, GLsizei count, GLenum type, bool bHighlighted, const GLvoid *pIds, const GLvoid *pArray = NULL);
};

#include "TD_PackPop.h"

#endif // _EXOPENGLMETAFILEREADER_INCLUDED_
