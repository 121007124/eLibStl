#include "EcontrolHelp.h"

class CMenu
{
public:
	HMENU m_hMenu = NULL;
	BOOL m_bDeleteMenu = FALSE;

	std::vector<HBITMAP> m_HbmNeedDelete{};// �˵�������ʱλͼ���ᱻɾ��������Ҫ����ά��

	CMenu()
	{

	}

	~CMenu()
	{
		if (m_bDeleteMenu)
			DestroyMenu(m_hMenu);
		for (HBITMAP hbm : m_HbmNeedDelete)
			DeleteObject(hbm);
	}
};


EXTERN_C void libstl_MenuEx_Constructor(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& p = elibstl::args_to_obj<CMenu>(pArgInf);
	p = new CMenu;
}
FucInfo Fn_MenuConstructor = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/-1,
		/*state*/   CT_IS_OBJ_CONSTURCT_CMD | CT_IS_HIDED,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,},ESTLFNAME(libstl_MenuEx_Constructor) };

EXTERN_C void libstl_MenuEx_Destructor(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& p = elibstl::args_to_obj<CMenu>(pArgInf);
	if (p)
	{
		p->~CMenu();
		operator delete(p);
	}
	p = NULL;
}
FucInfo Fn_MenuDestructor = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/-1,
		/*state*/   CT_IS_OBJ_FREE_CMD | CT_IS_HIDED,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,},ESTLFNAME(libstl_MenuEx_Destructor) };

EXTERN_C void libstl_MenuEx_Copy(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}
FucInfo Fn_MenuCopy = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/-1,
		/*state*/   CT_IS_OBJ_COPY_CMD | CT_IS_HIDED,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,},ESTLFNAME(libstl_MenuEx_Copy) };

EXTERN_C void libstl_MenuEx_Attach(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	p->m_hMenu = (HMENU)pArgInf[1].m_int;
}
static ARG_INFO s_Args_Attach[] =
{
	{"�˵����", "", 0, 0, SDT_INT, 0, NULL}
};
FucInfo Fn_MenuAttach = { {
		/*ccname*/  "�������",
		/*egname*/  "Attach",
		/*explain*/ NULL,
		/*category*/-1,
		/*state*/   0,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_Attach),
		/*arg lp*/  s_Args_Attach},ESTLFNAME(libstl_MenuEx_Attach) };

EXTERN_C void libstl_MenuEx_Detach(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	pRetData->m_int = (int)p->m_hMenu;
	p->m_hMenu = NULL;
}
FucInfo Fn_MenuDetach = { {
		/*ccname*/  "������",
		/*egname*/  "Detach",
		/*explain*/ "���������еĲ˵������ز˵������",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL},ESTLFNAME(libstl_MenuEx_Detach) };

EXTERN_C void libstl_MenuEx_SetDelFlag(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	p->m_bDeleteMenu = pArgInf[1].m_bool;
}
static ARG_INFO s_Args_SetDelFlag[] =
{
	{"��������ʱ�Ƿ����ٲ˵�", "", 0, 0, SDT_BOOL, 0, NULL}
};
FucInfo Fn_MenuSetDelFlag = { {
		/*ccname*/  "�û��ձ�־",
		/*egname*/  "SetDelFlag",
		/*explain*/ NULL,
		/*category*/-1,
		/*state*/   0,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_SetDelFlag),
		/*arg lp*/  s_Args_SetDelFlag},ESTLFNAME(libstl_MenuEx_SetDelFlag) };

EXTERN_C void libstl_MenuEx_InsertItem(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	MENUITEMINFOW mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STRING | MIIM_STATE;
	mii.dwTypeData = (PWSTR)elibstl::args_to_pszw(pArgInf, 1);
	mii.fState = pArgInf[4].m_int;
	if (pArgInf[5].m_dtDataType != _SDT_NULL)
	{
		mii.fMask |= MIIM_ID;
		mii.wID = pArgInf[5].m_int;
	}

	if (pArgInf[6].m_dtDataType != _SDT_NULL || !pArgInf[7].m_int)
	{
		mii.fMask |= MIIM_SUBMENU;
		mii.hSubMenu = (HMENU)pArgInf[6].m_int;
	}

	if (pArgInf[7].m_dtDataType != _SDT_NULL)
	{
		mii.fMask |= MIIM_DATA;
		mii.dwItemData = pArgInf[7].m_int;
	}

	if (pArgInf[8].m_dtDataType != _SDT_NULL)
	{
		mii.fMask |= MIIM_BITMAP;
		if (pArgInf[8].m_dtDataType == SDT_INT)
			mii.hbmpItem = (HBITMAP)pArgInf[8].m_int;
		else if (pArgInf[8].m_dtDataType == SDT_BIN)
		{
			HBITMAP hbm = elibstl::make_hbm_gp(
				elibstl::get_array_data_base(pArgInf[8].m_pBin),
				elibstl::get_array_count(pArgInf[8].m_pBin));
			if (!hbm)
			{
				pRetData->m_bool = FALSE;
				return;
			}
			mii.hbmpItem = hbm;
		}
		else
		{
			pRetData->m_bool = FALSE;
			return;
		}
	}

	int iPos;
	BOOL bIndex;
	if (pArgInf[2].m_dtDataType == _SDT_NULL)
	{
		iPos = GetMenuItemCount(p->m_hMenu);
		bIndex = TRUE;
	}
	else if (pArgInf[2].m_int < 0 && pArgInf[3].m_bool)
	{
		iPos = GetMenuItemCount(p->m_hMenu);
		bIndex = TRUE;
	}
	else
	{
		iPos = pArgInf[2].m_int;
		bIndex = pArgInf[3].m_bool;
	}

	pRetData->m_bool = InsertMenuItemW(p->m_hMenu, iPos, bIndex, &mii);
}
static ARG_INFO s_Args_InsertItem[] =
{
	{"����", "", 0, 0, SDT_BIN, 0, 0},
	{"����λ��", "������������½����뵽�˵�ĩβ��1)�ò���Ϊ�գ�2)������Ϊ���Ҹò���Ϊ����", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"״̬", "#�˵���״̬_ ������ʹ�� λ��() �������״̬", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
	{"ID", "", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"�Ӳ˵����", "", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"�Զ�����ֵ", "", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"λͼ���", "ָ��һ��λͼ�����ͼƬ�ֽڼ��� #�˵�Ԥ��λͼ_ ������"
			"ע�⣺1)�д���ͼƬ��Ҫ������ʱ��Ӧ�����ֽڼ�����ᵼ���������⣻"
			"2)���������������ֽڼ�����˵�Ex������������ڱ���������˵�������֮����Ϊ�������ڲ�ά��λͼ�����", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
};
FucInfo Fn_MenuInsertItem = { {
		/*ccname*/  "������Ŀ",
		/*egname*/  "InsertItem",
		/*explain*/ "�ɹ������棬ʧ�ܷ��ؼ�",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_InsertItem),
		/*arg lp*/  s_Args_InsertItem},ESTLFNAME(libstl_MenuEx_InsertItem) };

EXTERN_C void libstl_MenuEx_CreatePopupMenu(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	HMENU hMenu = CreatePopupMenu();
	if(pArgInf[1].m_bool)
	{
		if (p->m_bDeleteMenu)
			DestroyMenu(p->m_hMenu);
		p->m_hMenu = hMenu;
	}
	pRetData->m_int = (int)hMenu;
}
static ARG_INFO s_Args_CreatePopupMenu[] =
{
	{"�Ƿ�����", "", 0, 0, SDT_BOOL, FALSE, AS_HAS_DEFAULT_VALUE}
};
FucInfo Fn_MenuCreatePopupMenu = { {
		/*ccname*/  "��������ʽ�˵�",
		/*egname*/  "CreatePopupMenu",
		/*explain*/ "����һ������ʽ�˵����ɹ����ز˵������ʧ�ܷ���0",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_CreatePopupMenu),
		/*arg lp*/  s_Args_CreatePopupMenu},ESTLFNAME(libstl_MenuEx_CreatePopupMenu) };

EXTERN_C void libstl_MenuEx_GetCurrentMenu(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	pRetData->m_int = (int)p->m_hMenu;
	p->m_hMenu = NULL;
}
FucInfo Fn_MenuGetCurrentMenu = { {
		/*ccname*/  "ȡ��ǰ�˵����",
		/*egname*/  "GetCurrentMenu",
		/*explain*/ "���ز˵����",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL},ESTLFNAME(libstl_MenuEx_GetCurrentMenu) };

EXTERN_C void libstl_MenuEx_DeleteItem(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	DeleteMenu(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_int ? MF_BYPOSITION : MF_BYCOMMAND);
}
static ARG_INFO s_Args_DeleteItem[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuDeleteItem = { {
		/*ccname*/  "������Ŀ",
		/*egname*/  "DeleteItem",
		/*explain*/ "�ɹ������棬ʧ�ܷ��ؼ�",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_DeleteItem),
		/*arg lp*/  s_Args_DeleteItem},ESTLFNAME(libstl_MenuEx_DeleteItem) };

EXTERN_C void libstl_MenuEx_GetCount(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	pRetData->m_int = GetMenuItemCount(p->m_hMenu);
}
FucInfo Fn_MenuGetCount = { {
		/*ccname*/  "ȡ��Ŀ��",
		/*egname*/  "GetCount",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL},ESTLFNAME(libstl_MenuEx_GetCount) };

EXTERN_C void libstl_MenuEx_GetCaption(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	MENUITEMINFOW mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STRING;
	mii.dwTypeData = NULL;
	mii.cch = 0;
	GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
	if (!mii.cch)
	{
		pRetData->m_pBin = NULL;
		return;
	}
	pRetData->m_pBin = elibstl::malloc_wstring(mii.cch);
	mii.dwTypeData = (PWSTR)(pRetData->m_pBin + 8);
	mii.cch++;
	GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
}
static ARG_INFO s_Args_GetCaption[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuGetCaption = { {
		/*ccname*/  "ȡ����",
		/*egname*/  "GetCaption",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BIN,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_GetCaption),
		/*arg lp*/  s_Args_GetCaption},ESTLFNAME(libstl_MenuEx_GetCaption) };

EXTERN_C void libstl_MenuEx_SetCheck(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	CheckMenuItem(p->m_hMenu, pArgInf[1].m_int,
		(pArgInf[2].m_bool ? MF_BYPOSITION : MF_BYCOMMAND) |
		(pArgInf[3].m_bool ? MF_CHECKED : MF_UNCHECKED));
}
static ARG_INFO s_Args_SetCheck[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"�Ƿ�ѡ��", "", 0, 0, SDT_BOOL, 0, 0},
};
FucInfo Fn_MenuSetCheck = { {
		/*ccname*/  "��ѡ��״̬",
		/*egname*/  "SetCheck",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		_SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_SetCheck),
		/*arg lp*/  s_Args_SetCheck},ESTLFNAME(libstl_MenuEx_SetCheck) };

EXTERN_C void libstl_MenuEx_GetCheck(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	pRetData->m_bool = elibstl::IsBitExist(
		GetMenuState(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool ? MF_BYPOSITION : MF_BYCOMMAND),
		MF_CHECKED);
}
static ARG_INFO s_Args_GetCheck[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"�Ƿ�ѡ��", "", 0, 0, SDT_BOOL, 0, 0},
};
FucInfo Fn_MenuGetCheck = { {
		/*ccname*/  "ȡѡ��״̬",
		/*egname*/  "GetCheck",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_GetCheck),
		/*arg lp*/  s_Args_GetCheck},ESTLFNAME(libstl_MenuEx_GetCheck) };

EXTERN_C void libstl_MenuEx_SetRadioCheck(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	if (pArgInf[5].m_bool)
		pRetData->m_bool = CheckMenuRadioItem(p->m_hMenu, pArgInf[2].m_int, pArgInf[3].m_int, pArgInf[1].m_int,
			(pArgInf[4].m_bool ? MF_BYPOSITION : MF_BYCOMMAND));
	else
	{
		MENUITEMINFOW mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE | MIIM_FTYPE;
		GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
		mii.fState &= (~MFS_DEFAULT);
		mii.fType &= (~MFT_RADIOCHECK);
		pRetData->m_bool = SetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
	}
}
static ARG_INFO s_Args_SetRadioCheck[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"���ⷶΧ��ʼλ��", "", 0, 0, SDT_INT, 0, 0},
	{"���ⷶΧ����λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"�Ƿ�ѡ��", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuSetRadioCheck = { {
		/*ccname*/  "�õ�ѡ״̬",
		/*egname*/  "SetRadioCheck",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_SetRadioCheck),
		/*arg lp*/  s_Args_SetRadioCheck},ESTLFNAME(libstl_MenuEx_SetRadioCheck) };

EXTERN_C void libstl_MenuEx_GetRadioCheck(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	MENUITEMINFOW mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_FTYPE;
	GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);

	pRetData->m_bool =
		elibstl::IsBitExist(
			GetMenuState(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool ? MF_BYPOSITION : MF_BYCOMMAND),
			MF_CHECKED) &&
		elibstl::IsBitExist(mii.fType, MFT_RADIOCHECK);
}
static ARG_INFO s_Args_GetRadioCheck[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuGetRadioCheck = { {
		/*ccname*/  "ȡ��ѡ״̬",
		/*egname*/  "GetRadioCheck",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_GetRadioCheck),
		/*arg lp*/  s_Args_GetRadioCheck},ESTLFNAME(libstl_MenuEx_GetRadioCheck) };

EXTERN_C void libstl_MenuEx_SetDefault(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	MENUITEMINFOW mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
	if (pArgInf[3].m_bool)
		mii.fState |= MFS_DEFAULT;
	else
		mii.fState &= (~MFS_DEFAULT);
	pRetData->m_bool = SetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
}
static ARG_INFO s_Args_SetDefault[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"�Ƿ�Ĭ��", "", 0, 0, SDT_BOOL, 0, 0},
};
FucInfo Fn_MenuSetDefault = { {
		/*ccname*/  "��Ĭ��״̬",
		/*egname*/  "SetDefault",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_SetDefault),
		/*arg lp*/  s_Args_SetDefault},ESTLFNAME(libstl_MenuEx_SetDefault) };

EXTERN_C void libstl_MenuEx_GetDefault(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	MENUITEMINFOW mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	GetMenuItemInfoW(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool, &mii);
	pRetData->m_bool = elibstl::IsBitExist(mii.fState, MFS_DEFAULT);
}
static ARG_INFO s_Args_GetDefault[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuGetDefault = { {
		/*ccname*/  "ȡĬ��״̬",
		/*egname*/  "GetDefault",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_GetDefault),
		/*arg lp*/  s_Args_GetDefault},ESTLFNAME(libstl_MenuEx_GetDefault) };

EXTERN_C void libstl_MenuEx_SetDisable(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	pRetData->m_bool = EnableMenuItem(p->m_hMenu, pArgInf[1].m_int,
		(pArgInf[2].m_bool ? MF_BYPOSITION : MF_BYCOMMAND) |
		(pArgInf[3].m_bool ? (pArgInf[4].m_bool ? MF_GRAYED : MF_DISABLED) : MF_ENABLED));
}
static ARG_INFO s_Args_SetDisable[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
	{"�Ƿ��ֹ", "", 0, 0, SDT_BOOL, 0, 0},
	{"�Ƿ��ɫ", "���˲���Ϊ�٣����������Ŀ�������ı���Ŀ����ۣ����� �Ƿ��ֹ ����Ϊ��ʱ��Ч", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuSetDisable = { {
		/*ccname*/  "�ý�ֹ״̬",
		/*egname*/  "SetDisable",
		/*explain*/ "",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_SetDisable),
		/*arg lp*/  s_Args_SetDisable},ESTLFNAME(libstl_MenuEx_SetDisable) };

EXTERN_C void libstl_MenuEx_GetDisable(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto p = elibstl::args_to_obj_noref<CMenu>(pArgInf);

	UINT uState = GetMenuState(p->m_hMenu, pArgInf[1].m_int, pArgInf[2].m_bool ? MF_BYPOSITION : MF_BYCOMMAND);
	if (elibstl::IsBitExist(uState, MFS_GRAYED))
		pRetData->m_int = 2;
	else if (elibstl::IsBitExist(uState, MFS_DISABLED))
		pRetData->m_int = 1;
	else
		pRetData->m_int = 0;
}
static ARG_INFO s_Args_GetDisable[] =
{
	{"λ��", "", 0, 0, SDT_INT, 0, 0},
	{"λ���Ƿ�Ϊ����", "", 0, 0, SDT_BOOL, TRUE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_MenuGetDisable = { {
		/*ccname*/  "ȡ��ֹ״̬",
		/*egname*/  "GetDisable",
		/*explain*/ "����ֵ���壺0 - ����  1 - ����  2 - �����һ�ɫ",
		/*category*/-1,
		/*state*/   0,
		/*ret*/		SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_GetDisable),
		/*arg lp*/  s_Args_GetDisable},ESTLFNAME(libstl_MenuEx_GetDisable) };














static int s_Cmd_MenuEx[] = { 196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215 };
ESTL_NAMESPACE_BEGIN
LIB_DATA_TYPE_INFO ClMenu =
{
	"�˵�Ex",
	"MenuEx",
	"",
	ARRAYSIZE(s_Cmd_MenuEx),
	s_Cmd_MenuEx,
	_DT_OS(__OS_WIN),
	0,
	0,
	NULL,
	0,
	NULL,
	NULL,
	0,
	NULL
};
ESTL_NAMESPACE_END