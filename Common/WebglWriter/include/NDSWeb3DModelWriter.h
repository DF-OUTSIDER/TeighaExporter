// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WEB3DMODELWRITER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WEB3DMODELWRITER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#ifndef _WEB3DSTATIC
#ifdef WEB3DMODELWRITER_EXPORTS
#define WEB3DMODELWRITER_API __declspec(dllexport)
#else
#define WEB3DMODELWRITER_API __declspec(dllimport)
#endif
#else
#define  WEB3DMODELWRITER_API  
#endif

#include "NDSWeb3DObject.h"

using namespace ModelExporter;
class WEB3DMODELWRITER_API NDSWeb3DModelWriter
{
public:
	NDSWeb3DModelWriter(void);
	~NDSWeb3DModelWriter(void);

public:
	/************************************************************************/
	/*                          WriteWeb3DModelFiles参数说明                  */
	//nCompressLevel 有效范围为0 - 9，传入 -1 时表明不压缩
	//nNormalBit 有效范围为 1， 2， 4，标明单个法向量值的位数，传入4时表明不处理法向
	//bLineIndexSorted line索引是否是连续递增（间隔0或1）的，例如0 1 1 2 3 4
	/************************************************************************/
	static bool WriteWeb3DModelFiles(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bIsUTF8 = true);
	static bool WriteWeb3DModelFilesVersion3(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true);
	static bool WriteWeb3DModelFilesVersion4(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true);

	static bool WriteWeb3DModelFilesBufferChunks(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);
	static bool WriteWeb3DModelFilesVersion3BufferChunks(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);
	static bool WriteWeb3DModelFilesVersion4BufferChunks(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);
	//version 5 support skeleton and morph animation
	static bool WriteWeb3DModelFilesVersion5BufferChunks(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);
	//version 6 support PMI
	static bool WriteWeb3DModelFilesVersion6BufferChunks(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);
	//version 7 support bvh
	static bool WriteWeb3DModelFilesVersion7BufferChunksBvh(Va3cContainer& container,const string &strDestFolder,const string &strObjFile,const string &strBinFile,int nCompressLevel = -1,int nNormalBit = 1,bool bLineIndexSorted = true,bool bIsUTF8 = true,int nBufferChunkLimit = GEO_BUFFER_CHUNK_LIMIT);

	static bool WriteHtmlTemplate(const string &strDestFolder,const string & strObjFile,const string & strBinFile,bool bIsUTF8 = true);
	static const char *  ConvertToUTF8(const std::string& strGBK);	
	static string GenerateGUIDString();
	static int ColorToInt(const double& r,const double& g,const double& b);
	static int ColorToInt(const int& r,const int& g,const int& b);
	static void DecomposeMatrix(float pMatrix[16]  ,float pPosition[3] ,float pScale[3] ,float pQuaternion[4] );
	static Va3cContainer::Va3cObject* GetObjectByUUID(const string &strUUID , Va3cContainer::Va3cObject* pVa3cObject);
};
