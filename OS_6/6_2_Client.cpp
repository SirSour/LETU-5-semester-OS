#include "stdafx.h"
#include <iostream>
#include <windows.h>

using namespace std;
char message[255];
bool out = false;
HANDLE hOutPipe;

VOID CALLBACK FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED) {
	if (!strcmp(message, "����������"))
		out = true;
	else
		cout << "��������: " << message << endl;
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = ������");

	char answer;
	int current = 1, max = 5;
	while (current < max) {

		cout << "������� �����������: " << current << endl;
		hOutPipe = CreateFile("\\\\.\\pipe\\6Lab", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
		if (hOutPipe == INVALID_HANDLE_VALUE) {
			cout << "�� ������� ������������. ������� ��������������� ����� 2 �������." << endl;
		}
		else {
			cout << "���������� �����������." << endl;
			break;
		}
		++current;
		Sleep(2000);
	}
	if (current == max) {
		cout << "������������ ���������� �������, ����������� �� �������. ��������� ����� �������." << endl;
		system("pause");
		return -1;
	}

	while (true) {
		OVERLAPPED readOverlapped;
		ZeroMemory(&readOverlapped, sizeof(readOverlapped));

		cout << "�������� ���������." << endl;

		if (!ReadFileEx(hOutPipe, message, 255, &readOverlapped, FileIOCompletionRoutine)) {
			cout << "������ ������. " << GetLastError() << endl;
			return 1;
		}
		SleepEx(INFINITE, 1);
		if (out)
			break;
	}
	CloseHandle(hOutPipe);
	cout << "���������� �������." << endl;;

	system("pause");
	return 0;
}