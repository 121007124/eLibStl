#include"ElibHelp.h"

static ARG_INFO Args[] =
{
	{
		"�ڴ�ָ��",
		"",
		0,
		0,
		SDT_INT,
		0,
		AS_RECEIVE_VAR,
	}
};
EXTERN_C void Fn_e_free(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if (*pArgInf->m_pInt < 0) { put_errmsg(L"�������ͷ�һ������ָ��?!"); return; }
	if (auto& p = reinterpret_cast<void*&>(*pArgInf->m_pInt);  p)
	{
		try {
			// �����ͷ��ڴ�
			operator delete(p);
		}
		catch (...) {
			put_errmsg(L"�ڴ��ͷ�ʧ��!");
			return;
		}
		p = nullptr;
	}
}

FucInfo e_free = { {
		/*ccname*/  ("�ڴ��ͷ�"),
		/*egname*/  ("free"),
		/*explain*/ ("�ͷ���������ڴ档"),
		/*category*/15,
		/*state*/   NULL,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  Args,
	} ,Fn_e_free ,"Fn_e_free" };
