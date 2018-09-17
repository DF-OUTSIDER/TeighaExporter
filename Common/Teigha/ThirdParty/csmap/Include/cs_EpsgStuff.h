/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <math.h>
#include <stack>
#include <list>

// An enumeration of the tables in the EPSG Parameter Dataset.  All tables are
// enumerated even though some are not actually used.
enum EcsEpsgTable { epsgTblNone = 0,
					epsgTblAlias,
					epsgTblArea,
					epsgTblChange,
					epsgTblAxisName,
					epsgTblAxis,
					epsgTblReferenceSystem,
					epsgTblCoordinateSystem,
					epsgTblOperationMethod,
					epsgTblParameterUsage,
					epsgTblParameterValue,
					epsgTblParameter,
					epsgTblOperationPath,
					epsgTblCoordinateOperation,
					epsgTblDatum,
					epsgTblDeprecation,
					epsgTblEllipsoid,
					epsgTblNamingSystem,
					epsgTblPrimeMeridian,
					epsgTblSupercession,
					epsgTblUnitOfMeasure,
					epsgTblVersionHistory,
					epsgTblUnknown
				  };

// An enumeration of the fields within the EPSG Parameter Dataset.  To the
// extent possible, fields in the EPSG tables which are the same as fields
// in other tabl.es are given the same enumerator.  For example, the
// enumerator value epsgFldDatumCode is used to access the datuam code
// value in all three (or is it four?) tables in which this value appears.
enum EcsEpsgField { epsgFldNone = 0,
					epsgFldAction,
					epsgFldAliasCode,
					epsgFldAlias,
					epsgFldAreaCode,
					epsgFldAreaOfUse,
					epsgFldAreaOfUseCode,
					epsgFldAreaName,
					epsgFldAreaEastBoundLng,
					epsgFldAreaNorthBoundLat,
					epsgFldAreaSouthBoundLat,
					epsgFldAreaWestBoundLng,
					epsgFldBoundingPolygonFileName,
					epsgFldChangeId,
					epsgFldCmpdHorizCrsCode,
					epsgFldCmpdVertCrsCode,
					epsgFldCodeAreaOfUse,
					epsgFldCodeNamingSystem,
					epsgFldCodesAffected,
					epsgFldComment,
					epsgFldConcatOperationCode,
					epsgFldCoordAxisAbbreviation,
					epsgFldCoordAxisCode,
					epsgFldCoordAxisName,
					epsgFldCoordAxisNameCode,
					epsgFldCoordAxisOrientation,
					epsgFldCoordOpAccuracy,
					epsgFldCoordOpCode,
					epsgFldCoordOpMethodCode,
					epsgFldCoordOpMethodName,
					epsgFldCoordOpName,
					epsgFldCoordOpScope,
					epsgFldCoordOpType,
					epsgFldCoordOpVariant,
					epsgFldCoordRefSysCode,
					epsgFldCoordRefSysKind,
					epsgFldCoordRefSysName,
					epsgFldCoordSysCode,
					epsgFldCoordSysName,
					epsgFldCoordSysType,
					epsgFldCoordTfmVersion,
					epsgFldCrsScope,
					epsgFldDataSource,
					epsgFldDateClosed,
					epsgFldDatumCode,
					epsgFldDatumName,
					epsgFldDatumScope,
					epsgFldDatumType,
					epsgFldDeprecated,
					epsgFldDeprecationDate,
					epsgFldDeprecationId,
					epsgFldDeprecationReason,
					epsgFldDescription,
					epsgFldDimension,
					epsgFldEllipsoidCode,
					epsgFldEllipsoidName,
					epsgFldEllipsoidShape,
					epsgFldExample,
					epsgFldFactorB,
					epsgFldFactorC,
					epsgFldFormula,
					epsgFldGreenwichLongitude,
					epsgFldInformationSource,
					epsgFldInvFlattening,
					epsgFldIsoA2Code,
					epsgFldIsoA3Code,
					epsgFldIsoNumericCode,
					epsgFldLeftLongitude,
					epsgFldNamingSystemCode,
					epsgFldNamingSystemName,
					epsgFldNorthLatitude,
					epsgFldOrder,
					epsgFldObjectCode,
					epsgFldObjectType,
					epsgFldObjectTableName,
					epsgFldOpPathStep,
					epsgFldOriginDescription,
					epsgFldParameterCode,
					epsgFldParameterName,
					epsgFldParameterValue,
					epsgFldParamSignReversal,
					epsgFldParamValueFileRef,
					epsgFldPrimeMeridianCode,
					epsgFldPrimeMeridianName,
					epsgFldProjectionConvCode,
					epsgFldRealizationEpoch,
					epsgFldRemarks,
					epsgFldReplacedBy,
					epsgFldReportDate,
					epsgFldReporter,
					epsgFldRequest,
					epsgFldReverseOp,
					epsgFldRevisionDate,
					epsgFldRightLongitude,
					epsgFldSemiMajorAxis,
					epsgFldSemiMinorAxis,
					epsgFldShowCrs,
					epsgFldShowOperation,
					epsgFldSingleOperationCode,
					epsgFldSortOrder,
					epsgFldSourceCrsCode,
					epsgFldSourceGeogCrsCode,
					epsgFldSouthLatitude,
					epsgFldSupercededBy,
					epsgFldSupercedes,
					epsgFldSupersessionId,
					epsgFldSupersessionType,
					epsgFldSupersessionYear,
					epsgFldTablesAffected,
					epsgFldTargetCrsCode,
					epsgFldTargetUomCode,
					epsgFldUnitOfMeasName,
					epsgFldUnitOfMeasType,
					epsgFldUomCode,
					epsgFldUomCodeSourceCoordDiff,
					epsgFldUomCodeTargetCoordDiff,
					epsgFldVersionDate,
					epsgFldVersionHistoryCode,
					epsgFldVersionNumber,
					epsgFldVersionRemarks,
					epsgFldUnknown
				  };

// Note there is a difference between a CRS and  CSys
enum EcsCsysType { epsgCsysTypNone = 0,
			 	   epsgCsysTypAffine,
				   epsgCsysTypCartesian,
				   epsgCsysTypCylindrical,
				   epsgCsysTypEllipsoidal,
				   epsgCsysTypLinear,
				   epsgCsysTypPolar,
				   epsgCsysTypSpherical,
				   epsgCsysTypVertical,
				   epsgCsysTypUnknown
				 };

enum EcsCrsType { epsgCrsTypNone = 0,
				  epsgCrsTypCompund,
				  epsgCrsTypEngineering,
				  epsgCrsTypGeocentric,
				  epsgCrsTypGeographic2D,
				  epsgCrsTypGeographic3D,
				  epsgCrsTypProjected,
				  epsgCrsTypVertical,
				  epsgCrsTypUnknown
				};

enum EcsOpType { epsgOpTypNone = 0,
				 epsgOpTypConversion,
				 epsgOpTypTransformation,
				 epsgOpTypConcatenated,
				 epsgOpTypUnknown
			   };

enum EcsDtmType { epsgDtmTypNone = 0,
				  epsgDtmTypGeodetic,
				  epsgDtmTypVertical,
				  epsgDtmTypEngineering,
				  epsgDtmTypUnknown
			   };

enum EcsUomType { epsgUomTypNone = 0,
				  epsgUomTypLinear,
				  epsgUomTypAngular,
				  epsgUomTypScale,
				  epsgUomTypUnknown
			   };
			   
enum EcsOrientation { epsgOrntNone = 0,
					  epsgOrntEast,
					  epsgOrntWest,
					  epsgOrntNorth,
					  epsgOrntSouth,
					  epsgOrntUp,
					  epsgOrntDown,
					  epsgOrntUnknown
					};

struct TcsEpsgTblMap
{
	EcsEpsgTable  TableId;
	short         FieldCount;
	EcsEpsgField  CodeKeyFieldId;
	wchar_t       TableName [64];
	EcsEpsgField  Sort1;
	EcsEpsgField  Sort2;
	EcsEpsgField  Sort3;
	EcsEpsgField  Sort4;
};
struct TcsEpsgFldMap
{
	EcsEpsgTable TableId;
	EcsEpsgField FieldId;
	short        FieldNbr;
	wchar_t      FieldName [64];
};

struct TcsEpsgCsysTypeMap
{
	EcsCsysType CsysType;
	wchar_t     CsysTypeName [64];
};

struct TcsEpsgCrsTypeMap
{
	EcsCrsType  CrsType;
	wchar_t     CrsTypeName [64];
};

struct TcsEpsgOpTypeMap
{
	EcsOpType  OpType;
	wchar_t    OpTypeName [64];
};

struct TcsEpsgDtmTypeMap
{
	EcsDtmType  DtmType;
	wchar_t     DtmTypeName [64];
};

struct TcsEpsgUomTypeMap
{
	EcsUomType  UomType;
	wchar_t     UomTypeName [64];
};

struct TcsEpsgOrntTypeMap
{
	EcsOrientation  OrntType;
	wchar_t         OrntTypeName [64];
};

// The following functions simply provide access to the tables defined above.
// They are independent of the TcsEPsgDatSetV6 object.  However, it is unlikely
// that the results produced by these functions have much value outside the
// environment of a TcsEpsgDataSetV6 object.  So, they could. and some day
// probably will be, moved to reside within the namespace of the
// TcsEPsgDataSetV6 object.
const wchar_t* GetEpsgTableName (EcsEpsgTable tblId);
EcsEpsgTable GetEpsgTableId (const wchar_t* tableName);
EcsEpsgField GetEpsgCodeFieldId (EcsEpsgTable tableId);
short GetEpsgCodeFieldNbr (EcsEpsgTable tableId);
short GetEpsgFieldNumber (EcsEpsgTable tableId,EcsEpsgField fieldId);

EcsCsysType GetEpsgCsysType (const wchar_t* csysTypeName);
EcsCrsType GetEpsgCrsType (const wchar_t* crsTypeName);
EcsOpType GetEpsgOpType (const wchar_t* opTypeName);
EcsDtmType GetEpsgDtmType (const wchar_t* dtmTypeName);
EcsUomType GetEpsgUomType (const wchar_t* uomTypeName);
//newPage//
//============================================================================
// EPSG Code Variable
//
// This object is used to represent an EPSG code.  All code specific to EPSG
// code values is handled within this object.
//
// Look to TcsEPsgSupport.cpp for the implementation of this object.
//
class TcsEpsgCode
{
	//=========================================================================
	// Static Constants, Variables, and Member Functions
	static const unsigned long InvalidValue;		// Zero, for now!
public:
	//=========================================================================
	// Construction  /  Destruction  /  Assignment
	TcsEpsgCode (void);
	TcsEpsgCode (unsigned long epsgCode);
	TcsEpsgCode (unsigned int epsgCode);
	TcsEpsgCode (const wchar_t* epsgCode);
	TcsEpsgCode (const std::wstring& epsgCode);
	TcsEpsgCode (const TcsEpsgCode& source);
	~TcsEpsgCode (void);
	TcsEpsgCode& operator= (const TcsEpsgCode& rhs);
	TcsEpsgCode& operator= (unsigned long epsgCode);
	//=========================================================================
	// Operator Overrides
	bool operator< (unsigned long epsgCode) const;
	bool operator< (const std::wstring& epsgCode) const;
	bool operator== (unsigned long epsgCode) const;
	bool operator== (unsigned int epsgCode) const;
	bool operator== (const std::wstring& epsgCode) const;
	bool operator> (unsigned long epsgCode) const;
	bool operator> (const std::wstring& epsgCode) const;
	operator unsigned long () const {return EpsgCode; };
	TcsEpsgCode operator++ (void);
	TcsEpsgCode operator++ (int dummy);
	TcsEpsgCode& operator+= (unsigned long rhs);
	TcsEpsgCode& operator-= (unsigned long rhs);
	TcsEpsgCode& operator+= (int rhs);
	TcsEpsgCode& operator-= (int rhs);
	TcsEpsgCode operator-- (void);
	TcsEpsgCode operator-- (int dummy);
	TcsEpsgCode operator+ (unsigned long rhs) const;
	TcsEpsgCode operator+ (int rhs) const;
	TcsEpsgCode operator- (unsigned long rhs) const;
	TcsEpsgCode operator- (int rhs) const;
	//=========================================================================
	// Public Named Functions
	bool IsValid (void) const {return EpsgCode != 0UL; }
	bool IsNotValid (void) const {return EpsgCode == 0UL; }
	std::wstring AsWstring (void) const;
	std::string AsString (void) const;
	bool AsString (wchar_t* result,size_t resultSize) const;
	bool AsString (char* result,size_t resultSize) const;
	void Invalidate (void) {EpsgCode = InvalidValue; };
protected:
	//=========================================================================
	// Protected Support Functions
	unsigned long StrToEpsgCode (const wchar_t* epsgCodeStr) const;
	unsigned long StrToEpsgCode (const char* epsgCodeStr) const;
private:
	//=========================================================================
	// Private Support Functions
	//=========================================================================
	// Private Data Members
	unsigned long EpsgCode;
};
//newPage//
//newPage//
//============================================================================
// EPSG Table Specialization
//
// An object which encapulates all CSV file functionality, adding a few special
// features particular to EPSG tables:
//
// 1> Maintains a binary unsigned long index on the first field of each table.
//    The default Index feature of the TcsCsvFileBase object is std::wstring
//    based and not as useful as the unsigned long based index used in this
//    object.
// 2> Provides for setting a current record, and accessing fields in that record.
//    Use of this feature is discouraged, as a multi-thread safe API is now
//    available.
// 3> Provides getting field data in the specific forms useful for dealing
//    with EPSG type data.
//
// Currently, there are several tables which are sorted by the EPSG code
// value.  In this case, generating an Index for the table is superfluous,
// a simple binary search (i.e. lower_bound) would work just as well.
// Eliminating this redundancy remains a "TODO" item.
//
class TcsEpsgTable : public TcsCsvFileBase
{
public:
	//=========================================================================
	// Static Constants, Variables, and Member Functions
	static const wchar_t LogicalTrue  [6];
	static const wchar_t LogicalFalse [6];
	static bool IsLogicalTrue (const wchar_t* logicalValue);
	static bool IsLogicalFalse (const wchar_t* logicalValue);
	//=========================================================================
	// Construction  /  Destruction  /  Assignment
	TcsEpsgTable (const TcsEpsgTblMap& tblMap,const wchar_t* databaseFldr);
	TcsEpsgTable (const TcsEpsgTable& source);
	virtual ~TcsEpsgTable (void);
	TcsEpsgTable& operator= (const TcsEpsgTable& rhs);
	//=========================================================================
	// Operator Overrides
	//=========================================================================
	// Public Named Functions
	bool IsOk (void) const {return Ok; };
	EcsEpsgTable GetTableId (void) const {return TableId; };
	const TcsCsvStatus& GetStatus (void) const {return CsvStatus; };
	unsigned LocateRecordByEpsgCode (const TcsEpsgCode& epsgCode) const;
	bool EpsgLocateCode (TcsEpsgCode& epsgCode,EcsEpsgField fieldId,const wchar_t* fldValue) const;

	// The following functions locate the record which meets the criteria
	// provided, and return its record number.  In this case, the first data
	// record is record number zero as there are no label records in this
	// environment.  (There may be labels, but they are not considered a
	// record.)
	unsigned EpsgLocateFirst (EcsEpsgField fieldId,const wchar_t* fldValue,bool honorCase = false) const;
	unsigned EpsgLocateFirst (EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const;
	unsigned EpsgLocateNext (unsigned startAfter,EcsEpsgField fieldId,const wchar_t* fldValue,
																	  bool honorCase = false) const;
	unsigned EpsgLocateNext (unsigned startAfter,EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const;

	// The following apply to the record identified by the epsgCode parameter,
	// or the recordd number parameter, as the case may be; they do not affect
	// the current record setting.  More importantly, they will work on a
	// "const" object and are multi-thread safe.
	bool IsDeprecated (const TcsEpsgCode& epsgCode) const;
	bool IsDeprecated (unsigned recNbr) const;
	bool GetField (std::wstring& result,const TcsEpsgCode& epsgCode,short fieldNbr) const;
	bool GetAsLong (long& result,const TcsEpsgCode& epsgCode,short fieldNbr) const;
	bool GetAsULong (unsigned long& result,const TcsEpsgCode& epsgCode,short fieldNbr) const;
	bool GetAsEpsgCode (TcsEpsgCode& result,const TcsEpsgCode& epsgCode,short fieldNbr) const;
	bool GetAsReal (double& result,const TcsEpsgCode& epsgCode,short fieldNbr) const;

	bool GetField (std::wstring& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const;
	bool GetAsLong (long& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const;
	bool GetAsULong (unsigned long& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const;
	bool GetAsEpsgCode (TcsEpsgCode& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const;
	bool GetAsReal (double& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const;

	// The following apply to the record identified by the recordNbr parameter,
	// they do not affect the current record setting.
	bool GetField (std::wstring& result,unsigned int recNbr,EcsEpsgField fieldId) const;
	bool GetAsLong (long& result,unsigned recNbr,EcsEpsgField fieldId) const;
	bool GetAsULong (unsigned long& result,unsigned recNbr,EcsEpsgField fieldId) const;
	bool GetAsEpsgCode (TcsEpsgCode& result,unsigned recNbr,EcsEpsgField fieldId) const;
	bool GetAsReal (double& result,unsigned recNbr,EcsEpsgField fieldId) const;

	// The following provides access to the derived TcsCsvFileBase::GetField
	// function without the caller having to have knowledge of the
	// TcsCsvStatus object.
	bool GetField (std::wstring& result,unsigned recNbr,short fieldNbr) const;

	// This structure is used to provide the csvStatus parameter required by
	// the TcsCsvFileBase base object.  The contents are highly dependent
	// on the context and last operation.  Probably should be a protected
	// function.
	TcsCsvStatus& GetCsvStatus (void);
	const TcsCsvStatus& GetCsvStatus (void) const;
private:
	//=========================================================================
	// Private Support Functions
	bool PrepareCsvFile (void);
	bool BuildEpsgIndex (short fldNbr,TcsCsvStatus& csvStatus);
	//=========================================================================
	// Private Data Members
	bool Ok;								// true = construction succeeded
	bool Sorted;							// Sorted in designated order
	bool Indexed;							// true = indexed by EPSG code
	EcsEpsgTable TableId;					// The ID of this table.
	EcsEpsgField CodeKeyField;				// ID of EPSG unique identifying code
	TcsCsvSortFunctor SortFunctor;			// Specifies sort order for this table
	std::map<TcsEpsgCode,unsigned> CodeIndex;
	TcsCsvStatus CsvStatus;					// Status of CSV operations
};
//newPage//
//=============================================================================
// TcsEpsgDataSetV6  -  An EPSG dataset based on the version 6 model.
//
// An image of the dataset in .csv form is expected to reside in the directory
// provided to the constructor.  This object is intended to be a read only
// object but nothing specific was done to preclude changing the underlying
// tables or writing changes back to the .csv files.  There are just no member
// functions at this time to support such operation.
//
// EPSG is up to version 8 now (September 2013), but the format has not changed
// significantly since Version 6; so we'll stick with this name until a major
// change occurs.
//
class TcsEpsgDataSetV6
{
public:
	//=========================================================================
	// Static Constants, Variables, and Member Functions
    static short GetFldNbr (EcsEpsgTable tableId,EcsEpsgField fieldId);
    static short GetFldName (std::wstring& fieldName,EcsEpsgTable tableId,EcsEpsgField fieldId);
    //=========================================================================
	// Construction  /  Destruction  /  Assignment
	TcsEpsgDataSetV6 (const wchar_t* databaseFolder,const wchar_t* revLevel = 0);
	TcsEpsgDataSetV6 (const TcsEpsgDataSetV6& source);
	virtual ~TcsEpsgDataSetV6 (void);
	TcsEpsgDataSetV6& operator= (const TcsEpsgDataSetV6& rhs);
	//=========================================================================
	// Operator Overrides
	//=========================================================================
	// Public Named Functions
	// Implementation of these functions can be found in csEpsgStuff.cpp
	//=========================================================================
	//		Basic Support Functions
	bool IsOk (void);
	bool IsOk (void) const;
	TcsEpsgTable* GetTablePtr (EcsEpsgTable tableId);
	const TcsEpsgTable* GetTablePtr (EcsEpsgTable tableId) const;
	unsigned GetRecordCount (EcsEpsgTable tableId) const;
	bool ConvertUnits (double& result,TcsEpsgCode trgUomCode,double value,
															 TcsEpsgCode srcUomCode) const;
	const wchar_t* GetRevisionLevel (void) const;
	std::wstring GetFailMessage (void) const;
	//=========================================================================
	// Some general access functions:
	// Step through an EPSG table one record at a time.
	bool GetFieldByIndex (std::wstring& fieldData,EcsEpsgTable tableId,EcsEpsgField fieldId,
																	   unsigned recNbr) const;
	bool GetCodeByIndex (TcsEpsgCode& epsgCode,EcsEpsgTable tableId,EcsEpsgField fieldId,
																	unsigned recNbr) const;
	// Get a field from a table which is indexed by an EPSG code.
	bool GetFieldByCode (std::wstring& fieldData,EcsEpsgTable tableId,EcsEpsgField fieldId,
																	  const TcsEpsgCode& epsgCode) const;
	bool GetFieldByCode (TcsEpsgCode& result,EcsEpsgTable tableId,EcsEpsgField fieldId,
																  const TcsEpsgCode& epsgCode) const;
	bool GetFieldByCode (double& result,EcsEpsgTable tableId,EcsEpsgField fieldId,
															 const TcsEpsgCode& epsgCode) const;
	// Is a record in table indexed by an EPSG code deprecated?
	bool IsDeprecated (EcsEpsgTable tableId,const TcsEpsgCode& epsgCode) const;

	//=========================================================================
	// These functions will work on a constant object and, therefore, are
	// multi-thread safe.
	bool LocateGeographicBase (TcsEpsgCode& geographicBase,EcsCrsType crsType,
														   const TcsEpsgCode& datumCode) const;
	TcsEpsgCode LocateOperation (EcsOpType crsType,const TcsEpsgCode& sourceCode,
												   const TcsEpsgCode& targetCode,
												   long variant = 1L) const;
	bool GetParameterFileName (std::wstring& parameterFileName,const TcsEpsgCode& opCode,
					 										   const TcsEpsgCode& opMethCode,
															   const TcsEpsgCode& prmCode) const;
	bool GetParameterValue (double& parameterValue,const TcsEpsgCode& opCode,
												   const TcsEpsgCode& opMethCode,
												   const TcsEpsgCode& prmCode,
												   const TcsEpsgCode& trgUomCode) const;
	bool GetCsMapEllipsoid (struct cs_Eldef_& ellipsoid,const TcsEpsgCode& epsgCode) const;
	bool GetCsMapDatum (struct cs_Dtdef_& datum,struct cs_Eldef_& ellipsoid,const TcsEpsgCode& epsgDtmCode,
																			unsigned variant = 0U) const;
	bool GetCsMapCoordsys (struct cs_Csdef_& coordsys,struct cs_Dtdef_& datum,struct cs_Eldef_& ellipsoid,
																			  const TcsEpsgCode& crsEpsgCode) const;
	short DetermineCsMapDatumMethod (const TcsEpsgCode& operationCode,bool& coordFrame) const;
	bool GetCoordsysQuad (short& quad,TcsEpsgCode& horzUom,TcsEpsgCode& vertUom,
														   const TcsEpsgCode& crsEpsgCode) const;
	EcsCrsType GetCrsType (const TcsEpsgCode& crsCode) const;
	EcsUomType GetUomFactor (double& uomFactor,const TcsEpsgCode& uomCode) const;
protected:
	//=========================================================================
	// Protected Support Functions
	//
	// These functions provide support for the public member functions of this
	// object.  Implementation of these members can be found in
	// csEpsgSupport.cpp  Selecting these members to be protected is quite
	// /arbitrary.
	bool GetUomToDegrees (double& toDegrees,const TcsEpsgCode& uomCode) const;
	bool GetUomToMeters (double& toMeters,const TcsEpsgCode& uomCode) const;
	bool GetUomToUnity (double& toUnity,const TcsEpsgCode& uomCode) const;
	bool ConvertUnits (double& value,const TcsEpsgCode& trgUomCode,const TcsEpsgCode& srcUomCode) const;
	bool FieldToReal (double& result,const TcsEpsgCode& trgUomCode,const wchar_t* fldData,const TcsEpsgCode& srcUomCode) const;
	bool FieldToDegrees (double& result,const wchar_t* field,const TcsEpsgCode& uomCode) const;
	bool GetReferenceDatum (TcsEpsgCode& dtmEpsgCode,const TcsEpsgCode& crsEpsgCode) const;
	bool GetPrimeMeridian (double& primeMeridian,const TcsEpsgCode& crsEpsgCode) const;
	bool IsPrMerRotation (bool& isPrMerRot,const TcsEpsgCode& oprtnCode) const;
	unsigned LocateParameterValue (const TcsEpsgCode& opCode,const TcsEpsgCode& opMethCode,
															 const TcsEpsgCode& prmCode) const;
	bool AddDatumParameterValues (struct cs_Dtdef_& datum,const TcsEpsgCode& operationCode) const;
	bool DetermineRevisionLevel (void);
private:
	//=========================================================================
	// Private Support Functions
	// These are private as they are called by GetCsMapCoordsys only after all
	// the stuff common to both types is done.  Calling by any other function
	// is likely to cause some real problems.
	bool GeographicCoordsys (struct cs_Csdef_& coordsys,const TcsEpsgCode& crsEpsgCode,
														const TcsEpsgCode& horzUomCode) const;
	bool ProjectedCoordsys (struct cs_Csdef_& coordsys,const TcsEpsgCode& crsEpsgCode,
													   const TcsEpsgCode& horzUomCode) const;
	//=========================================================================
	// Private Data Members
	bool Ok;
	std::wstring RevisionLevel;				// The revision level of the EPSG data in this object
	std::wstring DatabaseFolder;			// The folder in which the .CSV files reside.
	std::wstring FailMessage;
	std::map<EcsEpsgTable,TcsEpsgTable*> EpsgTables;
											// The individual tables of the EPSG dataset.
};
//newPage//
//=============================================================================
// The followoing objects were developed in response to the issue of datums and
// geodetic transformations within EPSG.  Model the EPSG structure in a form
// which is somewhat rational and can be understood, used, and maintained by
// us mere mortals.  Implementation of these objects can be found in
// cs_EpsgSupport.cpp.
//
// Within EPSG, a conversion or transformation (usually transformation) may
// have serveral applicable variations.  Some more precise than others, but
// none the less, all have a certain degree of validity.  Especially with
// respect to datum transformations, there may be several variations which
// will get you from one datum to another.
//
// To complicate matters further, a variant maybe a concatenated operation.
// That is, consist of two or more single operations which must be performed
// in a specific order.  Generally, concatenated operations are defined in the
// Coordinate_Operation Path table.  Often, one or more of these concatenated
// operations are: 1> the null transformation, or 2> a longitude translation
// (i.e. application of a prime meridian).  In either of these two cases,
// they can usually be ignored in the CS-MAP environment.
//
// TcsOpSingle is a definition of a single operation. TcsSingleOp represents
//		an entry in the operation table which is NOT a concatenated entry.
// TcsOpVariant is a std::list of TcsOpSingle objects, often only one.
//		TcsOpVariant will contain more than one TcsOpSingle in the case
//		of a variant which is a concatenated operation.  Each phase of the
//		concatenated operation is represented by its own TcsOpSingle.
// TcsOpsVariants is a std::vector of TcsVariant objects, which is usually
//		sorted by the Accuracy member of the TcsOpVariant object.
//
struct TcsSingleOp
{
	//=========================================================================
	// Static Constants, Variables, and Member Functions
	//=========================================================================
	// Construction  /  Destruction  /  Assignment
public:
	TcsSingleOp (const	TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& operationCode);
	TcsSingleOp (const TcsSingleOp& source);
	~TcsSingleOp (void);
	TcsSingleOp& operator= (const TcsSingleOp& rhs);
	//=========================================================================
	// Operator Overrides
	//=========================================================================
	// Public Named Member Functions
	bool IsValid (void) const {return OperationCode.IsValid (); }
	bool IsNull (void) const {return Null; }
	bool IsLngXlation (void) const {return LngXlation; }
	EcsOpType GetType (void) const {return Type; }
	TcsEpsgCode GetSourceCRS (void) const {return SourceCRS; }
	TcsEpsgCode GetTargetCRS (void) const {return TargetCRS; }
	TcsEpsgCode GetOperationCode (void) const {return OperationCode; }
	TcsEpsgCode GetMethodCode (void) const {return OperationMethod; }
	double GetAccuracy (void) const {return Accuracy; }
	std::wstring GetOpName (void) const {return OpName; }
	const wchar_t* GetOpNamePtr (void) const {return OpName.c_str (); }
	//=========================================================================
	// Private Support Member Functions
	bool Classify (const TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& oprtnCode);
	//=========================================================================
	// Private Data Memebers
	bool Null;					// true says Null transformation
	bool LngXlation;			// true says Lonigtude Translation
	EcsOpType Type;				// Conversion or Transformation.
	TcsEpsgCode SourceCRS;		// Somewhat redundant, but convenient
	TcsEpsgCode TargetCRS;		// Somewhat redundant, but convenient
	TcsEpsgCode OperationCode;	// EPSG Operation code of the non-concatenated operation
	TcsEpsgCode OperationMethod;// EPSG Operation Method code for this operation
	double Accuracy;			// Accuracy of this operation per EPSG database
	std::wstring OpName;		// Name of the operation, usually for report generation
};
// TcsOpVariant represents an entry in the operation table which MAY BE
// concatenated.  Typically, it is not concatenated, and the Singles container
// will only carry a single element.  As CsMap does not support concatenated
// operations, it is at this level that an equivalent Datum Definition can
// be generated.  We use a list as the std::list object does not require a
// default constructor, and all such objects should be valid representations
// of a TcsSingleOp.
class TcsOpVariant
{
	//=========================================================================
	// Static Constants, Variables, and Member Functions
public:
	//=========================================================================
	// Construction  /  Destruction  /  Assignment
	TcsOpVariant (const TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& operationCode);
	TcsOpVariant (const TcsOpVariant& source);
	~TcsOpVariant (void);
	TcsOpVariant& operator= (const TcsOpVariant& rhs);
	//=========================================================================
	// Public Named Member Functions
	bool IsValid (void) const;
	unsigned GetOperationCount (void) const;
	EcsOpType GetType (void) const {return Type; }
	unsigned GetVariantNbr (void) const {return VariantNbr; }
	double GetAccuracy (void) const;
	const wchar_t* GetVariantNamePtr (void) const {return VariantName.c_str (); }
	TcsEpsgCode GetOpCodeForCsMap (void) const;
private:
	EcsOpType Type;						// May be Concatenated.
	TcsEpsgCode EpsgOpCode;				// OpCode of the primary operation.
	unsigned VariantNbr;				// EPSG variant number.  There is
										// no requirement about how variants
										// are numbered, only that they should be
										// (but aren't always) unique.
	std::wstring Version;				// Verision from EPSG operation table
	double Accuracy;					// Accuracy from the EPSG Operation table.
	std::wstring VariantName;			// Operation name, use in report generation.
	std::list<TcsSingleOp> Singles;		// container of the individual components of
										// a concatenated operation.   Order is the
										// order in which the operations are to be
										// performed, per EPSG Path table.
};
// TcsOpVariants is a collection of TcsOpVariant's.  Quite often, there is
// only one TcsOpVariant; but the exceptional cases need to be handled.
class TcsOpVariants
{
	//=========================================================================
	// Static Constants, Variables, and Member Functions
public:
	//=========================================================================
	// Construction  /  Destruction  /  Assignment
	TcsOpVariants (const TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& sourceCRS,
												  const TcsEpsgCode& targetCRS);
	TcsOpVariants (const TcsOpVariants& source);
	~TcsOpVariants (void);
	TcsOpVariants& operator= (const TcsOpVariants& rhs);
	//=========================================================================
	// Public Named Member Functions
	unsigned GetVariantCount (void) const {return static_cast<unsigned>(Variants.size ()); }
	TcsEpsgCode GetSourceCRS (void) const {return SourceCrs; }
	TcsEpsgCode GetTargetCRS (void) const {return TargetCrs; }
	bool GetVariant (TcsOpVariant& variant,unsigned index) const;
	const TcsOpVariant* GetVariantPtr (unsigned index) const;
	TcsEpsgCode GetOprtnCodeByVariant (unsigned variant) const;
	TcsEpsgCode GetBestOprtnCode (const TcsEpsgDataSetV6& epsgDB) const;
private:
	TcsEpsgCode SourceCrs;				// EPSG Code of the source CRS
	TcsEpsgCode TargetCrs;				// EPSG code of the target CRS
	std::vector<TcsOpVariant> Variants;	// Collection of variants.  Usually
										// only one, but there can be as many
										// as 100.
};
