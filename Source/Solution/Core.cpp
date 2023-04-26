#include "stdafx.h"
#include "Core.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

const string webSrc = "./web_src"; // "." == Local Folder
const string mess404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><head><title> 404 Not Found </title></head><body><p> The requested file cannot be found. </p></body></html>";
const string mess401 = "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<!DOCTYPE html><h1>401 Unauthorized</h1><p>This is a private area.</p>";

string getResponseMess(string RequestStr)
{
	if (RequestStr.find("GET") == 0) return ProcessGET(RequestStr);
	if (RequestStr.find("POST") == 0) return ProcessPOST(RequestStr);

	return "";
}

string ProcessGET(string RequestStr)
{
	string fileName = RequestStr.substr(4, RequestStr.find(" ", 4) - 4);
	if (fileName == "/images.html" && RequestStr != "GET /images.html TRUE") return mess401;
	if (fileName != "/") fileName = webSrc + fileName; else fileName = webSrc + "/index.html";

	ifstream fi(fileName, ios::binary);
	if (!fi) return mess404;

	string data = "";

	fi.seekg(0, fi.end);
	data.resize(fi.tellg());
	fi.seekg(0, fi.beg);
	
	fi.read((char*)data.c_str(), data.length());
	fi.close();

	return GetHeader(fileName, data.length()) + data;
}

string ProcessPOST(string RequestStr)
{
	size_t beg = RequestStr.find("uname="), end = RequestStr.find("&", beg);
	string username = RequestStr.substr(beg + strlen("uname="), end - (beg + strlen("uname=")));
	beg = RequestStr.find("psw="), end = RequestStr.find("&", beg);
	string password = RequestStr.substr(beg + strlen("psw="), end - (beg + strlen("psw=")));

	if (username == "admin" && password == "123456") return ProcessGET("GET /images.html TRUE");
	return mess401;
}

string GetHeader(string fileName, size_t length)
{
	string contentType = "";

	if (fileName.find(".html") != string::npos) contentType = "text/html"; else
	if (fileName.find(".htm") != string::npos) contentType = "text/html"; else
	if (fileName.find(".txt") != string::npos) contentType = "text/plain"; else
	if (fileName.find(".jpg") != string::npos) contentType = "image/jpeg"; else
	if (fileName.find(".jpeg") != string::npos) contentType = "image/jpeg"; else
	if (fileName.find(".gif") != string::npos) contentType = "image/gif"; else
	if (fileName.find(".png") != string::npos) contentType = "image/png"; else 
	if (fileName.find(".css") != string::npos) contentType = "text/css"; else contentType = "application/octet-stream";

	return "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\nContent-Length: " + to_string(length) + "\r\n\r\n";
}