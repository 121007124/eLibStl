// pch.h: ����Ԥ�����ͷ�ļ���
// �·��г����ļ�������һ�Σ�����˽������ɵ��������ܡ�
// �⻹��Ӱ�� IntelliSense ���ܣ�����������ɺ�������������ܡ�
// ���ǣ�����˴��г����ļ��е��κ�һ��������֮���и��£�����ȫ�����������±��롣
// �����ڴ˴����ҪƵ�����µ��ļ����⽫ʹ������������Ч��

#ifndef PCH_H
#define PCH_H

// ���Ҫ�ڴ˴�Ԥ����ı�ͷ
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <EKrnln_typedef.h>
#include <stdlib.h>

#define IEXT_NAME(_index, _name) iext_##_name##_##_index##_iext

#if 1
#define dbg_msg tstr_MessageBox
#else
#define dbg_msg
#endif



// ֧�ֿ����, ����֧�ֿ���Ϣ, ������Ϣ����dll��
IEKrnln IextEntry();

unsigned char* GetELibStlFneDllData(int* size);
unsigned char* GetELibStlFnrDllData(int* size);




#endif //PCH_H
