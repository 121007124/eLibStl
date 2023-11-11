#pragma once
#include "HexView_Function.h"

enum HEXSEARCH_WINDOWTYPE
{
    HEXSEARCH_WINDOWTYPE_ASM = 1,   // ��ര��
    HEXSEARCH_WINDOWTYPE_HEX,       // ʮ�����ƴ���
    HEXSEARCH_WINDOWTYPE_MEMORY,    // �����ڴ��ַ�ͳߴ�Ĵ���

};
typedef struct _HEXSEARCH
{
    HWND        hParent;    // �Ի��򸸴���
    LPCWSTR     szCaption;  // �Ի������
    LPCWSTR     szTips;     // ��ʾ�ı�
    PHEX_NODE   listText;   // Ĭ���������ı�����
    int         nMode;      // ����ģʽ, 0=ʮ������, 1=ansi�ַ���, 2=10����, 3=Unicode�ַ���, 4=�����ȸ�����, 5=˫���ȸ�����, ͬʱ��������ʱѡ�е�ģʽ
    int         windowType; // ��������, ���������������ͬ�Ĵ���

    LPWSTR      pRet;       // ��������������ı�
    LPWSTR      pRet2;      // ��������������ı�2
}HEXSEARCH, * PHEXSEARCH;


// ������������ݳߴ�
int HexView_Search_Dialog(PHEXSEARCH arg);

// �ͷ��������ص�����
void HexView_Search_Free(LPVOID ptr);

