#pragma once
#include"elib/lib2.h"
#include"elib/fnshare.h"
#include<vector>
#include<string>
#include"..\resource.h"

#include"Tace.hpp"
struct FucInfo
{
	CMD_INFO FucDef;
	PFN_EXECUTE_CMD pFuc;
	const char* FucName;
};
//�ַ���ƴ�������������Ҳ��ʹʹ���ַ���ƴ�Ӻ���
extern HMODULE g_elibstl_hModule;
//extern vector<PFN_EXECUTE_CMD> s_ExecuteCommand;
//extern vector<const char*> s_CommandNames;
//extern vector<CMD_INFO> s_Commands;
//extern vector<LIB_DATA_TYPE_INFO> g_DataType;

//void AddFuc(FucInfo Func);
//void AddDataType(LIB_DATA_TYPE_INFO Func);
//LIB_INFO* elibkrnlnmake();
namespace elibstl {
	bool CallElibFunc(const char* elib_name, const char* def_name, PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);

}