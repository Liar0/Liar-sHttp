#include "../pch.h"
#include "server.h"


server::server(const char *c, u_short port)
{
	memset(&serverIn, 0, sizeof(serverIn));
	serverIn.sin_family = AF_INET;
	serverIn.sin_port = htons(port);
	serverIn.sin_addr.S_un.S_addr = inet_addr(c);
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
server::server(const char *c, u_short port, int type) {
	memset(&serverIn, 0, sizeof(serverIn));
	serverIn.sin_family = AF_INET;
	serverIn.sin_port = htons(port);
	serverIn.sin_addr.S_un.S_addr = inet_addr(c);
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
};
void server::listen(int num) {
	errorInt =bind(s, (LPSOCKADDR)&serverIn, sizeof(serverIn));
	if (SOCKET_ERROR == errorInt) {
		cout << "listen:error code" << WSAGetLastError() << endl;
	}
	errorInt=::listen(s, num);
	if (SOCKET_ERROR == errorInt) {
		cout <<"listen:error code"<< WSAGetLastError() << endl;
	}
};
void server::waitClient() {
	nowClientAddrLen = sizeof(clientIn);
	nowClient = accept(s, (LPSOCKADDR)&clientIn, &nowClientAddrLen);
	int nowLen = recv(nowClient, buffer, MAX_CONNECT_CHAR_BUFFER, 0);
	nowCliengMsg = buffer;
	nowCliengMsg = nowCliengMsg.substr(0, nowLen);
	/*this->clientArr.push_back(nowClient);
	this->socketIndex.push_back(this->clientArr.size());*/
}
void server::waitClient(SOCKET & s2, string & ss) {
	nowClientAddrLen = sizeof(clientIn);
	s2 = accept(s, (LPSOCKADDR)&clientIn, &nowClientAddrLen);
	int nowLen = recv(s2, buffer, MAX_CONNECT_CHAR_BUFFER, 0);
	//cout << buffer << endl;
	ss = buffer;
	ss = ss.substr(0, nowLen);
	nowCliengMsg = ss;
	nowClient = s2;
};
BOOL server::createThread(int index) {
	HANDLE ev=WSACreateEvent();
	WSAEventSelect(this->clientArr[index], ev, FD_ACCEPT | FD_CLOSE);
	WSAWaitForMultipleEvents(1, &ev, TRUE, INFINITE, FALSE);
	return TRUE;
}
BOOL server::send(string s) {
	return this->send(s, nowClient);
}

BOOL server::allSend(string s) {

	for (unsigned int i = 0; i < clientArr.size(); i++) {
		this->send(s, clientArr[i]);
	}
	return TRUE;
}
BOOL server::send(string s, SOCKET s2) {
	errorInt = ::send(s2, s.c_str(), s.length() + 1, 0);
	if (errorInt == SOCKET_ERROR) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}
BOOL server::waitUDP(SOCKET  sock, char * bufer,const char * host,int port, int flags) {
	SOCKADDR_IN formIn;
	ZeroMemory(&formIn, sizeof(formIn));
	formIn.sin_family = AF_INET;
	formIn.sin_addr.S_un.S_addr = inet_addr(host);
	formIn.sin_port = htons(port);
	int bufInt = sizeof(formIn);
	
	errorInt=recvfrom(sock, buffer, MAX_CONNECT_CHAR_BUFFER, flags, (struct sockaddr*)&formIn, &bufInt);
	if (errorInt == SOCKET_ERROR) {
		cout<<"������!!"<<WSAGetLastError()<<endl;
		return FALSE;
	}
	else {
		string s = buffer;
		s = s.substr(0, errorInt);
		bufer = (char *)s.c_str();
		return TRUE;
	}
};
server::~server()
{
	
}
