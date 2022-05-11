// httpClass.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "classForBase/server.h"
#include "classForBase/helpClass.h"
#define PORT (u_short)8002
void hideWindow() {
	HWND w=GetConsoleWindow();
	ShowWindow(w, SW_HIDE);
}
int main()
{	
//	hideWindow();
	WSADATA data;
	
	WSAStartup(MAKEWORD(2, 2), &data);
	server * nowServer = new server("192.168.31.64", PORT);
	nowServer->listen(10);
	cout << "访问地址http://127.0.0.1:8002就可以调试本程序了:" << endl;
	while (1) {
		SOCKET s;
		string ss;
		nowServer->waitClient(s,ss);
		//cout << nowServer->nowCliengMsg.c_str() << endl;
		helpClass::handleRequestLine(s, ss);
	}

	WSACleanup();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
