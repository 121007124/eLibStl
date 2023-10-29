#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "HexView_Function.h"

#define WC_HEXVIEWA              "����QQ121007124"
#define WC_HEXVIEWW              L"����QQ121007124"

#ifdef UNICODE
#define WC_HEXVIEW              WC_HEXVIEWW
#else
#define WC_HEXVIEW              WC_HEXVIEWA
#endif

#define HVM_FIRST               0x3C00

#define HVM_SETTEXTCOLOR        (HVM_FIRST + 0)     // �����ı���ɫ, wParam = �Ƿ��ػ�, lParam = �ı���ɫ, ��������ǰ����ɫ
#define HVM_GETTEXTCOLOR        (HVM_FIRST + 1)     // ��ȡ�ı���ɫ, ���� wParam��lParam, �����ı���ɫ
#define HVM_SETBKCOLOR          (HVM_FIRST + 2)     // ���ñ�����ɫ, wParam = �Ƿ��ػ�, lParam = ������ɫ, ��������ǰ����ɫ
#define HVM_GETBKCOLOR          (HVM_FIRST + 3)     // ��ȡ������ɫ, ���� wParam��lParam, ���ر�����ɫ
#define HVM_SETSELBKCOLOR       (HVM_FIRST + 4)     // ����ѡ�б�����ɫ, wParam = �Ƿ��ػ�, lParam = ѡ�б�����ɫ, ��������ǰ����ɫ
#define HVM_GETSELBKCOLOR       (HVM_FIRST + 5)     // ��ȡѡ�б�����ɫ, ���� wParam��lParam, ����ѡ�б�����ɫ
#define HVM_SETMODIFIEDCOLOR    (HVM_FIRST + 6)     // �������޸��ı���ɫ, wParam = �Ƿ��ػ�, lParam = ���޸��ı���ɫ, ��������ǰ����ɫ
#define HVM_GETMODIFIEDCOLOR    (HVM_FIRST + 7)     // ��ȡ���޸��ı���ɫ, ���� wParam��lParam, �������޸��ı���ɫ

#define HVM_SETEXTENDEDSTYLE    (HVM_FIRST + 20)    // ���������չ��ʽ, wParam = �Ƿ��ػ�, lParam = �����չ��ʽ, ��������ǰ����ʽ
#define HVM_GETEXTENDEDSTYLE    (HVM_FIRST + 21)    // ��ȡ�����չ��ʽ, ���� wParam��lParam, ���������չ��ʽ
#define HVM_SETITEMCOUNT        (HVM_FIRST + 22)    // ������Ŀ��, wParam = �Ƿ��ػ�, lParam = ��Ŀ��, �����Ƿ�ɹ�
#define HVM_GETITEMCOUNT        (HVM_FIRST + 23)    // ��ȡ��Ŀ��, ���� wParam��lParam, ������Ŀ��
#define HVM_SETSEL              (HVM_FIRST + 24)    // ����ѡ������, wParam = �Ƿ��ػ�, lParam = BYTERANGE�ṹ, �����Ƿ�ɹ�
#define HVM_GETSEL              (HVM_FIRST + 25)    // ��ȡѡ������, ���� wParam, lParam = BYTERANGE�ṹ, ���ս��, �����Ƿ�ɹ�
#define HVM_SETCOLUMNCOUNT      (HVM_FIRST + 26)    // ��������, wParam = �Ƿ��ػ�, lParam = ����, �����Ƿ�ɹ�
#define HVM_GETCOLUMNCOUNT      (HVM_FIRST + 27)    // ��ȡ����, ���� wParam��lParam, ��������


#define HVN_GETDISPINFO         (WMN_FIRST - 0)     // ��ȡ����, NMHVDISPINFO �ṹ
#define HVN_ITEMCHANGING        (WMN_FIRST - 1)     // ���ݱ��ı�, NMHEXVIEW �ṹ
#define HVN_ITEMSEL             (WMN_FIRST - 2)     // ѡ������, NMHEXSEL �ṹ
#define HVN_SEARCH              (WMN_FIRST - 3)     // ��������, NMHEXSEARCH �ṹ
#define HVN_COPYDATA            (WMN_FIRST - 4)     // ��������, NMHEXCOPYDATA �ṹ


//
// Mask of an item
//

#define HVIF_ADDRESS            0x0001
#define HVIF_BYTE               0x0002

//
// State of an item
//

#define HVIS_MODIFIED           0x0001

//
// Extended styles
//

#define HVS_ADDRESS64           0x0001
#define HVS_READONLY            0x0002

//
// States of the control
//

#define HVF_FONTCREATED         0x0001
#define HVF_CARETVISIBLE        0x0002
#define HVF_SELECTED            0x0004

#define HVCPY_DEF               0x0000  // Ĭ�ϸ���, ֻ�����м���ı�, ����16����
#define HVCPY_ADDRESS           0x0001  // ������ߵĵ�ַ
#define HVCPY_CHAR              0x0002  // �����ұߵ��ַ�
#define HVCPY_HEX10             0x0004  // ����10����
#define HVCPY_ALL               HVCPY_ADDRESS | HVCPY_CHAR  // ����16���� + ��ַ + �ַ�

#define HVCPY_HEX_1             0x0100  // ����16����, ���ÿո�ָ�
#define HVCPY_HEX_C             0x0200  // ����16����, ��C���Եĸ�ʽ, 0xFF, 
#define HVCPY_HEX_ASM           0x0300  // ����16����, �û��ĸ�ʽ, 0FFh, 
#define HVCPY_HEX10_1           0x0400  // ����10����, �ֽ�û���ַ��ָ�


enum Column {
    COLUMN_DATA = 1,
    COLUMN_VALUE
};

typedef LRESULT(CALLBACK* PFN_HEXVIEW_NOTIFY)(HWND, LPNMHDR, LPVOID);

typedef struct _HEXVIEW {
    int         Line1;              // Start of the COLUMN_DATA
    int         Line2;              // Start of the COLUMN_VALUE

    int         VisibleLines;       //
    LONG_PTR    TotalLines;         //
    int         VisibleChars;       //
    int         LongestLine;        //

    LONG_PTR    VscrollPos;         //
    LONG_PTR    VscrollMax;         //
    int         HscrollPos;         //
    int         HscrollMax;         //

    int         HeightChar;         //
    int         WidthChar;          //
    int         HeightView;         //
    int         WidthView;          //

    int         ColumnCount;        // ��ʾ������, Ĭ������16
    int         ColumnSecond;       // �ұ��е�λ��, ���Ӱ���ұ��ַ���λ��
    LPBYTE      pSearchData;        // ����������
    int         nSearchSize;        // ���������ݴ�С
    int         nSearchBufSize;     // ���������ݻ�������С ���ڵ��� nSearchSize
    LPWSTR      pSearchText;        // �������ı�
    int         nSearchMode;        // ����ģʽ, 0=ʮ������, 1=ansi�ַ���, 2=10����, 3=Unicode�ַ���, 4=�����ȸ�����, 5=˫���ȸ�����
    PHEX_NODE   pNode;              // ��ʷ�������ı�����
    PHEX_NODE   pNodeJmp;           // ��ת��ʷ�������ı�����
    int         nJmpMode;           // ��תģʽ, 0=���ļ���ʼ, 1=��ǰλ��, 2����λ������, 3=���ļ�����, ��λ = �Ƿ���16����, ͬʱ��������ʱѡ�е�ģʽ
    HWND        hAsm;               // ��ര�ھ��, ����Ѿ��򿪾Ͳ��ٴ򿪵ڶ���
    int         asm_ShowMode;       // �����ʾ�﷨ģʽ, 0=MASM, 0x100=GoAsm, 0x200=Nasm, 0x400=AT, 0x800=IntrinsicMem

    HWND        hWndParent;         // ��������ʱ�ĸ�����, ���ܸ�������ô��, �����������֪ͨ��Ϣ
    PFN_HEXVIEW_NOTIFY pfnNotify;   // ֪ͨ�¼�, ʹ�����ַ�ʽ�Ͳ����򸸴���Ͷ��֪ͨ, ����ʡȥ���໯�����ڵĲ���
    LPVOID      lpParamNotify;      // ֪ͨ�¼����Ӳ���

    int         ActiveColumn;       // COLUMN_DATA or COLUMN_VALUE

    SIZE_T      TotalItems;         //
    SIZE_T      NumberOfItem;       //
    UINT        PositionInItem;     // 0 = HINIBBLE; 1 = LONIBBLE

    SIZE_T      SelectionStart;     //
    SIZE_T      SelectionEnd;       //

    DWORD       Flags;              // 
    DWORD       ExStyle;            // ��չ���

    HFONT       hFont;              // ���ʹ�õ�����

    COLORREF    clrTextSel;         // �ı���ɫ, ѡ�е��ı���ɫ, �ұ��ַ���
    COLORREF    clrTextAddress;     // �ı���ɫ, ��ߵ�ַ����ɫ
    COLORREF    clrText1;           // �ı���ɫ, ��һ�����ӵ���ɫ
    COLORREF    clrText2;           // �ı���ɫ, �ڶ������ӵ���ɫ
    COLORREF    clrTextSel1;        // ѡ���ı���ɫ, ��һ�����ӵ���ɫ
    COLORREF    clrTextSel2;        // ѡ���ı���ɫ, �ڶ������ӵ���ɫ
    COLORREF    clrTextBackground;  // �ı�������ɫ
    COLORREF    clrSelTextBack;     // ѡ���ı�������ɫ
    COLORREF    clrModifiedText;    // �޸Ĺ����ı���ɫ
    COLORREF    clrModifiedTextSel; // �޸Ĺ����ı���ɫ, ѡ�е���ɫ
    COLORREF    clrLine;            // ������ɫ, �����߿�ɫ
} HEXVIEW, * PHEXVIEW;

typedef struct _HVITEM {
    UINT Mask;
    UINT State;
    ULONG64 Address;
    SIZE_T NumberOfItem;
    BYTE Value;
} HVITEM, * PHVITEM;

typedef struct _NMHVDISPINFO {
    NMHDR NmHdr;
    HVITEM Item;
} NMHVDISPINFO, * PNMHVDISPINFO;

typedef struct _NMHEXVIEW {
    NMHDR NmHdr;
    HVITEM Item;
} NMHEXVIEW, * PNMHEXVIEW;

typedef struct _NMHEXSEL
{
    NMHDR  NmHdr;
    SIZE_T Min;
    SIZE_T Max;
    int    isSel;
} NMHEXSEL, * PNMHEXSEL;
typedef struct _NMHEXSEARCH
{
    NMHDR   NmHdr;
    LPBYTE  pSearch;    // Ҫ����������
    SIZE_T  nSize;      // ���������ݴ�С
    SIZE_T  start;      // ��ʼ������λ��
    SIZE_T  end;        // ����������λ��, ����������Ϊ0
    SIZE_T  pos;        // ���ҵ���λ��, ����֪ͨ�ĵط���д
} NMHEXSEARCH, * PNMHEXSEARCH;
typedef struct _NMHEXCOPYDATA
{
    NMHDR   NmHdr;
    LPVOID  pBuf;       // ��������ַ
    SIZE_T  nSize;      // �������ߴ�
    ULONG64 Address;    // ����������ʼ��ַ, ��ʾ���ʹ�������ַ
} NMHEXCOPYDATA, * PNMHEXCOPYDATA;

typedef struct _BYTERANGE {
    SIZE_T Min;
    SIZE_T Max;
} BYTERANGE, * PBYTERANGE;


HWND CreateHexView(DWORD dwExStyle, DWORD dwStyle,
    int x, int y, int nWidth, int nHeight, HWND hWndParent, LONG_PTR id = 0, LPVOID lpParam = 0);

BOOLEAN HexView_BindNotify(HWND hWnd, PFN_HEXVIEW_NOTIFY pfn, LPVOID lpParam);

