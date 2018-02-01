#include "stdafx.h"
#include <windows.h>
#include "conio.h"
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib") // ��� timeGetTime()
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
/*dwErrorCode - ��� ����������
dwNumberOfBytesTransfered - ������������ ����� ������
lpOverlapped - ����� ���������� �����/������
*/

int main()
{
	do {
		
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		system("title = ��������������� �������� �����/������.");


		char **lpBuffer;
		double countBuffer = 0;
		int count = 0;
		int offset = 0;
		int start, finish;
		int flag = 0;

		system("cls");
		cout << "			����� ������: " << endl;
		cout << "������� ������ ���� � ����������� �����: " << endl;
		cin >> fileNameOut;
		hFileOut = CreateFile(fileNameOut, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
		/*
		����� ������� GENERIC_READ - ������� ��� ����� ������
		���������� ������ - 0
		���������� ������������ - 0
		�������� - ������� ����
		�������� - ��� ���������� ����������� + ����������� ����/�����
		���������� ������� - 0
		*/
		if (hFileOut == INVALID_HANDLE_VALUE) {
			cout << "���������� ������� ���� ��� �����������." << endl;
			_getch();
		}
		if (!GetFileSizeEx(hFileOut, &lpFileSize)) {
			cout << "���������� ���������� ������ ����� ��� �����������." << endl;
			_getch();
		}

		cout << "������� ������ ���� � ������ �����: " << endl;
		cin >> fileNameIn;
		hFileIn = CreateFile(fileNameIn, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
		/*
		����� ������� GENERIC_WRITE - ������� ��� ������ ������
		���������� ������ - 0
		���������� ������������ - 0
		�������� - ������� ����� ���� (���������� ������������)
		�������� - ��� ���������� ����������� + ����������� ����/�����
		���������� ������� - 0
		*/
		if (hFileIn == INVALID_HANDLE_VALUE) {
			cout << "���������� ������� ����� ����." << endl;
			_getch();
		}
		do {
			cout << "������� ���������� ��������������� �������� �����/������ (1, 2, 4, 8, 12, 16): ";
			cin >> count;

			if (count != 1 && count != 2 && count != 4 && count != 8 && count != 12 && count != 16)
				cout << "1, 2, 4, 8, 12, 16." << endl;
		} while (count != 1 && count != 2 && count != 4 && count != 8 && count != 12 && count != 16);

		cout << "������� ������ ��������� ����� � ������ (������ ������� ��������): ";
		cin >> nNumberOfBytesToRead;
		nNumberOfBytesToWrite = nNumberOfBytesToRead;
		countBuffer = ceil(double(lpFileSize.QuadPart) / double(nNumberOfBytesToRead));
		
		cout << "���������� ������: " << countBuffer << endl;

		lpBuffer = new char *[64]; //����� ������

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
					cout << "���������� ������� ����." << GetLastError() << endl;
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
					cout << "���������� �������� ����." << GetLastError() << endl;
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

		cout << endl << "����� ���������� �������� ����������� � ��: " << finish << endl;

		endOfFile.QuadPart = lpFileSize.QuadPart;
		endOfFile.HighPart = lpFileSize.HighPart;
		SetFilePointerEx(hFileIn, endOfFile, 0, FILE_BEGIN);
		SetEndOfFile(hFileIn);

		CloseHandle(hFileOut);
		CloseHandle(hFileIn);

		do {
			cout << "������ ���������? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "������� Y ��� N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
	return 0;
}