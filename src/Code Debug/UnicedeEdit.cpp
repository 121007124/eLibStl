#include<windows.h>
#include<CommCtrl.h>
#include<Richedit.h>
#include<string>
namespace elibstl {
	/*�����Ե��Կ�*/

	class RichEdit
	{
	public:
		void Create(HWND parentWnd, const WCHAR* WindowName, int x, int y, int width, int height, DWORD dwStyle)
		{
			if (!m_isInit)
			{
				return;

			}
			DWORD dwDefStyle = ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_TABSTOP
				| WS_HSCROLL | ES_NOHIDESEL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
			dwStyle |= dwDefStyle;

			// ���� RichEdit �ؼ�
			m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, WindowName, dwStyle,
				x, y, width, height, parentWnd, NULL, GetModuleHandle(NULL), NULL);
			if (!m_hWnd)
			{
				OutputDebugStringW(std::to_wstring(GetLastError()).c_str());
			}
		}
		RichEdit() {
			/*���� 2.0
				NOTE:  MSFTEDIT.DLL only registers MSFTEDIT_CLASS.
				*/
			HMODULE hRichEditDLL = LoadLibrary(L"MSFTEDIT.DLL");
			if (hRichEditDLL != NULL)
			{
				m_isInit = true;
			}
		}
		~RichEdit()
		{
			if (m_hWnd != NULL)
			{
				if (IsWindow(m_hWnd)) {
					/*��ֹ�Ѿ�������*/
					DestroyWindow(m_hWnd);
				};

				m_hWnd = NULL;
			}
		}
		void SetText(const wchar_t* text)  const
		{
			SendMessageW(m_hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
		}
		void Move(int left, int top, int width, int height) const
		{
			MoveWindow(m_hWnd, left, top, width, height, TRUE);
		}
		std::wstring GetText()  const
		{
			int length = GetWindowTextLengthW(m_hWnd) + 1;
			std::wstring buffer(length, L'\0');
			GetWindowTextW(m_hWnd, &buffer[0], length);
			return buffer;
		}

		void ClearText()  const
		{
			SetWindowTextW(m_hWnd, L"");
		}
		/**/
		void AppendText(const wchar_t* text) const
		{
			int length = GetWindowTextLengthW(m_hWnd);
			SendMessageW(m_hWnd, EM_SETSEL, length, length);
			SendMessageW(m_hWnd, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
		}


		void AppendText(int codepage, const char* pstr, CHARFORMAT2W* characterFormat)
		{

			HWND focus = GetFocus();
			if (focus != m_hWnd)
			{
				SetFocus(m_hWnd);
			}


			typedef struct _settextex {
				DWORD flags;
				UINT  codepage;
			} SETTEXTEX;
			SETTEXTEX pSetTextEx{ ST_SELECTION ,codepage };

			SendMessageW(m_hWnd, EM_SETSEL, -2, -1);
			int old_pos = SendMessageW(m_hWnd, EM_GETSEL, 0, 0);
			int  new_pos = -1;

			SendMessageW(m_hWnd, EM_SETTEXTEX, reinterpret_cast<WPARAM>(&pSetTextEx), reinterpret_cast<LPARAM>(pstr));

			if (characterFormat)
			{
				SetSelection(old_pos, new_pos);
				SetSelectionCharacterFormat(*characterFormat);
			}

			if (focus != m_hWnd && focus != NULL)
			{
				SetFocus(focus);
			}
		}



		void AppendText(int codepage, const char* pstr)
		{

			HWND focus = GetFocus();
			if (focus != m_hWnd)
			{
				SetFocus(m_hWnd);
			}
			typedef struct _settextex {
				DWORD flags;
				UINT  codepage;
			} SETTEXTEX;
			SETTEXTEX pSetTextEx{ ST_SELECTION ,codepage };

			SendMessageW(m_hWnd, EM_SETSEL, -2, -1);
			int old_pos = SendMessageW(m_hWnd, EM_GETSEL, 0, 0);
			int  new_pos = -1;

			SendMessageW(m_hWnd, EM_SETTEXTEX, reinterpret_cast<WPARAM>(&pSetTextEx), reinterpret_cast<LPARAM>(pstr));
			if (focus != m_hWnd && focus != NULL)
			{
				SetFocus(focus);
			}
		}
	private:
		/*��ѡ�������ʽ*/
		bool SetSelectionCharacterFormat(const CHARFORMAT2W& format)
		{
			CHARFORMAT2W cf;
			cf.cbSize = sizeof(CHARFORMAT2W);
			cf.dwMask = CFM_FACE | CFM_EFFECTS | CFM_SIZE | CFM_OFFSET | CFM_COLOR | CFM_CHARSET;
			auto len = lstrlenW(format.szFaceName);
			if (len > LF_FACESIZE - 1)
			{
				len = LF_FACESIZE - 1;
			}
			wcsncpy_s(cf.szFaceName, format.szFaceName, len);
			cf.szFaceName[len] = L'\0';
			cf.dwEffects = format.dwEffects & ~CFE_AUTOCOLOR;
			cf.yHeight = format.yHeight;
			cf.yOffset = format.yOffset;
			cf.crTextColor = format.crTextColor;
			cf.bCharSet = format.bCharSet;
			cf.bPitchAndFamily = format.bPitchAndFamily;
			return SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&cf)) != 0;
		}
		/*��ѡ������*/
		bool SetSelection(int startPosition, int endPosition)
		{
			CHARRANGE range;
			range.cpMin = startPosition;
			range.cpMax = endPosition;
			return SendMessageW(m_hWnd, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range)) != 0;
		}
		HWND m_hWnd = NULL;
		bool m_isInit = false;
	};



	class EDebugEdit
	{
		RichEdit m_hRicEdit{};
		/*������IDE���*/
		HWND m_hEplIDE = NULL;
		/*ѡ�񿨾����Ҫ�����ڿ����л��ǵ����غ���ʾ*/
		HWND m_hTab = NULL;;
		/*ѡ��о��*/
		HWND m_hTabControl = NULL;
		/*���Կ���*/
		HWND m_hDebugBox = NULL;
		/*�Ƿ��ʼ���ɹ�*/
		bool m_isCreate = false;

		/*************************����Ϊ�ص�*************************/
		/*ԭѡ��ص��������軹ԭ*/
		WNDPROC m_WndOldProcTab = nullptr;
		/*ԭѡ��лص��������軹ԭ*/
		WNDPROC m_WndOldProcTabControl = nullptr;
		/*ԭ���Կ�ص��������軹ԭ*/
		WNDPROC m_WndOldProcDebugBox = nullptr;


	public:
		EDebugEdit() {
			/*��ʼ��ʧ��*/
			m_isCreate = GetEplDebugOutputWindow();
			if (!m_isCreate) {
				return;
			};
			GetOldDataAndMakeNewWnd();
			SubDebugBoxClass();
		};
		~EDebugEdit() {
			/*����ԭ����*/
			ReturnOldProc();

		};

	private:
		void GetOldDataAndMakeNewWnd() {
			/*��ȡԭ����*/
			/*ԭʼ���*/
			auto dwStyle = GetWindowLongPtrW(m_hDebugBox, GWL_STYLE);
			/*��������*/
			wchar_t szClassName[MAX_PATH + 1]{ 0 };
			GetClassNameW(m_hDebugBox, szClassName, MAX_PATH);
			/*����*/
			auto hFontA = SendMessageW(m_hDebugBox, WM_GETFONT, 0, 0);
			/*����λ��*/
			RECT  rect;
			GetWindowRect(m_hDebugBox, &rect);
			/*ȡ�ͻ�ȡ����*/
			POINT point = { rect.left ,rect.top };
			ScreenToClient(m_hTabControl, &point);
			/*ȡԭ����*/
			int WindowNameW_len = GetWindowTextLengthW(m_hDebugBox);
			std::wstring WindowNameW(WindowNameW_len, L'\0');
			GetWindowTextW(m_hDebugBox, &WindowNameW[0], WindowNameW_len + 1);

			m_hRicEdit.Create(m_hTabControl, WindowNameW.c_str(), point.x, point.y, rect.right - rect.left, rect.bottom - rect.top, dwStyle);

		}
		/*����ԭ����*/
		void ReturnOldProc() {
			if (m_WndOldProcTab)
			{
				SetWindowLongPtrW(m_hTab, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WndOldProcTab));
				m_WndOldProcTab = nullptr;
			}

			if (m_WndOldProcTabControl)
			{
				SetWindowLongPtrW(m_hTabControl, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WndOldProcTabControl));
				m_WndOldProcTabControl = nullptr;
			}

			if (m_WndOldProcDebugBox)
			{
				SetWindowLongPtrW(m_hDebugBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WndOldProcDebugBox));
				m_WndOldProcDebugBox = nullptr;
			}
		}

		/*��ȡ������IDE���ھ��*/
		HWND GetEplIDEWindowHandle()
		{
			if (m_hEplIDE)
				return m_hEplIDE;

			auto hProcessID = GetCurrentProcessId();
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
			return eFindWindow.m_RetWnd;
		}
		/*��ȡ�����Ե���������ھ��*/
		bool GetEplDebugOutputWindow() {

			m_hEplIDE = GetEplIDEWindowHandle();
			if (m_hEplIDE == NULL)
			{
				return false;
			}
			auto Temp_hWnd = FindWindowExW(m_hEplIDE, 0, L"AfxControlBar42s", L"״̬��");
			if (Temp_hWnd == NULL)
			{
				return NULL;
			}
			Temp_hWnd = GetDlgItem(Temp_hWnd, 130);
			if (Temp_hWnd == NULL)
			{
				return false;
			}
			m_hTab = FindWindowExW(Temp_hWnd, 0, L"#32770", NULL);
			if (m_hTab == NULL)
			{
				return false;
			}
			/*Ѱ��ѡ���*/
			m_hTabControl = FindWindowExW(m_hTab, 0, L"SysTabControl32", NULL);
			if (m_hTabControl == NULL)
			{
				return false;
			}
			/*ԭ���������*/
			m_hDebugBox = GetDlgItem(m_hTabControl, 1011);
			return true;

		}
		/*���໯,��ȡ���ݵ�*/
		void SubDebugBoxClass() {
			/*ѡ��а󶨶���*/
			SetWindowLongPtrW(m_hTab, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			/*���໯ѡ��,���ڿ���ѡ��е��л�*/
			m_WndOldProcTab = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_hTab, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProcTab)));

			/*ԭ���Կ�󶨶���*/
			SetWindowLongPtrW(m_hDebugBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			/*���໯ԭ���Կ�*/
			m_WndOldProcDebugBox = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_hDebugBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProcDebugBox)));


		}


	private:
		/*ö���״���*/
		static BOOL CALLBACK EnumWindowsProc_ENewFrame(HWND hwnd, LPARAM lParam) {
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
				return FALSE;
			}
			else
				return TRUE;
		}
		/*ѡ��лص�*/
		static LRESULT CALLBACK WndProcTab(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
			auto now = reinterpret_cast<EDebugEdit*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

			if (!now)
				return DefWindowProcW(hWnd, Msg, wParam, lParam);

			//if (Msg == WM_NOTIFY) {
			//	HWND hwndFrom = reinterpret_cast<LPNMHDR>(lParam)->hwndFrom;
			//	/*�����Դ����Ϊ���Կ��ѡ�*/
			//	if (hwndFrom == now->m_hTabControl) {
			//		UINT code = reinterpret_cast<LPNMHDR>(lParam)->code;
			//		if (code == TCN_SELCHANGE) {
			//			auto currentTab = SendMessageW(hwndFrom, TCM_GETCURSEL, 0, 0);
			//			if (currentTab != 1) {

			//			}
			//			else {

			//			}
			//		}
			//	}
			//}
			/*�κ�����¶�������ԭ����Կ���ʾ*/
			ShowWindow(now->m_hDebugBox, SW_HIDE);
			return CallWindowProcW(now->m_WndOldProcTab, hWnd, Msg, wParam, lParam);

		}

		/*ԭ���Կ�ص�*/
		static LRESULT CALLBACK WndProcDebugBox(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
			auto now = reinterpret_cast<EDebugEdit*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

			if (!now)
				return DefWindowProcW(hWnd, Msg, wParam, lParam);
			switch (Msg)
			{
			case WM_WINDOWPOSCHANGED: {

				WINDOWPOS* windowPos = reinterpret_cast<WINDOWPOS*>(lParam);
				//����Ƿ�ı�
				if ((windowPos->flags & SWP_NOSIZE) == 0 || (windowPos->flags & SWP_NOMOVE) == 0)
				{
					now->m_hRicEdit.Move(windowPos->x, windowPos->y, windowPos->cx, windowPos->cy);
				}

			}break;
			case WM_SETTEXT: {
				/*����ȡ����ȫ��ansi����Ϊ���еĵ��������Ϊansi*/
				const char* text = reinterpret_cast<const char*>(lParam);
				if (text && strlen(text))
				{
					now->m_hRicEdit.AppendText(936, "\n>>>>>>>  ��ֹ���  >>>>>>>  \n");
				}

			}break;
			case EM_REPLACESEL: {
				if (lParam == NULL)
					break;
				const char* text = reinterpret_cast<const char*>(lParam);
				if (text && strlen(text))
				{
					OutputDebugStringA(text);
					now->m_hRicEdit.AppendText(936, text);
				}


			}break;
			default:

				break;
			}
			return CallWindowProcW(now->m_WndOldProcDebugBox, hWnd, Msg, wParam, lParam);

		}
	};





	/*��ȡ��ǰ���̵������Դ��ھ��*/

	/*��ȡdebugput�༭��*/





	//static LRESULT CALLBACK WndEditProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	//	/*ת�������Կ�W*/
	//	SendMessageW(g_hDebugEditW, Msg, wParam, lParam);
	//	return CallWindowProcA(g_Old_WndTab, hWnd, Msg, wParam, lParam);

	//}
	//void MakeDebugEdit() {
	//	g_hDebugEdit = GetEdebugEdit();
	//	if (g_hDebugEdit == NULL)
	//	{
	//		//MessageBoxW(0, L"�޷���ȡ���Ա༭������Unicode���Կ��ʼ��ʧ�ܣ�", L"��׼ģ�����ʾ:", MB_ICONERROR);
	//		return;
	//	}
	//	// ��ȡԭ��༭����ı�����
	//	int textLength = GetWindowTextLengthW(g_hDebugEdit);
	//	if (textLength == 0) {
	//		return;
	//	}
	//	// ��ȡԭ��༭����ı�����
	//	wchar_t* buffer = new wchar_t[textLength + 1];
	//	GetWindowTextW(g_hDebugEdit, buffer, textLength + 1);
	//	// ��ȡԭ��༭���λ�úʹ�С
	//	RECT rect;
	//	if (!GetWindowRect(g_hDebugEdit, &rect)) {
	//		return;
	//	}

	//	int width = rect.right - rect.left;
	//	int height = rect.bottom - rect.top;
	//	int x = rect.left;
	//	int y = rect.top;

	//	// ��ȡԭ��༭��ķ��
	//	LONG_PTR style = GetWindowLongPtrW(g_hDebugEdit, GWL_STYLE);
	//	DWORD dwStyle = static_cast<DWORD>(style);

	//	//��ȡԭ��ѡ��ľ��
	//	auto hParent = GetParent(g_hDebugEdit);

	//	/*ѡ������໯*/
	//	if (!g_Old_WndTab)
	//	{

	//		g_Old_WndTab = (WNDPROC)SetWindowLongPtrW(GetParent(hParent), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
	//		if (g_Old_WndTab)
	//		{
	//			/*�ɹ���ѡ�������ȫ�ֱ���*/
	//			g_hPTab = hParent;
	//		}

	//	}

	//	/*���໯ԭ���Կ�*/
	//	if (!g_Old_WndEdit)
	//	{
	//		g_Old_WndEdit = (WNDPROC)SetWindowLongPtrW(g_hDebugEdit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndEditProc));

	//	}

	//	// �����µ�Unicode�༭��
	//	HWND hUnicodeEdit = CreateWindowExW(0, L"EDIT", buffer, dwStyle, x, y, width, height, hParent, NULL, NULL, NULL);
	//	if (hUnicodeEdit == NULL) {
	//		delete[] buffer;
	//		return;
	//	}
	//	ShowWindow(g_hDebugEdit, SW_HIDE);

	//	delete[] buffer;

	//	return;
	//}



	EDebugEdit& GetRichEdit() {
		/*�����ӳٳ�ʼ��*/
		static EDebugEdit instance;
		return instance;

	}


	void Edebug_init() {
		GetRichEdit();
	}
}