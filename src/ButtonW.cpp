#include"EcontrolHelp.h"

#include <unordered_map>

#pragma warning(disable:4996)

#define SCID_PUSHBTN		20230425'01u
#define SCID_PUSHBTNPARENT	20230425'02u
#define SCID_CHECKBTN		20230425'03u
#define SCID_CHECKBTNPARENT	20230425'04u
#define SCID_CMDLINK		20230425'05u
#define SCID_CMDLINKPARENT	20230425'06u


// ��������
/*
* �汾1���ݲ��֣�
* EBUTTONDATA�ṹ
* ͼƬ
* �ı�
*/
#define DATA_VER_BTN_1	1
struct EBUTTONDATA
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	int algH;				// �������
	int algV;				// �������
	BOOL bShowTextAndImage;	// �Ƿ�ͬʱ��ʾͼƬ���ı�
	LOGFONTA Font;			// ����
	int cchText;			// �ı����ȣ������ڱ�����Ϣ
	PSTR pszTextA;			// ����A
	PWSTR pszTextW;			// ����W
	int cbPic;				// ͼƬ�ֽ�������
	void* pPicData;			// ͼƬ����
};

// ��ͨ��ť
/*
* �汾1���ݲ��֣�
* EBUTTONDATA_PUSHBTN�ṹ
*/
#define DATA_VER_BTN_PUSHBTN_1	1
struct EBUTTONDATA_PUSHBTN
{
	int iVer;				// �汾
	DWORD dwReserved;		// ����

	int iType;				// ����
	int iDef;				// �Ƿ�Ĭ��
};

// ѡ��򣨵�ѡ����ѡ����̬��ѡ��
/*
* �汾1���ݲ��֣�
* EBUTTONDATA_CHECKBTN�ṹ
*/
#define DATA_VER_BTN_CHECKBTN_1	1
struct EBUTTONDATA_CHECKBTN
{
	int iVer;				// �汾
	DWORD dwReserved;		// ����

	int iType;				// ����
	int iCheckState;		// ѡ��״̬��0 δѡ��  1 ѡ��  2 ��ѡ�У���������̬��ѡ��
};

// ��������
/*
* �汾1���ݲ��֣�
* EBUTTONDATA_CMDLINK�ṹ
* ע���ı�
*/
#define DATA_VER_BTN_CMDLINK_1	1
struct EBUTTONDATA_CMDLINK
{
	int iVer;				// �汾
	DWORD dwReserved;		// ����

	int cchNote;			// ע���ı�����
	PWSTR pszNote;			// ע���ı�
};


/// <summary>
/// ��ť���ࡣ
/// ����ֱ��ʵ��������
/// </summary>
class CButton
{
protected:
	// ��ϵͳ���
	DWORD m_dwWinFormID = 0;
	DWORD m_dwUnitID = 0;
	BOOL m_blInDesignMode = FALSE;
	// ���ھ��
	HWND m_hWnd = NULL;
	// λͼ���
	HBITMAP m_hBitmap = NULL;
	// ������
	HFONT m_hFont = NULL;
	// ��Ϣ
	EBUTTONDATA m_Info{};

	SIZE_T InitBase(STD_ECTRL_CREATE_ARGS)
	{
		m_blInDesignMode = blInDesignMode;
		m_dwWinFormID = dwWinFormID;
		m_dwUnitID = dwUnitID;

		if (pAllData)
		{
			memcpy(&m_Info, pAllData, sizeof(EBUTTONDATA));
			BYTE* p = (BYTE*)pAllData + sizeof(EBUTTONDATA) + m_Info.cbPic;

			m_Info.pszTextW = NULL;
			if (m_Info.cchText)
				elibstl::DupStringForNewDeleteW(m_Info.pszTextW, (PCWSTR)p, m_Info.cchText);
			else
				elibstl::DupStringForNewDeleteW(m_Info.pszTextW, L"��ťW");
		}
		else
		{

			m_Info.pszTextW = NULL;
			elibstl::DupStringForNewDeleteW(m_Info.pszTextW, L"��ťW");
		}
		m_Info.pPicData = NULL;
		m_Info.pszTextA = elibstl::W2A(m_Info.pszTextW);

		m_Info.iVer = DATA_VER_BTN_1;

		return sizeof(EBUTTONDATA) + m_Info.cbPic + m_Info.cchText * sizeof(WCHAR);
	}

	void InitBase(PCVOID pAllData)
	{
		SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);

		if (pAllData)
		{
			BYTE* p = (BYTE*)pAllData + sizeof(EBUTTONDATA);
			SetPic(p, m_Info.cbPic);

			SetAlign(TRUE, m_Info.algH);
			SetAlign(FALSE, m_Info.algV);
			SetTextImageShowing(m_Info.bShowTextAndImage);
		}
		else
		{
			m_Info.Font = elibstl::GetEDefLOGFONT(m_hWnd);
			m_Info.algH = 1;
			m_Info.algV = 1;
		}

		SetFont(&m_Info.Font);

		SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
	}

	eStlInline void Redraw()
	{
		InvalidateRect(m_hWnd, NULL, TRUE);
		UpdateWindow(m_hWnd);
	}
public:
	CButton()
	{

	}

	virtual ~CButton()
	{
		DeleteObject(m_hFont);
		DeleteObject(m_hBitmap);
		delete[] m_Info.pszTextA;
		delete[] m_Info.pszTextW;
		delete[] m_Info.pPicData;
	}

	/// <summary>
	/// �ñ���A
	/// </summary>
	/// <param name="pszText">�ı�ָ��</param>
	/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
	eStlInline BOOL SetTextA(PCSTR pszText)
	{
		if (m_blInDesignMode)
		{
			elibstl::DupStringForNewDeleteA(m_Info.pszTextA, pszText);
			delete[] m_Info.pszTextW;
			m_Info.pszTextW = elibstl::A2W(pszText);
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
		if (m_blInDesignMode)
		{
			elibstl::DupStringForNewDeleteW(m_Info.pszTextW, pszText);
			delete[] m_Info.pszTextA;
			m_Info.pszTextA = elibstl::W2A(pszText);
		}
		return SetWindowTextW(m_hWnd, pszText);
	}

	/// <summary>
	/// ��ͼƬ
	/// </summary>
	/// <param name="pPic">ͼƬ����</param>
	/// <param name="cbSize">ͼƬ���ݳ���</param>
	void SetPic(void* pPic, int cbSize)
	{
		m_Info.cbPic = cbSize;
		delete[] m_Info.pPicData;
		if (m_hBitmap)
			DeleteObject(m_hBitmap);
		if (cbSize)
		{
			if (m_Info.bShowTextAndImage)
				SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) & (~BS_BITMAP));
			else
				SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) | BS_BITMAP);
			m_Info.pPicData = new BYTE[cbSize];
			memcpy(m_Info.pPicData, pPic, cbSize);
			m_hBitmap = elibstl::make_hbit((BYTE*)pPic, cbSize);
			SendMessageW(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_hBitmap);
		}
		else
		{
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) & (~BS_BITMAP));
			m_Info.pPicData = NULL;
			m_hBitmap = NULL;
			SendMessageW(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, NULL);
		}
	}

	/// <summary>
	/// ȡͼƬ��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <param name="pcb">ָ�����ͼƬ���ݳ��ȱ�����ָ��</param>
	/// <returns>ͼƬ����ָ��</returns>
	eStlInline BYTE* GetPic(int* pcb) const
	{
		*pcb = m_Info.cbPic;
		return (BYTE*)m_Info.pPicData;
	}

	/// <summary>
	/// ȡ�ı�W��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <returns>�ı�ָ��</returns>
	PWSTR GetTextW()
	{
		if (m_blInDesignMode)
		{
			int cch = GetWindowTextLengthW(m_hWnd);
			if (cch)
			{
				delete[] m_Info.pszTextW;
				m_Info.pszTextW = new WCHAR[cch + 1];
				GetWindowTextW(m_hWnd, m_Info.pszTextW, cch + 1);
			}
		}

		return m_Info.pszTextW;
	}

	/// <summary>
	/// ȡ�ı�A��
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <returns>�ı�ָ��</returns>
	PCSTR GetTextA()
	{
		if (m_blInDesignMode)
		{
			int cch = GetWindowTextLengthA(m_hWnd);
			if (cch)
			{
				delete[] m_Info.pszTextA;
				m_Info.pszTextA = new CHAR[cch + 1];
				GetWindowTextA(m_hWnd, m_Info.pszTextA, cch + 1);
			}
		}

		return m_Info.pszTextA;
	}

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="plf">LOGFONTAָ��</param>
	eStlInline void SetFont(LOGFONTA* plf)
	{
		if (m_hFont)
			DeleteObject(m_hFont);

		m_Info.Font = *plf;
		m_hFont = CreateFontIndirectA(plf);
		SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)m_hFont, TRUE);
	}

	/// <summary>
	/// ȡ����
	/// </summary>
	/// <returns>LOGFONTAָ�룬Ϊӭ�������Իص��������Ͷ���ΪBYTE*����Ҫ�ͷš�</returns>
	eStlInline BYTE* GetFont() const
	{
		return (BYTE*)&m_Info.Font;
	}

	/// <summary>
	/// ��ͼƬ�ı�ͬʱ��ʾ
	/// </summary>
	/// <param name="bShowTextAndImage">�Ƿ�ͬʱ��ʾ</param>
	void SetTextImageShowing(BOOL bShowTextAndImage)
	{
		m_Info.bShowTextAndImage = bShowTextAndImage;
		DWORD dwStyle = GetWindowLongPtrW(m_hWnd, GWL_STYLE);
		if (bShowTextAndImage)
			dwStyle &= (~(BS_BITMAP));
		else
			dwStyle |= BS_BITMAP;
		SetWindowLongPtrW(m_hWnd, GWL_STYLE, dwStyle);
	}

	/// <summary>
	/// ȡͼƬ�ı�ͬʱ��ʾ
	/// </summary>
	/// <returns>�Ƿ�ͬʱ��ʾ</returns>
	eStlInline BOOL GetTextImageShowing() const
	{
		return m_Info.bShowTextAndImage;
	}

	/// <summary>
	/// �ö���
	/// </summary>
	/// <param name="bHAlign">�Ƿ�ˮƽ����</param>
	/// <param name="iAlign">���룬�μ����Զ���</param>
	void SetAlign(BOOL bHAlign, int iAlign)
	{
		DWORD dwStyle = GetWindowLongPtrW(m_hWnd, GWL_STYLE);
		if (bHAlign)
		{
			dwStyle &= (~(BS_LEFT | BS_CENTER | BS_RIGHT));
			m_Info.algH = iAlign;
			switch (iAlign)
			{
			case 0: dwStyle |= BS_LEFT; break;
			case 1: dwStyle |= BS_CENTER; break;
			case 2: dwStyle |= BS_RIGHT; break;
			}
		}
		else
		{
			dwStyle &= (~(BS_TOP | BS_VCENTER | BS_BOTTOM));
			m_Info.algV = iAlign;
			switch (iAlign)
			{
			case 0: dwStyle |= BS_TOP; break;
			case 1: dwStyle |= BS_VCENTER; break;
			case 2: dwStyle |= BS_BOTTOM; break;
			}
		}
		SetWindowLongPtrW(m_hWnd, GWL_STYLE, dwStyle);
		Redraw();
	}

	/// <summary>
	/// ȡ����
	/// </summary>
	/// <param name="bHAlign">�Ƿ�ˮƽ����</param>
	/// <returns>���룬�μ����Զ���</returns>
	eStlInline int GetAlign(BOOL bHAlign) const
	{
		if (bHAlign)
			return m_Info.algH;
		else
			return m_Info.algV;
	}

	/// <summary>
	/// ƽ�滯�������ݡ�
	/// ��չ����Ӧ����󸽼�
	/// </summary>
	/// <returns></returns>
	eStlInline HGLOBAL FlattenInfoBase(SIZE_T cbExtra = 0u, SIZE_T* pcbBaseData = NULL)
	{
		BYTE* p;
		auto pszText = GetTextW();
		m_Info.cchText = wcslen(pszText);
		int cbText = m_Info.cchText * sizeof(WCHAR);
		SIZE_T cbMem = sizeof(EBUTTONDATA) + m_Info.cbPic + cbText;
		if (pcbBaseData)
			*pcbBaseData = cbMem;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cbMem + cbExtra);
		if (!hGlobal)
			goto FailAlloc;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto FailLock;
		// �ṹ
		memcpy(p, &m_Info, sizeof(EBUTTONDATA));
		// ͼƬ
		p += sizeof(EBUTTONDATA);
		memcpy(p, m_Info.pPicData, m_Info.cbPic);
		//// �ı�
		p += m_Info.cbPic;
		memcpy(p, pszText, cbText);
		// 
		GlobalUnlock(hGlobal);
		return hGlobal;
	FailLock:
		GlobalFree(hGlobal);
	FailAlloc:
		return NULL;
	}

	/// <summary>
	/// ƽ�滯���ݡ�
	/// �����������Է����������ԡ�
	/// �ڴ沼�ֲμ��ļ��ײ����ݰ汾���崦
	/// </summary>
	/// <returns></returns>
	virtual HGLOBAL FlattenInfo() { assert(FALSE); return NULL; }

	/// <summary>
	/// ȡ���ھ��
	/// </summary>
	/// <returns>���ھ��</returns>
	eStlInline HWND GetHWND() const
	{
		return m_hWnd;
	}
};


class CPushButton :public CButton
{
	SUBCLASS_MGR_DECL(CPushButton)
private:
	EBUTTONDATA_PUSHBTN m_InfoEx{};

	/// <summary>
	/// ������
	/// </summary>
	eStlInline void OnClick()
	{
		EVENT_NOTIFY2 evt(m_dwWinFormID, m_dwUnitID, 0);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD)&evt, 0);
	}

	/// <summary>
	/// ������ͷ������
	/// </summary>
	eStlInline void OnDropDownClick()
	{
		EVENT_NOTIFY2 evt(m_dwWinFormID, m_dwUnitID, 1);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD)&evt, 0);
	}

	/// <summary>
	/// ���������໯����
	/// </summary>
	SUBCLASS_PARENT_FNHEAD
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			if (m_CtrlSCInfo.count((HWND)lParam))
				m_CtrlSCInfo[(HWND)lParam]->OnClick();
			break;

		case WM_NOTIFY:
		{
			auto p = (NMHDR*)lParam;
			if (p->code == BCN_DROPDOWN)
				if (m_CtrlSCInfo.count(p->hwndFrom))
					m_CtrlSCInfo[p->hwndFrom]->OnDropDownClick();
		}
		break;

		case WM_DESTROY:
			m_SM.OnParentDestroy(hWnd);
			break;
		}

		SUBCLASS_RET_DEFPROC;
	}

	/// <summary>
	/// ��ť�������໯����
	/// </summary>
	SUBCLASS_CTRL_FNHEAD
	{
		auto p = (CPushButton*)dwRefData;
		switch (uMsg)
		{
		case WM_DESTROY:
			m_SM.OnCtrlDestroy(p);
			delete p;
			SUBCLASS_RET_DEFPROC;
		}

		elibstl::SendToParentsHwnd(p->m_dwWinFormID, p->m_dwUnitID, uMsg, wParam, lParam);
		SUBCLASS_RET_DEFPROC;
	}

public:
	CPushButton(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase(STD_ECTRL_CREATE_REAL_ARGS);

		if (pAllData)
			memcpy(&m_InfoEx, (BYTE*)pAllData + cbBaseData, sizeof(EBUTTONDATA_PUSHBTN));
		m_InfoEx.iVer = DATA_VER_BTN_PUSHBTN_1;

		m_hWnd = CreateWindowExW(0, WC_BUTTONW, m_Info.pszTextW, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, cx, cy, hParent, (HMENU)nId, GetModuleHandleW(NULL), NULL);

		InitBase(pAllData);
		SetDef(m_InfoEx.iDef);

		m_SM.OnCtrlCreate(this);
	}

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="iType">���ͣ��μ����Զ���</param>
	void SetType(int iType)
	{
		m_InfoEx.iType = iType;
		DWORD dwStyle = GetWindowLongPtrW(m_hWnd, GWL_STYLE)
			& (~(BS_PUSHBUTTON | BS_SPLITBUTTON | BS_DEFPUSHBUTTON | BS_DEFSPLITBUTTON));

		switch (iType)
		{
		case 0:
			dwStyle |= (m_InfoEx.iDef ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON);
			break;
		case 1:
			dwStyle |= (m_InfoEx.iDef ? BS_DEFSPLITBUTTON : BS_SPLITBUTTON);
			break;
		}

		SetWindowLongPtrW(m_hWnd, GWL_STYLE, dwStyle);
		Redraw();
	}

	/// <summary>
	/// ȡ����
	/// </summary>
	/// <returns>���ͣ��μ����Զ���</returns>
	eStlInline int GetType() const
	{
		return m_InfoEx.iType;
	}

	/// <summary>
	/// ���Ƿ�Ĭ��
	/// </summary>
	/// <param name="iDef">�Ƿ�Ĭ�ϣ��μ����Զ���</param>
	void SetDef(int iDef)
	{
		m_InfoEx.iDef = iDef;
		DWORD dwStyle = GetWindowLongPtrW(m_hWnd, GWL_STYLE)
			& (~(BS_DEFPUSHBUTTON | BS_DEFSPLITBUTTON | BS_PUSHBUTTON | BS_SPLITBUTTON));
		if (iDef)
			if (m_InfoEx.iType)
				dwStyle |= BS_DEFSPLITBUTTON;
			else
				dwStyle |= BS_DEFPUSHBUTTON;
		else
			if (m_InfoEx.iType)
				dwStyle |= BS_SPLITBUTTON;
			else
				dwStyle |= BS_PUSHBUTTON;

		SetWindowLongPtrW(m_hWnd, GWL_STYLE, dwStyle);
	}

	/// <summary>
	/// ȡ�Ƿ�Ĭ��
	/// </summary>
	/// <returns>�Ƿ�Ĭ�ϣ��μ����Զ���</returns>
	eStlInline int GetDef() const
	{
		return m_InfoEx.iDef;
	}

	/// <summary>
	/// ƽ�滯���ݡ�
	/// �����������Ի�ȡ���������ṩ���ݡ�
	/// �ڴ沼�ֲμ��ļ��ײ����ݰ汾���崦
	/// </summary>
	/// <returns></returns>
	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		auto hGlobal = FlattenInfoBase(sizeof(EBUTTONDATA_PUSHBTN), &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		memcpy(p + cbBaseData, &m_InfoEx, sizeof(EBUTTONDATA_PUSHBTN));
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}


	static HUNIT WINAPI ECreate(LPBYTE pAllPropertyData, INT nAllPropertyDataSize, DWORD dwStyle, HWND hParentWnd, UINT uID, HMENU hMenu,
		INT x, INT y, INT cx, INT cy, DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode
	)
	{
		auto pButton = new CPushButton(pAllPropertyData, nAllPropertyDataSize, dwStyle, x, y, cx, cy,
			hParentWnd, uID, blInDesignMode, dwWinFormID, dwUnitID);
		return elibstl::make_cwnd(pButton->GetHWND());
	}


	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0:// ͼƬ
			p->SetPic(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize);
			break;

		case 1:// �Ƿ�ͬʱ��ʾͼƬ���ı�
			p->SetTextImageShowing(pPropertyVaule->m_bool);
			break;

		case 2:// ����
			p->SetTextA(pPropertyVaule->m_szText);
			break;

		case 3:// ����W
			p->SetTextW((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;

		case 4:// �������
			p->SetAlign(TRUE, pPropertyVaule->m_int);
			break;

		case 5:// �������
			p->SetAlign(FALSE, pPropertyVaule->m_int);
			break;

		case 6:// ����
			p->SetFont((LOGFONTA*)pPropertyVaule->m_int);
			break;

		case 7:// Ĭ��
			p->SetDef(pPropertyVaule->m_int);
			break;

		case 8:// ����
			p->SetType(pPropertyVaule->m_int);
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
		case 0:// ͼƬ
			pPropertyVaule->m_data.m_pData = p->GetPic(&pPropertyVaule->m_data.m_nDataSize);
			break;

		case 1:// �Ƿ�ͬʱ��ʾͼƬ���ı�
			pPropertyVaule->m_bool = p->GetTextImageShowing();
			break;

		case 2:// ����
			pPropertyVaule->m_szText = p->GetTextA();
			break;

		case 3:// ����W
			if (pPropertyVaule->m_data.m_pData = (BYTE*)p->GetTextW())
				pPropertyVaule->m_data.m_nDataSize = wcslen((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;

		case 4:// �������
			pPropertyVaule->m_int = p->GetAlign(TRUE);
			break;

		case 5:// �������
			pPropertyVaule->m_int = p->GetAlign(FALSE);
			break;

		case 6:// ����
			pPropertyVaule->m_data.m_pData = p->GetFont();
			pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA);
			break;

		case 7:// Ĭ��
			pPropertyVaule->m_int = p->GetDef();
			break;

		case 8:// ����
			pPropertyVaule->m_int = p->GetType();
			break;
		}

		return TRUE;
	}

	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		if (nPropertyIndex == 3)
		{
			auto psz = p->GetTextW();
			std::wstring ws(psz);
			p->SetTextW(elibstl::MyInputBox(ws).c_str());
			*pblModified = TRUE;
		}
		else
			*pblModified = FALSE;

		return FALSE;
	}

	static PFN_INTERFACE WINAPI EGetInterface(INT nInterfaceNO)
	{
		switch (nInterfaceNO)
		{
		case ITF_CREATE_UNIT:
			// �������
			return (PFN_INTERFACE)ECreate;
		case ITF_NOTIFY_PROPERTY_CHANGED:
			// ֪ͨĳ�������ݱ��û��޸�
			return (PFN_INTERFACE)EChange;
		case ITF_GET_ALL_PROPERTY_DATA:
			// ȡȫ����������
			return (PFN_INTERFACE)EGetAlldata;
		case ITF_GET_PROPERTY_DATA:
			// ȡĳ��������
			return (PFN_INTERFACE)EGetData;
		case ITF_DLG_INIT_CUSTOMIZE_DATA:
			return (PFN_INTERFACE)EInputW;
		}
		return NULL;
	}
};

SUBCLASS_MGR_INIT(CPushButton, SCID_PUSHBTNPARENT, SCID_PUSHBTN)


class CCheckButton :public CButton
{
	SUBCLASS_MGR_DECL(CCheckButton)
private:
	EBUTTONDATA_CHECKBTN m_InfoEx{};

	SUBCLASS_PARENT_FNHEAD
	{

		SUBCLASS_RET_DEFPROC;
	}

	SUBCLASS_CTRL_FNHEAD
	{



		SUBCLASS_RET_DEFPROC;
	}
};

SUBCLASS_MGR_INIT(CCheckButton, SCID_CHECKBTNPARENT, SCID_CHECKBTN)

class CCommandLink :public CButton
{
	SUBCLASS_MGR_DECL(CCommandLink)
private:
	EBUTTONDATA_CMDLINK m_InfoEx{};

	SUBCLASS_PARENT_FNHEAD
	{

		SUBCLASS_RET_DEFPROC;
	}

		SUBCLASS_CTRL_FNHEAD
	{



		SUBCLASS_RET_DEFPROC;
	}
};

SUBCLASS_MGR_INIT(CCommandLink, SCID_CMDLINKPARENT, SCID_CMDLINK)

//////////////////////////////////////////////////////////////////////
/////////////////////////////��ͨ��ť//////////////////////////////////
//////////////////////////////////////////////////////////////////////
static EVENT_INFO2 s_Event_PushBtn[] =
{
	/*000*/ {"��ť������", "����ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
	/*001*/ {"������ͷ������", "��������ͷ������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_PushBtn[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/ {"ͼƬ", "Picture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*001*/ {"�Ƿ�ͬʱ��ʾͼƬ���ı�", "IsShowPicAndText", "�޷�Ԥ������Ҫͨ�������6.0", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*002*/ {"����", "Text", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_TEXT, _PROP_OS(__OS_WIN), NULL},
	/*003*/ {"����W", "TextW", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*004*/ {"������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0"},
	/*005*/ {"������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0"},
	/*006*/ {"����", "Font", "����������ʱ�ṩ������ĵ�ǰ�����ı�", UD_FONT, _PROP_OS(__OS_WIN) , NULL},
	/*007*/ {"Ĭ��", "Def", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "ͨ��\0""Ĭ��\0""\0"},
	/*008*/ {"����", "Type", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ͨ��ť\0""��ְ�ť\0""\0"},
};
//////////////////////////////////////////////////////////////////////
///////////////////////////////ѡ���//////////////////////////////////
//////////////////////////////////////////////////////////////////////
static EVENT_INFO2 s_Event_CheckBtn[] =
{
	/*000*/ {"��ť������", "����ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
	/*001*/ {"������ͷ������", "��������ͷ������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_CheckBtn[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/ {"ͼƬ", "Picture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*001*/ {"�Ƿ�ͬʱ��ʾͼƬ���ı�", "IsShowPicAndText", "�޷�Ԥ������Ҫͨ�������6.0", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*002*/ {"����", "Text", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_TEXT, _PROP_OS(__OS_WIN), NULL},
	/*003*/ {"����W", "TextW", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*004*/ {"������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0"},
	/*005*/ {"������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0"},
	/*006*/ {"����", "Font", "����������ʱ�ṩ������ĵ�ǰ�����ı�", UD_FONT, _PROP_OS(__OS_WIN) , NULL},
	/*007*/ {"Ĭ��", "Def", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "ͨ��\0""Ĭ��\0""\0"},
	/*008*/ {"����", "Type", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ͨ��ť\0""��ְ�ť\0""\0"},
};
//////////////////////////////////////////////////////////////////////
/////////////////////////////��������//////////////////////////////////
//////////////////////////////////////////////////////////////////////
static EVENT_INFO2 s_Event_CmdLink[] =
{
	/*000*/ {"��ť������", "����ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
	/*001*/ {"������ͷ������", "��������ͷ������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_CmdLink[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/ {"ͼƬ", "Picture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*001*/ {"�Ƿ�ͬʱ��ʾͼƬ���ı�", "IsShowPicAndText", "�޷�Ԥ������Ҫͨ�������6.0", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*002*/ {"����", "Text", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_TEXT, _PROP_OS(__OS_WIN), NULL},
	/*003*/ {"����W", "TextW", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*004*/ {"������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0"},
	/*005*/ {"������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0"},
	/*006*/ {"����", "Font", "����������ʱ�ṩ������ĵ�ǰ�����ı�", UD_FONT, _PROP_OS(__OS_WIN) , NULL},
	/*007*/ {"Ĭ��", "Def", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "ͨ��\0""Ĭ��\0""\0"},
	/*008*/ {"����", "Type", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ͨ��ť\0""��ְ�ť\0""\0"},
};

ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO CtButtonW =
{
	"��ťW",				//��������
	"ButtonW",			//Ӣ������
	"Unicode��ť",		//˵��
	0,					//��������
	0,					//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,
	103,				//��ԴID
	sizeof(s_Event_PushBtn) / sizeof(s_Event_PushBtn[0]),
	s_Event_PushBtn,
	sizeof(s_Member_PushBtn) / sizeof(s_Member_PushBtn[0]),
	s_Member_PushBtn,
	CPushButton::EGetInterface,
	0,					//��Ա����
	NULL				//��Ա��������
};

LIB_DATA_TYPE_INFO CtCheckButtonW =
{
	"ѡ���W",			//��������
	"CheckButtonW",		//Ӣ������
	"Unicodeѡ��򣨵�ѡ����ѡ����̬��ѡ��",//˵��
	0,					//��������
	0,					//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,
	103,				//��ԴID
	sizeof(s_Event_CheckBtn) / sizeof(s_Event_CheckBtn[0]),
	s_Event_CheckBtn,
	sizeof(s_Member_CheckBtn) / sizeof(s_Member_CheckBtn[0]),
	s_Member_CheckBtn,
	CPushButton::EGetInterface,
	0,					//��Ա����
	NULL				//��Ա��������
};

LIB_DATA_TYPE_INFO CtCommandLink =
{
	"�������Ӱ�ť",		//��������
	"CommandLink",		//Ӣ������
	"�������Ӱ�ť",		//˵��
	0,					//��������
	0,					//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,
	103,				//��ԴID
	sizeof(s_Event_CmdLink) / sizeof(s_Event_CmdLink[0]),
	s_Event_CmdLink,
	sizeof(s_Member_CmdLink) / sizeof(s_Member_CmdLink[0]),
	s_Member_CmdLink,
	CPushButton::EGetInterface,
	0,					//��Ա����
	NULL				//��Ա��������
};
ESTL_NAMESPACE_END