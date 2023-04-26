// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "Core.h"
#include <iostream>
#include <string>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


UINT responseThread(LPVOID pParam)
{
	AfxSocketInit(NULL);

	CSocket Request;
	Request.Attach(*(SOCKET*)pParam);

	do
	{
		char buffer[1024] = "";
		bool isConnected = recv(Request, buffer, 1024, 0);
		if (!isConnected) break;

		string ResponseMess = getResponseMess(buffer);

		if (ResponseMess != "")
		{
			send(Request, (char*)ResponseMess.c_str(), (int)ResponseMess.length(), 0);
		}
	}  while (1);

	Request.Close();
	delete pParam;
	return 1;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.

			if (!AfxSocketInit())
			{
				cout << "Cannot Init Socket Library\n";
				return 0;
			}

			CSocket ServerSocket;
			
			if (!ServerSocket.Create(8080, SOCK_STREAM, 0)) // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
			{
				cout << "Fail to create server!\n";
				cout << ServerSocket.GetLastError();
				return 0;
			}
			else
			{
				cout << "Server is created successfully!\n";

				if (!ServerSocket.Listen(1))
				{
					cout << "Cannot listen on this port!\n";
					ServerSocket.Close();
					return 0;
				}
			}

			CSocket Request;

			do
			{
				ServerSocket.Listen();
				ServerSocket.Accept(Request);

				SOCKET* h = new SOCKET;
				*h = Request.Detach();
				AfxBeginThread(responseThread, h);
			} while (1);
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


