#include "ElibHelp.h"

EXTERN_C void libstl_InputBox(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}
static ARG_INFO s_Args_InputBox[] =
{
	{"��ʾ��Ϣ", "", 0, 0, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"���ڱ���", "", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"��ʼ�ı�", "", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"����������ݵı���", "����ֵ��ָ���ı�������Ϊ��ֵ���ı��ͣ������Բ�ͬ����������ȡ����������", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},
	{"���뷽ʽ", "����ֵ����Ϊ���³���ֵ�� 1 - �����ı�  2 - ��������  3 - ����С��  4 - �������롣���ʡ�Ա�������Ĭ��Ϊ�������ı�����", 0, 0, SDT_INT, 1, AS_HAS_DEFAULT_VALUE},
	{"������", "ָ�������ĸ����ڣ�������һ�������ڡ��������ݻ���һ�������ʹ��ھ���������ʡ�ԣ�Ĭ��Ϊ�ޡ�", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"�Ƿ��������", "", 0, 0, SDT_BOOL, FALSE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_InputBox = { {
		/*ccname*/  "�����W",
		/*egname*/  "InputBoxW",
		/*explain*/ "��һ�Ի�������ʾ��ʾ���ȴ��û��������Ĳ����°�ť������û���ȷ������󣨰��¡�ȷ�����롱��ť��س������˳��������棬���򷵻ؼ١�",
		/*category*/13,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_InputBox),
		/*arg lp*/  s_Args_InputBox },ESTLFNAME(libstl_InputBox) };