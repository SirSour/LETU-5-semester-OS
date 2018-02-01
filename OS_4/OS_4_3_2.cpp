#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "windows.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS
HANDLE hMapFile;
PVOID lpMapAddress;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = ����������-��������");
	char answer;

	char data[4096];

	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "mapped");
	if (hMapFile != NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hMapFile = NULL;
		cout << "���� ��� ����������." << endl;
		system("pause");
		CloseHandle(hMapFile);
		return 0;
	}

	if (hMapFile == INVALID_HANDLE_VALUE)
		cout << "������. " << GetLastError() << endl;

	lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
	if (lpMapAddress == NULL) {
		cout << "�� ������ ������� �������� �����. " << endl;
		system("pause");	
		return 0;
	}

	memcpy(data, lpMapAddress, 4096);
	cout << "������ �� ������: " << lpMapAddress << " : " << endl;
	cout << data << endl;
	//Sleep(1000);
	system("pause");
	UnmapViewOfFile(lpMapAddress);
	CloseHandle(hMapFile);
	
	return 0;
}