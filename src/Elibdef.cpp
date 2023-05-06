#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include"ElibHelp.h"
#include"elib/lang.h"
#include"DefCmd.h"
#pragma comment(lib, "legacy_stdio_definitions.lib") 
//���´��������ע����������Խ�cpp,����Ͷ�����ӵ�DefCmd
#ifndef  __E_STATIC_LIB
//����vc2017linker���Ӳ���,��̬���ú����ε��ɣ�Ҳ��Ӱ�춯̬��ı���
#include<algorithm>
#include<iterator>

EXTERN_C INT WINAPI LibStl_ProcessNotifyLib(INT nMsg, DWORD dwParam1, DWORD dwParam2);

#pragma region ��¶�ӿ�
extern FucInfo
ALL_E_LIB_CMD;
//��ֹ������Ⱦ


ESTL_NAMESPACE_BEGIN

extern LIB_DATA_TYPE_INFO
ALL_E_LIB_CLASS;

//��������,�������ڶ�̬���,ֻ��Ҫ��ÿ��obj��©�Ľӿ���ӵ��·��ͺ�
std::vector<FucInfo> g_all_cmd = { ALL_E_LIB_CMD };

//���ж���,�������ڶ�̬���,ֻ��Ҫ��ÿ��obj��©�Ľӿ���ӵ��·��ͺ�
static LIB_DATA_TYPE_INFO g_DataType[] = { ALL_E_LIB_CLASS };

//�����������Ϣ
std::vector<CMD_INFO> g_all_cmd_info;

//���������ָ��
std::vector<PFN_EXECUTE_CMD> g_all_pcmd;

//�����������#���ھ�̬����
std::vector<const char*> g_cmd_name;

static LIB_INFO s_LibInfo =
{
	/*m_dwLibFormatVer*/        LIB_FORMAT_VER, // ���ʽ��, Ӧ�õ���LIB_FORMAT_VER
	/*m_szGuid*/                "9E98DDB731D5421890CAD9CC6C999442", // ��Ӧ�ڱ����ΨһGUID��, ����ΪNULL���, ������а汾�˴���Ӧ��ͬ
	/*m_nMajorVersion*/         1, // ��������汾��, �������0
	/*m_nMinorVersion*/         1, // ����Ĵΰ汾��
	/*m_nBuildNumber*/          1, // �����汾��, ����Դ˰汾�����κδ���

	/*m_nRqSysMajorVer*/        3, // ����Ҫ��������ϵͳ�����汾��
	/*m_nRqSysMinorVer*/        0, // ����Ҫ��������ϵͳ�Ĵΰ汾��
	/*m_nRqSysKrnlLibMajorVer*/ 3, // ����Ҫ��ϵͳ����֧�ֿ�����汾��
	/*m_nRqSysKrnlLibMinorVer*/ 0, // ����Ҫ��ϵͳ����֧�ֿ�Ĵΰ汾��

	/*m_szName*/                "��׼ģ���", // ����, ����ΪNULL���
	/*m_nLanguage*/             __GBK_LANG_VER, // ����֧�ֵ�����
	/*m_szExplain*/             "�����Ժ��Ŀ���չ", // ����ϸ����
	/*m_dwState*/               _LIB_OS(OS_ALL), // _LIB_OS() | LBS_ ��ͷ����, ����ǲ��,������� LBS_IDE_PLUGIN

	/*m_szAuthor*/              "���������С�QingKong", // ����
	/*m_szZipCode*/             "", // �ʱ�
	/*m_szAddress*/             "", // ��ַ
	/*m_szPhoto*/               "", // ��ϵ�绰
	/*m_szFax*/                 "", // ����
	/*m_szEmail*/               "", // ����
	/*m_szHomePage*/            "", // ��ҳ
	/*m_szOther*/               "", // ������Ϣ

	/*m_nDataTypeCount*/       sizeof(g_DataType) / sizeof(g_DataType[0]), // �������Զ����������͵���Ŀ
	/*g_DataType_web*/         g_DataType, // ���������е��Զ�����������

	/*m_nCategoryCount*/        15, // ȫ�����������Ŀ, ��Ϊ0
	/*m_szzCategory*/
	"0000��������\0"
	"0000�ı�����\0"
	"0000����ϵͳ\0"
	"0000���̲���\0"
	"0000����̨����\0"
	"0000ϵͳ����\0"
	"0000����ͨ��\0"
	"0000������ȡ\0"
	"0000�����߳�Э��\0"
	"0000���ڲ���\0"
	"0000�������\0"
	"0000����\0"
	"0000���öԻ���\0"
	"0000���ݿ�\0"
	"0000�ڴ����\0"
	"\0",

	/*m_nCmdCount*/             0, // �������ṩ����������(ȫ�������������)����Ŀ, ��Ϊ0
	/*m_pBeginCmdInfo*/			0, // �������ṩ����������
	/*m_pCmdsFunc*/				0, // ָ��ÿ�������ʵ�ִ����׵�ַ, ��ΪNULL

	/*m_pfnRunAddInFn*/         NULL,
	NULL, // ������ϸ����(����һ��60�ַ�), ����������մ�����

	/*m_pfnNotify*/             LibStl_ProcessNotifyLib, // ����ΪNULL,��������ͨѶ���ӳ���

	/*m_pfnSuperTemplate*/      NULL, // ģ�� ��ΪNULL
	//  �й�SuperTemplate��˵��, �����ַ���˵��һ��SuperTemplate��
	//  ��һ��ΪSuperTemplate����(����һ��30�ַ�), �ڶ���Ϊ��ϸ����(����), 
	//  ����������մ�������
	/*m_szzSuperTemplateInfo*/  NULL, // ģ�� ��ΪNULL
	/*m_nLibConstCount*/        0, // ����Ԥ�ȶ�������г�������
	/*m_pLibConst*/             0, // ����Ԥ�ȶ�������г���
	/*m_szzDependFiles*/        NULL // ��ΪNULL, ����������������Ҫ����������֧���ļ�
};
ESTL_NAMESPACE_END

extern "C" _declspec(dllexport) PLIB_INFO GetNewInf()
{
	using namespace elibstl;
	//û��ʼ���Ļ��ȳ�ʼ��
	if (s_LibInfo.m_nCmdCount != g_all_cmd.size())
	{
		std::transform(g_all_cmd.begin(), g_all_cmd.end(), std::back_inserter(g_all_cmd_info), [](const auto& elem) { return elem.FucDef; });
		std::transform(g_all_cmd.begin(), g_all_cmd.end(), std::back_inserter(g_all_pcmd), [](const auto& elem) { return elem.pFuc; });
		s_LibInfo.m_pBeginCmdInfo = g_all_cmd_info.data();
		s_LibInfo.m_pCmdsFunc = g_all_pcmd.data();
		s_LibInfo.m_nCmdCount = g_all_cmd.size();
	}
	return  &s_LibInfo;
};
#pragma endregion ��¶�ӿ�
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		g_elibstl_hModule = hModule;
		break;
	}
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

#endif  // !__E_STATIC_LIB
EXTERN_C INT WINAPI LibStl_ProcessNotifyLib(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	using namespace elibstl;
#ifndef __E_STATIC_LIB
	switch (nMsg)
	{
	case NL_GET_CMD_FUNC_NAMES:
		if (g_cmd_name.empty())
		{
			std::transform(g_all_cmd.begin(), g_all_cmd.end(), std::back_inserter(g_cmd_name), [](const auto& elem) { return elem.FucName; });
		}
		return (INT)g_cmd_name.data();
	case NL_GET_NOTIFY_LIB_FUNC_NAME:
		return (INT)"LibStl_ProcessNotifyLib";
	case NL_GET_DEPENDENT_LIBS:
		return (INT)NULL;
	}
#endif // !__E_STATIC_LIB
	return elibstl::ProcessNotifyLib(nMsg, dwParam1, dwParam2);
}