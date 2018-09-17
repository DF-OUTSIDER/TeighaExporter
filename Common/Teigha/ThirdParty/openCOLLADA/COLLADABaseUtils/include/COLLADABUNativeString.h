/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADABaseUtils.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADABU_NATIVESTRING_H__
#define __COLLADABU_NATIVESTRING_H__

#include "COLLADABUPrerequisites.h"


namespace COLLADABU
{

	/** A string class to store strings encoded in the systems native encoding.*/

	class _COLLADABU_API NativeString
	{
    //ODA changes start
    String imp;
    //ODA changes end
	public:
		enum Encoding
		{
			ENCODING_NATIVE,
			ENCODING_UTF8
		};
	public:
		/**Content is initialized to a copy of the string object str.
		If @a encoding is ENCODING_NATIVE, @a str is assumed to be encoded in the platform native encoding.
		If @a encoding is ENCODING_UTF8, @a str is assumed to be encoded in UTF8.*/
		explicit NativeString(const String& str, Encoding encoding = ENCODING_NATIVE);

		NativeString(const WideString& wstr)
		{
			fromWideString(wstr);
		}


		/** Content is initialized to an empty NativeString. */
		explicit NativeString ( );

//ODA changes start
    NativeString (const NativeString& str) : imp (str.imp) {}
    size_t length() const { return imp.length(); }
    NativeString& assign (const String& str) {imp.assign(str); return *this;}
    const char* c_str() const  {return imp.c_str();}

    NativeString& operator= (const NativeString& str)
                        { imp = str.imp; return *this; }
    NativeString& operator += (const NativeString& str) { imp += str.imp; return *this;}
    NativeString& operator += (const String& str) { imp += str; return *this;}
    NativeString& operator += (const char* str) { imp += str; return *this;}
    friend _COLLADABU_API NativeString operator+(const NativeString& str1, const NativeString& str2);

    friend _COLLADABU_API NativeString operator+(const NativeString& string1, String string2);
    friend _COLLADABU_API NativeString operator+(String string1, const NativeString& string2);
    friend _COLLADABU_API String operator+(const NativeString& string1, const char* string2);
    friend _COLLADABU_API String operator+(const char* string1, const NativeString& string2);


    NativeString& append (const String& str)
                        { imp.append (str); return *this; }
    size_t find (const NativeString& str, size_t pos = 0) const
                        { return imp.find (str.imp, pos); }
    int compare (const NativeString& str) const
                        { return imp.compare (str.imp); }
    String toString() {return imp;}
//ODA changes end
		/** Content is initialized to a copy of a substring of str. The substring is the portion of 
		str that begins at the character position pos and takes up to n characters (it takes less than 
		n if the end of str is reached before).*/
//		NativeString ( const String& str, size_t pos, size_t n = npos ) : String(str, pos, n){}

		/* Content is initialized to a copy of the string formed by the first n characters in the array 
		of characters pointed by s.*/
//		NativeString ( const char * s, size_t n ) : String(s, n){}

		/** Content is initialized to a copy of the string formed by the null-terminated character sequence 
		(C string) pointed by s. The length of the character sequence is determined by the first occurrence 
		of a null character (as determined by traits.length(s)). This version can be used to initialize a
		string object using a string literal constant.*/
		explicit NativeString ( const char * s, Encoding encoding = ENCODING_NATIVE);

		/** Content is initialized as a string formed by a repetition of character c, n times.*/
//		NativeString ( size_t n, char c ) : String(n, c){}

		/**If InputIterator is an integral type, behaves as the sixth constructor version (the one right above 
		this) by typecasting begin and end to call it:
		string(static_cast<size_t>(begin),static_cast<char>(end));
		In any other case, the parameters are taken as iterators, and the content is initialized with the values 
		of the elements that go from the element referred by iterator begin to the element right before the one 
		referred by iterator end.
		*/
		template<class InputIterator> 
		NativeString (InputIterator begin, InputIterator end) : String(begin, end) {}

		/** Returns the string as unicode encoded wide string.*/
		WideString toWideString()const;

		/** Returns the string as utf encoded string.*/
		String toUtf8String() const;


	private:
		void fromWideString(const WideString& wstr);
	};

}


#endif //__COLLADABU_NATIVESTRING_H__
