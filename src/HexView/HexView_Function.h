#ifndef _HEXVIEW_FUNCTION_H_
#define _HEXVIEW_FUNCTION_H_
#include <windows.h>

typedef struct _HEX_NODE
{
    LPWSTR text;
    struct _HEX_NODE* next;
}HEX_NODE, * PHEX_NODE;


// ��ת����˳��, ������������û���ظ���, �еĻ����ظ����������һ��λ��, �����Ƿ������˳��
// �������û�е���˳��, ��Ҫ��������ӵ�����ͷ
// ppNode = ָ��ͷ�ڵ�ָ��, ����˳��������µĽڵ�д�뵽�����ַ��
// pStr = ���ҵ��ı�, ���������������ı�, ����оͰ�����ڵ��������һ��λ��
BOOLEAN HexView_AdjustListOrder(PHEX_NODE* ppNode, LPWSTR pStr);

LPVOID HexViewAlloc(size_t size, bool isClear = false);
void HexViewFree(LPVOID p);

// ����һ���ڵ�, Ȼ���ֵд�뵽�ڵ���
PHEX_NODE HexView_NewNode(LPWSTR pStr, PHEX_NODE pNextNode);

void HexView_FreeList(PHEX_NODE pNode);
BOOLEAN HexView_SetClipboard(LPCWSTR str, int size);


#endif // _HEXVIEW_FUNCTION_H_