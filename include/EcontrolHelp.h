//���������ͨ�ú��������ͷ�ļ�
#include"ElibHelp.h"
#include <CommCtrl.h>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


std::vector<unsigned char> GetDataFromHBIT(HBITMAP hBitmap);
std::wstring MyInputBox(std::wstring  title);
//ת����Ϣ���Ӵ���
bool ForwardMessageToComponent(HWND componentHandle, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
//���໯���ڲ���������ɻص�ȷ���������ڲ����ظ����໯
WNDPROC SubclassParent(HWND hwnd, WNDPROC newProc);

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