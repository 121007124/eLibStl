#pragma once
#include"elib/lib2.h"
#include"elib/fnshare.h"
#include<vector>
#include<string>
#include <tuple>
#include"..\resource.h"

#include"Tace.hpp"

#define ESTL_NAMESPACE_BEGIN namespace elibstl {
#define ESTL_NAMESPACE_END }

#ifdef _MSC_VER
#define eStlInline __forceinline
#else
#define eStlInline inline
#endif

#define MAKEINTATOMW(i)  (PWSTR)((ULONG_PTR)((WORD)(i)))

typedef LPCVOID PCVOID;

#define va_arg_idx(ap, idx, t) (*(t*)((ap)+_INTSIZEOF(t)*(idx)))

template<class... Args>
eStlInline int ArgsNum(Args&&... args)
{
	return sizeof...(args);
}

#define ESTLVAL(...) ArgsNum(__VA_ARGS__), __VA_ARGS__

struct FucInfo
{
	CMD_INFO FucDef;
	PFN_EXECUTE_CMD pFuc;
	const char* FucName;
};

extern HMODULE g_elibstl_hModule;

ESTL_NAMESPACE_BEGIN
/// <summary>
/// Unicode��ANSI��ʹ��Ĭ�ϴ���ҳ
/// </summary>
/// <param name="pszW">Unicode�ַ���ָ��</param>
/// <returns>ANSI�ַ���ָ�룬Ӧʹ��delete[]�ͷ�</returns>
PSTR W2A(PCWSTR pszW);

/// <summary>
/// ANSI��Unicode��ʹ��Ĭ�ϴ���ҳ
/// </summary>
/// <param name="pszA">ANSI�ַ���ָ��</param>
/// <returns>Unicode�ַ���ָ�룬Ӧʹ��delete[]�ͷ�</returns>
PWSTR A2W(PCSTR pszA);

/// <summary>
/// ����������֧�ֿ⺯��
/// </summary>
bool CallElibFunc(const char* elib_name, const char* def_name, PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);

/// <summary>
/// ����ѡ��
/// ��һ������ָʾ��0��ʼ��������
/// </summary>
template <class T>
eStlInline T MultiSelect(int n, ...)
{
	assert(n >= 0);
	va_list Args;
	va_start(Args, n);
	T Ret = va_arg_idx(Args, n, T);
	va_end(Args);
	return Ret;
}
ESTL_NAMESPACE_END