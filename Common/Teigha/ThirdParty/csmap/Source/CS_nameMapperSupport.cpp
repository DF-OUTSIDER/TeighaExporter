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
// Now comes May 21, 2014 
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.

//lint -esym(766,..\Include\cs_wkt.h)          inreferenced header file (included for pre-compiled headers)
//lint -esym(1927,TcsGenericId::Id)            not initialized in formal initializer list (occasionally).
//lint -e1929                                  operator++ on enumerators returns a reference

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

extern "C" char cs_Dir [];
extern "C" char* cs_DirP;
extern "C" char csErrnam [];
extern "C" const unsigned long KcsNmMapNoNumber = 0UL;
extern "C" const unsigned long KcsNmInvNumber = 0xFFFFFFFFUL;
extern "C" char cs_NameMapperName [];

// The following variable is declared static to discourage code outside
// this module from using the name map object directly.
static TcsNameMapper* KcsNameMapperPtr = 0;

EcsMapObjType& operator++ (EcsMapObjType& mapClass)
{
	if (mapClass < csMapUnknown)
	{
		mapClass = EcsMapObjType (mapClass + 1);
	}
	else
	{
		mapClass = csMapUnknown;
	}
	return mapClass;
}
EcsNameFlavor& operator++ (EcsNameFlavor& flavor)
{
	if (flavor < csMapFlvrUnknown)
	{
		flavor = EcsNameFlavor (flavor + 1);
	}
	else
	{
		flavor = csMapFlvrUnknown;
	}
	return flavor;
}
EcsMapTableFields& operator++ (EcsMapTableFields& field)
{
	if (field < csMapFldUnknown)
	{
		field = EcsMapTableFields (field + 1);
	}
	else
	{
		field = csMapFldUnknown;
	}
	return field;
}
EcsMapSt& operator|= (EcsMapSt& primary,const EcsMapSt& rhs)		//lint !e714  not referenced
{
	primary = static_cast<EcsMapSt>(static_cast<int>(primary) | static_cast<int>(rhs));
	return primary;
}

unsigned long csGetFlavorBitMap  (enum EcsMapObjType type,const wchar_t* name)
{
	unsigned long flvrBitMap (0UL);
	TcsGenericId genericId;

	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
		EcsNameFlavor flavor = csMapFlvrNone;
		++flavor; 
		for (;flavor != csMapFlvrUnknown;++flavor)
		{
	        genericId = mapperPtr->Locate (type,flavor,name);
	        if (genericId.IsKnown ())
	        {
				flvrBitMap |= (1UL << static_cast<int>(flavor - 1));
	        }
		}
	}
	return flvrBitMap;
}
unsigned long csGetFlavorBitMapC (enum EcsMapObjType type,const char* name)
{
    wchar_t wcName [256];

    mbstowcs (wcName,name,256);
	unsigned long flvrBitMap = csGetFlavorBitMap (type,wcName);
	return flvrBitMap;
}
enum EcsNameFlavor csWktFlvrToCsMapFlvr (enum ErcWktFlavor wkt)
{
    EcsNameFlavor result = csMapFlvrUnknown;
    switch (wkt) {
    case wktFlvrNone:      result = csMapFlvrNone;     break;
    // Bit of a ludge here.  What CS-MAP has called Ocr for many years, should have been OGR;
    // and that's what is now coded.  What we now call OGC, is a separate flavor specifically
    // for OGC.  The CS-MAP author was confused between the two and really didn't know the
    // difference until recently. -- NTO 12/17/2007
    case wktFlvrOgc:       result = csMapFlvrOCR;      break;
    case wktFlvrGeoTiff:   result = csMapFlvrGeoTiff;  break;
    case wktFlvrEsri:      result = csMapFlvrEsri;     break;
    case wktFlvrOracle:    result = csMapFlvrOracle;   break;
    case wktFlvrGeoTools:  result = csMapFlvrGeoTools; break;
    case wktFlvrEpsg:      result = csMapFlvrEpsg;     break;
    case wktFlvrOracle9:   result = csMapFlvrOracle9;  break;
    case wktFlvrAutodesk:  result = csMapFlvrAutodesk; break;
    case wktFlvrUnknown:   result = csMapFlvrUnknown;  break;
    case wktFlvrAppAlt:    result = csMapFlvrUnknown;  break;
    case wktFlvrLclAlt:    result = csMapFlvrUnknown;  break;
    case wktFlvrLegacy:    result = csMapFlvrLegacy;   break;
    default:               result = csMapFlvrUnknown;  break;
    }
    return result;
}
enum ErcWktFlavor csCsMapFlvrToWktFlvr (enum EcsNameFlavor csMap)
{
    ErcWktFlavor result = wktFlvrNone;
    switch (csMap) {
    case csMapFlvrNone:      result = wktFlvrNone;      break;
    case csMapFlvrEpsg:      result = wktFlvrEpsg;      break;
    case csMapFlvrEsri:      result = wktFlvrEsri;      break;
    case csMapFlvrOracle:    result = wktFlvrOracle;    break;
    case csMapFlvrAutodesk:  result = wktFlvrAutodesk;  break;
    case csMapFlvrBentley:   result = wktFlvrNone;      break;
    case csMapFlvrSafe:      result = wktFlvrNone;      break;
    case csMapFlvrMapInfo:   result = wktFlvrNone;      break;
    case csMapFlvrCtmx:      result = wktFlvrNone;      break;
    case csMapFlvrCsMap:     result = wktFlvrNone;      break;
    case csMapFlvrOGC:       result = wktFlvrNone;      break;
    case csMapFlvrOCR:       result = wktFlvrOgc;       break;
    case csMapFlvrGeoTiff:   result = wktFlvrGeoTiff;   break;
    case csMapFlvrGeoTools:  result = wktFlvrGeoTools;  break;
    case csMapFlvrOracle9:   result = wktFlvrOracle9;   break;
    case csMapFlvrIBM:       result = wktFlvrNone;      break;
    case csMapFlvrOem:       result = wktFlvrNone;      break;
    case csMapFlvrAnon01:    result = wktFlvrNone;      break;
    case csMapFlvrAnon02:    result = wktFlvrNone;      break;
    case csMapFlvrAnon03:    result = wktFlvrNone;      break;
    case csMapFlvrAnon04:    result = wktFlvrNone;      break;
    case csMapFlvrAnon05:    result = wktFlvrNone;      break;
    case csMapFlvrAnon06:    result = wktFlvrNone;      break;
    case csMapFlvrAnon07:    result = wktFlvrNone;      break;
    case csMapFlvrAnon08:    result = wktFlvrNone;      break;
    case csMapFlvrAnon09:    result = wktFlvrNone;      break;
    case csMapFlvrAnon10:    result = wktFlvrNone;      break;
    case csMapFlvrAnon11:    result = wktFlvrNone;      break;
    case csMapFlvrAnon12:    result = wktFlvrNone;      break;
    case csMapFlvrAnon13:    result = wktFlvrNone;      break;
    case csMapFlvrUser:      result = wktFlvrNone;      break;
    case csMapFlvrLegacy:    result = wktFlvrLegacy;    break;
    case csMapFlvrUnknown:   result = wktFlvrNone;      break;
    default:                 result = wktFlvrNone;      break;
    }
    return result;
}

// Each of the Name mapper API functions calls this function to get a pointer
// to the name mapping object.  A pointer is returned, rather than a reference,
// as a null return value indicates that a name mapper object could not be
// constructed for whatever reason.
TcsNameMapper* cmGetNameMapperPtr (bool release)
{
    if (release)
    {
        if (KcsNameMapperPtr != 0)
        {
            delete KcsNameMapperPtr;
            KcsNameMapperPtr = 0;
        }
    }
    else if (KcsNameMapperPtr == 0)
    {
        // Load data from name mapper file
        // Load complete file at once into a memory buffer, then parse the information from there.
        TcsCsvStatus csvStatus;
        // Open name mapper file
        strcpy (cs_DirP,cs_NameMapperName);
        csFILE* pNameMapperFile = CS_fopen(cs_Dir, "rb");
        if (NULL != pNameMapperFile)
        {
            // Create new name mapper instance
            std::auto_ptr<TcsNameMapper> nameMapperPtr(new TcsNameMapper());
            if (0 == CS_fseek(pNameMapperFile, 0, SEEK_END))
            {
                // Get name mapper file size
                long fileSize = CS_ftell(pNameMapperFile);
                if (fileSize > 0L)
                {
                    // Allocate temporary memory buffer for the name mapper file content
                    char* pBuffer = (char*) CS_malc((size_t)fileSize);
                    if (NULL != pBuffer)
                    {
                        if (0 == CS_fseek(pNameMapperFile, 0, SEEK_SET))
                        {
                            // Copy complete name mapper file into the memory
                            CS_fread(pBuffer, sizeof(char),(size_t)fileSize, pNameMapperFile);		//lint !e534  ignoring return value
                            if (CS_ferror(pNameMapperFile))
			                {
				                CS_erpt (cs_IOERR);
			                }
                            else
                            {
                                // Close first name mapper file. The content is already in the memory buffer.
                                CS_fclose(pNameMapperFile);
                                pNameMapperFile = NULL;

                                // Fill up the name mapper with the in memory name mapper file buffer
                                EcsCsvStatus status = nameMapperPtr->ReadFromStream (pBuffer,(size_t)fileSize); // Note: Time consuming
                                if (status == csvOk)
                                {
                                    // Assign the name mapper
                                    KcsNameMapperPtr = nameMapperPtr.release();
                                }
                                else
                                {
                                    // Failed to fill up the name mapper
                                    char msgBufr [2048];
                                    CS_sprintf (msgBufr,"%s [%lu]",cs_NameMapperName,csvStatus.LineNbr);
                                    CS_stncp (csErrnam,msgBufr,MAXPATH);
                                    CS_erpt (cs_NMMAP_FAIL2);
                                }
                            }
                        }
                        // Clean up temporary file buffer
                        CS_free(pBuffer);
                        pBuffer = NULL;
                    }
                    else
                    {
                        CS_erpt(cs_NO_MEM);
                    }
                }
            }
            // Close name mapper file if it is not already done above
            if (NULL != pNameMapperFile)
            {
                CS_fclose(pNameMapperFile);
                pNameMapperFile = NULL;
            }
        }
        else
        {
            CS_stncp (csErrnam,cs_Dir,MAXPATH);
            CS_erpt (cs_NMMAP_FAIL1);
        }
    }
	return KcsNameMapperPtr;
}
void csReleaseNameMapper (void)
{
	cmGetNameMapperPtr (true);//lint !e534   ignoring return value
}
///////////////////////////////////////////////////////////////////////////////
unsigned long csMapNameToId (EcsMapObjType type,EcsNameFlavor trgFlavor,EcsNameFlavor srcFlavor,
																		const wchar_t* srcName)
{
    EcsMapObjType type2 = csMapNone;
	unsigned long rtnValue = KcsNmInvNumber;
	TcsGenericId genericId;

	// The csMapProjGeoCSys type is intended to search bot the projected and
	// geographic name spaces.  Note that the projected namespace is given
	// priority.  In the case of units, Linear untis is given the priority.
    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
	    genericId = mapperPtr->Locate (type,srcFlavor,srcName);
	    if (genericId.IsNotKnown () && type2 != csMapNone)
	    {
	        type = type2;
	        genericId = mapperPtr->Locate (type,srcFlavor,srcName);
	    }
	    if (genericId.IsKnown ())
	    {
		    rtnValue = mapperPtr->LocateNumber (type,trgFlavor,genericId);
	    }
	}
	return rtnValue;
}
unsigned long csMapIdToId (EcsMapObjType type,EcsNameFlavor trgFlavor,EcsNameFlavor srcFlavor,
																	  unsigned long srcId)
{
    EcsMapObjType type2 = csMapNone;
	unsigned long rtnValue = KcsNmInvNumber;
	TcsGenericId genericId;

    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
	    genericId = mapperPtr->Locate (type,srcFlavor,srcId);
	    if (genericId.IsNotKnown () && type2 != csMapNone)
	    {
	        type = type2;
	        genericId = mapperPtr->Locate (type,srcFlavor,srcId);
	    }
	    if (genericId.IsKnown ())
	    {
		    rtnValue = mapperPtr->LocateNumber (type,trgFlavor,genericId);
	    }
	}
	return rtnValue;
}
const wchar_t* csMapNameToName (EcsMapObjType type,EcsNameFlavor trgFlavor,EcsNameFlavor srcFlavor,
																		   const wchar_t* srcName)
{
    EcsMapObjType type2 = csMapNone;
	const wchar_t* rtnValue = 0;
	TcsGenericId genericId;

    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
	    genericId = mapperPtr->Locate (type,srcFlavor,srcName);
	    if (genericId.IsNotKnown () && type2 != csMapNone)
	    {
	        type = type2;
	        genericId = mapperPtr->Locate (type,srcFlavor,srcName);
	    }
	    if (genericId.IsKnown ())
	    {
		    rtnValue = mapperPtr->LocateName (type,trgFlavor,genericId);
	    }
	}
	return rtnValue;
}
const wchar_t* csMapIdToName (EcsMapObjType type,EcsNameFlavor trgFlavor,EcsNameFlavor srcFlavor,
																		 unsigned long srcId)
{
    EcsMapObjType type2 = csMapNone;
	const wchar_t* rtnValue = 0;
	TcsGenericId genericId;

    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
	    genericId = mapperPtr->Locate (type,srcFlavor,srcId);
	    if (genericId.IsNotKnown () && type2 != csMapNone)
	    {
	        type = type2;
	        genericId = mapperPtr->Locate (type,srcFlavor,srcId);
	    }
	    if (genericId.IsKnown ())
	    {
		    rtnValue = mapperPtr->LocateName (type,trgFlavor,genericId);
	    }
	}
	return rtnValue;
}
EcsMapSt csMapNameToNameC (EcsMapObjType type,char* trgName,size_t trgSize,
                                                            EcsNameFlavor trgFlavor,
															EcsNameFlavor srcFlavor,
															const char* srcName)
{
    EcsMapSt status = csMapNoMatch;
    const wchar_t* wcPtr;
    wchar_t wcName [256];

    if (trgName != 0 && trgSize > 0)
    {
        *trgName = '\0';
        mbstowcs (wcName,srcName,256);
        wcPtr = csMapNameToName (type,trgFlavor,srcFlavor,wcName);
        if (wcPtr != 0)
        {
            status = (*wcPtr == L'\0') ? csMapNoName : csMapOk;
            wcstombs (trgName,wcPtr,trgSize);
        }
    }
    return status;
}
EcsMapSt csMapIdToNameC (EcsMapObjType type,char* trgName,size_t trgSize,
                                                          EcsNameFlavor trgFlavor,
												          EcsNameFlavor srcFlavor,
													      unsigned long srcId)
{
    EcsMapSt status = csMapNoMatch;
    const wchar_t* wcPtr;

    if (trgName != 0 && trgSize > 0)
    {
        *trgName = '\0';
        wcPtr = csMapIdToName (type,trgFlavor,srcFlavor,srcId);
        if (wcPtr != 0)
        {
            status = (*wcPtr == L'\0') ? csMapNoName : csMapOk;
            wcstombs (trgName,wcPtr,trgSize);
        }
    }
    return status;
}
unsigned long csMapNameToIdC (EcsMapObjType type,EcsNameFlavor trgFlavor,EcsNameFlavor srcFlavor,
																		 const char* srcName)
{
    unsigned long rtnValue;
    wchar_t wcName [256];

    mbstowcs (wcName,srcName,256);
    rtnValue = csMapNameToId (type,trgFlavor,srcFlavor,wcName);
    return rtnValue;
}
// Access by index functions
unsigned long csGetIdsByIdx (enum EcsMapObjType type,EcsNameFlavor flavor,unsigned index)
{
    EcsMapObjType type2 = csMapNone;
    unsigned count;
    unsigned long rtnValue = KcsNmInvNumber;

    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
    	rtnValue = mapperPtr->LocateIdByIdx (type,flavor,index,&count);
    	if (rtnValue == KcsNmInvNumber && type2 != csMapNone)
    	{
    	    index -= count;
        	rtnValue = mapperPtr->LocateIdByIdx (type2,flavor,index);
    	}
	}
	return rtnValue;
}
const wchar_t* csGetNamesByIdx (enum EcsMapObjType type,EcsNameFlavor flavor,unsigned index)
{
    EcsMapObjType type2 = csMapNone;
    unsigned count;
    const wchar_t* wcPtr = 0;

    if (type == csMapProjGeoCSys)
    {
        type  = csMapProjectedCSysKeyName;
        type2 = csMapGeographicCSysKeyName;
    }
    else if (type == csMapUnitKeyName)
    {
        type  = csMapLinearUnitKeyName;
        type2 = csMapAngularUnitKeyName;
    }
	const TcsNameMapper* mapperPtr = cmGetNameMapperPtr ();
	if (mapperPtr == 0)
	{
	    CS_erpt (cs_NMMAP_INIT);
	}
	else
	{
    	wcPtr = mapperPtr->LocateNameByIdx (type,flavor,index,&count);
    	if (wcPtr == 0 && type2 != csMapNone)
    	{
    	    index -= count;
        	wcPtr = mapperPtr->LocateNameByIdx (type2,flavor,index);
    	}
	}
	return wcPtr;
}
EcsMapSt csGetNamesByIdxC (char* nextName,size_t nameSize,enum EcsMapObjType type,
                                                         EcsNameFlavor flavor,
                                                         unsigned index)
{
    EcsMapSt status = csMapNoMatch;
    const wchar_t* wcPtr;

    if (nextName != 0 && nameSize > 0)
    {
        *nextName = '\0';
        wcPtr = csGetNamesByIdx (type,flavor,index);
        if (wcPtr != 0)
        {
            if (*wcPtr == L'\0')
            {
                status = csMapNoName;
            }
            else
            {
                wcstombs (nextName,wcPtr,nameSize);
                status = csMapOk;
            }
        }
    }
    return status;
}                                                              
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following functions are identical to the above with the exception that
// the target flavor is maintained as a global variable whose value is set by
// the csSetSystemFlavor function.
static EcsNameFlavor KcsSystemFlavor = csMapFlvrAutodesk;
// Conversions _TO_ System Flavor; that is, the target flavor is preset to the
// system  flavor.
EcsNameFlavor csSetSystemFlavor (EcsNameFlavor sysFlvr)
{
    EcsNameFlavor rtnValue = KcsSystemFlavor;
    if (sysFlvr != csMapFlvrNone)
    {
        KcsSystemFlavor = sysFlvr;
    }
    return rtnValue;
}
unsigned long csNameToSysId (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													 const wchar_t* srcName)
{
    unsigned long rtnValue;
    
    rtnValue = csMapNameToId (type,KcsSystemFlavor,srcFlavor,srcName);
    return rtnValue;
}
unsigned long csIdToSysId (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
												   unsigned long srcId)
{
    unsigned long rtnValue;
    
    rtnValue = csMapIdToId (type,KcsSystemFlavor,srcFlavor,srcId);
    return rtnValue;
}
const wchar_t* csNameToSysName (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													    const wchar_t* srcName)
{
    const wchar_t* rtnValue;
    
    rtnValue = csMapNameToName (type,KcsSystemFlavor,srcFlavor,srcName);
    return rtnValue;
}													    
const wchar_t* csIdToSysName (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													  unsigned long srcId)
{
    const wchar_t* rtnValue;
    
    rtnValue = csMapIdToName (type,KcsSystemFlavor,srcFlavor,srcId);
    return rtnValue;
}													    
unsigned long csNameToSysIdC (enum EcsMapObjType type,enum EcsNameFlavor srcFlavor,
													  const char* srcName)
{
    unsigned long rtnValue;
    
    rtnValue = csMapNameToIdC (type,KcsSystemFlavor,srcFlavor,srcName);
    return rtnValue;
}													  
EcsMapSt csNameToSysNameC (enum EcsMapObjType type,char* trgName,size_t trgSize,
            												     enum EcsNameFlavor srcFlavor,
			            									     const char* srcName)
{
    EcsMapSt rtnValue;
    
    rtnValue = csMapNameToNameC (type,trgName,trgSize,KcsSystemFlavor,srcFlavor,srcName);
    return rtnValue;
}														
EcsMapSt csIdToSysNameC (enum EcsMapObjType type,char* trgName,size_t trgSize,
								        					   enum EcsNameFlavor srcFlavor,
									        				   unsigned long srcId)
{
    EcsMapSt rtnValue;
    
    rtnValue = csMapIdToNameC (type,trgName,trgSize,KcsSystemFlavor,srcFlavor,srcId);
    return rtnValue;
}													  
// Conversions _FROM_ System Flavor; that is the source flavor is pre-set to
// the system flavor.
unsigned long csSysNameToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													 const wchar_t* srcName)
{
    unsigned long rtnValue;
    
    rtnValue = csMapNameToId (type,trgFlavor,KcsSystemFlavor,srcName);
    return rtnValue;
}
unsigned long csSysIdToId (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
												   unsigned long srcId)
{
    unsigned long rtnValue;
    
    rtnValue = csMapIdToId (type,trgFlavor,KcsSystemFlavor,srcId);
    return rtnValue;
}
const wchar_t* csSysNameToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													    const wchar_t* srcName)
{
    const wchar_t* rtnValue;
    
    rtnValue = csMapNameToName (type,trgFlavor,KcsSystemFlavor,srcName);
    return rtnValue;
}													    
const wchar_t* csSysIdToName (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  unsigned long srcId)
{
    const wchar_t* rtnValue;
    
    rtnValue = csMapIdToName (type,trgFlavor,KcsSystemFlavor,srcId);
    return rtnValue;
}													    
unsigned long csSysNameToIdC (enum EcsMapObjType type,enum EcsNameFlavor trgFlavor,
													  const char* srcName)
{
    unsigned long rtnValue;
    
    rtnValue = csMapNameToIdC (type,trgFlavor,KcsSystemFlavor,srcName);
    return rtnValue;
}													  
EcsMapSt csSysNameToNameC (enum EcsMapObjType type,char* trgName,size_t trgSize,
            												     enum EcsNameFlavor trgFlavor,
			            									     const char* srcName)
{
    EcsMapSt rtnValue;
    
    rtnValue = csMapNameToNameC (type,trgName,trgSize,trgFlavor,KcsSystemFlavor,srcName);
    return rtnValue;
}														
EcsMapSt csSysIdToNameC (enum EcsMapObjType type,char* trgName,size_t trgSize,
								        					   enum EcsNameFlavor trgFlavor,
									        				   unsigned long srcId)
{
    EcsMapSt rtnValue;
    
    rtnValue = csMapIdToNameC (type,trgName,trgSize,trgFlavor,KcsSystemFlavor,srcId);
    return rtnValue;
}
//newPage//
///////////////////////////////////////////////////////////////////////////////
// TcsGenericId  --  Object which represents a generic ID.
//
// Every entry in the TcsNameMapper object has a generic ID assigned.  The
// generic ID is defined to be unique for each referenced definition.  That is,
// two entries which refer to the "same thing" will have the same generic ID.
//
// The design is such that the typical generic ID is the EPSG code number.  In
// the case an EPSG code number is not available, the generic ID becomes the
// the flavor numeric ID + the flavor number * 1.0E+08.  If the item/flavor
// does not have a numeric ID, one is assigned working from 90,000,000 on up.
// Each reincarnation of a TcsNameMapper object constructed from scratch will
// restart the process of assigning generic ID's.  Thus, generic ID's are only
// to be used internally; NEVER EXTERNALLY.  Should a public function ever be
// written which returns a Generic ID, it is to be considered a
// testing/debugging aid only.
///////////////////////////////////////////////////////////////////////////////
// Construction  /  Destruction  /  Assignment
TcsGenericId::TcsGenericId (unsigned long id) : Id (id)
{
}
TcsGenericId::TcsGenericId (EcsNameFlavor idFlavor,unsigned long id)
{
	Id = static_cast<unsigned long>(idFlavor) * TcsNameMapper::KcsNameMapBias + id;
}
TcsGenericId::TcsGenericId (const TcsGenericId& source) : Id (source.Id)
{
}
TcsGenericId& TcsGenericId::operator= (const TcsGenericId& rhs)
{
	Id = rhs.Id;
	return *this;
}								//lint !e1529   not checking for (this == &rhs)
TcsGenericId::~TcsGenericId (void)
{
	// Nothing to do here.  This is not virtual.
}
bool TcsGenericId::operator< (const TcsGenericId& rhs) const
{
	return (Id < rhs.Id);
}
bool TcsGenericId::operator== (const TcsGenericId& rhs) const
{
	return (Id == rhs.Id);
}
bool TcsGenericId::operator!= (const TcsGenericId& rhs) const
{
	return (Id != rhs.Id);
}
EcsNameFlavor TcsGenericId::GetInternalIdFlavor (void) const
{
	EcsNameFlavor rtnValue = csMapFlvrUnknown;
	int value = Id / TcsNameMapper::KcsNameMapBias;
	rtnValue = static_cast<EcsNameFlavor>(value);
	return rtnValue;
}
EcsNameFlavor TcsGenericId::ExtractFlavorId (unsigned long& flvrId) const
{
	EcsNameFlavor rtnValue (csMapFlvrUnknown);

	rtnValue = static_cast<EcsNameFlavor>(Id / TcsNameMapper::KcsNameMapBias);
	flvrId = static_cast<unsigned long>(Id % TcsNameMapper::KcsNameMapBias);
	return rtnValue;
}
//newPage//
const unsigned TcsKeyNameMapFile::BogusNbr = 0xFFFFFFFF;

// The following table is used to iterate through a record in a
// name mapping CSV file.
const TcsKeyNameMapFile::TcsMapTableFields TcsKeyNameMapFile::KcsMapTableFields [] =
{
    {  csMapFldSeqNbr,                csMapFlvrNone,        false,  L"SeqNbr"              },
    {  csMapFldEpsgNbr,               csMapFlvrEpsg,        true,   L"EpsgNbr"             },
    {  csMapFldEsriNbr,               csMapFlvrEsri,        true,   L"EsriNbr"             },
    {  csMapFldOracleNbr,             csMapFlvrOracle,      true,   L"OracleNbr"           },
    {  csMapFldMapInfoNbr,            csMapFlvrMapInfo,     true,   L"MapInfoNbr"          },
    {  csMapFldOemNbr,                csMapFlvrOem,         true,   L"OemNbr"              },
    {  csMapFldOracle9Nbr,            csMapFlvrOracle9,     true,   L"Oracle9Nbr"          },
    {  csMapFldCsMapNbr,              csMapFlvrCsMap,       true,   L"CsMapNbr"            },
    {  csMapFldCsMapName,             csMapFlvrCsMap,       false,  L"CsMapName"           },
    {  csMapFldCtmxName,              csMapFlvrCtmx,        false,  L"CtmxName"            },
    {  csMapFldAdskName,              csMapFlvrAutodesk,    false,  L"AdskName"            },
    {  csMapFldEpsgName,              csMapFlvrEpsg,        false,  L"EpsgName"            },
    {  csMapFldEsriName,              csMapFlvrEsri,        false,  L"EsriName"            },
    {  csMapFldOracleName,            csMapFlvrOracle,      false,  L"OracleName"          },
    {  csMapFldMapInfoName,           csMapFlvrMapInfo,     false,  L"MapInfoName"         },
    {  csMapFldOemName,               csMapFlvrOem,         false,  L"OemName"             },
    {  csMapFldGeoToolsName,          csMapFlvrGeoTools,    false,  L"GeoToolsName"        },
    {  csMapFldOcrName,               csMapFlvrOCR,         false,  L"OcrName"             },
    {  csMapFldOgcName,               csMapFlvrOGC,         false,  L"OgcName"             },
    {  csMapFldGeoTiffName,           csMapFlvrGeoTiff,     false,  L"GeoTiffName"         },
    {  csMapFldOracle9Name,           csMapFlvrOracle9,     false,  L"Oracle9Name"         },
    {  csMapFldLegacyName,            csMapFlvrLegacy,      false,  L"LegacyName"          },
    {  csMapFldCnvrsnName,            csMapFlvrNone,        false,  L"CnvrsnName"          },
    {  csMapFldCnvrsnComment,         csMapFlvrNone,        false,  L"CnvrsnComment"       },
    {  csMapFldMaintenanceComment,    csMapFlvrNone,        false,  L"MaintenanceComment"  },
    {  csMapFldQuality,               csMapFlvrNone,        false,  L"Quality"             },
    {  csMapFldUserFlag,              csMapFlvrNone,        false,  L"UserFlag"            },
    {  csMapFldRfcnQualifier,         csMapFlvrNone,        false,  L"RfcnQualifier"       },
    {  csMapFldUnknown,               csMapFlvrNone,        false,  L""                    }
};
void TcsKeyNameMapFile::WriteCsvFileHeader (std::wostream& oStrm)
{
	const TcsMapTableFields* tblPtr;
	
	for (tblPtr = KcsMapTableFields;tblPtr->FldId != csMapFldUnknown;++tblPtr)
	{
		if (tblPtr->FldId != csMapFldSeqNbr)
		{
			oStrm << L',';
		}
		oStrm << tblPtr->FldName;
	}
	oStrm << std::endl;
}
//=========================================================================
// Construction, Destruction, and Assignment
TcsKeyNameMapFile::TcsKeyNameMapFile (const wchar_t* mapFilePath,
                                      short fldCnt)
                                        :
                                      TcsCsvFileBase (true,fldCnt,fldCnt),
                                      CurrentRecord  (0U),
                                      Status         ()
{
    const wchar_t *wcPtrK;
    wchar_t *wcPtr;
    wchar_t extractedName [64];

    wcPtrK = wcsrchr (mapFilePath,L'\\');
    if (wcPtrK == 0)
    {
        wcPtrK = wcsrchr (mapFilePath,L'/');
    }
    if (wcPtrK == 0)
    {
        wcPtrK = mapFilePath;
    }
    else
    {
        wcPtrK++;
    }
    wcsncpy (extractedName,wcPtrK,wcCount (extractedName));
    extractedName [wcCount (extractedName) - 1] = L'\0';
    wcPtr = wcsrchr (extractedName,L'.');
    if (wcPtr != 0)
    {
        *wcPtr = L'\0';
    }
    std::wstring objectName (extractedName);
    SetObjectName (objectName);
    Status.SetObjectName (objectName);

// Seems gcc 3.2.2 wifstream::open requires an 8 bit character path.  NOT NICE!!!
char pathBufr [1024];
wcstombs (pathBufr,mapFilePath,sizeof (pathBufr));
std::wifstream iStrm (pathBufr,std::ios_base::in);
//  std::wifstream iStrm (mapFilePath,std::ios_base::in);
    if (iStrm.is_open ())
    {
        ReadFromStream (iStrm,true,Status);		//lint !e534  ignoring return value
    }
    else
    {
         Status.SetStatus (csvNoFile);
    }
    return;
}
TcsKeyNameMapFile::TcsKeyNameMapFile (const TcsKeyNameMapFile& source)
                                        :
                                      TcsCsvFileBase (source),
                                      CurrentRecord  (source.CurrentRecord),
                                      Status         (source.Status)
{
}
TcsKeyNameMapFile::~TcsKeyNameMapFile (void)
{
}
TcsKeyNameMapFile& TcsKeyNameMapFile::operator= (const TcsKeyNameMapFile& rhs)
{
    if (&rhs != this)
    {
        TcsCsvFileBase::operator= (rhs);
        CurrentRecord = rhs.CurrentRecord;
        Status        = rhs.Status;
    }
    return *this;
}
//=========================================================================
// Operator Overrides
//=========================================================================
// Public Named Member Functions
EcsMapTableFields TcsKeyNameMapFile::GetNbrFldId (EcsNameFlavor flavor) const
{
    EcsMapTableFields rtnValue = csMapFldUnknown;
    const TcsMapTableFields* tblPtr;
    
    for (tblPtr = KcsMapTableFields;tblPtr->FldId != csMapFldUnknown;tblPtr += 1)
    {
        if (tblPtr->IsNbr && tblPtr->Flavor == flavor)
        {
            rtnValue = tblPtr->FldId;
            break;
        }
    }
    return rtnValue;
}
EcsMapTableFields TcsKeyNameMapFile::GetNameFldId (EcsNameFlavor flavor) const
{
    EcsMapTableFields rtnValue = csMapFldUnknown;
    const TcsMapTableFields* tblPtr;
    
    for (tblPtr = KcsMapTableFields;tblPtr->FldId != csMapFldUnknown;tblPtr += 1)
    {
        if (!tblPtr->IsNbr && tblPtr->Flavor == flavor)
        {
            rtnValue = tblPtr->FldId;
            break;
        }
    }
    return rtnValue;
}
EcsCsvStatus TcsKeyNameMapFile::GetStatus (TcsCsvStatus& status) const
{
    status = Status;
    return Status.StatusValue;
}
unsigned TcsKeyNameMapFile::GetErrorValue (void) const
{
    return BogusNbr;
}
void TcsKeyNameMapFile::Rewind (void)
{
    CurrentRecord = 0UL;
}
bool TcsKeyNameMapFile::NextRecord (void)
{
    bool ok (true);
    
    if ((CurrentRecord < BogusNbr) && (CurrentRecord + 1U) < RecordCount ())
    {
        CurrentRecord += 1;
    }
    else
    {
        ok = false;
        Status.SetStatus (csvEof);
        Status.SetLineNbr (RecordCount ());
    }
    return ok;
}
bool TcsKeyNameMapFile::SetCurrentRecord (unsigned recordNbr)
{
    bool ok (true);
    
    if ((recordNbr < BogusNbr) && recordNbr < RecordCount ())
    {
        CurrentRecord = recordNbr;
    }
    else
    {
        ok = false;
        Status.SetStatus (csvInvRecordNbr);
        Status.SetLineNbr (recordNbr);
    }
    return ok;
}
bool TcsKeyNameMapFile::GetField (std::wstring& fieldValue,EcsMapTableFields fieldId) const
{
    std::wstring fieldName;
    
    bool ok = MapFieldIdToName (fieldName,fieldId);
    if (ok)
    {
        ok = TcsCsvFileBase::GetField (fieldValue,CurrentRecord,fieldName.c_str (),Status);
    }
    return ok;
}
unsigned long TcsKeyNameMapFile::GetFieldAsUL (EcsMapTableFields fieldId) const
{
    unsigned long rtnValue = BogusNbr;
    std::wstring fieldValue;
    
    bool ok = GetField (fieldValue,fieldId);
    if (ok)
    {
        rtnValue = wcstoul (fieldValue.c_str (),0,10);
    }
    return rtnValue;
}
void TcsKeyNameMapFile::GetFileRecordId (std::wstring& fileRecId) const
{
	wchar_t msgBufr [1024];
	size_t bufrLen = (sizeof (msgBufr)) / (sizeof (wchar_t));

    if ((CurrentRecord < BogusNbr) && (CurrentRecord < RecordCount ()))
    {
		unsigned long seqNbr = GetFieldAsUL (csMapFldSeqNbr);
		cs_swprintf (msgBufr,bufrLen,L"%.32s::%u::%lu",GetObjectName (),CurrentRecord,seqNbr);
		fileRecId = msgBufr;
	}
	else
	{
		fileRecId.clear ();
	}
}
bool TcsKeyNameMapFile::ReplaceField (EcsMapTableFields fieldId,const std::wstring& fieldValue)
{
    bool ok;
    std::wstring fieldName;
    
    ok = MapFieldIdToName (fieldName,fieldId);
    if (ok)
    {
        ok = TcsCsvFileBase::ReplaceField (fieldValue,CurrentRecord,fieldName.c_str (),Status);
    }
    return ok;
}
bool TcsKeyNameMapFile::ReplaceField (EcsMapTableFields fieldId,unsigned long fieldValue)
{
    bool ok (false);
    wchar_t wcArray [16];    
   
    if (fieldValue < BogusNbr)
    {
        cs_swprintf (wcArray,wcCount (wcArray),L"%lu",fieldValue);
        std::wstring tmpWstr (wcArray);
        ok = ReplaceField (fieldId,tmpWstr);
    }
    else
    {
        Status.SetStatus (csvBogusValue);
    }
    return ok;
}
bool TcsKeyNameMapFile::MapFieldIdToName (std::wstring& fieldName,EcsMapTableFields fieldId) const
{
    bool ok = true;
    
    const TcsMapTableFields *tblPtr;
    
    for (tblPtr = KcsMapTableFields;tblPtr->FldId != csMapFldUnknown;tblPtr += 1)
    {
        if (tblPtr->FldId == fieldId)
        {
            break;
        }
    }
    if (tblPtr->FldId != csMapFldUnknown)
    {
        fieldName = std::wstring (tblPtr->FldName);
    }
    else
    {
        ok = false;
        Status.SetStatus (csvInvFieldId);
    }
    return ok;
}
//=============================================================================
// The following rather simple functions replace the need for the sizeable
// MappingMsiToAdsk module, and the equally sizeable CS_epsg.c modules.
//=============================================================================
const char* CSepsg2adskCS (long epsgNbr)
{
    /* NOTE ** static array could cause multi-thread problems */
    static char staticBufr [256];

    enum EcsMapSt csMapSt;

    staticBufr [0] = '\0';
    csMapSt = csMapIdToNameC (csMapProjGeoCSys,staticBufr,sizeof (staticBufr),
                                                          csMapFlvrAutodesk,
                                                          csMapFlvrEpsg,
                                                          static_cast<unsigned long>(epsgNbr));
    return (csMapSt == csMapOk) ? staticBufr : 0;
}
//=============================================================================
long CSadsk2epsgCS (const char *adskKeyName)
{
    unsigned long ulEpsg;

    ulEpsg = csMapNameToIdC (csMapProjGeoCSys,csMapFlvrEpsg,csMapFlvrAutodesk,adskKeyName);
    if (ulEpsg == KcsNmInvNumber)
    {
        ulEpsg = 0UL;
    }
    return static_cast<long>(ulEpsg);
}
