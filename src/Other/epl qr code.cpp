#define NOMINMAX 
#include"ElibHelp.h"
#include"qrencode\qrencode.h"




namespace {

	void QRcode2Bmp(const QRcode* qrCode, elibstl::CFBitMap& bmp, INT backgroundColor, INT pixelColor, INT nZoom)
	{

		if (!qrCode) return;
		nZoom = std::max(1, nZoom);
		const INT qrWidth = (qrCode->width > 0 ? qrCode->width : 1);
		bmp.CreateBitmap(qrWidth * nZoom, qrWidth * nZoom, 24);
		bmp.FillBG(backgroundColor);    //��ɫ��ͼ
		for (int y = 0; y < qrWidth; y++)
		{
			for (int x = 0; x < qrWidth; x++)
			{
				unsigned char b = qrCode->data[y * qrWidth + x];
				if (b & 0x01)    //��ɫ,��֮����ɫ
				{
					for (int x2 = 0; x2 < nZoom; x2++)
						for (int y2 = 0; y2 < nZoom; y2++)
							bmp.SetPixel(x * nZoom + x2, y * nZoom + y2, pixelColor);
				}
			}
		}
	}


}




static ARG_INFO Args[] =
{
		{
			"����",
			"��ά�����������ı�����.���Զ������ַ�ʶ��Ϊ�ֽڼ�",
			0,
			0,
			_SDT_ALL,
			0,
			ArgMark::AS_NONE,
		},
		{
				"�ȼ�",
				"�ṩ����ʹ�õ����ɵȼ�,1-40֮��,ֵԽ�����ص�Խ��,�ܱ�ʾ������Խ��",
				0,
				0,
				SDT_INT,
				10,
				ArgMark::AS_HAS_DEFAULT_VALUE ,
		},{
			"�ݴ�",
			"�ṩ�����ɶ�ά����ݴ�ȼ�,0-3֮��.",
			0,
			0,
			SDT_INT,
			3,
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"��ɫ",
			"�ṩ�����ɶ�ά��ͼ��ĵ�ͼ��ɫ,Ĭ��Ϊ��ɫ",
			0,
			0,
			SDT_INT,
			RGB(255,255,255),
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"����ɫ",
			"�ṩ�����ɶ�ά��ͼ��Ķ�ά�����ص���ɫ.",
			0,
			0,
			SDT_INT,
			RGB(0,0,0),
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"�Ŵ���",
			"�ṩ�����ɶ�ά��ͼ��ķŴ���",
			0,
			0,
			SDT_INT,
			1,
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}
};

static ARG_INFO Args2[] =
{
		{
			"��������",
			"��ά��������������.",
			0,
			0,
			SDT_BIN,
			0,
			ArgMark::AS_NONE,
		},
		{
				"�ȼ�",
				"�ṩ����ʹ�õ����ɵȼ�,1-40֮��,ֵԽ�����ص�Խ��,�ܱ�ʾ������Խ��",
				0,
				0,
				SDT_INT,
				10,
				ArgMark::AS_HAS_DEFAULT_VALUE ,
		},{
			"�ݴ�",
			"�ṩ�����ɶ�ά����ݴ�ȼ�,0-3֮��.",
			0,
			0,
			SDT_INT,
			3,
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"��ɫ",
			"�ṩ�����ɶ�ά��ͼ��ĵ�ͼ��ɫ,Ĭ��Ϊ��ɫ",
			0,
			0,
			SDT_INT,
			RGB(255,255,255),
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"����ɫ",
			"�ṩ�����ɶ�ά��ͼ��Ķ�ά�����ص���ɫ.",
			0,
			0,
			SDT_INT,
			RGB(0,0,0),
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}, {
			"�Ŵ���",
			"�ṩ�����ɶ�ά��ͼ��ķŴ���",
			0,
			0,
			SDT_INT,
			1,
			ArgMark::AS_HAS_DEFAULT_VALUE,
		}
};


EXTERN_C void fn_QRcode_encodeString(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
	elibstl::CFBitMap bmp;
	
	auto inData = elibstl::arg_to_wstring(pArgInf, 0);
	auto u8str= elibstl::W2UTF8(inData);
	QRcode* qrCode = QRcode_encodeString(u8str, pArgInf[1].m_int, static_cast<QRecLevel> (pArgInf[2].m_int), QR_MODE_8, 1);
	QRcode2Bmp(qrCode, bmp, pArgInf[3].m_int, pArgInf[4].m_int, pArgInf[5].m_int);
	QRcode_free(qrCode);
	pRetData->m_pBin = elibstl::clone_bin(bmp.GetBitmap<unsigned char>(), bmp.GetBitmapSize());
	delete[]u8str;

}

EXTERN_C void fn_QRcode_encodeData(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf) {
	elibstl::CFBitMap bmp;

	auto inData = elibstl::args_to_ebin(pArgInf, 0);
	QRcode* qrCode = QRcode_encodeData(inData->m_size, inData->m_data, pArgInf[1].m_int, static_cast<QRecLevel> (pArgInf[2].m_int));
	QRcode2Bmp(qrCode, bmp, pArgInf[3].m_int, pArgInf[4].m_int, pArgInf[5].m_int);
	QRcode_free(qrCode);
	pRetData->m_pBin = elibstl::clone_bin(bmp.GetBitmap<unsigned char>(), bmp.GetBitmapSize());

}




FucInfo Fn_QRcode_encodeString = { {
		/*ccname*/  ("�����ı���ά��"),
		/*egname*/  ("QRcode_encodeString"),
		/*explain*/ ("��ָ���ı��������ɶ�ά��ͼ��."),
		/*category*/12,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
	ESTLARG(Args)
	} ,ESTLFNAME(fn_QRcode_encodeString)};

FucInfo Fn_QRcode_encodeData = { {
		/*ccname*/  ("�������ݶ�ά��"),
		/*egname*/  ("QRcode_encodeData"),
		/*explain*/ ("��ָ�������������ɶ�ά��ͼ��.."),
		/*category*/12,
		/*state*/   NULL,
		/*ret*/     SDT_BIN,
		/*reserved*/NULL,
		/*level*/   LVL_HIGH,
		/*bmp inx*/ 0,
		/*bmp num*/ 0,
	ESTLARG(Args2)
	} ,ESTLFNAME(fn_QRcode_encodeData) };
