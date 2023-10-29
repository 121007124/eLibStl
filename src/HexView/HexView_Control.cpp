#include "HexView_Help.h"
#ifdef _DEBUG
#include <assert.h>
#else
#   ifndef assert
#       define assert(_s) 
#   endif
#endif

namespace elibstl::hexview
{
    using namespace HEXVIEW_NAMESPACE;

    // ��¼�������������ڴ�, ����Ϊ���ھ��
    static std::unordered_map<HWND, PCONTROL_STRUCT_BASE> m_map;

    // ��Ҫ�ͷŵ�����, һ�������Ӧһ���ṹ
    static std::vector<PCONTROL_STRUCT_BASE> s_free_ptr;

    HUNIT WINAPI ECreate(LPBYTE pAllData, INT cbData, DWORD dwStyle, HWND hParent, UINT nID, \
                                HMENU hMenu, INT x, INT y, INT cx, INT cy, DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL bInDesignMode
    );
    BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, PSTR* ppszTipText);
    HGLOBAL WINAPI EGetAlldata(HUNIT hUnit);
    BOOL WINAPI EGetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule);
    BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData);
    INT WINAPI ENotify(INT nMsg, DWORD dwParam1, DWORD dwParam2);

    LRESULT CALLBACK OnNotify(HWND hWnd, LPNMHDR hdr, LPVOID lpParam);



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
#define PROP_WNDUNIT L"{949F3071-FD14-4C66-84D5-A92E0641EFFE}"
    inline void SetWndUnit(PCONTROL_STRUCT_BASE data, bool isClose)
    {
        ::SetPropW(data->hWnd, PROP_WNDUNIT, (HANDLE)data->hUnit);
        if ( isClose )
            PostMessageW(data->hWnd, WM_CLOSE, 0, 0);
    }
    inline HUNIT GetWndUnit(HWND hWnd)
    {
        //return (HUNIT)GetPropW(hWnd, PROP_WNDUNIT);
        return (HUNIT)RemovePropW(hWnd, PROP_WNDUNIT);
    }

    inline static bool InitDataEx(PHEXVIEW_PROPERTY& data, bool blInDesignMode)
    {
        if ( !data )
        {
            // �������´����Ĵ��ڲų�ʼ��������Ϣ, �Ѿ������Ĵ������Զ��м�¼��
            data = new HEXVIEW_PROPERTY;
            data->blInDesignMode = blInDesignMode;
            return true;
        }

        if ( data->hUnit )
        {
            //HWND hWndOld = (HWND)NotifySys(NAS_DETACH_CWND_OBJECT, (DWORD)data->hUnit, 0);
            SetWndUnit(data, true);
            //data->pCtl->Destroy();
        }

        m_map.erase(data->hWnd);
        return false;
    }

    inline HFONT Ctl_SetFontData(LOGFONTA& font, LONG& fontSize, HFONT& hFont, LPLOGFONTA lfA)
    {
        if ( hFont )
            DeleteObject(hFont);

        if ( !lfA )
        {
            LOGFONTW lf = { 0 };
            SystemParametersInfoW(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
            lf.lfHeight = -12;
            lf.lfWeight = FW_NORMAL;
            lf.lfItalic = 0;
            lf.lfUnderline = 0;
            lf.lfStrikeOut = 0;
            lf.lfCharSet = GB2312_CHARSET;
            memcpy(lf.lfFaceName, L"����", 6);
            hFont = CreateFontIndirectW(&lf);
            GetObjectA(hFont, sizeof(LOGFONTA), &font);
            fontSize = 0;
        }
        else
        {
            fontSize = sizeof(font);
            memcpy(&font, lfA, fontSize);
            hFont = CreateFontIndirectA(&font);
        }
        return hFont;
    }

    // ��������, ������ؼ��Ǿ���Ҫ����Ĭ��ֵ, �������ʾ������
    static bool ParsePropData(PHEXVIEW_PROPERTY pData, LPBYTE pAllPropertyData, INT nAllPropertyDataSize)
    {
        LPBYTE pBuf = pAllPropertyData;
        HEXVIEW_PROPERTY& prop = *pData;
        if ( !pBuf )
            return false;   // û�д�������, ���ؼ�, �ⲿ��Ĭ��ֵ
        prop.version        = read_int32(pBuf);     // �汾��, ���������ʾ��������, �ڲ�ʹ��, �����ж����ݽṹ��
        prop.border         = read_int32(pBuf);     // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
        prop.TextColor      = read_int32(pBuf);     // �ı���ɫ
        prop.TextBackColor  = read_int32(pBuf);     // �ı�������ɫ
        prop.BackColor      = read_int32(pBuf);     // ������ɫ
        prop.Editable       = read_int32(pBuf);     // ����༭, ������ָ���Ƿ�����༭

        prop.fontSize = read_font(pBuf, prop.font);      // 1. ��ȡ����, д�����A��ṹ, �Ժ�ʹ������ṹ, ����һ���ò���W��
        Ctl_SetFontData(prop.font, prop.fontSize, pData->hFont, prop.fontSize ? &prop.font : 0);

        size_t nReadSize = (LPBYTE)pBuf - pAllPropertyData;
        if ( nReadSize == (size_t)nAllPropertyDataSize )
            return true; // �Ѿ�������β����

        // Ϊ�����汾׼��

        return false;
    }
    // �������
    HUNIT WINAPI ECreate(LPBYTE pAllData, INT cbData, DWORD dwStyle,
                         HWND hParent, UINT nID, HMENU hMenu,
                         INT x, INT y, INT cx, INT cy,
                         DWORD dwWinFormID, DWORD dwUnitID,
                         HWND hDesignWnd, BOOL bInDesignMode)
    {

        //PHEXVIEW_PROPERTY pData = 0;
        //HUNIT hUnit = GethUnitFromId(dwWinFormID, dwUnitID, (PVOID*)&pData);

        //if ( InitDataEx(pData, bInDesignMode) )
        //{
        //    // �������´����Ĵ��ڲų�ʼ��������Ϣ, �Ѿ������Ĵ������Զ��м�¼��
        //    s_free_ptr.push_back(pData);
        //}


        PHEXVIEW_PROPERTY pData = new HEXVIEW_PROPERTY;
        memset(pData, 0, sizeof(HEXVIEW_PROPERTY));

        if ( !ParsePropData(pData, pAllData, cbData) )
        {
            HEXVIEW_PROPERTY& prop = *pData;
            prop.BackColor = RGB(255, 255, 255);
            prop.TextColor = RGB(0, 0, 0);
            prop.Editable = false;
            //prop.TextBackColor = 

            // Ĭ����ɫ�����������ɫֵ����
            //HexView->clrTextSel = RGB(255, 255, 255);
            //HexView->clrText1 = RGB(0, 0, 128);
            //HexView->clrText2 = RGB(0, 0, 255);
            //HexView->clrTextSel1 = RGB(255, 255, 127);
            //HexView->clrTextSel2 = RGB(255, 255, 0);
            //HexView->clrTextAddress = RGB(0, 0, 0);
            //HexView->clrLine = RGB(204, 206, 219);
            //HexView->clrTextBackground = RGB(255, 255, 255);
            //HexView->clrSelTextBack = RGB(8, 36, 107);
            //HexView->clrModifiedText = RGB(255, 0, 0);
            //HexView->clrModifiedTextSel = RGB(0, 255, 255);
        }
        pData->hDesignWnd = hDesignWnd;
        pData->hWndParent = hParent;
        pData->id = nID;
        pData->dwWinFormID = dwWinFormID;
        pData->dwUnitID = dwUnitID;
        pData->blInDesignMode = bInDesignMode;
        pData->style = dwStyle;
        pData->styleCtl = 0;
        pData->styleEx = 0;
        pData->hUnit = 0;
        pData->data = new std::vector<BYTE>;
        pData->modi = new std::vector<BYTE>;
        pData->hWnd = CreateHexView(0, dwStyle | WS_VISIBLE | WS_CHILD,
                                      x, y, cx, cy, hParent, nID, pData);
        
        SetWindowLongPtrW(pData->hWnd, 0, (LONG_PTR)pData);
        HexView_BindNotify(pData->hWnd, OnNotify, pData);
        return elibstl::make_cwnd(pData->hWnd);
    }


    // ֪ͨĳ�������ݱ��û��޸�
    BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, PSTR* ppszTipText)
    {
        return false;
        HWND hHexView = elibstl::get_hwnd_from_hunit(hUnit);
        if ( !hHexView ) return 0;
        PHEXVIEW_PROPERTY pData = (PHEXVIEW_PROPERTY)GetWindowLongPtrW(hHexView, 0);
        if ( !pData ) return 0;
        HEXVIEW_PROPERTY& prop = *pData;

        switch ( nPropertyIndex )
        {
        case HEXVIEW_PROP_BORDER:       // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
            prop.border = min(max(0, pPropertyVaule->m_int), 5);
            elibstl::ChangeBorder(pData->hWnd, prop.border);
            break;
        case HEXVIEW_PROP_TEXTCOLOR:    // �ı���ɫ
            prop.TextColor = pPropertyVaule->m_int;
            InvalidateRect(pData->hWnd, 0, 0);
            break;
        case HEXVIEW_PROP_TEXTBACKCOLOR:// �ı�������ɫ
            prop.TextBackColor = pPropertyVaule->m_int;
            InvalidateRect(pData->hWnd, 0, 0);
            break;
        case HEXVIEW_PROP_BACKCOLOR:    // ������ɫ
            prop.BackColor = pPropertyVaule->m_int;
            InvalidateRect(pData->hWnd, 0, 0);
            break;
        case HEXVIEW_PROP_FONT:         // ��������
        {
            Ctl_SetFontData(prop.font, prop.fontSize, pData->hFont, (LPLOGFONTA)pPropertyVaule->m_data.m_pData);
            SendMessageW(pData->hWnd, WM_SETFONT, (WPARAM)pData->hFont, 1);
            //InvalidateRect(pData->hWnd, 0, 0);
            break;
        }
        case HEXVIEW_PROP_EDITABLE:     // ����༭, ������ָ���Ƿ�����༭
            prop.Editable = pPropertyVaule->m_int;
            break;
        default:
            break;
        }
        return 0;
    }

    // ȡȫ����������
    HGLOBAL WINAPI EGetAlldata(HUNIT hUnit)
    {
        return 0;
        HWND hHexView = elibstl::get_hwnd_from_hunit(hUnit);
        if ( !hHexView ) return 0;
        PHEXVIEW_PROPERTY pData = (PHEXVIEW_PROPERTY)GetWindowLongPtrW(hHexView, 0);
        if ( !pData ) return 0;
        HEXVIEW_PROPERTY& prop = *pData;

        std::vector<BYTE> buffer;
        buffer.reserve(0x1000);
        buffer.clear();
        int size = 0;                // ʵ��д�뻺�������ֽ���

        size += write_int32(buffer, HEXVIEW_VERSION     );  // �汾��, ÿ��д��Ķ������°�İ汾��
        size += write_int32(buffer, prop.border         );  // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
        size += write_int32(buffer, prop.TextColor      );  // �ı���ɫ
        size += write_int32(buffer, prop.TextBackColor  );  // �ı�������ɫ
        size += write_int32(buffer, prop.BackColor      );  // ������ɫ
        size += write_int32(buffer, prop.Editable       );  // ����༭, ������ָ���Ƿ�����༭

        size += write_font(buffer, &prop.font, prop.fontSize);  // д������, д�����A��ṹ, �Ժ�ʹ������ṹ, ����һ���ò���W��


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

        assert(size == (int)buffer.size());

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

        return false;
        HWND hHexView = elibstl::get_hwnd_from_hunit(hUnit);
        if ( !hHexView ) return 0;
        PHEXVIEW_PROPERTY pData = (PHEXVIEW_PROPERTY)GetWindowLongPtrW(hHexView, 0);
        if ( !pData ) return 0;
        HEXVIEW_PROPERTY& prop = *pData;

        switch ( nPropertyIndex )
        {
        case HEXVIEW_PROP_BORDER:       // �߿�, 0=�ޱ߿�, 1=����ʽ, 2=͹��ʽ, 3=ǳ����ʽ, 4=����ʽ, 5=���߱߿�ʽ
            pPropertyVaule->m_int = prop.border;
            break;
        case HEXVIEW_PROP_TEXTCOLOR:    // �ı���ɫ
            pPropertyVaule->m_int = prop.TextColor;
            break;
        case HEXVIEW_PROP_TEXTBACKCOLOR:// �ı�������ɫ
            pPropertyVaule->m_int = prop.TextBackColor;
            break;
        case HEXVIEW_PROP_BACKCOLOR:    // ������ɫ
            pPropertyVaule->m_int = prop.BackColor;
            break;
        case HEXVIEW_PROP_FONT:         // ��������
        {
            if ( prop.fontSize > 0 )
            {
                pPropertyVaule->m_data.m_pData = (LPBYTE)&prop.font;
                pPropertyVaule->m_data.m_nDataSize = sizeof(prop.font);
            }
            break;
        }
        case HEXVIEW_PROP_EDITABLE:     // ����༭, ������ָ���Ƿ�����༭
            pPropertyVaule->m_int = prop.Editable;
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

    LRESULT CALLBACK OnNotify(HWND hWnd, LPNMHDR hdr, LPVOID lpParam)
    {
        PHEXVIEW_PROPERTY pData = (PHEXVIEW_PROPERTY)lpParam;

        switch ( hdr->code )
        {
        case HVN_SEARCH:    // ������Ŀ
            //return _search_bin((PNMHEXSEARCH)lParam, mem);
        case HVN_GETDISPINFO:
        {
            PNMHVDISPINFO DispInfo = (PNMHVDISPINFO)hdr;
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
            PNMHEXVIEW HexView = (PNMHEXVIEW)hdr;
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
            PNMHEXCOPYDATA arg = (PNMHEXCOPYDATA)hdr;
            std::vector<BYTE>& data = *pData->data;
            size_t size = data.size();
            if ( arg->nSize > size )
                arg->nSize = size;
            arg->Address = (ULONG64)pData->pAddress;
            memcpy(arg->pBuf, &data[0], arg->nSize);
            return 1;
        }
        default:
            return 0;
        }
        return 0;
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
        //FIXED_WIN_UNIT_PROPERTY,
        /*000*/ {"���", "left", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
        /*001*/ {"����", "top", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
        /*002*/ {"���", "width", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
        /*003*/ {"�߶�", "height", NULL, UD_INT, _PROP_OS(OS_ALL), NULL},
        /*004*/ {"���", "tag", NULL, UD_TEXT, _PROP_OS(OS_ALL), NULL},
        /*005*/ {"����", "visible", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL},
        /*006*/ {"��ֹ", "disable", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL},
        /*007*/ {"���ָ��", "MousePointer", NULL, UD_CURSOR, _PROP_OS(OS_ALL), NULL},

        // ������������������Ҫ��Ĭ������, ÿ�������������, ���ǿ����ֶ�����
        // ��������������������, ��Щ����ǿ����Ҫ��
        // ��Щ�����������ԵĻص����Ǵ�0��ʼ��, ����, ��Щ���Ҳ��0��ʼ
         
    //    //1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����
    

        /*000*/ {"�߿�", "border", NULL, UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
        
        /*001*/ {"�ı���ɫ1", "TextColor1", "�ı���ɫ, ��һ�����ӵ���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL },
        /*002*/ {"�ı���ɫ2", "TextColor2", "�ı���ɫ, �ڶ������ӵ���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL },
        /*003*/ {"�ı�ѡ����ɫ1", "TextColorSel1", "�ı�ѡ����ɫ, ��һ�����ӵ���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL },
        /*004*/ {"�ı�ѡ����ɫ2", "TextColorSel2", "�ı�ѡ����ɫ, �ڶ������ӵ���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL },
        
        /*005*/ {"������ɫ", "BackColor", "������ɫ", UD_COLOR_BACK, _PROP_OS(__OS_WIN), NULL },
        /*006*/ {"�ı�ѡ�б���ɫ", "TextBackColorSel", "ѡ���ı�������ɫ", UD_COLOR_BACK, _PROP_OS(__OS_WIN), NULL },
        
        /*007*/ {"�ı���ɫ_�ı���", "clrText_text", "�ұ��ַ���", UD_COLOR, _PROP_OS(__OS_WIN), NULL },
        /*008*/ {"�ı���ַ��ɫ", "TextColorAddress", "��ߵ�ַ���ı���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL},

        /*009*/ {"���޸��ı���ɫ", "clrModifiedText", "�޸Ĺ����ı���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL},
        /*010*/ {"���޸��ı�ѡ����ɫ", "clrModifiedTextSel", "�޸Ĺ����ı���ɫ, ѡ�е���ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL},
        /*011*/ {"������ɫ", "clrLine", "������ɫ", UD_COLOR, _PROP_OS(__OS_WIN), NULL},
        
        /*012*/ {"����", "font", "�ǵȿ��������ʾ�쳣, ��ѡ��ȿ�����", UD_FONT, _PROP_OS(__OS_WIN), NULL },
        /*013*/ {"����༭", "Editable", "�Ƿ�����༭, Ĭ�ϲ��ɱ༭", UD_BOOL, _PROP_OS(__OS_WIN), NULL},

    //COLORREF    clrTextSel;         // �ı���ɫ, ѡ�е��ı���ɫ, �ұ��ַ���
    //COLORREF    clrTextAddress;     // �ı���ɫ, ��ߵ�ַ����ɫ
    //COLORREF    clrText1;           // �ı���ɫ, ��һ�����ӵ���ɫ
    //COLORREF    clrText2;           // �ı���ɫ, �ڶ������ӵ���ɫ
    //COLORREF    clrTextSel1;        // ѡ���ı���ɫ, ��һ�����ӵ���ɫ
    //COLORREF    clrTextSel2;        // ѡ���ı���ɫ, �ڶ������ӵ���ɫ
    //COLORREF    clrTextBackground;  // �ı�������ɫ
    //COLORREF    clrSelTextBack;     // ѡ���ı�������ɫ
    //COLORREF    clrModifiedText;    // �޸Ĺ����ı���ɫ
    //COLORREF    clrModifiedTextSel; // �޸Ĺ����ı���ɫ, ѡ�е���ɫ
    //COLORREF    clrLine;            // ������ɫ, �����߿�ɫ
        
    };
    //1=��������,2=Ӣ������,3=��ϸ����,4=��������,5=����ֵ,6=��־ LDT_
    //����Ϊ���ڻ�˵����ʱ��Ч 7=��ԴID,8=�¼�����,9=����¼�����,10=������  11=�������� 12=��������ӳ���
    //��Ϊ���ڡ��˵������Ϊö����������ʱ����Ч 13=��Ա����,14=��Ա��������
    LIB_DATA_TYPE_INFO HexView_control =
    {
        "ʮ�����Ʊ༭��",
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