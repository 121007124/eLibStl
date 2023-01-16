#include"ElibHelp.h"
#include<commctrl.h>
#pragma comment(lib, "comctl32.lib")


static ARG_INFO Args[] =
{
	{
		/*name*/    "��ʾ��Ϣ",
		/*explain*/ ("�Խ�С������ʾ����ָ���·�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   0,
	},
	{
		/*name*/    "��ʾ����",
		/*explain*/ ("������ʾ����ָ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "��ť",
		/*explain*/ ("��ͨ����ͬλ����ϲ�ͬ��ť��Ĭ��Ϊȷ��,1.ȷ��,2.��,4.��,8.ȡ��(����Ϊ�˶Ի���ָ���˰�ť��������Ӧ alt-F4 ��ת��) �ĵ���ȡ��������),16.����,32.�ر�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "���ڱ���",
		/*explain*/ ("������ʾ�ı���"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "������",
		/*explain*/ ("�����ھ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "ͼ��",
		/*explain*/ ("����ʾͼ��,Ĭ��Ϊ:��ȷ,0.��,1.����Ի����л���ʾһ������������ͼ��.2.��ԲȦ�е�Сд��ĸ i ��ɵ�ͼ����ʾ������Ի����С�3.����Ի����л���ʾһ����ȫ����ͼ�ꡣ4.����Ի����л���ʾ��̾��ͼ�ꡣĬ��Ϊ1��5.��ȷ��6.����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	}
};


EXTERN_C void Fn_TaskDialogW(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	const PWSTR icon[] = { nullptr,TD_ERROR_ICON,TD_INFORMATION_ICON,TD_SHIELD_ICON,TD_WARNING_ICON , MAKEINTRESOURCEW(-8),MAKEINTRESOURCEW(-7) };
	const std::wstring_view&
		lpContent = elibkrnln::args_to_wsdata(pArgInf, 0),
		lpTitle = elibkrnln::args_to_wsdata(pArgInf, 1),
		lpWindowTitle = elibkrnln::args_to_wsdata(pArgInf, 3);
	auto hwndParent = elibkrnln::args_to_data<INT>(pArgInf, 4),
		standardButtons = elibkrnln::args_to_data<INT>(pArgInf, 2),
		inicon = elibkrnln::args_to_data<INT>(pArgInf, 5);
	PWSTR boxicon = MAKEINTRESOURCEW(-8);
	if (inicon.has_value() && inicon.value() > 0 && inicon.value() <= 6)
	{
		boxicon = icon[inicon.value()];
	}

	int buttonPressed;
	TaskDialog(reinterpret_cast<HWND>(hwndParent.has_value() ? hwndParent.value() : 0), NULL, !lpWindowTitle.empty() ? std::wstring(lpWindowTitle).c_str() : L"��ʾ", std::wstring(lpTitle).c_str(), std::wstring(lpContent).c_str(), standardButtons.has_value() ? standardButtons.value() : TDCBF_OK_BUTTON, boxicon, &buttonPressed);
	pRetData->m_int = buttonPressed;
}

FucInfo task_dialog_w = { {
		/*ccname*/  ("�߼���Ϣ��W"),
		/*egname*/  ("TaskDialog"),
		/*explain*/ ("0.��������ʧ��,1.��ѡ��ȷ������ť��2.��ѡ��ȡ������ť���� Alt-F4���Ѱ���ת�壬�����û������رմ��ڰ�ť��4.��ѡ�����ԡ���ť��6.��ѡ���ǡ���ť��7.δѡ���κΰ�ť��"),
		/*category*/3,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/6,
		/*arg lp*/  &Args[0],
	} ,Fn_TaskDialogW ,"Fn_TaskDialogW" };
