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
#ifndef __DG_MODEL_H__
#define __DG_MODEL_H__

#include "TD_PackPush.h"
#include "TGVersion.h"
#include "DgExport.h"
#include "DgMacros.h"
#include "DgElement.h"
#include "DgElementIterator.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector3d.h"
#include "DgGraphicsElement.h"
#include "DgCommonStructs.h"
#include "Ge/GeExtents3d.h"



/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgModel : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgModel);
public:

  // The Model Types
  enum Type
  {
    kDesignModel     = 0,
    kSheetModel      = 1,
    kExtractionModel = 2
  };

  // The types of the Auxiliary Coordinate System
  enum AcsType
  {
    kRectangular  = 0,
    kCylindrical  = 1,
    kSpherical    = 2
  };

  // Types of available unit measures for the model
  enum WorkingUnit
  {
    kWuUnitOfResolution = 0,  //the smallest units that are used to keep data in files
    kWuStorageUnit      = 1,  //intermediate measure units
    kWuWorldUnit        = 2,  //meters
    kWuMasterUnit       = 3,
    kWuSubUnit          = 4,
  };

  // Bases for unit measures
  enum UnitBase
  {
    kNone   = 0,
    kMeter  = 1
  };

  // Systems for unit measures (if the base is kMeter)
  enum UnitSystem
  {
    kCustom   = 0,
    kEnglish  = 1,
    kMetric   = 2
  };

  // Description for the storage unit
  struct StorageUnitDescription
  {
    UnitBase m_base;
    UnitSystem m_system;

    //numerator/denominator determine how big is 1 storage unit relatively to meters
    double m_numerator;
    double m_denominator;

    //this field determine the size of 1 storage units (based on UORs)
    double m_uorPerStorageUnit;
  };

  // Description for the master/sub units
  struct UnitDescription
  {
    UnitBase m_base;
    UnitSystem m_system;

    //scale factor relative to the storage unit measure
    double m_numerator;
    double m_denominator;

    //name
    OdString m_name;
  };

  // Known unit measures to simplify the initialization of units for the model
  enum UnitMeasure
  {
    kUndefinedMeasure,

    kMegameters,
    kKilometers,
    kMeters,
    kDecimeters,
    kCentimeters,
    kMillimeters,
    kMicrometers,
    kNanometers,
    kPicometers,

    kMiles,
    kYards,
    kFeet,
    kInches,
    kMils,
    kUSSurveyFeet,
    kMicroinches
  };

  

  // In case this model is placed as a cell:
  enum CellType
  {
    kNormalGraphics = 0,
    kMenu           = 3,
    kPoint          = 7
  };

  enum OdDgGridOrientationType
  {
    kView   = 0,
    kTop    = 1,
    kRight  = 2,
    kFront  = 3,
    kACS    = 4
  };

  /** \details
    Adds the specified element to this Model object.

    \param pElement [in]  Pointer to the element to add.

    \returns
    Returns the Element ID of the newly added element.
  */
  OdDgElementId addElement(OdDgElementPtr pElement);

  /** \details
    Returns an Iterator object that can be used to traverse this Model object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted elements are skipped.
  */
  OdDgElementIteratorPtr createGraphicsElementsIterator(
    bool atBeginning = true,
    bool skipDeleted = true ) const;
  OdDgElementIteratorPtr createControlElementsIterator(
    bool atBeginning = true,
    bool skipDeleted = true ) const;

  OdUInt32 getEntryId() const;

  // OdGiDrawable overridden

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;
  OdResult subGetGeomExtents( OdGeExtents3d& extent ) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;

  // TODO: Temporary methods to support vectorization.
  void fitToView();

  //
  // Accessors / transformers
  //

  // The Model type
  Type getType() const;
  void setType(Type val);

  // Name of the model
  OdString getName() const;
  void setName(const OdString& name );

  // Description of the model
  OdString getDescription() const;
  void setDescription(const OdString& name );

  // The type and other properties of the active Auxiliary Coordinate System
  AcsType getAcsType() const;
  void setAcsType(AcsType val);

  OdGePoint3d getAcsOrigin() const;
  void setAcsOrigin( OdGePoint3d const& origin );

  OdGeMatrix3d getAcsRotation() const;
  void setAcsRotation( OdGeMatrix3d const& rotation );

  OdDgElementId getAcsElementId() const;
  void setAcsElementId( OdDgElementId const& id );

  // The global origin defines the start position (zero point) of the world coordinate system inside of the model space
   OdGePoint3d getGlobalOrigin() const;
  void setGlobalOrigin(const OdGePoint3d& origin ); 

  // The model-specific settings
  bool getUnitLockFlag() const;
  void setUnitLockFlag(bool val);
  bool getIsometricLockFlag() const;
  void setIsometricLockFlag(bool val);
  OdUInt16 getIsometricPlane() const;
  void setIsometricPlane(OdUInt16 val);

  // Grid settings

  bool getGridLockFlag() const;
  void setGridLockFlag(bool val);

  bool getIsometricGridFlag() const;
  void setIsometricGridFlag(bool val);

  double   getGridMaster() const;
  void     setGridMaster( double dGridMaster );

  OdUInt32 getGridReference() const;
  void     setGridGridReference( OdUInt32 uGridReference );

  double   getGridRatio() const;
  void     setGridRatio( double dGridRatio );

  double   getGridAngle() const;
  void     setGridAngel( double dGridAngle );

  OdGePoint2d getGridBase() const;
  void     setGridBase( const OdGePoint2d& ptGridBase );

  OdDgGridOrientationType getGridOrientation() const;
  void     setGridOrientation( OdDgGridOrientationType uOrientation );

  // The number of grid points per reference grid cross
  //virtual OdUInt32 getRefGrid() const;
  //virtual void setRefGrid(OdUInt32 val);

  // The model-specific properties
  bool getModelIs3dFlag() const;
  void setModelIs3dFlag(bool val);
  bool getHiddenFlag() const;
  void setHiddenFlag(bool val);
  bool getLockedFlag() const;
  void setLockedFlag(bool val);
  bool getNotInCellListFlag() const;
  void setNotInCellListFlag(bool val);
  bool getUseBackgroundColorFlag() const;
  void setUseBackgroundColorFlag(bool val);

  double getLastSaveTime() const;
  void setLastSaveTime(double val);

  OdUInt16 getProperties() const;       // Only in case a model is placed as a cell
  void setProperties(OdUInt16 val);
  OdInt32 getStyle() const;             // Only in case a model is placed as a cell
  void setStyle(OdInt32 val);
  OdUInt32 getWeight() const;           // Only in case a model is placed as a cell
  void setWeight(OdUInt32 val);
  OdUInt32 getColorIndex() const;
  void setColorIndex(OdUInt32 val);

  CellType getCellType() const;                 // Only in case a model is placed as a cell
  void setCellType(CellType val);

  // Only when a model is placed as a cell:
  //  the method returns/changes the element class property.
  OdDgGraphicsElement::Class getClass() const;
  void setClass( OdDgGraphicsElement::Class newClass );

  double getSolidExtent() const;
  void setSolidExtent(double  val);

  OdGePoint3d getInsertionBase() const;       // Only in case a model is placed as a cell
  void setInsertionBase( const OdGePoint3d& base ); 

  //double getRoundOffUnit() const;
  //void setRoundOffUnit(double  val);
  //double getRoundOffRatio() const;
  //void setRoundOffRatio(double  val);

  double getLinestyleScale() const;
  void setLinestyleScale(double  val);

  ODCOLORREF getBackground() const;
  void setBackground(ODCOLORREF val);

  //
  // Manipulation with unit measures
  //
  // Note:
  // 1. If some measure is changed then all previous measurements with that measure can become obsolete.
  // 2. World units are assumed to be meters. Thus, master/sub units could have either correct settings 
  //    (e.g.: metric, 10 times smaller than storage units => millimeters) or incorrect settings
  //    (e.g.: english, 100 times smaller than storage units => English measurement does not have such size).
  //    It is recommended to use fillUnitDescriptor() method.
  //

  // The working unit measure used to represent all coordinates/distances for everything inside of the model
  WorkingUnit getWorkingUnit() const;
  void setWorkingUnit( WorkingUnit eUnit);

  // Storage unit
  void getStorageUnit( StorageUnitDescription& description ) const;
  void setStorageUnit( const StorageUnitDescription& description );

  // Master unit
  void getMasterUnit( UnitDescription& description ) const;
  void setMasterUnit( const UnitDescription& description );

  UnitMeasure getMasterUnit() const;
  void setMasterUnit( const UnitMeasure eUnitMeasure );
  
  // Sub unit
  void getSubUnit( UnitDescription& description ) const;
  void setSubUnit( const UnitDescription& description );

  UnitMeasure getSubUnit() const;
  void setSubUnit(const UnitMeasure eUnitMeasure);

  // This method simplifies the filling of UnitDescription and guarantees that the result is appropriate
  static void fillUnitDescriptor( UnitMeasure source, UnitDescription& result );
  static void fillUnitMeasure(UnitDescription & description, UnitMeasure & measure);

  // Returns the scale factor for conversion from the first measure unit to the second one
  double getMeasuresConversion( WorkingUnit from, WorkingUnit to ) const;

  //
  // Conversion between UORs and working units
  //

  double convertUORsToWorkingUnits( double uorsDistance ) const;
  double convertWorkingUnitsToUORs( double workingUnitsDistance ) const;

  OdGePoint2d convertUORsToWorkingUnits( OdGePoint2d const& uorsPoint ) const;
  OdGePoint2d convertWorkingUnitsToUORs( OdGePoint2d const& workingUnitsPoint ) const;

  OdGePoint3d convertUORsToWorkingUnits( OdGePoint3d const& uorsPoint ) const;
  OdGePoint3d convertWorkingUnitsToUORs( OdGePoint3d const& workingUnitsPoint ) const;

  OdGeVector2d convertUORsToWorkingUnits( OdGeVector2d const& uorsVector ) const;
  OdGeVector2d convertWorkingUnitsToUORs( OdGeVector2d const& workingUnitsVector ) const;

  OdGeVector3d convertUORsToWorkingUnits( OdGeVector3d const& uorsVector ) const;
  OdGeVector3d convertWorkingUnitsToUORs( OdGeVector3d const& workingUnitsVector ) const;

  /** \details
    Creates a new View Group for the model.
  */
  void createViewGroup();

   /** \details
    Reads the DGN format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \remarks
    This function calls dgnInFields(pFiler),
    then loads any data associated with this object.
   */
  void dgnIn(
    OdDgFiler* pFiler);
  
  /** \details
    Writes the DGN format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
    \remarks
    This function calls this object's dgnOutFields() function,
    writes loads any data associated with this object.
  */
  void dgnOut(
    OdDgFiler* pFiler) const;
};

typedef OdSmartPtr<OdDgModel> OdDgModelPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgSheetModel : public OdDgModel
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgSheetModel);

public:

  enum PaperType
  {
     kCustom     = 0
    ,kAnsiA      = 1
    ,kAnsiACut   = 2
    ,kAnsiB      = 3
    ,kAnsiBCut   = 4
    ,kAnsiC      = 5
    ,kAnsiCCut   = 6
    ,kAnsiD      = 7
    ,kAnsiDCut   = 8
    ,kAnsiE      = 9
    ,kAnsiECut   = 10
    ,kArchA      = 11
    ,kArchB      = 12
    ,kArchC      = 13
    ,kArchD      = 14
    ,kArchE      = 15
    ,kArchF      = 16
    ,kIsoA0      = 17
    ,kIsoA1      = 18
    ,kIsoA2      = 19
    ,kIsoA3      = 20
    ,kIsoA4      = 21
  };

public:

  OdResult subGetGeomExtents( OdGeExtents3d& extent ) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;

  virtual bool   getDrawBorderFlag() const;
  virtual void   setDrawBorderFlag( bool bSet );

  virtual bool   getDrawMarginsFlag() const;
  virtual void   setDrawMarginsFlag( bool bSet );

  virtual OdUInt32 getSheetNumber() const;
  virtual void     setSheetNumber( OdUInt32 uNumber );

  // Units of measurement for sheet width ( in master units )

  virtual double   getSheetWidth() const;
  virtual void     setSheetWidth( double dWidth );
  virtual double   getSheetHeight() const;
  virtual void     setSheetHeight( double dHeight );

  virtual OdGePoint2d   getSheetOffset() const;
  virtual void          setSheetOffset( const OdGePoint2d ptOffset );

  virtual double   getSheetRotation() const;
  virtual void     setSheetRotation( double dAngle );

  virtual double   getSheetLeftMargin() const;
  virtual void     setSheetLeftMargin( double dMargin );
  virtual double   getSheetRightMargin() const;
  virtual void     setSheetRightMargin( double dMargin );
  virtual double   getSheetTopMargin() const;
  virtual void     setSheetTopMargin( double dMargin );
  virtual double   getSheetBottomMargin() const;
  virtual void     setSheetBottomMargin( double dMargin );

  virtual double   getSheetAnnotationScale() const;
  virtual void     setSheetAnnotationScale( double dScale );

  virtual OdString getSheetPaperName() const;
  virtual void     setSheetPaperName( OdString strName );

  virtual void     setPaper( PaperType iPaperType );

  virtual OdDgModel::UnitDescription getSheetUnits() const;
  virtual void setSheetUnits( const OdDgModel::UnitDescription& uUnits );
};

typedef OdSmartPtr<OdDgSheetModel> OdDgSheetModelPtr;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgModelTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgModelTable);
public:
  /** \details
    Adds the specified model to this collection.

    \param pModel [in]  Pointer to the model to add.
  */
  void add(OdDgModelPtr pModel, bool bCreateDefaultViewGroup = true );

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator(
    bool atBeginning = true,
    bool skipDeleted = true ) const;

  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;
  virtual OdDgElementId getAt( OdUInt32 entryId, bool getErasedRecord = false) const;
  virtual bool has( const OdDgElementId& elementId) const;
};

typedef OdSmartPtr<OdDgModelTable> OdDgModelTablePtr;

/** \details
This class is the Protocol Extension class for OdDgModel objects.

\remarks
The Protocol Extension class can be registered on the OdDgModel class 
to provide a vectorization client the opportunity to customize the drawing
of sheet border during sheet model vectorization.

\sa
TD_Dg

<group OdDg_Classes> 
*/
class TG_EXPORT OdDgModelSheetPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgModelSheetPE);

  /** \details
  This function is called during vectorization to draw the paper.  

  \param pThis [in]  The model that is being vectorized.
  \param pWd [in/out] Draw object that can be used by this function to set the 
  necessary attributes and create the necessary geometry.
  \param points [in] Contains the 4 corners of the paper.

  \returns
  true if the paper was successfully drawn, otherwise false, in which case
  the layout will do a default rendering of the paper.
  */
  virtual bool drawPaper(const OdDgModel* pThis, OdGiWorldDraw* pWd, OdGePoint3d* points) = 0;

  /** \details
  This function is called during vectorization to draw the paper border.

  \param pThis [in]  The model that is being vectorized.
  \param pWd [in/out] Draw object that can be used by this function to set the 
  necessary attributes and create the necessary geometry.
  \param points [in] Contains the 4 corners of the paper.

  \returns
  true if the border was successfully drawn, otherwise false, in which case
  the layout will do a default rendering of the border.
  */
  virtual bool drawBorder(const OdDgModel* pThis, OdGiWorldDraw* pWd, OdGePoint3d* points) = 0;

  /** \details
  This function is called during vectorization to draw the paper margins.

  \param pThis [in]  The model that is being vectorized.
  \param pWd [in/out] Draw object that can be used by this function to set the 
  necessary attributes and create the necessary geometry.
  \param points [in] Contains the 4 margin points.

  \returns
  true if the margins were successfully drawn, otherwise false, in which case
  the layout will do a default rendering of the margins.
  */
  virtual bool drawMargins(const OdDgModel* pThis, OdGiWorldDraw* pWd, OdGePoint3d* points) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgModelSheetPE object pointers.
*/
typedef OdSmartPtr<OdDgModelSheetPE> OdDgModelSheetPEPtr;

#include "TD_PackPop.h"

#endif // __DG_MODEL_H__
