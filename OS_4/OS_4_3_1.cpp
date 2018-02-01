
#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "windows.h"
#include "string.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

HANDLE hFile;
HANDLE hMapFile;
LPVOID lpMapAddress;
char fileName[250] = "writer.txt";

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = ����������-��������");
	char answer;
	char data[4096];

	hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		cout << "������. " << GetLastError() << endl;
	hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(data), "mapped");
	if (hMapFile != NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hMapFile = NULL;
		cout << "���� ��� ����������." << endl;
		system("pause");
		CloseHandle(hMapFile);
		return 0;
	}

	if (hMapFile != INVALID_HANDLE_VALUE)
		cout << "������������ ���� ������." << endl;

	lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (lpMapAddress == NULL) {
		cout << " ������ ����������� �����." << GetLastError() << endl;
		system("pause");
		return 0;
	}
	cout << "����� ��������: " << lpMapAddress << endl;
	
	cout << "������ ��� ��������: ";
	cin >> data;
	memcpy(lpMapAddress, data, strlen(data));

	//Sleep(3000);
	system("pause");
	UnmapViewOfFile(lpMapAddress);

	CloseHandle(hMapFile);
	CloseHandle(hFile);
	
	return 0;
}
