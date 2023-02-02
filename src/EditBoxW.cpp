#include"EcontrolHelp.h"
#include<Richedit.h>
#pragma warning(disable:4996)
static INT s_editbox_cmd[] = { 50 ,110 };

typedef struct _CEditExDATA
{
	INT m_Border;
	HFONT m_font;//�����ж������Ƿ���Ч��
	LOGFONTA m_fontdata;
	INT m_max;
	BOOL m_vscroll;
	COLORREF m_color;
	COLORREF m_font_bkgcolor;
	COLORREF m_bkgcolor;
	_CEditExDATA()
	{
		memset(this, 0, sizeof(*this));
	}
} CEDITDATA, * PCEDITDATA;
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
class eEditBoxEx
{
public:
	eEditBoxEx(
		LPBYTE pAllPropertyData,
		INT nAllPropertyDataSize,
		DWORD dwStyle,
		int x,
		int y,
		int cx,
		int cy,
		HWND hParent,
		UINT nId,
		DWORD dwWinFormID,
		DWORD dwUnitID
	)
		:
		m_dwWinFormID(dwWinFormID),
		m_dwUnitID(dwUnitID),
		m_hWnd(NULL),
		m_Style(0),
		m_Color(0),
		m_hFont(0),
		m_Font(0),
		m_TextA(0),
		m_TextW(0),
		m_Max(0),
		m_oldProc(0),
		m_hParentWnd(0),
		m_font_bkgcolor(16777215),
		m_bkgcolor(16777215),
		m_is_vsroll(0)
	{
		m_hParentWnd = CreateWindowExW(0, L"Static", 0, dwStyle | WS_VISIBLE | WS_CHILDWINDOW | WS_CLIPSIBLINGS, x, y, cx, cy, hParent, (HMENU)nId, GetModuleHandle(0), 0);

		m_hWnd = CreateWindowExW(
			0,
			L"Edit",
			L"",
			dwStyle | WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_MULTILINE | WS_CLIPSIBLINGS | ES_AUTOVSCROLL,
			0,
			0,
			cx,
			cy,
			m_hParentWnd,
			(HMENU)nId,
			GetModuleHandle(0),
			0
		);
		if (pAllPropertyData && nAllPropertyDataSize > 0)
		{
			PCEDITDATA WinData = (PCEDITDATA)pAllPropertyData;
			SetBorder(WinData->m_Border);
			if (WinData->m_font)
			{
				SetFont(WinData->m_fontdata);
			}
			else {
				m_Font = new LOGFONTA{ 0 };
				char FontName[LF_FACESIZE] = "Microsoft YaHei UI";
				strcpy(m_Font->lfFaceName, FontName);
				m_Font->lfHeight = 16;
				m_hFont = CreateFontIndirectA(m_Font);
				SetFont(*m_Font);
			}
			SetMaxText(WinData->m_max);
			SetText((WCHAR*)(pAllPropertyData + sizeof(CEDITDATA)));
			SetVscroll(WinData->m_vscroll);
			SetFontColor(WinData->m_color);
			SetFontBkgColor(WinData->m_font_bkgcolor);
			SetBkgColor(WinData->m_bkgcolor);
		}
		else
		{	//Ĭ����ʽ
			SetBorder(1);
			//Ĭ������
			m_Font = new LOGFONTA{ 0 };
			char FontName[LF_FACESIZE] = "Microsoft YaHei UI";
			strcpy(m_Font->lfFaceName, FontName);
			m_Font->lfHeight = 16;
			m_hFont = CreateFontIndirectA(m_Font);
			SetFont(*m_Font);
		}
		SetWindowLongPtrW(m_hParentWnd, GWL_USERDATA, (LONG_PTR)this);
		m_oldProc = (WNDPROC)SetWindowLongW(m_hParentWnd, GWL_WNDPROC, (LONG_PTR)WndProc);
	}
	void SetFontColor(COLORREF color)
	{
		if (!m_hWnd)
		{
			return;
		}
		m_Color = color;
	}
	COLORREF GetFontColor()
	{
		if (!m_hWnd)
		{
			return 0;
		}
		return m_Color;

	}
	void SetFontBkgColor(COLORREF color)
	{
		if (!m_hWnd)
		{
			return;
		}
		m_font_bkgcolor = color;
	}
	COLORREF GetFontBkgColor()
	{
		if (!m_hWnd)
		{
			return 0;
		}
		return m_font_bkgcolor;

	}
	void SetBkgColor(COLORREF color)
	{
		if (!m_hWnd)
		{
			return;
		}
		m_bkgcolor = color;
	}
	COLORREF GetBkgColor()
	{
		if (!m_hWnd)
		{
			return 0;
		}
		return m_bkgcolor;

	}
	void SetMaxText(INT Max) {
		if (!m_hWnd)
		{
			return;
		}
		SendMessage(m_hWnd, EM_LIMITTEXT, (WPARAM)Max, 0);
		m_Max = Max;
	}
	void SetVscroll(BOOL is_set) {
		m_is_vsroll = is_set;
		ShowScrollBar(m_hWnd, SB_VERT, is_set);
	}
	BOOL GetVscroll() {
		if (!m_hWnd)
		{
			return false;
		}
		//long style = GetWindowLong(m_hWnd, GWL_STYLE);
		//return (style & WS_VSCROLL);
		return m_is_vsroll;
	}
	void OnChange() {
		EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 0);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
	}
	BOOL SetPos(int width, int height) {//�������ı�ʱ����ô˺���
		return SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	}
	WNDPROC GetOldProc() const {
		if (!m_hWnd)
		{
			return 0;
		}
		if (m_oldProc <= 0)
		{
			return 0;
		}
		return m_oldProc;
	}
	INT GetMaxText()const {
		if (!m_hWnd)
		{
			return 0;
		}
		return m_Max;
	}
	void SetFont(LOGFONTA Font)
	{
		if (!m_hWnd)
		{
			return;
		}
		if (m_hFont) {//���ھ��������

			DeleteObject(m_hFont);
			m_hFont = NULL;
		}
		if (m_Font)//��������������
		{
			delete m_Font;
			m_Font = NULL;
		}
		m_Font = new LOGFONTA{ 0 };
		memcpy(m_Font, &Font, sizeof(LOGFONTA));
		m_hFont = CreateFontIndirectA(&Font);
		SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)m_hFont, 1);
	}
	~eEditBoxEx()
	{
		if (m_hParentWnd)
		{
			::SetWindowLongW(m_hParentWnd, GWL_WNDPROC, (LONG_PTR)m_oldProc);
		}
		if (m_hWnd)
		{

			DestroyWindow(m_hWnd);
			m_hWnd = 0;
		}

		Delete();

	}
	void Delete() {

		if (m_hFont) {//���ھ��������

			DeleteObject(m_hFont);
			m_hFont = NULL;
		}
		if (m_Font)//��������������
		{
			delete m_Font;
			m_Font = NULL;
		}
		if (m_TextA)
		{
			delete[]m_TextA;
			m_TextA = NULL;
		}
		if (m_TextW)
		{
			delete[]m_TextW;
			m_TextW = NULL;
		}
	}
	HWND GetHwnd() const
	{
		return m_hParentWnd;
	}
	HWND GetcHwnd() const
	{
		return m_hWnd;
	}
	HFONT GetFont(LOGFONTA* Font) {
		if (m_hFont)
		{
			memcpy(Font, m_Font, sizeof(LOGFONTA));
		}
		return m_hFont;
	}
	LOGFONTA* outGetFont() {
		if (!m_hWnd)
		{
			return 0;
		}
		return m_Font;
	}
	void SetText(const wchar_t* text)
	{
		if (!m_hWnd)
		{
			return;
		}

		SetWindowTextW(m_hWnd, text);
	}
	void SetText(const char* text)
	{
		if (!m_hWnd)
		{
			return;
		}

		SetWindowTextA(m_hWnd, text);
	}
	void AddText(const std::wstring& text) {
		// ��ȡ�༭����ı�����
		LRESULT length = SendMessageW(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
		// �����ı�ѡ��Ŀ�ʼ�ͽ���λ�õ��ı�ĩβ
		SendMessageW(m_hWnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
		SendMessageW(m_hWnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)text.c_str());
		length = SendMessageW(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
	}
	std::wstring GetText() const
	{
		if (!m_hWnd)
		{
			return std::wstring();
		}
		int length = GetWindowTextLengthW(m_hWnd);
		std::vector<wchar_t> buffer(length + 1);
		GetWindowTextW(m_hWnd, buffer.data(), buffer.size());
		return std::wstring(buffer.data());
	}
	char* outGetText()
	{
		if (!m_hWnd)
		{
			return 0;
		}
		if (m_TextA)
		{
			delete[]m_TextA;
			m_TextA = NULL;
		}
		int length = GetWindowTextLengthA(m_hWnd);
		m_TextA = new char[length + 1]{ 0 };
		GetWindowTextA(m_hWnd, m_TextA, length);
		return m_TextA;
	}
	wchar_t* GetTextWtoE()
	{
		if (!m_hWnd)
		{
			return 0;
		}
		if (m_TextW)
		{
			delete[]m_TextW;
			m_TextW = NULL;
		}
		int length = GetWindowTextLengthW(m_hWnd);
		m_TextW = new wchar_t[length + 1]{ 0 };
		GetWindowTextW(m_hWnd, m_TextW, length + 1);

		return m_TextW;
	}
	// ���ñ߿�
	void SetBorder(INT type)
	{
		// �����ǰ���ڴ��е��߱߿���ʽ�������Ƴ����߱߿���ʽ
		if (GetWindowLongPtrW(m_hWnd, GWL_STYLE) & WS_BORDER)
		{
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) & ~WS_BORDER);
		}

		// �����������ô��ڵ���չ��ʽ
		switch (type)
		{
		case 0:
			// �ޱ߿�
			m_Style = 0;
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, 0);
			break;
		case 1:
			// ����ʽ
			m_Style = 1;
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, WS_EX_CLIENTEDGE);
			break;
		case 2:
			// ͻ��ʽ
			m_Style = 2;
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME);
			break;
		case 3:
			// ǳ����ʽ
			m_Style = 3;
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, WS_EX_STATICEDGE);
			break;
		case 4:
			// ����ʽ
			m_Style = 4;
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, WS_EX_DLGMODALFRAME);
			break;
		case 5:
			// ���߱߿�ʽ
			m_Style = 5;
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) | WS_BORDER);
			SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, 0);
			break;
		default:
			break;
		}

		// ֪ͨϵͳ������ʽ�Ѿ��ı�
		SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	INT GetBorder() const
	{
		return m_Style;
	}
private:
	WNDPROC m_oldProc;
	INT m_Max;
	LOGFONTA* m_Font;
	HFONT m_hFont;
	COLORREF m_Color, m_font_bkgcolor, m_bkgcolor;
	INT m_Style;
	HWND m_hWnd;
	wchar_t* m_TextW;
	HWND m_hParentWnd;
	char* m_TextA;
	DWORD m_dwWinFormID, m_dwUnitID;
	bool m_is_vsroll;

};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	eEditBoxEx* pEditBox = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);
	if (!pEditBox)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	WNDPROC oldproc = pEditBox->GetOldProc();
	if (oldproc == NULL)
	{
		oldproc = DefWindowProc;
	}

	switch (message)
	{
	case WM_SIZE://�������ڱ��ı�ʱ�ı䰴ť��С
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		pEditBox->SetPos(width, height);
		break;
	}
	case WM_CTLCOLOREDIT:
	{
		//�ı���ɫ
		SetTextColor(reinterpret_cast<HDC>(wParam), pEditBox->GetFontColor());
		//�ı�������ɫ
		SetBkColor(reinterpret_cast<HDC>(wParam), pEditBox->GetFontBkgColor());
		//HDC��ˢ,�༭��������ɫ
		HBRUSH hBrush = CreateSolidBrush(pEditBox->GetBkgColor());
		return reinterpret_cast<LRESULT>(hBrush);
	}
	case WM_DESTROY: {
		HBRUSH hBrush = reinterpret_cast<HBRUSH>(GetClassLong(hWnd, GCL_HBRBACKGROUND));
		if (hBrush)
		{
			DeleteObject(hBrush);
		}
		pEditBox->~eEditBoxEx();
		break;
	}

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			pEditBox->OnChange();
		}
		break;
	default:
		break;
	}
	return oldproc(hWnd, message, wParam, lParam);
};

static HUNIT WINAPI Create(
	LPBYTE pAllPropertyData,            //   ָ�򱾴��ڵ�Ԫ��������������, �ɱ����ڵ�Ԫ��ITF_GET_PROPERTY_DATA�ӿڲ���, ���û��������ΪNULL
	INT nAllPropertyDataSize,           //   �ṩpAllPropertyData��ָ�����ݵĳߴ�, ���û����Ϊ0
	DWORD dwStyle,                      //   Ԥ�����õĴ��ڷ��
	HWND hParentWnd,                    //   �����ھ��
	UINT uID,                           //   �ڸ������е�ID
	HMENU hMenu,                        //   δʹ��
	INT x, INT y, INT cx, INT cy,       //   ָ��λ�ü��ߴ�
	DWORD dwWinFormID, DWORD dwUnitID,  //   �����ڵ�Ԫ���ڴ��ڼ������ID, ����֪ͨ��ϵͳ
	HWND hDesignWnd,                    //   ���blInDesignModeΪ��, ��hDesignWnd�ṩ����ƴ��ڵĴ��ھ��
	BOOL blInDesignMode                 //   ˵���Ƿ�������IDE�����Խ��п��ӻ����, ����ʱΪ��
)
{
	eEditBoxEx* EditBox = new eEditBoxEx(pAllPropertyData, nAllPropertyDataSize, dwStyle, x, y, cx, cy, hParentWnd, uID, dwWinFormID, dwUnitID);

	return elibstl::make_cwnd(EditBox->GetHwnd());
}

static BOOL WINAPI Change(HUNIT hUnit, INT nPropertyIndex,  // ���޸ĵ���������
	UNIT_PROPERTY_VALUE* pPropertyVaule, // �����޸ĵ���Ӧ��������
	LPTSTR* ppszTipText) {  //Ŀǰ��δʹ��
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eEditBoxEx* EditBox = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);;

	switch (nPropertyIndex)
	{
	case 0: {
		EditBox->SetText(pPropertyVaule->m_szText);
		break;
	}
	case 1:
		if (pPropertyVaule->m_data.m_pData && pPropertyVaule->m_data.m_nDataSize)
		{
			std::vector<unsigned char> wcs(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_pData + pPropertyVaule->m_data.m_nDataSize);
			std::wstring wstr;
			wstr.append((wchar_t*)wcs.data(), wcs.size() / sizeof(wchar_t));
			EditBox->SetText(std::wstring(wstr.c_str()).c_str());
		}
		break;
	case 2:
		EditBox->SetBorder(pPropertyVaule->m_int);
		break;
	case 3: {
		LOGFONTA* Font = (LOGFONTA*)pPropertyVaule->m_int;
		if (Font)
		{
			EditBox->SetFont(*Font);
		}
		break;
	}
	case 4:
	{
		EditBox->SetMaxText(pPropertyVaule->m_int);
		break;
	}
	case 5:
	{
		EditBox->SetVscroll(pPropertyVaule->m_bool);
		break;
	}
	case 6:
	{
		EditBox->SetFontColor(pPropertyVaule->m_clr);
		break;
	}
	case 7:
	{
		EditBox->SetFontBkgColor(pPropertyVaule->m_clr);
		break;
	}
	case 8:
	{
		EditBox->SetBkgColor(pPropertyVaule->m_clr);
		break;
	}
	default:

		break;
	}
	return 1;
}

static HGLOBAL WINAPI GetAlldata(HUNIT hUnit)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eEditBoxEx* EditBox = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);;
	CEDITDATA temp;
	temp.m_font = EditBox->GetFont(&temp.m_fontdata);
	temp.m_Border = EditBox->GetBorder();
	temp.m_max = EditBox->GetMaxText();
	temp.m_vscroll = EditBox->GetVscroll();
	temp.m_color = EditBox->GetFontColor();
	temp.m_font_bkgcolor = EditBox->GetFontBkgColor();
	temp.m_bkgcolor = EditBox->GetBkgColor();
	std::wstring wstr = EditBox->GetText();

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(CEDITDATA) + (wstr.size() + 1) * sizeof(wchar_t));
	if (hGlobal)
	{
		PVOID pGlobal = ::GlobalLock(hGlobal);
		if (pGlobal)
		{
			memcpy(pGlobal, &temp, sizeof(CEDITDATA));
			if (!wstr.empty())
			{
				memcpy((LPBYTE)pGlobal + sizeof(CEDITDATA), wstr.c_str(), wstr.size() * sizeof(wchar_t));
			}
			::GlobalUnlock(hGlobal);
		}
	}
	return hGlobal;
	//return 0;
}

static BOOL WINAPI GetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eEditBoxEx* EditBox = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);;
	if (!EditBox)
	{
		return 0;
	}
	switch (nPropertyIndex)
	{
	case 0:
	{
		pPropertyVaule->m_szText = EditBox->outGetText();
		break;
	}
	case 1:
	{
		WCHAR* wcs = EditBox->GetTextWtoE();
		if (wcs)
		{
			pPropertyVaule->m_data.m_pData = (LPBYTE)wcs;
			pPropertyVaule->m_data.m_nDataSize = wcslen(wcs) * sizeof(wchar_t);
		}
		break;
	}
	case 2:
	{
		pPropertyVaule->m_int = EditBox->GetBorder();
		break;
	}
	case 3:
	{
		LOGFONTA* Font = EditBox->outGetFont();
		if (Font)
		{
			pPropertyVaule->m_data.m_pData = (LPBYTE)Font;
			pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA);

		}

		break;
	}
	case 4:
	{
		pPropertyVaule->m_int = EditBox->GetMaxText();
		break;
	}
	case 5:
	{
		pPropertyVaule->m_bool = EditBox->GetVscroll();
		break;
	}
	case 6:
	{
		pPropertyVaule->m_clr = EditBox->GetFontColor();
		//MessageBoxA(0, std::to_string((int)EditBox->GetEditBoxFontColor()).c_str(), 0, 0);
		break;
	}
	case 7:
	{
		pPropertyVaule->m_clr = EditBox->GetFontBkgColor();
		break;
	}
	case 8:
	{
		pPropertyVaule->m_clr = EditBox->GetBkgColor();
		break;
	}
	default:

		return false;
	}

	return true;
}
static BOOL WINAPI InputW(HUNIT hUnit, INT nPropertyIndex,
	BOOL* pblModified, LPVOID pResultExtraData)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eEditBoxEx* Button = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);;
	if (nPropertyIndex == 1)
	{
		Button->SetText(std::wstring(MyInputBox(Button->GetText()).c_str()).c_str());
		*pblModified = true;
	}
	return *pblModified;
}

EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_EditBoxW(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
	{
		// �������
		return (PFN_INTERFACE)Create;
	}
	case ITF_NOTIFY_PROPERTY_CHANGED:
	{
		// ֪ͨĳ�������ݱ��û��޸�
		return (PFN_INTERFACE)Change;
	}
	case ITF_GET_ALL_PROPERTY_DATA:
	{
		// ȡȫ����������
		return (PFN_INTERFACE)GetAlldata;
	}
	case ITF_GET_PROPERTY_DATA:
	{
		// ȡĳ��������
		return (PFN_INTERFACE)GetData;
	}
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
	{
		return (PFN_INTERFACE)InputW;
	}
	default:
		return NULL;
	}
	return NULL;
}

static EVENT_INFO2 s_event[] =
{
	/*000*/ {"�ַ����ı�", "���ַ����ı�ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_member[] =
{
	// FIXED_WIN_UNIT_PROPERTY,    // ������ϴ˺�, ����ֱ��չ��, �����չ��
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/ {"���", "left", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
	/*001*/ {"����", "top", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
	/*002*/ {"���", "width", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
	/*003*/ {"�߶�", "height", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
	/*004*/ {"���", "tag", NULL, UD_TEXT, _PROP_OS(OS_ALL), NULL},
	/*005*/ {"����", "visible", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL},
	/*006*/ {"��ֹ", "disable", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL},
	/*007*/ {"���ָ��", "MousePointer", NULL, UD_CURSOR, _PROP_OS(OS_ALL), NULL},

	// ������������������Ҫ��Ĭ������, ÿ�������������, ���ǿ����ֶ�����
	// ��������������������, ��Щ����ǿ����Ҫ��
	// ��Щ�����������ԵĻص����Ǵ�0��ʼ��, ����, ��Щ���Ҳ��0��ʼ
	/*000*/  {"����", "title", "", UD_TEXT, _PROP_OS(__OS_WIN), NULL},
	/*001*/  {"����W", "PIC", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*002*/	 {"�߿�", "", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""ͻ��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
	/*003*/  {"����", "Font", "", UD_FONT, _PROP_OS(__OS_WIN) , NULL} ,
	/*004*/  {"������Ƴ���", "", "������Ϊ0", UD_INT, _PROP_OS(__OS_WIN),  NULL},
	/*005*/  {"������", "", "���ô�֮������", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*006*/  {"������ɫ", "", "����������ɫ", UD_COLOR, _PROP_OS(__OS_WIN),  NULL},
	/*007*/  {"���屳����ɫ", "", "���ñ༭��������ı�����ɫ", UD_COLOR, _PROP_OS(__OS_WIN),  NULL},
	/*008*/  {"�༭�򱳾���ɫ", "", "���ñ༭��ı�����ɫ", UD_COLOR, _PROP_OS(__OS_WIN),  NULL},

};
namespace elibstl {

	LIB_DATA_TYPE_INFO editboxw = {
		"�༭��W",//��������
		"EditBoxW",//Ӣ������
		"unicode�༭��",//˵��
		sizeof(s_editbox_cmd) / sizeof(s_editbox_cmd[0]),//��������
		s_editbox_cmd,//��ȫ�ֺ����ж�Ӧ������
		_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
		104,//��ԴID
		sizeof(s_event) / sizeof(s_event[0]),
		s_event,
		sizeof(s_member) / sizeof(s_member[0]),//������
		s_member,//����ָ��
		libstl_GetInterface_EditBoxW,//��������ӳ���
		NULL,//��Ա����
		NULL//��Ա��������
	};
}






#pragma region ����

static ARG_INFO Args[] =
{
	{
		/*name*/    "�ı�",
		/*explain*/ ("��������ı�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   NULL,
	}
};

EXTERN_C void Fn_EditBoxW_AddText(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	eEditBoxEx* Button = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);

	for (INT i = 1; i < nArgCount; i++)
	{
		auto bnow = elibstl::args_to_wsdata(pArgInf, i);
		Button->AddText(std::wstring(bnow));

	}
}


FucInfo EditBoxW_AddText = { {
		/*ccname*/  ("�����ı�"),
		/*egname*/  ("AddText"),
		/*explain*/ ("��ָ���ı����뵽�༭�����ݵ�β����"),
		/*category*/-1,
		/*state*/   CT_ALLOW_APPEND_NEW_ARG,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  Args,
	} , Fn_EditBoxW_AddText ,"Fn_EditBoxW_AddText" };


EXTERN_C void Fn_EditBoxW_GetHwnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	eEditBoxEx* Button = (eEditBoxEx*)GetWindowLongPtrW(hWnd, GWL_USERDATA);

	pRetData->m_int = reinterpret_cast<INT>(Button->GetcHwnd());
}


FucInfo EditBoxW_GetHwnd = { {
		/*ccname*/  ("ȡ�༭����"),
		/*egname*/  ("GetHwnd"),
		/*explain*/ ("ȡ���༭��ľ����"),
		/*category*/-1,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} , Fn_EditBoxW_GetHwnd ,"Fn_EditBoxW_GetHwnd" };

#pragma endregion