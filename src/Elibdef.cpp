#include"ElibHelp.h"
#include"elib/lang.h"



#ifndef  __E_STATIC_LIB

EXTERN_C INT WINAPI LibKrnln_ProcessNotifyLib(INT nMsg, DWORD dwParam1, DWORD dwParam2);
#pragma region ��¶�ӿ�_����
extern FucInfo
L,//unicode
fint_text_a,//Ѱ���ı�A
find_text_w,//Ѱ���ı�W
strlen_a,//ȡ�ı�����A
wcslen_w,//ȡ�ı�����W
rnd_ex,//ȡ�����Ex
ator,//���ȵ��Ƕ�
rtoa,//�Ƕȵ�����
prime,//�Ƿ�����
message_box_w,//�ӳ���Ϣ��W
read_file_a,//�����ļ�A
read_file_w,//�����ļ�W
write_file_a,//д���ļ�A
write_file_w,//д���ļ�W
task_dialog_w,//�߼���Ϣ��W
set_clip_board_text_w,//�ü������ı�W
get_clip_board_text_w,//ȡ�������ı�W
to_upper_w,//����дW
to_lower_w,//��СдW
to_full_w,//��ȫ��W
to_half_w,//�����W
in_str_rev_w,//�����ı�W
split_w,//�ָ��ı�W
get_character_w,//�ַ�W
get_char_code_w,//ȡ����W
ltrim_w,//ɾ�տ�W
rtrim_w,//ɾβ��W
trim_w,//ɾ��β��W
trim_all_w,//ɾȫ����W
left_text_t,//ȡ�ı����W
right_text_w,//ȡ�ı��ұ�W
mid_text_w,//ȡ�ı��м�W
open_console,//�򿪿���̨
set_console_title,//�ÿ���̨����W
read_console_w,//��׼����W
cout_w,//��׼���W
get_console_locale,//ȡ����̨����
set_console_locale,//�ÿ���̨����
get_net_work_types_w,//ö����������W
Server_GetData,//ȡ������
Server_GetCline,//ȡ�ؿͻ�
Server_CloseClient,//�Ͽ��ͻ�
Server_start,//����
Server_close,//�ر�
Server_is_open,//�Ƿ�������
Server_Send,//��������
Clinet_connect,//����
Clinet_close,//�ر�
Clinet_GetData,//ȡ������
Clinet_Send,//��������
EditBoxW_AddText,//�༭��.�����ı�
clear_folder_W,//���̲��������Ŀ¼
create_dir_W,//����Ŀ¼W
remove_dir_W,//ɾ��Ŀ¼W
remove_file_W,//ɾ���ļ�W
current_path_W,//ȡ����Ŀ¼W
get_complete_path_W,//ȡ����·��W
normalization_path_W,//�淶��·��W
get_exe_name_W,//ȡִ���ļ���W
get_cmdline_W,//ȡ������W
get_env_W,//����������W
set_env_W,//д��������W
get_special_folder_path_W,//ȡ�ض�Ŀ¼W
prevent_duplicate_execution_W,//��ֹ�����ظ�����W
atomic_increment,//ԭ�ӵ���
atomic_exchange,//ԭ�Ӹ�ֵ
atomic_decrement,//ԭ�ӵݼ�
atomic_addition,//ԭ������
create_coroutine,//����Э��
switch_coroutine,//�л�Э��
is_valid_hCoprocess,//Э�̾���Ƿ���Ч
delete_coroutine,//����Э��
get_h_coroutine,//ȡ��ǰ��������Э��
free_console,//�������̨
get_ip_this,//ȡ����ip
kill_process,//��������
is_64_bit,//�Ƿ�λ64λϵͳ
Media_structure,//ý�岥�Ź���
Media_copy,//ý�岥�Ÿ���
Media_destruct,//ý�岥������
Media_open,//ý�岥�Ŵ�
Media_IsVideo,//�Ƿ�Ϊ��Ƶ
Media_GetHwnd,//ȡ���
Media_SetHwnd,//�þ��
Media_GetMode,//ȡ״̬
Media_GetLength,//ȡ����
Media_GetPosition,//ȡλ��
Media_GetTotalSec,//ȡ��ʱ��
Media_GetFrameRate,//ȡ����
Media_Play,//����
Media_Pause,//��ͣ
Media_Stop,//ֹͣ
Media_GetMCIAlias,//ȡ����
Media_Close,//�ر�
Media_SetVolume,//������
Media_GetVolume,//ȡ����
Media_continue,//����
Media_GetChannel,//ȡý������
Media_SetChannel,//��ý������
Media_GetFrames,//ȡ֡��
Media_GetMediaVolume,//ȡý������
Media_SetMediaVolume,//��ý������
Media_SetPlaySpeed,//�ò����ٶ�
Media_GetPlaySpeed,//ȡ�����ٶ�
Media_SetPos,//��λ��
Media_SetFrame//����ָ��֡
;

extern LIB_DATA_TYPE_INFO
buttonw,//��ťW
editboxw,//�༭��W
severex,//������Ex
clinetex,//�ͻ���Ex
hCoprocessD,//Э�̾��
media_play_ex//ý�岥��
;
#pragma endregion

static const char* g_cmdname[] =
{
	rnd_ex.FucName,
	L.FucName,
	fint_text_a.FucName,
	find_text_w.FucName,
	strlen_a.FucName,
	wcslen_w.FucName,
	ator.FucName,
	rtoa.FucName,
	prime.FucName,
	message_box_w.FucName,
	read_file_a.FucName,
	read_file_w.FucName,
	write_file_a.FucName,
	write_file_w.FucName,
	task_dialog_w.FucName,
	set_clip_board_text_w.FucName,
	get_clip_board_text_w.FucName,
	to_upper_w.FucName,
	to_lower_w.FucName,
	to_full_w.FucName,
	to_half_w.FucName,
	in_str_rev_w.FucName,
	split_w.FucName,
	get_character_w.FucName,
	get_char_code_w.FucName,
	ltrim_w.FucName,
	rtrim_w.FucName,
	trim_w.FucName,
	trim_all_w.FucName,
	left_text_t.FucName,
	right_text_w.FucName,
	mid_text_w.FucName,
	open_console.FucName,
	set_console_title.FucName,
	read_console_w.FucName,
	cout_w.FucName,
	get_console_locale.FucName,
	set_console_locale.FucName,
	get_net_work_types_w.FucName,
	Server_GetData.FucName,
	Server_GetCline.FucName,
	Server_CloseClient.FucName,
	Server_start.FucName,
	Server_close.FucName,
	Server_is_open.FucName,
	Server_Send.FucName,
	Clinet_connect.FucName,
	Clinet_close.FucName,
	Clinet_GetData.FucName,
	Clinet_Send.FucName,
	EditBoxW_AddText.FucName,
	clear_folder_W.FucName,
	create_dir_W.FucName,
	remove_dir_W.FucName,
	remove_file_W.FucName,
	current_path_W.FucName,
	get_complete_path_W.FucName,
	normalization_path_W.FucName,
	get_exe_name_W.FucName,
	get_cmdline_W.FucName,
	get_env_W.FucName,
	set_env_W.FucName,
	get_special_folder_path_W.FucName,
	prevent_duplicate_execution_W.FucName,
	atomic_increment.FucName,
	atomic_exchange.FucName,
	atomic_decrement.FucName,
	atomic_addition.FucName,
	create_coroutine.FucName,
	switch_coroutine.FucName,
	is_valid_hCoprocess.FucName,
	delete_coroutine.FucName,
	get_h_coroutine.FucName,
	free_console.FucName,
	get_ip_this.FucName,
	kill_process.FucName,
	is_64_bit.FucName,
	Media_structure.FucName,
	Media_copy.FucName,
	Media_destruct.FucName,
	Media_open.FucName,
	Media_IsVideo.FucName,
	Media_GetHwnd.FucName,
	Media_SetHwnd.FucName,
	Media_GetMode.FucName,
	Media_GetLength.FucName,
	Media_GetPosition.FucName,
	Media_GetTotalSec.FucName,
	Media_GetFrameRate.FucName,
	Media_Play.FucName,
	Media_Pause.FucName,
	Media_Stop.FucName,
	Media_GetMCIAlias.FucName,
	Media_Close.FucName,
	Media_SetVolume.FucName,
	Media_GetVolume.FucName,
	Media_continue.FucName,
	Media_GetChannel.FucName,
	Media_SetChannel.FucName,
	Media_GetFrames.FucName,
	Media_GetMediaVolume.FucName,
	Media_SetMediaVolume.FucName,
	Media_SetPlaySpeed.FucName,
	Media_GetPlaySpeed.FucName,
	Media_SetPos.FucName,
	Media_SetFrame.FucName,
};
static CMD_INFO g_cmdInfo[] =
{
	rnd_ex.FucDef,
	L.FucDef,
	fint_text_a.FucDef,
	find_text_w.FucDef,
	strlen_a.FucDef,
	wcslen_w.FucDef,
	ator.FucDef,
	rtoa.FucDef,
	prime.FucDef,
	message_box_w.FucDef,
	read_file_a.FucDef,
	read_file_w.FucDef,
	write_file_a.FucDef,
	write_file_w.FucDef,
	task_dialog_w.FucDef,
	set_clip_board_text_w.FucDef,
	get_clip_board_text_w.FucDef,
	to_upper_w.FucDef,
	to_lower_w.FucDef,
	to_full_w.FucDef,
	to_half_w.FucDef,
	in_str_rev_w.FucDef,
	split_w.FucDef,
	get_character_w.FucDef,
	get_char_code_w.FucDef,
	ltrim_w.FucDef,
	rtrim_w.FucDef,
	trim_w.FucDef,
	trim_all_w.FucDef,
	left_text_t.FucDef,
	right_text_w.FucDef,
	mid_text_w.FucDef,
	open_console.FucDef,
	set_console_title.FucDef,
	read_console_w.FucDef,
	cout_w.FucDef,
	get_console_locale.FucDef,
	set_console_locale.FucDef,
	get_net_work_types_w.FucDef,
	Server_GetData.FucDef,
	Server_GetCline.FucDef,
	Server_CloseClient.FucDef,
	Server_start.FucDef,
	Server_close.FucDef,
	Server_is_open.FucDef,
	Server_Send.FucDef,
	Clinet_connect.FucDef,
	Clinet_close.FucDef,
	Clinet_GetData.FucDef,
	Clinet_Send.FucDef,
	EditBoxW_AddText.FucDef,
	clear_folder_W.FucDef,
	create_dir_W.FucDef,
	remove_dir_W.FucDef,
	remove_file_W.FucDef,
	current_path_W.FucDef,
	get_complete_path_W.FucDef,
	normalization_path_W.FucDef,
	get_exe_name_W.FucDef,
	get_cmdline_W.FucDef,
	get_env_W.FucDef,
	set_env_W.FucDef,
	get_special_folder_path_W.FucDef,
	prevent_duplicate_execution_W.FucDef,
	atomic_increment.FucDef,
	atomic_exchange.FucDef,
	atomic_decrement.FucDef,
	atomic_addition.FucDef,
	create_coroutine.FucDef,
	switch_coroutine.FucDef,
	is_valid_hCoprocess.FucDef,
	delete_coroutine.FucDef,
	get_h_coroutine.FucDef,
	free_console.FucDef,
	get_ip_this.FucDef,
	kill_process.FucDef,
	is_64_bit.FucDef,
	Media_structure.FucDef,
	Media_copy.FucDef,
	Media_destruct.FucDef,
	Media_open.FucDef,
	Media_IsVideo.FucDef,
	Media_GetHwnd.FucDef,
	Media_SetHwnd.FucDef,
	Media_GetMode.FucDef,
	Media_GetLength.FucDef,
	Media_GetPosition.FucDef,
	Media_GetTotalSec.FucDef,
	Media_GetFrameRate.FucDef,
	Media_Play.FucDef,
	Media_Pause.FucDef,
	Media_Stop.FucDef,
	Media_GetMCIAlias.FucDef,
	Media_Close.FucDef,
	Media_SetVolume.FucDef,
	Media_GetVolume.FucDef,
	Media_continue.FucDef,
	Media_GetChannel.FucDef,
	Media_SetChannel.FucDef,
	Media_GetFrames.FucDef,
	Media_GetMediaVolume.FucDef,
	Media_SetMediaVolume.FucDef,
	Media_SetPlaySpeed.FucDef,
	Media_GetPlaySpeed.FucDef,
	Media_SetPos.FucDef,
	Media_SetFrame.FucDef,
};
static PFN_EXECUTE_CMD g_pcmd[] =
{
	rnd_ex.pFuc,
	L.pFuc,
	fint_text_a.pFuc,
	find_text_w.pFuc,
	strlen_a.pFuc,
	wcslen_w.pFuc,
	ator.pFuc,
	rtoa.pFuc,
	prime.pFuc,
	message_box_w.pFuc,
	read_file_a.pFuc,
	read_file_w.pFuc,
	write_file_a.pFuc,
	write_file_w.pFuc,
	task_dialog_w.pFuc,
	set_clip_board_text_w.pFuc,
	get_clip_board_text_w.pFuc,
	to_upper_w.pFuc,
	to_lower_w.pFuc,
	to_full_w.pFuc,
	to_half_w.pFuc,
	in_str_rev_w.pFuc,
	split_w.pFuc,
	get_character_w.pFuc,
	get_char_code_w.pFuc,
	ltrim_w.pFuc,
	rtrim_w.pFuc,
	trim_w.pFuc,
	trim_all_w.pFuc,
	left_text_t.pFuc,
	right_text_w.pFuc,
	mid_text_w.pFuc,
	open_console.pFuc,
	set_console_title.pFuc,
	read_console_w.pFuc,
	cout_w.pFuc,
	get_console_locale.pFuc,
	set_console_locale.pFuc,
	get_net_work_types_w.pFuc,
	Server_GetData.pFuc,
	Server_GetCline.pFuc,
	Server_CloseClient.pFuc,
	Server_start.pFuc,
	Server_close.pFuc,
	Server_is_open.pFuc,
	Server_Send.pFuc,
	Clinet_connect.pFuc,
	Clinet_close.pFuc,
	Clinet_GetData.pFuc,
	Clinet_Send.pFuc,
	EditBoxW_AddText.pFuc,
	clear_folder_W.pFuc,
	create_dir_W.pFuc,
	remove_dir_W.pFuc,
	remove_file_W.pFuc,
	current_path_W.pFuc,
	get_complete_path_W.pFuc,
	normalization_path_W.pFuc,
	get_exe_name_W.pFuc,
	get_cmdline_W.pFuc,
	get_env_W.pFuc,
	set_env_W.pFuc,
	get_special_folder_path_W.pFuc,
	prevent_duplicate_execution_W.pFuc,
	atomic_increment.pFuc,
	atomic_exchange.pFuc,
	atomic_decrement.pFuc,
	atomic_addition.pFuc,
	create_coroutine.pFuc,
	switch_coroutine.pFuc,
	is_valid_hCoprocess.pFuc,
	delete_coroutine.pFuc,
	get_h_coroutine.pFuc,
	free_console.pFuc,
	get_ip_this.pFuc,
	kill_process.pFuc,
	is_64_bit.pFuc,
	Media_structure.pFuc,
	Media_copy.pFuc,
	Media_destruct.pFuc,
	Media_open.pFuc,
	Media_IsVideo.pFuc,
	Media_GetHwnd.pFuc,
	Media_SetHwnd.pFuc,
	Media_GetMode.pFuc,
	Media_GetLength.pFuc,
	Media_GetPosition.pFuc,
	Media_GetTotalSec.pFuc,
	Media_GetFrameRate.pFuc,
	Media_Play.pFuc,
	Media_Pause.pFuc,
	Media_Stop.pFuc,
	Media_GetMCIAlias.pFuc,
	Media_Close.pFuc,
	Media_SetVolume.pFuc,
	Media_GetVolume.pFuc,
	Media_continue.pFuc,
	Media_GetChannel.pFuc,
	Media_SetChannel.pFuc,
	Media_GetFrames.pFuc,
	Media_GetMediaVolume.pFuc,
	Media_SetMediaVolume.pFuc,
	Media_SetPlaySpeed.pFuc,
	Media_GetPlaySpeed.pFuc,
	Media_SetPos.pFuc,
	Media_SetFrame.pFuc,
};
static LIB_DATA_TYPE_INFO g_DataType[] = {
	buttonw,
	editboxw,
	severex,
	clinetex,
	hCoprocessD,
	media_play_ex
};
static LIB_INFO s_LibInfo =
{
	/*m_dwLibFormatVer*/        LIB_FORMAT_VER, // ���ʽ��, Ӧ�õ���LIB_FORMAT_VER
	/*m_szGuid*/                "9E98DDB731D5421890CAD9CC6C999442", // ��Ӧ�ڱ����ΨһGUID��, ����ΪNULL���, ������а汾�˴���Ӧ��ͬ
	/*m_nMajorVersion*/         2, // ��������汾��, �������0
	/*m_nMinorVersion*/         2, // ����Ĵΰ汾��
	/*m_nBuildNumber*/          1, // �����汾��, ����Դ˰汾�����κδ���

	/*m_nRqSysMajorVer*/        3, // ����Ҫ��������ϵͳ�����汾��
	/*m_nRqSysMinorVer*/        0, // ����Ҫ��������ϵͳ�Ĵΰ汾��
	/*m_nRqSysKrnlLibMajorVer*/ 3, // ����Ҫ��ϵͳ����֧�ֿ�����汾��
	/*m_nRqSysKrnlLibMinorVer*/ 0, // ����Ҫ��ϵͳ����֧�ֿ�Ĵΰ汾��

	/*m_szName*/                "LibKrnln", // ����, ����ΪNULL���
	/*m_nLanguage*/             __GBK_LANG_VER, // ����֧�ֵ�����
	/*m_szExplain*/             "", // ����ϸ����
	/*m_dwState*/               _LIB_OS(OS_ALL), // _LIB_OS() | LBS_ ��ͷ����, ����ǲ��,������� LBS_IDE_PLUGIN

	/*m_szAuthor*/              "����", // ����
	/*m_szZipCode*/             "", // �ʱ�
	/*m_szAddress*/             "", // ��ַ
	/*m_szPhoto*/               "", // ��ϵ�绰
	/*m_szFax*/                 "", // ����
	/*m_szEmail*/               "", // ����
	/*m_szHomePage*/            "", // ��ҳ
	/*m_szOther*/               "", // ������Ϣ

	/*m_nDataTypeCount*/       sizeof(g_DataType) / sizeof(g_DataType[0]), // �������Զ����������͵���Ŀ
	/*g_DataType_web*/        g_DataType, // ���������е��Զ�����������

	/*m_nCategoryCount*/        9, // ȫ�����������Ŀ, ��Ϊ0
	/*m_szzCategory*/           "0000��������\0" "0000�ı�����\0""0000����ϵͳ\0""0000���̲���\0""0000����̨����\0""0000ϵͳ����\0""0000����ͨ��\0""0000������ȡ\0""0000�����߳�Э��\0""\0", // ȫ�����������Ŀ, ��Ϊ0

	/*m_nCmdCount*/             sizeof(g_cmdInfo) / sizeof(g_cmdInfo[0]), // �������ṩ����������(ȫ�������������)����Ŀ, ��Ϊ0
	/*m_pBeginCmdInfo*/        g_cmdInfo, // �������ṩ����������
	/*m_pCmdsFunc*/            g_pcmd, // ָ��ÿ�������ʵ�ִ����׵�ַ, ��ΪNULL

	/*m_pfnRunAddInFn*/         NULL,
	NULL, // ������ϸ����(����һ��60�ַ�), ����������մ�����

	/*m_pfnNotify*/             LibKrnln_ProcessNotifyLib, // ����ΪNULL,��������ͨѶ���ӳ���

	/*m_pfnSuperTemplate*/      NULL, // ģ�� ��ΪNULL
  //  �й�SuperTemplate��˵��, �����ַ���˵��һ��SuperTemplate��
 //  ��һ��ΪSuperTemplate����(����һ��30�ַ�), �ڶ���Ϊ��ϸ����(����), 
  //  ����������մ�������
  /*m_szzSuperTemplateInfo*/  NULL, // ģ�� ��ΪNULL
  /*m_nLibConstCount*/        0, // ����Ԥ�ȶ�������г�������
  /*m_pLibConst*/             0, // ����Ԥ�ȶ�������г���
  /*m_szzDependFiles*/        NULL // ��ΪNULL, ����������������Ҫ����������֧���ļ�
};

extern "C" _declspec(dllexport)  PLIB_INFO  GetNewInf()
{

	return  &s_LibInfo;

};
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

#endif 
EXTERN_C INT WINAPI LibKrnln_ProcessNotifyLib(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{

#ifndef __E_STATIC_LIB

	if (nMsg == NL_GET_CMD_FUNC_NAMES)

		return (INT)g_cmdname;

	else if (nMsg == NL_GET_NOTIFY_LIB_FUNC_NAME)

		return (INT)"LibKrnln_ProcessNotifyLib";

	else if (nMsg == NL_GET_DEPENDENT_LIBS) return (INT)NULL;


#endif

	return elibkrnln::ProcessNotifyLib(nMsg, dwParam1, dwParam2);

}