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

template<class T>
eStlInline const T& ESTLPRIV_MultiSelectHelp___(int i, const T& Item)
{
	return Item;
}

template<class T, class...Args>
eStlInline const T& ESTLPRIV_MultiSelectHelp___(int i, const T& Item, const Args&...Items)
{
	if (i > 0)
	{
		i--;
		return ESTLPRIV_MultiSelectHelp___(i, Items...);
	}
	else
		return Item;
}

/// <summary>
/// ����ѡ��
/// ��һ������ָʾ��0��ʼ��������
/// </summary>
template<class...T>
eStlInline const auto& MultiSelect(int i, const T&...Items)
{
	assert(i >= 0 || i < sizeof...(Items));
	return ESTLPRIV_MultiSelectHelp___(i, Items...);
}
ESTL_NAMESPACE_END