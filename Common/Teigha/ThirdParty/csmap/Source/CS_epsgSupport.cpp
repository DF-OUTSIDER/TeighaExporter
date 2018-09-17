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
//lint -esym(534,TcsCsvFileBase::SetMinFldCnt,TcsCsvFileBase::SetMaxFldCnt)  ignoring retun value
//lint -esym(1536,TcsEpsgTable::CsvStatus)              exposing low access member
//lint -esym(766,..\Include\cs_wkt.h)                   unreferenced header, needed for pre-compiled headers)
//lint -esym(1927,TcsEpsgCode::EpsgCode)                not initialized in formal initializer list.
//lint -e514                                            unusual use of boolean expression (e.g. ok &= )

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include "cs_Legacy.h"
#include "cs_EpsgStuff.h"

//=============================================================================
// TcsEpsgCode  --  Distinct type for an EPSG code value.
//
// For function overloading and parameter safety, its very nice to have a
// distinct type for the EPSG code value.
//=============================================================================
// Static Constants, Variables, and Member Functions
const unsigned long TcsEpsgCode::InvalidValue = 0UL;
//=============================================================================
// Construction, Destruction, & Assignment
TcsEpsgCode::TcsEpsgCode () : EpsgCode (InvalidValue)
{
}
TcsEpsgCode::TcsEpsgCode (unsigned long epsgCode) : EpsgCode (epsgCode)
{
}
TcsEpsgCode::TcsEpsgCode (unsigned int epsgCode)
{
	EpsgCode = static_cast <unsigned long>(epsgCode);
}
TcsEpsgCode::TcsEpsgCode (const wchar_t* epsgCode)
{
	EpsgCode = StrToEpsgCode (epsgCode);
}
TcsEpsgCode::TcsEpsgCode (const std::wstring& epsgCode)
{
	EpsgCode = StrToEpsgCode (epsgCode.c_str ());
}
TcsEpsgCode::TcsEpsgCode (const TcsEpsgCode& source) : EpsgCode (source.EpsgCode)
{
}
TcsEpsgCode::~TcsEpsgCode ()
{
}
TcsEpsgCode& TcsEpsgCode::operator= (const TcsEpsgCode& rhs)
{
	EpsgCode = rhs.EpsgCode;
	return *this;
}
TcsEpsgCode& TcsEpsgCode::operator= (unsigned long epsgCode)
{
	EpsgCode = epsgCode;
	return *this;
}
//=============================================================================
// Operator Overrides
bool TcsEpsgCode::operator< (unsigned long epsgCode) const
{
	bool lessThan = (EpsgCode < epsgCode);
	return lessThan;
}
bool TcsEpsgCode::operator< (const std::wstring& epsgCode) const
{
	TcsEpsgCode tmpEpsgCode (epsgCode);
	bool lessThan = (EpsgCode < tmpEpsgCode);
	return lessThan;
}
bool TcsEpsgCode::operator== (unsigned long epsgCode) const
{
	bool equal = (EpsgCode == epsgCode);
	return equal;
}
bool TcsEpsgCode::operator== (unsigned int epsgCode) const
{
	bool equal = (EpsgCode == static_cast<unsigned long>(epsgCode));
	return equal;
}
bool TcsEpsgCode::operator== (const std::wstring& epsgCode) const
{
	TcsEpsgCode tmpEpsgCode (epsgCode);
	bool equal = (EpsgCode == tmpEpsgCode);
	return equal;
}
bool TcsEpsgCode::operator> (unsigned long epsgCode) const
{
	bool greaterThan = (EpsgCode > epsgCode);
	return greaterThan;
}
bool TcsEpsgCode::operator> (const std::wstring& epsgCode) const
{
	TcsEpsgCode tmpEpsgCode (epsgCode);
	bool greaterThan = (EpsgCode > tmpEpsgCode);
	return greaterThan;
}
TcsEpsgCode TcsEpsgCode::operator++ ()
{
	EpsgCode += 1;
	return EpsgCode;
}
TcsEpsgCode TcsEpsgCode::operator++ (int)
{
	unsigned long prevValue = EpsgCode;
	EpsgCode += 1;
	return prevValue;
}
TcsEpsgCode TcsEpsgCode::operator-- ()
{
	EpsgCode -= 1;
	return EpsgCode;
}
TcsEpsgCode TcsEpsgCode::operator-- (int)
{
	unsigned long prevValue = EpsgCode;
	EpsgCode -= 1;
	return prevValue;
}
TcsEpsgCode& TcsEpsgCode::operator+= (unsigned long rhs)
{
	EpsgCode += rhs;
	return *this;
}
TcsEpsgCode& TcsEpsgCode::operator-= (unsigned long rhs)
{
	EpsgCode -= rhs;
	return *this;
}
TcsEpsgCode& TcsEpsgCode::operator+= (int rhs)
{
	unsigned long tmpRhs;

	if (rhs < 0)
	{
		tmpRhs = static_cast<unsigned long>(-rhs);
		EpsgCode -= tmpRhs;
	}
	else
	{
		tmpRhs = static_cast<unsigned long>(rhs);
		EpsgCode += tmpRhs;
	}
	return *this;
}
TcsEpsgCode& TcsEpsgCode::operator-= (int rhs)
{
	unsigned long tmpRhs;

	if (rhs < 0)
	{
		tmpRhs = static_cast<unsigned long>(-rhs);
		EpsgCode += tmpRhs;
	}
	else
	{
		tmpRhs = static_cast<unsigned long>(rhs);
		EpsgCode -= tmpRhs;
	}
	return *this;
}
TcsEpsgCode TcsEpsgCode::operator+ (unsigned long rhs) const
{
	unsigned long sum = EpsgCode + rhs;
	return TcsEpsgCode (sum);
}
TcsEpsgCode TcsEpsgCode::operator- (unsigned long rhs) const
{
	unsigned long difference = EpsgCode - rhs;
	return TcsEpsgCode (difference);
}
TcsEpsgCode TcsEpsgCode::operator+ (int rhs) const
{
	unsigned long sum = EpsgCode + rhs;
	return TcsEpsgCode (sum);
}
TcsEpsgCode TcsEpsgCode::operator- (int rhs) const
{
	unsigned long difference = EpsgCode - rhs;
	return TcsEpsgCode (difference);
}
//=============================================================================
// Public Named Member Functions
std::wstring TcsEpsgCode::AsWstring () const
{
	wchar_t* wcPtr;
	wchar_t wcArray [32];

	if (EpsgCode > 1991000UL)
	{
		// Appears to be a change ID.
		double realValue = (static_cast<double>(EpsgCode) / 1000.0);
		cs_swprintf (wcArray,wcCount (wcArray),L"%.3f",realValue);
		wcPtr = wcArray + wcslen (wcArray) - 1;
		if (*wcPtr == L'0') *--wcPtr = L'\0';
		if (*wcPtr == L'0') *--wcPtr = L'\0';
	}
	else
	{
		cs_swprintf (wcArray,wcCount (wcArray),L"%lu",EpsgCode);
	}
	return std::wstring (wcArray);
}
std::string TcsEpsgCode::AsString () const
{
	char* cPtr;
	char cArray [32];

	if (EpsgCode > 1991000UL)
	{
		// Appears to be a change ID.
		double realValue = (static_cast<double>(EpsgCode) / 1000.0);
		sprintf (cArray,"%.3f",realValue);
		cPtr = cArray + strlen (cArray) - 1;
		if (*cPtr == '0') *--cPtr = '\0';
		if (*cPtr == '0') *--cPtr = '\0';
	}
	else
	{
		sprintf (cArray,"%lu",EpsgCode);
	}
	return std::string (cArray);
}
bool TcsEpsgCode::AsString (wchar_t* result,size_t resultSize) const
{
	std::wstring tmpEpsgCode = AsWstring ();
	wcsncpy (result,tmpEpsgCode.c_str (),resultSize);
	result [resultSize - 1] = L'\0';
	return true;
}
bool TcsEpsgCode::AsString (char* result,size_t resultSize) const
{
	std::string tmpEpsgCode = AsString ();
	strncpy (result,tmpEpsgCode.c_str (),resultSize);
	result [resultSize - 1] = '\0';
	return true;
}
//=============================================================================
// Protected Member Functions
unsigned long TcsEpsgCode::StrToEpsgCode (const wchar_t* epsgCodeStr) const
{
	const wchar_t* wcPtr;
	TcsEpsgCode epsgCodeNbr (0UL);

	wcPtr = wcschr (epsgCodeStr,L'.');
	if (wcPtr == 0)
	{
		// Appears to be a normal code value.
		epsgCodeNbr = wcstoul (epsgCodeStr,0,10);
	}
	else
	{
		// The string appears to be a change ID.
		double realValue = wcstod (epsgCodeStr,0);
		realValue *= 1000;
		epsgCodeNbr = static_cast<unsigned long>(realValue);
	}
	return epsgCodeNbr;
}
unsigned long TcsEpsgCode::StrToEpsgCode (const char* epsgCodeStr) const
{
	const char* cPtr;
	TcsEpsgCode epsgCodeNbr (0UL);

	cPtr = strchr (epsgCodeStr,'.');
	if (cPtr == 0)
	{
		// Appears to be a normal code value.
		epsgCodeNbr = strtoul (epsgCodeStr,0,10);
	}
	else
	{
		// The string appears to be a change ID.
		double realValue = strtod (epsgCodeStr,0);
		realValue *= 1000;
		epsgCodeNbr = static_cast<unsigned long>(realValue);
	}
	return epsgCodeNbr;
}
//newPage//
//=============================================================================
// TcsEpsgTable  --  Customization of TcsCsvTableBase for EPSG Tables
//=============================================================================
// Static Constants, Variables, and Member Functions
const wchar_t TcsEpsgTable::LogicalTrue  [] = L"TRUE";
const wchar_t TcsEpsgTable::LogicalFalse [] = L"FALSE";

bool TcsEpsgTable::IsLogicalTrue (const wchar_t* logicalValue)
{
	wchar_t lclValue [16];

	wcsncpy (lclValue,logicalValue,wcCount (lclValue));
	lclValue [wcCount (lclValue) - 1] = L'\0';
	CS_trimWc (lclValue);
	int cmpValue = CS_wcsicmp (lclValue,LogicalTrue);
	return (cmpValue == 0);
}
bool TcsEpsgTable::IsLogicalFalse (const wchar_t* logicalValue)
{
	wchar_t lclValue [16];

	wcsncpy (lclValue,logicalValue,wcCount (lclValue));
	lclValue [wcCount (lclValue) - 1] = L'\0';
	CS_trimWc (lclValue);
	int cmpValue = CS_wcsicmp (lclValue,LogicalFalse);
	return (cmpValue == 0);
}
//=============================================================================
// Construction, Destruction, & Assignment
TcsEpsgTable::TcsEpsgTable (const TcsEpsgTblMap& tblMap,const wchar_t* databaseFldr)
															:
														TcsCsvFileBase   (true,2,35),
														Ok               (false),
														Sorted           (false),
														Indexed          (false),
														TableId          (epsgTblNone),
														CodeKeyField     (epsgFldNone),
														SortFunctor      (-1),
														CodeIndex        (),
														CsvStatus        ()
{
	// Seems gcc 3.2.2 wifstream::open requires an 8 bit character path.  NOT NICE!!!
	char pathBufr [1024];
	// Set the min and max fild counts.
	TableId = tblMap.TableId;
	CodeKeyField = tblMap.CodeKeyFieldId;
	SetMinFldCnt (tblMap.FieldCount);
	SetMaxFldCnt (tblMap.FieldCount);

	SortFunctor.FirstField  = GetEpsgFieldNumber (TableId,tblMap.Sort1);
	SortFunctor.SecondField = GetEpsgFieldNumber (TableId,tblMap.Sort2);
	SortFunctor.ThirdField  = GetEpsgFieldNumber (TableId,tblMap.Sort3);
	SortFunctor.FourthField = GetEpsgFieldNumber (TableId,tblMap.Sort4);
	std::wstring objName (tblMap.TableName);
	objName += L" Table";
	SetObjectName (objName);
	
	std::wstring filePath (databaseFldr);
	filePath += L"/";
	filePath += tblMap.TableName;
	filePath += L".csv";
	wcstombs (pathBufr,filePath.c_str (),sizeof (pathBufr));
	std::wifstream iStrm (pathBufr,std::ios_base::in);
	if (iStrm.is_open ())
	{
		// My Linux box (CentOS 6.3) requires a locale specification in order
		// to read the EPSG .csv files.  Windows needs two parameters to this
		// function call; best I can tell, the C++ standard says one should
		// be sufficient.  Anyway, the following seems to work on my systems.
#if (_RUN_TIME < _rt_UNIXPCC)
		std::locale epsgLocale("en_US",LC_ALL);
#else
		std::locale epsgLocale("en_US");
#endif
		iStrm.imbue(epsgLocale);		//lint !e534  ignoring return value
		Ok = ReadFromStream (iStrm,true,CsvStatus);
	}
	if (Ok)
	{
		Ok = PrepareCsvFile ();
	}
}
TcsEpsgTable::TcsEpsgTable (const TcsEpsgTable& source) : TcsCsvFileBase   (source),
														  Ok               (source.Ok),
														  Sorted           (source.Sorted),
														  Indexed          (source.Indexed),
														  TableId          (source.TableId),
														  CodeKeyField     (source.CodeKeyField),
														  SortFunctor      (source.SortFunctor),
														  CodeIndex        (source.CodeIndex),
														  CsvStatus        (source.CsvStatus)
{
}
TcsEpsgTable::~TcsEpsgTable (void)
{
	// Nothing to do here (yet).
}
TcsEpsgTable& TcsEpsgTable::operator= (const TcsEpsgTable& rhs)
{
	if (&rhs != this)
	{
		TcsCsvFileBase::operator= (rhs);
		Ok               = rhs.Ok;
		Sorted           = rhs.Sorted;
		Indexed          = rhs.Indexed;
		TableId          = rhs.TableId;
		CodeKeyField     = rhs.CodeKeyField;
		SortFunctor      = rhs.SortFunctor;
		CodeIndex        = rhs.CodeIndex;
		CsvStatus        = rhs.CsvStatus;
	}
	return *this;
}
unsigned TcsEpsgTable::LocateRecordByEpsgCode (const TcsEpsgCode& epsgCode) const
{
	bool ok;
	short fieldNbr;
	unsigned recordNumber (InvalidRecordNbr);
	wchar_t srchString [32];

	ok = epsgCode.IsValid ();
	if (ok)
	{
		bool found (false);
		if (Indexed)
		{
		    std::map<TcsEpsgCode,unsigned>::const_iterator findItr;
	        findItr = CodeIndex.find (epsgCode);
			if (findItr != CodeIndex.end ())
			{
				recordNumber = findItr->second;
				found = true;
			}
		}
		if (!found)
		{
			// The table is not indexed, or for some reason the index lookup
			// failed.  Do a brute force linear search.  This is not supposed
			// to happen.
			epsgCode.AsString (srchString,wcCount (srchString));
			fieldNbr = GetEpsgFieldNumber (TableId,CodeKeyField);
			if (fieldNbr >= 0)
			{
				ok = Locate (recordNumber,fieldNbr,srchString);
			}
		}
	}
	return recordNumber;
}
// Note that the following function returns the code associated with the first
// occurence of the specified locator value.  This may or may not be what you
// want.  Take advantage of the convenience of this function only in cases
// where the locator value can be assumed to be unique within the
// non-deprecated table entries.
bool TcsEpsgTable::EpsgLocateCode (TcsEpsgCode& epsgCode,EcsEpsgField fieldId,const wchar_t* fldValue) const
{
	bool ok;
	bool deprecated;
	std::wstring fldData;

	// Prepare for an error of some sort.
	epsgCode = 0UL;

	// The field containing the EPSG code is almost always field number 0, but
	// the actual EcsEpsgField of the EPSG code varies from one table to
	// another.
	EcsEpsgField codeFldId = GetEpsgCodeFieldId (TableId);

	// We don't really need the fieldNbr, but we do the following to verify
	// that the provided fieldId is valid for the current table.
	short fldNbr = GetEpsgFieldNumber (TableId,fieldId);
	ok = (fldNbr >= 0);

	// OK, we search the table for the provided search value, skipping
	// all deprecated entries.
	deprecated = false;
	unsigned recCnt = RecordCount ();
	for (unsigned recNbr = 0; ok && recNbr < recCnt;++recNbr)
	{
		// epsgFldDeprecated is defined for all tables.  Note, if
		// this fails, we assume that the record is not deprecated
		// and continue with the search.
		if (GetField (fldData,recNbr,epsgFldDeprecated))
		{
			deprecated = (CS_wcsicmp (fldData.c_str (),LogicalTrue) == 0);
			if (deprecated)
			{
				continue;
			}
		}
		ok = GetField (fldData,recNbr,fieldId);
		if (ok)
		{
			if (CS_wcsicmp (fldData.c_str (),fldValue) == 0)
			{
				ok = GetField (fldData,recNbr,codeFldId);
				if (ok)
				{
					epsgCode = TcsEpsgCode (fldData);
					break;
				}
			}
		}
	}
	return (epsgCode.IsValid ());
}
// LocateFirst functions presume the field ID is not the index field and not
// the primary sort key.  That is, these 'LocateFirst' functions do a linear
// search of the whole table.
unsigned TcsEpsgTable::EpsgLocateFirst (EcsEpsgField fieldId,const wchar_t* fldValue,bool honorCase) const
{
	unsigned recordNumber (InvalidRecordNbr);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	bool ok = (fieldNbr >= 0);
	if (ok)
	{
		ok = Locate (recordNumber,fieldNbr,fldValue,honorCase);
	}
	return recordNumber;
}
unsigned TcsEpsgTable::EpsgLocateFirst (EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const
{
	std::wstring epsgCodeStr (epsgCode.AsWstring ());
	unsigned recordNumber = EpsgLocateFirst (fieldId,epsgCodeStr.c_str (),false);
	return recordNumber;
}
unsigned TcsEpsgTable::EpsgLocateNext (unsigned startAfter,EcsEpsgField fieldId,const wchar_t* fldValue,bool honorCase) const
{
	unsigned recordNumber (InvalidRecordNbr);
	unsigned myRecordNbr (startAfter);
	
	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	bool ok = (fieldNbr >= 0);
	if (ok)
	{
		ok = LocateNext (myRecordNbr,fieldNbr,fldValue,honorCase);
		if (ok)
		{
			recordNumber = myRecordNbr;
		}
	}
	return recordNumber;
}
unsigned TcsEpsgTable::EpsgLocateNext (unsigned startAfter,EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const
{
	std::wstring epsgCodeStr (epsgCode.AsWstring ());
	unsigned recordNumber = EpsgLocateNext (startAfter,fieldId,epsgCodeStr.c_str (),false);
	return recordNumber;
}
bool TcsEpsgTable::IsDeprecated (const TcsEpsgCode& epsgCode) const
{
	bool ok;
	bool deprecated (false);
	std::wstring fldData;

	ok = GetField (fldData,epsgCode,epsgFldDeprecated);
	if (ok)
	{
		deprecated = IsLogicalTrue (fldData.c_str ());
	}
	return deprecated;
}
bool TcsEpsgTable::IsDeprecated (unsigned recNbr) const
{
	bool ok;
	bool deprecated (false);
	std::wstring fldData;

	ok = GetField (fldData,recNbr,epsgFldDeprecated);
	if (ok)
	{
		deprecated = IsLogicalTrue (fldData.c_str ());
	}
	return deprecated;
}
bool TcsEpsgTable::GetField (std::wstring& result,const TcsEpsgCode& epsgCode,short fieldNbr) const
{
	bool ok (false);
	unsigned recordNumber (InvalidRecordNbr);
	TcsCsvStatus csvStatus;

	result.clear ();
	recordNumber = LocateRecordByEpsgCode (epsgCode);
	if (recordNumber != InvalidRecordNbr)
	{
		ok = TcsCsvFileBase::GetField (result,recordNumber,fieldNbr,csvStatus);
	}
	return ok;
}
bool TcsEpsgTable::GetAsLong (long& result,const TcsEpsgCode& epsgCode,short fieldNbr) const
{
	bool ok (false);
	long myResult (0L);
	wchar_t *endPtr;
	std::wstring fldValue;

	ok = GetField (fldValue,epsgCode,fieldNbr);
	if (ok)
	{
		myResult = wcstol (fldValue.c_str (),&endPtr,10);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	if (ok)
	{
		result = myResult;
	}
	return ok;
}
bool TcsEpsgTable::GetAsULong (unsigned long& result,const TcsEpsgCode& epsgCode,short fieldNbr) const
{
	bool ok (false);
	unsigned long myResult;
	wchar_t *endPtr;
	std::wstring fldValue;

	myResult = 0UL;
	ok = GetField (fldValue,epsgCode,fieldNbr);
	if (ok)
	{
		myResult = wcstoul (fldValue.c_str (),&endPtr,10);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	result = myResult;
	return ok;
}
bool TcsEpsgTable::GetAsEpsgCode (TcsEpsgCode& result,const TcsEpsgCode& epsgCode,short fieldNbr) const
{
	bool ok (false);
	TcsEpsgCode myResult;
	std::wstring fldValue;

	ok = GetField (fldValue,epsgCode,fieldNbr);
	if (ok)
	{
		myResult = TcsEpsgCode (fldValue.c_str ());
		ok = myResult.IsValid ();
	}
	if (ok)
	{
		result = myResult;
	}
	return ok;
}
bool TcsEpsgTable::GetAsReal (double& result,const TcsEpsgCode& epsgCode,short fieldNbr) const
{
	bool ok (false);
	double myResult (0.0);
	wchar_t *endPtr;
	std::wstring fldValue;

	ok = GetField (fldValue,epsgCode,fieldNbr);
	if (ok)
	{
		myResult = wcstod (fldValue.c_str (),&endPtr);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	if (ok)
	{
		result = myResult;
	}
	return ok;
}
bool TcsEpsgTable::GetField (std::wstring& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const
{
	bool ok (false);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	if (fieldNbr >= 0)
	{
		ok = GetField (result,epsgCode,fieldNbr);
	}
	return ok;
}
bool TcsEpsgTable::GetAsLong (long& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const
{
	bool ok (false);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	if (fieldNbr >= 0)
	{
		ok = GetAsLong (result,epsgCode,fieldNbr);
	}
	return ok;
}
bool TcsEpsgTable::GetAsULong (unsigned long& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const
{
	bool ok (false);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	if (fieldNbr >= 0)
	{
		ok = GetAsULong (result,epsgCode,fieldNbr);
	}
	return ok;
}
bool TcsEpsgTable::GetAsEpsgCode (TcsEpsgCode& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const
{
	bool ok (false);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	if (fieldNbr >= 0)
	{
		ok = GetAsEpsgCode (result,epsgCode,fieldNbr);
	}
	return ok;
}
bool TcsEpsgTable::GetAsReal (double& result,const TcsEpsgCode& epsgCode,EcsEpsgField fieldId) const
{
	bool ok (false);

	short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
	if (fieldNbr >= 0)
	{
		ok = GetAsReal (result,epsgCode,fieldNbr);
	}
	return ok;
}
bool TcsEpsgTable::GetField (std::wstring& result,unsigned int recNbr,EcsEpsgField fieldId) const
{
	bool ok (false);
	TcsCsvStatus status;

	ok = (recNbr < RecordCount ()); 
	if (ok)
	{
		short fieldNbr = GetEpsgFieldNumber (TableId,fieldId);
		ok = (fieldNbr >= 0);
		if (ok)
		{
			ok = TcsCsvFileBase::GetField (result,recNbr,fieldNbr,status);
		}
	}
	return ok;
}
bool TcsEpsgTable::GetAsLong (long& result,unsigned recNbr,EcsEpsgField fieldId) const
{
	bool ok (false);
	long myResult;
	wchar_t *endPtr;
	std::wstring fldValue;

	myResult = 0L;
	ok = GetField (fldValue,recNbr,fieldId);
	if (ok)
	{
		myResult = wcstol (fldValue.c_str (),&endPtr,10);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	result = myResult;
	return ok;
}
bool TcsEpsgTable::GetAsULong (unsigned long& result,unsigned recNbr,EcsEpsgField fieldId) const
{
	bool ok (false);
	unsigned long myResult (0UL);
	wchar_t *endPtr;
	std::wstring fldValue;

	ok = GetField (fldValue,recNbr,fieldId);
	if (ok)
	{
		myResult = wcstoul (fldValue.c_str (),&endPtr,10);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	result = myResult;
	return ok;
}
bool TcsEpsgTable::GetAsEpsgCode (TcsEpsgCode& result,unsigned recNbr,EcsEpsgField fieldId) const
{
	bool ok (false);
	TcsEpsgCode myResult (0UL);
	std::wstring fldValue;

	ok = GetField (fldValue,recNbr,fieldId);
	if (ok)
	{
		myResult = TcsEpsgCode (fldValue.c_str ());
		ok = myResult.IsValid ();
	}
	result = myResult;
	return ok;
}
bool TcsEpsgTable::GetAsReal (double& result,unsigned recNbr,EcsEpsgField fieldId) const
{
	bool ok (false);
	double myResult (0.0);
	wchar_t *endPtr;
	std::wstring fldValue;

	ok = GetField (fldValue,recNbr,fieldId);
	if (ok)
	{
		myResult = wcstod (fldValue.c_str (),&endPtr);
		while (iswspace (*endPtr)) endPtr += 1;
		ok = (*endPtr == L'\0');
	}
	result = myResult;
	return ok;
}
TcsCsvStatus& TcsEpsgTable::GetCsvStatus ()
{
	return CsvStatus;
}
const TcsCsvStatus& TcsEpsgTable::GetCsvStatus () const
{
    return CsvStatus;
}

//=============================================================================
// Private Support Functions
bool TcsEpsgTable::PrepareCsvFile ()
{
	bool ok (true);

	if (SortFunctor.FirstField >= 0)
	{
		// We have a sort specification for this table, so we sort it.
		Sorted = StableSort (SortFunctor);
		ok = Sorted;
	}
	if (ok && CodeKeyField != epsgFldNone)
	{
		// The CodeKeyField element of the "Tables" table indicates that there
		// is a unique key field for this table.  This is present only in the
		// case where there is a unique primary key field (it might not be
		// an Access primary key) and the type of that field is essentially
		// an unsigned long.  In this case, we build an index which consists
		// of the the code value (usually the EPSG code, not always) and the
		// record number of the value in the field.  We don't use the CSV
		// file index, as that is a string to record number index.  Our code
		// to record number index is a lot faster. 
		short fieldNbr = GetEpsgFieldNumber (TableId,CodeKeyField);
		ok = BuildEpsgIndex (fieldNbr,CsvStatus);
		Indexed = ok;
	}
	return ok;
}
bool TcsEpsgTable::BuildEpsgIndex (short fldNbr,TcsCsvStatus& csvStatus)
{
    bool ok = true;
    unsigned recNbr = 0;
    unsigned recCnt;

    std::wstring keyValue;
    std::pair<std::map<TcsEpsgCode,unsigned>::iterator,bool> idxStatus; 

    CodeIndex.clear ();
    recCnt = RecordCount ();
    for (recNbr = 0;recNbr < recCnt;++recNbr)
    {
		ok = TcsCsvFileBase::GetField (keyValue,recNbr,fldNbr,csvStatus);
		if (ok)
		{
			TcsEpsgCode recCode (keyValue);
            idxStatus = CodeIndex.insert (std::make_pair (recCode,recNbr));
            ok = idxStatus.second;
            if (!ok)
            {
                csvStatus.SetStatus (csvDupIndex);
                csvStatus.SetFieldNbr (KeyField);
                csvStatus.SetFieldId (keyValue);
                csvStatus.SetLineNbr (recNbr + FirstIsLabels);
                break;
            }
        }
        else
        {
			csvStatus.SetStatus (csvInvFieldNbr);
			csvStatus.SetFieldNbr (fldNbr);
			csvStatus.SetFieldId (keyValue);
			csvStatus.SetLineNbr (recNbr + FirstIsLabels);
        }
    }
    if (!ok)
    {
		// If the index didn't get created successfully, we clear it to
		// release any resources it may have consumed.
		CodeIndex.clear ();
    }
    return ok;
}
//newPage//
//=============================================================================
// TcsOpSingle -- A non-concatenated operation
//
// TcsOpSingle represents a specific operation in the EPSG Coordinate Operation
// table which is not concatenated.  That is, it represents a fundamental
// operation.
//
//=============================================================================
// Static Constants, Variables, and Member Functions
//=============================================================================
// Construction  /  Destruction  /  Assignment
TcsSingleOp::TcsSingleOp (const	TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& operationCode)
															:
														 Null            (false),
														 LngXlation      (false),
														 Type            (epsgOpTypNone),
														 SourceCRS       (),
														 TargetCRS       (),
														 OperationCode   (),
														 OperationMethod (),
														 Accuracy        (),
														 OpName          ()
{
	bool ok (false);

	// Get the table pointer, then extract the memebers.  We then classify the
	// operation with regard to NULL, and longitude translation.
	const TcsEpsgTable* oprtnTblPtr = epsgDB.GetTablePtr (epsgTblCoordinateOperation);
	ok = (oprtnTblPtr != 0);
	if (ok)
	{
		std::wstring fldData;
		ok  = oprtnTblPtr->GetField (fldData,operationCode,epsgFldCoordOpType);
		if (ok)
		{
			Type = GetEpsgOpType (fldData.c_str ());
			ok = (Type == epsgOpTypConversion || Type == epsgOpTypTransformation);
		}
	}
	if (ok)
	{
		ok  = oprtnTblPtr->GetAsEpsgCode (SourceCRS,operationCode,epsgFldSourceCrsCode);
		ok &= oprtnTblPtr->GetAsEpsgCode (TargetCRS,operationCode,epsgFldTargetCrsCode);
		ok &= oprtnTblPtr->GetAsEpsgCode (TargetCRS,operationCode,epsgFldTargetCrsCode);
		ok &= oprtnTblPtr->GetAsEpsgCode (OperationMethod,operationCode,epsgFldCoordOpMethodCode);
		ok &= oprtnTblPtr->GetAsReal (Accuracy,operationCode,epsgFldCoordOpAccuracy);
	}
	if (ok)
	{
		ok  = Classify (epsgDB,operationCode);
		ok &= oprtnTblPtr->GetField (OpName,operationCode,epsgFldCoordOpName);
		if (ok)
		{
			OperationCode = operationCode;
		}
	}
}
TcsSingleOp::TcsSingleOp (const TcsSingleOp& source) : Null            (source.Null),
													   LngXlation      (source.LngXlation),
													   Type            (source.Type),
													   SourceCRS       (source.SourceCRS),
													   TargetCRS       (source.TargetCRS),
													   OperationCode   (source.OperationCode),
													   OperationMethod (source.OperationMethod),
													   Accuracy        (source.Accuracy),
													   OpName          (source.OpName)
{
}
TcsSingleOp::~TcsSingleOp (void)
{
	// Nothing to do here.
}
TcsSingleOp& TcsSingleOp::operator= (const TcsSingleOp& rhs)
{
	if (&rhs != this)
	{
		Null            = rhs.Null;
		LngXlation      = rhs.LngXlation;
		Type            = rhs.Type;
		SourceCRS       = rhs.SourceCRS;
		TargetCRS       = rhs.TargetCRS;
		OperationCode   = rhs.OperationCode;
		OperationMethod = rhs.OperationMethod;
		Accuracy        = rhs.Accuracy;
		OpName          = rhs.OpName;
	}
	return *this;
}
//=============================================================================
// Operator Overrides
//=============================================================================
// Public Named Member Functions
//=============================================================================
// Private Support Member Functions
bool TcsSingleOp::Classify (const TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& oprtnCode)
{
//lint -save -esym(644,deltaY,deltaZ)		(may not be initialized)
	bool ok (true);

	Null = false;
	LngXlation = (OperationMethod == 9601UL);

	if (OperationMethod == 9603UL)
	{
		double deltaX (0.0);
		double deltaY (0.0);
		double deltaZ (0.0);
		TcsEpsgCode uomCode;

		uomCode = epsgDB.GetParameterValue (deltaX,oprtnCode,OperationMethod,8605UL,9001UL);
		ok = uomCode.IsValid ();
		if (ok)
		{
			uomCode = epsgDB.GetParameterValue (deltaY,oprtnCode,OperationMethod,8606UL,9001UL);
			ok = uomCode.IsValid ();
		}
		if (ok)
		{
			uomCode = epsgDB.GetParameterValue (deltaZ,oprtnCode,OperationMethod,8607UL,9001UL);
			ok = uomCode.IsValid ();
		}
		if (ok)
		{
			// If this is a Geocentric Translation, and all three translation
			// parameters are zero, it is the NULL datum transformation.
			Null = (fabs (deltaX) < 1.0E-10) && (fabs (deltaY) < 1.0E-10) && (fabs (deltaZ) < 1.0E-10);
		}
	}
	return ok;
//lint -restore
}
//newPage//
//=========================================================================
// TcsOpVariant -- A collection of one or more TcsOpSingle objects.
//
// This object represents an entry in the EPSG Coordinate Operation table.
// In the normal case, there is a single TcsOpSingle operation in the
// container.  In the case of a coordinate operation of the concatenated
// type, a TcsOpSingle object will be created and added to the container
// for each operation in the "concatenated" operation definition.
//=========================================================================
// Static Constants, Variables, and Member Functions
//=========================================================================
// Construction  /  Destruction  /  Assignment
TcsOpVariant::TcsOpVariant (const TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& operationCode)
																:
														   Type        (epsgOpTypNone),
														   EpsgOpCode  (operationCode),
														   VariantNbr  (0U),
														   Version     (),
														   Accuracy    (),
														   VariantName (),
														   Singles     ()
{
//lint -save -esym(613,copTblPtr) -esym(644,opType)
	bool ok (false);
	bool deprecated (false);

	EcsOpType opType (epsgOpTypNone);
	TcsEpsgCode codeSingle;
	std::wstring fldData;

	unsigned invalidRecNbr = TcsCsvFileBase::GetInvalidRecordNbr ();
	const TcsEpsgTable* copTblPtr = epsgDB.GetTablePtr (epsgTblCoordinateOperation);
	const TcsEpsgTable* pthTblPtr = epsgDB.GetTablePtr (epsgTblOperationPath);
	ok = (copTblPtr != 0) && (pthTblPtr != 0);
	if (ok)
	{
		deprecated = copTblPtr->IsDeprecated (operationCode);
		ok = !deprecated;
	}
	if (ok)
	{
		ok &= copTblPtr->GetField (fldData,operationCode,epsgFldCoordOpType);
		if (ok)
		{
			opType = GetEpsgOpType (fldData.c_str ());
			ok = (opType != epsgOpTypUnknown);
		}
	}
	if (ok)
	{
		if (opType == epsgOpTypTransformation || opType == epsgOpTypConversion)
		{
			TcsSingleOp singleOp (epsgDB,operationCode);
			if (singleOp.IsValid ())
			{
				Singles.push_back (singleOp);
			}
		}
		else
		{
			unsigned long lastStep (0UL);
			unsigned recNbr;

			// Must be a concatenated thing.  Need to examine the Path
			// table and extract each of the single Op's separately.  Note,
			// we rely on the proper sorted order here.
			recNbr = pthTblPtr->EpsgLocateFirst (epsgFldConcatOperationCode,operationCode);
			ok = (recNbr != invalidRecNbr);
			while (recNbr != invalidRecNbr)
			{
				unsigned long step;
				ok  = pthTblPtr->GetAsEpsgCode (codeSingle,recNbr,epsgFldSingleOperationCode);
				ok &= pthTblPtr->GetAsULong (step,recNbr,epsgFldOpPathStep);
				ok &= (step > lastStep);
				if (ok)
				{
					TcsSingleOp singleOp (epsgDB,codeSingle);
					ok = singleOp.IsValid ();
					if (ok)
					{
						Singles.push_back (singleOp);
					}
				}
				if (ok)
				{
					recNbr = pthTblPtr->EpsgLocateNext (recNbr,epsgFldConcatOperationCode,operationCode);
				}
				else
				{
					recNbr = invalidRecNbr;
				}
			}
		}
	}
	if (ok)
	{
		unsigned long variant;
		ok  = copTblPtr->GetField (VariantName,operationCode,epsgFldCoordOpName);
		ok  = copTblPtr->GetField (Version,operationCode,epsgFldCoordTfmVersion);
		ok &= copTblPtr->GetAsReal (Accuracy,operationCode,epsgFldCoordOpAccuracy);
		ok &= copTblPtr->GetAsULong (variant,operationCode,epsgFldCoordOpVariant);
		if (ok)
		{
			VariantNbr = static_cast<unsigned>(variant);
			Type = opType;
		}
	}
//lint -restore
}
TcsOpVariant::TcsOpVariant (const TcsOpVariant& source) : Type        (source.Type),
														  EpsgOpCode  (source.EpsgOpCode),
														  VariantNbr  (source.VariantNbr),
														  Version     (source.Version),
														  Accuracy    (source.Accuracy),
														  VariantName (source.VariantName),
														  Singles     (source.Singles)
{
}
TcsOpVariant::~TcsOpVariant (void)
{
	// Nothing to do here.
}
TcsOpVariant& TcsOpVariant::operator= (const TcsOpVariant& rhs)
{
	if (&rhs != this)
	{
		Type        = rhs.Type;
		EpsgOpCode  = rhs.EpsgOpCode;
		VariantNbr  = rhs.VariantNbr;
		Version     = rhs.Version;
		Accuracy    = rhs.Accuracy;
		VariantName = rhs.VariantName;
		Singles     = rhs.Singles;
	}
	return *this;
}
bool TcsOpVariant::IsValid (void) const
{
	bool ok = (Type != epsgOpTypNone && Type != epsgOpTypUnknown);
	return ok;
}
unsigned TcsOpVariant::GetOperationCount (void) const
{
	unsigned rtnValue;
	rtnValue = static_cast<unsigned>(Singles.size ());
	return rtnValue;
}
double TcsOpVariant::GetAccuracy (void) const
{
	return Accuracy;
}
// This function attempts to filter out Longitude Translations (i.e. prime
// meridian shifts) and Null datum transformations, to isolate a real
// datum shift conversion method.
TcsEpsgCode TcsOpVariant::GetOpCodeForCsMap (void) const
{
	unsigned opCount (0U);
	TcsEpsgCode rtnValue;		// default constructor produces invalid object
	std::list<TcsSingleOp>::const_iterator itr;

	// See if there is method other than longitude translation and
	// the Null translation.  If so, it is the one we wish to focus our
	// attention on in the CS-MAP environment.
	for (itr = Singles.begin ();itr != Singles.end ();itr++)
	{
		if (itr->IsNull() || itr->IsLngXlation ())
		{
			continue;
		}
		rtnValue = itr->GetOperationCode ();
		opCount += 1;
	}
	if (opCount == 0)
	{
		// If that failed, we go back and accept a Nul transformation
		// if there is one there.
		for (itr = Singles.begin ();itr != Singles.end ();itr++)
		{
			if (itr->IsLngXlation ())
			{
				continue;
			}
			rtnValue = itr->GetOperationCode ();
			opCount += 1;
		}
	}
	if (opCount != 1U)
	{
		rtnValue = 0UL;
	}
	return rtnValue;
}
//newPage//
//=============================================================================
// TcsOpVariants -- a collection of TcsOpVariant
//
// For any given datum transformation, from datum A to datum B, there may be
// several of what EPSG refers to as variants.  There is usually one variant
// for a datum shift transformation, but there can be as many as a hundred.
// Also, any specific variant may be of the concatenated type.  This object is
// a collection of TcsOpVariant objects.
//
// This sequnce of objects were devised to preserve my sanity when dealing with
// datum shift transformations. 
//
//=============================================================================
// Static Constants, Variables, and Member Functions
//=============================================================================
// Construction  /  Destruction  /  Assignment
TcsOpVariants::TcsOpVariants (const	TcsEpsgDataSetV6& epsgDB,const TcsEpsgCode& sourceCRS,
															 const TcsEpsgCode& targetCRS)
																:
															 SourceCrs (sourceCRS),
															 TargetCrs (targetCRS),
															 Variants  ()
{
	bool ok (false);
	bool deprecated (false);

	unsigned recNbr;
	unsigned invalidRecNbr;

	TcsEpsgCode operationCode;
	TcsEpsgCode recTrgCRS;

	invalidRecNbr = TcsCsvFileBase::GetInvalidRecordNbr ();
	recNbr = invalidRecNbr;			// to keep lint happy
	const TcsEpsgTable* copTblPtr = epsgDB.GetTablePtr (epsgTblCoordinateOperation);
	ok = (copTblPtr != 0);

	if (ok)
	{
		recNbr = copTblPtr->EpsgLocateFirst (epsgFldSourceCrsCode,SourceCrs);
		ok = (recNbr != invalidRecNbr);
	}
	if (ok)
	{
		while (recNbr != invalidRecNbr)
		{
			deprecated = copTblPtr->IsDeprecated (recNbr);
			if (!deprecated)
			{
				ok = copTblPtr->GetAsEpsgCode (recTrgCRS,recNbr,epsgFldTargetCrsCode);
				if (ok && recTrgCRS == TargetCrs)
				{
					ok = copTblPtr->GetAsEpsgCode (operationCode,recNbr,epsgFldCoordOpCode);
					if (ok)
					{
						TcsOpVariant opVariant (epsgDB,operationCode);
						ok = opVariant.IsValid ();
						if (ok)
						{
							Variants.push_back (opVariant);
						}
					}
				}
			}
			recNbr = copTblPtr->EpsgLocateNext (recNbr,epsgFldSourceCrsCode,SourceCrs);
			ok = (recNbr != invalidRecNbr);
		}
	}
}
TcsOpVariants::TcsOpVariants (const TcsOpVariants& source) : SourceCrs (source.SourceCrs),
															 TargetCrs (source.TargetCrs),
															 Variants  (source.Variants)
{
}
TcsOpVariants::~TcsOpVariants (void)
{
	// Nothing to do here.
}
TcsOpVariants& TcsOpVariants::operator= (const TcsOpVariants& rhs)
{
	if (&rhs != this)
	{
		SourceCrs = rhs.SourceCrs;
		TargetCrs = rhs.TargetCrs;
		Variants  = rhs.Variants;
	}
	return *this;
}
//=========================================================================
// Public Named Member Functions
bool TcsOpVariants::GetVariant (TcsOpVariant& variant,unsigned index) const
{
	bool ok (false);

	if (index < Variants.size ())
	{
		variant = Variants [index];
		ok = true;
	}
	return ok;
}
const TcsOpVariant* TcsOpVariants::GetVariantPtr (unsigned index) const
{
	const TcsOpVariant* rtnValue (0);

	if (index < Variants.size ())
	{
		rtnValue = &(Variants [index]);
	}
	return rtnValue;
}
TcsEpsgCode TcsOpVariants::GetOprtnCodeByVariant (unsigned variant) const
{
	unsigned itrVariant;
	TcsEpsgCode operationCode (0UL);
	std::vector<TcsOpVariant>::const_iterator itr;

	for (itr = Variants.begin ();itr != Variants.end ();itr++)
	{
		itrVariant = itr->GetVariantNbr ();
		if (itrVariant == variant)
		{
			operationCode = itr->GetOpCodeForCsMap ();
			break;
		}
	}
	return operationCode;
}
static short KcsTo94ViaPreference [] =
{
	cs_DTCTYP_NONE,
	cs_DTCTYP_MOLO,
	cs_DTCTYP_GEOCTR,
	cs_DTCTYP_MREG,
	cs_DTCTYP_3PARM,
	cs_DTCTYP_6PARM,
	cs_DTCTYP_4PARM,
	cs_DTCTYP_BURS,
	cs_DTCTYP_7PARM,
	cs_DTCTYP_WGS72,
	cs_DTCTYP_NAD27,
	cs_DTCTYP_HPGN,
	cs_DTCTYP_AGD66,
	cs_DTCTYP_AGD84,
	cs_DTCTYP_NZGD49,
	cs_DTCTYP_ATS77,
	cs_DTCTYP_CSRS,
	cs_DTCTYP_TOKYO,
	cs_DTCTYP_RGF93,
	cs_DTCTYP_ED50,
	cs_DTCTYP_DHDN,
	cs_DTCTYP_CHENYX,
	cs_DTCTYP_GDA94,
	cs_DTCTYP_NZGD2K,
	cs_DTCTYP_NAD83,
	cs_DTCTYP_WGS84,
	cs_DTCTYP_ETRF89,
	-1
};
TcsEpsgCode	TcsOpVariants::GetBestOprtnCode (const TcsEpsgDataSetV6& epsgDB) const
{
	bool dummy;
	short crntPreference (0);
	short bestPreference (0);
	short crntTo84Via;
	short idx;
	TcsEpsgCode crntCode;
	TcsEpsgCode bestCode;

	std::vector<TcsOpVariant>::const_iterator itr;

	for (itr = Variants.begin ();itr != Variants.end ();itr++)
	{
		crntCode = itr->GetOpCodeForCsMap ();
		if (crntCode.IsNotValid ())
		{
			continue;
		}
		crntTo84Via = epsgDB.DetermineCsMapDatumMethod (crntCode,dummy);
		for (idx = 0;KcsTo94ViaPreference [idx] >= 0;idx++)
		{
			if (KcsTo94ViaPreference [idx] == crntTo84Via)
			{
				break;
			}
		}
		crntPreference = idx;
		if (crntPreference >= bestPreference)
		{
			bestCode = crntCode;
			bestPreference = crntPreference;
		}
	}
	return bestCode;
}
