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

#if !defined (csCsvFileSupportHpp)
#define csCsvFileSupportHpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>

#ifndef wcCount
inline size_t wcCount(const wchar_t* wcArray)
{
    return  size_t(sizeof(wcArray)) / size_t(sizeof(wchar_t));
}
#endif

//=============================================================================
// EcsCsvStatus Object -- Enumerates the various status values associated with
//                        TcsCsv objects.
//=============================================================================
enum EcsCsvStatus {    csvOk = 0,          // Unremarkable completion
					   csvEof,             // Last record has already been read.
					   csvNoFile,          // CSV file open failed
					   csvEmpty,           // Warning: returned field is empty
					   csvEndInQuote,      // Record ended within a quoted field
					   csvInvLineBrk,      // Line break not in quote.
					   csvLastWasEsc,      // Last character in record was an
										   // escape character
					   csvAmbigQuote,      // nonspace data after quote, before
										   // separator.
					   csvInternal,        // Internal error in parser.
					   csvInvRecordId,     // Record key value failed to match
										   // an existing record.
					   csvNoRecords,       // No records in the CsvFile object
					   csvNoFields,        // No fields in the CsvReocrd object
					   csvInvRecordNbr,    // Invalid record number.
					   csvInvFieldId,      // Invalid field identifier.
					   csvInvFieldNbr,     // Invalid field number.
					   csvInvRecord,       // improper record format.
					   csvRecordTooLong,   // record is too long.
					   csvFieldTooLong,    // Field is too long.
					   csvTooManyFields,   // Too many fields for record.
					   csvTooFewFields,    // Not enough fields in record.
					   csvNoFldLabels,     // No field labels in this CsvFile object. 
					   csvLblsOnAppend,    // Labels no allowed on append.
					   csvBogusValue,      // A bogus value was presented as a field value.
					   csvDupIndex,        // INdex creation encountered a dup[;icate value
					   csvEndOfTable = 999 // Used to mark the end of tables, etc.
				  };

struct TcsCsvMsgTbl
{
	EcsCsvStatus StatusValue;
	wchar_t Message [80];
};

//newPage//
// Standalone function support.
//	csGetCsvRecord: extracts a csvRecord from a stream, allowing for new lines in a quoted field.
//	csCsvFieldParse: parses a csvRecord returning a vector of std::wstrings representing the individual fields.
//	csCsvQuoter: quotes a field in place, if necessary, for output.
//	csQuoteCsvField: returns a quoted a field, only quoted if necessary, for output.
EcsCsvStatus csGetCsvRecord (std::wstring& csvRecord,std::wistream& iStrm,const wchar_t* delimiters = 0);
EcsCsvStatus csCsvFieldParse (std::vector<std::wstring>& fields,const std::wstring& csvRecord,const wchar_t *delimiters = 0);
bool csCsvQuoter (char* csvField,size_t csvSize,const char* delimiters = 0);
bool csCsvQuoter (std::wstring& csvField,bool forceIt = false,const wchar_t* delimiters = 0);
std::wstring csQuoteCsvField (const std::wstring& csvField,bool forceIt = false,const wchar_t* delimiters = 0);
const char* CS_ccPad (int length);
const wchar_t* CS_wcPad (int length);

//newPage//
//=============================================================================
// TcsCsvStatus Object -- Encapsulates the functionality of a Comma Separated
//                        Value status report.
// Should any of these objects throw an execption, this is what they will
// throw.
//=============================================================================
class TcsCsvStatus
{
public:
	//=========================================================================
	// Construction, Destruction, Assignment
	TcsCsvStatus (EcsCsvStatus status = csvOk);
	TcsCsvStatus (EcsCsvStatus status,unsigned long lineNbr,short fieldNbr);
	TcsCsvStatus (EcsCsvStatus status,unsigned long lineNbr,short fieldNbr,
															const std::wstring& fieldId);
	TcsCsvStatus (const TcsCsvStatus& source);
	~TcsCsvStatus (void);
	TcsCsvStatus& operator= (const TcsCsvStatus& rhs);
	//=========================================================================
	// Named Member Functions
	void SetStatus (EcsCsvStatus status);
	void BumpLineNbr (void);
	void SetLineNbr (unsigned long lineNbr);
	void SetFieldNbr (short fieldNbr);
	void SetFieldId (const std::wstring& fieldId);
	void SetObjectName (const std::wstring& objectName);
	std::wstring GetMessage (void);
	//=========================================================================
	// Public Named Data Members
	EcsCsvStatus StatusValue;
	unsigned long LineNbr;
	short FieldNbr;
	std::wstring FieldId;
	std::wstring ObjectName;    
};
//=============================================================================
// TcsCsvRecord Object -- Encapsulates the functionality of a Comma Separated
//                        Value Record.
// Top avoid the complications of throwing exceptions, we don't throw
// explicitly.  Obviously, the underlying STL containers may throw.
//
// Generally, functions return a boolean value and require a TcsCsvStatus as
// an argument.  Functions return false if any kind of exceptional condition
// is encountered, and reason and information about the condition is returned
// in the provided TcsCsvStatus object.  Obviously, this object cannot provide
// status object elements such as object name and line number.  The user of
// this object must provide them.  Suggested that users declare an TcsCsvStatus
// object and set the Object name, and update the line number, within it, and
// simply let this oobject supply the condition and the field number.
//
// Thus, no execptions from this object other than standard STL stuff which
// should be very very rare.
//=============================================================================
class TcsCsvRecord
{
public:
	typedef std::vector<std::wstring>::iterator fldItr;
	typedef std::vector<std::wstring>::const_iterator fldItrK;
	//=========================================================================
	// Construction, Destruction, Assignment
	TcsCsvRecord (void);
	TcsCsvRecord (const TcsCsvRecord& source);
	virtual ~TcsCsvRecord (void);
	TcsCsvRecord& operator= (const TcsCsvRecord& rhs);
	//=========================================================================
	// Public Named Member Functions
	bool Empty (void) const;
	void Clear (void);
	void Reserve (unsigned expectedFldCnt);
	short FieldCount (void) const;
	short SetMinFldCnt (short minFldCnt);
	short SetMaxFldCnt (short maxFldCnt);
	bool ReplaceField (const std::wstring& newField,short fieldNbr,TcsCsvStatus& status);
	bool AppendField (const std::wstring& newField,TcsCsvStatus& status);
	bool InsertField (const std::wstring& newField,short before,TcsCsvStatus& status);
	bool RemoveField (short fieldNbr,TcsCsvStatus& status);
	bool GetField (std::wstring& field,short fieldNbr,TcsCsvStatus& status) const;
	short FindField (const std::wstring& fieldId,TcsCsvStatus& status) const;
	bool ReplaceRecord (const std::wstring& newRecord,TcsCsvStatus& status,const wchar_t* delimiters = 0);
	bool ReplaceRecord (std::wistream& iStrm,TcsCsvStatus& status,const wchar_t* delimiters = 0);
	bool ReturnAsRecord (std::wstring& record,TcsCsvStatus& status,const wchar_t* delimiters = 0) const;
	bool WriteToStream (std::wostream& oStrm,TcsCsvStatus& status,const wchar_t* delimiters = 0) const;
private:
	short MinFldCnt;
	short MaxFldCnt;
	std::vector<std::wstring> Fields;
};
//newPage//
//=============================================================================
// CSV File Sort Function Object
//
// Create an object of this type to specify the fields which are to be used in
// a CSV file sort operation.  Then provide the object to the CSV file sort
// function.  Note that the sort field numbers are public members.
//
class TcsCsvSortFunctor
{
public:
	enum EcsCsvCmpType { csCmpTypString = 0,
						 csCmpTypStringI,
						 csCmpTypInteger,
						 csCmpTypeReal
					   };
	//=============================================================================
	// Construction  /  Destruction  /  Assignment
	TcsCsvSortFunctor (short first,short second = -1,short third = -1,short fourth = -1);
	TcsCsvSortFunctor (const TcsCsvSortFunctor& source);
	~TcsCsvSortFunctor (void);
	TcsCsvSortFunctor& operator= (const TcsCsvSortFunctor& rhs);
	//=============================================================================
	// Operator Overrides
	bool operator() (const TcsCsvRecord& recordOne,const TcsCsvRecord& recordTwo);
	//=============================================================================
	// All data fields are public!!!
	short FirstField;
	short SecondField;
	short ThirdField;
	short FourthField;
	TcsCsvStatus CsvStatus;
private:
	enum EcsCmpResult { cmpLessThan    = -1,
						cmpEqualTo     =  0,
						cmpGreaterThan =  1
					  };
	EcsCmpResult CsvFieldCompare (const std::wstring& fieldOne,const std::wstring& fieldTwo) const;						
};
//newPage//
//=============================================================================
// TcsCsvFileBase Object -- Encapsulates the functionality of a Comma Separated
//                          Value file.
//=============================================================================
class TcsCsvFileBase
{
	//=========================================================================
	// Static Functions, Variables, and Constants.
	typedef std::vector<TcsCsvRecord>::const_iterator recItrK;
	typedef std::vector<TcsCsvRecord>::iterator recItr;
protected:
	static const unsigned InvalidRecordNbr;
public:
	static unsigned GetInvalidRecordNbr (void);
	//=========================================================================
	// Construction, Destruction, and Assignment
	TcsCsvFileBase (bool firstIsLabels,short minFldCnt,short maxFldCnt,const wchar_t* delimeters = 0);
	TcsCsvFileBase (const TcsCsvFileBase& source);
	virtual ~TcsCsvFileBase (void);
	TcsCsvFileBase& operator= (const TcsCsvFileBase& rhs);
	//=========================================================================
	// Operator Overrides
	//=========================================================================
	// Public Named Member Functions
	bool HasLabels (void) const;
	short SetMinFldCnt (short newCnt = -1);
	short SetMaxFldCnt (short newCnt = -1);
	bool SetDelimiters (const wchar_t* delimiters = 0);
	void SetObjectName (const std::wstring& objectName);
	unsigned RecordCount (void) const;
	short FieldCount (unsigned recordNbr) const;
	short GetFldNbr (const wchar_t* fieldId,TcsCsvStatus& status) const;
	bool GetFieldId (std::wstring& filedId,short fieldNbr,TcsCsvStatus& status) const;
	void Clear (void);
	bool SetRecordKeyField (const wchar_t* fieldId,TcsCsvStatus& status);
	bool SetRecordKeyField (short fieldNbr,TcsCsvStatus& status);
	const TcsCsvRecord& GetAllLabels (void) const {return Labels; };
	bool GetFieldLabel (std::wstring& label,short fieldNbr) const;
	bool GetFieldLabel (std::wstring& label,short fieldNbr,TcsCsvStatus& status) const;
	bool SetFieldLabel (const wchar_t* label,short fieldNbr);
	bool SetFieldLabel (const wchar_t* label,short fieldNbr,TcsCsvStatus& status);
	void ResetAllLabels (void);
	void SetAllLabels (const TcsCsvRecord& newLabels);
	bool GetField (std::wstring& field,unsigned recordNbr,short fieldNbr,TcsCsvStatus& status) const;
	bool GetField (std::wstring& field,unsigned recordNbr,const wchar_t* fieldId,
														  TcsCsvStatus& status) const;
	bool ReplaceField (const std::wstring& newValue,unsigned recordNbr,short fieldNbr,
																	   TcsCsvStatus& status);
	bool ReplaceField (const std::wstring& newValue,unsigned recordNbr,const wchar_t* fieldId,
																	   TcsCsvStatus& status);
	bool GetRecord (TcsCsvRecord& record,unsigned recordNbr,TcsCsvStatus& status) const;
	bool Locate (unsigned& recordNumber,const wchar_t* srchString) const;
	bool Locate (unsigned& recordNumber,short fieldNbr,const wchar_t* srchString,bool honorCase = false) const;
	bool Locate (unsigned& recordNumber,const wchar_t* fieldId,const wchar_t* srchString,bool honorCase = false) const;
	bool LocateNext (unsigned& recordNumber,short fieldNbr,const wchar_t* srchString,bool honorCase) const;
	bool LocateNext (unsigned& recordNumber,const wchar_t* fieldId,const wchar_t* srchString,bool honorCase = false) const;
	bool StableSort (const TcsCsvSortFunctor& functor);
	unsigned LowerBound (const TcsCsvRecord& searchRec,const TcsCsvSortFunctor& functor);
	bool ReadFromStream (std::wistream& iStrm,bool firstIsLabels,TcsCsvStatus& status);
	bool WriteToStream (std::wostream& oStrm,bool writeLabels,TcsCsvStatus& status) const;
	const wchar_t* GetObjectName (void) const;
	void GetObjectName (std::wstring& objName) const;
protected:
	//=========================================================================
	// Protected Named Member Functions
	bool BuildIndex (TcsCsvStatus& status);
	//=========================================================================
	// Protected Data Members
	bool FirstIsLabels;
	bool IsSorted;
	bool IsIndexed;
	wchar_t Separator;
	wchar_t Quote;
	wchar_t Escape;
	short KeyField;
private:
	//=========================================================================
	// Private Data Members
	short MinFldCnt;
	short MaxFldCnt;
	std::wstring ObjectName;
	TcsCsvRecord Labels;
	std::vector<TcsCsvRecord> Records;
	std::map<std::wstring,unsigned> Index;
};

#endif
