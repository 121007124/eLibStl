#include"ElibHelp.h"
static void* GetAryElementInf(void* pAryData, DWORD& AryElementCount)
{
	LPINT pnData;
	INT nArys;
	UINT nElementCount;

	pnData = (LPINT)pAryData;

	nArys = *pnData; //ȡ��ά����
	pnData += 1;

	// �����Ա��Ŀ��
	nElementCount = 1;
	while (nArys > 0)
	{
		nElementCount *= pnData[0];
		pnData++;
		nArys--;
	}

	AryElementCount = nElementCount;
	return pnData;
}
static wchar_t* ArryToString(void* pArry, DWORD szData)
{
	DWORD dwSize;
	wchar_t* pText = (wchar_t*)GetAryElementInf(pArry, dwSize);
	if (dwSize == 0)
		return NULL;
	INT nMax = dwSize * szData;
	INT i;
	for (i = 0; i < nMax; i++)
	{
		if (pText[i] == L'\0')
			break;
	}
	UINT nLen = i;

	if (nLen == 0)return NULL;
	wchar_t* pNewText = reinterpret_cast<wchar_t*>(elibstl::malloc((nLen + 1) * sizeof(wchar_t)));
	memcpy(pNewText, pText, nLen * sizeof(wchar_t));
	pNewText[nLen] = L'\0';
	return pNewText;

}
//inline void LTrimZeroChr(std::wstring& result)
//{
//
//	auto pos = result.find_first_not_of(L'0'); // �ҵ���һ�������ַ���λ��
//	if (pos != std::wstring::npos)
//	{
//		result.erase(0, pos); // ɾ��������õ���
//	}
//	// �������С���㣬��ɾ����β�����С����
//	if (auto pos_dot = result.find_last_of(L'.'); pos_dot != std::wstring::npos)
//	{
//		// �ҵ����һ���������ֵ�λ��
//		auto pos_nonzero = result.find_last_not_of(L'0');
//		// ������һ����������λ��С����֮ǰ����С����һ��ɾ��
//		if (pos_nonzero < pos_dot)
//		{
//			result.erase(pos_dot, std::wstring::npos);
//		}
//		// ������һ����������λ��С����֮�����ɾ����β�����ַ�
//		else
//		{
//			result.erase(pos_nonzero + 1, std::wstring::npos);
//		}
//	}
//
//}


inline void LTrimZeroChr(wchar_t* str)
{
	wchar_t* z = NULL;
	wchar_t* p = NULL;

	// ��С����
	for (; *str; str++)
	{
		if (*str == L'.')
		{
			p = str++;
			break;
		}
	}

	//
	if (p) //����С����
	{
		for (; *str; str++)
		{
			if (*str == L'0')
			{
				if (!z)
				{
					z = str;
				}
			}
			else
			{
				z = NULL;
			}
		}

		//
		if (z)
		{
			if (z == p + 1) // ��С�����β��ɾ��С����
			{
				*p = L'\0';
			}
			else
			{
				*z = L'\0';
			}
		}
	}
}

EXTERN_C void Fn_to_string(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

	LPBYTE pText = NULL;
	auto ArgInf = *pArgInf;
	if ((ArgInf.m_dtDataType & DT_IS_ARY) == DT_IS_ARY)//������
	{
		ArgInf.m_dtDataType &= ~DT_IS_ARY; //ȥ�������־
		DWORD szData = 0;
		switch (ArgInf.m_dtDataType)
		{
		case SDT_BYTE:
			szData = sizeof(BYTE);
			break;
		case SDT_SHORT:
			szData = sizeof(SHORT);
			break;
		case SDT_INT64:
			szData = sizeof(INT64);
			break;
		case SDT_INT:
		case SDT_SUB_PTR:
			szData = sizeof(DWORD);
			break;
		}
		if (szData)
			pText = elibstl::clone_textw(ArryToString(ArgInf.m_pAryData, szData), static_cast<bool>(elibstl::args_to_data<BOOL>(pArgInf, 1).value_or(TRUE)));
	}
	else
	{
		wchar_t str[MAX_PATH]{ 0 };
		int nLen = 0;
		wchar_t* pSrc = NULL;

		switch (ArgInf.m_dtDataType)
		{
		case SDT_TEXT:
			nLen = strlen(ArgInf.m_pText);
			if (nLen) {
				pSrc = elibstl::A2W(ArgInf.m_pText);
				pText = elibstl::clone_textw(pSrc, static_cast<bool>(elibstl::args_to_data<BOOL>(pArgInf, 1).value_or(TRUE)));
				delete[]pSrc;
				pSrc = nullptr;
				nLen = 0;
			}
			break;
		case SDT_BYTE:
		{
			INT nVal = ArgInf.m_byte;
			wsprintf(str, L"%d", nVal);
			nLen = wcslen(str);
			pSrc = str;
		}
		break;
		case SDT_SHORT:
		{
			INT nVal = ArgInf.m_short;
			wsprintf(str, L"%d", nVal);
			nLen = wcslen(str);
			pSrc = str;
		}
		break;
		case SDT_INT:
		case SDT_SUB_PTR:
			wsprintf(str, L"%d", ArgInf.m_int);
			nLen = wcslen(str);
			pSrc = str;
			break;
		case SDT_INT64:
			wsprintf(str, L"%I64d", ArgInf.m_int64);
			nLen = wcslen(str);
			pSrc = str;
			break;
		case SDT_FLOAT:
			wsprintf(str, L"%f", ArgInf.m_float);
			LTrimZeroChr(str);
			nLen = wcslen(str);
			pSrc = str;
			break;
		case SDT_DOUBLE:
			wsprintf(str, L"%.13g", ArgInf.m_double);
			LTrimZeroChr(str);
			nLen = wcslen(str);
			pSrc = str;
			break;
		case SDT_BOOL:
			if (ArgInf.m_bool)
				wcscpy(str, L"��");
			else
				wcscpy(str, L"��");
			nLen = wcslen(str);
			pSrc = str;
			break;
		case SDT_BIN:
			pText = ArgInf.m_pBin;
			break;
		case SDT_DATE_TIME:
			auto DateTimeFormat = [](LPWSTR strValue, DATE dtDt, BOOL bOnlyDatePart)
			{
				SYSTEMTIME st = { 0 };
				VariantTimeToSystemTime(dtDt, &st);

				wchar_t strFormat[128]{ 0 };
				wcscpy_s(strFormat, L"%d��%d��%d��");


				//��ʽ��ʱ�䲿��
				wchar_t strFormatTime[128]{ 0 };

				if (st.wSecond)
					wcscpy_s(strFormatTime, L"%dʱ%d��%d��");
				else if (st.wMinute && st.wSecond == 0)
					wcscpy_s(strFormatTime, L"%dʱ%d��");
				else if (st.wHour && st.wMinute == 0 && st.wSecond == 0)
					wcscpy_s(strFormatTime, L"%dʱ");
				else
					strFormatTime[0] = 0;

				if (bOnlyDatePart == FALSE)
					wcscat_s(strFormat, strFormatTime);

				wsprintf(strValue, strFormat, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			};
			DateTimeFormat(str, ArgInf.m_double, FALSE);
			nLen = wcslen(str);
			if (nLen)
				pSrc = str;
			break;
		}
		if (pSrc && nLen)
		{
			pText = elibstl::clone_textw(pSrc, nLen, static_cast<bool>(elibstl::args_to_data<BOOL>(pArgInf, 1).value_or(TRUE)));
		}

	}
	pRetData->m_pBin = pText;
}



static ARG_INFO Args[] =
{
	{
		/*name*/    "��ת��������",
		/*explain*/ ("��ת��������"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    _SDT_ALL,
		/*default*/ 0,
		/*state*/    AS_RECEIVE_ALL_TYPE_DATA,
	},{
		/*name*/    "�Ƿ����������",
		/*explain*/ ("Ĭ�ϰ���������"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BOOL,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	}
};

FucInfo   eto_wstring = { {
		/*ccname*/  ("���ı�W"),
		/*egname*/  ("to_wstring"),
		/*explain*/ ("ת��ָ������Ϊ�ı���"),
		/*category*/2,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  &Args[0],
	} , Fn_to_string ,"Fn_to_string" };

