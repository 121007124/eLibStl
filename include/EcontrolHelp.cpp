#include"EcontrolHelp.h"
#include <CommCtrl.h>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


vector<unsigned char> GetDataFromHBIT(HBITMAP hBitmap)
{
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
	vector<unsigned char> head((unsigned char*)&bmfhdr, (unsigned char*)&bmfhdr + sizeof(BITMAPFILEHEADER));
	vector<unsigned char> body((unsigned char*)lpbi, (unsigned char*)lpbi + dwdibsize);

	//��� 
	GlobalUnlock(hdib);
	GlobalFree(hdib);
	std::vector<unsigned char> result;
	result.reserve(head.size() + body.size());
	result.insert(result.end(), head.begin(), head.end());
	result.insert(result.end(), body.begin(), body.end());
	return result;
}




PCONTROL_STRUCT_BASE DefControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LPCWSTR propName, WNDPROC& oldProc)
{
	// ������Լ�������ݵĻ�, ����ĳɻ�ȡ�Լ�������
	PCONTROL_STRUCT_BASE pBase = (PCONTROL_STRUCT_BASE)GetPropW(hWnd, propName);
	if (!pBase) return 0;
	oldProc = pBase->oldProc;
	switch (message)
	{
	case WM_STYLECHANGED:
	{
		// ��¼���ڵ���ʽ
		LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
		if (wParam == GWL_STYLE)
			pBase->style = ss->styleNew;
		else
			pBase->styleEx = ss->styleNew;
		break;
	}
	case WM_DESTROY:
	{
		// ������Լ�������ݵĻ�, ����ĳ�ɾ���Լ�������
		RemovePropW(hWnd, propName);
		// ȡ�����໯, ��ȡ���Ļ��رմ��ڻ����
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)pBase->oldProc);
		delete pBase;
		pBase = 0;
		break;
	}
	// ������Դ������ͨ�õ���Ϣ, ���� WM_WINDOWPOSCHANGED �Ѵ���λ�ü�¼����
	// �л��� WM_SETFOCUS/WM_KILLFOCUS, �Ѵ����Ƿ��н����¼����
	// �����ͨ����Ϣ������չ
	default:
		break;
	}
	return pBase;
}




static BOOL CALLBACK MyInputBoxDlgProcW(HWND hwndDlg,  // handle to dialog box 
	UINT uMsg,     // message 
	WPARAM wParam, // first message parameter 
	LPARAM lParam  // second message parameter 
)
{
	HWND hwndEDIT = GetDlgItem(hwndDlg, 1001);
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		::SetWindowTextW(hwndDlg, L"�����������ı�:");
		::SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, 0);
		::SetWindowTextW(hwndEDIT, (LPCWSTR)lParam);
	}

	break;
	case WM_COMMAND:
	{
		INT nID = wParam & 0xffff;
		if (nID == IDCANCEL)
			DestroyWindow(hwndDlg);
		else if (nID == IDOK) {


			size_t nLen = ::GetWindowTextLengthW(hwndEDIT);
			if (nLen > 0)
			{
				wchar_t* Editstr = new wchar_t[nLen + 1]{ 0 };
				GetWindowTextW(hwndEDIT, Editstr, nLen + 1);
				wstring* result = (wstring*)::GetWindowLongPtrW(hwndDlg, GWLP_USERDATA);
				*result = wstring(Editstr);
				delete[]Editstr;
			}
			DestroyWindow(hwndDlg);
		}
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
void EnableNewCommonControls()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);
}
wstring MyInputBox(wstring  title)
{
	//�Ի��������ģ�壬��������MFC
	const  BYTE MyInputBoxDialogTemplateData[] = {
	0x01,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x08,0xC8,0x80,
	0x03,0x00,0x00,0x00,0x00,0x00,0x5F,0x01,0xE2,0x00,0x00,0x00,0x00,0x00,0xF7,0x8B,
	0x93,0x8F,0x65,0x51,0x1A,0xFF,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x8B,0x5B,
	0x53,0x4F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x00,0x81,0x50,
	0x05,0x00,0x03,0x00,0x53,0x01,0xC0,0x00,0xE9,0x03,0x00,0x00,0xFF,0xFF,0x81,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x50,
	0x63,0x00,0xC7,0x00,0x37,0x00,0x13,0x00,0x01,0x00,0x00,0x00,0xFF,0xFF,0x80,0x00,
	0x6E,0x78,0xA4,0x8B,0x93,0x8F,0x65,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0xBB,0x00,0xC7,0x00,0x37,0x00,0x13,0x00,
	0x02,0x00,0x00,0x00,0xFF,0xFF,0x80,0x00,0xD6,0x53,0x88,0x6D,0x00,0x00,0x00,0x00
	}
	;
	wstring result;
	HWND hDlg = CreateDialogIndirectParamW(::GetModuleHandleW(NULL), (LPCDLGTEMPLATE)MyInputBoxDialogTemplateData, 0, (DLGPROC)MyInputBoxDlgProcW, (LPARAM)title.c_str());
	::SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)&result);
	//result = temp;
	if (hDlg) {
		ShowWindow(hDlg, SW_SHOW);
		UpdateWindow(hDlg);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return result;
}
