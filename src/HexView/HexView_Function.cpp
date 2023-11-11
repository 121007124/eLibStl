#include "HexView_Function.h"

// ��ת����˳��, ������������û���ظ���, �еĻ����ظ����������һ��λ��, �����Ƿ������˳��
// �������û�е���˳��, ��Ҫ��������ӵ�����ͷ
// ppNode = ָ��ͷ�ڵ�ָ��, ����˳��������µĽڵ�д�뵽�����ַ��
// pStr = ���ҵ��ı�, ���������������ı�, ����оͰ�����ڵ��������һ��λ��
BOOLEAN HexView_AdjustListOrder(PHEX_NODE* ppNode, LPWSTR pStr)
{
    PHEX_NODE node_base = *ppNode;

    // ��ö��һ������, ������û���ظ���, �еĻ����ظ����ó����ŵ���һ��λ��
    PHEX_NODE node = node_base;
    PHEX_NODE find = 0;
    PHEX_NODE prev = 0;
    while ( node )
    {
        if ( wcscmp(node->text, pStr) == 0 )
        {
            find = node;
            if ( prev )
            {
                // ����ڵ㲻���׽ڵ�, �Ǿ�����һ���ڵ�ָ������ڵ����һ���ڵ�
                // Ȼ�������ڵ�ŵ���һ��λ��
                prev->next = node->next;
                find->next = prev;
                *ppNode = find;

                HexViewFree(find->text);
                find->text = pStr;
                return TRUE;
            }

            // ��һ�������ظ����, Ҳ������, �õ��÷������´����ڵ�
            return TRUE;
        }
        prev = node;
        node = node->next;
    }
    return FALSE;
}


LPVOID HexViewAlloc(size_t size, bool isClear)
{
    LPVOID p = ::malloc(size);
    if ( !p )
    {
        MessageBoxW(NULL, L"�����ڴ�ʧ��, �޷�����", L"�����ڴ�ʧ��", MB_ICONERROR);
        throw;
    }
    if ( isClear )
        memset(p, 0, size);
    return p;
}

void HexViewFree(LPVOID p)
{
    if ( p )
        ::free(p);
}

// ����һ���ڵ�, Ȼ���ֵд�뵽�ڵ���
PHEX_NODE HexView_NewNode(LPWSTR pStr, PHEX_NODE pNextNode)
{
    PHEX_NODE node = (PHEX_NODE)HexViewAlloc(sizeof(HEX_NODE));
    node->text = pStr;
    node->next = pNextNode;
    return node;
}

void HexView_FreeList(PHEX_NODE pNode)
{
    PHEX_NODE node = pNode;
    while ( node )
    {
        HexViewFree(node->text);
        PHEX_NODE pFree = node;
        node = node->next;
        free(pFree);
    }
}


BOOLEAN HexView_SetClipboard(LPCWSTR str, int size)
{
    if ( !str )
        return FALSE;
    if ( size <= 0 )
        size = (int)(LONG_PTR)wcslen(str);
    size_t len = size + 1;
    HGLOBAL hMem = GlobalAlloc(GHND, len * 2);
    if ( !hMem )
        return FALSE;
    void* p = GlobalLock(hMem);
    if ( p )
    {
        memcpy(p, str, len * 2);
        GlobalUnlock(hMem);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_UNICODETEXT, hMem);
        //GlobalFree(hMem);
        CloseClipboard();
    }
    return TRUE;
}