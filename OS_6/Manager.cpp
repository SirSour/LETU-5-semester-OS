#include "stdafx.h"
#include <windows.h>
#include <iostream>

using namespace std;
LPCSTR writerProgramPath = "C:\\Users\\ibm\\Documents\\Visual Studio 2015\\Projects\\Writer\\Debug\\Writer.exe";
LPCSTR readerProgramPath = "C:\\Users\\ibm\\Documents\\Visual Studio 2015\\Projects\\Reader\\Debug\\Reader.exe";
const int READER_COUNT = 19;
const int WRITER_COUNT = 19;

const int SEMAPHORE_MAX_VALUE = 36;
const int PAGE_COUNT = 18;
const int PAGE_SIZE = 4096;
SIZE_T dwSize = PAGE_COUNT*PAGE_SIZE + PAGE_COUNT * sizeof(char);

const char memoryName[] = "mapping";

const char WriterSemaphoreName[] = "Writer";
const char ReaderSemaphoreName[] = "Reader";

PROCESS_INFORMATION piWriterProcessInfo[WRITER_COUNT];
PROCESS_INFORMATION piReaderProcessInfo[READER_COUNT];

HANDLE phMemoryPageMutex[PAGE_COUNT];

const char* mutexName[] = { "mutex0", "mutex1", "mutex2", "mutex3", "mutex4", "mutex5","mutex6","mutex7",
"mutex8","mutex9", "mutex10", "mutex11", "mutex12", "mutex13", "mutex14", "mutex15","mutex16","mutex17" };

int main() {

	SetConsoleCP(1251);	
	SetConsoleOutputCP(1251);
	system("title = Менеджер");
	HANDLE hWriterSemaphore = CreateSemaphore(NULL, SEMAPHORE_MAX_VALUE, SEMAPHORE_MAX_VALUE, WriterSemaphoreName);
	if (!hWriterSemaphore)
		cout << "Невозможно создать семафор (писатель). " << endl;

	HANDLE hReaderSemaphore = CreateSemaphore(NULL, SEMAPHORE_MAX_VALUE, SEMAPHORE_MAX_VALUE, ReaderSemaphoreName);
	if (!hReaderSemaphore)
		cout << "Невозможно создать семафор (читатель). " << endl;

	for (int i = 0; i < PAGE_COUNT; i++) {
		phMemoryPageMutex[i] = CreateMutex(NULL, false, mutexName[i]);
		if (!phMemoryPageMutex[i])
			cout << "Невозможно создать  " << i << " мьютекс" << endl;
	}

	HANDLE hFile = CreateFile("file.txt",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile)
		cout << "Файл создан." << endl;

	HANDLE hMemory = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0,
		PAGE_COUNT*PAGE_SIZE + PAGE_COUNT * sizeof(char), memoryName);
	if (!hMemory)
		cout << "Невозможно создать проецируемый файл. Ошибка: " << GetLastError() << endl;

	char* memory = (char*)MapViewOfFile(hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!memory)
		cout << "Ошибка. " << endl;
	ZeroMemory(memory, dwSize);

	for (int i = 0; i < WRITER_COUNT; i++) {
		STARTUPINFOA startinfo = { sizeof(startinfo) };
		if (CreateProcess(writerProgramPath, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startinfo, &(piWriterProcessInfo[i])) == NULL)
			cout << "Невозможно создать писателя номер " << i << " Ошибка: " << GetLastError() << endl;
		else
			cout << "Писатель номер " << i << " создан" << endl; 
		Sleep(100);
	}
	for (int i = 0; i < READER_COUNT; i++) {
		STARTUPINFOA startinfo = { sizeof(startinfo) };
		if (CreateProcess(readerProgramPath, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startinfo, &(piWriterProcessInfo[i])) == NULL)
			cout << "Невозможно создать читателя номер " << i << " Ошибка: " << GetLastError() << endl;
		else
			cout << "Читатель номер " << i << " создан" << endl;
		Sleep(100);
	}
//	system("pause");
	for (int i = 0; i < WRITER_COUNT; i++) {
		cout << "Ожидание писателя номер " << i << endl;
		WaitForSingleObject(piWriterProcessInfo[i].hProcess, INFINITY);
	}
	for (int i = 0; i < READER_COUNT; i++) {
		cout << "Ожидание читателя номер " << i << endl;
		WaitForSingleObject(piReaderProcessInfo[i].hProcess, INFINITY);
	}
	
	cout << "Завершено. " << endl;
	system("pause");
	return 0;
}


