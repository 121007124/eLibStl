//#include"ElibHelp.h"
//
//void  __stdcall fnDebugbreak() {
//	__asm int 3;
//}
//
//EXTERN_C void  fnDebugbreak(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
//{
//	OutputDebugStringA("* ����ִ�б�<<��׼ģ���>>�ġ��ϵ㡱�������ж� *");
//	fnDebugbreak();
//}
//
//
//FucInfo Fn_debugbreak = { {
//		/*ccname*/  ("�ϵ�"),
//		/*egname*/  ("debugbreak"),
//		/*explain*/ ("���µ�ǰ�����з����ϵ��쳣�� �����������߳̾Ϳ���������������ź��������쳣����Ҫ����һ�������е��¶ϵ��쳣����ʹ�� DebugBreakProcess ������"),
//		/*category*/11,
//		/*state*/     CT_DISABLED_IN_RELEASE,
//		/*ret*/     _SDT_NULL,
//		/*reserved*/NULL,
//		/*level*/   LVL_HIGH,
//		/*bmp inx*/ 0,
//		/*bmp num*/ 0,
//		/*ArgCount*/0,
//		/*arg lp*/ 0
//	} ,ESTLFNAME(fnDebugbreak)};