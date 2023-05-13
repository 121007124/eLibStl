#include <sstream>  
#include <iomanip>
#include"ElibHelp.h"


static ARG_INFO Args[] =
{
		{
		/*name*/	"�����ֵ",
		/*explain*/	"���ڷǡ��ı��͡����������Զ�ת��Ϊ�ı����������ɽ�������������ͣ��������������Լ��Զ����������͡�",
		/*bmp inx*/	0,
		/*bmp num*/	0,
		/*type*/	_SDT_ALL,
		/*default*/	0,
		/*state*/	AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_ALL_TYPE_DATA,
			}
};



template <typename T>
inline std::basic_string<T> ByteArrayToString(const std::vector<unsigned char>& byteVector) {
	if (byteVector.empty()) {
		return std::basic_string<T>();
	}

	const size_t length = byteVector.size();
	std::vector<T> buffer(length * 4 + sizeof("{}"));

	size_t bufferIndex = 0;
	buffer[bufferIndex++] = '{';

	for (const auto byte : byteVector) {
		auto hundreds = byte / 100;
		auto tens = byte % 100 / 10;
		auto ones = byte % 10;

		if (hundreds > 0) {
			buffer[bufferIndex++] = hundreds + T('0');
			buffer[bufferIndex++] = tens + T('0');
		}
		else if (tens > 0) {
			buffer[bufferIndex++] = tens + T('0');
		}
		buffer[bufferIndex++] = ones + T('0');
		buffer[bufferIndex++] = ',';
	}
	buffer[bufferIndex - 1] = '}';

	return std::basic_string<T>(buffer.data(), bufferIndex);
}


/*����������ԭ��*/
inline std::string DateTimeFormat(DATE dtDt)
{
	SYSTEMTIME st = { 0 };
	VariantTimeToSystemTime(dtDt, &st);
	std::ostringstream ss;
	ss << st.wYear << "��" << st.wMonth << "��" << st.wDay << "��";
	ss << std::setfill('0') << std::right << " " << std::setw(2) << st.wHour << "ʱ" << std::setw(2) << st.wMinute << "��";
	if (st.wSecond)
	{
		ss << std::setw(2) << st.wSecond << "��";
	}
	return ss.str();
}

// ��ӡ���������ĺ���
static void printParam(std::ostringstream& oss, const PMDATA_INF& param)
{
	// �����������ͽ��з�֧����
	switch (param->m_dtDataType)
	{
	case SDT_INT:
		oss << param->m_int << " | "; break;
	case SDT_SHORT:
		oss << param->m_short << " | "; break;
	case SDT_BYTE:
		oss << static_cast<int>(param->m_byte) << " | "; break;
	case SDT_INT64:
		oss << param->m_int64 << " | "; break;
	case SDT_FLOAT:
		oss << param->m_float << " | "; break;
	case SDT_DOUBLE:
		oss << param->m_double << " | "; break;
	case SDT_BOOL:
		oss << (param->m_bool ? "true" : "false") << " | "; break;
	case SDT_TEXT:
	{
		const char* pText = param->m_pText;
		if (pText == nullptr || *pText == '\0')
		{
			oss << "[���ı�] | ";
		}
		else
		{
			oss << "[�ı�:" << strlen(pText) << "] ��" << pText << "�� | ";
		}
		break;
	}
	case SDT_BIN:
	{
		const auto& bytes = elibstl::arg_to_vdata(param->m_pBin);
		oss << "[�ֽڼ�:" << bytes.size() << "] " << ByteArrayToString<char>(bytes) << " | ";

		//ByteArrayToString<char>(bytes);

		//for (const auto& b : bytes)
		//{
		//	oss << static_cast<int>(b) << ",";
		//}
		//if (!bytes.empty()) { oss.seekp(-1, std::ios_base::end); }  // ȥ�����һ������
		//oss << "} | ";
		break;
	}
	case SDT_DATE_TIME:
		oss << "[" << DateTimeFormat(param->m_date) << "] | "; break;
	case SDT_SUB_PTR:
		oss << "(�ӳ����ַ:" << param->m_dwSubCodeAdr << ") | "; break;
	case _SDT_NULL:
		oss << "(��) | "; break;
	default:
		oss << "(?δ֪?) | "; break;
	}
}



inline int GetTypeSize(const int dtDataType) {
	switch (dtDataType) {
	case SDT_INT:
		return sizeof(INT);
	case SDT_SHORT:
		return sizeof(SHORT);
	case SDT_BYTE:
		return sizeof(BYTE);
	case SDT_INT64:
		return sizeof(INT64);
	case SDT_FLOAT:
		return sizeof(FLOAT);
	case SDT_DOUBLE:
		return sizeof(DOUBLE);
	case SDT_BOOL:
		return sizeof(BOOL);
	case SDT_TEXT:
		return sizeof(LPVOID);
	case SDT_BIN:
		return sizeof(LPVOID);
	case SDT_DATE_TIME:
		return sizeof(DATE);
	case SDT_SUB_PTR:
		return sizeof(LPVOID);
	case _SDT_NULL:
		return 1;
	default:
		return 1;
	}
}

static void printParamArray(std::ostringstream& oss, PMDATA_INF pParam)
{

	const int nDimension = elibstl::get_array_dimension(pParam->m_pAryData);
	oss << "[" << nDimension << "ά����:";
	int nElementCount = 0;
	for (int i = 0; i < nDimension; i++) {
		oss << (i == 0 ? "" : "-") << elibstl::get_array_count(pParam->m_pAryData, i + 1);
		if (i == 0)
		{
			nElementCount = elibstl::get_array_count(pParam->m_pAryData, 1);
		}
		else
		{
			nElementCount *= elibstl::get_array_count(pParam->m_pAryData, i + 1);
		}


	}
	oss << "] {";
	MessageBoxA(0, oss.str().c_str(), 0, 0);
	const auto pStart = reinterpret_cast<const LPBYTE>(pParam->m_pAryData) + (nDimension + 1) * sizeof(INT);
	const auto pEnd = pStart + nElementCount * GetTypeSize(pParam->m_dtDataType);
	std::string s = ", ";
	for (auto p = pStart; p < pEnd; p += GetTypeSize(pParam->m_dtDataType)) {

		if (p + GetTypeSize(pParam->m_dtDataType) == pEnd) {
			s = "";
		}
		switch (pParam->m_dtDataType)
		{
		case SDT_INT:
			oss << *(INT*)p;
			break;
		case SDT_SHORT:
			oss << *(SHORT*)p;
			break;
		case SDT_BYTE:
			oss << static_cast<int>(*(BYTE*)p);
			break;
		case SDT_INT64:
			oss << *(INT64*)p;
			break;
		case SDT_FLOAT:
			oss << *(FLOAT*)p;
			break;
		case SDT_DOUBLE:
			oss << *(DOUBLE*)p;
			break;
		case SDT_BOOL:
			oss << ((*(BOOL*)p) == TRUE ? "��" : "��");
			break;
		case SDT_TEXT:
		{
			char* pText = (char*)(*(LPVOID*)p);
			if (pText == NULL || strlen(pText) == 0)
			{
				oss << "���ı�:����";
				break;
			}
			oss << "�ı�:" << strlen(pText) << "��" << std::string(pText) << "��";

			break;
		}
		case SDT_BIN:
		{

			if ((*(LPVOID*)p) == 0)
			{
				oss << "�ֽڼ�:0{}";
				break;
			}
			std::vector<unsigned char> pBin = elibstl::arg_to_vdata((LPBYTE)(*(LPVOID*)p));
			oss << "�ֽڼ�:" << pBin.size() << "{";
			for (size_t i = 0; i < pBin.size(); i++)
			{
				oss << static_cast<int>(pBin.data()[i]);
				oss << ",";
			}
			oss.seekp(-1, std::ios_base::end); // ɾ�����һ������
			oss << "} ";
			break;
		}
		case SDT_DATE_TIME:
		{
			const DATE temp = *(DATE*)p;  p += sizeof(DATE);
			oss << "[" << DateTimeFormat(temp) << "],";
			break;
		}
		case SDT_SUB_PTR:
			oss << "(�ӳ����ַ:" + std::to_string(*reinterpret_cast<const DWORD*>(p));
			break;
		case _SDT_NULL:
			oss << "(��)";
			break;
		default:
			oss << "(?δ֪?)";
			break;
		}
		oss << s;
	}
	//oss.seekp(-1, std::ios::end);
	oss << "} | ";
}







EXTERN_C void Fn_debugput(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

	std::ostringstream str;
	for (int i = 0; i < nArgCount; i++)
	{
		PMDATA_INF pParam = &pArgInf[i];
		BOOL bIsArray = ((pParam->m_dtDataType & DT_IS_ARY) != 0);
		if (bIsArray) pParam->m_dtDataType &= (~DT_IS_ARY);
		if (!bIsArray) //����Ϊ���������
		{
			printParam(str, pParam);
		}
		else //����Ϊ��������
		{
			printParamArray(str, pParam);
		}
	}
	//��" | "ȥ��
	if (str.str().size() >= 3) {
		str.str(str.str().substr(0, str.str().size() - 3));
	}
	MDATA_INF RetData, ArgInf;
	ArgInf.m_dtDataType = SDT_TEXT;
	char* pDebugText = elibstl::clone_text(str.str());
	ArgInf.m_pText = pDebugText;
	elibstl::CallElibFunc("krnln.fne", "��������ı�", &RetData, 1, &ArgInf);

}

FucInfo e_debugput = { {
		/*ccname*/  ("����"),
		/*egname*/  ("debugput"),
		/*explain*/ ("ͬ�����Ե������"),
		/*category*/11,
		/*state*/    CT_ALLOW_APPEND_NEW_ARG | CT_DISABLED_IN_RELEASE,
		/*ret*/     _SDT_NULL,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  &Args[0],
	} ,Fn_debugput ,"Fn_debugput" };

