
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
	system("title = ������������ ������������ ��������� ������������ ��������");
	int choice;

	do {
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "1. ��������� ���������� � �������������� �������" << endl;
		cout << "2. ����������� ������� ����������� ������ " << endl;
		cout << "3. ����������� ��������� ����������� ������� ������ \n   �� ��������� � ���������� ������ " << endl;
		cout << "4. �������������� ������� � �������������� ������ � \n   � ������ ����� ������ ������ �������" << endl;
		cout << "5. �������������� ������� � �������� ��� ���������� ������ \n   � �������������� ������ � � ������ ����� ������ ������ ������� " << endl;
		cout << "6. ������ ������ � ������ ������ �� �������� � ���������� ������� " << endl;
		cout << "7. ��������� ������ ������� ��� ��������� (� ����������) ������� ������ \n   � �� �������� " << endl;
		cout << "8. ������� ���������� ������ � ������������ ������� \n   ��������� ������������, ��������� � ���������� " << endl;
		cout << "9. ����� �� ����������" << endl;
		cout << "�������� ����� ����: ";
		cin >> choice;
		if (choice < 0 || choice > 9) {
			cout << "������. ������� ����� ���� ������.";
			_getch();
		}
		switch (choice) {
		case 1:
			system("cls");
			cout << "		��������� ���������� � �������������� �������: " << endl;
			GetInfo();
			cout << endl;
			system("pause");
			break;
		case 2:
			system("cls");
			cout << "		����������� ������� ����������� ������: " << endl;
			VirtualMemory();
			cout << endl;
			system("pause");
			break;
		case 3:
			system("cls");
			cout << "		����������� ��������� ����������� ������� ������ \n			 �� ��������� � ���������� ������: " << endl;
			LPVOID adress;
			GetSystemInfo(&systemInfo);
			cout << "������� ����� � ��������� �� " << systemInfo.lpMinimumApplicationAddress << " �� " << systemInfo.lpMaximumApplicationAddress << ": ";
			cin  >> adress;
			StatusOfPage(adress);
			cout << endl;
			system("pause");
			break;
		case 4:
			system("cls");
			cout << "		�������������� ������� � �������������� ������ � \n		� ������ ����� ������ ������ �������: " << endl;
			Reserved();
			cout << endl;
			system("pause");
			break;
		case 5:
			system("cls");
			cout << "	�������������� ������� � �������� ��� ���������� ������ \n	 � �������������� ������ � � ������ ����� ������ ������ �������: " << endl;
			ReservedAndTransit();
			cout << endl;
			system("pause");
			break;
		case 6:
			system("cls");
			cout << "	������ ������ � ������ ������ �� �������� � ���������� �������: " << endl;
			SetData();
			cout << endl;
			system("pause");
			break;
		case 7:
			system("cls");
			cout << "	��������� ������ ������� ��� ��������� (� ����������) ������� ������ \n   � �� ��������: " << endl;
			SetProtect();
			cout << endl;
			system("pause");
			break;
		case 8:
			system("cls");
			cout << "	������� ���������� ������ � ������������ ������� \n		��������� ������������, ��������� � ����������: " << endl;
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
	cout << "����������� ����������: ";
	switch (systemInfo.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		cout << "x64 (AMD ��� Intel)" << endl;
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
		cout << "����������� ����������� " << endl;
		break;
	default:
		break;
	}
	cout << "������ ��������: " << systemInfo.dwPageSize << endl;
	cout << "����������� ����� ��� ����������: " << systemInfo.lpMinimumApplicationAddress << endl;
	cout << "������������ ����� ��� ����������: " << systemInfo.lpMaximumApplicationAddress << endl;

	cout << "�������� ����: " << systemInfo.dwActiveProcessorMask << endl;
	cout << "���������� ����: " << systemInfo.dwNumberOfProcessors << endl;
	cout << "��� ����������: ";
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
	cout << "������� ����������: " << systemInfo.wProcessorLevel << endl;
}

void VirtualMemory() {

	statEx.dwLength = sizeof(statEx);
	GlobalMemoryStatusEx(&statEx);
	cout << "������ ��������� MEMORYSTATUS: " << statEx.dwLength << " ���� " << endl;
	cout << "������������ ������: " << statEx.dwMemoryLoad << " % " << endl;
	cout << "����� ������: " << statEx.ullTotalPhys << " ���� " << "/ " << statEx.ullTotalPhys / (1024 * 1024) << " �� " << endl;
	cout << "�������� ������: " << statEx.ullAvailPhys << " ���� " << "/ " << statEx.ullAvailPhys / (1024 * 1024) << " �� " << endl;
	cout << "���������� ����: " << statEx.ullTotalPageFile << " ���� " << "/ " << statEx.ullTotalPageFile / (1024 * 1024) << " �� " << endl;
	cout << "���������� ���� (��������): " << statEx.ullAvailPageFile << " ���� " << "/ " << statEx.ullAvailPageFile / (1024 * 1024) << " �� " << endl;
	cout << "����������� ������ (�����): " << statEx.ullTotalVirtual << " ���� " << "/ " << statEx.ullTotalVirtual / (1024 * 1024) << " �� " << endl;
	cout << "����������� ������ (��������): " << statEx.ullAvailVirtual << " ���� " << "/ " << statEx.ullAvailVirtual / (1024 * 1024) << " �� " << endl;
	cout << "����������� ����������� ������ (��������): " << statEx.ullAvailExtendedVirtual << " ����" << "/ " << statEx.ullAvailExtendedVirtual / (1024 * 1024) << " �� " << endl;

}

void StatusOfPage(LPVOID adress) {

	MEMORY_BASIC_INFORMATION memoryInfo = {0};
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	VirtualQuery(adress, &memoryInfo, memorySize);
	cout << "������� �����: " << memoryInfo.BaseAddress << endl;
	cout << "������� ����� ���������� ������: " << memoryInfo.AllocationBase << endl;
	cout << "������� ������: " << memoryInfo.AllocationProtect << endl;
	cout << "������ �������: " << memoryInfo.RegionSize << endl;

	cout << "��������� ��������: ";
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

	cout << "����� �������: " << memoryInfo.Protect << endl;
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

	cout << "��� ��������: " << memoryInfo.Type << endl;
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
	cout << "������� ������ (����): ";
	cin >> dwSize;

	cout << "���� ������ ������ ����� (������ �����), ������� 1, ����� - 0: ";
	cin >> answer;
	if (answer) {
		adress = 0;
		cout << "������� ������� �����: ";
		cin >> pBaseAdress;
	}
	else
		adress = NULL;
	pMemory = VirtualAlloc(pBaseAdress, dwSize, MEM_RESERVE, PAGE_READWRITE);

	if (pMemory == NULL) {
		cout << "������. ��� ���������������."  << endl;
		return;
	}
	else {
		cout << "��������������� " << endl;
		return;
	}
	
}

void ReservedAndTransit() {

	DWORD commit = MEM_COMMIT | MEM_RESERVE;
	LPVOID pMemory, adress;
	int answer;

	cout << "������� ������ (����): ";
	cin >> dwSize;
	cout << "���� ������ ������ ����� (������ �����), ������� 1, ����� - 0: ";
	cin >> answer;
	if (answer) {
		adress = 0;
		cout << "������� ������� �����: ";
		cin >> pBaseAdress;
	}
	else
		adress = NULL;
//	LPVOID p = VirtualAlloc((LPVOID)pBaseAdress, dwSize, MEM_RESERVE, PAGE_READWRITE);
	pMemory = VirtualAlloc(pBaseAdress, dwSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (pMemory)
		ZeroMemory(pMemory, dwSize);
//	else
	//	cout << "������  " << GetLastError();

	if (pMemory == NULL) {
		cout << "������. ��� ���������������."  << endl;
		return;
		}
	else {
		cout << "��������������� " << endl;
		return;
	}
}

void SetData() {

	MEMORY_BASIC_INFORMATION memoryInfo;
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	int data = 0;
	cout << "������� ������� �����: ";
	cin >> pBaseAdress;
	
	VirtualQuery(pBaseAdress, &memoryInfo, memorySize);
	if (memoryInfo.Protect & PAGE_NOACCESS) {
		cout << "������ ��������. ���� ������ ����������." << endl;
		return;
	}

	cout << "������� ������: ";
	cin >> data;

	if (pBaseAdress) {
		memcpy(pBaseAdress, &data, sizeof(int));
		cout << "������ �������� �� ������: " << pBaseAdress << endl;
		cout << "��������: " << endl;
		cout << "������ �� ������: " <<  pBaseAdress << " ������: " << (*(PDWORD)pBaseAdress) << endl;
	}
	
}

void SetProtect() {

	DWORD oldProtect;
	MEMORY_BASIC_INFORMATION memoryInfo;
	SIZE_T memorySize = sizeof(MEMORY_BASIC_INFORMATION);
	cout << "������� ������� �����: ";
	cin >> pBaseAdress;
	cout << "������� ������ ������� (����): ";
	cin >> dwSize;
	bool f;
	if (pBaseAdress) {
		f = VirtualProtect(pBaseAdress, dwSize, 0x04, &oldProtect);
		if (!f)
			cout << GetLastError();
		cout << "������ �������� " << endl;
		cout << "��������: " << endl;
		StatusOfPage(pBaseAdress);
	}
	else {
		cout << "��������. ���: " << GetLastError() << endl;
		system("pause");
	}

}

void Free() {

	GetSystemInfo(&systemInfo);
	cout << "������� ����� � ��������� �� " << systemInfo.lpMinimumApplicationAddress << " �� " << systemInfo.lpMaximumApplicationAddress << ": ";
	cin >> pBaseAdress;
	cout << "������� ������ ������� (����): ";
	cin >> dwSize;
	
	if (!VirtualFree(pBaseAdress, dwSize, MEM_DECOMMIT))
	{
		cout << "������: " << GetLastError() << endl;
		system("pause");
	}

	if (VirtualFree(pBaseAdress, 0, MEM_RELEASE)) {
		cout << "������ �������." << endl;
		cout << "��������: " << endl;
		StatusOfPage(pBaseAdress);
	}
	else
		cout << "������. ���: " << GetLastError();
	
}