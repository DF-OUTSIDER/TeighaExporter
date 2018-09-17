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


//=============================================================================
// Enumerations used through the name ampping system.
//
// The types of objects which have names.  Note that an object type and a
// name flavor uniquely define a name space.
//
//                                                      for debug/test convenience
enum EcsMapObjType {	csMapNone = 0,
						csMapFlavorName,				//   1
						csMapParameterKeyName,			//   2
						csMapProjectionKeyName,			//   3
						csMapGeodeticOpMthKeyName,		//   4
						csMapVerticalOpMthKeyName,		//   5
						csMapLinearUnitKeyName,			//   6
						csMapAngularUnitKeyName,		//   7
						csMapPrimeMeridianKeyName,		//   8
						csMapEllipsoidKeyName,			//   9
						csMapGeodeticXfrmKeyName,		//  10
						csMapVerticalXfrmKeyName,		//  11
						csMapDatumKeyName,				//  12
						csMapVerticalDatumKeyName,		//  13
						csMapGeoidKeyName,				//  14
						csMapGeographicCSysKeyName,		//  15
						csMapProjectedCSysKeyName,		//  16
						csMapGeographic3DKeyName,		//  17
						csMapUnknown,					//  18
						csMapUnitKeyName,				// See comments below
						csMapProjGeoCSys				// See comments below
				   };

// The various flavors currently supported.
enum EcsNameFlavor {	csMapFlvrNone      =  0,		// 0x00000000
						csMapFlvrEpsg      =  1,		// 0x00000001
						csMapFlvrEsri      =  2,		// 0x00000002
						csMapFlvrOracle    =  3,		// 0x00000004
						csMapFlvrAutodesk  =  4,		// 0x00000008
						csMapFlvrBentley   =  5,		// 0x00000010
						csMapFlvrSafe      =  6,		// 0x00000020
						csMapFlvrMapInfo   =  7,		// 0x00000040
						csMapFlvrCtmx      =  8,		// 0x00000080
						csMapFlvrCsMap     =  9,		// 0x00000100
						csMapFlvrOGC       = 10,		// 0x00000200
						csMapFlvrOCR       = 11,		// 0x00000400
						csMapFlvrGeoTiff   = 12,		// 0x00000800
						csMapFlvrGeoTools  = 13,		// 0x00001000
						csMapFlvrOracle9   = 14,		// 0x00002000
						csMapFlvrIBM       = 15,		// 0x00004000
						csMapFlvrOem       = 16,		// 0x00008000
						csMapFlvrAnon01    = 17,
						csMapFlvrAnon02    = 18,
						csMapFlvrAnon03    = 19,
						csMapFlvrAnon04    = 20,
						csMapFlvrAnon05    = 21,
						csMapFlvrAnon06    = 22,
						csMapFlvrAnon07    = 23,
						csMapFlvrAnon08    = 24,
						csMapFlvrAnon09    = 25,
						csMapFlvrAnon10    = 26,
						csMapFlvrAnon11    = 27,
						csMapFlvrAnon12    = 28,
						csMapFlvrAnon13    = 29,
						csMapFlvrUser      = 30,
						csMapFlvrLegacy    = 31,
						csMapFlvrUnknown   = 32
				   };

// The following enumerates the fields in the maintenance name mapping tables.
// A structure array is used to map these values to field names.  The field
// names are then used to extract data.  In this manner, the order of the
// fields in a CSV file is not important, and can be adjusted to suit the
// needs of the individual editing the file.  Probably not necessary in this
// particular 'C' header, but its nice to have the enumerations in one place.
//
enum EcsMapTableFields {    csMapFldSeqNbr             = 0,
                            csMapFldEpsgNbr            = 1,
                            csMapFldEsriNbr            = 2,
                            csMapFldOracleNbr          = 3,
                            csMapFldMapInfoNbr         = 4,
                            csMapFldOemNbr             = 5,
                            csMapFldOracle9Nbr         = 6,
                            csMapFldCsMapNbr           = 7,
                            csMapFldCsMapName          = 8,
                            csMapFldCtmxName           = 9,
                            csMapFldAdskName           = 10,
                            csMapFldEpsgName           = 11,
                            csMapFldEsriName           = 12,
                            csMapFldOracleName         = 13,
                            csMapFldMapInfoName        = 14,
                            csMapFldOemName            = 15,
                            csMapFldGeoToolsName       = 16,
                            csMapFldOcrName            = 17,
                            csMapFldOgcName            = 18,
                            csMapFldGeoTiffName        = 19,
                            csMapFldOracle9Name        = 20,
                            csMapFldLegacyName         = 21,
                            csMapFldCnvrsnName         = 22,
                            csMapFldCnvrsnComment      = 23,
                            csMapFldMaintenanceComment = 24,
                            csMapFldQuality            = 25,
                            csMapFldUserFlag           = 26,
                            csMapFldRfcnQualifier      = 27,
                            csMapFldUnknown            = 99
                       };

// Status return value provided by certain API functions.  Note that
// csMapNoMatch indicates that the source name or ID was not located in the
// mapping table.  csMapNoName and csMapNoNbr indicate that the source name or
// ID was indeed located in the table, but that there was no name or number
// of the flavor requested resideint in the mapping table.
enum EcsMapSt { csMapOk = 0,
                csMapNoName  = 1,
                csMapNoNbr   = 2,
                csMapNoMatch = 4
              };
//newpage//
///////////////////////////////////////////////////////////////////////////////
// 'C' Function Prototypes.
//
// Identification Numbers are unsigned longs.  The following two values have
// the special values as indicated.  See csNameMapperSupport.cpp for the
// definitions.
//
// KcsNmMapNoNumber (i.e. No Number) indicates that the there is no number
// associated with a specific item.  For example, when mapping a name to a
// number, KcsNmMapNoNumber is returned when the name was indeed located, but
// no numeric ID was associated with the specific entitiy referenced.  This
// is not considered an error.
//
// KcsNmMapInvNumber (i.e. invalid number) indicates that a number expected
// to exist does not.  This value is returned when mapping, for example,
// an EPSG code to an Oracle SRID number and the EPSG code is not found in the
// tables.  This is considered an error and is reported through CS_erpt.  A
// description of the error suitable for reporting to the user is availble
// from CS_errmsg.
//
#if defined (__cplusplus)
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
// The following are the basic name/ID mapping functions.  Generally, To
// obtain a specific numeric ID, one uses one of the first two functions.  To
// obtain a specific definition name, one uses one of the second two functions.
// Eight but character functions are also provided.  Note that the wide
// character functions are the primary functions, and the 8 bit functions
// simply call the wide character functions after making the proper
// conversions.
//
// While we've successfully avoided this for 20 years in CS-MAP, it is now
// necessary to release some resources in certain environments.  You'll want
// to be sure to release the name mapper when using the name mapper in the
// Visual C++ debug environment with the memory leak detector active.  If you
// fail to release the name mapper, you'll twiddle your thumbs for a several
// minutes while Visual C++ compiles a list of some 50,000 suspected
// memory leaks.  Calling this function when a TcsNameMapper object does not
// exist is not a problem.
void csReleaseNameMapper (void);
///////////////////////////////////////////////////////////////////////////////
// The Name Mapper considers Projected Coordinate systems and Geographic
// Systems to be distinct groups of objects.  Since in CS-MAP, and perhaps
// other libraries, these two groups are considered to be a single name space.
// To alleviate the application programmer from having to continually code
// call to two separate API functions, a special object type has been
// intriduced: csMapProjGeoCSys.  When this object type is presented to
// any of the following functions, the function will always look for the
// source name or Id in the Projective group first, and then if that fails
// a search of the Geographic group is made.  Obviously, when using this
// object type, it is up to the application to determine what it got back, if
// it is important in the context of the application.
//
// Wide character API.
unsigned long csMapNameToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													 enum EcsNameFlavor srcFlavor,
													 const wchar_t* srcName);
unsigned long csMapIdToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
											       enum EcsNameFlavor srcFlavor,
												   unsigned long srcId);
const wchar_t* csMapNameToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
												        enum EcsNameFlavor srcFlavor,
													    const wchar_t* srcName);
const wchar_t* csMapIdToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  enum EcsNameFlavor srcFlavor,
													  unsigned long srcId);
// Narrow character API													  
unsigned long csMapNameToIdC (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  enum EcsNameFlavor srcFlavor,
													  const char* srcName);
enum EcsMapSt csMapNameToNameC (enum EcsMapObjType type,char* trgName,
                                                        size_t trgSize,
                                                        enum EcsNameFlavor trgFlavor,
												        enum EcsNameFlavor srcFlavor,
														const char* srcName);
enum EcsMapSt csMapIdToNameC (enum EcsMapObjType type,char* trgName,
                                                      size_t trgSize,
                                                      enum EcsNameFlavor trgFlavor,
													  enum EcsNameFlavor srcFlavor,
													  unsigned long srcId);
// Access by index functions, useful for producing lists and reports.  These
// functions will not work with csMapProjGeog object type.
unsigned long csGetIdsByIdx (enum EcsMapObjType type,enum EcsNameFlavor flavor,unsigned index);
const wchar_t* csGetNamesByIdx (enum EcsMapObjType type,enum EcsNameFlavor flavor,unsigned index);
enum EcsMapSt csGetNamesByIdxC (char* nextName,size_t nameSize,enum EcsMapObjType type,
                                                               enum EcsNameFlavor flavor,
                                                               unsigned index);
                                                              
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following functions provide a generic means of mapping names to a
// common flavor not necessarily known by a remote portion of an application.
//
// That is, a specific flavor is maintained in a global variable, and the
// associated mapping functions assume that flavor to be the target flavor
// for all mappings.
//
enum EcsNameFlavor csSetSystemFlavor (enum EcsNameFlavor sysFlvr);
unsigned long csNameToSysId (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													 const wchar_t* srcName);
unsigned long csIdToSysId (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
												   unsigned long srcId);
const wchar_t* csNameToSysName (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													    const wchar_t* srcName);
const wchar_t* csIdToSysName (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													  unsigned long srcId);
unsigned long csNameToSysIdC (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													  const char* srcName);
enum EcsMapSt csNameToSysNameC (enum EcsMapObjType type,char* trgName,
                                                        size_t trgSize,
												        enum EcsNameFlavor srcFlavor,
														const char* srcName);
enum EcsMapSt csIdToSysNameC (enum EcsMapObjType type,char* trgName,
                                                      size_t trgSize,
													  enum EcsNameFlavor srcFlavor,
													  unsigned long srcId);
													  
unsigned long csSysNameToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													 const wchar_t* srcName);
unsigned long csSysIdToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
												   unsigned long srcId);
const wchar_t* csSysNameToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													    const wchar_t* srcName);
const wchar_t* csSysIdToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  unsigned long srcId);
unsigned long csSysNameToIdC (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  const char* srcName);
enum EcsMapSt csSysNameToNameC (enum EcsMapObjType type,char* trgName,
                                                        size_t trgSize,
												        enum EcsNameFlavor trgFlavor,
														const char* srcName);
enum EcsMapSt csSysIdToNameC (enum EcsMapObjType type,char* trgName,
                                                      size_t trgSize,
													  enum EcsNameFlavor trgFlavor,
													  unsigned long srcId);
///////////////////////////////////////////////////////////////////////////////
// Use the following functions to determine the existence, and flavor, of a
// specific name.  The return value is a bit map of the EcsNameFlavor
// enumeration values; each bit indicating that the name is recognized for the
// indicated flavor.  If the name exists, there needs to be a flavor assigned.
// Therefore, a zero return indicates that the name did not exist.
unsigned long csGetFlavorBitMap  (enum EcsMapObjType type,const wchar_t* name);
unsigned long csGetFlavorBitMapC (enum EcsMapObjType type,const char* name);
///////////////////////////////////////////////////////////////////////////////
// Until such time as the WKT code is modified to use the Name Mapping flavor
// enumerator, we use the following to switch between the two distinct flavor
// enumerators.
enum EcsNameFlavor csWktFlvrToCsMapFlvr (enum ErcWktFlavor wkt);
enum ErcWktFlavor csCsMapFlvrToWktFlvr (enum EcsNameFlavor csMap);
//=============================================================================
// The following rather simple functions replace the need for the sizeable
// MappingMsiToAdsk module, and the equally sizeable CS_epsg.c modules.
// Note that the pointer returned by the CSepsg2adskCS function is a pointer
// to a static character array declared in that function.  This is necessary
// as the Name Mapper only deals with wchar_t names.
//=============================================================================
const char* CSepsg2adskCS (long epsgNbr);
long CSadsk2epsgCS (const char *adskKeyName);

#if defined (__cplusplus)
}
#endif
