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

#define cs_EL2WKT_NMTRUNC  1
#define cs_DT2WKT_NMTRUNC  2
#define cs_CS2WKT_NMTRUNC  4
#define cs_DT2WKT_DTDEF    8
#define cs_DT2WKT_NODEF   16

#define cs_WKTFLG_MAPNAMES    0x01
#define cs_WKTFLG_ALLWFLVRSW  0x02
#define cs_WKTFLG_SGNFLVRSW   0x04

/******************************************************************************
   Various arrays of the following structure are used to reduce the length
   of WKT names assigned to coordinate systems, datums, and ellipsoids.  We
   use pointers here as all such arrays are declared as constants and the
   strings appearing on the "replace" side are often repeated. */
struct csKeyNmRed_
{
	const char *original;
	const char *replace;
};
/******************************************************************************
   TrcWktPrjNameMap  --  Form of Projection Code to Name mappint Table
  
   An array of the following struture is used to map WKT projection names to a
   CS-MAP cs_PRJCOD_????? value.  Not all of the supposedly supported WKT
   projection names (I've never heard of some of them) are supported by CS-MAP.

   In the table below, we have several names.  There is an enumeration,
   csWktFlavor, which is usually used to select which of the name entries is to
   be used.  It is likely that the number of these will grow. */
struct csWktPrjNameMap_
{
	unsigned short CsMapCode;			/* CS-MAP cs_PRJCOD_???? value */
	unsigned short EpsgNbr;				/* EPSG projection code number */
	char WktOgcName [64];				/* WKT OGC name, this may be the best choice */
	char WktGeoTiffName [64];			/* WKT GeoTiff name */ 
	char WktEsriName [64];				/* WKT ESRI name */
	char WktOracleName [64];			/* WKT Oracle name */
	char WktGeoToolsName [64];			/* GeoTools name */
	char EpsgName [64];					/* EPSG name as of v6.2 */
	/* The following field is reserved for application developer use.  That is,
	   the name and the space reserved by the declaration will be part of any
	   future distribution of this product.  Application developers who need
	   to, should write a completely separate module which populates this
	   member of the structure with the values they require.  This module can
	   be maintained completely independent of CS-MAP.  As a result, the
	   official table based on this structure as defined by RefCon is NOT
	   declared as constant. */
	char AppAltName [64];
	/* The following field is reserved for future support of a local flavor.
	   Names for the local flavor are expected to be obtained from the system's
	   XML initialization file; and these elements populated during
	   initialization.  */
	char LclAltName [64];
};

/* The following structure is used to convert EPSG Operation Codes to CS-MAP
   projection codes.  The cs_PRJCOD_END value is used to indicate an EPSG
   operation code which does not map to a supported CS-MAP projection. */
struct cs_EpsgOpMthMap_
{
	long epsgOpCodeLwr;
	long epsgOpCodeUpr;
	long csMapPrjCode;
};

/******************************************************************************
   The following functions hide all of the flavor stuff.  Probably not very
   useful if __CPP__ is not defined, but there is no reason why they should
   not compile. */

#ifdef __cplusplus
extern "C" {
#endif
int CS_isWkt (const char *wellKnownText);
long32_t CSwktPrjName2Code (const char *wktName,enum ErcWktFlavor flavor);
long32_t CSepsgOpCodeMap (const char *wktName,enum ErcWktFlavor flavor);
long32_t CSwktPrjName2Epsg (const char *wktName,enum ErcWktFlavor flavor);
const char* CSwktPrjCode2Name (long32_t csCode,enum ErcWktFlavor flavor);
const char* CSwktPrjEpsg2Name (long32_t epsgCode,enum ErcWktFlavor flavor);
int CSwktPrjSetName (long32_t csmapCode,const char *newName,enum ErcWktFlavor flavor);
long32_t CSwktPrmName2Code (const char *wktName,enum ErcWktFlavor flavor);
long32_t CSwktPrmName2Epsg (const char *wktName,enum ErcWktFlavor flavor);
const char* CSwktPrmEnum2Name (enum EcsWktParameter enumCode,enum ErcWktFlavor flavor);
const char* CSwktPrmCode2Name (long32_t csCode,enum ErcWktFlavor flavor);
const char* CSwktPrmEpsg2Name (long32_t epsgCode,enum ErcWktFlavor flavor);
int CSwktPrmSetName (enum EcsWktParameter selCode,const char *newName,enum ErcWktFlavor flavor);
const char* CSwktUnitName2MsiName (const char *wktName,enum ErcWktFlavor flavor);
int CSwktUnitSetName (long32_t espgNbr,const char *newName,enum ErcWktFlavor flavor);
const char* CSmsiUnitName2WktName (const char *msiName,enum ErcWktFlavor flavor);
long32_t CSwktUnitName2Epsg (const char *wktName,enum ErcWktFlavor flavor);
const char* CSepsgUnitNbr2Name (long32_t epsgNbr,enum ErcWktFlavor flavor);
int CSwktUnitSetName (long32_t espgNbr,const char *newName,enum ErcWktFlavor flavor);
double EXP_LVL3 CSwktMrcatStdParallelFromK (double e_sq,double scl_red);
#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////
// The following functions implement the CS-MAP 'C' interface to the Well
// Known Text C++ objects.
#ifdef __cplusplus
extern "C" {
#endif
int CS_wktToDt (struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,enum ErcWktFlavor flavor,const char *wellKnownText);
int CS_wktToCs (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,enum ErcWktFlavor flavor,const char *wellKnownText);
int CS_wktToCsEx (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,enum ErcWktFlavor flavor,const char *wellKnownText, int bRunBinaryFallback);
int CS_wktDictRpl (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef);
#ifdef __cplusplus
}
int CSwktToCs (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,enum ErcWktFlavor flavor,const class TrcWktElement* wktElement);
int CSwktToNerth (struct cs_Csdef_ *csDef,struct cs_Eldef_ *elDef,enum ErcWktFlavor flavor,const class TrcWktElement* wktElement);
#endif

/******************************************************************************
   The following are support functions to the above.  It is likely that these
   functions will become private members of the TrcWktElement class at some
   point in the future.  Application programmers should not rely on the
   future existence of these functions. */
#ifdef __cplusplus
int CS_wktEleToEl (struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktPtr);
int CS_wktEleToDt (struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktPtr);

/******************************************************************************
   Again, these functions are used internally.  Developers should not rely
   on access to these functions in future releases. */
short CS_wktReduceKeyNm (char *result,size_t rsltSize,const char *source,struct csKeyNmRed_ *reduceTbl);
void CS_wktElNameFix (char *ellipsoidName,size_t rsltSize,const char *srcName);
void CS_wktDtNameFix (char *datumName,size_t rsltSize,const char *srcName);
void CS_wktCsNameFix (char *csysName,size_t rsltSize,const char *srcName);
unsigned short CS_wktProjLookUp (ErcWktFlavor flavor,const char *wktName);
bool EXP_LVL3 CSgetParamNm (char* paramName,size_t paramSize,EcsNameFlavor nmFlavor,int paramCode);
#endif
