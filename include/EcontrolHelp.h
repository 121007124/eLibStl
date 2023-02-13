//���������ͨ�ú��������ͷ�ļ�
#include"ElibHelp.h"
#include <CommCtrl.h>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace elibstl {
	std::vector<unsigned char> GetDataFromHBIT(HBITMAP hBitmap);
	std::wstring MyInputBox(const std::wstring& title);
	//ת����Ϣ���Ӵ���
	bool ForwardMessageToComponent(HWND componentHandle, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//���໯���ڲ���������ɻص�ȷ���������ڲ����ظ����໯
	WNDPROC SubclassParent(HWND hwnd, WNDPROC newProc);
	inline
		bool SendToParentsHwnd(DWORD m_dwWinFormID, DWORD m_dwUnitID, INT uMsg, WPARAM wParam, LPARAM lParam) {

		if (uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS || uMsg == WM_MOUSELAST || uMsg >= WM_MOUSEMOVE && uMsg <= WM_RBUTTONUP || uMsg >= WM_KEYDOWN && uMsg <= WM_CHAR)
		{
			//�⼸���¼�ȫ��ת���������
			EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 0);
			INT control_type = 0;
			if (uMsg != WM_CHAR && uMsg != WM_SETFOCUS && uMsg != WM_KILLFOCUS) {
				if ((GetKeyState(VK_CONTROL) & 16384) != 0) {
					control_type = 1;
				}
				if ((GetKeyState(VK_SHIFT) & 16384) != 0) {
					control_type = control_type | 2;
				}
				if ((GetKeyState(VK_MENU) & 16384) != 0) {
					control_type = control_type | 4;
				}
			}
			if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_RBUTTONUP)
			{
				if (uMsg == WM_MOUSEMOVE)
				{
					event.m_nEventIndex = -6;
				}
				else
				{
					event.m_nEventIndex = 512 - uMsg;

				}
				event.m_nArgCount = 3;
				event.m_arg[0].m_inf.m_int = lParam & 65535;
				event.m_arg[1].m_inf.m_int = lParam >> 16;
				event.m_arg[2].m_inf.m_int = control_type;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				return true;
			}
			switch (uMsg)
			{
			case WM_SETFOCUS: {
				event.m_nEventIndex = -uMsg;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}
			case WM_KILLFOCUS: {
				event.m_nEventIndex = -uMsg;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}
			case WM_KEYDOWN: {
				event.m_nEventIndex = (247 - uMsg);
				event.m_nArgCount = 2;
				event.m_arg[0].m_inf.m_int = wParam;
				event.m_arg[1].m_inf.m_int = control_type;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}
			case WM_KEYUP: {
				event.m_nEventIndex = (247 - uMsg);
				event.m_nArgCount = 2;
				event.m_arg[0].m_inf.m_int = wParam;
				event.m_arg[1].m_inf.m_int = control_type;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}
			case WM_CHAR: {
				event.m_nEventIndex = -11;
				event.m_nArgCount = 1;
				event.m_arg[0].m_inf.m_int = wParam;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}case WM_MOUSELAST: {
				event.m_nEventIndex = 12;
				event.m_nArgCount = 2;
				event.m_arg[0].m_inf.m_int = (wParam >> 16) > 0 ? 1 : -1;
				event.m_arg[1].m_inf.m_int = control_type;
				elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
				break;
			}
			default:
				break;
			}
		}
		return true;

	}
}
typedef struct CONTROL_STRUCT_BASE
{
	WNDPROC             oldProc;        // ����ԭ����, һ��������Լ��������������Ҫ���໯, �����¼������໯��ԭ����
	HFONT               hFont;          // ����
	HWND                hDesignWnd;     // ���blInDesignModeΪ��, ��hDesignWnd����ƴ��ڵĴ��ھ��, ����Ϊ0
	HWND                hWndParent;     // ��������ھ��
	HWND                hWnd;           // ������
	DWORD               hUnit;          // �����Է��ص�һ��CWnd��ָ��
	UINT                id;             // �����ID
	DWORD               dwWinFormID;    // �����ڵ�Ԫ���ڴ��ڼ������ID, ����֪ͨ��ϵͳ
	DWORD               dwUnitID;       // �����ڵ�Ԫ���ڴ��ڼ������ID, ����֪ͨ��ϵͳ��
	BOOL                blInDesignMode; // �Ƿ�������IDE�����Խ��п��ӻ����, ����ʱΪ��
	LONG                flags;          // �����һЩ��־, �Լ�����Լ�����
	DWORD               style;          // �������ʽ, �� WM_STYLECHANGED ���޸�
	DWORD               styleEx;        // �������չ��ʽ, �� WM_STYLECHANGED ���޸�
	DWORD               styleCtl;       // ���ר����չ��ʽ
}*PCONTROL_STRUCT_BASE;

// ���ͳһ������Ϣ����, ���¼�����һЩ������Ϣ, �������base�ṹָ��


struct EFONTDATA
{
	HFONT m_hFont;
	LOGFONTA m_data;
	EFONTDATA()
	{
		memset(this, 0, sizeof(EFONTDATA));
	}
};