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
Fn_EditAddText,/*�༭��.�����ı�*/\
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
Fn_EditCharFromPos,/*�༭��.ȡ���괦�ַ�*/\
e_coroutine_close,/*�ͷ�Э�̵�����*/\
e_coroutine_open,/*����Э�̵�����*/\
e_hCoi2h,/*ָ�뵽������*/\
analog_press,/*ģ�ⰴ��*/\
analog_up,/*ģ�ⵯ��*/\
replace_substring_w,/*ȡCPU��ǰʹ����*/\
get_cpu_id,/*��ȡCPU���к�*/\
network_post/*��ҳ����*/\
,capitalize_first_letter/*����ĸ��д*/\
,Fn_BtnGetIdealSize/*��ť.ȡ����ߴ�*/\
,Fn_UpDownSetAccel/*������.�ü��ٶ�*/\
,e_debugput/*����*/\
,e_CallEfun/*ִ�к���*/\
,_e_delay/*�ӳ�Ex*/\
,e_GetTickCount_ex/*ȡ����ʱ��Ex*/\
,e_eLibCallEfun/*�����׿⺯��*/\
,e_Open_File_Dialog_A/*�ļ�ѡ���*/\
,e_Open_File_Dialog_W/*�ļ�ѡ���W*/\
,e_BrowseForFolder_W/*�ļ������W*/\
,e_BrowseForFolder_A/*�ļ������*/\
,e_lib_CreateEbds/*�ļ������*/\
,edbs_ex_structure/*EDBS����*/\
,edbs_ex_copy/*EDB����*/\
,edbs_ex_destruct/*����*/\
,edbs_ex_open/*��*/\
,edbs_get_row_num/*ȡ��¼��*/\
,edbs_ex_addpendnop/*�ӿռ�¼*/\
,edbs_set_current/*����*/\
,edbs_read/*��*/\
,edbs_write/*д*/\
, edbs_close/*�ر�*/\
,edbs_next/*EDB.��һ��*/\
,edbs_previous/*EDB.��һ��*/\
,edbs_clean/*EDB.���*/\
,edbs_begin/*EDB.��������*/\
,edbs_commit/*EDB.�ύ����*/\
,edbs_get_current/*EDB.��ǰ��*/\
,edbs_get_column_name/*EDB.�ֶ���*/\
,edbs_get_column_num/*EDB.�ֶ���*/\
,edbs_set_column_name/*EDB.���ֶ���*/\
,Fn_EditCanUndo/*�༭��.�Ƿ�ɳ���*/\
,Fn_EditEmptyUndoBuf/*�༭��.��ճ�������*/\
,Fn_EditGetFirstLine/*�༭��.ȡ��һ�ɼ���*/\
,Fn_EditGetLineCount/*�༭��.ȡ����*/\
,Fn_EditGetModify/*�༭��.ȡ�޸ı�־*/\
,Fn_EditHideBallloonTip/*�༭��.����������ʾ*/\
,Fn_EditLineLength/*�༭��.ȡĳ�г���*/\
,Fn_EditGetLine/*�༭��.ȡ���ı�*/\
,Fn_EditGetMargins/*�༭��.ȡ�߾�*/\
,Fn_EditGetRect/*�༭��.ȡ��ʾ����*/\
,Fn_EditLineFromChar/*�༭��.�ַ�λ�õ�����*/\
,Fn_EditLineIndex/*�༭��.ȡĳ�е�һ�ַ�λ��*/\
,Fn_EditScroll/*�༭��.����*/\
,Fn_EditLineScroll/*�༭��.������*/\
,Fn_EditPosFromChar/*�༭��.ȡ�ַ�����*/\
,Fn_EditReplaceSel/*�༭��.�滻ѡ���ı�*/\
,Fn_EditSetMargins/*�༭��.�ñ߾�*/\
,Fn_EditSetModify/*�༭��.���޸ı�־*/\
,Fn_EditSetRect/*�༭��.����ʾ����*/\
,Fn_EditSetTabStop/*�༭��.���Ʊ�λ*/\
,Fn_EditSetBallloonTip/*�༭��.����������ʾ*/\
,Fn_EditUndo/*�༭��.����*/\
,Fn_EditPaste/*�༭��.ճ��*/\
,Fn_EditCopy/*�༭��.����*/\
,Fn_EditSelAll/*�༭��.ȫѡ*/\
,eto_wstring/*���ı�W*/\
,e_malloc/*�ڴ�����*/\
,e_free/*�ڴ��ͷ�*/\
,clearConsole/*��տ���̨*/\
,e_WriteMem/*д���ڴ�*/\
,Fn_UpDownGetAccel/*������.ȡ���ٶ�*/\
,g_trim_leading_zeros/*ɾ�ı������Ч��*/\





//,ansiwindow_to_unicodewindow/*����unicode֧��*/



#pragma endregion





#pragma region ������ӵ���
#define ALL_E_LIB_CLASS \
CtButtonW,/*��ťW*/\
CtEdit,/*�༭��W*/\
severex,/*������Ex*/\
clinetex,/*�ͻ���Ex*/\
hCoprocessD,/*Э�̾��*/\
media_play_ex,/*ý�岥��*/\
TaskDialog_Button_Type,/*�߼���Ϣ��ť*/\
UpDown,/*������*/\
hCoroutine,/*Э�̵�����*/\
edbs_ex,/*edbs*/\
CtCheckButtonW,/*ѡ���*/\
CtCommandLink/*��������*/\

#pragma endregion

