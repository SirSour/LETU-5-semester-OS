#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <iomanip>

#pragma comment (lib, "Winmm.lib") // ��� �����
using namespace std;

const long double studNumber = 530717;
const long double  N = 100000000;
const long double sizeOfBlock = 10 * studNumber;

DWORD WINAPI Formula(LPVOID);


_declspec(thread) DWORD indexOfThread; //��� ������������ TLS

typedef struct _THREAD {
	HANDLE hThreads;
	long double PI = 0;
	volatile bool counting;
	volatile UINT BlockIndex = 0;

}THREAD; 

THREAD *thread;
double countOperations = ceil(N / sizeOfBlock); //���������� ��������;
long nextBlock = 0;

int main() {

	
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	system("title = �������� � ������");
	srand(time(NULL));
	int index;
	double pi = 0;
	int countOfThreads; //���-�� �������
	unsigned int start = 0;

	cout << "������ ����� �������� = " << sizeOfBlock << endl;
	cout << "���������� ������ �������� = " << countOperations << endl;
	cout << "������� ���������� ����������� �������: ";
	cin >> countOfThreads;

	start = timeGetTime();
	HANDLE* arrayOfThreads;
	thread = new THREAD[countOfThreads];
	arrayOfThreads = new HANDLE[countOfThreads];

	nextBlock = countOfThreads;

	for (index = 0; index < countOfThreads; index++) {
		
		thread[index].hThreads = CreateThread(NULL, 0, &Formula, (LPVOID)index, CREATE_SUSPENDED, NULL);
		thread[index].PI = 0;
		thread[index].counting = false;
		thread[index].BlockIndex = index;
	
		arrayOfThreads[index] = thread[index].hThreads;
	}
	
	for (index = 0; index < countOfThreads; index++)
		ResumeThread(thread[index].hThreads);

	while (nextBlock <= countOperations) {
		int i;
		for (i = rand() % countOfThreads; ;i  = (i + 1) % countOfThreads) {
			SwitchToThread();
			if (!thread[i].counting || nextBlock > countOperations) 
				break;
		}
		ResumeThread(thread[i].hThreads);
	}

	for (int i = 0; i < countOfThreads; i++)
		ResumeThread(thread[i].hThreads);
	
	WaitForMultipleObjects(countOfThreads, arrayOfThreads, true, INFINITE);
	
	for (index = 0; index < countOfThreads; index++)
		pi += thread[index].PI;

	pi = pi / N;
	start = (timeGetTime() - start);
	cout << endl << setprecision(10) << "���������: Pi = " << pi << endl;
	cout << "����� ����������: " << start << " ��." << endl;
	system("pause");

	for (index = 0; index < countOfThreads; index++)
		CloseHandle(thread[index].hThreads);

	return 0;
}

DWORD WINAPI Formula(LPVOID start) {

	indexOfThread = (DWORD)start;
	thread[indexOfThread].counting = true;
	double transitionalPI = 0, x_i;
	long int begin = 0, end = 0, i;
	
	do {
		begin = thread[indexOfThread].BlockIndex*sizeOfBlock;
		end = begin + sizeOfBlock;
		if (end > N)
			end = N;

		for (i = begin; i < end; i++) {
			x_i = (i + 0.5) / N;
			transitionalPI = transitionalPI + 4 / (1 + x_i*x_i);
		}

	//	cout << nextBlock << " " << thread[indexOfThread].BlockIndex << " " << indexOfThread << endl;
		
		thread[indexOfThread].counting = false;
		if(nextBlock + 1 <= countOperations)
			SuspendThread(thread[indexOfThread].hThreads); //�������������
		thread[indexOfThread].BlockIndex = InterlockedExchangeAdd(&nextBlock, 1); 

		thread[indexOfThread].counting = true;
	} while (nextBlock <= countOperations);

//	cout << indexOfThread << endl;
	thread[indexOfThread].PI += transitionalPI;
	return 0; 

}
