#pragma once
#include<string>

enum NDSMsgID
{
	eInitilizeData,
	eInitilizeDone,
	eBeginModeler,
	eModiling,
	eEndModeler,
};

struct stuCoreProcessMsg
{
	std::string strInfo;	
};

typedef void (*CoreProcessCallBack)(UINT nMsg, std::string strInfo);