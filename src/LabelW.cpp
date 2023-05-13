#include "EcontrolHelp.h"
#include "GdiplusFlatDef.h"

#include <shlwapi.h>

#define SCID_LABELPARENT	20230512'01u

#define WCN_LABELW			L"eLibStl.WndClass.LabelW"



HBITMAP CreateDIB(HDC h, int cx, int cy, RGBQUAD** pprgb)
{
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = cx;
	bi.bmiHeader.biHeight = cy;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	return CreateDIBSection(h, &bi, DIB_RGB_COLORS, (void**)pprgb, NULL, 0);
}

BOOL DIBHasAlpha(int cx, int cy, RGBQUAD* prgb)
{
	int cTotal = cx * cy;
	for (int i = 0; i < cTotal; i++)
	{
		if (prgb[i].rgbReserved != 0)
			return TRUE;
	}

	return FALSE;
}

void PreProcessDIB(int cx, int cy, RGBQUAD* pargb)
{
	int cTotal = cx * cy;
	for (int i = 0; i < cTotal; i++)
	{
		RGBQUAD* prgb = &pargb[i];
		if (prgb->rgbReserved != 0)
		{
			prgb->rgbRed = ((prgb->rgbRed * prgb->rgbReserved) + 128) / 255;
			prgb->rgbGreen = ((prgb->rgbGreen * prgb->rgbReserved) + 128) / 255;
			prgb->rgbBlue = ((prgb->rgbBlue * prgb->rgbReserved) + 128) / 255;
		}
		else
		{
			*((DWORD*)prgb) = 0;
		}
	}
}


ESTL_NAMESPACE_BEGIN
// ��ǩ
/*
* �汾1���ݲ���
* ELABELDATA�ṹ
* ��ͼ
*/
#define DATA_VER_LABEL_1	1
struct ELABELDATA
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	SIZE_T cbBKPic;			// ��ͼ��С
	int iBKPicMode;			// ��ͼģʽ
	int iAlignH;			// �������
	int iAlignV;			// �������
	BOOL bAutoWrap;			// �Զ�����
	COLORREF crText;		// �ı���ɫ
	COLORREF crTextBK;		// �ı�������ɫ
	COLORREF crBK;			// ������ɫ
	int iGradientMode;		// ���䱳��ģʽ
	COLORREF crGradient[3];// ���䱳����ɫ
	BOOL bEllipsis;			// ʡ�Ժ�
	int iEllipsisMode;		// ʡ�Ժ�ģʽ
	int iPrefixMode;		// ǰ׺ģʽ
};

class CLabel :public elibstl::CCtrlBase
{
	SUBCLASS_MGR_DECL(CLabel)
private:
	ELABELDATA m_Info{};

	int m_cxClient = 0, m_cyClient = 0;// �ͻ�����С
	HDC m_hCDC = NULL;// ��̨����DC
	HDC m_hcdcHelper = NULL;// ��λͼʹ�õĸ���DC
	HBITMAP m_hBitmap = NULL;// ��̨����λͼ
	HGDIOBJ m_hOld1 = NULL/*��̨DC��λͼ���*/, hOld2 = NULL/*����DC��λͼ���*/;
	HBRUSH m_hbrBK = NULL;// ������ˢ
	
	BYTE* m_pBKPicData = NULL;// ��ͼ����
	HBITMAP m_hbmBK = NULL;// ��ͼ

	static ATOM m_atomLabel;// ��ǩ��ԭ��

	void Update(BOOL bShowImmd = TRUE)
	{
		//
		// ������
		//
		RECT rc = { 0,0,m_cxClient,m_cyClient };
		FillRect(m_hCDC, &rc, m_hbrBK);

		SelectObject(m_hcdcHelper, m_hbmBK);
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 255;
		bf.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(m_hCDC, 0, 0, m_cxClient, m_cyClient, m_hcdcHelper, 0, 0, m_cxClient, m_cyClient, bf);
		//
		// ���ı�
		//
		UINT uDTFlags = DT_NOCLIP |
			elibstl::MultiSelect<UINT>(m_Info.iAlignH, DT_LEFT, DT_CENTER, DT_RIGHT) |
			elibstl::MultiSelect<UINT>(m_Info.iEllipsisMode, 0, DT_END_ELLIPSIS, DT_PATH_ELLIPSIS, DT_WORD_ELLIPSIS) |
			elibstl::MultiSelect<UINT>(m_Info.iPrefixMode, 0, DT_NOPREFIX, DT_HIDEPREFIX, DT_PREFIXONLY);
		if (m_Info.bAutoWrap)
		{
			uDTFlags |= (DT_WORDBREAK | DT_CALCRECT);
			// �޸���ֱ���У�DrawText��û�е��б�־ʱ����Դ�ֱ���룬�����ֶ�ʵ��һ��
			DrawTextW(m_hCDC, m_pszTextW, -1, &rc, uDTFlags);
			uDTFlags &= (~DT_CALCRECT);
			int cyText = rc.bottom - rc.top;
			rc.left = 0;
			rc.right = m_cxClient;
			switch (m_Info.iAlignV)
			{
			case 0:// �ϱ�
				rc.top = 0;
				rc.bottom = rc.top + cyText;
				break;
			case 1:// �м�
				rc.top = (m_cyClient - cyText) / 2;
				rc.bottom = rc.top + cyText;
				break;
			case 2:// �±�
				rc.bottom = m_cyClient;
				rc.top = (rc.bottom - cyText) / 2;
				break;
			}
		}
		else
		{
			uDTFlags |= DT_SINGLELINE;
			uDTFlags |= elibstl::MultiSelect<UINT>(m_Info.iAlignV, DT_TOP, DT_VCENTER, DT_BOTTOM);
		}
		DrawTextW(m_hCDC, m_pszTextW, -1, &rc, uDTFlags);
		
		if (bShowImmd)
		{
			HDC hDC = GetDC(m_hWnd);
			BitBlt(hDC, 0, 0, m_cxClient, m_cyClient, m_hCDC, 0, 0, SRCCOPY);
			ReleaseDC(m_hWnd, hDC);
		}
	}

	static LRESULT CALLBACK ParentSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	// δ��
	static LRESULT CALLBACK CtrlSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) { return 0; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto p = (CLabel*)GetWindowLongPtrW(hWnd, 0);
		if (p)
			elibstl::SendToParentsHwnd(p->m_dwWinFormID, p->m_dwUnitID, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_SIZE:
		{
			p->m_cxClient = LOWORD(lParam);
			p->m_cyClient = HIWORD(lParam);
			SelectObject(p->m_hCDC, p->m_hOld1);
			DeleteObject(p->m_hBitmap);
			HDC hDC = GetDC(hWnd);
			p->m_hBitmap = CreateCompatibleBitmap(hDC, p->m_cxClient, p->m_cyClient);
			p->m_hOld1 = SelectObject(p->m_hCDC, p->m_hBitmap);
			ReleaseDC(hWnd, hDC);

			p->Update(FALSE);
		}
		return 0;

		case WM_NCCREATE:
			SetWindowLongPtrW(hWnd, 0, (LONG_PTR)((CREATESTRUCTW*)lParam)->lpCreateParams);
			return TRUE;

		case WM_CREATE:
		{
			HDC hDC = GetDC(hWnd);
			p->m_hCDC = CreateCompatibleDC(hDC);
			p->m_hcdcHelper = CreateCompatibleDC(hDC);
			ReleaseDC(hWnd, hDC);
			p->m_hbrBK = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		}
		return 0;

		case WM_DESTROY:
		{
			m_SM.OnCtrlDestroy(p);
			DeleteDC(p->m_hCDC);
			DeleteDC(p->m_hcdcHelper);
			DeleteObject(p->m_hBitmap);
			DeleteObject(p->m_hbrBK);
			delete p;
		}
		return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			BitBlt(ps.hdc,
				ps.rcPaint.left,
				ps.rcPaint.top,
				ps.rcPaint.right - ps.rcPaint.left,
				ps.rcPaint.bottom - ps.rcPaint.top,
				p->m_hCDC,
				ps.rcPaint.left,
				ps.rcPaint.top,
				SRCCOPY);
			EndPaint(hWnd, &ps);
		}
		return 0;

		case WM_SETFONT:
		{
			SelectObject(p->m_hCDC, (HFONT)wParam);
			p->Update();
		}
		break;

		case WM_SETTEXT:
		{
			p->Update();
		}
		break;
		}

		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
public:
	CLabel() = delete;
	CLabel(STD_ECTRL_CREATE_ARGS)
	{
		if (!m_atomLabel)
		{
			WNDCLASSW wc{};
			wc.cbWndExtra = sizeof(void*);
			wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
			wc.hInstance = g_elibstl_hModule;
			wc.lpfnWndProc = WndProc;
			wc.lpszClassName = WCN_LABELW;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			m_atomLabel = RegisterClassW(&wc);
			if (!m_atomLabel)
			{
				assert(FALSE);
				return;
			}

			ULONG_PTR uToken;
			GdiplusStartupInput gpsi{};
			gpsi.GdiplusVersion = 1;
			GdiplusStartup(&uToken, &gpsi, NULL);
		}

		auto cbBaseData = InitBase0(pAllData, cbData, bInDesignMode, dwWinFormID, dwUnitID);
		if (pAllData)
		{
			BYTE* p = (BYTE*)pAllData + cbBaseData;
			memcpy(&m_Info, p, sizeof(ELABELDATA));

			p += sizeof(ELABELDATA);
			SetBKPic(p, m_Info.cbBKPic);
		}
		else
		{
			m_Info.iAlignV = 1;
			m_Info.crBK = GetSysColor(COLOR_BTNFACE);
			m_Info.crTextBK = CLR_DEFAULT;
		}
		m_Info.iVer = DATA_VER_LABEL_1;

		m_hWnd = CreateWindowExW(0, WCN_LABELW, m_pszTextW, WS_CHILD | WS_CLIPSIBLINGS, 
			x, y, cx, cy, hParent, (HMENU)nID, g_elibstl_hModule, this);
		m_hParent = hParent;
		m_SM.OnCtrlCreate2(this);

		SetClr(0, m_Info.crText);
		SetClr(1, m_Info.crBK);
		SetClr(2, m_Info.crTextBK);

		InitBase0(pAllData);
	}

	~CLabel()
	{
		 
	}

	void SetBKPic(BYTE* pPic, SIZE_T cbSize)
	{
		m_Info.cbBKPic = cbSize;
		if (m_hbmBK)
			DeleteObject(m_hbmBK);

		if (cbSize && pPic)
		{
			if (m_bInDesignMode)
			{
				delete[] m_pBKPicData;
				m_pBKPicData = new BYTE[cbSize];
				memcpy(m_pBKPicData, pPic, cbSize);
			}
			m_hbmBK = elibstl::make_hbm_gp((BYTE*)pPic, cbSize);
		}
		else
		{
			if (m_bInDesignMode)
			{
				delete[] m_pBKPicData;
				m_pBKPicData = NULL;
			}
			m_hbmBK = NULL;
		}
		Update();
	}

	eStlInline BYTE* GetBKPic(int* pcbPic) const
	{
		if (pcbPic)
			*pcbPic = m_Info.cbBKPic;
		return m_pBKPicData;
	}

	eStlInline void SetBKPicMode(int iBKPicMode)
	{
		m_Info.iBKPicMode = iBKPicMode;
		Update();
	}

	eStlInline int GetBKPicMode() const
	{
		return m_Info.iBKPicMode;
	}

	eStlInline void SetAlign(BOOL bHAlign, int iAlign)
	{
		if (bHAlign)
			m_Info.iAlignH = iAlign;
		else
			m_Info.iAlignV = iAlign;
		Update();
	}

	eStlInline int GetAlign(BOOL bHAlign) const
	{
		if (bHAlign)
			return m_Info.iAlignH;
		else
			return m_Info.iAlignV;
	}

	eStlInline void SetAutoWrap(BOOL bAutoWrap)
	{
		m_Info.bAutoWrap = bAutoWrap;
		Update();
	}

	eStlInline BOOL GetAutoWrap() const
	{
		return m_Info.bAutoWrap;
	}

	eStlInline void SetClr(int idx, COLORREF cr)
	{
		switch(idx)
		{
		case 0:
			m_Info.crText = cr;
			SetTextColor(m_hCDC, cr);
			break;
		case 1:
			m_Info.crBK = cr;
			DeleteObject(m_hbrBK);
			m_hbrBK = CreateSolidBrush(cr);
			break;
		case 2:
			m_Info.crTextBK = cr;
			if (cr == CLR_DEFAULT)
				SetBkMode(m_hCDC, TRANSPARENT);
			else
			{
				SetBkMode(m_hCDC, OPAQUE);
				SetBkColor(m_hCDC, cr);
			}
		}

		Update();
	}

	eStlInline COLORREF GetClr(int idx) const
	{
		switch (idx)
		{
		case 0:return m_Info.crText;
		case 1:return m_Info.crBK;
		case 2:return m_Info.crTextBK;
		}
	}

	eStlInline void SetGradientMode(int iGradientMode)
	{
		m_Info.iGradientMode = iGradientMode;
		Update();
	}

	eStlInline int GetGradientMode() const
	{
		return m_Info.iGradientMode;
	}

	eStlInline void SetGradientClr(int idx, COLORREF cr)
	{
		m_Info.crGradient[idx] = cr;
		Update();
	}

	eStlInline COLORREF GetGradientClr(int idx) const
	{
		return m_Info.crGradient[idx];
	}

	eStlInline void SetEllipsisMode(int iEllipsisMode)
	{
		m_Info.iEllipsisMode = iEllipsisMode;
		Update();
	}

	eStlInline int GetEllipsisMode() const
	{
		return m_Info.iEllipsisMode;
	}

	eStlInline void SetPrefixMode(int iPrefixMode)
	{
		m_Info.iPrefixMode = iPrefixMode;
		Update();
	}

	eStlInline int GetPrefixMode() const
	{
		return m_Info.iPrefixMode;
	}

	HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		auto hGlobal = FlattenInfoBase0(sizeof(ELABELDATA) + m_Info.cbBKPic, &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		// �ṹ
		p += cbBaseData;
		memcpy(p, &m_Info, sizeof(ELABELDATA));
		// ��ͼ
		p += sizeof(ELABELDATA);
		memcpy(p, m_pBKPicData, m_Info.cbBKPic);
		// 
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}

	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto pButton = new CLabel(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(pButton->GetHWND());
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0:// ����
			p->SetTextA(pPropertyVaule->m_szText);
			break;
		case 1:// ����W
			p->SetTextW((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;
		case 2:// ͼƬ
			p->SetPic(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize);
			break;
		case 3:// ��ͼ
			p->SetBKPic(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize);
			break;
		case 4:// ��ͼ��ʽ
			p->SetBKPicMode(pPropertyVaule->m_int);
			break;
		case 5:// ����
			p->SetFont((LOGFONTA*)pPropertyVaule->m_int);
			break;
		case 6:// �������
			p->SetAlign(TRUE, pPropertyVaule->m_int);
			break;
		case 7:// �������
			p->SetAlign(FALSE, pPropertyVaule->m_int);
			break;
		case 8:// �Զ�����
			p->SetAutoWrap(pPropertyVaule->m_bool);
			break;
		case 9:// �ı���ɫ
		case 10:// ������ɫ
		case 11:// �ı�������ɫ
			p->SetClr(nPropertyIndex - 9, pPropertyVaule->m_clr);
			break;
		case 12:// ���䱳����ʽ
			p->SetGradientMode(pPropertyVaule->m_int);
			break;
		case 13:// ���䱳����ɫ1
		case 14:// ���䱳����ɫ2
		case 15:// ���䱳����ɫ3
			p->SetGradientClr(nPropertyIndex - 13, pPropertyVaule->m_clr);
			break;		
		case 16:// ʡ�Ժŷ�ʽ
			p->SetEllipsisMode(pPropertyVaule->m_bool);
			break;
		case 17:// ǰ׺�ַ����ͷ�ʽ
			p->SetPrefixMode(pPropertyVaule->m_int);
			break;
		case 18:// �߿�
			p->SetFrame(pPropertyVaule->m_int);
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
			pPropertyVaule->m_szText = p->GetTextA();
			break;
		case 1:// ����W
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetTextW((SIZE_T*)&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 2:// ͼƬ
			pPropertyVaule->m_data.m_pData = p->GetPic(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 3:// ��ͼ
			pPropertyVaule->m_data.m_pData = p->GetBKPic(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 4:// ��ͼ��ʽ
			pPropertyVaule->m_int = p->GetBKPicMode();
			break;
		case 5:// ����
			pPropertyVaule->m_data.m_pData = p->GetFont();
			pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA);
			break;
		case 6:// �������
			pPropertyVaule->m_int = p->GetAlign(TRUE);
			break;
		case 7:// �������
			pPropertyVaule->m_int = p->GetAlign(FALSE);
			break;
		case 8:// �Զ�����
			pPropertyVaule->m_bool = p->GetAutoWrap();
			break;
		case 9:// �ı���ɫ
		case 10:// ������ɫ
		case 11:// �ı�������ɫ
			pPropertyVaule->m_clr = p->GetClr(nPropertyIndex - 9);
			break;
		case 12:// ���䱳����ʽ
			pPropertyVaule->m_int = p->GetGradientMode();
			break;
		case 13:// ���䱳����ɫ1
		case 14:// ���䱳����ɫ2
		case 15:// ���䱳����ɫ3
			pPropertyVaule->m_clr = p->GetGradientClr(nPropertyIndex - 13);
			break;
		case 16:// ʡ�Ժŷ�ʽ
			pPropertyVaule->m_int = p->GetEllipsisMode();
			break;
		case 17:// ǰ׺�ַ����ͷ�ʽ
			pPropertyVaule->m_int = p->GetPrefixMode();
			break;
		case 18:// �߿�
			pPropertyVaule->m_int = p->GetFrame();
			break;
		}

		return TRUE;
	}

	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		*pblModified = FALSE;
		if (nPropertyIndex == 1)
		{
			PWSTR psz;
			if (elibstl::IntputBox(&psz, p->GetTextW()))
			{
				p->SetTextNoCopyW(psz);
				*pblModified = TRUE;
			}
		}
		return FALSE;
	}

	static INT WINAPI ENotify(INT nMsg, DWORD dwParam1, DWORD dwParam2)
	{
		switch (nMsg)
		{
		case NU_GET_CREATE_SIZE_IN_DESIGNER:
		{
			*((int*)dwParam1) = 80;
			*((int*)dwParam2) = 32;
		}
		return TRUE;
		}
		return FALSE;
	}
};
SUBCLASS_MGR_INIT(CLabel, SCID_LABELPARENT, 0)
ATOM CLabel::m_atomLabel = 0;
ESTL_NAMESPACE_END

static UNIT_PROPERTY s_Member_LabelW[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/{ "����", "Text", "", UD_TEXT, _PROP_OS(__OS_WIN), NULL },
	/*001*/{ "����W", "TextW", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL },
	/*002*/{ "ͼƬ", "Picture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*003*/{ "��ͼ", "BKPicture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*004*/		{ "��ͼ��ʽ", "BKPicAlign", "", UD_PICK_INT, _PROP_OS(__OS_WIN) | UW_HAS_INDENT, "���\0""����\0""�ұ�\0""\0" },
	/*005*/{ "����", "Font", "", UD_FONT, _PROP_OS(__OS_WIN) , NULL },
	/*006*/{ "������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0" },
	/*007*/{ "������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0" },
	/*008*/{ "�Ƿ��Զ�����", "AutoWrap", "", UD_BOOL, _PROP_OS(__OS_WIN) , NULL },
	/*009*/{ "�ı���ɫ", "TextClr", "", UD_COLOR, _PROP_OS(__OS_WIN) , NULL },
	/*010*/{ "������ɫ", "BKClr", "", UD_COLOR_BACK, _PROP_OS(__OS_WIN) , NULL },
	/*011*/{ "�ı�������ɫ", "BKClr", "", UD_COLOR_BACK, _PROP_OS(__OS_WIN) , NULL },
	/*012*/{ "���䱳����ʽ", "GradientMode", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0" },
	/*013*/		{ "���䱳����ɫ1", "GradientClr1", "", UD_COLOR_TRANS, _PROP_OS(__OS_WIN) | UW_HAS_INDENT , NULL },
	/*014*/		{ "���䱳����ɫ2", "GradientClr2", "", UD_COLOR_TRANS, _PROP_OS(__OS_WIN) | UW_HAS_INDENT , NULL },
	/*015*/		{ "���䱳����ɫ3", "GradientClr3", "", UD_COLOR_TRANS, _PROP_OS(__OS_WIN) | UW_HAS_INDENT , NULL },
	/*016*/{ "ʡ�Ժŷ�ʽ", "EllipsisMode", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ʡ��\0""ĩβʡ��\0""�м�ʡ��\0""ʡ�Ե���\0""\0" },
	/*017*/{ "ǰ׺�ַ����ͷ�ʽ", "PrefixMode", "", UD_PICK_INT, _PROP_OS(__OS_WIN) , "����\0""������ǰ׺\0""�����»���\0""ֻ��ʾ�»���\0""\0" },
	/*018*/{ "�߿�", "Frame", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
	/*019*/{ "��ͼ�Ƿ��������ؼ�", "FullWndPic", "", UD_BOOL, _PROP_OS(__OS_WIN) , NULL },
};

EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_LabelW(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
		return (PFN_INTERFACE)elibstl::CLabel::ECreate;
	case ITF_NOTIFY_PROPERTY_CHANGED:
		return (PFN_INTERFACE)elibstl::CLabel::EChange;
	case ITF_GET_ALL_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CLabel::EGetAlldata;
	case ITF_GET_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CLabel::EGetData;
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
		return (PFN_INTERFACE)elibstl::CLabel::EInputW;
	case ITF_GET_NOTIFY_RECEIVER:
		return (PFN_INTERFACE)elibstl::CLabel::ENotify;
	}
	return NULL;
}

ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO CtLabelW =
{
	"��ǩW",		//��������
	"LabelW",	//Ӣ������
	"",			//˵��
	0,			//��������
	0,			//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,
	IDB_COMLINKBUTTON_W,//��ԴID
	0,
	NULL,
	ARRAYSIZE(s_Member_LabelW),
	s_Member_LabelW,
	libstl_GetInterface_LabelW,
	0,			//��Ա����
	NULL		//��Ա��������
};
ESTL_NAMESPACE_END