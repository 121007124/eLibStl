#pragma region ������ӵ���
#define ALL_E_LIB_CMD \
rnd_ex,/*ȡ�����Ex*/\
L,/*unicode_base*/\
fint_text_a,/*Ѱ���ı�A*/\
find_text_w,/*Ѱ���ı�W*/\
strlen_a,/*ȡ�ı�����A*/\
wcslen_w,/*ȡ�ı�����W*/\
ator,/*���ȵ��Ƕ�*/\
rtoa,/*�Ƕȵ�����*/\
prime,/*�Ƿ�����*/\
message_box_w,/*�ӳ���Ϣ��W*/\
read_file_a,/*�����ļ�A*/\
read_file_w,/*�����ļ�W*/\
write_file_a,/*д���ļ�A*/\
write_file_w,/*д���ļ�W*/\
task_dialog_w,/*�߼���Ϣ��W*/\
set_clip_board_text_w,/*�ü������ı�W*/\
get_clip_board_text_w,/*ȡ�������ı�W*/\
to_upper_w,/*����дW*/\
to_lower_w,/*��СдW*/\
to_full_w,/*��ȫ��W*/\
to_half_w,/*�����W*/\
in_str_rev_w,/*�����ı�W*/\
split_w,/*�ָ��ı�W*/\
get_character_w,/*�ַ�W*/\
get_char_code_w,/*ȡ����W*/\
ltrim_w,/*ɾ�տ�W*/\
rtrim_w,/*ɾβ��W*/\
trim_w,/*ɾ��β��W*/\
trim_all_w,/*ɾȫ����W*/\
left_text_t,/*ȡ�ı����W*/\
right_text_w,/*ȡ�ı��ұ�W*/\
mid_text_w,/*ȡ�ı��м�W*/\
open_console,/*�򿪿���̨*/\
set_console_title,/*�ÿ���̨����W*/\
read_console_w,/*��׼����W*/\
cout_w,/*��׼���W*/\
get_console_locale,/*ȡ����̨����*/\
set_console_locale,/*�ÿ���̨����*/\
get_net_work_types_w,/*ö����������W*/\
Server_GetData,/*ȡ������*/\
Server_GetCline,/*ȡ�ؿͻ�*/\
Server_CloseClient,/*�Ͽ��ͻ�*/\
Server_start,/*����*/\
Server_close,/*�ر�*/\
Server_is_open,/*�Ƿ�������*/\
Server_Send,/*��������*/\
Clinet_connect,/*����*/\
Clinet_close,/*�ر�*/\
Clinet_GetData,/*ȡ������*/\
Clinet_Send,/*��������*/\
EditBoxW_AddText,/*�༭��.�����ı�*/\
clear_folder_W,/*���̲��������Ŀ¼*/\
create_dir_W,/*����Ŀ¼W*/\
remove_dir_W,/*ɾ��Ŀ¼W*/\
remove_file_W,/*ɾ���ļ�W*/\
current_path_W,/*ȡ����Ŀ¼W*/\
get_complete_path_W,/*ȡ����·��W*/\
normalization_path_W,/*�淶��·��W*/\
get_exe_name_W,/*ȡִ���ļ���W*/\
get_cmdline_W,/*ȡ������W*/\
get_env_W,/*����������W*/\
set_env_W,/*д��������W*/\
get_special_folder_path_W,/*ȡ�ض�Ŀ¼W*/\
prevent_duplicate_execution_W,/*��ֹ�����ظ�����W*/\
atomic_increment,/*ԭ�ӵ���*/\
atomic_exchange,/*ԭ�Ӹ�ֵ*/\
atomic_decrement,/*ԭ�ӵݼ�*/\
atomic_addition,/*ԭ������*/\
create_coroutine,/*����Э��*/\
switch_coroutine,/*�л�Э��*/\
is_valid_hCoprocess,/*Э�̾���Ƿ���Ч*/\
delete_coroutine,/*����Э��*/\
get_h_coroutine,/*ȡ��ǰ��������Э��*/\
free_console,/*�������̨*/\
get_ip_this,/*ȡ����ip*/\
kill_process,/*��������*/\
is_64_bit,/*�Ƿ�λ64λϵͳ*/\
Media_structure,/*ý�岥�Ź���*/\
Media_copy,/*ý�岥�Ÿ���*/\
Media_destruct,/*ý�岥������*/\
Media_open,/*ý�岥�Ŵ�*/\
Media_IsVideo,/*�Ƿ�Ϊ��Ƶ*/\
Media_GetHwnd,/*ȡ���*/\
Media_SetHwnd,/*�þ��*/\
Media_GetMode,/*ȡ״̬*/\
Media_GetLength,/*ȡ����*/\
Media_GetPosition,/*ȡλ��*/\
Media_GetTotalSec,/*ȡ��ʱ��*/\
Media_GetFrameRate,/*ȡ����*/\
Media_Play,/*����*/\
Media_Pause,/*��ͣ*/\
Media_Stop,/*ֹͣ*/\
Media_GetMCIAlias,/*ȡ����*/\
Media_Close,/*�ر�*/\
Media_SetVolume,/*������*/\
Media_GetVolume,/*ȡ����*/\
Media_continue,/*����*/\
Media_GetChannel,/*ȡý������*/\
Media_SetChannel,/*��ý������*/\
Media_GetFrames,/*ȡ֡��*/\
Media_GetMediaVolume,/*ȡý������*/\
Media_SetMediaVolume,/*��ý������*/\
Media_SetPlaySpeed,/*�ò����ٶ�*/\
Media_GetPlaySpeed,/*ȡ�����ٶ�*/\
Media_SetPos,/*��λ��*/\
Media_SetFrame,/*����ָ��֡*/\
Media_IsOpen,/*�Ƿ��Ѵ�*/\
is_connect_to_internet,/*�Ƿ�������*/\
check_port,/*�˿��Ƿ����*/\
send_net_msg,/*����������Ϣ*/\
EditBoxW_GetHwnd/*�༭��ȡ�༭����*/\

#pragma endregion





#pragma region ������ӵ���
#define ALL_E_LIB_CLASS \
buttonw,/*��ťW*/\
editboxw,/*�༭��W*/\
severex,/*������Ex*/\
clinetex,/*�ͻ���Ex*/\
hCoprocessD,/*Э�̾��*/\
media_play_ex,/*ý�岥��*/\
TaskDialog_Button_Type,/*�߼���Ϣ��ť*/\
UpDown/*������*/\

#pragma endregion

