#include"ElibHelp.h"

static ARG_INFO Args[] =
{
	{
		"���볤��",
		"",
		0,
		0,
		SDT_INT,
		0,
		ArgMark::AS_NONE,
	},
	{
		"�Ƿ��ʼ��Ϊ��",
		"",
		0,
		0,
		SDT_BOOL,
		0,
		ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}

};

EXTERN_C void Fn_e_malloc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = 0;
	if (pArgInf[0].m_int <= 0) { put_errmsg(L"����������һ��������/�㳤�ȵ��ڴ�!"); return; }
	auto p = operator new(static_cast<std::size_t>(pArgInf[0].m_int), std::nothrow);
	if (!p)return;
	if (pArgInf[1].m_dtDataType != _SDT_NULL && pArgInf[1].m_bool)std::memset(p, 0, static_cast<std::size_t>(pArgInf[0].m_int));
	pRetData->m_int = reinterpret_cast<std::uintptr_t>(p);
}

FucInfo e_malloc = { {
		/*ccname*/  ("�ڴ�����"),
		/*egname*/  ("malloc"),
		/*explain*/ ("�˴��������ڴ治����������ide�����ü���,��ȫ�ɳ���Ա����,�������ͷš�"),
		/*category*/15,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  Args,
	} ,Fn_e_malloc ,"Fn_e_malloc" };
