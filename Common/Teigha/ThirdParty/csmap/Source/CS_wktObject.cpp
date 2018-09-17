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

//lint -esym(766,..\Include\cs_wkt.h)         Disable PC-Lint's warning of unreferenced headers
//lint -esym(766,..\Include\cs_Legacy.h)         Disable PC-Lint's warning of unreferenced headers
//lint -e534                                  ignoring return value

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

extern TrcWktEleTypeMap KrcWktEleTypeMap [];
extern TrcWktAxisValueMap KrcWktAxisValueMap [];

//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following are non-member support functions implemented specifically to
// support processing of WKT data.
std::string rcExtractWkt (std::istream& inStrm,unsigned& lineNbr,unsigned& startLineNbr)
{
	int curChar = '\0';
	int startChar = '\0';
	int termChar = '\0';
	int nestLevel = 0;
	int keyWordIdx = 0;
	int whiteSpaceIdx = 0;

	char keyWord [32];
	char whiteSpace [64];

	enum ErcWktPrsState {	wktScan = 0,
							wktKeyword,
							wktBracket,
							wktCapture,
							wktDone
						};
	ErcWktPrsState state = wktScan;

	// wktPrimitive is the return value.  It is empty upon return if the stream
	// did not have a WKT primitive.  You could also check the stream for EOF.
	// Note, as far as I knowm, WKT is only defined for 8 bit characters.
	std::string wktPrimitive;

	// Skip stuff in stream until we see a WKT keyword.  What little spec
	// there is for WKT says that keywords are always upper case.
	keyWord [0] = '\0';
	whiteSpace [0] = '\0';
	while (state != wktDone)
	{
		if (inStrm.eof ()) break;
		curChar = inStrm.get ();
		if (curChar == '\n')
		{
			lineNbr += 1;
			continue;
		}

		switch (state) {
		case wktScan:
			if (isupper (curChar))
			{
				keyWordIdx = 0;
				keyWord [keyWordIdx++] = static_cast<char>(curChar);
				state = wktKeyword;
			}
			break;

		case wktKeyword:
			if (isupper (curChar) || curChar == '_')
			{
				keyWord [keyWordIdx++] = static_cast<char>(curChar);
			}
			else
			{
				keyWord [keyWordIdx] = '\0';
				ErcWktEleType wktType = rcWktNameToType (keyWord);
				if (wktType != rcWktUnknown)
				{
					startLineNbr = lineNbr;			// line number at which this def starts
					whiteSpace [0] = '\0';
					whiteSpaceIdx = 0;
					if (curChar == '(' || curChar == '[')
					{
						startChar = curChar;
						termChar = (curChar == '(') ? ')' : ']';
						nestLevel = 1;
						state = wktCapture;
						wktPrimitive.erase ();
						wktPrimitive += keyWord;
						wktPrimitive += whiteSpace;
						wktPrimitive += static_cast<char>(startChar);
					}
					else if (isspace (curChar))
					{
						whiteSpace [whiteSpaceIdx++] = static_cast<char>(curChar);
						whiteSpace [whiteSpaceIdx] = '\0';
						state = wktBracket;
					}
					else
					{
						state = wktScan;
					}
				}
				else
				{
					state = wktScan;
				}
			}
			break;

		case wktBracket:
			if (curChar == '(' || curChar == '[')
			{
				startChar = curChar;
				termChar = (startChar == '(') ? ')' : ']';
				state = wktCapture;
				wktPrimitive.erase ();
				wktPrimitive += keyWord;
				wktPrimitive += whiteSpace;
				wktPrimitive += static_cast<char>(startChar);
				nestLevel = 1;
				state = wktCapture;
			}
			else if (isspace (curChar))
			{
				if (whiteSpaceIdx < (int)sizeof (whiteSpace))
				{
					whiteSpace [whiteSpaceIdx++] = static_cast<char>(curChar);
				}
			}
			else
			{
				state = wktScan;
			}
			break;

		case wktCapture:
			wktPrimitive += static_cast<char>(curChar);
			if (curChar == startChar)
			{
				nestLevel += 1;
			}
			else if (curChar == termChar)
			{
				nestLevel -= 1;
				if (nestLevel <= 0)
				{
					state = wktDone;
					nestLevel = 0;
				}
			}
			break;
			
		case wktDone:
			// to keep gcc compiler happy.
			break;
		}
	}
	if (state != wktDone) wktPrimitive.erase ();
	return wktPrimitive;
}
const char *rcWktTypeToName (ErcWktEleType type)
{
	const char *rtnValue = "";

	if (type < rcWktUnknown)
	{
		rtnValue = KrcWktEleTypeMap [type].Name;
	}
	return rtnValue;
}
bool rcWktHasInitialName (ErcWktEleType type)
{
	bool hasInitialName = false;
	if (type < rcWktUnknown)
	{
		hasInitialName = KrcWktEleTypeMap [type].InitlName;
	}
	return hasInitialName;
}
ErcWktEleType rcWktNameToType (const char *name)
{
	TrcWktEleTypeMap *tblPtr;
	for (tblPtr = KrcWktEleTypeMap;tblPtr->Type < rcWktUnknown;tblPtr += 1)			//lint !e440    type of loop variable (tblPtr) is not same as conditional variable (rcWktUnknown)
	{
		if (!strcmp (tblPtr->Name,name)) break;
	}
	return tblPtr->Type;
}
const char *rcWktAxisValueToName (ErcWktAxisValue axisValue)
{
	if (axisValue > rcWktAxisNone && axisValue < rcWktAxisUnknown)
	{
		axisValue = rcWktAxisUnknown;
	}
	return	KrcWktAxisValueMap [axisValue].Name;
}
ErcWktAxisValue rcWktNameToAxisValue (const char *name)
{
	TrcWktAxisValueMap *tblPtr;

	for (tblPtr = &KrcWktAxisValueMap [1];tblPtr->Value < rcWktAxisUnknown;tblPtr += 1)			//lint !e440    type of loop variable (tblPtr) is not same as conditional variable (rcWktUnknown)
	{
		if (!CS_stricmp (tblPtr->Name,name)) break;
	}
	return tblPtr->Value;
}
//newPage//
class TrcWktElements;
///////////////////////////////////////////////////////////////////////////////
// The TrcWktElement Object
///////////////////////////////////////////////////////////////////////////////
// Static Constants, Variables, and Functions
short TrcWktElement::BitCounter (ulong32_t bitMap)
{
	short bitCount (0);
	
	while (bitMap != 0)
	{
		bitCount += static_cast<short>((bitMap & 1) != 0);
		bitMap >>= 1;
	}
	return bitCount;
}
///////////////////////////////////////////////////////////////////////////////
// Construction  /  Destruction  /  Assignment
TrcWktElement::TrcWktElement (void) :	WktParent (0),
										ElementType (rcWktNone),
										ParentType (rcWktNone),
										Value (),
										ElementName (),
										Children (),
										LineNbr (0),
										FileName ()
{
}
// For this constructor, name (if appropriate for the type) must be the first
// CSV field of the value string.
TrcWktElement::TrcWktElement (ErcWktEleType type,const char *value) :
										WktParent (0),
										ElementType (type),
										ParentType (rcWktNone),
										Value (value),
										ElementName (),
										Children (),
										LineNbr (0),
										FileName ()
{
	CommonConstruct ();
}
// For this constructor, name (if appropriate for the type) must be the first
// CSV field of the value string.
TrcWktElement::TrcWktElement (ErcWktEleType type,const std::string& value) :
										WktParent (0),
										ElementType (type),
										ParentType (rcWktNone),
										Value (value),
										ElementName (),
										Children (),
										LineNbr (0),
										FileName ()
{
	CommonConstruct ();
}
// This constructor will prepend the name to the value for you.
TrcWktElement::TrcWktElement (ErcWktEleType type,const std::string& name,const std::string& value) :
										WktParent (0),
										ElementType (type),
										ParentType (rcWktNone),
										Value (),
										ElementName (name),
										Children (),
										LineNbr (0),
										FileName ()
{
	Value = "\"" + name + "\"" + "," + value;
}
TrcWktElement::TrcWktElement (const char *wellKnownText) :
										WktParent (0),
										ElementType (rcWktNone),
										ParentType (rcWktNone),
										Value (),
										ElementName (),
										Children (),
										LineNbr (0),
										FileName ()
{
	size_t eleStrt;
	size_t eleTerm;

	ElementType = ParseWellKnownText (Value,eleStrt,eleTerm,wellKnownText);
	if (ElementType == rcWktUnknown)
	{
		// Guess the text was not so Well Known!!!
		Value.erase ();
	}
	else
	{
		CommonConstruct ();
	}
}
TrcWktElement::TrcWktElement (const TrcWktElement& source) :
										WktParent (0),
										ElementType (source.ElementType),
										ParentType (source.ParentType),
										Value (source.Value),
										ElementName (source.ElementName),
										Children (source.Children),
										LineNbr (0),
										FileName ()
{
}
TrcWktElement::~TrcWktElement (void)
{
	// Nothing to do here, yet!
}											//lint !e1540    member pointer variable WktParent not free'd in destructor
TrcWktElement& TrcWktElement::operator= (const TrcWktElement& rhs)
{
	if (&rhs != this)
	{
		WktParent   = 0;
		ElementType = rhs.ElementType;
		ParentType  = rhs.ParentType;
		Value       = rhs.Value;
		ElementName = rhs.ElementName;
		Children    = rhs.Children;
		LineNbr     = rhs.LineNbr;
		FileName    = rhs.FileName;
	}
	return *this;
}
void TrcWktElement::CommonConstruct (void)
{
	if (HasInitialName ())
	{
		ElementName = ExtractField (0);
		DeleteField (0);
	}
	else
	{
		ElementName.erase ();
	}
}
///////////////////////////////////////////////////////////////////////////////
// Public Named Memebr Functions
bool TrcWktElement::HasInitialName (void) const
{
	return rcWktHasInitialName (ElementType);
}
void TrcWktElement::GetFieldChar (char *bufr,size_t bufrSize,size_t fieldNbr) const
{
	ExtractField (bufr,bufrSize,fieldNbr);
}
std::string TrcWktElement::GetFieldStr (size_t fieldNbr) const
{
	return ExtractField (fieldNbr);
}
double TrcWktElement::GetFieldDouble (size_t fieldNbr) const
{
	char bufr [64];
	ExtractField (bufr,sizeof (bufr),fieldNbr);
	return atof (bufr);
}
long32_t TrcWktElement::GetFieldLong (size_t fieldNbr) const
{
	char bufr [64];
	ExtractField (bufr,sizeof (bufr),fieldNbr);
	return (long32_t)atol (bufr);
}
unsigned TrcWktElement::GetValueCount () const
{
    unsigned valueCount = 0;
	bool inQuote;
	char ch;
	const char *chPtr;	

	// Because we can have quoted strings which can have commas in them,
	// we must avoid the temptation to used "find_first_of" et al.
	inQuote = false;
	chPtr = Value.c_str ();
	if (*chPtr != '\0')
	{
	    // A non-empty string implies that there is at least one value.
	    valueCount += 1;
	}
	while ((ch = *chPtr++) != '\0')
	{
		if (!inQuote)
		{
			if (ch == '\"')
			{
				inQuote = true;
			}
			else
			{
				if (ch == ',')
				{
				    // A comma not within quotes indicates the start
				    // of a new value.  Even if that value is null?
				    valueCount += 1;
				}
			}
		}
		else    // if (inQuote)
		{
			if (ch == '\"')
			{
				if (*chPtr == '\"')
				{
			        // We _assume_ that the double quote character convention
			        // is used should a quote character need to be included
			        // in a quoted string.
			        chPtr += 1;
				}
				else
				{
					inQuote = false;
				}
			}
		}
	}
	return valueCount;
}
unsigned TrcWktElement::GetParameterCount (void) const
{
	size_t index = 0;
	unsigned paramCount = 0;
	const char *nmPtr;
	const TrcWktElement* paramPtr;

    // Count the number of parameters in the WKT string.  Certain flavors like
    // to produce WKT definitions without any parameters and it is convenient
    // to be able to determine this situation.
	for (;;)
	{
		paramPtr = Children.LocateChild (rcWktParameter,index);
		if (paramPtr == 0) break;
		nmPtr = paramPtr->GetElementNameC ();
		if (nmPtr == 0 || *nmPtr == '\0') continue;
		paramCount += 1;
	}
	return paramCount;
}
///////////////////////////////////////////////////////////////////////////
// The following member functions are used to construct a WKT object for
// subsequent output.
void TrcWktElement::AddChild (const TrcWktElement& childElement)
{
	Children.AddChild (childElement,this,ElementType);
}
void TrcWktElement::RemoveChild (const TrcWktElement* toBeRemoved)
{
	Children.RemoveChild (toBeRemoved);
}
void TrcWktElement::ParseChildren (void)
{
	// Extract each sub-element from the value of this element.
	for (;;)
	{
		size_t eleStrt;
		size_t eleTerm;
		ErcWktEleType elementType;
		std::string value;
		const char *wellKnownText = Value.c_str ();
		elementType = ParseWellKnownText (value,eleStrt,eleTerm,wellKnownText);

		// We get an unknown back if there were no more sub-elements.
		if (elementType == rcWktUnknown)
		{
			break;
		}
	
		// Add the element we just extracted to out Children list.
		TrcWktElement newChild (elementType,value);
		AddChild (newChild);
		RemoveElementFromString (Value,eleStrt,eleTerm);
	}

	// Now we need to parse the grandchildren.
	Children.ParseGrandChildren ();
}
void TrcWktElement::ReconstructValue (void)		// from possibly modified children
{
	size_t ii;
	const TrcWktElement* wktPtr;

	Value.erase ();
	Value += '\"';
	Value += ElementName;
	Value += '\"';
	Value += ',';

	switch (ElementType) {
	case rcWktAuthority:
	case rcWktAxis:
	case rcWktCompDCS:
	case rcWktConcatMt:
	case rcWktDatum:
	case rcWktFittedCS:
	case rcWktGeoCCS:
	case rcWktGeogCS:
	case rcWktLocalCS:
	case rcWktMethod:
	case rcWktParameter:
	case rcWktParamMt:
	case rcWktPrimeM:
	case rcWktProjCS:
	case rcWktProjection:
	case rcWktSpheroid:
	case rcWktUnit:
	case rcWktVertDatum:
	case rcWktVertCS:
		for (ii = 0;ii < Children.GetCount ();ii += 1)
		{
			wktPtr = Children [ii];
			Value += wktPtr->ProduceWkt ();
		}
		Value += "]";
		break;
	case rcWktToWgs84:
		break;
	case rcWktInverseMt:
	case rcWktPassThroughMt:
	default:
		// These don't have a name or any sub-elements, so we
		// don't really have anything to do here.
		Value.erase ();
		break;
	case rcWktNone:
	case rcWktGeogTran:
	case rcWktLocalDatum:
	case rcWktUnknown:
		// These are not currently supported.
		break;
	}
}
std::string TrcWktElement::ProduceWkt () const
{
	const TrcWktElement* wktPtr;
	std::string rtnValue;
	const char *typeName = rcWktTypeToName (ElementType);
	rtnValue += typeName;
	rtnValue += "[";

	// The actual value which we produce is a function of the element
	// type.
	if (HasInitialName ())
	{
		rtnValue += '"';
		rtnValue += ElementName;
		rtnValue += '"';
	}

	// Any Children???
	for (size_t ii = 0;ii < Children.GetCount ();ii += 1)
	{
		wktPtr = Children [ii];
		rtnValue += ',';
		rtnValue += wktPtr->ProduceWkt ();
	}

	// Any additional Value???
	if (Value.length () > 0)
	{
		rtnValue += ',';
		rtnValue += Value;
	}
	rtnValue += "]";
	return rtnValue;
}
///////////////////////////////////////////////////////////////////////////////
// Private Support Functions
std::string TrcWktElement::ExtractField (size_t fieldNbr,bool trimQuotes) const
{
	bool inQuote;
	char ch;
	int idx;
	size_t first = 0;
	size_t last = 0;
	size_t curFldNbr = 0;
	const char *chPtr;	

	// Because we can have quoted strings which can have commas in them,
	// we must avoid the temptation to used "find_first_of" et al.
	idx = -1;
	chPtr = Value.c_str ();
	inQuote = false;
	while ((ch = *chPtr++) != '\0')
	{
		idx += 1;
		if (inQuote)
		{
			if (ch == '\"')
			{
				if (*chPtr == '\"')
				{
					idx += 1;
					chPtr += 1;
				}
				else
				{
					inQuote = false;
				}
			}
		}
		else					// !inQuote
		{
			if (ch == '\"')
			{
				inQuote = true;
			}
			else
			{
				if (ch == ',')
				{
					if (curFldNbr == fieldNbr)
					{
						last = static_cast<size_t>(idx - 1); 
					}
					else
					{
						curFldNbr += 1;
						if (curFldNbr == fieldNbr)
						{
							first = static_cast<size_t>(idx + 1);
						}
					}
				}
			}
		}
		if (last != 0) break;
	}
	if (ch == '\0' && curFldNbr == fieldNbr) last = static_cast<size_t>(idx);
	std::string tmpStr = Value.substr (first,(last - first + 1));
	TrimWhiteSpace (tmpStr);
	if (trimQuotes)
	{
		TrimQuotes (tmpStr);
		TrimWhiteSpace (tmpStr);
	}
	return tmpStr;
}
void TrcWktElement::DeleteField (size_t fieldNbr)
{
	bool inQuote;
	char ch;
	int idx;
	size_t first = 0;
	size_t last = 0;
	size_t curFldNbr = 0;
	const char *chPtr;	

	// Because we can have quoted strings which can have commas in them,
	// we must avoid the temptation to used "find_first_of" et al.
	idx = -1;
	chPtr = Value.c_str ();
	inQuote = false;
	while ((ch = *chPtr++) != '\0')
	{
		idx += 1;
		if (inQuote)
		{
			if (ch == '\"')
			{
				if (*chPtr == '\"')
				{
					idx += 1;
					chPtr += 1;
				}
				else
				{
					inQuote = false;
				}
			}
		}
		else			// !inQuote
		{
			if (ch == '\"')
			{
				inQuote = true;
			}
			else if (ch == ',')
			{
				if (curFldNbr == fieldNbr)
				{
					// If we are removing the first field, we include the
					// terminating comma.  Otherwise, we leave the terminating
					// comma in.
					if (fieldNbr == 0) last = static_cast<size_t>(idx);
					else               last = static_cast<size_t>(idx - 1);
				}
				else
				{
					curFldNbr += 1;
					if (curFldNbr == fieldNbr)
					{
						// Can't get here for field zero, and we include the
						// initial comma in the erase portion.
						first = static_cast<size_t>(idx);
					}
				}
			}
		}
		if (last != 0) break;
	}
	if (ch == '\0' && curFldNbr == fieldNbr) last = static_cast<size_t>(idx);
	if (last != 0)
	{
		Value.erase (first,(last - first + 1));
		TrimWhiteSpace (Value);
	}
}
void TrcWktElement::ExtractField (char *bufr,size_t bufrSize,size_t fieldNbr) const
{
	std::string tmpStr = ExtractField (fieldNbr);
	const char *chPtr = tmpStr.c_str ();
	strncpy (bufr,chPtr,bufrSize);
	*(bufr + bufrSize - 1) = '\0';
}
void TrcWktElement::TrimQuotes (std::string& value)
{
	size_t length;
	
	length = value.length ();
	if (length >= 2)
	{
		if (value [0] == '\"' && value [length - 1] == '\"')
		{
			value.erase (length - 1,1);
			value.erase (0,1);	
		}
	}
}
void TrcWktElement::TrimWhiteSpace (std::string& value)
{
	size_t begin;		// index of first char of trimmed string
	size_t end;			// index of last char of trimmed string
	size_t last;		// index of the last char of the untrimmed string.
	size_t length;
	
	length = value.length ();
	if (length > 0)
	{
		last = length - 1;
		begin = value.find_first_not_of (' ');
		end = value.find_last_not_of (' ');
		if ((begin > 0 || end < last) && (end >= begin)) 
		{
			value = value.substr (begin,(end - begin + 1));
		}
	}
	return;
}
ErcWktEleType TrcWktElement::ParseWellKnownText (std::string& value,size_t& eleStrt,size_t& eleTerm,const char *wellKnownText)
{
	char curChar = '\0';
	char strtChar = '\0';
	char termChar = '\0';
	int nestLevel = 0;
	size_t keyWordIdx = 0;
	const char *chPtr;
	char keyWord [64];

	value.erase ();
	ErcWktEleType elementType = rcWktUnknown;
	eleStrt = 0;
	eleTerm = 0;

	enum  {	wktScan,
			wktKeyword,
			wktBracket,
			wktCapture,
			wktCaptureQ,
			wktTrim,
			wktDone
		  } state;

	state = wktScan;
	chPtr = wellKnownText;
	while ((curChar = *chPtr++) != '\0')
	{
		switch (state) {
		case wktScan:
			if (isupper (curChar))
			{
				keyWordIdx = 0;
				keyWord [keyWordIdx++] = curChar;
				eleStrt = (unsigned)(chPtr - wellKnownText - 1);
				state = wktKeyword;
			}
			break;

		case wktKeyword:
			if (isupper (curChar) || (curChar == '_') || ((keyWordIdx > 0) && isdigit (curChar)))
			{
				if (keyWordIdx < 60)
				{
					keyWord [keyWordIdx++] = curChar;
				}
			}
			else if (curChar == '(' || curChar == '[')
			{
				keyWord [keyWordIdx] = '\0';
				elementType = rcWktNameToType (keyWord);
				if (elementType == rcWktUnknown)
				{
					keyWordIdx = 0;	
					keyWord [0] = '\0';		//make debugging easier
					eleStrt = 0;
					state = wktScan;
				}
				else
				{
					strtChar = curChar;
					termChar = (strtChar == '(') ? ')' : ']';
					value.erase ();
					nestLevel = 1;
					state = wktCapture;
				}
			}
			else if (isspace (curChar))
			{
				state = wktBracket;
			}
			else
			{
				keyWordIdx = 0;	
				keyWord [0] = '\0';		//make debugging easier
				eleStrt = 0;
				state = wktScan;
			}
			break;

		case wktBracket:
			if (curChar == '(' || curChar == '[')
			{
				keyWord [keyWordIdx] = '\0';
				elementType = rcWktNameToType (keyWord);
				if (elementType == rcWktUnknown)
				{
					keyWordIdx = 0;	
					keyWord [0] = '\0';		//make debugging easier
					eleStrt = 0;
					state = wktScan;
				}
				else
				{
					strtChar = curChar;
					termChar = (strtChar == '(') ? ')' : ']';
					value.erase ();
					nestLevel = 1;
					state = wktCapture;
				}
			}
			else if (!isspace (curChar))
			{
				keyWordIdx = 0;	
				keyWord [0] = '\0';		//make debugging easier
				eleStrt = 0;
				state = wktScan;
			}
			break;

		case wktCapture:
			if (curChar == termChar)
			{
				nestLevel -= 1;
				if (nestLevel <= 0)
				{
					// Don't add the terminating bracket.
					if (*chPtr == '\0')
					{
						eleTerm = (unsigned)(chPtr - wellKnownText - 1);
						state = wktDone;
					}
					else
					{
						state = wktTrim;
					}
				}
				else
				{
					value += curChar;
				}
			}
			else if (curChar == strtChar)
			{
				value += curChar;
				nestLevel += 1;
			}
			else if (curChar == '\"')
			{
				value += curChar;
				state = wktCaptureQ;
			}
			else if (curChar != '\n')
			{
				value += curChar;
			}
			break;

		case wktCaptureQ:
			value += curChar;
			if (curChar == '\"' && *chPtr == '\"')
			{
				// Escaped quote, copy both and maintain state.
				value += *chPtr++;
			}
			else if (curChar == '\"')
			{
				state = wktCapture;
			}
			break;

		case wktTrim:
			if (!isspace (curChar))
			{
				eleTerm = (unsigned)(chPtr - wellKnownText - 2);
				state = wktDone;
			}
			break;

		case wktDone:
		default:
			// To keep lint happy.			
			break;
		}
		if (state == wktDone) break;
	}
	if (!value.empty ())
	{
		TrimWhiteSpace (value);
	}
	return elementType;
}
// eleStrt and eleTerm are the index values for the first and last characters
// to be removed.  We may remove some extra commas and whitespace as indicated
// by the surrounding data.
void TrcWktElement::RemoveElementFromString (std::string& value,size_t eleStrt,size_t eleTerm)
{
	size_t begIdx = eleStrt;
	size_t endIdx = eleTerm;

	// Move begIdx back over any leading whitespace in the stuff we are to remove.
	while (begIdx > 0 && isspace (value [begIdx - 1])) begIdx -= 1;

	// If the thing we are removing is followed by a comma, we always remove that.
	if ((endIdx+1) < value.length() && value [endIdx + 1] == ',')
	{
		endIdx += 1;
	}
	else
	{
		// Here if the thing we are removing is not followed by a comma.
		// Remove any comma which might precede the thing we are removing.
		if (begIdx > 0 && value [begIdx - 1] == ',')
		{
			begIdx -= 1;
			while (begIdx > 0 && isspace (value [begIdx])) begIdx -= 1;
		}
	}
	value.erase (begIdx,(endIdx - begIdx + 1));
}
// This function will only look at the children of this element.  Returns the
// first child so located.
const TrcWktElement *TrcWktElement::ChildLocate (ErcWktEleType type) const
{
	return Children.LocateChild (type);
}
// This function will only look at the children of this element.  Use the index
// arument to step through multiple children of the same type.
const TrcWktElement *TrcWktElement::ChildLocate (ErcWktEleType type,size_t& index) const
{
	return Children.LocateChild (type,index);
}
// This function will not only look at the children of this element, but also
// cause a search of all children.  In other words, a search is a recursive
// search, while a Locate is simply a search of the children of a specific
// elements.  Returns the first element so located.
const TrcWktElement *TrcWktElement::ChildSearch (ErcWktEleType type) const
{
	size_t childIdx;
	const TrcWktElement *rtnValue = 0;
	const TrcWktElement *srchPtr;

	rtnValue = ChildLocate (type);
	if (rtnValue == 0)
	{
		for (childIdx = 0;childIdx < Children.GetCount ();childIdx += 1)
		{
			srchPtr = Children [childIdx];
			rtnValue = srchPtr->ChildSearch (type);
			if (rtnValue != 0) break;
		}
	}
	return rtnValue;
}
const TrcWktElement* TrcWktElement::ParameterLocate (ErcWktFlavor flavor,short csMapParmCode) const
{
	size_t index = 0;
	long32_t adskCode;
	ulong32_t ulAdskCode;
	EcsNameFlavor nmFlavor;
	const char *nmPtr;
	const TrcWktElement* rtnParameter = 0;
	const TrcWktElement* srchParameter;

	nmFlavor = csWktFlvrToCsMapFlvr (flavor);
	
	// We use a double loop.  Outer loop, which should be the smallest, steps
	// through each WKT PARAMETER object contained in this element.
	do
	{
		srchParameter = Children.LocateChild (rcWktParameter,index);
		if (srchParameter == 0) break;
		nmPtr = srchParameter->GetElementNameC ();
		if (nmPtr == 0 || *nmPtr == '\0') continue;

		// OK, we have a parameter with a name.  We need to see if it meets our criteria.
		ulAdskCode = csMapNameToIdC (csMapParameterKeyName,csMapFlvrCsMap,nmFlavor,nmPtr);	//??//
		adskCode = static_cast<long32_t>(ulAdskCode);
		if (adskCode == csMapParmCode)
		{
			rtnParameter = srchParameter;
		}
	} while (rtnParameter == 0);
	return rtnParameter;
}
ErcWktAxisId TrcWktElement::GetAxisId (void) const
{	
	ErcWktAxisId axisId = rcWktAxisIdNone;
	if (ElementType == rcWktAxis)
	{
		if      (ElementName == "Lat") axisId = rcWktAxisIdLat;
		else if (ElementName == "Lon") axisId = rcWktAxisIdLng;
		else if (ElementName == "X")   axisId = rcWktAxisIdX;
		else if (ElementName == "Y")   axisId = rcWktAxisIdY;
		else if (ElementName == "Z")   axisId = rcWktAxisIdZ;
	}
	return axisId;
}
ErcWktAxisValue TrcWktElement::GetAxisValue (void) const
{
	ErcWktAxisValue axisValue = rcWktAxisNone;
	if (ElementType == rcWktAxis)
	{
		axisValue = rcWktNameToAxisValue (Value.c_str ());
	}
	return axisValue;
}
// Extract up to four names, dependning upon the type, and then use the Name
// Mapper to determine the flavors for which the names are valid.  The returned
// bit map is a logical AND of the flavor bit maps for all four names.  Thus,
// if the return value is not zero, the flavor of the result is pretty solid.
// If the return value is zero, the flavor cannot be determined by mapping
// the names.  This function could return a bit map with more than one bit
// set.  In that case, you take your choice based on some sort of
// precedence, or AND it with a flavor determination based on other
// criteria.
ulong32_t TrcWktElement::NameBitMap (void) const
{
	ulong32_t oracleBit;
	ulong32_t oracleBits;
	ulong32_t oracle9Bit;
	ulong32_t oracleMask;

	const char* wktPrjName = 0;
	const char* wktGcsName = 0;
	const char* wktDtmName = 0;
	const char* wktElpName = 0;
	
	const TrcWktElement* gcsElePtr = 0;
	const TrcWktElement* dtmElePtr = 0;
	const TrcWktElement* elpElePtr = 0;

	ulong32_t prjNmBitMap = 0;
	ulong32_t geoNmBitMap = 0;
	ulong32_t dtmNmBitMap = 0;
	ulong32_t elpNmBitMap = 0;
	ulong32_t flvrBitMap = 0;

	oracleBit  = (1 << (csMapFlvrOracle  - 1));
	oracle9Bit = (1 << (csMapFlvrOracle9 - 1));
	oracleMask = oracleBit | oracle9Bit;

	if (ElementType == rcWktProjCS)
	{
		wktPrjName = GetElementNameC ();
		gcsElePtr = ChildLocate (rcWktGeogCS);
	}
	else if (ElementType == rcWktGeogCS)
	{
		gcsElePtr = this;
	}
	if (gcsElePtr != 0)
	{
		wktGcsName = gcsElePtr->GetElementNameC ();
		dtmElePtr = gcsElePtr->ChildLocate (rcWktDatum);
		if (dtmElePtr != 0)
		{
			wktDtmName = dtmElePtr->GetElementNameC ();
			elpElePtr = dtmElePtr->ChildLocate (rcWktSpheroid);
			if (elpElePtr != 0)
			{
				wktElpName = elpElePtr->GetElementNameC ();
			}
		}
	}

	if (wktPrjName != 0)
	{
		prjNmBitMap = csGetFlavorBitMapC (csMapProjectedCSysKeyName,wktPrjName);
	}
	if (wktGcsName != 0)
	{
		geoNmBitMap = csGetFlavorBitMapC (csMapGeographicCSysKeyName,wktGcsName);
	}
	if (wktDtmName != 0)
	{
		dtmNmBitMap = csGetFlavorBitMapC (csMapDatumKeyName,wktDtmName);
	}
	if (wktElpName != 0)
	{
		elpNmBitMap = csGetFlavorBitMapC (csMapEllipsoidKeyName,wktElpName);
	}

	// Kludge for Oracle 9.  The GEOGCS names in the PROJCS definitions are not
	// the same as the names used in the GEOGCS definitions.  In the PROJCS
	// definitions, the GEOGCS names are (only usually, of course) the same as
	// the DATUM name.
	oracleBits = (prjNmBitMap | dtmNmBitMap | elpNmBitMap) & oracleMask;
	if (oracleBits != 0)
	{
		if (CS_stristr (wktDtmName,"(EPSG ID") == 0 ||
			CS_stristr (wktElpName,"(EPSG ID") == 0)
		{
			geoNmBitMap |= oracle9Bit;
		}
		else
		{
			geoNmBitMap |= oracleBit;
		}
	}

	flvrBitMap = 0xFFFFFFFF;
	if (prjNmBitMap != 0)
	{
		flvrBitMap &= prjNmBitMap;
	}
	if (geoNmBitMap != 0)
	{
		flvrBitMap &= geoNmBitMap;
	}
	if (dtmNmBitMap != 0)
	{
		flvrBitMap &= dtmNmBitMap;
	}
	if (elpNmBitMap != 0)
	{
		flvrBitMap &= elpNmBitMap;
	}
	return (flvrBitMap != 0xFFFFFFFF) ? flvrBitMap : 0;
}
ulong32_t TrcWktElement::ProjectionBitMap (void) const
{
	ulong32_t flvrBitMap = 0;
	const char* cPtr;
	const char* wktPrjctnName = 0;
	const TrcWktElement* prjctnElePtr = 0;

	if (ElementType == rcWktProjCS)
	{
		prjctnElePtr = ChildLocate (rcWktProjection);
		if (prjctnElePtr != 0)
		{
			wktPrjctnName = prjctnElePtr->GetElementNameC ();
			
			// Special kludge for Oracle.  Oracle 10g likes to refer to an
			// EPSG OP code (a coordinate operation code, not a projection
			// code).  If we see such a reference, we'll assume Oracle 10g
			// for now.  Since we are only determining flavor here, we don't
			// really care about the code; that's dealt with elsewhere.
            cPtr = strstr(wktPrjctnName,"EPSG OP");
            if (cPtr != NULL)
            {
				flvrBitMap = 1 << (static_cast<int>(csMapFlvrOracle) - 1);
			}
			else
			{
				flvrBitMap = csGetFlavorBitMapC (csMapProjectionKeyName,wktPrjctnName);
			}
		}
	}
	return flvrBitMap;
}
ulong32_t TrcWktElement::ParameterBitMap (short& paramCount) const
{
	ulong32_t flvrBitMap = 0xFFFFFFFF;
	size_t prmIdx;
	const char* wktParamName = 0;
	const TrcWktElement* paramElePtr = 0;

	paramCount = 0;
	if (ElementType == rcWktProjCS)
	{
		prmIdx = 0;
		while ((paramElePtr = ChildLocate (rcWktParameter,prmIdx)) != 0)
		{
			wktParamName = paramElePtr->GetElementNameC ();
			flvrBitMap &= csGetFlavorBitMapC (csMapParameterKeyName,wktParamName);
			prmIdx += 1;
			paramCount += 1;
		}	
	}
	return (flvrBitMap != 0xFFFFFFFF) ? flvrBitMap : 0;
}
ulong32_t TrcWktElement::LinearUnitBitMap (void) const
{
	ulong32_t flvrBitMap = 0;
	const char* wktUnitName = 0;
	const TrcWktElement* unitElePtr = 0;

	if (ElementType == rcWktProjCS || ElementType == rcWktLocalCS)
	{
		unitElePtr = ChildLocate (rcWktUnit);
		if (unitElePtr != 0)
		{
			wktUnitName = unitElePtr->GetElementNameC ();
			flvrBitMap = csGetFlavorBitMapC (csMapLinearUnitKeyName,wktUnitName);
		}
	}
	return flvrBitMap;
}
ulong32_t TrcWktElement::AngularUnitBitMap (void) const
{
	ulong32_t flvrBitMap = 0;
	const char* wktUnitName = 0;
	const TrcWktElement* geogElePtr = 0;
	const TrcWktElement* unitElePtr = 0;

	geogElePtr = this;
	if (ElementType == rcWktProjCS)
	{
		geogElePtr = ChildLocate (rcWktGeogCS);
	}
	if (geogElePtr != 0)
	{
		unitElePtr = geogElePtr->ChildLocate (rcWktUnit);
		if (unitElePtr != 0)
		{
			wktUnitName = unitElePtr->GetElementNameC ();
			flvrBitMap = csGetFlavorBitMapC (csMapAngularUnitKeyName,wktUnitName);
		}
	}
	return flvrBitMap;
}
ErcWktFlavor TrcWktElement::DetermineFlavor (ErcWktFlavor preferredFlvr) const
{
	bool isFlvr;
	int shiftValue;	
	ErcWktFlavor wktFlavor = wktFlvrUnknown;
	ulong32_t flvrBitMap;
	ulong32_t preferredBit;
	EcsNameFlavor prefFlvrNm;

	ulong32_t bmNames;
	ulong32_t bmProjection;
	ulong32_t bmParameters;
	ulong32_t bmProjUnit;
	ulong32_t bmGeogUnit;

	// Order the flavors in this array in the order of desired precedence.
	EcsNameFlavor flavors [] = {	csMapFlvrUser,
									csMapFlvrEsri,
									csMapFlvrOracle,
									csMapFlvrAutodesk,
									csMapFlvrOGC,
									csMapFlvrOCR,
									csMapFlvrOracle9,
									csMapFlvrIBM,
									csMapFlvrGeoTools,
									csMapFlvrEpsg,
									csMapFlvrBentley,
									csMapFlvrSafe,
									csMapFlvrMapInfo,
									csMapFlvrCtmx,
									csMapFlvrCsMap,
									csMapFlvrGeoTiff,
									csMapFlvrOem,
									csMapFlvrAnon01,
									csMapFlvrAnon02,
									csMapFlvrAnon03,
									csMapFlvrAnon04,
									csMapFlvrAnon05,
									csMapFlvrAnon06,
									csMapFlvrAnon07,
									csMapFlvrAnon08,
									csMapFlvrAnon09,
									csMapFlvrAnon10,
									csMapFlvrAnon11,
									csMapFlvrAnon12,
									csMapFlvrAnon13,
									csMapFlvrLegacy,
									csMapFlvrUnknown
							   };
	
	bmNames = NameBitMap ();
	if (ElementType == rcWktProjCS)
	{
		short paramCount;
		
		bmProjection = ProjectionBitMap ();
		bmParameters = ParameterBitMap (paramCount);
		bmProjUnit = LinearUnitBitMap ();
		bmGeogUnit = AngularUnitBitMap ();
		flvrBitMap = bmProjection & bmProjUnit & bmGeogUnit;
		if (paramCount != 0)
		{
			flvrBitMap &= bmParameters;
		}
	}
	else if (ElementType == rcWktGeogCS)
	{
		flvrBitMap = AngularUnitBitMap ();
	}
	else if (ElementType == rcWktLocalCS)
	{
		flvrBitMap = LinearUnitBitMap ();
	}
	else
	{
		flvrBitMap = 0;
	}
	
	if (bmNames != 0)
	{
		flvrBitMap &= bmNames;
	}

	if (flvrBitMap != 0UL)
	{
		if (preferredFlvr != wktFlvrNone)
		{
			prefFlvrNm = csWktFlvrToCsMapFlvr (preferredFlvr);
			preferredBit = (1 << (prefFlvrNm - 1));					//lint !e701    left shift of a signed(?) integer
			if ((flvrBitMap & preferredBit) != 0)
			{
				wktFlavor = preferredFlvr;
			}
		}
		if (wktFlavor == wktFlvrUnknown)
		{
			// Preferred wasn't specified, or that isn't it.  Choose the
			// first flavor in our canned preferred list and return it.
			for (size_t idx = 0;flavors [idx] != csMapFlvrUnknown;idx += 1)
			{
				shiftValue = static_cast<int>(flavors [idx]) - 1;
				isFlvr = ((flvrBitMap & (1U << shiftValue)) != 0);
				if (isFlvr)
				{
					wktFlavor = csCsMapFlvrToWktFlvr (flavors [idx]);
					break;
				}
			}
		}
	}
	return wktFlavor;
}
//newPage//
///////////////////////////////////////////////////////////////////////////////
// TrcWktElements  --  Abstracts a vector of TrcWktElement
TrcWktElements::TrcWktElements (void)
{
}
TrcWktElements::TrcWktElements (const TrcWktElements& source) :
										Elements (source.Elements)
{
}
TrcWktElements::~TrcWktElements (void)
{
	// Nothing to do here, yet!
}
TrcWktElements& TrcWktElements::operator= (const TrcWktElements& rhs)
{
	if (&rhs != this)
	{
		Elements = rhs.Elements;
	}
	return *this;
}
const TrcWktElement* TrcWktElements::operator[] (ErcWktEleType type) const
{
	return LocateChild (type);
}
const TrcWktElement* TrcWktElements::operator[] (size_t index) const
{
	return (index < Elements.size ()) ? &Elements [index] : 0;
}
const TrcWktElement* TrcWktElements::LocateChild (ErcWktEleType type) const
{
	size_t index;
	const TrcWktElement* rtnValue = 0;

	for (index = 0;index < Elements.size ();index += 1)
	{
		if (Elements [index].GetElementType () == type)
		{
			rtnValue = &Elements [index];
			break;
		}
	}
	return rtnValue;
}
// Having this as an inline defined in the header has caused problems
// with some compilers.
size_t TrcWktElements::GetCount (void) const
{
	return Elements.size ();
}
const TrcWktElement* TrcWktElements::LocateChild (ErcWktEleType type,size_t& index) const
{
	size_t myIndex = index;
	const TrcWktElement* rtnValue = 0;

	for (myIndex = index;myIndex < Elements.size ();myIndex += 1)
	{
		if (Elements [myIndex].GetElementType () == type)
		{
			rtnValue = &Elements [myIndex];
			break;
		}
	}
	if (rtnValue != 0) index = myIndex + 1;
	return rtnValue;
}
TrcWktElements& TrcWktElements::AddChild (const TrcWktElement& newChild,
										  const TrcWktElement* parent,
										  ErcWktEleType parentType)
{
	Elements.push_back (newChild);

	TrcWktElement& newChldRef = Elements [Elements.size () - 1];
	newChldRef.SetParent (parent);
	newChldRef.SetParentType (parentType);
	return *this;
}
void TrcWktElements::ParseGrandChildren (void)
{
	std::vector<TrcWktElement>::iterator wktItr;

	for (wktItr = Elements.begin ();wktItr != Elements.end ();wktItr++)
	{
		wktItr->ParseChildren ();
	}
}
TrcWktElements& TrcWktElements::RemoveChild (const TrcWktElement* toBeRemoved)
{
	std::vector<TrcWktElement>::iterator wktItr;

	for (wktItr = Elements.begin ();wktItr != Elements.end ();wktItr++)
	{
		if (&(*wktItr) == toBeRemoved)
		{
			Elements.erase (wktItr);
			break;
		}
	}
	return *this;
}
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following two functions are used to extract the identification numbers
// from the data files we have been provided with.  They MUST extract the
// 'next' id number without processing even the first character of the WKT
// definition.  This is based on very subjective data.  Hopefully, it will
// not need to be used often as once the name/number mapping is done correctly,
// this process will not need to be repeated.
//
long32_t csExtractEsriNbr (std::istream& inStrm,unsigned& lineNbr)
{
	int curChar;
	long32_t esriNbr = -1;
	char* dummy;
	char contextData [8];
	char nbrNameTag [] = "<b> ";
	char wktDefTag []  = "colSpan";

	// Assumption here is that the file is the HTML file you can download from
	// the web.  Thus, we are looking for a certain HTML sequence which
	// encloses the identification number.  If we don't see it before we
	// encounter a sequence that normally preceeds a WKT definition, we return
	// a zero.  If we see an 'end of file' in all this, we return -1L.
	
	// We start a loop which will extract data into the contectData array in a
	// circular fashion.  That is, the context data array will always have the
	// last 7 characters obtained from the stream.  We use those 8 characters
	// to determine where we are in the data stream.
	contextData [0] = ' ';
	contextData [1] = ' ';
	contextData [2] = ' ';
	contextData [3] = ' ';
	contextData [4] = ' ';
	contextData [5] = ' ';
	contextData [6] = ' ';
	contextData [7] = '\0';
	while (esriNbr <= 0L)
	{
		if (inStrm.eof ()) break;
	
		curChar = inStrm.get ();
		if (curChar == '\n')
		{
			lineNbr += 1;
		}
		contextData [0] = contextData [1];
		contextData [1] = contextData [2];
		contextData [2] = contextData [3];
		contextData [3] = contextData [4];
		contextData [4] = contextData [5];
		contextData [5] = contextData [6];
		contextData [6] = static_cast<char>(curChar);
		
		// See what we have now.
		if (!strcmp (contextData,wktDefTag))
		{
			// We didn't see an identification number.  Return zero now.
			esriNbr = 0L;
			break;
		}
		else if (contextData [0] == '<')
		{
			if (!strncmp (contextData,nbrNameTag,4))
			{
				size_t nbrIdx = 0;
				char nbrData [16];
				nbrData [nbrIdx] = '\0';
				
				// Could be a number.  Could be a name
				while (isspace (inStrm.peek ()))
				{
					inStrm.get ();
				}
				if (isdigit (contextData [5]))
				{
					nbrData [nbrIdx++] = contextData [5];
					nbrData [nbrIdx] = '\0';
				}
				if (isdigit (contextData [6]))
				{
					nbrData [nbrIdx++] = contextData [6];
					nbrData [nbrIdx] = '\0';
				}
				while (isdigit (inStrm.peek ()))
				{
					// Looks like a number.  We chase it and see where it leads us.
					nbrData [nbrIdx++] = static_cast<char>(inStrm.get ());
					nbrData [nbrIdx] = '\0';
				}
				if (nbrIdx >= 4)
				{
					// If we got 4 pr more digits, we assume we have seen an
					// identification number.  We capture it, and return it now.
					esriNbr = CS_strtol (nbrData,&dummy,10);
				}
			}
		}
	}
	return esriNbr;
}
