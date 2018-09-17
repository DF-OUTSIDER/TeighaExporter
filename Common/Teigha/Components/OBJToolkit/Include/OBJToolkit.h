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

#ifndef OBJTOOLKIT_H
#define OBJTOOLKIT_H

// Teigha
#include "SharedPtr.h"
#include "RxVariant.h"
#include "RxModule.h"
#include "RxObjectImpl.h"
#include "DynamicLinker.h"
#include "OdString.h"
#include "UInt32Array.h"

#include "Ge/GePoint3dArray.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbSurface.h"
#include "Ge/GeKnotVector.h"

#include "Gi/GiDrawable.h"
#include "DbBaseDatabase.h"
#include "Br/BrBrep.h"

#include "TD_PackPush.h"

#ifdef OBJTOOLKIT_EXPORTS
#define OBJTOOLKIT_EXPORT         OD_TOOLKIT_EXPORT
#define OBJTOOLKIT_EXPORT_STATIC  OD_STATIC_EXPORT
#else                               
#define OBJTOOLKIT_EXPORT         OD_TOOLKIT_IMPORT
#define OBJTOOLKIT_EXPORT_STATIC  OD_STATIC_IMPORT
#endif

namespace OBJToolkit {

  enum eOBJEntType {
      eOBJEntUnknown  = 0
    , eOBJEntPoints   = 1
    , eOBJEntLine     = 2
    , eOBJEntFace     = 3
    , eOBJEntCurve    = 4
    , eOBJEntCurve2D  = 5
    , eOBJEntSurface  = 6
  };

  enum OdMtlOptVal {

    mopvMultiplier = 1   // factor 
    , mopvClamp = 2   // on/off
    , mopvBlendU = 3   // on/off
    , mopvBlendV = 4   // on/off
    , mopvColorCorrection = 5   // on/off
    , mopvIMFChan = 6   // r/g/b/m/l/z
    , mopvMMBase = 7
    , mopvMMGain = 8
    , mopvOU = 9
    , mopvOV = 10
    , mopvOW = 11
    , mopvSU = 12
    , mopvSV = 13
    , mopvSW = 14
    , mopvTU = 15
    , mopvTV = 16
    , mopvTW = 17
    , mopvTexRes = 18
    , mopvReflectionType = 19
    , mopvBoost = 20  // value
    , mopvFilename = 21  // picture filename

    , mopvEnd = 0

  };

  enum eMtlReflType {
    eReflSphere = 0,
    eReflCubeTop = 1,
    eReflCubeBottom = 2,
    eReflCubeFront = 3,
    eReflCubeBack = 4,
    eReflCubeLeft = 5,
    eReflCubeRight = 6,
    eReflUnknown = 100
  };

  enum OdMtlTokens {

    mtNewMtl = 0

    // Following properties should be set on latest created material

    , mtNs = 1
    , mtNi = 2
    , mtTr = 3
    , mtIllum = 4
    , mtSharpness = 5

    , mtD = 6

    , mtKa = 7
    , mtKd = 8
    , mtKs = 9
    , mtTf = 10


    // Options

    , mtMapKa = 20
    , mtMapKd = 21
    , mtMapKs = 22
    , mtMapD = 23
    , mtMapNs = 24
    , mtDecal = 25
    , mtDisp = 26
    , mtBump = 27
    , mtRefl = 28

    , mtDHalo = 29
    , mtMapAat = 30 // IAE: I haven't ever seen mtl files which use map_aat

    , mtUnknown = mtMapAat + 1
  };

  enum eObjCurveType {
    kBSpline = 0,
    kBezier = 1,
    kCardinal = 2,
    kTaylor = 3,
    kUnknown = 4
  };

  enum ObjLoopType
  {
    lTrim = 0,
    lHole = 1,
  };

/** \details
Class which implements a material of MTL file
*/
class OBJTOOLKIT_EXPORT OdMtlOptions : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdMtlOptions);

  virtual OdVariant getOption(OdMtlOptVal optVal) = 0;

};
typedef OdSmartPtr<OdMtlOptions> OdMtlOptionsPtr;

/** \details
Class which implements a material of MTL file
*/
class OBJTOOLKIT_EXPORT OdMtl : public OdRxObject
{
public:

  enum eMtlDataType {
    eMtlUndef           = 0,
    eMtlRGB             = 1,
    eMtlSpectralFactor  = 2,
    eMtlXYZ             = 3
  };

  ODRX_DECLARE_MEMBERS(OdMtl);

  virtual OdString        getName() const = 0;
  virtual OdString        getPath() const = 0;

  /*
    Refraction index (from 0 to 1000)
    if hasNs() == false, will return default value 0.
  */
  virtual double          getNs() = 0;
  virtual bool            hasNs() = 0;

  /*
    Sharpness (0 .. 1000)
    if hasSharpness() == false, will return default value 60.
  */
  virtual OdUInt16        getSharpness() = 0;
  virtual bool            hasSharpness() = 0;

  /*
    Map texture anti aliasing
    if hasAntiAliasing() == false, will return default value false
  */
  virtual bool            getAntiAliasing() = 0;
  virtual bool            hasAntiAliasing() = 0;

  /*
    Optical density
    if hasNi() == false, will return default value false
  */
  virtual double          getNi() = 0;
  virtual bool            hasNi() = 0;

  /*
    Dissolve
    if hasD() == false, will return default value 1.
  */
  virtual double          getD() = 0;
  virtual bool            hasD() = 0;

  /*
  Dissolve halo
  if hasDHalo() == false, will return default value false
  */
  virtual bool            getDHalo() = 0;
  virtual bool            hasDHalo() = 0;

  /*
    Transparency (1 - Dissolve)
    if hasTr() == false, will return default value 0.
  */
  virtual double          getTr() = 0;
  virtual bool            hasTr() = 0;

  /*
    Sometimes both values, D and Tr appear in files, this function returns Opacity
    based on appearance of D and Tr
    if hasOpacity() == false, will return default value for Dissolve 1.
  */
  virtual double          getOpacity() = 0;
  virtual bool            hasOpacity() = 0;

  /*
    Standard .obj illumination schemas
    if hasIllumination() == false, will return default value 0
  */
  virtual OdUInt8         getIllumination() = 0;
  virtual bool            hasIllumination() = 0;

  virtual eMtlDataType    getAmbientReflectivity(double &Ka1, double &Ka2, double &Ka3, OdString &mapKaRflFilename, double &KaFactor) = 0;
  virtual eMtlDataType    getDiffuseReflectivity(double &Kd1, double &Kd2, double &Kd3, OdString &map_Kd_rfl_filename, double &KdFactor) = 0;
  virtual eMtlDataType    getSpecularReflectivity(double &Ks1, double &Ks2, double &Ks3, OdString &mapKsRflFilename, double &KsFactor) = 0;
  virtual eMtlDataType    getTransmissionFilter(double &Tf1, double &Tf2, double &Tf3, OdString &mapTfRflFilename, double &TfFactor) = 0;

  virtual OdMtlOptionsPtr getOptions(OdMtlTokens optionsGroup) = 0;
};
typedef OdSmartPtr<OdMtl> OdMtlPtr;

class OBJTOOLKIT_EXPORT OdMtlIterator : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdMtlIterator);

  virtual void start() = 0;
  virtual bool done() const = 0;
  virtual const OdString& item() const = 0;
  virtual void step() = 0;
  virtual OdMtlPtr element() = 0;

};
typedef OdSmartPtr<OdMtlIterator> OdMtlIteratorPtr;

/** \details
Class which implements database database of materials from MTL file
*/
class OBJTOOLKIT_EXPORT OdMtlDb : public OdRxObject
{
  
public:
  
  OdGiRasterImage *m_pImg;

  ODRX_DECLARE_MEMBERS(OdMtlDb);

  virtual OdUInt32  getMaterialsCount() const = 0;
  virtual OdMtlPtr  getMaterial(OdString &sMaterialName, bool createIfNotExists = false) = 0;

  virtual OdMtlIteratorPtr createMtlIterator() = 0;

};
typedef OdSmartPtr<OdMtlDb> OdMtlDbPtr;

class OdObjGeomEntity;
class OdObjSurface;
class OdObjBrFile;

class OBJTOOLKIT_EXPORT OdObjEntityIterator : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjEntityIterator);

  virtual void start(bool skipFace = true) = 0;
  virtual bool done() const = 0;
  virtual OdUInt32 item() const = 0;
  virtual void step(bool skipFace = true) = 0;
  virtual bool seek(const OdUInt32& entityIndex) = 0;
  virtual const OdObjGeomEntity* element() = 0;

};
typedef OdSmartPtr<OdObjEntityIterator> OdObjEntityIteratorPtr;

/** \details
This structure keeps entities derived from OdObjGeomEntity
*/
class OBJTOOLKIT_EXPORT OdObjGroup : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjGroup);

  /** Description:
  Returns true if there are no any objects derived from OdObjGeomEntity in the group.
  */
  virtual bool isEmpty() = 0;

  /**
  Returns group name
  */
  virtual OdString getGroupName() = 0;

  /**
  Creates an iterator through all entities inside group and set it into the start position.
  */
  virtual OdObjEntityIteratorPtr createEntitiesIterator() = 0;

};
typedef OdSmartPtr<OdObjGroup> OdObjGroupPtr;

class OBJTOOLKIT_EXPORT OdObjGroupIterator : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjGroupIterator);

  virtual void start(
    bool atBeginning = true/*, 
    bool skipErased = true*/) = 0;

  /** Description:
    Returns true if and only if the traversal by this Iterator object is complete.
  */  
  virtual bool done() const = 0;

  /** Description:
    Returns the Object ID of the *entity* currently referenced by this Iterator object.
  */
  virtual OdUInt32 item() const = 0;

  /** Description:
    Opens and returns the *entity* currently referenced by this Iterator object.

    Arguments:
    openMode (I) Mode in which to open the *entity*.
    openErasedEntity (I) If and only if true, *erased* records will be opened or retrieved.

    Remarks:
    Returns a SmartPointer to the opened *entity* if successful, otherwise a null SmartPointer.
  */
  virtual OdObjGroupPtr element() = 0;
  //OdDb::OpenMode openMode = OdDb::kForRead, 
  //bool openErasedEntity = false) = 0;

  /** Description:
    Steps this Iterator object.

    Arguments:
    forward (I) True to step *forward*, false to step backward.
    skipErased (I) If and only if true, *erased* records are skipped.
  */
  virtual void step(
    bool forward = true/*, 
    bool skipErased = true*/) = 0;

  /** Description:
    Positions this Iterator object at the specified record.
    Arguments:
    objectId (I) Object ID of the *entity*.
    pEntity(I) Pointer to the *entity*.
  */  
  virtual bool seek(const OdUInt32& groupIndex) = 0;

};
typedef OdSmartPtr<OdObjGroupIterator> OdObjGroupIteratorPtr;

/** \details
Class which implements OBJ file database
*/
class OBJTOOLKIT_EXPORT OdObjDb : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjDb);

  /** \details
  Returns true if all groups are empty, or no vertexes in the database
  */
  virtual bool                  isEmpty() = 0;

  /** \details
  Creates an iterator of groups
  */
  virtual OdObjGroupIteratorPtr createGroupsIterator() = 0;

  /** \details
  Returns base of materials
  */
  virtual OdMtlDbPtr            getMtlDb() = 0;

  /** \details
  Collects all faces of the group in one shell and returns its geometry
  */
  virtual OdResult getGroupShell(
    OdObjGroupPtr     pGroup,
    OdGePoint3dArray  &vertices,
    OdGePoint2dArray  &texCoo,
    OdGeVector3dArray &normals,
    OdUInt32Array     &faces,
    OdArray< OdMtlPtr > &faceMaterials) = 0;

  /** \details
  Returns a brep built for pSurface
  */
  virtual OdResult getBrep(OdBrBrep &brep, const OdObjSurface *pSurface, OdObjBrFile *&file) = 0;

  /** \details
  Returns an array of vertexes by their indexes

  \remarks
  returns false if inds is empty or no vertex with such index in database
  */
  virtual bool getVS(OdGePoint3dArray &vs, const OdUInt32Array &inds) = 0;

  /** \details
  Returns an array of texture coordinates by their indexes

  \remarks
  returns false if inds is empty or no texture coordinate with such index in database
  */
  virtual bool getVTS(OdGePoint2dArray &vts, const OdUInt32Array &inds) = 0;

  /** \details
  Fill an array of normals by their indexes

  \remarks
  returns false if inds is empty or no normal with such index in database
  */
  virtual bool getVNS(OdGeVector3dArray &vns, const OdUInt32Array &inds) = 0;
   
  /** \details
  Fill an array of a point in the parameters by their indexes

  \remarks
  returns false if inds is empty or no point with such index in database
  */
  virtual bool getVPS(OdGePoint3dArray &vps, const OdUInt32Array &inds) = 0;
  /** \details
  Calculate normals and fill an array of normals
  */
  virtual void calculateVertexNormals(OdObjGroupPtr pGroup) = 0;

};
typedef OdSmartPtr<OdObjDb> OdObjDbPtr;


struct OdObjImportOptions
{
  /** \details
  Flip U and V directions for BRep surfaces and pcurves.
  */
  bool    m_bFlipUVForBRepSurfaces;

  /** \details
  Calculate normals for shell vertexes if they aren't set for vertexes of facets
  */
  bool    m_bCalculateNormals;

  OdObjImportOptions()
    : m_bFlipUVForBRepSurfaces(false)
    , m_bCalculateNormals(false)
  {};
};

/** \details
OBJ files importer into OdObjDb structures
*/
class OBJTOOLKIT_EXPORT OdObjImport : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjImport);

  virtual OdResult      importFile(const OdString &fileName, OdObjImportOptions *options = NULL) = 0;
  virtual OdResult      importFile(const OdStreamBufPtr &pStreamBuf, OdObjImportOptions *options = NULL) = 0;

  virtual OdObjDbPtr    getObjDb() = 0;
  virtual OdString      getDrawingName() = 0;

};
typedef OdSmartPtr<OdObjImport> OdObjImportPtr;


/** \details
OdObjDb structures exporter into OBJ files
*/
class OBJTOOLKIT_EXPORT OdObjExport : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdObjExport);

  virtual void       setObjDb(OdObjDbPtr pObjDb) = 0;
  virtual OdObjDbPtr getObjDb() = 0;


  /** \details
  Save OdObjDb into .obj file

  \remarks
  returns eNoDatabase if OBJ database is empty
  */
  virtual OdResult exportFile(const OdString &fileName, const OdString &applicationComment) = 0;
  virtual OdResult exportFile(const OdStreamBufPtr &pStreamBuf, const OdString &applicationComment) = 0;

};
typedef OdSmartPtr<OdObjExport> OdObjExportPtr;


/** \details
MTL files importer into OdMtlDb structures
*/
class OBJTOOLKIT_EXPORT OdMtlImport : public OdRxObject
{
public:
  
  ODRX_DECLARE_MEMBERS(OdMtlImport);

  virtual OdMtlDbPtr getMtlDb() = 0;

  virtual OdResult importFile(const OdString &fileName) = 0;
  virtual OdResult importFile(const OdStreamBufPtr &pStreamBuf) = 0;

};
typedef OdSmartPtr<OdMtlImport> OdMtlImportPtr;


/** \details
OdMtlDb structures exporter into MTL files
*/
class OBJTOOLKIT_EXPORT OdMtlExport : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdMtlExport);

  virtual void        setMtlDb(OdMtlDbPtr pMtlDb) = 0;
  virtual OdMtlDbPtr  getMtlDb() = 0;
  virtual OdResult    exportFile(const OdString &fileName, const OdString &applicationComment) = 0;
  virtual OdResult    exportFile(const OdStreamBufPtr &pStreamBuf, const OdString &applicationComment) = 0;

};
typedef OdSmartPtr<OdMtlExport> OdMtlExportPtr;



struct OdObjExportOptions
{
  /** \details
  This string will be placed inside obj/mtl header when exporting,
  here coud be date/time of saving, application name etc.
  */
  OdString    m_applicationComment;

  /** \details
  If true, all texture files will be copied in
  resulting obj file directory, otherwise, mtl file will have full paths
  to texture files. True by default.
  */
  bool        m_keepTexturesNearObj;

  /** \details
  Index of view to export in obj file.
  */
  int    m_nView;

  /** \details
  Try to export GiDrawable as BRep, if convenient object
  */
  //bool        m_exportBrep;

  /** \details
  Fill/append data into defined m_pObjDb. If m_pObjDb is null, it will be created.
  */
  bool        m_bFillDb;

  /** \details
  OdObjDb to fill if m_bFillDb is true.
  */
  OdObjDbPtr  m_pObjDb;

  OdObjExportOptions()
    : m_keepTexturesNearObj(true)
    , m_nView(0)
    //, m_exportBrep(false)
    , m_bFillDb(false)
  {};
};

/** \details
This class declares the OBJToolkit module.
*/
class OBJTOOLKIT_EXPORT OdObjToolkitModule : public OdRxModule
{
public:

  virtual OdObjImportPtr  createObjImporter() = 0;
  virtual OdObjExportPtr  createObjExporter() = 0;

  // IAE: Users can manage MTL files and materials libraries separately from OBJ files
  virtual OdMtlImportPtr  createMtlImporter() = 0;
  virtual OdMtlExportPtr  createMtlExporter() = 0;

  /** \details
  Export defined object (OdGiDrawable) into obj.
  */
  virtual OdResult        exportObj(OdDbBaseDatabase *pDb, const OdGiDrawable *pEntity, const OdString &fileName, OdObjExportOptions *options = NULL) = 0;
  
  /** \details
  Export active view of pDb into obj (if found).

  \remarks
  Pass fileName empty together with options.m_bFillDb = true to collect entities into options.m_pObjDb
  */
  virtual OdResult        exportObj(OdDbBaseDatabase *pDb, const OdString &fileName, OdObjExportOptions *options = NULL) = 0;

  /** \details
  Check texture files appearance, try to update incorrect paths and copy texture files into exporting directory
  if m_keepTexturesNearObj of OdObjExportOptions is true.
  */
  virtual void            updateTexturesPaths(OdMtlDbPtr pMtlDb, OdDbBaseDatabase *pDb, const OdString & sPath, OdObjExportOptions *options = NULL) = 0;

};
typedef OdSmartPtr<OdObjToolkitModule> OdObjToolkitModulePtr;



inline OdObjToolkitModulePtr loadObjToolkitModule()
{
  OdObjToolkitModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdObjToolkitModuleName);
  return pModule;
}

inline OdObjImportPtr createObjImporter()
{
  OdSmartPtr<OdObjToolkitModule> pModule = ::odrxDynamicLinker()->loadApp(OdObjToolkitModuleName);
  if (!pModule.isNull())
    return pModule->createObjImporter();
  return OdObjImportPtr();
}

inline OdObjExportPtr createObjExporter()
{
  OdSmartPtr<OdObjToolkitModule> pModule = ::odrxDynamicLinker()->loadApp(OdObjToolkitModuleName);
  if (!pModule.isNull())
    return pModule->createObjExporter();
  return OdObjExportPtr();
}

inline OdMtlImportPtr createMtlImporter()
{
  OdSmartPtr<OdObjToolkitModule> pModule = ::odrxDynamicLinker()->loadApp(OdObjToolkitModuleName);
  if (!pModule.isNull())
    return pModule->createMtlImporter();
  return OdMtlImportPtr();
}

class OBJTOOLKIT_EXPORT OdObjEntity
{
public:

  OdObjEntity();
  virtual ~OdObjEntity();

  virtual const eOBJEntType type() const = 0;

};

class OBJTOOLKIT_EXPORT OdObjGeomEntity : public OdObjEntity
{
public:

  OdUInt32Array m_vind;

};

class OBJTOOLKIT_EXPORT OdObjPoints : public OdObjGeomEntity
{
public:

  virtual const eOBJEntType type() const;

};

class OBJTOOLKIT_EXPORT OdObjCurve : public OdObjGeomEntity
{
public:

  eObjCurveType       m_cstype;
  bool                m_rational;
  OdUInt8             m_degree;
  bool                m_periodic;
  double              m_startU;
  double              m_endU;
  OdDoubleArray       m_knots;
  OdDoubleArray       m_weights;

  virtual const eOBJEntType type() const;

};

class OBJTOOLKIT_EXPORT OdObjTextureEntity : public OdObjGeomEntity
{
public:

  OdUInt32Array m_vtind;
  OdMtlPtr      m_material;

};

class OBJTOOLKIT_EXPORT OdObjPolyline : public OdObjTextureEntity
{
public:

  virtual const eOBJEntType type() const;

};

class OBJTOOLKIT_EXPORT OdObjNormalEntity : public OdObjTextureEntity
{
public:

  OdUInt32Array m_vnind;

};

class OBJTOOLKIT_EXPORT OdObjFace : public OdObjNormalEntity
{
public:

  virtual const eOBJEntType type() const;

};

class OdObjLoopCurve2d
{
public:
  double m_u0;
  double m_u1;
  int m_curve2d_index;

  OdObjLoopCurve2d();

  OdObjLoopCurve2d(OdDouble u0, OdDouble u1, OdUInt32 index);
  ~OdObjLoopCurve2d();
};

class OdObjLoop
{
public:
  ObjLoopType m_type;
  OdArray <OdObjLoopCurve2d*> m_loop;

  OdObjLoop();
  virtual ~OdObjLoop();
};

class OBJTOOLKIT_EXPORT OdObjSurface : public OdObjNormalEntity
{
public:

  eObjCurveType       m_cstype;
  bool                m_rational;
  OdUInt8             m_degreeU;
  OdUInt8             m_degreeV;
  OdDouble            m_startU;
  OdDouble            m_endU;
  OdDouble            m_startV;
  OdDouble            m_endV;
  OdDoubleArray       m_knotsU;
  OdDoubleArray       m_knotsV;
  OdDoubleArray       m_weights;
  OdArray <OdObjLoop*> m_loops;
  
  ~OdObjSurface();

  virtual const eOBJEntType type() const;

};

class OBJTOOLKIT_EXPORT OdObjCurve2D : public OdObjEntity
{
public:

  eObjCurveType       m_cstype;
  bool                m_rational;
  OdUInt8             m_degree;
  OdDoubleArray       m_knots;
  OdDoubleArray       m_weights;

  OdUInt32Array       m_vpind;

  virtual const eOBJEntType type() const;

};

} //namespace OBJToolkit

#include "TD_PackPop.h"

#endif // OBJTOOLKIT_H
