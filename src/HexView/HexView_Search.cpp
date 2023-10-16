#include "HexView_Search.h"

#define IDC_CBB_MODE    1000    // ����ģʽ��Ͽ�
#define IDC_CBB_INPUT   1001    // �����ı���Ͽ�

#define IDC_CBB_ADDR    1000    // �ڴ��ַ��Ͽ�
#define IDC_CBB_ADDRSI  1001    // �ڴ�ߴ���Ͽ�


typedef struct HEXSEARCH_DATA
{
    HWND        hWnd;       // �Ի�����
    PHEXSEARCH  pArg;       // ���ݸ��Ի���Ĳ���
    BOOLEAN     isOk;       // �Ƿ�����ȷ����ť
    SIZE_T      start;      // ��ת�Ի�����
}*PHEXSEARCH_DATA;

HWND HexView_Search_CreateControl(LPCWSTR szClass, LPCWSTR title, DWORD styleEx, DWORD style, int x, int y, int cx, int cy, HWND hParent, INT_PTR id, LPCWSTR lpszTips = 0);
INT_PTR CALLBACK HexView_Search_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int HexView_Search_Dialog(PHEXSEARCH arg)
{
    struct __DLGTEMPLATE : DLGTEMPLATE
    {
        WORD menuid;
        WORD cls;
        wchar_t caption[1024];
    };

    __DLGTEMPLATE dlg;
    memset(&dlg, 0, sizeof(dlg));
    dlg.style           = WS_SYSMENU | WS_CAPTION | WS_CLIPSIBLINGS | WS_VISIBLE;
    dlg.dwExtendedStyle = WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CONTROLPARENT;
    dlg.cdit            = 0;

    HEXSEARCH_DATA data = { 0 };
    data.pArg = arg;

    INT_PTR bRet = DialogBoxIndirectParamW(GetModuleHandleW(0), &dlg, arg->hParent, HexView_Search_DlgProc, (LPARAM)&data);

    return data.isOk;
}

// �ͷ��������ص�����
void HexView_Search_Free(LPVOID ptr)
{
    if ( ptr )
        free(ptr);
}

HWND HexView_Search_CreateControl(LPCWSTR szClass, LPCWSTR title, DWORD styleEx, DWORD style, int x, int y, int cx, int cy, HWND hParent, INT_PTR id, LPCWSTR lpszTips)
{
    HWND hWnd = CreateWindowExW(styleEx, szClass, title, style, x, y, cx, cy, hParent, (HMENU)(LONG_PTR)id, GetModuleHandleW(0), 0);

    static HFONT hFont;
    if ( !hFont )
    {
        LOGFONTW lf = { 0 };
        SystemParametersInfoW(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
        lf.lfHeight     = -12;
        lf.lfWeight     = FW_NORMAL;
        lf.lfItalic     = false; // б��
        lf.lfUnderline  = false; // �»���
        lf.lfStrikeOut  = false; // ɾ����
        lf.lfCharSet    = GB2312_CHARSET;
        wcscpy_s(lf.lfFaceName, L"΢���ź�");
        hFont = CreateFontIndirectW(&lf);
    }
    SendMessageW(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
    return hWnd;
}

// ���һ���ַ�����������, ���ҰѲ������ý�ȥ
inline int HexView_Combobox_AddString(HWND hWnd, LPCWSTR lpsz, LPARAM param = 0, int index = -1)
{
    int ret = ( (int)(DWORD)SendMessageW(hWnd, CB_INSERTSTRING, (WPARAM)(int)index, (LPARAM)lpsz) );
    if ( ret >= 0 && param )
    {
        SendMessageW(hWnd, CB_SETITEMDATA, ret, param);
    }
    return ret;
}
INT_PTR CALLBACK HexView_Search_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    if ( message == WM_INITDIALOG )   // �Ի����ʼ��, ��ָ�����ý�������
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)lParam);

    PHEXSEARCH_DATA pData = (PHEXSEARCH_DATA)GetWindowLongPtrW(hDlg, GWLP_USERDATA);  // ÿ����Ϣ��Ҫ��ȡ���ָ��
    if ( !pData ) return false;
    INT_PTR ret = 0;
    switch ( message )
    {
    case WM_INITDIALOG:
    {
        pData->hWnd = hDlg;
        RECT rc = { 0 };
        if ( pData->pArg->hParent )
            GetWindowRect(pData->pArg->hParent, &rc);
        if ( rc.right - rc.left <= 0 || rc.bottom - rc.top <= 0 )
        {
            rc.left = 0;
            rc.top = 0;
            rc.right = GetSystemMetrics(SM_CXSCREEN);
            //rc.bottom = GetSystemMetrics(SM_CYSCREEN);
            rc.bottom = GetSystemMetrics(SM_CYMAXIMIZED);
        }

        const int margin = 11;  // �߾�
        const int editWidth = 460;
        const int cbkWidth = 140;
        const int btnWidth = 90;
        const int inputWidth = editWidth + margin * 2 + 16;
        const int inputHeight = 155;
        const int width = rc.right - rc.left;
        const int height = rc.bottom - rc.top;
        const int inputLeft = rc.left + ( width - inputWidth ) / 2;
        const int inputTop = rc.top + ( height - inputHeight ) / 2;
        const int height_cbb = 200;
        MoveWindow(hDlg, inputLeft, inputTop, inputWidth, inputHeight, true);

        int top = margin;

        const DWORD styleStatic = WS_GROUP | WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE;
        DWORD style = WS_TABSTOP |  WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
        DWORD styleEx = WS_EX_NOPARENTNOTIFY;

        HWND hCbb = 0;
        LPCWSTR btn_ok_title = L"����";
        switch ( pData->pArg->windowType )
        {
        case HEXSEARCH_WINDOWTYPE_ASM:  // ��ര��
        {
            // ��ര��ֻ��һ����ʾ
            const int static_height = 24;
            HexView_Search_CreateControl(L"static", pData->pArg->szTips, styleEx, styleStatic, margin, top, editWidth, static_height, hDlg, 0);
            top += static_height + 5;


            style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
            hCbb = HexView_Search_CreateControl(L"ComboBox", 0, styleEx, style, margin + 40, top, editWidth - 40, height_cbb, hDlg, IDC_CBB_INPUT);
            GetWindowRect(hCbb, &rc);
            const int cbbHeight = rc.bottom - rc.top;

            HexView_Search_CreateControl(L"static", L"����", styleEx, styleStatic, margin, top, 40, cbbHeight, hDlg, 0);
            top += cbbHeight + 5;
            break;
        }
        case HEXSEARCH_WINDOWTYPE_HEX:  // ʮ�����ƴ���
        {
            hCbb = HexView_Search_CreateControl(L"ComboBox", 0, styleEx, style, margin + 40, top, editWidth - 40, height_cbb, hDlg, IDC_CBB_MODE);
            GetWindowRect(hCbb, &rc);
            const int cbbHeight = rc.bottom - rc.top;
            HexView_Search_CreateControl(L"static", L"����", styleEx, styleStatic, margin, top, 40, cbbHeight, hDlg, 0);
            top += cbbHeight + 5;

            HexView_Combobox_AddString(hCbb, L"ʮ������");
            HexView_Combobox_AddString(hCbb, L"ANSI �ַ���");
            HexView_Combobox_AddString(hCbb, L"ʮ����");
            HexView_Combobox_AddString(hCbb, L"Unicode �ַ���");
            HexView_Combobox_AddString(hCbb, L"�����ȸ�����");
            HexView_Combobox_AddString(hCbb, L"˫���ȸ�����");

            int cbbIndex = pData->pArg->nMode;
            if ( cbbIndex < 0 || cbbIndex > 5 )
                cbbIndex = 0;
            SendMessageW(hCbb, CB_SETCURSEL, cbbIndex, 0);

            style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
            HexView_Search_CreateControl(L"static", L"����", styleEx, styleStatic, margin, top, 40, cbbHeight, hDlg, 0);
            hCbb = HexView_Search_CreateControl(L"ComboBox", 0, styleEx, style, margin + 40, top, editWidth - 40, height_cbb, hDlg, IDC_CBB_INPUT);
            top += cbbHeight + 5;
            break;
        }
        case HEXSEARCH_WINDOWTYPE_MEMORY:   // �����ڴ��ַ�ͳߴ�Ĵ���
        {
            // ����һ����ʾ�ڴ��ַ, ������ʾ�ڴ�ߴ�
            const int static_height = 24;
            const int staticWidth = 60;
            style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
            hCbb = HexView_Search_CreateControl(L"ComboBox", 0, styleEx, style, margin + staticWidth, top, editWidth - staticWidth, height_cbb, hDlg, IDC_CBB_ADDR);
            GetWindowRect(hCbb, &rc);
            const int cbbHeight = rc.bottom - rc.top;
            HexView_Search_CreateControl(L"static", L"�ڴ��ַ", styleEx, styleStatic, margin, top, staticWidth, cbbHeight, hDlg, 0);
            top += cbbHeight + 5;

            HWND hCbb2 = HexView_Search_CreateControl(L"ComboBox", 0, styleEx, style, margin + staticWidth, top, editWidth - staticWidth, height_cbb, hDlg, IDC_CBB_ADDRSI);
            HexView_Search_CreateControl(L"static", L"�ڴ�ߴ�", styleEx, styleStatic, margin, top, staticWidth, cbbHeight, hDlg, 0);
            HexView_Combobox_AddString(hCbb2, pData->pArg->szTips);
            SendMessageW(hCbb2, CB_SETCURSEL, 0, 0);

            btn_ok_title = L"�鿴�ڴ�";
            top += cbbHeight + 5;
            break;
        }
        default:
            break;
        }




        PHEX_NODE node = pData->pArg->listText;

        int count = 0;
        while ( node )
        {
            HexView_Combobox_AddString(hCbb, node->text);
            count++;
            node = node->next;
            if ( count >= 20 )
                break;  // �����ʾ20��
        }

        if ( hCbb && count > 0 )
            SendMessageW(hCbb, CB_SETCURSEL, 0, 0);


        style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
        styleEx = WS_EX_NOPARENTNOTIFY;
        int left = inputWidth / 2;
        HexView_Search_CreateControl(L"button", btn_ok_title, styleEx, style, left - btnWidth - 5, top, btnWidth, 31, hDlg, IDOK);


        style = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
        HexView_Search_CreateControl(L"button", L"ȡ��", styleEx, style, left + 5, top, btnWidth, 31, hDlg, IDCANCEL);

        SetWindowTextW(hDlg, pData->pArg->szCaption);
        SetFocus(hCbb);
        return false;
        break;
    }
    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        switch ( id )
        {
        case IDOK:
        {
            HWND hWndTop = 0, hWndBottom = 0;
            switch ( pData->pArg->windowType )
            {
            case HEXSEARCH_WINDOWTYPE_HEX:  // ʮ�����ƴ���
            case HEXSEARCH_WINDOWTYPE_ASM:  // ��ര��
            {
                hWndTop = GetDlgItem(hDlg, IDC_CBB_MODE);
                hWndBottom = GetDlgItem(hDlg, IDC_CBB_INPUT);
                if ( pData->pArg->windowType == HEXSEARCH_WINDOWTYPE_HEX )
                    pData->pArg->nMode = (int)(DWORD)SendMessageW(hWndTop, CB_GETCURSEL, 0, 0);

                int len = GetWindowTextLengthW(hWndBottom) + 1;
                int size = len * sizeof(wchar_t);

                pData->pArg->pRet = (LPWSTR)malloc(size);
                if ( !pData->pArg->pRet ) throw;
                len = GetWindowTextW(hWndBottom, pData->pArg->pRet, len);

                break;
            }
            case HEXSEARCH_WINDOWTYPE_MEMORY:   // �����ڴ��ַ�ͳߴ�Ĵ���
            {
                hWndTop = GetDlgItem(hDlg, IDC_CBB_ADDR);       // �ڴ��ַ
                hWndBottom = GetDlgItem(hDlg, IDC_CBB_ADDRSI);  // �ڴ�ߴ�

                int len1 = GetWindowTextLengthW(hWndTop) + 1;
                int size1 = len1 * sizeof(wchar_t);

                int len2 = GetWindowTextLengthW(hWndTop) + 1;
                int size2 = len2 * sizeof(wchar_t);

                pData->pArg->pRet = (LPWSTR)HexViewAlloc(size1);
                len1 = GetWindowTextW(hWndTop, pData->pArg->pRet, len1);

                pData->pArg->pRet2 = (LPWSTR)HexViewAlloc(size2);
                len2 = GetWindowTextW(hWndBottom, pData->pArg->pRet2, len2);

                break;
            }
            default:
                EndDialog(hDlg, 1);
                return 0;
                break;
            }


            pData->isOk = TRUE;
            EndDialog(hDlg, 1);
            break;
        }
        case IDCANCEL:
        {
            EndDialog(hDlg, 0);
            break;
        }
        default:
            return (INT_PTR)false;
        }
        break;
    }
    case WM_CTLCOLORMSGBOX:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSCROLLBAR:
    case WM_CTLCOLORSTATIC:
        return (INT_PTR)GetStockObject(WHITE_BRUSH);
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps = { 0 };
        BeginPaint(hDlg, &ps);
        RECT rc = { 0 };
        GetClientRect(hDlg, &rc);
        FillRect(ps.hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
        EndPaint(hDlg, &ps);
        break;
    }
    default:
        return (INT_PTR)false;
    }
    return (INT_PTR)true;
}
