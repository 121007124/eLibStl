#include"EcontrolHelp.h"
#include<string>
typedef struct _ColumnInfo//�ֶ���Ϣ
{
	const char* Name;
	int Type;
	int StrDataLenth;
} *lpColumInfo;
#pragma once

#ifndef KRNLNDATABSE
#define KRNLNDATABSE
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<filesystem>
#include <map>
#include <unordered_set>
#include<cstring>
#include<functional>
namespace elibstl {
#pragma pack(1)//�߽����
#ifdef _MSC_VER 
#pragma warning(disable:4996)
#endif 

	enum class ColumnDataType {
		BYTE = 1,         // �ֽ���
		SHORT_INT,    // ��������
		INT,          // ������
		LONG_INT,     // ��������
		FLOAT,        // С����
		DOUBLE,       // ˫����С����
		BOOLEAN,      // �߼���
		DATE_TIME,    // ����ʱ����
		PTR,		  // ָ���ͣ��ļ����ݿⲻ���õ�
		TEXT,         // �ı���
		BYTE_ARRAY,   // �ֽڼ���
		REMARK,		  // ��ע��
		PRIMARY_KEY	  // ����
	};


	/*��֤��*/


	/*ʧ�ܺ�*/
	constexpr int EDB_ERROR_SUCCESS = 0;
	constexpr int EDB_ERROR_INVALID_COLUMN_NAME = -1;//�ֶ����ƴ���
	constexpr int EDB_ERROR_INVALID_COLUMN_TYPE = -2;//�������ʹ���
	constexpr int EDB_ERROR_INVALID_COLUMN_SIZE = -3;//���ݳ���Ϊ0
	constexpr int EDB_ERROR_DUPLICATE_COLUMN_NAME = -4;//�ֶ����ظ�
	constexpr int EDB_ERROR_CREATE_EDBS = -5;//����EDBʧ��
	constexpr int EDB_ERROR_CREATE_EDT = -6;//����EDTʧ��
	constexpr int EDB_ERROR_INVALID_EDBSFILE = -7;//��Чedb�ļ�
	constexpr int EDB_ERROR_NOOPEN_EDBS = -8;//û���ļ�
	constexpr int EDB_ERROR_INVALID_INDEX = -9;//��Ч���ֶλ��¼����
	constexpr int EDB_ERROR_INVALID_DATA_SIZE = -10;//д���������ݳ���
	constexpr int EDB_ERROR_INVALID_DATA_TYPE = -11;//д��������������
	constexpr int EDB_ERROR_TRANSACTION_OPENED = -12; //�����ѿ���
	constexpr int EDB_ERROR_TRANSACTION_NOT_OPENED = -13;//����δ����
	constexpr int EDB_ERROR_INVALID_COLUMN_NUM = -14;//��������������ƥ��
	constexpr int EDB_ERROR_HASBEEN_OPEN_EDBS = -15; // �Ѿ���һ����
	constexpr int EDB_ERROR_INVALID_ROWS_NUM = -16;//������Ч�����ļ�¼
	constexpr int EDB_ERROR_CANTOPEN_DAT = -17;//�޷��򿪶�ӦEDT�ļ�
	constexpr int EDB_ERROR_HAS_DEL_OR_INVALID = -18;//������Ч���Ѿ�����ɾ�����
	constexpr int EDB_ERROR_HAS_NO_DEL_OR_INVALID = -18;//������Ч���Ѿ���δ����ɾ�����




	struct ColumnInfo {
		std::string m_name;
		ColumnDataType m_dataType = ColumnDataType::TEXT;
		unsigned int m_strDataLength = 20;
		ColumnInfo() = default;
		ColumnInfo(const std::string& n, ColumnDataType t = ColumnDataType::TEXT, int l = 20) : m_name(n), m_dataType(t), m_strDataLength(l) {}
	};



#ifndef _WIN32
#include <unistd.h>

#else
#include<windows.h>
#include <SHLWAPI.h>
#pragma comment(lib,"SHLWAPI.lib") 

#endif // !_WIN32

	class edb_file
	{
		class edt_file
		{

			struct edt_header//EDT��������
			{
				edt_header() {
					m_createTime = 0;//COleDateTime::GetTickCount()��ÿ�δ򿪻�ˢ�£�����ԭ��EDB;;
					m_recordNum = 1;
					m_un_using_count = 0;
					m_nop_index = 0;
					std::fill(m_data, m_data + sizeof(m_data), 0);
				}
				double  m_createTime;//COleDateTime::GetTickCount()��ÿ�δ򿪻�ˢ�£�����ԭ��EDB;;
				int m_recordNum;//δʹ�ð����1;
				int m_un_using_count;//δʹ������
				int m_nop_index;//����ڳ�����������;
				unsigned char m_data[484];
			};
			struct edt_data {
				int m_previous;//ǰһ������������
				int m_next;//��һ����������,�ٴα�ʹ��ʱ�Ὣָ��������Ϊ������,���Ϊ�ձ�ʾû�к������ݻ���û�п�������
				int m_data_size;//���ݳ���
				unsigned char m_pbuffer[500];
				edt_data() {
					std::memset(this, 0, sizeof(*this));
				}
			};
			static constexpr int BUFFER_SIZE = 500;/*���С*/
			static constexpr char CHECK_EDT[] = { 'W', 'E', 'D', 'T', 0x00, 0x00, 0x01, 0x00 };
		private://��Ա
			std::fstream  m_file_edt;  // EDT�ļ�
			std::string m_filename;
			edt_header m_edb_inf;
		private://˽�к���
			bool __check_close_file() {
				if (!m_file_edt) {
					m_file_edt.close();
					return false;
				}
				return true;
			}
			bool __is_edt_file() {
				m_file_edt.seekg(0, std::ios::end);
				if (m_file_edt.tellg() < sizeof(edt_data)) {
					return false;
				}

				m_file_edt.seekg(0, std::ios::beg);

				char rfile_flag[sizeof(CHECK_EDT)];
				m_file_edt.read(rfile_flag, sizeof(CHECK_EDT));

				if (std::memcmp(rfile_flag, CHECK_EDT, sizeof(CHECK_EDT)) != 0) {
					return false;
				}
				m_file_edt.seekg(0, std::ios::beg);
				return true;
			}
		private://��Ա

			inline static std::string __rename_file_ext(const std::string& fname, const std::string& extname) {
				if (extname.empty()) {
					return fname;
				}
				std::filesystem::path filepath(fname);
				std::string suffix = extname;
				if (extname[0] != '.') {
					suffix = "." + suffix;
				}
				filepath.replace_extension(suffix);
				return filepath.string();
			}
		public://ȫ�ֺ���
			static bool create(const std::string& nfilename, double time) {
				std::string edtFileName = __rename_file_ext(nfilename, ".EDT");
				// �ļ��������ڶ�����������У���֤�ļ���ȷ�ر�
				{
					std::ofstream edtFile(edtFileName, std::ios::binary);
					if (!static_cast<const std::ios&>(edtFile)) {
						// ����EDT�ļ�ʧ��
						return false;
					}
					// д��EDT�ļ�ͷ����Ϣ
					edt_header edtHeader;
					edtHeader.m_createTime = time;
					edtFile.write(CHECK_EDT, sizeof(CHECK_EDT));
					edtFile.write(reinterpret_cast<char*>(&edtHeader), sizeof(edt_header));
				}
				return true;
			}
		public://��Ա����
			bool is_open() const {
				return m_file_edt.is_open();
			}
			bool open(const std::string& filename, double n_time) {
				if (m_file_edt.is_open()) {
					return false;
				}

				m_file_edt.open(filename, std::ios::binary | std::ios::out | std::ios::in);
				if (!__check_close_file()) {
					return false;
				}
				if (!__is_edt_file()) {
					return false;
				}
				static constexpr auto edt_check_size = sizeof(CHECK_EDT);
				m_file_edt.seekg(edt_check_size);
				m_file_edt.read(reinterpret_cast<char*>(&m_edb_inf), sizeof(edt_header));
				if (!__check_close_file() || m_edb_inf.m_recordNum <= 0) {
					return false;
				}

				m_file_edt.seekg(0, std::ios::end);
				auto fileSize = m_file_edt.tellg();
				if (!__check_close_file() || fileSize < static_cast<decltype(fileSize)>(sizeof(edt_data)) * m_edb_inf.m_recordNum) {
					return false;
				}
				/*if (n_time != m_edb_inf.m_createTime) {
					return false;
				}*/
				m_filename = filename;
				return true;
			}
			void read(int n_index, std::vector<unsigned char>& n_pData) {
				n_pData.clear();
				if (!m_file_edt.is_open() || n_index < 0 || n_index >= m_edb_inf.m_recordNum) {
					return;
				}

				m_file_edt.seekg(n_index * sizeof(edt_data), std::ios::beg);
				while (true) {
					edt_data temp_data;

					m_file_edt.read(reinterpret_cast<char*>(&temp_data), sizeof(temp_data));
					if (temp_data.m_data_size == 0 || m_file_edt.fail() || m_file_edt.bad()) {
						return;
					}

					n_pData.insert(n_pData.end(), temp_data.m_pbuffer, temp_data.m_pbuffer + temp_data.m_data_size);
					if (temp_data.m_next == 0) {
						return;
					}

					m_file_edt.clear();
					m_file_edt.seekg(temp_data.m_next * sizeof(edt_data), std::ios::beg);
				}
			}
			int write(const std::vector<unsigned char>& n_pData) {
				if (!m_file_edt.is_open() || n_pData.empty()) {
					return 0;
				}

				int data_len = static_cast<int>(n_pData.size());
				int last_segment_size = data_len % BUFFER_SIZE;
				int segment_num = data_len / BUFFER_SIZE + (last_segment_size > 0 ? 1 : 0);
				int s_last_index = 0;
				int ret = 0;

				for (int i = 0; i < segment_num; ++i) {
					edt_data temp_data;
					int i_index;
					if (m_edb_inf.m_un_using_count > 0) {
						i_index = m_edb_inf.m_nop_index;
						m_file_edt.seekg(i_index * sizeof(edt_data), std::ios::beg);
						m_file_edt.read(reinterpret_cast<char*>(&temp_data), sizeof(int) * 2);
						m_edb_inf.m_un_using_count--;
						m_edb_inf.m_nop_index = temp_data.m_next;
					}
					else {
						i_index = m_edb_inf.m_recordNum++;
					}

					auto i_next = m_edb_inf.m_un_using_count > 0 ? m_edb_inf.m_nop_index : m_edb_inf.m_recordNum;
					auto data_size = i == segment_num - 1 ? last_segment_size : BUFFER_SIZE;
					std::memcpy(temp_data.m_pbuffer, n_pData.data() + static_cast<size_t>(i) * static_cast<size_t>(BUFFER_SIZE), data_size);
					temp_data.m_data_size = data_size;

					if (i == 0) {
						ret = i_index;
						temp_data.m_previous = 0;
						temp_data.m_next = segment_num == 1 ? 0 : i_next;
					}
					else if (i == segment_num - 1) {
						temp_data.m_previous = s_last_index;
						temp_data.m_next = 0;
					}
					else {
						temp_data.m_previous = s_last_index;
						temp_data.m_next = i_next;
					}

					s_last_index = i_index;
					m_file_edt.seekp(i_index * sizeof(edt_data), std::ios::beg);
					m_file_edt.write(reinterpret_cast<const char*>(&temp_data), sizeof(temp_data));
				}

				return ret;
			}
			void del(int n_index) {
				if (!m_file_edt.is_open() || n_index < 0 || n_index >= m_edb_inf.m_recordNum) {
					return;
				}

				edt_data temp_data;
				m_file_edt.seekg(n_index * sizeof(edt_data), std::ios::beg);
				m_file_edt.read(reinterpret_cast<char*>(&temp_data), sizeof(int) * 2);

				if (temp_data.m_next != 0) {
					del(temp_data.m_next);
				}

				m_file_edt.seekp(n_index * sizeof(edt_data), std::ios::beg);
				temp_data.m_next = m_edb_inf.m_nop_index;
				m_edb_inf.m_nop_index = n_index;
				m_edb_inf.m_un_using_count++;
				m_file_edt.write(reinterpret_cast<const char*>(&temp_data), sizeof(int) * 2);
			}
			int change(int n_index, const std::vector<unsigned char>& n_pData) {
				del(n_index);
				if (n_pData.empty()) {
					return 0;
				}
				int new_index = write(n_pData);
				return new_index;
			}
			bool clean() {
				if (!is_open())
				{
					return false;
				}
				close(); // Reuse close() method
				if (!create(m_filename.c_str(), m_edb_inf.m_createTime))
					return false;
				if (!open(m_filename.c_str(), m_edb_inf.m_createTime))
					return false;;
				return true;
			}
			void close() {
				if (m_file_edt.is_open()) {
					m_file_edt.close();
				}
				m_filename.clear(); // Use clear() method instead of assigning a new empty string
				m_edb_inf = edt_header(); // This is already optimal
			}
			void updata() {
				if (!m_file_edt.is_open()) {
					return;
				}
				/*�����ļ�ͷ*/
				m_file_edt.seekp(sizeof(CHECK_EDT), std::ios::beg);
				m_file_edt.write(reinterpret_cast<char*>(&m_edb_inf), sizeof(m_edb_inf));
				m_file_edt.flush(); // This is already optimal
			};

		};
		std::map<ColumnDataType, int> type_lengths = {
			{ColumnDataType::BYTE, 1},
			{ColumnDataType::SHORT_INT, 2},
			{ColumnDataType::INT, 4},
			{ColumnDataType::LONG_INT, 8},
			{ColumnDataType::FLOAT, 4},
			{ColumnDataType::DOUBLE, 8},
			{ColumnDataType::BOOLEAN, 1},
			{ColumnDataType::DATE_TIME, 8},
			{ColumnDataType::PTR, 4},
			{ColumnDataType::BYTE_ARRAY, 4},
			{ColumnDataType::REMARK, 4},
			{ColumnDataType::PRIMARY_KEY, 4} // ������Ĭ�ϳ���Ϊ4
		};

		struct edb_header {

			double m_createTime = 0;//OLEʱ��
			int m_recordNum = 0;//��ǰ��¼����
			int m_unusedPrimaryKey = 1;//Ŀǰ��ʹ�õ�����,ÿ�β�������ݻ�����1,�����ظ�,������
			int  m_totalLength = 0;;//�����ֶ�����ĳ���,��λ�ֽ�
			unsigned char  m_blankBYTE[84]{ 0 };//Ԥ���ռ�
			int m_validColumnNum = 0;;//��Ч�ֶ���
		};
		struct colimn_data
		{
			char m_ColumnName[16]{ 0 };  // �������16�ֽ�,
			int m_delimiter = 0;    // �ָ���
			ColumnDataType m_ColumnType = ColumnDataType::TEXT;       // ��ʼƫ��λ��
			int m_offset = 0;           // ���ı����������ݳ���;
			int m_strlenth = 0;         // ��Ϊ�ı���������Ҫ����;
			unsigned char m_Table[40]{ 0 };
		};
		static constexpr char CHECK_EDB[] = { 'W', 'E', 'D', 'B', 0x00, 0x00, 0x01, 0x00 };
	public:/*ȫ�־�̬����*/
		static
			int
			create_edbs(const std::string& nfilename, const std::vector< ColumnInfo>& columns) {

			static  std::map<ColumnDataType, int> stype_lengths = {
			{ColumnDataType::BYTE, 1},
			{ColumnDataType::SHORT_INT, 2},
			{ColumnDataType::INT, 4},
			{ColumnDataType::LONG_INT, 8},
			{ColumnDataType::FLOAT, 4},
			{ColumnDataType::DOUBLE, 8},
			{ColumnDataType::BOOLEAN, 1},
			{ColumnDataType::DATE_TIME, 8},
			{ColumnDataType::PTR, 4},
			{ColumnDataType::BYTE_ARRAY, 4},
			{ColumnDataType::REMARK, 4},
			{ColumnDataType::PRIMARY_KEY, 4} // ������Ĭ�ϳ���Ϊ4
			};
			/*�����ֶε���Ч����֤��������ȡ*/
			std::unordered_set<std::string> columnNames;

			std::vector<colimn_data> wFileCoumns;

			//Ĭ��Ҫ�����ݿ����һ��������С,ͬʱҲ��ƫ�ƺ���չ��ȥ�ĳ���
			std::uint32_t all_columns_datasize = stype_lengths[ColumnDataType::PRIMARY_KEY];
			//��Ч�ֶ�����
			std::uint32_t valid_column_num = 0;

			bool bIsHaveBigData = false;
			if (columns.empty()) {
				return EDB_ERROR_INVALID_COLUMN_NAME;  // û����Ч����Ϣ
			}
			for (ColumnInfo column : columns)
			{

				// �ж��ֶ����Ƿ��Ѿ����ֹ�
				if (columnNames.count(column.m_name) > 0)
				{
					return EDB_ERROR_DUPLICATE_COLUMN_NAME;
				}
				columnNames.insert(column.m_name);


				if (!__is_vaild_name(column.m_name))
				{

					return  EDB_ERROR_INVALID_COLUMN_NAME;
				}


				if (column.m_dataType< ColumnDataType::BYTE || column.m_dataType > ColumnDataType::REMARK)
				{
					return EDB_ERROR_INVALID_COLUMN_TYPE;
				}



				//׼������д���ļ��е��ֶ�����
				colimn_data TempwFileCoumns;
				TempwFileCoumns.m_ColumnType = column.m_dataType;

				std::strcpy(TempwFileCoumns.m_ColumnName, column.m_name.c_str());
				TempwFileCoumns.m_ColumnName[15] = '\0';
				if (std::strlen(TempwFileCoumns.m_ColumnName) < 15)
				{
					std::memset(&TempwFileCoumns.m_ColumnName[strlen(TempwFileCoumns.m_ColumnName)], 0, 15 - strlen(TempwFileCoumns.m_ColumnName));
				}

				//ͨ�����ƺ������ж�����г�����֤
				//�������ж��Ƿ�Ϊ�ı���
				std::uint32_t TempColumnSize = 0;
				//EDBS�����ı�����󳤶Ƚ�������
				if (column.m_dataType == ColumnDataType::TEXT)
				{
					//��֤
					TempColumnSize = column.m_strDataLength;
					//׼��д�����������
					TempwFileCoumns.m_strlenth = column.m_strDataLength;
				}
				else
				{
					TempColumnSize = stype_lengths[column.m_dataType];
				}

				if (TempColumnSize == 0)
				{
					return EDB_ERROR_INVALID_COLUMN_SIZE;
				}

				if (column.m_dataType == ColumnDataType::REMARK || column.m_dataType == ColumnDataType::BYTE_ARRAY)
				{
					bIsHaveBigData = true;
				}
				TempwFileCoumns.m_offset = all_columns_datasize;
				wFileCoumns.push_back(TempwFileCoumns);
				all_columns_datasize += TempColumnSize;
				valid_column_num++;
			}

			auto filename = nfilename;// __rename_file_ext(nfilename, ".edbs");

			std::ofstream file(filename, std::ios::binary);
			if (!file.is_open())
			{
				// ����EDT�ļ�ʧ��
				return EDB_ERROR_CREATE_EDBS;
			}
			edb_header EDB;
			auto time = __get_nowtm_to_oletm();
			EDB.m_createTime = time;
			EDB.m_validColumnNum = valid_column_num;
			EDB.m_totalLength = all_columns_datasize;
			file.write(CHECK_EDB, sizeof(CHECK_EDB));
			file.write(reinterpret_cast<char*>(&EDB), sizeof(edb_header));
			// Ȼ��д���ֶ�����
			for (auto& column : wFileCoumns)
			{
				file.write(reinterpret_cast<char*>(&column), sizeof(colimn_data));
			}
			file.close();

			if (bIsHaveBigData)
			{
				edt_file::create(nfilename, time);
			}
			return EDB_ERROR_SUCCESS;
		}
	private:/*ȫ�ָ�������*/
		//��ȡʱ�䲢ת��ΪOLEʱ��,�˴���Ϊ����EDBԭ��
		inline static
			double
			__get_nowtm_to_oletm() {
			auto now = std::chrono::system_clock::now();
			auto time_t_now = std::chrono::system_clock::to_time_t(now);
			auto  t = static_cast<time_t>(time_t_now);
			return static_cast<double>(25569 + t / 86400.0 + 8.0 / 24.0);

		}
		/*EDBS��������й������ƣ���������ϸ�Ĺ�����飬��������������*/
		inline static
			bool
			__is_vaild_name(const std::string& text) {
			if (text.empty()
				|| text.size() > 16
				|| text.find(" ") != std::string::npos
				)
			{
				return false;
			}
			if ((text[0] >= '0' && text[0] <= '9') || text[0] == '.') {
				return false;
			}
			return true;
		}
		inline static
			std::string
			__rename_file_ext(const std::string& fname, const std::string& extname) {
			if (extname.empty()) {
				return fname;
			}
			std::filesystem::path filepath(fname);
			std::string suffix = extname;
			if (extname[0] != '.') {
				suffix = "." + suffix;
			}
			filepath.replace_extension(suffix);
			return filepath.string();
		}
	public:
		inline
			edb_file() = default;
		inline
			edb_file(const std::string& filename) :m_errorCode(EDB_ERROR_SUCCESS), m_dataOffset(0), m_cur_off(0), m_isTransactionOpened(false) {
			// �Ѿ��򿪾Ͳ��ٴ�
			if (m_file.is_open()) {
				m_errorCode = EDB_ERROR_HASBEEN_OPEN_EDBS;
				return;
			}

			m_file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return;
			}

			if (!__is_edbs_file()) {
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return;
			}

			// ��ȡEDB��Ϣ
			m_file.read(reinterpret_cast<char*>(&m_edbInf), sizeof(edb_header));

			// ����Ч�ֶ�
			if (m_edbInf.m_validColumnNum <= 0) {
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return;
			}

			auto allCoLimnsize = m_edbInf.m_validColumnNum * sizeof(colimn_data);
			auto allRowsize = m_edbInf.m_totalLength * m_edbInf.m_recordNum;

			// �ٴ���֤�ļ���С
			m_file.seekg(0, std::ios::end);
			size_t fileSize = m_file.tellg();

			if (fileSize < sizeof(edb_header) + sizeof(CHECK_EDB) + allCoLimnsize + allRowsize) {
				// �ļ�̫С�������ܶ�ȡ��������Ϣ���㹻������
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return;
			}

			m_file.seekg(sizeof(edb_header) + sizeof(CHECK_EDB));
			m_allCoLimns.resize(m_edbInf.m_validColumnNum);
			m_file.read(reinterpret_cast<char*>(m_allCoLimns.data()), allCoLimnsize);
			m_dataOffset = static_cast<int>(m_file.tellg());

			if (__has_big_data()) {
				// �򿪶�ӦEDT�ļ�
				if (!m_edt_file.open(__rename_file_ext(filename, ".EDT"), m_edbInf.m_createTime)) {
					m_file.close();
					// �ļ��޷���
					m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
					return;
				}
			}

			/*�����ʼ������*/
			if (m_edbInf.m_recordNum > 0) {
				m_cur_off = 1;
			}

			m_fileName = filename;
			return;
		}
		inline
			bool
			open(const std::string& filename) {
			// �Ѿ��򿪾Ͳ��ٴ�
			if (m_file.is_open()) {
				m_errorCode = EDB_ERROR_HASBEEN_OPEN_EDBS;
				return false;
			}

			m_file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return false;
			}

			if (!__is_edbs_file()) {
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return false;
			}

			// ��ȡEDB��Ϣ
			m_file.read(reinterpret_cast<char*>(&m_edbInf), sizeof(edb_header));

			// ����Ч�ֶ�
			if (m_edbInf.m_validColumnNum <= 0) {
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return false;
			}

			auto allCoLimnsize = m_edbInf.m_validColumnNum * sizeof(colimn_data);
			auto allRowsize = m_edbInf.m_totalLength * m_edbInf.m_recordNum;

			// �ٴ���֤�ļ���С
			m_file.seekg(0, std::ios::end);
			size_t fileSize = m_file.tellg();

			if (fileSize < sizeof(edb_header) + sizeof(CHECK_EDB) + allCoLimnsize + allRowsize) {
				// �ļ�̫С�������ܶ�ȡ��������Ϣ���㹻������
				m_file.close();
				m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
				return false;
			}

			m_file.seekg(sizeof(edb_header) + sizeof(CHECK_EDB));
			m_allCoLimns.resize(m_edbInf.m_validColumnNum);
			m_file.read(reinterpret_cast<char*>(m_allCoLimns.data()), allCoLimnsize);
			m_dataOffset = static_cast<int>(m_file.tellg());

			if (__has_big_data()) {
				// �򿪶�ӦEDT�ļ�
				if (!m_edt_file.open(__rename_file_ext(filename, ".EDT"), m_edbInf.m_createTime)) {
					m_file.close();
					// �ļ��޷���
					m_errorCode = EDB_ERROR_INVALID_EDBSFILE;
					return false;
				}
			}

			/*�����ʼ������*/
			if (m_edbInf.m_recordNum > 0) {
				m_cur_off = 1;
			}

			m_fileName = filename;
			return true;
		}
		inline
			void clean() {
			if (!m_file.is_open())
			{
				debug_put(m_file.is_open());
				return;
			}
			/*�ر�*/
			if (m_edt_file.is_open())
			{
				m_edt_file.close();
				//	m_edt_file.clean();
			}
	
			
			

			std::vector<ColumnInfo> _ColumnInfo;
			/*�ؽ�*/
			debug_put(m_allCoLimns.size());
			for (const auto& temp : m_allCoLimns)
			{
				ColumnInfo i_ColumnInfo;
				i_ColumnInfo.m_dataType = temp.m_ColumnType;
				i_ColumnInfo.m_name = temp.m_ColumnName;
				
				i_ColumnInfo.m_strDataLength = temp.m_strlenth;
				_ColumnInfo.push_back(i_ColumnInfo);
			}

			m_file.clear();
			m_file.close();
			char edt_file_name[MAX_PATH]{ 0 };
			strcpy_s(edt_file_name, m_fileName.c_str());
			PathRenameExtensionA(edt_file_name, "EDT");
			std::remove(m_fileName.c_str());
			std::remove(edt_file_name);

			m_errorCode = create_edbs(m_fileName, _ColumnInfo);
			debug_put(m_errorCode);
			/*��*/
			open(m_fileName);
	
		}
		inline
			void
			close() {
			if (m_edt_file.is_open())
			{
				m_edt_file.close();
			}
			m_fileName = {};
			if (m_file.is_open())
			{
				m_file.clear();
				m_file.close();
			}
			m_dataOffset = 0;
			m_errorCode = EDB_ERROR_SUCCESS;
			m_edbInf = {};
			m_allCoLimns = {};
			m_isTransactionOpened = false;
		}
		template<typename T>
		inline
			void
			read(int nIndex_column, int nIndex_row, T& pData) {
			std::memset(&pData, 0, sizeof(T));
			std::vector<unsigned char> vpData;
			read(nIndex_column, nIndex_row, vpData);
			if (vpData.size() < sizeof(T))
				return;
			pData = *reinterpret_cast<T*>(vpData.data());
		}
		inline
			void
			read(int nIndex_column, int nIndex_row, std::vector<unsigned char>& pData) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if ((nIndex_row >= 0 && nIndex_row + 1 > m_edbInf.m_recordNum) || (nIndex_column + 1 > m_edbInf.m_validColumnNum && nIndex_column >= 0))
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return;
			}
			m_file.seekg(m_dataOffset + nIndex_row * m_edbInf.m_totalLength + m_allCoLimns[nIndex_column].m_offset);

			//�ı��ͳ��������⴦��
			int needsize = m_allCoLimns[nIndex_column].m_strlenth;
			if (m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::TEXT)
			{	//����Ĭ�ϳ���
				needsize = type_lengths[m_allCoLimns[nIndex_column].m_ColumnType];
			}

			if (m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::BYTE_ARRAY && m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::REMARK)
			{
				pData.resize(needsize, 0);
				m_file.read(reinterpret_cast<char*>(pData.data()), pData.size());
			}
			else
			{
				/*�����ֽڼ��ͱ�ע��*/
				/*��������*/
				int index = 0;
				m_file.read(reinterpret_cast<char*>(&index), sizeof(index));
				if (index > 0)
				{
					/*��ָ��EDT�ļ���Ѱ������*/
					m_edt_file.read(index, pData);
				}

			}


		};
		/*������0,0��ʼ*/
		inline
			bool
			write(int nIndex_column, int nIndex_row, const std::vector<unsigned char>& data) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}
			if (m_edbInf.m_recordNum <= 0 || nIndex_row < 0)
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}
	
			if (nIndex_row + 1 > m_edbInf.m_recordNum || nIndex_column + 1 > m_edbInf.m_validColumnNum)
			{

				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}
			m_file.seekp(m_dataOffset + nIndex_row * m_edbInf.m_totalLength + m_allCoLimns[nIndex_column].m_offset);
			//�ı��ͳ��������⴦��
			int needsize = m_allCoLimns[nIndex_column].m_strlenth;
			if (m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::TEXT)
			{	//����Ĭ�ϳ���
				needsize = type_lengths[m_allCoLimns[nIndex_column].m_ColumnType];
			}

			if (m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::BYTE_ARRAY && m_allCoLimns[nIndex_column].m_ColumnType != ColumnDataType::REMARK)
			{
				/*���ֽڼ��ͱ�ע��*/
				if (data.size() >= needsize) {
					// ���ȳ�����ض�
					m_file.write(reinterpret_cast<const char*>(data.data()), needsize);
				}
				else {
					// ���Ȳ�������
					m_file.write(reinterpret_cast<const char*>(data.data()), data.size());
					if (data.size() < needsize) {
						auto padData = new char[needsize - data.size()] { 0 };
						m_file.write(padData, needsize - data.size());
						delete[]padData;
					}
				}
			}
			else
			{
				/*�����ֽڼ��ͱ�ע��*/
				/*��������*/
				m_file.seekg(m_dataOffset + nIndex_row * m_edbInf.m_totalLength + m_allCoLimns[nIndex_column].m_offset);
				int index = 0;
				m_file.read(reinterpret_cast<char*>(&index), sizeof(index));
				if (index > 0)
				{
					/*��ָ��EDT�ļ��д�������*/
					if (data.empty())
					{
						m_edt_file.del(index);
						index = 0;
					}
					else
					{
						index = m_edt_file.change(index, data);
					}
				}
				else
				{
					/*������*/
					if (data.empty())
					{
						return true;
					}
					else
					{
						index = m_edt_file.write(data);

					}
				}
				m_edt_file.updata();//IO
				m_file.seekp(m_dataOffset + nIndex_row * m_edbInf.m_totalLength + m_allCoLimns[nIndex_column].m_offset);
				m_file.write(reinterpret_cast<const char*>(&index), sizeof(int));

				/*�����ֽڼ��ͱ�ע��*/
			}
			if (!m_isTransactionOpened)
			{
				m_file.flush();
			}

			return  true;
		}

		inline
			bool
			del(int nIndex_row) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}
			if (m_edbInf.m_recordNum<=0)
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}
			if (nIndex_row <= 0)
				nIndex_row = m_cur_off-1 ;
			//debug_put(nIndex_row);
			if (nIndex_row <= 0) {
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}
			
			if (nIndex_row + 1 > m_edbInf.m_recordNum)
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}

			
			m_file.seekg(m_dataOffset + nIndex_row * m_edbInf.m_totalLength);
			int primary_key = 0;
			m_file.read(reinterpret_cast<char*>(&primary_key), sizeof(int));
			if (primary_key <= 0) {
				m_errorCode = EDB_ERROR_HAS_DEL_OR_INVALID;
				return false;
			}

			m_file.seekp(m_dataOffset + nIndex_row * m_edbInf.m_totalLength);
			primary_key = ~primary_key + 1;
			m_file.write(reinterpret_cast<char*>(&primary_key), sizeof(int));

			/*�����ֽڼ��ͱ�ע��*/

			if (!m_isTransactionOpened)
				m_file.flush();

			return  true;
		}
		inline
			bool
			cancel_del(int nIndex_row) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}
			if (m_edbInf.m_recordNum <= 0)
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}
			if (nIndex_row <= 0)
				nIndex_row = m_cur_off - 1;
			//debug_put(nIndex_row);
			if (nIndex_row <= 0) {
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}

			if (nIndex_row + 1 > m_edbInf.m_recordNum)
			{
				m_errorCode = EDB_ERROR_INVALID_INDEX;
				return false;
			}

			m_file.seekg(m_dataOffset + nIndex_row * m_edbInf.m_totalLength);
			int primary_key = 0;
			m_file.read(reinterpret_cast<char*>(&primary_key), sizeof(int));
			if (primary_key > 0) {
				m_errorCode = EDB_ERROR_HAS_DEL_OR_INVALID;
				return false;
			}

			m_file.seekp(m_dataOffset + nIndex_row * m_edbInf.m_totalLength);
			primary_key = ~primary_key + 1;
			m_file.write(reinterpret_cast<char*>(&primary_key), sizeof(int));

			/*�����ֽڼ��ͱ�ע��*/

			if (!m_isTransactionOpened)
				m_file.flush();

			return  true;
		}
		inline
			std::string
			get_column_name(int nIndex_column)const {
			if (!m_file.is_open()) {
				// �ļ��޷���
				return std::string();
			}
			if (nIndex_column < 0 || nIndex_column >= m_allCoLimns.size())
			{
				return std::string();
			}
			return m_allCoLimns[nIndex_column].m_ColumnName;
		}
		inline
			void
			set_column_name(int nIndex_column, const std::string& name) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				return;
			}
			if (nIndex_column < 0 || nIndex_column >= m_allCoLimns.size())
			{
				return;
			}
			//COLIMNDATA TempwFileCoumns = m_allCoLimns[nIndex_column];
			std::strcpy(m_allCoLimns[nIndex_column].m_ColumnName, name.c_str());
			m_allCoLimns[nIndex_column].m_ColumnName[15] = '\0';
			if (std::strlen(m_allCoLimns[nIndex_column].m_ColumnName) < 15)
			{
				std::memset(&m_allCoLimns[nIndex_column].m_ColumnName[strlen(m_allCoLimns[nIndex_column].m_ColumnName)], 0, 15 - strlen(m_allCoLimns[nIndex_column].m_ColumnName));
			}
			__updata_heade();
			;
		}
		inline
			bool
			append_blank(int nCount) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}
			if (nCount <= 0)
			{
				m_errorCode = EDB_ERROR_INVALID_ROWS_NUM;
				return false;
			}
			//�ļ�ͷ����
			m_edbInf.m_recordNum += nCount;
			m_file.seekp(0, std::ios::end);
			//�ı��ͳ��������⴦��


			std::vector<char> pData_nop(nCount * m_edbInf.m_totalLength, 0);
			for (size_t i = 0; i < nCount; i++)
			{

				*reinterpret_cast<int*>(&pData_nop[m_edbInf.m_totalLength * i]) = m_edbInf.m_unusedPrimaryKey++;

			}
			// ���ȳ�����ض�
			m_file.write(reinterpret_cast<const char*>(pData_nop.data()), pData_nop.size());

			if (!m_isTransactionOpened)
			{
				m_file.flush();
			}
			__updata_heade();
			to_end();
			return  true;
		}
		inline
			void
			to_end() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			m_cur_off = m_edbInf.m_recordNum;
		};
		inline
			bool pop() {
			/*����Ѱ�Ҵ���ɾ����ǵ���*/
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return false;
			}

			//size_t i = 0;
			//while (i< m_edbInf.m_recordNum)
			//{
			//	m_file.seekg(m_dataOffset + i * m_edbInf.m_totalLength, std::ios::beg);
			//	int primary_key = 0;
			//	m_file.read(reinterpret_cast<char*>(&primary_key), sizeof(int));
			//	if (primary_key < 0) {
			//		m_edbInf.m_recordNum--;
			//		__updata_heade();
			//		/*ɾ��ʱһ����ǿ��ˢ��*/
			//		std::vector<std::pair<std::streampos, std::streampos>> segments;
			//		segments.push_back({ m_dataOffset + i * m_edbInf.m_totalLength ,m_dataOffset + (i + 1) * m_edbInf.m_totalLength });
			//		__delete_segments(segments);
			//		m_file.flush();
			//	}
			//	i++;
			//}

			
			std::vector<std::pair<std::streampos, std::streampos>> segments;
			auto recordNu = m_edbInf.m_recordNum;
			for (int i = 0; i < recordNu; i++)
			{
				m_file.seekg(m_dataOffset + i * m_edbInf.m_totalLength, std::ios::beg);
				int primary_key = 0;
				m_file.read(reinterpret_cast<char*>(&primary_key), sizeof(int));
				debug_put(primary_key);
				if (primary_key < 0) {
					segments.push_back({ m_dataOffset + i * m_edbInf.m_totalLength ,m_dataOffset + (i + 1) * m_edbInf.m_totalLength });
					m_edbInf.m_recordNum--;
				}
			}
			__updata_heade();
			/*ɾ��ʱһ����ǿ��ˢ��*/
			m_file.flush();
			
			__delete_segments(segments);

			return true;;
		}

		inline
			int
			get_error_code() const {
			return m_errorCode;
		}
		inline
			int
			get_column_num()const {
			if (!m_file.is_open()) {
				// �ļ��޷���
				return 0;
			}
			return m_edbInf.m_validColumnNum;
		}
		using ReadCallback = std::function<void(int, const std::vector<uint8_t>&)>;
		/*�첽��ȡ*/
		inline
			//	void async_read(int column, int row, ReadCallback callback) {
			//	// �����߳���ִ�ж�������ͨ���ص�����
			//	std::thread([=]() {
			//		std::vector<uint8_t> data;
			//		read(column, row, data);
			//		callback(column, data);
			//		}).detach();
			//}
			inline
			void set_current(int cur_off) {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if (cur_off < 0)cur_off = m_edbInf.m_recordNum > 0 ? 1 : 0;
			else if (cur_off > m_edbInf.m_recordNum)cur_off = m_edbInf.m_recordNum;
			m_cur_off = cur_off;
		};
		inline
			void to_begin() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if (m_edbInf.m_recordNum > 0)
			{
				m_cur_off = 1;
			}
			m_cur_off = 0;
		};
		inline
			void next() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}

			if (m_cur_off + 1 > m_edbInf.m_recordNum)
			{
				return;
			}
			m_cur_off++;
		}
		inline
			void previous() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if (m_cur_off - 1 <= 0)
			{
				return;
			}
			m_cur_off--;
		}
		inline
			int get_current() const {
			return m_cur_off;
		}
		inline
			int
			get_row_num()const {
			if (!m_file.is_open()) {
				// �ļ��޷���
				return 0;
			}
			return m_edbInf.m_recordNum;
		}
		/*��ͬ��,��1-1��ʼ*/
		inline
			auto read(int nIndex_column) {
			std::vector<unsigned char> p_get_data;
			read(nIndex_column - 1, m_cur_off - 1, p_get_data);
			return p_get_data;
		}
		/*��ͬ��,��1-1��ʼ*/
		inline
			bool write(int nIndex_column, const std::vector<unsigned char>& pData) {
			return write(nIndex_column - 1, m_cur_off - 1, pData);
		}
		/*��ֹio*/
		inline
			void
			cant_be_io() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if (m_isTransactionOpened) {
				m_errorCode = EDB_ERROR_TRANSACTION_OPENED;
				return;
			}
			m_isTransactionOpened = true;
		}
		//ˢ��
		inline
			void
			commit_io() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			if (!m_isTransactionOpened) {
				m_errorCode = EDB_ERROR_TRANSACTION_NOT_OPENED;
				return;
			}
			m_file.flush();
		}

	private:
		inline
			void __delete_segments(const std::vector<std::pair<std::streampos, std::streampos>>& segments) {
			std::string tempFileName = __get_temp_file("");
			std::ofstream tempFile(tempFileName, std::ios::binary);

			// ��ȡ�ļ���С
			m_file.seekg(0, std::ios::end);
			std::streampos fileSize = m_file.tellg();
			m_file.seekg(0, std::ios::beg);

			std::streampos currentPosition = 0;

			for (const auto& segment : segments) {
				std::streampos start = segment.first;
				std::streampos end = segment.second;

				// �����ݶ�֮ǰ������д����ʱ�ļ�
				std::vector<char> buffer(start - currentPosition);
				m_file.read(buffer.data(), start - currentPosition);
				tempFile.write(buffer.data(), start - currentPosition);

				// ����Ҫɾ�������ݶ�
				m_file.seekg(end, std::ios::beg);
				currentPosition = end;
			}

			// ��ʣ�������д����ʱ�ļ�
			std::vector<char> buffer(fileSize - currentPosition);
			m_file.read(buffer.data(), fileSize - currentPosition);
			tempFile.write(buffer.data(), fileSize - currentPosition);

			// �ر��ļ�
			m_file.close();
			tempFile.close();

			// ɾ��ԭʼ�ļ�
			std::remove(m_fileName.c_str());

			// ��������ʱ�ļ�Ϊԭʼ�ļ���
			std::rename(tempFileName.c_str(), m_fileName.c_str());
		}
		//�����ļ�ͷ��,ÿ��������¼ʱ��Ҫ����
		inline
			void
			__updata_heade() {
			if (!m_file.is_open()) {
				// �ļ��޷���
				m_errorCode = EDB_ERROR_NOOPEN_EDBS;
				return;
			}
			//��¼����λ��
			auto now = m_file.tellp();
			//�Ƶ��ļ�ͷ,�������ļ���ʶλ��+
			m_file.seekp(sizeof(CHECK_EDB), std::ios::beg);
			m_file.write(reinterpret_cast<const char*>(&m_edbInf), sizeof(m_edbInf));
			//�ƶ���֮ǰ��λ��
			m_file.seekp(now, std::ios::beg);

			if (!m_isTransactionOpened)
			{
				m_file.flush();
			}
		}
		inline
			bool
			__has_big_data() const {
			if (m_allCoLimns.empty()) {
				return true;
			}

			for (const auto& temp : m_allCoLimns) {
				if (temp.m_ColumnType == ColumnDataType::REMARK || temp.m_ColumnType == ColumnDataType::BYTE_ARRAY) {
					return true;
				}
			}

			return false;
		}
#ifndef _WIN32
		inline static std::string
			__get_temp_file(const std::string& dir)
		{
			std::string tempFileName = dir + "/tempXXXXXX";  // �ļ���ģ�壬"XXXXXX"�ᱻ�滻Ϊ����ַ���

			char* tempFilePath = new char[tempFileName.size() + 1];
			std::strcpy(tempFilePath, tempFileName.c_str());

			int fileDescriptor = mkstemp(tempFilePath);  // ������ʱ�ļ�

			if (fileDescriptor == -1)
			{
				std::cerr << "�޷�������ʱ�ļ�" << std::endl;
				delete[] tempFilePath;
				return "";
			}

			close(fileDescriptor);  // �ر��ļ�������

			std::string result(tempFilePath);
			delete[] tempFilePath;

			return result;
		}
#else
		inline static std::string
			__get_temp_file(const std::string& dir) {
			std::string DirName;
			DirName.resize(MAX_PATH);

			if (dir.empty()) {
				if (GetTempPathA(MAX_PATH, DirName.data()) == FALSE)
					DirName.clear();
			}
			else {
				DirName = dir;
			}

			char TempFileName[MAX_PATH];
			std::size_t nLen = DirName.length();

			if (!DirName.empty() && DirName[nLen - 1] != '\\') {
				DirName += '\\';
			}

			do {
				std::sprintf(TempFileName, "%s%x.tmp", DirName.c_str(), GetTickCount());
			} while (PathFileExistsA(TempFileName));

			std::string pText(TempFileName);
			return pText;
		}
#endif
		inline bool
			__is_edbs_file() {
			// ȷ���ļ���С
			m_file.seekg(0, std::ios::end);
			if (m_file.tellg() < sizeof(edb_header) + sizeof(CHECK_EDB)) {
				// �ļ�̫С������������Ч��EDBS�ļ�
				return false;
			}

			m_file.seekg(0, std::ios::beg);
			char rfile_flag[sizeof(CHECK_EDB)]{ 0 };
			m_file.read(rfile_flag, sizeof(CHECK_EDB));

			if (std::memcmp(rfile_flag, CHECK_EDB, sizeof(CHECK_EDB)) != 0) {
				// �ļ�����EDBS�ļ�
				return false;
			}

			return true;
		}


	private:
		int m_cur_off{ 0 };                      // ��ǰ��
		edt_file m_edt_file;                // EDT�ļ�����
		std::string m_fileName;             // �ļ���
		std::fstream m_file;                // �ļ���
		int m_dataOffset{ 0 };                   // ���ݿ�ʼλ��
		int m_errorCode{ EDB_ERROR_SUCCESS };                    // ������
		edb_header m_edbInf;                 // ���ݱ���Ϣ
		std::vector<colimn_data> m_allCoLimns;// �ֶ���Ϣ
		bool m_isTransactionOpened{ false };         // �Ƿ�������
	};




}
#endif




static ARG_INFO Args[] =
{
	{
		/*name*/    "�ļ���",
		/*explain*/ ("�������������ݿ��ļ�,�˴������п��ܲ�����ԭ���������ԭ�洴����һ�������°�����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "�ֶ���Ϣ",
		/*explain*/ ("�������������ݿ��ļ�,�˴������п��ܲ�����ԭ���������ԭ�洴����һ�������°�����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    DATA_TYPE::KRNLN_EDB,
		/*default*/ 0,
		/*state*/   ArgMark::AS_RECEIVE_ARRAY_DATA,
		},
};



EXTERN_C void Fn_CreateEbds(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{



	if (pArgInf[0].m_dtDataType != SDT_TEXT)
	{
		pRetData->m_bool = FALSE;
		return;
	}

	const char* filename = pArgInf[0].m_pText;


	if (nArgCount != 2)
	{
		pRetData->m_bool = FALSE;
		return;
	}

	int nElementCount;

	lpColumInfo* pAryDataBegin = elibstl::get_array_element_inf<lpColumInfo*>(pArgInf[1].m_pAryData, &nElementCount);

	if (nElementCount < 1) {//�������涨����Ϊ����
		pRetData->m_bool = FALSE;
		return;
	}
	std::vector<elibstl::ColumnInfo> InAryData;
	if (!pAryDataBegin || nElementCount <= 0)
	{
		pRetData->m_bool = FALSE;
		return;
	}

	if (nElementCount > 0)
	{
		for (DWORD i = 0; i < nElementCount; i++) {
			if (pAryDataBegin[i])
			{
				elibstl::ColumnInfo Temp;
				Temp.m_name = pAryDataBegin[i]->Name == nullptr ? "" : pAryDataBegin[i]->Name;
				Temp.m_strDataLength = pAryDataBegin[i]->StrDataLenth;
				Temp.m_dataType = static_cast<elibstl::ColumnDataType> (pAryDataBegin[i]->Type);
				InAryData.push_back(Temp);
			}


		}

	}//memcppy
	BOOL ret = elibstl::edb_file::create_edbs(filename, InAryData) == 0;

	pRetData->m_bool = ret;
}

FucInfo e_lib_CreateEbds = { {
		/*ccname*/  ("���������ݿ�"),
		/*egname*/  ("create_edbs"),
		/*explain*/ ("�������������ݿ��ļ�,�˴������п��ܲ�����ԭ���������ԭ�洴����һ�������°�����"),
		/*category*/14,
		/*state*/   NULL,
		/*ret*/     SDT_BOOL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  &Args[0],
	} ,Fn_CreateEbds ,"Fn_CreateEbds" };


//����
EXTERN_C void Fn_edbs_ex_structure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self = new elibstl::edb_file;
}
//����
FucInfo edbs_ex_structure = { {
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
	} ,Fn_edbs_ex_structure ,"Fn_edbs_ex_structure" };

//����
EXTERN_C void fn_edbs_ex_copy(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	auto rht = static_cast<elibstl::edb_file*>(*pArgInf[1].m_ppCompoundData);
	put_errmsg(L"���ݿ��ļ���д��Ψһ��,��Ӧ�ڲ�ͬλ�ô�����������дͬһ�����ݿ��ļ�,�˻����������޵е��쳣,����汾��ͬ�������!");
	exit(0);
	//*self = *rht;
}
FucInfo edbs_ex_copy = { {
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
		/*ArgCount*/0,
		/*arg lp*/  NULL,
	} ,fn_edbs_ex_copy ,"fn_edbs_ex_copy" };

//����
EXTERN_C void fn_edbs_ex_des(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	if (self)
	{
		self->~edb_file();
		operator delete(self);
	}
	self = nullptr;
}
FucInfo edbs_ex_destruct = { {
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
	} ,fn_edbs_ex_des ,"fn_edbs_ex_des" };


//��
EXTERN_C void fn_edbs_ex_open(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	if (!pArgInf[1].m_pText)
	{
		pRetData->m_bool = false;
		return;
	}
	pRetData->m_bool = self->open(pArgInf[1].m_pText);
}
static ARG_INFO edbs_ex_open_Args[] =
{
	{
		/*name*/    "�ļ�·��",
		/*explain*/ ("edbs/edb���ݿ��ļ�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
FucInfo edbs_ex_open = { {
		/*ccname*/  "��",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  edbs_ex_open_Args,
	} ,fn_edbs_ex_open ,"fn_edbs_ex_open" };


//ȡ��¼��
EXTERN_C void fn_edbs_get_row_num(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_int = self->get_row_num();
}




EXTERN_C void fn_edbs_ex_addpendnop(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_bool = self->append_blank(pArgInf[1].m_int);
}
static ARG_INFO edbs_ex_addpendnop_Args[] =
{
	{
		/*name*/    "����",
		/*explain*/ ("����������ݵ�����"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
FucInfo	edbs_ex_addpendnop = { {
		/*ccname*/  "�ӿռ�¼",
		/*egname*/  "",
		/*explain*/ "�������ڵ�ǰ���ݿ��β�����һ���ռ�¼��������ǰ��¼ָ���ƶ������ļ�¼�ϡ��ɹ������棬ʧ�ܷ��ؼ١�",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  edbs_ex_addpendnop_Args,
	} ,fn_edbs_ex_addpendnop ,"fn_edbs_ex_addpendnop" };

FucInfo edbs_get_row_num = { {
		/*ccname*/  "ȡ��¼��",
		/*egname*/  "",
		/*explain*/ NULL,
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_get_row_num ,"fn_edbs_get_row_num" };






//����
EXTERN_C void fn_edbs_set_current(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);

	self->set_current(pArgInf[1].m_int);
}
static ARG_INFO edbs_set_current_Args[] =
{
	{
		/*name*/    "λ��",
		/*explain*/ ("��������λ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
FucInfo	edbs_set_current = { {
		/*ccname*/  "����",
		/*egname*/  "",
		/*explain*/ "���õ�ǰλ��,���ڼ�¼�����������޷�����0�ģ������ڵ�����һֱΪ0",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  edbs_set_current_Args,
	} ,fn_edbs_set_current ,"fn_edbs_set_current" };



//��
EXTERN_C void fn_edbs_read(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	auto temp = self->read(pArgInf[1].m_int);
	pRetData->m_pBin = elibstl::clone_bin(temp.data(), temp.size());
}
static ARG_INFO fn_edbs_read_Args[] =
{
	{
		/*name*/    "�ֶλ��ֶ�����",
		/*explain*/ ("����ȡ���ֶ�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
FucInfo	edbs_read = { {
		/*ccname*/  "��",
		/*egname*/  "��ȡ��ǰ�е�ָ���ֶ�����",
		/*explain*/ "�������ڵ�ǰ���ݿ��β�����һ���ռ�¼��������ǰ��¼ָ���ƶ������ļ�¼�ϡ��ɹ������棬ʧ�ܷ��ؼ١�",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BIN,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  fn_edbs_read_Args,
	} ,fn_edbs_read ,"fn_edbs_read" };


EXTERN_C void fn_edbs_write(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_bool = self->write(pArgInf[1].m_int, elibstl::arg_to_vdata(pArgInf, 2));
}
static ARG_INFO fn_edbs_write_Args[] =
{
	{
		/*name*/    "�ֶλ��ֶ�����",
		/*explain*/ ("����ȡ���ֶ�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},{
		/*name*/    "����",
		/*explain*/ ("��Ӧ�ֶε����ݣ���������0��������ض�"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_BIN,
		/*default*/ 0,
		/*state*/  ArgMark::AS_NONE,
	}
};
FucInfo	edbs_write = { {
		/*ccname*/  "д",
		/*egname*/  "",
		/*explain*/ " ������д�뵽��ǰ���ݿ��е�ǰ��¼����ָ���ֶ��ڡ��ɹ�������",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  fn_edbs_write_Args,
	} ,fn_edbs_write ,"fn_edbs_write" };




EXTERN_C void fn_edbs_close(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->close();
}

FucInfo	edbs_close = { {
		/*ccname*/  "�ر�",
		/*egname*/  "",
		/*explain*/ " ������ر��Ѿ����ݿ�",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_close ,"fn_edbs_close" };

EXTERN_C void fn_edbs_next(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->next();
}

FucInfo	edbs_next = { {
		/*ccname*/  "��һ��",
		/*egname*/  "",
		/*explain*/ "�ƶ�����һ��",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_next ,"fn_edbs_next" };

EXTERN_C void fn_edbs_previous(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->previous();
}

FucInfo	edbs_previous = { {
		/*ccname*/  "��һ��",
		/*egname*/  "",
		/*explain*/ " �ƶ�����һ��",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_previous ,"fn_edbs_previous" };


EXTERN_C void fn_edbs_clean(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->clean();
}

FucInfo	edbs_clean = { {
		/*ccname*/  "���",
		/*egname*/  "",
		/*explain*/ "������ݿ�",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_clean ,"fn_edbs_clean" };

EXTERN_C void fn_edbs_begin(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->cant_be_io();
}

FucInfo	edbs_begin = { {
		/*ccname*/  "��ֹIO",
		/*egname*/  "cant_be_io",
		/*explain*/ "��Ϊ��ֹIO,ֱ���ύ����֮ǰ����д�붼����ʵʱ���µ��ļ�",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_begin ,"fn_edbs_begin" };

EXTERN_C void fn_edbs_commit(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->commit_io();
}

FucInfo	edbs_commit = { {
		/*ccname*/  "����IO",
		/*egname*/  "",
		/*explain*/ "ˢ��,�����и���д�����",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_commit ,"fn_edbs_commit" };

EXTERN_C void fn_edbs_get_current(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_int = self->get_current();
}

FucInfo	edbs_get_current = { {
		/*ccname*/  "ȡ��ǰλ��",
		/*egname*/  "",
		/*explain*/ "��ȡ��ǰ��¼ָ����λ��",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_get_current ,"fn_edbs_get_current" };


EXTERN_C void fn_edbs_get_column_name(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_pText = elibstl::clone_text(self->get_column_name(pArgInf[1].m_int));
}

FucInfo	edbs_get_column_name = { {
		/*ccname*/  "ȡ�ֶ���",
		/*egname*/  "",
		/*explain*/ "��ȡָ���ֶ��������ֶ���",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_TEXT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  fn_edbs_read_Args,
	} ,fn_edbs_get_column_name ,"fn_edbs_get_column_name" };


EXTERN_C void fn_edbs_get_column_num(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_int = self->get_column_num();
}

FucInfo	edbs_get_column_num = { {
		/*ccname*/  "ȡ�ֶ���",
		/*egname*/  "",
		/*explain*/ "��ȡ���ݿ���ֶ���Ŀ",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_INT,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/  0,
	} ,fn_edbs_get_column_num ,"fn_edbs_get_column_num" };


EXTERN_C void fn_edbs_set_column_name(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	self->set_column_name(pArgInf[1].m_int, std::string(elibstl::args_to_sdata(pArgInf, 2)));
}
static ARG_INFO fn_edbs_set_column_name_Args[] =
{
	{
		/*name*/    "�ֶ�����",
		/*explain*/ (""),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	},
	{
		/*name*/    "���޸�Ϊ���ֶ���",
		/*explain*/ (""),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_TEXT,
		/*default*/ 0,
		/*state*/   ArgMark::AS_NONE,
	}
};
FucInfo	edbs_set_column_name = { {
		/*ccname*/  "���ֶ���",
		/*egname*/  "",
		/*explain*/ "����ָ���ֶ��������ֶ���",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ _SDT_NULL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/2,
		/*arg lp*/  fn_edbs_set_column_name_Args,
	} ,fn_edbs_set_column_name ,"fn_edbs_set_column_name" };






EXTERN_C void fn_edbs_del(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_bool = self->del(pArgInf[1].m_int - 1);
}
static ARG_INFO fn_edbs_del_Args[] =
{
	{
		/*name*/    "�ֶ�����",
		/*explain*/ ("С�ڵ���0��Ĭ��Ϊ��ǰλ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ -1,
		/*state*/   ArgMark::AS_HAS_DEFAULT_VALUE,
	}
};
FucInfo	edbs_del = { {
		/*ccname*/  "ɾ��",
		/*egname*/  "",
		/*explain*/ "����ɾ�����",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  fn_edbs_del_Args,
	} ,fn_edbs_del ,"fn_edbs_del" };


EXTERN_C void fn_edbs_pop(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_bool = self->pop();
}

FucInfo	edbs_pop = { {
		/*ccname*/  "����ɾ��",
		/*egname*/  "pop",
		/*explain*/ "ɾ�����б�ǵļ�¼",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/0,
		/*arg lp*/ 0,
	} ,fn_edbs_pop ,"fn_edbs_pop" };


EXTERN_C void fn_edbs_cancel_del(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	auto& self = elibstl::args_to_obj<elibstl::edb_file>(pArgInf);
	pRetData->m_bool = self->cancel_del(pArgInf[1].m_int - 1);
}
static ARG_INFO fn_edbs_cancel_del_Args[] =
{
	{
		/*name*/    "�ֶ�����",
		/*explain*/ ("С�ڵ���0��Ĭ��Ϊ��ǰλ��"),
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*type*/    SDT_INT,
		/*default*/ -1,
		/*state*/   ArgMark::AS_HAS_DEFAULT_VALUE,
	}
};
FucInfo	edbs_cancel_del = { {
		/*ccname*/  "ȡ��ɾ��",
		/*egname*/  "",
		/*explain*/ "ȡ���Ѿ����ϵ�ɾ�����",
		/*category*/ -1,
		/*state*/    _CMD_OS(__OS_WIN) ,
		/*ret*/ SDT_BOOL,
		/*reserved*/0,
		/*level*/   LVL_SIMPLE,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  fn_edbs_cancel_del_Args,
	} ,fn_edbs_cancel_del ,"fn_edbs_cancel_del" };




static INT s_dtCmdIndexcommobj_edbs_ex[] = { 132,133,134,135 ,136,137,138 ,139 ,140 ,141,142,143,144,145,146,147,148,149,150,300,301,302 };
namespace elibstl {


	LIB_DATA_TYPE_INFO edbs_ex =
	{
		"�����ݿ�",
		"EDBS",
		"���������ݿ�Ķ������",
		sizeof(s_dtCmdIndexcommobj_edbs_ex) / sizeof(s_dtCmdIndexcommobj_edbs_ex[0]),
		 s_dtCmdIndexcommobj_edbs_ex,
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