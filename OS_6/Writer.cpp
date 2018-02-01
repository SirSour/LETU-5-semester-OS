#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;
const char WriterSemaphoreName[] = "Writer";
const char ReaderSemaphoreName[] = "Reader";
const int PAGE_COUNT = 18;
const int PAGE_SIZE = 4096;
SIZE_T dwSize = PAGE_COUNT*PAGE_SIZE + PAGE_COUNT * sizeof(char);

const char memoryName[] = "mapping";
const char* mutexName[] = { "mutex0", "mutex1", "mutex2", "mutex3", "mutex4", "mutex5","mutex6","mutex7",
"mutex8","mutex9","mutex10","mutex11", "mutex12","mutex13", "mutex14","mutex15","mutex16", "mutex17" };
fstream LogFile;

int main() {

	srand(time(NULL));

	LogFile.open("log.txt", fstream::out | fstream::app); //������� � ����� �����

	HANDLE hReaderSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, ReaderSemaphoreName);
	if (!hReaderSemaphore)
		LogFile << "���������� ������� �������. (��������)\n";

	HANDLE hWriterSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, WriterSemaphoreName);
	if (!hWriterSemaphore)
		LogFile << "���������� ������� �������. (��������)\n";

	HANDLE hMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, memoryName);
	if (!hMemory)
		LogFile << "���������� ������� ������������ ����. (��������)\n";

	char* memory = (char*)MapViewOfFile(hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!memory)
		LogFile << "���������� �������� ������ (��������)\n";
	VirtualLock((LPVOID)memory, dwSize);

	int pageNumber = 0;
	char data[PAGE_SIZE];

	for (int i = 0; i < 2; i++) {
		LogFile << GetTickCount() << " �������� ��������. (��������)\n";
		WaitForSingleObject(hReaderSemaphore, INFINITE); // ��������� � ��������� ������� �������� 
		LogFile << GetTickCount() << " �����. (��������)\n";
		Sleep(10);
		for (int j = 0; j < PAGE_COUNT; j++) {
			pageNumber = (GetCurrentThreadId() + j) % PAGE_COUNT;
			if (memory[pageNumber] == 0)
				break;
			if (j == PAGE_COUNT - 1) {
				j = 0;
				LogFile << GetTickCount() << " �� ������� ������ ��������. ��������. (��������) \n";
				Sleep(300);
			}
		}
		int offset = pageNumber * PAGE_SIZE + PAGE_COUNT * sizeof(char);

		sprintf_s(data, "��� �������� # %d. ����� ������ = %d", pageNumber, GetTickCount());

		HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, mutexName[pageNumber]);
		WaitForSingleObject(mutex, INFINITE);
		if (mutex) {
			Sleep(10);
			if (memory[pageNumber] == 1) {
				LogFile << GetTickCount() << " (��������) ��������. ��������: " << pageNumber << "\n";
				i++;
				ReleaseMutex(mutex);
				ReleaseSemaphore(hReaderSemaphore, 1, NULL);
			}
			else {
				memory[pageNumber] = 1;
				LogFile << GetTickCount() << " ������ ������. (��������)\n";
				LogFile << GetTickCount() << " ������ ���������. �������� " << pageNumber << " �������\n";
				ReleaseMutex(mutex);
				ReleaseSemaphore(hWriterSemaphore, 1, NULL);
			}
		}
		Sleep(200 + rand() % 500);
	}
	LogFile.close();
	return 0;
}
