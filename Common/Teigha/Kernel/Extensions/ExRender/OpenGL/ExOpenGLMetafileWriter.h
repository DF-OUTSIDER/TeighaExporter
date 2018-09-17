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

#ifndef _EXOPENGLMETAFILEWRITER_INCLUDED_
#define _EXOPENGLMETAFILEWRITER_INCLUDED_

#include "TD_PackPush.h"

#include "ExOpenGLMetafileStream.h"
#include "GsOpenGLDefs.h"
#include "PseudoGLDefs.h"
#include "OpenGLMetafileStream.h"
#include "Gs/Gs.h"
#include "OdVector.h"

// Texture coordinates recalculation entry

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
struct OdOpenGLDelayedMappingEntry
{
  OdOpenGLDelayedMappingEntry() : pContainer(NULL), bFlushed(false) { }
  virtual ~OdOpenGLDelayedMappingEntry() { }

  OdBaseMetafileContainer *pContainer;

  OdUInt32Vector m_texIds;

  bool bFlushed;

  struct ArrayElement
  {
    OdUInt32 texId;
    OdGePoint3d verts[3];
    OdGeVector3d normal;
    OdUInt32 crdIds[3];
  };
  OdVector<ArrayElement, OdMemoryAllocator<ArrayElement> > m_Elements;
};

// OpenGL metafile stream writer

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdOpenGLMetafileWriter : public OdOpenGLMetafileOutputStream
{
  protected:
    struct TraitsOptions
    {
      union StateFlags
      {
        OdUInt16 flags;
        struct Regs
        {
          OdUInt8 attributes;
          OdUInt8 shading;
        } regs;

        void setAttribute(OdOpenGLAttribute nAttr, bool bSet) { SETBIT(regs.attributes, 1 << nAttr, bSet); }
        void setAttribute(OdOpenGLAttribute nAttr) { SETBIT_1(regs.attributes, 1 << nAttr); }
        void resetAttribute(OdOpenGLAttribute nAttr) { SETBIT_0(regs.attributes, 1 << nAttr); }
        bool getAttribute(OdOpenGLAttribute nAttr) const { return GETBIT(regs.attributes, 1 << nAttr); }

        void setShading(OdOpenGLShading nShading, bool bSet) { SETBIT(regs.shading, 1 << nShading, bSet); }
        void setShading(OdOpenGLShading nShading) { SETBIT_1(regs.shading, 1 << nShading); }
        void resetShading(OdOpenGLShading nShading) { SETBIT_0(regs.shading, 1 << nShading); }
        bool getShading(OdOpenGLShading nShading) const { return GETBIT(regs.shading, 1 << nShading); }

        void setAll() { flags = 0xFFFF; }
        void resetAll() { flags = 0; }

        bool getAttributeMaskAll() const { return (regs.attributes & OdOpenGLAttribute_Mask) == OdOpenGLAttribute_Mask; }
        bool getShadingMaskAll() const { return (regs.shading & OdOpenGLShading_Mask) == OdOpenGLShading_Mask; }
      } bOptFlagsState, bOptFlagsTouch;
      OdUInt16 eCullMode  : 8;  // treat as OdUInt8
      OdUInt16 bHLR       : 8;  // treat as OdUInt8

      OdOpenGLFaceCulling cullMode() const { return (OdOpenGLFaceCulling)eCullMode; }
      void setCullMode(OdOpenGLFaceCulling mode) { eCullMode = (OdUInt8)mode; }

      OdUInt8 getCombinedAttribute(OdOpenGLAttribute nAttr) const {
        return ((bOptFlagsState.regs.attributes >> nAttr) & 1) | (((bOptFlagsTouch.regs.attributes >> nAttr) << 1) & 2);
      }
      void setCombinedAttribute(OdOpenGLAttribute nAttr, OdUInt8 nSet) {
        bOptFlagsState.setAttribute(nAttr, GETBIT(nSet, 1));
        bOptFlagsTouch.setAttribute(nAttr, GETBIT(nSet, 2));
      }
      OdUInt8 getCombinedShading(OdOpenGLShading nShad) const {
        return ((bOptFlagsState.regs.shading >> nShad) & 1) | (((bOptFlagsTouch.regs.shading >> nShad) << 1) & 2);
      }
      void setCombinedShading(OdOpenGLShading nShad, OdUInt8 nSet) {
        bOptFlagsState.setShading(nShad, GETBIT(nSet, 1));
        bOptFlagsTouch.setShading(nShad, GETBIT(nSet, 2));
      }

      void setDefaults();
    };
    struct PrevState : public TraitsOptions
    {
      //OdUInt8 bClientState[OdOpenGLMetaRecArrayType_NTypes];
      ODCOLORREF crColor;
      OdOpenGLLinePatt lnPatt;
      OdOpenGLFillPatt fillPatt;
      OdGsMarker gsMarker;
      OdUInt8 nSelectionFlags;
      OdOpenGLLwdSetting lwd;
      OdOpenGLLwdStyle lwdStyle;

      void setDefaults();
    };
    PrevState prevState;
  public:
    enum PackageType
    {
      PackageType_Null = 0, // Packaging disabled
      // Vertex primitives
      // Implemented as bit set now:
      // |1|2|3|4|5|6|
      // |L|T|I|C|N|T|
      PackageType_IsLines     = 1,
      PackageType_IsTriangles = 2,
      PackageType_IsPoints    = 3, // IsLines vs IsTriangles == IsPoints;
      PackageType_IsTypeMask  = PackageType_IsPoints, // This is type mask.
      PackageType_IsIndexed   = 4,
      PackageType_IsColors    = 8,
      PackageType_IsNormals   = 16,
      PackageType_IsTextured  = 32,
      // Non-indexed primitives
      // Points
      PackageType_VPoints  = PackageType_IsPoints,                             // Pack points with vertices
      PackageType_VCPoints = PackageType_IsPoints    | PackageType_IsColors,   // Pack points with vertices and colors
      // Lines
      PackageType_VLines   = PackageType_IsLines,                              // Pack lines with vertices
      PackageType_VCLines  = PackageType_IsLines     | PackageType_IsColors,   // Pack lines with vertices and colors
      PackageType_VTLines  = PackageType_IsLines     | PackageType_IsTextured, // Pack lines with vertices and texture coordinates
      PackageType_VCTLines = PackageType_VCLines     | PackageType_IsTextured, // Pack lines with vertices, colors and texture coordinates
      // Triangles
      PackageType_VTri     = PackageType_IsTriangles,                          // Pack triangles with vertices
      PackageType_VCTri    = PackageType_IsTriangles | PackageType_IsColors,   // Pack triangles with vertices and colors
      PackageType_VNTri    = PackageType_IsTriangles | PackageType_IsNormals,  // Pack triangles with vertices and normals
      PackageType_VCNTri   = PackageType_VCTri       | PackageType_IsNormals,  // Pack triangles with vertices, colors and normals
      PackageType_VNTTri   = PackageType_VNTri       | PackageType_IsTextured, // Pack triangles with vertices, normals and texture coordinates
      PackageType_VCNTTri  = PackageType_VCNTri      | PackageType_IsTextured, // Pack triangles with vertices, colors, normals and texture coordinates
      // Indexed primitives
      // Points
      PackageType_IPoints  = PackageType_IsPoints    | PackageType_IsIndexed,  // Pack indexed points
      PackageType_ICPoints = PackageType_IPoints     | PackageType_IsColors,   // Pack indexed points with colors
      // Lines
      PackageType_ILines   = PackageType_IsLines     | PackageType_IsIndexed,  // Pack indexed lines
      PackageType_ICLines  = PackageType_ILines      | PackageType_IsColors,   // Pack indexed lines with colors
      PackageType_ITLines  = PackageType_ILines      | PackageType_IsTextured, // Pack indexed lines with texture coordinates
      PackageType_ICTLines = PackageType_ICLines     | PackageType_IsTextured, // Pack indexed lines with colors and texture coordinates
      // Triangles
      PackageType_ITri     = PackageType_IsTriangles | PackageType_IsIndexed,  // Indexed triangles
      PackageType_ICTri    = PackageType_ITri        | PackageType_IsColors,   // Indexed triangles with vertex colors
      PackageType_INTri    = PackageType_ITri        | PackageType_IsNormals,  // Triangles with normals
      PackageType_ICNTri   = PackageType_ICTri       | PackageType_IsNormals,  // Triangles with colors and normals
      PackageType_INTTri   = PackageType_INTri       | PackageType_IsTextured, // Triangles with normals and texture coordinates
      PackageType_ICNTTri  = PackageType_ICNTri      | PackageType_IsTextured, // Triangles with colors, normals and texture coordinates
    };
    enum PointMode
    {
      PointMode_Unopt = 0, // Drop all points as points to stream
      PointMode_Opt,       // Drop each two points as points to stream
      PointMode_High,      // Drop all points as lines if lines package enabled
      PointMode_Sep        // Drop points as separate array to stream
    };
  protected:
    bool m_bIsGLES;
    struct ColorTriplet
    {
      OdUInt8 r, g, b;
#if (OD_OGL_NCOLORCOMPONENTS == 4)
      OdUInt8 a;
#endif
    };
    typedef OdVector<ColorTriplet, OdMemoryAllocator<ColorTriplet> > OdColorTripletArray;
    struct TextureCoord
    {
      double s, t;
    };
    typedef OdVector<TextureCoord, OdMemoryAllocator<TextureCoord> > OdTextureCoordArray;
    typedef OdVector<OdOpenGLSelMark, OdMemoryAllocator<OdOpenGLSelMark> > OdOpenGLSelMarkArray;
    struct PackageEntry
    {
      PackageEntry() : packType(PackageType_Null) {
        m_inputTraits.setDefaults();
        m_outputTraits.setDefaults();
      }

      PackageType           packType;
      OdGePoint3dVector     m_vertArray;
      OdColorTripletArray   m_clrArray;
      OdGeVector3dVector    m_normArray;
      OdTextureCoordArray   m_texArray;
      OdOpenGLSelMarkArray  m_selMarkArray;
      // For indexed modes
      OdInt32Vector         m_indices;
      // Attribute packaging traits
      TraitsOptions         m_inputTraits;
      TraitsOptions         m_outputTraits;
    } m_packEntry[2];

    OdUInt32 m_uAffects;
    OdUInt32 m_uIndexIncr;
    OdUInt32 m_uMaxId;
    ODCOLORREF m_crHLR;
    ODCOLORREF m_crPrevColor;
    OdUInt32 m_uReserve[2];
    OdOpenGLMetaRecType m_prevChunk;
    PointMode m_pointMode;
    OdGePoint3d m_prevPoint; // Only for PointMode_Opt mode
    OdUInt8 m_bPrevPoint;    // Only for PointMode_Opt mode
    bool m_bStrictAlignment; // Force strict alignment process
    struct PointOptEntry
    {
      OdGePoint3dVector    m_vertArray;
      OdColorTripletArray  m_clrArray;
      OdTextureCoordArray  m_texArray;
      OdOpenGLSelMarkArray m_selMarkArray;

      bool empty() const { return m_vertArray.empty(); }
      bool hasData() const { return !m_vertArray.empty(); }
    } m_pointEntry; // Only for PointMode_Sep mode
    struct WriterSettings
    {
      enum Flags
      {
        kFlushing          = (1 << 0), // Flushing procedure initiated
        kIndexedMode       = (1 << 1), // Indexed primitives packaging enabled
        kInsertMode        = (1 << 2), // 3d modes can pack lines and triangles in separate arrays
        // Here is two packaging traits: one for line segments, second for triangles. This two traits works
        // absolutely separately only in 3d modes, in 2d optimized mode work only one trait at time (for 2d data),
        // but in optimization cases second trait may be used for triangles until color is not changed.
        kLinesRepMode      = (1 << 3), // Add points onto line ends (to make lines visible in case if them parallel to viewing direction)
        kRestoreGsMarkers  = (1 << 4)  // Update GsMarker after flushing procedure completion
      };
      OdUInt16 m_uFlags;
      inline void resetFlags() { m_uFlags = 0; }
#define OD_OGL_DECLAREWSFLAGACCESORS(FlagName) \
      inline bool is##FlagName() const { return GETBIT(m_uFlags, k##FlagName); } \
      inline void set##FlagName() { SETBIT_1(m_uFlags, k##FlagName); } \
      inline void reset##FlagName() { SETBIT_0(m_uFlags, k##FlagName); } \
      inline void set##FlagName(bool bSet) { if (bSet) set##FlagName(); else reset##FlagName(); }
      OD_OGL_DECLAREWSFLAGACCESORS(Flushing)
      OD_OGL_DECLAREWSFLAGACCESORS(IndexedMode)
      OD_OGL_DECLAREWSFLAGACCESORS(InsertMode)
      OD_OGL_DECLAREWSFLAGACCESORS(LinesRepMode)
      OD_OGL_DECLAREWSFLAGACCESORS(RestoreGsMarkers)
#undef OD_OGL_DECLAREWSFLAGACCESORS

      WriterSettings() : m_uFlags(0) { }
    } m_settings;
    OdVector<OdOpenGLDelayedMappingEntry *, OdMemoryAllocator<OdOpenGLDelayedMappingEntry *> > m_delEntry;
    OdUInt32 m_nPackReserve;
  public:
    // VC6 workaround
    typedef OdOpenGLMetafileWriter::PackageEntry FriendPEntry;
  protected:
    friend void packTriangleVertices(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                     const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3);
    friend void packTriangleColors(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                   const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                                   OdUInt8 a);
    friend void packTriangleNormals(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                    const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3);
    friend void packTriangleTexCoords(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                      const double *pTC1, const double *pTC2, const double *pTC3);

    inline TraitsOptions &getTraitsOptions(OdUInt32 trait, bool input)
    {
      if (input)
        return m_packEntry[trait].m_inputTraits;
      else
        return m_packEntry[trait].m_outputTraits;
    }

    void flushTraitsOptions(TraitsOptions &options);
    void resolveTraitsConflicts(TraitsOptions &prev, const TraitsOptions &next);

    void flushTriDelayCache(OdUInt32 nArrayId);
    bool validateGsMarkers(int nPackEntry);
  public:
    OdOpenGLMetafileWriter(bool bIsES = false)
      : OdOpenGLMetafileOutputStream()
      , m_bIsGLES(bIsES)
      , m_uAffects(0)
      , m_uIndexIncr(0)
      , m_uMaxId(0)
      , m_pointMode(PointMode_Unopt)
      , m_bPrevPoint(0)
#ifdef TD_STRICT_ALIGNMENT
      , m_bStrictAlignment(true)
#else
      , m_bStrictAlignment(false)
#endif
      , m_nPackReserve(256)
    {
      setContainerIO(new OdOpenGLFlatMetafileContainerWriter());
      m_uReserve[0] = m_uReserve[1] = 0;
    }
    virtual ~OdOpenGLMetafileWriter()
    {
      if (containerIO())
        delete containerIO();
    }

    inline OdOpenGLFlatMetafileContainerWriter *glContainerIO() const { return static_cast<OdOpenGLFlatMetafileContainerWriter*>(containerIO()); }

    inline OdOpenGLFlatMetafileContainer *glContainer() const { return static_cast<OdOpenGLFlatMetafileContainer*>(containerIO()->container()); }

    inline bool isES() const { return m_bIsGLES; }

    void beginWriting(OdBaseMetafileContainer *pContainer);
    void completeWriting();

    void clearArrays();

    inline void setPointMode(PointMode newMode)
    {
      m_pointMode = newMode;
      if (newMode == PointMode_Opt)
      {
        if (isES())
          throw OdError(eInvalidInput);
        m_bPrevPoint = false;
      }
    }
    // Triangles insertion mode
    inline void setInsertMode(bool bSet) { m_settings.setInsertMode(bSet); }
    inline bool isInsertModeEnabled() const { return m_settings.isInsertMode(); }
    // Lines replication as points mode
    inline void setLinesRepMode(bool bSet) { m_settings.setLinesRepMode(bSet); }
    inline bool isLinesRepModeEnabled() const { return m_settings.isLinesRepMode(); }
    // Force strict alignment process
    inline void forceStrictAlignment(bool bEnable) { m_bStrictAlignment = bEnable; }
    inline bool isStrictAlignmentForced() const { return m_bStrictAlignment; }

    inline bool isActive() const { return containerIO()->container() != NULL; }
    inline bool isCurrent(const OdBaseMetafileContainer *pObj) const
    {
      return containerIO()->container() == pObj;
    }
    inline bool isPackaging(OdUInt32 nLayer) const { return m_packEntry[nLayer].packType != PackageType_Null; }
    inline bool hasGeometry(OdUInt32 nLayer) const
    { return (m_settings.isIndexedMode()) ? !m_packEntry[nLayer].m_indices.isEmpty() : !m_packEntry[nLayer].m_vertArray.isEmpty(); }
    inline bool isPackaging() const { return isPackaging(0) || isPackaging(1); }

    inline void writeChunk(OdOpenGLMetaRecType recType)
    {
      writeUInt8((OdUInt8)recType);
      m_prevChunk = recType;
    }

    inline OdGsMarker prevMarker() const { return prevState.gsMarker; }
  public:
    void flushPackager(PackageType type, OdUInt32 apprLayer = 0xFFFFFFFF);

    bool flushOnIndexedMode(bool newMode);

    inline bool indexedMode() const { return m_settings.isIndexedMode(); }

    void beginIndexedMode(bool bColors, bool bNormals, bool bTexCoords,
                          OdUInt32 nPoints, const OdGePoint3d *pPoints,
                          bool bColoredLines = false, bool bTexturedLines = false);

    // Packagers
    /* This enable/disable methods used in memory optimization in frequently called parts. */
    void glEnableOpt(OdOpenGLAttribute attrib);
    void glDisableOpt(OdOpenGLAttribute attrib);
    void glEnableOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input = true);
    void glDisableOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input = true);
    void glUnsetOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input = true);
    void glDropOpt(OdOpenGLAttribute attrib);
    /* Use similar logic as in glEnableOpt/glDisableOpt. */
    void glEnableShadingOpt(OdOpenGLShading shad);
    void glDisableShadingOpt(OdOpenGLShading shad);
    void glEnableShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input = true);
    void glDisableShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input = true);
    void glUnsetShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input = true);
    void glDropShadingOpt(OdOpenGLShading shad);
    /* - - - */
    void glColor(ODCOLORREF color);
    ODCOLORREF glTransparency(OdUInt8 alpha, bool bUpdateBlending = true);
    void glSelectionMarker(OdGsMarker gsMarker);
    void glSelectionFlags(OdUInt8 selFlags);
    void glEnableSelMarkArray();
    void glDisableSelMarkArray();
    void glLineweight(OdDb::LineWeight lwd);
    void glLineweight(double lwd, bool bAbsolute = false);
    void glLineweight(OdInt32 lwd);
    void glLineStyle(bool bByDefault, OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle);
    void glCullFace(OdOpenGLFaceCulling mode);
    void glCullFaceToTraits(OdOpenGLFaceCulling mode, OdUInt32 trait, bool input = true);
    void glUnsetCullFaceToTraits(OdUInt32 trait, bool input = true);
    void glLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state);
    void glDropLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state);
    void glPolygonStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state);
    void glDropPolygonStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state);
    void glEnableArray(OdOpenGLMetaRecArrayType type, OdGLint /*stride*/ /* 0 for default */);
    void glDisableArray(OdOpenGLMetaRecArrayType type);
    void glPoint(const OdGePoint3d &point);
    void glLine(const OdGePoint3d &point1, const OdGePoint3d &point2);
    void glPoint(OdInt32 id);
    void glLine(OdInt32 id1, OdInt32 id2);
    void glDrawArrays(OdGLenum mode, OdGLint first, OdGLsizei count);
    void glDrawElements(OdGLenum mode, OdGLsizei count, const OdInt32 *indices);

    void glPackLine(OdInt32 nPoints, const OdGePoint3d *pPoints);
    //void glPackLines(OdInt32 nLines, const OdGePoint3d *pPoints, const OdGeVector3d *pVectors);
    void glPackCLine(OdInt32 nPoints, const OdGePoint3d *pPoints, const OdCmEntityColor *pColors);
    //void glPackCLines(OdInt32 nLines, const OdGePoint3d *pPoints, const OdGeVector3d *pVectors, const OdCmEntityColor *pColors);

    void glPackPoint(const OdGePoint3d *pPt);
    //void glPackPoints(OdInt32 nPoints, const OdGePoint3d *pPoints);
    void glPackCPoint(const OdGePoint3d *pPt, const OdCmEntityColor *color);
    //void glPackCPoints(OdInt32 nPoints, const OdGePoint3d *pPoints, const OdCmEntityColor *pColors);

    void glPackLine(OdInt32 nPoints, const OdInt32 *pPoints);
    void glSetPackILineColors(const OdInt32 *ids, const OdCmEntityColor *color1, const OdCmEntityColor *color2);

    void reserveBuffers(OdUInt32 nPoints);
    void reserveBuffers(OdUInt32 valIL, OdUInt32 valTR);
    void glPackTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3);
    void glPackCTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                         const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3);
    void glPackNTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                         const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3);
    void glPackCNTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                          const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                          const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3);
    void glPackNTTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                          const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3,
                          const double *pTC1, const double *pTC2, const double *pTC3,
                          bool bDelayMapping = false);
    void glPackCNTTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                           const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                           const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3,
                           const double *pTC1, const double *pTC2, const double *pTC3,
                           bool bDelayMapping = false);
    void glPackITriangle(const OdInt32 *ids);
    void glSetPackITriColors(const OdInt32 *ids,
                             const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3);
    void glSetPackITriNormals(const OdInt32 *ids,
                              const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3);
    void glSetPackITriTexCoords(const OdInt32 *ids,
                                const double *pTC1, const double *pTC2, const double *pTC3,
                                bool bDelayMapping = false);

    // Arrays processing
    OdUInt32 glAppendArray(const void *pPtr, OdUInt32 size, bool writeId = true);
    void glConcatArray(OdUInt32 id, const void *pPtr, OdUInt32 size, bool writeId = false);
    OdUInt32 glAppendArray_dbl(const double *pPtr, OdUInt32 size, bool writeId = true);
    void glConcatArray_dbl(OdUInt32 id, const double *pPtr, OdUInt32 size, bool writeId = false);
    OdUInt32 glAppendRxObject(OdRxObjectPtr pObject, bool writeId = true);
    OdUInt32 glAppendArray(const ColorTriplet *pPtr, OdUInt32 size, bool writeId = true);

    void glAppendMaterial(OdRxObjectPtr pMaterial);

    void glAppendUserEntry();
    void glAppendUserEntry(const void *pMem, OdUInt32 length);

    void glInitTexture(const void *dataSet, OdUInt32 sizeofDataSet, OdRxObject *pData = NULL,
                       const void *subDataSet = NULL, OdUInt32 sizeofSubDataSet = 0,
                       const void *subDataSet2 = NULL, OdUInt32 sizeofSubDataSet2 = 0);
    void glUninitTexture();

    void glHLRStencil(bool beginFlag);
    void glHLRStencilToTraits(bool beginFlag, OdUInt32 trait, bool input = true);
    void glUnsetHLRStencilToTraits(OdUInt32 trait, bool input = true);
    inline void setHLRColor(ODCOLORREF crColor) { m_crHLR = crColor; }

    void glTtfText(OdUInt64 fontKeyA, OdUInt64 fontKeyB, const OdUInt32 *pText, OdUInt32 nText,
                   const OdGeMatrix3d &baseTransform, const OdGeVector3d &stepDir);

    void startDelayMappingEntry(OdOpenGLDelayedMappingEntry *pEntry);
    void clearDelayMappingCache();
    void glResetTexCoords(const OdUInt32 ids[3], const OdGePoint2d coords[3]);

    void setupPackagerGrowOptions(OdInt32 newGrowSet, OdUInt32 nPackReserve);
};

void packTriangleColors(OdOpenGLMetafileWriter::FriendPEntry &pe,
                        const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                        OdUInt8 a = 255);

#include "TD_PackPop.h"

#endif // _EXOPENGLMETAFILEWRITER_INCLUDED_
