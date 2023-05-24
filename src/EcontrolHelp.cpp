#include "EcontrolHelp.h"
#pragma comment(lib, "comctl32.lib")

#define WCN_IDEINTPUTBOX			L"eLibStl.WndClass.IDEInputBox"

#define IDC_ED_INPUT	101
#define IDC_BT_READIN	201
#define IDC_BT_OK		202
#define IDC_BT_CANCEL	203

#define PROP_IDEINPUTBOXEDITPROC	L"eLibStl.Prop.IDEInputBox.Edit.Proc"

#define DEF_SIZE(height,width) static INT WINAPI DefSize(INT nMsg, DWORD dwParam1, DWORD dwParam2){switch (nMsg){case NU_GET_CREATE_SIZE_IN_DESIGNER:{*((intptr_t*)dwParam1) = height;*((intptr_t*)dwParam2) = width;}return TRUE;}return FALSE;}



//��д�ط���Ҫʹ�õ��������ȶ���
HMODULE g_elibstl_hModule = NULL;

ESTL_NAMESPACE_BEGIN
void  GetDataFromHBIT(HBITMAP hBitmap, std::vector<unsigned char>& pData)
{
	pData.clear();
	if (hBitmap == NULL)
	{
		return;
	}
	HDC hdc;			//�豸������
	int ibits;
	WORD wbitcount;     //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���

	//λͼ��ÿ��������ռ�ֽ����������ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С ��д���ļ��ֽ���
	DWORD dwpalettesize = 0, dwbmbitssize, dwdibsize;

	BITMAP bitmap;				//λͼ���Խṹ
	BITMAPFILEHEADER bmfhdr;	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ

	//�����ļ��������ڴ�������ɫ����
	HANDLE hdib, hpal, holdpal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hdc = CreateDCW(L"display", NULL, NULL, NULL);
	ibits = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);
	DeleteDC(hdc);

	if (ibits <= 1)
		wbitcount = 1;
	else if (ibits <= 4)
		wbitcount = 4;
	else if (ibits <= 8)
		wbitcount = 8;
	else if (ibits <= 16)
		wbitcount = 16;
	else if (ibits <= 24)
		wbitcount = 24;
	else
		wbitcount = 32;

	//�����ɫ���С
	if (wbitcount <= 8)
		dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bitmap.bmWidth;
	bi.biHeight = bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wbitcount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwbmbitssize = ((bitmap.bmWidth * wbitcount + 31) / 32) * 4 * bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ�
	hdib = GlobalAlloc(GHND, dwbmbitssize + dwpalettesize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
	*lpbi = bi;

	// �����ɫ�� 
	hpal = GetStockObject(DEFAULT_PALETTE);
	if (hpal)
	{
		hdc = ::GetDC(NULL);
		holdpal = SelectPalette(hdc, (HPALETTE)hpal, false);
		RealizePalette(hdc);
	}

	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettesize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ�� 
	if (holdpal)
	{
		SelectPalette(hdc, (HPALETTE)holdpal, true);
		RealizePalette(hdc);
		::ReleaseDC(NULL, hdc);
	}

	// ����λͼ�ļ�ͷ
	bmfhdr.bfType = 0x4d42; // "bm"
	dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		dwpalettesize + dwbmbitssize;
	bmfhdr.bfSize = dwdibsize;
	bmfhdr.bfReserved1 = 0;
	bmfhdr.bfReserved2 = 0;
	bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		(DWORD)sizeof(BITMAPINFOHEADER) + dwpalettesize;
	std::vector<unsigned char> head((unsigned char*)&bmfhdr, (unsigned char*)&bmfhdr + sizeof(BITMAPFILEHEADER));
	std::vector<unsigned char> body((unsigned char*)lpbi, (unsigned char*)lpbi + dwdibsize);

	//��� 
	GlobalUnlock(hdib);
	GlobalFree(hdib);
	pData.reserve(head.size() + body.size());
	pData.insert(pData.end(), head.begin(), head.end());
	pData.insert(pData.end(), body.begin(), body.end());;
}

HFONT EzFont(PCWSTR pszFontName, int iPoint, int iWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut)
{
	HDC hDC = GetDC(NULL);
	int iSize;
	iSize = -MulDiv(iPoint, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);
	return CreateFontW(iSize, 0, 0, 0, iWeight, bItalic, bUnderline, bStrikeOut, 0, 0, 0, 0, 0, pszFontName);
}

ESTL_EZDLG_NAMESPACE_BEGIN
void EIDEDlgShow(PCWSTR pszWndClass, PCWSTR pszCaption, int x, int y, int cx, int cy, DWORD dwStyle, EDLGCTX_BASE* pCtx)
{
	RECT rcEMain{};
	if (x == CW_USEDEFAULT)
	{
		GetWindowRect(pCtx->hwndEMain, &rcEMain);
		rcEMain.left += ((rcEMain.right - rcEMain.left - cx) / 2);
		rcEMain.top += ((rcEMain.bottom - rcEMain.top - cy) / 2);
	}
	else
	{
		rcEMain.left = x;
		rcEMain.top = y;
	}
	if (!dwStyle)
		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	HWND hWnd = CreateWindowExW(0, pszWndClass, pszCaption, dwStyle,
		rcEMain.left, rcEMain.top, cx, cy, NULL, NULL, g_elibstl_hModule, pCtx);
	SetWindowLongPtrW(hWnd, GWLP_HWNDPARENT, (LONG_PTR)pCtx->hwndEMain);
	assert(hWnd);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessageW(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}

LRESULT CALLBACK SubclassProc_TabRepair(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_GETDLGCODE)
	{
		LRESULT lResult = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		lResult &= ~DLGC_WANTTAB;
		if (lParam)
			if (((MSG*)lParam)->message == WM_KEYDOWN && ((MSG*)lParam)->wParam == VK_TAB)
				lResult &= ~DLGC_WANTALLKEYS;
		return lResult;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
ESTL_EZDLG_NAMESPACE_END

struct EDLGCTX_IDEINPUTBOX :public EzDlg::EDLGCTX_BASE
{
	PCWSTR pszInitContent;
	PWSTR* ppszInput;
};
static ATOM s_atomIDEInputBox = 0;
//static BOOL CALLBACK MyInputBoxDlgProcW(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	HWND hwndEDIT = GetDlgItem(hwndDlg, 1001);
//	switch (uMsg)
//	{
//	case WM_INITDIALOG:
//	{
//		::SetWindowTextW(hwndDlg, L"�����������ı�:");
//		::SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, 0);
//		::SetWindowTextW(hwndEDIT, (LPCWSTR)lParam);
//	}
//
//	break;
//	case WM_COMMAND:
//	{
//		INT nID = wParam & 0xffff;
//		if (nID == IDCANCEL)
//			DestroyWindow(hwndDlg);
//		else if (nID == IDOK) {
//
//
//			size_t nLen = ::GetWindowTextLengthW(hwndEDIT);
//			if (nLen > 0)
//			{
//				wchar_t* Editstr = new wchar_t[nLen + 1]{ 0 };
//				GetWindowTextW(hwndEDIT, Editstr, nLen + 1);
//				std::wstring* result = (std::wstring*)::GetWindowLongPtrW(hwndDlg, GWLP_USERDATA);
//				*result = std::wstring(Editstr);
//				delete[]Editstr;
//			}
//			DestroyWindow(hwndDlg);
//		}
//	}
//	break;
//	case WM_CLOSE: {
//		size_t nLen = ::GetWindowTextLengthW(hwndEDIT);
//		if (nLen > 0)
//		{
//			wchar_t* Editstr = new wchar_t[nLen + 1]{ 0 };
//			GetWindowTextW(hwndEDIT, Editstr, nLen + 1);
//			std::wstring* result = (std::wstring*)::GetWindowLongPtrW(hwndDlg, GWLP_USERDATA);
//			*result = std::wstring(Editstr);
//			delete[]Editstr;
//		}
//		DestroyWindow(hwndDlg);
//		break;
//	}
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return FALSE;
//	}
//	return TRUE;
//}

static LRESULT CALLBACK WndProc_InputBoxEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
		if (wParam == 'A')
			if (GetKeyState(VK_CONTROL) & 0x80000000)
				SendMessageW(hWnd, EM_SETSEL, 0, -1);// Ctrl + Aȫѡ

	return CallWindowProcW((WNDPROC)GetPropW(hWnd, PROP_IDEINPUTBOXEDITPROC), hWnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK WndProc_InputBox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const int cyBtn = 36, cxBtn1 = 110, cxBtn2 = 90, sizePadding = 10;
	switch (uMsg)
	{
	case WM_SIZE:
	{
		int cxClient = LOWORD(lParam), cyClient = HIWORD(lParam);
		SetWindowPos(GetDlgItem(hWnd, IDC_ED_INPUT), NULL, 0, 0,
			cxClient - sizePadding * 2,
			cyClient - cyBtn - sizePadding * 3,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

		int yBtn = cyClient - cyBtn - sizePadding;
		SetWindowPos(GetDlgItem(hWnd, IDC_BT_READIN), NULL,
			sizePadding,
			yBtn,
			0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

		SetWindowPos(GetDlgItem(hWnd, IDC_BT_OK), NULL,
			cxClient - cxBtn2 * 2 - sizePadding * 2,
			yBtn,
			0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

		SetWindowPos(GetDlgItem(hWnd, IDC_BT_CANCEL), NULL,
			cxClient - cxBtn2 - sizePadding,
			yBtn,
			0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	return 0;

	case WM_COMMAND:
	{
		if (HIWORD(wParam) != BN_CLICKED)
			break;
		if (!lParam)
			break;
		switch (LOWORD(wParam))
		{
		case IDC_BT_READIN:
		{
			WCHAR szPath[MAX_PATH]{};
			OPENFILENAMEW ofn{};
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szPath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (!GetOpenFileNameW(&ofn))
				return 0;
			HANDLE hFile = CreateFileW(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				wsprintfW(szPath, L"���ļ�ʧ�ܣ�\n�����룺0x%08X", GetLastError());
				MessageBoxW(hWnd, szPath, L"����", MB_ICONERROR);
				return 0;
			}

			ULONGLONG ullSize;
			GetFileSizeEx(hFile, (LARGE_INTEGER*)&ullSize);
			if (ullSize >> 32)
			{
				MessageBoxW(hWnd, L"�ļ�����", L"����", MB_ICONERROR);
				CloseHandle(hFile);
				return 0;
			}
			DWORD dwSize = (DWORD)ullSize;
			if (!dwSize)
			{
				CloseHandle(hFile);
				return 0;
			}

			PWSTR p;
			ULONG uExceptionCode;
			__try
			{
				p = (PWSTR)HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + sizeof(WCHAR));
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				uExceptionCode = GetExceptionCode();
				CloseHandle(hFile);
				wsprintfW(szPath, L"����%u�ֽ��ڴ�ʧ�ܣ�\n�쳣���룺0x%08X", dwSize + sizeof(WCHAR), uExceptionCode);
				MessageBoxW(hWnd, szPath, L"����", MB_ICONERROR);
				return 0;
			}
			assert(p);// ���������Ǹ�����
			DWORD dwRead;
			ReadFile(hFile, p, dwSize, &dwRead, NULL);
			CloseHandle(hFile);
			*(p + dwSize / sizeof(WCHAR)) = L'\0';
			SetWindowTextW(GetDlgItem(hWnd, IDC_ED_INPUT), p);
			HeapFree(GetProcessHeap(), 0, p);
		}
		return 0;

		case IDC_BT_OK:
		{
			auto p = (EDLGCTX_IDEINPUTBOX*)GetWindowLongPtrW(hWnd, 0);
			p->bOK = TRUE;
			HWND hED = GetDlgItem(hWnd, IDC_ED_INPUT);
			int cch = GetWindowTextLengthW(hED);
			if (cch)
			{
				PWSTR psz = new WCHAR[cch + 1];
				GetWindowTextW(hED, psz, cch + 1);
				*(p->ppszInput) = psz;
			}
			else
				*(p->ppszInput) = NULL;

			EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
		}
		return 0;

		case IDC_BT_CANCEL:
		{
			auto p = (EDLGCTX_IDEINPUTBOX*)GetWindowLongPtrW(hWnd, 0);
			p->bOK = FALSE;
			*(p->ppszInput) = NULL;
			EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
		}
		return 0;
		}
	}
	break;

	case WM_CLOSE:
	{
		auto p = (EDLGCTX_IDEINPUTBOX*)GetWindowLongPtrW(hWnd, 0);
		p->bOK = FALSE;
		*(p->ppszInput) = NULL;
		EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
	}
	return 0;

	case WM_CREATE:
	{
		auto p = (EDLGCTX_IDEINPUTBOX*)((CREATESTRUCTW*)lParam)->lpCreateParams;
		SetWindowLongPtrW(hWnd, 0, (LONG_PTR)p);
		p->hFont = EzFont(L"΢���ź�", 10);
		SendMessageW(hWnd, WM_SETFONT, (WPARAM)p->hFont, FALSE);
		HWND hCtrl;
		hCtrl = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, p->pszInitContent, WS_CHILD | WS_VISIBLE | ES_MULTILINE,
			sizePadding, sizePadding, 0, 0, hWnd, (HMENU)IDC_ED_INPUT, g_elibstl_hModule, NULL);
		SetPropW(hCtrl, PROP_IDEINPUTBOXEDITPROC, (HANDLE)SetWindowLongPtrW(hCtrl, GWLP_WNDPROC, (LONG_PTR)WndProc_InputBoxEdit));
		ShowScrollBar(hCtrl, SB_VERT, TRUE);
		SendMessageW(hCtrl, WM_SETFONT, (WPARAM)p->hFont, FALSE);
		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"�����ı�(&I)", WS_CHILD | WS_VISIBLE,
			0, 0, cxBtn1, cyBtn, hWnd, (HMENU)IDC_BT_READIN, g_elibstl_hModule, NULL);
		SendMessageW(hCtrl, WM_SETFONT, (WPARAM)p->hFont, FALSE);
		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ȷ��(&O)", WS_CHILD | WS_VISIBLE,
			0, 0, cxBtn2, cyBtn, hWnd, (HMENU)IDC_BT_OK, g_elibstl_hModule, NULL);
		SendMessageW(hCtrl, WM_SETFONT, (WPARAM)p->hFont, FALSE);
		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ȡ��(&C)", WS_CHILD | WS_VISIBLE,
			0, 0, cxBtn2, cyBtn, hWnd, (HMENU)IDC_BT_CANCEL, g_elibstl_hModule, NULL);
		SendMessageW(hCtrl, WM_SETFONT, (WPARAM)p->hFont, FALSE);
	}
	return 0;

	case WM_DESTROY:
	{
		auto p = (EDLGCTX_IDEINPUTBOX*)GetWindowLongPtrW(hWnd, 0);
		DeleteObject(p->hFont);
		EzDlg::DlgModelOnDestroy(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
		PostQuitMessage(0);
	}
	return 0;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

BOOL IntputBox(PWSTR* ppszInput, PCWSTR pszInitContent, PCWSTR pszCaption)
{
	auto pCtx = EzDlg::EIDEDlgPreShow<EDLGCTX_IDEINPUTBOX>(&s_atomIDEInputBox, WCN_IDEINTPUTBOX, WndProc_InputBox);

	pCtx->ppszInput = ppszInput;
	pCtx->pszInitContent = pszInitContent;

	EIDEDlgShow(WCN_IDEINTPUTBOX, pszCaption, CW_USEDEFAULT, 0, 560, 500, 0, pCtx);

	BOOL bOK = pCtx->bOK;
	delete pCtx;
	return bOK;
}

bool SendToParentsHwnd(DWORD m_dwWinFormID, DWORD m_dwUnitID, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS || uMsg == WM_MOUSELAST || uMsg >= WM_MOUSEMOVE
		&& uMsg <= WM_RBUTTONUP || uMsg >= WM_KEYDOWN && uMsg <= WM_CHAR)
	{
		//�⼸���¼�ȫ��ת���������
		EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 0);
		INT control_type = 0;
		if (uMsg != WM_CHAR && uMsg != WM_SETFOCUS && uMsg != WM_KILLFOCUS)
		{
			if ((GetKeyState(VK_CONTROL) & 16384) != 0)
				control_type = 1;
			if ((GetKeyState(VK_SHIFT) & 16384) != 0)
				control_type = control_type | 2;
			if ((GetKeyState(VK_MENU) & 16384) != 0)
				control_type = control_type | 4;
		}

		if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_RBUTTONUP)
		{
			if (uMsg == WM_MOUSEMOVE)
				event.m_nEventIndex = -6;
			else
				event.m_nEventIndex = 512 - uMsg;
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

//std::wstring MyInputBox(const std::wstring& title)
//{
//	//�Ի��������ģ�壬��������MFC
//	const  BYTE MyInputBoxDialogTemplateData[] = {
//	0x01,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x08,0xC8,0x80,
//	0x03,0x00,0x00,0x00,0x00,0x00,0x5F,0x01,0xE2,0x00,0x00,0x00,0x00,0x00,0xF7,0x8B,
//	0x93,0x8F,0x65,0x51,0x1A,0xFF,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x8B,0x5B,
//	0x53,0x4F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x00,0x81,0x50,
//	0x05,0x00,0x03,0x00,0x53,0x01,0xC0,0x00,0xE9,0x03,0x00,0x00,0xFF,0xFF,0x81,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x50,
//	0x63,0x00,0xC7,0x00,0x37,0x00,0x13,0x00,0x01,0x00,0x00,0x00,0xFF,0xFF,0x80,0x00,
//	0x6E,0x78,0xA4,0x8B,0x93,0x8F,0x65,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0xBB,0x00,0xC7,0x00,0x37,0x00,0x13,0x00,
//	0x02,0x00,0x00,0x00,0xFF,0xFF,0x80,0x00,0x05,0x6E,0x7A,0x7A,0x00,0x00,0x00,0x00
//	};
//	std::wstring result;
//	HWND hDlg = CreateDialogIndirectParamW(::GetModuleHandleW(NULL), (LPCDLGTEMPLATE)MyInputBoxDialogTemplateData, 0, (DLGPROC)MyInputBoxDlgProcW, (LPARAM)title.c_str());
//	::SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)&result);
//	//result = temp;
//	if (hDlg) {
//		ShowWindow(hDlg, SW_SHOW);
//		UpdateWindow(hDlg);
//		MSG msg;
//		while (GetMessage(&msg, NULL, 0, 0)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//	return result;
//}

int DupStringForNewDeleteW(PWSTR& pszDst, PCWSTR pszSrc, int cchSrc)
{
	delete[] pszDst;
	if (!pszSrc || !cchSrc)
	{
		pszDst = NULL;
		return 0;
	}
	if (cchSrc < 0)
		cchSrc = wcslen(pszSrc);
	pszDst = new WCHAR[cchSrc + 1];
	wcsncpy(pszDst, pszSrc, cchSrc);
	*(pszDst + cchSrc) = L'\0';
	return cchSrc;
}

int DupStringForNewDeleteA(PSTR& pszDst, PCSTR pszSrc, int cchSrc)
{
	delete[] pszDst;
	if (!pszSrc || !cchSrc)
	{
		pszDst = NULL;
		return 0;
	}
	if (cchSrc < 0)
		cchSrc = strlen(pszSrc);
	pszDst = new CHAR[cchSrc + 1];
	strncpy(pszDst, pszSrc, cchSrc);
	*(pszDst + cchSrc) = '\0';
	return cchSrc;
}

SIZE_T DupStreamForNewDelete(BYTE*& pDst, PCBYTE pSrc, SIZE_T cbSrc)
{
	delete[] pDst;
	if (!pSrc)
	{
		pDst = NULL;
		return 0u;
	}
	pDst = new BYTE[cbSrc];
	memcpy(pDst, pSrc, cbSrc);
	return cbSrc;
}

void SetFrameType(HWND hWnd, int iFrame)
{
	DWORD dwStyle = GetWindowLongPtrW(hWnd, GWL_STYLE) & (~WS_BORDER);
	DWORD dwExStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE)
		& (~(WS_EX_WINDOWEDGE | WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE));

	switch (iFrame)
	{
	case 0: break;// �ޱ߿�
	case 1: dwExStyle |= WS_EX_CLIENTEDGE; break;// ����ʽ
	case 2: dwExStyle |= (WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME); break;// ͹��ʽ
	case 3: dwExStyle |= WS_EX_STATICEDGE; break;// ǳ����ʽ
	case 4: dwExStyle |= (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE); break;// ����ʽ
	case 5: dwStyle |= WS_BORDER; break;// ���߱߿�ʽ
	}

	SetWindowLongPtrW(hWnd, GWL_STYLE, dwStyle);
	SetWindowLongPtrW(hWnd, GWL_EXSTYLE, dwExStyle);
}

int GetFrameType(HWND hWnd)
{
	DWORD dwStyle = GetWindowLongPtrW(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
	if (dwExStyle & WS_EX_DLGMODALFRAME)
	{
		if (dwExStyle & WS_EX_WINDOWEDGE)
			return 2;// ͹��ʽ
		if (dwExStyle & WS_EX_CLIENTEDGE)
			return 4;// ����ʽ
	}

	if (dwExStyle & WS_EX_CLIENTEDGE)
		return 1;// ����ʽ
	if (dwExStyle & WS_EX_STATICEDGE)
		return 3;// ǳ����ʽ
	if (dwStyle & WS_BORDER)
		return 5;// ���߱߿�ʽ

	return 0;// �ޱ߿�
}

HBITMAP make_hbm_gp(BYTE* pData, SIZE_T cbPic)
{
	ULONG_PTR uToken;
	GdiplusStartupInput gpsi{};
	gpsi.GdiplusVersion = 1;
	if (GdiplusStartup(&uToken, &gpsi, NULL) != Ok)
		return NULL;
	HBITMAP hbm;
	GpBitmap* pBitmap;
	IStream* pStream = SHCreateMemStream(pData, cbPic);
	if (!pStream)
		goto Fail;
	if (GdipCreateBitmapFromStream(pStream, &pBitmap) != Ok)
	{
		pStream->Release();
		goto Fail;
	}
	if (GdipCreateHBITMAPFromBitmap(pBitmap, &hbm, 0))
	{
		GdipDisposeImage(pBitmap);
		pStream->Release();
		goto Fail;
	}
	GdipDisposeImage(pBitmap);
	pStream->Release();
	GdiplusShutdown(uToken);
	return hbm;
Fail:
	GdiplusShutdown(uToken);
	return NULL;
}

ESTL_CLRPICKER_NAMESPACE_BEGIN
static ATOM s_atomColorPicker = 0;
static WNDPROC s_pfnColorPickerDefProc = NULL;
#define WCN_COLORPICKER			L"eLibStl.WndClass.ColorPicker"
#define PROP_COLORPICKERCTX		L"eLibStl.Prop.ColorPickerContext"
#define SCID_COLORPICKERPARENT	20230524'01u

struct COLORPICKERCTX
{
	UINT uNotifyMsg;
	COLORREF crCustom;
	COLORREF crCCDlgCustom[16];// ��ʱ����
};

constexpr struct CPPRESETCOLOR
{
	COLORREF cr;
	PCWSTR pszName;
}
c_ColorPickerPresetClr[] =
{
	{CLR_DEFAULT,L"Ĭ��"},
	{0x000000,L"��ɫ"},
	{0xFFFFFF,L"��ɫ"},
	{0x0000FF,L"��ɫ"},
	{0x00FF00,L"��ɫ"},
	{0xFF0000,L"��ɫ"},
	{0x00FFFF,L"��ɫ"},
	{0xFF00FF,L"Ʒ��"},
	{0xFFFF00,L"��ɫ"},
	{CLR_INVALID,L"�Զ���..."},
};
#define CLPIDX_CUSTOM (ARRAYSIZE(c_ColorPickerPresetClr) - 1)

static LRESULT CALLBACK WndProc_ColorPicker(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
		SetPropW(hWnd, PROP_COLORPICKERCTX, ((CREATESTRUCTW*)lParam)->lpCreateParams);
		break;

	case WM_DESTROY:
	{
		auto pCtx = (COLORPICKERCTX*)GetPropW(hWnd, PROP_COLORPICKERCTX);
		RemovePropW(hWnd, PROP_COLORPICKERCTX);
		delete pCtx;
	}
	break;
	}

	return CallWindowProcW(s_pfnColorPickerDefProc, hWnd, uMsg, wParam, lParam);
}

constexpr int iCPItemPadding = 2;
constexpr int cxCPClrBlock = 20;
static LRESULT CALLBACK SubclassProc_ColorPickerParent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		auto pdis = (DRAWITEMSTRUCT*)lParam;
		auto pCtx = (COLORPICKERCTX*)GetPropW(pdis->hwndItem, PROP_COLORPICKERCTX);
		if (!pCtx)
			break;
		COLORREF cr = c_ColorPickerPresetClr[pdis->itemID].cr;
		HBRUSH hbr;
		HDC hDC = pdis->hDC;
		if (cr == CLR_DEFAULT)
			hbr = CreateHatchBrush(HS_BDIAGONAL, 0x000000);
		else if (cr == CLR_INVALID)
			hbr = CreateSolidBrush(pCtx->crCustom);
		else
			hbr = CreateSolidBrush(cr);

		if (IsBitExist(pdis->itemState, ODS_SELECTED))
		{
			FillRect(hDC, &pdis->rcItem, (HBRUSH)GetSysColorBrush(COLOR_HIGHLIGHT));
			SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			FillRect(hDC, &pdis->rcItem, (HBRUSH)GetSysColorBrush(COLOR_WINDOW));
			SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));
		}

		HGDIOBJ hOldBr = SelectObject(hDC, hbr);
		HGDIOBJ hOldPen = SelectObject(hDC, GetStockObject(BLACK_PEN));
		int xClrBlock = pdis->rcItem.left + iCPItemPadding;
		Rectangle(hDC,
			xClrBlock,
			pdis->rcItem.top + iCPItemPadding,
			xClrBlock + cxCPClrBlock,
			pdis->rcItem.bottom - iCPItemPadding);
		SelectObject(hDC, hOldPen);
		DeleteObject(SelectObject(hDC, hOldBr));
		SetBkMode(hDC, TRANSPARENT);

		RECT rcText = pdis->rcItem;
		rcText.left += (xClrBlock + cxCPClrBlock + iCPItemPadding);
		DrawTextW(hDC, c_ColorPickerPresetClr[pdis->itemID].pszName, -1, &rcText,
			DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);
		return TRUE;
	}
	break;

	case WM_MEASUREITEM:
	{
		auto pmis = (MEASUREITEMSTRUCT*)lParam;
		auto pCtx = (COLORPICKERCTX*)GetPropW(GetDlgItem(hWnd,pmis->CtlID), PROP_COLORPICKERCTX);
		if (!pCtx)
			break;
		pmis->itemHeight = 20;
		return TRUE;
	}
	break;

	case WM_COMMAND:
	{
		if (HIWORD(wParam) != CBN_SELCHANGE)
			break;
		auto pCtx = (COLORPICKERCTX*)GetPropW((HWND)lParam, PROP_COLORPICKERCTX);
		if (!pCtx)
			break;

		int idxCurrSel = SendMessageW((HWND)lParam, CB_GETCURSEL, 0, 0);
		COLORREF cr;
		if (idxCurrSel == CLPIDX_CUSTOM)
		{
			CHOOSECOLORW cc{ sizeof(CHOOSECOLORW) };
			cc.hwndOwner = hWnd;
			cc.lpCustColors = pCtx->crCCDlgCustom;
			cc.Flags = CC_ANYCOLOR | CC_FULLOPEN;
			if (ChooseColorW(&cc))
			{
				pCtx->crCustom = cc.rgbResult;
				InvalidateRect((HWND)lParam, NULL, FALSE);
			}
			cr = pCtx->crCustom;
		}
		else
			cr = c_ColorPickerPresetClr[idxCurrSel].cr;
		if (pCtx->uNotifyMsg)
			DefSubclassProc(hWnd, pCtx->uNotifyMsg, LOWORD(wParam), cr);
		return 0;
	}
	break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

HWND Create(int x, int y, int cx, int cy, DWORD dwStyle, DWORD dwExStyle, UINT uID, HWND hParent, UINT uNotifyMsg)
{
	if (!s_atomColorPicker)
	{
		WNDCLASSEXW wcex{ sizeof(WNDCLASSEXW) };
		GetClassInfoExW(NULL, WC_COMBOBOXW, &wcex);
		s_pfnColorPickerDefProc = wcex.lpfnWndProc;
		wcex.style &= (~CS_GLOBALCLASS);
		wcex.hInstance = g_elibstl_hModule;
		wcex.lpszClassName = WCN_COLORPICKER;
		wcex.lpfnWndProc = WndProc_ColorPicker;
		s_atomColorPicker = RegisterClassExW(&wcex);
	}

	auto p = new COLORPICKERCTX;
	ZeroMemory(p, sizeof(COLORPICKERCTX));
	p->uNotifyMsg = uNotifyMsg;
	p->crCustom = CLR_INVALID;
	SetWindowSubclass(hParent, SubclassProc_ColorPickerParent, SCID_COLORPICKERPARENT, 0);
	dwStyle |= (WS_CHILD | WS_VISIBLE | CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST);
	HWND hWnd = CreateWindowExW(0, WCN_COLORPICKER, NULL, dwStyle, 
		x, y, cx, cy, hParent, (HMENU)uID, NULL, p);

	int idx;
	SendMessageW(hWnd, WM_SETREDRAW, FALSE, 0);
	SendMessageW(hWnd, CB_INITSTORAGE, ARRAYSIZE(c_ColorPickerPresetClr), 0);
	for (auto& x : c_ColorPickerPresetClr)
	{
		idx = SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)x.pszName);
	}
	SendMessageW(hWnd, CB_SETCURSEL, 0, 0);
	SendMessageW(hWnd, WM_SETREDRAW, TRUE, 0);
	return hWnd;
}

COLORREF GetColor(HWND hWnd)
{
	auto pCtx = (COLORPICKERCTX*)GetPropW(hWnd, PROP_COLORPICKERCTX);
	if (!pCtx)
		return CLR_INVALID;
	int idx = SendMessageW(hWnd, CB_GETCURSEL, 0, 0);
	if (idx == CB_ERR)
		return CLR_INVALID;
	COLORREF cr = c_ColorPickerPresetClr[idx].cr;
	if (cr == CLR_INVALID)
		return pCtx->crCustom;
	else
		return cr;
}

BOOL SetColor(HWND hWnd, COLORREF cr)
{
	auto pCtx = (COLORPICKERCTX*)GetPropW(hWnd, PROP_COLORPICKERCTX);
	if (!pCtx)
		return FALSE;
	if (cr == CLR_INVALID)
		return FALSE;
	for (int i = 0; i < ARRAYSIZE(c_ColorPickerPresetClr); ++i)
	{
		if (c_ColorPickerPresetClr[i].cr == cr)
		{
			SendMessageW(hWnd, CB_SETCURSEL, i, 0);
			return TRUE;
		}
	}
	pCtx->crCustom = cr;
	SendMessageW(hWnd, CB_SETCURSEL, CLPIDX_CUSTOM, 0);
	return TRUE;
}
ESTL_CLRPICKER_NAMESPACE_END

SIZE_T CCtrlBase::InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID, UINT uID, HWND hParent)
{
	m_bInDesignMode = bInDesignMode;
	m_dwWinFormID = dwWinFormID;
	m_dwUnitID = dwUnitID;
	m_uID = uID;
	m_hParent = hParent;

	if (pAllData)
	{
		memcpy(&m_Info0, pAllData, sizeof(ECTRLINFO));
		BYTE* p = (BYTE*)pAllData + sizeof(ECTRLINFO) + m_Info0.cbPic;

		m_pszTextW = NULL;
		if (m_Info0.cchText)
		{
			elibstl::DupStringForNewDeleteW(m_pszTextW, (PCWSTR)p, m_Info0.cchText);
			m_pszTextA = elibstl::W2A(m_pszTextW);
		}
	}
	else
	{
		m_pszTextW = NULL;
		m_pszTextA = NULL;
	}

	m_pPicData = NULL;
	m_Info0.iVer = DATA_VER_BASE_1;

	if (pAllData)
		return sizeof(ECTRLINFO) + m_Info0.cbPic + m_Info0.cchText * sizeof(WCHAR);
	else
		return 0;
}

void CCtrlBase::InitBase0(PCVOID pAllData)
{
	if (pAllData)
	{
		BYTE* p = (BYTE*)pAllData + sizeof(ECTRLINFO);
		SetPic(p, m_Info0.cbPic);
	}
	else
		m_Info0.Font = elibstl::GetEDefLOGFONT(m_hWnd);

	SetFrame(m_Info0.iFrame);
	SetFont(&m_Info0.Font);
}

void CCtrlBase::SetPic(void* pPic, int cbSize)
{
	m_Info0.cbPic = cbSize;
	if (m_hbmPic)
		DeleteObject(m_hbmPic);

	if (cbSize)
	{
		if (m_bInDesignMode)
		{
			delete[] m_pPicData;
			m_pPicData = new BYTE[cbSize];
			memcpy(m_pPicData, pPic, cbSize);
		}
		if (m_bInDesignMode && !m_bGpDecodePicInDesignMode)
			m_hbmPic = elibstl::make_hbm((BYTE*)pPic, cbSize);
		else
			m_hbmPic = elibstl::make_hbm_gp((BYTE*)pPic, cbSize);
	}
	else
	{
		if (m_bInDesignMode)
		{
			delete[] m_pPicData;
			m_pPicData = NULL;
		}
		m_hbmPic = NULL;
	}
}

HGLOBAL CCtrlBase::FlattenInfoBase0(SIZE_T cbExtra, SIZE_T* pcbBaseData)
{
	BYTE* p;
	auto pszText = GetTextW();
	if (pszText)
		m_Info0.cchText = wcslen(pszText);
	else
		m_Info0.cchText = 0;
	int cbText = m_Info0.cchText * sizeof(WCHAR);
	SIZE_T cbMem = sizeof(ECTRLINFO) + m_Info0.cbPic + cbText;
	if (pcbBaseData)
		*pcbBaseData = cbMem;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cbMem + cbExtra);
	if (!hGlobal)
		goto FailAlloc;
	p = (BYTE*)GlobalLock(hGlobal);
	if (!p)
		goto FailLock;
	// �ṹ
	memcpy(p, &m_Info0, sizeof(ECTRLINFO));
	// ͼƬ
	p += sizeof(ECTRLINFO);
	memcpy(p, m_pPicData, m_Info0.cbPic);
	// �ı�
	p += m_Info0.cbPic;
	memcpy(p, pszText, cbText);
	// 
	GlobalUnlock(hGlobal);
	return hGlobal;
FailLock:
	GlobalFree(hGlobal);
FailAlloc:
	return NULL;
}

PWSTR CCtrlBase::GetTextW(SIZE_T* pcb)
{
	if (!m_bInDesignMode)
	{
		int cch = GetWindowTextLengthW(m_hWnd);
		if (cch)
		{
			delete[] m_pszTextW;
			m_pszTextW = new WCHAR[cch + 1];
			GetWindowTextW(m_hWnd, m_pszTextW, cch + 1);
			if (pcb)
				*pcb = (cch + 1) * sizeof(WCHAR);
		}
		else
			if (pcb)
				*pcb = 0u;
	}
	else
		if (pcb)
		{
			if (!m_pszTextW)
				*pcb = 0u;
			else
				*pcb = (wcslen(m_pszTextW) + 1) * sizeof(WCHAR);
		}

	return m_pszTextW;
}

BYTE* CCtrlBase::GetPic(int* pcb)
{
	if (!m_bInDesignMode)
	{
		std::vector<unsigned char> x;
		elibstl::GetDataFromHBIT(m_hbmPic, x);
		if (x.empty()) {
			*pcb = 0;
			return nullptr;
		}

		delete[] m_pPicData;
		m_Info0.cbPic = x.size();
		m_pPicData = new BYTE[m_Info0.cbPic];
		memcpy(m_pPicData, x.data(), m_Info0.cbPic);
	}

	*pcb = m_Info0.cbPic;
	return (BYTE*)m_pPicData;
}

SIZE_T CCtrlBaseSimple::InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID, UINT uID, HWND hParent)
{
	m_bInDesignMode = bInDesignMode;
	m_dwWinFormID = dwWinFormID;
	m_dwUnitID = dwUnitID;
	m_uID = uID;
	m_hParent = hParent;

	if (pAllData)
	{
		memcpy(&m_Info0, pAllData, sizeof(ECTRLINFOSMP));
	}

	m_Info0.iVer = DATA_VER_BASE_SIMPLE_1;

	if (pAllData)
		return sizeof(ECTRLINFOSMP);
	else
		return 0;
}

HGLOBAL CCtrlBaseSimple::FlattenInfoBase0(SIZE_T cbExtra, SIZE_T* pcbBaseData)
{
	BYTE* p;
	if (pcbBaseData)
		*pcbBaseData = sizeof(ECTRLINFOSMP);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(ECTRLINFOSMP) + cbExtra);
	if (!hGlobal)
		goto FailAlloc;
	p = (BYTE*)GlobalLock(hGlobal);
	if (!p)
		goto FailLock;
	// �ṹ
	memcpy(p, &m_Info0, sizeof(ECTRLINFOSMP));

	GlobalUnlock(hGlobal);
	return hGlobal;
FailLock:
	GlobalFree(hGlobal);
FailAlloc:
	return NULL;
}
ESTL_NAMESPACE_END