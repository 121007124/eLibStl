#pragma once
#include"elib/lib2.h"
#include"elib/fnshare.h"
#include<vector>
#include<string>
#include <tuple>
#include"..\resource.h"
#include<assert.h>
#include <CommCtrl.h>
#include"Tace.hpp"



#define ESTL_NAMESPACE_BEGIN namespace elibstl {
#define ESTL_NAMESPACE_END }

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#define eStlInline __forceinline
#else
#define eStlInline inline
#endif

#define MAKEINTATOMW(i)  (PWSTR)((ULONG_PTR)((WORD)(i)))
#define va_arg_idx(ap, idx, t) (*(t*)((ap)+_INTSIZEOF(t)*(idx)))

struct FucInfo
{
	CMD_INFO FucDef;
	PFN_EXECUTE_CMD pFuc;
	const char* FucName;
};
#define ESTLFNAME(s) s,#s

extern HMODULE g_elibstl_hModule;

ESTL_NAMESPACE_BEGIN

typedef LPCVOID PCVOID;
typedef LPCBYTE PCBYTE;
typedef UINT BITBOOL;

template<class... Args>
eStlInline int ArgsNum(Args&&... args)
{
	return sizeof...(args);
}

#define ESTLVAL(...) ArgsNum(__VA_ARGS__), __VA_ARGS__

eStlInline bool isUnicode(const unsigned char* data, size_t length) {
	// ����ֽ����� (BOM)
	if (length >= 2 && data[0] == 0xFF && data[1] == 0xFE) {
		// UTF-16LE �ֽ��򣬵�λ�ֽ���ǰ
		return true;
	}
	else if (length >= 2 && data[0] == 0xFE && data[1] == 0xFF) {
		// UTF-16BE �ֽ��򣬸�λ�ֽ���ǰ
		return true;
	}

	// ����ַ���Ӧ���ֽ�����
	for (size_t i = 0; i < length; i += 2) {
		unsigned char highByte = data[i];
		unsigned char lowByte = data[i + 1];

		if ((highByte >= 0xD8 && highByte <= 0xDB) && (lowByte >= 0xDC && lowByte <= 0xDF)) {
			// �ߴ�����͵ʹ������ֽڣ��Ϸ��� UTF-16 ����
			continue;
		}

		if ((highByte >= 0xD8 && highByte <= 0xDB) || (lowByte >= 0xDC && lowByte <= 0xDF)) {
			// �ߴ������ʹ������ֽڵ������֣��Ƿ��� UTF-16 ����
			return false;
		}
	}

	return true;
}

/// <summary>
/// Unicode��ANSI��ʹ��Ĭ�ϴ���ҳ
/// </summary>
/// <param name="pszW">Unicode�ַ���ָ��</param>
/// <returns>ANSI�ַ���ָ�룬Ӧʹ��delete[]�ͷ�</returns>
PSTR W2A(PCWSTR pszW);


PSTR W2UTF8(const std::wstring_view& text);

PSTR W2UTF8(const std::wstring_view& text);

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

eStlInline BOOL IsFILETIMEZero(const FILETIME* pft)
{
	// ���ܽ�FILETIME*ת��ΪULONGLONG*����Ϊ�����ж������
	ULARGE_INTEGER ull;
	ull.HighPart = pft->dwHighDateTime;
	ull.LowPart = pft->dwLowDateTime;
	return ull.QuadPart == 0;
}

eStlInline BOOL operator==(const FILETIME& ft1, const FILETIME& ft2)
{
	return CompareFileTime(&ft1, &ft2) == 0;
}
eStlInline BOOL operator>(const FILETIME& ft1, const FILETIME& ft2)
{
	return CompareFileTime(&ft1, &ft2) == 1;
}
eStlInline BOOL operator<(const FILETIME& ft1, const FILETIME& ft2)
{
	return CompareFileTime(&ft1, &ft2) == -1;
}

eStlInline BOOL IsBitExist(DWORD dw1, DWORD dw2)
{
	return ((dw1 & dw2) == dw2);
}

#define SRSAllocW(cch) (PWSTR)HeapAlloc(GetProcessHeap(), 0, cch * sizeof(WCHAR))
#define SRSReAllocW(psz, cch) (PWSTR)HeapReAlloc(GetProcessHeap(), 0, psz, cch * sizeof(WCHAR))
#define SRSFree(psz) HeapFree(GetProcessHeap(), 0, psz)
#define SRSMakeCapacity(cch) (cch * 2)

// ���ַ������ṩ͸������������
class CSimpleRefStrW
{
public:
	PWSTR m_pszText = NULL;
	int m_cchText = 0;
	int m_cchCapacity = 0;

	CSimpleRefStrW() = default;

	CSimpleRefStrW(int cchInit)
	{
		m_cchCapacity = cchInit + 1;
		m_pszText = SRSAllocW(cchInit + 1);
	}

	CSimpleRefStrW(PCWSTR psz, int cchText = -1)
	{
		if (!psz || !cchText)
			return;
		if (cchText < 0)
			cchText = wcslen(psz);
		if (!cchText)
			return;
		m_cchText = cchText;
		m_cchCapacity = SRSMakeCapacity(cchText + 1);
		m_pszText = SRSAllocW(m_cchCapacity);
		wcscpy(m_pszText, psz);
	}

	CSimpleRefStrW(const CSimpleRefStrW& x)
	{
		m_cchText = x.m_cchText;
		m_cchCapacity = SRSMakeCapacity(m_cchText + 1);
		m_pszText = SRSAllocW(m_cchCapacity);
		if (x.m_pszText)
			wcscpy(m_pszText, x.m_pszText);
	}

	CSimpleRefStrW(CSimpleRefStrW&& x) noexcept
	{
		SRSFree(m_pszText);
		m_cchText = x.m_cchText;
		m_cchCapacity = x.m_cchCapacity;
		m_pszText = x.m_pszText;
		x.m_pszText = NULL;
		x.m_cchText = 0;
		x.m_cchCapacity = 0;
	}

	~CSimpleRefStrW()
	{
		SRSFree(m_pszText);
	}

	CSimpleRefStrW& operator=(PCWSTR pszSrc)
	{
		DupString(pszSrc);
		return *this;
	}

	CSimpleRefStrW& operator=(CSimpleRefStrW& x)
	{
		DupString(x.m_pszText, x.m_cchText);
		return *this;
	}

	CSimpleRefStrW& operator=(CSimpleRefStrW&& x) noexcept
	{
		SRSFree(m_pszText);
		m_cchText = x.m_cchText;
		m_cchCapacity = x.m_cchCapacity;
		m_pszText = x.m_pszText;
		x.m_pszText = NULL;
		x.m_cchText = 0;
		x.m_cchCapacity = 0;
		return *this;
	}

	operator PWSTR()
	{
		return m_pszText;
	}

	int DupString(PCWSTR pszSrc, int cchSrc = -1);

	PWSTR Attach(PWSTR psz, int cchCapacity, int cchText = -1);

	eStlInline PWSTR Dettach()
	{
		auto pTemp = m_pszText;
		m_cchCapacity = 0;
		m_cchText = 0;
		m_pszText = NULL;
		return pTemp;
	}

	int PushBack(PCWSTR pszSrc, int cchSrc = -1);

	eStlInline void CopyTo(PWSTR pszDst, int cch = -1)
	{
		if (cch < 0)
			cch = m_cchText;
		if (!cch || !m_pszText || !pszDst)
			return;
		wcsncpy(pszDst, m_pszText, cch);
		*(pszDst + cch) = L'\0';
	}
};
ESTL_NAMESPACE_END