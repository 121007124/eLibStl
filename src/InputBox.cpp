#include "ElibHelp.h"


/*ת16�����ڴ��ı�*/
/*#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

typedef unsigned char BYTE;

std::string makedatamem(BYTE* data, size_t size) {
	std::ostringstream oss;
	oss << "BYTE Data[] = {\n";

	for (size_t i = 0; i < size; ++i) {
		if (i % 16 == 0) {
			oss << "    ";
		}

		oss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);

		if (i != size - 1) {
			oss << ",";
		}

		if ((i + 1) % 16 == 0 && i != size - 1) {
			oss << "\n";
		} else {
			oss << " ";
		}
	}

	oss << "};";

	return oss.str();
}*/

//216ƫ��Ϊ�༭����
BYTE InputBoxDialogTemplate[] = {
	0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x08, 0xC8, 0x80,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x8B,
	0x93, 0x8F, 0x65, 0x51, 0x87, 0x65, 0x2C, 0x67, 0x1A, 0xFF, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x4D, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6C, 0x00,
	0x6C, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x02, 0x50, 0x07, 0x00, 0x07, 0x00, 0xEE, 0x00, 0x23, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x82, 0x00, 0x4B, 0x6D, 0xD5, 0x8B, 0x00, 0x4E, 0x0B, 0x4E,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50,
	0x3F, 0x00, 0x42, 0x00, 0x37, 0x00, 0x13, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00,
	0x6E, 0x78, 0xA4, 0x8B, 0x93, 0x8F, 0x65, 0x51, 0x28, 0x00, 0x26, 0x00, 0x4F, 0x00, 0x29, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50,
	0x86, 0x00, 0x42, 0x00, 0x37, 0x00, 0x13, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00,
	0xD6, 0x53, 0x88, 0x6D, 0x28, 0x00, 0x26, 0x00, 0x43, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x81, 0x50, 0x07, 0x00, 0x2D, 0x00,
	0xEE, 0x00, 0x0C, 0x00, 0x3B, 0x87, 0x00, 0x00, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00 };

//82ƫ��Ϊ�༭����
BYTE InputBoxDialogTemplateWithOutPrompt[] = {
	0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x08, 0xC8, 0x80,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD6, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x8B,
	0x93, 0x8F, 0x65, 0x51, 0x1A, 0xFF, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x00,
	0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x20, 0x00,
	0x44, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0x00, 0x81, 0x50, 0x07, 0x00, 0x07, 0x00, 0xC8, 0x00, 0x0C, 0x00, 0x3B, 0x87, 0x00, 0x00,
	0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x50, 0x52, 0x00, 0x1E, 0x00, 0x37, 0x00, 0x13, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x80, 0x00, 0x6E, 0x78, 0xA4, 0x8B, 0x93, 0x8F, 0x65, 0x51, 0x28, 0x00, 0x26, 0x00,
	0x4F, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x50, 0x97, 0x00, 0x1E, 0x00, 0x37, 0x00, 0x13, 0x00, 0x02, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x80, 0x00, 0xD6, 0x53, 0x88, 0x6D, 0x28, 0x00, 0x26, 0x00, 0x43, 0x00, 0x29, 0x00,
	0x00, 0x00, 0x00, 0x00 };

EXTERN_C void libstl_InputBox(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
#pragma region ��̬��ֵ

	//	/*����Ϊ�ؼ�*/
	///*��ʾ��*/
	//	std::wstring prompt_title = L"��ʾ��Ϣ";
	//	WORD prompt_check = 0xffff; //��ʾϵͳע��ؼ�
	//	WORD prompt_class = 0x0082; //��̬
	//	auto prompt_size = (prompt_title.size() + 1) * sizeof(WCHAR) + sizeof(WORD) * 2 + sizeof(DLGITEMTEMPLATE);
	//
	//	DLGITEMTEMPLATE* prompt_box = reinterpret_cast<DLGITEMTEMPLATE*>(new char[prompt_size] {});
	//	prompt_box->cx = 238;
	//	prompt_box->cy = 35;
	//	prompt_box->dwExtendedStyle = 0;
	//	prompt_box->id = 65535;
	//	prompt_box->style = 1342312448;
	//	prompt_box->x = 7;
	//	prompt_box->y = 7;
	//
	//	auto pbegin_prompt = reinterpret_cast<WORD*>(prompt_box + 1);
	//	*pbegin_prompt++ = prompt_check;
	//	*pbegin_prompt++ = prompt_class;
	//
	//	wcscpy_s(reinterpret_cast<WCHAR*>(pbegin_prompt), prompt_title.size() + 1, prompt_title.c_str());
	//	/*��ť*/
	//	std::wstring confirm_title = L"ȷ������(&O)";
	//	WORD confirm_check = 0xffff; //��ʾϵͳע��ؼ�
	//	WORD confirm_class = 0x0080; //��̬
	//	auto confirm_size = (confirm_title.size() + 1) * sizeof(WCHAR) + sizeof(WORD) * 2 + sizeof(DLGITEMTEMPLATE);
	//
	//	DLGITEMTEMPLATE* confirm_box = reinterpret_cast<DLGITEMTEMPLATE*>(new char[confirm_size] {});
	//	confirm_box->cx = 238;
	//	confirm_box->cy = 35;
	//	confirm_box->dwExtendedStyle = 4;
	//	confirm_box->id = IDOK;
	//	confirm_box->style = 1342242816;
	//	confirm_box->x = 7;
	//	confirm_box->y = 7;
	//	auto confirm_prompt = reinterpret_cast<WORD*>(confirm_box + 1);
	//	*confirm_prompt++ = confirm_check;
	//	*confirm_prompt++ = confirm_class;
	//	wcscpy_s(reinterpret_cast<WCHAR*>(confirm_prompt), confirm_title.size() + 1, confirm_title.c_str());
	//
	//	// �Ի���ģ������
	//	const WCHAR* dlgCaption = L"������:";
	//	const WCHAR* dlgFontName = L"MS Shell Dlg";
	//
	//	// ����Ի���ģ��Ĵ�С
	//	const int dlgCaptionLen = lstrlenW(dlgCaption) + 1;
	//	const int dlgTemplateSize =
	//		sizeof(DLGTEMPLATE) //�Ի����ͷ
	//		+ (sizeof(WORD) * 2) //�˵���Ĭ�ϴ���(��Ϊ0)
	//		+ (dlgCaptionLen * sizeof(WCHAR)) //����
	//		+ sizeof(WORD) //�����С
	//		+ (lstrlenW(dlgFontName) + 1) * sizeof(WCHAR) //��������
	//		+ prompt_size//��ʾ��
	//		+ confirm_size
	//		;
	//
	//	// �����Ի���ģ��
	//	DLGTEMPLATE* pDlgTemplate = reinterpret_cast<DLGTEMPLATE*>(new char[dlgTemplateSize] {});
	//	auto pbegin = reinterpret_cast<char*>(pDlgTemplate);
	//
	//	DLGITEMTEMPLATE* A = (DLGITEMTEMPLATE*)BMInputBoxDialogTemplateData;
	//
	//
	//	// ���Ի���ģ��
	//	pDlgTemplate->style = 4294901761;
	//	pDlgTemplate->cdit = 1;
	//	pDlgTemplate->x = 0;
	//	pDlgTemplate->y = 0;
	//	pDlgTemplate->cx = 252;
	//	pDlgTemplate->cy = 92;
	//
	//	// ���ñ���
	//	WCHAR* pCaption = reinterpret_cast<WCHAR*>(pbegin + sizeof(DLGTEMPLATE) + (sizeof(WORD) * 2));
	//	wcscpy_s(pCaption, dlgCaptionLen, dlgCaption);
	//
	//	// ����������Ϣ
	//	WORD* pWord = reinterpret_cast<WORD*>(pbegin + sizeof(DLGTEMPLATE) + (sizeof(WORD) * 2) + dlgCaptionLen * sizeof(WCHAR));
	//	*pWord = 8;
	//	wcscpy_s(reinterpret_cast<LPWSTR>(pWord + 1), lstrlenW(dlgFontName) + 1, dlgFontName);
	//	auto controlbegin = reinterpret_cast<BYTE*>(reinterpret_cast<LPWSTR>(pWord + 1) + lstrlenW(dlgFontName) + 1);
	//	memcpy(controlbegin, prompt_box, prompt_size);
	//	controlbegin += prompt_size;
	//	memcpy(controlbegin, confirm_box, confirm_size);
#pragma endregion




	/*.�汾 2
.֧�ֿ� libstl

.�ӳ��� __��������_�������
.�ֲ����� A, �ı���


�����W (����, ����, ����,A , 1, )

*/

	auto prompt_str = elibstl::arg_to_wstring(pArgInf, 0);

	/*Ĭ��Ϊ��ǰ�����*/
	auto hParent = elibstl::args_to_data<INT>(pArgInf, 5).value_or(reinterpret_cast<INT>(GetActiveWindow()));

	auto title = elibstl::arg_to_wstring(pArgInf, 1);

	auto init_text = elibstl::arg_to_wstring(pArgInf, 2);

	auto nType = elibstl::args_to_data<INT>(pArgInf, 4).value_or(1);

	if (nType < 1 || nType>4)
		nType = 1;


	if (nType)
	{
		/*����ģʽ*/
		InputBoxDialogTemplate[216] = 0xA0;//ES_PASSWORD
	}

	HWND hDlg = CreateDialogIndirectParam(::GetModuleHandle(NULL), (LPCDLGTEMPLATE)InputBoxDialogTemplate, (HWND)hParent, 0, 0);
	SetWindowTextW(hDlg, title.c_str());
	/*��ȡ��ʾ��*/
	auto hPrompt = GetDlgItem(hDlg, -1);
	SetWindowTextW(hPrompt, prompt_str.c_str());
	/*��ȡ�༭��*/
	auto hEdit = GetDlgItem(hDlg, 34619);
	SetWindowTextW(hPrompt, init_text.c_str());

	if (hDlg) {
		ShowWindow(hDlg, SW_SHOW);
		UpdateWindow(hDlg);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


}
static ARG_INFO s_Args_InputBox[] =
{
	{"��ʾ��Ϣ", "֧���������ͣ��Ƽ�utf16�ı�", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"���ڱ���", "֧���������ͣ��Ƽ�utf16�ı�", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"��ʼ�ı�", "֧���������ͣ��Ƽ�utf16�ı�", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	{"����������ݵı���", "����ֵ��ָ���ı�������Ϊ��ֵ���ı��ͣ������Բ�ͬ����������ȡ����������", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},
	{"���뷽ʽ", "����ֵ����Ϊ���³���ֵ�� 1 - �����ı�  2 - ��������  3 - ����С��  4 - �������롣���ʡ�Ա�������Ĭ��Ϊ�������ı�����", 0, 0, SDT_INT, 1, AS_HAS_DEFAULT_VALUE},
	{"������", "ָ�������ĸ����ڣ�������һ�������ڡ��������ݻ���һ�������ʹ��ھ���������ʡ�ԣ�Ĭ��Ϊ�ޡ�", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
	//{"�Ƿ��������", "", 0, 0, SDT_BOOL, FALSE, AS_HAS_DEFAULT_VALUE},
};
FucInfo Fn_InputBox = { {
		/*ccname*/  "�����W",
		/*egname*/  "InputBoxW",
		/*explain*/ "��һ�Ի�������ʾ��ʾ���ȴ��û��������Ĳ����°�ť������û���ȷ������󣨰��¡�ȷ�����롱��ť��س������˳��������棬���򷵻ؼ١�",
		/*category*/13,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/ARRAYSIZE(s_Args_InputBox),
		/*arg lp*/  s_Args_InputBox },ESTLFNAME(libstl_InputBox) };