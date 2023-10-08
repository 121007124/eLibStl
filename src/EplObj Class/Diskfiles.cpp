#pragma once
#include<string>
#include <vector>
#include <cstring>
#include <cstdlib>

#include"EcontrolHelp.h"


#ifdef _WIN32
namespace elibstl {
	class CFile{
		HANDLE m_hFile{ INVALID_HANDLE_VALUE };
	public:
		BOOL Open(const std::wstring_view& filename,int openmode,int sharemod) {
			Close(); // ���ȹر��Ѿ��򿪵ĵ�ǰ�ļ�
			if (filename.empty())
				return FALSE;
			 DWORD dwAccess = 0;
			 DWORD dwCreateFlag = 0;
			 switch (openmode)
			 {
			 case 1:  dwCreateFlag = OPEN_EXISTING;  dwAccess = GENERIC_READ;  break;
			 case 2:  dwCreateFlag = OPEN_EXISTING;  dwAccess = GENERIC_WRITE;  break;
			 case 3:  dwCreateFlag = OPEN_EXISTING;  dwAccess = (GENERIC_READ | GENERIC_WRITE);  break;
			 case 4:  dwCreateFlag = CREATE_ALWAYS;  dwAccess = GENERIC_WRITE;  break;
			 case 5:  dwCreateFlag = OPEN_ALWAYS;  dwAccess = GENERIC_WRITE;  break;
			 case 6:  dwCreateFlag = OPEN_ALWAYS;  dwAccess = (GENERIC_READ | GENERIC_WRITE);  break;
			 default:  return FALSE;
			 }
			 DWORD dwShareMode = 0;
			 switch (sharemod)
			 {
			 case 1:  dwShareMode = (FILE_SHARE_WRITE | FILE_SHARE_READ);  break;
			 case 2:  dwShareMode = FILE_SHARE_WRITE;  break;
			 case 3:  dwShareMode = FILE_SHARE_READ;  break;
			 case 4:  dwShareMode = 0;  break;
			 default:  return FALSE;
			 }
			 SECURITY_ATTRIBUTES sa;
			 sa.nLength = sizeof(sa);
			 sa.lpSecurityDescriptor = NULL;
			 sa.bInheritHandle = TRUE;
			 m_hFile = ::CreateFileW(filename.data(), dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
			 if (m_hFile == INVALID_HANDLE_VALUE)
				     return FALSE;
			 return TRUE;
		}
		void Close() {
			if (m_hFile != INVALID_HANDLE_VALUE )
			{
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}
		BOOL Lock(INT64 position, INT64 size, INT time) {

			if (m_hFile == INVALID_HANDLE_VALUE || size <= 0)
				return FALSE;
			const DWORD dwBeginTickCount = GetTickCount();
			while (TRUE)
			{
				if (::LockFile(GetFileHandle(), (DWORD)position, (DWORD)(position >> 32), (DWORD)size, (DWORD)(size >> 32)))
					return TRUE;
				const DWORD dwLastError = ::GetLastError();
				if ((dwLastError != ERROR_SHARING_VIOLATION && dwLastError != ERROR_LOCK_VIOLATION) ||
					time == 0 || (time > 0 && GetTickCount() - dwBeginTickCount > (DWORD)time))
					break;
				Sleep(20 + (INT)(randint() % 20));
			}
			return FALSE;
		}
		BOOL Unlock(INT64 position, INT64 size) {
			return ::UnlockFile(m_hFile, (DWORD)position, (DWORD)(position >> 32), (DWORD)size, (DWORD)(size >> 32));
		}
		BOOL  MoveWrOff(int ��׼�ƶ�λ��, INT64 �ƶ�����) {

			LARGE_INTEGER dis;
			dis.QuadPart = (ULONGLONG)�ƶ�����;
			return ::SetFilePointerEx(m_hFile, dis, NULL, (DWORD)��׼�ƶ�λ��);
		}
		BOOL MoveToBegin() {
			return (::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER);
		}
		BOOL MoveToEnd() {
			return (::SetFilePointer(m_hFile, 0, NULL, FILE_END) != INVALID_SET_FILE_POINTER);
		}
		LPBYTE ReadBin(size_t ���������ݵĳߴ�) {
			std::vector<unsigned char> mem;
			const HANDLE hFile = GetFileHandle();
			if (m_hFile != INVALID_HANDLE_VALUE && ���������ݵĳߴ� > 0)
			{
				DWORD dwRead = 0;

				mem.resize(���������ݵĳߴ�);
				if (::ReadFile(hFile, mem.data(), ���������ݵĳߴ�, &dwRead, NULL))
					mem.resize(dwRead);
				else
					::SetFilePointer(hFile, 0, NULL, FILE_END);
			}
			return elibstl::clone_bin(mem.data(), mem.size());
		}
		BOOL WriteBin(const elibstl::classhelp::span<unsigned char>& ��д�����ֽڼ�����) {
			return WriteData(��д�����ֽڼ�����.data(), ��д�����ֽڼ�����.size());
		}
		auto GetOffst() {
			return (m_hFile == INVALID_HANDLE_VALUE) ? -1 : MoveAndGetFilePos(FILE_CURRENT);
		}
		auto GetCurrentPos() {
			return MoveAndGetFilePos(FILE_CURRENT);
		}
		BOOL SetCurrentPos(INT64 n64CurrentPos)
		{
			if (m_hFile == INVALID_HANDLE_VALUE)
				return FALSE;
			LARGE_INTEGER pos;
			pos.QuadPart = (ULONGLONG)n64CurrentPos;
			return ::SetFilePointerEx(m_hFile, pos, NULL, FILE_BEGIN);
		}

		INT64 MoveAndGetFilePos(DWORD dwMoveMethod)
		{
			if (m_hFile == INVALID_HANDLE_VALUE)
				return 0;
			LARGE_INTEGER dis, current_pos;
			dis.QuadPart = current_pos.QuadPart = 0;
			return (::SetFilePointerEx(m_hFile, dis, &current_pos, dwMoveMethod) ? (INT64)current_pos.QuadPart : -1);
		}
		/*Unicode*/
		LPBYTE ReadText(INT wannasize) {
			std::wstring ret;
			if (m_hFile != INVALID_HANDLE_VALUE && wannasize != 0)
			{
				BOOL blpSuceeded = FALSE;
				do
				{
					const INT64 n64CurrentPos = GetCurrentPos();
					if (n64CurrentPos < 0)
						break;
					INT npReadSize;
					if (wannasize < 0)
					{
						const INT64 n64FileSize = MoveAndGetFilePos(FILE_END);
						if (n64FileSize < 0)
							break;
						npReadSize = (INT)(n64FileSize - n64CurrentPos);
						SetCurrentPos( n64CurrentPos);
					}
					else
					{
						npReadSize = wannasize * sizeof(WCHAR);
					}

					std::vector<unsigned char> mem;
					DWORD dwRead = 0;
					mem.resize(npReadSize);
					if (::ReadFile(m_hFile, mem.data(), (DWORD)npReadSize, &dwRead, NULL) == FALSE)
						break;
					mem.resize(dwRead);
					mem.push_back(0);
					mem.push_back(0);

					const WCHAR* psBegin = (const WCHAR*)mem.data();
					if (n64CurrentPos == 0 && (WORD)*psBegin == 0xFEFF)  // Ϊ�ļ��ײ�MS��Unicode�ı��ļ���ʼ��־?
						psBegin++;  // ��������
					const WCHAR* ps = psBegin;
					while (*ps != '\0' && *ps != 0x1A)
						ps++;
					ret = std::wstring(psBegin, ps - psBegin);
					const DWORD dwOffset = (DWORD)((const BYTE*)(ps + 1) - mem.data());
					blpSuceeded = SetCurrentPos(n64CurrentPos + (INT64)min(dwOffset, dwRead));
				} while (FALSE);

				if (blpSuceeded == FALSE)
					::SetFilePointer(m_hFile, 0, NULL, FILE_END);
			}
			return clone_textw(ret);

		}
		BOOL WriteText(INT nArgCount, PMDATA_INF pArgInf) {
			DWORD dwNumOfByteRead;
			auto bRet = TRUE;
			for (INT i = 1; i < nArgCount; i++)
			{
				auto pData = elibstl::arg_to_wstring(pArgInf, i);
				const auto dwTextSize = (DWORD)(pData.size() * sizeof(WCHAR));
				DWORD dwWritten = 0;
				if (pData.size() > 0)
				{
					if (!::WriteFile(m_hFile, pData.c_str(), dwTextSize, &dwWritten, NULL) || dwWritten != dwTextSize) {
						bRet = FALSE;
						break;
					}
				}	
			}
			return bRet;
		}
	private: 
		inline static BOOL s_blpSrand = FALSE;
		INT randint()
		{
			if (s_blpSrand == FALSE)
			{
				s_blpSrand = TRUE;
				srand(::GetTickCount());
			}

			return rand();
		}
		HANDLE GetFileHandle()const
		{
			return m_hFile;
		}
		/*��������д�벻�˴���DWORD���ȵ��ڴ�,�����벻��*/
		BOOL WriteData(const void* pData, const int npDataSize)
		{
			if (pData == nullptr || npDataSize == 0)
				return FALSE;
			if (m_hFile == INVALID_HANDLE_VALUE)
				return FALSE;
			DWORD dwWritten = 0;
			return (npDataSize == 0 || (::WriteFile(m_hFile, pData, (DWORD)npDataSize, &dwWritten, NULL) && dwWritten == (DWORD)npDataSize));
		}



	};


}


#endif // _WIN32

//����
EXTERN_C void fn_CFile_structure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	self = new elibstl::CFile;
}
//����
FucInfo Fn_CFile_structure = { {
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
	} ,fn_CFile_structure ,"Fn_CFile_structure" };
//����
static ARG_INFO s_CopyArgs[] =
{
	{
		/*name*/    "����",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/   DTP_VAR,
		/*default*/ 0,
		/*state*/   ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
EXTERN_C void fn_CFile_copy(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	const auto& rht = elibstl::classhelp::get_other<elibstl::CFile>(pArgInf);
	self = new elibstl::CFile{ *rht };
}
FucInfo Fn_CFile_copy = { {
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
		/*ArgCount*/1,
		/*arg lp*/ s_CopyArgs,
	} ,fn_CFile_copy ,"Fn_CFile_copy" };

//����
EXTERN_C void fn_CFile_des(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	if (self)
	{
		self->elibstl::CFile::~CFile();
		operator delete(self);
	}
	self = nullptr;
}
FucInfo Fn_CFile_destruct = { {
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
	} ,fn_CFile_des ,"Fn_CFile_des" };


static ARG_INFO s_OpenArgs[] =
{
	{
		/*name*/    "�ļ���",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "�򿪷�ʽ",
		/*explain*/ "1��#���룺��ָ���ļ��������ݣ�������ļ���������ʧ�ܣ�2��#д����д�����ݵ�ָ���ļ���������ļ���������ʧ�ܣ�3��#��д�����ļ��ж������ݻ���д�����ݵ��ļ���������ļ���������ʧ�� 4��#��д��д�����ݵ�ָ���ļ���������ļ����������ȴ���һ�����ļ�������Ѿ����ھ���������е��������ݣ�5��#��д��д�����ݵ�ָ���ļ���������ļ��������򴴽�һ�����ļ�������Ѿ����ھ�ֱ�Ӵ򿪣�6��#�Ķ������ļ��ж������ݻ���д�����ݵ��ļ���������ļ��������򴴽�һ�����ļ�������Ѿ����ھ�ֱ�Ӵ򿪡�",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
		/*name*/    "����ģʽ",
		/*explain*/ "1��#�����ƣ������������������д���ļ���2��#��ֹ������ֹ�������̶����ļ���3��#��ֹд����ֹ��������д���ļ���4��#��ֹ��д����ֹ�������̶�д���ļ���",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
EXTERN_C void fn_CFile_Open(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	int openmode = 3, sharemode = 1;
	if (pArgInf[2].m_dtDataType != _SDT_NULL && pArgInf[2].m_int>=1 && pArgInf[2].m_int <= 6)
	{//#��д
		openmode = pArgInf[2].m_int;
	}
	if (pArgInf[3].m_dtDataType != _SDT_NULL && pArgInf[3].m_int >= 1 && pArgInf[3].m_int <= 3) {
		sharemode= pArgInf[3].m_int;
	}
	auto file = elibstl::args_to_wsdata(pArgInf,1);
	pRetData->m_bool=self->Open(file, openmode, sharemode);
}
FucInfo Fn_CFile_Open = { {
		/*ccname*/  "��",
		/*egname*/  "��һ����ͨ�ļ����Զ��ļ����������������ɹ������棬ʧ�ܷ��ؼ١�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/3,
		/*arg lp*/ s_OpenArgs,
	} ,fn_CFile_Open ,"Fn_CFile_Open" };
EXTERN_C void fn_CFile_Close(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	 self->Close();
}

FucInfo Fn_CFile_Close = { {
		/*ccname*/  "�ر�",
		/*egname*/  "�ر��ļ�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ 0,
	} ,fn_CFile_Close ,"Fn_CFile_Close" };
static ARG_INFO s_LockArgs[] =
{
	{
		/*name*/    "�������������λ��",
		/*explain*/ "λ��ֵ�� 0 ��ʼ��",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "������������ĳ���",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "��������ʱ��",
		/*explain*/ "������ָ������ʧ�ܺ󷴸��������Եĺ������������ʡ�ԣ�Ĭ��Ϊ 0 ����һ��ʧ���������ء��������ֵ�ṩΪ -1 ���򷴸��������ԣ�ֱ�������ɹ�Ϊֹ��",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_DEFAULT_VALUE_IS_EMPTY,
	}
};
EXTERN_C void fn_CFile_Lock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	auto time = elibstl::args_to_data<int>(pArgInf, 3).value_or(0);
	pRetData->m_bool = self->Lock(pArgInf[1].m_int64, pArgInf[2].m_int64, time);
}
FucInfo Fn_CFile_Lock = { {
		/*ccname*/  "��ס",
		/*egname*/  " �������ɸ����̷���ͬһ���ļ��Ļ����У�ʹ�ñ�����ܾ������û��Ա����ļ��е�ĳ��������ж�д���ʡ�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/3,
		/*arg lp*/ s_LockArgs,
	} ,fn_CFile_Lock ,"Fn_CFile_Lock" };


static ARG_INFO s_UnLockArgs[] =
{
	{
		/*name*/    "�������������λ��",
		/*explain*/ "λ��ֵ�� 0 ��ʼ��",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "������������ĳ���",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_UnLock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->Unlock(pArgInf[1].m_int64, pArgInf[2].m_int64);
}
FucInfo Fn_CFile_UnLock = { {
		/*ccname*/  "����",
		/*egname*/  "����ɡ���ס�ļ���������ļ������е��������ɹ������棬ʧ�ܷ��ؼ١�ע�⣺���������ʱ���ṩ�Ĳ���ֵ��������á���ס�ļ�������ʱ���ṩ�Ĳ���ֵ��ȫһ�¡�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/ s_UnLockArgs,
	} ,fn_CFile_UnLock ,"Fn_CFile_UnLock" };

static INT s_dtCmdIndexcommobj_memfile_ex[] = { 318,319 ,320 ,321,322,323,324};

namespace elibstl {


	LIB_DATA_TYPE_INFO Obj_DiskFile =
	{
		"�ļ���д",
		"DiskFile",
		"�ļ���д",
		sizeof(s_dtCmdIndexcommobj_memfile_ex) / sizeof(s_dtCmdIndexcommobj_memfile_ex[0]),
		 s_dtCmdIndexcommobj_memfile_ex,
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