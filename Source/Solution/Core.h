#pragma once
#include <string>

using namespace std;

string getResponseMess(string RequestStr);
string ProcessGET(string RequestStr);
string ProcessPOST(string RequestStr);
string GetHeader(string fileName, size_t length);