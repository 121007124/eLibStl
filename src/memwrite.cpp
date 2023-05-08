#include"ElibHelp.h"

static ARG_INFO Args[] =
{
	{
		"��д���ڴ������",
		"",
		0,
		0,
		_SDT_ALL,
		0,
		NULL,
	},
	{
		"�ڴ�����ָ��",
		"",
		0,
		0,
		SDT_INT,
		0,
		NULL,
	},
	{
		"�ڴ�����ָ��",
		"",
		0,
		0,
		SDT_INT,
		0,
		AS_DEFAULT_VALUE_IS_EMPTY,
	}

};

EXTERN_C void Fn_e_WriteMem(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	unsigned char* pData = nullptr;
	intptr_t dwLen;
	if ((pArgInf[0].m_dtDataType & DT_IS_ARY) == DT_IS_ARY)//������
	{
		pArgInf[0].m_dtDataType &= ~DT_IS_ARY; //ȥ�������־
		if (pArgInf[0].m_dtDataType == SDT_BYTE)//�ֽ�����
		{
			pData = elibstl::get_array_element_inf<unsigned char*>(pArgInf[0].m_pAryData, &dwLen);
			if (dwLen == 0)
				return;
		}
		else
			return;
	}
	else
	{
		if (pArgInf[0].m_dtDataType == SDT_TEXT)
		{
			if (pArgInf[0].m_pText == NULL)
				return;

			dwLen = strlen(pArgInf[0].m_pText);
			if (dwLen == 0)
				return;
			dwLen++;
			pData = (LPBYTE)pArgInf[0].m_pText;

		}
		else if (pArgInf[0].m_dtDataType == SDT_BIN)
		{
			if (pArgInf[0].m_pBin == NULL)
				return;

			LPINT p = (LPINT)pArgInf[0].m_pBin;
			dwLen = p[1];
			p += 2;
			pData = (LPBYTE)p;


		}
		else {
			auto  GetSysDataTypeDataSize = [](DATA_TYPE dtSysDataType)
			{
				switch (dtSysDataType)
				{
				case SDT_BYTE:
					return sizeof(BYTE);
				case SDT_SHORT:
					return sizeof(SHORT);
				case SDT_BOOL:
					return sizeof(BOOL);
				case SDT_INT:
					return sizeof(INT);
				case SDT_FLOAT:
					return sizeof(FLOAT);
				case SDT_SUB_PTR:    // ��¼�ӳ������ĵ�ַָ��
					return sizeof(DWORD);
				case SDT_TEXT:    // �ı��ͺ��ֽڼ���Ϊһ��ָ��,���Ϊ�ĸ��ֽ�.
				case SDT_BIN:
					return sizeof(DWORD);
				case SDT_INT64:
					return sizeof(INT64);
				case SDT_DOUBLE:
					return sizeof(DOUBLE);
				case SDT_DATE_TIME:
					return sizeof(DATE);
				default:
					return 0u;
				}

				return 0u;
			};
			dwLen = GetSysDataTypeDataSize(pArgInf[0].m_dtDataType);
			if (dwLen == 0)//��֧�ֵ���������
				return;
			pData = (unsigned char*)&pArgInf[0].m_int;
		}

	}
	if (pArgInf[2].m_dtDataType != _SDT_NULL && pArgInf[2].m_int > 0)
	{
		if (pArgInf[2].m_int < dwLen)
			dwLen = pArgInf[2].m_int;
	}
	memcpy(pArgInf[1].m_pCompoundData, pData, dwLen);
}

FucInfo e_WriteMem = { {
		/*ccname*/  ("д���ڴ�"),
		/*egname*/  ("writemem"),
		/*explain*/ ("�˴��������ڴ治����������ide�����ü���,��ȫ�ɳ���Ա����,�������ͷš�"),
		/*category*/15,
		/*state*/   NULL,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args) / sizeof(Args[0]),
		/*arg lp*/  Args,
	} ,Fn_e_WriteMem ,"Fn_e_WriteMem" };
