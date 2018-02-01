#include "stdafx.h"
#include <iostream>
#include <windows.h>

using namespace std;
char message[255];
bool out = false;
HANDLE hOutPipe;

VOID CALLBACK FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED) {
	if (!strcmp(message, "достаточно"))
		out = true;
	else
		cout << "Проверка: " << message << endl;
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = Клиент");

	char answer;
	int current = 1, max = 5;
	while (current < max) {

		cout << "Попытка подключения: " << current << endl;
		hOutPipe = CreateFile("\\\\.\\pipe\\6Lab", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
		if (hOutPipe == INVALID_HANDLE_VALUE) {
			cout << "Не удалось подключиться. Попытка переподключения через 2 секунды." << endl;
		}
		else {
			cout << "Соединение установлено." << endl;
			break;
		}
		++current;
		Sleep(2000);
	}
	if (current == max) {
		cout << "Максимальное количество попыток, подключение не удалось. Программа будет закрыта." << endl;
		system("pause");
		return -1;
	}

	while (true) {
		OVERLAPPED readOverlapped;
		ZeroMemory(&readOverlapped, sizeof(readOverlapped));

		cout << "Ожидание сообщения." << endl;

		if (!ReadFileEx(hOutPipe, message, 255, &readOverlapped, FileIOCompletionRoutine)) {
			cout << "Ошибка чтения. " << GetLastError() << endl;
			return 1;
		}
		SleepEx(INFINITE, 1);
		if (out)
			break;
	}
	CloseHandle(hOutPipe);
	cout << "Соединение закрыто." << endl;;

	system("pause");
	return 0;
}