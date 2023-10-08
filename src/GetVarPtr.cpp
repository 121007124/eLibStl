#include"ElibHelp.h"








static ARG_INFO Args[] =
{
		{
			"����ȡ���ַ�ı���",
			"",
			0,
			0,
			_SDT_ALL,
			0,
			ArgMark::AS_RECEIVE_VAR_OR_ARRAY,
		}
};
EXTERN_C void libstl_fn_get_var_ptr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
	*pRetData = *pArgInf;
}

EXTERN_C void libstl_fn_get_var_data_ptr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
	auto primitive_type = pArgInf->ret_type_noflag();
	/*������*/
	if (pArgInf->is_dt_flag()) {
		/*��ʹ�ð�,������������*/
		auto get_arr_p = [](void* pAryData)->void* {
			if (pAryData == nullptr) return nullptr;
			auto pn_data = static_cast<int*>(pAryData);
			int count = *pn_data++; //��ȡά��
			size_t nElementCount = 1; //����Ԫ������
			for (auto it = pn_data; it != pn_data + count; ++it)
			{
				nElementCount *= *it;
			}
			return reinterpret_cast<void*>(pn_data + count);
			};
		pRetData->m_pAryData = get_arr_p(*pArgInf->m_ppAryData);
	}
	else if (primitive_type == SDT_TEXT)/*�������*/
	{
		auto pText = *pArgInf->m_ppText;
		pRetData->m_int = reinterpret_cast<uintptr_t>((pText == nullptr || strlen(pText) == 0) ? nullptr : pText);
	}
	else if (primitive_type == SDT_BIN) {
		auto pData = elibstl::classhelp::eplarg::get_bin(*pArgInf->m_ppBin);
		if (pData.size() > 0)
			pRetData->m_int = reinterpret_cast<uintptr_t>(pData.data());
	}
	else
		*pRetData = *pArgInf;

}
FucInfo Fn_GetVarPtr = { {
		/*ccname*/  ("ȡ����ָ��"),
		/*egname*/  ("GetVarPtr"),
		/*explain*/ ("����ָ���������ڴ��ַ���������������п�����ִ�б�������������EAX�Ĵ�����ã��������޸ĸõ�ַ�����ݣ��������ı��͡��ֽڼ��ͻ򸴺��������ͱ����������������������ܿ�������ڴ���������������������Ϊ�߼����"),
		/*category*/12,
		/*state*/   0,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  Args,
	} ,ESTLFNAME(libstl_fn_get_var_ptr) };

FucInfo Fn_GetVarDataPtr = { {
		/*ccname*/  ("ȡ��������ָ��"),
		/*egname*/  ("GetVarDataPtr"),
		/*explain*/ ("ȡָ�����������ݵ��ڴ��ַ������ֵΪ0��ʾû��ȡ��������ַ�����ı��ͻ��ֽڼ��ͱ������ԣ�����䳤��Ϊ0��������0��������������ԣ�������Ա��Ϊ0��Ҳ������0�������ı��ͻ��ֽڼ��ͱ���������ֵΪ�ı����ݻ��ֽڼ����ݵĵ�ַ����������£��������롰ȡ������ַ��������ͬ�ķ���ֵ��ע�⣺һ������������������Χ�����ı���/�ֽڼ��ͱ��������¸�ֵ����������������¸�ֵ����ǰȡ���ı�����ַ�������Ч��ַ���ٶԸõ�ַ���ж�д�������п��ܵ��³��������������Ϊ�߼����"),
		/*category*/12,
		/*state*/   0,
		/*ret*/     SDT_INT,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
		/*ArgCount*/1,
		/*arg lp*/  Args,
	} ,ESTLFNAME(libstl_fn_get_var_data_ptr) };