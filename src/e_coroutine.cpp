#include"ElibHelp.h"
#include"MiniCo/coroutine.h"

//static LIB_DATA_TYPE_ELEMENT* m_hCoprocess = nullptr;
//static LIB_DATA_TYPE_ELEMENT* m_hCoroutine = nullptr;
namespace elibstl {
	LIB_DATA_TYPE_INFO hCoprocessD = {
		  "Э�̾��", "hCoprocess", "Э��ӳ��Ķ���ID", NULL, NULL, LDT_IS_HIDED,NULL ,NULL , NULL,NULL , NULL, NULL, 0,nullptr ,
	};
	LIB_DATA_TYPE_INFO hCoroutine = {
		  "Э�̵�����", "hCoroutine", "����Э�̵����л�", NULL, NULL, LDT_IS_HIDED,NULL ,NULL , NULL,NULL , NULL, NULL, 0,nullptr ,
	};
}

static ARG_INFO createArgs[] =
{
	{
		/*name*/    "Э�̵��������",
		/*explain*/ ("�ɡ�����Э�̵����������ء�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	},
	{
		/*name*/    "�ӳ����ַ",
		/*explain*/ ("��ִ�е��ӳ���ָ��,����ԭ�ͱ���Ϊ��[�޷���ֵ] ����([������]Э�̵�����,[������]����)��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_SUB_PTR,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "����",
		/*explain*/ ("�����ӳ���Ĳ���"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};


EXTERN_C void Fn_create_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto pSchedule = reinterpret_cast<schedule*>(pArgInf->m_int);
	auto Fun = reinterpret_cast<coroutine_func>(pArgInf[1].m_pdwSubCodeAdr);
	void* arg = reinterpret_cast<void*>(pArgInf[2].m_int);
	pRetData->m_int = coroutine_new(pSchedule, Fun, arg);
}
FucInfo create_coroutine = { {
		/*ccname*/  ("����Э��"),
		/*egname*/  ("create_coroutine"),
		/*explain*/ ("��ָ���������м���Э�̲�����Э��ID,Э�����������̣߳�������Э�̾��.���������Ǻ���ִ�пɼ�¼������c��Ŀ纯����ת�����Ǹ���ȫ�ɿ���ͬһ����������ӵ�ж��Э�̣����ڵ������жԺ���Э�̽����л����л�֮���¼Э��ִ��λ�ã��´������Э��ʱ��Ȼ�Ӽ�¼λ��ִ�С�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     SDT_INT ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/3,
		/*arg lp*/  &createArgs[0],
	} ,Fn_create_coroutine ,"Fn_create_coroutine" };

static ARG_INFO m_Args[] =
{
	{
		/*name*/    "Э�̵��������",
		/*explain*/ ("�ɡ�����Э�̵����������ء�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "Э��ID",
		/*explain*/ ("�ɴ���Э�����ɵ�Э�̾��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};

EXTERN_C void Fn_switch_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto pSchedule = reinterpret_cast<schedule*>(pArgInf->m_int);
	coroutine_resume(pSchedule, pArgInf[1].m_int);
}
FucInfo switch_coroutine = { {
		/*ccname*/  ("�ָ�Э��"),
		/*egname*/  ("coroutine_resume"),
		/*explain*/ ("�ָ�ָ��ID��Э�̡�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     DATA_TYPE::_SDT_NULL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  m_Args,
	} ,Fn_switch_coroutine ,"Fn_switch_coroutine" };



static ARG_INFO m_hCo_args[] =
{
	{
		/*name*/    "Э�̵��������",
		/*explain*/ ("�ɴ���Э�̵��������ɵľ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};

EXTERN_C void Fn_is_valid_hCoprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	coroutine_yield(reinterpret_cast<schedule*>(pArgInf->m_int));
}
FucInfo is_valid_hCoprocess = { {
		/*ccname*/  ("����Э��"),
		/*egname*/  ("coroutine_yield"),
		/*explain*/ ("�һ�ָ���������ĵ�ǰ����ִ�е�Э�̡�"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     _SDT_NULL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_hCo_args,
	} ,Fn_is_valid_hCoprocess ,"Fn_is_valid_hCoprocess" };



EXTERN_C void Fn_delete_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto pSchedule = reinterpret_cast<schedule*>(pArgInf->m_int);
	pRetData->m_int = coroutine_status(pSchedule, pArgInf[1].m_int);
}
FucInfo delete_coroutine = { {
		/*ccname*/  ("ȡЭ��״̬"),
		/*egname*/  ("coroutine_status"),
		/*explain*/ ("����ָ��Э�̵�������ָ��Э�̵�״̬��"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     DTP_COROUTINE ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  m_Args,
	} ,Fn_delete_coroutine ,"Fn_delete_coroutine" };


EXTERN_C void Fn_get_h_coroutine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = coroutine_running(reinterpret_cast<schedule*>(pArgInf->m_int));
}

FucInfo get_h_coroutine = { {
		/*ccname*/  ("ȡ��ǰִ��Э��ID"),
		/*egname*/  ("get_h_coroutine"),
		/*explain*/ ("����ָ���������������е�Э��ID��"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     SDT_INT ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_hCo_args,
	} ,Fn_get_h_coroutine ,"Fn_get_h_coroutine" };


/*����Э�̵�����*/
EXTERN_C void Fn_coroutine_open(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = reinterpret_cast<std::uintptr_t> (coroutine_open());
}
FucInfo e_coroutine_open = { {
		/*ccname*/  ("����Э�̵�����"),
		/*egname*/  ("coroutine_open"),
		/*explain*/ ("����������һ���µ�Э�̵����������"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     SDT_INT ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,Fn_coroutine_open ,"Fn_coroutine_open" };


/*����Э�̵�����*/
EXTERN_C void Fn_coroutine_close(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	coroutine_close(reinterpret_cast<schedule*>(pArgInf->m_int));
}
FucInfo e_coroutine_close = { {
		/*ccname*/  ("����Э�̵�����"),
		/*egname*/  ("coroutine_close"),
		/*explain*/ ("�رղ��ͷ�Э�̵�������"),
		/*category*/9,
		/*state*/   NULL,
		/*ret*/     _SDT_NULL ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  m_hCo_args,
	} ,Fn_coroutine_close ,"Fn_coroutine_close" };


static ARG_INFO m_hCo_int_args[] =
{
	{
		/*name*/    "Э�̵�����ָ��",
		/*explain*/ ("���ݽ���Э�̵ĵ�����ָ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}
};


EXTERN_C void Fn_hCoi2h(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_pCompoundData = (reinterpret_cast<void*>(pArgInf->m_int));
}

FucInfo e_hCoi2h = { {
		/*ccname*/  ("ָ�뵽������"),
		/*egname*/  ("hCoi2h"),
		/*explain*/ ("��Э�̺��������ָ��תΪ��������"),
		/*category*/9,
		/*state*/   CT_IS_HIDED,
		/*ret*/     SDT_INT ,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ m_hCo_int_args,
	} ,Fn_hCoi2h ,"Fn_hCoi2h" };



static LIB_DATA_TYPE_ELEMENT s_Enum[] =
{
	 {SDT_INT, 0, "����", "COROUTINE_DEAD", NULL, LES_HAS_DEFAULT_VALUE, COROUTINE_DEAD},
	 {SDT_INT, 0, "����", "COROUTINE_READY", NULL, LES_HAS_DEFAULT_VALUE, COROUTINE_READY},
	 {SDT_INT, 0, "������", "COROUTINE_RUNNING", NULL, LES_HAS_DEFAULT_VALUE,COROUTINE_RUNNING},
	 {SDT_INT, 0, "������", "COROUTINE_SUSPEND", NULL, LES_HAS_DEFAULT_VALUE, COROUTINE_SUSPEND},
};
namespace elibstl {
	LIB_DATA_TYPE_INFO COROUTINE_Type =
	{
		"Э��״̬",
		"COROUTINE",
		"��ǰЭ������״̬",
		NULL,
		NULL,
		_DT_OS(__OS_WIN) | LDT_ENUM,
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		sizeof(s_Enum) / sizeof(s_Enum[0]),
		s_Enum
	};
}