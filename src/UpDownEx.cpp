#include"EcontrolHelp.h"
#include<Richedit.h>
#pragma warning(disable:4996)
//static INT s_cmd[] = { 0 };

static EVENT_ARG_INFO2  s_event_arg = { "��ťֵ", "������µ��ǵ����������ϼ�ͷ��ť������ֵΪ 1������Ϊ -1", 0, SDT_INT };



static EVENT_INFO2 s_event[] =
{
	/*000*/ {"����ť������", "������ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 1, &s_event_arg, _SDT_NULL},
};
static UNIT_PROPERTY s_member[] =
{
	// FIXED_WIN_UNIT_PROPERTY,    // ������ϴ˺�, ����ֱ��չ��, �����չ��
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

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
	/*000*/  {"�Զ�ģʽ","SpinMod", "�Զ����������ٽ��ܻص�����,����ϵͳ���ơ����ڰ�������ı�������ֵ'+','-' ����",  UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*001*/  {"����������", "SpinMin", "���õ���������", UD_INT, _PROP_OS(OS_ALL) | UW_HAS_INDENT, NULL},
	/*002*/  {"����������", "SpinMax", "���õ���������", UD_INT, _PROP_OS(OS_ALL) | UW_HAS_INDENT, NULL},
	/*003*/  {"����", "SpinDirection", "���õ���������", UD_PICK_INT, _PROP_OS(OS_ALL),  "����\0""����\0""\0"},
	/*004*/  {"�����", "BindHwnd", "���󶨵Ŀɱ༭�ı������Ի�����", UD_INT, _PROP_OS(OS_ALL) | UW_CANNOT_INIT, NULL},
};


typedef struct _EBinWindowData
{
	HWND m_hWnd;
	RECT m_Rect;
	_EBinWindowData() {
		memset(this, 0, sizeof(*this));
	}
}BINDWINDOWDATA, * LPBINDWINDOWDATA;

typedef struct _eUpDowndata {
	bool m_Auto;
	int m_min;
	int m_max;
	bool m_IsHorizontal;
	BINDWINDOWDATA m_win_data;
	_eUpDowndata() {
		memset(this, 0, sizeof(*this));
	}
}EUODOWNDATA, * LPEUODOWNDATA;

class eUpDown
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		eUpDown* pEUpDown = (eUpDown*)GetWindowLongPtrW(hWnd, GWL_USERDATA);
		if (!pEUpDown)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		WNDPROC oldproc = pEUpDown->GetOldProc();
		if (oldproc == NULL)
		{
			oldproc = DefWindowProc;
		}

		switch (message)
		{
		case WM_SIZE://�������ڱ��ı�ʱ�ı䰴ť��С
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			pEUpDown->SetPos(width, height);
			break;
		}
		case WM_NOTIFY: {
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;
			// �����Ϣ����
			switch (lpnmhdr->code)
			{
			case UDN_DELTAPOS:
			{
				//���ܵ�����������
				NMUPDOWN nID = *(NMUPDOWN*)lParam;
				pEUpDown->OnDeltaPos(nID.iDelta);
				//���������ͨ�� nID ��֪������һ������
				break;
			}
			}
			//return TRUE;
			break;
		}
		case WM_DESTROY: {

			pEUpDown->~eUpDown();
			return FALSE;
		}
		default:
			break;
		}
		return oldproc(hWnd, message, wParam, lParam);
	};
	static LRESULT CALLBACK WndCProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		eUpDown* pEUpDown = (eUpDown*)GetWindowLongPtrW(hWnd, GWL_USERDATA);
		if (!pEUpDown)
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		WNDPROC oldproc = pEUpDown->GetCOldProc();
		if (oldproc == NULL)
		{
			oldproc = DefWindowProc;
		}

		bool is_continue = elibstl::SendToParentsHwnd(pEUpDown->GetID(), pEUpDown->GetUID(), uMsg, wParam, lParam);
		if (!is_continue)
		{
			return 0;
		}
		return oldproc(hWnd, uMsg, wParam, lParam);
	};
private:
	//����������͵��������
	HWND m_hWnd, m_hParentWnd;
	//�����ɹ���
	WNDPROC m_oldProc, m_ColdProc;;
	//�󶨴��ڵ�����
	BINDWINDOWDATA m_BindWinow;
	INT m_dwWinFormID, m_dwUnitID;
public:
	~eUpDown() {
		if (m_hWnd)
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}
		if (m_hParentWnd)
		{
			::SetWindowLongW(m_hParentWnd, GWL_WNDPROC, (LONG_PTR)m_oldProc);
			::DestroyWindow(m_hParentWnd);
		}

	}
	eUpDown(
		LPBYTE pAllPropertyData,
		INT nAllPropertyDataSize,
		DWORD dwStyle,
		int x,
		int y,
		int cx,
		int cy,
		HWND hParent,
		UINT nId,
		DWORD dwWinFormID,
		DWORD dwUnitID
	) :
		m_BindWinow({}),
		m_hWnd(nullptr),
		m_hParentWnd(nullptr),
		m_oldProc(nullptr),
		m_dwWinFormID(dwWinFormID), m_dwUnitID(dwUnitID),
		m_ColdProc(NULL)
	{

		//��������
		m_hParentWnd = CreateWindowExW(0, L"Static", 0, dwStyle | WS_VISIBLE | WS_CHILDWINDOW | WS_CLIPSIBLINGS, x, y, cx, cy, hParent, (HMENU)nId, GetModuleHandle(0), 0);
		//����������

		dwStyle |= WS_CHILD | UDS_ARROWKEYS | WS_VISIBLE;
		BOOL HasData = FALSE;
		if (pAllPropertyData && nAllPropertyDataSize == sizeof(EUODOWNDATA))
		{
			HasData = TRUE;
			LPEUODOWNDATA nData = reinterpret_cast<LPEUODOWNDATA>(pAllPropertyData);
			if (nData->m_IsHorizontal)
			{
				dwStyle |= UDS_HORZ;
			}
			if (nData->m_Auto)
			{
				dwStyle |= UDS_SETBUDDYINT;
			}

		}
		m_hWnd = CreateWindowExW(0,
			UPDOWN_CLASS,
			L"",
			dwStyle,
			0,
			0,
			0,
			0,
			m_hParentWnd,
			(HMENU)nId,
			GetModuleHandle(0),
			NULL
		);

		if (HasData)
		{
			LPEUODOWNDATA nData = reinterpret_cast<LPEUODOWNDATA>(pAllPropertyData);
			SetUpDownMax(nData->m_max);
			SetUpDownMin(nData->m_min);
			SetUpDownAuto(nData->m_Auto);
			if (nData->m_win_data.m_hWnd)
			{
				BindHwnd(nData->m_win_data.m_hWnd);
			}
		}
		else
		{
			SetUpDownMax(100);
			SetUpDownMin(0);

		}
		//����������Ͷ���
		SetWindowLongPtrW(m_hParentWnd, GWL_USERDATA, (LONG_PTR)this);
		//�������ص�����
		m_oldProc = (WNDPROC)SetWindowLongW(m_hParentWnd, GWL_WNDPROC, (LONG_PTR)WndProc);
		//���໯���
		SetWindowLongPtrW(m_hWnd, GWL_USERDATA, (LONG_PTR)this);
		//��¼ԭʼ�ص�
		m_ColdProc = (WNDPROC)SetWindowLongW(m_hWnd, GWL_WNDPROC, (LONG_PTR)WndCProc);
		SetPos(cx, cy);

	}
public:
	DWORD GetID() const {

		return m_dwWinFormID;
	}
	DWORD GetUID() const {

		return m_dwUnitID;
	}
	WNDPROC GetCOldProc() const {
		if (!m_hWnd)
		{
			return 0;
		}
		if (m_ColdProc <= 0)
		{
			return 0;
		}
		return m_ColdProc;
	}
	//���»ص�
	void OnDeltaPos(int nID)
	{
		EVENT_NOTIFY event(m_dwWinFormID, m_dwUnitID, 0);
		event.m_nArgCount = 1;  // һ��������  
		event.m_nArgValue[0] = nID;
		elibstl::NotifySys(NRS_EVENT_NOTIFY, (DWORD) & event, 0);


	}
	//�����Զ�,�Զ��Ͱ󶨴��ڱ���ͬʱ�����򲻻�ִ��
	void SetUpDownAuto(bool nIsAuto) {
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		if (nIsAuto)
		{
			dwStyle |= UDS_SETBUDDYINT;
		}
		else
		{
			dwStyle &= ~UDS_SETBUDDYINT;
		}
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	}
	//ȡ�Զ�
	bool GetUpDownAuto() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		return (dwStyle & UDS_SETBUDDYINT) != 0;

	}
	//������С���ı�ʱ����ô˺���
	BOOL SetPos(int width, int height) {
		if (!m_hWnd || !m_hParentWnd)
		{
			return FALSE;
		}
		return SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	}
	//ȡ�������
	HWND GetContainerHwnd() {
		return m_hParentWnd;
	}
	//ȡ���������
	HWND GetUpDownHwnd() {
		if (!m_hParentWnd)
		{
			return 0;
		}
		return m_hWnd;
	}
	//ȡ����ԭ����
	WNDPROC GetOldProc() {
		if (!m_hParentWnd)
		{
			return nullptr;
		}
		if (m_oldProc < 0)
		{
			return nullptr;
		}
		return m_oldProc;
	}
	//ȡ����������
	int GetUpDownMax() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		int nMax;
		SendMessageW(m_hWnd, UDM_GETRANGE32, NULL, reinterpret_cast<LPARAM>(&nMax));
		return nMax;
	}
	//ȡ����������
	int GetUpDownMin() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		int nMin;
		SendMessageW(m_hWnd, UDM_GETRANGE32, reinterpret_cast<WPARAM>(&nMin), NULL);
		return nMin;
	}
	//�õ���������
	bool SetUpDownMax(int nMax) {
		if (!m_hParentWnd || !m_hWnd)
		{
			return false;
		}
		return SendMessageW(m_hWnd, UDM_SETRANGE32, GetUpDownMin(), nMax);
	}
	//�õ���������
	bool SetUpDownMin(int nMin) {
		if (!m_hParentWnd || !m_hWnd)
		{
			return false;
		}
		return SendMessageW(m_hWnd, UDM_SETRANGE32, nMin, GetUpDownMax());
	}
	//���õ���������
	void SetDirection(bool isHorizontal)
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		if (isHorizontal)
			dwStyle |= UDS_HORZ;
		else
			dwStyle &= ~UDS_HORZ;
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
		RECT rect = { 0,0,100,100 };
		InvalidateRect(m_hWnd, &rect, TRUE);
	}
	//ȡ�������Ƿ����
	bool IsHorizontal()
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return false;
		}
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		return (dwStyle & UDS_HORZ) != 0;
	}
	//�󶨴���
	void BindHwnd(HWND nContorl) {
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}
		if (nContorl == 0)//�������Ϊ0��ȡ����
		{
			memset(&m_BindWinow, 0, sizeof(_EBinWindowData));
			SendMessage(m_hWnd, UDM_SETBUDDY, 0, 0);
		}
		if (!IsWindow(nContorl))
		{
			return;
		}
		m_BindWinow.m_hWnd = nContorl;
		GetWindowRect(m_hWnd, &m_BindWinow.m_Rect);
		SendMessage(m_hWnd, UDM_SETBUDDY, reinterpret_cast <WPARAM>(m_BindWinow.m_hWnd), 0);
	}
	//ȡ�󶨵Ĵ���
	HWND GetBindHwnd() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		return m_BindWinow.m_hWnd;
	}
	//ȡ�󶨵Ĵ�������
	BINDWINDOWDATA GetBindHwndData() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return {};
		}
		return m_BindWinow;
	}
};


#pragma region �����Խӿں���

#pragma endregion

static BOOL WINAPI Change(HUNIT hUnit, INT nPropertyIndex,  // ���޸ĵ���������
	UNIT_PROPERTY_VALUE* pPropertyVaule, // �����޸ĵ���Ӧ��������
	LPTSTR* ppszTipText) {  //Ŀǰ��δʹ��


	eUpDown* pEupDown = elibstl::get_obj_from_hunit<eUpDown*>(hUnit);
	if (!pEupDown)
	{
		return false;
	}
	switch (nPropertyIndex)
	{
	case 0:
		pEupDown->SetUpDownAuto(pPropertyVaule->m_bool);
		return TRUE;
	case 1:
		pEupDown->SetUpDownMin(pPropertyVaule->m_int);
		return TRUE;
	case 2:
		pEupDown->SetUpDownMax(pPropertyVaule->m_int);
		return TRUE;
	case 3:
		pEupDown->SetDirection(pPropertyVaule->m_int);
		return TRUE;
	case 4:
	{
		pEupDown->BindHwnd(reinterpret_cast<HWND>(pPropertyVaule->m_int));
		return TRUE;
	}
	default:

		break;
	}
	return FALSE;
}
static BOOL WINAPI GetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
{
	eUpDown* pEupDown = elibstl::get_obj_from_hunit<eUpDown*>(hUnit);
	if (!pEupDown)
	{
		return false;
	}
	switch (nPropertyIndex)
	{
	case 0:
	{
		pPropertyVaule->m_bool = pEupDown->GetUpDownAuto();
		break;
	}
	case 1:
		pPropertyVaule->m_int = pEupDown->GetUpDownMin();

		break;
	case 2:
		pPropertyVaule->m_int = pEupDown->GetUpDownMax();
		break;
	case 3:
	{
		pPropertyVaule->m_int = pEupDown->IsHorizontal();
		break;
	}
	case 4:
	{
		pPropertyVaule->m_int = reinterpret_cast<INT>(pEupDown->GetBindHwnd());
		break;
	}

	default:

		return false;
	}
	return false;
}
static HGLOBAL WINAPI GetAlldata(HUNIT hUnit)
{
	eUpDown* pEupDown = elibstl::get_obj_from_hunit<eUpDown*>(hUnit);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(EUODOWNDATA));
	EUODOWNDATA temp;
	temp.m_Auto = pEupDown->GetUpDownAuto();
	temp.m_IsHorizontal = pEupDown->IsHorizontal();
	temp.m_max = pEupDown->GetUpDownMax();
	temp.m_min = pEupDown->GetUpDownMin();
	temp.m_win_data = pEupDown->GetBindHwndData();
	if (hGlobal)
	{
		PVOID pGlobal = ::GlobalLock(hGlobal);
		if (pGlobal)
		{
			memcpy(pGlobal, &temp, sizeof(EUODOWNDATA));
			::GlobalUnlock(hGlobal);
		}
	}
	return  hGlobal;
}
static HUNIT WINAPI Create(
	LPBYTE pAllPropertyData,            //   ָ�򱾴��ڵ�Ԫ��������������, �ɱ����ڵ�Ԫ��ITF_GET_PROPERTY_DATA�ӿڲ���, ���û��������ΪNULL
	INT nAllPropertyDataSize,           //   �ṩpAllPropertyData��ָ�����ݵĳߴ�, ���û����Ϊ0
	DWORD dwStyle,                      //   Ԥ�����õĴ��ڷ��
	HWND hParentWnd,                    //   �����ھ��
	UINT uID,                           //   �ڸ������е�ID
	HMENU hMenu,                        //   δʹ��
	INT x, INT y, INT cx, INT cy,       //   ָ��λ�ü��ߴ�
	DWORD dwWinFormID, DWORD dwUnitID,  //   �����ڵ�Ԫ���ڴ��ڼ������ID, ����֪ͨ��ϵͳ
	HWND hDesignWnd,                    //   ���blInDesignModeΪ��, ��hDesignWnd�ṩ����ƴ��ڵĴ��ھ��
	BOOL blInDesignMode                 //   ˵���Ƿ�������IDE�����Խ��п��ӻ����, ����ʱΪ��
)
{
	eUpDown* pUpDown = new eUpDown(pAllPropertyData, nAllPropertyDataSize, dwStyle, x, y, cx, cy, hParentWnd, uID, dwWinFormID, dwUnitID);
	return elibstl::make_cwnd(pUpDown->GetContainerHwnd());
}
static INT WINAPI DefSize(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	switch (nMsg)
	{
	case NU_GET_CREATE_SIZE_IN_DESIGNER:
	{
		*((int*)dwParam1) = 17;
		*((int*)dwParam2) = 110;
	}
	return TRUE;
	}
	return FALSE;
}


EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_UpDown(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
	{
		// �������
		return (PFN_INTERFACE)Create;
	}
	case ITF_NOTIFY_PROPERTY_CHANGED:
	{
		// ֪ͨĳ�������ݱ��û��޸�
		return (PFN_INTERFACE)Change;
	}
	case ITF_GET_ALL_PROPERTY_DATA:
	{
		// ȡȫ����������
		return (PFN_INTERFACE)GetAlldata;
	}
	case ITF_GET_PROPERTY_DATA:
	{
		// ȡĳ��������
		return (PFN_INTERFACE)GetData;
	}
	case ITF_GET_NOTIFY_RECEIVER:
	{
		// ȡ����ĸ���֪ͨ������(PFN_ON_NOTIFY_UNIT)
		return (PFN_INTERFACE)DefSize;
	}
	default:
		return NULL;
	}
	return NULL;
}




static INT s_cmd[] = { 121 };


namespace elibstl {

	LIB_DATA_TYPE_INFO UpDown = {
		"������",//��������
		"UpDown",//Ӣ������
		"���Ʊ༭�ĵ����������и���Ĺ��ܺ���չ��",//˵��
		sizeof(s_cmd) / sizeof(INT),//��������
		s_cmd,//��ȫ�ֺ����ж�Ӧ������
		_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
		IDB_UPDOWN_EX,//��ԴID
		sizeof(s_event) / sizeof(s_event[0]),
		s_event,//�¼�
		sizeof(s_member) / sizeof(s_member[0]),//������
		s_member,//����ָ��
		libstl_GetInterface_UpDown,//��������ӳ���
		NULL,//��Ա����
		NULL//��Ա��������
	};
}


EXTERN_C void Fn_EUpDown_GetHwnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	eUpDown* pEUpDown = (eUpDown*)GetWindowLongPtrW(hWnd, GWL_USERDATA);

	pRetData->m_int = reinterpret_cast<INT>(pEUpDown->GetUpDownHwnd());
}


FucInfo EUpDown_GetHwnd = { {
		/*ccname*/  ("ȡ������"),
		/*egname*/  ("GetHwnd"),
		/*explain*/ ("ȡ���ھ����ȡ��Ϊ�������,������Ϊȡ������������ľ����"),
		/*category*/-1,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,Fn_EUpDown_GetHwnd ,"Fn_EUpDown_GetHwnd" };


