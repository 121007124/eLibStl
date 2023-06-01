/*
* 2023.5.19
* FIXME��Unicode���Կ�����ʱ���ã�����Ҫ������ƽṹ��439�У�
*/

#include<windows.h>
#include<CommCtrl.h>
#include<Richedit.h>
#include <OleIdl.h>
#include <RichOle.h>
#include<string>
#include<vector>
#include <sstream>
#pragma comment (lib,"Crypt32.lib")
//#include"elib\fnshare.h"
#include "ElibHelp.h"



ESTL_NAMESPACE_BEGIN
/*��Ӧ��16����תΪunicode*/
inline std::wstring UmakeW(const std::wstring& str)
{
	std::wstring wstr;

	std::wstring hexValue;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == L'\\' && str[i + 1] == L'u')
		{
			hexValue = str.substr(i + 2, 4);
			unsigned int unicodeValue = std::stoi(hexValue, nullptr, 16);
			wstr += static_cast<wchar_t>(unicodeValue);
			i += 5; // Skip the next 5 characters ('\uXXXX')
		}
		else
		{
			wstr += static_cast<wchar_t>(str[i]);
		}
	}

	return wstr;
}

inline std::vector<unsigned char> Base64ToString(const std::wstring& base64String)
{
	DWORD binaryLength = 0;

	// ��ȡ�����Ķ��������ݳ���
	if (::CryptStringToBinaryW(base64String.c_str(), base64String.length(),
		CRYPT_STRING_BASE64, nullptr, &binaryLength, nullptr, nullptr)) {
		std::vector<unsigned char> binaryData(binaryLength);

		// �� Base64 ������ַ�������Ϊ����������
		if (::CryptStringToBinaryW(base64String.c_str(), base64String.length(),
			CRYPT_STRING_BASE64, binaryData.data(), &binaryLength, nullptr, nullptr)) {
			return binaryData;
		}
	}

	// ʧ��ʱ���ؿ�����
	return std::vector<unsigned char>();
}

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
		/*| ES_AUTOHSCROLL| WS_HSCROLL*/
		DWORD dwDefStyle = ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_TABSTOP
			| ES_NOHIDESEL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | EM_PASTESPECIAL;
		//dwStyle |= dwDefStyle;
		// ���� RichEdit �ؼ�
		m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, WindowName, dwDefStyle,
			x, y, width, height, parentWnd, NULL, GetModuleHandle(NULL), NULL);
		SendMessageW(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)NULL);
		if (!m_hWnd)
		{
			OutputDebugStringW(std::to_wstring(GetLastError()).c_str());
		}
	}
	RichEdit() {
		/*���� 2.0
			NOTE:  MSFTEDIT.DLL only registers MSFTEDIT_CLASS.
			*/
		HMODULE hRichEditDLL = LoadLibraryW(L"MSFTEDIT.DLL");
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
	/*�ƶ�*/
	void Move(int left, int top, int width, int height) const
	{
		MoveWindow(m_hWnd, left, top, width, height, TRUE);
	}
	/*��ȡ�ı�*/
	std::wstring GetText()  const
	{
		int length = GetWindowTextLengthW(m_hWnd) + 1;
		std::wstring buffer(length, L'\0');
		GetWindowTextW(m_hWnd, &buffer[0], length);
		return buffer;
	}
	/*����ı�*/
	void ClearText()  const
	{
		SetWindowTextW(m_hWnd, L"");
	}
	/*׷���ı�*/
	void AppendText(const wchar_t* text) const
	{
		int length = GetWindowTextLengthW(m_hWnd);
		SendMessageW(m_hWnd, EM_SETSEL, length, length);
		SendMessageW(m_hWnd, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
	}
	/*���ͼƬ*/
	void AppendImage()
	{
		if (!m_isInit || m_hWnd == NULL)
			return;
		// �ڸ��ı����в���������е�ͼ��
		SendMessageW(m_hWnd, WM_PASTE, 0, 0);
	}

	eStlInline operator HWND()
	{
		return m_hWnd;
	}


private:
	/*��ѡ�������ʽ*/
	bool SetSelectionCharacterFormat(const CHARFORMAT2W& format)   const
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
	bool SetSelection(int startPosition, int endPosition)   const
	{
		CHARRANGE range;
		range.cpMin = startPosition;
		range.cpMax = endPosition;
		return SendMessageW(m_hWnd, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range)) != 0;
	}
	HWND m_hWnd = NULL;
	bool m_isInit = false;
};


#define TCITEM_PARAM_OUTPUTEX	20230531'01u
#define SCID_TABPARENT			20230531'02u
#define SCID_TAB				20230531'03u
#define SCID_EDITORG			20230531'04u
class EDebugEdit
{
public:
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

	int m_idxNewTab = -1;
	WNDPROC m_oldproc = nullptr;
public:
	EDebugEdit() {
		/*��ʼ��ʧ��*/
		m_isCreate = GetEplDebugOutputWindow();
		if (!m_isCreate) {
			return;
		};

		TCITEMA tci;
		tci.mask = TCIF_TEXT | TCIF_PARAM;
		tci.pszText = (PSTR)"��չ���";
		tci.lParam = TCITEM_PARAM_OUTPUTEX;
		m_idxNewTab = SendMessageA(m_hTabControl, TCM_INSERTITEMA,
			SendMessageA(m_hTabControl, TCM_GETITEMCOUNT, 0, 0), (LPARAM)&tci);

		GetOldDataAndMakeNewWnd();
		SubDebugBoxClass();
	};
	~EDebugEdit() {
		if (m_oldproc)
		{
			SetWindowLongA(m_hDebugBox, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(m_oldproc));
			RemoveWindowSubclass(m_hTab, SubclassProc_TabParent, SCID_TABPARENT);
			RemoveWindowSubclass(m_hTabControl, SubclassProc_Tab, SCID_TAB);
		}
	};

private:
	static LRESULT CALLBACK SubclassProc_TabParent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto p = (EDebugEdit*)dwRefData;
		switch (uMsg)
		{
		case WM_NOTIFY:
		{
			if (((NMHDR*)lParam)->code != TCN_SELCHANGE)
				break;

			int idxCurr = SendMessageW(p->m_hTabControl, TCM_GETCURSEL, 0, 0);
			if (idxCurr == 1)
				TaskDialog(GetActiveWindow(), NULL, L"��ʾ", L"������ʹ��ԭ����Կ�!", L"ԭ����Ϣ�򲢲�֧��unicode��ͼƬ", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);

		}
		break;
		}

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	static LRESULT CALLBACK SubclassProc_Tab(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto p = (EDebugEdit*)dwRefData;

		switch (uMsg)
		{
		case TCM_INSERTITEMA:
		case TCM_INSERTITEMW:

			break;
		case TCM_SETCURSEL:
			if (wParam == 1)
				wParam = 8;
			break;

		case TCM_DELETEITEM:

			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	static LRESULT CALLBACK SubclassProc_OrgEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto p = (EDebugEdit*)GetWindowLongA(hWnd, GWL_USERDATA);
		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS* windowPos = reinterpret_cast<WINDOWPOS*>(lParam);
			//����Ƿ�ı�
			if ((windowPos->flags & SWP_NOSIZE) == 0 || (windowPos->flags & SWP_NOMOVE) == 0)
			{
				p->m_hRicEdit.Move(windowPos->x, windowPos->y, windowPos->cx, windowPos->cy);
			}

		}
		break;
		case EM_REPLACESEL:
		{
			if (lParam == NULL)
				break;
			auto text = elibstl::A2W(reinterpret_cast<const CHAR*>(lParam));
			if (text && wcslen(text))//�����ж�
			{

				auto  is_img = [](const wchar_t* str1) {
					const wchar_t* img = L"* [elibstl::image]";
					while (*str1 && *img && (*str1 == *img)) {
						++str1;
						++img;
					}
					return (*img == L'\0'); // �ж��Ƿ� str2 �Ѿ���������
				};
				if (is_img(text))
				{
					auto img = Base64ToString(std::wstring(text).substr(sizeof("* [elibstl::image]") - sizeof('\0')));
					p->m_hRicEdit.AppendText(L"* [ͼƬ:]");
					p->m_hRicEdit.AppendImage();

					p->m_hRicEdit.AppendText(L"\r\n");
				}
				else
					p->m_hRicEdit.AppendText(UmakeW(text).c_str());
			}
			if (text)
				delete[]text;
		}
		break;
		}
		return CallWindowProcW(p->m_oldproc, hWnd, uMsg, wParam, lParam);
	}

	void GetOldDataAndMakeNewWnd()
	{
		/*����λ��*/
		RECT  rect;
		GetWindowRect(m_hDebugBox, &rect);
		/*ȡ�ͻ�ȡ����*/
		POINT point = { rect.left ,rect.top };
		ScreenToClient(m_hTabControl, &point);
		/*ȡԭ����*/
		int cchText = GetWindowTextLengthW(m_hDebugBox);
		std::wstring sText(cchText, L'\0');
		GetWindowTextW(m_hDebugBox, &sText[0], cchText + 1);

		m_hRicEdit.Create(m_hTabControl, sText.c_str(),
			point.x, point.y, rect.right - rect.left, rect.bottom - rect.top,
			GetWindowLongPtrW(m_hDebugBox, GWL_STYLE));
	}


	/*���໯,��ȡ���ݵ�*/
	void SubDebugBoxClass() {
		SetWindowSubclass(m_hTab, SubclassProc_TabParent, SCID_TABPARENT, (DWORD_PTR)this);
		SetWindowSubclass(m_hTabControl, SubclassProc_Tab, SCID_TAB, (DWORD_PTR)this);
		m_oldproc = (WNDPROC)SetWindowLongA(m_hDebugBox, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(SubclassProc_OrgEdit));
		SetWindowLongA(m_hDebugBox, GWL_USERDATA, (LONG)this);
		//SetWindowSubclass(m_hDebugBox, SubclassProc_OrgEdit, SCID_EDITORG, (DWORD_PTR)this);
	}
private:

	/*��ȡ�����Ե���������ھ��*/
	bool GetEplDebugOutputWindow() {

		m_hEplIDE = reinterpret_cast<HWND>(elibstl::NotifySys(NES_GET_MAIN_HWND, 0, 0));
		if (m_hEplIDE == NULL)
		{
			return false;
		}
		auto Temp_hWnd = FindWindowExW(m_hEplIDE, 0, L"AfxControlBar42s", L"״̬��");
		if (Temp_hWnd == NULL)
		{
			return false;
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
};


void e_debugbox_init() {
	static EDebugEdit instance;
}
ESTL_NAMESPACE_END