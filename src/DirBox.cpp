#include "EcontrolHelp.h"

#define SCID_DIRBOX			20230527'01u
#define SCID_DIRBOXPARENT	20230527'02u

ESTL_NAMESPACE_BEGIN
// Ŀ¼��
/*
* �汾1���ݲ��֣�
* EDIRBOXDATA�ṹ
*/
#define DATA_VER_DIRBOX_1	1
struct EDIRBOXDATA
{
	int iVer;				// �汾��
	DWORD dwReserved;		// ����

	COLORREF crBKClr;		// ������ɫ
	int cchDir;				// Ŀ¼�ַ�����ȫ��ά����ֵ
	BITBOOL bHasButton : 1;			// �Ӽ���
	BITBOOL bHasLine : 1;			// ����
	BITBOOL bFullRowSel : 1;		// ����ѡ��
	BITBOOL bFadeInOutExpandos : 1;	// ���뵭���Ӽ���
	BITBOOL bCheckBox : 1;			// ��ѡ��
	BITBOOL bFile : 1;				// ��ʾ�ļ�
	BITBOOL bNoCache : 1;			// ��ֹ��������
};

#define DBITEMFLAG_HASCHILDPATH 1
#define DBITEMFLAG_ISHIDEITEM 1

class CDirBox :public elibstl::CCtrlBase
{
	SUBCLASS_MGR_DECL(CDirBox)
private:
	EDIRBOXDATA m_Info{};

	PWSTR m_pszDir = NULL;
	IImageList* m_pIImageList = NULL;
public:
	std::wstring m_sCurrPath{};

	static LRESULT CALLBACK ParentSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		switch (uMsg)
		{
		case WM_NOTIFY:
		{
			if (!m_CtrlSCInfo.count(((NMHDR*)lParam)->hwndFrom))
				break;
			auto p = m_CtrlSCInfo[((NMHDR*)lParam)->hwndFrom];
			auto pnmtv = (NMTREEVIEWW*)lParam;
			HWND hTV = ((NMHDR*)lParam)->hwndFrom;
			TVITEMEXW tvi;
			switch (((NMHDR*)lParam)->code)
			{
			case TVN_ITEMEXPANDEDA:// һ��Ҫ��A��
			{
				if (pnmtv->action == TVE_EXPAND)
				{
					p->SetRedraw(FALSE);
					std::wstring sPath{};
					WCHAR szBuf[MAX_PATH];
					int cch;
					HTREEITEM hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)pnmtv->itemNew.hItem);
					if (!hItem)
						break;
					tvi.mask = TVIF_PARAM | TVIF_TEXT;
					tvi.hItem = hItem;
					tvi.cchTextMax = MAX_PATH - 1;
					tvi.pszText = szBuf;
					SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)&tvi);

					if (tvi.lParam == DBITEMFLAG_HASCHILDPATH)
					{
						tvi.mask = TVIF_TEXT;
						tvi.cchTextMax = MAX_PATH - 1;
						tvi.pszText = szBuf;
						tvi.hItem = pnmtv->itemNew.hItem;
						SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)&tvi);
						sPath = tvi.pszText;
						SendMessageW(hTV, TVM_DELETEITEM, 0, (LPARAM)hItem);
						hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)pnmtv->itemNew.hItem);
						while (hItem)
						{
							tvi.hItem = hItem;
							tvi.cchTextMax = MAX_PATH - 1;
							SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)&tvi);
							cch = wcslen(tvi.pszText);

							if (*(tvi.pszText + cch - 1) == L'\\')
								sPath = std::wstring(tvi.pszText) + sPath;
							else
								sPath = std::wstring(tvi.pszText) + L"\\" + sPath;
							hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
						}
						p->EnumFile(sPath.c_str(), pnmtv->itemNew.hItem);
					}
					p->SetRedraw(TRUE);
				}
				else if (pnmtv->action == TVE_COLLAPSE)
				{
					if (!p->m_Info.bNoCache)
						break;
					HTREEITEM hItemTemp;
					HTREEITEM hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)pnmtv->itemNew.hItem);
					if (!hItem)
						break;
					hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
					if (!hItem)
						break;

					p->SetRedraw(FALSE);

					tvi.mask = TVIF_PARAM;
					tvi.hItem = hItem;
					tvi.lParam = DBITEMFLAG_ISHIDEITEM;
					SendMessageW(hTV, TVM_SETITEMW, 0, (LPARAM)&tvi);
					
					hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
					while (hItem)
					{
						hItemTemp = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
						SendMessageW(hTV, TVM_DELETEITEM, 0, (LPARAM)hItem);
						hItem = hItemTemp;
					}
					p->SetRedraw(TRUE);
				}
			}
			break;

			case TVN_SELCHANGEDA:
			{
				std::wstring sPath{};
				WCHAR szBuf[MAX_PATH];
				int cch;
				HTREEITEM hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)pnmtv->itemNew.hItem);
				if (!hItem)
					break;

				tvi.mask = TVIF_TEXT;
				tvi.cchTextMax = MAX_PATH - 1;
				tvi.pszText = szBuf;
				tvi.hItem = pnmtv->itemNew.hItem;
				SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)&tvi);
				sPath = tvi.pszText;
				hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)pnmtv->itemNew.hItem);
				while (hItem)
				{
					tvi.hItem = hItem;
					tvi.cchTextMax = MAX_PATH - 1;
					SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)&tvi);
					cch = wcslen(tvi.pszText);

					if (*(tvi.pszText + cch - 1) == L'\\')
						sPath = std::wstring(tvi.pszText) + sPath;
					else
						sPath = std::wstring(tvi.pszText) + L"\\" + sPath;
					hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
				}

				p->m_sCurrPath = std::move(sPath);
			}
			break;
			}
		}
		break;
		}

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	static LRESULT CALLBACK CtrlSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto p = (CDirBox*)dwRefData;
		switch (uMsg)
		{
		case WM_SHOWWINDOW:
			CHECK_PARENT_CHANGE;
			break;

		case WM_DESTROY:
			m_SM.OnCtrlDestroy(p);
			delete p;
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);

		case WM_NOTIFY:
		{
			switch (((NMHDR*)lParam)->code)
			{
			case TVN_ITEMEXPANDING:
			{
				int a = 0;
			}
			}
		}
		}

		SendToParentsHwnd(p->m_dwWinFormID, p->m_dwUnitID, uMsg, wParam, lParam);
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
public:
	CDirBox() = delete;
	CDirBox(STD_ECTRL_CREATE_ARGS)
	{
		auto cbBaseData = InitBase0(pAllData, cbData, bInDesignMode, dwWinFormID, dwUnitID, nID, hParent);

		if (pAllData)
		{
			BYTE* p = (BYTE*)pAllData+ cbBaseData;
			memcpy(&m_Info, p, sizeof(EDIRBOXDATA));
			p += sizeof(EDIRBOXDATA);
			DupStringForNewDeleteW(m_pszDir, (PCWSTR)p, m_Info.cchDir);
		}
		else
		{
			m_Info0.iFrame = 2;
			m_Info.bHasButton = TRUE;
		}
		m_Info.iVer = DATA_VER_DIRBOX_1;

		DWORD dwTVStyle = 0;
		if (m_Info.bHasButton)
			dwTVStyle |= TVS_HASBUTTONS;
		if (m_Info.bHasLine)
			dwTVStyle |= TVS_HASLINES;
		if (m_Info.bFullRowSel)
			dwTVStyle |= TVS_FULLROWSELECT;
		if (m_Info.bCheckBox)
			dwTVStyle |= TVS_CHECKBOXES;

		m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_TREEVIEWW, NULL, WS_CHILD | WS_CLIPSIBLINGS | dwTVStyle,
			x, y, cx, cy, hParent, (HMENU)nID, NULL, NULL);
		m_SM.OnCtrlCreate(this);

		UINT uExTVStyle = TVS_EX_DOUBLEBUFFER;
		SendMessageW(m_hWnd, TVM_SETEXTENDEDSTYLE, uExTVStyle, uExTVStyle);

		int cxImage, cyImage;
		SHGetImageList(SHIL_SMALL, IID_PPV_ARGS(&m_pIImageList));
		SendMessageW(m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)m_pIImageList);
		m_pIImageList->GetIconSize(&cxImage, &cyImage);
		SendMessageW(m_hWnd, TVM_SETITEMHEIGHT, cyImage + 4, 0);

		SetFadeInOutExpandos(m_Info.bFadeInOutExpandos);
		InitBase0(pAllData);

		FillCtrl();
	}

	HTREEITEM InsertItem(PCWSTR pszText, PWSTR pszNextLevelPath, int cchNextLevelPath,
		HTREEITEM hParentItem, HTREEITEM hItemAfter, BOOL* pbHasChildPath = NULL)
	{
		if (pbHasChildPath)
			*pbHasChildPath = FALSE;
		PWSTR pTemp = pszNextLevelPath + cchNextLevelPath;

		TVINSERTSTRUCTW tis;
		SHFILEINFOW sfi{};
		WIN32_FIND_DATAW wfd;
		HANDLE hFind;
		HTREEITEM hNewItem = NULL;

		if (!SHGetFileInfoW(pszNextLevelPath, 0, &sfi, sizeof(SHFILEINFOW), SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
			return NULL;
		tis.hParent = hParentItem;
		tis.hInsertAfter = hItemAfter;
		tis.itemex.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		tis.itemex.iSelectedImage = tis.itemex.iImage = sfi.iIcon;
		tis.itemex.pszText = (PWSTR)pszText;
		hNewItem = (HTREEITEM)SendMessageW(m_hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tis);

		wcscat(pTemp, L"\\*");
		hFind = FindFirstFileW(pszNextLevelPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (memcmp(wfd.cFileName, L".", 2 * sizeof(WCHAR)) == 0 ||
					memcmp(wfd.cFileName, L"..", 3 * sizeof(WCHAR)) == 0)
					continue;

				tis.hParent = hNewItem;
				tis.hInsertAfter = TVI_FIRST;
				tis.itemex.mask = TVIF_TEXT | TVIF_PARAM;
				tis.itemex.pszText = (PWSTR)L"0";
				tis.itemex.lParam = DBITEMFLAG_ISHIDEITEM;
				SendMessageW(m_hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tis);

				if (pbHasChildPath)
					*pbHasChildPath = TRUE;
				break;
			} while (FindNextFileW(hFind, &wfd));
			FindClose(hFind);
		}

		return hNewItem;
	}

	void EnumFile(PCWSTR pszFile, HTREEITEM hParentItem)
	{
		int cchFile = wcslen(pszFile);
		if (!cchFile)
			return;
		WCHAR pszPath[MAX_PATH];
		wcscpy(pszPath, pszFile);
		PWSTR pszPathTemp = pszPath + cchFile;
		if (*(pszPathTemp - 1) != L'\\')
		{
			wcscpy(pszPathTemp, L"\\*");
			++cchFile;
		}
		else
			wcscpy(pszPathTemp, L"*");

		WCHAR pszNextLevelPath[MAX_PATH];
		PWSTR pszTemp = pszNextLevelPath + cchFile;
		wcscpy(pszNextLevelPath, pszPath);
		*pszTemp = L'\0';

		TVINSERTSTRUCTW tis;
		SHFILEINFOW sfi;
		WIN32_FIND_DATAW wfd;
		HANDLE hFind;
		HTREEITEM hNewItem = NULL, hItemAfterDir = TVI_FIRST, hItemAfterFile = TVI_LAST;

		hFind = FindFirstFileW(pszPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (memcmp(wfd.cFileName, L".", 2 * sizeof(WCHAR)) == 0 ||
					memcmp(wfd.cFileName, L"..", 3 * sizeof(WCHAR)) == 0)
					continue;
				wcscpy(pszTemp, wfd.cFileName);
				if (IsBitExist(wfd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
					hItemAfterDir = InsertItem(wfd.cFileName, pszNextLevelPath, cchFile, hParentItem, hItemAfterDir);
				else if (m_Info.bFile)
					hItemAfterFile = InsertItem(wfd.cFileName, pszNextLevelPath, cchFile, hParentItem, hItemAfterFile);
			} while (FindNextFileW(hFind, &wfd));
			FindClose(hFind);
		}
	}

	void FillCtrl()
	{
		SetRedraw(FALSE);
		SendMessageW(m_hWnd, TVM_DELETEITEM, 0, NULL);
		if (!PathFileExistsW(m_pszDir))
		{
			SetRedraw(TRUE);
			return;
		}

		int cchFile = m_Info.cchDir;
		WCHAR pszPath[MAX_PATH];
		wcscpy(pszPath, m_pszDir);
		PathStripPathW(pszPath);

		WCHAR pszNextLevelPath[MAX_PATH];
		PWSTR pszTemp = pszNextLevelPath + cchFile;
		wcscpy(pszNextLevelPath, m_pszDir);
		if (*(pszTemp - 1) != L'\\')
		{
			wcscpy(pszTemp, L"\\");
			++cchFile;
		}

		BOOL bHasChildPath;
		auto hNewItem = InsertItem(pszPath, pszNextLevelPath, cchFile, TVI_ROOT, TVI_LAST, &bHasChildPath);
		if (bHasChildPath)
		{
			SendMessageW(m_hWnd, TVM_DELETEITEM, 0, SendMessageW(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hNewItem));
			EnumFile(m_pszDir, hNewItem);
		}
		SendMessageW(m_hWnd, TVM_EXPAND, TVE_EXPAND, (LPARAM)hNewItem);
		SetRedraw(TRUE);
	}

	eStlInline void SetDir(PCWSTR pszDir)
	{
		m_Info.cchDir = DupStringForNewDeleteW(m_pszDir, pszDir);
		FillCtrl();
	}

	eStlInline PCWSTR GetDir(int* pcb)
	{
		if (pcb)
			if (m_pszDir)
				*pcb = (m_Info.cchDir + 1) * sizeof(WCHAR);
			else
				*pcb = 0;
		return m_pszDir;
	}

	eStlInline void SetBKClr(COLORREF cr)
	{
		m_Info.crBKClr = cr;
		Redraw();
	}

	eStlInline COLORREF GetBKClr()
	{
		return m_Info.crBKClr;
	}

	eStlInline void SetHasButton(BOOL bHasButton)
	{
		m_Info.bHasButton = bHasButton;
		ModifyWindowStyle(m_hWnd, bHasButton ? TVS_HASBUTTONS : 0, TVS_HASBUTTONS);
	}

	eStlInline BOOL GetHasButton()
	{
		if (m_bInDesignMode)
			return m_Info.bHasButton;
		else
			IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), TVS_HASBUTTONS);
	}

	eStlInline	void SetHasLine(BOOL bHasLine)
	{
		m_Info.bHasLine = bHasLine;
		ModifyWindowStyle(m_hWnd, bHasLine ? TVS_HASLINES : 0, TVS_HASLINES);
	}

	eStlInline BOOL GetHasLine()
	{
		if (m_bInDesignMode)
			return m_Info.bHasLine;
		else
			IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), TVS_HASLINES);
	}

	eStlInline void SetFullRowSel(BOOL bFullRowSel)
	{
		m_Info.bFullRowSel = bFullRowSel;
		ModifyWindowStyle(m_hWnd, bFullRowSel ? TVS_FULLROWSELECT : 0, TVS_FULLROWSELECT);
	}

	eStlInline BOOL GetFullRowSel()
	{
		if (m_bInDesignMode)
			return m_Info.bFullRowSel;
		else
			IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), TVS_FULLROWSELECT);
	}

	eStlInline void SetFadeInOutExpandos(BOOL bFadeInOutExpandos)
	{
		m_Info.bFadeInOutExpandos = bFadeInOutExpandos;
		SendMessageW(m_hWnd, TVM_SETEXTENDEDSTYLE, TVS_EX_FADEINOUTEXPANDOS, bFadeInOutExpandos ? TVS_EX_FADEINOUTEXPANDOS : 0);
	}

	eStlInline BOOL GetFadeInOutExpandos()
	{
		if (m_bInDesignMode)
			return m_Info.bFadeInOutExpandos;
		else
			IsBitExist(SendMessageW(m_hWnd, TVM_GETEXTENDEDSTYLE, 0, 0), TVS_EX_FADEINOUTEXPANDOS);
	}

	eStlInline void SetCheckBox(BOOL bCheckBox)
	{
		m_Info.bCheckBox = bCheckBox;
		ModifyWindowStyle(m_hWnd, bCheckBox ? TVS_CHECKBOXES : 0, TVS_CHECKBOXES);
	}

	eStlInline BOOL GetCheckBox()
	{
		if (m_bInDesignMode)
			return m_Info.bCheckBox;
		else
			IsBitExist(GetWindowLongPtrW(m_hWnd, GWL_STYLE), TVS_CHECKBOXES);
	}

	eStlInline void SetFileShowing(BOOL bFileShowing)
	{
		if (m_Info.bFile != bFileShowing)
		{
			m_Info.bFile = bFileShowing;
			FillCtrl();
		}
	}

	eStlInline BOOL GetFileShowing()
	{
		return m_Info.bFile;
	}

	eStlInline void SetNoCache(BOOL bNoCache)
	{
		m_Info.bNoCache = bNoCache;
	}

	eStlInline BOOL GetNoCache()
	{
		return m_Info.bNoCache;
	}

	eStlInline HGLOBAL FlattenInfo() override
	{
		BYTE* p;
		SIZE_T cbBaseData;

		auto hGlobal = FlattenInfoBase0(sizeof(EDIRBOXDATA) + m_Info.cchDir * sizeof(WCHAR), &cbBaseData);
		if (!hGlobal)
			goto Fail;
		p = (BYTE*)GlobalLock(hGlobal);
		if (!p)
			goto Fail;
		// �ṹ
		p += cbBaseData;
		memcpy(p, &m_Info, sizeof(EDIRBOXDATA));
		p += sizeof(EDIRBOXDATA);
		// Ŀ¼
		memcpy(p, m_pszDir, m_Info.cchDir * sizeof(WCHAR));
		// 
		GlobalUnlock(hGlobal);
	Fail:
		return hGlobal;
	}

	static HUNIT WINAPI ECreate(STD_EINTF_CREATE_ARGS)
	{
		auto p = new CDirBox(STD_ECTRL_CREATE_REAL_ARGS);
		return elibstl::make_cwnd(p->GetHWND());
	}

	static BOOL WINAPI EChange(HUNIT hUnit, INT nPropertyIndex, UNIT_PROPERTY_VALUE* pPropertyVaule, LPTSTR* ppszTipText)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));

		switch (nPropertyIndex)
		{
		case 0:// Ŀ¼
			p->SetDir((PCWSTR)pPropertyVaule->m_data.m_pData);
			break;
		case 1:// �߿�
			p->SetFrame(pPropertyVaule->m_int);
			break;
		case 2:// ������ɫ
			p->SetBKClr(pPropertyVaule->m_clr);
			break;
		case 3:// �Ӽ���
			p->SetHasButton(pPropertyVaule->m_bool);
			break;
		case 4:// ����
			p->SetHasLine(pPropertyVaule->m_bool);
			break;
		case 5:// ����ѡ��
			p->SetFullRowSel(pPropertyVaule->m_bool);
			break;
		case 6:// �Ӽ��ŵ��뵭��
			p->SetFadeInOutExpandos(pPropertyVaule->m_bool);
			break;
		case 7:// ��ѡ��
			p->SetCheckBox(pPropertyVaule->m_bool);
			return TRUE;
		case 8:// ��ʾ��ʽ
			p->SetFileShowing(pPropertyVaule->m_bool);
			break;
		case 9:// ����
			p->SetFont((LOGFONTA*)pPropertyVaule->m_data.m_pData);
			break;
		case 10:// ��ֹ��������
			p->SetNoCache(pPropertyVaule->m_bool);
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
		case 0:// Ŀ¼
			pPropertyVaule->m_data.m_pData = (BYTE*)p->GetDir(&pPropertyVaule->m_data.m_nDataSize);
			break;
		case 1:// �߿�
			pPropertyVaule->m_int = p->GetFrame();
			break;
		case 2:// ������ɫ
			pPropertyVaule->m_clr = p->GetBKClr();
			break;
		case 3:// �Ӽ���
			pPropertyVaule->m_bool = p->GetHasButton();
			break;
		case 4:// ����
			pPropertyVaule->m_bool = p->GetHasLine();
			break;
		case 5:// ����ѡ��
			pPropertyVaule->m_bool = p->GetFullRowSel();
			break;
		case 6:// �Ӽ��ŵ��뵭��
			pPropertyVaule->m_bool = p->GetFadeInOutExpandos();
			break;
		case 7:// ��ѡ��
			pPropertyVaule->m_bool = p->GetCheckBox();
			break;
		case 8:// ��ʾ��ʽ
			pPropertyVaule->m_bool = p->GetFileShowing();
			break;
		case 9:// ����
			pPropertyVaule->m_data.m_pData = p->GetFont();
			pPropertyVaule->m_data.m_nDataSize = sizeof(LOGFONTA);
			break;
		case 10:// ��ֹ��������
			pPropertyVaule->m_bool = p->GetNoCache();
			break;
		}

		return TRUE;
	}

	static BOOL WINAPI EInputW(HUNIT hUnit, INT nPropertyIndex, BOOL* pblModified, LPVOID pResultExtraData)
	{
		auto p = m_CtrlSCInfo.at(elibstl::get_hwnd_from_hunit(hUnit));
		*pblModified = FALSE;
		switch (nPropertyIndex)
		{
		case 0:// Ŀ¼
		{
			PWSTR psz;
			if (elibstl::IntputBox(&psz, p->GetDir(NULL)))
			{
				delete[] p->m_pszDir;
				p->m_pszDir = psz;
				if (psz)
					p->m_Info.cchDir = wcslen(psz);
				else
					p->m_Info.cchDir = 0;
				p->FillCtrl();
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
			*((int*)dwParam1) = 200;
			*((int*)dwParam2) = 400;
		}
		return TRUE;
		}
		return FALSE;
	}
};
SUBCLASS_MGR_INIT(CDirBox, SCID_DIRBOXPARENT, SCID_DIRBOX)
ESTL_NAMESPACE_END

EXTERN_C PFN_INTERFACE WINAPI libstl_GetInterface_DirBoxW(INT nInterfaceNO)
{
	switch (nInterfaceNO)
	{
	case ITF_CREATE_UNIT:
		return (PFN_INTERFACE)elibstl::CDirBox::ECreate;
	case ITF_NOTIFY_PROPERTY_CHANGED:
		return (PFN_INTERFACE)elibstl::CDirBox::EChange;
	case ITF_GET_ALL_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CDirBox::EGetAlldata;
	case ITF_GET_PROPERTY_DATA:
		return (PFN_INTERFACE)elibstl::CDirBox::EGetData;
	case ITF_DLG_INIT_CUSTOMIZE_DATA:
		return (PFN_INTERFACE)elibstl::CDirBox::EInputW;
	case ITF_GET_NOTIFY_RECEIVER:
		return (PFN_INTERFACE)elibstl::CDirBox::ENotify;
	}
	return NULL;
}

static EVENT_INFO2 s_Event_DirBox[] =
{
	/*000*/ {"��ǰѡ����ı�", NULL, _EVENT_OS(OS_ALL) | EV_IS_VER2, 0, 0, _SDT_NULL},
};
static UNIT_PROPERTY s_Member_DirBox[] =
{
	FIXED_WIN_UNIT_PROPERTY,
	//1=������, 2=Ӣ��������, 3=���Խ���, 4=���Ե���������UD_,5=���Եı�־, 6=˳���¼���еı�ѡ�ı�UW_(����UD_FILE_NAME), ��һ���մ�����

	/*000*/  {"Ŀ¼", "Dir", "", UD_CUSTOMIZE, _PROP_OS(__OS_WIN), NULL},
	/*001*/	 {"�߿�", "Frame", "", UD_PICK_INT, _PROP_OS(__OS_WIN), "�ޱ߿�\0""����ʽ\0""͹��ʽ\0""ǳ����ʽ\0""����ʽ\0""���߱߿�ʽ\0""\0"},
	/*002*/  {"������ɫ", "BKClr", "", UD_COLOR, _PROP_OS(__OS_WIN),  NULL},
	/*003*/  {"�Ӽ���", "HasButton", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*004*/  {"����", "HasLine", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*005*/  {"����ѡ��", "FullRowSel", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*006*/  {"�Ӽ��ŵ��뵭��", "FadeInOutExpandos", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*007*/  {"��ѡ��", "CheckBox", "", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
	/*008*/	 {"�����ļ�", "ShowFile", "", UD_BOOL, _PROP_OS(__OS_WIN), NULL},
	/*009*/	 {"����", "Font", "", UD_FONT, _PROP_OS(__OS_WIN), NULL},
	/*010*/  {"��ֹ��������", "DisableContentCache", "������Ϊ��ʱÿ������չ��Ŀ¼�������»�ȡ��Ŀ¼", UD_BOOL, _PROP_OS(__OS_WIN),  NULL},
};


EXTERN_C void libstl_DirBox_GetCurrentItem(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	auto p = elibstl::CDirBox::m_CtrlSCInfo[hWnd];

	int cch = p->m_sCurrPath.size();
	pRetData->m_pBin = elibstl::malloc_wstring(cch);
	wcsncpy((PWSTR)(pRetData->m_pBin + 8), p->m_sCurrPath.c_str(), cch);
	*((PWSTR)(pRetData->m_pBin + 8) + cch) = L'\0';
}
FucInfo Fn_DirBoxGetCurrentItem = { {
		/*ccname*/  "ȡ��ǰѡ����",
		/*egname*/  "GetCurrentItem",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} , libstl_DirBox_GetCurrentItem ,"libstl_DirBox_GetCurrentItem" };

EXTERN_C void libstl_DirBox_Extend(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);

	PCWSTR pszPath = elibstl::args_to_pszw(pArgInf, 1);
	std::wstring_view svPath(pszPath), svSubStr;
	TVITEMEXW tvi;
	WCHAR szBuf[MAX_PATH];

	HTREEITEM hTopItem = (HTREEITEM)SendMessageW(hWnd, TVM_GETNEXTITEM, TVGN_ROOT, NULL);
	HTREEITEM hItem;
	BOOL bExpand;
	size_t uPos = svPath.find(L"\\");
	size_t uOldPos = 0u;
	SendMessageW(hWnd, WM_SETREDRAW, FALSE, 0);
	while (uPos != std::wstring_view::npos && hTopItem)
	{
		svSubStr = svPath.substr(uOldPos, uPos - uOldPos + 1);
		hItem = hTopItem;
		bExpand = FALSE;
		do
		{
			tvi.mask = TVIF_TEXT;
			tvi.cchTextMax = MAX_PATH - 1;
			tvi.pszText = szBuf;
			tvi.hItem = hItem;
			SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)&tvi);
			if (svSubStr == szBuf)
			{
				bExpand = TRUE;
				SendMessageW(hWnd, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItem);
				break;
			}
			else
			{
				hItem = (HTREEITEM)SendMessageW(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
			}
		} while (hItem);

		if (!bExpand)
			break;
		uOldPos = uPos + 1;
		hTopItem = (HTREEITEM)SendMessageW(hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
		uPos = svPath.find(L"\\", uOldPos);
	}
	SendMessageW(hWnd, WM_SETREDRAW, TRUE, 0);
}
static ARG_INFO s_ArgsExtend[] =
{
	{ "�ļ���Ŀ¼","",0,0,SDT_BIN,0,NULL },
};
FucInfo Fn_DirBoxExtend = { {
		/*ccname*/  "չ����",
		/*egname*/  "Extend",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} , libstl_DirBox_Extend ,"libstl_DirBox_Extend" };

void GetCheckedItemsHelper(HWND hTV, std::vector<BYTE*>& aText, HTREEITEM hParentItem, TVITEMEXW* ptvi)
{
	HTREEITEM hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hParentItem);
	BYTE* pMem;
	while (hItem)
	{
		if ((SendMessageW(hTV, TVM_GETITEMSTATE, (WPARAM)hItem, TVIS_STATEIMAGEMASK) >> 12) == 2)
		{
			ptvi->hItem = hItem;
			ptvi->cchTextMax = MAX_PATH - 1;
			SendMessageW(hTV, TVM_GETITEMW, 0, (LPARAM)ptvi);
			pMem = elibstl::malloc_wstring(ptvi->cchTextMax);
			wcscpy((PWSTR)(pMem + 8), ptvi->pszText);
			aText.push_back(pMem);
		}
		GetCheckedItemsHelper(hTV, aText, hItem, ptvi);
		hItem = (HTREEITEM)SendMessageW(hTV, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
	}
}
EXTERN_C void libstl_DirBox_GetCheckedItems(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);

	WCHAR szBuf[MAX_PATH];
	TVITEMEXW tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = szBuf;
	std::vector<BYTE*> aText{};

	GetCheckedItemsHelper(hWnd, aText, (HTREEITEM)SendMessageW(hWnd, TVM_GETNEXTITEM, TVGN_ROOT, NULL), &tvi);
	size_t c = aText.size();
	if (!c)
	{
		pRetData->m_pAryData = NULL;
		return;
	}
	pRetData->m_pAryData = elibstl::malloc_array<BYTE*>(c);
	memcpy((BYTE*)pRetData->m_pAryData + 8, aText.data(), c * sizeof(BYTE*));
}
FucInfo Fn_DirBoxGetCheckedItems = { {
		/*ccname*/  "ȡ���м����Ŀ",
		/*egname*/  "GetCheckedItems",
		/*explain*/ "",
		/*category*/-1,                
		/*state*/   CT_RETRUN_ARY_TYPE_DATA,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} , libstl_DirBox_GetCheckedItems ,"libstl_DirBox_GetCheckedItems" };

EXTERN_C void libstl_DirBox_Refresh(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	HWND hWnd = elibstl::get_hwnd_from_arg(pArgInf);
	auto p = elibstl::CDirBox::m_CtrlSCInfo[hWnd];

	p->FillCtrl();
}
FucInfo Fn_DirBoxRefresh = { {
		/*ccname*/  "ˢ��",
		/*egname*/  "Refresh",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} , libstl_DirBox_Refresh ,"libstl_DirBox_Refresh" };

static INT s_Cmd_DirBox[] = { 270,271,272,273 };
ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO CtDirBoxW = {
	"Ŀ¼��W",//��������
	"DirBoxW",//Ӣ������
	"",//˵��
	ARRAYSIZE(s_Cmd_DirBox),//��������
	s_Cmd_DirBox,//��ȫ�ֺ����ж�Ӧ������
	_DT_OS(__OS_WIN) | LDT_WIN_UNIT,//��־
	104,//��ԴID
	ARRAYSIZE(s_Event_DirBox),
	s_Event_DirBox,
	ARRAYSIZE(s_Member_DirBox),//������
	s_Member_DirBox,//����ָ��
	libstl_GetInterface_DirBoxW,//��������ӳ���
	NULL,//��Ա����
	NULL//��Ա��������
};
ESTL_NAMESPACE_END