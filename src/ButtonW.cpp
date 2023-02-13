#include"EcontrolHelp.h"
#pragma warning(disable:4996)
using namespace std;
#define PARBUTTONPROC L"Buntton_ParentOldProc"




static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK WndCProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//�����ͼƬ��ͼƬ��ֱ�����ݽ���Ϊ���ڱ���
typedef struct _EButtonExDATA
{
	//������뷽ʽ
	INT m_transverse;
	//������뷽ʽ
	INT m_portrait;
	//��������
	EFONTDATA m_font_data;
	//ͼƬ����
	DWORD m_pic_lenth;
	_EButtonExDATA()
	{
		memset(this, 0, sizeof(_EButtonExDATA));
	}
	LPBYTE get_pic(DWORD* size) {
		*size = 0;
		if (m_pic_lenth <= 0)
		{
			return 0;
		}
		*size = m_pic_lenth;
		return reinterpret_cast<LPBYTE>(this) + sizeof(_EButtonExDATA);
	};
	DWORD get_size_with_pic() {
		return sizeof(_EButtonExDATA) + m_pic_lenth;
	}
	wchar_t* get_text() {
		return reinterpret_cast<wchar_t*>(reinterpret_cast<LPBYTE>(this) + sizeof(_EButtonExDATA) + m_pic_lenth);
	}
} EBUTTONDATA, * PEBUTTONDATA;//�������ͼƬ�����ݣ����ݳ���Ϊpiclenth

class eButtonEx
{
public:
	eButtonEx(
		LPVOID data,
		size_t lenth,
		DWORD dwStyle,
		int x,
		int y,
		int cx,
		int cy,
		HWND hParent,
		UINT nId,
		BOOL blInDesignMode,
		DWORD dwWinFormID,
		DWORD dwUnitID
	) :m_hParentWnd(NULL),
		m_hWnd(NULL),
		m_oldProc(NULL),
		m_blInDesignMode(blInDesignMode),
		m_dwWinFormID(dwWinFormID),
		m_dwUnitID(dwUnitID),
		m_TextA(NULL),
		m_transverse(1),
		m_portrait(1),
		m_hBitmap(NULL),
		m_TextW(NULL),
		m_ColdProc(NULL)

	{
		//���������������
		m_hParentWnd = CreateWindowExW(0, L"Static", 0, dwStyle | WS_VISIBLE | WS_CHILDWINDOW | WS_CLIPSIBLINGS, x, y, cx, cy, hParent, (HMENU)nId, GetModuleHandle(0), 0);
		//���ڷ��
		dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_MULTILINE | BS_PUSHBUTTON | WS_CLIPSIBLINGS | dwStyle;
		//������ť
		if (data)
		{
			wstring Title = L"��ť";
			PEBUTTONDATA base = reinterpret_cast<PEBUTTONDATA>(data);


			if (lenth - base->get_size_with_pic() > 0)//������ⲻΪ��
			{
				Title = wstring(base->get_text(), (lenth - base->get_size_with_pic()) / 2);
			}
			m_hWnd = CreateWindowExW(0, WC_BUTTON, Title.c_str(), dwStyle, 0, 0, cx, cy, m_hParentWnd, (HMENU)nId, GetModuleHandle(0), 0);
			if (base->m_font_data.m_hFont)//������Ч
			{
				SetFont(base->m_font_data.m_data);
			}
			SetButtonTextTransverse(base->m_transverse);
			SetButtonTextPortrait(base->m_portrait);
			DWORD picsize = 0;
			LPBYTE pic = base->get_pic(&picsize);
			if (picsize > 0 && pic)
			{
				SetButtonPicture(pic, picsize);
			}
		}
		else {
			m_hWnd = CreateWindowExW(0, WC_BUTTON, L"��ťW", dwStyle, 0, 0, cx, cy, m_hParentWnd, (HMENU)nId, GetModuleHandle(0), 0);
			strcpy_s(m_Fontdata.m_data.lfFaceName, "SimSun");
			m_Fontdata.m_data.lfHeight = 13;
			m_Fontdata.m_hFont = CreateFontIndirectA(&m_Fontdata.m_data);
			SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)m_Fontdata.m_hFont, 1);
			//DeleteObject(temp_hFont);
		}


		//���໯����
		SetWindowLongPtrW(m_hParentWnd, GWLP_USERDATA, (LONG_PTR)this);
		//��¼ԭʼ�ص�
		m_oldProc = (WNDPROC)SetWindowLongW(m_hParentWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

		//���໯���
		SetWindowLongPtrW(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
		//��¼ԭʼ�ص�
		m_ColdProc = (WNDPROC)SetWindowLongW(m_hWnd, GWLP_WNDPROC, (LONG_PTR)WndCProc);

	}
	EBUTTONDATA GetBaseData() const {
		EBUTTONDATA temp;
		temp.m_font_data = m_Fontdata;
		temp.m_pic_lenth = m_picdata.size();
		temp.m_portrait = m_portrait;
		temp.m_transverse = m_transverse;
		return temp;
	}
	WNDPROC GetOldProc() const {
		if (!m_hParentWnd)
		{
			return 0;
		}
		if (m_oldProc <= 0)
		{
			return 0;
		}
		return m_oldProc;
	}
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
	HWND GetChild()const {
		if (!m_hParentWnd)
		{
			return 0;
		}
		return m_hWnd;
	}
	BOOL isDesign()const {
		return m_blInDesignMode;
	}
	BOOL SetPos(int width, int height) {//�������ı�ʱ����ô˺���
		return SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	}
	HWND GetHwnd()const {
		return m_hParentWnd;
	}
	void OnClick() {
		EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 0);
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
	}
	void OnKEYDOWN_UP(UINT MSG, WPARAM wparam, int mod) {

		EVENT_NOTIFY2 event(m_dwWinFormID, m_dwUnitID, 247 - MSG);
		event.m_nArgCount = 2;
		event.m_arg[0].m_inf.m_int = wparam;
		event.m_arg[1].m_inf.m_int = mod;
		elibstl::NotifySys(NRS_EVENT_NOTIFY2, (DWORD) & event, 0);
	}

	//�ڲ�ʹ�õ�
	void SetText(const wchar_t* text)
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}

		SetWindowTextW(m_hWnd, text);
	}
	void SetText(const char* text)
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}

		SetWindowTextA(m_hWnd, text);
	}
	std::wstring GetText() const
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return std::wstring();
		}
		int length = GetWindowTextLengthW(m_hWnd);
		std::vector<wchar_t> buffer(length + 1);
		GetWindowTextW(m_hWnd, buffer.data(), buffer.size());
		return std::wstring(buffer.data());
	}
	//����ʹ�õ�
	void SetButtonTextTransverse(INT transverse)
	{
		if (transverse >= 0 && transverse <= 2)
		{
			m_transverse = transverse;
			DWORD style = transverse == 0 ? BS_LEFT : transverse == 1 ? BS_CENTER : BS_RIGHT;
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, (GetWindowLongPtrW(m_hWnd, GWL_STYLE) & ~(BS_LEFT | BS_CENTER | BS_RIGHT)) | style);
		}

	}
	INT GetButtonTextTransverse() {
		return m_transverse;

	}
	INT GetButtonTextPortrait() {
		return m_portrait;

	}
	void SetButtonTextPortrait(INT portrait)
	{
		if (portrait >= 0 && portrait <= 2)
		{
			m_portrait = portrait;
			DWORD style = portrait == 0 ? BS_TOP : portrait == 1 ? BS_VCENTER : BS_BOTTOM;
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, (GetWindowLongPtrW(m_hWnd, GWLP_STYLE) & ~(BS_TOP | BS_VCENTER | BS_BOTTOM)) | style);
		}
	}
	char* GetTextAtoE()
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		if (m_TextA)
		{
			delete[]m_TextA;
			m_TextA = NULL;
		}
		int length = GetWindowTextLengthA(m_hWnd);
		m_TextA = new char[length + 1]{ 0 };
		GetWindowTextA(m_hWnd, m_TextA, length);
		return m_TextA;
	}
	wchar_t* GetTextWtoE()
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		if (m_TextA)
		{
			delete[]m_TextA;
			m_TextA = NULL;
		}
		int length = GetWindowTextLengthW(m_hWnd);
		m_TextW = new wchar_t[length + 1]{ 0 };
		GetWindowTextW(m_hWnd, m_TextW, length);
		return m_TextW;
	}
	~eButtonEx() {
		//�ͷ���Դ
		delete_data();
		//���ٰ�ť
		destroy_button();
		//����WNDPROC
		if (m_hParentWnd)
		{
			::SetWindowLongW(m_hParentWnd, GWLP_WNDPROC, (LONG_PTR)m_oldProc);
		}
		m_hParentWnd = NULL;
		//�ٱ����١�
	}
	void SetFont(LOGFONTA Font)
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}
		if (m_Fontdata.m_hFont) {//���ھ��������

			DeleteObject(m_Fontdata.m_hFont);
			m_Fontdata.m_hFont = NULL;
		}
		memcpy(&m_Fontdata.m_data, &Font, sizeof(LOGFONTA));
		m_Fontdata.m_hFont = CreateFontIndirectA(&Font);
		SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)m_Fontdata.m_hFont, 1);
	}
	LOGFONTA* GetFont()
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		if (!m_Fontdata.m_hFont) {//���ھ��������

			return 0;
		}
		return &m_Fontdata.m_data;
	}

	void SetButtonPicture(LPBYTE pData, DWORD nSize)
	{
		if (!m_hParentWnd || !m_hWnd)
		{
			return;
		}
		if (!m_picdata.empty())
		{
			m_picdata = {};
		}
		if (m_hBitmap)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}

		if (pData != NULL && nSize != 0)
		{
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) | BS_BITMAP);
			m_hBitmap = elibstl::make_hbit(pData, nSize);
			if (m_hBitmap)
			{
				SendMessageW(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_hBitmap);
				BITMAP bmp;
				GetObject(m_hBitmap, sizeof(BITMAP), &bmp);
				m_picdata.resize(nSize);
				memcpy(m_picdata.data(), pData, nSize);
				/*if (!m_picdata.empty())
				{
					out_pBuffer = new unsigned char[m_picdata.size()];
					memcpy(out_pBuffer, m_picdata.data(), m_picdata.size());
				}*/
			}
		}
		else
		{
			SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetWindowLongPtrW(m_hWnd, GWL_STYLE) & ~BS_BITMAP);
			int length = GetWindowTextLengthW(m_hWnd);
			std::vector<wchar_t> buffer(length + 1);
			GetWindowTextW(m_hWnd, buffer.data(), buffer.size());
			SendMessageW(m_hWnd, WM_SETTEXT, 0, (LPARAM)buffer.data());
		}
	}
	vector<unsigned char>* GetButtonPicture() {
		if (!m_hParentWnd || !m_hWnd)
		{
			return 0;
		}
		if (m_picdata.empty())
		{
			return 0;
		}
		if (!m_hBitmap)
		{
			return 0;
		}
		return &m_picdata;
	}

private:
	void delete_data() {
		if (m_TextA)
		{
			delete[]m_TextA;
			m_TextA = NULL;
		}
		if (m_TextW)
		{
			delete[]m_TextW;
			m_TextW = NULL;
		}
		if (m_Fontdata.m_hFont) {//���崴����������

			DeleteObject(m_Fontdata.m_hFont);
			m_Fontdata.m_hFont = NULL;
		}
		if (m_hBitmap) {//ͼƬ

			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
	}
	void destroy_button() {
		if (m_hWnd)
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

private:
	//��Ҫ���������
	//����
	HBITMAP m_hBitmap;
	vector<unsigned char> m_picdata;
	wchar_t* m_TextW;
	char* m_TextA;
	INT m_transverse, m_portrait;
	EFONTDATA m_Fontdata;
	//����������������ı�ʶ
	DWORD m_dwWinFormID, m_dwUnitID;
	HWND m_hParentWnd,//�������
		m_hWnd;//��ť���
	WNDPROC m_oldProc, m_ColdProc;
	BOOL m_blInDesignMode;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	eButtonEx* pButton = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	if (!pButton)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	WNDPROC oldproc = pButton->GetOldProc();
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
		pButton->SetPos(width, height);
		break;
	}
	case WM_COMMAND:
		//��ť�����¼�
		if (HIWORD(wParam) == BN_CLICKED) {
			if (reinterpret_cast<HWND>(lParam) == pButton->GetChild()) {
				pButton->OnClick();
			}
		}
		break;
	case WM_DESTROY:
		pButton->~eButtonEx();
		break;
	default:
		break;
	}
	return oldproc(hWnd, message, wParam, lParam);
};

static LRESULT CALLBACK WndCProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	eButtonEx* pButton = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	if (!pButton)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	WNDPROC oldproc = pButton->GetCOldProc();
	if (oldproc == NULL)
	{
		oldproc = DefWindowProc;
	}

	bool is_continue = SendToParentsHwnd(pButton->GetID(), pButton->GetUID(), uMsg, wParam, lParam);
	if (!is_continue)
	{
		return 0;
	}
	return oldproc(hWnd, uMsg, wParam, lParam);
};



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
	eButtonEx* Button = new eButtonEx(pAllPropertyData, nAllPropertyDataSize, dwStyle, x, y, cx, cy, hParentWnd, uID, blInDesignMode, dwWinFormID, dwUnitID);
	return elibstl::make_cwnd(Button->GetHwnd());
}


static BOOL WINAPI Change(HUNIT hUnit, INT nPropertyIndex,  // ���޸ĵ���������
	UNIT_PROPERTY_VALUE* pPropertyVaule, // �����޸ĵ���Ӧ��������
	LPTSTR* ppszTipText) {  //Ŀǰ��δʹ��
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eButtonEx* Button = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);;

	switch (nPropertyIndex)
	{
	case 0: {
		LPBYTE pData = pPropertyVaule->m_data.m_pData;
		DWORD nSize = pPropertyVaule->m_data.m_nDataSize;
		Button->SetButtonPicture(pData, nSize);
		break;
	}
	case 1:
		Button->SetText(pPropertyVaule->m_szText);
		break;
	case 2:
		Button->SetButtonTextTransverse(pPropertyVaule->m_int);
		break;
	case 3:
		Button->SetButtonTextPortrait(pPropertyVaule->m_int);
		break;
	case 4: {
		LOGFONTA* Font = (LOGFONTA*)pPropertyVaule->m_int;
		Button->SetFont(*Font);
	}
		  break;
	case 5:
	{
		if (pPropertyVaule->m_data.m_pData && pPropertyVaule->m_data.m_nDataSize)
		{
			std::vector<unsigned char> wcs(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_pData + pPropertyVaule->m_data.m_nDataSize);
			std::wstring wstr;
			wstr.append((wchar_t*)wcs.data(), wcs.size() / sizeof(wchar_t));
			Button->SetText(wstr.c_str());
		}
		return FALSE;
	}
	default:
		break;
	}
	return 1;
}

static HGLOBAL WINAPI GetAlldata(HUNIT hUnit)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);

	eButtonEx* Button = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);;
	//��ȡ��������
	EBUTTONDATA temp = Button->GetBaseData();
	//��ȡ����
	wstring temp_title = Button->GetText();
	//��ȡͼƬ����
	vector<unsigned char>* temp_pic = nullptr;
	if (temp.m_pic_lenth > 0)
	{
		temp_pic = Button->GetButtonPicture();
	}
	//�ٴ���֤
	if (!temp_pic)
	{
		temp.m_pic_lenth = 0;
	}
	//��������+ͼƬ����+�ı�����
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(EBUTTONDATA) + temp.m_pic_lenth + temp_title.size() * sizeof(wchar_t));
	if (hGlobal)
	{
		PVOID pGlobal = ::GlobalLock(hGlobal);
		if (pGlobal)
		{
			memcpy(pGlobal, &temp, sizeof(EBUTTONDATA));
			if (temp.m_pic_lenth != 0 && temp_pic)
			{
				memcpy((LPBYTE)pGlobal + sizeof(EBUTTONDATA), temp_pic->data(), temp_pic->size());
			}
			if (temp_title.size() > 0)
			{
				memcpy((LPBYTE)pGlobal + sizeof(EBUTTONDATA) + temp.m_pic_lenth, temp_title.c_str(), temp_title.size() * sizeof(wchar_t));
			}
			::GlobalUnlock(hGlobal);
		}
	}
	return hGlobal;
}

static BOOL WINAPI GetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eButtonEx* Button = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);;
	if (!Button)
	{
		return 0;
	}
	switch (nPropertyIndex)
	{
	case 0:
	{
		vector<unsigned char>* data = Button->GetButtonPicture();
		if (data && data->size() != 0)
		{
			pPropertyVaule->m_data.m_nDataSize = data->size();
			pPropertyVaule->m_data.m_pData = data->data();
		}
		return FALSE;
	}
	case 1:
	{
		pPropertyVaule->m_szText = Button->GetTextAtoE();
		return FALSE;
	}
	case 2:
	{
		pPropertyVaule->m_int = Button->GetButtonTextTransverse();
		return FALSE;
	}
	case 3:
	{
		pPropertyVaule->m_int = Button->GetButtonTextPortrait();
		return FALSE;
	}
	case 4:
	{
		LOGFONTA* Font = Button->GetFont();
		if (Font)
		{
			pPropertyVaule->m_data.m_pData = (LPBYTE)Font;
			pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA);
		}
		return FALSE;
	}
	case 5:
	{
		wchar_t* wText = Button->GetTextWtoE();
		if (wText)
		{
			pPropertyVaule->m_data.m_pData = (LPBYTE)wText;
			pPropertyVaule->m_data.m_nDataSize = wcslen(wText);
		}
		return FALSE;
	}
	default:
		return false;
	}
	return true;
}
static BOOL WINAPI InputW(HUNIT hUnit, INT nPropertyIndex,
	BOOL* pblModified, LPVOID pResultExtraData)
{
	HWND hWnd = elibstl::get_hwnd_from_hunit(hUnit);
	eButtonEx* Button = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);;
	if (nPropertyIndex == 5)
	{
		Button->SetText(MyInputBox(Button->GetText()).c_str());
		*pblModified = true;
	}
	return *pblModified;
}

EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_ButtonW(INT nInterfaceNO)
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
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
	{
		return (PFN_INTERFACE)InputW;
	}
	default:
		return NULL;
	}
	return NULL;
}

static EVENT_INFO2 s_event[] =
{
	/*000*/ {"��ť������", "����ť������ʱ����", _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
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
	/*000*/ {"ͼƬ", "PIC", "", UD_PIC, _PROP_OS(__OS_WIN), NULL},
	/*002*/ {"����", "Titile", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_TEXT, _PROP_OS(__OS_WIN), NULL},
	/*003*/ {"������뷽ʽ", "", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "���\0""����\0""�ұ�\0""\0"},
	/*004*/ {"������뷽ʽ", "", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ϱ�\0""����\0""�±�\0""\0"},
	/*005*/ {"����", "Font", "����������ʱ�ṩ������ĵ�ǰ�����ı�", UD_FONT, _PROP_OS(__OS_WIN) , NULL},
	/*000*/ {"����W", "PIC", "��ť����������ʱ֧�����259���ַ����������й�����û������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
};

static INT s_cmd[] = { 120 };
namespace elibstl {


	LIB_DATA_TYPE_INFO buttonw = {
		"��ťW",//��������
		"ButtonW",//Ӣ������
		"unicode��ť",//˵��
		sizeof(s_cmd) / sizeof(INT),//��������
		s_cmd,//��ȫ�ֺ����ж�Ӧ������
		_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
		103,//��ԴID
		sizeof(s_event) / sizeof(s_event[0]),
		s_event,
		sizeof(s_member) / sizeof(s_member[0]),//������
		s_member,//����ָ��
		libstl_GetInterface_ButtonW,//��������ӳ���
		NULL,//��Ա����
		NULL//��Ա��������
	};



}
EXTERN_C void Fn_EButtonW_GetHwnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	eButtonEx* Button = (eButtonEx*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);

	pRetData->m_int = reinterpret_cast<INT>(Button->GetChild());
}


FucInfo EButtonW_GetHwnd = { {
		/*ccname*/  ("ȡ������"),
		/*egname*/  ("GetHwnd"),
		/*explain*/ ("ȡ���ھ����ȡ��Ϊ�������,������Ϊȡ������ľ����"),
		/*category*/-1,
		/*state*/   NULL,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} , Fn_EButtonW_GetHwnd ,"Fn_EButtonW_GetHwnd" };