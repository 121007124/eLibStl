#include<windows.h>
#include<CommCtrl.h>
namespace elibstl {
	HWND g_hDebugEdit = NULL;/*ԭ����Կ�*/
	HWND g_hDebugEditW = NULL;/*���Կ�*/
	HWND g_hPTab = NULL;/*���Կ�ѡ���*/
	WNDPROC g_Old_WndTab = nullptr;/*ѡ���ԭ�ص�*/
	WNDPROC g_Old_WndEdit = nullptr;/*ѡ���ԭ�ص�*/
	/*��ȡ��ǰ���̵������Դ��ھ��*/

	/*BOOL CALLBACK EnumWindowsProc_ENewFrame(HWND hwnd, LPARAM lParam) {
		struct EFINDWND
		{
			DWORD m_hProcessID;
			HWND m_RetWnd;
		};
		EFINDWND* data = reinterpret_cast<EFINDWND*>(lParam);
		DWORD dwProcessID = NULL;
		GetWindowThreadProcessId(hwnd, &dwProcessID);
		WCHAR szClassName[MAX_PATH + 1]{ 0 };
		GetClassNameW(hwnd, szClassName, MAX_PATH);

		if (dwProcessID == data->m_hProcessID && wcscmp(szClassName, L"ENewFrame") == 0) {
			data->m_RetWnd = hwnd;
			return false;
		}
		else
			return true;
	}*/

	static HWND GetThisWindow()
	{
		HWND hMainWnd = GetForegroundWindow(); // ��ȡ��ǰǰ̨���ڵľ��
		DWORD dwCurrentProcessId = GetCurrentProcessId(); // ��ȡ��ǰ����ID
		DWORD dwWindowProcessId;
		do {
			dwWindowProcessId = 0;
			GetWindowThreadProcessId(hMainWnd, &dwWindowProcessId);
			if (dwWindowProcessId == dwCurrentProcessId) {
				// �ҵ���ǰ���̵������ڶ�������
				break;
			}
			hMainWnd = GetParent(hMainWnd); // ��ȡ������
		} while (hMainWnd != NULL);
		return hMainWnd;

		/*auto hProcessID = GetCurrentProcessId();
		if (hProcessID == NULL)
			return NULL;
		struct EFINDWND
		{
			DWORD m_hProcessID;
			HWND m_RetWnd;
			EFINDWND(DWORD hProcessID, HWND RetWnd) {
				m_hProcessID = hProcessID;
				m_RetWnd = RetWnd;
			}
		};
		EFINDWND eFindWindow(hProcessID, NULL);
		if (EnumWindows(&EnumWindowsProc_ENewFrame, reinterpret_cast<LPARAM> (&eFindWindow))) {
			return NULL;
		}
		else
		{
			return eFindWindow.m_RetWnd;
		}*/
	}

	static HWND GetEdebugEdit() {
		auto hEWND = GetThisWindow();
		if (hEWND == NULL)
		{
			return NULL;
		}
		auto hWnd = FindWindowExW(hEWND, 0, L"AfxControlBar42s", L"״̬��");
		if (hWnd == NULL)
		{
			return NULL;
		}
		hWnd = GetDlgItem(hWnd, 130);
		if (hWnd == NULL)
		{
			return NULL;
		}
		hWnd = FindWindowExW(hWnd, 0, L"#32770", NULL);
		if (hWnd == NULL)
		{
			return NULL;
		}
		hWnd = FindWindowExW(hWnd, 0, L"SysTabControl32", NULL);
		if (hWnd == NULL)
		{
			return NULL;
		}
		return GetDlgItem(hWnd, 1011);
	}
	//HHOOK g_hookHandle = NULL;  // ȫ�ֹ��Ӿ��

	//LRESULT CALLBACK EditHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//	if (nCode >= 0) {
	//		// ����������Ϣ
	//		MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	//		if (pMsg->message == WM_KEYDOWN) {
	//			// ������̰�����Ϣ
	//			if (pMsg->wParam == VK_RETURN) {
	//				// �û����»س���
	//			}
	//		}
	//	}
	//	// �������ݹ�����Ϣ����һ�����ӻ�Ŀ�괰��
	//	return CallNextHookEx(g_hookHandle, nCode, wParam, lParam);
	//}
	///*Hook���Կ�*/
	//void SetEditHook(HWND hWndEdit) {
	//	// ж��֮ǰ�Ĺ���
	//	UnhookWindowsHookEx(g_hookHandle);
	//	// �����µĹ���
	//	DWORD dwThreadId = GetWindowThreadProcessId(hWndEdit, NULL);
	//	g_hookHandle = SetWindowsHookEx(WH_GETMESSAGE, EditHookProc, NULL, dwThreadId);
	//	if (g_hookHandle == NULL) {
	//	}
	//}
	///*UnHook���Կ�*/
	//void UnsetEditHook() {
	//	// ж�ع���
	//	if (g_hookHandle != NULL) {
	//		UnhookWindowsHookEx(g_hookHandle);
	//		g_hookHandle = NULL;
	//	}
	//}


	/*ѡ��лص�*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
		if (Msg == WM_NOTIFY) {
			HWND hwndFrom = reinterpret_cast<LPNMHDR>(lParam)->hwndFrom;
			/*�����Դ����Ϊ���Կ��ѡ�*/
			if (hwndFrom == g_hPTab) {
				UINT code = reinterpret_cast<LPNMHDR>(lParam)->code;
				if (code == TCN_SELCHANGE) {
					auto currentTab = SendMessageW(hwndFrom, TCM_GETCURSEL, 0, 0);
					if (currentTab != 1) {

					}
					else {

					}
				}
			}
		}
		/*�κ�����¶�������ԭ����Կ���ʾ*/
		ShowWindow(g_hDebugEdit, SW_HIDE);
		return CallWindowProcA(g_Old_WndTab, hWnd, Msg, wParam, lParam);

	}

	static LRESULT CALLBACK WndEditProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
		/*ת�������Կ�W*/
		SendMessageW(g_hDebugEditW, Msg, wParam, lParam);
		return CallWindowProcA(g_Old_WndTab, hWnd, Msg, wParam, lParam);

	}
	void MakeDebugEdit() {
		g_hDebugEdit = GetEdebugEdit();
		if (g_hDebugEdit == NULL)
		{
			//MessageBoxW(0, L"�޷���ȡ���Ա༭������Unicode���Կ��ʼ��ʧ�ܣ�", L"��׼ģ�����ʾ:", MB_ICONERROR);
			return;
		}
		// ��ȡԭ��༭����ı�����
		int textLength = GetWindowTextLengthW(g_hDebugEdit);
		if (textLength == 0) {
			return;
		}
		// ��ȡԭ��༭����ı�����
		wchar_t* buffer = new wchar_t[textLength + 1];
		GetWindowTextW(g_hDebugEdit, buffer, textLength + 1);
		// ��ȡԭ��༭���λ�úʹ�С
		RECT rect;
		if (!GetWindowRect(g_hDebugEdit, &rect)) {
			return;
		}

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		int x = rect.left;
		int y = rect.top;

		// ��ȡԭ��༭��ķ��
		LONG_PTR style = GetWindowLongPtrW(g_hDebugEdit, GWL_STYLE);
		DWORD dwStyle = static_cast<DWORD>(style);

		//��ȡԭ��ѡ��ľ��
		auto hParent = GetParent(g_hDebugEdit);

		/*ѡ������໯*/
		if (!g_Old_WndTab)
		{

			g_Old_WndTab = (WNDPROC)SetWindowLongPtrW(GetParent(hParent), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
			if (g_Old_WndTab)
			{
				/*�ɹ���ѡ�������ȫ�ֱ���*/
				g_hPTab = hParent;
			}

		}

		/*���໯ԭ���Կ�*/
		if (!g_Old_WndEdit)
		{
			g_Old_WndEdit = (WNDPROC)SetWindowLongPtrW(g_hDebugEdit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndEditProc));

		}

		// �����µ�Unicode�༭��
		HWND hUnicodeEdit = CreateWindowExW(0, L"EDIT", buffer, dwStyle, x, y, width, height, hParent, NULL, NULL, NULL);
		if (hUnicodeEdit == NULL) {
			delete[] buffer;
			return;
		}
		ShowWindow(g_hDebugEdit, SW_HIDE);

		delete[] buffer;

		return;
	}
}