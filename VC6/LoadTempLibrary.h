#include "version_winos.h"
#include <string>
#include <vector>
#include <wincrypt.h>
#include <time.h>


// ������ʱĿ¼dll��, ֧�ֿ�ʹ��, ����д��dll��, ֧�ֿ����dll, ����ļ���vc6ʹ��, ��Ҫ����vc6
typedef LPCVOID(__cdecl* PFN_GetDllData)( int* size );
class CLoadTempLibrary
{
#define __ADD_L____1(s) L ## s
#define __ADD_L____2(s) __ADD_L____1(s)
#define __FILE__W__ __ADD_L____2(__FILE__)

private:
    HMODULE         hModule;        // ���ص�ģ����
    std::wstring    dllFile;        // ģ��·��, ����ʱ��ɾ������ļ�, ֻɾ��, �����Ƿ�ɾ���ɹ�
    PFN_GetDllData  pfn_GetDllData; // ��ȡdll���ݵĺ���
    LPCWSTR         dllName;        // д����dll����, ��ʱĿ¼ + dll����
public:
    CLoadTempLibrary() :hModule(0), pfn_GetDllData(0), dllName(0) { ; }
    void init(PFN_GetDllData pfn, LPCWSTR name)
    {
        dllName = name;
        pfn_GetDllData = pfn;
        if ( !dllName || !dllName[0] || !pfn_GetDllData )
            throw;  // ��֧�ִ��ݿ����ֺͿպ���
    }
    inline LPCWSTR GetDllFile()
    {
        return dllFile.c_str();
    }
    ~CLoadTempLibrary()
    {
        if ( !hModule )return;
        FreeLibrary(hModule);
        hModule = 0;
        DeleteFileW(dllFile.c_str());   // ɾ���ļ�, ����ɾ���Ƿ�ɹ�, �����ļ������ʹ��, �ǻ�ɾ��ʧ��
    }
    
    
    
    inline HMODULE GetModule(LPCVOID* pDll = 0, int* pSize = 0)
    {
        if (hModule)return hModule;
        wchar_t tempFile[260];
        dllFile.resize(260);
        GetTempPathW(260, tempFile);
        dllFile.assign(tempFile).append(dllName);

        bool reLoad = true;
        int size = 0;
        LPCVOID dll = pfn_GetDllData(&size);
        if ( pDll  ) *pDll = dll;
        if ( pSize ) *pSize = size;
        


        if ( fnPathFileExistsW(dllFile.c_str()) )
        {
            //TODO ������Ҫ�ж���ʱĿ¼��DLL�Ƿ���ʹ��, ���ǻ�ȡ��������ݻ��Ǽ���dll��ȡ�汾, �ж�MD5����򵥵ķ���
            
            // ����
            const DWORD dwDesiredAccess = GENERIC_READ;
            const DWORD dwCreationDisposition = OPEN_EXISTING;
            const DWORD dwShareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
            HANDLE hFile = CreateFileW(dllFile.c_str(),
                dwDesiredAccess,
                dwShareMode,
                0,
                dwCreationDisposition,
                FILE_ATTRIBUTE_NORMAL,
                0);

            if (hFile != INVALID_HANDLE_VALUE)
            {
                int oldSize = GetFileSize(hFile, NULL);
                if ( oldSize > 0 )
                {
                    LPBYTE pOldData = new BYTE[oldSize];
                    wchar_t newMd5[65], oldMd5[65];
                    DWORD dwNumOfByteRead = 0;
                    if (ReadFile(hFile, pOldData, size, &dwNumOfByteRead, 0))
                    {
                        _GetMd5(dll, size, newMd5);
                        _GetMd5(pOldData, oldSize, oldMd5);
                        if ( memcmp(newMd5, oldMd5, 16) == 0 )
                            reLoad = false;
                    }
                    delete[] pOldData;
                }
                CloseHandle(hFile);
            }
            // �����������ﲻд, ������ļ�ʧ��, ������ᴴ��һ���������, �ٴ�д��

        }

        if ( reLoad )
        {
            // ��д
            const DWORD dwDesiredAccess = GENERIC_WRITE;
            const DWORD dwCreationDisposition = CREATE_ALWAYS;
            const DWORD dwShareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);

            HANDLE hFile = CreateFileW(dllFile.c_str(),
                dwDesiredAccess,
                dwShareMode,
                0,
                dwCreationDisposition,
                FILE_ATTRIBUTE_NORMAL,
                0);

            if ( hFile == INVALID_HANDLE_VALUE)
            {
                // ��ʧ��, Ūһ���µ�����ļ���, Ȼ���ٴδ�
                srand((unsigned int)time(0));
                dllFile.append(randStr(20));
                hFile = CreateFileW(dllFile.c_str(),
                    dwDesiredAccess,
                    dwShareMode,
                    0,
                    dwCreationDisposition,
                    FILE_ATTRIBUTE_NORMAL,
                    0);
            }

            // ���� �� ����������, ��һ�����������Ͳ����ߵڶ�������, ��ʧ�ܾͲ���ִ��д��, �򿪳ɹ����ж�д���Ƿ�ɹ�
            DWORD h = 0;
            if (hFile == INVALID_HANDLE_VALUE || WriteFile(hFile, dll, size, &h, NULL) == FALSE)
            {
                // ��ʧ��, �ǾͲ�����������
                int err = GetLastError();
                CloseHandle(hFile);
                wchar_t str[260];
                const wchar_t* const fmtStr = L"û����ʱĿ¼��дȨ��, д���ļ�ʧ��\n���б�������Ҫ��ʱĿ¼��дȨ��, �볢���Թ���ԱȨ�޴򿪱�����\n������ = %d";
                swprintf(str, fmtStr, err);
                MessageBoxW(0, str, L"���������������", 0);
                TerminateProcess(GetCurrentProcess(), 0);
                exit(0);
                return 0;
            }

            CloseHandle(hFile);
        }
        hModule = LoadLibraryW(dllFile.c_str());
        return hModule;
    }


    // �Ӷ�̬�����ȡ������ַ
    inline void* GetAddrProc(LPCSTR name)
    {
        if (!GetModule()) return 0;
        return GetProcAddress(hModule, name);
    }

private:

    inline static BOOL _GetMd5(LPCVOID pData, int size, wchar_t MD5[33])
    {
        MD5[0] = 0;
        if ( !pData || !size ) return false;
        HCRYPTPROV hProv = NULL;
        HCRYPTPROV hHash = NULL;
        BOOL retVl = false;

        do
        {
            if ( !CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) )
                break;       //���CSP��һ����Կ�����ľ��

            //��ʼ������������hash, ����������һ����CSP��hash������صľ��������������������    CryptHashData���á�

            if ( !CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) )
                break;

            if ( !CryptHashData(hHash, (LPBYTE)pData, size, 0) )
                break;      //hash�ļ�  


            unsigned char* pbHash;
            DWORD dwHashLen = sizeof(DWORD);
            if ( !CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0) )
                break; //��Ҳ��֪��ΪʲôҪ����������CryptGetHashParam, ����ǲ��յ�msdn       

            pbHash = (unsigned char*)malloc(dwHashLen);
            if ( !pbHash )
                break;
            
            retVl = CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0);    //���md5ֵ 
            if ( retVl )
            {
                if ( dwHashLen >= 16 )
                {
                    memcpy(MD5, pbHash, 16);
                }
                else
                {
                    memcpy(MD5, pbHash, dwHashLen);
                    for ( DWORD i = dwHashLen; i < 16; i++ )
                        MD5[i] = 0;
                }
                MD5[16] = 0;
            }
            free(pbHash);
        } while ( false );

        //�ƺ���
        if ( hHash )
            CryptDestroyHash(hHash);
        if ( hProv )
            CryptReleaseContext(hProv, 0);

        return retVl;
    }
    
    inline bool check_dos_nt(LPBYTE pData)
    {
        if ( !pData ) return false;
        LPBYTE base = pData;
        PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pData;
        if ( dos->e_magic != IMAGE_DOS_SIGNATURE )
            return false;
        PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)( base + dos->e_lfanew );
        if ( nt->Signature != IMAGE_NT_SIGNATURE || ( nt->FileHeader.Machine & IMAGE_FILE_MACHINE_I386 ) == 0 )
            return false;
        return true;
    }

    inline LPBYTE MmMapFile(LPCVOID lpData, std::string& dllData)
    {
        LPBYTE base = (LPBYTE)lpData;
        if ( !check_dos_nt(base) )
            return 0;

        PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
        PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)( base + dos->e_lfanew );

        DWORD dwSizeOfHeaders = nt->OptionalHeader.SizeOfHeaders;   // ��ȡ����ͷ��+���α�Ĵ�С
        WORD wNumberOfSections = nt->FileHeader.NumberOfSections;   // ��ȡ��������
        PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(nt);   // ��ȡ���α��������Ԫ��
        dllData.resize(nt->OptionalHeader.SizeOfImage);
        LPBYTE lpBaseAddress = (LPBYTE)dllData.data();


        memcpy(lpBaseAddress, lpData, dwSizeOfHeaders); // ��ͷ��, �������α�, �������ڴ�
        LPVOID lpSrcMem = NULL;
        LPVOID lpDestMem = NULL;

        //ѭ��������������
        for ( WORD i = 0; i < wNumberOfSections; i++ )
        {
            //���˵���Ч����
            if ( 0 == pSection->VirtualAddress || 0 == pSection->SizeOfRawData )
            {
                pSection++;
                continue;
            }

            lpSrcMem = (LPVOID)( (DWORD)lpData + pSection->PointerToRawData );          // ��ȡ�������ļ��е�λ��
            lpDestMem = (LPVOID)( (DWORD)lpBaseAddress + pSection->VirtualAddress );    // ��ȡ����ӳ�䵽�ڴ��е�λ��

            memcpy(lpDestMem, lpSrcMem, pSection->SizeOfRawData);   // ���������ݿ������ڴ���

            pSection++; // ��ȡ��һ������ͷ
        }
        return lpBaseAddress;
    }

public:
    inline void chech_import(LPBYTE pData, int size, int err, std::string& dbgText, LPCWSTR dllFile)
    {
        std::string dllData;
        dbgText.reserve(0x1000);
        dbgText.clear();
        char fmtBuffer[1024];
        DRIVER_GETVERSION_STRUCT ver = { 0 };
        WIN_VER_DETAIL type = GetWindowsVersion(&ver);
        if ( type != WINDOWS_VERSION_NONE )
        {
            LPCSTR fmt = "����\"%s\"ʧ��\r\n������ = %d\r\n"
                "ϵͳ�汾: MajorVersion = %d, MinorVersion = %d, BuildNumber = %d, ProductType = %d\r\n"
                "����ϵͳ����Ϊ: %s\r\n��ϸʧ����Ϣ����:\r\n\r\n";
            if ( !dllFile || !dllFile[0] )
            {
                fmt = "����DLLʧ��\r\n"
                    "ϵͳ�汾: MajorVersion = %d, MinorVersion = %d, BuildNumber = %d, ProductType = %d\r\n"
                    "����ϵͳ����Ϊ: %s\r\n��ϸʧ����Ϣ����:\r\n\r\n";
                sprintf_s(fmtBuffer, fmt, ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber, ver.wProductType, GetVersionStringA(type));
 
            }
            else
            {
                std::string a = __W2A(dllFile);
                sprintf_s(fmtBuffer, fmt, a.c_str(), err,
                                     ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber, ver.wProductType, GetVersionStringA(type));
            }
            dbgText.append(fmtBuffer);

        }
        else if ( dllFile )
        {
            std::string a = __W2A(dllFile);
            sprintf_s(fmtBuffer,
                "����\"%s\"ʧ��\r\n������ = %d, ��ϸʧ����Ϣ����:\r\n\r\n",
                a.c_str(), err);
        }

        dbgText.append("��֧�ֿⲻ֧�ִ�ϵͳ, ȱ������dll����dll�еĺ���\r\n");
        LPBYTE base = MmMapFile(pData, dllData);
        if ( !base )
            return;

        PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
        PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)( base + dos->e_lfanew );
        PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)( base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );
        bool isFree = false;
        std::vector<std::string> arr;
        arr.reserve(100);
        arr.resize(0);

        while ( pImport->Name )
        {
            LPCSTR pDllName = (LPCSTR)( base + pImport->Name );
            if ( *pDllName == 0 )
            {
                pImport++;
                continue;
            }
            isFree = false;
            HMODULE hModule = GetModuleHandleA(pDllName);
            if ( !hModule )
            {
                hModule = LoadLibraryA(pDllName);
                isFree = hModule != 0;
            }


            arr.resize(0);
            PIMAGE_THUNK_DATA pIat = (PIMAGE_THUNK_DATA)( base + pImport->FirstThunk );
            //IAT�������ȷ�������һ����ȫΪ0��β
            while ( pIat->u1.Ordinal )
            {
                // �ж��Ƿ����Ƶ���������ŵ���
                // ͨ�����λ�Ƿ�Ϊ1, ���Ϊ1, ��ô��ŵ���
                // ���Ϊ0, ���Ƶ���
                LPCSTR pszFunName = 0;
                if ( pIat->u1.Ordinal & 0x80000000 )
                {
                    // ���λΪ1, ��ŵ���
                    pszFunName = (LPCSTR)( pIat->u1.Ordinal & 0xFFFF );
                }
                else
                {
                    //���λλ0, ���Ƶ���
                    //��ʾ���, ������
                    PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)( base + pIat->u1.AddressOfData );
                    pszFunName = (LPCSTR)&pName->Name[0];
                }

                if ( !hModule || GetProcAddress(hModule, pszFunName) == 0 )
                {
                    // ��ȡ�������ʧ��, ȱ���������, ȱ�ٵĲż��뵽������
                    const LPCSTR fmt = ( (DWORD)pszFunName <= 0xffff ? "��������: %d" : "%s" );
                    sprintf_s(fmtBuffer, fmt, pszFunName);
                    arr.push_back(fmtBuffer);
                }

                pIat++;
            }

            int count = arr.size();
            if ( count > 0 )
            {
                if ( !hModule )
                {
                    dbgText.append("ȱ��dll: ").append(pDllName).append("\r\n");
                }
                else
                {
                    dbgText.append("ȱ�� ").append(pDllName).append(" �е����º���:\r\n");

                    for ( int i = 0; i < count; i++ )
                    {
                        dbgText.append("        ").append(arr[i]).append("\r\n");
                    }
                }
                dbgText.append("\r\n");
                arr.resize(0);
            }

            pImport++;
        }

    }

private:
    inline static BOOL fnPathFileExistsW(LPCWSTR file)
    {
        typedef BOOL(WINAPI* PFN_PathFileExistsW)(LPCWSTR pszPath);
        static PFN_PathFileExistsW pFun;
        if (!pFun)
        {
            HMODULE hModule = LoadLibraryW(L"Shlwapi.dll");
            if (!hModule)
                return false;
            pFun = (PFN_PathFileExistsW)GetProcAddress(hModule, "PathFileExistsW");
            if (!pFun)
                return false;
        }
        return pFun(file);
    }

    // ȡָ������������ַ�
    static std::wstring randStr(const size_t _count)
    {
        std::wstring str;
        str.resize(_count);
        LPWSTR p = &str[0];

        const int bufferSize = 62;
        const LPCWSTR buffer = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        for (size_t i = 0; i < _count; i++)
        {
            int r = __rand(0, bufferSize);
            p[i] = buffer[r];
        }
        return str;
    }
    inline static int __rand(int min, int max)
    {
        return (::rand() % (max - min + 1)) + min;
    }
    inline static std::string __W2A(LPCWSTR str, int len = 0, int cp = 936)
    {
        if (!str)return std::string();
        if (len <= 0)len = (int)wcslen(str);
        if (len <= 0)return std::string();
        int aLen = WideCharToMultiByte(cp, 0, str, (int)len, 0, 0, 0, 0) + 1;
        if (aLen < 1) return std::string();

        std::string ret;
        ret.resize(aLen);
        LPSTR szStr = &ret[0];
        memset(szStr, 0, aLen);
        aLen = WideCharToMultiByte(cp, 0, str, (int)len, szStr, aLen, 0, 0);
        ret.resize(aLen);
        return ret;
    }
};

