#pragma once

#define ___VER_1(_major, _minor, _build) L#_major##"_"###_minor##"_"###_build
#define ___VER_2(_major, _minor, _build) L#_major##"."###_minor##"."###_build
#define ___VER_3(_major, _minor, _build) ___VER_1(_major, _minor, _build)
#define ___VER_4(_major, _minor, _build) ___VER_2(_major, _minor, _build)

#define ___VER_1A(_major, _minor, _build) #_major##"_"###_minor##"_"###_build
#define ___VER_2A(_major, _minor, _build) #_major##"."###_minor##"."###_build
#define ___VER_3A(_major, _minor, _build) ___VER_1A(_major, _minor, _build)
#define ___VER_4A(_major, _minor, _build) ___VER_2A(_major, _minor, _build)

// �汾��ͳһ����һ��λ��, �õ��汾�ŵ�cpp�ļ��Լ��������ͷ�ļ�, ��Ҫ��.h�ļ�������
// һ���޸İ汾�ž������޸�

#define EKRNLN_MAJORVERSION               2         // ֧�ֿ����汾��
#define EKRNLN_MINORVERSION               0         // ֧�ֿ�ΰ汾��
#define EKRNLN_BUILDVERSION               1         // ֧�ֿ����汾��
#define EKRNLN_VERSION                    ___VER_4(EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION)    // ֧�ֿ�汾��
#define EKRNLN_VERSION_                   ___VER_3(EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION)    // ֧�ֿ�汾��, �㻻���»��� 6_1_101
#define EKRNLN_VERSIONA                   ___VER_4A(EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION)   // ֧�ֿ�汾��
#define EKRNLN_VERSIONA_                  ___VER_3A(EKRNLN_MAJORVERSION, EKRNLN_MINORVERSION, EKRNLN_BUILDVERSION)   // ֧�ֿ�汾��, �㻻���»��� 6_1_101

