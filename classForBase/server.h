#pragma once
//����������ַ����� 10MB
#if !MAX_CONNECT_CHAR_BUFFER
#define MAX_CONNECT_CHAR_BUFFER 10485760  
#endif
class server
{
public:
	// c ����ipv4��ַ port ����˿�
	server(const char *c,u_short port); 
	server(const char *c, u_short port, int type);

	// ���� ������������num����
	void listen(int num); 

	// ��������
	~server();

	// �������׽���
	SOCKET s; 

	// ��ǰ�ķ�������������Ϣ 
	SOCKADDR_IN serverIn; 

	// �ȴ��ͻ�������
	void waitClient();
	void waitClient(SOCKET & s, string & ss);

	// ��ǰ�ͻ��˵��׽���
	SOCKET nowClient;

	// ��ǰ�ͻ��˵�������Ϣ
	SOCKADDR_IN clientIn; 

	// ���ӵĿͻ��˵�ַ��Ϣ���ڴ泤��
	int nowClientAddrLen; 

	// ��ǰ����Ļ�������
	char buffer[MAX_CONNECT_CHAR_BUFFER]; 

	// ���ͻ��˷����ַ���
	BOOL send(string s); 
	BOOL send(string s, SOCKET s2);

	BOOL allSend(string s);
	int errorInt; //����int
	BOOL waitUDP(SOCKET  s,char * bufer, const char * host, int port,int flags=0);
	vector<SOCKET> clientArr;
	vector<int> socketIndex;
	BOOL createThread(int index);
	string nowCliengMsg;
};

