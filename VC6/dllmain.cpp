// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "pch.h"
#include "elib/lib2.h"

#if _MSC_VER < 1201

#else

#endif



static IEKrnln g_call;


inline void init_call_dll()
{
    if ( g_call ) return;
    g_call = IextEntry();
}
#ifndef __E_STATIC_LIB
#define IEXT_DEF_STR(_index, _name, _remarks) (INT) ______E_FNENAME(IEXT_NAME(_index, _name)),
static INT s_names[] =
{
    2
};
#endif


// �����Ϊ�˾�̬������ôһ������, ����ȫ����dll����
// ��̬�������������, �����������Ϣ��ת��dll��
EXTERN_C INT WINAPI iext_ProcessNotifyLib_iext(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
    switch ( nMsg )
    {
#ifndef __E_STATIC_LIB
    // ������������ʵ�ֺ����ĵĺ�����������(char*[]), ֧�־�̬����Ķ�̬����봦��
    case NL_GET_CMD_FUNC_NAMES:
    {
        g_call->NotifyLib(nMsg, (DWORD)"QQ121007124__Group_20752843", (DWORD)s_names);
        return (INT)s_names;
    }
    // ���ش���ϵͳ֪ͨ�ĺ�������(PFN_NOTIFY_LIB��������), ֧�־�̬����Ķ�̬����봦��
    case NL_GET_NOTIFY_LIB_FUNC_NAME:
    {
        return (INT)"iext_ProcessNotifyLib_iext";
    }
    // ���ؾ�̬����������������̬���ļ����б�(��ʽΪ\0�ָ����ı�,��β����\0), ֧�־�̬����Ķ�̬����봦��
    // kernel32.lib user32.lib gdi32.lib �ȳ��õ�ϵͳ�ⲻ��Ҫ���ڴ��б���
    // ����NULL��NR_ERR��ʾ��ָ�������ļ�
    case NL_GET_DEPENDENT_LIBS:
    {
        return (INT)"\0\0";
    }
#endif

    // ��֪��֪ͨϵͳ�õĺ���ָ��,��װ��֧�ֿ�ǰ֪ͨ,�����ж��,��֪ͨ��ֵӦ�ø���ǰ����֪ͨ��ֵ,���Է���ֵ
    // ��ɽ��˺���ָ���¼�����Ա�����Ҫʱʹ����֪ͨ��Ϣ��ϵͳ
    // dwParam1: (PFN_NOTIFY_SYS)
    case NL_SYS_NOTIFY_FUNCTION:
    {
        init_call_dll();
        break;
    }
    default:
        break;
    }
    return g_call->NotifyLib(nMsg, dwParam1, dwParam2);
    return 0;
}



inline static void iext_CallDllFun_iext(int index, PVOID pRetData, INT nArgCount, PVOID pArgInf)
{
    //g_call->CallFun(index, pRetData, nArgCount, pArgInf);
}

#define IEXT_DEF_CALL(_index, _name, _remarks) \
    extern "C" void IEXT_NAME(_index, _name)(PVOID pRetData, INT nArgCount, PVOID pArgInf)\
    {\
        iext_CallDllFun_iext(_index, pRetData, nArgCount, pArgInf);\
    }
#define IEXT_DEF_INTERFACE(_name) extern "C" PVOID __stdcall iext_GetInterface_##_name(INT nInterfaceNO) { return g_call->GetInterface_##_name(nInterfaceNO); }


//! ����ļ���Ҫ���þ�̬���е�������, ��Щ����ǵ���dllʵ��
//! ��dll������Щ���������鷵�ظ�������
//IEXT_DEF(IEXT_DEF_CALL)             // ���е�����, ���ǵ���dll��ʵ��, �Ѻ��������Ͳ������ݵ�dll��, ��dll����
//IEXT_DEF_INTERFACE(TreeBox)         // ���Ϳ������ӿں���
//IEXT_DEF_INTERFACE(StatusBar)       // ״̬��������ӿں���
//IEXT_DEF_INTERFACE(ToolBar)         // ������������ӿں���
//IEXT_DEF_INTERFACE(ListView)        // ���е�����ӿں���
//IEXT_DEF_INTERFACE(TransLabel)      // ͸����ǩ������ӿں���

#ifndef __E_STATIC_LIB

//����Offset
inline ULONG RvaToOffset(PIMAGE_NT_HEADERS pNtHeader, ULONG Rva)
{
    //PE��
    PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)( pNtHeader + 1 );

    ULONG sNum, i;
    //ȡ�ýڱ�����Ŀ
    sNum = pNtHeader->FileHeader.NumberOfSections;
    for ( i = 0; i < sNum; i++ )
    {
        //printf("PE ������: %s\n",p_section_header->Name);
        if ( ( pSection->VirtualAddress <= Rva ) && Rva < ( pSection->VirtualAddress + pSection->SizeOfRawData ) )
        {
            return Rva - pSection->VirtualAddress + pSection->PointerToRawData;
        }
        pSection++;
    }
    return 0;
}

// ��ȡ�����ַ�ǵ�ǰ�����ĸ�ģ����ĵ�ַ, ��������ע����Ϣ/�¼���ջ���������
inline const char* GetDllNameFromAddress(LPCVOID pAddress)
{
    if ( !pAddress )
        return 0;
    MEMORY_BASIC_INFORMATION mem;
    BOOL isOk = VirtualQuery(pAddress, &mem, sizeof(MEMORY_BASIC_INFORMATION));
    LPBYTE pCmp = (LPBYTE)mem.AllocationBase;
    if ( !isOk ) return 0;

    static char runFile[260];
    runFile[0] = 0;
    const int fileBuffer = sizeof(runFile) / sizeof(runFile[0]);
    GetModuleFileNameA((HMODULE)pCmp, runFile, fileBuffer);   // �Ȼ�ȡ����������·��
    if ( runFile[0] )
    {
        char* pos = strrchr(runFile, '\\');
        if ( pos )
        {
            *pos++ = 0;
            return pos;
        }
        return runFile;
    }

    if ( pCmp[0] != 'M' || pCmp[1] != 'Z' )
        return 0;

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)pCmp;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)( pCmp + dosHeader->e_lfanew );

    ULONG offsetExportTable = RvaToOffset(nt, nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);    // ������ƫ��
    PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)( pCmp + offsetExportTable );   // ������
    if ( !offsetExportTable ) return 0;

    char* dllName = (char*)( pCmp + RvaToOffset(nt, pExportTable->Name) );
    return dllName;
}

EXTERN_C void* WINAPI GetNewInf()
{
    //! ��̬���벻��������, �����������ȡ��exe���ļ���
    LPCSTR name = GetDllNameFromAddress(GetNewInf);
    //if ( stricmp(name, "iext.fne") != 0 && stricmp(name, "iext.fnr") != 0 )
    //{
    //    MessageBoxW(0, L"��⵽֧�ֿ��ļ����ѱ��޸�, ���β������֧�ֿ�\r\n��֧�ֿ�ֻ�����ļ���Ϊ iext.fne", L"��չ����֧�ֿ�һEx", MB_ICONERROR);
    //    return 0;
    //}
    init_call_dll();
    if ( !g_call )return 0;
#define IEXT_INTERFACE_NAME(_name) (int)iext_GetInterface_##_name
#ifdef __E_STATIC_LIB
    static bool isStatic = true;
#else
    static bool isStatic = false;
#endif

    //PLIB_INFO info = (PLIB_INFO)g_call->GetNewInf(iext_ProcessNotifyLib_iext, &isStatic);


    //int fn[5][2] =
    //{
    //    { IEXT_INTERFACE_NAME(TreeBox),    IDICON_TREEVIEW },
    //    { IEXT_INTERFACE_NAME(StatusBar),  IDICON_STATUSBAR },
    //    { IEXT_INTERFACE_NAME(ToolBar),    IDICON_TOOLBAR },
    //    { IEXT_INTERFACE_NAME(ListView),   IDICON_LISTVIEW },
    //    { IEXT_INTERFACE_NAME(TransLabel), IDICON_TRANSPARENTSTATIC },
    //};

    //// �޸����֪ͨ�ص������ͼ������
    //// ��Ϊ�����Լ��ص������֧�ֿ�, ������Ҫ�������֧�ֿ����ԴID
    //for ( int i = 0; i < 5; i++ )
    //{
    //    info->m_pDataType[i].m_pfnGetInterface = (PFN_GET_INTERFACE)fn[i][0];
    //    info->m_pDataType[i].m_dwUnitBmpID = fn[i][1];
    //}

    //return info;
    return 0;
}

#endif

