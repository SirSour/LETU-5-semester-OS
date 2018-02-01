#include "stdafx.h"
#include <windows.h>
#include "conio.h"
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib") // для timeGetTime()
using namespace std;

HANDLE hFileOut;
HANDLE hFileIn;
TCHAR fileNameOut[_MAX_FNAME];
TCHAR fileNameIn[_MAX_FNAME];
OVERLAPPED OverlappedRead[64];
OVERLAPPED OverlappedWrite[64];
LARGE_INTEGER lpFileSize, endOfFile;
DWORD dwMilliseconds = INFINITE;
BOOL bAlertable = TRUE;
DWORD nNumberOfBytesToRead, nNumberOfBytesToWrite;

char answer;
int InOut = 0;
void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	InOut++;
};
/*dwErrorCode - код завершения
dwNumberOfBytesTransfered - передаваемое число байтов
lpOverlapped - буфер информации ввода/вывода
*/

int main()
{
	do {
		
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		system("title = Перекрывающиеся операции ввода/вывода.");


		char **lpBuffer;
		double countBuffer = 0;
		int count = 0;
		int offset = 0;
		int start, finish;
		int flag = 0;

		system("cls");
		cout << "			Выбор файлов: " << endl;
		cout << "Введите полный путь к копируемому файлу: " << endl;
		cin >> fileNameOut;
		hFileOut = CreateFile(fileNameOut, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
		/*
		режим доступа GENERIC_READ - Консоль для ввода данных
		совместный доступ - 0
		дескриптор безопасности - 0
		действие - открыть файл
		атрибуты - без системного кэширования + асинхронный ввод/вывод
		дескриптор шаблона - 0
		*/
		if (hFileOut == INVALID_HANDLE_VALUE) {
			cout << "Невозможно открыть файл для копирования." << endl;
			_getch();
		}
		if (!GetFileSizeEx(hFileOut, &lpFileSize)) {
			cout << "Невозможно определить размер файла для копирования." << endl;
			_getch();
		}

		cout << "Введите полный путь к новому файлу: " << endl;
		cin >> fileNameIn;
		hFileIn = CreateFile(fileNameIn, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
		/*
		режим доступа GENERIC_WRITE - Консоль для записи данных
		совместный доступ - 0
		дескриптор безопасности - 0
		действие - создать новый файл (переписать существующий)
		атрибуты - без системного кэширования + асинхронный ввод/вывод
		дескриптор шаблона - 0
		*/
		if (hFileIn == INVALID_HANDLE_VALUE) {
			cout << "Невозможно открыть новый файл." << endl;
			_getch();
		}
		do {
			cout << "Введите количество перекрывающихся операций ввода/вывода (1, 2, 4, 8, 12, 16): ";
			cin >> count;

			if (count != 1 && count != 2 && count != 4 && count != 8 && count != 12 && count != 16)
				cout << "1, 2, 4, 8, 12, 16." << endl;
		} while (count != 1 && count != 2 && count != 4 && count != 8 && count != 12 && count != 16);

		cout << "Введите размер читаемого блока в байтах (Кратен размеру кластера): ";
		cin >> nNumberOfBytesToRead;
		nNumberOfBytesToWrite = nNumberOfBytesToRead;
		countBuffer = ceil(double(lpFileSize.QuadPart) / double(nNumberOfBytesToRead));
		
		cout << "Количество блоков: " << countBuffer << endl;

		lpBuffer = new char *[64]; //буфер данных

		for (int i = 0; i < 64; i++) {
			lpBuffer[i] = new char[nNumberOfBytesToRead];
		}

		for (int i = 0; i < count; i++) {
			OverlappedRead[i].Offset = 0;
			OverlappedRead[i].OffsetHigh = 0;
			OverlappedRead[i].Pointer = 0;
			OverlappedRead[i].hEvent = 0;

			OverlappedWrite[i].Offset = 0;
			OverlappedWrite[i].OffsetHigh = 0;
			OverlappedWrite[i].Pointer = 0;
			OverlappedWrite[i].hEvent = 0;
		}

		start = timeGetTime();

		for (int i = 0; i < ceil((countBuffer / count)); i++) {
			for (int j = 0; j < count; j++) {
				OverlappedRead[j].Offset = offset + j * nNumberOfBytesToRead;

				if (!ReadFileEx(hFileOut, lpBuffer[j], nNumberOfBytesToRead, &OverlappedRead[j], &FileIOCompletionRoutine)) {
					cout << "Невозможно считать файл." << GetLastError() << endl;
					_getch();
					return 0;
				}
			}
			while (InOut < count) {
				SleepEx(dwMilliseconds, bAlertable);
			}
			InOut = 0;
			for (int k = 0; k < count; k++) {
				OverlappedWrite[k].Offset = offset + k * nNumberOfBytesToWrite;
				if (!WriteFileEx(hFileIn, lpBuffer[k], nNumberOfBytesToWrite, &OverlappedWrite[k], &FileIOCompletionRoutine)) {
					cout << "Невозможно записать файл." << GetLastError() << endl;
					_getch();
				}
			}
			while (InOut < count) {
				SleepEx(dwMilliseconds, bAlertable);
			}
			InOut = 0;

			offset = offset + (count * nNumberOfBytesToRead);
		}

		finish = timeGetTime() - start;

		cout << endl << "Время выполнения операции копирования в мс: " << finish << endl;

		endOfFile.QuadPart = lpFileSize.QuadPart;
		endOfFile.HighPart = lpFileSize.HighPart;
		SetFilePointerEx(hFileIn, endOfFile, 0, FILE_BEGIN);
		SetEndOfFile(hFileIn);

		CloseHandle(hFileOut);
		CloseHandle(hFileIn);

		do {
			cout << "Хотите повторить? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "Введите Y или N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
	return 0;
}