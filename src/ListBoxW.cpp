#include "EcontrolHelp.h"

#define SCID_LISTBOX		20230521'01u
#define SCID_LISTBOXPARENT	20230521'02u

ESTL_NAMESPACE_BEGIN

struct LBITEMCOMMINFO
{
	int idxImage;
	int cyItem;
	COLORREF crText;
	COLORREF crBK;
	COLORREF crSelText;
	COLORREF crSelBK;
	LPARAM lParam;
	BITBOOL bChecked : 1;
	BITBOOL bDisabled : 1;
};

// ֻ��������ʱ������Ϣ
struct LBITEMINFO
{
	CSimpleRefStrW rsCaption;
	CSimpleRefStrW rsTip;
	HBRUSH hbrBK;
	HBRUSH hbrSelBK;
	LBITEMCOMMINFO Info;
};

// ֻ������ƶԻ���
struct LBITEMINFO_DESIGN
{
	PWSTR pszTip;
	LBITEMCOMMINFO Info;
};
/*
* �汾1���ݲ��֣�
* LBITEMSDATAHEADER_MEM�ṹ
* {
* LBITEMHEADER_MEM�ṹ
* ����
* ��ʾ�ı�
* }
*/
#define DATA_VER_LBITEMS_1 1
// ��Ŀ����ͷ��ֻ���ڱ�������
struct LBITEMSDATAHEADER_MEM
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	int cItems;				// ��Ŀ��
};
// ��Ŀ����ͷ����������ʾ�ı�������󣨶�������βNULL�����ڳ�ʼ��ʱ������Ŀ����ֻ���ڱ�������
struct LBITEMHEADER_MEM
{
	int cchCaption;			// �����ַ�����������βNULL
	int cchTip;				// ��ʾ�ַ�����������βNULL
	LBITEMCOMMINFO Info;
};

struct EDLGCTX_LISTBOXITEMS :public EzDlg::EDLGCTX_BASE
{
	BYTE** ppItemsData;
	SIZE_T* pcbItems;
	BYTE* pInitData;
};

#define WCN_LISTBOXDESIGN L"eLibStl.WndClass.ListBoxItemsDesign"

static ATOM s_atomListBoxItemsDesign = 0;

#define IDC_LB_ITEMS		101
#define IDC_BT_OK			201
#define IDC_BT_CANCEL		202
#define IDC_BT_INSERTF		203
#define IDC_BT_INSERTB		204
#define IDC_BT_DEL			205
#define IDC_ED_CAPTION		301
#define IDC_ED_TIP			302
#define IDC_ED_IMAGEIDX		303
#define IDC_ED_ITEMDATA		304
#define IDC_CB_CHECKED		401
#define IDC_CB_DISABLE		402
#define IDC_CLP_TEXT		501
#define IDC_CLP_BK			502
#define IDC_CLP_SELTEXT		503
#define IDC_CLP_SELBK		504

#define CWM_CLRPICKERNOTIFY	WM_USER + 2

static LRESULT CALLBACK WndProc_ListBoxDesign(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto p = (EDLGCTX_LISTBOXITEMS*)GetWindowLongPtrW(hWnd, 0);
	switch (uMsg)
	{
	case CWM_CLRPICKERNOTIFY:
	{
		HWND hLB = GetDlgItem(hWnd, IDC_LB_ITEMS);
		int iCurrSel = SendMessageW(hLB, LB_GETCURSEL, 0, 0);
		if (iCurrSel == LB_ERR)
			return 0;
		auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
		switch (wParam)
		{
		case IDC_CLP_BK:
			pItem->Info.crBK = lParam;
			break;
		case IDC_CLP_SELBK:
			pItem->Info.crSelBK = lParam;
			break;
		case IDC_CLP_TEXT:
			pItem->Info.crText = lParam;
			break;
		case IDC_CLP_SELTEXT:
			pItem->Info.crSelText = lParam;
			break;
		}
	}
	return 0;

	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		{
			HWND hLB = GetDlgItem(hWnd, IDC_LB_ITEMS);
			int iCurrSel = SendMessageW(hLB, LB_GETCURSEL, 0, 0);
			switch (LOWORD(wParam))
			{
			case IDC_BT_INSERTF:
			case IDC_BT_INSERTB:
			{
				auto pItem = new LBITEMINFO_DESIGN;
				ZeroMemory(pItem, sizeof(LBITEMINFO_DESIGN));
				pItem->Info.idxImage = -1;
				pItem->Info.crBK = pItem->Info.crSelBK = pItem->Info.crText = pItem->Info.crSelText = CLR_DEFAULT;
				int idx = SendMessageW(hLB, LB_INSERTSTRING,
					(LOWORD(wParam) == IDC_BT_INSERTF) ? iCurrSel : iCurrSel + 1, (LPARAM)L"");
				SendMessageW(hLB, LB_SETITEMDATA, idx, (LPARAM)pItem);
				SendMessageW(hLB, LB_SETCURSEL, idx, 0);
				SendMessageW(hWnd, WM_COMMAND, MAKEWPARAM(IDC_LB_ITEMS, LBN_SELCHANGE), (LPARAM)GetDlgItem(hWnd, IDC_LB_ITEMS));
				SetFocus(GetDlgItem(hWnd, IDC_ED_CAPTION));
			}
			return 0;

			case IDC_BT_DEL:
			{
				if (iCurrSel != LB_ERR)
				{
					auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
					delete[] pItem->pszTip;
					delete pItem;
					SendMessageW(hLB, LB_DELETESTRING, iCurrSel, 0);
				}
			}
			return 0;

			case IDC_CB_CHECKED:
			{
				if (iCurrSel == LB_ERR)
					break;
				auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
				pItem->Info.bChecked = (SendMessageW((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
			return 0;

			case IDC_CB_DISABLE:
			{
				if (iCurrSel == LB_ERR)
					break;
				auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
				pItem->Info.bDisabled = (SendMessageW((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
			return 0;

			case IDC_BT_OK:
			{
				int cItems = SendMessageW(hLB, LB_GETCOUNT, 0, 0);
				if (cItems <= 0)
				{
					*p->ppItemsData = NULL;
					p->bOK = TRUE;
					EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
					return 0;
				}

				int* pcchCaption = new int[cItems];
				int* pcchTip = new int[cItems];

				int iTemp;
				int cchTotal = 0;
				LBITEMINFO_DESIGN* pItem;
				for (int i = 0; i < cItems; ++i)
				{
					iTemp = SendMessageW(hLB, LB_GETTEXTLEN, i, 0) + 1;
					pcchCaption[i] = iTemp;
					cchTotal += iTemp;

					pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
					if (pItem->pszTip)
						iTemp = wcslen(pItem->pszTip) + 1;
					else
						iTemp = 1;
					pcchTip[i] = iTemp;
					cchTotal += iTemp;
				}

				SIZE_T cbTotal =
					sizeof(LBITEMSDATAHEADER_MEM) +
					sizeof(LBITEMHEADER_MEM) * cItems +
					cchTotal * sizeof(WCHAR);
				BYTE* pData = new BYTE[cbTotal];
				*p->pcbItems = cbTotal;
				*p->ppItemsData = pData;
				auto pHeader = (LBITEMSDATAHEADER_MEM*)pData;
				pHeader->iVer = DATA_VER_LBITEMS_1;
				pHeader->dwReserved = 0;
				pHeader->cItems = cItems;

				pData += sizeof(LBITEMSDATAHEADER_MEM);
				LBITEMHEADER_MEM* pItemHeader;
				for (int i = 0; i < cItems; ++i)
				{
					pItemHeader = (LBITEMHEADER_MEM*)pData;
					pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, i, 0);

					pItemHeader->cchCaption = pcchCaption[i];
					pItemHeader->cchTip = pcchTip[i];
					pItemHeader->Info = pItem->Info;

					pData += sizeof(LBITEMHEADER_MEM);

					SendMessageW(hLB, LB_GETTEXT, i, (LPARAM)pData);
					pData += (pcchCaption[i] * sizeof(WCHAR));

					if (pItem->pszTip)
						wcscpy((PWSTR)pData, pItem->pszTip);
					else
						*(PWSTR)pData = L'\0';
					pData += (pcchTip[i] * sizeof(WCHAR));
				}

				delete[] pcchCaption;
				delete[] pcchTip;

				p->bOK = TRUE;
				EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
			}
			return 0;

			case IDC_BT_CANCEL:
			{
				p->bOK = FALSE;
				EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
			}
			return 0;
			}
		}
		return 0;

		case EN_CHANGE:
		{
			HWND hLB = GetDlgItem(hWnd, IDC_LB_ITEMS);
			int iCurrSel = SendMessageW(hLB, LB_GETCURSEL, 0, 0);
			if (iCurrSel == LB_ERR)
				break;
			switch (LOWORD(wParam))
			{
			case IDC_ED_CAPTION:
			{
				QuickGetWindowTextW(pszText, (HWND)lParam);
				SendMessageW(hLB, WM_SETREDRAW, FALSE, 0);
				SetLBText(hLB, iCurrSel, pszText);
				SendMessageW(hLB, WM_SETREDRAW, TRUE, 0);
				QuickGetWindowTextWFree(pszText);
			}
			break;

			case IDC_ED_TIP:
			{
				auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
				int cch = GetWindowTextLengthW((HWND)lParam);
				delete[] pItem->pszTip;
				if (cch)
				{
					pItem->pszTip = new WCHAR[cch + 1];
					GetWindowTextW((HWND)lParam, pItem->pszTip, cch + 1);
				}
				else
					pItem->pszTip = NULL;
			}
			break;

			case IDC_ED_IMAGEIDX:
			{
				auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
				QuickGetWindowTextW(pszText, (HWND)lParam);
				if (pszText)
					pItem->Info.idxImage = _wtoi(pszText);
				else
					pItem->Info.idxImage = -1;
				QuickGetWindowTextWFree(pszText);
			}
			break;

			case IDC_ED_ITEMDATA:
			{
				auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
				QuickGetWindowTextW(pszText, (HWND)lParam);
				if (pszText)
					pItem->Info.lParam = _wtoi(pszText);
				else
					pItem->Info.lParam = 0;
				QuickGetWindowTextWFree(pszText);
			}
			break;
			}
			
		}
		return 0;

		case LBN_SELCHANGE:
		{
			if (LOWORD(wParam) != IDC_LB_ITEMS)
				break;
			HWND hLB = (HWND)lParam;
			int iCurrSel = SendMessageW(hLB, LB_GETCURSEL, 0, 0);
			if (iCurrSel == LB_ERR)
				break;
			// ����
			int cch = SendMessageW(hLB, LB_GETTEXTLEN, iCurrSel, 0);
			PWSTR pszText;
			if (cch)
			{
				pszText = (PWSTR)_malloca((cch + 1) * sizeof(WCHAR));
				SendMessageW(hLB, LB_GETTEXT, iCurrSel, (LPARAM)pszText);
			}
			else
				pszText = NULL;
			SetDlgItemTextW(hWnd, IDC_ED_CAPTION, pszText);
			_freea(pszText);
			// ��ʾ�ı�
			auto pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, iCurrSel, 0);
			SetDlgItemTextW(hWnd, IDC_ED_TIP, pItem->pszTip);
			// ͼ������
			WCHAR szText[32]{};
			swprintf(szText, L"%d", pItem->Info.idxImage);
			SetDlgItemTextW(hWnd, IDC_ED_IMAGEIDX, szText);
			// ������ֵ
			swprintf(szText, L"%d", pItem->Info.lParam);
			SetDlgItemTextW(hWnd, IDC_ED_ITEMDATA, szText);
			// ѡ��
			CheckDlgButton(hWnd, IDC_CB_CHECKED, pItem->Info.bChecked ? BST_CHECKED : BST_UNCHECKED);
			// ��ֹ
			CheckDlgButton(hWnd, IDC_CB_DISABLE, pItem->Info.bDisabled ? BST_CHECKED : BST_UNCHECKED);
			// ��ɫ
			ClrPicker::SetColor(GetDlgItem(hWnd, IDC_CLP_BK), pItem->Info.crBK);
			ClrPicker::SetColor(GetDlgItem(hWnd, IDC_CLP_TEXT), pItem->Info.crText);
			ClrPicker::SetColor(GetDlgItem(hWnd, IDC_CLP_SELBK), pItem->Info.crSelBK);
			ClrPicker::SetColor(GetDlgItem(hWnd, IDC_CLP_SELTEXT), pItem->Info.crSelText);
		}
		return 0;
		}
	}
	break;

	case WM_CREATE:
	{
#pragma region �����ؼ�
		constexpr int DlgPadding = 8;
		constexpr int cxLB = 240;
		constexpr int cyLB = 240;
		constexpr int xStatic = 256;
		constexpr int xStatic2 = 400;
		constexpr int cxStatic = 70;
		constexpr int cxStatic2 = 95;
		constexpr int cyStatic = 26;
		constexpr int StaticLineStep = 40;
		constexpr int xEdit = 329;
		constexpr int cxEdit1 = 256;
		constexpr int cxEdit2 = 60;
		constexpr int xBTOK = 384;
		constexpr int cxBT = 96;
		constexpr int cyBT = 32;
		constexpr int yBT = DlgPadding * 2 + cyLB;
		constexpr int xTip = 361;
		constexpr int yTip = 167;
		constexpr int cxTip = 216;
		constexpr int cyTip = 80;
		constexpr int xClrPicker = 495;
		constexpr int cxClrPicker = 90;
		constexpr int cyClrPicker = 300;

		p = (EDLGCTX_LISTBOXITEMS*)((CREATESTRUCTW*)lParam)->lpCreateParams;
		SetWindowLongPtrW(hWnd, 0, (LONG_PTR)p);
		p->hFont = EzFont(L"΢���ź�", 10);

		HWND hCtrl;
		HWND hLB = CreateWindowExW(0, WC_LISTBOXW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_GROUP | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
			DlgPadding, DlgPadding, cxLB, cyLB, hWnd, (HMENU)IDC_LB_ITEMS, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"���⣺", WS_CHILD | WS_VISIBLE,
			xStatic, DlgPadding, cxStatic, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"��ʾ�ı���", WS_CHILD | WS_VISIBLE,
			xStatic, DlgPadding + StaticLineStep, cxStatic, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"ͼ��������", WS_CHILD | WS_VISIBLE,
			xStatic, DlgPadding + StaticLineStep * 2, cxStatic, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"��Ŀ��ֵ��", WS_CHILD | WS_VISIBLE,
			xStatic, DlgPadding + StaticLineStep * 3, cxStatic, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"�ı���ɫ��", WS_CHILD | WS_VISIBLE,
			xStatic2, DlgPadding + StaticLineStep * 2, cxStatic2, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"������ɫ��", WS_CHILD | WS_VISIBLE,
			xStatic2, DlgPadding + StaticLineStep * 3, cxStatic2, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"ѡ���ı���ɫ��", WS_CHILD | WS_VISIBLE,
			xStatic2, DlgPadding + StaticLineStep * 4, cxStatic2, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_STATICW, L"ѡ�б�����ɫ��", WS_CHILD | WS_VISIBLE,
			xStatic2, DlgPadding + StaticLineStep * 5, cxStatic2, cyStatic, hWnd, NULL, NULL, NULL);

		hCtrl = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | ES_AUTOHSCROLL | ES_MULTILINE,
			xEdit, DlgPadding, cxEdit1, cyStatic, hWnd, (HMENU)IDC_ED_CAPTION, NULL, NULL);
		EzDlg::TabRepairInstall(hCtrl);

		hCtrl = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_MULTILINE,
			xEdit, DlgPadding + StaticLineStep, cxEdit1, cyStatic, hWnd, (HMENU)IDC_ED_TIP, NULL, NULL);
		EzDlg::TabRepairInstall(hCtrl);

		hCtrl = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_MULTILINE,
			xEdit, DlgPadding + StaticLineStep * 2, cxEdit2, cyStatic, hWnd, (HMENU)IDC_ED_IMAGEIDX, NULL, NULL);
		EzDlg::TabRepairInstall(hCtrl);

		hCtrl = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_MULTILINE,
			xEdit, DlgPadding + StaticLineStep * 3, cxEdit2, cyStatic, hWnd, (HMENU)IDC_ED_ITEMDATA, NULL, NULL);
		EzDlg::TabRepairInstall(hCtrl);

		hCtrl = ClrPicker::Create(xClrPicker, DlgPadding + StaticLineStep * 2, cxClrPicker, cyClrPicker,
			WS_TABSTOP | WS_GROUP, 0, IDC_CLP_TEXT, hWnd, CWM_CLRPICKERNOTIFY);

		hCtrl = ClrPicker::Create(xClrPicker, DlgPadding + StaticLineStep * 3, cxClrPicker, cyClrPicker,
			WS_TABSTOP, 0, IDC_CLP_BK, hWnd, CWM_CLRPICKERNOTIFY);

		hCtrl = ClrPicker::Create(xClrPicker, DlgPadding + StaticLineStep * 4, cxClrPicker, cyClrPicker,
			WS_TABSTOP, 0, IDC_CLP_SELTEXT, hWnd, CWM_CLRPICKERNOTIFY);

		hCtrl = ClrPicker::Create(xClrPicker, DlgPadding + StaticLineStep * 5, cxClrPicker, cyClrPicker,
			WS_TABSTOP, 0, IDC_CLP_SELBK, hWnd, CWM_CLRPICKERNOTIFY);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ѡ��", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_AUTOCHECKBOX,
			xStatic, DlgPadding + StaticLineStep * 4, cxEdit2, cyStatic, hWnd, (HMENU)IDC_CB_CHECKED, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"��ֹ", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
			xEdit, DlgPadding + StaticLineStep * 4, cxEdit2, cyStatic, hWnd, (HMENU)IDC_CB_DISABLE, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"��ǰ����(&F)", WS_CHILD | WS_TABSTOP | WS_GROUP | WS_VISIBLE,
			DlgPadding, yBT, cxBT, cyBT, hWnd, (HMENU)IDC_BT_INSERTF, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"������(&B)", WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			DlgPadding * 2 + cxBT, yBT, cxBT, cyBT, hWnd, (HMENU)IDC_BT_INSERTB, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ɾ��(&D)", WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			DlgPadding * 3 + cxBT * 2, yBT, cxBT, cyBT, hWnd, (HMENU)IDC_BT_DEL, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ȷ��(&O)", WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
			xBTOK, yBT, cxBT, cyBT, hWnd, (HMENU)IDC_BT_OK, NULL, NULL);

		hCtrl = CreateWindowExW(0, WC_BUTTONW, L"ȡ��(&C)", WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			xBTOK + DlgPadding + cxBT, yBT, cxBT, cyBT, hWnd, (HMENU)IDC_BT_CANCEL, NULL, NULL);

		EzDlg::SetFontForWndAndCtrl(hWnd, p->hFont);
#pragma endregion �����ؼ�
		BYTE* pData = p->pInitData;
		if (!pData)
			return 0;

		auto pHeader = (LBITEMSDATAHEADER_MEM*)pData;

		int cItems = pHeader->cItems;
		if (cItems <= 0)
			return 0;

		SendMessageW(hLB, WM_SETREDRAW, FALSE, 0);
		SendMessageW(hLB, LB_INITSTORAGE, cItems, 48);
		int iTemp;
		int cchTotal = 0;
		LBITEMINFO_DESIGN* pItem;
		pData += sizeof(LBITEMSDATAHEADER_MEM);
		LBITEMHEADER_MEM* pItemHeader;
		for (int i = 0; i < cItems; ++i)
		{
			pItemHeader = (LBITEMHEADER_MEM*)pData;
			pData += sizeof(LBITEMHEADER_MEM);
			// ��Ŀͷ
			pItem = new LBITEMINFO_DESIGN;
			ZeroMemory(pItem, sizeof(LBITEMINFO_DESIGN));
			pItem->Info = pItemHeader->Info;
			// �ı�
			int idx = SendMessageW(hLB, LB_ADDSTRING,0, (LPARAM)pData);
			SendMessageW(hLB, LB_SETITEMDATA, idx, (LPARAM)pItem);
			// ��ʾ�ı�
			pData += (pItemHeader->cchCaption * sizeof(WCHAR));
			DupStringForNewDeleteW(pItem->pszTip, (PCWSTR)pData);
			pData += (pItemHeader->cchTip * sizeof(WCHAR));
		}
		SendMessageW(hLB, LB_SETCURSEL, 0, 0);
		SendMessageW(hWnd, WM_COMMAND, MAKEWPARAM(IDC_LB_ITEMS, LBN_SELCHANGE), (LPARAM)GetDlgItem(hWnd, IDC_LB_ITEMS));
		SetFocus(GetDlgItem(hWnd, IDC_ED_CAPTION));

		SendMessageW(hLB, WM_SETREDRAW, TRUE, 0);
	}
	return 0;

	case WM_CLOSE:
	{
		p->bOK = FALSE;
		EzDlg::DlgEndModel(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
	}
	return 0;

	case WM_DESTROY:
	{
		HWND hLB = GetDlgItem(hWnd, IDC_LB_ITEMS);
		LBITEMINFO_DESIGN* pItem;
		for (int i = SendMessageW(hLB, LB_GETCOUNT, 0, 0) - 1; i >= 0; --i)
		{
			pItem = (LBITEMINFO_DESIGN*)SendMessageW(hLB, LB_GETITEMDATA, i, 0);
			delete[] pItem->pszTip;
			delete pItem;
		}
		DeleteObject(p->hFont);
		EzDlg::DlgModelOnDestroy(hWnd, p->hwndEMain, p->bEMainWndInitEnabled);
		PostQuitMessage(0);
	}
	return 0;
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

BOOL ShowListBoxDesignDlg(BYTE** ppItemsData, SIZE_T* pcbItems, BYTE* pInitData)
{
	auto pCtx = EzDlg::EIDEDlgPreShow<EDLGCTX_LISTBOXITEMS>(&s_atomListBoxItemsDesign, WCN_LISTBOXDESIGN, WndProc_ListBoxDesign);
	if (ppItemsData)
		*ppItemsData = NULL;
	if (pcbItems)
		*pcbItems = 0u;
	pCtx->pInitData = pInitData;
	pCtx->ppItemsData = ppItemsData;
	pCtx->pcbItems = pcbItems;

	EzDlg::EIDEDlgShow(WCN_LISTBOXDESIGN, L"�б����Ŀ����", 500,500, 600, 325, 
		WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_POPUP, pCtx);

	BOOL bOK = pCtx->bOK;
	delete pCtx;
	return bOK;
}

// �б��
/*
* �汾1���ݲ��֣�
* ELISTBOXDATA�ṹ
* ��Ŀ����
* ͼƬ��
* Ŀ¼��������βNULL��
* �ļ���������������βNULL��
*/
#define DATA_VER_LISTBOX_1	1
struct ELISTBOXDATA
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	int idxCurrSel;			// ����ѡ��
	int cyItem;				// �и�
	COLORREF crText;		// �ı���ɫ
	COLORREF crBK;			// ������ɫ
	COLORREF crSelText;		// ѡ���ı���ɫ
	COLORREF crSelBK;		// ѡ�񱳾���ɫ
	int iAlignH;			// �������
	int iAlignV;			// �������
	UINT uFileAttr;			// �ļ���������
	FILETIME ftMinTime;		// �ļ���Сʱ�䣬����Э������ʱ
	FILETIME ftMaxTime;		// �ļ����ʱ�䣬����Э������ʱ
	SIZE_T cbItems;			// �������ݳ���
	SIZE_T cbImageList;		// ͼƬ�鳤��
	int cchDir;				// Ŀ¼�ַ���
	int cchFilePattern;		// �ļ��������ַ���
	BITBOOL bMultiColumn : 1;	// ������չ
	BITBOOL bAutoSort : 1;	// �Զ�����
	BITBOOL bMultiSel : 1;	// ����ѡ
	BITBOOL bExtSel : 1;	// ������ѡ
	BITBOOL bToolTip : 1;	// ������ʾ
	BITBOOL bUseTabStop: 1;	// �����Ʊ��
	BITBOOL bIntegralHeight: 1;	// ȡ���ؼ��߶�
	BITBOOL bDisableNoScroll: 1;// ��ʾ��ֹ�Ĺ�����
	BITBOOL bEllipsis: 1;	// ʡ�Ժ�
};

class CListBox :public CCtrlBase
{
	SUBCLASS_MGR_DECL(CListBox)
private:
	ELISTBOXDATA m_Info{};

	BYTE* m_pItems = NULL;

	BOOL m_bImageListNeedDel = FALSE;
	HIMAGELIST m_hImageList = NULL;
	HGLOBAL m_hglbImageListData = NULL;
	BOOL m_bILDataLocked = FALSE;
	BYTE* m_pImageListData = NULL;

	PWSTR m_pszDir = NULL;
	PWSTR m_pszFilePattern = NULL;

	HBRUSH m_hbrBK = NULL;
	HBRUSH m_hbrSelBK = NULL;

	std::vector<LBITEMINFO> m_ItemsInfo{};

	void AddFile()
	{
		if (!m_Info.cchDir)
			return;
		LBDeleteString(-1);

		PWSTR pszPath = (PWSTR)_malloca((
			m_Info.cchDir +
			m_Info.cchFilePattern +
			5/*һ����б�ܣ�����*.*��ͨ���Ϊ��ʱ�ã���һ����βNULL*/) * sizeof(WCHAR));
		assert(pszPath);// ��������
		wcscpy(pszPath, m_pszDir);
		PWSTR pszTemp = pszPath + m_Info.cchDir;// ָ��ָ��Ŀ¼�ĺ��棬�����滻ͨ���

		PWSTR pszFilePattern;
		if (m_Info.cchFilePattern && m_pszFilePattern)
			pszFilePattern = m_pszFilePattern;
		else
		{
#pragma warning(push)
#pragma warning(disable:6255)// ���þ��棺���Ǹ���_malloca
			pszFilePattern = (PWSTR)_alloca(4 * sizeof(WCHAR));
#pragma warning(pop)
			assert(pszFilePattern);// ��������
			wcscpy(pszFilePattern, L"*.*");
		}

		WIN32_FIND_DATAW wfd;
		HANDLE hFind;
		PWSTR pszDivPos, pszOld = pszFilePattern;
		while (TRUE)
		{
			pszDivPos = wcsstr(pszFilePattern, L"|");
			if (pszDivPos != pszOld && pszDivPos)// �������
			{
				int cch = pszDivPos - pszOld - 1;
				wcscpy(pszTemp, L"\\");
				wcsncat(pszTemp, pszOld, cch);
				*(pszTemp + cch + 1) = L'\0';
			}
			else if (!pszDivPos)// �Ҳ�����һ���ָ���
			{
				wcscpy(pszTemp, L"\\");
				wcscat(pszTemp, pszOld);
			}
			else// β����pszDivPos==pszOld��
				break;

			LBITEMCOMMINFO CommInfo{};
			pszOld = pszDivPos + 1;
			hFind = FindFirstFileW(pszPath, &wfd);
			if (hFind == INVALID_HANDLE_VALUE)
				if (!pszDivPos)
					break;
				else
					continue;
			do
			{
				if (memcmp(wfd.cFileName, L".", 2 * sizeof(WCHAR)) == 0 ||
					memcmp(wfd.cFileName, L"..", 3 * sizeof(WCHAR)) == 0)
					continue;
				if (!m_Info.uFileAttr || wfd.dwFileAttributes & m_Info.uFileAttr)
				{
					if (m_Info.ftMaxTime == m_Info.ftMinTime || IsFILETIMEZero(&m_Info.ftMaxTime))
						LBAddString(wfd.cFileName, NULL, CommInfo);
					else if (wfd.ftCreationTime > m_Info.ftMinTime && wfd.ftCreationTime < m_Info.ftMaxTime)
						LBAddString(wfd.cFileName, NULL, CommInfo);
				}
			} while (FindNextFileW(hFind, &wfd));
			FindClose(hFind);
			if (!pszDivPos)
				break;
		}

		_freea(pszPath);
	}

	static LRESULT CALLBACK ParentSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		switch (uMsg)
		{
		case WM_MEASUREITEM:
		{
			HWND hCtrl = GetDlgItem(hWnd, wParam);
			if (!m_CtrlSCInfo.count(hCtrl))
				break;
			auto pmis = (MEASUREITEMSTRUCT*)lParam;
			auto p = m_CtrlSCInfo[hCtrl];
			pmis->itemHeight = p->m_Info.cyItem;
			return TRUE;
		}
		break;

		case WM_DRAWITEM:
		{
			auto pdis = (DRAWITEMSTRUCT*)lParam;
			if (!m_CtrlSCInfo.count(pdis->hwndItem))
				break;
			auto p = m_CtrlSCInfo[pdis->hwndItem];
			if (pdis->itemID == -1)
				break;
			auto& Item = p->m_ItemsInfo[pdis->itemID];
			UINT uDTFlags = DT_NOCLIP | DT_SINGLELINE |
				MultiSelect<UINT>(p->m_Info.iAlignH, DT_LEFT, DT_CENTER, DT_VCENTER) |
				MultiSelect<UINT>(p->m_Info.iAlignV, DT_TOP, DT_VCENTER, DT_BOTTOM) |
				(p->m_Info.bEllipsis ? DT_END_ELLIPSIS : 0);
			HDC hDC = pdis->hDC;

			if (IsBitExist(pdis->itemState, ODS_SELECTED))
			{
				if (Item.hbrSelBK)
					FillRect(hDC, &pdis->rcItem, Item.hbrSelBK);
				else if (p->m_hbrSelBK)
					FillRect(hDC, &pdis->rcItem, p->m_hbrSelBK);
				else
					FillRect(hDC, &pdis->rcItem, (HBRUSH)GetSysColorBrush(COLOR_HIGHLIGHT));

				if (Item.Info.crSelText != CLR_DEFAULT)
					SetTextColor(hDC, Item.Info.crSelText);
				else if (p->m_Info.crSelText != CLR_DEFAULT)
					SetTextColor(hDC, p->m_Info.crSelText);
				else
					SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				if (Item.hbrBK)
					FillRect(hDC, &pdis->rcItem, Item.hbrBK);
				else if (p->m_hbrBK)
					FillRect(hDC, &pdis->rcItem, p->m_hbrBK);
				else
					FillRect(hDC, &pdis->rcItem, (HBRUSH)GetSysColorBrush(COLOR_WINDOW));

				if (Item.Info.crText != CLR_DEFAULT)
					SetTextColor(hDC, Item.Info.crText);
				else if (p->m_Info.crText != CLR_DEFAULT)
					SetTextColor(hDC, p->m_Info.crText);
				else
					SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));
			}

			SetBkMode(hDC, TRANSPARENT);
			DrawTextW(hDC, Item.rsCaption, -1, &pdis->rcItem, uDTFlags);
			return TRUE;
		}
		break;

		case WM_DESTROY:
			m_SM.OnParentDestroy(hWnd);
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	static LRESULT CALLBACK CtrlSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto p = (CListBox*)dwRefData;
		switch (uMsg)
		{
		case WM_SHOWWINDOW:
			CHECK_PARENT_CHANGE;
			break;
		case WM_DESTROY:
			m_SM.OnCtrlDestroy(p);
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
public:
	CListBox() = delete;
	CListBox(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase0(pAllData, cbData, bInDesignMode, dwWinFormID, dwUnitID, nID, hParent);

		if (pAllData)
		{
			BYTE* p = (BYTE*)pAllData + cbBaseData;
			memcpy(&m_Info, p, sizeof(ELISTBOXDATA));

			p += sizeof(ELISTBOXDATA);

			DupStreamForNewDelete(m_pItems, p, m_Info.cbItems);
			p += m_Info.cbItems;

			p += m_Info.cbImageList;

			DupStringForNewDeleteW(m_pszDir, (PCWSTR)p, m_Info.cchDir);
			p += m_Info.cchDir * sizeof(WCHAR);

			DupStringForNewDeleteW(m_pszFilePattern, (PCWSTR)p, m_Info.cchFilePattern);
			p += m_Info.cchFilePattern * sizeof(WCHAR);
		}
		else
		{
			m_Info0.iFrame = 1;
			m_Info.crBK = CLR_DEFAULT;
			m_Info.crSelBK = CLR_DEFAULT;
			m_Info.crSelBK = CLR_DEFAULT;
			m_Info.crSelText = CLR_DEFAULT;
			m_Info.cyItem = 16;
			m_Info.idxCurrSel = -1;
			m_Info.iAlignV = 1;
		}
		m_Info.iVer = DATA_VER_LISTBOX_1;

		DWORD dwLBStyle = LBS_OWNERDRAWFIXED | LBS_NODATA;
		if (m_Info.bMultiColumn)
			dwLBStyle |= LBS_MULTICOLUMN;
		if (m_Info.bAutoSort)
			dwLBStyle |= LBS_SORT;
		if (m_Info.bMultiSel)
			dwLBStyle |= LBS_MULTIPLESEL;
		if (m_Info.bExtSel)
			dwLBStyle |= LBS_EXTENDEDSEL;
		if (m_Info.bUseTabStop)
			dwLBStyle |= LBS_USETABSTOPS;
		if (!m_Info.bIntegralHeight)
			dwLBStyle |= LBS_NOINTEGRALHEIGHT;
		if (m_Info.bDisableNoScroll)
			dwLBStyle |= LBS_DISABLENOSCROLL;

		m_hWnd = CreateWindowExW(0, WC_LISTBOXW, NULL, WS_VSCROLL | WS_CHILD | WS_CLIPSIBLINGS | dwLBStyle,
			x, y, cx, cy, hParent, (HMENU)nID, NULL, NULL);
		m_SM.OnCtrlCreate(this);
		m_hParent = hParent;
		SetRedraw(FALSE);
		// �������
		if (m_pszDir)
			AddFile();
		else
			SetItems(m_pItems, m_Info.cbItems, TRUE);

		SetClr(1, m_Info.crBK);
		SetSelClr(1, m_Info.crSelBK);
		SetCurrSel(m_Info.idxCurrSel);
		InitBase0(pAllData);
		if (m_Info.cyItem)// ������InitBase0����
			SetItemHeight(m_Info.cyItem);
		SetRedraw(TRUE);
	}

	void LBInsertString(PCWSTR pszString, PCWSTR pszTip, const LBITEMCOMMINFO& CommInfo, int iPos = -1)
	{
		LBITEMINFO Item{ pszString,pszTip,NULL,NULL,CommInfo };
		if (CommInfo.crBK != CLR_DEFAULT)
			Item.hbrBK = CreateSolidBrush(CommInfo.crBK);
		if (CommInfo.crSelBK != CLR_DEFAULT)
			Item.hbrSelBK = CreateSolidBrush(CommInfo.crSelBK);
		if (iPos < 0)
			m_ItemsInfo.push_back(std::move(Item));
		else
			m_ItemsInfo.insert(m_ItemsInfo.begin() + iPos, std::move(Item));
		SendMessageW(m_hWnd, LB_INSERTSTRING, iPos, (LPARAM)L"");
	}

	void LBAddString(PCWSTR pszString, PCWSTR pszTip, const LBITEMCOMMINFO& CommInfo)
	{
		LBITEMINFO Item{ pszString,pszTip,NULL,NULL,CommInfo };
		if (CommInfo.crBK != CLR_DEFAULT)
			Item.hbrBK = CreateSolidBrush(CommInfo.crBK);
		if (CommInfo.crSelBK != CLR_DEFAULT)
			Item.hbrSelBK = CreateSolidBrush(CommInfo.crSelBK);
		m_ItemsInfo.push_back(std::move(Item));
		SendMessageW(m_hWnd, LB_ADDSTRING, 0, (LPARAM)L"");
	}

	void LBSetItem(int idx, PCWSTR pszString, PCWSTR pszTip, const LBITEMCOMMINFO& CommInfo)
	{
		LBITEMINFO& Item = m_ItemsInfo[idx];
		Item = { pszString,pszTip,NULL,NULL,CommInfo };
		if (CommInfo.crBK != CLR_DEFAULT)
			Item.hbrBK = CreateSolidBrush(CommInfo.crBK);
		if (CommInfo.crSelBK != CLR_DEFAULT)
			Item.hbrSelBK = CreateSolidBrush(CommInfo.crSelBK);
	}

	void LBSetCount(int cItems)
	{
		SendMessageW(m_hWnd, LB_SETCOUNT, cItems, 0);
		m_ItemsInfo.resize(cItems);
	}

	void LBDeleteString(int iPos)
	{
		if (iPos < 0)
		{
			SendMessageW(m_hWnd, LB_RESETCONTENT, 0, 0);
			for (auto& x : m_ItemsInfo)
			{
				DeleteObject(x.hbrBK);
				DeleteObject(x.hbrSelBK);
			}
			m_ItemsInfo.resize(0u);
		}
		else if (iPos >= 0 && iPos < m_ItemsInfo.size())
		{
			SendMessageW(m_hWnd, LB_DELETESTRING, iPos, 0);
			auto Item = m_ItemsInfo[iPos];
			DeleteObject(Item.hbrBK);
			DeleteObject(Item.hbrSelBK);
			m_ItemsInfo.erase(m_ItemsInfo.begin() + iPos);
		}
		Redraw();
	}

	void LBInitStorage(int cItems)
	{
		m_ItemsInfo.reserve(cItems);
		SendMessageW(m_hWnd, LB_INITSTORAGE, cItems, 0);
	}

	void SetItems(BYTE* pItems, SIZE_T cbItems, BOOL bNoCopy)
	{
		//std::vector<LBITEMINFO*> v1{};
		//std::vector<LBITEMINFO> v2{};
		//LBITEMCOMMINFO s{};
		//LBITEMINFO* pp;
		//SetRedraw(FALSE);
		//auto ull = GetTickCount64();
		////v1.reserve(50000);
		//SendMessageW(m_hWnd, LB_INITSTORAGE, 100000, 0);
		//for (int i = 0; i < 100000; ++i)
		//{
		//	pp = new LBITEMINFO{ L"154465565664465",L"5456466646468",0,0,s };
		//	//v1.push_back(pp);
		//	SendMessageW(m_hWnd, LB_ADDSTRING, 0, (LPARAM)L"");
		//	SendMessageW(m_hWnd, LB_SETITEMDATA, i, (LPARAM)pp);
		//}
		//ull = GetTickCount64() - ull;
		//SendMessageW(m_hWnd, LB_RESETCONTENT, 0, 0);
		//ull = GetTickCount64();
		//v2.reserve(100000);
		//SendMessageW(m_hWnd, LB_INITSTORAGE, 100000, 0);
		//for (int i = 0; i < 100000; ++i)
		//{
		//	SendMessageW(m_hWnd, LB_ADDSTRING, 0, (LPARAM)L"");
		//	v2.push_back({ L"154465565664465",L"5456466646468",0,0,s });
		//}
		//ull = GetTickCount64() - ull;
		//SetRedraw(TRUE);
		SetRedraw(FALSE);
		LBDeleteString(-1);
		if (!pItems || !cbItems)
		{
			if (m_bInDesignMode)
			{
				delete[] m_pItems;
				m_pItems = NULL;
				m_Info.cbItems = 0u;
			}
			SetRedraw(TRUE);
			return;
		}
		
		BYTE* pData = pItems;
		LBITEMSDATAHEADER_MEM* pHeader;
		LBITEMHEADER_MEM* pItemHeader;
		PCWSTR pszCaption;
		PCWSTR pszTip;
		if (m_bInDesignMode)
			if (bNoCopy)
			{
				if (m_pItems != pItems)
					delete[] m_pItems;
				m_pItems = pItems;
				m_Info.cbItems = cbItems;
			}
			else
				m_Info.cbItems = DupStreamForNewDelete(m_pItems, pItems, cbItems);

		pHeader = (LBITEMSDATAHEADER_MEM*)pData;
		pData += sizeof(LBITEMSDATAHEADER_MEM);
		int cItems = pHeader->cItems;
		LBSetCount(cItems);
		for (int i = 0; i < cItems; ++i)
		{
			pItemHeader = (LBITEMHEADER_MEM*)pData;
			pData += sizeof(LBITEMHEADER_MEM);

			pszCaption = (PCWSTR)pData;
			pData += (pItemHeader->cchCaption * sizeof(WCHAR));
			pszTip = (PCWSTR)pData;
			pData += (pItemHeader->cchTip * sizeof(WCHAR));

			LBSetItem(i, pszCaption, pszTip, pItemHeader->Info);
		}

		SetRedraw(TRUE);
		Redraw();

		if (!m_bInDesignMode && bNoCopy)
			delete[] pItems;
	}

	BYTE* GetItems()
	{
		if (m_bInDesignMode)
			return m_pItems;

		delete[] m_pItems;
		m_Info.cbItems = 0u;
		int cItems = m_ItemsInfo.size();
		if (!cItems)
			return NULL;

		int cchTotal = 0;
		for (auto& x : m_ItemsInfo)
		{
			cchTotal += (x.rsCaption.m_cchText + 1);
			cchTotal += (x.rsTip.m_cchText + 1);
		}

		SIZE_T cbTotal =
			sizeof(LBITEMSDATAHEADER_MEM) +
			sizeof(LBITEMHEADER_MEM) * cItems +
			cchTotal * sizeof(WCHAR);
		m_Info.cbItems = cbTotal;
		BYTE* pData = new BYTE[cbTotal];
		m_pItems = pData;

		auto pHeader = (LBITEMSDATAHEADER_MEM*)pData;
		pHeader->iVer = DATA_VER_LBITEMS_1;
		pHeader->dwReserved = 0;
		pHeader->cItems = cItems;

		pData += sizeof(LBITEMSDATAHEADER_MEM);
		LBITEMHEADER_MEM* pItemHeader;
		for (int i = 0; i < cItems; ++i)
		{
			pItemHeader = (LBITEMHEADER_MEM*)pData;

			pItemHeader->cchCaption = m_ItemsInfo[i].rsCaption.m_cchText;
			pItemHeader->cchTip = m_ItemsInfo[i].rsTip.m_cchText;
			pItemHeader->Info = m_ItemsInfo[i].Info;

			pData += sizeof(LBITEMHEADER_MEM);

			m_ItemsInfo[i].rsCaption.CopyTo((PWSTR)pData);
			pData += (pItemHeader->cchCaption * sizeof(WCHAR));

			m_ItemsInfo[i].rsTip.CopyTo((PWSTR)pData);
			pData += (pItemHeader->cchTip * sizeof(WCHAR));
		}

		return m_pItems;
	}

	eStlInline void SetCurrSel(int idx)
	{
		m_Info.idxCurrSel = idx;
		SendMessageW(m_hWnd, LB_SETCURSEL, idx, 0);
	}

	eStlInline int GetCurrSel()
	{
		if (m_bInDesignMode)
			return m_Info.idxCurrSel;
		else
			return SendMessageW(m_hWnd, LB_GETCURSEL, 0, 0);
	}

	eStlInline void SetMultiColumn(BOOL bMultiColumn)
	{
		m_Info.bMultiColumn = bMultiColumn;
	}

	eStlInline BOOL GetMultiColumn()
	{
		if (m_bInDesignMode)
			return m_Info.bMultiColumn;
		else
			return IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_MULTICOLUMN);
	}

	eStlInline void SetItemHeight(int cy)
	{
		m_Info.cyItem = cy;
		SendMessageW(m_hWnd, LB_SETITEMHEIGHT, 0, cy);
	}

	eStlInline int GetItemHeight()
	{
		if (m_bInDesignMode)
			return m_Info.cyItem;
		else
			return SendMessageW(m_hWnd, LB_GETITEMHEIGHT, 0, 0);
	}

	void SetClr(int idx, COLORREF cr)
	{
		if (idx)
		{
			m_Info.crBK = cr;
			DeleteObject(m_hbrBK);
			if (cr != CLR_DEFAULT)
				m_hbrBK = CreateSolidBrush(cr);
			else
				m_hbrBK = NULL;
		}
		else
			m_Info.crText = cr;
		Redraw();
	}

	eStlInline int GetClr(int idx)
	{
		if (idx)
			return m_Info.crBK;
		else
			return m_Info.crText;
	}

	eStlInline void SetAutoSort(BOOL bAutoSort)
	{
		m_Info.bAutoSort = bAutoSort;
	}

	eStlInline BOOL GetAutoSort()
	{
		if (m_bInDesignMode)
			return m_Info.bAutoSort;
		else
			return IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_SORT);
	}

	eStlInline void SetMultiSel(BOOL bMultiSel)
	{
		m_Info.bMultiSel = bMultiSel;
	}

	eStlInline BOOL GetMultiSel()
	{
		if (m_bInDesignMode)
			return m_Info.bMultiSel;
		else
			return IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_MULTIPLESEL);
	}

	eStlInline void SetExtSel(BOOL bExtSel)
	{
		m_Info.bExtSel = bExtSel;
	}

	eStlInline BOOL GetExtSel()
	{
		if (m_bInDesignMode)
			return m_Info.bExtSel;
		else
			return IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_EXTENDEDSEL);
	}

	eStlInline void SetToolTip(BOOL bToolTip)
	{
		m_Info.bToolTip = bToolTip;
	}

	eStlInline BOOL GetToolTip()
	{
		return m_Info.bToolTip;
	}

	eStlInline void SetUseTabStop(BOOL bUseTabStop)
	{
		m_Info.bUseTabStop = bUseTabStop;
	}

	eStlInline BOOL GetUseTabStop()
	{
		if (m_bInDesignMode)
			return m_Info.bUseTabStop;
		else
			return IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_USETABSTOPS);
	}

	eStlInline void SetIntegralHeight(BOOL bIntegralHeight)
	{
		m_Info.bIntegralHeight = bIntegralHeight;
	}

	eStlInline BOOL GetIntegralHeight()
	{
		if (m_bInDesignMode)
			return m_Info.bIntegralHeight;
		else
			return !IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_NOINTEGRALHEIGHT);
	}

	eStlInline void SetDisableNoScroll(BOOL bDisableNoScroll)
	{
		m_Info.bDisableNoScroll = bDisableNoScroll;
	}

	eStlInline BOOL GetDisableNoScroll()
	{
		if (m_bInDesignMode)
			return m_Info.bDisableNoScroll;
		else
			return !IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), LBS_DISABLENOSCROLL);
	}

	eStlInline void SetDir(PCWSTR pszDir)
	{
		m_Info.cchDir = DupStringForNewDeleteW(m_pszDir, pszDir);
		AddFile();
	}

	eStlInline PCWSTR GetDir(int* pcb)
	{
		if (pcb)
			*pcb = (m_Info.cchDir + 1) * sizeof(WCHAR);
		return m_pszDir;
	}

	eStlInline void SetFilePattern(PCWSTR pszFilePattern)
	{
		m_Info.cchFilePattern = DupStringForNewDeleteW(m_pszFilePattern, pszFilePattern);
		AddFile();
	}

	eStlInline PCWSTR GetFilePattern(int* pcb)
	{
		if (pcb)
			*pcb = (m_Info.cchFilePattern + 1) * sizeof(WCHAR);
		return m_pszFilePattern;
	}

	void SetFileAttr(UINT uAttr)
	{
		m_Info.uFileAttr = uAttr;
		AddFile();
	}

	UINT GetFileAttr()
	{
		return m_Info.uFileAttr;
	}

	void SetFileTime(BOOL bMaxTime, DATE date)
	{
		SYSTEMTIME st;
		FILETIME ft;
		VariantTimeToSystemTime(date, &st);
		SystemTimeToFileTime(&st, &ft);
		if (bMaxTime)
			LocalFileTimeToFileTime(&ft, &m_Info.ftMaxTime);
		else
			LocalFileTimeToFileTime(&ft, &m_Info.ftMinTime);
	}

	DATE GetFileTime(BOOL bMaxTime)
	{
		SYSTEMTIME st;
		FILETIME ft;
		DATE date;
		if (bMaxTime)
			FileTimeToLocalFileTime(&m_Info.ftMaxTime, &ft);
		else
			FileTimeToLocalFileTime(&m_Info.ftMinTime, &ft);
		FileTimeToSystemTime(&ft, &st);
		SystemTimeToVariantTime(&st, &date);
		return date;
	}

	void SetImageList(BYTE* pData, SIZE_T cb)
	{
		if (m_bILDataLocked)
			GlobalUnlock(m_hglbImageListData);
		GlobalFree(m_hglbImageListData);
		if (m_hImageList && m_bImageListNeedDel)
			ImageList_Destroy(m_hImageList);

		m_hImageList = NULL;
		m_bILDataLocked = FALSE;
		m_hglbImageListData = NULL;
		m_bImageListNeedDel = FALSE;
		m_pImageListData = NULL;
		if (!pData || !cb)
			return;

		HGLOBAL hGlobal;
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
		m_hglbImageListData = hGlobal;
		if (!hGlobal)
			return;
		
		void* p;
		if (p = GlobalLock(hGlobal))
		{
			memcpy(p, pData, cb);
			GlobalUnlock(hGlobal);
		}

		IStream* pIStream;
		if (SUCCEEDED(CreateStreamOnHGlobal(hGlobal, FALSE, &pIStream)))
		{
			m_hImageList = ImageList_Read(pIStream);
			m_bImageListNeedDel = TRUE;
			pIStream->Release();
		}
		Redraw();
	}

	BYTE* GetImageList(int* pcb)
	{
		if (m_bInDesignMode)
		{
			if (!m_hglbImageListData)
				return NULL;
			if (m_bILDataLocked)
				return m_pImageListData;
			else
			{
				m_bILDataLocked = TRUE;
				return m_pImageListData = (BYTE*)GlobalLock(m_hglbImageListData);
			}
		}
		
		if (!m_hImageList)
			return;
		if (m_bILDataLocked)
			GlobalUnlock(m_hglbImageListData);
		IStream* pIStream;
		if (SUCCEEDED(CreateStreamOnHGlobal(m_hglbImageListData, FALSE, &pIStream)))
		{
			ImageList_Write(m_hImageList, pIStream);
			pIStream->Release();
		}

		m_bILDataLocked = TRUE;
		return m_pImageListData = (BYTE*)GlobalLock(m_hglbImageListData);
	}

	void SetSelClr(int idx, COLORREF cr)
	{
		if (idx)
		{
			m_Info.crSelBK = cr;
			DeleteObject(m_hbrSelBK);
			if (cr != CLR_DEFAULT)
				m_hbrSelBK = CreateSolidBrush(cr);
			else
				m_hbrSelBK = GetSysColorBrush(COLOR_HIGHLIGHT);// ����ɾ��ϵͳ��ɫ��ˢ
		}
		else
			m_Info.crSelText = cr;
		Redraw();
	}

	eStlInline COLORREF GetSelClr(int idx)
	{
		if (idx)
			return m_Info.crSelBK;
		else
			return m_Info.crSelText;
	}

	eStlInline void SetEllipsis(BOOL bEllipsis)
	{
		m_Info.bEllipsis = bEllipsis;
	}

	eStlInline BOOL GetEllipsis()
	{
		return m_Info.bEllipsis;
	}

	eStlInline void SetAlign(BOOL bHAlign, int iAlign)
	{
		if (bHAlign)
			m_Info.iAlignH = iAlign;
		else
			m_Info.iAlignV = iAlign;
	}

	eStlInline int GetAlign(BOOL bHAlign)
	{
		if (bHAlign)
			return m_Info.iAlignH;
		else
			return m_Info.iAlignV;
	}

	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		SIZE_T cbDir = m_Info.cchDir * sizeof(WCHAR), 
			cbFilePattern = m_Info.cchFilePattern * sizeof(WCHAR);
		auto hGlobal = FlattenInfoBase0(
			sizeof(ELISTBOXDATA) +
			m_Info.cbItems +
			m_Info.cbImageList +
			cbDir +
			cbFilePattern, &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		p += cbBaseData;
		// �ṹ
		memcpy(p, &m_Info, sizeof(ELISTBOXDATA));
		p += sizeof(ELISTBOXDATA);
		// ��Ŀ
		memcpy(p, m_pItems, m_Info.cbItems);
		p += m_Info.cbItems;
		// ͼ���б�

		p += m_Info.cbImageList;
		// Ŀ¼
		memcpy(p, m_pszDir, cbDir);
		p += cbDir;
		// �ļ�������
		memcpy(p, m_pszFilePattern, cbFilePattern);
		// 
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}

	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto p = new CListBox(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(p->GetHWND());
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0:// ����
			p->SetItems(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize, FALSE);
			break;
		case 1:// ����ѡ����
			p->SetCurrSel(pPropertyVaule->m_int);
			break;
		case 2:// ������չ
			p->SetMultiColumn(pPropertyVaule->m_bool);
			return TRUE;
		case 3:// �и�
			p->SetItemHeight(pPropertyVaule->m_int);
			break;
		case 4:// �ı���ɫ
		case 5:// ������ɫ
			p->SetClr(nPropertyIndex - 4, pPropertyVaule->m_clr);
			break;
		case 6:// �߿�
			p->SetFrame(pPropertyVaule->m_int);
			break;
		case 7:// �Զ�����
			p->SetAutoSort(pPropertyVaule->m_bool);
			return TRUE;
		case 8:// ����ѡ
			p->SetMultiSel(pPropertyVaule->m_bool);
			return TRUE;
		case 9:// ������ѡ
			p->SetExtSel(pPropertyVaule->m_bool);
			return TRUE;
		case 10:// ������ʾ
			p->SetToolTip(pPropertyVaule->m_bool);
			break;
		case 11:// �����Ʊ��
			p->SetUseTabStop(pPropertyVaule->m_bool);
			return TRUE;
		case 12:// ȡ���ؼ��߶�
			p->SetIntegralHeight(pPropertyVaule->m_bool);
			return TRUE;
		case 13:// ��ʾ��ֹ������
			p->SetDisableNoScroll(pPropertyVaule->m_bool);
			return TRUE;
		case 14:// Ŀ¼
			p->SetDir((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;
		case 15:// �ļ�������
			p->SetFilePattern((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;
		case 16:// �ļ���������
			p->SetFileAttr(pPropertyVaule->m_int);
			break;
		case 17:// �ļ���С����
			p->SetFileTime(FALSE, pPropertyVaule->m_dtDateTime);
			break;
		case 18:// �ļ��������
			p->SetFileTime(TRUE, pPropertyVaule->m_dtDateTime);
			break;
		case 19:// ͼƬ��
			p->SetImageList(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize);
			break;
		case 20:// ѡ���ı���ɫ
		case 21:// ѡ�б�����ɫ
			p->SetSelClr(nPropertyIndex - 21, pPropertyVaule->m_clr);
			break;
		case 22:// ʡ�ԺŲü�
			p->SetEllipsis(pPropertyVaule->m_bool);
			break;
		case 23:// �������
			p->SetAlign(TRUE, pPropertyVaule->m_int);
			break;
		case 24:// �������
			p->SetAlign(FALSE, pPropertyVaule->m_int);
			break;
		}

		return FALSE;
	}

	static HGLOBAL WINAPI EGetAlldata(HUNIT hUnit)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		return p->FlattenInfo();
	}

	static BOOL WINAPI EGetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0:// ����
			pPropertyVaule->m_data.m_pData = p->GetItems();
			break;
		case 1:// ����ѡ����
			pPropertyVaule->m_int = p->GetCurrSel();
			break;
		case 2:// ������չ
			pPropertyVaule->m_bool = p->GetMultiColumn();
			break;
		case 3:// �и�
			pPropertyVaule->m_int = p->GetItemHeight();
			break;
		case 4:// �ı���ɫ
		case 5:// ������ɫ
			pPropertyVaule->m_clr = p->GetClr(nPropertyIndex - 4);
			break;
		case 6:// �߿�
			pPropertyVaule->m_int = p->GetFrame();
			break;
		case 7:// �Զ�����
			pPropertyVaule->m_bool = p->GetAutoSort();
			break;
		case 8:// ����ѡ
			pPropertyVaule->m_bool = p->GetMultiSel();
			break;
		case 9:// ������ѡ
			pPropertyVaule->m_bool = p->GetExtSel();
			break;
		case 10:// ������ʾ
			pPropertyVaule->m_bool = p->GetToolTip();
			break;
		case 11:// �����Ʊ��
			pPropertyVaule->m_bool = p->GetUseTabStop();
			break;
		case 12:// ȡ���ؼ��߶�
			pPropertyVaule->m_bool = p->GetIntegralHeight();
			break;
		case 13:// ��ʾ��ֹ������
			pPropertyVaule->m_bool = p->GetDisableNoScroll();
			break;
		case 14:// Ŀ¼
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetDir(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 15:// �ļ�������
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetFilePattern(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 16:// �ļ���������
			pPropertyVaule->m_int = p->GetFileAttr();
			break;
		case 17:// �ļ���С����
			pPropertyVaule->m_dtDateTime = p->GetFileTime(FALSE);
			break;
		case 18:// �ļ��������
			pPropertyVaule->m_dtDateTime = p->GetFileTime(TRUE);
			break;
		case 19:// ͼƬ��
			pPropertyVaule->m_data.m_pData = p->GetImageList(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 20:// ѡ���ı���ɫ
		case 21:// ѡ�б�����ɫ
			pPropertyVaule->m_clr = p->GetSelClr(nPropertyIndex - 21);
			break;
		case 22:// ʡ�ԺŲü�
			pPropertyVaule->m_bool = p->GetEllipsis();
			break;
		case 23:// �������
			pPropertyVaule->m_int = p->GetAlign(TRUE);
			break;
		case 24:// �������
			pPropertyVaule->m_int = p->GetAlign(FALSE);
			break;
		}

		return TRUE;
	}

	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		PWSTR psz;
		*pblModified = FALSE;
		switch (nPropertyIndex)
		{
		case 0:// ����
		{
			BYTE* pItems;
			SIZE_T cb;
			BOOL bOK = ShowListBoxDesignDlg(&pItems, &cb, p->GetItems());
			if (bOK)
				p->SetItems(pItems, cb, TRUE);
			*pblModified = bOK;
		}
		break;
		case 14:// Ŀ¼
			if (elibstl::IntputBox(&psz, p->GetDir(NULL)))
			{
				delete[] p->m_pszDir;
				p->m_pszDir = psz;
				if (psz)
					p->m_Info.cchDir = wcslen(psz);
				else
					p->m_Info.cchDir = 0;
				p->AddFile();
				*pblModified = TRUE;
			}
			break;
		case 15:// ������
			if (elibstl::IntputBox(&psz, p->GetFilePattern(NULL)))
			{
				delete[] p->m_pszFilePattern;
				p->m_pszFilePattern = psz;
				if (psz)
					p->m_Info.cchFilePattern = wcslen(psz);
				else
					p->m_Info.cchFilePattern = 0;
				p->AddFile();
				*pblModified = TRUE;
			}
			break;
		}
		return FALSE;
	}

	static BOOL WINAPI EPropUpdateUI(HUNIT hUnit, INT nPropertyIndex)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		switch (nPropertyIndex)
		{

		}

		return TRUE;
	}

	static INT WINAPI ENotify(INT nMsg, DWORD dwParam1, DWORD dwParam2)
	{
		switch (nMsg)
		{
		case NU_GET_CREATE_SIZE_IN_DESIGNER:
		{
			*((int*)dwParam1) = 80;
			*((int*)dwParam2) = 20;
		}
		return TRUE;
		}
		return FALSE;
	}
};
SUBCLASS_MGR_INIT(CListBox, SCID_LISTBOXPARENT, SCID_LISTBOX)
ESTL_NAMESPACE_END

EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_ListBoxW(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
		return (PFN_INTERFACE)elibstl::CListBox::ECreate;
	case ITF_NOTIFY_PROPERTY_CHANGED:
		return (PFN_INTERFACE)elibstl::CListBox::EChange;
	case ITF_GET_ALL_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CListBox::EGetAlldata;
	case ITF_GET_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CListBox::EGetData;
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
		return (PFN_INTERFACE)elibstl::CListBox::EInputW;
	case ITF_PROPERTY_UPDATE_UI:
		return (PFN_INTERFACE)elibstl::CListBox::EPropUpdateUI;
	case ITF_GET_NOTIFY_RECEIVER:
		return (PFN_INTERFACE)elibstl::CListBox::ENotify;
	}
	return NULL;
}

static EVENT_INFO2 s_Event_ListBox[] =
{
	/*000*/ {"���ݱ��ı�", NULL, _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};

static UNIT_PROPERTY s_Member_ListBox[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/  {"����", "Items", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*001*/  {"����ѡ����", "CurrSel", "", UD_INT, _PROP_OS(__OS_WIN), NULL},
	/*002*/	 {"������չ", "MultiColumn", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*003*/  {"�и�", "ItemHeight", "", UD_INT, _PROP_OS(__OS_WIN), NULL},
	/*004*/  {"�ı���ɫ", "TextClr", "", UD_COLOR, _PROP_OS(__OS_WIN), NULL},
	/*005*/  {"������ɫ", "BKClr", "", UD_COLOR, _PROP_OS(__OS_WIN),NULL},
	/*006*/  {"�߿�", "Frame", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
	/*007*/  {"�Զ�����", "AutoSort", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*008*/  {"����ѡ", "MultiSel", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*009*/  {"������ѡ", "ExtSel", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*010*/  {"������ʾ", "ToolTip", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*011*/  {"�����Ʊ��", "UseTabStop", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*012*/  {"ȡ���ؼ��߶�", "IntegralHeight", "�Ƿ񽫿ؼ��߶ȶ��뵽��Ŀ�߶ȵ�����������Ȼ�����������Ŀ�߶������岻ͬ��������û�����壬���Կ��ã���ʱ�����뵽����߶ȡ�", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*013*/  {"��ʾ��ֹ������", "DisableNoScroll", "������������ʱ��ʾ��ֹ�Ĺ��������������ع�����", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*014*/  {"Ŀ¼", "Dir", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN),  NULL},
	/*015*/  {"�ļ�������", "FilePattern", "�硰*.mp3�������������֮�������ߡ�|���ָ�", UD_CUSTOMIZE, _PROP_OS(__OS_WIN),  NULL},
	/*016*/  {"�ļ���������", "FileAttr", "����ֵ��λ����ϣ�1 - ֻ��  2 - ����  4 - ϵͳ  16 - Ŀ¼  32 - �浵  128 - ͨ��", UD_INT, _PROP_OS(__OS_WIN),  NULL},
	/*017*/  {"�ļ���С����", "FileDateMin", "", UD_DATE_TIME, _PROP_OS(__OS_WIN),  NULL},
	/*018*/  {"�ļ��������", "FileDataMax", "", UD_DATE_TIME, _PROP_OS(__OS_WIN),  NULL},
	/*019*/  {"ͼƬ��", "ImageList", "", UD_IMAGE_LIST, _PROP_OS(__OS_WIN) | UW_HAS_INDENT,  NULL},
	/*020*/  {"ѡ���ı���ɫ", "SelTextClr", "", UD_COLOR, _PROP_OS(__OS_WIN) | UW_HAS_INDENT,NULL},
	/*021*/  {"ѡ�б�����ɫ", "SelBKClr", "", UD_COLOR, _PROP_OS(__OS_WIN) | UW_HAS_INDENT, NULL},
	/*022*/	 {"ʡ�ԺŲü�", "Ellipsis", "", UD_BOOL, _PROP_OS(__OS_WIN) | UW_HAS_INDENT,  NULL},
	/*023*/  {"������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN) | UW_HAS_INDENT, "���\0""����\0""�ұ�\0""\0"},
	/*024*/  {"������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN) | UW_HAS_INDENT,"�ϱ�\0""����\0""�±�\0""\0"},
};

static INT s_Cmd_ListBox[] = { 0 };
ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO CtListBoxW = {
	"�б��W",//��������
	"ListBoxW",//Ӣ������
	"Unicode�б���ļ���ѡ���б������һ��ע�⣺֧�ֿⴴ��һ���������б��Ȼ����������Ŀ��������ƹ�������˲���ʹ��LB_ADDSTRING����Ϣ��",//˵��
	ARRAYSIZE(s_Cmd_ListBox),//��������
	s_Cmd_ListBox,//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
	104,//��ԴID
	ARRAYSIZE(s_Event_ListBox),
	s_Event_ListBox,
	ARRAYSIZE(s_Member_ListBox),//������
	s_Member_ListBox,//����ָ��
	libstl_GetInterface_ListBoxW,//��������ӳ���
	NULL,//��Ա����
	NULL//��Ա��������
};
ESTL_NAMESPACE_END