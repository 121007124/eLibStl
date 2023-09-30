//���������ͨ�ú��������ͷ�ļ�
#include "ElibHelp.h"
#include "GdiplusFlatDef.h"
#include <CommCtrl.h>
#include <unordered_map>
#include <Shlwapi.h>
#include <windowsx.h>
#include <commoncontrols.h>
/*�׶���ı�ǩ�����¼�*/
#define WM_EFEEDBACK  32885


ESTL_NAMESPACE_BEGIN
eStlInline void SetLBText(HWND hLB, int idx, PCWSTR pszText)
{
	int iCurrSel = SendMessageW(hLB, LB_GETCURSEL, 0, 0);
	auto lParam = SendMessageW(hLB, LB_GETITEMDATA, idx, 0);
	SendMessageW(hLB, LB_DELETESTRING, idx, 0);
	SendMessageW(hLB, LB_INSERTSTRING, idx, (LPARAM)(pszText ? pszText : L""));
	SendMessageW(hLB, LB_SETITEMDATA, idx, lParam);
	SendMessageW(hLB, LB_SETCURSEL, iCurrSel, 0);
}

void GetDataFromHBIT(HBITMAP hBitmap, std::vector<unsigned char>& pData);

/// <summary>
/// �������塣
/// easy font
/// ��CreateFont wrapper��
/// </summary>
/// <param name="pszFontName">������</param>
/// <param name="nPoint">����</param>
/// <param name="nWeight">��ϸ��1~1000</param>
/// <param name="IsItalic">�Ƿ���б</param>
/// <param name="IsUnderline">�Ƿ��»���</param>
/// <param name="IsStrikeOut">�Ƿ�ɾ����</param>
/// <returns>�ɹ�������������ʧ�ܷ���NULL</returns>
HFONT EzFont(PCWSTR pszFontName, int iPoint = 9, int iWeight = 400, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bStrikeOut = FALSE);

#define ESTL_EZDLG_NAMESPACE_BEGIN namespace EzDlg {
#define ESTL_EZDLG_NAMESPACE_END }

ESTL_EZDLG_NAMESPACE_BEGIN
/// <summary>
/// ���ô������塣
/// ����ö�ٴ��ڵ������Ӵ��ڲ�Ϥ����������
/// </summary>
/// <param name="hWnd">���ھ��</param>
/// <param name="hFont">������</param>
/// <param name="bRedraw">�Ƿ��ػ�</param>
eStlInline void SetFontForWndAndCtrl(HWND hWnd, HFONT hFont, BOOL bRedraw = FALSE)
{
	EnumChildWindows(hWnd,
		[](HWND hWnd, LPARAM lParam)->BOOL
		{
			SendMessageW(hWnd, WM_SETFONT, lParam, FALSE);
			return TRUE;
		}, (LPARAM)hFont);
	if (bRedraw)
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

/// <summary>
/// ģ̬�Ի���Ԥ����
/// �˺�����ģ��ģ̬�Ի���ʹ�á�
/// ���ø�����
/// </summary>
/// <param name="hParent">������</param>
/// <returns>���ظ������ڵ��ú���ǰ�Ƿ����ã�����DlgEndModelʱ���ݴ�ֵ</returns>
eStlInline BOOL DlgPreModel(HWND hParent)
{
	BOOL bEnabled = IsWindowEnabled(hParent);
	if (bEnabled)
		EnableWindow(hParent, FALSE);
	return bEnabled;
}

/// <summary>
/// ģ̬�Ի��������
/// �˺�����ģ��ģ̬�Ի���ʹ�ã�����Ӧֱ��ʹ��DestroyWindow��
/// ���ø����ڲ�ʹ���ý��㡣ע�⣺���ø����ڽ����Ǳ�Ҫ�ģ������������ڽ������ý���Ӷ����´���
/// </summary>
/// <param name="hWnd">�Ի��򴰿�</param>
/// <param name="hParent">������</param>
/// <param name="bParentShouldBeEnabled">DlgPreModel�ķ���ֵ</param>
eStlInline void DlgEndModel(HWND hWnd, HWND hParent, BOOL bParentShouldBeEnabled)
{
	if (bParentShouldBeEnabled)
		EnableWindow(hParent, TRUE);
	SetFocus(hParent);
	DestroyWindow(hWnd);
}

/// <summary>
/// ģ̬�Ի��򽫱����١�
/// �˺�����ģ��ģ̬�Ի���ʹ�á�
/// ��ҪΪ�˷�ֹһЩ���������ʹδͨ��DlgEndModel���ٵĶԻ���ĸ�����Ҳ�ܱ����
/// </summary>
/// <param name="hWnd">�Ի��򴰿�</param>
/// <param name="hParent">������</param>
/// <param name="bParentShouldBeEnabled">DlgPreModel�ķ���ֵ</param>
/// <returns>�Ի���δͨ��DlgEndModel����ʱ����TRUE�����򷵻�FALSE</returns>
eStlInline BOOL DlgModelOnDestroy(HWND hWnd, HWND hParent, BOOL bParentShouldBeEnabled)
{
	if (bParentShouldBeEnabled && !IsWindowEnabled(hParent))// ��ֹһЩ�����������
	{
		EnableWindow(hParent, TRUE);
		return TRUE;
	}
	else
		return FALSE;
}

// IDE�Ի��������Ļ������ݣ����������ı����ɴ�����
struct EDLGCTX_BASE
{
	HWND hDlg;
	HWND hParent;
	BOOL bParentEnabled;
	HFONT hFont;
	BOOL bOK;
};

/// <summary>
/// ׼����ʾ��IDE�Ի���
/// ����ע�ᴰ���ಢΪ�����Ľṹ�����ڴ档
/// ģ������������Ľṹ����
/// </summary>
/// <param name="pAtom">��ԭ�Ӿ�̬����ָ��</param>
/// <param name="pszWndClass">��������</param>
/// <param name="pfnWndProc">���ڹ���</param>
/// <returns>����������ָ��</returns>
template<class T>
T* EIDEDlgPreShow(ATOM* pAtom, PCWSTR pszWndClass, WNDPROC pfnWndProc)
{
	if (!*pAtom)
	{
		WNDCLASSW wc{};
		wc.lpszClassName = pszWndClass;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.hIcon = (HICON)GetClassLongPtrW((HWND)NotifySys(NES_GET_MAIN_HWND, 0, 0), GCLP_HICON);
		wc.style = CS_VREDRAW | CS_HREDRAW;
		wc.hInstance = g_elibstl_hModule;
		wc.lpfnWndProc = pfnWndProc;
		wc.cbWndExtra = sizeof(void*);
		*pAtom = RegisterClassW(&wc);
	}

	auto pCtx = new T;
	ZeroMemory(pCtx, sizeof(T));
	return pCtx;
}

/// <summary>
/// ��ʾ��IDE�Ի���
/// ��������IDE�����ڣ�Ȼ�󴴽����ڲ�����ģ̬��Ϣѭ����
/// ע�⣺��Ϣѭ������IsDialogMessageW��֧�ֵ�������
/// �����򴰿ڷ���DM_GETDEFID��DM_SETDEFID��
/// �ֱ���ΪWM_USER��WM_USER+1��
/// Ӧ����ʹ��������ֵ�������Զ�����Ϣ��
/// </summary>
/// <param name="pszWndClass">������</param>
/// <param name="pszCaption">����</param>
/// <param name="x">x����ΪCW_USEDEFAULT����ʾ��IDE��������</param>
/// <param name="y">y</param>
/// <param name="cx">���</param>
/// <param name="cy">�߶�</param>
/// <param name="dwStyle">������ʽ����Ϊ0��ʹ��WS_OVERLAPPEDWINDOW | WS_VISIBLE</param>
/// <param name="pCtx">������</param>
void EIDEDlgShow(PCWSTR pszWndClass, PCWSTR pszCaption, int x, int y, int cx, int cy, 
	DWORD dwStyle, EDLGCTX_BASE* pCtx, HWND hParent = NULL);

eStlInline void EIDEDlgEnd(EDLGCTX_BASE* pCtx)
{
	EzDlg::DlgEndModel(pCtx->hDlg, pCtx->hParent, pCtx->bParentEnabled);
}

eStlInline void EIDEDlgOnDestroy(EDLGCTX_BASE* pCtx)
{
	DeleteObject(pCtx->hFont);
	pCtx->hFont = NULL;
	EzDlg::DlgModelOnDestroy(pCtx->hDlg, pCtx->hParent, pCtx->bParentEnabled);
}

LRESULT CALLBACK SubclassProc_TabRepair(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

#define SCID_TABREPAIR 20230524'02u
/// <summary>
/// Tab�������ƶ��޸�
/// </summary>
/// <param name="hWnd">�ؼ����</param>
/// <returns>�ɹ�����TRUE</returns>
eStlInline BOOL TabRepairInstall(HWND hWnd)
{
	return SetWindowSubclass(hWnd, SubclassProc_TabRepair, SCID_TABREPAIR, 0);
}

/// <summary>
/// ȡ��Tab�������ƶ��޸�
/// </summary>
/// <param name="hWnd">�ؼ����</param>
/// <returns>�ɹ�����TRUE</returns>
eStlInline BOOL TabRepairUnInstall(HWND hWnd)
{
	return RemoveWindowSubclass(hWnd, SubclassProc_TabRepair, SCID_TABREPAIR);
}
#undef SCID_TABREPAIR
ESTL_EZDLG_NAMESPACE_END

/// <summary>
/// ����ͨ���¼���
/// ��������֪ͨ�ؼ�ͨ���¼��Ĳ���
/// </summary>
bool SendToParentsHwnd(DWORD m_dwWinFormID, DWORD m_dwUnitID, INT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// ��¡Unicode�ַ�����
/// �ڴ�����ͷ���new/delete[]����
/// </summary>
/// <param name="pszDst">Ŀ���ַ����������ΪNULL�����Ὣ���ͷ�</param>
/// <param name="pszSrc">Դ�ַ���</param>
/// <param name="cchSrc">�ַ���</param>
/// <returns>���Ƶ��ַ���</returns>
int DupStringForNewDeleteW(PWSTR& pszDst, PCWSTR pszSrc, int cchSrc = -1);

/// <summary>
/// ��¡ANSI�ַ�����
/// �ڴ�����ͷ���new/delete[]����
/// </summary>
/// <param name="pszDst">Ŀ���ַ����������ΪNULL�����Ὣ���ͷ�</param>
/// <param name="pszSrc">Դ�ַ���</param>
/// <param name="cchSrc">�ַ���</param>
/// <returns>���Ƶ��ַ���</returns>
int DupStringForNewDeleteA(PSTR& pszDst, PCSTR pszSrc, int cchSrc = -1);

SIZE_T DupStreamForNewDelete(BYTE*& pDst, PCBYTE pSrc, SIZE_T cbSrc);

/// <summary>
/// ȡĬ������
/// </summary>
/// <param name="hWnd">�ؼ����ھ��</param>
/// <returns>Ĭ������LOGFONTA�ṹ</returns>
eStlInline LOGFONTA GetEDefLOGFONT(HWND hWnd)
{
	LOGFONTA lf{};
	HDC hDC = GetDC(hWnd);
	lf.lfHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = 400;// Ȩ����Ĭ�ϣ���Ȼѡ�����ʱ����ܻ�ֱ��ת�Ƶ�ϸ����
	strcpy(lf.lfFaceName, "����");
	lf.lfCharSet = GB2312_CHARSET;
	ReleaseDC(hWnd, hDC);
	return lf;
}
 
/// <summary>
/// �����
/// </summary>
/// <param name="ppszInput">������������ָ�룬��ʹ��delete[]�ͷ�</param>
/// <param name="pszInitContent">�༭���ʼ����</param>
/// <param name="pszCaption">����</param>
/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
BOOL IntputBox(PWSTR* ppszInput, PCWSTR pszInitContent = NULL, PCWSTR pszCaption = L"�������ı���");

BOOL ImageListSelectDlg(HIMAGELIST hImageList, int idxInit, int* pidxSel, HWND hParent = NULL);

/// <summary>
/// �ؼ�ʵ���ࣺ���໯������
/// </summary>
/// <typeparam name="TC">�ؼ��࣬����ʵ��GetHWND()���������ؿؼ����ھ��</typeparam>
template<class TC>
class CSubclassMgr
{
private:
	using TCtrlInfo = std::unordered_map<HWND, TC*>;
	using TParentInfo = std::unordered_map<HWND, int>;

	TParentInfo& m_ParentInfo;
	TCtrlInfo& m_CtrlInfo;
	SUBCLASSPROC m_pfnParentSubclass;
	SUBCLASSPROC m_pfnCtrlSubclass;
	UINT_PTR m_uIDParent;
	UINT_PTR m_uIDCtrl;
public:
	CSubclassMgr() = delete;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="ParentInfo">���������ü�����ϣ��</param>
	/// <param name="CtrlInfo">�ؼ����ؼ����ϣ��</param>
	/// <param name="pfnParentSubclass">���������໯����</param>
	/// <param name="pfnCtrlSubclass">�ؼ����໯����</param>
	/// <param name="uIDParent">���������໯ID</param>
	/// <param name="uIDCtrl">�ؼ����໯ID</param>
	CSubclassMgr(TParentInfo& ParentInfo, TCtrlInfo& CtrlInfo,
		SUBCLASSPROC pfnParentSubclass, SUBCLASSPROC pfnCtrlSubclass, UINT_PTR uIDParent, UINT_PTR uIDCtrl) :
		m_ParentInfo(ParentInfo), m_CtrlInfo(CtrlInfo),
		m_pfnParentSubclass(pfnParentSubclass), m_pfnCtrlSubclass(pfnCtrlSubclass), m_uIDParent(uIDParent), m_uIDCtrl(uIDCtrl) {}

	~CSubclassMgr() = default;

	/// <summary>
	/// �ؼ��Ѵ���
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	eStlInline void OnCtrlCreate(TC* pClass)
	{
		OnCtrlCreate2(pClass);
		SetWindowSubclass(pClass->GetHWND(), m_pfnCtrlSubclass, m_uIDCtrl, (DWORD_PTR)pClass);
	}

	/// <summary>
	/// �ؼ��Ѵ�����
	/// �˷������Կؼ��������໯�������ڷǱ�׼�ؼ�
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	void OnCtrlCreate2(TC* pClass)
	{
		HWND hWnd = pClass->GetHWND();
		HWND hParent = GetParent(hWnd);
		m_CtrlInfo[hWnd] = pClass;

		if (!m_ParentInfo.count(hParent))
		{
			SetWindowSubclass(hParent, m_pfnParentSubclass, m_uIDParent, 0);
			m_ParentInfo[hParent] = 1;
		}
		else
			m_ParentInfo[hParent]++;
	}

	/// <summary>
	/// �ؼ��ĸ������ѱ��ı䡣
	/// ԭ�ȵĸ��������ò���ȥ������ΪWM_NOTIFY���Ƿ�������ʱ�ĸ����ڣ�WM_COMMAND���Ƿ���ֱ�Ӹ�����
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	void OnParentChanged(TC* pClass)
	{
		HWND hParent = GetParent(pClass->GetHWND());
		if (!m_ParentInfo.count(hParent))
		{
			SetWindowSubclass(hParent, m_pfnParentSubclass, m_uIDParent, 0);
			m_ParentInfo[hParent] = 1;
		}
		else
			m_ParentInfo[hParent]++;
	}

	/// <summary>
	/// �ؼ���������
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	void OnCtrlDestroy(TC* pClass)
	{
		HWND hWnd = pClass->GetHWND();
		HWND hParent = GetParent(hWnd);

		if (m_ParentInfo.count(hParent))
		{
			auto& v = m_ParentInfo[hParent];
			v--;
			if (v <= 0)
			{
				RemoveWindowSubclass(hParent, m_pfnParentSubclass, m_uIDParent);
				m_ParentInfo.erase(hParent);
			}
		}

		HWND hParent2 = pClass->GetHParent();
		if (hParent != hParent2)
			if (m_ParentInfo.count(hParent2))
			{
				auto& v = m_ParentInfo[hParent2];
				v--;
				if (v <= 0)
				{
					RemoveWindowSubclass(hParent2, m_pfnParentSubclass, m_uIDParent);
					m_ParentInfo.erase(hParent2);
				}
			}

		if (m_CtrlInfo.count(hWnd))
			m_CtrlInfo.erase(hWnd);
	}

	/// <summary>
	/// �����ڽ�������
	/// </summary>
	/// <param name="hParent"></param>
	void OnParentDestroy(HWND hParent)
	{
		RemoveWindowSubclass(hParent, m_pfnParentSubclass, m_uIDParent);
		if (m_ParentInfo.count(hParent))
			m_ParentInfo.erase(hParent);
	}
};

/// <summary>
/// �ؼ�ʵ���ࣺ���໯��������
/// ���಻ִ�и��������໯����
/// </summary>
/// <typeparam name="TC">�ؼ��࣬����ʵ��GetHWND()���������ؿؼ����ھ��</typeparam>
template<class TC>
class CSubclassMgrSimple
{
private:
	using TCtrlInfo = std::unordered_map<HWND, TC*>;

	TCtrlInfo& m_CtrlInfo;
	SUBCLASSPROC m_pfnCtrlSubclass;
	UINT_PTR m_uIDCtrl;
public:
	CSubclassMgrSimple() = delete;
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="CtrlInfo">�ؼ����ؼ����ϣ��</param>
	/// <param name="pfnCtrlSubclass">�ؼ����໯����</param>
	/// <param name="uIDCtrl">�ؼ����໯ID</param>
	CSubclassMgrSimple(TCtrlInfo& CtrlInfo, SUBCLASSPROC pfnCtrlSubclass, UINT_PTR uIDCtrl) :
		m_CtrlInfo(CtrlInfo), m_pfnCtrlSubclass(pfnCtrlSubclass), m_uIDCtrl(uIDCtrl) {}

	~CSubclassMgrSimple() = default;

	/// <summary>
	/// �ؼ��Ѵ���
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	eStlInline void OnCtrlCreate(TC* pClass)
	{
		OnCtrlCreate2(pClass);
		SetWindowSubclass(pClass->GetHWND(), m_pfnCtrlSubclass, m_uIDCtrl, (DWORD_PTR)pClass);
	}

	/// <summary>
	/// �ؼ��Ѵ�����
	/// �˷������Կؼ��������໯�������ڷǱ�׼�ؼ�
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	eStlInline void OnCtrlCreate2(TC* pClass)
	{
		HWND hWnd = pClass->GetHWND();
		m_CtrlInfo[hWnd] = pClass;
	}

	/// <summary>
	/// �ؼ���������
	/// </summary>
	/// <param name="pClass">�ؼ���</param>
	eStlInline void OnCtrlDestroy(TC* pClass)
	{
		HWND hWnd = pClass->GetHWND();
		if (m_CtrlInfo.count(hWnd))
			m_CtrlInfo.erase(hWnd);
	}
};

eStlInline DWORD ModifyWindowStyle(HWND hWnd, DWORD dwNew, DWORD dwMask = 0u, int idx = GWL_STYLE, BOOL bAutoMaskNew = FALSE)
{
	if (bAutoMaskNew)
		dwMask |= dwNew;
	DWORD dwStyle = GetWindowLongPtrW(hWnd, idx);
	dwStyle &= ~dwMask;
	dwStyle |= dwNew;
	SetWindowLongPtrW(hWnd, idx, dwStyle);
	return dwStyle;
}

eStlInline int ESTLPRIV_MultiSelectWndStyle___(HWND hWnd, int idx, int cStyle, ...)
{
	va_list Args;
	va_start(Args, cStyle);
	for (int i = 0; i < cStyle; ++i)
	{
		if (IsBitExist(GetWindowLongPtrW(hWnd, idx), va_arg_idx(Args, i, DWORD)))
		{
			va_end(Args);
			return i;
		}
	}
	va_end(Args);
	return -1;
}
#define MultiSelectWndStyle(hWnd, ...) elibstl::ESTLPRIV_MultiSelectWndStyle___(hWnd, GWL_STYLE, ESTLVAL(__VA_ARGS__))
#define MultiSelectWndExStyle(hWnd, ...) elibstl::ESTLPRIV_MultiSelectWndStyle___(hWnd, GWL_EXSTYLE, ESTLVAL(__VA_ARGS__))

template<class T>
eStlInline int ESTLPRIV_MultiSelectEqual___(T ii, int cItem, ...)
{
	va_list Args;
	va_start(Args, cItem);
	for (int i = 0; i < cItem; ++i)
	{
		if (ii == va_arg_idx(Args, i, T))
		{
			va_end(Args);
			return i;
		}
	}
	va_end(Args);
	return -1;
}
#define MultiSelectEqual(i, ...) elibstl::ESTLPRIV_MultiSelectEqual___(i, ESTLVAL(__VA_ARGS__))

void SetFrameType(HWND hWnd, int iFrame);
int GetFrameType(HWND hWnd);

/// <summary>
/// �ֽ�����λͼ�����
/// NAS_GET_HBITMAP�е����⣬��͸�����ض���ɫ�Ҳûȥϸ�о�����������֪��������ԭ��ؼ�������ôд��
/// </summary>
/// <param name="pData">�ֽ���</param>
/// <param name="cbPic">����</param>
/// <returns>�ɹ�����λͼ�����ʧ�ܷ���NULL</returns>
HBITMAP make_hbm_gp(BYTE* pData, SIZE_T cbPic);

#define ESTL_CLRPICKER_NAMESPACE_BEGIN namespace ClrPicker {
#define ESTL_CLRPICKER_NAMESPACE_END }

ESTL_CLRPICKER_NAMESPACE_BEGIN
HWND Create(int x, int y, int cx, int cy, DWORD dwStyle, DWORD dwExStyle, UINT uID, HWND hParent, UINT uNotifyMsg);
eStlInline void SelColor(HWND hWnd, int idx)
{
	SendMessageW(hWnd, CB_SETCURSEL, idx, 0);
}
COLORREF GetColor(HWND hWnd);
BOOL SetColor(HWND hWnd, COLORREF cr);
ESTL_CLRPICKER_NAMESPACE_END

class CCtrlWnd
{
protected:
	HWND m_hWnd = NULL;
	HWND m_hParent = NULL;
	BOOL m_bParentChanged = FALSE;

public:
	CCtrlWnd() = default;
	virtual ~CCtrlWnd() {}

	/// <summary>
	/// �ػ�
	/// </summary>
	eStlInline void Redraw()
	{
		InvalidateRect(m_hWnd, NULL, TRUE);
		UpdateWindow(m_hWnd);
	}

	/// <summary>
	/// ǿ�����º���ǿͻ���
	/// </summary>
	eStlInline void FrameChanged()
	{
		SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	/// <summary>
	/// ȡ���ھ��
	/// </summary>
	/// <returns>���ھ��</returns>
	eStlInline HWND GetHWND() const
	{
		return m_hWnd;
	}

	/// <summary>
	/// ȡ�״δ���������
	/// </summary>
	eStlInline HWND GetHParent() const
	{
		return m_hParent;
	}

	eStlInline void SetRedraw(BOOL bRedraw)
	{
		SendMessageW(m_hWnd, WM_SETREDRAW, bRedraw, 0);
	}
};

// �ؼ���������
/*
* �汾1���ݲ��֣�
* ECTRLINFO�ṹ
* ͼƬ
* �ı�
*/
#define DATA_VER_BASE_1	1
struct ECTRLINFO
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	LOGFONTA Font;			// ����
	int cchText;			// �ı����ȣ������ڱ�����Ϣ
	SIZE_T cbPic;			// ͼƬ�ֽ�������
	int iFrame;				// �߿�
};
// �ؼ�����
// ����ֱ��ʵ��������
class CCtrlBase :public CCtrlWnd
{
protected:
	// ��ϵͳ���
	DWORD m_dwWinFormID = 0;
	DWORD m_dwUnitID = 0;
	BOOL m_bInDesignMode = FALSE;
	UINT m_uID = 0;

	BOOL m_bGpDecodePicInDesignMode = FALSE;// ���ģʽ���Ƿ���GDI+����ͼƬ

	HBITMAP m_hbmPic = NULL;// λͼ���
	HFONT m_hFont = NULL;// ������
	ECTRLINFO m_Info0{};// ��Ϣ

	PSTR m_pszTextA = NULL;// ����A
	PWSTR m_pszTextW = NULL;// ����W
	void* m_pPicData = NULL;// ͼƬ����

	SIZE_T InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID, UINT uID, HWND hParent);

	void InitBase0(PCVOID pAllData);
public:
	CCtrlBase() = default;
	~CCtrlBase()
	{
		DeleteObject(m_hFont);
		DeleteObject(m_hbmPic);
		delete[] m_pszTextA;
		delete[] m_pszTextW;
		delete[] m_pPicData;
	}

	/// <summary>
	/// �ñ���A
	/// </summary>
	/// <param name="pszText">�ı�ָ��</param>
	/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
	eStlInline BOOL SetTextA(PCSTR pszText)
	{
		if (m_bInDesignMode)
		{
			DupStringForNewDeleteA(m_pszTextA, pszText);
			delete[] m_pszTextW;
			if (pszText)
				m_pszTextW = A2W(pszText);
			else
				m_pszTextW = NULL;
		}
		return SetWindowTextA(m_hWnd, pszText);
	}

	/// <summary>
	/// �ñ���W
	/// </summary>
	/// <param name="pszText">�ı�ָ��</param>
	/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
	eStlInline BOOL SetTextW(PCWSTR pszText)
	{
		if (m_bInDesignMode)
		{
			DupStringForNewDeleteW(m_pszTextW, pszText);
			delete[] m_pszTextA;
			if (pszText)
				m_pszTextA = W2A(pszText);
			else
				m_pszTextA = NULL;
		}
		return SetWindowTextW(m_hWnd, pszText);
	}

	/// <summary>
	/// �ñ���W��
	/// �˺����������ַ���
	/// </summary>
	/// <param name="pszText">�ı�ָ�룬��ָ���ں������ý������ؼ���������</param>
	/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
	eStlInline BOOL SetTextNoCopyW(PWSTR pszText)
	{
		if (m_bInDesignMode)
		{
			delete[] m_pszTextW;
			m_pszTextW = pszText;
			delete[] m_pszTextA;
			if (pszText)
				m_pszTextA = W2A(pszText);
			else
				m_pszTextA = NULL;
		}
		return SetWindowTextW(m_hWnd, pszText);
	}

	/// <summary>
	/// ȡ�ı�W��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <returns>�ı�ָ��</returns>
	PWSTR GetTextW(SIZE_T* pcb = NULL);

	/// <summary>
	/// ȡ�ı�A��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <returns>�ı�ָ��</returns>
	eStlInline PCSTR GetTextA()
	{
		if (!m_bInDesignMode)
		{
			int cch = GetWindowTextLengthA(m_hWnd);
			if (cch)
			{
				delete[] m_pszTextA;
				m_pszTextA = new CHAR[cch + 1];
				GetWindowTextA(m_hWnd, m_pszTextA, cch + 1);
			}
		}

		return m_pszTextA;
	}

	/// <summary>
	/// ��ͼƬ
	/// </summary>
	/// <param name="pPic">ͼƬ����</param>
	/// <param name="cbSize">ͼƬ���ݳ���</param>
	void SetPic(void* pPic, int cbSize);

	/// <summary>
	/// ȡͼƬ��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <param name="pcb">ָ�����ͼƬ���ݳ��ȱ�����ָ��</param>
	/// <returns>ͼƬ����ָ��</returns>
	BYTE* GetPic(int* pcb);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="plf">LOGFONTAָ��</param>
	eStlInline void SetFont(LOGFONTA* plf)
	{
		if (m_hFont)
			DeleteObject(m_hFont);

		m_Info0.Font = *plf;
		m_hFont = CreateFontIndirectA(plf);
		SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)m_hFont, TRUE);
	}

	/// <summary>
	/// ȡ����
	/// </summary>
	/// <returns>LOGFONTAָ�룬Ϊӭ�������Իص��������Ͷ���ΪBYTE*����Ҫ�ͷš�</returns>
	eStlInline BYTE* GetFont()
	{
		if (!m_bInDesignMode)
		{
			HFONT hFont = (HFONT)SendMessageW(m_hWnd, WM_GETFONT, 0, 0);
			GetObjectA(hFont, sizeof(m_Info0.Font), &m_Info0.Font);
		}
		return (BYTE*)&m_Info0.Font;
	}

	/// <summary>
	/// �ñ߿�
	/// </summary>
	/// <param name="iFrame">�߿�</param>
	eStlInline void SetFrame(int iFrame)
	{
		m_Info0.iFrame = iFrame;
		SetFrameType(m_hWnd, iFrame);
		FrameChanged();
	}

	/// <summary>
	/// ȡ�߿�
	/// </summary>
	/// <returns>�߿�</returns>
	eStlInline int GetFrame() const
	{
		if (m_bInDesignMode)
			return m_Info0.iFrame;
		else
			return GetFrameType(m_hWnd);
	}

	/// <summary>
	/// ƽ�滯�������ݡ�
	/// ��չ����Ӧ����󸽼�
	/// </summary>
	/// <returns></returns>
	HGLOBAL FlattenInfoBase0(SIZE_T cbExtra = 0u, SIZE_T* pcbBaseData = NULL);

	/// <summary>
	/// ƽ�滯���ݡ�
	/// �����������Է����������ԡ�
	/// �ڴ沼�ֲμ��ļ��ײ����ݰ汾���崦
	/// </summary>
	/// <returns></returns>
	virtual HGLOBAL FlattenInfo() { assert(FALSE); return NULL; }
};

// �򵥿ؼ���������
/*
* �汾1���ݲ��֣�
* ECTRLINFOSMP�ṹ
*/
#define DATA_VER_BASE_SIMPLE_1	1
struct ECTRLINFOSMP
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	int iFrame;				// �߿�
};
// �򵥿ؼ�����
// ����ֱ��ʵ��������
class CCtrlBaseSimple :public CCtrlWnd
{
protected:
	// ��ϵͳ���
	DWORD m_dwWinFormID = 0;
	DWORD m_dwUnitID = 0;
	BOOL m_bInDesignMode = FALSE;
	UINT m_uID = 0;

	ECTRLINFOSMP m_Info0{};// ��Ϣ

	CCtrlBaseSimple() = default;
	~CCtrlBaseSimple() = default;

	SIZE_T InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID, UINT uID, HWND hParent);

	/// <summary>
	/// ƽ�滯�������ݡ�
	/// ��չ����Ӧ����󸽼�
	/// </summary>
	/// <returns></returns>
	HGLOBAL FlattenInfoBase0(SIZE_T cbExtra = 0u, SIZE_T* pcbBaseData = NULL);

	/// <summary>
	/// �ñ߿�
	/// </summary>
	/// <param name="iFrame">�߿�</param>
	eStlInline void SetFrame(int iFrame)
	{
		m_Info0.iFrame = iFrame;
		SetFrameType(m_hWnd, iFrame);
		FrameChanged();
	}

	/// <summary>
	/// ȡ�߿�
	/// </summary>
	/// <returns>�߿�</returns>
	eStlInline int GetFrame() const
	{
		if (m_bInDesignMode)
			return m_Info0.iFrame;
		else
			return GetFrameType(m_hWnd);
	}

	/// <summary>
	/// ƽ�滯���ݡ�
	/// �����������Է����������ԡ�
	/// �ڴ沼�ֲμ��ļ��ײ����ݰ汾���崦
	/// </summary>
	/// <returns></returns>
	virtual HGLOBAL FlattenInfo() { assert(FALSE); return NULL; }
};
ESTL_NAMESPACE_END

// �����ؼ��ӿڲ�������
#define STD_ECTRL_CREATE_ARGS \
	LPVOID pAllData, int cbData, DWORD dwStyle, int x, int y, int cx, int cy, \
	HWND hParent, UINT nID, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID
// �����ؼ��ӿڲ���
#define STD_ECTRL_CREATE_REAL_ARGS \
	pAllData, cbData, dwStyle, x, y, cx, cy, \
	hParent, nID, bInDesignMode, dwWinFormID, dwUnitID
// �����Դ����ؼ��ӿڲ������壬����������в�ͬ
#define STD_EINTF_CREATE_ARGS \
	LPBYTE pAllData, INT cbData, DWORD dwStyle, HWND hParent, UINT nID, \
	HMENU hMenu, INT x, INT y, INT cx, INT cy, DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL bInDesignMode
// �������໯�����������Ա
#define SUBCLASS_MGR_DECL(Class) \
	public: \
		static ::std::unordered_map<HWND, Class*> m_CtrlSCInfo; \
		static ::std::unordered_map<HWND, int> m_ParentSCInfo; \
	private: \
		static ::elibstl::CSubclassMgr<Class> m_SM;
// ��ʼ�����໯�����������Ա
#define SUBCLASS_MGR_INIT(Class, uIDParent, uIDCtrl) \
	::std::unordered_map<HWND, Class*> Class::m_CtrlSCInfo{}; \
	::std::unordered_map<HWND, int> Class::m_ParentSCInfo{}; \
	::elibstl::CSubclassMgr<Class> Class::m_SM{ m_ParentSCInfo,m_CtrlSCInfo,ParentSubclassProc,CtrlSubclassProc,uIDParent,uIDCtrl };
// ���������໯�����������Ա
#define SUBCLASS_SMP_MGR_DECL(Class) \
	public: \
		static ::std::unordered_map<HWND, Class*> m_CtrlSCInfo; \
	private: \
		static ::elibstl::CSubclassMgrSimple<Class> m_SM;
// ��ʼ�������໯�����������Ա
#define SUBCLASS_SMP_MGR_INIT(Class, uIDCtrl) \
	::std::unordered_map<HWND, Class*> Class::m_CtrlSCInfo{}; \
	::elibstl::CSubclassMgrSimple<Class> Class::m_SM{ m_CtrlSCInfo,CtrlSubclassProc,uIDCtrl };
// ��鸸�����Ƿ�ı䣬����WM_SHOWWINDOW�£��ؼ���������ʱ�����������еĸ����ھ��ʵ�����Ƕ������ھ����WM_SHOWWINDOWǰ�ᱻ��Ϊ��������
#define CHECK_PARENT_CHANGE \
	if (!p->m_bParentChanged) \
		if (GetParent(hWnd) != p->m_hParent) \
		{ \
			m_SM.OnParentChanged(p); \
			p->m_bParentChanged = TRUE; \
		}

#define QuickGetWindowTextW(VarName, hWnd) \
			PWSTR VarName; \
			int ESTLPRIV_cch_##VarName##___ = GetWindowTextLengthW((hWnd)); \
			if (ESTLPRIV_cch_##VarName##___) \
			{ \
				VarName = (PWSTR)_malloca((ESTLPRIV_cch_##VarName##___ + 1)*sizeof(WCHAR)); \
				assert(VarName);/*��������*/ \
				GetWindowTextW((hWnd), VarName, ESTLPRIV_cch_##VarName##___ + 1); \
			} \
			else \
				VarName = NULL;

#define QuickGetWindowTextWFree(VarName) _freea(VarName)

#define QuickDupStringW(VarName, pszSrc, cchSrc) \
			PWSTR VarName; \
			if (!pszSrc || !cchSrc) \
			{ \
				VarName = NULL; \
			} \
			else \
			{ \
				int ESTLPRIV_QDS_cch_##VarName##___; \
				if (cchSrc < 0) \
					ESTLPRIV_QDS_cch_##VarName##___ = wcslen(pszSrc); \
				else \
					ESTLPRIV_QDS_cch_##VarName##___ = cchSrc; \
				VarName = (PWSTR)_malloca((cchSrc + 1) * sizeof(WCHAR)); \
				assert(VarName);/*��������*/ \
				wcsncpy(VarName, pszSrc, cchSrc); \
				*(VarName + cchSrc) = L'\0'; \
			} \

#define QuickDupStringFree(VarName) _freea(VarName)