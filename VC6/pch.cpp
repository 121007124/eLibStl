// pch.cpp: ��Ԥ�����ͷ��Ӧ��Դ�ļ�

#include "pch.h"
#include "LoadTempLibrary.h"
#include <EKrnln_Version.h>

//#define __MEM_LOAD_DLL__    // ��Ҫ������ڴ���þʹ������
#define _DBG_LOCAL          // ���ص���, һ�����ʱʹ��, ֱ�ӵ��ñ��ص�dll



// ��ʹ��Ԥ�����ͷʱ����Ҫʹ�ô�Դ�ļ���������ܳɹ���
typedef IEKrnln( WINAPI* PFN_CreateEKrnlnInterface)( HMODULE hCurDllModule );

#ifdef __MEM_LOAD_DLL__
#define IEXT_SPACENAME  iext_space_mmloaddll
#define IEXT_SPACENAME_BEGIN  namespace IEXT_SPACENAME {
#define IEXT_SPACENAME_END  }

IEXT_SPACENAME_BEGIN

//************************************
// ������:  LoadLibrary
// ��������:   LPVOID
// ����: ģ��LoadLibrary�����ڴ��ļ���������
// ����1: LPVOID lpData    �ļ���ַ
// ����2: BOOL IsExe    �ļ����Ա�־��TRUE��ʾexe�ļ���FALSE��ʾdll�ļ�
//************************************
HMODULE MmLoadLibrary(LPCVOID lpData);

// ģ��GetProcAddress��ȡ�ڴ�DLL�ĵ�������
// lpBaseAddress: �ڴ�DLL�ļ����ص������еļ��ػ�ַ
// lpszFuncName: ��������������
// ����ֵ: ���ص��������ĵĵ�ַ
FARPROC MmGetProcAddress(HMODULE lpBaseAddress, LPCSTR lpszFuncName);

// �ͷŴ��ڴ���ص�DLL�������ڴ�Ŀռ�
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL MmFreeLibrary(HMODULE lpBaseAddress);

IEXT_SPACENAME_END

#endif


static inline LPCVOID GetELibStlDllData(int* size)
{

    static LPBYTE dll;
    static int retSize;
    if ( dll )
    {
        if ( size )
            *size = retSize;
        return dll;
    }

#if defined(__COMPILE_FNR) || defined(__E_STATIC_LIB)
    dll = GetELibStlFnrDllData(&retSize);
#else
    dll = GetELibStlFneDllData(&retSize);
#endif
    if ( size )*size = retSize;
    return dll;
}

static CLoadTempLibrary s_dll;


static inline PFN_CreateEKrnlnInterface GetEKrnlnEntry(HMODULE hModule, LPCVOID pData, int size, LPCWSTR dllFile)
{
    if ( !hModule )
    {
        std::string caption, text;
        char fmtText[1024];
        sprintf_s(fmtText, "������չ�����һ %d.%d.%d", EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION);
        s_dll.chech_import((LPBYTE)pData, size, GetLastError(), text, dllFile);
        MessageBoxA(0, text.c_str(), caption.c_str(), MB_ICONERROR);
        exit(0);
        TerminateProcess(GetCurrentProcess(), 0);
        return 0;
    }
    PFN_CreateEKrnlnInterface pfn = (PFN_CreateEKrnlnInterface)GetProcAddress(hModule, "CreateIextInterface");
    if ( !pfn )
    {
        std::wstring caption;
        wchar_t fmtText[1024];

        swprintf_s(fmtText, L"������չ�����һ %d.%d.%d", EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION);
        caption = fmtText;
        MessageBoxW(0, L"���ظ�����չ�����һExʧ��, ��ȡ֧�ֿ���Ϣʧ��\r\n"
                    L"��ȷ��֧�ֿ��ļ��Ƿ��޸�\r\n"
                    L"��֧�ֿⲻ֧��xp, server2003, �Լ����°汾��ϵͳ", caption.c_str(), MB_ICONERROR);
        exit(0);
        TerminateProcess(GetCurrentProcess(), 0);
        return 0;
    }
    return pfn;
}


#if defined(_DBG_LOCAL)
// �����ļ�, ��֧�ֳ���4G���ļ�
inline int readFile(LPCWSTR file, std::string& data)
{
    FILE* f;
    errno_t err = _wfopen_s(&f, file, L"rb+");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    data.resize(size);
    size = (long)fread_s(&data[0], size, 1, size, f);

    fclose(f);
    return size;
}

// �����ļ�, ��֧�ֳ���4G���ļ�
inline int writeFile(LPCWSTR file, LPCVOID pData, int size)
{
    FILE* f;
    errno_t err = _wfopen_s(&f, file, L"wb+");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_SET);
    int ret = (int)fwrite(pData, size, 1, f);
    fclose(f);
    return ret;
}

#endif

IEKrnln IextEntry()
{
#if defined(__COMPILE_FNR) || defined(__E_STATIC_LIB)
    s_dll.init(GetIextDllData, L"iext1.fnr.bbs.125.la");
#else
    s_dll.init(GetELibStlDllData, L"iext1.fne.bbs.125.la");
#endif

    int size = 0;
    PFN_CreateEKrnlnInterface pfn = 0;
    HMODULE hModule = 0;
    LPCVOID dll = 0;
    GetELibStlDllData(0);


#if defined(_DBG_LOCAL)
    const LPCWSTR _dll_file = L"I:\\cpp\\_git_works\\iext\\����������Դ��\\iext_fne.dll";
    //const LPCWSTR _dll_file = L"C:\\Users\\n\\Desktop\\666\\iext_fne.dll";
    std::string _dll_data;
    readFile(_dll_file, _dll_data);
    hModule = LoadLibraryW(_dll_file);
    //hModule = LoadLibraryW(L"c:\\iext_fne.dll");
    pfn = GetEKrnlnEntry(hModule, _dll_data.c_str(), (int)_dll_data.size(), _dll_file);
#elif defined(__MEM_LOAD_DLL__)
    dll = GetIextDllData(&size);
    hModule = IEXT_SPACENAME::MmLoadLibrary(dll);
    if ( !hModule )
    {
        _str caption, text;
        caption.Format("������չ�����һ %d.%d.%d", IEXT_MAJORVERSION, IEXT_MINORVERSION, IEXT_BUILDVERSION);
        s_dll.chech_import((LPBYTE)dll, size, GetLastError(), text, 0);
        MessageBoxA(0, text.c_str(), caption.c_str(), MB_ICONERROR);
        exit(0);
        TerminateProcess(GetCurrentProcess(), 0);
        return 0;
    }

    pfn = (PFN_CreateIextInterface)IEXT_SPACENAME::MmGetProcAddress(hModule, "CreateIextInterface");

    if ( !pfn )
    {
        // ����ʧ�ܾ��ͷŵ���ʱĿ¼, Ȼ����������
        hModule = s_dll.GetModule(&dll, &size);
        pfn = GetIextEntry(hModule, dll, size, s_dll.GetDllFile());
        if ( !pfn )
            return 0;
    }

    if ( !pfn )
    {
        wstr caption;
        caption.Format(L"������չ�����һ %d.%d.%d", IEXT_MAJORVERSION, IEXT_MINORVERSION, IEXT_BUILDVERSION);
        MessageBoxW(0, L"���ظ�����չ�����һExʧ��, ��ȡ֧�ֿ���Ϣʧ��\r\n"
                    L"��ȷ��֧�ֿ��ļ��Ƿ��޸�\r\n"
                    L"��֧�ֿⲻ֧��xp, server2003, �Լ����°汾��ϵͳ", caption.c_str(), MB_ICONERROR);
        exit(0);
        TerminateProcess(GetCurrentProcess(), 0);
        return 0;
    }
#else
    //dll = GetIextDllData(&size);

    //hModule = (HMODULE)MemoryLoadLibrary(dll);
    //if(hModule )
    //    pfn = (PFN_GetNewInf)MemoryGetProcAddress(hModule, "CreateIextInterface");

    //if ( !pfn )
    //{
    //    hModule = MmLoadLibrary(dll);
    //    if ( hModule )  // ���ڴ����
    //        pfn = (PFN_GetNewInf)MmGetProcAddress(hModule, "CreateIextInterface");
    //}
    if ( !pfn )
    {
        // ����ʧ�ܾ��ͷŵ���ʱĿ¼, Ȼ����������
        hModule = s_dll.GetModule(&dll, &size);
        LPCWSTR _dll_file = s_dll.GetDllFile();
        pfn = GetIextEntry(hModule, dll, size, _dll_file);
    }
#endif

    if ( !pfn )
        return 0;

    

    HMODULE hBase = 0;
    MEMORY_BASIC_INFORMATION mem = { 0 };
    if ( VirtualQuery(IextEntry, &mem, sizeof(mem)) )
    {
        hBase = (HMODULE)mem.AllocationBase;
    }
    if ( !hBase )
        hBase = GetModuleHandleW(0);

    return pfn(hBase);
}


#ifdef __MEM_LOAD_DLL__


IEXT_SPACENAME_BEGIN

#if 0
#define MSGBOX_SHOW(_0, _1, _2, _3) MessageBoxW(_0, _1, _2, _3)
#else
#define MSGBOX_SHOW(_0, _1, _2, _3) 
#endif

DWORD GetSizeOfImage(LPCVOID lpData);
BOOL MmMapFile(LPCVOID lpData, LPVOID lpBaseAddress);
BOOL DoRelocationTable(LPVOID lpBaseAddress);
BOOL DoImportTable(LPVOID lpBaseAddress);
BOOL SetImageBase(LPVOID lpBaseAddress);
BOOL CallDllMain(LPCVOID lpBaseAddress, BOOL IsExe);



//************************************
// ������:  LoadLibrary
// ��������:   LPVOID
// ����: ģ��LoadLibrary�����ڴ��ļ���������
// ����1: LPVOID lpData    �ļ���ַ
// ����2: BOOL IsExe    �ļ����Ա�־��TRUE��ʾexe�ļ���FALSE��ʾdll�ļ�
//************************************
HMODULE MmLoadLibrary(LPCVOID lpData)
{

    HMODULE lpBaseAddress = NULL;
    // ��ȡӳ���С
    DWORD dwSizeOfImage = GetSizeOfImage(lpData);
    // �ڽ���������һ���ɶ�����д����ִ�е��ڴ��
    lpBaseAddress = (HMODULE)VirtualAlloc((LPVOID)0, dwSizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if ( NULL == lpBaseAddress )
    {
        MSGBOX_SHOW(0, L"����ռ�ʧ�ܣ�", 0, 0);
        return NULL;
    }
    // ������Ŀռ������ȫ����0
    RtlZeroMemory(lpBaseAddress, dwSizeOfImage);
    // ���ڴ�DLL���ݰ�ӳ������С��SectionAlignment��ӳ�䵽�ո�������ڴ���
    if ( FALSE == MmMapFile(lpData, lpBaseAddress) )
    {
        MSGBOX_SHOW(0, L"����ӳ�䵽�ڴ�ʧ��!", 0, 0);
        return NULL;
    }


    // �޸�PE�ļ����ض�λ����Ϣ
    if ( FALSE == DoRelocationTable(lpBaseAddress) )
    {
        MSGBOX_SHOW(0, L"�޸��ض�λʧ��!", 0, 0);
        return NULL;
    }


    // ��дPE�ļ��ĵ������Ϣ
    if ( FALSE == DoImportTable(lpBaseAddress) )
    {
        MSGBOX_SHOW(0, L"�������дʧ��!", 0, 0);
        return NULL;
    }
    // �޸�PE�ļ��ļ��ػ�ַIMAGE_NT_HEADERS.OptionalHeader.ImageBase
    if ( FALSE == SetImageBase(lpBaseAddress) )
    {
        MSGBOX_SHOW(0, L"�޸ļ��ػ���ʧ��!", 0, 0);
        return NULL;
    }
    // ����DLL����ں���DllMain,������ַ��ΪPE�ļ�����ڵ�AddressOfEntryPoint
    if ( FALSE == CallDllMain(lpBaseAddress, false) )
    {
        MSGBOX_SHOW(0, L"������ں���ʧ��!", 0, 0);
        return NULL;
    }
    return (HMODULE)lpBaseAddress;
}


// ģ��GetProcAddress��ȡ�ڴ�DLL�ĵ�������
// lpBaseAddress: �ڴ�DLL�ļ����ص������еļ��ػ�ַ
// lpszFuncName: ��������������
// ����ֵ: ���ص��������ĵĵ�ַ
FARPROC MmGetProcAddress(HMODULE lpBaseAddress, LPCSTR lpszFuncName)
{
    FARPROC lpFunc = NULL;
    // ��ȡ������
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG32)pDosHeader + pDosHeader->e_lfanew );
    PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)( (DWORD)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );
    // ��ȡ�����������
    PDWORD lpAddressOfNamesArray = (PDWORD)( (DWORD)pDosHeader + pExportTable->AddressOfNames );
    PCHAR lpFuncName = NULL;
    PWORD lpAddressOfNameOrdinalsArray = (PWORD)( (DWORD)pDosHeader + pExportTable->AddressOfNameOrdinals );
    WORD wHint = 0;
    PDWORD lpAddressOfFunctionsArray = (PDWORD)( (DWORD)pDosHeader + pExportTable->AddressOfFunctions );
    DWORD dwNumberOfNames = pExportTable->NumberOfNames;
    DWORD i = 0;
    // ����������ĵ�������������, ������ƥ��
    for ( i = 0; i < dwNumberOfNames; i++ )
    {
        lpFuncName = (PCHAR)( (DWORD)pDosHeader + lpAddressOfNamesArray[i] );
        if ( 0 == ::_stricmp(lpFuncName, lpszFuncName) )
        {
            // ��ȡ����������ַ
            wHint = lpAddressOfNameOrdinalsArray[i];
            lpFunc = (FARPROC)( (DWORD)pDosHeader + lpAddressOfFunctionsArray[wHint] );
            break;
        }
    }
    return lpFunc;
}

// �ͷŴ��ڴ���ص�DLL�������ڴ�Ŀռ�
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL MmFreeLibrary(HMODULE lpBaseAddress)
{
    BOOL bRet = FALSE;
    if ( NULL == lpBaseAddress )
        return bRet;
    bRet = ::VirtualFree(lpBaseAddress, 0, MEM_RELEASE);
    lpBaseAddress = NULL;
    return bRet;
}



//************************************
// ������:  GetSizeOfImage
// ��������:   DWORD
// ����: ��ȡ�ļ�ӳ���С
// ����1: LPVOID lpData    �ļ���ַ
//************************************
DWORD GetSizeOfImage(LPCVOID lpData)
{
    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
    //�ж��Ƿ�����Ч��PE�ļ�        0x5A4D
    if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
    {
        MSGBOX_SHOW(0, L"�ⲻ��һ��PE�ļ�!", 0, 0);
        return 0;
    }

    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );
    //�ж��Ƿ�����Ч��PE�ļ�        0x00004550
    if ( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
    {
        MSGBOX_SHOW(0, L"�ⲻ��һ��PE�ļ�!", 0, 0);
        return 0;
    }

    //��ȡ�ļ�ӳ���С
    return pNtHeader->OptionalHeader.SizeOfImage;
}



//************************************
// ������:  MmMapFile
// ��������:   BOOL
// ����: ���ڴ�DLL���ݰ�ӳ������С��SectionAlignment��ӳ�䵽�ո�������ڴ���
// ����1: LPVOID lpData    �ļ���ַ
// ����2: LPVOID lpBaseAddress    ������ڴ���׵�ַ
//************************************
BOOL MmMapFile(LPCVOID lpData, LPVOID lpBaseAddress)
{
    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );
    //��ȡ����ͷ��+���α�Ĵ�С
    DWORD dwSizeOfHeaders = pNtHeader->OptionalHeader.SizeOfHeaders;
    //��ȡ��������
    WORD wNumberOfSections = pNtHeader->FileHeader.NumberOfSections;
    //��ȡ���α��������Ԫ��
    PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
    //��ͷ�����������α��������ڴ�
    RtlCopyMemory(lpBaseAddress, lpData, dwSizeOfHeaders);
    LPVOID lpSrcMem = NULL;
    LPVOID lpDestMem = NULL;
    DWORD dwSizeOfRawData = 0;
    const  int asdas = offsetof(IMAGE_SECTION_HEADER, Characteristics);
    //ѭ��������������
    for ( WORD i = 0; i < wNumberOfSections; i++ )
    {
        //���˵���Ч����
        if ( 0 == pSectionHeader->VirtualAddress || 0 == pSectionHeader->SizeOfRawData )
        {
            pSectionHeader++;
            continue;
        }
        //��ȡ�������ļ��е�λ��
        lpSrcMem = (LPVOID)( (DWORD)lpData + pSectionHeader->PointerToRawData );
        //��ȡ����ӳ�䵽�ڴ��е�λ��
        lpDestMem = (LPVOID)( (DWORD)lpBaseAddress + pSectionHeader->VirtualAddress );
        //��ȡ�������ļ��еĴ�С
        dwSizeOfRawData = pSectionHeader->SizeOfRawData;
        //���������ݿ������ڴ���
        RtlCopyMemory(lpDestMem, lpSrcMem, dwSizeOfRawData);
        //��ȡ��һ������ͷ�����ԣ�
        pSectionHeader++;
    }
    return TRUE;
}


//************************************
// ������:  DoRelocationTable
// ��������:   BOOL
// ����: �޸�PE�ļ����ض�λ����Ϣ
// ����1: LPVOID lpBaseAddress    ӳ��������ļ���ַ
//************************************
BOOL DoRelocationTable(LPVOID lpBaseAddress)
{
    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );
    //��ȡ�ض�λ��ĵ�ַ
    PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)( (DWORD)pDosHeader +
                                                             pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress );

    //ע���ض�λ���λ�ÿ��ܺ�Ӳ���ļ��е�ƫ�Ƶ�ַ��ͬ��Ӧ��ʹ�ü��غ�ĵ�ַ

    //�ж��Ƿ����ض�λ��
    if ( (PVOID)pReloc == (PVOID)pDosHeader )
    {
        //û���ض�λ��
        return TRUE;
    }

    int nNumberOfReloc = 0;
    WORD* pRelocData = NULL;
    DWORD* pAddress = NULL;
    //��ʼ�޸��ض�λ
    while ( pReloc->VirtualAddress != 0 && pReloc->SizeOfBlock != 0 )
    {
        //���㱾����ÿһ��������4KB��С��������ض�λ��Ϣ����Ҫ�������ض�λ�������
        nNumberOfReloc = ( pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION) ) / sizeof(WORD);

        for ( int i = 0; i < nNumberOfReloc; i++ )
        {
            //��ȡIMAGE_BASE_RELOCATION�ṹ��������ݵĵ�ַ
            pRelocData = (WORD*)( (DWORD)pReloc + sizeof(IMAGE_BASE_RELOCATION) );

            //ÿ��WORD����������ɣ���4λָ�����ض�λ�����ͣ�WINNT.H�е�һϵ��IMAGE_REL_BASED_xxx�������ض�λ���͵�ȡֵ��
            //�󲿷��ض�λ����ֵ����0x3
            //��12λ�������IMAGE_BASE_RELOCATION�е�һ��Ԫ��VirtualAddress����λ�õ�ƫ��
            //�ҳ���Ҫ�����ĵ�ַ
            if ( (WORD)( pRelocData[i] & 0xF000 ) == 0x3000 )
            {
                //��ȡ��Ҫ�������ݵĵ�ַ,    ��λ��������ȼ��ȼӼ��˳���
                int offset = (int)pDosHeader - (int)pNtHeader->OptionalHeader.ImageBase;
                DWORD addrOffset = pReloc->VirtualAddress + ( pRelocData[i] & 0x0FFF );
                pAddress = (DWORD*)( (DWORD)pDosHeader + addrOffset );
                //�����޸�
                *pAddress += (DWORD)offset;
            }

        }
        //��һ���ض�λ��
        pReloc = (PIMAGE_BASE_RELOCATION)( (DWORD)pReloc + pReloc->SizeOfBlock );
    }
    return TRUE;
}



//************************************
// ������:  DoImportTable
// ��������:   BOOL
// ����: ��дPE�ļ��ĵ������Ϣ
// ����1: LPVOID lpBaseAddress    ӳ��������ļ���ַ
//************************************
BOOL DoImportTable(LPVOID lpBaseAddress)
{
    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );
    //��ȡ������ַ
    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)( (DWORD)pDosHeader +
                                                                  pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );

    char* pDllName = 0;
    HMODULE hDll = NULL;
    PIMAGE_THUNK_DATA pIat = NULL;
    FARPROC pFuncAddress = NULL;
    PIMAGE_IMPORT_BY_NAME pImportByName = NULL;

    //ѭ�����������
    while ( pImport->Name )
    {
        //��ȡ������е�Dll����
        pDllName = (char*)( (DWORD)pDosHeader + pImport->Name );
        //����Dllģ���ȡģ����
        hDll = GetModuleHandleA(pDllName);
        //��ȡʧ��
        if ( NULL == hDll )
        {
            //����Dllģ���ȡģ����
            hDll = LoadLibraryA(pDllName);
            //����ʧ��
            if ( NULL == hDll )
            {
                pImport++;
                continue;
            }
        }

        //��ȡIAT
        pIat = (PIMAGE_THUNK_DATA)( (DWORD)pDosHeader + pImport->FirstThunk );

        //����IAT�к���
        while ( pIat->u1.Ordinal )
        {
            //�жϵ���ĺ��������Ƶ��뻹����ŵ���
            //�ж����λ�Ƿ�Ϊ1�������1��ô����ŵ���
            if ( pIat->u1.Ordinal & 0x80000000 )
            {
                //��ȡ������ַ
                pFuncAddress = GetProcAddress(hDll, (LPCSTR)( pIat->u1.Ordinal & 0x7FFFFFFF ));

            }
            //���Ƶ���
            else
            {
                //��ȡIMAGE_IMPORT_BY_NAME�ṹ
                pImportByName = (PIMAGE_IMPORT_BY_NAME)( (DWORD)pDosHeader + pIat->u1.AddressOfData );
                //��ȡ������ַ
                pFuncAddress = GetProcAddress(hDll, (LPCSTR)pImportByName->Name);
            }
            //��������ַ���뵽IAT��
            pIat->u1.Function = (DWORD)pFuncAddress;
            pIat++;
        }
        pImport++;
    }

    return TRUE;
}




//************************************
// ������:  SetImageBase
// ��������:   BOOL
// ����: �޸�PE�ļ��ļ��ػ�ַIMAGE_NT_HEADERS.OptionalHeader.ImageBase
// ����1: LPVOID lpBaseAddress    ӳ��������ļ���ַ
//************************************
BOOL SetImageBase(LPVOID lpBaseAddress)
{
    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );
    //�޸�Ĭ�ϼ��ػ�ַ
    pNtHeader->OptionalHeader.ImageBase = (DWORD)lpBaseAddress;
    return TRUE;
}



//************************************
// ������:  CallDllMain
// ��������:   BOOL
// ����: ����PE�ļ�����ں���
// ����1: LPVOID lpBaseAddress    ӳ��������ļ���ַ
// ����2: BOOL IsExe    �ļ����Ա�־��TRUE��ʾexe�ļ���FALSE��ʾdll�ļ�
//************************************
BOOL CallDllMain(LPCVOID lpBaseAddress, BOOL IsExe)
{
    typedef BOOL(APIENTRY* typedef_DllMain)( HMODULE hModule,
                                            DWORD  ul_reason_for_call,
                                            LPVOID lpReserved
                                            );
    //���庯��ָ�����
    typedef_DllMain DllMain = NULL;
    //typedef_wWinMain MyWinMain = NULL;

    //��ȡDosͷ
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
    //��ȡNTͷ
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (DWORD)pDosHeader + pDosHeader->e_lfanew );

    BOOL bRet = TRUE;
    //�����exe�ļ�
    if ( IsExe )
    {
        //MSGBOX_SHOW(0, L"�����⣬�����", 0, 0);
        //MyWinMain = (typedef_wWinMain)((DWORD)pDosHeader + pNtHeader->OptionalHeader.AddressOfEntryPoint+0xF8D);
        //bRet = MyWinMain((HINSTANCE)lpBaseAddress, NULL, NULL, SW_SHOWNORMAL);
    }
    //dll �ļ�
    else
    {
        DllMain = (typedef_DllMain)( (DWORD)pDosHeader + pNtHeader->OptionalHeader.AddressOfEntryPoint );
        //������ں��������ӽ���DLL_PROCESS_ATTACH
        bRet = DllMain((HINSTANCE)lpBaseAddress, DLL_PROCESS_ATTACH, NULL);
    }
    return bRet;
}

IEXT_SPACENAME_END

#endif