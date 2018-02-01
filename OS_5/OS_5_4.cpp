#include "stdafx.h"
#include "Windows.h"
#include <iostream>
#include <omp.h>
#include <iomanip>

#pragma comment (lib, "Winmm.lib")
using namespace std;

const long double studNumber = 530717;
const long double  N = 100000000;
const long double sizeOfBlock = 10 * studNumber;

int main() {

	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	system("title = Процессы и потоки. OMP");

	unsigned int start;
	long double PI = 0;
	double countOperations = ceil((double)N / double(sizeOfBlock));
	int countOfThreads;
	double long x_i;
	int n = N - 1;
	cout << "Размер блока итераций = " << sizeOfBlock << endl;
	cout << "Количество блоков итераций = " << countOperations << endl;

	cout << "Введите количество создаваемых потоков: ";
	cin >> countOfThreads;
	
	omp_set_num_threads(countOfThreads);
	start = timeGetTime();
#pragma omp parallel num_threads(countOfThreads)
	{
#pragma omp  for schedule(dynamic, 5307170)  reduction(+ : PI)
		for (int i = 0; i < n; i++)
		{
			x_i = (i + 0.5) / N;
			PI += 4 / (1 + x_i*x_i);
		}
	}
	PI = PI / N;

	start = (timeGetTime() - start);
	cout << setprecision(10) << "Результат: Pi = " << PI << endl;
	cout << "Время: " << start << " мс." << endl;
	system("pause");

	return 0;
}
