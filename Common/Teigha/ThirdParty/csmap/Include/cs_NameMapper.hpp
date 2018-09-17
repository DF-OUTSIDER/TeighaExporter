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

#if !defined (csNameMapperHpp)
#define csNameMapperHpp

#include <set>

#include "cs_CsvFileSupport.hpp"

// Include the 'C' header stuff with 'C" linkage so that there is only one
// copy of all that stuff.
#include "cs_NameMapperSupport.h"

class TcsGenericId;
class TcsNameMap;
class TcsNameMapper;

#if defined(__GNUC__) && defined(_WIN32)
#define cs_swprintf _snwprintf
#else
#define cs_swprintf swprintf
#endif
///////////////////////////////////////////////////////////////////////////////
// The following operator overrides for the name mapper enumerations help make
// the implementation general and easy to expand; i.e. just add an entry to
// the enumeration definition.
//
EcsMapObjType& operator++ (EcsMapObjType& mapClass);	//preIncrememnt operator
EcsNameFlavor& operator++ (EcsNameFlavor& flavor);	// preIncrememnt operator
EcsMapTableFields& operator++ (EcsMapTableFields& flavor);	// preIncrememnt operator

///////////////////////////////////////////////////////////////////////////////
// The following global function is used by the above functions to obtain a
// pointer to the mapping class.  This is used instead of a global variable to
// support future modifications.  Note, this is intentionally does not have
// "C" linkage.  Maybe that will change.
TcsNameMapper* cmGetNameMapperPtr (bool release = false);

///////////////////////////////////////////////////////////////////////////////
// The following function is used to map a flavor ID to a string for reporting
// purposes.  This is separate from that defined below such that it works
// independently of a TcsNameMapper object.  Probably should be part of the
// 'C' interface.
const wchar_t* cmFlavorToName (EcsNameFlavor flavor);

///////////////////////////////////////////////////////////////////////////////
// TcsGenericId  --  Internal Definition ID number
//
// The TcsGenericId object is simply a number assigned to definitions in a
// manner which assures that each entry added to the TcsNameMapper object has a
// unique number assigned.  When generating a TcsGenericId number, the flavor
// used ((flavor - 1) * 1.0E+06) to insure the numeric value is unique
// across flavors.  For items which are unnumbered by the specific flavor,
// (e.g. ESRI does not assign numbers to ellipsoid definitions) values are
// assigned sequentially from KcsNameMapInitial on up (see
// TcsNameMapper::GetNextDfltId); again an attempt to assign a unique value.
// The end result is the generic ID for all EPSG definitions is the EPSG code
// number.  For ESRI, values will be between KcsNameMapBias (100 million) and
// 199999999UL.
//
// Generic ID's are used to map one flavor of name/number to another in a
// generic way (hence the name).  That is, one obtains the generic ID of the
// item which is to be mapped (be it a name or number), and then uses the
// generic ID to obtain the item desired (be it name or number).  Thus, a
// totally generic mapping system becomes available, and new flavors can
// be added by simply adding to the flavor enumeration above.
//
// It is the specific intent of the design of this group of objects that the
// numeric value of a generic ID is not used externally.  That is, you must
// not rely on the GenericID numeric value assigned to a specific object in
// one Name Mapper table or object will be the same in another.  The actual
// numeric value associated with a Generic ID should never in any be used
// for any purpose with a different Name Mapper object than that from which
// it was obtained.

class TcsGenericId
{
public:
	static const unsigned long KcmFlvrModulus;
	///////////////////////////////////////////////////////////////////////////
	// Construction  /  Destruction  /  Assignment
	TcsGenericId (unsigned long id = 0UL);
	TcsGenericId (EcsNameFlavor idFlavor,unsigned long id);
	TcsGenericId (const TcsGenericId& source);
	TcsGenericId& operator= (const TcsGenericId& rhs);
	~TcsGenericId (void);
	///////////////////////////////////////////////////////////////////////////
	// Operator Overrides
	bool operator< (const TcsGenericId& rhs) const;
	bool operator== (const TcsGenericId& rhs) const;
	bool operator!= (const TcsGenericId& rhs) const;
	operator unsigned long () const {return Id; };
	///////////////////////////////////////////////////////////////////////////
	// Named Member functions
	bool IsKnown (void) const {return (Id != 0UL); }
	bool IsNotKnown (void) const {return (Id == 0UL); }
	unsigned long GetGenericId (void) const {return Id; }
	EcsNameFlavor GetInternalIdFlavor (void) const;
	EcsNameFlavor ExtractFlavorId (unsigned long& flvrId) const;
private:
	unsigned long Id;
};
extern const TcsGenericId KcsGenericIdNull;

//newPage//
///////////////////////////////////////////////////////////////////////////////
// TcsNameMap  --  A specific name/ID map entry.
//
// An instance of this object establishes the existence of a name, and
// optionally an associated numeric ID value.  The Type member defines the name
// scope to which the name and/or numeric ID belongs, and the Flavor defines
// the flavor to which it belongs.
//
// The primary premise of this object is that these objects will be maintained
// in a std::set, wherein the uniqueness is determined by Type, Flavor, and
// Name.  This is required as for many type/flavor combinations, there are
// no numbers, but every type/flavor combination has a name.  The Deprecated
// member has been added to the key so that the same name can reference a
// deprecated definition as a valid one.  This is specifically for EPSG, where
// a definition which replaces a deprecated definition will often have the
// same name, but a different numeric ID value.
//
// While true in 98% of the cases, the above premise fails on two counts:
//
// 1> A given item may have several different names within a flavor.  For
//    example, ESRI likes to use several different names for the
//    Stereographic projection, and also uses two different names for the
//    Lambert Conformal Conic.  To handle this case, we introduce the
//    AliasFlag.  The intent here is that entries where the AliasFlag
//    member is non-zero are examined upon input; but ignored on output.
//
// 2> Within a type and flavor, the same name may be used for different
//    items.  ESRI, for example, will use the name Lambert_Conformal_Conic
//    for both the single and double parallel variations.  (Presumably,
//    the specific variation is to be determined by determining the number
//    of standard parallel parameters present.)  To handle this case,
//    the DupSort member is added.  When duplicate names are added, the
//    DupSort member is used to make each entry unique.  This is done
//    for three purposes.  First, it enables us to have duplicate names
//    for the same item in the std::set without having to resort to a
//    std::multiset.  Second, to insure that the mapping of the name
//    on input produces consistent results.  Third, that the duplicate
//    name can be present in the proper location upon output.  That is,
//    in the case cited above, on input the Lambert_Conformal_Conic
//    name will always produce a mapping to 'LM2SP'; but the name is
//    also present for output when mapping 'LM1SP' to an ESRI name
//    for output.
//
// It is difficult to imagine a case where there are duplicate aliases.
// After all, the idea of an alias is to simply introduce a new and
// different name for an item; therefore, the name should not duplicate
// another.  Since (initially anyway) the mapping table will be maintained
// manually, it is possible for this to happen.  The code is written
// (intended anyway) to handle this and produce the expected results.
//
// The short version of this is simple:  When reading a WKT file (or similar
// file where the Name Mapper is being used) aliases are active and will be
// mapped to the appropriate object while duplicate names will consisently
// map to the same object.  When writing a WKT file, aliases are ignored, and
// duplicate names are available for output as required.
//
// A NumericId value of zero is taken to mean no value is defined or known.
//
class TcsNameMap
{
public:
	// MatchSort produces the order required internally when the TcsNameMapper
	// object is used to perform certain automatic match-up operations (not
	// yet implemented).
	static bool MatchSort (const TcsNameMap& lhs,const TcsNameMap& rhs);
	// CsvSort produces the order desired externally in the .CSV file.
	static bool CsvSort (const TcsNameMap& lhs,const TcsNameMap& rhs);
	///////////////////////////////////////////////////////////////////////////
	// Construction  /  Destruction  /  Assignment
	TcsNameMap (void);
	TcsNameMap (const TcsGenericId& genericId,EcsMapObjType type,EcsNameFlavor flavor,
																 unsigned long numericId,
																 const wchar_t* name);
	TcsNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long numericId,
														const wchar_t* name);
	TcsNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* flavorName,
														short dupSort = 0,
														short aliasFlag = 0);
	TcsNameMap (const TcsNameMap& source);
	virtual ~TcsNameMap (void);
	TcsNameMap& operator= (const TcsNameMap& rhs);
	///////////////////////////////////////////////////////////////////////////
	// Operator Overrides
	// Since the collection used in TcsNameMapper is a 'set', the operator<
	// function determines the unique key used for the set.  This is the
	// primary purpose of this function.
	bool operator< (const TcsNameMap& rhs) const;
	std::wostream operator<< (std::wostream& outStrm) const;	// writes CSV
	///////////////////////////////////////////////////////////////////////////
	// Public Named Member Functions
	EcsCsvStatus ReadFromStream (std::wistream& inStrm);
	EcsCsvStatus ReadFromStream (std::wistream& inStrm,TcsCsvStatus& csvStatus);
	bool HasGenericId (void) const {return GenericId.IsKnown (); }
	bool NeedsGenericId (void) const {return GenericId.IsNotKnown (); }
	TcsGenericId GetGenericId (void) const {return GenericId; }
	EcsMapObjType GetMapClass (void) const {return Type; }
	EcsNameFlavor GetFlavor (void) const {return Flavor; }
	unsigned long GetNumericId (void) const {return NumericId; }
	const wchar_t* GetNamePtr (void) const {return Name.c_str (); }
	short GetDupSort (void) const {return DupSort; }
	short GetAliasFlag (void) const {return AliasFlag; }
	void* GetUserValue (void) const;
	const wchar_t* GetComments (void) const;
	void GetComments (std::wstring& comments) const;
	const wchar_t* GetRemarks (void) const;
	void GetRemarks (std::wstring& remarks) const;
	void SetGenericId (const TcsGenericId& newId) {GenericId = newId; };
	void SetType (EcsMapObjType newClass) {Type = newClass; };
	void SetFlavor (EcsNameFlavor newFlavor) {Flavor = newFlavor; };
	void SetNameId (const wchar_t* newNameId);
	void SetNumericId (unsigned long newNumericId);
	void SetDupSort (short dupSort) {DupSort = dupSort; };
	void SetAliasFlag (short aliasFlag) {AliasFlag = aliasFlag; };
	void* SetUserValue (void* userValue);
	void SetDeprecated (const TcsGenericId& deprecatedBy) {Deprecated = deprecatedBy; };
	void SetRemarks (const wchar_t* remarks);
	void SetComments (const wchar_t* comments);
	TcsGenericId DeprecatedBy (void) {return Deprecated; };
	TcsGenericId DeprecatedBy (void) const {return Deprecated; };
	void WriteAsCsv (std::wostream& outStrm,bool flvrLbls = false) const;
private:
	TcsGenericId GenericId;				// Flavor independent ID
	EcsMapObjType Type;					// Type (namespace) of this definition
	EcsNameFlavor Flavor;				// Flavor of the Name and/or NumericId
	unsigned long NumericId;			// Numeric Code or ID (e.g. EPSG Number)
	std::wstring Name;					// Name of the object
	short DupSort;						// Used to set priority order on
										// duplicate names
	short AliasFlag;					// Set to non-zero if this is an alias
	// The above are straight forward, and all that are populated in the normal
	// case.  The following are used to handle the strange stuff.
	unsigned long Flags;				// Flags used to provide legacy
										// consistency.
	void* User;							// Pointer sized variable for use by
										// the consumer, typically for
										// maintenance purposes.
	TcsGenericId Deprecated;			// If set to a valid value (i.e. not
										// zero, this name/number entry has
										// been deprecated and replaced by the
										// item with the generic ID provided.
	std::wstring Remarks;				// Permanent remarks concerning name
										// mapping as pertains to the actual
										// definitions being mapped.
	std::wstring Comments;				// Comments about the processing as
										// pertains to the mapping algorithms,
										// matching techniques, etc.  I.e.
										// programmer type stuff.
};

///////////////////////////////////////////////////////////////////////////////
// TcsNameMapList  --  An ordered collection of TcsNameMap objects
//
// It is often desirable to obtain an ordered list of all names, or numbers,
// which belong to a specific group or meet an arbitrary selection criteria.
// Generaing a list (i.e. an enumeration) of Oracle name, perhaps with
// associated SRID values, is an example.  The algoirthms used in the Name
// Mapper, being designed to perform single item mappings, are rather
// inefficient for this purpose.  Thus, the TcsNameMapList object, and related
// functions, are provided to support generating such an enumeration in an
// efficient manner.
// 
// This object is a simple collection (currently, a vector is used) of constant
// pointers to TcsNameMap objects which carry the identification information
// for all objects which meet the provided  criteria.  A simple interface to
// iterate through the list is included.
class TcsNameMapList
{
public:
    //=========================================================================
    // Construction,  Assignment,  Destruction
	TcsNameMapList (void);
	TcsNameMapList (const TcsNameMapList& source);
	virtual ~TcsNameMapList (void);
	TcsNameMapList& operator= (const TcsNameMapList& rhs);
    //=========================================================================
    // Operator Overrides
    TcsNameMapList& operator+= (const TcsNameMap* newNameMapPtr);
    TcsNameMapList& operator-= (const TcsNameMap* existingNameMapPtr);
    //=========================================================================
    // Public Named Member Functions
    size_t GetCount (void) const { return NameMapList.size (); };
	void AddNameMap (const TcsNameMap* newNameMapPtr);
	bool RemoveNameMap (const TcsNameMap* existingNameMapPtr);
	const TcsNameMap* GetNameMapPtr (size_t index) const;
	void Erase (void);
private:
    //=========================================================================
    // Private Data Members
	std::vector<const TcsNameMap*> NameMapList;    
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// TcsKeyNameMapFile  --  Represents an in-memory form of a name mapping table
//
// Objects of this type are used to represent in a memory resident, easily
// accessible, form a mapping table as one might use to maintain name mappings
// in an Excel spreadsheet.  The data is expected to be exported in CSV form
// before use.
class TcsKeyNameMapFile : public TcsCsvFileBase
{
    static const unsigned BogusNbr;
    
    // This table is used to associate the individual fields in the source .CSV
    // with field names, flavors, etc.  Additional flavors, numbers, names,
    // etc. can be added by simply adding to this table.  That's the idea,
    // anyway. 
    struct TcsMapTableFields
    {
        EcsMapTableFields FldId;
        EcsNameFlavor     Flavor;          // Flavor associated with the field
        bool              IsNbr;
        wchar_t           FldName [64];
    };
    static const TcsMapTableFields KcsMapTableFields [];
public:
    static void WriteCsvFileHeader (std::wostream& oStrm);
    //=========================================================================
    // Construction, Destruction, and Assignment
    TcsKeyNameMapFile (const wchar_t* mapFilePath,short fldCnt);
    TcsKeyNameMapFile (const TcsKeyNameMapFile& source);
    virtual ~TcsKeyNameMapFile (void);
    TcsKeyNameMapFile& operator= (const TcsKeyNameMapFile& rhs);
    //=========================================================================
    // Operator Overrides
    //=========================================================================
    // Public Named Member Functions
    EcsMapTableFields GetNbrFldId (EcsNameFlavor flavor) const;
    EcsMapTableFields GetNameFldId (EcsNameFlavor flavor) const;
    unsigned GetErrorValue (void) const;
    EcsCsvStatus GetStatus (TcsCsvStatus& status) const;
    void Rewind (void);                        // first non-label record
    bool NextRecord (void);
    bool SetCurrentRecord (unsigned recordNbr);
    bool GetField (std::wstring& fieldValue,EcsMapTableFields fieldId) const;
    unsigned long GetFieldAsUL (EcsMapTableFields fieldId) const;
    void GetFileRecordId (std::wstring& fileRecId) const;
    bool ReplaceField (EcsMapTableFields fieldId,const std::wstring& fieldValue);
    bool ReplaceField (EcsMapTableFields fieldId,unsigned long fieldValue);
private:
    //=========================================================================
    // Private Support Functions
    bool MapFieldIdToName (std::wstring& fieldName,EcsMapTableFields fieldId) const;
    //=========================================================================
    // Private Data Members
    unsigned CurrentRecord;
    mutable TcsCsvStatus Status;
};

//newPage//
///////////////////////////////////////////////////////////////////////////////
// TcsNameMapper  --  A collection of TcsNameMap objects.
//
// The collection object is a std::set where the operator< override assures
// that there is no duplication with respect to type, flavor, and name,
// duplicate order, and allias flag.  Each element in the std::set will have a
// genericID assigned.  TcsNameMap objects within the same type that have the
// same genericId are considered to be references to the same "thing".
//
// Thus, a mapping from anything to anything can be obtained by locating the
// genericID of the source item (name or number) and then searching for a
// target item of the desired flavor with the same generidID.  By examining
// aliases when searching for a generic ID, and ignoring them when locating
// an object by generic ID, we enable having multiple names for an item.  By
// allowing duplicate names for a specific item, we allow a specific name to
// be assigned to two separate objects in the same type/flavor name space.
//
// KcsNameMapInitial is the initial value assigned to entries for which a
//                   numeric ID is not assigned or is not known.
// KcsNameMapFinal   is the maximum value which can be assigned to entries
//                   for which a numeric ID is not assigned or known.
// KcsNameMapBias    is the value which is used to identify the flavor of
//                   an numeric ID generated by this object in the case of
//                   an entry for which a numeric ID is not assigned or is
//                   not known.
class TcsNameMapper
{
	typedef std::set<TcsNameMap>::iterator iterator;
	typedef std::set<TcsNameMap>::const_iterator const_iterator;
	static const short KcsNameMapFlvrCnt = 32;
	static const unsigned long KcsNameMapInitial = 90000000UL;
	static const unsigned long KcsNameMapFinal   = 99999999UL;
	static const wchar_t FirstName [32];
	static const wchar_t LastName [32];
	static wchar_t DefaultFlavorNames [KcsNameMapFlvrCnt][32];
public:
	static const wchar_t Delimiters [4];
	static const unsigned long KcsNameMapBias = 100000000UL;
	static EcsNameFlavor FlvrNameToNbr (const wchar_t* flvrName);
	static const wchar_t* FlvrNbrToName (EcsNameFlavor flvrNbr);
	static bool AnalyzeGenericId (EcsNameFlavor& flavor,unsigned long& flvrId,
														const TcsGenericId& genericId);
	///////////////////////////////////////////////////////////////////////////
	// Construction  /  Destruction  /  Assignment
	TcsNameMapper (void);
	TcsNameMapper (const TcsNameMapper& source);
	virtual ~TcsNameMapper (void);
	TcsNameMapper& operator= (const TcsNameMapper& rhs);
	///////////////////////////////////////////////////////////////////////////
	// Operator Overrides
	TcsNameMapper& operator+= (const TcsNameMap& newItem);
	TcsNameMapper& operator-= (const TcsNameMap& newItem);
	std::wostream operator<< (std::wostream& outStrm) const;	// writes CSV
	///////////////////////////////////////////////////////////////////////////
	// Named Member functions
	bool IsInitialized (void) const;
	bool SetRecordDuplicates (bool recordDuplicates);
	void ClearDuplicateList (void);
	size_t GetDuplicateCount (void) const { return Duplicates.size (); }
	EcsCsvStatus ReadFromStream (char* pBuffer, size_t const bufferSize);
	EcsCsvStatus ReadFromStream (std::wistream& inStrm);
	EcsCsvStatus ReadFromStream (std::wistream& inStrm,TcsCsvStatus& status);
	bool Add (TcsNameMap& newItem,bool addDupName = false,const wchar_t* objSrcId = 0);
	bool Add (const TcsNameMap& newItem);		// can't add a duplicate if the item is const
	bool Replace (const TcsNameMap& newItem,const TcsNameMap& oldItem);
	TcsGenericId Locate (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name) const;
	TcsGenericId Locate (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const;
	const wchar_t* LocateName (EcsMapObjType type,EcsNameFlavor flavor,const TcsGenericId& genericId) const;
	unsigned long LocateNumber (EcsMapObjType type,EcsNameFlavor flavor,const TcsGenericId& genericId) const;
	bool GetUserByNbr (void*& result,EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const;
	bool SetUserByNbr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id,void* userValue);
	bool GetUserByName (void*& result,EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name) const;
	bool SetUserByName (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name,void* userValue);
	const wchar_t* FlavorToName (EcsNameFlavor flavor) const;
	unsigned long GetNextDfltId (EcsNameFlavor flavor);
	void WriteLabelLine (std::wostream& outStrm) const;
	void WriteAsCsv (std::wostream& outStrm,bool flvrLbls = false) const;
	void WriteDuplicates (std::wostream& outStrm);
	unsigned long LocateIdByIdx (EcsMapObjType type,EcsNameFlavor flavor,unsigned index,
	                                                                     unsigned* count = 0) const;
	const wchar_t* LocateNameByIdx (EcsMapObjType type,EcsNameFlavor flavor,unsigned index,
	                                                                        unsigned* count = 0) const;
	TcsNameMapList* Enumerate (EcsMapObjType type,EcsNameFlavor flavor,bool deprecated = false);
	///////////////////////////////////////////////////////////////////////////
	// Updating functions.
	bool ExtractSpecificId (TcsNameMap& nameMap,EcsMapObjType mapType,TcsGenericId systemId,
																	  EcsNameFlavor flavor,
																	  unsigned long wkid) const;
	bool ExtractSpecificName (TcsNameMap& nameMap,EcsMapObjType mapType,TcsGenericId systemId,
																		EcsNameFlavor flavor,
																		const wchar_t* name) const;
	bool AliasExistingName (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* oldName,
																	const wchar_t* newName,
																	const wchar_t* comment = 0,
																	const TcsGenericId& deprecatedBy = KcsGenericIdNull);
	bool AliasExistingName (EcsMapObjType type,EcsNameFlavor flavor,unsigned long wkid,
																	const wchar_t* newName,
																	const wchar_t* comment = 0,
																	const TcsGenericId& deprecatedBy = KcsGenericIdNull);
	bool ExtractAndRemove (TcsNameMap& extractedNameMap,EcsMapObjType type,EcsNameFlavor flavor,
																		   const wchar_t* name,
																		   short aliasFlag = 0,
																		   short dupSort = 0);

	///////////////////////////////////////////////////////////////////////////
	// Manufacturing functions.
	bool AddKeyNameMap (EcsMapObjType mapType,const wchar_t* mapFilePath);
	bool AddKeyMapFields (EcsMapObjType mapType,unsigned long genericId,const TcsKeyNameMapFile& mapFileObj);
	EcsNameFlavor KeyMapFlavor (const TcsKeyNameMapFile& mapFileObj) const;
	unsigned long KeyMapGenericId (const TcsKeyNameMapFile& mapFileObj) const;

private:
	///////////////////////////////////////////////////////////////////////////
	// Private Support Member Functions
	void InitializeFlavors (void);
	void AdjustDefaultIDs (void);
	TcsNameMap* LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id);
	const TcsNameMap* LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const;
	TcsNameMap* LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name);
	const TcsNameMap* LocateNameMap (EcsMapObjType type,EcsNameFlavor flavor,const wchar_t* name,short dupSort = 0) const;
	iterator LocateNameMapItr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id);
	const_iterator LocateNameMapItr (EcsMapObjType type,EcsNameFlavor flavor,unsigned long id) const;

	///////////////////////////////////////////////////////////////////////////
	// Private Data Members
	bool RecordDuplicates;
	unsigned long InitialDfltFlvrIds [KcsNameMapFlvrCnt];
	std::set<TcsNameMap> DefinitionSet;
	std::vector<TcsNameMap> Duplicates;
};
#endif
