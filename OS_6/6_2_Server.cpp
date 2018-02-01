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
	system("title = Сервер");

	char message[255];

	cout << "		Для завершения работы введите 'достаточно'" << endl;
	hPipe = CreateNamedPipe("\\\\.\\pipe\\6Lab", PIPE_ACCESS_OUTBOUND | WRITE_DAC | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		cout << "Не удалось создать канал. Ошибка: " << GetLastError() << endl;
	else
		cout << "Канал создан." << endl;

	hWriteEvent = CreateEvent(NULL, TRUE, FALSE, "pipe");
	if (hWriteEvent != INVALID_HANDLE_VALUE)
		cout << "Событие создано." << endl;
	else
		cout << "Не удалось создать событие. Ошибка: " << GetLastError() << endl;

	cout << "			Попытка соединения: " << endl;
	if (ConnectNamedPipe(hPipe, NULL))
		cout << "Соединение установлено." << endl;
	else
		cout << "Ошибка соединения." << endl;

	while (true) { 

		cout << "Введите сообщение: ";
		cin >> message;
		ZeroMemory(&writeOverlapped, sizeof(writeOverlapped));
		writeOverlapped.hEvent = hWriteEvent;
		WriteFile(hPipe, message, 255, NULL, &writeOverlapped);
		if (!strcmp(message, "достаточно")) {
			break;
		}
		if (WaitForSingleObject(hWriteEvent, INFINITE) == WAIT_OBJECT_0)
			cout << "Сообщение отправлено." << endl;
		else
			cout << "Ошибка. " << GetLastError() << endl;
	}

	cout << "			Отсоединение сервера: " << endl;
	if (DisconnectNamedPipe(hPipe))
		cout << "Отсоединение сервера завершено." << endl;
	else
		cout << "Отсоединение сервера не удалось." << endl;

	CloseHandle(hPipe);
	CloseHandle(hWriteEvent);

	system("pause");
	return 0;
}


