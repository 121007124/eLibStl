#include"EcontrolHelp.h"

#include <unordered_map>

#pragma warning(disable:4996)

#define SCID_PUSHBTN		20230425'01u
#define SCID_PUSHBTNPARENT	20230425'02u
#define SCID_CHECKBTN		20230425'03u
#define SCID_CHECKBTNPARENT	20230425'04u
#define SCID_CMDLINK		20230425'05u
#define SCID_CMDLINKPARENT	20230425'06u

#define BTN_COMM_PROP \
	/*000*/{"ͼƬ", "Picture", "", UD_PIC, _PROP_OS(__OS_WIN), NULL}, \
	/*001*/{ "�Ƿ�ͬʱ��ʾͼƬ���ı�", "IsShowPicAndText", "�޷�Ԥ������Ҫͨ�������6.0", UD_BOOL, _PROP_OS(__OS_WIN), NULL }, \
	/*002*/{ "����", "Text", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_TEXT, _PROP_OS(__OS_WIN), NULL }, \
	/*003*/{ "����W", "TextW", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL }, \
	/*004*/{ "������뷽ʽ", "AlignH", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0" }, \
	/*005*/{ "������뷽ʽ", "AlignV", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0" }, \
	/*006*/{ "����", "Font", "", UD_FONT, _PROP_OS(__OS_WIN) , NULL }

#define BTN_COMM_GETPROP \
	case 0:/*ͼƬ*/ \
		pPropertyVaule->m_data.m_pData = p->GetPic(&pPropertyVaule->m_data.m_nDataSize); \
		break; \
	case 1:/*�Ƿ�ͬʱ��ʾͼƬ���ı�*/ \
		pPropertyVaule->m_bool = p->GetTextImageShowing(); \
		break; \
	case 2:/*����*/ \
		pPropertyVaule->m_szText = p->GetTextA(); \
		break; \
	case 3:/*����W*/ \
		if (pPropertyVaule->m_data.m_pData = (BYTE*)p->GetTextW()) \
			pPropertyVaule->m_data.m_nDataSize = wcslen((PCWSTR)pPropertyVaule->m_data.m_pData); \
		break; \
	case 4:/*�������*/ \
		pPropertyVaule->m_int = p->GetAlign(TRUE); \
		break; \
	case 5:/*�������*/ \
		pPropertyVaule->m_int = p->GetAlign(FALSE); \
		break; \
	case 6:/*����*/ \
		pPropertyVaule->m_data.m_pData = p->GetFont(); \
		pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA); \
		break;

#define BTN_COMM_SETPROP \
	case 0:/*ͼƬ*/ \
		p->SetPic(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize); \
		break; \
	case 1:/*�Ƿ�ͬʱ��ʾͼƬ���ı�*/ \
		p->SetTextImageShowing(pPropertyVaule->m_bool); \
		break; \
	case 2:/*����*/ \
		p->SetTextA(pPropertyVaule->m_szText); \
		break; \
	case 3:/*����W*/ \
		p->SetTextW((PCWSTR)pPropertyVaule->m_data.m_pData); \
		break; \
	case 4:/*�������*/ \
		p->SetAlign(TRUE, pPropertyVaule->m_int); \
		break; \
	case 5:/*�������*/ \
		p->SetAlign(FALSE, pPropertyVaule->m_int); \
		break; \
	case 6:/*����*/ \
		p->SetFont((LOGFONTA*)pPropertyVaule->m_int); \
		break;

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
	int iCheckState;		// ѡ��״̬
	BOOL bPushLike;			// ��ť��ʽ
	BOOL bFlat;				// ƽ��
	BOOL bLeftText;			// �������
	int iFrame;				// �߿򣡣�������������������������������FIXME��������������������������������
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

	int cchNote;			// ע���ı����ȣ������ڱ�����Ϣ
	PWSTR pszNote;			// ע���ı�
	BOOL bShieldIcon;		// �Ƿ����ͼ��
	int iDef;				// Ĭ��
};


// ��ť���ࡣ
// ����ֱ��ʵ��������
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

	SIZE_T InitBase(LPVOID pAllData, int cbData, BOOL blInDesignMode, DWORD dwWinFormID, DWORD dwUnitID)
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
			{
				elibstl::DupStringForNewDeleteW(m_Info.pszTextW, (PCWSTR)p, m_Info.cchText);
				m_Info.pszTextA = elibstl::W2A(m_Info.pszTextW);
			}
			else
				m_Info.pszTextA = NULL;
		}
		else
		{
			m_Info.pszTextW = NULL;
			m_Info.pszTextA = NULL;
		}

		m_Info.pPicData = NULL;
		m_Info.iVer = DATA_VER_BTN_1;

		return sizeof(EBUTTONDATA) + m_Info.cbPic + m_Info.cchText * sizeof(WCHAR);
	}

	void InitBase(PCVOID pAllData)
	{
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
	}

	eStlInline void Redraw()
	{
		InvalidateRect(m_hWnd, NULL, TRUE);
		UpdateWindow(m_hWnd);
	}
public:
	CButton() {}

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
		if (!m_blInDesignMode)
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
		if (!m_blInDesignMode)
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
	HGLOBAL FlattenInfoBase(SIZE_T cbExtra = 0u, SIZE_T* pcbBaseData = NULL)
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

// ��ͨ��ť
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
	CPushButton() = delete;
	CPushButton(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase(pAllData, cbData, blInDesignMode, dwWinFormID, dwUnitID);
		if (!m_Info.pszTextW)
		{
			elibstl::DupStringForNewDeleteW(m_Info.pszTextW, L"��ťW");
			m_Info.pszTextA = elibstl::W2A(m_Info.pszTextW);
		}

		if (pAllData)
			memcpy(&m_InfoEx, (BYTE*)pAllData + cbBaseData, sizeof(EBUTTONDATA_PUSHBTN));
		m_InfoEx.iVer = DATA_VER_BTN_PUSHBTN_1;

		m_hWnd = CreateWindowExW(0, WC_BUTTONW, m_Info.pszTextW, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, cx, cy, hParent, (HMENU)nID, GetModuleHandleW(NULL), NULL);

		SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);
		InitBase(pAllData);
		SetDef(m_InfoEx.iDef);
		SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
		Redraw();
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


	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto pButton = new CPushButton(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(pButton->GetHWND());
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		BTN_COMM_SETPROP

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
		BTN_COMM_GETPROP

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

// ѡ���
class CCheckButton :public CButton
{
	SUBCLASS_MGR_DECL(CCheckButton)
private:
	EBUTTONDATA_CHECKBTN m_InfoEx{};

	/// <summary>
	/// ������
	/// </summary>
	eStlInline void OnClick()
	{
		EVENT_NOTIFY2 evt(m_dwWinFormID, m_dwUnitID, 0);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD)&evt, 0);
	}

	SUBCLASS_PARENT_FNHEAD
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			if (m_CtrlSCInfo.count((HWND)lParam))
				m_CtrlSCInfo[(HWND)lParam]->OnClick();
			break;

		case WM_DESTROY:
			m_SM.OnParentDestroy(hWnd);
			break;
		}

		SUBCLASS_RET_DEFPROC;
	}

	SUBCLASS_CTRL_FNHEAD
	{
		auto p = (CCheckButton*)dwRefData;
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
	CCheckButton() = delete;
	CCheckButton(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase(pAllData, cbData, blInDesignMode, dwWinFormID, dwUnitID);
		if (!m_Info.pszTextW)
		{
			elibstl::DupStringForNewDeleteW(m_Info.pszTextW, L"ѡ���W");
			m_Info.pszTextA = elibstl::W2A(m_Info.pszTextW);
		}

		if (pAllData)
			memcpy(&m_InfoEx, (BYTE*)pAllData + cbBaseData, sizeof(EBUTTONDATA_CHECKBTN));
		m_InfoEx.iVer = DATA_VER_BTN_CHECKBTN_1;

		m_hWnd = CreateWindowExW(0, WC_BUTTONW, m_Info.pszTextW, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_AUTORADIOBUTTON,
			x, y, cx, cy, hParent, (HMENU)nID, GetModuleHandleW(NULL), NULL);

		SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);
		InitBase(pAllData);
		SetType(m_InfoEx.iType);
		SetCheckState(m_InfoEx.iCheckState);
		SetPushLike(m_InfoEx.bPushLike);
		SetFlat(m_InfoEx.bFlat);
		SetLeftText(m_InfoEx.bLeftText);
		SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
		Redraw();
		m_SM.OnCtrlCreate(this);
	}

	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		auto hGlobal = FlattenInfoBase(sizeof(EBUTTONDATA_CHECKBTN), &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		memcpy(p + cbBaseData, &m_InfoEx, sizeof(EBUTTONDATA_CHECKBTN));
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}

	void SetType(int iType)
	{
		m_InfoEx.iType = iType;
		elibstl::ModifyWindowStyle(
			m_hWnd,
			elibstl::MultiSelect(iType, BS_AUTORADIOBUTTON, BS_AUTOCHECKBOX, BS_AUTO3STATE),
			BS_AUTORADIOBUTTON | BS_AUTOCHECKBOX | BS_AUTO3STATE);
		Redraw();
	}

	int GetType()
	{
		if (m_blInDesignMode)
			return m_InfoEx.iType;
		else
		{
			auto i = elibstl::MultiSelectWndStyle(m_hWnd, GWL_STYLE,
				BS_AUTORADIOBUTTON | BS_RADIOBUTTON, BS_AUTOCHECKBOX | BS_CHECKBOX, BS_AUTO3STATE | BS_3STATE);
			if (i < 0)
				i = 0;
			return i;
		}
	}

	eStlInline void SetCheckState(int iState)
	{
		m_InfoEx.iCheckState = iState;
		SendMessageW(m_hWnd, BM_SETCHECK, elibstl::MultiSelect(iState, BST_UNCHECKED, BST_CHECKED, BST_INDETERMINATE), 0);
	}

	int GetCheckState()
	{
		if (m_blInDesignMode)
			return m_InfoEx.iCheckState;
		else
		{
			int i = elibstl::MultiSelectEqual(SendMessageW(m_hWnd, BM_GETCHECK, 0, 0), BST_UNCHECKED, BST_CHECKED, BST_INDETERMINATE);
			if (i < 0)
				i = 0;
			return i;
		}
	}

	eStlInline void SetPushLike(BOOL bPushLike)
	{
		m_InfoEx.bPushLike = bPushLike;
		elibstl::ModifyWindowStyle(m_hWnd, bPushLike ? BS_PUSHLIKE : 0, BS_PUSHLIKE);
		Redraw();
	}

	eStlInline BOOL GetPushLike()
	{
		if (m_blInDesignMode)
			return m_InfoEx.bPushLike;
		else
			return !!(GetWindowLongPtrW(m_hWnd, GWL_STYLE) & BS_PUSHLIKE);
	}

	eStlInline void SetFlat(BOOL bFlat)
	{
		m_InfoEx.bFlat = bFlat;
		elibstl::ModifyWindowStyle(m_hWnd, bFlat ? BS_FLAT : 0, BS_FLAT);
		Redraw();
	}

	eStlInline BOOL GetFlat()
	{
		if (m_blInDesignMode)
			return m_InfoEx.bFlat;
		else
			return !!(GetWindowLongPtrW(m_hWnd, GWL_STYLE) & BS_FLAT);
	}

	eStlInline void SetLeftText(BOOL bLeftText)
	{
		m_InfoEx.bLeftText = bLeftText;
		elibstl::ModifyWindowStyle(m_hWnd, bLeftText ? BS_LEFTTEXT : 0, BS_LEFTTEXT);
		Redraw();
	}

	eStlInline BOOL GetLeftText()
	{
		if (m_blInDesignMode)
			return m_InfoEx.bLeftText;
		else
			return !!(GetWindowLongPtrW(m_hWnd, GWL_STYLE) & BS_LEFTTEXT);
	}

	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto pButton = new CCheckButton(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(pButton->GetHWND());
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		BTN_COMM_SETPROP

		case 7:// ����
			p->SetType(pPropertyVaule->m_int);
			break;

		case 8:// ѡ��
			p->SetCheckState(pPropertyVaule->m_int);
			break;

		case 9:// ��ť��ʽ
			p->SetPushLike(pPropertyVaule->m_bool);
			break;

		case 10:// ƽ��
			p->SetFlat(pPropertyVaule->m_bool);
			break;

		case 11:// �������
			p->SetLeftText(pPropertyVaule->m_bool);
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
		BTN_COMM_GETPROP

		case 7:// ����
			pPropertyVaule->m_int = p->GetType();
			break;

		case 8:// ѡ��
			pPropertyVaule->m_int = p->GetCheckState();
			break;

		case 9:// ��ť��ʽ
			pPropertyVaule->m_bool = p->GetPushLike();
			break;

		case 10:// ƽ��
			pPropertyVaule->m_bool = p->GetFlat();
			break;

		case 11:// �������
			pPropertyVaule->m_bool = p->GetLeftText();
			break;
		}
		return TRUE;
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
		}
		return NULL;
	}
};
SUBCLASS_MGR_INIT(CCheckButton, SCID_CHECKBTNPARENT, SCID_CHECKBTN)

// ��������
class CCommandLink :public CButton
{
	SUBCLASS_MGR_DECL(CCommandLink)
private:
	EBUTTONDATA_CMDLINK m_InfoEx{};

	/// <summary>
	/// ������
	/// </summary>
	eStlInline void OnClick()
	{
		EVENT_NOTIFY2 evt(m_dwWinFormID, m_dwUnitID, 0);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD)&evt, 0);
	}

	SUBCLASS_PARENT_FNHEAD
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			if (m_CtrlSCInfo.count((HWND)lParam))
				m_CtrlSCInfo[(HWND)lParam]->OnClick();
			break;

		case WM_DESTROY:
			m_SM.OnParentDestroy(hWnd);
			break;
		}

		SUBCLASS_RET_DEFPROC;
	}

	SUBCLASS_CTRL_FNHEAD
	{
		auto p = (CCommandLink*)dwRefData;
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
	CCommandLink() = delete;
	CCommandLink(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase(pAllData, cbData, blInDesignMode, dwWinFormID, dwUnitID);
		if (!m_Info.pszTextW)
		{
			elibstl::DupStringForNewDeleteW(m_Info.pszTextW, L"��������");
			m_Info.pszTextA = elibstl::W2A(m_Info.pszTextW);
		}

		if (pAllData)
			memcpy(&m_InfoEx, (BYTE*)pAllData + cbBaseData, sizeof(EBUTTONDATA_CMDLINK));
		m_InfoEx.iVer = DATA_VER_BTN_CHECKBTN_1;
		m_InfoEx.pszNote = NULL;
		if (m_InfoEx.cchNote)
		{
			elibstl::DupStringForNewDeleteW(m_InfoEx.pszNote, 
				(PCWSTR)((BYTE*)pAllData + cbBaseData + sizeof(EBUTTONDATA_CMDLINK)), m_InfoEx.cchNote);
		}

		m_hWnd = CreateWindowExW(0, WC_BUTTONW, m_Info.pszTextW, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE |
			(m_blInDesignMode ? BS_PUSHBUTTON : BS_COMMANDLINK),
			x, y, cx, cy, hParent, (HMENU)nID, GetModuleHandleW(NULL), NULL);

		SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);
		InitBase(pAllData);
		SetShieldIcon(m_InfoEx.bShieldIcon);
		SetNote(m_InfoEx.pszNote);
		SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
		Redraw();
		m_SM.OnCtrlCreate(this);
	}

	~CCommandLink()
	{
		delete[] m_InfoEx.pszNote;
	}

	/// <summary>
	/// ��ע���ı�
	/// </summary>
	/// <param name="pszText">�ı�ָ��</param>
	/// <returns>�ɹ�����TRUE��ʧ�ܷ���FALSE</returns>
	eStlInline BOOL SetNote(PCWSTR pszText)
	{
		if (m_blInDesignMode)
		{
			if (pszText)
				elibstl::DupStringForNewDeleteW(m_InfoEx.pszNote, pszText);
			else
			{
				delete[] m_InfoEx.pszNote;
				m_InfoEx.pszNote = NULL;
			}
			return TRUE;
		}
		else
			return SendMessageW(m_hWnd, BCM_SETNOTE, 0, (LPARAM)pszText);
	}

	/// <summary>
	/// ȡע���ı���
	/// ���ص��ı�Ϊ�����ڲ����У������ͷ�
	/// </summary>
	/// <returns>�ı�ָ��</returns>
	PWSTR GetNote()
	{
		if (!m_blInDesignMode)
		{
			int cch = SendMessageW(m_hWnd, BCM_GETNOTELENGTH, 0, 0);
			delete[] m_InfoEx.pszNote;
			if (cch)
			{
				m_InfoEx.pszNote = new WCHAR[cch + 1];
				SendMessageW(m_hWnd, BCM_GETNOTE, cch + 1, (LPARAM)m_InfoEx.pszNote);
			}
			else
				m_InfoEx.pszNote = NULL;
		}

		return m_InfoEx.pszNote;
	}

	eStlInline void SetShieldIcon(BOOL bShieldIcon)
	{
		m_InfoEx.bShieldIcon = bShieldIcon;
		SendMessageW(m_hWnd, BCM_SETSHIELD, 0, bShieldIcon);
	}

	eStlInline BOOL GetShieldIcon()
	{
		return m_InfoEx.bShieldIcon;// �������ֻ���ò���ȡ.....�Ѽ�¼��ֵ���ػ�ȥ��
	}

	/// <summary>
	/// ���Ƿ�Ĭ��
	/// </summary>
	/// <param name="iDef">�Ƿ�Ĭ�ϣ��μ����Զ���</param>
	void SetDef(int iDef)
	{
		m_InfoEx.iDef = iDef;
		DWORD dwStyle = GetWindowLongPtrW(m_hWnd, GWL_STYLE)
			& (~(BS_DEFPUSHBUTTON | BS_PUSHBUTTON | BS_DEFCOMMANDLINK | BS_COMMANDLINK));
		if (iDef)
			dwStyle |= (m_blInDesignMode ? BS_DEFPUSHBUTTON : BS_DEFCOMMANDLINK);
		else
			dwStyle |= (m_blInDesignMode ? BS_PUSHBUTTON : BS_COMMANDLINK);

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

	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		SIZE_T cbData = sizeof(EBUTTONDATA_CMDLINK);
		if (GetNote())
		{
			m_InfoEx.cchNote = wcslen(m_InfoEx.pszNote);
			cbData += (m_InfoEx.cchNote * sizeof(WCHAR));
		}
		else
			m_InfoEx.cchNote = 0;
		auto hGlobal = FlattenInfoBase(cbData, &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		// �ṹ
		p += cbBaseData;
		memcpy(p, &m_InfoEx, sizeof(EBUTTONDATA_CMDLINK));
		// ע���ı�
		p += sizeof(EBUTTONDATA_CMDLINK);
		if (m_InfoEx.pszNote)
			memcpy(p, m_InfoEx.pszNote, m_InfoEx.cchNote * sizeof(WCHAR));
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}

	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto pButton = new CCommandLink(STD_ECTRL_CREATE_REAL_ARGS);
		if (pButton->GetHWND())
			return elibstl::make_cwnd(pButton->GetHWND());
		else
		{
			delete pButton;
			return NULL;
		}
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		BTN_COMM_SETPROP

		case 7:// ע���ı�
			p->SetNote((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;

		case 8:// �Ƿ����ͼ��
			p->SetShieldIcon(pPropertyVaule->m_bool);
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
		BTN_COMM_GETPROP

		case 7:// ע���ı�
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetNote();
			break;

		case 8:// �Ƿ����ͼ��
			pPropertyVaule->m_bool = p->GetShieldIcon();
			break;
		}
		return TRUE;
	}

	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		if (nPropertyIndex == 7)
		{
			PCWSTR psz = p->GetNote();
			std::wstring ws;// ��������������������FIXME����������������������
			p->SetNote(elibstl::MyInputBox(ws).c_str());
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

	BTN_COMM_PROP,
	/*007*/ {"Ĭ��", "Def", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "ͨ��\0""Ĭ��\0""\0"},
	/*008*/ {"����", "Type", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ͨ��ť\0""��ְ�ť\0""\0"},
};

//////////////////////////////////////////////////////////////////////
///////////////////////////////ѡ���//////////////////////////////////
//////////////////////////////////////////////////////////////////////
static EVENT_INFO2 s_Event_CheckBtn[] =
{
	/*000*/ {"������", "��ѡ��򱻵���ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_CheckBtn[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	BTN_COMM_PROP,
	/*007*/ {"����", "Type", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��ѡ��\0""��ѡ��\0""��̬��ѡ��\0""\0"},
	/*008*/ {"ѡ��", "CheckState", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "δѡ��\0""ѡ��\0""��ѡ��\0""\0"},
	/*009*/ {"��ť��ʽ", "IsBtnLike", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*010*/ {"ƽ��", "IsFlat", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*011*/ {"�������", "IsTextLeft", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*012*/ {"�߿�", "Frame", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""���侵��ʽ\0""\0"},
};

//////////////////////////////////////////////////////////////////////
/////////////////////////////��������//////////////////////////////////
//////////////////////////////////////////////////////////////////////
static EVENT_INFO2 s_Event_CmdLink[] =
{
	/*000*/ {"������", "����ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_CmdLink[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	BTN_COMM_PROP,
	/*007*/ {"ע���ı�", "Note", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*008*/ {"�Ƿ�Ϊ����ͼ��", "IsShieldIcon", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL },
};


static int s_Cmd_PushBtn[] = { 120 };

EXTERN_C void BtnGetIdealSize(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	SIZE size{ *pArgInf[1].m_pInt,0 };
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	if (!hWnd)
		goto Fail;
	if(!SendMessageW(hWnd, BCM_GETIDEALSIZE, 0, (LPARAM)&size))
		goto Fail;
	*pArgInf[1].m_pInt = size.cx;
	*pArgInf[2].m_pInt = size.cy;
	pRetData->m_bool = TRUE;
	return;
Fail:
	pRetData->m_bool = FALSE;
	return;
}

static ARG_INFO s_Args_BtnGetIdealSize[] =
{
	{
		/*name*/    "���",
		/*explain*/ "���ܿ�ȵı�����������ǰ������Ϊ0���������ֵָ��Ϊ��ť�Ŀ�ȣ����Դ�ֵΪ��׼��������߶�",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_RECEIVE_VAR,
	},
	{
		/*name*/    "�߶�",
		/*explain*/ "���ܸ߶ȵı���",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_RECEIVE_VAR,
	}
};

FucInfo s_Fn_BtnGetIdealSize = { {
		/*ccname*/  ("ȡ����ߴ�"),
		/*egname*/  ("GetIdealSize"),
		/*explain*/ ("ȡ��ť����ߴ硣��������Ҫ���嵥��ָ��Comctl6.0"),
		/*category*/-1,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_BtnGetIdealSize),
		/*arg lp*/  s_Args_BtnGetIdealSize,
	} ,BtnGetIdealSize ,"BtnGetIdealSize" };


ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO CtButtonW =
{
	"��ťW",				//��������
	"ButtonW",			//Ӣ������
	"Unicode��ť",		//˵��
	ARRAYSIZE(s_Cmd_PushBtn),
	s_Cmd_PushBtn,
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,
	103,				//��ԴID
	ARRAYSIZE(s_Event_PushBtn),
	s_Event_PushBtn,
	ARRAYSIZE(s_Member_PushBtn),
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
	ARRAYSIZE(s_Event_CheckBtn),
	s_Event_CheckBtn,
	ARRAYSIZE(s_Member_CheckBtn),
	s_Member_CheckBtn,
	CCheckButton::EGetInterface,
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
	ARRAYSIZE(s_Event_CmdLink),
	s_Event_CmdLink,
	ARRAYSIZE(s_Member_CmdLink),
	s_Member_CmdLink,
	CCommandLink::EGetInterface,
	0,					//��Ա����
	NULL				//��Ա��������
};
ESTL_NAMESPACE_END