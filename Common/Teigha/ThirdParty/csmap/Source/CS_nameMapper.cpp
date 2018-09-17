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

//lint -esym(766,..\Include\cs_wkt.h)				unreferenced header, needed for pre-compiled headers
//lint -esym(1540,TcsNameMap::User)					pointer not freed or zeroed in destructor
//lint -esym(1554,TcsNameMap::User)					copying a pointer directly
//lint -esym(1555,TcsNameMap::User)					copying a pointer directly
//lint -esym(1763,TcsNameMap::GetUserValue)			modifies const object indirectly?

// Now comes May 21, 2014
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// The following table assigns a short name to each of the various
// flavors which are supported.  The csMapFlavorName portion of the
// name map is initialized to these values.  Entries in a name map
// can, and and maybe will in future, overwrite these name values.
wchar_t TcsNameMapper::DefaultFlavorNames [KcsNameMapFlvrCnt][32] =
{
	L"",			// Index =  0 place holder
	L"EPSG",		// Index =  1
	L"ESRI",		// Index =  2
	L"Oracle",		// Index =  3
	L"Autodesk",	// Index =  4
	L"Bentley",		// Index =  5
	L"Safe",		// Index =  6
	L"MapInfo",		// Index =  7
	L"Ctmx",		// Index =  8
	L"CsMap",		// Index =  9
	L"OGC",			// Index = 10
	L"OCR",			// Index = 11
	L"GeoTiff",		// Index = 12
	L"GeoTools",	// Index = 13
	L"Oracle9", 	// Index = 14
	L"IBM",			// Index = 15
	L"OEM",			// Index = 16
	L"Anon01",		// Index = 17
	L"Anon02",		// Index = 18
	L"Anon03",		// Index = 19
	L"Anon04",		// Index = 10
	L"Anon05",		// Index = 21
	L"Anon06",		// Index = 22
	L"Anon07",		// Index = 23
	L"Anon08",		// Index = 24
	L"Anon09",		// Index = 25
	L"Anon10",		// Index = 26
	L"Anon11",		// Index = 27
	L"Anon12",		// Index = 28
	L"Anon13",		// Index = 29
	L"User",		// Index = 30
	L"Legacy",		// Index = 31
};

// An instance of a Generic ID object with an unknown value, occasionally
// useful when passing generic ID's as function arguments.
const TcsGenericId KcsGenericIdNull;

// The following are used as bounds for a search of the set.  FirstName
// is set to a value that will collate before any legitimate name;
// LastName is set to a value which collates after any legitmate name
// (we hope).  Someone more familiar with internationalization should adjust
// these values to be consistent with internationalized names.
const wchar_t TcsNameMapper::FirstName [32] = L" ";
const wchar_t TcsNameMapper::LastName [32] = L"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";

///////////////////////////////////////////////////////////////////////////////
// TcsNameMap Object  --  Object used to associate various names for the same
//						  thing.
///////////////////////////////////////////////////////////////////////////////
// Static Constants, Variables, and Functions.
//
// CsvSort simply sorts into the preferred order when writting to the external
// CSV file
bool TcsNameMap::CsvSort (const TcsNameMap& lhs,const TcsNameMap& rhs)
{
	bool lessThan = (lhs.Type < rhs.Type);
	if (lhs.Type == rhs.Type)
	{
		lessThan = (lhs.GenericId < rhs.GenericId);
		if (lhs.GenericId == rhs.GenericId)
		{
			lessThan = (lhs.Flavor < rhs.Flavor);
			if (lhs.Flavor == rhs.Flavor)
			{
				lessThan = (lhs.AliasFlag < rhs.AliasFlag);
				if (lhs.AliasFlag == rhs.AliasFlag)
				{
					int cmpVal = CS_wcsicmp (lhs.Name.c_str (),rhs.Name.c_str ());
					lessThan = (cmpVal < 0);
					if (cmpVal == 0)
					{
						lessThan = (lhs.DupSort < rhs.DupSort);
					}
				}
			}
		}
	}
	return lessThan;
}
///////////////////////////////////////////////////////////////////////////////
// Construction  /  Destruction  /  Assignment
TcsNameMap::TcsNameMap () : GenericId (0UL),Type (csMapNone),Flavor     (csMapFlvrNone),
															 NumericId  (0UL),
															 Name       (),
															 DupSort    (0),
															 AliasFlag  (0),
															 Flags      (0UL),
															 User       (0),
															 Deprecated (0UL),
															 Remarks    (),
															 Comments   ()
{
}
TcsNameMap::TcsNameMap (const TcsGenericId& genericId,EcsMapObjType type,
													  EcsNameFlavor flavor,
													  unsigned long numericId,
													  const wchar_t* name)
														:
													  GenericId  (genericId),
													  Type       (type),
													  Flavor     (flavor),
													  NumericId  (numericId),
													  Name       (name),
													  DupSort    (0),
													  AliasFlag  (0),
													  Flags      (0UL),
													  User       (0),
													  Deprecated (),
													  Remarks    (),
													  Comments   ()
{
}
TcsNameMap::TcsNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long numericId,
																const wchar_t* name)
																	:
																GenericId  (),
																Type       (type),
																Flavor     (flavor),
																NumericId  (numericId),
																Name       (name),
																DupSort    (0),
																AliasFlag  (0),
																Flags      (0UL),
																User       (0),
																Deprecated (),
																Remarks    (),
																Comments   ()
{
	if (NumericId != 0)
	{
		GenericId = TcsGenericId (flavor,numericId);
	}
}													  
TcsNameMap::TcsNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name,
																short dupSort,
																short aliasFlag)
																	:
																GenericId  (),
																Type       (type),
																Flavor     (flavor),
																NumericId  (0UL),
																Name       (name),
																DupSort    (dupSort),
																AliasFlag  (aliasFlag),
																Flags      (0UL),
																User       (0),
																Deprecated (),
																Remarks    (),
																Comments   ()
{
}														  
TcsNameMap::TcsNameMap (const TcsNameMap& source) : GenericId  (source.GenericId),
													Type       (source.Type),
													Flavor     (source.Flavor),
													NumericId  (source.NumericId),
													Name       (source.Name),
													DupSort    (source.DupSort),
													AliasFlag  (source.AliasFlag),
													Flags      (source.Flags),
													User       (source.User),
													Deprecated (source.Deprecated),
													Remarks    (source.Remarks),
													Comments   (source.Comments)
{
}
TcsNameMap::~TcsNameMap (void)
{
	// Nothing to do here.  This is a virtual destructor.
}
TcsNameMap& TcsNameMap::operator= (const TcsNameMap& rhs)
{
	if (&rhs != this)
	{
		GenericId  = rhs.GenericId;
		Type       = rhs.Type;
		Flavor     = rhs.Flavor;
		NumericId  = rhs.NumericId;
		Name       = rhs.Name;
		DupSort    = rhs.DupSort;
		AliasFlag  = rhs.AliasFlag;
		Flags      = rhs.Flags;
		User       = rhs.User;
		Deprecated = rhs.Deprecated;
		Remarks    = rhs.Remarks;
		Comments   = rhs.Comments;
		Flags      = rhs.Flags;
	}
	return *this;
}
// This is the function used by std::set to determine if an object has a
// unique key value.
bool TcsNameMap::operator< (const TcsNameMap& rhs) const
{
	bool lessThan = (Type < rhs.Type);
	if (Type == rhs.Type)
	{
		lessThan = (Flavor < rhs.Flavor);
		if (Flavor == rhs.Flavor)
		{
			lessThan = (AliasFlag < rhs.AliasFlag);
			if (AliasFlag == rhs.AliasFlag)
			{
				// Not all items have a numeric ID.  All do have a name.  If, by
				// chance, we run into something that doesn't have a name, we
				// would construct a name, such as EPSG:9999.
				
				// We need a case insensitive comparison, so we can't use the
				// std::wstring::operator< ().
				int cmpVal = CS_wcsicmp (Name.c_str (),rhs.Name.c_str ());
				lessThan = (cmpVal < 0);
				if (cmpVal == 0)
				{
					lessThan = (DupSort < rhs.DupSort);
				}
			}
		}
	}
	return lessThan;
}
EcsCsvStatus TcsNameMap::ReadFromStream (std::wistream& inStrm)
{
	EcsCsvStatus rtnStatus;
	TcsCsvStatus csvStatus;

	rtnStatus = ReadFromStream (inStrm,csvStatus);
	return rtnStatus;
}
EcsCsvStatus TcsNameMap::ReadFromStream (std::wistream& inStrm,TcsCsvStatus& csvStatus)
{
	wint_t firstChar;
	unsigned long ulTmp;
	std::vector<std::wstring> fields(16);
	std::wstring lineBufr;

	// We do not allow for comments as whatever the comment character would
	// be might end up in a name somewhere.
	Name.clear ();
	Remarks.clear ();
	Comments.clear ();

	EcsCsvStatus status = csGetCsvRecord (lineBufr,inStrm,TcsNameMapper::Delimiters);
	if (status == csvOk)
	{
		status = csCsvFieldParse (fields,lineBufr,TcsNameMapper::Delimiters);
		if (status == csvOk)
		{
			// We have big problems if we try to process an empty field.
			// We could check each field individually, but that would be
			// slow.  Perhaps sometime in the future.
			size_t emptyCount = 0;
			for (size_t idx = 0;idx < 7;idx += 1)
			{
				if (fields [idx].empty ())
				{
					emptyCount += 1;
				}
			}
			if (emptyCount != 0)
			{
				status = csvEmpty;
			}
		}
		if (status == csvOk)
		{
			size_t fldCnt = fields.size ();
			if (fldCnt >= 7 && fldCnt <= 16)
			{
				ulTmp = wcstoul (fields [0].c_str (),0,10);
				GenericId = TcsGenericId (ulTmp);

				ulTmp = wcstoul (fields [1].c_str (),0,10);
				Type = EcsMapObjType (static_cast<int>(ulTmp));

				firstChar = fields[2].at(0);
				if (iswdigit (firstChar))
				{
					ulTmp = wcstoul (fields [2].c_str (),0,10);
					Flavor = EcsNameFlavor (static_cast<int>(ulTmp));
				}
				else
				{
					Flavor = TcsNameMapper::FlvrNameToNbr (fields [2].c_str ()); 
				}

				NumericId = wcstoul (fields [3].c_str (),0,10);
				Name = fields [4];
				DupSort = static_cast<short>(wcstol (fields [5].c_str (),0,10));
				AliasFlag = static_cast<short>(wcstol (fields [6].c_str (),0,10));

				if (fldCnt > 7)
				{
					Flags = wcstoul (fields [7].c_str (),0,10);
				}
				if (fldCnt > 8)
				{
					ulTmp = wcstoul (fields [8].c_str (),0,10);
					Deprecated = TcsGenericId (ulTmp);
				}
				if (fldCnt > 9)
				{
					Remarks = fields [9];
				}
				if (fldCnt > 10)
				{
					Comments = fields [10];
				}
			}
			else if (fldCnt < 7)
			{
				status = csvTooFewFields;
				csvStatus.SetStatus (status);
			}
			else
			{
				status = csvTooManyFields;
				csvStatus.SetStatus (status);
			}
		}
		else
		{
			csvStatus.SetStatus (status);
		}
	}
	else
	{
		csvStatus.SetStatus (status);
	}
	return status;
}
void* TcsNameMap::GetUserValue (void) const
{
	return User;
}
const wchar_t* TcsNameMap::GetComments (void) const
{
	return Comments.c_str ();
}
void TcsNameMap::GetComments (std::wstring& comments) const
{
	comments = Comments;
}
const wchar_t* TcsNameMap::GetRemarks (void) const
{
	return Remarks.c_str ();
}
void TcsNameMap::GetRemarks (std::wstring& remarks) const
{
	remarks = Remarks;
}
void* TcsNameMap::SetUserValue (void* userValue)
{
	void* rtnValue = User;
	User = userValue;
	return rtnValue;
}
void TcsNameMap::SetComments (const wchar_t* comments)
{
	Comments = comments;
}
void TcsNameMap::SetRemarks (const wchar_t* remarks)
{
	Remarks = remarks;
}
void TcsNameMap::SetNameId (const wchar_t* newNameId)
{
	Name = newNameId;
}
void TcsNameMap::SetNumericId (unsigned long newNumericId)
{
	NumericId = newNumericId;
}
void TcsNameMap::WriteAsCsv (std::wostream& outStrm,bool flvrLbls) const
{
	int type;
	int flavor;
	unsigned long genericId;
	const wchar_t* flvrName;
	std::wstring name;

	genericId = GenericId.GetGenericId ();
	type = static_cast<int>(Type);
	flavor = static_cast<int>(Flavor);
	name = Name;
	csCsvQuoter (name,true);

	if (flvrLbls && Type != csMapFlavorName)
	{
		flvrName = TcsNameMapper::FlvrNbrToName (Flavor);
		outStrm	<< genericId << L','
				<< type      << L','
				<< flvrName  << L','
				<< NumericId << L','
				<< name      << L','
				<< DupSort   << L','
				<< AliasFlag << L','
				<< Flags     << L','
				<< Deprecated.GetGenericId () << L',';
	}
	else
	{
		outStrm	<< genericId << L','
				<< type      << L','
				<< flavor    << L','
				<< NumericId << L','
				<< name      << L','
				<< DupSort   << L','
				<< AliasFlag << L','
				<< Flags     << L','
				<< Deprecated.GetGenericId () << L',';
	}

	std::wstring remarks = Remarks;
	std::wstring comments = Comments;
	csCsvQuoter (remarks,true);
	csCsvQuoter (comments,true);
	outStrm << remarks  << L','
			<< comments;
	outStrm << std::endl;
	return;
}
//newPage//
//=========================================================================
// TcsNameList  --  A collection of const TcsNameMap pointers.
//					Typically used as means of returning a selection of
//					names and/or ID's.
//=========================================================================
// Construction,  Assignment,  Destruction
TcsNameMapList::TcsNameMapList (void) : NameMapList ()
{
}
TcsNameMapList::TcsNameMapList (const TcsNameMapList& source) : NameMapList (source.NameMapList)
{
}
TcsNameMapList::~TcsNameMapList (void)
{
}
TcsNameMapList& TcsNameMapList::operator= (const TcsNameMapList& rhs)
{
	if (&rhs != this)
	{
		NameMapList = rhs.NameMapList;
	}
	return *this;
}
//=========================================================================
// Operator Overrides
TcsNameMapList& TcsNameMapList::operator+= (const TcsNameMap* newNameMapPtr)
{
	if (newNameMapPtr != 0)
	{
		NameMapList.push_back (newNameMapPtr);
	}
	return *this;
}
TcsNameMapList& TcsNameMapList::operator-= (const TcsNameMap* existingNameMapPtr)
{
	(void)RemoveNameMap (existingNameMapPtr);
	return *this;
}
//=========================================================================
// Public Named Member Functions
void TcsNameMapList::AddNameMap (const TcsNameMap* newNameMapPtr)
{
	if (newNameMapPtr != 0)
	{
		NameMapList.push_back (newNameMapPtr);
	}
}
bool TcsNameMapList::RemoveNameMap (const TcsNameMap* existingNameMapPtr)
{
	bool ok = false;
	std::vector<const TcsNameMap*>::iterator itr;
	
	for (itr = NameMapList.begin ();itr != NameMapList.end ();itr++)
	{
		if (*itr == existingNameMapPtr)
		{
			NameMapList.erase (itr);		//lint !e534   ignoring return value
			ok = true;
			break;
		}
	}
	return ok;
}
const TcsNameMap* TcsNameMapList::GetNameMapPtr (size_t index) const
{
	std::vector<const TcsNameMap*>::iterator itr;

	const TcsNameMap* rtnValue = 0;
	
	if (index < NameMapList.size ())
	{
		rtnValue = *(itr + index);
	}
	return rtnValue;
}
void TcsNameMapList::Erase (void)
{
	NameMapList.clear ();
}
//newPage//
///////////////////////////////////////////////////////////////////////////////
// TcsNameMapper  --  A collection of TcsNameMap objects.
const wchar_t TcsNameMapper::Delimiters [4] = {L',', L'\"', L'\"', L'\0' };
EcsNameFlavor TcsNameMapper::FlvrNameToNbr (const wchar_t* flvrName)
{
	EcsNameFlavor idx = csMapFlvrNone;
	for (idx = csMapFlvrNone;idx != csMapFlvrUnknown;++idx)
	{
		if (!CS_wcsicmp (flvrName,DefaultFlavorNames [idx]))
		{
			break;
		}
	}
	return idx;
}
const wchar_t* TcsNameMapper::FlvrNbrToName (EcsNameFlavor flvrNbr)
{
	const wchar_t* rtnValue = DefaultFlavorNames [0];
	if (flvrNbr < csMapFlvrUnknown)
	{
		rtnValue = DefaultFlavorNames [flvrNbr];
	}
	return rtnValue;
}
bool TcsNameMapper::AnalyzeGenericId (EcsNameFlavor& flavor,unsigned long& flvrId,
															const TcsGenericId& genericId)
{
	bool isDefault (false);
	unsigned long numericId;

	flavor = genericId.ExtractFlavorId (numericId);
	isDefault = (numericId >= KcsNameMapInitial) && (numericId >= KcsNameMapFinal);

	flvrId = numericId;
	return isDefault;
}
///////////////////////////////////////////////////////////////////////////////
// Construction  /  Destruction  /  Assignment
TcsNameMapper::TcsNameMapper () : RecordDuplicates (false),
								  DefinitionSet    (),
								  Duplicates       ()
{

	InitializeFlavors ();
	AdjustDefaultIDs ();
}
TcsNameMapper::TcsNameMapper (const TcsNameMapper& source) : RecordDuplicates (source.RecordDuplicates),
															 DefinitionSet    (source.DefinitionSet),
															 Duplicates       (source.Duplicates)
{
	AdjustDefaultIDs ();
}
TcsNameMapper::~TcsNameMapper (void)
{
}
TcsNameMapper& TcsNameMapper::operator= (const TcsNameMapper& rhs)
{
	if (&rhs != this)
	{
		RecordDuplicates = rhs.RecordDuplicates;
		DefinitionSet    = rhs.DefinitionSet;
		Duplicates       = rhs.Duplicates;
		AdjustDefaultIDs ();
	}
	return *this;
}
///////////////////////////////////////////////////////////////////////////
// Operator Overrides
TcsNameMapper& TcsNameMapper::operator+= (const TcsNameMap& newItem)
{
	// We assume that if this fails, it is only because an equivalent
	// object already exists in the set.  This is an essential element
	// of the design of this object.
	DefinitionSet.insert (newItem);		//lint !e534  ignoring return value
	return *this;
}
///////////////////////////////////////////////////////////////////////////
// Named Member functions
bool TcsNameMapper::IsInitialized () const
{
	bool initialized = (DefinitionSet.size () > 32);
	return initialized;
}
bool TcsNameMapper::SetRecordDuplicates (bool recordDuplicates)
{
		bool rtnValue = RecordDuplicates;
		RecordDuplicates = recordDuplicates;
		return rtnValue;
}
void TcsNameMapper::ClearDuplicateList (void)
{
	Duplicates.clear ();
}
EcsCsvStatus TcsNameMapper::ReadFromStream (std::wistream& inStrm)
{
	EcsCsvStatus rtnStatus;
	TcsCsvStatus csvStatus;

	rtnStatus = ReadFromStream (inStrm,csvStatus);
	return rtnStatus;
}

EcsCsvStatus TcsNameMapper::ReadFromStream (char* pBuffer, size_t const bufferSize)
{
	if (NULL == pBuffer)
		return csvNoFile;

	EcsCsvStatus csvStatus  = csvOk;

	size_t const lastIndex = bufferSize - 1; // Last index of the stream buffer (file)

	// Temporary line buffer
	size_t lineBufferLength = 300; // Expected maximum length of a line
	char* pLineBuffer = (char*) CS_malc (lineBufferLength);
	if (NULL == pLineBuffer)
	{
		CS_erpt(cs_NO_MEM);
		return csvInternal;
	}

	size_t lineStart = 0; // Start index of the line
	bool isEOF = false;
	size_t i = 0; // Current index in the stream buffer (file)
	while(i < bufferSize && !isEOF)
	{
		// Current character from the buffer
		char const* curChar = &pBuffer[i];

		// Advance [i] to next line breaking character
		if ('\n' != *curChar && '\r' != *curChar)
		{
			// Pass any non-line breaking character;
			// [lineStart] we still keep at the line start position
			if (i != lastIndex)
			{
				++i;
				continue;
			}

			// Last line. Last line need to be processed. EOF is handled below.
		}

		// Line length
		// If the buffer did start with empty lines only, we're getting an 0-size length here
		size_t lineLength = i - lineStart;
		
		// Advance [i] beyond all ine breaking characters (\n & \r)
		while(i < bufferSize)
		{
			curChar = &pBuffer[i];
			if ('\n' == *curChar || '\r' == *curChar)
			{
				++i;
				continue;
			}

			break;
		}
		// Handle the end of the file
		if (i >= bufferSize - 1)
			isEOF = true;

		// Resize the temp line buffer if it is too small
		if (lineBufferLength < lineLength)
		{
			lineBufferLength = lineLength + (1 << 6);
			char* pReBuffered = (char*) CS_ralc (pLineBuffer, lineBufferLength);
			if (NULL == pReBuffered)
			{
				CS_erpt(cs_NO_MEM); //pLineBuffer still has to be free - see below
				return csvNoFile;
			}
			pLineBuffer = pReBuffered;
		}

		// Fill up temp line buffer with current line
		memset(pLineBuffer, '\0', lineBufferLength);
		memcpy(pLineBuffer, &pBuffer[lineStart], lineLength);

		// Next line starting position
		lineStart = i; //our next potential start; EOF doesn't matter here

		// Temporary line buffer (wide char)
		wchar_t wcharBuffer[1 << 10] = { L'\0' };
		mbstowcs(wcharBuffer, pLineBuffer, lineLength);
		wcharBuffer[lineLength] = L'\n'; // Add a newline character. This will be needed below in the csv support class.

		// Prepare a line stream
		std::wstringstream inStrm(wcharBuffer);
		inStrm.seekg(0);			//lint !e534   ignoring return value

		// Skip the line if it looks like a label line. Note that we
		// require a label line to be a valid CSV record, even though we
		// ignore the contents.
		// A data line need to start with a digit otherwise it is expected to be a label/comment line and we skip
		if (!isdigit(pLineBuffer[0]))
		{
			//// Validate the record
			//std::wstring lineBufferUnused;
			//csvStatus = csGetCsvRecord (lineBufferUnused, inStrm, Delimiters);
			//if (csvOk != csvStatus)
			//    break;

			// Skip the line
			// Note that [i] and [lineStart] point already to the beginning of the next line, or EOF...
			continue;
		}

		// Create name map item (Data of one line)
		TcsNameMap nextItem;
		TcsCsvStatus status;
		// Read the data from the line
		csvStatus= nextItem.ReadFromStream (inStrm, status);
		if (csvStatus == csvOk)
		{
			// Add new item to mapping collection
			Add (nextItem);		//lint !e534   ignoring return value
		}
		else
		{
			continue;
		}
	}

	// Free temp line buffer
	CS_free(pLineBuffer);
	pLineBuffer = NULL;
	
	return csvStatus;
}

EcsCsvStatus TcsNameMapper::ReadFromStream (std::wistream& inStrm,TcsCsvStatus& status)
{
	wint_t firstChar;
	wint_t nextChar;
	EcsCsvStatus csvStatus = csvOk;
	std::wstring lineBufr;
	TcsNameMap nextItem;
	
	firstChar = inStrm.peek ();
	if (firstChar != WEOF && !iswdigit (firstChar))
	{
		// Skip the first line if it looks like a label line.  Note that we
		// require a label line to be a valid CSV record, even though we
		// ignore the contents.
		csvStatus = csGetCsvRecord (lineBufr,inStrm,Delimiters);
	}	
	while (csvStatus == csvOk && inStrm.good() && !inStrm.eof())
	{
		csvStatus= nextItem.ReadFromStream (inStrm,status);
		if (csvStatus == csvOk)
		{
			Add (nextItem);		//lint !e534   ignoring return value
		}
		nextChar = inStrm.peek ();
		if (nextChar == WEOF)
		{
			break;
		}
	}
	return csvStatus;
}
bool TcsNameMapper::Add (TcsNameMap& newItem,bool addDupName,const wchar_t* objSrcId)
{
	std::pair<iterator,bool> insertStatus;

	// If the presented object does not have a generic ID yet, we assign
	// one before adding the object to the set.
	if (newItem.NeedsGenericId ())
	{
		EcsNameFlavor flavor = newItem.GetFlavor ();
		unsigned long newId = GetNextDfltId (flavor);
		TcsGenericId newGenericId (flavor,newId);
		newItem.SetGenericId (newGenericId);
	}
	insertStatus = DefinitionSet.insert (newItem);
	
	// Record all duplicates, even if we eventually add it.
	if (RecordDuplicates && !insertStatus.second)
	{
		if (objSrcId == 0)
		{
			objSrcId = L"?";
		}
		TcsNameMap dupItem (newItem);
		dupItem.SetComments (objSrcId);
		Duplicates.push_back (dupItem);
	}

	// If so requested, we add a duplicate name.  A little defensive
	// programming here.  We will not try to add the name more then 20
	// times.
	while (addDupName && !insertStatus.second)
	{
		short dupSort = newItem.GetDupSort ();
		dupSort += 1;
		if (dupSort > 20)
		{
			break;
		}
		newItem.SetDupSort (dupSort);
		insertStatus = DefinitionSet.insert (newItem);
	}
	return insertStatus.second;
}
bool TcsNameMapper::Add (const TcsNameMap& newItem)
{
	std::pair<iterator,bool> insertStatus;
	insertStatus = DefinitionSet.insert (newItem);
	return insertStatus.second;
}
bool TcsNameMapper::Replace (const TcsNameMap& newItem,const TcsNameMap& oldItem)
{
	iterator itr;
	std::pair<iterator,bool> insertStatus;

	itr = DefinitionSet.find (oldItem);
	if (itr != DefinitionSet.end ())
	{
		DefinitionSet.erase (itr);		//lint !e534   ignoring return value
	}
	insertStatus = DefinitionSet.insert (newItem);
	return insertStatus.second;
}
// Given a specific system id the systemId parameter, this function will
// extract a copy of a TcsNameMap record with the specific objType, flavor,
// and flavor ID (wkid); if it exists of course.  
bool TcsNameMapper::ExtractSpecificId (TcsNameMap& nameMap,EcsMapObjType objType,TcsGenericId systemId,
																				 EcsNameFlavor flavor,
																				 unsigned long wkid) const
{
	bool ok (false);

	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;
	
	TcsNameMap beginSearchObj (objType,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (objType,flavor,LastName,9999,9999);
	
	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetGenericId () == systemId)
		{
			if (searchItr->GetNumericId () == wkid)
			{
				nameMap = *searchItr;
				ok = true;
				break;
			}
		}
	}
	return ok;
}
// Given a specific system id the systemId parameter, this function will
// extract a copy of a TcsNameMap record with the specific objType, flavor,
// and flavored name; if it exists of course.  
bool TcsNameMapper::ExtractSpecificName (TcsNameMap& nameMap,EcsMapObjType objType,TcsGenericId systemId,
																				   EcsNameFlavor flavor,
																				   const wchar_t* name) const
{
	bool ok (false);

	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;

	TcsNameMap beginSearchObj (objType,flavor,name,0,0);
	TcsNameMap endSearchObj (objType,flavor,name,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetGenericId () == systemId)
		{
			if (!CS_wcsicmp (searchItr->GetNamePtr (),name))
			{
				nameMap = *searchItr;
				ok = true;
				break;
			}
		}
	}
	return ok;
}

bool TcsNameMapper::AliasExistingName (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* oldName,
																			   const wchar_t* newName,
																			   const wchar_t* comment,
																			   const TcsGenericId& deprecatedBy)
{
	bool ok (false);

	iterator itr;

	TcsNameMap searchObject (type,flavor,oldName);
	itr = DefinitionSet.find (searchObject);
	if (itr != DefinitionSet.end ())
	{
		// OK, we found it.  Should usually be the case.  The intent of this
		// function is to update a mapping to the newest valid name, while
		// preserving the old name as an alias.  If the entry we found is
		// already marked as an alias, then this function is not being used
		// as intended and we simply report failure by returning false.
		if (itr->GetAliasFlag () == 0)
		{
			// We need to make two copies of this entry, modify them so that
			// the old name remains as an alias, the new one has the new name.
			// Then delete the existing entry and add the two new ones back.
			// We need to do this as the alias flag and the name are part of
			// the std::set<> key. It is not nice to fool with the std::set's
			// key behind its back.
			
			// First, we get two copies of thew existing entry.
			TcsNameMap oldEntry (*itr);
			TcsNameMap newEntry (*itr);
			
			// Erase the old entry we are replacing.
			DefinitionSet.erase (itr);		//lint !e534   ignoring return value

			// Modify the two copies as appropriate.
			oldEntry.SetAliasFlag (1);
			if (deprecatedBy.IsKnown ())
			{
				oldEntry.SetDeprecated (deprecatedBy);
			}
			if (comment != 0 && *comment != L'\0')
			{
				oldEntry.SetComments (comment);
			}
			newEntry.SetNameId (newName);
			newEntry.SetAliasFlag (0);		// redundant???
			newEntry.SetComments (L"");

			// Add both modified entries back into the set.
			ok = Add (newEntry);
			if (ok)
			{
				ok = Add (oldEntry);
			}
		}
	}
	return ok;
}
bool TcsNameMapper::AliasExistingName (EcsMapObjType type,EcsNameFlavor flavor,unsigned long wkid,
																			   const wchar_t* newName,
																			   const wchar_t* comment,
																			   const TcsGenericId& deprecatedBy)
{
	bool ok (false);
	iterator entryItr;

	entryItr = LocateNameMapItr (type,flavor,wkid);
	if (entryItr != DefinitionSet.end ())
	{
		// OK, we found it.  Should usually be the case.  The intent of this
		// function is to update a mapping to the newest valid name, while
		// preserving the old name as an alias.  If the entry we found is
		// already marked as an alias, then this function is not being used
		// as intended and we simply report failure by returning false.
		if (entryItr->GetAliasFlag () == 0)
		{
			// We need to make two copies of this entry, modify them so that
			// the old name remains as an alias, the new one has the new name.
			// Then delete the existing entry and add the two new ones back.
			// I believe we need to do this as the alias flag and the name are
			// part of the std::set<> key. I would imagine that it is not nice
			// to fool with the std::set's key behind its back.
			
			// First, we get two copies of thew existing entry.
			TcsNameMap oldEntry (*entryItr);
			TcsNameMap newEntry (*entryItr);
			
			// Erase the old entry we are replacing.
			DefinitionSet.erase (entryItr);		//lint !e534   ignoring return value

			// Modify the two copies as appropriate.
			oldEntry.SetAliasFlag (1);
			if (deprecatedBy.IsKnown ())
			{
				oldEntry.SetDeprecated (deprecatedBy);
			}
			if (comment != 0 && *comment != L'\0')
			{
				oldEntry.SetComments (comment);
			}
			newEntry.SetNameId (newName);
			newEntry.SetAliasFlag (0);		// redundant???
			newEntry.SetComments (L"");

			// Add both modified entries back into the set.
			ok = Add (newEntry);
			if (ok)
			{
				ok = Add (oldEntry);
			}
		}
	}
	return ok;
}
// The following extracts the specific record from the NameMapper's
// DefinitionSet and removes it from the std::set<> object.  The idea here
// is that having removed the item from the set, the item can be modified
// and added back without having to concern oneself with whether any of the
// set's key fields were modified.
bool TcsNameMapper::ExtractAndRemove (TcsNameMap& extractedNameMap,EcsMapObjType type,
																   EcsNameFlavor flavor,
																   const wchar_t* name,
																   short aliasFlag,
																   short dupSort)
{
	bool ok (false);

	iterator itr;

	// Note that the dupSort and aliasFlag arguments have default
	// values of 0.  This is the typical case, and these are the
	// values which are provided by TcsNameMap constructor which
	// is being used here.  This might display some strange effects.
	// At this point, it is a good idea to provide all elements of
	// the std::set<>'s key to assure you're getting the correct
	// entry.
	TcsNameMap searchObject (type,flavor,name,dupSort,aliasFlag);
	itr = DefinitionSet.find (searchObject);
	if (itr != DefinitionSet.end ())
	{
		extractedNameMap = *itr;
		DefinitionSet.erase (itr);		//lint !e534   ignoring return value
		ok = true;
	}
	return ok;
}
// The implication here is that we are to locate an item by name.  Thus, we
// call the LocateNameMap function which will include aliases in its search.
TcsGenericId TcsNameMapper::Locate (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name) const
{
	TcsGenericId rtnValue;
	const TcsNameMap* nmMapPtr;
	
	nmMapPtr = LocateNameMap (type,flavor,name);
	if (nmMapPtr != 0)
	{
		rtnValue = nmMapPtr->GetGenericId ();
	}
	return rtnValue;
}
TcsGenericId TcsNameMapper::Locate (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const
{
	TcsGenericId rtnValue;
	const TcsNameMap* nmMapPtr;
	
	nmMapPtr = LocateNameMap (type,flavor,id);
	if (nmMapPtr != 0)
	{
		rtnValue = nmMapPtr->GetGenericId ();
	}
	return rtnValue;
}
// The implication here is that we want a name to identify something.  We do
// specifically do not want any aliases.  Due to the collating sequence of the
// underlying std::set, by using the first name we encounter with the given
// generic ID, we will get the real name and not an alias.
const wchar_t* TcsNameMapper::LocateName (EcsMapObjType type,EcsNameFlavor flavor,const TcsGenericId& genericId) const
{
	const wchar_t* rtnValue = 0;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;

	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);
	
	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetGenericId () == genericId)
		{
			rtnValue = searchItr->GetNamePtr ();
			break;
		}
	}
	return rtnValue;
}
// Again, we take the first record we encounter, so we will get the number
// associated with the first real name.
unsigned long TcsNameMapper::LocateNumber (EcsMapObjType type,EcsNameFlavor flavor,const TcsGenericId& genericId) const
{
	unsigned long rtnValue = 0UL;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;
	
	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);
	
	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetGenericId () == genericId)
		{
			rtnValue = searchItr->GetNumericId ();
			break;
		}
	}
	return rtnValue;
}
const wchar_t* TcsNameMapper::FlavorToName (EcsNameFlavor flavor) const
{
	const wchar_t* nameOfFlavor = L"<unknown>";
	const_iterator itr;
	TcsNameMap searchObject (csMapFlavorName,flavor,FirstName,0,0);
	
	itr = DefinitionSet.lower_bound (searchObject);
	if (itr != DefinitionSet.end ())
	{
		nameOfFlavor = itr->GetNamePtr ();
	}
	return nameOfFlavor;
}
unsigned long TcsNameMapper::GetNextDfltId (EcsNameFlavor flavor)
{
	unsigned long newId;

	InitialDfltFlvrIds [flavor] += 1;
	newId = InitialDfltFlvrIds [flavor] + ((flavor) * KcsNameMapBias);
	return newId;
}
void TcsNameMapper::WriteLabelLine (std::wostream& outStrm) const
{
	outStrm         << L"GenericId"
			<< L',' << L"NameType"
			<< L',' << L"Flavor"
			<< L',' << L"NumericId"
			<< L',' << L"NameId"
			<< L',' << L"DupSort"
			<< L',' << L"AliasFlag"
			<< L',' << L"Flags"
			<< L',' << L"Deprecation"
			<< L',' << L"Remarks"
			<< L',' << L"Comment"
			<< std::endl;
}
void TcsNameMapper::WriteAsCsv (std::wostream& outStrm,bool flvrLbls) const
{
	const_iterator setItr;
	std::vector<TcsNameMap> sortedVector;
	std::vector<TcsNameMap>::iterator sortedItr;

	// Copy the DefinitionSet to a vector so we can sort it.
	for (setItr = DefinitionSet.begin ();setItr != DefinitionSet.end ();setItr++)
	{
		sortedVector.push_back (*setItr);
	}
	// Second thing: sort the resulting vector into an order that will make
	// sense to a human observer.
	std::sort (sortedVector.begin (),sortedVector.end (),TcsNameMap::CsvSort);		//lint !e864    result may depend upon order of evaluation

	// We always label the Name Mapper file.
	WriteLabelLine (outStrm);

	// Output each of the entries in the sorted vector.
	for (sortedItr = sortedVector.begin ();sortedItr != sortedVector.end ();sortedItr++)
	{
		sortedItr->WriteAsCsv (outStrm,flvrLbls);
	}
	return;
}
void TcsNameMapper::WriteDuplicates (std::wostream& outStrm)
{
	std::vector<TcsNameMap>::iterator dupItr;

	std::sort (Duplicates.begin (),Duplicates.end (),TcsNameMap::CsvSort);
	
	WriteLabelLine (outStrm);
	for (dupItr = Duplicates.begin ();dupItr != Duplicates.end ();dupItr++)
	{
		dupItr->WriteAsCsv (outStrm,true);
	}
	return;
}
bool TcsNameMapper::GetUserByNbr (void*& result,EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const
{
	bool ok (false);
	
	const TcsNameMap* mapPtr = LocateNameMap (type,flavor,id);
	if (mapPtr != 0)
	{
		result = mapPtr->GetUserValue ();
		ok = true;
	}
	return ok;
}
bool TcsNameMapper::SetUserByNbr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id,void* userValue)
{
	bool ok (false);
	
	TcsNameMap* mapPtr = LocateNameMap (type,flavor,id);
	if (mapPtr != 0)
	{
		mapPtr->SetUserValue (userValue);		//lint !e534   ignoring return value
		ok = true;
	}
	return ok;
}
bool TcsNameMapper::GetUserByName (void*& result,EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name) const
{
	bool ok (false);
	
	const TcsNameMap* mapPtr = LocateNameMap (type,flavor,name);
	if (mapPtr != 0)
	{
		result = mapPtr->GetUserValue ();
		ok = true;
	}
	return ok;
}
bool TcsNameMapper::SetUserByName (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name,void* userValue)
{
	bool ok (false);
	
	TcsNameMap* mapPtr = LocateNameMap (type,flavor,name);
	if (mapPtr != 0)
	{
		mapPtr->SetUserValue (userValue);		//lint !e534   ignoring return value
		ok = true;
	}
	return ok;
}
///////////////////////////////////////////////////////////////////////////////
// Private Support Functions
///////////////////////////////////////////////////////////////////////////////
void TcsNameMapper::InitializeFlavors (void)
{
	EcsNameFlavor idx;
	for (idx = EcsNameFlavor (1);idx != csMapFlvrUnknown;++idx)
	{
		const wchar_t* wcPtr = DefaultFlavorNames [idx];
		if (*wcPtr != L'\0')
		{
			TcsGenericId flvrId (idx,0);
			TcsNameMap flvrMap (flvrId,csMapFlavorName,idx,0UL,wcPtr);
			Add (flvrMap);		//lint !e534   ignoring return value
		}
	}
}									//lint !e1565    this initializer does not initialize ALL class members (eg RecordDuplicates)
void TcsNameMapper::AdjustDefaultIDs (void)
{
	EcsNameFlavor flavor;
	unsigned long genericId;

	for (EcsNameFlavor idx = csMapFlvrNone;idx < csMapFlvrUnknown;++idx)
	{
		InitialDfltFlvrIds [idx] = KcsNameMapInitial;		//lint !e662
	}

	iterator setItr;
	for (setItr = DefinitionSet.begin ();setItr != DefinitionSet.end ();setItr++)
	{
		genericId = (setItr->GetGenericId ()).GetGenericId ();
		genericId %= KcsNameMapBias;
		if (genericId > KcsNameMapInitial)
		{
			flavor = setItr->GetFlavor ();
			if (InitialDfltFlvrIds [flavor] < genericId)
			{
				InitialDfltFlvrIds [flavor] = genericId;
			}
		}
	}
}									//lint !e1565    this initializer does not initialize ALL class members (eg RecordDuplicates)
///////////////////////////////////////////////////////////////////////////////
// Given a generic ID (as an unsigned long) locate the first record in the set
// sequence which has the given generic ID.  Due to the ordering of the set,
// the entry returned is always that record with the lowest dup sort, and
// the real name (rather than any alias) is favored.
//
// This is, essentially, what we want to use when we are outputting WKT.  That
// is, we always get the standard name (no aliases).  Since duplicate names
// are the same, it really doesn't matter which one we get; but we will always
// get the one with the given generic ID and with DupSort == 0.
TcsNameMap* TcsNameMapper::LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id)
{
	TcsNameMap *nmMapPtr = 0;
	iterator beginItr;
	iterator endItr;
	iterator searchItr;

	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetNumericId () == id)
		{
//			nmMapPtr = &(*searchItr);
//	Change required to achieve compilation on Linux, gcc 3.2.2
//  I can't see why the const_cast is necessary.
			nmMapPtr = const_cast<TcsNameMap*>(&(*searchItr));
			break;
		}
	}
	return nmMapPtr;
}
const TcsNameMap* TcsNameMapper::LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const
{
	TcsNameMap *nmMapPtr = 0;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;

	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetNumericId () == id)
		{
//			nmMapPtr = &(*searchItr);
//	Change required to achieve compilation on Linux, gcc 3.2.2
//  I can't see why the const_cast is necessary.
			nmMapPtr = const_cast<TcsNameMap*>(&(*searchItr));
			break;
		}
	}
	return nmMapPtr;
}
// Similar to the above, but returns an iterator rather than a pointer.
// Sometimes an iterator is more convenient, sometimes a pointer is more
// convenient.
TcsNameMapper::iterator TcsNameMapper::LocateNameMapItr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id)
{
	iterator beginItr;
	iterator endItr;
	iterator searchItr;
	iterator returnItr;

	returnItr = DefinitionSet.end ();
	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetNumericId () == id)
		{
			returnItr = searchItr;
			break;
		}
	}
	return returnItr;
}
TcsNameMapper::const_iterator TcsNameMapper::LocateNameMapItr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const
{
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;
	const_iterator returnItr;

	returnItr = DefinitionSet.end ();
	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (searchItr->GetNumericId () == id)
		{
			returnItr = searchItr;
			break;
		}
	}
	return returnItr;
}
///////////////////////////////////////////////////////////////////////////////
// Given a name, locate the first member of the std::set of the same type and
// flavor with that name.  This will locate an alias if the name is a valid
// alias.  Due to the set collating sequence, we will find either the real name
// or an alias name, whatever is provided.  Having two members with the same
// name with one marked as an alias is not supposed to happen, but it will not
// produce invalidd results.  The std::set member marked as an alias, in this
// case, will always be ignored.
//
// This is the function we want to use on input.  That is, we want to locate
// an item by its alias; while we never want to actually output an alias name.
//
// An item can have several aliases, but since the alias names will all be
// different (if not, why bother), there is no contention between AliasFlag
// and DupSort.  Should this occur for whatever reason, the correct results
// will still be produced.  That is, the generic ID of the first item in the
// std::set collating sequence with the name provided will be returned.
TcsNameMap* TcsNameMapper::LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name)
{
	TcsNameMap *nmMapPtr = 0;
	iterator beginItr;
	iterator endItr;
	iterator searchItr;

	TcsNameMap beginSearchObj (type,flavor,name,0,0);
	TcsNameMap endSearchObj (type,flavor,name,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (!CS_wcsicmp (searchItr->GetNamePtr (),name))
		{
//			nmMapPtr = &(*searchItr);
//	Change required to achieve compilation on Linux, gcc 3.2.2
//  I can't see why the const_cast is necessary.
			nmMapPtr = const_cast<TcsNameMap*>(&(*searchItr));
			break;
		}
	}
	return nmMapPtr;
}
// Same as above, but works on constant map objects.  This is necessary (???)
// as the iterators need to be const_iterators for a constant object.
const TcsNameMap* TcsNameMapper::LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name,short dupSort) const
{
	const TcsNameMap *nmMapPtr = 0;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;

	TcsNameMap beginSearchObj (type,flavor,name,0,0);
	TcsNameMap endSearchObj (type,flavor,name,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		if (!CS_wcsicmp (searchItr->GetNamePtr (),name))
		{
			nmMapPtr = &(*searchItr);
			break;
		}
	}
	return nmMapPtr;
}
// Entries in the set are not sorted by numeric ID, so the values returned by
// this function WILL NOT be in any kind of sorted order.
unsigned long TcsNameMapper::LocateIdByIdx (EcsMapObjType type,EcsNameFlavor flavor,
															   unsigned index,
															   unsigned* count) const
{
	unsigned counter = 0;
	unsigned long rtnValue = 0xFFFFFFFFUL;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;
	
	TcsNameMap beginSearchObj (type,flavor,FirstName,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	searchItr = beginItr;
	while (searchItr != endItr)
	{
		if (searchItr->GetAliasFlag () == 0)
		{
			if (counter == index)
			{
				break;
			}
			++counter;
		}
		++searchItr;
	}
	if (searchItr != endItr)
	{
		rtnValue = searchItr->GetNumericId ();
	}
	if (count != 0)
	{
		*count = counter;
	}
	return rtnValue;
}
const wchar_t* TcsNameMapper::LocateNameByIdx (EcsMapObjType type,EcsNameFlavor flavor,
																  unsigned index,
																  unsigned* count) const
{
	unsigned counter = 0;
	const wchar_t* rtnValue = 0;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;
	
	TcsNameMap beginSearchObj (type,flavor,FirstName,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999);
	
	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	searchItr = beginItr;
	while (searchItr != endItr)
	{
		if (counter == index)
		{
			break;
		}
		++counter;
		++searchItr;
	}
	if (searchItr != endItr)
	{
		rtnValue = searchItr->GetNamePtr ();
	}
	if (count != 0)
	{
		*count = counter;
	}
	return rtnValue;
}
///////////////////////////////////////////////////////////////////////////////
// Enumerator function.
TcsNameMapList* TcsNameMapper::Enumerate (EcsMapObjType type,EcsNameFlavor flavor,bool deprecated)
{
	const TcsNameMap *nmMapPtr = 0;
	const_iterator beginItr;
	const_iterator endItr;
	const_iterator searchItr;

	TcsNameMapList* nameMapList = new TcsNameMapList ();

	TcsNameMap beginSearchObj (type,flavor,FirstName,0,0);
	TcsNameMap endSearchObj (type,flavor,LastName,9999,9999);

	beginItr = DefinitionSet.lower_bound (beginSearchObj);
	endItr   = DefinitionSet.upper_bound (endSearchObj);
	for (searchItr = beginItr;searchItr != endItr;searchItr++)
	{
		nmMapPtr = &(*searchItr);
		if (!deprecated ^ (nmMapPtr->DeprecatedBy()).IsNotKnown ())
		{
			nameMapList->AddNameMap (nmMapPtr);
		}
	}
	return nameMapList;
}
///////////////////////////////////////////////////////////////////////////////
// Manufacturing Functions
bool TcsNameMapper::AddKeyNameMap (EcsMapObjType mapType,const wchar_t* mapFilePath)
{
	bool ok = true;
	EcsNameFlavor flavor;
	unsigned long genericId;
	TcsCsvStatus csvStatus;

	TcsKeyNameMapFile mapFileObj (mapFilePath,28);
	ok = (mapFileObj.GetStatus (csvStatus) == csvOk);

	if (ok)
	{
		// Loop once for each record in the mapping table.
		do
		{
			// Get a default flavor.
			flavor = KeyMapFlavor (mapFileObj);

			// Extract a generic ID from this record.
			genericId = KeyMapGenericId (mapFileObj);
			if (genericId == 0UL)
			{
				// There were no numbers in the provided record.  Need to generate
				// a flavor dependent generic ID.  To do this, we need to find the
				// flavor of the base name we will use.
				genericId = GetNextDfltId (flavor);
			}
			ok = AddKeyMapFields (mapType,genericId,mapFileObj);
		} while (ok && mapFileObj.NextRecord ());
	}
	return ok;
}
bool TcsNameMapper::AddKeyMapFields (EcsMapObjType mapType,unsigned long genericId,const TcsKeyNameMapFile& mapFileObj)
{
	bool ok;
	bool addDups;
	unsigned long itmNbr;
	EcsNameFlavor flavor = csMapFlvrNone;
	EcsMapTableFields nbrItmId;
	EcsMapTableFields nameItmId;
	std::wstring sourceId;
	std::wstring itmName;

	addDups = (mapType == csMapParameterKeyName) ||
			  (mapType == csMapProjectionKeyName) ||
			  (mapType == csMapLinearUnitKeyName) ||
			  (mapType == csMapAngularUnitKeyName);

	++flavor;
	for (;flavor != csMapFlvrUnknown;++flavor)
	{
		ok = true;
		itmNbr = 0UL;
		nbrItmId = mapFileObj.GetNbrFldId (flavor);
		if (nbrItmId != csMapFldUnknown)
		{
			// Numbers are optional.
			itmNbr = mapFileObj.GetFieldAsUL (nbrItmId);
			ok = (itmNbr != mapFileObj.GetErrorValue ());
		}
		if (ok)
		{
			// Names are not optional.
			nameItmId = mapFileObj.GetNameFldId (flavor);
			ok = nameItmId != csMapFldUnknown;
			if (ok)
			{
				ok = mapFileObj.GetField (itmName,nameItmId);
				if (ok)
				{
					ok = !itmName.empty ();
				}
			}
			if (ok)
			{
				// If we have some aliases, they will be present in the item name,
				// separated by vertical bar characters.  The first name is always
				// the real name.
				std::wstring::size_type start = 0;
				std::wstring::size_type end = 0;
				do
				{
					end = itmName.find (L'|',start);
					if (end == std::wstring::npos)
					{
						end = itmName.length ();
					}
					std::wstring nextName = itmName.substr (start,end - start);

					TcsNameMap newItem (genericId,mapType,flavor,itmNbr,nextName.c_str ());
					if (start != 0)
					{
						newItem.SetAliasFlag (1);
					}

					// We obtain a string which identifies the source.  The Add
					// overload that we use here will add the source to the
					// comments field if the record is added to the Duplicates
					// name map.
					mapFileObj.GetFileRecordId (sourceId);
					ok = Add (newItem,addDups,sourceId.c_str ());
					start = end + 1;
				} while (end < itmName.length ());
			}
		}
	}
	return true;
}
EcsNameFlavor TcsNameMapper::KeyMapFlavor (const TcsKeyNameMapFile& mapFileObj) const
{
	EcsNameFlavor flavor = csMapFlvrNone;
	unsigned long itmNbr;
	EcsMapTableFields nbrItmId;
	EcsMapTableFields nameItmId;
	std::wstring itmName;

	++flavor;
	for (;flavor != csMapFlvrUnknown;++flavor)
	{
		// Check for the presence of a number for this flavor.
		nbrItmId = mapFileObj.GetNbrFldId (flavor);
		if (nbrItmId != csMapFldUnknown)
		{
			itmNbr = mapFileObj.GetFieldAsUL (nbrItmId);
			if (itmNbr != mapFileObj.GetErrorValue () && itmNbr != 0UL)
			{
				break;
			}
		}

		// Check for the presence of a name for this flavor.
		nameItmId = mapFileObj.GetNameFldId (flavor);
		if (nameItmId != csMapFldUnknown)
		{
			bool ok = mapFileObj.GetField (itmName,nameItmId);
			if (ok && !itmName.empty ())
			{
				break;
			}
		}
	}
	return flavor;
}
unsigned long TcsNameMapper::KeyMapGenericId (const TcsKeyNameMapFile& mapFileObj) const
{
	EcsNameFlavor flavor = csMapFlvrNone;
	EcsMapTableFields nbrItmId;
	unsigned long itmNbr;
	unsigned long genericId = 0UL;

	++flavor;
	for (;flavor != csMapFlvrUnknown;++flavor)
	{
		if (flavor == csMapFlvrNone)
		{
			continue;
		}
		// Check for the presence of a number for this flavor.
		nbrItmId = mapFileObj.GetNbrFldId (flavor);
		if (nbrItmId != csMapFldUnknown)
		{
			// There is a number field for this flavor in the key map file.
			itmNbr = mapFileObj.GetFieldAsUL (nbrItmId);
			if (itmNbr != mapFileObj.GetErrorValue () && itmNbr != 0UL)
			{
				// There is a non-zero number in that field.  It becomes the
				// genericId, after suitable flavoring.
				long bias = static_cast<int>(flavor) - 1;
				bias *= KcsNameMapBias;
				genericId = itmNbr + bias;
				break;
			}
		}
	}
	return genericId;
}
