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





#ifndef __ODSTRING_H__
#define __ODSTRING_H__

#include <stdarg.h>
#include <stdio.h>

#include "OdMutex.h"
#include "OdPlatformSettings.h"
#include "OdCodePage.h"
#include "OdAnsiString.h"

#define odaIsValidString(lpch)    (true)

#include "TD_PackPush.h"

#ifdef sgi
#include <string>
using namespace std;  // va_XXXX items are in std namespace for newest SGI compiler
#endif

/** \details
    This structure contains string data used by OdString.

    \remarks
    An OdChar array (buffer) is allocated along with this structure, and
    is freed when this structure is freed.
    
    Corresponding C++ library: TD_Root
    <group !!RECORDS_TD_APIRef>
*/
struct OdStringData
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

  /** \details
    Underlying OdChar array (buffer) of this StringData object.
  */
  OdChar* unicodeBuffer;
  
  /** \details
    Underlying char array (buffer) of this StringData object.
  */
  char* ansiString;
};

/** \details
    This class implements Teigha character String objects.

    \remarks
    OdString objects use zero-based indices to access string elements.

    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdString
{
public:

  FIRSTDLL_EXPORT_STATIC const static OdString kEmpty;

  OdString()
  { init(); }

  /** \param source [in]  Source.
  */
  OdString(const OdString& source);

  /** \param ch [in]  Character to repeat.
    \param length [in]  Number of characters.
  */
  OdString(OdChar ch, int length);

  OdString(const OdChar* source);
  OdString(const OdChar* source, int length);
#if defined(_MSC_VER) && _MSC_VER >= 1300
  OdString(const __wchar_t* source);
  OdString(const __wchar_t* source, int length);
#endif


  OdString(const char* lpch, OdCodePageId codepage = CP_CNT);
  OdString(const char* lpch, int nLength, OdCodePageId codepage = CP_CNT);
  OdString(const OdAnsiString&);
protected:
  explicit OdString(OdStringData* pData);
public:

  /** \details
    Returns the number of characters in this String object.
  */
  int getLength() const
  { 
    if (isUnicodeNotInSync())
      syncUnicode();
    return getData()->nDataLength; 
  }
  int getLengthA() const;

  /** \details
    Returns true and only if this String object is empty.
  */
  bool isEmpty() const
  {
    return (getData()->nDataLength == 0 && 
      (getData()->ansiString == 0 || getAnsiString()->isEmpty()));
  }

  /** \details
    Sets this String object to the empty string.
  */
  void empty();

  /** \details
    Returns the single character of this String object at the specified position.
    \param charIndex [in]  Character index. 
  */
  OdChar getAt(int charIndex) const
  {
#ifdef TD_UNICODE
    if (isUnicodeNotInSync())
      syncUnicode();
#endif
    ODA_ASSERT(charIndex >= 0);
    ODA_ASSERT(charIndex < getData()->nDataLength);
    return getData()->unicodeBuffer[charIndex];
  }

  /** \details
    Indexing Operator
    
    \returns
    Returns the single character of this String object at the specified position.
    \param charIndex [in]  Character index. 
  */
  OdChar operator[](int charIndex) const
  {
    return getAt(charIndex);
  }

  /** \details
    Sets the single character of this String object at the specified position.
    \param charIndex [in]  Character index.
    \param ch [in]  Character.
    \remarks
    No range checking is performed.
  */
  void setAt(int charIndex, OdChar ch);

#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG) && defined(ODA_LINT)
#define UPDATECONSTSTRINGDIAGNISTIC
private:
  void updateConstStringDiagnostic(const OdChar* pAddr) const;
public:
#else
  #define updateConstStringDiagnostic(pAddr)
#endif

  /** \details
    Returns the underlying OdChar array (buffer) of this String object.
  */
  const OdChar* c_str() const
  { 
    if (isUnicodeNotInSync())
      syncUnicode();
    const OdChar* pRet = getData()->unicodeBuffer;
    updateConstStringDiagnostic(pRet);
    return pRet;
  }

  /** \details
    Returns the underlying character array (buffer) of this String object.
  */
  inline operator const OdChar*() const
  { 
    return c_str();
  }

#if defined(_MSC_VER) && _MSC_VER >= 1300
  operator const __wchar_t*() const
  { 
    return reinterpret_cast<const __wchar_t*>(c_str());
  }
#endif

  operator const char*() const;

  OdString& operator=(const OdString& source);

  OdString& operator=(OdChar ch);
  
  OdString& operator=(const OdChar* source);

#if defined(_MSC_VER) && _MSC_VER >= 1300
  OdString& operator=(const __wchar_t* source){ return operator=((const OdChar*)source); }
#endif

  OdString& operator=(const OdAnsiString& source);

  OdString& operator=(const char* source);

  /** \details
    Concatenation-Equals Operator.
	
	\param string [in] Input value for concatenation
    \remarks
    Sets this object to the concatenation of this String object with the 
    value on the right, and returns a reference to this String object.
  */
  OdString& operator+=(const OdString& string);

  /** \details
    Concatenation-Equals Operator.
	
	\param ch [in] Input value for concatenation
    \remarks
    Sets this object to the concatenation of this String object with the 
    value on the right, and returns a reference to this String object.
  */
  OdString& operator+=(OdChar ch);

  /** \details
    Concatenation-Equals Operator.
	
	\param string [in] Input value for concatenation
    \remarks
    Sets this object to the concatenation of this String object with the 
    value on the right, and returns a reference to this String object.
  */
  OdString& operator+=(const OdChar* string);

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Concatenation-Equals Operator.
	
	\param string [in] Input value for concatenation
    \remarks
    Sets this object to the concatenation of this String object with the 
    value on the right, and returns a reference to this String object.
  */
  OdString& operator+=(const __wchar_t* string){ return operator+=((const OdChar*)string); }
#endif

  /** \details
    Concatenation-Equals Operator.
	
	\param string [in] Input value for concatenation
    \remarks
    Sets this object to the concatenation of this String object with the 
    value on the right, and returns a reference to this String object.
  */
  OdString& operator+=(const char* string);

  /** \details
    Concatenation Operator.
    \returns
    Returns the concatenation of this object on the left with the object on the right.
  */
  friend FIRSTDLL_EXPORT OdString operator+(const OdString& string1, const OdString& string2);
  friend FIRSTDLL_EXPORT OdString operator+(const OdString& string, OdChar ch);
  friend FIRSTDLL_EXPORT OdString operator+(OdChar ch, const OdString& string);

  friend FIRSTDLL_EXPORT OdString operator+(const OdString& string1, const OdChar* string2);
  friend FIRSTDLL_EXPORT OdString operator+(const OdChar* string1, const OdString& string2);

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
  int compare(const OdChar* otherString) const
  { 
    if (isUnicodeNotInSync())
      syncUnicode();
    return odStrCmp((const wchar_t*)getData()->unicodeBuffer, (const wchar_t*)otherString);
  }

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
  inline int compare(const OdString& otherString) const{ return compare(otherString.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
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
  int compare(const __wchar_t* otherString) const { return compare((const OdChar*)otherString);}
#endif

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
  int compare(const char* otherString) const;

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
  int iCompare(const OdChar* otherString) const
  { 
    if (isUnicodeNotInSync())
      syncUnicode();
    return odStrICmp(getData()->unicodeBuffer, otherString); 
  }   
  
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
  inline int iCompare(const OdString& otherString) const{ return iCompare(otherString.c_str()); }
#if defined(_MSC_VER) && _MSC_VER >= 1300
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
  __forceinline int iCompare(const __wchar_t* otherString) const {return iCompare((const OdChar*)otherString);}
#endif   

  /** \details
    Returns the substring specified by a start index and a length from this String object.
    \param startIndex [in]  Starting index.
    \param length [in]  Length of substring. 
    
  */
  OdString mid(int startIndex, int length) const;

  /** \details
    Returns the substring specified by a start index from this String object.
    \param startIndex [in]  Starting index.

  */
  OdString mid(int startIndex) const;

  /** \details
    Returns the leftmost substring of the specified length from this String object.
    \param length [in]  Length of substring. 
  */
  OdString left(int length) const;

  /** \details
    Returns the rightmost substring of the specified length from this String object.
    \param length [in]  Length of substring. 
  */
  OdString right(int length) const;

  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object is not in the specified character set,
    an empty string is returned.
  */
  OdString spanIncluding(const OdChar* charSet) const;
  
  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object is not in the specified character set,
    an empty string is returned.
  */
  inline OdString spanIncluding(const OdString& charSet) const { return spanIncluding(charSet.c_str()); }

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object is not in the specified character set,
    an empty string is returned.
  */
  OdString spanIncluding(const __wchar_t* charSet) const{return spanIncluding((const OdChar*)charSet);}
#endif

  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    not contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object in the specified character set,
    an empty string is returned.
  */
  OdString spanExcluding(const OdChar* charSet) const;

  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    not contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object in the specified character set,
    an empty string is returned.
  */
  inline OdString spanExcluding(const OdString& charSet) const{ return spanExcluding(charSet.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Returns the longest leftmost substring of this String object 
    that consists solely of characters that are
    not contained in the specified character set.

    \param charSet [in]  Character set.
    
    \remarks
    If the first character in this String object in the specified character set,
    an empty string is returned.
  */
  OdString spanExcluding(const __wchar_t* charSet) const{return spanExcluding((const OdChar*)charSet);}
#endif
  
  /** \details
    Converts this String object to upper case.

    \returns
    Returns a reference to this String object.  
  */
  OdString& makeUpper();

  /** \details
    Converts this String object to lower case.

    \returns
    Returns a reference to this String object.  
  */
  OdString& makeLower();

  /** \details
    Reverses this String object.

    \returns
    Returns a reference to this String object.  
  */
  OdString& makeReverse();


  /** \details
    Removes all whitespace from the right side of this String object.
   
    \returns
    Returns a reference to this String object.
    \remarks
    A space (' ') is used if no whitespace characters are specfied.  
  */
  OdString& trimRight();

  /** \details
    Removes all whitespace from the left side of this String object.
   
    \returns
    Returns a reference to this String object.  
    \remarks
    A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdString& trimLeft();

  /** \details
    Removes all whitespace from the right side of this String object.
   
    \param whiteChar [in]  Whitespace character.
   
    \returns
    Returns a reference to this String object.
    \remarks
    A space (' ') is used if no whitespace characters are specfied.  
  */
  OdString& trimRight(OdChar whiteChar);

  /** \details
    Removes all whitespace from the right side of this String object.
   
    \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.
    \remarks
    A space (' ') is used if no whitespace characters are specfied.  
  */
  OdString& trimRight(const OdChar* whiteChars);

  /** \details
    Removes all whitespace from the right side of this String object.
   
    \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.
    \remarks
    A space (' ') is used if no whitespace characters are specfied.  
  */
  inline OdString& trimRight(const OdString& whiteChars){ return trimRight(whiteChars.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Removes all whitespace from the right side of this String object.
   
    \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.
    \remarks
    A space (' ') is used if no whitespace characters are specfied.  
  */
  OdString& trimRight(const __wchar_t* whiteChars){return trimRight((const OdChar*)whiteChars);}
#endif

  /** \details
    Removes all whitespace from the left side of this String object.
   
   \param whiteChar [in]  Whitespace character.
   
    \returns
    Returns a reference to this String object.  
    \remarks
    A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdString& trimLeft(OdChar whiteChar);

  /** \details
    Removes all whitespace from the left side of this String object.
   
   \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.  
    \remarks
    A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdString& trimLeft(const OdChar* whiteChars);
  
  /** \details
    Removes all whitespace from the left side of this String object.
   
   \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.  
    \remarks
    A space (' ') is used if no other whitespace characters are specfied.  
  */
  inline OdString& trimLeft(const OdString& whiteChars){ return trimLeft(whiteChars.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Removes all whitespace from the left side of this String object.
   
   \param whiteChars [in]  Whitespace characters.
   
    \returns
    Returns a reference to this String object.  
    \remarks
    A space (' ') is used if no other whitespace characters are specfied.  
  */
  OdString& trimLeft(const __wchar_t* whiteChars){return trimLeft((const OdChar*)whiteChars);}
#endif
  /** \details
    Replaces all occurrences of the specified character in this String object.

    \param oldChar [in]  Old character.
    \param newChar [in]  New character.
    
    \returns
    Returns the number of characters or strings replaced.
  */
  int replace(OdChar oldChar, OdChar newChar);

  /** \details
    Replaces all occurrences of the specified string in this String object.

    \param oldString [in]  Old string.
    \param newString [in]  New string.
    
    \returns
    Returns the number of characters or strings replaced.
  */
  int replace(const OdChar* oldString, const OdChar* newString);
  
  /** \details
    Replaces all occurrences of the specified string in this String object.

    \param oldString [in]  Old string.
    \param newString [in]  New string.
    
    \returns
    Returns the number of characters or strings replaced.
  */
  inline int replace(const OdString& oldString, const OdString& newString){ return replace(oldString.c_str(), newString.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
  
  /** \details
    Replaces all occurrences of the specified string in this String object.

    \param oldString [in]  Old string.
    \param newString [in]  New string.
    
    \returns
    Returns the number of characters or strings replaced.
  */
  int replace(const __wchar_t* oldString, const __wchar_t* newString){return replace((const OdChar*)oldString, (const OdChar*)newString);}
#endif
  /** \details
    Removes all occurrences of the specified character from this String object.

    \param removeChar [in]  Character to remove.

    \returns
    Returns the number of characters removed.
  */
  int remove(OdChar chRemove);

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
  int insert(int insertIndex, OdChar insertChar);

  /** \details
    \param insertString [in]  String to insert.
  */

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
  int insert(int insertIndex, const OdChar* insertString);
  
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
  int insert(int insertIndex, const OdString& insertString){return insert(insertIndex, insertString.c_str());}

#if defined(_MSC_VER) && _MSC_VER >= 1300
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
  int insert(int insertIndex, const __wchar_t* insertString){return insert(insertIndex,(const OdChar*)insertString);}
#endif
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
  int find(OdChar searchChar) const;

  /** \details
    Returns the last occurrence of the specified character in this String object.

    \param searchChar [in]  Search character.
     
    \remarks
    Returns -1 if not found.
  */
  int reverseFind(OdChar searchChar) const;

  /** \details
    Returns the last occurrence of the specified character in this String object.

    \param searchChar [in]  Search character.
    \param startIndex [in]  An index from which to start searching back.

    \remarks
    Returns -1 if not found.
  */
  int reverseFind(OdChar searchChar, int startIndex) const;

  /** 
  */
  /** \details
    Returns the first occurrence of the specified character in this String object.

    \param searchChar [in]  Search character.
	\param startIndex [in]  Start index of search.
	
    \remarks
    Returns -1 if not found.
  */
  int find(OdChar searchChar, int startIndex) const;

  /** \details
    Returns the first occurrence in this String object of any member of the specified character set. 

    \param charSet [in]  Character set.
     
    \remarks
    Returns -1 if not found.
  */
  int findOneOf(const OdChar* charSet) const;

  /** \details
    Returns the first occurrence of the specified string in this String object.

    \param searchString [in]  Search string.
     
    \remarks
    Returns -1 if not found.
  */
  int find(const OdChar* searchString) const;

  /** \details
    Returns the first occurrence of the specified string in this String object.

    \param searchString [in]  Search string.
	\param startIndex [in]  Start index of search.
     
    \remarks
    Returns -1 if not found.
  */
  int find(const OdChar* searchString, int startIndex) const;

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Returns the first occurrence in this String object of any member of the specified character set. 

    \param charSet [in]  Character set.
     
    \remarks
    Returns -1 if not found.
  */
  int findOneOf(const __wchar_t* charSet) const{return findOneOf((const OdChar*)charSet);}
  
  /** \details
    Returns the first occurrence in this String object of any member of the specified character set. 

    \param charSet [in]  Character set.
     
    \remarks
    Returns -1 if not found.
  */
  int findOneOf(const OdString& charSet) const{return findOneOf((const OdChar*)charSet);}

  int find(const __wchar_t* searchString) const{return find((const OdChar*)searchString);}

  int find(const OdString& searchString) const{return find((const OdChar*)searchString);}

  int find(const __wchar_t* searchString, int startIndex) const{return find((const OdChar*)searchString,startIndex);}

  int find(const OdString& searchString, int startIndex) const{return find((const OdChar*)searchString,startIndex);}
#endif
  /** \details
    Assigns a value to this String object using a printf-style format string and 
    arguments.
    \param formatString [in]  Format string.
    \returns
    Returns a reference to this String Object.
  */
  OdString& format(const OdChar* formatString, ...);

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Assigns a value to this String object using a printf-style format string and 
    arguments.
    \param formatString [in]  Format string.
    \returns
    Returns a reference to this String Object.
  */
  OdString& format(const __wchar_t* formatString, ...);
#endif

  /** \details
    Assigns a value to this String object using a vsprintf-style format string and 
    argument list.
    \param formatString [in]  Format string.
    \param argList [in]  Argument list.   
    \returns
    Returns a reference to this String Object.
  */
  OdString& formatV(const OdChar* formatString, va_list argList); 

#if defined(_MSC_VER) && _MSC_VER >= 1300
  /** \details
    Assigns a value to this String object using a vsprintf-style format string and 
    argument list.
    \param formatString [in]  Format string.
    \param argList [in]  Argument list.   
    \returns
    Returns a reference to this String Object.
  */
  inline OdString& formatV(const __wchar_t* formatString, va_list argList)
  {
    return formatV((const OdChar*)formatString, argList);
  }
#endif
                                                                
  /** \details
    Returns a modifiable C style OdChar array (buffer) of the specified minimum length for this String object.
    \param minBufferLength [in]  Minimum buffer length.
    
    \remarks
    releaseBuffer() should be called before using any other OdString methods.
    
    The pointer returned by this function is not valid after releaseBuffer() is called.
  */
  OdChar* getBuffer(int minBufLength);

  /** \details
    Releases a buffer obtained by getBuffer() or getBufferSetLength(), and sets its length.
    \param newLength [in]  New buffer length.
    \remarks
    If newLength == -1, the buffer is treated as 0 terminated.  
  */
  void releaseBuffer(int newLength = -1);

  /** \details
    Returns a modifiable C style OdChar array (buffer) of of the specified length for this String object.
    \param length [in]  buffer length.
    
    \remarks
    releaseBuffer() should be called before using any other OdString methods.
    
    The pointer returned by this function is not valid after releaseBuffer() is called.
  */
  OdChar* getBufferSetLength(int length);

  /** \details
    Releases any unused memory allocated by this String object.
  */
  void freeExtra();

  /** \details
    Enables reference counting for this String object.
  */
  OdChar* lockBuffer();

  /** \details
    Disables reference counting for this String object.
  */
  void unlockBuffer();

/*   disable wide char functionalities
   Constructor.  This OdString will receive a copy of the wide character string referenced
   by lpsz.
  OdString(const OdCharW* lpsz);

  Constructor. This OdString will receive a copy of the first nLength wide characters of
  the string referenced by lpch.
  OdString(const OdCharW* lpch, int nLength);

   Assignment operator.  This OdString is assigned the value of lpsz.
  const OdString& operator=(const OdCharW* lpsz);

*/

#ifdef NOT_IMPLEMENTED

  /*!DOM*/
  /** \details
    Case sensitive string comparison.  
    
    \param otherString [in]  Other string.
    
    \remarks
    collate() returns one of the following:
    
    <table>
    Returns   Condition
    < 0       This String object < otherString. 
    0         This String object == otherString.
    > 0       This String object > otherString.
    </table>
  */
  int collate(const OdChar* otherString) const;

  /*!DOM*/
  /** \details
    Case insensitive string comparison.  
    
    \param otherString [in]  Other string.
    
    \remarks
    icollate() returns one of the following:
    
    <table>
    Returns   Condition
    < 0       This String object < otherString. 
    0         This String object == otherString.
    > 0       This String object > otherString.
    </table>
  */
  int iCollate(const OdChar* otherString) const;

  /** \details
    Assigns a value to this String object using a printf-style format string and 
    arguments.
    \param formatID [in]  Format ID. 
    \returns
    Returns a reference to this String Object.
  */
  OdString& ODA_CDECL format(unsigned int formatID, ...);
#endif


public:
  ~OdString();

  /** \details
    Returns the number of characters allocated for the underlying OdChar array (buffer) in this String object.
  */
  int getAllocLength() const
  { return getData()->nAllocLength; }

protected:
  FIRSTDLL_EXPORT_STATIC static OdStringData kEmptyData;

  /** \details
    Pointer to the underlying OdChar array (buffer). 
  */
  OdStringData* m_pData;  
  
  /** \details
    Returns the StringData for this String object.
  */
  OdStringData* getData() const
    { ODA_ASSERT(m_pData!= NULL); return m_pData; }

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
  void allocCopy(OdString& destString, int copyLength, int copyIndex, int extraLength) const;

  /** \details
    Allocates the specified number of characters for the character buffer of this String object.
    \param length [in]  buffer length.
    allocAlways - if true, allocates new OdStringData even for 0 length unicode buffer (for ansi string constructors)
  */
  void allocBuffer(int length, bool allocAlways = false);

  /** \details
    Copies the specified string to this String object.
    
    \param sourceLength [in]  Number of characters to copy.
    \param source [in]  Source string.

    \remarks
    If getAllocLength() < sourceLength, this function calls allocBuffer(sourceLength).
  */
    void assignCopy(int sourceLength, const OdChar* source);

  /** \details
    Concatenates and copies the specified strings to this String object.
    
    \param sourceLength1 [in]  Number of characters in first string to copy.
    \param sourceLength2 [in]  Number of characters in second string to copy.
    \param source1 [in]  First source string.
    \param source2 [in]  Second source string.

    \remarks
    If getAllocLength() < (sourceLength1 + sourceLength2), this function calls allocBuffer(sourceLength1 + sourceLength2).
  */
    void concatCopy(int sourceLength1, const OdChar* source1, int sourceLength2, const OdChar* source2);

  /** \details
    Concatenates the string to this String object.
    
    \param sourceLength [in]  Number of characters to copy.
    \param source [in]  Source string.

    \remarks
    If getAllocLength() < getLength() + sourceLength, this function calls allocBuffer(getLength()+ sourceLength2).
  */
    void concatInPlace(int sourceLength, const OdChar* source);
 
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

  static void release(OdStringData* pStringData);

  /** \details
    Returns the length of the specified string.
    \param string [in]  0 terminated string or NULL.
    \remarks
    Returns 0 if string == NULL.
  */
  static int safeStrlen(const OdChar* string)
  { return (string == NULL) ? (int)0 : (int)odStrLen((const wchar_t*)string); }

  /** \details
    Frees the specified StringData object.
    \param pStringData [in]  Pointer to the StringData object.
  */
  static void freeData(OdStringData* pStringData);

  bool isUnicodeNotInSync() const
  {
    return !getData()->unicodeBuffer && getData()->ansiString;
  }
  void syncUnicode() const;
  OdAnsiString* getAnsiString() const
  {
    if (getData()->ansiString)
      return reinterpret_cast<OdAnsiString*>(&getData()->ansiString);
    else 
      return 0;
  }
  void freeAnsiString()const;
  friend class OdAnsiString;
};

typedef OdChar OdCharW;
typedef OdString OdString;


#if defined(_MSC_VER) && _MSC_VER >= 1300
inline OdString operator+(const OdString& string1, const __wchar_t* string2)
{
  return operator+(string1, (const OdChar*)string2);
}
inline OdString operator+(const __wchar_t* string1, const OdString& string2)
{
  return operator+((const OdChar*)string1, string2);
}
#endif

inline bool operator==(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) == 0; }

inline bool operator==(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) == 0; }

inline bool operator==(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) == 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator==(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) == 0; }

inline bool operator==(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) == 0; }
#endif

inline bool operator==(const OdString& s1, const char* s2)
{ return s1.compare(s2) == 0; }

inline bool operator==(const char* s1, const OdString& s2)
{ return odStrCmpA((const char*)s2, s1) == 0; }

inline bool operator!=(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) != 0; }

inline bool operator!=(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) != 0; }

inline bool operator!=(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) != 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator!=(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) != 0; }

inline bool operator!=(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) != 0; }
#endif

inline bool operator!=(const OdString& s1, const char* s2)
{ return s1.compare(s2) != 0; }

inline bool operator!=(const char* s1, const OdString& s2)
{ return s2.compare(s1) != 0; }

inline bool operator<(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) < 0; }

inline bool operator<(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) < 0; }

inline bool operator<(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) > 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator<(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) < 0; }

inline bool operator<(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) > 0; }
#endif

inline bool operator<(const OdString& s1, const char* s2)
{ return s1.compare(s2) < 0; }

inline bool operator<(const char* s1, const OdString& s2)
{ return s2.compare(s1) > 0; }

inline bool operator>(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) > 0; }

inline bool operator>(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) > 0; }

inline bool operator>(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) < 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator>(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) > 0; }
inline bool operator>(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) < 0; }
#endif

inline bool operator>(const OdString& s1, const char* s2)
{ return s1.compare(s2) > 0; }

inline bool operator>(const char* s1, const OdString& s2)
{ return s2.compare(s1) < 0; }

inline bool operator<=(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) <= 0; }

inline bool operator<=(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) <= 0; }

inline bool operator<=(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) >= 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator<=(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) <= 0; }
inline bool operator<=(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) >= 0; }
#endif

inline bool operator<=(const OdString& s1, const char* s2)
{ return s1.compare(s2) <= 0; }

inline bool operator<=(const char* s1, const OdString& s2)
{ return s2.compare(s1) >= 0; }

inline bool operator>=(const OdString& s1, const OdString& s2)
  { return s1.compare(s2) >= 0; }

inline bool operator>=(const OdString& s1, const OdChar* s2)
  { return s1.compare(s2) >= 0; }

inline bool operator>=(const OdChar* s1, const OdString& s2)
  { return s2.compare(s1) <= 0; }

#if defined(_MSC_VER) && _MSC_VER >= 1300
inline bool operator>=(const OdString& s1, const __wchar_t* s2)
{ return s1.compare((const OdChar*)s2) >= 0; }

inline bool operator>=(const __wchar_t* s1, const OdString& s2)
{ return s2.compare((const OdChar*)s1) <= 0; }
#endif

inline bool operator>=(const OdString& s1, const char* s2)
{ return s1.compare(s2) >= 0; }

inline bool operator>=(const char* s1, const OdString& s2)
{ return s2.compare(s1) <= 0; }

#if defined(ODA_UNIXOS)
/** <group Other_Classes>
*/
class OdW2US
{
  OdUInt16* _buf;
public:
  OdW2US(const wchar_t* pStr)
  {
    size_t len = Od_wcslen(pStr);
    _buf = new OdUInt16[len+1];
    for (size_t i = 0; i < len; i++)
    {
      _buf[i] = (OdUInt16)pStr[i];
    }
    _buf[len] = 0;
  }
  ~OdW2US() 
  {
    delete _buf;
  }
  operator const OdUInt16*() const { return _buf; }
};

/** \details
    <group Other_Classes>
*/
class OdUS2W
{
  wchar_t* _buf;
public:
  OdUS2W(const OdUInt16* pStr)
  {
    size_t len = 0;
    if (const OdUInt16* p = pStr)
    {
      while (*p++)
        len++;
    }
    _buf = new wchar_t[len+1];
    for (size_t i = 0; i < len; i++)
    {
      _buf[i] = (wchar_t)pStr[i];
    }
    _buf[len] = 0;
  }
  ~OdUS2W() 
  {
    delete _buf;
  }
  operator const wchar_t*() const { return _buf; }
};

#else

#define OdUS2W(a) (const wchar_t*)(a)
#define OdW2US(a) (const OdUInt16*)(a)

#endif

//////////////////////////////////////////////////////////////////////////

/** \details
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdConstString : public OdString
{
public:
  OdConstString(const OdChar* pSource = NULL);
  // Destructor of base class OdString is not virtual (critical for performance).
  // Therefore ~OdConstString is missing

  OdConstString& operator=(const OdConstString& pSource);
  OdString& operator=(const OdString& pSource);
  OdString& operator=(OdChar ch);
  OdString& operator=(const OdChar* pSource);
  OdString& operator=(const OdAnsiString& pSource);
  OdString& operator=(const char* pSource);
  operator const OdChar*() const;

protected:
  OdStringData m_data;  
};

//////////////////////////////////////////////////////////////////////////

#include "TD_PackPop.h"

#endif // __ODSTRING_H__


