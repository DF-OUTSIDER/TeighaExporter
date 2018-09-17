#pragma once
#include <atlstr.h>
#include <atltime.h>
#include <io.h>
enum enLogMode{
	enDebug = 1,
	enStatistics,
};

class WEB3DMODELWRITER_API NDSUtils
{
public:
	static bool createDirsA(const CStringA &dir,bool is_a_file = false)
	{
		CStringA tempDir(dir);

		tempDir = formatPathA(dir,'\\','/');

		if(is_a_file){

			int pos = tempDir.ReverseFind('/');
			if(pos > 0)
				tempDir = tempDir.Mid(0,pos);
		}

		if(_access(tempDir,0) == 0)//path exists.so return directly.
			return true;

		for(int i = 0;i < tempDir.GetLength();i++){

			if(tempDir.GetAt(i) == '/'){

				CStringA subPath = tempDir.Mid(0,i);
				if(::GetFileAttributesA(subPath) == -1)
					::CreateDirectoryA(subPath,NULL);
			}
		}

		if(::GetFileAttributesA(tempDir) == -1)
			CreateDirectoryA(tempDir,NULL);

		return true;
	}

	static CStringA formatPathA(const CStringA& path,const char source_char = '\\',const char dest_char = '/')
	{

		CStringA resultPath(path);

		resultPath.Replace(source_char,dest_char);

		CStringA doubleDestChar;
		doubleDestChar.Format("%c%c",dest_char , dest_char);

		while(true){
			int pos = resultPath.Find(doubleDestChar);
			if(pos < 0)
				break;

			resultPath.Replace(doubleDestChar,CStringA(dest_char));
		}
		return resultPath;
	}

	static CStringA getCurrentDirA()
	{

		static CStringA csCurPath("");

		if(!csCurPath.IsEmpty())
			return csCurPath;

		char aCurPath[MAX_PATH] = {0};
		GetModuleFileNameA(0,aCurPath,MAX_PATH);
		csCurPath = aCurPath;
		int pos = csCurPath.ReverseFind('\\');
		if(pos > 0)
			csCurPath = csCurPath.Mid(0,pos);

		return csCurPath;
	}

	static void getDateTime(std::string& dateTime)
	{

		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);

		char cDateTime[32] = "";
		sprintf(cDateTime,"%04d-%02d-%02d %02d:%02d:%02d",sysTime.wYear,sysTime.wMonth,sysTime.wDay,
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

		dateTime = cDateTime;
	}
};

class WEB3DMODELWRITER_API MyLog
{
public:
	static MyLog* instance(){
		static MyLog log;
		return &log;
	}
protected:
	MyLog(){
		m_mode = enDebug;
		m_curDir = "";
	}

public:

	~MyLog(){
	}

	void setCurDir(const char* curDir){
		m_curDir = curDir;
	}

	void setMode(enLogMode mode){
		m_mode = mode;
	}

	void toLog(const char* info){

		CTime curTime;
		curTime = curTime.GetCurrentTime();

		char pcCurDate[32] = "";
		sprintf(pcCurDate,("%4d-%02d-%02d"),curTime.GetYear(),curTime.GetMonth(),curTime.GetDay());

		CStringA logPath("");
		if(m_mode == enDebug){
			logPath = NDSUtils::getCurrentDirA() + "\\Log\\Debug";
		}
		else{
			logPath = NDSUtils::getCurrentDirA() + "\\Log\\Stat";
		}

		logPath = logPath + "\\" + pcCurDate + ".log";

		toLog(logPath,info);
	}

	void toLog(const char* logFile,const char* info){

		int enabled = -1;
		if(m_curDir.IsEmpty())
			m_curDir = NDSUtils::getCurrentDirA();
		if(enabled == -1 && _access(m_curDir + "\\Log.ini",0) == 0)
			enabled = GetPrivateProfileIntA("Log","toFile",0,m_curDir + "\\Log.ini");
		if(enabled == 1){

			if(_access(logFile,0) != 0)
				NDSUtils::createDirsA(logFile,true);

			FILE* pFile = fopen(logFile,"ab");
			if(pFile == NULL)
				return;

			fwrite(info,1,strlen(info),pFile);

			fclose(pFile);
		}		
	}
private:
	 enLogMode m_mode;
	 CStringA m_curDir;
};


class WEB3DMODELWRITER_API FuncLog{

public:
	FuncLog(const char* info,const char* logFilePath = 0){

		static unsigned long index = 1;
		m_index = index;
		index ++;

		if(index > 99999999)
			index = 1;

		now = 0;endTime = 0;
		m_funcName = info;
		start();

		std::string strDateTime;
		NDSUtils::getDateTime(strDateTime);

		CStringA cInfo;
		cInfo.Format("%s<<<%s[%lu]\r\n",strDateTime.c_str(),m_funcName.c_str(),m_index);

		m_logPath = logFilePath;
		if(logFilePath == 0){
			m_logPath = NDSUtils::getCurrentDirA() + "\\function.debug.info.log";
		}

		MyLog::instance()->toLog(m_logPath,cInfo);
	}

	void Log(const char* info){
		MyLog::instance()->toLog(m_logPath,info);
	}

	~FuncLog(){

		stop();

		std::string strDateTime;
		NDSUtils::getDateTime(strDateTime);

		CStringA cInfo;
		cInfo.Format("%s>>>%s[%lu]time interval: %d milliseconds\r\n",strDateTime.c_str(),m_funcName.c_str(),m_index,getTime());

		MyLog::instance()->toLog(m_logPath,cInfo);
	}

protected:
	void start(){
		now = GetTickCount();
	}
	void stop(){
		endTime = GetTickCount();
	}
	unsigned long getTime(){
		return endTime - now;
	}

public:
	const CStringA  & GetLogPath() { return m_logPath; }
private:
	unsigned long now;
	unsigned long endTime;
	std::string m_funcName;
	unsigned long m_index;
	CStringA m_logPath;
};