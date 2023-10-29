#include "HexView.h"
#include <vector>

// �汾��, �����ֽڼ�¼���汾�ʹΰ汾��, �����ֽڼ�¼����
#define HEXVIEW_VERSION MAKELONG(MAKEWORD(1, 0), MAKEWORD(10, 15))

#define HEXVIEW_PROP_BORDER                 0   // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
#define HEXVIEW_PROP_TEXTCOLOR              1   // �ı���ɫ
#define HEXVIEW_PROP_TEXTBACKCOLOR          2   // �ı�������ɫ
#define HEXVIEW_PROP_BACKCOLOR              3   // ������ɫ
#define HEXVIEW_PROP_FONT                   4   // ��������
#define HEXVIEW_PROP_EDITABLE               5  // ����༭, ������ָ���Ƿ�����༭


#define HEXVIEW_NAMESPACE _hexview
#define HEXVIEW_NAMESPACE_BEGIN namespace HEXVIEW_NAMESPACE {
#define HEXVIEW_NAMESPACE_END }

HEXVIEW_NAMESPACE_BEGIN
typedef struct CONTROL_STRUCT_BASE
{
    LONG                controlType;    // �������, �����ݶ���һ���Ĳ���, �͸������ȷ����ʲô�����, CONTROLTYPE_ ��ͷ����
    WNDPROC             oldProc;        // ����ԭ����
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

// 
typedef struct _HEXVIEW_PROPERTY : CONTROL_STRUCT_BASE
{
    LONG            version;                // ȷ���汾�ŵ�һ����־
    LONG            border;                 // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
    LONG            TextColor;              // �ı���ɫ
    LONG            TextBackColor;          // �ı�������ɫ
    LONG            BackColor;              // ������ɫ
    BOOL            Editable;               // ����༭, ������ָ���Ƿ�����༭
    LONG            fontSize;               // �������Գߴ�, Ϊ0��д������
    LOGFONTA        font;                   // ��������
    ULONG64         pAddress;               // ��ʾ�ĵ�ַ
    std::vector<BYTE>* data;                // ��ʾ������
    std::vector<BYTE>* modi;                // ָ���ֽ��Ƿ��޸�
}HEXVIEW_PROPERTY, * PHEXVIEW_PROPERTY, * LPHEXVIEW_PROPERTY;



HEXVIEW_NAMESPACE_END