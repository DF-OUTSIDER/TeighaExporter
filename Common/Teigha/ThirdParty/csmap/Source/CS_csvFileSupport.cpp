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

//lint -esym(534,TcsCsvFileBase::SetDelimiters)		(ignoring return value)
//lint -esym(534,TcsCsvRecord::SetMinFldCnt)		(ignoring return value)
//lint -esym(534,TcsCsvRecord::SetMaxFldCnt)		(ignoring return value)

// The original intent of the designers of this module was to keep this module
// completely independent of CS-MAP.  However, since ANSI seem to be obstinant
// about adding wcsicmp to the standard, we need access to the CS_wcsicmp
// function in CS-MAP.  We could have duplicated the CS_wcsicmp code here, but
// we chose not to.  Anyway, wcsicmp is the only reason cs_map.h is included
// here (at least as of this writing: March 2009).  If the include of
// "cs_map.h" is removed for any reason, you'll need to add and include of
// <math.h> to get this to complie.

// Now comes May 21, 2014 
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.
//lint -e766    Disable PC-Lint's warning of unreferenced headers.
//lint -efile(*,xutility)   turn off messages from STL include files

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include <math.h>

static const TcsCsvMsgTbl KcsCsvMsgTbl [] =
{
	{            csvOk, L"OK!"                                                                              },
	{           csvEof, L"Eod encountered in a quoted field"                                                },
	{        csvNoFile, L"CSV file open failed"                                                             },
	{         csvEmpty, L"Requested field is empty"                                                         },
	{    csvEndInQuote, L"Record ended within a quoted field"                                               },
	{    csvInvLineBrk, L"Unquoted new line in what was presented as a parsed CSV record"                   },
	{    csvLastWasEsc, L"Text form of CSV record ended with an escape character"                           },
	{    csvAmbigQuote, L"Non-whitespace character(s) after end of quoted field"                            },
	{      csvInternal, L"Internal software error, please report to developer"                              },
	{   csvInvRecordId, L"Invalid key presented for indexed fetch"                                          },
	{     csvNoRecords, L"The CSV file object is empty, no data records present"                            },
	{      csvNoFields, L"The indicated reocrd does not have any data fields"                               },
	{  csvInvRecordNbr, L"Record number provided in request is invalid"                                     },
	{    csvInvFieldId, L"Field name provided in request is invalid"                                        },
	{   csvInvFieldNbr, L"Field number provided in request is invalid"                                      },
	{     csvInvRecord, L"What was presented as a CSV record is not properly formatted"                     },
	{ csvRecordTooLong, L"Record is too long, suspect a CSV format error"                                   },
	{  csvFieldTooLong, L"Field is too long, suspect a CSV format error"                                    },
	{ csvTooManyFields, L"Record has too many fields for a valid record, pssoble CSV format error"          },
	{  csvTooFewFields, L"Record has less than the required number of fields for a valid record"            },
	{   csvNoFldLabels, L"Current file does not have field labels, request is invalid"                      },
	{  csvLblsOnAppend, L"Attempt to add a label record to a non-empty CSV file object"                     },
	{    csvBogusValue, L"Value presented for field is invalid"                                             },
	{      csvDupIndex, L"Index creation/update encountered a duplicate key value"                          },
	{    csvEndOfTable, L"Software error: no message coded for CSV file exceptional condition"              }
};

///////////////////////////////////////////////////////////////////////////////
// csGetCsvRecord -- Extracts a record from a .csv file as a wstring.
//
// Extracts a .csv record from a stream.  Essentially a "get line of text into
// a wstring" function, except that this is smart enough to be able to recognize
// and simply copy new-line characters within a quoted field.  The function
// clears the provided string before processing, and DOES NOT include the
// terminating new line.  An empty line causes an empty string to be returned.
// Users are advised to use 'reserve' method on the csvRecord parameter before
// passing to this function to preserve performance.
//
// Returns a value of the EcsCsvStatus enumeration.  Possible values are:
// csvOk            -> record was extracted successfully
// csvRecordTooLong -> a resulting string with more than 100,000 characters
// csvEndInQuote    -> encountering a end-of-file within a quoted string
// csvLastWasEsc    -> a file whose last character is not a new line character
// csvInvRecord     -> invalid record format with regard to quoted fields
// csvInternal      -> internal state machine failure
//
//lint -save -esym(788,done,error)  enumeration names not used in switch
EcsCsvStatus csGetCsvRecord (std::wstring& csvRecord,std::wistream& iStrm,const wchar_t* delimiters)
{
	EcsCsvStatus csvStatus = csvOk;

	// This is a state machine, this enum defines the various states.
	enum {error, preamble, copyingFields, copyingQuotedField, lastWasEscape, lastWasQuote, done} state;
	wint_t wcAsInt;
	wchar_t wc;

	wchar_t separator = L',';
	wchar_t quote     = L'\"';
	wchar_t escape    = L'\"';

	// Process the user supplied delimiters, if any.
	if (delimiters != 0)
	{
		if (*delimiters != L'\0')
		{
			separator = *delimiters;
			if (*(delimiters + 1) != L'\0')
			{
				quote = *(delimiters + 1);
				if (*(delimiters + 2) != L'\0')
				{
					escape = *(delimiters + 2);
				}
			}
		}
	}

	csvRecord.clear ();
	state = preamble;
	while (state != done && state != error)
	{
		wcAsInt = iStrm.get ();
		if (wcAsInt == WEOF)		/*lint !e1924   C style cast, in WEOF definition */
		{
			break;
		}
		wc = static_cast<wchar_t>(wcAsInt);
		if (wc == L'\r')
		{
			// We choose not to try to deal with carriage returns; we see no
			// value in that for anyone.
			continue;
		}

		switch (state) {
		case preamble:
			if (wc == quote)
			{
				state = copyingQuotedField;
			}
			else if (wc == L'\n')
			{
				state = done;
			}
			else if (wc == separator)
			{
				state = preamble;
			}
			else
			{
				state = copyingFields;
			}
			break;

		case copyingFields:
			if (wc == L'\n') state = done;
			if (wc == separator) state = preamble;
			break;

		case copyingQuotedField:
			if (wc == escape)
			{
				if (escape == quote)
				{
					state = lastWasQuote;
				}
				else
				{
					state = lastWasEscape;
				}
			}
			break;

		case lastWasQuote:
			if (wc == L'\n')
			{
				state = done;
			}
			else if (wc == quote)
			{
				state = copyingQuotedField;
			}
			else if (wc == separator)
			{
				state = preamble;
			}		
			else
			{
				state = copyingFields;
			}
			break;

		case lastWasEscape:
			state = copyingQuotedField;
			break;

		default:
			state = error;
			csvStatus = csvInternal;
			break;
		}
		if (state != done && state != error)
		{
			csvRecord += wc;
		}
		if (csvRecord.length () >= 100000)
		{
			state = error;
			csvStatus = csvRecordTooLong;
			break;
		}
	}

	switch (state) {
	case error:
		// csvStatus already set appropriately.
		break;
	case done:
		csvStatus = csvOk;
		break;
	case preamble:
		csvStatus = csvOk;
		break;
	case copyingFields:
		csvStatus = csvOk;
		break;
	case copyingQuotedField:
		csvStatus = csvEndInQuote;
		break;
	case lastWasEscape:
		csvStatus = csvLastWasEsc;
		break;
	case lastWasQuote:
		csvStatus = csvInvRecord;
		break;
	default:
		csvStatus = csvInternal;
		break;
	}
	return csvStatus;
}
//lint -restore
///////////////////////////////////////////////////////////////////////////////
// csCsvFieldParse  --  Parses a CSV record into separate fields.
//
// The following does a CSV parse.  Note that:
//
// 1> there MUST NOT be a trailing line break in the provided csvRecord
//    argument.
// 2> the delimiters argument can provide user specified delimeters:
//		[0] == separator
//		[1] == quote
//		[2] == escape
// 3> if delimiters is the null pointer, hardcoded default values are used:
//		separator == comma,
//		quote     == double quote
//		escape    == double quote
// 4> regardless how set, if the quote character equals the escape
//    character, the quote character is escaped in a quoted field only
//    by two consecutive quote characters.
// 5> regardless how set, if the escape character is not the same as
//    the quote character, the escape character will escape anything
//    in a quoted field, including the quote character of course.
// 6> this function executes a fields.clear () on the fields agrument.
// 7> this function operates on 16 bit UNICODE characters only.
// 8> the fields vector is populated with a std::wstring for each
//    field in the provided record as determined by the separator
//    character.  An empty field produces an empty std::wstring.
//    The returned std::wstrings may include line breaks if such
//    were present within a quoted value.
// 9> the csGetCsvRecord defined above is useful in extracting
//    records from a CSV file when a quoted field may have
//    line breaks in it; often the case when the CSV file is
//    exported by Acess, Excel, or similar programs.
//
// Returns a value of the EcsCsvStatus enumeration.  Possible values are:
//
//  csvOk            -> record successfully parsed
//  csvEndInQuote    -> Record ended within a quoted field
//  csvInvLineBrk    -> Line break not in quote.
//  csvLastWasEsc    -> Last character in record was an escape character
//  csvAmbigQuote    -> Non-space data after quote, before separator
//  csvInvRecord     -> improper record format.
//  csvFieldTooLong  -> A field longer than 10,000 characters was encountered
//  csvTooManyFields -> Record appeared to have more than 300 fields.
//  csvInternal      -> Internal error in parser
//
//..10,000 character limit and 300 field limit were arbitrarily chosen simply
//..to prevent infinte loop/crash when processing a completely bogus file;
//..that is bogus from a CSV standpoint of view.
//
//  The function itself does not throw, but obviously uses std::vector and
//  std::wstring STL components which may throw under wierd circumstances.
//
//lint -save
//lint -esym(788,csvState::csvDone,csvState::csvError)  // enumeration values not used in switch
EcsCsvStatus csCsvFieldParse (std::vector<std::wstring>& fields,const std::wstring& csvRecord,const wchar_t *delimiters)
{
	EcsCsvStatus status = csvOk;
	
	// This is a state machine, these are the states.
	enum csvState {	csvNone = 0,		// not set yet.
					csvBegTrim,			// Trim leading whitespace on current field
					csvQuoted,			// Processing quoted field content, escape == quote
					csvQuotedEsc,		// Processing quoted field content, escape != quote
					csvUnQuoted,		// Processing unquoted field content
					csvEndTrim,			// Trimming 'stuff' after end of quoted field 
					csvDone,			// completed entire line
					csvError			// ecnountered an error in CSV format.
				  } state;

	wchar_t wc;
	wchar_t separator = L',';
	wchar_t quote     = L'\"';
	wchar_t escape    = L'\"';

	// Process the user supplied delimiters, if any.
	if (delimiters != 0)
	{
		if (*delimiters != L'\0')
		{
			separator = *delimiters;
			if (*(delimiters + 1) != L'\0')
			{
				quote = *(delimiters + 1);
				if (*(delimiters + 2) != L'\0')
				{
					escape = *(delimiters + 2);
				}
			}
		}
	}

	// Initialize the parse of the data.
	fields.clear ();
	std::wstring field;
	field.reserve (200);
	std::wstring::const_iterator itr = csvRecord.begin ();
	state = csvBegTrim;

	// Here we go.
	while (state != csvDone && state != csvError)
	{
		if (itr == csvRecord.end ())
		{
			// We are at the end of the line of text.
			if (state == csvQuoted || state == csvQuotedEsc)
			{
				// Record ended in the middle of a quoted field.
				state = csvError;
				status = csvEndInQuote;
			}
			else
			{
				// Record ended normally.  If state is csvEndTrim, the field
				// will have already been pushed onto the fields vector.
				if (state != csvEndTrim)
				{
					fields.push_back (field);
				}
				state = csvDone;
			}
		}
		else
		{
			// We have a non-null character to process, i.e. we are not done.
			wc = *itr++;
			if (wc == L'\r')
			{
					continue;
			}
			switch (state) {
			case csvBegTrim:
				// We are looking for the beginning of the field.
				if (wc == quote)
				{
					// First non-whitespace character is a quote, this is a
					// quoted field.  The quote is not considered part of the
					// data for the field.
					state = (escape == quote) ? csvQuoted : csvQuotedEsc;
				}
				else if (!iswspace (wc))
				{
					// Not whitespace, and not a quote.  Must be the beginning
					// of a non-quoted field.  We start a new field by stashing
					// the value of the current field (which may be empty) and
					// initializing the std::wstring field object with the new
					// value.
					if (wc == separator)
					{
						// If the first character of the data is the separator,
						// the field is the null field.
						fields.push_back (field);
						field.clear ();
					}
					else
					{
						// We have an unquoted field to extract.
						state = csvUnQuoted;
						field += wc;
					}
				}
				// Otherwise, we have leading whitespace on the field, and
				// we simply ignore it, it does not become part of the field
				// data.  We do this specifically to prevent a whitespace
				// character which immediately preceeds a quoted field from
				// screwing up the whole world.
				break;

			case csvUnQuoted:
				if (wc == L'\n')
				{
					// Should not see an unquoted new-line in a csvRecord.
					state = csvError;
					status = csvInvLineBrk;
					break;
				}
				if (wc != separator)
				{
					// In this state, we buy anything and everything until we
					// see a separator character.
					field += wc;
				}
				else
				{
					// This is a separator which terminates the current field.
					fields.push_back (field);
					field.clear ();
					state = csvBegTrim;
				}
				break;

			case csvQuoted:
				// Here if we are parsing a quoted field and the escape
				// character is the same as the quote character.
				if (wc == quote)
				{
					if (itr != csvRecord.end () && *itr == quote)
					{
						// Two consecutive quote characters.  Push one of them.
						wc = *itr++;
						field += wc;
					}
					else
					{
						// This is the end of the field.
						fields.push_back (field);
						field.clear ();
						state = csvEndTrim;
					}
				}
				else
				{
					field += wc;
				}
				break;

			case csvQuotedEsc:
				// Here if we are parsing a quoted field and the escape
				// character is different from the quote character.
				if (wc == escape)
				{
					if (itr != csvRecord.end ())
					{
						// Escape applies to any character, including the quote character.
						wc = *itr++;
						field += wc;
					}
					else
					{
						state = csvError;
						status = csvLastWasEsc;
					}
				}
				else if (wc == quote)
				{
					// This is the end of the field.
					fields.push_back (field);
					field.clear ();
					state = csvEndTrim;
				}
				else
				{
					field += wc;
				}
				break;

			case csvEndTrim:
				// Ignore white space.  Anything else before a separator or the
				// end of the record is an error.
				if (wc == separator)
				{
					state = csvBegTrim;
				}
				else if (!iswspace (wc))
				{
					state = csvError;
					status = csvAmbigQuote;
				}
				break;

			case csvNone:
			default:
				state = csvError;
				status = csvInternal;
				break;
			}
		}
		if (field.length () > 10000)
		{
			state = csvError;
			status = csvFieldTooLong;
		}
		if (fields.size () > 300)
		{
			state = csvError;
			status = csvTooManyFields;
		}
	}
	return status;
}
//lint -restore
////////////////////////////////////////////////////////////////
// The following will quote an 8 bit character array, if needed; in place,  For
// example, if the string has a separator character in it, it will be quoted.
// Also, any existing quote in the string will be doubled up.  The delimiters
// argument is used as described above for csCsvFieldParse.
//
// The function returns true if the field was indeed quoted.
//
// Note that:
// 1> This function will not quote any field whose quoted length is longer
//    than 4095 characters.  This rather generous limitation removes the need
//    for a memory allocation which, therefore, improves performance and
//    removes the possibility of a throw due memory allocation failure.
// 2> as currently written, this function will convert line breaks to a single
//    space.  This may not be desirable in all cases.
// 3> if the field is already quoted, it will be quoted again.
//
// The function does not throw, nor does it call anything that might throw.
//lint -save -esym(534,*::insert,*::replace,*::append)
bool csCsvQuoter (char* csvField,size_t csvSize,const char* delimiters)
{
	bool quoteNeeded (false);

	char cc;
	char *srcPtr;
	char *trgPtr;
	char *lstPtr;

    char myBuffer [4096];

   	char separator = ',';
	char quote     = '\"';
	char escape    = '\"';

	// Prevent a crash to the degree reasonable.
	if (csvField != 0 && *csvField != '\0' && csvSize >= 4)
	{
		// Process the user supplied delimiters, if any.
		if (delimiters != 0)
		{
			if (*delimiters != '\0')
			{
				separator = *delimiters;
				if (*(delimiters + 1) != '\0')
				{
					quote = *(delimiters + 1);
					if (*(delimiters + 2) != '\0')
					{
						escape = *(delimiters + 2);
					}
				}
			}
		}

		// See if quoting is necessary.
		srcPtr = csvField;
		while ((cc = *srcPtr++) != '\0')
		{
			if (cc == separator || cc == quote ||  cc == '\n')
			{
				quoteNeeded = true;
				break;
			}
		}

		if (quoteNeeded)
		{
			quoteNeeded = false;
			srcPtr = csvField;
			trgPtr = myBuffer;
			lstPtr = myBuffer + sizeof (myBuffer) - 3;

			*trgPtr++ = quote;
			while (trgPtr < lstPtr && (cc = *srcPtr++) != '\0')
			{
				if (cc == '\r') continue;
				else if (cc == '\n') cc = ' ';
				else if (cc == quote) *trgPtr++ = escape;
				*trgPtr++ = cc;
			}
			if (trgPtr < lstPtr)
			{
				*trgPtr++ = quote;
				*trgPtr = '\0';
				if (strlen (myBuffer) < csvSize)
				{
					strncpy (csvField,myBuffer,csvSize);
					csvField [csvSize - 1] = '\0';
					quoteNeeded = true;
				}
			}
		}
	}
	return quoteNeeded;
}
///////////////////////////////////////////////////////////////////////////////
// The following will quote the CSV field provided by the csvField argument.
// If the forceIt argument is false, the quoting operation takes place only if
// necessary.  Otherwise, the quoting operation is always performed.
//
// The quoting operation consists of applying leading and trailing quote
// characters and escaping all quote characters within the field.
//
// If the field already has lleading and trailing quotes, it assumed to be in
// the quoted state and the function does nothing.
//
// The delimiters argument is used as described above for csCsvFieldParse.
//
// The itself function does not throw, but uses the STL std::wstring object
// (obviously) which may throw.
//
bool csCsvQuoter (std::wstring& csvField,bool forceIt,const wchar_t* delimiters)
{
	bool weQuotedIt (false);
	bool quoteNeeded = false;
	std::wstring::size_type idx; 
	std::wstring::size_type idxLast; 

	wchar_t separator = L',';
	wchar_t quote     = L'\"';
	wchar_t escape    = L'\"';

	// Process the user supplied delimiters, if any.
	if (delimiters != 0)
	{
		if (*delimiters != L'\0')
		{
			separator = *delimiters;
			if (*(delimiters + 1) != L'\0')
			{
				quote = *(delimiters + 1);
				if (*(delimiters + 2) != L'\0')
				{
					escape = *(delimiters + 2);
				}
			}
		}
	}

	// If it's already quoted, we will assume it is quoted correctly and
	// leave it alone.
	idxLast = csvField.find_last_not_of (L" \t\n\f");
	if (idxLast == std::wstring::npos)
	{
		// It's all whitespace.
		return weQuotedIt;
	}

	idx = csvField.find_first_not_of (L" \t\n\f");
	if (csvField [idx] == quote && csvField [idxLast] == quote)
	{
		// It appears that it has already been quoted.
		return weQuotedIt;
	}

	// Determine if this field needs to be quoted.
	quoteNeeded = forceIt;
	if (!quoteNeeded)
	{
		idx = csvField.find_first_of (separator);
		quoteNeeded = (idx != std::wstring::npos);
		if (!quoteNeeded)
		{
		    // We've already dealt with the 'already quoted" situation.
			idx = csvField.find_first_of (quote);
			quoteNeeded = (idx != std::wstring::npos);
		}
		if (!quoteNeeded)
		{
			idx = csvField.find_first_of (L"\n");
			quoteNeeded = (idx != std::wstring::npos);
		}
	}

	if (quoteNeeded)
	{
		// We'll prepend, and append, the quote character after we have
		// escaped all quote characters in the string with the escape
		// character.  This eliminates the need to worry about the first and
		// last characters in the following loop.
		idx = 0;
		while (idx != std::wstring::npos)
		{
			idx = csvField.find_first_of (quote,idx);
			if (idx != std::wstring::npos)
			{
				csvField.insert (idx,1,escape);
				// Bump past the escape we inserted, and the quote we found.
				idx += 2;
			}
		}
        // Replace all new lines, if any, with a space.
		while (idx != std::wstring::npos)
		{
			idx = csvField.find_first_of (L'\n',idx);
			if (idx != std::wstring::npos)
			{
			    wchar_t wStr [2];
			    wStr [0] = L' ';
			    wStr [1] = L'\0';
				csvField.replace (idx,1,wStr);
			}
		}
		csvField.append (1,quote);
#if (_OPR_SYSTEM == _os_SOLARIS) && !defined(__GNUC__)
		csvField.insert ((wchar_t*)NULL,1,quote);
#else
		csvField.insert (0,1,quote);
#endif
		weQuotedIt = true;
	}
	return weQuotedIt;
}
std::wstring csQuoteCsvField (const std::wstring& csvField,bool forceIt,const wchar_t* delimiters)
{
	std::wstring rtnValue;
	
	rtnValue = csvField;
	csCsvQuoter (rtnValue,forceIt,delimiters);			//lint !e534  (ignoring return value)
	return rtnValue;
}
// Functions which return a pointer to null terminated strings of spaces,
// useful when formatting string output.
const char* CS_ccPad (int length)
{
	static char spaces [256];

	if (length < 0) length = 0;
	if (length >= sizeof (spaces)) length = sizeof (spaces) - 1;
	memset (spaces,' ',sizeof (spaces));
	spaces [length] = '\0';
	return spaces;
}

const wchar_t* CS_wcPad (int length)
{
	static wchar_t spaces [256];

	if (length < 0) length = 0;
	if (length >= wcCount (spaces)) length = wcCount (spaces) - 1;
	wmemset (spaces,L' ',wcCount (spaces));
	spaces [length] = L'\0';
	return spaces;
}

//lint -restore
//newPage//
//=============================================================================
// TcsCsvStatus Object -- Encapsulates the functionality of a Comma Separated
//                        Value status report.
// Should any of these objects throw an execption in the future, this is what
// they should throw.
//=============================================================================
TcsCsvStatus::TcsCsvStatus (EcsCsvStatus status) : StatusValue (status),
                                                   LineNbr     (0UL),
                                                   FieldNbr    (0),
                                                   FieldId     (),
                                                   ObjectName  ()
{
}
TcsCsvStatus::TcsCsvStatus (EcsCsvStatus status,unsigned long lineNbr,
                                                short fieldNbr)
                                                    :
                                                StatusValue (status),            
                                                LineNbr     (lineNbr),
                                                FieldNbr    (fieldNbr),
                                                FieldId     (),
                                                ObjectName  ()
{
}
TcsCsvStatus::TcsCsvStatus (EcsCsvStatus status,unsigned long lineNbr,
                                                short fieldNbr,
                                                const std::wstring& fieldId)
                                                    :
                                                StatusValue (status),            
                                                LineNbr     (lineNbr),
                                                FieldNbr    (fieldNbr),
                                                FieldId     (fieldId),
                                                ObjectName  ()
{
}
TcsCsvStatus::TcsCsvStatus (const TcsCsvStatus& source) : StatusValue (source.StatusValue),
                                                          LineNbr     (source.LineNbr),
                                                          FieldNbr    (source.FieldNbr),
                                                          FieldId     (source.FieldId),
                                                          ObjectName  (source.ObjectName)
{
}                                                         
TcsCsvStatus::~TcsCsvStatus (void)
{
}
TcsCsvStatus& TcsCsvStatus::operator= (const TcsCsvStatus& rhs)
{
    if (&rhs != this)
    {
        StatusValue = rhs.StatusValue;
        LineNbr     = rhs.LineNbr;
        FieldNbr    = rhs.FieldNbr;
        FieldId     = rhs.FieldId;
        ObjectName  = rhs.ObjectName;
    }
    return *this;
}
//=========================================================================
// Named Member Functions
void TcsCsvStatus::SetStatus (EcsCsvStatus status)
{
    StatusValue = status;
}
void TcsCsvStatus::BumpLineNbr ()
{
    ++LineNbr;
}
void TcsCsvStatus::SetLineNbr (unsigned long lineNbr)
{
    LineNbr = lineNbr;
}
void TcsCsvStatus::SetFieldNbr (short fieldNbr)
{
    FieldNbr = fieldNbr;
}
void TcsCsvStatus::SetFieldId (const std::wstring& fieldId)
{
    FieldId = fieldId;
}
void TcsCsvStatus::SetObjectName (const std::wstring& objectName)
{
    ObjectName = objectName;
}

std::wstring TcsCsvStatus::GetMessage (void)
{
	const TcsCsvMsgTbl* tblPtr;
	const wchar_t* statusMsgPtr;
	std::wstring message;
	wchar_t msgBuffer [256];
	wchar_t fldDesignation [128];

	for (tblPtr = KcsCsvMsgTbl;tblPtr->StatusValue != csvEndOfTable;tblPtr += 1)
	{
		if (StatusValue == tblPtr->StatusValue)
		{
			break;
		}
	}
	statusMsgPtr = tblPtr->Message;
	if (FieldId.empty ())
	{
		cs_swprintf (fldDesignation,wcCount (fldDesignation),L"%d",FieldNbr);
	}
	else
	{
		cs_swprintf (fldDesignation,wcCount (fldDesignation),L"%S",FieldId.c_str ());		//lint !e559  parameter does not match format.
	}

	cs_swprintf (msgBuffer,wcCount (msgBuffer),L"Obj: %S; Line %lu; Field %S; Reason: %S.",
											ObjectName.c_str (),			//lint !e559   PC-Lint does not recognize %S in format */
											LineNbr,
											fldDesignation,					//lint !e559   PC-Lint does not recognize %S in format */
											statusMsgPtr);					//lint !e559   PC-Lint does not recognize %S in format */
	message = msgBuffer;
	return message;
}
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
//=========================================================================
// Construction, Destruction, Assignment
TcsCsvRecord::TcsCsvRecord (void) : MinFldCnt (0),
                                    MaxFldCnt (300),
                                    Fields    ()
{
}
TcsCsvRecord::TcsCsvRecord (const TcsCsvRecord& source) : MinFldCnt (source.MinFldCnt),
                                                          MaxFldCnt (source.MaxFldCnt),
                                                          Fields    (source.Fields)
{
}
TcsCsvRecord::~TcsCsvRecord (void)
{
}
TcsCsvRecord& TcsCsvRecord::operator= (const TcsCsvRecord& rhs)
{
    if (&rhs != this)
    {
        MinFldCnt = rhs.MinFldCnt;
        MaxFldCnt = rhs.MaxFldCnt;
        Fields    = rhs.Fields;
    }
    return *this;
}
//=========================================================================
// Public Named Member Functions
bool TcsCsvRecord::Empty (void) const
{
    bool empty = Fields.size () == 0;
    return empty;
}
void TcsCsvRecord::Clear (void)
{
    Fields.clear ();
}
void TcsCsvRecord::Reserve (unsigned expectedFldCnt)
{
    Fields.reserve (expectedFldCnt);
}
short TcsCsvRecord::FieldCount (void) const
{
    short fldCount = static_cast<short>(Fields.size ());
    return fldCount;
}
short TcsCsvRecord::SetMinFldCnt (short minFldCnt)
{
    short rtnValue = MinFldCnt;
    if (minFldCnt >= 0)
    {
        MinFldCnt = minFldCnt;
    }
    return rtnValue;
}
short TcsCsvRecord::SetMaxFldCnt (short maxFldCnt)
{
    short rtnValue = MaxFldCnt;
    if (maxFldCnt >= 0)
    {
        MaxFldCnt = maxFldCnt;
    }
    return rtnValue;
}
bool TcsCsvRecord::ReplaceField (const std::wstring& newField,short fieldNbr,TcsCsvStatus& status)
{
    bool ok = true;

    if (fieldNbr >= 0 && static_cast<unsigned>(fieldNbr) < Fields.size ())	//lint !e571  (suspicious cast)
    {
        std::wstring& fldRef = Fields [static_cast<unsigned>(fieldNbr)];	//lint !e571  (suspicious cast)
        fldRef = newField;
     }
     else
     {
        ok = false;
        status.SetStatus (csvInvFieldNbr);
        status.SetFieldNbr (fieldNbr);
     }
     return ok;
}
bool TcsCsvRecord::AppendField (const std::wstring& newField,TcsCsvStatus& status)
{
    bool ok = true;
    
    if (Fields.size () < static_cast<unsigned>(MaxFldCnt))		//lint !e571   (suspicious cast)
    {
        Fields.push_back (newField);
    }
    else
    {
        ok = false;
        status.SetStatus (csvTooManyFields);
    }
    return ok;
}
bool TcsCsvRecord::InsertField (const std::wstring& newField,short before,TcsCsvStatus& status)
{
    bool ok = true;

    if (before < 0 || static_cast<unsigned>(before) > Fields.size ())		//lint !e571   (suspicious cast)
    {
        ok = false;
        status.SetStatus (csvInvFieldNbr);
    }
    else if (Fields.size () >= static_cast<unsigned>(MaxFldCnt))		//lint !e571   (suspicious cast)
    {
        ok = false;
        status.SetStatus (csvTooManyFields);
    }
    else
    {
        fldItr itr = Fields.begin () + before;
        Fields.insert (itr,newField);
    }
    return ok;
}
bool TcsCsvRecord::RemoveField (short fieldNbr,TcsCsvStatus& status)
{
    bool ok = true;

    if (fieldNbr < 0 || static_cast<unsigned>(fieldNbr) >= Fields.size ())		//lint !e571   (suspicious cast)
    {
        ok = false;
        status.SetStatus (csvInvFieldNbr);
    }
    else if (Fields.size () <= static_cast<unsigned>(MinFldCnt))		//lint !e571   (suspicious cast)
    {
        ok = false;
        status.SetStatus (csvTooManyFields);
    }
    else
    {
        fldItr itr = Fields.begin () + fieldNbr;
        Fields.erase (itr);							//lint !e534  ignoring return value
    }
    return ok;
}
bool TcsCsvRecord::GetField (std::wstring& field,short fieldNbr,TcsCsvStatus& status) const
{
    bool ok = true;
    
    if (fieldNbr >= 0 && static_cast<unsigned>(fieldNbr) < Fields.size ())		//lint !e571   (suspicious cast)
    {
        field = Fields [static_cast<unsigned>(fieldNbr)];		//lint !e571   (suspicious cast)
    }
    else
    {
        ok = false;
        status.SetStatus (csvInvFieldNbr);
    }
    return ok;
}
short TcsCsvRecord::FindField (const std::wstring& fieldValue,TcsCsvStatus& status) const
{
    short fieldNbr = -1;
    fldItrK findItr;
    
    if (Fields.size () != 0)
    {
        //??// The following is probably case sensitive.  We probably want to
        //??// change that???
        findItr = std::find (Fields.begin (),Fields.end (),fieldValue);
        if (findItr != Fields.end ())
        {
            fieldNbr = static_cast<short>(findItr - Fields.begin ());
        }
        else
        {
            status.SetStatus (csvInvFieldId);
        }
    }
    else
    {
        status.SetStatus (csvNoFields);
    }
    return fieldNbr;
}
bool TcsCsvRecord::ReplaceRecord (const std::wstring& newRecord,TcsCsvStatus& status,
                                                                const wchar_t* delimiters)
{
    bool ok = true;
    EcsCsvStatus prsStatus;

    Fields.clear ();
    Fields.reserve (static_cast<unsigned>(MinFldCnt));		//lint !e571   (suspicious cast)
    prsStatus = csCsvFieldParse (Fields,newRecord,delimiters);
    if (prsStatus == csvOk)
    {
        size_t fldCnt = Fields.size ();
        if (fldCnt < static_cast<size_t>(MinFldCnt))		//lint !e571   (suspicious cast)
        {
            ok = false;
            status.SetStatus (csvTooFewFields);
        }
        else if (fldCnt > static_cast<size_t>(MaxFldCnt))		//lint !e571   (suspicious cast)
        {
            ok = false;
            status.SetStatus (csvTooManyFields);
        }
    }
    else
    {
        ok = false;
        status.SetStatus (csvEndInQuote);
    }
    return ok;
}
bool TcsCsvRecord::ReplaceRecord (std::wistream& iStrm,TcsCsvStatus& status,const wchar_t* delimiters)
{
    bool ok = false;
    std::wstring newRecord;

    EcsCsvStatus csvStatus = csGetCsvRecord (newRecord,iStrm,delimiters);
    if (csvStatus == csvOk)
    {
        ok = ReplaceRecord (newRecord,status,delimiters);
    }
    else
    {
        status.SetStatus (csvStatus);
    }
    return ok;
}
bool TcsCsvRecord::ReturnAsRecord (std::wstring& record,TcsCsvStatus& /*status*/,const wchar_t* delimiters) const
{
    bool ok = true;

    wchar_t separator = L',';
    std::wstring field;

    if (delimiters != 0 && *delimiters != '\0')
    {
        separator = *delimiters;
    }

    record.clear ();
    for (fldItrK itr = Fields.begin ();itr != Fields.end ();itr++)
    {
        field = csQuoteCsvField (*itr,false,delimiters);
        if (itr != Fields.begin ())
        {
            record += separator;
        }
        record += field;
    }
    return ok;
}
bool TcsCsvRecord::WriteToStream (std::wostream& oStrm,TcsCsvStatus& status,const wchar_t* delimiters) const
{
    std::wstring record;

    bool ok = ReturnAsRecord (record,status,delimiters);
    if (ok)
    {
        oStrm << record << std::endl;
    }
    return ok;
}
//newPage//
//=============================================================================
// CSV File Sort Function Object
//
// Create an object of this type to specify the fields which are to be used in
// a CSV file sort operation.  Then provide the object to the CSV file sort
// function.
//
//=================================================================================
// Construction  /  Destruction  /  Assignment
TcsCsvSortFunctor::TcsCsvSortFunctor (short first,short second,short third,
															   short fourth)
																:
															   FirstField  (first),
															   SecondField (second),
															   ThirdField  (third),
															   FourthField (fourth)
{
}
TcsCsvSortFunctor::TcsCsvSortFunctor (const TcsCsvSortFunctor& source)
											:
									  FirstField  (source.FirstField),
									  SecondField (source.SecondField),
									  ThirdField  (source.ThirdField),
									  FourthField (source.FourthField),
									  CsvStatus   ()
{
}
TcsCsvSortFunctor::~TcsCsvSortFunctor ()
{
}
TcsCsvSortFunctor& TcsCsvSortFunctor::operator= (const TcsCsvSortFunctor& rhs)
{
	if (&rhs != this)
	{
		FirstField  = rhs.FirstField;
		SecondField = rhs.SecondField;
		ThirdField  = rhs.ThirdField;
		FourthField = rhs.FourthField;
		CsvStatus   = rhs.CsvStatus;
	}
	return *this;
}
//=================================================================================
// Operator Overrides
bool TcsCsvSortFunctor::operator() (const TcsCsvRecord& recordOne,const TcsCsvRecord& recordTwo)
{
	bool ok1;
	bool ok2;
	EcsCmpResult cmpResult (cmpGreaterThan);		// initialization to keep lint happy.
	std::wstring fieldOne;
	std::wstring fieldTwo;

	ok1 = recordOne.GetField (fieldOne,FirstField,CsvStatus);
	ok2 = recordTwo.GetField (fieldTwo,FirstField,CsvStatus);
	if (ok1 && ok2)
	{
		cmpResult = CsvFieldCompare (fieldOne,fieldTwo);
		if (cmpResult == cmpEqualTo && SecondField >= 0)
		{
			ok1 = recordOne.GetField (fieldOne,SecondField,CsvStatus);
			ok2 = recordTwo.GetField (fieldTwo,SecondField,CsvStatus);
			if (ok1 && ok2)
			{
				cmpResult = CsvFieldCompare (fieldOne,fieldTwo);
				if (cmpResult == cmpEqualTo && ThirdField >= 0)
				{
					ok1 = recordOne.GetField (fieldOne,ThirdField,CsvStatus);
					ok2 = recordTwo.GetField (fieldTwo,ThirdField,CsvStatus);
					if (ok1 && ok2)
					{
						cmpResult = CsvFieldCompare (fieldOne,fieldTwo);
						if (cmpResult == cmpEqualTo && FourthField >= 0)
						{
							ok1 = recordOne.GetField (fieldOne,FourthField,CsvStatus);
							ok2 = recordTwo.GetField (fieldTwo,FourthField,CsvStatus);
							if (ok1 && ok2)
							{
								cmpResult = CsvFieldCompare (fieldOne,fieldTwo);
							}
						}
					}
				}
			}
		}
	}
	if (ok1 && !ok2)
	{
		cmpResult = cmpLessThan;
	}
	else if (!ok1 && ok2)
	{
		cmpResult = cmpGreaterThan;
	}
	else if (!ok1 && !ok2)
	{
		cmpResult = cmpEqualTo;
	}
	return (cmpResult == cmpLessThan);
}
TcsCsvSortFunctor::EcsCmpResult TcsCsvSortFunctor::CsvFieldCompare (const std::wstring& fieldOne,
																	const std::wstring& fieldTwo) const
{
	EcsCmpResult result;
	wchar_t *wcPtr1;
	wchar_t *wcPtr2;
	long longOne, longTwo;
	double realOne, realTwo;

	// If the fields are numeric in nature, we want to compare the values in
	// numeric form.  This hurts performance, but aleviates the need for users
	// too know of and specify the type of the fields.  So, this is a great
	// convenience, and makes the whole thing a bit more general.
	longOne = wcstol (fieldOne.c_str (),&wcPtr1,10);
	longTwo = wcstol (fieldTwo.c_str (),&wcPtr2,10);
	if (*wcPtr1 == L'\0' && *wcPtr2 == L'\0' &&
	    longOne != LONG_MAX && longOne != LONG_MIN &&
	    longTwo != LONG_MAX && longTwo != LONG_MIN)
	{
		// It appears that both fields are numeric and of the integer
		// type.  So the comparision is as follows:
		if      (longOne < longTwo) result = cmpLessThan;
		else if (longOne > longTwo) result = cmpGreaterThan;
		else                        result = cmpEqualTo;
	}
	else
	{
		// Maybe the fields are doubles.
		realOne = wcstod (fieldOne.c_str (),&wcPtr1);
		realTwo = wcstod (fieldTwo.c_str (),&wcPtr2);
		if (*wcPtr1 == L'\0' && *wcPtr2 == L'\0' &&
			fabs (realOne) < HUGE_VAL && fabs (realTwo) < HUGE_VAL)
		{
			// It appears that both fields are numeric and of the real
			// type.  So the comparision is as follows:
			if      (realOne < realTwo) result = cmpLessThan;
			else if (realOne > realTwo) result = cmpGreaterThan;
			else                        result = cmpEqualTo;
		}
		else
		{
			// We consider the fields to be strings.  We default to a
			// case insensitive string comparison.
			int iResult = CS_wcsicmp (fieldOne.c_str (),fieldTwo.c_str ());
			if      (iResult < 0) result = cmpLessThan;
			else if (iResult > 0) result = cmpGreaterThan;
			else                  result = cmpEqualTo;
		}
	}
	return result;
}
//newPage//
//=============================================================================

// TcsCsvFileBase Object -- Encapsulates the functionality of a Comma Separated
//                          Value file.
//=============================================================================
// Static Functions, Variables, and Constants
const unsigned TcsCsvFileBase::InvalidRecordNbr = UINT_MAX;
unsigned TcsCsvFileBase::GetInvalidRecordNbr (void)
{
    return InvalidRecordNbr;
}
//=========================================================================
// Construction, Destruction, and Assignment
TcsCsvFileBase::TcsCsvFileBase (bool firstIsLabels,short minFldCnt,
                                                   short maxFldCnt,
                                                   const wchar_t* delimiters)
                                                     :
                                                   FirstIsLabels (firstIsLabels),
                                                   IsSorted      (false),
                                                   IsIndexed     (false),
                                                   Separator     (L','),
                                                   Quote         (L'\"'),
                                                   Escape        (L'\"'),
                                                   KeyField      (-1),
                                                   MinFldCnt     (minFldCnt),
                                                   MaxFldCnt     (maxFldCnt),
                                                   ObjectName    (),
                                                   Labels        (),
                                                   Records       (),
                                                   Index         ()
{
    SetDelimiters (delimiters);
    if ((MinFldCnt > MaxFldCnt) || (maxFldCnt == 0))
    {
        MinFldCnt = 0;
        MaxFldCnt = 300;
    }
}                                               
TcsCsvFileBase::TcsCsvFileBase (const TcsCsvFileBase& source) : FirstIsLabels (source.FirstIsLabels),
                                                                IsSorted      (source.IsSorted),
                                                                IsIndexed     (source.IsIndexed),
                                                                Separator     (source.Separator),
                                                                Quote         (source.Quote),
                                                                Escape        (source.Escape),
                                                                KeyField      (source.KeyField),
                                                                MinFldCnt     (source.MinFldCnt),
                                                                MaxFldCnt     (source.MaxFldCnt),
                                                                ObjectName    (source.ObjectName),
                                                                Labels        (source.Labels),
                                                                Records       (source.Records),
                                                                Index         (source.Index)
{
}
TcsCsvFileBase::~TcsCsvFileBase (void)
{
}
TcsCsvFileBase& TcsCsvFileBase::operator= (const TcsCsvFileBase& rhs)
{
    if (&rhs != this)
    {
        FirstIsLabels = rhs.FirstIsLabels;
        IsSorted      = rhs.IsSorted;
        IsIndexed     = rhs.IsIndexed;
        Separator     = rhs.Separator;
        Quote         = rhs.Quote;
        Escape        = rhs.Escape;
        KeyField      = rhs.KeyField;
        MinFldCnt     = rhs.MinFldCnt;
        MaxFldCnt     = rhs.MaxFldCnt;
        ObjectName    = rhs.ObjectName;
        Labels        = rhs.Labels;
        Records       = rhs.Records;
        Index         = rhs.Index;
    }
    return *this;
}
//=========================================================================
// Operator Overrides
//=========================================================================
// Public Named Member Functions
bool TcsCsvFileBase::HasLabels () const
{
    return FirstIsLabels;
}
short TcsCsvFileBase::SetMinFldCnt (short newCnt)
{
    short rtnValue = MinFldCnt;
    if (newCnt >= 0)
    {
        MinFldCnt = newCnt;
    }
    return rtnValue;
}
short TcsCsvFileBase::SetMaxFldCnt (short newCnt)
{
    short rtnValue = MaxFldCnt;
    if (newCnt >= 0)
    {
        MaxFldCnt = newCnt;
    }
    return rtnValue;
}
bool TcsCsvFileBase::SetDelimiters (const wchar_t* delimiters)
{
    if (delimiters != 0 && *delimiters != L'\0')
    {
        Separator = *delimiters;
        if (*(delimiters +1) != L'\0')
        {
           Quote = *(delimiters + 1);
           if (*(delimiters + 2) != L'\0')
           {
               Escape = *(delimiters + 2);
           }
        }
    }
    else
    {
        Separator = L',';
        Quote     = L'\"';
        Escape    = L'\"';
    }
    return true;
}
void TcsCsvFileBase::SetObjectName (const std::wstring& objectName)
{
    ObjectName = objectName;
}
unsigned TcsCsvFileBase::RecordCount (void) const
{
    unsigned recordCount = static_cast<unsigned>(Records.size ());
    return recordCount;
}
short TcsCsvFileBase::FieldCount (unsigned recordNbr) const
{
    short fldCount = -1;

    if (recordNbr < Records.size ())
    {
        const TcsCsvRecord& recRef = Records [recordNbr];
        fldCount = recRef.FieldCount ();
    }
    return fldCount;
}
short TcsCsvFileBase::GetFldNbr (const wchar_t* fieldId,TcsCsvStatus& status) const
{
    short fieldNbr = -1;

    if (!FirstIsLabels || (Labels.FieldCount () == 0))
    {
        status.SetStatus (csvNoFldLabels);
        status.SetObjectName (ObjectName);
    }
    else if (Records.size () == 0)
    {
        status.SetStatus (csvNoRecords);
        status.SetObjectName (ObjectName);
    }
    else if (fieldId != 0 && *fieldId != L'\0')
    {
        std::wstring findValue (fieldId);
        fieldNbr = Labels.FindField (findValue,status);
        if (fieldNbr < 0)
        {
            status.SetLineNbr (0UL);
            status.SetObjectName (ObjectName);
        }
    }
    return fieldNbr;
}
bool TcsCsvFileBase::GetFieldId (std::wstring& fieldId,short fieldNbr,TcsCsvStatus& status) const
{
    bool ok = true;
    
    if (!FirstIsLabels || (Labels.FieldCount () == 0))
    {
        ok = false;
        status.SetStatus (csvNoFldLabels);
        status.SetObjectName (ObjectName);
    }
    else if (Records.size () == 0)
    {
        status.SetStatus (csvNoRecords);
        status.SetObjectName (ObjectName);
    }
    else
    {
        if (fieldNbr < 0 || fieldNbr >= Labels.FieldCount ())
        {
            ok = false;
            status.SetStatus (csvInvFieldNbr);
            status.SetObjectName (ObjectName);
        }
        else
        {
            ok = Labels.GetField (fieldId,fieldNbr,status);
            if (!ok)
            {
                status.SetObjectName (ObjectName);
            }
        }
    }
    return ok;
}
void TcsCsvFileBase::Clear ()
{
    FirstIsLabels = false;
    Labels.Clear ();
    IsSorted = false;
    Records.clear ();
    KeyField = -1;
    IsIndexed = false;
    Index.clear ();
}
bool TcsCsvFileBase::SetRecordKeyField (const wchar_t* fieldId,TcsCsvStatus& status)
{
    bool ok = false;

    Index.clear ();
    IsIndexed = false;
    KeyField = GetFldNbr (fieldId,status);
    if (KeyField < 0)
    {
        status.SetStatus (csvInvFieldId);
        status.SetFieldId (fieldId);
        status.SetObjectName (ObjectName);
    }
    else
    {
        ok = BuildIndex (status);
    }
    return ok;
}
bool TcsCsvFileBase::SetRecordKeyField (short fieldNbr,TcsCsvStatus& status)
{
    bool ok = false;

    Index.clear ();
    IsIndexed = false;
    if (fieldNbr < 0 || fieldNbr >= MinFldCnt)
    {
        status.SetStatus (csvInvFieldNbr);
        status.SetFieldNbr (fieldNbr);
        status.SetObjectName (ObjectName);
    }
    else
    {
        KeyField = fieldNbr;
        ok = BuildIndex (status);
        IsIndexed = true;
    }
    return ok;
}
bool TcsCsvFileBase::GetFieldLabel (std::wstring& label,short fieldNbr) const
{
	bool ok (false);
	TcsCsvStatus status;

	label.clear ();
	if (fieldNbr >= 0 && fieldNbr < Labels.FieldCount ())
	{
		ok = Labels.GetField (label,fieldNbr,status);
	}
	return ok;
}
bool TcsCsvFileBase::GetFieldLabel (std::wstring& label,short fieldNbr,TcsCsvStatus& status) const
{
	bool ok (false);

	label.clear ();
	if (fieldNbr >= 0 && fieldNbr < Labels.FieldCount ())
	{
		ok = Labels.GetField (label,fieldNbr,status);
	}
	return ok;
}
bool TcsCsvFileBase::SetFieldLabel (const wchar_t* newLabel,short fieldNbr)
{
	bool ok (false);
	TcsCsvStatus status;

	if (fieldNbr >= 0 && fieldNbr < Labels.FieldCount ())
	{
		std::wstring wsTemp (newLabel);
		ok = Labels.ReplaceField (wsTemp,fieldNbr,status);
	}
	return ok;
}
bool TcsCsvFileBase::SetFieldLabel (const wchar_t* newLabel,short fieldNbr,TcsCsvStatus& status)
{
	bool ok (false);

	if (fieldNbr >= 0 && fieldNbr < Labels.FieldCount ())
	{
		std::wstring wsTemp (newLabel);
		ok = Labels.ReplaceField (wsTemp,fieldNbr,status);
	}
	return ok;
}
void TcsCsvFileBase::ResetAllLabels ()
{
	Labels.Clear ();
	
}
void TcsCsvFileBase::SetAllLabels (const TcsCsvRecord& newLabels)
{
	Labels = newLabels;
}

bool TcsCsvFileBase::GetField (std::wstring& field,unsigned recordNbr,short fieldNbr,TcsCsvStatus& status) const
{
    bool ok = true;

    if (recordNbr >= Records.size ())
    {
        ok = false;
        status.SetStatus (csvInvRecordNbr);
        status.SetLineNbr (recordNbr);
        status.SetObjectName (ObjectName);
    }
    else
    {
        recItrK recordItr = Records.begin() + recordNbr;
        ok = recordItr->GetField (field,fieldNbr,status);
        if (!ok)
        {
            status.SetLineNbr (recordNbr + FirstIsLabels);		// status LineNbr is always absolute
            status.SetObjectName (ObjectName);
        }
    }
    return ok;
}
bool TcsCsvFileBase::GetField (std::wstring& field,unsigned recordNbr,const wchar_t* fieldId,TcsCsvStatus& status) const
{
    bool ok = true;

    if (FirstIsLabels && (Labels.FieldCount () > 0))
    {
        short fieldNbr = GetFldNbr (fieldId,status);
        if (fieldNbr < 0)
        {
            ok = false;
            status.SetStatus (csvInvFieldId);
            status.SetLineNbr (recordNbr + FirstIsLabels);		// status LineNbr is always absolute
            status.SetObjectName (ObjectName);
        }
        else
        {
            ok = GetField (field,recordNbr,fieldNbr,status);
        }
    }
    else
    {
        ok = false;
        status.SetStatus (csvNoFldLabels);
        status.SetObjectName (ObjectName);
    }
    return ok;
}
bool TcsCsvFileBase::ReplaceField (const std::wstring& newValue,unsigned recordNbr,short fieldNbr,TcsCsvStatus& status)
{
    bool ok = true;

    if (recordNbr >= Records.size ())
    {
        ok = false;
        status.SetStatus (csvInvRecordNbr);
        status.SetLineNbr (recordNbr + FirstIsLabels);		// status LineNbr is always absolute
        status.SetObjectName (ObjectName);
    }
    else
    {
        recItr recordItr = Records.begin() + recordNbr;
        ok = recordItr->ReplaceField (newValue,fieldNbr,status);
        if (!ok)
        {
            status.SetLineNbr (recordNbr + FirstIsLabels);		// status LineNbr is always absolute
            status.SetObjectName (ObjectName);
        }
    }
    return ok;
}
bool TcsCsvFileBase::ReplaceField (const std::wstring& newValue,unsigned recordNbr,const wchar_t* fieldId,TcsCsvStatus& status)
{
    bool ok = true;

    if (FirstIsLabels && (Labels.FieldCount () > 0))
    {
        short fieldNbr = GetFldNbr (fieldId,status);
        if (fieldNbr < 0)
        {
            ok = false;
            status.SetStatus (csvInvFieldId);
            status.SetLineNbr (recordNbr + FirstIsLabels);    // status LineNbr is always absolute
            status.SetObjectName (ObjectName);
        }
        else
        {
            ok = ReplaceField (newValue,recordNbr,fieldNbr,status);
        }
    }
    else
    {
        ok = false;
        status.SetStatus (csvNoFldLabels);
        status.SetObjectName (ObjectName);
    }
    return ok;
}

bool TcsCsvFileBase::GetRecord (TcsCsvRecord& record,unsigned recordNbr,TcsCsvStatus& status) const
{
	bool ok;

	if (recordNbr >= Records.size ())
	{
		ok = false;
		status.SetStatus (csvInvRecordNbr);
		status.SetLineNbr (recordNbr);
		status.SetObjectName (ObjectName);
	}
	else
	{
		recItrK recordItr = Records.begin() + recordNbr;
		record = *recordItr;
		ok = true;
	}
	return ok;
 }
// Implies that the file has been properly indexed, and srchString is the value
// to be searched for in the index.
bool TcsCsvFileBase::Locate (unsigned& recordNumber,const wchar_t* srchString) const
{
    bool ok (false);
    std::map<std::wstring,unsigned>::const_iterator findItr;

    recordNumber = InvalidRecordNbr;
    if (IsIndexed && KeyField >= 0)
    {
        findItr = Index.find (srchString);
        if (findItr != Index.end ())
        {
            recordNumber = findItr->second;
            ok = true;
        }
    }
    return ok;
}
bool TcsCsvFileBase::Locate (unsigned& recordNumber,short fieldNbr,const wchar_t* srchString,bool honorCase) const
{
	bool ok (false);
	bool match (false);
	recItrK itr;
	std::wstring field;
	TcsCsvStatus status;

    recordNumber = InvalidRecordNbr;
	for (itr = Records.begin ();itr != Records.end ();itr++)
	{
	    bool fldOk = itr->GetField (field,fieldNbr,status);
	    if (fldOk)
	    {
			if (honorCase)
			{
				match = (wcscmp (field.c_str (),srchString) == 0);
			}
			else
			{
				match = (CS_wcsicmp (field.c_str (),srchString) == 0);
			}
			if (match)
			{
				recordNumber = static_cast<unsigned>(itr - Records.begin ());
				ok = true;
				break;
			}
		}
	}
	return ok;
}
bool TcsCsvFileBase::Locate (unsigned& recordNumber,const wchar_t* fieldId,const wchar_t* srchString,bool honorCase) const
{
	bool ok (false);
	TcsCsvStatus status;

    recordNumber = InvalidRecordNbr;
	short fieldNbr = GetFldNbr (fieldId,status);
	if (fieldNbr >= 0)
	{
		ok = Locate (recordNumber,fieldNbr,srchString,honorCase);
	}
	return ok;
}
// The record number argument is expected to be the record number of a record
// returned by locate.  The search starts with the first record immediately
// after that identified by the recordNumber argument.  The new record number
// is returned in this same variable, and et to InvalidRecordNumber if a
// record matching the search criteria is  not found.
bool TcsCsvFileBase::LocateNext (unsigned& recordNumber,short fieldNbr,const wchar_t* srchString,bool honorCase) const
{
	bool ok;
	bool found (false);
	bool match (false);
	bool fldOk (true);
	recItrK itr;
	recItrK firstRec;
	std::wstring field;
	TcsCsvStatus status;

    ok = (recordNumber != InvalidRecordNbr);
    if (ok)
    {
		recordNumber += 1;
		ok = (recordNumber < RecordCount ());
	}
    if (ok)
    {
		firstRec = Records.begin () + recordNumber;
		for (itr = firstRec;fldOk && itr != Records.end ();itr++)
		{
			fldOk = itr->GetField (field,fieldNbr,status);
			if (fldOk)
			{
				if (honorCase)
				{
					match = (wcscmp (field.c_str (),srchString) == 0);
				}
				else
				{
					match = (CS_wcsicmp (field.c_str (),srchString) == 0);
				}
				if (match)
				{
					recordNumber = static_cast<unsigned>(itr - Records.begin ());
					found = true;
					break;
				}
			}
		}
	}
	return found;
}
// The record number argument is expected to be the record number of a record
// returned by locate.  The search starts with the first record immediately
// after that identified by the recordNumber argument.  The new record number
// is returned in this same variable, and et to InvalidRecordNumber if a
// record matching the search criteria is  not found.
bool TcsCsvFileBase::LocateNext (unsigned& recordNumber,const wchar_t* fieldId,const wchar_t* srchString,bool honorCase) const
{
	bool ok (false);
	TcsCsvStatus status;

	short fieldNbr = GetFldNbr (fieldId,status);
	if (fieldNbr >= 0)
	{
		ok = LocateNext (recordNumber,fieldNbr,srchString,honorCase);
	}
	return ok;
}
bool TcsCsvFileBase::StableSort (const TcsCsvSortFunctor& functor)
{
	bool ok;
	
	ok = functor.FirstField < MinFldCnt;
	if (ok && functor.SecondField >= 0)
	{
		ok = functor.SecondField < MinFldCnt;
	}
	if (ok && functor.ThirdField >= 0)
	{
		ok = functor.ThirdField < MinFldCnt;
	}
	if (ok && functor.FourthField >= 0)
	{
		ok = functor.FourthField < MinFldCnt;
	}
	if (ok)
	{
		recItr start = Records.begin ();
		recItr finish = Records.end ();
		std::stable_sort (start,finish,functor);
	}
	return ok;
}
// Essentially, a binary search for a record in the table which matches that
// provided by the searchRec argument.  No check is made concerning the
// sorted status of the file.
unsigned TcsCsvFileBase::LowerBound (const TcsCsvRecord& searchRec,const TcsCsvSortFunctor& functor)
{
	unsigned recordNbr (InvalidRecordNbr);
	recItr begItr = Records.begin ();
	recItr endItr = Records.end ();
	recItr locItr;

	locItr = std::lower_bound (begItr,endItr,searchRec,functor);
	if (locItr != endItr)
	{
		recordNbr = static_cast<unsigned>(locItr - begItr);
	}
	return recordNbr;
}
bool TcsCsvFileBase::ReadFromStream (std::wistream& iStrm,bool firstIsLabels,TcsCsvStatus& status)
{
	bool ok = true;
	unsigned long lineNbr = 0UL;
	wchar_t delimiters [4];
	TcsCsvRecord csvRecord;

	if (firstIsLabels && Records.size () != 0)
	{
		ok = false;
		status.SetStatus (csvLblsOnAppend);
		status.SetObjectName (ObjectName);
	}
	else
	{
		csvRecord.SetMinFldCnt (MinFldCnt);
		csvRecord.SetMaxFldCnt (MaxFldCnt);
		csvRecord.Reserve (static_cast<unsigned>(MaxFldCnt));		//lint !e571   (suspicious cast)

		delimiters [0] = Separator;
		delimiters [1] = Quote;
		delimiters [2] = Escape;
		delimiters [3] = L'\0';

		// Read the labels if they're supposed to be there.
		if (firstIsLabels)
		{
			ok =Labels.ReplaceRecord (iStrm,status,delimiters);
			if (ok)
			{
				lineNbr += 1;
			}
		}

		// Read the rest of the input stream.
		while (ok && iStrm.good ())
		{
			// See if we are at the end of the file.
			iStrm.peek ();					//lint !e534    (ignoring return value)
			if (iStrm.eof ()) break;

			// OK, there should be more stuff out there.
			lineNbr += 1;
			status.BumpLineNbr ();
			ok = csvRecord.ReplaceRecord (iStrm,status,delimiters);
			if (!ok)
			{
				status.SetLineNbr (lineNbr);
				status.SetObjectName (ObjectName);
				continue;						//lint !e845   PC-Lint suggests break instead of continue as ok == false here
			}
			Records.push_back (csvRecord);
		}
	}
	return ok;
}
bool TcsCsvFileBase::WriteToStream (std::wostream& oStrm,bool writeLabels,TcsCsvStatus& status) const
{
    bool ok = true;
    recItrK wrtItr;
    wchar_t delimiters [4];

    delimiters [0] = Separator;
    delimiters [1] = Quote;
    delimiters [2] = Escape;
    delimiters [3] = L'\0';
    
    if (writeLabels && !FirstIsLabels)
    {
        ok = false;
        status.SetStatus (csvNoFldLabels);
        status.SetObjectName (ObjectName);
    }
    else
    {
		// Write the labels if so requested.
		if (writeLabels)
		{
            ok = Labels.WriteToStream (oStrm,status,delimiters);
		}
		
		// Write the bulk of the data file.
        for (wrtItr = Records.begin ();ok && wrtItr != Records.end ();wrtItr++)
        {
            ok = wrtItr->WriteToStream (oStrm,status,delimiters);
            if (!ok)
            {
                status.SetLineNbr (static_cast<unsigned long>(wrtItr - Records.begin()));
                status.SetObjectName (ObjectName);
            }
        }
    }
    return ok;
}
const wchar_t* TcsCsvFileBase::GetObjectName (void) const
{
	return ObjectName.c_str ();
}
void TcsCsvFileBase::GetObjectName (std::wstring& objName) const
{
	objName = ObjectName;
}
bool TcsCsvFileBase::BuildIndex (TcsCsvStatus& status)
{
    bool ok = true;
    unsigned recNbr = 0;
    recItr itr;
    std::wstring keyValue;
    std::pair<std::map<std::wstring,unsigned>::iterator,bool> idxStatus; 

    Index.clear ();
    for (itr = Records.begin ();ok && itr !=Records.end ();itr++)
    {
        ok = itr->GetField (keyValue,KeyField,status);
        if (ok)
        {
            idxStatus = Index.insert (std::make_pair (keyValue,recNbr));
            ok = idxStatus.second;
            if (!ok)
            {
                status.SetStatus (csvDupIndex);
                status.SetFieldNbr (KeyField);
                status.SetFieldId (keyValue);
                status.SetLineNbr (recNbr + FirstIsLabels);
                status.SetObjectName (ObjectName);
            }
        }
        recNbr += 1;
    }
    return ok;
}
