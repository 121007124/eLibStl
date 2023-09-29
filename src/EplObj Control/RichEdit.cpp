#include "EcontrolHelp.h"
#include<Richedit.h>
#pragma comment (lib,"Crypt32.lib")

#define SCID_RICHEDIT		20230527'01u
#define SCID_RICHEDITPARENT	20230527'02u
#define DATA_VER_RICHEDIT_1	1
struct ERICHEDITDATA
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	int Mod;				//����ģʽ
	COLORREF crBK;			// �༭�򱳾�ɫ
	BOOL bMultiLine;		// �Ƿ��������
	BOOL bAutoWrap;			// �Զ�����
	BOOL bOnlyRead;			//�Ƿ�ֻ��
	BOOL bHideSel;          //����ѡ��
	BOOL iScrollBar;		// ������
	BOOL bAutoDetectURL;	// �Զ��������
	INT RevocationTimes;    //��������
	INT iMaxLen;			// ���������
	INT iSelPos;			// ��ʼѡ��λ��
	INT iSelNum;			// ��ѡ���ַ���
};
ESTL_NAMESPACE_BEGIN
class CRichEdit :public CCtrlBase
{
	SUBCLASS_MGR_DECL(CRichEdit)
public:
	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto p = new CRichEdit(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(p->GetHWND());
	}
	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0://���ݸ�ʽ
			if (pPropertyVaule->m_int < 0 || pPropertyVaule->m_int>1)
			{
				TaskDialog(GetActiveWindow(), NULL, L"����", L"�����������ͣ������������", L"����ɵ����", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
				return FALSE;
			}
			p->m_Info.Mod = pPropertyVaule->m_int;
			break;
		case 1:// ����
			p->SetTextA(pPropertyVaule->m_szText);
			break;
		case 2:// ����W
			switch (p->m_Info.Mod)
			{
			case 1: {
				/*RTF����*/
				p->SetEditData(pPropertyVaule->m_data.m_pData, pPropertyVaule->m_data.m_nDataSize);
				break;
			}
			default:
				p->SetTextW((PCWSTR)pPropertyVaule->m_data.m_pData);
				break;
			}
			break;
		case 3:// �߿�
			p->SetFrame(pPropertyVaule->m_int);
			break;
		case 4:// �༭�򱳾���ɫ
			p->SetCorlor(pPropertyVaule->m_clr);
			break;
		case 5:// �Ƿ��������
			p->SetMultiLine(pPropertyVaule->m_bool);
			return TRUE;
		case 6:// �Զ�����
			return p->SetAutoWrap(pPropertyVaule->m_bool);
		case 7://ֻ��
			p->SetOnlyRead(pPropertyVaule->m_bool);
			break;
		case 8:// ����ѡ��
			p->SetHideSel(pPropertyVaule->m_bool);
			break;
		case 9:// ������
			p->SetScrollBar(pPropertyVaule->m_int);
			break;
		case 10:// �Զ��������
			p->SetAutoDetectURL(pPropertyVaule->m_bool);
			return TRUE;/*�ػ�*/
			break;
		case 11:// ��������
			p->SetUndoLimit(pPropertyVaule->m_int);
			break;

		case 12:// ���������
			p->SetMaxLen(pPropertyVaule->m_int);
			break;
		case 13:// ��ʼѡ��λ��
			p->SetSelPos(pPropertyVaule->m_int);
			break;
		case 14:// ��ѡ���ַ���
			p->SetSelNum(pPropertyVaule->m_int);
			break;
		case 15:// ��ѡ���ı�
			p->SetSelText((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;
		case 16://�Ƿ��Ѹ���
			p->SetModify(pPropertyVaule->m_bool);
			break;
		case 17://��������
		case 18://�����С
		case 19://�ı���ɫ
		case 20://�ı���ɫ
		case 21://����
		case 22://б��
		case 23://�»���
		case 24://ɾ����
		case 25://�ϱ�
		case 26://�±�
		case 27://����ƫ��
		case 28://�����
		case 29://�ַ���
		{
			CHARFORMAT2W cf{ 0 };
			p->GetSelectionCharacterFormat(cf);
			switch (nPropertyIndex)
			{
			case 17://��������
				p->SetSelectionCharacterFormat(cf, (PCWSTR)pPropertyVaule->m_data.m_pData);
				return FALSE;
			case 18://�����С
				if (pPropertyVaule->m_int <= 0)
					TaskDialog(GetActiveWindow(), NULL, L"����", L"�����С����С��0��", L"����ɵ����", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
				else
					cf.yHeight = pPropertyVaule->m_int;
				break;
			case 19://�ı���ɫ
				if (pPropertyVaule->m_int != CLR_INVALID)
				{
					cf.crTextColor = pPropertyVaule->m_int;
				}
				break;
			case 20://�ı���ɫ
				if (pPropertyVaule->m_int != CLR_INVALID)
					cf.crBackColor = pPropertyVaule->m_int;

				break;
			case 21://����
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_BOLD;
				else
					cf.dwEffects &= ~CFE_BOLD;
				break;
			case 22://б��
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_ITALIC;
				else
					cf.dwEffects &= ~CFE_ITALIC;
				break;
			case 23://�»���
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_UNDERLINE;
				else
					cf.dwEffects &= ~CFE_UNDERLINE;
				break;
			case 24://ɾ����
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_STRIKEOUT;
				else
					cf.dwEffects &= ~CFE_STRIKEOUT;
				break;
			case 25://�ϱ�
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_SUPERSCRIPT;
				else
					cf.dwEffects &= ~CFE_SUPERSCRIPT;

				break;
			case 26://�±�
				if (pPropertyVaule->m_bool)
					cf.dwEffects |= CFE_SUBSCRIPT;
				else
					cf.dwEffects &= ~CFE_SUBSCRIPT;
				break;
			case 27://����ƫ��
				cf.yOffset = pPropertyVaule->m_int;
				break;
			case 28://�����
				cf.bPitchAndFamily = pPropertyVaule->m_int;
				break;
			case 29://�ַ���
				cf.bCharSet = pPropertyVaule->m_int;
				break;
			default:
				break;
			}
			p->SetSelectionCharacterFormat(cf);
		}break;
		case 30://���뷽ʽ
		case 31://��������
		case 32://�ұ�����
		case 33://����������
		case 34://�Ʊ��λ���
		case 35://��ǰ���
		case 36://�κ���
		case 37://�м��
		case 38://�������
		case 39://�м������
		{
			PARAFORMAT2 pf{ 0 };
			p->GetSelectionParagraphFormat(pf);
			switch (nPropertyIndex)
			{
			case 30://���뷽ʽ
				pf.wAlignment = pPropertyVaule->m_int;;
				break;
			case 31://��������
				pf.dxStartIndent = pPropertyVaule->m_int;;
				break;
			case 32://�ұ�����
				pf.dxRightIndent = pPropertyVaule->m_int;;
				break;
			case 33://����������
				pf.dxOffset = pPropertyVaule->m_int;
				break;
			case 34://�Ʊ��λ���
				pf.cTabCount = 1;
				pf.rgxTabs[0] = pPropertyVaule->m_int * 1440; // ���Ʊ��λ���ת��Ϊ twips ��λ
				break;
			case 35://��ǰ���
				pf.dySpaceBefore = pPropertyVaule->m_int;
				break;
			case 36://�κ���
				pf.dySpaceAfter = pPropertyVaule->m_int;
				break;
			case 37://�м��
				pf.dyLineSpacing = pPropertyVaule->m_int;
				break;
			case 38://�������
				if (pPropertyVaule->m_int < 1 || pPropertyVaule->m_int > 6)
				{
					TaskDialog(GetActiveWindow(), NULL, L"����", L"�����������ͣ������������", L"����ɵ����", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
					return FALSE;
				}
				pf.wNumbering = pPropertyVaule->m_int;
				break;
			case 39://�м������
				if (pPropertyVaule->m_int < 0 || pPropertyVaule->m_int > 5)
				{
					TaskDialog(GetActiveWindow(), NULL, L"����", L"�����������ͣ������������", L"����ɵ����", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
					return FALSE;
				}
				pf.bLineSpacingRule = pPropertyVaule->m_int;
				break;
			}
			p->SetSelectionParagraphFormat(pf);
		}break;

		}

		return FALSE;
	}

	static BOOL WINAPI EGetData(HUNIT hUnit, INT nPropertyIndex, PUNIT_PROPERTY_VALUE pPropertyVaule)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0://���ݸ�ʽ
			pPropertyVaule->m_int = p->m_Info.Mod;
			break;
		case 1:// ����
			pPropertyVaule->m_szText = p->GetTextA();
			break;
		case 2:// ����W
			switch (p->m_Info.Mod)
			{
			case 1: {
				if (p->m_bInDesignMode) {/*���ģʽֱ�ӷ����ı�*/
					pPropertyVaule->m_data.m_pData = (BYTE*)p->GetTextW((SIZE_T*)&pPropertyVaule->m_data.m_nDataSize);
				}
				else {
					/*RTF����*/
					pPropertyVaule->m_data.m_pData = p->GetEditData((SIZE_T*)&pPropertyVaule->m_data.m_nDataSize);

				}
				break;
			}
			default:
				pPropertyVaule->m_data.m_pData = (BYTE*)p->GetTextW((SIZE_T*)&pPropertyVaule->m_data.m_nDataSize);
				break;
			}
			break;




			break;
		case 3:// �߿�
			pPropertyVaule->m_int = p->GetFrame();
			break;
		case 4:// �༭�򱳾���ɫ
			pPropertyVaule->m_clr = p->GetCorlor();
			break;
		case 5:// �Ƿ��������
			pPropertyVaule->m_bool = p->GetMultiLine();
			break;
		case 6:// �Զ�����
			pPropertyVaule->m_bool = p->GetAutoWrap();
			break;
		case 7://ֻ��
			pPropertyVaule->m_bool = p->GetOnlyRead();
			break;
		case 8:// ����ѡ��
			pPropertyVaule->m_bool = p->GetHideSel();
			break;
		case 9:// ������
			pPropertyVaule->m_int = p->GetScrollBar();
			break;
		case 10:// �Զ��������
			pPropertyVaule->m_bool = p->GetAutoDetectURL();
			break;
		case 11:// ��������
			pPropertyVaule->m_int = p->GetUndoLimit();
			break;
		case 12:// �Ƿ��������
			pPropertyVaule->m_int = p->GetMaxLen();
			break;
		case 13:// ��ʼѡ��λ��
			pPropertyVaule->m_int = p->GetSelPos();
			break;
		case 14:// ��ѡ���ַ���
			pPropertyVaule->m_int = p->GetSelNum();
			break;
		case 15:// ��ѡ���ı�
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetSelText((SIZE_T*)&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 16://�Ƿ񱻸ı�
			pPropertyVaule->m_bool = p->GetModify();
			break;
		case 17://��������
		case 18://�����С
		case 19://�ı���ɫ
		case 20://�ı���ɫ
		case 21://����
		case 22://б��
		case 23://�»���
		case 24://ɾ����
		case 25://�ϱ�
		case 26://�±�
		case 27://����ƫ��
		case 28://�����
		case 29://�ַ���
		{
			CHARFORMAT2W cf{ 0 };
			p->GetSelectionCharacterFormat(cf);
			switch (nPropertyIndex)
			{
			case 17://��������
				pPropertyVaule->m_data.m_nDataSize = (wcslen(cf.szFaceName) + 1) * sizeof(wchar_t);
				pPropertyVaule->m_data.m_pData = (LPBYTE)cf.szFaceName;
				break;
			case 18://�����С
				pPropertyVaule->m_int = cf.yHeight;
				break;
			case 19://�ı���ɫ
				pPropertyVaule->m_int = cf.crTextColor;
				break;
			case 20://�ı���ɫ
				if (pPropertyVaule->m_int != CLR_INVALID)
					pPropertyVaule->m_int = cf.crBackColor;

				break;
			case 21://����
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_BOLD);
				break;
			case 22://б��
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_ITALIC);
				break;
			case 23://�»���
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_UNDERLINE);
				break;
			case 24://ɾ����
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_STRIKEOUT);
				break;
			case 25://�ϱ�
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_SUPERSCRIPT);
				break;
			case 26://�±�
				pPropertyVaule->m_bool = elibstl::IsBitExist(cf.dwEffects, CFE_SUBSCRIPT);
				break;
			case 27://����ƫ��
				pPropertyVaule->m_int = cf.yOffset;
				break;
			case 28://�����
				pPropertyVaule->m_int = cf.bPitchAndFamily;
				break;
			case 29://�ַ���
				pPropertyVaule->m_int = cf.bCharSet;
				break;
			default:
				break;
			}
		}break;
		case 30://���뷽ʽ
		case 31://��������
		case 32://�ұ�����
		case 33://����������
		case 34://�Ʊ��λ���
		case 35://��ǰ���
		case 36://�κ���
		case 37://�м��
		case 38://�������
		case 39://�м������
		{
			PARAFORMAT2 pf{ 0 };
			p->GetSelectionParagraphFormat(pf);
			switch (nPropertyIndex)
			{
			case 30://���뷽ʽ
				pPropertyVaule->m_int = pf.wAlignment;;
				break;
			case 31://��������
				pPropertyVaule->m_int = pf.dxStartIndent;;
				break;
			case 32://�ұ�����
				pPropertyVaule->m_int = pf.dxRightIndent;;
				break;
			case 33://����������
				pPropertyVaule->m_int = pf.dxOffset;
				break;
			case 34://�Ʊ��λ���
				pPropertyVaule->m_int = pf.rgxTabs[0] / 1440; // ���Ʊ��λ���ת��Ϊ twips ��λ
				break;
			case 35://��ǰ���
				pPropertyVaule->m_int = pf.dySpaceBefore;
				break;
			case 36://�κ���
				pPropertyVaule->m_int = pf.dySpaceAfter;
				break;
			case 37://�м��
				pPropertyVaule->m_int = pf.dyLineSpacing;
				break;
			case 38://�������
				pPropertyVaule->m_int = pf.wNumbering;
				break;
			case 39://�м������
				pPropertyVaule->m_int = pf.bLineSpacingRule;
				break;
			}
		}break;
		case 40:/*�ɷ���*/
			pPropertyVaule->m_bool = p->CanUnDo();
			break;
		case 41:/*�ɷ�����*/
			pPropertyVaule->m_bool = p->CanReDo();
			break;
		case 42:/*�ɷ�ճ��*/
			pPropertyVaule->m_bool = p->CanPaste();
			break;
		}

		return TRUE;
	}
	static HGLOBAL WINAPI EGetAlldata(HUNIT hUnit)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		return p->FlattenInfo();
	}
	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		PWSTR psz;
		*pblModified = FALSE;
		switch (nPropertyIndex)
		{
		case 2:// ����W
		{
			if (elibstl::IntputBox(&psz, p->GetTextW()))
			{
				p->SetTextNoCopyW(psz);
				*pblModified = TRUE;
			}
		}
		break;
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
			*((int*)dwParam2) = 20;
		}
		return TRUE;
		}
		return FALSE;
	}

public:

	/*�ص�����*/
	static LRESULT CALLBACK ParentSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		/*	switch (uMsg)
			{

			default:
				break;
			}*/


		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
	static LRESULT CALLBACK CtrlSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto p = reinterpret_cast <CRichEdit*>(dwRefData);
		elibstl::SendToParentsHwnd(p->m_dwWinFormID, p->m_dwUnitID, uMsg, wParam, lParam);
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

public:
	CRichEdit(STD_ECTRL_CREATE_ARGS)
	{
		static	HMODULE hRichEditDLL = NULL;
		if (!hRichEditDLL)
		{
			hRichEditDLL = LoadLibraryW(L"MSFTEDIT.DLL");
			if (!hRichEditDLL)
			{
				TaskDialog(GetActiveWindow(), NULL, L"����", L"��ϵͳ����֧��!", L"B�Ͼ�ϵͳ��ɶ���ı���", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
				return;
			}
		}

		auto cbBaseData = InitBase0(pAllData, cbData, bInDesignMode, dwWinFormID, dwUnitID, nID, hParent);


		if (pAllData)
		{
			BYTE* p = (BYTE*)pAllData + cbBaseData;
			memcpy(&m_Info, p, sizeof(ERICHEDITDATA));
		}
		else
		{
			m_Info0.iFrame = 1;
			m_Info.RevocationTimes = 1;
			m_Info.bAutoWrap = TRUE;
			m_Info.bMultiLine = TRUE;
			m_Info.bAutoDetectURL = TRUE;
			/*��ɫ*/
			m_Info.crBK = 16777215;
		}
		m_Info.iVer = DATA_VER_RICHEDIT_1;

		DWORD dwEDStyle;
		if (m_Info.bMultiLine)
			dwEDStyle = ES_MULTILINE | ES_AUTOVSCROLL | (m_Info.bAutoWrap ? ES_AUTOHSCROLL : 0);
		else
			dwEDStyle = ES_AUTOHSCROLL;

		dwStyle |= dwEDStyle;
		// ���� RichEdit �ؼ�
		m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, m_pszTextW, dwStyle | WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | EM_PASTESPECIAL,
			x, y, cx, cy, hParent, (HMENU)nID, NULL, NULL);

		if (!m_hWnd)
		{
			TaskDialog(GetActiveWindow(), NULL, L"����", L"��������ʧ��", L"B�Ͼ�ϵͳ��ɶ���ı���", TDCBF_OK_BUTTON, LPCWSTR(65529), 0);
			return;
		}

		m_SM.OnCtrlCreate(this);
		InitBase0(pAllData);
		SetScrollBar(m_Info.iScrollBar);/*������*/
		SetHideSel(m_Info.bHideSel);/*����ѡ��*/
		SetAutoDetectURL(m_Info.bAutoDetectURL);/*������*/
		SetAutoWrap(m_Info.bAutoWrap);/*�Զ�����*/
		SetMultiLine(m_Info.bMultiLine);/*�Ƿ��������*/
		SetMaxLen(m_Info.iMaxLen);/*��󳤶�*/
		SetOnlyRead(m_Info.bOnlyRead);/*ֻ��*/
		SetUndoLimit(m_Info.RevocationTimes);/*���������*/
		SetCorlor(m_Info.crBK);
		FrameChanged();
	}

	~CRichEdit()
	{
		delete[] m_pszSelText;
	}
	/*�Ƿ�ɳ���*/
	eStlInline BOOL CanUnDo() {
		return SendMessageW(m_hWnd, EM_CANUNDO, 0, 0) != 0;
	}
	static 	DWORD CALLBACK EditStreamCallback_OUT(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
	{
		// ������׷�ӵ�������
		std::vector<unsigned char>* pData = reinterpret_cast<std::vector<unsigned char>*>(dwCookie);
		pData->insert(pData->end(), pbBuff, pbBuff + cb);
		*pcb = cb; // ���ض�ȡ���ֽ���
		return 0;
	}
	/*RTF��������*/
	eStlInline LPBYTE GetEditData(SIZE_T* size) {
		m_editData.clear();
		EDITSTREAM es;
		es.dwCookie = reinterpret_cast<DWORD_PTR>(&m_editData); // ��������ַ��Ϊ�û��Զ�������
		es.pfnCallback = &EditStreamCallback_OUT; // �ص�����ָ��
		// ��ȡ���ı������ݲ����浽������
		SendMessageW(m_hWnd, EM_STREAMOUT, SF_RTF | SF_UNICODE, reinterpret_cast<LPARAM>(&es));
		if (m_editData.empty())
		{
			*size = 0;
			return nullptr;
		}
		*size = m_editData.size();
		return m_editData.data();
	}
	static DWORD CALLBACK EditStreamCallback_IN(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
	{
		struct no_copy_vector
		{
			const BYTE* m_pData;
			size_t m_Size;
		};
		// �����ݴ�����д�뵽������
		const BYTE* pData = reinterpret_cast<const BYTE*>(dwCookie);
		size_t dataSize = reinterpret_cast<size_t>(pbBuff);

		if (dataSize > 0)
		{
			size_t bytesToCopy = min(static_cast<size_t>(cb), dataSize);
			memcpy(pbBuff, pData, bytesToCopy);
			*pcb = static_cast<LONG>(bytesToCopy); // ����д����ֽ���
			return 0;
		}
		else
		{
			*pcb = 0; // �����Ѿ�ȫ��д�룬����0��ʾ����
			return 1;
		}
	}


	static DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
	{
		auto pRichEdit = reinterpret_cast<std::vector<unsigned char>*>(dwCookie);
		if (pRichEdit)
		{
			size_t dataSize = pRichEdit->size();
			size_t copySize = std::min<size_t>(dataSize, cb);
			memcpy(pbBuff, pRichEdit->data(), copySize);
			pRichEdit->erase(pRichEdit->begin(), pRichEdit->begin() + copySize);
			*pcb = static_cast<LONG>(copySize);
			return 0;
		}
		return -1;
	}
	void SetEditData(const BYTE* pdata, const size_t size)
	{

		m_editData.resize(size);
		memcpy(m_editData.data(), pdata, size);
		EDITSTREAM es;
		es.dwCookie = reinterpret_cast<DWORD_PTR>(&m_editData);
		es.dwError = 0;
		es.pfnCallback = EditStreamCallback;
		SendMessageW(m_hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"")); // Clear the existing text
		SendMessageW(m_hWnd, EM_STREAMIN, SF_RTF | SF_UNICODE, reinterpret_cast<LPARAM>(&es));
	}

	/*�Ƿ������*/
	eStlInline BOOL CanReDo() {
		return SendMessageW(m_hWnd, EM_CANREDO, 0, 0) != 0;
	}
	/*�ɷ�ճ��*/
	eStlInline BOOL CanPaste() {
		return SendMessageW(m_hWnd, EM_CANPASTE, 0, 0) != 0;
	}
	eStlInline void SetSelText(PCWSTR pszText)
	{
		SendMessageW(m_hWnd, EM_REPLACESEL, TRUE, (LPARAM)pszText);
	}
	BOOL  GetModify() {
		return SendMessageW(m_hWnd, EM_GETMODIFY, 0, 0) != 0;
	};
	void SetModify(BOOL Modify) {
		SendMessageW(m_hWnd, EM_SETMODIFY, Modify, 0);
	}
	PWSTR GetSelText(SIZE_T* pcb = NULL)
	{
		delete[] m_pszSelText;
		m_pszSelText = NULL;
		if (pcb)
			*pcb = 0u;
		int cch = GetWindowTextLengthW(m_hWnd);
		if (!cch)
			return NULL;

		DWORD dwStart, dwEnd;
		SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
		DWORD dwLen;
		if (dwStart > dwEnd)
			dwLen = dwStart - dwEnd;
		else
			dwLen = dwEnd - dwStart;
		if (!dwLen)
			return NULL;
		m_pszSelText = new WCHAR[dwEnd + 1];// UTF-16�Ǳ䳤�ģ����ﰴ˵��Ҫ��һ���������������ɡ���
		GetWindowTextW(m_hWnd, m_pszSelText, (int)dwEnd + 1);
		if (pcb)
			*pcb = (dwEnd + 1) * sizeof(WCHAR);
		return m_pszSelText;
	}
	eStlInline void SetUndoLimit(int limit)
	{
		m_Info.RevocationTimes = limit;
		m_Info.RevocationTimes = SendMessageW(m_hWnd, EM_SETUNDOLIMIT, m_Info.RevocationTimes, 0);
	}
	eStlInline int GetUndoLimit() const
	{
		return m_Info.RevocationTimes;
	}

	void SetScrollBar(int i)
	{
		m_Info.iScrollBar = i;
		switch (i)
		{
		case 0:
			ShowScrollBar(m_hWnd, SB_VERT, FALSE);
			ShowScrollBar(m_hWnd, SB_HORZ, FALSE);
			break;
		case 1:
			ShowScrollBar(m_hWnd, SB_VERT, FALSE);
			ShowScrollBar(m_hWnd, SB_HORZ, TRUE);
			break;
		case 2:
			ShowScrollBar(m_hWnd, SB_VERT, TRUE);
			ShowScrollBar(m_hWnd, SB_HORZ, FALSE);
			break;
		case 3:
			ShowScrollBar(m_hWnd, SB_VERT, TRUE);
			ShowScrollBar(m_hWnd, SB_HORZ, TRUE);
			break;
		}
	}

	int GetScrollBar()
	{
		if (m_bInDesignMode)
			return m_Info.iScrollBar;
		else
		{
			BOOL bVSB = elibstl::IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), WS_VSCROLL);
			BOOL bHSB = elibstl::IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), WS_HSCROLL);
			if (bVSB)
				if (bHSB)
					return 3;
				else
					return 2;
			if (bHSB)
				return 1;

			return 0;
		}
	}
	void SetSelPos(int iSelPos)
	{
		m_Info.iSelPos = iSelPos;
		DWORD dwStart, dwEnd;
		SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
		DWORD dwLen;
		if (dwStart > dwEnd)
			dwLen = dwStart - dwEnd;
		else
			dwLen = dwEnd - dwStart;
		SendMessageW(m_hWnd, EM_SETSEL, iSelPos, iSelPos + dwLen);
	}

	eStlInline int GetSelPos()
	{
		if (m_bInDesignMode)
			return m_Info.iSelPos;
		else
		{
			DWORD dwStart;
			SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, NULL);
			return dwStart;
		}
	}
	eStlInline void SetAutoDetectURL(BOOL bAutoDetect)
	{
		m_Info.bAutoDetectURL = bAutoDetect;
		SendMessageW(m_hWnd, EM_AUTOURLDETECT, m_Info.bAutoDetectURL, NULL);
	}
	eStlInline BOOL GetAutoDetectURL()
	{
		m_Info.bAutoDetectURL = SendMessageW(m_hWnd, EM_GETAUTOURLDETECT, 0, 0);
		return m_Info.bAutoDetectURL;
	}


	eStlInline void SetMaxLen(int iMaxLen)
	{
		m_Info.iMaxLen = iMaxLen;
		SendMessageW(m_hWnd, EM_SETLIMITTEXT, iMaxLen, 0);
	}

	eStlInline int GetMaxLen()
	{
		if (m_bInDesignMode)
			return m_Info.iMaxLen;
		else
			return SendMessageW(m_hWnd, EM_GETLIMITTEXT, 0, 0);
	}

	eStlInline void SetMultiLine(BOOL bMultiLine)
	{
		BOOL bOld = m_Info.bMultiLine;
		m_Info.bMultiLine = bMultiLine;// ����Ҫ�޸ķ����Ϊ�ؼ�Ҫ���´���
	}

	eStlInline BOOL GetMultiLine()
	{
		if (m_bInDesignMode)
			return m_Info.bMultiLine;
		else
			return elibstl::IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), ES_MULTILINE);
	}
	eStlInline void SetHideSel(BOOL bHideSel)
	{
		m_Info.bHideSel = bHideSel;
		elibstl::ModifyWindowStyle(m_hWnd, (bHideSel ? 0 : ES_NOHIDESEL), ES_NOHIDESEL);
	}

	eStlInline BOOL GetHideSel()
	{
		if (m_bInDesignMode)
			return m_Info.bHideSel;
		else
			return !(!!(GetWindowLongPtrW(m_hWnd, GWL_STYLE) & ES_NOHIDESEL));
	}
	BOOL SetAutoWrap(BOOL bAutoWrap)
	{
		m_Info.bAutoWrap = bAutoWrap;
		return GetMultiLine();
	}

	eStlInline BOOL GetAutoWrap()
	{
		return m_Info.bAutoWrap;
	}
	eStlInline void SetCorlor(COLORREF cr)
	{
		m_Info.crBK = cr;
		SendMessageW(m_hWnd, EM_SETBKGNDCOLOR, 0, cr);
		Redraw();
	}
	eStlInline COLORREF GetCorlor() {
		return m_Info.crBK;
	}
	eStlInline void SetSelNum(int iSelNum)
	{
		m_Info.iSelNum = iSelNum;
		DWORD dwStart;
		SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, NULL);
		SendMessageW(m_hWnd, EM_SETSEL, dwStart, dwStart + iSelNum);
	}
	BOOL GetOnlyRead() {
		return m_Info.bOnlyRead;
	}
	void SetOnlyRead(BOOL ReadOnly) {
		m_Info.bOnlyRead = ReadOnly;
		SendMessage(m_hWnd, EM_SETREADONLY, m_Info.bOnlyRead, 0);

	}
	int GetSelNum()
	{
		if (m_bInDesignMode)
			return m_Info.iSelNum;
		else
		{
			DWORD dwStart, dwEnd;
			SendMessageW(m_hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
			DWORD dwLen;
			if (dwStart > dwEnd)
				dwLen = dwStart - dwEnd;
			else
				dwLen = dwEnd - dwStart;
			return (int)dwLen;
		}
	}
	///*׷���ı�*/
	//void AppendText(const wchar_t* text) const
	//{
	//	int length = GetWindowTextLengthW(m_hWnd);
	//	SendMessageW(m_hWnd, EM_SETSEL, length, length);
	//	SendMessageW(m_hWnd, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
	//}
	///*���ͼƬ*/
	//void AppendImage()
	//{
	//	if (!m_isInit || m_hWnd == NULL)
	//		return;
	//	// �ڸ��ı����в���������е�ͼ��
	//	SendMessageW(m_hWnd, WM_PASTE, 0, 0);

	//}


private:

	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;
		auto hGlobal = FlattenInfoBase0(sizeof(ERICHEDITDATA), &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		// �ṹ
		p += cbBaseData;
		memcpy(p, &m_Info, sizeof(ERICHEDITDATA));

		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}
	/*��ȡѡ�������ַ���ʽ*/
	bool GetSelectionCharacterFormat(CHARFORMAT2W& format) const
	{

		format.cbSize = sizeof(CHARFORMAT2W);
		return SendMessageW(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&format)) != 0;
	}

	/*��ѡ�������ַ���ʽ*/
	bool SetSelectionCharacterFormat(const CHARFORMAT2W& format, const std::wstring_view& format_name)   const
	{
		CHARFORMAT2W cf;
		cf.cbSize = sizeof(CHARFORMAT2W);
		cf.dwMask = CFM_FACE | CFM_EFFECTS | CFM_SIZE | CFM_OFFSET | CFM_COLOR | CFM_CHARSET;

		if (format_name.empty())
		{
			auto len = lstrlenW(format.szFaceName);
			if (len > LF_FACESIZE - 1)
			{
				len = LF_FACESIZE - 1;
			}
			wcsncpy_s(cf.szFaceName, format.szFaceName, len);
			cf.szFaceName[len] = L'\0';
		}
		else
		{
			auto len = format_name.size();
			if (len > LF_FACESIZE - 1)
			{
				len = LF_FACESIZE - 1;
			}
			wcsncpy_s(cf.szFaceName, format_name.data(), len);
			cf.szFaceName[len] = L'\0';
		}

		cf.dwEffects = format.dwEffects & ~CFE_AUTOCOLOR;
		cf.yHeight = format.yHeight;
		cf.yOffset = format.yOffset;
		cf.crTextColor = format.crTextColor;
		cf.bCharSet = format.bCharSet;
		cf.bPitchAndFamily = format.bPitchAndFamily;
		return SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&cf)) != 0;
	}
	bool SetSelectionCharacterFormat(const CHARFORMAT2W& format)   const
	{
		CHARFORMAT2W cf;
		cf.cbSize = sizeof(CHARFORMAT2W);
		cf.dwMask = CFM_FACE | CFM_EFFECTS | CFM_SIZE | CFM_OFFSET | CFM_COLOR | CFM_CHARSET;


		auto len = lstrlenW(format.szFaceName);
		if (len > LF_FACESIZE - 1)
		{
			len = LF_FACESIZE - 1;
		}
		wcsncpy_s(cf.szFaceName, format.szFaceName, len);
		cf.szFaceName[len] = L'\0';


		cf.dwEffects = format.dwEffects & ~CFE_AUTOCOLOR;
		cf.yHeight = format.yHeight;
		cf.yOffset = format.yOffset;
		cf.crTextColor = format.crTextColor;
		cf.bCharSet = format.bCharSet;
		cf.bPitchAndFamily = format.bPitchAndFamily;
		return SendMessageW(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&cf)) != 0;
	}
	/*�����ʽ*/
	bool GetSelectionParagraphFormat(PARAFORMAT2& format) const
	{
		format.cbSize = sizeof(PARAFORMAT2);
		return SendMessageW(m_hWnd, EM_GETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&format)) != 0;
	}
	bool SetSelectionParagraphFormat(PARAFORMAT2& format) const
	{

		format.cbSize = sizeof(PARAFORMAT2);
		format.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_RIGHTINDENT | PFM_OFFSET | PFM_ALIGNMENT | PFM_LINESPACING | PFM_SPACEAFTER | PFM_SPACEBEFORE;


		if (format.wAlignment == 0) {
			format.wAlignment = PFA_LEFT;
		}
		return SendMessageW(m_hWnd, EM_SETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&format)) != 0;
	}


	/*��ѡ������*/
	bool SetSelection(int startPosition, int endPosition)   const
	{
		CHARRANGE range;
		range.cpMin = startPosition;
		range.cpMax = endPosition;
		return SendMessageW(m_hWnd, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range)) != 0;
	}
	ERICHEDITDATA m_Info{ 0 };
	std::vector<unsigned char> m_editData;/*����������ܴ�����,�������赣����������ݹ����ĸ�ʽ*/
	PWSTR m_pszSelText = NULL;

};


SUBCLASS_MGR_INIT(CRichEdit, SCID_RICHEDITPARENT, SCID_RICHEDIT)
ESTL_NAMESPACE_END



EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_RichEdit(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
		return (PFN_INTERFACE)elibstl::CRichEdit::ECreate;
	case ITF_NOTIFY_PROPERTY_CHANGED:
		return (PFN_INTERFACE)elibstl::CRichEdit::EChange;
	case ITF_GET_ALL_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CRichEdit::EGetAlldata;
	case ITF_GET_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CRichEdit::EGetData;
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
		return (PFN_INTERFACE)elibstl::CRichEdit::EInputW;
	case ITF_GET_NOTIFY_RECEIVER:
		return (PFN_INTERFACE)elibstl::CRichEdit::ENotify;
	}
	return NULL;
}


ESTL_NAMESPACE_BEGIN


static UNIT_PROPERTY s_Member_RichEdit[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ��
	//�մ�����

	/*0*/{"���ݸ�ʽ","TextFormat","������ָ�����ı�������ݽ��ж�дʱ��ʹ�õ��ı���ʽ�������ʽ����Ͽ������ĵ���",UD_PICK_INT,_PROP_OS(OS_ALL),"���ı�\0""RTF�ḻ�ı�\0""\0"},
	/*1*/{"�ı�","Text","��ansi�ı�",UD_TEXT,_PROP_OS(OS_ALL),NULL},
	/*2*/{"����", "TextW", "��������ʹ�õ��ı���ʽ�ɡ����ݸ�ʽ�����Ծ���,���ı�Ϊ��׼utf16�ַ�,RTF����Ϊ��������", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*3*/{"�߿�","Style","",UD_PICK_INT,_PROP_OS(OS_ALL),"�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
	/*4*/{"������ɫ","BackColor","�ı���ı�����ɫ",UD_COLOR,_PROP_OS(OS_ALL),NULL},
	/*5*/{"�Ƿ��������","AllowMultiLines","�����������ı����Ƿ�������������ı�",UD_BOOL,_PROP_OS(OS_ALL),NULL},
	/*6*/{"�Զ�����","WrapMode","�����������ı����Ƿ�֧���Զ�����",UD_BOOL,_PROP_OS(OS_ALL),NULL},
	/*7*/{"ֻ��","ReadOnly","���������ñ༭���Ƿ�ֻ�ܶ�ȡ������д��",UD_BOOL,_PROP_OS(OS_ALL),NULL},
	/*8*/{"����ѡ��","HideSel","������ָ��ʧȥ���뽹����Ƿ���ʾ��ǰ��ѡ�������",UD_BOOL,_PROP_OS(OS_ALL),NULL},
	/*9*/{"������", "ScrollBar", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "��\0""���������\0""���������\0""�������������\0""\0"},
	/*10*/{"�Զ��������","AutoURLDetect","�Զ�����ĵ����URL���ӣ����磺HTTP:\\��ftp:\\��mailto:��ͷ�Ĺؼ���",UD_BOOL,_PROP_OS(OS_ALL),NULL},
	/*11*/{"�������������","MultileveUndo","���ػ����ÿ��Գ����Ĵ�����ռ��ϵͳ��Դ����������̫��",UD_INT,_PROP_OS(OS_ALL),NULL},
	/*12*/{"���������","MaxAllowLength","��������������������뵽�ı����е��ַ���Ŀ����Ϊ0�������ַ���Ŀ��������",UD_INT,_PROP_OS(OS_ALL),NULL},
	/*13*/{"��ʼѡ��λ��","SelStart","���ػ�������ѡ���ı�����ʼ�㣻0 Ϊλ�� 1 ��1 Ϊλ�� 2��������ơ����û���ı���ѡ�У� ���ṩ��ǰ���λ�á��������λ��ʱʹ��ֵ -1���򽫵�ǰ���λ���ƶ����ı�β��",UD_INT,_PROP_OS(OS_ALL),NULL},
	/*14*/{"��ѡ���ַ���","SelLength","���ػ�������ѡ����ַ�������������ַ���ʱʹ��ֵ -1����ѡ��༭���ڵ������ַ�",UD_INT,_PROP_OS(OS_ALL),NULL},
	/*15*/{"��ѡ���ı�","SelText","���ػ��滻��ǰ��ѡ����ı����ı���ʽ�ɡ����ݸ�ʽ�����Ծ���",UD_CUSTOMIZE,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*16*/{"�Ƿ��Ѹı�","CanUnDo","����ʱ�ɿ��ƣ����ı��򷵻��棬����Ϊ�١�",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*17*/{"��������","FontName","�ڱ༭ʱ�����ú�ȡ�ñ�ѡ���ı����������ơ����������ѡ��,��ȡ�ù��λ�õ���������",UD_CUSTOMIZE,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*18*/{"�����С","FontSize","�ڱ༭ʱ�����ú�ȡ�ñ�ѡ���ı��������С�����������ѡ�У�ȡ��괦��",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*19*/{"�ı���ɫ","ForeColor","�ڱ༭ʱ�����ú�ȡ�ñ�ѡ���ı�����ɫֵ�����������ѡ�У�ȡ��괦��",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*20*/{"�ı���ɫ","FontBackColor","�ڱ༭ʱ�����ú�ȡ�ñ�ѡ���ı��ı�����ɫֵ�����������ѡ�У�ȡ��괦��",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*21*/{"����","Bold","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊ���塣���������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*22*/{"б��","Italic","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊб�塣���������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*23*/{"�»���","Underline","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊ�»��ߡ����������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*24*/{"ɾ����","StrikeThrough","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊɾ���ߡ����������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*25*/{"�ϱ�","Superscript","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊ�ϱꡣ���������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*26*/{"�±�","Subscript","�ڱ༭ʱ����ѡ���ı��Ƿ�Ϊ�±ꡣ���������ѡ��,��Ϊ��",UD_BOOL,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*27*/{"����ƫ��","YOffset","�ڱ༭ʱ�����ú�ȡ�ñ�ѡ���ı��������λ�ã���ֵ���ϱ꣬��ֵ���±꣬��λΪ���ء�������ѡ �У�ȡ��괦��",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*28*/{"������","PitchAndFamily","LOGFONT�ṹ��lfPitchAndFamily��Ա",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*29*/{"�ַ���","CharSet","LOGFONT�ṹ��lfCharSet��Ա",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*30*/{"���뷽ʽ","Alignment","ѡ���Ķ��뷽ʽ������Ϊ���³���ֵ֮һ�� 1��#�ı�����2��#�ı����ң�3��#�ı����С�",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*31*/{"��������","StartIndent","����������ı�����߿������������λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*32*/{"�ұ�����","RightIndent","�����ұ�������ı����ұ߿������������λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*33*/{"����������","Indent","ָ�����������з�������������е�����������λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*34*/{"�Ʊ�λ���","TopStops","TAB�����Ʊ�λ��ȣ���λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*35*/{"��ǰ���","SpaceBefore","����������࣬��λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*36*/{"�κ���","SpaceAfter","����������࣬��λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},
	/*37*/{"�м��","LineSpacing","������֮��ľ��룬��λΪ������",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},


	/*38*/{"�������","LineSpacing","\
	#0.�޶������\
	#1.Բ��\
	#2.����������\
	#3.Сд��ĸ\
	#4.��д��ĸ\
	#5.Сд������ĸ	\
	#6.��д������ĸ\
	",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},


	/*39*/{"�м������","LineSpacingRule","\
	#0.��һ�����м�ࡱ��Ա�����ԣ�\n\
	#1.һ�루���м�ࡱ��Ա�����ԣ� \n\
	#2.���������м�ࡱ��Ա�����ԣ�\n\
	#3.��С�ڵ�һ(���м�ࡱ��Աָ��һ�е���һ�еļ�࣬��λ���(1/1440Ӣ��)�����С�� ��һ ��࣬����Ч)\n\
	#4.��ȷ���ƣ����м�ࡱ��Աָ��һ�е���һ�еļ�࣬��λ���(1/1440Ӣ��)��\n\
	#5.����20(���м�ࡱ����20 Ϊ��һ��࣬40Ϊ˫����60Ϊ����...)	\
	",UD_INT,UW_CANNOT_INIT | _PROP_OS(OS_ALL),NULL},

	/*40*/{"�ɷ���","CanUnDo","������ɳ������ٲ��ɡ�",UD_BOOL,UW_ONLY_READ | _PROP_OS(OS_ALL),NULL},
	/*41*/{"�ɷ�����","CanUnDo","������ɣ��ٲ��ɡ�",UD_BOOL,UW_ONLY_READ | _PROP_OS(OS_ALL),NULL},
	/*42*/{"�ɷ�ճ��","CanUnDo","������ɣ��ٲ��ɡ�",UD_BOOL,UW_ONLY_READ | _PROP_OS(OS_ALL),NULL},

	///*38*/{"����ͼƬ","BackImage","����ͼƬ���ݣ�֧��BMP��JPG��GIF�ȸ�ʽ��ͼƬ",UD_PIC,UW_IS_HIDED | _PROP_OS(OS_ALL),NULL},
	///*39*/{"��ʾ��ʽ","ShowStyle","����趨��ͼƬ��������ָ������ͼƬ����ʾ��ʽ",UD_PICK_INT,UW_HAS_INDENT | UW_IS_HIDED | _PROP_OS(OS_ALL),"ͼƬ����\0""ͼƬ����\0""ͼƬ����\0""ͼƬƽ��\0""\0"},
	///*40*/{"͸����","Alpha","����趨��ͼƬ��������ָ������ͼƬ��͸���ȣ���ɫΪ������ɫ��������,0~255��ֵ��0Ϊȫ͸����255Ϊ ��͸��",UD_INT,UW_HAS_INDENT | UW_IS_HIDED | _PROP_OS(OS_ALL),NULL},
	///*41*/{"����͸��","BgTransparent","���ı���ı�������Ϊ͸�������Կ�������������������Ǵ��ڵĿ����õ�ͼ�����ǻ���� ��ͼƬ���ϵ�ͼƬ����������Ϊ�ı���ı༭��ͼ��",UD_BOOL,_PROP_OS(OS_ALL),NULL}

};

LIB_DATA_TYPE_INFO CtRichEdit = {
	"���ı���",//��������
	"RichEdit",//Ӣ������
	"",//˵��
	0,//��������
	0,//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
	IDB_UPDOWN_EX,//��ԴID
	0,
	0,//�¼�
	ARRAYSIZE(s_Member_RichEdit),//������
	s_Member_RichEdit,//����ָ��
	libstl_GetInterface_RichEdit,//��������ӳ���
	NULL,//��Ա����
	NULL//��Ա��������
};
ESTL_NAMESPACE_END