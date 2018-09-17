/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////





#ifndef __ODANSISTRING_H__
#define __ODANSISTRING_H__

#ifdef TD_UNICODE
#define ODA_CDECL 
#define ODA_PASCAL 

#include <stdarg.h>
#include <stdio.h>

#include "OdMutex.h"
#include "OdPlatform.h"
#include "OdArrayPreDef.h"
#include "OdCodePage.h"

#define odaIsValidString(lpch)    (true)

#include "TD_PackPush.h"
class OdString;

#ifdef sgi
#include <string>
 // va_XXXX items are in std namespace for newest SGI compiler
 
using namespace std; 
#endif

/** \details
This structure contains string data used by OdAnsiString.

\remarks
An char array (buffer) is allocated along with this structure, and
is freed when this structure is freed.

Corresponding C++ library: TD_Root
<group !!RECORDS_tkernel_apiref>
*/
struct OdStringDataA
{            
  /** \details
  Number of references to this string.
  */
  OdRefCounter nRefs;      

  /** \details
  String length in characters, omitting the 0 terminator.
  */
  int nDataLength; 

  /** \details
  String allocation length in characters, omitting the 0 terminator. 
  */
  int nAllocLength;       
  
  OdCodePageId codepage;

  /** \details
  Returns the underlying char array (buffer) of this StringData object.
  */
  char* data()           
  { return (char*)(this+1); }
};

#ifdef _WIN32_WCE
ODA_ASSUME((sizeof(OdStringDataA)%8)==0);
#endif

/** \details
This structure represents empty strings used by OdAnsiString.

<group !!RECORDS_tkernel_apiref>
*/
      struct OdEmptyStringDataA
{
  OdStringDataA  kStrData;
  char        kChar;
};

/** \details
This class implements Teigha single-byte character string objects.

\remarks
OdAnsiString objects use zero-based indices to access string elements.

<group Other_Classes>
*/
class FIRSTDLL_EXPORT OdAnsiString
{
public:

  OdAnsiString()
  { init(); }

  /** \param source [in]  Source.
  */
  OdAnsiString(const OdAnsiString& source);

  /** \param ch [in]  Character to repeat.
  \param length [in]  Number of characters.
  */
  OdAnsiString(char ch, int length);

  OdAnsiString(const char* source);

  OdAnsiString(const char* source, int length);
  OdAnsiString(const char* source, OdCodePageId);
  OdAnsiString(const char* source, int length, OdCodePageId);
  OdAnsiString(const OdString& , OdCodePageId);

  void setCodepage(OdCodePageId id);
  
  OdCodePageId codepage() const
  { return getData()->codepage; }

  /** \details
  Returns the number of characters in this String object.
  */
  int getLength() const
  { return getData()->nDataLength; }

  /** \details
  Returns true and only if this String object is empty.
  */
  bool isEmpty() const
  { return getData()->nDataLength == 0; }

  /** \details
  Sets this String object to the empty string.
  */
  void empty();

  /** \details
  Returns the single character of this String object at the specified position.
  \param charIndex [in]  Character index. 
  */
  char getAt(int charIndex) const
  {
    ODA_ASSERT(charIndex >= 0);
    ODA_ASSERT(charIndex < getData()->nDataLength);
    return m_pchData[charIndex];
  }

  /** \details
  Indexing Operator

  \returns
  Returns the single character of this String object at the specified position.
  \param charIndex [in]  Character index. 
  */
  char operator[](int charIndex) const
  {
    ODA_ASSERT(charIndex >= 0);
    ODA_ASSERT(charIndex < getData()->nDataLength);
    return m_pchData[charIndex];
  }

  /** \details
  Sets the single character of this String object at the specified position.
  \param charIndex [in]  Character index.
  \param ch [in]  Character.
  \remarks
  No range checking is performed.
  */
  void setAt(int charIndex, char ch);

  /** \details
  Returns the underlying char array (buffer) of this String object.
  */
  const char* c_str() const
  { return m_pchData; }

  /** \details
  Returns the underlying char array (buffer) of this String object.
  */
  operator const char*() const
  { return m_pchData; }

  OdAnsiString& operator=(const OdAnsiString& source);

  OdAnsiString& operator=(char ch);

  OdAnsiString& operator=(const char* source);

  OdAnsiString& operator=(const OdString& source);

  /** \details
  Concatenation-Equals Operator.
  \remarks
  Sets this object to the concatenation of this String object with the 
  value on the right, and returns a reference to this String object.
  */
  OdAnsiString& operator+=(const OdAnsiString& string);

  /** \details
  Concatenation-Equals Operator.
  \remarks
  Sets this object to the concatenation of this String object with the 
  value on the right, and returns a reference to this String object.
  */
  OdAnsiString& operator+=(char ch);

  /** \details
  Concatenation-Equals Operator.
  \remarks
  Sets this object to the concatenation of this String object with the 
  value on the right, and returns a reference to this String object.
  */
  OdAnsiString& operator+=(const char* string);

  /** \details
  Concatenation Operator.
  \returns
  Returns the concatenation of this object on the left with the object on the right.
  */
  friend FIRSTDLL_EXPORT OdAnsiString operator+(const OdAnsiString& string1, const OdAnsiString& string2);
  friend FIRSTDLL_EXPORT OdAnsiString operator+(const OdAnsiString& string, char ch);
  friend FIRSTDLL_EXPORT OdAnsiString operator+(char ch, const OdAnsiString& string);

  friend FIRSTDLL_EXPORT OdAnsiString operator+(const OdAnsiString& string1, const char* string2);
  friend FIRSTDLL_EXPORT OdAnsiString operator+(const char* string1, const OdAnsiString& string2);

  /** \details
  Case sensitive string comparison.  

  \param otherString [in]  Other string.

  \remarks
  compare() returns one of the following:

  <table>
  Returns   Condition
  < 0       This String object < otherString. 
  0         This String object == otherString.
  > 0       This String object > otherString.
  </table>
  */
  int compare(const char* otherString) const
  { ODA_ASSERT(odaIsValidString(otherString)); return odStrCmpA(m_pchData, otherString); }    // MBCS/Unicode aware 

  /** \details
  Case insensitive string comparison.  

  \param otherString [in]  Other string.

  \remarks
  iCompare() returns one of the following:

  <table>
  Returns   Condition
  < 0       This String object < otherString. 
  0         This String object == otherString.
  > 0       This String object > otherString.
  </table>
  */
  int iCompare(const char* otherString) const
  { ODA_ASSERT(odaIsValidString(otherString)); return odStrICmpA(m_pchData, otherString); }   

  /** \details
  Returns the substring specified by a start index and a length from this String object.
  \param startIndex [in]  Starting index.
  \param length [in]  Length of substring. 

  \remarks
  If length is not specified, the remainder of this String object is returned.
  */
  OdAnsiString mid(int startIndex, int length) const;

  /** \details
  Returns the substring specified by a start indexfrom this String object.
  \param startIndex [in]  Starting index.

  */
  OdAnsiString mid(int startIndex) const;

  /** \details
  Returns the leftmost substring of the specified length from this String object.
  \param length [in]  Length of substring. 
  */
  OdAnsiString left(int length) const;

  /** \details
  Returns the rightmost substring of the specified length from this String object.
  \param length [in]  Length of substring. 
  */
  OdAnsiString right(int length) const;

  /** \details
  Returns the longest leftmost substring of this String object 
  that consists solely of characters that are
  contained in the specified character set.

  \param charSet [in]  Character set.

  \remarks
  If the first character in this String object is not in the specified character set,
  an empty string is returned.
  */
  OdAnsiString spanIncluding(const char* charSet) const;

  /** \details
  Returns the longest leftmost substring of this String object 
  that consists solely of characters that are
  not contained in the specified character set.

  \param charSet [in]  Character set.

  \remarks
  If the first character in this String object in the specified character set,
  an empty string is returned.
  */
  OdAnsiString spanExcluding(const char* charSet) const;

  /** \details
  Converts this String object to upper case.

  \returns
  Returns a reference to this String object.  
  */
  OdAnsiString& makeUpper();

  /** \details
  Converts this String object to lower case.

  \returns
  Returns a reference to this String object.  
  */
  OdAnsiString& makeLower();

  /** \details
  Reverses this String object.

  \returns
  Returns a reference to this String object.  
  */
  OdAnsiString& makeReverse();


  /** \details
  Removes all whitespace from the right side of this String object.

  \returns
  Returns a reference to this String object.
  \remarks
  A space (' ') is used if no whitespace characters are specfied.  
  */
  OdAnsiString& trimRight();

  /** \details
  Removes all whitespace from the left side of this String object.

  \returns
  Returns a reference to this String object.  
  \remarks
  A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdAnsiString& trimLeft();

  /** \param whiteChar [in]  Whitespace character.
  */
  /** \details
  Removes all whitespace from the right side of this String object.

  \param whiteChar [in]  Whitespace character.
  
  \returns
  Returns a reference to this String object.
  \remarks
  A space (' ') is used if no whitespace characters are specfied.  
  */
  OdAnsiString& trimRight(char whiteChar);

  /** \details
  Removes all whitespace from the right side of this String object.

  \param whiteChars [in]  Whitespace characters.
  
  \returns
  Returns a reference to this String object.
  \remarks
  A space (' ') is used if no whitespace characters are specfied.  
  */
  OdAnsiString& trimRight(const char* whiteChars);

  /** \details
  Removes all whitespace from the left side of this String object.

  \param whiteChar [in]  Whitespace character.
  
  \returns
  Returns a reference to this String object.  
  \remarks
  A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdAnsiString& trimLeft(char whiteChar);

  /** \details
  Removes all whitespace from the left side of this String object.

  \param whiteChars [in]  Whitespace characters.
  
  \returns
  Returns a reference to this String object.  
  \remarks
  A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdAnsiString& trimLeft(const char* whiteChars);

  /** \details
  Replaces all occurrences of the specified character in this String object.

  \param oldChar [in]  Old character.
  \param newChar [in]  New character.

  \returns
  Returns the number of characters or strings replaced.
  */
  int replace(char oldChar, char newChar);

  /** \details
  Replaces all occurrences of the specified string in this String object.

  \param oldString [in]  Old string.
  \param newString [in]  New string.

  \returns
  Returns the number of characters or strings replaced.
  */
  int replace(const char* oldString, const char* newString);

  /** \details
  Removes all occurrences of the specified character from this String object.

  \param removeChar [in]  Character to remove.

  \returns
  Returns the number of characters removed.
  */
  int remove(char chRemove);

  /** \details
  Inserts the specified character into this String object.
  
  \param insertIndex [in]  Insertion index.
  \param insertChar [in]  Character to insert.

  \returns
  Returns the new length of this String object.
  \remarks
  If insertIndex exceeds the length of this String object, 
  this String object is lengthened as necessary to accommodate the insertion.
  */
  int insert(int insertIndex, char insertChar);

  /** \details
  Inserts the specified string into this String object.
  
  \param insertIndex [in]  Insertion index.
  \param insertString [in]  String to insert.

  \returns
  Returns the new length of this String object.
  \remarks
  If insertIndex exceeds the length of this String object, 
  this String object is lengthened as necessary to accommodate the insertion.
  */
  int insert(int insertIndex, const char* insertString);

  /** \details
  Deletes the specified number characters from this String object.

  \param deleteIndex [in]  Deletion index.
  \param count [in]  Number of characters to delete.
  \returns
  Returns the new length of this String object.
  */
  int deleteChars(int deleteIndex, int count = 1);


  /** \details
  Returns the first occurrence of the specified character in this String object.

  \param searchChar [in]  Search character.

  \remarks
  Returns -1 if not found.
  */
  int find(char searchChar) const;

  /** \details
  Returns the last occurrence of the specified character in this String object.

  \param searchChar [in]  Search character.

  \remarks
  Returns -1 if not found.
  */
  int reverseFind(char searchChar) const;

  /** 
  */
  /** \details
  Returns the first occurrence of the specified character in this String object.

  \param searchChar [in]  Search character.
  \param startIndex [in]  Start index.
  
  \remarks
  Returns -1 if not found.
  */
  int find(char searchChar, int startIndex) const;

  /** \details
  Returns the first occurrence in this String object of any member of the specified character set. 

  \param charSet [in]  Character set.

  \remarks
  Returns -1 if not found.
  */
  int findOneOf(const char* charSet) const;

  /** \details
  Returns the first occurrence of the specified string in this String object.

  \param searchString [in]  Search string.

  \remarks
  Returns -1 if not found.
  */
  int find(const char* searchString) const;

  /** \details
  Returns the first occurrence of the specified character or string in this String object.

  \param searchString [in]  Search string.
  \param startIndex [in]  Start index.

  \remarks
  Returns -1 if not found.
  */
  int find(const char* searchString, int startIndex) const;

  /** \details
  Assigns a value to this String object using a printf-style format string and 
  arguments.
  \param formatString [in]  Format string.
  \returns
  Returns a reference to this String Object.
  */
  OdAnsiString& format(const char* formatString, ...);

  /** \details
  Assigns a value to this String object using a vsprintf-style format string and 
  argument list.
  \param formatString [in]  Format string.
  \param argList [in]  Argument list.   
  \returns
  Returns a reference to this String Object.
  */
  OdAnsiString& formatV(const char* formatString, va_list argList); 

  /** \details
  Returns a modifiable C style char array (buffer) of the specified minimum length for this String object.
  \param minBufferLength [in]  Minimum buffer length.

  \remarks
  releaseBuffer() should be called before using any other OdAnsiString methods.

  The pointer returned by this function is not valid after releaseBuffer() is called.
  */
  char* getBuffer(int minBufLength);

  /** \details
  Releases a buffer obtained by getBuffer() or getBufferSetLength(), and sets its length.
  \param newLength [in]  New buffer length.
  \remarks
  If newLength == -1, the buffer is treated as 0 terminated.  
  */
  void releaseBuffer(int newLength = -1);

  /** \details
  Returns a modifiable C style char array (buffer) of of the specified length for this String object.
  \param length [in]  buffer length.

  \remarks
  releaseBuffer() should be called before using any other OdAnsiString methods.

  The pointer returned by this function is not valid after releaseBuffer() is called.
  */
  char* getBufferSetLength(int length);

  /** \details
  Releases any unused memory allocated by this String object.
  */
  void freeExtra();

  /** \details
  Enables reference counting for this String object.
  */
  char* lockBuffer();

  /** \details
  Disables reference counting for this String object.
  */
  void unlockBuffer();

public:
  ~OdAnsiString();

  /** \details
  Returns the number of characters allocated for the underlying char array (buffer) in this String object.
  */
  int getAllocLength() const
  { return getData()->nAllocLength; }

protected:
  FIRSTDLL_EXPORT_STATIC static OdEmptyStringDataA kEmptyData;

  /** \details
  Pointer to the underlying char array (buffer). 
  */
  char* m_pchData;  

  /** \details
  Returns the StringData for this String object.
  */
  OdStringDataA* getData() const
  { ODA_ASSERT(m_pchData != NULL); return ((OdStringDataA*)m_pchData)-1; }

  /** \details
  Initializes this String object to an empty string.
  */
  void init();

  /** \details
  Copies the specified number of characters from this String object to the destination String object.
  \param destString [out]  Destination string.
  \param copyLength [in]  Number of characters to copy.
  \param copyIndex [in]  First byte to copy.
  \param extraLength [in]  Extra characters to allocate.

  \remarks
  This function calls allocBuffer(copyLength + extraLength)
  */
  void allocCopy(OdAnsiString& destString, int copyLength, int copyIndex, int extraLength) const;

  /** \details
  Allocates the specified number of characters for the character buffer of this String object.
  \param length [in]  buffer length.
  */
  void allocBuffer(int length);

  /** \details
  Copies the specified string to this String object.

  \param sourceLength [in]  Number of characters to copy.
  \param source [in]  Source string.

  \remarks
  If getAllocLength() < sourceLength, this function calls allocBuffer(sourceLength).
  */
    void assignCopy(int sourceLength, const char* source);

  /** \details
  Concatenates and copies the specified strings to this String object.

  \param sourceLength1 [in]  Number of characters in first string to copy.
  \param sourceLength2 [in]  Number of characters in second string to copy.
  \param source1 [in]  First source string.
  \param source2 [in]  Second source string.

  \remarks
  If getAllocLength() < (sourceLength1 + sourceLength2), this function calls allocBuffer(sourceLength1 + sourceLength2).
  */
    void concatCopy(int sourceLength1, const char* source1, int sourceLength2, const char* source2);

  /** \details
  Concatenates the string to this String object.

  \param sourceLength [in]  Number of characters to copy.
  \param source [in]  Source string.

  \remarks
  If getAllocLength() < getLength() + sourceLength, this function calls allocBuffer(getLength()+ sourceLength2).
  */
    void concatInPlace(int sourceLength, const char* source);

  /** \details
  Assures there are no multiple references to the StringData associated with this String object. 

  \remarks
  If there is more than one reference to the StringData object, it is released, and
  replaced with a copy thereof 
  */
  void copyBeforeWrite();

  /** \details
  Assures the character buffer of this String object is at least the specified size.
  \param newLength [in]  New length of buffer.

  \remarks
  If getAllocLength() < newLength, this function calls allocBuffer(newLength).
  */
  void allocBeforeWrite(int newLength);

  /** \details
  Releases the reference to the specified StringData object.

  \param pStringData [in]  Pointer to the StringData object.

  \remarks
  If pStringData is not specified, the StringData associated with
  this String object is released.

  Decrements the reference count of the StringData object.

  When its reference count reaches zero, the StringData object is freed.
  */
  void release();

  static void ODA_PASCAL release(OdStringDataA* pStringData);

  /** \details
  Returns the length of the specified string.
  \param string [in]  0 terminated string or NULL.
  \remarks
  Returns 0 if string == NULL.
  */
  static int ODA_PASCAL safeStrlen(const char* string)
  { return (string == NULL) ? (int)0 : (int)odStrLenA(string); }

  /** \details
  Frees the specified StringData object.
  \param pStringData [in]  Pointer to the StringData object.
  */
  static void freeData(OdStringDataA* pStringData);
  friend class OdString;
};

inline bool operator==(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) == 0; }

inline bool operator==(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) == 0; }

inline bool operator==(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) == 0; }

inline bool operator!=(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) != 0; }

inline bool operator!=(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) != 0; }

inline bool operator!=(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) != 0; }

inline bool operator<(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) < 0; }

inline bool operator<(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) < 0; }

inline bool operator<(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) > 0; }

inline bool operator>(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) > 0; }

inline bool operator>(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) > 0; }

inline bool operator>(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) < 0; }

inline bool operator<=(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) <= 0; }

inline bool operator<=(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) <= 0; }

inline bool operator<=(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) >= 0; }

inline bool operator>=(const OdAnsiString& s1, const OdAnsiString& s2)
{ return s1.compare(s2) >= 0; }

inline bool operator>=(const OdAnsiString& s1, const char* s2)
{ return s1.compare(s2) >= 0; }

inline bool operator>=(const char* s1, const OdAnsiString& s2)
{ return s2.compare(s1) <= 0; }

typedef OdArray<OdAnsiString> OdAnsiStringArray;

#include "TD_PackPop.h"

#endif // TD_UNICODE

#endif // __ODANSISTRING_H__


