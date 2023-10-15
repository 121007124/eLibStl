#include "HexView_Control.h"
#include <unordered_map>
#include "../../include/ElibHelp.h"


namespace elibstl::hexview
{
    using namespace HEXVIEW_NAMESPACE;

    std::unordered_map<HWND, WNDPROC> m_map_subclass_hexview;
    std::unordered_map<HWND, PHEXVIEW_PROPERTY> m_map_hexview;
    HUNIT WINAPI ECreate(LPBYTE pAllData, INT cbData, DWORD dwStyle, HWND hParent, UINT nID, \
                                HMENU hMenu, INT x, INT y, INT cx, INT cy, DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL bInDesignMode
    );
    BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, PSTR* ppszTipText);
    HGLOBAL WINAPI EGetAlldata(HUNIT hUnit);
    BOOL WINAPI EGetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule);
    BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData);
    INT WINAPI ENotify(INT nMsg, DWORD dwParam1, DWORD dwParam2);

    LRESULT CALLBACK WndProc_Parent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

}
EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_HexView(INT nInterfaceNO)
{
    switch ( nInterfaceNO )
    {
    case ITF_CREATE_UNIT:               // �������
        return (PFN_INTERFACE)elibstl::hexview::ECreate;
    case ITF_NOTIFY_PROPERTY_CHANGED:   // ֪ͨĳ�������ݱ��û��޸�
        return (PFN_INTERFACE)elibstl::hexview::EChange;
    case ITF_GET_ALL_PROPERTY_DATA:     // ȡȫ����������
        return (PFN_INTERFACE)elibstl::hexview::EGetAlldata;
    case ITF_GET_PROPERTY_DATA:         // ȡĳ��������
        return (PFN_INTERFACE)elibstl::hexview::EGetData;
    case ITF_DLG_INIT_CUSTOMIZE_DATA:   // ʹ�öԻ��������Զ�������
        return (PFN_INTERFACE)elibstl::hexview::EInputW;
    case ITF_GET_NOTIFY_RECEIVER:       // ȡ����ĸ���֪ͨ������(PFN_ON_NOTIFY_UNIT)
        return (PFN_INTERFACE)elibstl::hexview::ENotify;
    }
    return NULL;
}



namespace elibstl::hexview
{
    // �������
    HUNIT WINAPI ECreate(LPBYTE pAllData, INT cbData, DWORD dwStyle,
                         HWND hParent, UINT nID, HMENU hMenu,
                         INT x, INT y, INT cx, INT cy,
                         DWORD dwWinFormID, DWORD dwUnitID,
                         HWND hDesignWnd, BOOL bInDesignMode)
    {
        PHEXVIEW_PROPERTY pData = new HEXVIEW_PROPERTY;
        memset(pData, 0, sizeof(HEXVIEW_PROPERTY));
        pData->hDesignWnd       = hDesignWnd;
        pData->hWndParent       = hParent;
        pData->id               = nID;
        pData->dwWinFormID      = dwWinFormID;   
        pData->dwUnitID         = dwUnitID;      
        pData->blInDesignMode   = bInDesignMode;
        pData->style            = dwStyle;       
        pData->styleCtl         = 0;             
        pData->styleEx          = 0;             
        pData->hUnit            = 0;
        pData->data = new std::vector<BYTE>;
        pData->modi = new std::vector<BYTE>;
        pData->hWnd = CreateHexView(0, dwStyle | WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      x, y, cx, cy, hParent, nID, pData);
        
        m_map_hexview[hParent] = pData;
        m_map_hexview[pData->hWnd] = pData;

        SetWindowLongPtrW(pData->hWnd, 0, (LONG_PTR)pData);
        auto it = m_map_subclass_hexview.find(hParent);
        if ( it == m_map_subclass_hexview.end() )
        {
           WNDPROC oldProc = (WNDPROC)SetWindowLongPtrA(hParent, GWLP_WNDPROC, (LONG_PTR)WndProc_Parent);
           pData->oldProc = oldProc;
        }
        return elibstl::make_cwnd(pData->hWnd);
    }

    // ֪ͨĳ�������ݱ��û��޸�
    BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, PSTR* ppszTipText)
    {
        switch ( nPropertyIndex )
        {
        case 0:
            break;
        default:
            break;
        }
        return 0;
    }

    // ȡȫ����������
    HGLOBAL WINAPI EGetAlldata(HUNIT hUnit)
    {
        // TODO ������Ҫ��ȡȫ�����Ա�������
        std::vector<BYTE> buffer;
        const size_t size = buffer.size();
        HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size);
        if ( hGlobal )
        {
            PVOID pGlobal = ::GlobalLock(hGlobal);
            if ( pGlobal && size > 0 )
            {
                LPCVOID pBuffer = &buffer[0];
                memcpy(pGlobal, pBuffer, size);
                ::GlobalUnlock(hGlobal);
            }

        }

        return hGlobal;
    }

    // ȡĳ��������
    BOOL WINAPI EGetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
    {
        // ȡĳ�������ݵ�pPropertyValue��, �ɹ�������, ���򷵻ؼ�
        // ע�⣺��������ʱ(�ɵ���PFN_CREATE_UNITʱ��blInDesignMode��������)
        // pPropertyValue���뷵�����洢��ֵ�����������ʱ(blInDesignModeΪ��)
        // ���뷵��ʵ�ʵĵ�ǰʵʱֵ
        // ����˵, �༭�򴰿ڵ�Ԫ��"����"����, ���ʱ���뷵���ڲ��������ֵ
        // ������ʱ�ͱ������ GetWindowText ȥʵʱ��ȡ
        switch ( nPropertyIndex )
        {
        case 0:
            break;
        default:
            break;
        }
        return 0;
    }

    // �����Ի��򷽷�
    BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
    {
        // ������������ΪUD_CUSTOMIZE�ĵ�Ԫ����
        // �����Ҫ���´����õ�Ԫ�����޸ĵ�Ԫ����, �뷵����
        // pblModified ��ΪNULL, �������з����Ƿ��û������޸�(����������IDE����UNDO��¼)

        return 0;
    }

    // ȡ����ĸ���֪ͨ������(PFN_ON_NOTIFY_UNIT)
    INT WINAPI ENotify(INT nMsg, DWORD dwParam1, DWORD dwParam2)
    {
        switch ( nMsg )
        {
        case NU_GET_CREATE_SIZE_IN_DESIGNER:
        {
            // ȡ���ʱ������������õ�������ʱ��Ĭ�ϴ����ߴ�
            // dwParam1: ����: INT*, ���ؿ��(��λ����)
            // dwParam2: ����: INT*, ���ظ߶�(��λ����)
            // �ɹ�����1,ʧ�ܷ���0.
            *( (int*)dwParam1 ) = 200; // ����Ϸŵ�ide�Ͼ�ʹ�����Ĭ�Ͽ��
            *( (int*)dwParam2 ) = 100; // ����Ϸŵ�ide�Ͼ�ʹ�����Ĭ�ϸ߶�
            return 1; // ʹ���޸ĺ�Ŀ��, �Լ�������Ҫ�޸�
            break;
        }
        default:
            return 0;
        }
        return 0;
    }

    LRESULT CALLBACK OnNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, PHEXVIEW_PROPERTY pData, bool& isDeal)
    {
        LPNMHDR hdr = (LPNMHDR)lParam;
        if ( hdr->hwndFrom != pData->hWnd )
            return DefWindowProcW(hWnd, message, wParam, lParam);

        isDeal = true;
        switch ( hdr->code )
        {
        case HVN_SEARCH:    // ������Ŀ
            //return _search_bin((PNMHEXSEARCH)lParam, mem);
        case HVN_GETDISPINFO:
        {
            PNMHVDISPINFO DispInfo = (PNMHVDISPINFO)lParam;
            const SIZE_T index = DispInfo->Item.NumberOfItem;
            std::vector<BYTE>& data = *pData->data;
            std::vector<BYTE>& modi = *pData->modi;
            if ( index >= data.size() )
                break;  // Խ����

            LPBYTE pBase = (LPBYTE)( &data[index] );
            if ( DispInfo->Item.Mask & HVIF_ADDRESS )
            {
                DispInfo->Item.Address = (ULONG64)pData->pAddress + index;
            }
            if ( DispInfo->Item.Mask & HVIF_BYTE )
            {
                DispInfo->Item.Value = *pBase;
                DispInfo->Item.State = ( modi[index] ) ? HVIS_MODIFIED : 0;
            }
            break;
        }
        case HVN_ITEMCHANGING:
        {
            PNMHEXVIEW HexView = (PNMHEXVIEW)lParam;
            const SIZE_T index = HexView->Item.NumberOfItem;
            std::vector<BYTE>& data = *pData->data;
            std::vector<BYTE>& modi = *pData->modi;
            if ( index >= data.size() )
                break;  // Խ����
            LPBYTE pBase = (LPBYTE)( &data[index] );
            *pBase = HexView->Item.Value;
            modi[index] = true;    // ��¼�����Ա�Ѿ��޸�
            break;
        }
        case HVN_COPYDATA:
        {
            PNMHEXCOPYDATA arg = (PNMHEXCOPYDATA)lParam;
            std::vector<BYTE>& data = *pData->data;
            size_t size = data.size();
            if ( arg->nSize > size )
                arg->nSize = size;
            arg->Address = (ULONG64)pData->pAddress;
            memcpy(arg->pBuf, &data[0], arg->nSize);
            return 1;
        }
        default:
            isDeal = false;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
        return 0;
    }


    LRESULT CALLBACK WndProc_Parent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PHEXVIEW_PROPERTY pData = m_map_hexview[hWnd];
        if ( !pData )
            return DefWindowProcA(hWnd, message, wParam, lParam);
        switch ( message )
        {
        case WM_NOTIFY:
        {
            bool isDeal = false;
            LRESULT ret = OnNotify(hWnd, message, wParam, lParam, pData, isDeal);
            return ret;
            return isDeal;
        }
        default:
            break;
        }
        return CallWindowProcA(pData->oldProc, hWnd, message, wParam, lParam);
    }

}

namespace elibstl
{

    // ��������, ��HexView_CmdDef.cpp�ﶨ��
    static INT s_dtCmdIndexcommobj_HexView[] = { 325 };

    // �¼�
    static EVENT_INFO2 s_Event_HexView[] =
    {
        /*000*/ {"�¼����༭", "�ȴ����������", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
        
    };

    // ����
    static UNIT_PROPERTY s_Member_HexView[] =
    {
        FIXED_WIN_UNIT_PROPERTY,
        //1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

        /*000*/ {"������", "Def", "������", UD_INT, _PROP_OS(__OS_WIN), ""},
        
    };
    //1=��������,2=Ӣ������,3=��ϸ����,4=��������,5=����ֵ,6=��־ LDT_
    //����Ϊ���ڻ�˵����ʱ��Ч 7=��ԴID,8=�¼�����,9=����¼�����,10=������  11=�������� 12=��������ӳ���
    //��Ϊ���ڡ��˵������Ϊö����������ʱ����Ч 13=��Ա����,14=��Ա��������
    LIB_DATA_TYPE_INFO HexView_control =
    {
        "���ƿ�",
        "HexView",
        "16���Ʊ༭��",
        sizeof(s_dtCmdIndexcommobj_HexView) / sizeof(s_dtCmdIndexcommobj_HexView[0]),
         s_dtCmdIndexcommobj_HexView,
        _DT_OS(__OS_WIN) | LDT_WIN_UNIT,// | LDT_IS_HIDED,
        IDB_BUTTON_W,
        ARRAYSIZE(s_Event_HexView),
        s_Event_HexView,
        ARRAYSIZE(s_Member_HexView),
        s_Member_HexView,
        libstl_GetInterface_HexView,
        0,
        0
    };
}