#pragma once

class helpClass
{
public:
	helpClass();
	~helpClass();

	// ����ַ���
	static vector<string> split(string s,string s2);

	// ��ȡ����ͷ
	static oMap<string,string> getRequestHead(string request);

	// ��ȡĬ�ϵ���Ӧͷ
	static string getResponseHead(string request, int status, string statusMsg, oMap<string, string>headObj);

	// ��ȡϵͳʱ��
	static string getTime();

	// ��ȡ�����������ļ���ַ
	static string getFileUrl(string url,string methodType, oMap<string, string>getMap);

	// ��ȡminitype 
	static string getMINItype(string path);

	// ��ȡ�����ļ����ַ���
	static string helpClass::getFileCharset(string s);

	// ��ں��� һ��������������
	static void handleRequestLine(SOCKET & s, string request);

	// ����get���� �������Ǿʹ���һ��get post����Ͳ�˵��
	static void helpClass::methodGetHandle(SOCKET & s, string request, oMap<string, string>getMap);

	// ��ȡ�ļ��ķ��� ��������������ļ�ӳ���api��ô�᲻����Ӧ�ĸ�����
	static int readFile(string filePath, string & content);

	// �ַ�����ص�ת�뷽��
	static LPWSTR user_stringToLPWSTR(string orig);
	static string user_LPWSTRTostring(LPWSTR lpw);
	static LPCSTR user_LPWSTRToLPCSTR(LPWSTR lpw);
	static LPWSTR user_LPCSTRToLPWSTR(LPCSTR lpc);

};
namespace METHOD_TYPE {
	const string uHTTP_GET = "GET";
	const string uHTTP_POST = "POST";
	const string uHTTP_OPTIONS = "OPTIONS";
	const string uHTTP_DELETE = "DELETE";
	const string uHTTP_PUT = "PUT";
};

