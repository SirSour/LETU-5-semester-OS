#include "stdafx.h"
#include <windows.h>
#include <iostream>

using namespace std;
HANDLE hPipe;
HANDLE hWriteEvent;
OVERLAPPED writeOverlapped;

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = ������");

	char message[255];

	cout << "		��� ���������� ������ ������� '����������'" << endl;
	hPipe = CreateNamedPipe("\\\\.\\pipe\\6Lab", PIPE_ACCESS_OUTBOUND | WRITE_DAC | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		cout << "�� ������� ������� �����. ������: " << GetLastError() << endl;
	else
		cout << "����� ������." << endl;

	hWriteEvent = CreateEvent(NULL, TRUE, FALSE, "pipe");
	if (hWriteEvent != INVALID_HANDLE_VALUE)
		cout << "������� �������." << endl;
	else
		cout << "�� ������� ������� �������. ������: " << GetLastError() << endl;

	cout << "			������� ����������: " << endl;
	if (ConnectNamedPipe(hPipe, NULL))
		cout << "���������� �����������." << endl;
	else
		cout << "������ ����������." << endl;

	while (true) { 

		cout << "������� ���������: ";
		cin >> message;
		ZeroMemory(&writeOverlapped, sizeof(writeOverlapped));
		writeOverlapped.hEvent = hWriteEvent;
		WriteFile(hPipe, message, 255, NULL, &writeOverlapped);
		if (!strcmp(message, "����������")) {
			break;
		}
		if (WaitForSingleObject(hWriteEvent, INFINITE) == WAIT_OBJECT_0)
			cout << "��������� ����������." << endl;
		else
			cout << "������. " << GetLastError() << endl;
	}

	cout << "			������������ �������: " << endl;
	if (DisconnectNamedPipe(hPipe))
		cout << "������������ ������� ���������." << endl;
	else
		cout << "������������ ������� �� �������." << endl;

	CloseHandle(hPipe);
	CloseHandle(hWriteEvent);

	system("pause");
	return 0;
}


