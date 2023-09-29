


#include"ElibHelp.h"

static ARG_INFO Args[] =
{
	{
		"Ŀ���ڴ�",
		"",
		0,
		0,
		SDT_INT,
		0,
		NULL,
	},
	{
		"�����Ƶ��ڴ�",
		"",
		0,
		0,
		SDT_INT,
		0,
		NULL,
	},
	{
		"�ڴ泤��",
		"��ȷ���ڴ氲ȫ",
		0,
		0,
		SDT_INT,
		0,
		NULL,
	},/*{
		"�Ƿ񱨴�?",
		"���ڲ���ȫ���ڴ�������б���֮���˳�����.��������ᰲȫ������º�����Ч!�����ڵ���ʱΪ�淽����ԺͶ�λ����λ��,�����Ϊ��!",
		0,
		0,
		SDT_BOOL,
		0,
		AS_HAS_DEFAULT_VALUE,
	}*/

};

EXTERN_C void Fn_e_memcpy(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
	BOOL dbg = elibstl::is_debug();

	// �������Ƿ���Ч
	for (int i = 0; i < 3; ++i) {
		if (pArgInf[i].m_int <= 0) {
			if (!dbg) {
				return;
			}
			const wchar_t* errorMsg = nullptr;
			switch (i) {
			case 0:
				errorMsg = L"����Ҫ���ڴ渴�Ƶ�һ����ָ��/��ָ����?!";
				break;
			case 1:
				errorMsg = L"����Ҫ����ָ��/��ָ����ڴ渴��?!";
				break;
			case 2:
				errorMsg = L"�����ڸ��Ƹ�����/�㳤�ȵ��ڴ�?!";
				break;
			}
			put_errmsg(errorMsg);
			return;
		}
	}
	
	auto destination = elibstl::args_to_data_noop<char*>(pArgInf, 0);
	auto source = elibstl::args_to_data_noop<char*>(pArgInf, 1);
	auto count = elibstl::args_to_data_noop<size_t>(pArgInf, 2);

	std::copy(source, source + count, destination);
}
FucInfo e_memcpy = { {
		/*ccname*/  ("�ڴ渴��"),
		/*egname*/  ("memcpy"),
		/*explain*/ ("�����ڴ�"),
		/*category*/15,
		/*state*/   NULL,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/3,
		/*arg lp*/  Args,
	} ,Fn_e_memcpy ,"Fn_e_memcpy" };
