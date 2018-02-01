
#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "iostream"
#include <bitset>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
void GetInfo();
void VirtualMemory();
void StatusOfPage(LPVOID adress);
void Reserved();
void ReservedAndTransit();
void SetData();
void SetProtect();
void Free();

MEMORYSTATUS memStatus;
SYSTEM_INFO systemInfo;
MEMORYSTATUSEX statEx;

DWORD dwSize = NULL;
LPVOID pBaseAdress;


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = Исследование виртуального адресного пространства процесса");
	int choice;

	do {
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "1. Получение информации о вычислительной системе" << endl;
		cout << "2. Определение статуса виртуальной памяти " << endl;
		cout << "3. Определение состояния конкретного участка памяти \n   по заданному с клавиатуры адресу " << endl;
		cout << "4. Резервирование региона в автоматическом режиме и \n   в режиме ввода адреса начала региона" << endl;
		cout << "5. Резервирование региона и передача ему физической памяти \n   в автоматическом режиме и в режиме ввода адреса начала региона " << endl;
		cout << "6. Запись данных в ячейки памяти по заданным с клавиатуры адресам " << endl;
		cout << "7. Установка защиты доступа для заданного (с клавиатуры) региона памяти \n   и ее проверка " << endl;
		cout << "8. Возврат физической памяти и освобождение региона \n   адресного пространства, заданного с клавиатуры " << endl;
		cout << "9. Выход из приложения" << endl;
		cout << "Выберите пункт меню: ";
		cin >> choice;
		if (choice < 0 || choice > 9) {
			cout << "Ошибка. Введите пункт меню заново.";
			_getch();
		}
		switch (choice) {
		case 1:
			system("cls");
			cout << "		Получение информации о вычислительной системе: " << endl;
			GetInfo();
			cout << endl;
			system("pause");
			break;
		case 2:
			system("cls");
			cout << "		Определение статуса виртуальной памяти: " << endl;
			VirtualMemory();
			cout << endl;
			system("pause");
			break;
		case 3:
			system("cls");
			cout << "		Определение состояния конкретного участка памяти \n			 по заданному с клавиатуры адресу: " << endl;
			LPVOID adress;
			GetSystemInfo(&systemInfo);
			cout << "Введите адрес в диапазоне от " << systemInfo.lpMinimumApplicationAddress << " до " << systemInfo.lpMaximumApplicationAddress << ": ";
			cin  >> adress;
			StatusOfPage(adress);
			cout << endl;
			system("pause");
			break;
		case 4:
			system("cls");
			cout << "		Резервирование региона в автоматическом режиме и \n		в режиме ввода адреса начала региона: " << endl;
			Reserved();
			cout << endl;
			system("pause");
			break;
		case 5:
			system("cls");
			cout << "	Резервирование региона и передача ему физической памяти \n	 в автоматическом режиме и в режиме ввода адреса начала региона: " << endl;
			ReservedAndTransit();
			cout << endl;
			system("pause");
			break;
		case 6:
			system("cls");
			cout << "	Запись данных в ячейки памяти по заданным с клавиатуры адресам: " << endl;
			SetData();
			cout << endl;
			system("pause");
			break;
		case 7:
			system("cls");
			cout << "	Установка защиты доступа для заданного (с клавиатуры) региона памяти \n   и ее проверка: " << endl;
			SetProtect();
			cout << endl;
			system("pause");
			break;
		case 8:
			system("cls");
			cout << "	Возврат физической памяти и освобождение региона \n		адресного пространства, заданного с клавиатуры: " << endl;
			Free();
			cout << endl;
			system("pause");
			break;
		case 9:
			break;
		default:
			rewind(stdin);
		}
	} while (choice != 9);

	return 0;
}

void GetInfo() {
	
	GetSystemInfo(&systemInfo);
	cout << "OEM ID: " << systemInfo.dwOemId << endl;
	cout << "Архитектура процессора: ";
	switch (systemInfo.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		cout << "x64 (AMD или Intel)" << endl;
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		cout << "ARM " << endl;
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		cout << "Intel Itanium-based " << endl;
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		cout << "x86 " << endl;
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		cout << "Неизвестная архитектура " << endl;
		break;
	default:
		break;
	}
	cout << "Размер страницы: " << systemInfo.dwPageSize << endl;
	cout << "Минимальный адрес для приложений: " << systemInfo.lpMinimumApplicationAddress << endl;
	cout << "Максимальный адрес для приложений: " << systemInfo.lpMaximumApplicationAddress << endl;

	cout << "Активные ядра: " << systemInfo.dwActiveProcessorMask << endl;
	cout << "Количество ядер: " << systemInfo.dwNumberOfProcessors << endl;
	cout << "Тип процессора: ";
	switch (systemInfo.dwProcessorType) {
	case PROCESSOR_INTEL_386:
		cout << "Intel 386 " << endl;
		break;
	case PROCESSOR_INTEL_486:
		cout << "Intel 486 " << endl;
		break;
	case PROCESSOR_INTEL_PENTIUM:
		cout << "Intel Pentium " << endl;
		break;
	case PROCESSOR_INTEL_IA64:
		cout << "Intel IA64 " << endl;
		break;
	case PROCESSOR_AMD_X8664:
		cout << "AMD x8664 " << endl;
		break;
	default:
		cout << "ARM " << endl;
		break;
	}
	cout << "Уровень процессора: " << systemInfo.wProcessorLevel << endl;
}

void VirtualMemory() {

	statEx.dwLength = sizeof(statEx);
	GlobalMemoryStatusEx(&statEx);
	cout << "Размер структуры MEMORYSTATUS: " << statEx.dwLength << " байт " << endl;
	cout << "Используется памяти: " << statEx.dwMemoryLoad << " % " << endl;
	cout << "Всего памяти: " << statEx.ullTotalPhys << " байт " << "/ " << statEx.ullTotalPhys / (1024 * 1024) << " Мб " << endl;
	cout << "Доступно памяти: " << statEx.ullAvailPhys << " байт " << "/ " << statEx.ullAvailPhys / (1024 * 1024) << " Мб " << endl;
	cout << "Страничный файл: " << statEx.ullTotalPageFile << " байт " << "/ " << statEx.ullTotalPageFile / (1024 * 1024) << " Мб " << endl;
	cout << "Страничный файл (доступно): " << statEx.ullAvailPageFile << " байт " << "/ " << statEx.ullAvailPageFile / (1024 * 1024) << " Мб " << endl;
	cout << "Виртуальная память (всего): " << statEx.ullTotalVirtual << " байт " << "/ " << statEx.ullTotalVirtual / (1024 * 1024) << " Мб " << endl;
	cout << "Виртуальная память (доступно): " << statEx.ullAvailVirtual << " байт " << "/ " << statEx.ullAvailVirtual / (1024 * 1024) << " Мб " << endl;
	cout << "Расширенная виртуальная память (доступно): " << statEx.ullAvailExtendedVirtual << " байт" << "/ " << statEx.ullAvailExtendedVirtual / (1024 * 1024) << " Мб " << endl;

}

void StatusOfPage(LPVOID adress) {

	MEMORY_BASIC_INFORMATION memoryInfo = {0};
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	VirtualQuery(adress, &memoryInfo, memorySize);
	cout << "Базовый адрес: " << memoryInfo.BaseAddress << endl;
	cout << "Базовый адрес выделенной памяти: " << memoryInfo.AllocationBase << endl;
	cout << "Атрибут защиты: " << memoryInfo.AllocationProtect << endl;
	cout << "Размер региона: " << memoryInfo.RegionSize << endl;

	cout << "Состояние страницы: ";
	switch (memoryInfo.State) {
	case MEM_COMMIT:
		cout << "  MEM_COMMIT" << endl;
		break;
	case MEM_FREE:
		cout << "  MEM_FREE" << endl;
		break;
	case MEM_RESERVE:
		cout << "  MEM_RESERVE" << endl;
		break;
	}

	cout << "Режим доступа: " << memoryInfo.Protect << endl;
	switch (memoryInfo.Protect) {
	case PAGE_READWRITE:
		cout << "  PAGE_READWRITE" << endl;
		break;
	case PAGE_READONLY:
		cout << "  PAGE_READONLY" << endl;
		break;
	case PAGE_EXECUTE:
		cout << "  PAGE_EXECUTE" << endl;
		break;
	case PAGE_EXECUTE_READ:
		cout << "  PAGE_EXECUTE_READ" << endl;
		break;
	case PAGE_EXECUTE_READWRITE:
		cout << "  PAGE_EXECUTE_READWRITE" << endl;
		break;
	case PAGE_NOACCESS:
		cout << "  PAGE_NOACCESS" << endl;
		break;
	case PAGE_GUARD:
		cout << "  PAGE_GUARD" << endl;
		break;
	case PAGE_NOCACHE:
		cout << "  PAGE_NOCACHE" << endl;
		break;
	case PAGE_EXECUTE_WRITECOPY:
		cout << "  PAGE_EXECUTE_WRITECOPY" << endl;
		break;
	case PAGE_WRITECOMBINE:
		cout << "  PAGE_WRITECOMBINE" << endl;
		break;
	}

	cout << "Тип страницы: " << memoryInfo.Type << endl;
	switch (memoryInfo.Type) {
	case MEM_IMAGE:
		cout << "  MEM_IMAGE" << endl;
		break;
	case MEM_MAPPED:
		cout << "  MEM_MAPPED" << endl;
		break;
	case MEM_PRIVATE:
		cout << "  MEM_PRIVATE" << endl;
		break;
	}
		
}

void Reserved() {

	int answer;
	LPVOID pMemory, adress;
	cout << "Введите размер (байт): ";
	cin >> dwSize;

	cout << "Если хотите ввести адрес (ручной режим), введите 1, иначе - 0: ";
	cin >> answer;
	if (answer) {
		adress = 0;
		cout << "Введите базовый адрес: ";
		cin >> pBaseAdress;
	}
	else
		adress = NULL;
	pMemory = VirtualAlloc(pBaseAdress, dwSize, MEM_RESERVE, PAGE_READWRITE);

	if (pMemory == NULL) {
		cout << "Ошибка. Уже зарезервировано."  << endl;
		return;
	}
	else {
		cout << "Зарезервировано " << endl;
		return;
	}
	
}

void ReservedAndTransit() {

	DWORD commit = MEM_COMMIT | MEM_RESERVE;
	LPVOID pMemory, adress;
	int answer;

	cout << "Введите размер (байт): ";
	cin >> dwSize;
	cout << "Если хотите ввести адрес (ручной режим), введите 1, иначе - 0: ";
	cin >> answer;
	if (answer) {
		adress = 0;
		cout << "Введите базовый адрес: ";
		cin >> pBaseAdress;
	}
	else
		adress = NULL;
//	LPVOID p = VirtualAlloc((LPVOID)pBaseAdress, dwSize, MEM_RESERVE, PAGE_READWRITE);
	pMemory = VirtualAlloc(pBaseAdress, dwSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (pMemory)
		ZeroMemory(pMemory, dwSize);
//	else
	//	cout << "Ошибка  " << GetLastError();

	if (pMemory == NULL) {
		cout << "Ошибка. Уже зарезервировано."  << endl;
		return;
		}
	else {
		cout << "Зарезервировано " << endl;
		return;
	}
}

void SetData() {

	MEMORY_BASIC_INFORMATION memoryInfo;
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	int data = 0;
	cout << "Введите базовый адрес: ";
	cin >> pBaseAdress;
	
	VirtualQuery(pBaseAdress, &memoryInfo, memorySize);
	if (memoryInfo.Protect & PAGE_NOACCESS) {
		cout << "Память защищена. Ввод данных невозможен." << endl;
		return;
	}

	cout << "Введите данные: ";
	cin >> data;

	if (pBaseAdress) {
		memcpy(pBaseAdress, &data, sizeof(int));
		cout << "Данные записаны по адресу: " << pBaseAdress << endl;
		cout << "Проверка: " << endl;
		cout << "Чтение по адресу: " <<  pBaseAdress << " Данные: " << (*(PDWORD)pBaseAdress) << endl;
	}
	
}

void SetProtect() {

	DWORD oldProtect;
	MEMORY_BASIC_INFORMATION memoryInfo;
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	cout << "Введите базовый адрес: ";
	cin >> pBaseAdress;
	cout << "Введите размер региона (байт): ";
	cin >> dwSize;
	bool f;
	if (pBaseAdress) {
		f = VirtualProtect(pBaseAdress, dwSize, 0x04, &oldProtect);
		if (!f)
			cout << GetLastError();
		cout << "Память защищена " << endl;
		cout << "Проверка: " << endl;
		StatusOfPage(pBaseAdress);
	}
	else {
		cout << "Неудачно. Код: " << GetLastError() << endl;
		system("pause");
	}

}

void Free() {

	GetSystemInfo(&systemInfo);
	cout << "Введите адрес в диапазоне от " << systemInfo.lpMinimumApplicationAddress << " до " << systemInfo.lpMaximumApplicationAddress << ": ";
	cin >> pBaseAdress;
	cout << "Введите размер региона (байт): ";
	cin >> dwSize;
	
	if (!VirtualFree(pBaseAdress, dwSize, MEM_DECOMMIT))
	{
		cout << "Ошибка: " << GetLastError() << endl;
		system("pause");
	}

	if (VirtualFree(pBaseAdress, 0, MEM_RELEASE)) {
		cout << "Память очищена." << endl;
		cout << "Проверка: " << endl;
		StatusOfPage(pBaseAdress);
	}
	else
		cout << "Ошибка. Код: " << GetLastError();
	
}