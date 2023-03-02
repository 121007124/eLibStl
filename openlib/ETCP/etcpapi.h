/*********************************
			��װ��ETCP
***********************************/
#include<string>
#include<vector>
using namespace std;
class eClinet
{
public:
	//�ڲ�ʹ��
	vector<unsigned char> m_temp_data;
	SOCKET m_temp_server = NULL;
	DWORD m_dwWinFormID = 0;
	DWORD m_dwUnitID = 0;

	void* m_connect_cb = NULL;
	void* m_data_get_cb = NULL;
	void* m_backoff_cb = NULL;
	~eClinet();
	eClinet(void* clinet_connect_cb = NULL, void* data_get_cb = NULL, void* clinet_backoff_cb = NULL);
	bool connect(string ip, u_short  port, int  timeout = 10, bool  mod = false, int  type = 0, string pxoryip = "", u_short  proxy_port = 0, string name = "", string password = "");
	bool connect(wstring ip, u_short  port, int  timeout = 10, bool  mod = false, int  type = 0, wstring pxoryip = L"", u_short  proxy_port = 0, wstring name = L"", wstring password = L"");
	bool  send(vector<unsigned char> data, bool is_ret = false, vector<unsigned char>* ret_data = NULL, int delay_ret = 2);
	bool backoff();
	SOCKET get_socket();
	//����������##���ݵ���##������ʹ��;
	vector<unsigned char> get_data();

	//����������##�ص�����##��ʹ��;
	SOCKET get_server_socket();
private:
	void pointer_synchronization();
	void* m_clinet_num = NULL;
	int m_error_code = 0;
};
class eServer
{
public:
	SOCKET m_temp_clinet = NULL;
	vector<unsigned char> temp_data;
	DWORD m_dwWinFormID = 0;
	DWORD m_dwUnitID = 0;
	u_short m_port_num = 0;
	void* m_clinet_enter = NULL;
	void* m_data_get = NULL;
	void* m_clinet_leave = NULL;
	eServer(u_short uport, void* clinet_enter = NULL, void* data_get = NULL, void* clinet_leave = NULL);
	eServer();
	~eServer();
	bool start();
	//�����������ڻص����� ##���ݵ��� ��ʹ��;
	vector<unsigned char> get_data();
	bool is_create();
	void set_ip(string IP);
	void set_ip(wstring IP);
	char* get_ip();
	bool breakoff(SOCKET pC, bool is_now = false);
	string get_clinet_ip(SOCKET pC);
	wstring get_clinet_ip_w(SOCKET pC);
	bool  send(SOCKET pC, vector<unsigned char>  data, bool is_ret = false);
	//�����������ڻص����� ##�ͻ�����##���ݵ���##�ͻ��뿪 ��ʹ��;
	SOCKET get_clinet();
	//bool �Ͽ�(�ͻ��� Ҫ�Ͽ��Ŀͻ���, bool �����Ͽ� = false);
	bool close();
	SOCKET get_socket();
private:
	void pointer_synchronization();
	void* m_sever_num = NULL;
	int m_error_code = 0;
	string m_defalt_ip = "";
};