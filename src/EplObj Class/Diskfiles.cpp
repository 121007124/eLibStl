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
		~CFile() {
			Close();
		}
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
		void FSeek(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
			BOOL bRet = FALSE;
			DWORD dwMoveMethod;
			switch (pArgInf[1].m_int)
			{
			case 2://#�ļ�β
				dwMoveMethod = FILE_END;
				break;
			case 3://#����λ��
				dwMoveMethod = FILE_CURRENT;
				break;
			default://#�ļ���
				dwMoveMethod = FILE_BEGIN;
			}
			LARGE_INTEGER dis;
			dis.QuadPart = (ULONGLONG)pArgInf[2].m_int64;
			bRet = ::SetFilePointerEx(m_hFile, dis, NULL, (DWORD)dwMoveMethod);
			pRetData->m_bool = bRet;

		}
		BOOL SeekToBegin() {
			return (::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER);
		}
		BOOL SeekToEnd() {
			return (::SetFilePointer(m_hFile, 0, NULL, FILE_END) != INVALID_SET_FILE_POINTER);
		}
		BOOL WriteBin(INT nArgCount, PMDATA_INF pArgInf) {
			DWORD dwNumOfByteRead;
			auto bRet = TRUE;
			for (INT i = 1; i < nArgCount; i++)
			{
				LPBYTE pData = pArgInf[i].m_pBin + 2 * sizeof(INT);
				INT nLen = pArgInf[i].m_pInt[1];
				if (WriteFile(m_hFile, pData, nLen, &dwNumOfByteRead, NULL) == FALSE)
				{
					bRet = FALSE;
					break;
				}
			}
			return bRet;
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

		INT64 GetCurrentPos()const {
			return MoveAndGetFilePos(FILE_CURRENT);
		}
		auto GetCurrent()const {
			return (m_hFile == INVALID_HANDLE_VALUE ? -1 : GetCurrentPos());

		}
		BOOL SetCurrentPos(INT64 n64CurrentPos)
		{
			if (m_hFile == INVALID_HANDLE_VALUE)
				return FALSE;
			LARGE_INTEGER pos;
			pos.QuadPart = (ULONGLONG)n64CurrentPos;
			return ::SetFilePointerEx(m_hFile, pos, NULL, FILE_BEGIN);
		}

		INT64 MoveAndGetFilePos(DWORD dwMoveMethod)const
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
		INT64 GetSize()const {
			LARGE_INTEGER fileSize;
			if (m_hFile == INVALID_HANDLE_VALUE)
				return -1;
			if (::GetFileSizeEx(m_hFile, &fileSize) == FALSE) {
				return -1;
			}
			return fileSize.QuadPart;
		}

		INT64 MoveToEndAndGetFileSize() {
			return MoveAndGetFilePos(FILE_END); 
		}
		auto ReadLineW()->std::wstring {
			LARGE_INTEGER 
				orgLoc{ 0 }, /*ԭʼλ��*/
				newLoc{ 0 }, /*���ڴ�����λ��*/
				nTLen{ 0 };/*���ڼ�¼��ȡ�ܳ���*/
			if (SetFilePointerEx(m_hFile, orgLoc, &newLoc, FILE_CURRENT) == 0) {
				// �ļ�ָ��λ�û�ȡʧ�ܣ��ƶ����ļ�ĩβ�����ؿ��ַ���
				SetFilePointerEx(m_hFile, orgLoc, &newLoc, FILE_END);
				return {};
			}
			auto nLen = GetSize() - orgLoc.QuadPart;/*ʣ�೤��*/
			constexpr auto once_max = 4096;/*���嵥�ζ�ȡ����*/
			constexpr auto once_wchar_max = once_max / sizeof(wchar_t);/*���嵥�ʶ�ȡwchar_t�ַ�����*/
			
			std::wstring line;
			DWORD dwNumOfByteRead;
			BOOL bFoundNewline = FALSE;

		
			for (long long i = 0; i < nLen; i += once_max)
			{
				std::wstring tmp(once_wchar_max, L'\0');
				const auto needsize = static_cast<DWORD>(min(nLen - i, static_cast<long long>(4096)));
				auto nRet = ReadFile(m_hFile, tmp.data(), needsize, &dwNumOfByteRead, 0);/*һ���������4096,���Կ�����ReadFile��*/;
				if (nRet == FALSE)
				{
					SetFilePointer(m_hFile, 0, NULL, FILE_END);
					break;
				}
				bool bFind{ false };/*���ڼ�¼�Ƿ�ƥ�䵽���з�*/
				DWORD j = 0;/*���ڼ�¼����ַ��е�����λ��*/
				for (const auto& ch : tmp )/*��������ַ�*/
				{
					if (ch == L'\0')
					{
						bFind = true;/*ƥ�䵽������*/
						dwNumOfByteRead = j * sizeof(wchar_t);
						newLoc.QuadPart = orgLoc.QuadPart + nTLen.QuadPart + j * sizeof(wchar_t);	/*������λ��:��λ��+��ǰ����+�����ַ�����*/;
						SetFilePointerEx(m_hFile, newLoc, nullptr, FILE_BEGIN);/*������λ��*/
						break;
					}
					else if (ch == L'\n')
					{
						
						bFind = true;/*ƥ�䵽���з�*/
						dwNumOfByteRead = j;
						newLoc.QuadPart = orgLoc.QuadPart + nTLen.QuadPart + (j + 1) * sizeof(wchar_t);/*����ڽ�����,��δ�����Ҫ������һ����������+1���ַ�*/
						SetFilePointerEx(m_hFile, newLoc, nullptr, FILE_BEGIN); /*������λ��*/
						break;
					}
					else if (ch == L'\r') {
						
						bFind = true;/*ƥ�䵽�س�*/
						if (j + 1 == dwNumOfByteRead)/*����س������һ���ַ�*/
						{
							wchar_t szNewline{ L'\0' };
							
							nRet = ReadFile(m_hFile, &szNewline, sizeof(szNewline), &dwNumOfByteRead, 0);//�ٶ������ֽڣ������ǲ���\r\n���
							if (nRet == FALSE || szNewline != L'\n')
							{
								//����\r\n��ϣ��Ѷ�дλ�÷ŵ�\r����
								newLoc.QuadPart = orgLoc.QuadPart + nTLen.QuadPart + (j + 1) * sizeof(wchar_t);
								SetFilePointerEx(m_hFile, newLoc, nullptr, FILE_BEGIN); /*������λ��*/
							}
						}
						else if (tmp[j + 1] == L'\n')/*�������һ���ַ�,�һس���Ϊ����,��λ�ò���at������Ϊһ������Խ��*/
						{
							
							newLoc.QuadPart = orgLoc.QuadPart + nTLen.QuadPart + (j + 2) * sizeof(wchar_t);
							SetFilePointerEx(m_hFile, newLoc, nullptr, FILE_BEGIN); /*������λ��*/
						}
						else/*�������һ���ַ�,�һس���Ϊ����*/
						{
							// �ҵ� '\r' ���治�� '\n'���ضϵ�ǰ�в������ļ�ָ��λ��
							newLoc.QuadPart = orgLoc.QuadPart + nTLen.QuadPart + (j + 1) * sizeof(wchar_t);
							SetFilePointerEx(m_hFile, newLoc, nullptr, FILE_BEGIN); /*������λ��*/
						}
						dwNumOfByteRead = j;
						break;
					}
					j++;
					
				}
				
				tmp.resize(dwNumOfByteRead);
				nTLen.QuadPart += dwNumOfByteRead;
				line += tmp;
				if (bFind || dwNumOfByteRead != 4096)//����ҵ��˻��з����߶�ȡ�ĳ��Ȳ�����4096�����ʵ�ʳ��Ȳ���Ҳ��ı�dwNumOfByteRead,������ѭ��
					break;

			}

			return line;
		}

		BOOL isOpen() const{
			return m_hFile != INVALID_HANDLE_VALUE;
		}
		BOOL WriteLine(INT nArgCount, PMDATA_INF pArgInf) {
			BOOL bRet = FALSE;
			DWORD dwNumOfByteRead;
			bRet = TRUE;
			for (INT i = 1; i < nArgCount; i++)
			{
				auto pData = elibstl::arg_to_wstring(pArgInf, i);
				if (pData.empty())
				{
					WriteFile(m_hFile, L"\r\n", 4, &dwNumOfByteRead, NULL);
					continue;
				}
				auto nLen = pData.size() * sizeof(wchar_t);
				if (nLen > 0)
				{
					if (WriteFile(m_hFile, pData.data(), nLen, &dwNumOfByteRead, NULL))
						WriteFile(m_hFile, L"\r\n", 4, &dwNumOfByteRead, NULL);
					else
						bRet = FALSE;
				}
				if (bRet == FALSE)
					break;

			}
			return bRet;
		}
		BOOL RemoveData(INT64 size) {
			if (m_hFile == INVALID_HANDLE_VALUE || size <= 0)
				return FALSE;
			LARGE_INTEGER orgLoc, newLoc;
			orgLoc.QuadPart = 0;
			newLoc.QuadPart = 0;

			if (SetFilePointerEx(m_hFile, orgLoc, &newLoc, FILE_CURRENT) == 0)
				return FALSE;

			auto bRet = TRUE;
			DWORD dwNumOfByteRead;
			LARGE_INTEGER fileSize;
			if (GetFileSizeEx(m_hFile, &fileSize) == FALSE)
				return FALSE;

			LARGE_INTEGER nBkLen;
			nBkLen.QuadPart = fileSize.QuadPart - orgLoc.QuadPart - size;
			LPBYTE pBkData = NULL;
			if (nBkLen.QuadPart > 0)  // �в�������
			{
				
				pBkData = new BYTE[static_cast<size_t>(nBkLen.QuadPart)];
				LARGE_INTEGER nLen;
				nLen.QuadPart = size;
				if (SetFilePointerEx(m_hFile, nLen, NULL, FILE_CURRENT) == 0)  // ���Ƶ�
				{
					delete[] pBkData;
					return FALSE;
				}
				if (ReadFile(m_hFile, pBkData, static_cast<DWORD>(nBkLen.QuadPart), &dwNumOfByteRead, 0) == FALSE)
				{
					delete[] pBkData;
					return FALSE;
				}
				if (SetFilePointerEx(m_hFile, orgLoc, NULL, FILE_BEGIN) == 0)  // �ָ�ԭλ��
				{
					delete[] pBkData;
					return FALSE;
				}
				if (WriteFile(m_hFile, pBkData, static_cast<DWORD>(nBkLen.QuadPart), &dwNumOfByteRead, NULL))
				{
					if (FlushFileBuffers(m_hFile) == FALSE)
						bRet = FALSE;
				}
				else
					bRet = FALSE;
				delete[] pBkData;
			}
			if (bRet)
				SetEndOfFile(m_hFile);
			return bRet;
		}
		BOOL InsertStr(INT nArgCount, PMDATA_INF pArgInf) {
			if (m_hFile == INVALID_HANDLE_VALUE) {
				// �ļ������Ч
				return FALSE;
			}

			LARGE_INTEGER orgLoc, newLoc;
			orgLoc.QuadPart = 0;
			newLoc.QuadPart = 0;

			// ���ļ�ָ���ƶ�����ǰλ��
			if (SetFilePointerEx(m_hFile, orgLoc, &newLoc, FILE_CURRENT) == 0) {
				// �ļ�ָ���ƶ�ʧ��
				return FALSE;
			}

			auto bRet = TRUE;
			DWORD dwNumOfBytesWritten;
			LARGE_INTEGER fileSize;

			// ��ȡ�ļ���ǰ��С
			if (GetFileSizeEx(m_hFile, &fileSize) == FALSE) {
				// ��ȡ�ļ���Сʧ��
				return FALSE;
			}

			// ����Ҫ���������ݳ���
			LARGE_INTEGER nBkLen;
			nBkLen.QuadPart = fileSize.QuadPart - orgLoc.QuadPart;
			LPBYTE pBkData = nullptr;

			if (nBkLen.QuadPart > 0) {
				// �в������ݣ���ȡ������
				pBkData = new BYTE[static_cast<size_t>(nBkLen.QuadPart)];
				if (!ReadFile(m_hFile, pBkData, static_cast<DWORD>(nBkLen.QuadPart), &dwNumOfBytesWritten, nullptr)) {
					// ��ȡ��������ʧ�ܣ��ͷ��ڴ沢����
					delete[] pBkData;
					return FALSE;
				}
				// �ָ�ԭ�ļ�ָ��λ��
				SetFilePointerEx(m_hFile, orgLoc, nullptr, FILE_BEGIN);
			}

			for (INT i = 1; i < nArgCount; i++) {
				auto dataToInsert = elibstl::arg_to_wstring(pArgInf, i);
				if (dataToInsert.empty()) {
					continue;  // ����������
				}

				INT dataLength = dataToInsert.size() * sizeof(wchar_t);

				if (dataLength > 0) {
					// �������ݵ��ļ�
					bRet = WriteFile(m_hFile, dataToInsert.data(), dataLength, &dwNumOfBytesWritten, nullptr);
				}

				if (bRet == FALSE) {
					break;  // д������ʧ�ܣ��˳�ѭ��
				}
			}

			if (bRet && pBkData) {
				// �������ݺ󣬻ָ�ԭ�ļ�λ�ã�д�ز�������
				if (SetFilePointerEx(m_hFile, orgLoc, &newLoc, FILE_CURRENT) == 0) {
					// �ָ�ԭ�ļ�λ��ʧ��
					return FALSE;
				}

				if (!WriteFile(m_hFile, pBkData, static_cast<DWORD>(nBkLen.QuadPart), &dwNumOfBytesWritten, nullptr)) {
					// д�ز�������ʧ��
					bRet = FALSE;
				}
				// �ָ�ԭ�ļ�λ��
				SetFilePointerEx(m_hFile, orgLoc, nullptr, FILE_BEGIN);
			}

			if (pBkData) {
				delete[] pBkData;
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
	} ,ESTLFNAME(fn_CFile_structure) };
//����
static ARG_INFO s_CopyArgs[] =
{
	{
		/*name*/    "����",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/   DTP_CFILE,
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
	} ,ESTLFNAME(fn_CFile_copy)};

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
	} ,ESTLFNAME(fn_CFile_des) };


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
	} ,ESTLFNAME(fn_CFile_Open)  };
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
	} ,ESTLFNAME(fn_CFile_Close) };
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
	} ,ESTLFNAME(fn_CFile_Lock) };


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
	} ,ESTLFNAME(fn_CFile_UnLock) };


static ARG_INFO s_FSeekArgs[] =
{
	{
		/*name*/    "��ʼ�ƶ�λ��",
		/*explain*/ "����ֵָ�����ļ��п�ʼ�ƶ���λ�á�����Ϊ���³���֮һ��1��#�ļ��ף� 2��#�ļ�β�� 3��#����λ�á������������ʡ�ԣ�Ĭ��ֵΪ��#�ļ��ס���",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "�ƶ�����",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_FSeek(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	self->FSeek(pRetData, nArgCount, pArgInf);
}
FucInfo Fn_CFile_FSeek = { {
		/*ccname*/  "�ƶ���дλ��",
		/*egname*/  "�ڱ��򿪵��ļ��У�������һ������д������λ�á��ɹ������棬ʧ�ܷ��ؼ١�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/ s_FSeekArgs,
	} ,ESTLFNAME(fn_CFile_FSeek) };

EXTERN_C void fn_CFile_SeekToBegin(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	self->SeekToBegin();
}
FucInfo Fn_CFile_SeekToBegin = { {
		/*ccname*/  "�Ƶ��ļ���",
		/*egname*/  "�ڱ��򿪵��ļ��У�������һ������д������λ�õ��ļ��ס��ɹ������棬ʧ�ܷ��ؼ١�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_SeekToBegin) };

EXTERN_C void fn_CFile_SeekToEnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	self->SeekToEnd();
}
FucInfo Fn_CFile_SeekToEnd = { {
		/*ccname*/  "�Ƶ��ļ�β",
		/*egname*/  "�ڱ��򿪵��ļ��У�������һ������д������λ�õ��ļ��ס��ɹ������棬ʧ�ܷ��ؼ١�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_SeekToEnd) };





static ARG_INFO s_ReadBinArgs[] =
{
	{
		/*name*/    "���������ݵĳߴ�",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_ReadBin(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_pBin = self->ReadBin(pArgInf[1].m_int);
}
FucInfo Fn_CFile_ReadBin = { {
		/*ccname*/  "�����ֽڼ�",
		/*egname*/  "���ļ��е�ǰ��дλ�ö�ȡ������һ���ֽڼ�����,ʵ�ʶ��볤��(���������ֽڼ��ĳߴ�)���ܻ�С�������볤��.�������ʧ��,������һ�����ֽڼ������Զ�����ǰ�ļ���дλ���Ƶ��ļ�β��.",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BIN,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_ReadBinArgs,
	} ,ESTLFNAME(fn_CFile_ReadBin) };

static ARG_INFO s_WriteBinArgs[] =
{
	{
		/*name*/    "��д�������",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_WriteBin(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->WriteBin(nArgCount,pArgInf);
}
FucInfo Fn_CFile_WriteBin = { {
		/*ccname*/  "д���ֽڼ�",
		/*egname*/  "����������д��һ�λ������ֽڼ����ݵ��ļ��е�ǰ��дλ�ô����ɹ������棬ʧ�ܷ��ؼ١�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   CT_ALLOW_APPEND_NEW_ARG  ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_WriteBinArgs,
	} ,ESTLFNAME(fn_CFile_WriteBin) };

static ARG_INFO s_ReadTextArgs[] =
{
	{
		/*name*/    "�������ı��ĳߴ�",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ -1,
		/*state*/   ArgMark::AS_HAS_DEFAULT_VALUE,
	}
};
EXTERN_C void fn_CFile_ReadText(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_pBin = self->ReadText(pArgInf[1].m_int);
}
FucInfo Fn_CFile_ReadText = { {
		/*ccname*/  "�����ı�W",
		/*egname*/  " ���ļ��е�ǰ��дλ�ö�ȡ������һ���ı�����,ʵ�ʶ��볤��(�������ı��ĳߴ�)���ܻ�С�������볤��.����������д����ַ�ֵ0��26(�ı�������־),�������ظ��ַ�֮ǰ������(������д���������ַ�).�������ʧ��,������һ�����ı������Զ�����ǰ�ļ���дλ���Ƶ��ļ�β��.ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BIN,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_ReadTextArgs,
	} ,ESTLFNAME(fn_CFile_ReadText) };

static ARG_INFO s_WriteTextArgs[] =
{
	{
		/*name*/    "��д���UTF16�ı�����,��Ϊ���ı��������Զ�ת��Ϊ�ı�",
		/*explain*/ "",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    _SDT_ALL,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_WriteText(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool= self->WriteText(nArgCount, pArgInf);
}
FucInfo Fn_CFile_WriteText = { {
		/*ccname*/  "д���ı�W",
		/*egname*/  "����������д��һ�λ������ı����ݵ��ļ��е�ǰ��дλ�ô����ɹ������棬ʧ�ܷ��ؼ١�ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   CT_ALLOW_APPEND_NEW_ARG ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_WriteTextArgs,
	} ,ESTLFNAME(fn_CFile_WriteText) };

EXTERN_C void fn_CFile_ReadLine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_pBin = elibstl::clone_textw(self->ReadLineW());
}

FucInfo Fn_CFile_ReadLine = { {
		/*ccname*/  "����һ��W",
		/*egname*/  "�������������ļ��е�ǰ��дλ�ö�ȡ������һ���ı����ݣ���ĩ�Ļس������з�����������ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BIN,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_ReadLine) };


static ARG_INFO s_WriteLineArgs[] =
{
	{
		/*name*/    "��д����ı�������",
		/*explain*/ "����ֵ�����Ϊ�ı��������ݣ����Զ�����ת��������޷�ת��������������Ϊ�ֽڼ����ӳ���ָ�롢����û��Զ����������ͣ�����д�������ݡ�",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    _SDT_ALL,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_WriteLine(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->WriteLine(nArgCount, pArgInf);
}
FucInfo Fn_CFile_WriteLine = { {
		/*ccname*/  "д�ı���W",
		/*egname*/  "����������д��һ�л�����ı����ݵ��ļ��е�ǰ��дλ�ô���ÿ�е�β�������Զ����ϻس������з���ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   CT_ALLOW_APPEND_NEW_ARG ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_WriteLineArgs,
	} ,ESTLFNAME(fn_CFile_WriteLine) };

EXTERN_C void fn_CFile_isOpen(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->isOpen();
}
FucInfo Fn_CFile_isOpen = { {
		/*ccname*/  "�Ƿ��Ѵ�",
		/*egname*/  "�����ж��Ƿ��Ѿ����ı��ļ�",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   0 ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_isOpen) };

EXTERN_C void fn_CFile_GetCurrent(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_int64 = self->GetCurrent();
}
FucInfo Fn_CFile_GetCurrent = { {
		/*ccname*/  "ȡ��ǰ��дλ��",
		/*egname*/  "��ȡ��ǰ��дλ��,ʧ�ܷ���-1",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   0 ,
		/*ret*/ SDT_INT64,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_GetCurrent) };

EXTERN_C void fn_CFile_GetSize(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_int64 = self->GetSize();
}
FucInfo Fn_CFile_GetSize = { {
		/*ccname*/  "ȡ�ļ���С",
		/*egname*/  "��ȡ��ǰ�ļ�ռ�ô��̴�С,ʧ�ܷ���-1����λ:(�ֽ�)",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   0 ,
		/*ret*/ SDT_INT64,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ nullptr,
	} ,ESTLFNAME(fn_CFile_GetSize) };

static ARG_INFO s_RemoveDataArgs[] =
{
	{
		/*name*/    "��ɾ�����ֽ�",
		/*explain*/ "��INT64û��,���������벻����ô���ڴ�,�����������й�",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT64,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_RemoveData(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->RemoveData(pArgInf[1].m_int64);
}
FucInfo Fn_CFile_RemoveData = { {
		/*ccname*/  "ɾ������",
		/*egname*/  R"(�������������ļ��е�ǰ��дλ�ô�ɾ��һ���ֽ����ݣ��ļ����������˳��ǰ�ơ����ļ�����ʱ�������"#��д"��"#�Ķ�"Ȩ�ޡ�ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).)",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   NULL ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_RemoveDataArgs,
	} ,ESTLFNAME(fn_CFile_RemoveData) };
static ARG_INFO s_InsertStrArgs[] =
{
	{
		/*name*/    "��������ı�",
		/*explain*/ "����ֵ�����Ϊ�ı��������ݣ����Զ�����ת��������޷�ת��������������Ϊ�ֽڼ����ӳ���ָ�롢����û��Զ����������ͣ�����д�������ݡ�",
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    _SDT_ALL,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
EXTERN_C void fn_CFile_InsertStr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::classhelp::get_this<elibstl::CFile>(pArgInf);
	pRetData->m_bool = self->InsertStr(nArgCount, pArgInf);
}
FucInfo Fn_CFile_InsertStr = { {
		/*ccname*/  "�����ı�W",
		/*egname*/ R"( ��������������һ�λ������ı����ݵ��ļ��е�ǰ��дλ�ô������ļ�����ʱ�������"#��д"��"#�Ķ�"Ȩ�ޡ��ɹ������棬ʧ�ܷ��ؼ١�ע��: �ı��ļ��ı����ʽ����ΪUnicode(��UTF - 16).)",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/   CT_ALLOW_APPEND_NEW_ARG ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/ s_InsertStrArgs,
	} ,ESTLFNAME(fn_CFile_InsertStr) };

static INT s_dtCmdIndexcommobj_memfile_ex[] = { 318,319 ,320 ,321,322,323,324,326,327,328,329,330,331,332,333,334,335,336,337,338,339};

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