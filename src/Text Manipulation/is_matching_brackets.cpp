#include"ElibHelp.h"
static ARG_INFO Args[] =
{
	{
		/*name*/    "�������ı�",
		/*explain*/ (""),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    _SDT_ALL,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
#include <stack>
#include <unordered_map>

static bool is_matching_brackets(const std::wstring& text) {
	std::stack<wchar_t> bracketStack;
	static std::unordered_map<wchar_t, wchar_t> bracketPairs = { {L')', L'('}, {L']', L'['}, {L'}', L'{'} };

	for (char c : text) {
		if (c == L'(' || c == L'[' || c == L'{') {
			bracketStack.push(c);
		}
		else if (c == L')' || c == L']' || c == L'}') {
			if (bracketStack.empty() || bracketStack.top() != bracketPairs[c]) {
				return false;  // ���Ų�ƥ��
			}
			bracketStack.pop();
		}
	}

	return bracketStack.empty();  // �����ջΪ�գ���ʾ����ƥ��
}


EXTERN_C void efn_is_matching_brackets(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto text = elibstl::arg_to_wstring(pArgInf, 0);
	if (!text.empty()) {
		pRetData->m_bool = is_matching_brackets(text);
	}
}
FucInfo Fn_is_matching_brackets = { {
		/*ccname*/  ("�����Ƿ�ƥ��"),
		/*egname*/  ("is_matching_brackets"),
		/*explain*/ ("����ı�ֵ�ð������()[]{}�Ƿ�ƥ�䡣([)]֮��Ľ�����Ϊ��ƥ�䡣)(֮��ĳɶ�ȴ����������Ϊ��ƥ��"),
		/*category*/2,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args)/sizeof(Args[0]),
		/*arg lp*/  Args,
	} ,ESTLFNAME(efn_is_matching_brackets)};