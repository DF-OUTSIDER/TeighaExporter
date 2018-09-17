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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// The following enumerator, structure, and table define the WKT primitive
// types and the name assigned to them.  The table is designed to be a one for
// one equivalent to the enumerator.  Maintain this relationship!
enum ErcWktEleType {	rcWktNone,
						rcWktAuthority,
						rcWktAxis,
						rcWktCompDCS,
						rcWktConcatMt,
						rcWktDatum,
						rcWktFittedCS,
						rcWktGeoCCS,
						rcWktGeogCS,
						rcWktGeogTran,
						rcWktInverseMt,
						rcWktLocalCS,
						rcWktMethod,
						rcWktParameter,
						rcWktParamMt,
						rcWktPassThroughMt,
						rcWktPrimeM,
						rcWktProjCS,
						rcWktProjection,
						rcWktSpheroid,
						rcWktToWgs84,
						rcWktUnit,
						rcWktVertDatum,
						rcWktVertCS,
						rcWktLocalDatum,		// found in Oracle stuff; is this standard(?) WKT???
						rcWktUnknown
				 };
struct TrcWktEleTypeMap
{
	char Name [16];
	ErcWktEleType Type;
	bool InitlName;
	
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// Similarly with the values for the AXIS primitive.  We have an enumerator and
// a mapping table.  Internally, we use the enumerator.  Turns out that this
// stuff is not really used.  But since it exists and seems to be correct, we
// leave it here for possible use in the future.
enum ErcWktAxisId {	rcWktAxisIdNone = 0,
					rcWktAxisIdLat,
					rcWktAxisIdLng,
					rcWktAxisIdX,
					rcWktAxisIdY,
					rcWktAxisIdZ,
					rcWktAxisIdUnknown
					};
enum ErcWktAxisValue {	rcWktAxisNone = 0,
						rcWktAxisNorth,
						rcWktAxisSouth,
						rcWktAxisEast,
						rcWktAxisWest,
						rcWktAxisUp,
						rcWktAxisDown,
						rcWktAxisOther,
						rcWktAxisUnknown
					};
struct TrcWktAxisValueMap
{
	char Name [16];
	ErcWktAxisValue Value;
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following enumerates the various parameters supported by WKT, and the
// various names used to identify these.  Nothing like a "standard" :>)
//
// All ESPG parameters are listed even though many are not really used by
// anything in the real world.  In these cases, the parameter "types" are
// mapped, via the cs_PRMCOD_?????? values to the usual use in reality.
//
// Since the original form of WKT used the PARAMETER element to convey the
// three/seven datum shift parameters, we include them in the parameter table.
//
// We considered making the enumeration tag values map to EPSG codes, but
// we decided to remain as independent of EPSG as possible.  They are not
// software developers and often make changes that us developers would frown
// upon.  Maybe the POSC folk will prevent that from happening in the future.
enum TrcWktParameter {	rcWktPrmNone = 0,
						// Some datum transformation parameters
						rcWktPrmXTranslation = 101,
						rcWktPrmYTranslation,
						rcWktPrmZTranslation,
						rcWktPrmXRotation,
						rcWktPrmYRotation,
						rcWktPrmZRotation,
						rcWktPrmDatumScale,
						// Projection parameters
						rcWktPrmLatitudeOfOrg = 201,
						rcWktPrmLongitudeOfOrg,
						rcWktPrmScaleAtOrigin,
						rcWktPrmFalseEasting,
						rcWktPrmFalseNorthing,
						rcWktPrmLatitudeOfCtr,
						rcWktPrmLongitudeOfCtr,
						rcWktPrmAzimuth,
						rcWktPrmRectifiedGrdAng,
						rcWktPrmScaleAtCenter,
						rcWktPrmEastingOfCtr,
						rcWktPrmNorthingOfCtr,
						rcWktPrmPsdoParallelLat,
						rcWktPrmPsdoParallelScl,
						rcWktPrmLatOfFalseOrg,
						rcWktPrmLngOfFalseOrg,
						rcWktPrmStdParallel1,
						rcWktPrmStdParallel2,
						rcWktPrmEastingFalseOrg,
						rcWktPrmNorthingFalseOrg,
						rcWktPrmSphericalOrgLat,
						rcWktPrmSphericalOrgLng,
						rcWktPrmSystemWestLimit,
						rcWktPrmSystemZoneWidth,
						rcWktPrmLatOfStdParall,
						rcWktPrmZone,
						rcWktPrmRegion,
						// The following WKT,  but not EPSG parameters
						rcWktPrmLatFirstPoint,
						rcWktPrmLngFirstPoint,
						rcWktPrmLatSecondPoint,
						rcWktPrmLngSecondPoint,
						// The following neither WKT oro EPSG
						rcWktPrmYaxisAzimuth,
						// Cartesian transformation parameters
						rcWktPrmAffineA0 = 501,
						rcWktPrmAffineB0,
						rcWktPrmAffineA1,
						rcWktPrmAffineB1,
						rcWktPrmAffineA2,
						rcWktPrmAffineB2,
						// The following is used to terminate tables, etc.
						rcWktPrmUnknown
					 };
struct TrcWktPrmNameMap
{
	TrcWktParameter PrmCode;
	unsigned short MsiParmCode;
	unsigned short EpsgNbr;
	char WktOgcName [48];
	char WktGeoTiffName [48];
	char WktEsriName [48];
	char WktOracleName [48];
	char WktGeoToolsName [48];
	char EpsgName   [64];
	char AppAltName [64];
	///////////////////////////////////////////////////////////////////////////
	// In future, we expect to support a "local" flavor of WKT.  This element
	// will contain the Local Alternate Name, which will be supplied via the
	// system's XML configuration file.
	char LclAltName [64];
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// TrcWktUnitNameMap  --  Form of Units Name Mapping Table
//
// An array of the following struture is used to map WKT unit names to a
// CS-MAP unit name and vice versa.  I don't know that there are different
// names for the different flavors, but I'm going to assume so.
//
// We're just mapping names here, so we care not about unit types.
//
struct TrcWktUnitNameMap
{
	char MsiName [16];					/* MSI unit name. */
	unsigned short EpsgNbr;				/* EPSG "Unit of Measure" code number */
	char EpsgName [32];					/* EPSG name as of v6.2 */
	char WktOgcName [32];				/* WKT OGC name, this may be the best choice */
	char WktGeoTiffName [32];			/* WKT GeoTiff name */ 
	char WktEsriName [32];				/* WKT ESRI name */
	char WktOracleName [32];			/* WKT Oracle name */
	char AppAltName [32];
	char LclAltName [32];
};
//newPage//
class TrcWktElement;
///////////////////////////////////////////////////////////////////////////////
// The following are non-member support functions implemented specifically to
// support processing of WKT data.
long32_t csExtractEsriNbr (std::istream& inStrm,unsigned& lineNbr);
std::string rcExtractWkt (std::istream& inStrm,unsigned& lineNbr,unsigned& startLineNbr);
const char *rcWktTypeToName (ErcWktEleType type);
bool rcWktHasInitialName (ErcWktEleType type);
ErcWktEleType rcWktNameToType (const char *name);
const char *rcWktAxisDirToName (ErcWktAxisValue axisValue);
ErcWktAxisValue rcWktNameToAxisValue (const char *name);
int CSelDictWktCompare (const char* elKeyNamePtr,const struct cs_Eldef_ *wktElDef);
///////////////////////////////////////////////////////////////////////////////
// TrcWktElements  --  Abstracts a vector (array) of TrcWktElement
class TrcWktElements
{
public:
	///////////////////////////////////////////////////////////////////////////
	// Construction  /  Detsruction  /  Assignemnt
	TrcWktElements (void);
	TrcWktElements (const TrcWktElements& source);
	~TrcWktElements (void);
	TrcWktElements& operator= (const TrcWktElements& rhs);
	///////////////////////////////////////////////////////////////////////////
	// Operator Overrides
	const TrcWktElement* operator[] (ErcWktEleType type) const;
	const TrcWktElement* operator[] (size_t index) const;
	TrcWktElements& operator-= (const TrcWktElement* toBeRemoved)
	{
		return RemoveChild (toBeRemoved);
	}
	///////////////////////////////////////////////////////////////////////////
	// Public Named Function Memebers
	size_t GetCount (void) const;
	void ParseGrandChildren (void);
	const TrcWktElement* LocateChild (ErcWktEleType type) const;
	const TrcWktElement* LocateChild (ErcWktEleType type,size_t& index) const;
	TrcWktElements& AddChild (const TrcWktElement& newChild,const TrcWktElement* parent,
															ErcWktEleType parentType);
	TrcWktElements& RemoveChild (const TrcWktElement* toBeRemoved);
private:
	std::vector<TrcWktElement> Elements;
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// A WKT primitive takes the following form.  it consists of a type (which
// replaces the keyword), a name (as most all primitives have a name as the
// first element of the primitive value), and a string value which is the
// remainder of the value after the name if any.
//
class TrcWktElement
{
public:
	///////////////////////////////////////////////////////////////////////////
	// Static Constants, Variables, and Functions
	static short BitCounter (ulong32_t bitMap);
	///////////////////////////////////////////////////////////////////////////
	// Construction  /  Destruction  /  Assignment
	TrcWktElement (void);
	TrcWktElement (ErcWktEleType type,const char *value);
	TrcWktElement (ErcWktEleType type,const char *name,const char *value);
	TrcWktElement (ErcWktEleType type,const std::string& value);
	TrcWktElement (ErcWktEleType type,const std::string& name,const std::string& value);
	TrcWktElement (const char *wellKnownText);
	TrcWktElement (const TrcWktElement& source);
	~TrcWktElement (void);
	TrcWktElement& operator= (const TrcWktElement& rhs);
	///////////////////////////////////////////////////////////////////////////
	// Public Named Member Functions
	ErcWktEleType GetElementType (void) const {return ElementType; };
	ErcWktEleType GetParentType (void) const {return ParentType; };
	std::string GetElementName (void) const {return ElementName; };
	const char* GetElementNameC (void) const {return ElementName.c_str (); };
	std::string GetCompleteValue (void) const {return Value; };
	bool HasInitialName (void) const;
	void SetParent (const TrcWktElement* parent) {WktParent = parent; };
	void SetParentType (ErcWktEleType type) {ParentType = type; };
	const TrcWktElement *ChildLocate (ErcWktEleType type) const;
	const TrcWktElement *ChildLocate (ErcWktEleType type,size_t& index) const;
	const TrcWktElement *ChildSearch (ErcWktEleType type) const;
	const TrcWktElement *ParameterLocate (ErcWktFlavor flavor,short csMapParamCode) const;
	// First field, which is usually the name, is field number 0.
	void GetFieldChar (char *bufr,size_t bufrSize,size_t fieldNbr) const;
	std::string GetFieldStr (size_t fieldNbr) const;
	double GetFieldDouble (size_t fieldNbr) const;
	long32_t GetFieldLong (size_t fieldNbr) const;
	ErcWktAxisId GetAxisId (void) const;
	ErcWktAxisValue GetAxisValue (void) const;
	ErcWktFlavor DetermineFlavor (ErcWktFlavor preferredFlvr = wktFlvrNone) const;
	unsigned GetValueCount (void) const;
	unsigned GetParameterCount (void) const;
	///////////////////////////////////////////////////////////////////////////
	// The following member functions are used to construct a WKT object for
	// subsequent output.
	void AddChild (const TrcWktElement& childElement);
	void RemoveChild (const TrcWktElement* toBeRemoved);
	void ReconstructValue (void);
	std::string ProduceWkt (void) const;
	void ParseChildren (void);
protected:
	ulong32_t NameBitMap (void) const;
	ulong32_t ProjectionBitMap (void) const;
	ulong32_t ParameterBitMap (short& paramCount) const;
	ulong32_t LinearUnitBitMap (void) const;
	ulong32_t AngularUnitBitMap (void) const;
private:
	///////////////////////////////////////////////////////////////////////////
	// Private Support Functions
	static ErcWktEleType ParseWellKnownText (std::string& value,size_t& eleStrt,
																size_t& eleTerm,
																const char *wellKnownText);
	static void RemoveElementFromString (std::string& value,size_t eleStrt,size_t eleTerm);
	std::string ExtractField  (size_t fieldNbr,bool trimQuotes = true) const;
	void ExtractField  (char *bufr,size_t bufrSize,size_t fieldNbr) const;
	void DeleteField (size_t fieldNbr);
	void CommonConstruct (void);
	static void TrimQuotes (std::string& value);
	static void TrimWhiteSpace (std::string& value);
	///////////////////////////////////////////////////////////////////////////
	// Private data items.
	const TrcWktElement *WktParent;			// pointer to parent element
	ErcWktEleType ElementType;				// Type of this primitive
	ErcWktEleType ParentType;				// Type of parent primitive
	std::string Value;						// Complete CSV value of the
											// primitive.
	std::string ElementName;				// First field of the value CSV,
											// enclosing quotes are stripped.
											// We call it name as it is a name
											// 99% of the time.
	TrcWktElements Children;				// WKT primitives parsed from
											// Value.  Extraneous stuff on
											// the front and end of Value
											// is ignored.
	unsigned short LineNbr;					// Line number in the stream at
											// which this primitive began.
	std::string FileName;					// last 15 characters of the
											// name of the file from which
											// this element was extracted
											// if known.  No path, no
											// extension.
};
