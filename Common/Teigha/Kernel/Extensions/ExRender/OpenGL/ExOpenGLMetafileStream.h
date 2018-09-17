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

#ifndef _EXOPENGLMETAFILESTREAM_INCLUDED_
#define _EXOPENGLMETAFILESTREAM_INCLUDED_

#include "TD_PackPush.h"

#include "MetafileStreamBase.h"
#include "Gs/Gs.h"
#include "GsOpenGLDefs.h"

// Some configuration definitions

// Enable support of indexed primitives packaging for shells and meshes
#define OD_OGL_SUPPORTINDEXEDMODE

// Enable texture coordinates in indexed mode
//#define OD_OGL_ENABLEINDEXEDTEXCOORDS

// Enable face normals in indexed mode
//#define OD_OGL_ENABLEINDEXEDFACENORMALS

// Enable conversion from double to float for vertex and normal buffers
#define OD_OGL_ENABLEFLOATCONVERSION

// Enables dynamic subentity highlighting mode
//#define OD_OGL_DYNAMICSUBENTHLT // (moved to device properties #12382)

// Minimum polyline vertices size which will be packaged as separate vertices buffer
#define OD_OGL_MINLINESTRIP     12
// Minimum polyline vertices size which will be packaged as separate vertices buffer if lines
// packaging already started.
#define OD_OGL_MINLINESTRIPWORK 40

// Minimum count of vertices in shell/mesh which must be packaged as indexed primitive
#define OD_OGL_MINSHELLINDEXED  40

// Minimum count of vertices to process polypoint primitive as separate packaging primitive
#define OD_OGL_MINPOLYPOINTWORK 50

// Keep temporary lwd buffer not larger this limit
#define OD_OGL_LWDBUFKEEPLIMIT  256

// Number of color components in color arrays
#define OD_OGL_NCOLORCOMPONENTS 4 // If it will be changed to 3 alpha component will be lost

#ifdef OD_OGL_ES
// OpenGL ES doesn't support GLdouble, so enable float convertion
#ifndef OD_OGL_ENABLEFLOATCONVERSION
#define OD_OGL_ENABLEFLOATCONVERSION
#endif
// OpenGL ES doesn't support line/polygon stippling, so enable alternative highlighting method
#ifndef OD_OGL_ALTERNATIVEHLT
#define OD_OGL_ALTERNATIVEHLT
#endif
// Require 4 color components
#if (OD_OGL_NCOLORCOMPONENTS != 4)
#undef OD_OGL_NCOLORCOMPONENTS
#define OD_OGL_NCOLORCOMPONENTS 4
#endif
#endif

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLMetaRecType // Currently packed as one byte
{
  // The entries rearranged to optimize... Mostly used moved to top.
  OdOpenGLMetaRecType_Empty              = 0x00, // No data
  OdOpenGLMetaRecType_EnableOpt          = 0x01, // glEnable with optimization
  OdOpenGLMetaRecType_DisableOpt         = 0x02, // glDisable with optimization
  OdOpenGLMetaRecType_Color              = 0x03, // glColor
  OdOpenGLMetaRecType_EnableArray        = 0x04, // glEnableClientState/glXXXPointer
  OdOpenGLMetaRecType_DisableArray       = 0x05, // glDisableClientState
  OdOpenGLMetaRecType_DrawArrays         = 0x06, // glDrawArrays
  OdOpenGLMetaRecType_DrawElements       = 0x07, // glDrawElements
  OdOpenGLMetaRecType_CullFace           = 0x08, // glCullFace
  OdOpenGLMetaRecType_LStipple           = 0x09, // glLineStipple
  OdOpenGLMetaRecType_PStipple           = 0x0A, // glPolygonStipple
  OdOpenGLMetaRecType_VPoint             = 0x0B, // Single point
  OdOpenGLMetaRecType_VLine              = 0x0C, // Single line
  OdOpenGLMetaRecType_IPoint             = 0x0D, // Single indexed point
  OdOpenGLMetaRecType_ILine              = 0x0E, // Single indexed line
  OdOpenGLMetaRecType_HLRStencil         = 0x0F, // Only for HLR mode
  OdOpenGLMetaRecType_EnableShading      = 0x10, // Enable shading flag
  OdOpenGLMetaRecType_DisableShading     = 0x11, // Disable shading flag
  OdOpenGLMetaRecType_Material           = 0x12, // Change material (shaded mode)
  OdOpenGLMetaRecType_UserEntry          = 0x13, // User can process own data entries
  OdOpenGLMetaRecType_InitTexture        = 0x14, // Set texture for raster image
  OdOpenGLMetaRecType_UninitTexture      = 0x15, // Reset texture for raster image
  OdOpenGLMetaRecType_SelectionMarker    = 0x16, // Stand-alone selection marker
  OdOpenGLMetaRecType_EnableMarkerArray  = 0x17, // Enable selection markers array
  OdOpenGLMetaRecType_DisableMarkerArray = 0x18, // Disable selection markers array
  OdOpenGLMetaRecType_SelectionFlags     = 0x19, // Display and selection geometry markers
  OdOpenGLMetaRecType_Lineweight         = 0x1A, // Lineweight
  OdOpenGLMetaRecType_Linestyle          = 0x1B, // Cap&Joint style
  OdOpenGLMetaRecType_TtfText            = 0x1C, // Cached TtfFont

  OdOpenGLMetaRecType_NTypes                     // == last + 1
};

// Convert OdOpenGLMetaRecType into string representation
const OdChar *chunkDecode(OdOpenGLMetaRecType rt); // Debug

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLMetaRecArrayType // Type of initialization array
{
  OdOpenGLMetaRecArrayType_Vertex       = 0x00, // glVertexPointer
  OdOpenGLMetaRecArrayType_Color        = 0x01, // glColorPointer
  OdOpenGLMetaRecArrayType_TexCoord     = 0x02, // glTexCoordPointer
  OdOpenGLMetaRecArrayType_Normal       = 0x03, // glNormalPointer

  OdOpenGLMetaRecArrayType_NTypes               // == last + 1
};

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLAttribute // List of frequently called attributes
{
  OdOpenGLAttribute_CullFace            = 0x00, // GL_CULL_FACE - not frequent (initially disabled)
  OdOpenGLAttribute_Blend               = 0x01, // GL_BLEND - not frequent (initially disabled)
  OdOpenGLAttribute_Lighting            = 0x02, // GL_LIGHTING - very frequent. Called on each triangleOut, polylineOut operations. (!droppable)
  OdOpenGLAttribute_Texture2D           = 0x03, // GL_TEXTURE_2D - very frequent. Called on each triangleOut, polylineOut operations. (!droppable)

  OdOpenGLAttribute_NTypes,                     // == last + 1
  OdOpenGLAttribute_Mask                = 0x0F
};

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLShading // List of shading OpenGL attributes
{
  OdOpenGLShading_NoColorOverride       = 0x00, // Avoid all color overrides

  OdOpenGLShading_NTypes,                       // == last + 1
  OdOpenGLShading_Mask                  = 0x01
};

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLFaceCulling // List of face culling states
{
  // Culling modes for faces
  OdOpenGLCullFace_None                 = 0x00, // Disable face culling (default state)
  OdOpenGLCullFace_Back                 = 0x01, // Enable back faces culling
  OdOpenGLCullFace_Front                = 0x02, // Enable front faces culling
  OdOpenGLCullFace_Default              = 0x03 // Invoke default faces culling method
};

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLSelectionFlags
{
  OdOpenGLSelFlag_DontDisplayUnhighlighted = (1 << 0),
  OdOpenGLSelFlag_DontSelectUnhighlighted  = (1 << 1),
  OdOpenGLSelFlag_DontDisplayHighlighted   = (1 << 2),
  OdOpenGLSelFlag_DontSelectHighlighted    = (1 << 3),
  OdOpenGLSelFlag_FlagsMask                = 15
};

/** \details
  <group ExRender_Classes>
*/
enum OdOpenGLAffection // List of metafile affections
{
  OdOpenGLAffection_Attributes          =  0x01,        // Affects attributes/disable/enable
  OdOpenGLAffection_Colors              = (0x01 << 1),  // Affects color
  OdOpenGLAffection_Materials           = (0x01 << 2),  // Affects materal
  OdOpenGLAffection_Arrays              = (0x01 << 3),  // Affects arrays
  OdOpenGLAffection_Geometry            = (0x01 << 4)   // Call's geometry rendering
};

/** \details
    This structure extends flat metafile containers by specific OpenGL elements.

    <group !!RECORDS_tkernel_apiref>
*/
struct OdOpenGLArrayWrapper
{
  void *m_pData;
  OdUInt32 m_uSize;

  OdOpenGLArrayWrapper()
    : m_pData(NULL)
    , m_uSize(0)
  {
  }

  void setArray(const void *pPtr, OdUInt32 size);
  void clearArray();
  void concatArray(const void *pPtr, OdUInt32 size);

  void setArray_dbl(const double *pPtr, OdUInt32 size);
  void concatArray_dbl(const double *pPtr, OdUInt32 size);

  void setArray_ush(const OdInt32 *pPtr, OdUInt32 size);
};

// Selection markers

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLSelMark
{
  OdOpenGLGsMarker gsMark;
  OdUInt32 uFrom;
  // OdUInt32 uLen;
};
typedef OdArray<OdOpenGLSelMark, OdMemoryAllocator<OdOpenGLSelMark> > OdOpenGLSelMarkArray;

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLFlatMetafileContainer : public OdFlatMetafileContainer
{
  public:
    OdArray<OdOpenGLArrayWrapper> m_ArrayElements; // Array contain pointers to some memory elements
    OdRxObjectPtrArray m_RxObjects; // Array contain pointers to some RxObjects to control they life cycle
  public:
    OdOpenGLFlatMetafileContainer();
    ~OdOpenGLFlatMetafileContainer();

    void clearOpenGLFlatContainer();

    OdUInt32 appendArrayElement(const OdOpenGLArrayWrapper &element)
    {
      m_ArrayElements.push_back(element);
      return m_ArrayElements.size() - 1;
    }
    OdUInt32 arrayElementsSize() const
    {
      return m_ArrayElements.size();
    }
    OdOpenGLArrayWrapper &arrayElement(OdUInt32 nElem)
    {
      return m_ArrayElements.at(nElem);
    }
    const OdOpenGLArrayWrapper &arrayElement(OdUInt32 nElem) const
    {
      return m_ArrayElements.at(nElem);
    }

    OdUInt32 lastArrayElement() const { return arrayElementsSize() - 1; }

    // Base container overrides
    void optimizeMemory(OdUInt32 uSize);

    OdBaseMetafileContainerReader *createReader() const;
    OdBaseMetafileContainerWriter *createWriter() const;

    // Extended OpenGL container manipulators

    // Direct access to stream memory
    inline const OdUInt8 *memoryPtr() const
    {
      return m_pData;
    }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLFlatMetafileContainerWriter : public OdFlatMetafileContainerWriter
{
  public:
    OdOpenGLFlatMetafileContainerWriter() : OdFlatMetafileContainerWriter() { }
    OdOpenGLFlatMetafileContainerWriter(OdBaseMetafileContainer *pContainer) : OdFlatMetafileContainerWriter(pContainer) { }

    inline OdBaseMetafileContainer *createContainer() const
    {
      return new OdOpenGLFlatMetafileContainer();
    }

    inline OdOpenGLFlatMetafileContainer *glContainer() const { return static_cast<OdOpenGLFlatMetafileContainer*>(container()); }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLFlatMetafileContainerReader : public OdFlatMetafileContainerReader
{
  public:
    OdOpenGLFlatMetafileContainerReader() : OdFlatMetafileContainerReader() { }
    OdOpenGLFlatMetafileContainerReader(OdBaseMetafileContainer *pContainer) : OdFlatMetafileContainerReader(pContainer) { }

    inline OdBaseMetafileContainer *createContainer() const
    {
      return new OdOpenGLFlatMetafileContainer();
    }

    inline OdOpenGLFlatMetafileContainer *glContainer() const { return static_cast<OdOpenGLFlatMetafileContainer*>(container()); }
};

/* Note:
   In case of optimization stream reading process, inheritance from OdOpenGLMetafileInputStream excluded.
   Now OdOpenGLMetafileReader is standalone class, which reads linear stream directly from memory pointer.
*/
// Optimization definitions:
#define OD_OGL_RDR_SEEKFWD(pMemPtr, uSize) (pMemPtr) += (uSize)
#define OD_OGL_RDR_SEEKBWD(pMemPtr, uSize) (pMemPtr) -= (uSize)
#define OD_OGL_RDR_SEEKINC(pMemPtr) (pMemPtr)++
#define OD_OGL_RDR_SEEKDEC(pMemPtr) (pMemPtr)--

#define OD_OGL_RDR_READVAL(cType, pMemPtr) *(cType*)(pMemPtr)
#define OD_OGL_RDR_READVALOFFSET(cType, pMemPtr, uOffset) *(cType*)((pMemPtr) + (uOffset))
#define OD_OGL_RDR_READVALRETYPE(cOutType, cInType, pMemPtr) (cOutType)OD_OGL_RDR_READVAL(cInType, pMemPtr)
#define OD_OGL_RDR_READVALOFFSETRETYPE(cOutType, cInType, pMemPtr, uOffset) (cOutType)OD_OGL_RDR_READVALOFFSET(cInType, pMemPtr, uOffset)
#define OD_OGL_RDR_GETPTRNATIVE(cType, pMemPtr) (cType*)(pMemPtr)
#define OD_OGL_RDR_GETPTRNATIVEOFFSET(cType, pMemPtr, uSize) (cType*)((pMemPtr) + (uSize))
#define OD_OGL_RDR_GETPTRSTORAGE(imedType, name, count)
#define OD_OGL_RDR_GETPTR(cType, imedType, name, count, pMemPtr) OD_OGL_RDR_GETPTRNATIVE(cType, pMemPtr)
#define OD_OGL_RDR_GETPTROFFSET(cType, imedType, name, count, pMemPtr, uSize) OD_OGL_RDR_GETPTRNATIVEOFFSET(cType, pMemPtr, uSize)

#define OD_OGL_RDR_INITIATE(pMemPtr, uSize) const OdUInt8 *pMemPtrReadFor = (const OdUInt8*)pMemPtr + uSize
#define OD_OGL_RDR_CHECKEOF(pMemPtr) pMemPtr < pMemPtrReadFor

#include "TD_PackPop.h"

#endif // _EXOPENGLMETAFILESTREAM_INCLUDED_
