#include"intrin.h"
#include"ElibHelp.h"
//static std::vector<std::string> OpenManyFileDialog(const char* title, const char* filter, int initFilter, const char* initDir, bool noChangeDir, HWND parentWnd, int openMode)
//{
//	std::vector<std::string> fileNames;
//	std::vector<char> szFile(MAX_PATH * 200, 0);
//	OPENFILENAMEA ofn = { sizeof(ofn) };
//	ofn.hwndOwner = parentWnd;
//	ofn.lpstrTitle = title;
//	ofn.lpstrFilter = filter;
//	ofn.nFilterIndex = initFilter;
//	ofn.lpstrFile = szFile.data();
//	ofn.nMaxFile = static_cast<DWORD>(szFile.size());
//	ofn.lpstrInitialDir = initDir;
//	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
//	if (noChangeDir) ofn.Flags |= OFN_NOCHANGEDIR;
//	if (openMode == 1) ofn.Flags |= OFN_OVERWRITEPROMPT;
//	if (GetOpenFileNameA(&ofn))
//	{
//		char* p = szFile.data() + strlen(szFile.data()) + 1;
//		while (*p)
//		{
//			std::string szFileName(szFile.data());
//			szFileName += '\\';
//			szFileName += p;
//			fileNames.push_back(szFileName);
//			p += strlen(p) + 1;
//		}
//	}
//	return fileNames;
//}

static ARG_INFO Args[] =
{
	{
		/*name*/    "����",
		/*explain*/ ("ָ���ļ��򿪶Ի���ı��⣬�����ʡ�ԣ���Ĭ��Ϊ�����������򿪵��ļ���"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/    AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "������",
		/*explain*/ ("�������ı��ɵ��������ɶԵ��ı�����ɣ�ÿ���ı����ĵ�һ��������ʾ��ʽ���磺���ı��ļ���*.txt�������ڶ���ָ��ʵ�ʵĹ���ƥ������磺��*.txt�������и��ı���֮���á�|���Ÿ����������ʡ�ԣ���Ĭ��û�й�����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/  AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "��ʼ������",
		/*explain*/ ("���Ա�ʡ�ԡ������һ�����ṩ����Ч�Ĺ������ı����򱾲�������ָ����ʼ�Ĺ�������0Ϊ��һ���������������ʡ�ԣ���Ĭ��ֵΪ0"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},{
		/*name*/    "��ʼĿ¼",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ�����򿪶Ի���ʱ���Զ���ת����Ŀ¼�������ʡ�ԣ���Ĭ��Ϊ��ǰĿ¼��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY ,
	},
	{
		/*name*/    "���ı�Ŀ¼",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ���ڶԻ���رպ��Ƿ��Զ����ص�����Ի���ǰ���ļ�Ŀ¼�������ʡ�ԣ���Ĭ��ֵΪ�١�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BOOL,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "������",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ���Ի���ĸ�����,������һ��\"����\"�������ݻ���һ�������ʹ��ھ��.�����ʡ��,Ĭ��Ϊ��."),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "�Ի�������",
		/*explain*/ ("0Ϊ�򿪣�1Ϊ���棬Ĭ��Ϊ0"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	}
};


static std::string OpenFileDialog(const char* title, const char* filter, int initFilter, const char* initDir, bool noChangeDir, HWND parentWnd, int openMode)
{
	std::string fileName;
	CHAR szFile[MAX_PATH] = { 0 };
	OPENFILENAMEA ofn = { sizeof(ofn) };
	ofn.hwndOwner = parentWnd;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = initFilter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrInitialDir = initDir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (noChangeDir) ofn.Flags |= OFN_NOCHANGEDIR;
	if (openMode == 1) ofn.Flags |= OFN_OVERWRITEPROMPT;
	if (GetOpenFileNameA(&ofn))
	{
		fileName = szFile;
	}
	return fileName;
}
EXTERN_C void Fn_Open_File_Dialog_A(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto title = elibstl::args_to_sdata(pArgInf, 0), filter = elibstl::args_to_sdata(pArgInf, 1), initDir = elibstl::args_to_sdata(pArgInf, 3);
	int initFilter = elibstl::args_to_data<int>(pArgInf, 2).value_or(0), openMode = elibstl::args_to_data<int>(pArgInf, 6).value_or(0);
	bool noChangeDir = elibstl::args_to_data<BOOL>(pArgInf, 4).value_or(false);
	HWND parentWnd = reinterpret_cast<HWND> (elibstl::args_to_data<INT>(pArgInf, 5).value_or(0));
	pRetData->m_pText = elibstl::clone_text(OpenFileDialog(title.data(), filter.data(), initFilter, initDir.data(), noChangeDir, parentWnd, openMode));
}

FucInfo e_Open_File_Dialog_A = { {
		/*ccname*/  ("�ļ�ѡ���"),
		/*egname*/  (""),
		/*explain*/ ("��ʾһ���ļ��򿪶Ի��������û�ѡ�����������Ҫ�򿪵��Ѵ����ļ����򱣴��ļ��������û���ѡ��������Ľ���ı�������û�δ����򰴡�ȡ������ť�˳����򷵻�һ�����ı�"),
		/*category*/6,
		/*state*/   NULL,
		/*ret*/     SDT_TEXT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args) / sizeof(Args[0]),
		/*arg lp*/  Args,
	} ,Fn_Open_File_Dialog_A ,"Fn_Open_File_Dialog_A" };






static ARG_INFO WArgs[] =
{
	{
		/*name*/    "����",
		/*explain*/ ("ָ���ļ��򿪶Ի���ı��⣬�����ʡ�ԣ���Ĭ��Ϊ�����������򿪵��ļ���"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/    AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "������",
		/*explain*/ ("�������ı��ɵ��������ɶԵ��ı�����ɣ�ÿ���ı����ĵ�һ��������ʾ��ʽ���磺���ı��ļ���*.txt�������ڶ���ָ��ʵ�ʵĹ���ƥ������磺��*.txt�������и��ı���֮���á�|���Ÿ����������ʡ�ԣ���Ĭ��û�й�����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/  AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "��ʼ������",
		/*explain*/ ("���Ա�ʡ�ԡ������һ�����ṩ����Ч�Ĺ������ı����򱾲�������ָ����ʼ�Ĺ�������0Ϊ��һ���������������ʡ�ԣ���Ĭ��ֵΪ0"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},{
		/*name*/    "��ʼĿ¼",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ�����򿪶Ի���ʱ���Զ���ת����Ŀ¼�������ʡ�ԣ���Ĭ��Ϊ��ǰĿ¼��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY ,
	},
	{
		/*name*/    "���ı�Ŀ¼",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ���ڶԻ���رպ��Ƿ��Զ����ص�����Ի���ǰ���ļ�Ŀ¼�������ʡ�ԣ���Ĭ��ֵΪ�١�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BOOL,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "������",
		/*explain*/ ("���Ա�ʡ�ԡ�ָ���Ի���ĸ�����,������һ��\"����\"�������ݻ���һ�������ʹ��ھ��.�����ʡ��,Ĭ��Ϊ��."),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "�Ի�������",
		/*explain*/ ("0Ϊ�򿪣�1Ϊ���棬Ĭ��Ϊ0"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   AS_DEFAULT_VALUE_IS_EMPTY,
	}
};



static std::wstring OpenFileDialogW(const wchar_t* title, const wchar_t* filter, int initFilter, const wchar_t* initDir, bool noChangeDir, HWND parentWnd, int openMode)
{
	std::wstring fileName;
	WCHAR szFile[MAX_PATH] = { 0 };
	OPENFILENAMEW ofn = { sizeof(ofn) };
	ofn.hwndOwner = parentWnd;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = initFilter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrInitialDir = initDir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (noChangeDir) ofn.Flags |= OFN_NOCHANGEDIR;
	if (openMode == 1) ofn.Flags |= OFN_OVERWRITEPROMPT;
	if (GetOpenFileNameW(&ofn))
	{
		fileName = szFile;
	}
	return fileName;
}
EXTERN_C void Fn_Open_File_Dialog_W(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto title = elibstl::args_to_wsdata(pArgInf, 0), filter = elibstl::args_to_wsdata(pArgInf, 1), initDir = elibstl::args_to_wsdata(pArgInf, 3);
	int initFilter = elibstl::args_to_data<int>(pArgInf, 2).value_or(0), openMode = elibstl::args_to_data<int>(pArgInf, 6).value_or(0);
	bool noChangeDir = elibstl::args_to_data<BOOL>(pArgInf, 4).value_or(false);
	HWND parentWnd = reinterpret_cast<HWND> (elibstl::args_to_data<INT>(pArgInf, 5).value_or(0));
	pRetData->m_pBin = elibstl::clone_textw(OpenFileDialogW(std::wstring(title).c_str(), std::wstring(filter).data(), initFilter, std::wstring(initDir).data(), noChangeDir, parentWnd, openMode));
}

FucInfo e_Open_File_Dialog_W = { {
		/*ccname*/  ("�ļ�ѡ���W"),
		/*egname*/  (""),
		/*explain*/ ("��ʾһ���ļ��򿪶Ի��������û�ѡ�����������Ҫ�򿪵��Ѵ����ļ����򱣴��ļ��������û���ѡ��������Ľ���ı�������û�δ����򰴡�ȡ������ť�˳����򷵻�һ�����ı�"),
		/*category*/6,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/sizeof(Args) / sizeof(Args[0]),
		/*arg lp*/  Args,
	} ,Fn_Open_File_Dialog_W ,"Fn_Open_File_Dialog_W" };