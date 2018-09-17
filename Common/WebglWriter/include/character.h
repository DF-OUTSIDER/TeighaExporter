#pragma once

#include <Windows.h>
#include<string>   
using namespace std; 

#ifndef tstring
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
#endif


/*
*	文件名:	character.h
*	功	能:	wchar_t * 与 char * 之间的转换，string 与wstring之间的转换
*	说	明:	wchar_t * 与 char * 之间的转换 之间转换 方式采用动态申请内存方式
*			宏TEXT_ANTI_TO_TCHAR等都是申请了一个匿名临时对象，并且调用这个临时对象
*			的方法返回wchar_t * 或char * 指针
*			使用时应当注意: 临时对象在出了作用域之后，会析构内部申请的内存，
*			所以以下方式的调用是错误的:
*			
*		1. ************************************************
*			char charbuf[500]="你好"
*			wchar_t * pss = TEXT_ANTI_TO_TCHAR(charbuf);  //错误的方式，返回的指针已经被析构
			在给pss 赋值之前，匿名对象已经调用了析构函数
*			************************************************
*			
		2.	wcout << TEXT_ANTI_TO_TCHAR(charbuf)<< endl; 这样连用的调用方式是正确的
*			
*			如果要保存，请使用下面几种方式：
*		3.	*************************************************
*			Ascii_To_Wide as(charbuf) ;// 申明命名对象，调用转换方法
*			wchar_t * tst = as.wchar_rep();	
*			*************************************************
*		4.	**************************************************
*			TCHAR ss[1024];// 申请buf,将数据复制到buf中，(要足够的空间)
*			_tcscpy(ss, TEXT_ANTI_TO_TCHAR(charbuf)); 
*			**************************************************
*			
*			inline void InitLocal() 函数是为了输出wchar_t * 中的中文而设置，在调用
*			wcout wprintf 等输出函数时候，如果不能输出正常的字符串，请先调用该函数
*			可以解决调试输出问题
*
*/


//inline char *WideCharToChar(const wchar_t *pwstr,char *pcstr);
//inline wchar_t * CharToWideChar(const char *str,wchar_t *pwstr);
inline void InitLocal();  
inline string WString2String(const wstring wstr);
inline wstring String2WString(const string str);



//以下是std::string 与std::wstring 直接转换
#ifdef _UNICODE 
#define SWITCH_STRING String2WString 
#define TO_ALWAYS_STRING WString2String	//将wstring 转换成 string 适用情况是已知转换目标是wstring
#define TO_ALWAYS_WSTRING String2WString	//将string 转换成wstring 适用情况是已知转换目标是string
#else 
#define SWITCH_STRING WString2String 
#define TO_ALWAYS_STRING
#define TO_ALWAYS_WSTRING 
#endif
//end




//以下是char* 与wchar_t *直接转换
#ifdef _UNICODE
# define TEXT_ALWAYS_CHAR(STRING) (Wide_To_Ascii (STRING).char_rep ())
# define TEXT_ALWAYS_WCHAR(STRING) STRING
# define TEXT_CHAR_TO_TCHAR(STRING) (Ascii_To_Wide (STRING).wchar_rep ())
# define TEXT_WCHAR_TO_TCHAR(STRING) STRING
# define TEXT_ANTI_TO_TCHAR(STRING) (Ascii_To_Wide (STRING).wchar_rep ())
#else
# define TEXT_ALWAYS_CHAR(STRING) STRING
# define TEXT_ALWAYS_WCHAR(STRING) (Ascii_To_Wide (STRING).wchar_rep ())
# define TEXT_CHAR_TO_TCHAR(STRING) STRING
# define TEXT_WCHAR_TO_TCHAR(STRING) (Wide_To_Ascii (STRING).char_rep ())
# define TEXT_ANTI_TO_TCHAR(STRING) (Wide_To_Ascii (STRING).char_rep ())
#endif
//end

class Wide_To_Ascii
{
public:
	/// Ctor must take a wchar string.
	Wide_To_Ascii (const wchar_t *s);

	/// Dtor will free up the memory.
	~Wide_To_Ascii (void);

	/// Return the internal char* representation.
	char *char_rep (void);

	/// Converts an wchar_t string to ascii and returns a new string.
	static char *convert (const wchar_t *wstr);

private:
	/// Internal pointer to the converted string.
	char *s_;

	/// Disallow these operation.
	Wide_To_Ascii (void);
	Wide_To_Ascii (Wide_To_Ascii &);
	Wide_To_Ascii& operator= (Wide_To_Ascii &);
};

class Ascii_To_Wide
{
public:
	/// Ctor must take a wchar string.
	Ascii_To_Wide (const char *s);

	/// Dtor will free up the memory.
	~Ascii_To_Wide (void);

	/// Return the internal wchar* representation.
	wchar_t *wchar_rep (void);

	/// Converts an char string to unicode/wide and returns a new string.
	static wchar_t *convert (const char *str);

private:
	/// Internal pointer to the converted string.
	wchar_t *s_;

	/// Disallow these operation.
	Ascii_To_Wide (void);
	Ascii_To_Wide (Ascii_To_Wide &);
	Ascii_To_Wide operator= (Ascii_To_Wide &);
};

inline Wide_To_Ascii::~Wide_To_Ascii (void)
{
	delete [] this->s_;
}

inline char * Wide_To_Ascii::char_rep (void)
{
	//setlocale(LC_ALL, "chs");
	return this->s_;
}

inline char *Wide_To_Ascii::convert (const wchar_t *wstr)
{
	// Short circuit null pointer case
	if (wstr == 0)
		return 0;

	UINT cp = GetACP ();
	int len = ::WideCharToMultiByte (cp,
		0,
		wstr,
		-1,
		0,
		0,
		0,
		0);


	char *str = new char[len];

	::WideCharToMultiByte (cp, 0, wstr, -1, str, len, 0, 0);

	return str;
}

inline Wide_To_Ascii::Wide_To_Ascii (const wchar_t *s)
: s_ (Wide_To_Ascii::convert (s))
{
}

inline
Ascii_To_Wide::~Ascii_To_Wide (void)
{
	delete [] this->s_;
}

inline wchar_t * Ascii_To_Wide::wchar_rep (void)
{
	return this->s_;
}

inline wchar_t * Ascii_To_Wide::convert (const char *str)
{
	// Short circuit null pointer case
	if (str == 0)
		return 0;
	
	UINT cp = GetACP ();
	int len = ::MultiByteToWideChar (cp, 0, str, -1, 0, 0);


	wchar_t *wstr = new wchar_t[len];
	::MultiByteToWideChar (cp, 0, str, -1, wstr, len);
	return wstr;
}

inline Ascii_To_Wide::Ascii_To_Wide (const char *s)
: s_ (Ascii_To_Wide::convert (s))
{
}


inline void InitLocal()
{
	char   scp[16]; 
	UINT   cp   =   GetACP(); 
	sprintf_s(scp, ".%d ",cp); 
	setlocale(LC_ALL,   scp   );
}


//
//char *WideCharToChar(const wchar_t *pwstr,char *pcstr)
//{
//	setlocale(LC_ALL, "chs");
//	int nlength=(int)wcslen(pwstr);
//	//获取转换后的长度
//	
//	int nbytes = WideCharToMultiByte(CP_ACP,	// specify the code page used to perform the conversion
//									 0,      // no special flags to handle unmapped characters
//									 pwstr,  // wide character string to convert
//									 nlength+1, // the number of wide characters in that string
//									 NULL,     // no output buffer given, we just want to know how long it needs to be
//									 0,
//									 NULL,      // no replacement character given
//									 NULL );    // we don't want to know if a character didn't make it through the translation
//	
//	
//	//// make sure the buffer is big enough for this, making it larger if necessary
//	//if(nbytes>=cstrlen)
//	//	nbytes=cstrlen-1;
//
//	// 通过以上得到的结果，转换unicode 字符为ascii 字符
//
//	WideCharToMultiByte( CP_ACP,			// specify the code page used to perform the conversion
//						 0,         // no special flags to handle unmapped characters
//						 pwstr,     // wide character string to convert
//						 nlength+1,   // the number of wide characters in that string
//						 pcstr,		// put the output ascii characters at the end of the buffer
//						 nbytes,    // there is at least this much space there
//						 NULL,      // no replacement character given
//						 NULL );
//
//	//pcstr[nbytes] = '\0'; // 如果没有足够空间，那么可能出现违规访问!!
//	//nlength = strlen(pcstr);
//	return pcstr ;
//
//}
//
//
//
//
//wchar_t * CharToWideChar(const char *str,wchar_t *pwstr)
//{
//	
//	setlocale(LC_ALL, "chs");
//	int nlength =(int)strlen(str);
//	int nbytes =MultiByteToWideChar(CP_ACP,
//											0,
//											str,
//											nlength,
//											NULL,
//											0
//											);
//	//if(nbytes>=wstrlen)
//	//	nbytes=wstrlen-1;
//	MultiByteToWideChar(CP_ACP,
//						0,
//						str,
//						nlength,
//						pwstr,
//						nbytes
//						);
//	pwstr[nbytes]=0;	// 如果没有足够空间，那么可能出现违规访问!!
//	return pwstr;
//
//}
//
//

//将string转换成wstring   
wstring String2WString(const string str)   
{   
	wstring result;  
	setlocale(LC_ALL, "chs");
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算   
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(),(int)str.size(), NULL, 0);   
	wchar_t* buffer = new wchar_t[len + 1];   
	//多字节编码转换成宽字节编码   
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), buffer, len);   
	 buffer[len] ='\0';             //添加字符串结尾   
	//删除缓冲区并返回值   
	result.append(buffer);   
	delete[] buffer;   

	 return result;   
}

//将wstring转换成string   
string WString2String(const wstring wstr)   
{   
	string result;  
	setlocale(LC_ALL, "chs");
	 //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的   
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);   
	char* buffer = new char[len + 1];   
	//宽字节编码转换成多字节编码   
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);   
	buffer[len] = '\0';   
	//删除缓冲区并返回值   
	result.append(buffer);   
	delete[] buffer;  

	return result;   
}
