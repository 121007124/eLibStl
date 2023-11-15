#include"EcontrolHelp.h"
#include <algorithm>
//#include"memload\MemoryModule.h"

#pragma comment (lib, "winmm.lib")
#pragma warning(disable:28159)
using namespace std;
//using namespace eMemoryModule;


namespace {
typedef int (APIENTRY* DLLMAIN)(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
typedef int (APIENTRY* WINMAIN)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
typedef DWORD(WINAPI* FRCALLBACK)(LPCSTR pModuleName, LPCSTR pProcName, DWORD pFR_lParam);

//
//	�ڲ�����
//
LPVOID VirAlloc(LPVOID LpAddress, SIZE_T DwSize);
BOOL VirFree(LPVOID LpAddress);
UINT GetAlignedSize(UINT Origin, UINT Alignment);
VOID DoRelocation(DWORD NewBase);
BOOL FillRavAddress(DWORD ImageBase, FRCALLBACK pFR_CallBack, DWORD pFR_lParam);


//
//	��������
//
LPBYTE WINAPI _LoadLibrary(LPCBYTE LpLibData, FRCALLBACK pFR_CallBack, DWORD pFR_lParam);
BOOL WINAPI _FreeLibrary(LPBYTE hLibModule);
FARPROC WINAPI _GetProcAddress(LPBYTE hLibModule, LPCSTR lpProcName);
DWORD WINAPI _GetEntryPoint(LPBYTE hLibModule);







LPBYTE WINAPI _LoadLibrary(LPCBYTE LpLibData, FRCALLBACK pFR_CallBack, DWORD pFR_lParam)
{
	if (!LpLibData)
		return NULL;

	PIMAGE_DOS_HEADER LpDosHeader = (PIMAGE_DOS_HEADER)LpLibData;
	PIMAGE_NT_HEADERS LpNtHeader = (PIMAGE_NT_HEADERS)(LpLibData + LpDosHeader->e_lfanew);

	//
	//	�����Ч��
	//
	if (LpDosHeader->e_magic != IMAGE_DOS_SIGNATURE)	//	MZ
		return NULL;

	if (LpNtHeader->Signature != IMAGE_NT_SIGNATURE)	//	PE
		return NULL;

	if (!(LpNtHeader->FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE))	//	��ִ��
		return NULL;

	//
	//	����ӳ���С
	//
	PIMAGE_SECTION_HEADER SectionHeader;
	DWORD SizeOfImage = LpNtHeader->OptionalHeader.SizeOfImage;

	for (int i = 0; i < LpNtHeader->FileHeader.NumberOfSections; i++)
	{
		SectionHeader = (PIMAGE_SECTION_HEADER)(LpLibData
			+ LpDosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS)
			+ i * sizeof(IMAGE_SECTION_HEADER));

		SizeOfImage = max(SizeOfImage, GetAlignedSize(
			SectionHeader->VirtualAddress + max(SectionHeader->SizeOfRawData,
				SectionHeader->Misc.VirtualSize), LpNtHeader->OptionalHeader.SectionAlignment));
	}
	if (!SizeOfImage)
		return NULL;

	//
	//	�����ڴ�, ��������
	//
	LPBYTE hLibModule = (LPBYTE)VirAlloc((LPVOID)LpNtHeader->OptionalHeader.ImageBase, SizeOfImage);
	if (!hLibModule)
		return NULL;

	memcpy(hLibModule, LpLibData, LpDosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS)
		+ LpNtHeader->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));

	for (int i = 0; i < LpNtHeader->FileHeader.NumberOfSections; i++)
	{
		SectionHeader = (PIMAGE_SECTION_HEADER)(LpLibData + LpDosHeader->e_lfanew
			+ sizeof(IMAGE_NT_HEADERS) + i * sizeof(IMAGE_SECTION_HEADER));

		if (SectionHeader->SizeOfRawData && SectionHeader->VirtualAddress)
			memcpy(hLibModule + SectionHeader->VirtualAddress, LpLibData
				+ SectionHeader->PointerToRawData, SectionHeader->SizeOfRawData);
	}

	//
	//	�ض�λ
	//
	if (LpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
		DoRelocation((DWORD)hLibModule);

	//
	//	��䵼���
	//
	if (LpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		if (!FillRavAddress((DWORD)hLibModule, pFR_CallBack, pFR_lParam))
		{
			VirFree(hLibModule);
			return NULL;
		}
	}

	//
	//	ִ����ڴ���
	//
	if (LpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)	//	�Ƿ�Ϊ DLL
	{
		DLLMAIN _DllMain = (DLLMAIN)(hLibModule + LpNtHeader->OptionalHeader.AddressOfEntryPoint);
		if (!_DllMain(NULL, DLL_PROCESS_ATTACH, NULL))
		{
			_DllMain(NULL, DLL_PROCESS_DETACH, NULL);
			VirFree(hLibModule);
			return NULL;
		}
	}

	return hLibModule;
}

BOOL WINAPI _FreeLibrary(LPBYTE hLibModule)
{
	if (!hLibModule)
		return false;

	PIMAGE_DOS_HEADER LpDosHeader = (PIMAGE_DOS_HEADER)hLibModule;
	PIMAGE_NT_HEADERS LpNtHeader = (PIMAGE_NT_HEADERS)(hLibModule + LpDosHeader->e_lfanew);

	//
	//	ִ����ڴ���
	//
	if (LpNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)	//	�Ƿ�Ϊ DLL
	{
		DLLMAIN _DllMain = (DLLMAIN)(hLibModule + LpNtHeader->OptionalHeader.AddressOfEntryPoint);
		_DllMain(0, DLL_PROCESS_DETACH, 0);
	}

	return VirFree(hLibModule);
}

FARPROC WINAPI _GetProcAddress(LPBYTE hLibModule, LPCSTR lpProcName)
{
	if (!hLibModule || !lpProcName)
		return NULL;

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hLibModule;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(hLibModule + pDosHeader->e_lfanew);

	if (!pNtHeader->OptionalHeader.
		DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size)	//	�Ƿ���ڵ�����
		return NULL;

	PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(hLibModule
		+ pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	PDWORD AddressOfNames = (PDWORD)(hLibModule + pExportDirectory->AddressOfNames);
	PDWORD AddressOfFunctions = (PDWORD)(hLibModule + pExportDirectory->AddressOfFunctions);
	PWORD AddressOfNameOrdinals = (PWORD)(hLibModule + pExportDirectory->AddressOfNameOrdinals);
	LPSTR pProcName;
	DWORD Ordinals = (DWORD)lpProcName - pExportDirectory->Base;

	//
	//	Ѱ��ָ������
	//

	if (Ordinals >= 0 && Ordinals <= pExportDirectory->NumberOfFunctions)	//	�Ƿ�Ϊ���
	{
		return (FARPROC)(hLibModule + AddressOfFunctions[Ordinals]);
	}
	else	//	�����Ƶ����ĺ���
	{
		Ordinals = 0;
		for (DWORD i = 0; i < pExportDirectory->NumberOfNames; i++)
		{
			pProcName = (LPSTR)(hLibModule + AddressOfNames[i]);
			Ordinals = AddressOfNameOrdinals[i];

			if (Ordinals >= 0 && Ordinals <= pExportDirectory->NumberOfFunctions)
			{
				if (!strcmp(lpProcName, pProcName))
				{
					return (FARPROC)(hLibModule + AddressOfFunctions[Ordinals]);
				}
			}
		}
	}

	return NULL;
}



DWORD WINAPI _GetEntryPoint(LPBYTE hLibModule)
{
	return (DWORD)(hLibModule + ((PIMAGE_NT_HEADERS)(hLibModule
		+ ((PIMAGE_DOS_HEADER)hLibModule)->e_lfanew))->OptionalHeader.AddressOfEntryPoint);
}


LPVOID VirAlloc(LPVOID LpAddress, SIZE_T DwSize)
{
	LPVOID pMem;

	//
	//	�����ڴ�
	//
	pMem = VirtualAlloc(LpAddress, DwSize,	//	ָ����ַ
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pMem)
		pMem = VirtualAlloc(NULL, DwSize,	//	�����ַ
			MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	return pMem;
}

BOOL VirFree(LPVOID LpAddress)
{
	//
	//	�ͷ��ڴ�
	//
	return VirtualFree(LpAddress, NULL, MEM_RELEASE);
}

UINT GetAlignedSize(UINT Origin, UINT Alignment)
{
	//
	//	���������С
	//
	return (Origin + Alignment - 1) / Alignment * Alignment;
}

VOID DoRelocation(DWORD NewBase)
{
	PIMAGE_DOS_HEADER LpDosHeader = (PIMAGE_DOS_HEADER)NewBase;
	PIMAGE_NT_HEADERS LpNtHeader = (PIMAGE_NT_HEADERS)(NewBase + LpDosHeader->e_lfanew);
	DWORD Delta = NewBase - LpNtHeader->OptionalHeader.ImageBase;
	DWORD Offset = LpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	PIMAGE_BASE_RELOCATION LpBaseRelocation = (PIMAGE_BASE_RELOCATION)(NewBase + Offset);
	short LocData;
	DWORD Address;

	//
	//	�ض�λ
	//
	do
	{
		for (DWORD i = 0; i < (LpBaseRelocation->SizeOfBlock - 8) / 2; i++)
		{
			LocData = *((PWORD)(NewBase + Offset + 8 + i * 2));
			if ((LocData & 61440) == 12288)
			{
				Address = NewBase + LpBaseRelocation->VirtualAddress + (LocData & 4095);
				*((PDWORD)Address) = *((PDWORD)Address) + Delta;
			}
		}
		Offset += LpBaseRelocation->SizeOfBlock;
		LpBaseRelocation = (PIMAGE_BASE_RELOCATION)(NewBase + Offset);
	} while (LpBaseRelocation->VirtualAddress);
}

BOOL FillRavAddress(DWORD ImageBase, FRCALLBACK pFR_CallBack, DWORD pFR_lParam)
{
	PIMAGE_DOS_HEADER LpDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	PIMAGE_NT_HEADERS LpNtHeader = (PIMAGE_NT_HEADERS)(ImageBase + LpDosHeader->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(ImageBase + LpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	DWORD dwRealIAT;
	HMODULE hModule;
	LPCSTR pModuleName;
	int i, j = 0;
	DWORD dwProcName;
	LPCSTR pProcName;
	DWORD dwFunction;
	char pStr[64 + 1];
	BOOL bRet = TRUE;

	//
	//	��䵼���
	//

	while (ImportDescriptor[j].FirstThunk)
	{
		pModuleName = (LPCSTR)(ImageBase	//	ģ����
			+ ImportDescriptor[j].Name);
		hModule = LoadLibraryA(pModuleName);

		i = 0;
		while (((PDWORD)(ImageBase + ImportDescriptor[j].FirstThunk))[i])
		{
			if (((PDWORD)(ImageBase + ImportDescriptor[j].OriginalFirstThunk))[i])
				dwRealIAT = ((PDWORD)(ImageBase + ImportDescriptor[j].OriginalFirstThunk))[i];
			else
				dwRealIAT = ((PDWORD)(ImageBase + ImportDescriptor[j].FirstThunk))[i];

			if (dwRealIAT & IMAGE_ORDINAL_FLAG32)	//	�Ƿ�Ϊ���
			{
				dwProcName = dwRealIAT & 65535;
				wsprintfA(pStr, "%d", dwProcName);
				pProcName = pStr;
			}
			else	//	����
				pProcName =		//	������
				(LPCSTR)(dwProcName = ImageBase + dwRealIAT + 2);

			if (pFR_CallBack)	//	�Ƿ���Ҫѯ�� ( �ص� )
				dwFunction = pFR_CallBack(pModuleName, pProcName, pFR_lParam);
			else
				dwFunction = 0;

			if (!dwFunction)	//	������ַ�Ƿ�Ϊ 0
				dwFunction = (DWORD)GetProcAddress(hModule, (LPCSTR)dwProcName);
			if (!dwFunction)
			{
				return FALSE;
			}

			((PDWORD)(ImageBase + ImportDescriptor[j].FirstThunk))[i] = dwFunction;

			i++;
		}
		j++;
	}

	return TRUE;
}

}
namespace {

	class MemoryModule
	{
	public:
		MemoryModule() = default;
		~MemoryModule() {
			if (hMode)
				_FreeLibrary(hMode);
			
		};
		auto MemoryLoadLibrary(const elibstl::ebin* pData) {
			if (hMode)
				_FreeLibrary(hMode);
		
			hMode = ::_LoadLibrary(pData->m_data, nullptr, 0);

			return hMode != NULL;
		}
		auto MemoryGetProcAddress(const char* fun)-> FARPROC {
			if (!hMode || !fun)
				return nullptr;
			return ::_GetProcAddress(hMode, fun);
		}
		auto MemoryCallEntryPoint() {
			if (!hMode)
				return DWORD{};
			return ::_GetEntryPoint(hMode);
		}

	private:
		LPBYTE hMode{ nullptr };
	};


}






static INT s_dtCmdIndexcommobj_MemoryModule_play[] = { 351,352,353,354,355,356 };



//����
EXTERN_C void fn_MemoryModule_structure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<MemoryModule>(pArgInf);
	self = new MemoryModule;
}
//����
FucInfo Fn_MemoryModule_structure = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/  _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} ,ESTLFNAME(fn_MemoryModule_structure) };
static ARG_INFO s_CopyArgs[] =
{
	{
		/*name*/    "����",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/   DATA_TYPE(6),
		/*default*/ 0,
		/*state*/   ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
//����
EXTERN_C void fn_MemoryModule_copy(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

	debug_put("�ڴ�Dll,����Ψһ��,�ôθ��Ʋ�����Ч");
}
FucInfo Fn_MemoryModule_copy = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		ESTLARG(s_CopyArgs)
	},ESTLFNAME(fn_MemoryModule_copy)};

//����
EXTERN_C void fn_MemoryModule_destruct(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<MemoryModule>(pArgInf);
	if (self)
	{
		self->~MemoryModule();
		operator delete(self);
	}
	self = nullptr;
}
FucInfo Fn_MemoryModule_destruct = { {
		/*ccname*/  "",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} ,ESTLFNAME(fn_MemoryModule_destruct) };

static ARG_INFO m_MemoryModule_arg[] =
{
	{
		/*name*/    "ģ��",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}

};
EXTERN_C void fn_MemoryModule_open(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<MemoryModule>(pArgInf);

	pRetData->m_bool = self->MemoryLoadLibrary(elibstl::args_to_ebin(pArgInf, 1));
}

FucInfo Fn_MemoryModule_LoadLibrary = { {
		/*ccname*/  ("����ģ��"),
		/*egname*/  ("LoadLibrary"),
		/*explain*/ R"(���ڴ���ؾ���ָ����С��EXE/DLL�ļ�.
ע��: ������ص�EXE�������,������Ҳ����ֹ)",
		/*category*/-1,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
	ESTLARG(m_MemoryModule_arg)
	} ,ESTLFNAME(fn_MemoryModule_open) };

static ARG_INFO m_MemoryModule_arg2[] =
{
	{
		/*name*/    "��������",
		/*explain*/ "��������ʱ������",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}
};

EXTERN_C void fn_MemoryModule_GetProcAddress(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<MemoryModule>(pArgInf);
	pRetData->m_pAryData = self->MemoryGetProcAddress(elibstl::args_to_sdata(pArgInf,1).data());
}
FucInfo Fn_MemoryModule_GetProcAddress = { {
		"ȡ����ָ��", "GetProcAddress", "�д�ģ�����л�ȡ�������ö���", -1, _CMD_OS(__OS_WIN), SDT_SUB_PTR, 0, LVL_SIMPLE, 0, 0, 
		ESTLARG(m_MemoryModule_arg2)
	} ,ESTLFNAME(fn_MemoryModule_GetProcAddress)};


EXTERN_C void fn_MemoryModule_CallEntryPoint(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<MemoryModule>(pArgInf);
	pRetData->m_int = self->MemoryCallEntryPoint();
}
FucInfo Fn_MemoryModule_CallEntryPoint = { {
		"ȡ��ڵ�ַ", "CallEntryPoint", R"(��ȡ��ڵ�(����EXE).ֻ�е�EXE�Ѽ��ص���ȷ�Ļ�ַ��������¶�λʱ(��������δ�������¶�λ��Ϣ),����ִ����ڵ�.
��Ҫ��ʾ:������ڵ�ַ���᷵��,��һ��ִ�е�EXE�������,���̽���ֹ.)", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, 0
} ,ESTLFNAME(fn_MemoryModule_CallEntryPoint) };

namespace elibstl {


	LIB_DATA_TYPE_INFO Obj_MemoryModule =
	{
		"�ڴ�ģ����",
		"MemoryModule",
		R"(�ڴ�ģ������һ����������ȫ���ڴ���� DLL \ COM \EXE ��PE�ļ����࣬�����ȴ洢�ڴ����ϡ�)",
		ESTLARG(s_dtCmdIndexcommobj_MemoryModule_play),
		_DT_OS(__OS_WIN),
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		0
	};
}