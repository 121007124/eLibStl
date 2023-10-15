#pragma once
#include "HexView_Function.h"


typedef struct _HEXJMP
{
    HWND        hParent;    // �Ի��򸸴���
    LPCWSTR     szCaption;  // �Ի������
    PHEX_NODE   listText;   // Ĭ���������ı�����
    int         nMode;      // ��תģʽ, 0=���ļ���ʼ, 1=��ǰλ��, 2����λ������, 3=���ļ�����, ��λ = �Ƿ���16����, ͬʱ��������ʱѡ�е�ģʽ

    LPWSTR      pRet;       // �����������ת�ı�
    INT64       pos;        // �����λ��
    BOOLEAN     isOffset;   // �Ƿ�������ƫ��

    BOOLEAN     isAsm;      // �Ƿ��ǻ�ര�ڽ�������ת
}HEXJMP, * PHEXJMP;


// ������������ݳߴ�
int HexView_Jmp_Dialog(PHEXJMP arg);

// �ͷ��������ص�����
void HexView_Jmp_Free(LPVOID ptr);

