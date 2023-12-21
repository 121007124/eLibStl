#include"ElibHelp.h"
#include"EplPinYin.h"/*ƴ���ֵ�*/
#include <unordered_map>
#include <sstream>
#include <mutex>







static ARG_INFO Args1[] =
{
	{
		"��ȡ��ƴ������ĺ���",
		"ֻȡ���ı��ײ��ĵ�һ�����֣�������ı��ײ���Ϊ���꺺�֣��������һ����Ա��ĿΪ 0 �Ŀ��ı�����",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	}
};
EXTERN_C void efn_get_all_py(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	if (data.empty())
	{
		pRetData->m_pAryData = elibstl::empty_array();
		return;
	}
	pRetData->m_pAryData = elibstl::eplpinyin::GetAllPinyinW(*data.data());
}

FucInfo g_get_all_py = { {
		/*ccname*/  ("ȡ���з���W"),
		/*egname*/  ("get_all_py"),
		/*explain*/ ("���ذ���ָ����������ƴ��������ı����飨֧�ֶ����֣������ص������������á��������顱����浽ͬ�������������������Ϊ�������"),
		/*category*/16,
		/*state*/   CT_RETRUN_ARY_TYPE_DATA,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args1) / sizeof(Args1[0]),
		/*arg lp*/  Args1,
	} ,efn_get_all_py ,"efn_get_all_py" };



static ARG_INFO Args2[] =
{
	{
		"��ȡ��ƴ���ĺ�",
		"ֻȡ���ı��ײ��ĵ�һ�����֣�������ı��ײ���Ϊ���꺺�֣��������һ����Ա��ĿΪ 0 �Ŀ��ı�����",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	}
};
EXTERN_C void efn_get_py_count(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	if (data.empty())
	{
		pRetData->m_int = 0;
		return;
	}
	std::uint8_t count = 0;
	elibstl::eplpinyin::GetPinyinStruct(*data.data(), count);
	pRetData->m_int = static_cast<INT>(count);
}

FucInfo g_get_py_count = { {
		/*ccname*/  ("ȡ������ĿW"),
		/*egname*/  ("get_py_count"),
		/*explain*/ ("����ָ�����ֵķ�����Ŀ��"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args2) / sizeof(Args2[0]),
		/*arg lp*/  Args2,
	} ,efn_get_py_count ,"efn_get_py_count" };


static ARG_INFO Args3[] =
{
	{
		"��ȡ��ƴ������ĺ���",
		"ֻȡ���ı��ײ��ĵ�һ�����֣�������ı��ײ���Ϊ���꺺�֣��������һ�����ı���",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	},
	{
		"��ȡƴ�����������",
		"ֻȡ���ı��ײ��ĵ�һ�����֣�������ı��ײ���Ϊ���꺺�֣��������һ�����ı���",
		0,
		0,
		SDT_INT,
		1,
		ArgMark::AS_HAS_DEFAULT_VALUE,
	}
};
EXTERN_C void efn_get_py(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	if (data.empty())
	{
		pRetData->m_pBin = nullptr;
		return;
	}
	pRetData->m_pBin = elibstl::eplpinyin::GetPinyinW(*data.data(), elibstl::args_to_data<INT>(pArgInf, 1).value_or(1));
}

FucInfo g_get_py = { {
		/*ccname*/  ("ȡƴ��W"),
		/*egname*/  ("get_py_count"),
		/*explain*/ ("���ذ���ָ�����ֵ�ָ��ƴ��������ı��������ָ��ƴ�����벻���ڣ������ؿ��ı���Ŀǰ��֧�ֹ��꺺�֡������ֵĵ�һ������Ϊ��������ͬ����ȫ��ͬ"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/    SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args3) / sizeof(Args3[0]),
		/*arg lp*/  Args3,
	} ,efn_get_py ,"efn_get_py" };



EXTERN_C void efn_get_py_sm(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	if (data.empty())
	{
		pRetData->m_pBin = nullptr;
		return;
	}
	pRetData->m_pBin = elibstl::clone_textw(elibstl::eplpinyin::GetSm(*data.data(), elibstl::args_to_data<INT>(pArgInf, 1).value_or(1)));
}

FucInfo g_get_py_sm = { {
		/*ccname*/  ("ȡ��ĸW"),
		/*egname*/  ("get_py_sm"),
		/*explain*/ ("���ذ���ָ�����ֵ�ָ��ƴ��������ı��������ָ��ƴ�����벻���ڣ������ؿ��ı���Ŀǰ��֧�ֹ��꺺�֡������ֵĵ�һ������Ϊ��������ͬ����ȫ��ͬ"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args3) / sizeof(Args3[0]),
		/*arg lp*/  Args3,
	} ,efn_get_py_sm ,"efn_get_py_sm" };



EXTERN_C void efn_get_py_ym(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	if (data.empty())
	{
		pRetData->m_pBin = nullptr;
		return;
	}
	pRetData->m_pBin = elibstl::clone_textw(elibstl::eplpinyin::GetYm(*data.data(), elibstl::args_to_data<INT>(pArgInf, 1).value_or(1)));
}

FucInfo g_get_py_ym = { {
		/*ccname*/  ("ȡ��ĸW"),
		/*egname*/  ("get_py_ym"),
		/*explain*/ ("���ذ���ָ������ָ��ƴ���������ĸ�����ı������ָ��ƴ�����벻���ڻ�ú��ִ˷�������ĸ�������ؿ��ı���"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args3) / sizeof(Args3[0]),
		/*arg lp*/  Args3,
	} ,efn_get_py_ym ,"efn_get_py_ym" };

#include <algorithm>

inline auto __PyToLow(const std::wstring_view& text)
{
	std::wstring ret(text);
	std::transform(text.begin(), text.end(), ret.begin(), tolower);
	return ret;
}
inline auto& __PyRemoveTrim(std::wstring& text) {
	std::wstring mark = L" ";
	size_t nSize = mark.size();
	if (text.empty() || text == L"") {
		return text;
	}
	while (1)
	{
		size_t pos = text.find(mark);    //  ����������
		if (pos == std::wstring::npos)
		{
			return text;
		}
		text.erase(pos, nSize);
	}
}
EXTERN_C void efn_comp_py(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = FALSE;

	auto IsFuzzy = elibstl::args_to_data<BOOL>(pArgInf, 2).value_or(FALSE);
	if (IsFuzzy)
	{
		auto str1 = elibstl::eplpinyin::GetStrPinyinFirst(elibstl::args_to_wsdata(pArgInf, 0));
		auto str2 = elibstl::eplpinyin::GetStrPinyinFirst(elibstl::args_to_wsdata(pArgInf, 1));
		if (__PyToLow(str1) == __PyToLow(str2))
			pRetData->m_bool = TRUE;
	}
	else {
		auto str1 = elibstl::eplpinyin::GetStrPinyinNoNop(elibstl::args_to_wsdata(pArgInf, 0));
		auto str2 = elibstl::eplpinyin::GetStrPinyinNoNop(elibstl::args_to_wsdata(pArgInf, 1));
		if (__PyToLow(str1) == __PyToLow(str2))
			pRetData->m_bool = TRUE;
	}
}
static ARG_INFO Args4[] =
{
	{
		"���Ƚ��ı�һ",
		"",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	},{
		"���Ƚ��ı���",
		"",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	},
	{
		"�Ƿ�Ϊģ���Ƚ�",
		"������Ϊ��ʱ�������Ƚ��ı�һ�ķ����ڴ��Ƚ��ı������ײ�������ʱ����Ϊ�Ƚ�ͨ���������ʡ�ԣ�Ĭ��ֵΪ�١�",
		0,
		0,
		SDT_BOOL,
		0,
		ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
FucInfo Fn_comp_py = { {
		/*ccname*/  ("�����Ƚ�W"),
		/*egname*/  ("comp_py"),
		/*explain*/ ("�Ƚ������ı��ķ��������������ͬ�����棬���򷵻ؼ١������ı��еķǹ��꺺�ֲ��֣��������м򵥵�ֵ�Ƚϡ�"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args4) / sizeof(Args4[0]),
		/*arg lp*/  Args4,
	} ,efn_comp_py ,"efn_comp_py" };





EXTERN_C void efn_comp_py_code(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = FALSE;
	auto s = elibstl::args_to_wsdata(pArgInf, 0);
	auto IsFuzzy = elibstl::eplpinyin::IsAllSpelled(s);

	if (IsFuzzy)
	{
		auto str1 = elibstl::eplpinyin::GetStrPinyinNoNop(elibstl::args_to_wsdata(pArgInf, 0));
		auto str2 = elibstl::eplpinyin::GetStrPinyinNoNop(elibstl::args_to_wsdata(pArgInf, 1));
		auto py1 = __PyToLow(str1), py2 = __PyToLow(str2);
		if (__PyRemoveTrim(py1) == __PyRemoveTrim(py2))
			pRetData->m_bool = TRUE;
	}
	else {
		auto str1 = elibstl::eplpinyin::GetStrPinyinFirst(elibstl::args_to_wsdata(pArgInf, 0));
		auto str2 = elibstl::eplpinyin::GetStrPinyinFirst(elibstl::args_to_wsdata(pArgInf, 1));
		auto py1 = __PyToLow(str1), py2 = __PyToLow(str2);
		if (__PyRemoveTrim(py1) == __PyRemoveTrim(py2))
			pRetData->m_bool = TRUE;
	
	}
	
}
static ARG_INFO Args5[] =
{
	{
		"���Ƚ�ƴ��������",
		R"(
ϵͳ�Զ��б�������־�������ƴ��ʽ����ȫƴ��ʽ,��ƴ�����ְ���Ӣ�Ĵ�Сдͬ��,ȫ�����֣�
�磺
��jsjA������ƥ�䡰�����a��
�������A��,��jsj������ƥ�䡰�������
��jisuanji��Ҳ����ƥ�䡰�������
��j��j1��Ҳ����ƥ�䡰�����1��
��jsj1��ƥ�䡰�����1����
)",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	},{
		"���Ƚ���ͨ�ı�",
		"",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	}
};
FucInfo Fn_comp_py_code = { {
		/*ccname*/  ("�����ֱȽ�W"),
		/*egname*/  ("comp_py_code"),
		/*explain*/ ("�Ƚ������ı��ķ��������������ͬ�����棬���򷵻ؼ١������ı��еķǹ��꺺�ֲ��֣��������м򵥵�ֵ�Ƚϡ�"),
		/*category*/16,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args5) / sizeof(Args5[0]),
		/*arg lp*/  Args5,
	} ,efn_comp_py_code ,"efn_comp_py_code" };



static ARG_INFO Args99[] =
{
	{
		"����ȡƴ�����ַ���",
		"�Ὣ��ת��Ϊƴ�����ַ���תΪƴ��",
		0,
		0,
		SDT_BIN,
		0,
		ArgMark::AS_NONE,
	},{
		"�Ƿ����ӿո�",
		"��Ϊÿ���ַ����ӿո��������Ϊ��ʱ����á����Ϊ��ni hao������Ϊ��nihao��,Ĭ��Ϊ��",
		0,
		0,
		SDT_BOOL,
		0,
		ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	},{
		"�Ƿ�����ƴ���ַ�",
		"Ϊ��ʱ����,���硰���A��,���Ϊ��ni hao A ma������֮Ϊ��ni hao ma��",
		0,
		0,
		SDT_BOOL,
		0,
		ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
EXTERN_C void efn_get_str_py(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_pBin = nullptr;
	auto data = elibstl::args_to_wsdata(pArgInf, 0);
	auto IsNop = elibstl::args_to_data<BOOL>(pArgInf, 1).value_or(TRUE);
	auto IsRetain = elibstl::args_to_data<BOOL>(pArgInf, 2).value_or(TRUE);
	if (data.empty())
		return;
	
	if (IsNop)
		pRetData->m_pBin = elibstl::clone_textw(elibstl::eplpinyin::GetStrPinyin(data.data(), IsRetain == TRUE));
	
	else
		pRetData->m_pBin = elibstl::clone_textw(elibstl::eplpinyin::GetStrPinyinNoNop(data.data(), IsRetain == TRUE));
	
	
}

FucInfo Fn_get_str_py = { {
		/*ccname*/  ("ȡ�ı�ƴ��W"),
		/*egname*/  ("get_str_py"),
		/*explain*/ ("��ȡ�ַ����ı��ĳ���ƴ��,���硰�Ұ��ҵ������,���ɡ�wo ai wo de zu guo��,������ƴ������᷵��ԭ����"),
		/*category*/16,
		/*state*/	NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args99) / sizeof(Args99[0]),
		/*arg lp*/  Args99,
	} ,ESTLFNAME(efn_get_str_py)};