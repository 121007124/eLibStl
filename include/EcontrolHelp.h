//���������ͨ�ú��������ͷ�ļ�
#include "ElibHelp.h"
#include "GdiplusFlatDef.h"
#include <CommCtrl.h>
#include <unordered_map>
#include <Shlwapi.h>
#include <windowsx.h>
/*�׶���ı�ǩ�����¼�*/
#define WM_EFEEDBACK  32885


ESTL_NAMESPACE_BEGIN

std::vector<unsigned char> GetDataFromHBIT(HBITMAP hBitmap);

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

/// <summary>
/// ����ͨ���¼���
/// ��������֪ͨ�ؼ�ͨ���¼��Ĳ���
/// </summary>
eStlInline bool SendToParentsHwnd(DWORD m_dwWinFormID, DWORD m_dwUnitID, INT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS || uMsg == WM_MOUSELAST || uMsg >= WM_MOUSEMOVE
		&& uMsg <= WM_RBUTTONUP || uMsg >= WM_KEYDOWN && uMsg <= WM_CHAR)
	{
		//�⼸���¼�ȫ��ת���������
		EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 0);
		INT control_type = 0;
		if (uMsg != WM_CHAR && uMsg != WM_SETFOCUS && uMsg != WM_KILLFOCUS)
		{
			if ((GetKeyState(VK_CONTROL) & 16384) != 0)
				control_type = 1;
			if ((GetKeyState(VK_SHIFT) & 16384) != 0)
				control_type = control_type | 2;
			if ((GetKeyState(VK_MENU) & 16384) != 0)
				control_type = control_type | 4;
		}

		if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_RBUTTONUP)
		{
			if (uMsg == WM_MOUSEMOVE)
				event.m_nEventIndex = -6;
			else
				event.m_nEventIndex = 512 - uMsg;
			event.m_nArgCount = 3;
			event.m_arg[0].m_inf.m_int = lParam & 65535;
			event.m_arg[1].m_inf.m_int = lParam >> 16;
			event.m_arg[2].m_inf.m_int = control_type;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			return true;
		}

		switch (uMsg)
		{
		case WM_SETFOCUS: {
			event.m_nEventIndex = -uMsg;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}
		case WM_KILLFOCUS: {
			event.m_nEventIndex = -uMsg;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}
		case WM_KEYDOWN: {
			event.m_nEventIndex = (247 - uMsg);
			event.m_nArgCount = 2;
			event.m_arg[0].m_inf.m_int = wParam;
			event.m_arg[1].m_inf.m_int = control_type;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}
		case WM_KEYUP: {
			event.m_nEventIndex = (247 - uMsg);
			event.m_nArgCount = 2;
			event.m_arg[0].m_inf.m_int = wParam;
			event.m_arg[1].m_inf.m_int = control_type;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}
		case WM_CHAR: {
			event.m_nEventIndex = -11;
			event.m_nArgCount = 1;
			event.m_arg[0].m_inf.m_int = wParam;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}case WM_MOUSELAST: {
			event.m_nEventIndex = 12;
			event.m_nArgCount = 2;
			event.m_arg[0].m_inf.m_int = (wParam >> 16) > 0 ? 1 : -1;
			event.m_arg[1].m_inf.m_int = control_type;
			elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
			break;
		}
		default:
			break;
		}
	}

	return true;
}

/// <summary>
/// ��¡Unicode�ַ�����
/// �ڴ�����ͷ���new/delete[]����
/// </summary>
/// <param name="pszDst">Ŀ���ַ����������ΪNULL�����Ὣ���ͷ�</param>
/// <param name="pszSrc">Դ�ַ���</param>
/// <param name="cchSrc">�ַ���</param>
/// <returns>���Ƶ��ַ���</returns>
int DupStringForNewDeleteW(PWSTR& pszDst, PCWSTR pszSrc, int cchSrc = 0);

/// <summary>
/// ��¡ANSI�ַ�����
/// �ڴ�����ͷ���new/delete[]����
/// </summary>
/// <param name="pszDst">Ŀ���ַ����������ΪNULL�����Ὣ���ͷ�</param>
/// <param name="pszSrc">Դ�ַ���</param>
/// <param name="cchSrc">�ַ���</param>
/// <returns>���Ƶ��ַ���</returns>
int DupStringForNewDeleteA(PSTR& pszDst, PCSTR pszSrc, int cchSrc = 0);

/// <summary>
/// ȡĬ������
/// </summary>
/// <param name="hWnd">�ؼ����ھ��</param>
/// <returns>Ĭ������LOGFONTA�ṹ</returns>
eStlInline LOGFONTA GetEDefLOGFONT(HWND hWnd)
{
	LOGFONTA lf{};
	HDC hDC = GetDC(hWnd);
	lf.lfHeight = -MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);
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

eStlInline BOOL IsBitExist(DWORD dw1, DWORD dw2)
{
	return ((dw1 & dw2) == dw2);
}

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
	int cbPic;				// ͼƬ�ֽ�������
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

	HBITMAP m_hbmPic = NULL;// λͼ���
	HFONT m_hFont = NULL;// ������
	ECTRLINFO m_Info0{};// ��Ϣ

	PSTR m_pszTextA = NULL;// ����A
	PWSTR m_pszTextW = NULL;// ����W
	void* m_pPicData = NULL;// ͼƬ����

	SIZE_T InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID);

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
			elibstl::DupStringForNewDeleteA(m_pszTextA, pszText);
			delete[] m_pszTextW;
			if (pszText)
				m_pszTextW = elibstl::A2W(pszText);
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
			elibstl::DupStringForNewDeleteW(m_pszTextW, pszText);
			delete[] m_pszTextA;
			if (pszText)
				m_pszTextA = elibstl::W2A(pszText);
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
				m_pszTextA = elibstl::W2A(pszText);
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
	eStlInline PWSTR GetTextW(SIZE_T* pcb = NULL)
	{
		if (!m_bInDesignMode)
		{
			int cch = GetWindowTextLengthW(m_hWnd);
			if (cch)
			{
				delete[] m_pszTextW;
				m_pszTextW = new WCHAR[cch + 1];
				GetWindowTextW(m_hWnd, m_pszTextW, cch + 1);
				if (pcb)
					*pcb = (cch + 1) * sizeof(WCHAR);
			}
			else
				if (pcb)
					*pcb = 0u;

		}
		else
			if (pcb)
			{
				if (!m_pszTextW)
					*pcb = 0u;
				else
					*pcb = (wcslen(m_pszTextW) + 1) * sizeof(WCHAR);
			}



		return m_pszTextW;
	}

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
	eStlInline BYTE* GetPic(int* pcb)
	{
		if (!m_bInDesignMode)
		{
			auto&& x = elibstl::GetDataFromHBIT(m_hbmPic);
			delete[] m_pPicData;
			m_Info0.cbPic = x.size();
			m_pPicData = new BYTE[m_Info0.cbPic];
			memcpy(m_pPicData, x.data(), m_Info0.cbPic);
		}

		*pcb = m_Info0.cbPic;
		return (BYTE*)m_pPicData;
	}

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
		elibstl::SetFrameType(m_hWnd, iFrame);
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
			return elibstl::GetFrameType(m_hWnd);
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

	ECTRLINFOSMP m_Info0{};// ��Ϣ

	CCtrlBaseSimple() = default;
	~CCtrlBaseSimple() = default;

	SIZE_T InitBase0(LPVOID pAllData, int cbData, BOOL bInDesignMode, DWORD dwWinFormID, DWORD dwUnitID);

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
		elibstl::SetFrameType(m_hWnd, iFrame);
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
			return elibstl::GetFrameType(m_hWnd);
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
		static std::unordered_map<HWND, Class*> m_CtrlSCInfo; \
		static std::unordered_map<HWND, int> m_ParentSCInfo; \
	private: \
		static elibstl::CSubclassMgr<Class> m_SM;
// ��ʼ�����໯�����������Ա
#define SUBCLASS_MGR_INIT(Class, uIDParent, uIDCtrl) \
	std::unordered_map<HWND, Class*> Class::m_CtrlSCInfo{}; \
	std::unordered_map<HWND, int> Class::m_ParentSCInfo{}; \
	elibstl::CSubclassMgr<Class> Class::m_SM{ m_ParentSCInfo,m_CtrlSCInfo,ParentSubclassProc,CtrlSubclassProc,uIDParent,uIDCtrl };
// ���������໯�����������Ա
#define SUBCLASS_SMP_MGR_DECL(Class) \
	public: \
		static std::unordered_map<HWND, Class*> m_CtrlSCInfo; \
	private: \
		static elibstl::CSubclassMgrSimple<Class> m_SM;
// ��ʼ�������໯�����������Ա
#define SUBCLASS_SMP_MGR_INIT(Class, uIDCtrl) \
	std::unordered_map<HWND, Class*> Class::m_CtrlSCInfo{}; \
	elibstl::CSubclassMgrSimple<Class> Class::m_SM{ m_CtrlSCInfo,CtrlSubclassProc,uIDCtrl };
// ��鸸�����Ƿ�ı䣬����WM_SHOWWINDOW�£��ؼ���������ʱ�����������еĸ����ھ��ʵ�����Ƕ������ھ����WM_SHOWWINDOWǰ�ᱻ��Ϊ��������
#define CHECK_PARENT_CHANGE \
	if (!p->m_bParentChanged) \
		if (GetParent(hWnd) != p->m_hParent) \
		{ \
			m_SM.OnParentChanged(p); \
			p->m_bParentChanged = TRUE; \
		}