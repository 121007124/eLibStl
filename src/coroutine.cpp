#include"ElibHelp.h"
static LIB_DATA_TYPE_ELEMENT* m_hCoprocess;
namespace libkrnln {
	LIB_DATA_TYPE_INFO hCoprocessD = {
		  "Э�̾��", "hCoprocess", "Э��ӳ��Ķ��󣬿���Э�̾���Ƿ���Ч���ж�", NULL, NULL, NULL,NULL ,NULL , NULL,NULL , NULL, NULL, 0,m_hCoprocess ,
	};
}
#define DTP_HCOPROCESS   MAKELONG (5, 0) 
namespace elibcoroutine {
	static void* create_coroutine(PFIBER_START_ROUTINE func, void* arg) {
		//�Ƚ��߳�ת��ΪЭ�̣���ֹ�߳���û��ת��Э��ʱ�л�Э��
		ConvertThreadToFiber(NULL);;
		if (IsBadCodePtr((FARPROC)func)) {
			return NULL;
		}
		void* coroutine_fiber = CreateFiber(0, func, arg);
		if (coroutine_fiber == NULL) {
			return NULL;
		}
		return coroutine_fiber;
	}
	//��׼���ò��ˣ���ķ�����
	//#include <functional>
	//void* create_coroutine(void* func, void* arg) {
	//	std::function<void()> fn = std::bind((void(*)(void*))func, arg);
	//	auto pfn_fiber = [](void* pfn) -> void
	//	{
	//		std::function<void()>* fn = (std::function<void()>*)pfn;
	//		(*fn)();
	//	};
	//	void* coroutine_fiber = CreateFiber(0, (LPFIBER_START_ROUTINE)pfn_fiber, new std::function<void()>(fn));
	//	if (coroutine_fiber == NULL) {
	//		return NULL;
	//	}
	//	return coroutine_fiber;
	//}
}
static ARG_INFO createArgs[] =
{
	{
		/*name*/    "�ӳ���ָ��",
		/*explain*/ ("��ִ�е��ӳ���ָ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_SUB_PTR,
		/*default*/ 0,
		/*state*/   NULL,
	},
	{
		/*name*/    "����",
		/*explain*/ ("�����ӳ���Ĳ���"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	}
};

EXTERN_C void Fn_create_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	PFIBER_START_ROUTINE    Fun = reinterpret_cast<PFIBER_START_ROUTINE>(pArgInf[0].m_dwSubCodeAdr);
	auto arg = elibkrnln::args_to_data<INT>(pArgInf, 1);
	void* hCoprocess = elibcoroutine::create_coroutine(Fun, reinterpret_cast<void*>((arg.has_value() ? arg.value() : 0)));
	if (hCoprocess)
	{
		pRetData->m_pCompoundData = hCoprocess;
	}
}
FucInfo create_coroutine = { {
		/*ccname*/  ("����Э��"),
		/*egname*/  ("create_coroutine"),
		/*explain*/ ("����Э�̾������ͨ�� \"Э�̾���Ƿ���Ч()\"���ж�Э���Ƿ񴴽��ɹ�,Э�����������̣߳����������Ǻ���ִ�пɼ�¼������c��Ŀ纯����ת�����Ǹ���ȫ�ɿ���ͬһ���߳̿�ӵ�ж��Э�̣�ÿ���߳̿���ת��ͬЭ�̣���ת֮���¼Э��ִ��λ�ã��´������Э��ʱ��Ȼ�Ӽ�¼λ��ִ�С�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     DTP_HCOPROCESS ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  &createArgs[0],
	} ,Fn_create_coroutine ,"Fn_create_coroutine" };

static ARG_INFO m_Args[] =
{
	{
		/*name*/    "Э�̾��",
		/*explain*/ ("�ɴ���Э�����ɵ�Э�̾��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    DTP_HCOPROCESS,
		/*default*/ 0,
		/*state*/   NULL,
	}
};

EXTERN_C void Fn_switch_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	void* hCoprocess = pArgInf[0].m_pCompoundData;
	if (!hCoprocess) return;
	SwitchToFiber(hCoprocess);
}
FucInfo switch_coroutine = { {
		/*ccname*/  ("�л�Э��"),
		/*egname*/  ("switch_coroutine"),
		/*explain*/ ("�л���ǰ�߳���ָ��Э��"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     NULL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_Args,
	} ,Fn_switch_coroutine ,"Fn_switch_coroutine" };


EXTERN_C void Fn_is_valid_hCoprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = pArgInf[0].m_pCompoundData != nullptr;
}
FucInfo is_valid_hCoprocess = { {
		/*ccname*/  ("Э�̾���Ƿ���Ч"),
		/*egname*/  ("is_valid_hCoprocess"),
		/*explain*/ ("�����ж�Э���Ƿ񴴽��������ٳɹ�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_Args,
	} ,Fn_is_valid_hCoprocess ,"Fn_is_valid_hCoprocess" };

static ARG_INFO m_delete_Args[] =
{
	{
		/*name*/    "Э�̾��",
		/*explain*/ ("�ɴ���Э�����ɵ�Э�̾��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    DTP_HCOPROCESS,
		/*default*/ 0,
		/*state*/   AS_RECEIVE_VAR,
	}
};

EXTERN_C void Fn_delete_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	LPVOID* hCoprocess = pArgInf[0].m_ppCompoundData;
	if (!*hCoprocess) return;
	DeleteFiber(*hCoprocess);
	*hCoprocess = nullptr;
}
FucInfo delete_coroutine = { {
		/*ccname*/  ("����Э��"),
		/*egname*/  ("delete_coroutine"),
		/*explain*/ ("����ָ��Э��,��ͨ�� \"Э�̾���Ƿ���Ч()\"���ж�Э���Ƿ����ٳɹ�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     NULL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_Args,
	} ,Fn_delete_coroutine ,"Fn_delete_coroutine" };


EXTERN_C void Fn_get_h_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	void* ret = ConvertThreadToFiber(NULL);;
	if (!ret)
	{
		ret = GetCurrentFiber();
	}

	pRetData->m_pCompoundData = ret;
}

FucInfo get_h_coroutine = { {
		/*ccname*/  ("ȡ��ǰ����Э�̾��"),
		/*egname*/  ("get_h_coroutine"),
		/*explain*/ ("��ȡ��ǰ��������Э�̵ľ��,����ָ��Э��֮����л�,��ͨ�� \"Э�̾���Ƿ���Ч()\"���ж��Ƿ��ȡ�ɹ�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     DTP_HCOPROCESS ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,Fn_get_h_coroutine ,"Fn_get_h_coroutine" };

