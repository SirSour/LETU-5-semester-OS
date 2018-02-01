#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include <cstdio>
#include "vector"
#include "iostream"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS
HANDLE hFile;
BY_HANDLE_FILE_INFORMATION lpFileInformation;
TCHAR FileName[_MAX_FNAME];
TCHAR FileNameNew[_MAX_FNAME];
TCHAR DriveName[_MAX_FNAME];
TCHAR DirectoryName[_MAX_FNAME];
DWORD attr;
TCHAR info[256];
DWORD buffer = 256;
FILETIME CreationTime;
FILETIME LastAccessTime;
FILETIME LastWriteTime;
FILETIME LastWriteLocalTime;
SYSTEMTIME SystemTimeCreation;
SYSTEMTIME SystemTimeAccess;
SYSTEMTIME SystemTimeWrite;

vector <char*> vDrive; //������ ������

void Drive();
void DriveStrings();
void DriveInfo();
void NewDirectory();
void DeleteDirectory();
void NewFile();
void CopyFiles();
void MovingFile();
void FileAttr();
void ReadOnly();
void HideFile();
void SetFileTimeToCurrentTime();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title = ���������� �������� ��������");

	int choice, choiceDir, choiceCopy, choiceAttr;

	do {
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "			1. ����� ������ ������" << endl;
		cout << "			2. ���������� � ��������� ����� " << endl;
		cout << "			3. �������� � �������� ��������� �������� " << endl;
		cout << "			4. �������� ����� � ����� �������� " << endl;
		cout << "			5. ����������� � ����������� ����� " << endl;
		cout << "			6. ������ ��������� " << endl;
		cout << "			7. ��������� ��������� " << endl;
		cout << "			8. ����� �� ����������" << endl;
		cout << "�������� ����� ����: " << endl;
		cin >> choice;
		if (choice < 0 || choice > 8) {
			cout << "������. ������� ����� ���� ������.";
			_getch();
		}
		switch (choice) {
		case 1:
			system("cls");
			cout << "				����� ������ ������: " << endl;
			cout << "� ������� ������� GetLogicalDrives(): " << endl;
			Drive();
			cout << "������� Enter ��� �����������." << endl;
			_getch();
			cout << "� ������� ������� GetLogicalDriveStrings(): " << endl;
			DriveStrings();
			cout << "������� Enter ��� �����������.";
			_getch();
			break;
		case 2:
			system("cls");
			DriveInfo();
			break;
		case 3:
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. ������� �������." << endl;
			cout << "2. ������� �������." << endl;
			cout << "�������� ����� ����: " << endl;
			cin >> choiceDir;
			if (choiceDir < 0 || choiceDir > 2) {
				cout << "������. ������� ����� ���� ������.";
				_getch();
			}
			switch (choiceDir) {
			case 1:
				NewDirectory();
				break;
			case 2:
				DeleteDirectory();
				break;
			}
			break;
		case 4:
			NewFile();
			break;
		case 5:
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. ����������� ����." << endl;
			cout << "2. ����������� ����." << endl;
			cout << "�������� ����� ����: " << endl;
			cin >> choiceCopy;
			if (choiceCopy < 0 || choiceCopy > 2) {
				cout << "������. ������� ����� ���� ������.";
				_getch();
			}
			switch (choiceCopy) {
			case 1:
				CopyFiles();
				break;
			case 2:
				MovingFile();
				break;
			}
			break;
		case 6:
			FileAttr();
			break;
		case 7:
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. ���������� ������� ������ ��� ������." << endl;
			cout << "2. ������ ����." << endl;
			cout << "3. �������� �����." << endl;
			cout << "�������� ����� ����: " << endl;
			cin >> choiceAttr;
			if (choiceAttr < 0 || choiceAttr > 3) {
				cout << "������. ������� ����� ���� ������.";
				_getch();
			}
			switch (choiceAttr) {
			case 1:
				ReadOnly();
				break;
			case 2:
				HideFile();
				break;
			case 3:
				SetFileTimeToCurrentTime();
				break;
			}
			break;
		case 8:
			break;
		default:
			rewind(stdin);
		}
	} while (choice != 8);

	return 0;
}

void Drive() {

	DWORD drive = 0;
	drive = GetLogicalDrives();
	unsigned long mask = 1;
	int n = 1;
	cout << "������ ������: " << endl;
	for (int i = 0; i < 32; i++) {
		if (drive & mask) {
			printf("%d)%c\n", n, i + 65);
			n++;
		}
		mask <<= 1;
	}
}

void DriveStrings() {
	//������ ����� ������ � �������, ������� 0
	DWORD dwSize = GetLogicalDriveStrings(0, NULL);  //����� ������, � ������� ������������ ������ � �������
													 //���������� ��������� ������� ������ � �������� ������
	HANDLE hHeap = GetProcessHeap();
	char* sizeDrives = (char*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize); //��������� �� ����� ��� ������
	GetLogicalDriveStrings(dwSize, sizeDrives); //��������� ����� ��������, ������� ���������� �����
	char* sizeNext = sizeDrives;
	cout << "������ ������: " << endl;
	//�.�. ��� ���������� 0-������, ������ �� ����� ������ �����
	while (sizeNext[0] != 0)
	{
		cout << sizeNext << endl;
		vDrive.push_back(sizeNext);
		sizeNext = strchr(sizeNext, 0) + 1; //strchr - ������ ��������� ������� � ������
	}
	HeapFree(hHeap, 0, sizeDrives);
}

void DriveInfo() {
	UINT type = 0;
	char answer, VolumeNameBuffer[100], FileSystemNameBuffer[100];
	DWORD dwSectorsPerCluster, dwBytesPerCluster, dwNumberOfFreeClusters, dwTotalNumberOfClusters, MaximumComponentLength, FileSF;
	unsigned long SerialNumber;
	float fTotalSpace, fFreeSpace;
	do {
		system("cls");
		cout << "			���������� � ��������� �����: " << endl;
		Drive();
		cout << "�������� ����: " << endl;
		cout << "������ �����: C:\\ � �.�. English, please.  ";
		//while (getchar() != '\n');
		//gets_s(string);

		cin >> DriveName;
		cout << "				���������: " << endl;
		type = GetDriveType(DriveName);
		cout << endl;
		switch (type) {
		case DRIVE_UNKNOWN:
			cout << "�� ������� ���������� ���." << endl;
			break;
		case DRIVE_NO_ROOT_DIR:
			cout << "�������� ���� �� ����������." << endl;
			break;
		case DRIVE_REMOVABLE:
			cout << "������ ����." << endl;
			break;
		case DRIVE_FIXED:
			cout << "Ƹ����� ����." << endl;
			break;
		case DRIVE_REMOTE:
			cout << "������� ����." << endl;
			break;
		case DRIVE_CDROM:
			cout << "CD/DVD ����." << endl;
			break;
		case DRIVE_RAMDISK:
			cout << "RAM-����." << endl;
			break;
		}

		bool res = GetVolumeInformation(DriveName, VolumeNameBuffer, sizeof(VolumeNameBuffer), &SerialNumber, &MaximumComponentLength, &FileSF,
			FileSystemNameBuffer, sizeof(FileSystemNameBuffer));
		if (res) {
			cout << endl;
			cout << "�������� ����� (������������ ������ ����������): " << VolumeNameBuffer << endl;
			cout << "�������� �����: " << SerialNumber << endl;
			cout << "�������� �������: " << FileSystemNameBuffer << endl;
		}
		bool res2 = GetDiskFreeSpace(DriveName, &dwSectorsPerCluster, &dwBytesPerCluster,
			&dwNumberOfFreeClusters, &dwTotalNumberOfClusters);
		if (res2) {
			cout << endl;
			cout << "�������� � ��������: " << dwSectorsPerCluster << endl;
			cout << "���� � ��������: " << dwBytesPerCluster << endl;
			cout << "����� ��������� ��������� �� �����: " << dwNumberOfFreeClusters << endl;
			cout << "����� ��������� �� �����: " << dwTotalNumberOfClusters << endl;
			fTotalSpace = (float)dwTotalNumberOfClusters / 1024 * dwSectorsPerCluster*dwBytesPerCluster / 1048576;
			fFreeSpace = (float)dwNumberOfFreeClusters / 1024 * dwSectorsPerCluster*dwBytesPerCluster / 1048576;
			//cout << "���������" << endl;
			printf_s("�����: %3.1f GB, ��������: %3.1f GB\n\n", fTotalSpace, fFreeSpace);
		}
		do {
			cout << "������ ���������? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "������� Y ��� N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void NewDirectory() {
	char answer;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	do {
		system("cls");
		cout << "			�������� ��������: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "������. ���������� ���������� �������." << endl;
		else
			cout << "������� �������: " << info << endl;

		cout << "������� �������� ������ ��������: " << endl;
		//while (getchar() != '\n');
		//gets_s(directory);
		cin >> DirectoryName;

		if (!CreateDirectory(DirectoryName, lpSecurityAttributes)) {
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				cout << "����� ������� ��� ����������." << endl;
		}
		else
			cout << "������� ������." << endl;
		if (!SetCurrentDirectory(DirectoryName))
			cout << "������. ���������� ���������� ������ �������." << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "������. ���������� ���������� �������." << endl;
		else
			cout << "������� �������: " << info << endl;
		do {
			cout << "������ ���������? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "������� Y ��� N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void DeleteDirectory() {

	char answer;

	do {
		system("cls");
		cout << "			�������� ��������: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "������. ���������� ���������� �������." << endl;
		else
			cout << "������� �������: " << info << endl;
		cout << "������� �������� ���������� �������� (������� ������� ������� ������): " << endl;
		//while (getchar() != '\n');
		//	gets_s(directory);

		cin >> DirectoryName;

		if (RemoveDirectory(DirectoryName))
			cout << "������� �����." << endl;
		else
			cout << "������ ������� �������. ��������, �� �� ���� ��� �� ����������." << endl;

		do {
			cout << "������ ���������? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "������� Y ��� N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void NewFile() {
	char answer;
	do {
		system("cls");
		cout << "			�������� ������ �����: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "������. ���������� ���������� �������." << endl;
		else
			cout << "������� �������: " << info << endl;

		cout << "������� �������� ������ �����:" << endl;
		//	while (getchar() != '\n');
		//	gets_s(fileName);
		cin >> FileName;

		HANDLE hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			cout << "���� �� ������." << endl;
		else
			cout << "���� ������." << endl;
		CloseHandle(hFile);

		do {
			cout << "������ ���������? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "������� Y ��� N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void CopyFiles() {

	system("cls");
	cout << "			����������� �����: " << endl;
	cout << "������� ������ ���� � ����������� �����:" << endl;
	//	while (getchar() != '\n');
	//	gets_s(fileName);

	cin >> FileName;
	cout << "������� ������ ���� � ������ �����:" << endl;
	cout << "������� ������ ���� �������������� ������." << endl;
	//while (getchar() != '\n');
	//gets_s(fileNameNew);
	cin >> FileNameNew;

	if (CopyFile(FileName, FileNameNew, false)) {
		//if (GetLastError() == ERROR_FILE_NOT_FOUND)
		//	cout << "���� �� ������." << endl;
		//else
		cout << "�����������." << endl;
	}
	else
		cout << "���������� ��������� �����������." << endl;
	_getch();
}

void MovingFile() {

	system("cls");
	cout << "			����������� �����: " << endl;
	cout << "������� ������ ���� � ������������� �����:" << endl;
	//	while (getchar() != '\n');
	//gets_s(fileName);

	cin >> FileName;
	cout << "������� ������ ���� � ������ �����:" << endl;
	cout << "������� ������ ���� �������������� ������." << endl;
	//while (getchar() != '\n');
	//gets_s(fileNameNew);
	cin >> FileNameNew;

	if (MoveFile(FileName, FileNameNew)) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			cout << "���� �� ������." << endl;
		else
			cout << "����������." << endl;
	}
	else
		cout << "���������� ����������� ����." << endl;
	_getch();
}

void FileAttr() {

	system("cls");
	cout << "			������ ���������: " << endl;
	cout << "������� ������ ���� � �����:" << endl;
	cin >> FileName;
	hFile = CreateFile(FileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "������. ���������� ������� ����." << endl <<
			"���: " << GetLastError() << endl;
		_getch();
	}
	if (!GetFileInformationByHandle(hFile, &lpFileInformation)) {
		cout << "������." << GetLastError() << endl;
		_getch();
	}
	attr = lpFileInformation.dwFileAttributes;
	//if (attr = INVALID_FILE_ATTRIBUTES)
	//	cout << "������. ���������� ���������� ��������." << endl;
	//else {
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		cout << "��� - �������." << endl;
	if (attr & FILE_ATTRIBUTE_ARCHIVE)
		cout << "Archive. �������� ���������� �����������." << endl;
	if (attr & FILE_ATTRIBUTE_COMPRESSED)
		cout << "������." << endl;
	if (attr & FILE_ATTRIBUTE_DEVICE)
		cout << "��������������." << endl;
	if (attr & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "����������." << endl;
	if (attr & FILE_ATTRIBUTE_HIDDEN)
		cout << "�����." << endl;
	if (attr & FILE_ATTRIBUTE_NORMAL)
		cout << "��� ������ ������������� ���������." << endl;
	if (attr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
		cout << "�� �����������." << endl;
	if (attr & FILE_ATTRIBUTE_OFFLINE)
		cout << "������ �������� �� �����." << endl;
	if (attr & FILE_ATTRIBUTE_READONLY)
		cout << "������ ��� ������." << endl;
	if (attr & FILE_ATTRIBUTE_REPARSE_POINT)
		cout << "����� ��������� ����� ��������� ���������." << endl;
	if (attr & FILE_ATTRIBUTE_SPARSE_FILE)
		cout << "����������� ����." << endl;
	if (attr & FILE_ATTRIBUTE_SYSTEM)
		cout << "������������ ��." << endl;
	if (attr & FILE_ATTRIBUTE_TEMPORARY)
		cout << "������������ ��� ���������� ��������." << endl;

	if (FileTimeToLocalFileTime(&lpFileInformation.ftCreationTime, &CreationTime) &&
		FileTimeToSystemTime(&CreationTime, &SystemTimeCreation)) {
		cout << "����� ��������: " << SystemTimeCreation.wDay << "/" << SystemTimeCreation.wMonth <<
			"/" << SystemTimeCreation.wYear << "     " << SystemTimeCreation.wHour << ":" <<
			SystemTimeCreation.wMinute << ":" << SystemTimeCreation.wSecond << ":" <<
			SystemTimeCreation.wMilliseconds << endl;
	}
	else
		cout << "���������� ���������� ����� ��������.";

	if (FileTimeToLocalFileTime(&lpFileInformation.ftLastAccessTime, &LastAccessTime) &&
		FileTimeToSystemTime(&LastAccessTime, &SystemTimeAccess)) {
		cout << "����� ���������� ������� � �����: " << SystemTimeAccess.wDay << "/" <<
			SystemTimeAccess.wMonth << "/" << SystemTimeAccess.wYear << "     " <<
			SystemTimeAccess.wHour << ":" << SystemTimeAccess.wMinute << ":" << SystemTimeAccess.wSecond << ":" <<
			SystemTimeAccess.wMilliseconds << endl;
	}
	else
		cout << "���������� ���������� ����� ���������� ������� � �����.";

	if (FileTimeToLocalFileTime(&lpFileInformation.ftLastWriteTime, &LastWriteTime) &&
		FileTimeToSystemTime(&LastWriteTime, &SystemTimeWrite)) {
		cout << "����� ��������� ������ � ����: " << SystemTimeWrite.wDay << "/" <<
			SystemTimeWrite.wMonth << "/" << SystemTimeWrite.wYear << "     " <<
			SystemTimeWrite.wHour << ":" << SystemTimeWrite.wMinute << ":" << SystemTimeWrite.wSecond << ":" <<
			SystemTimeWrite.wMilliseconds << endl;
	}
	else
		cout << "���������� ���������� ����� ��������� ������ � ����.";

	CloseHandle(hFile);
	_getch();
}

void ReadOnly() {

	system("cls");
	cout << "			��������� �������� ������ ��� ������: " << endl;
	cout << "������� ������ ���� � �����: " << endl;
	cin >> FileName;
	attr = GetFileAttributes(FileName);
	if (attr == INVALID_FILE_ATTRIBUTES)
		cout << "���������� ���������� ��������." << endl;
	else
	{
		if (SetFileAttributes(FileName, attr | FILE_ATTRIBUTE_READONLY))
			cout << "������� ������ ��� ������ ����������." << endl;
		else
			cout << "���������� ���������� �������." << endl;
	}
	_getch();
}

void HideFile() {

	system("cls");
	cout << "			������� �����: " << endl;
	cout << "������� ������ ���� � �����: " << endl;
	cin >> FileName;
	attr = GetFileAttributes(FileName);
	if (attr == INVALID_FILE_ATTRIBUTES)
		cout << "���������� ���������� ��������." << endl;
	else
	{
		if (SetFileAttributes(FileName, attr | FILE_ATTRIBUTE_HIDDEN))
			cout << "���� �����." << endl;
		else
			cout << "���������� ���������� �������." << endl;
	}
	_getch();
}

void SetFileTimeToCurrentTime() {

	system("cls");

	cout << "������� ������ ���� � �����:" << endl;
	cin >> FileName;
	hFile = CreateFile(FileName, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "������. ���������� ������� ����." << endl <<
			"���: " << GetLastError() << endl;
		_getch();
	}

	WORD year, month, day, hour, minute, seconds;
	cout << "������� ���: ";
	cin >> year;
	cout << "������� �����: ";
	cin >> month;
	cout << "������� ����: ";
	cin >> day;
	cout << "������� ���: ";
	cin >> hour;
	cout << "������� ������: ";
	cin >> minute;
	cout << "������� �������: ";
	cin >> seconds;

	SystemTimeWrite.wYear = year;
	SystemTimeWrite.wMonth = month;
	SystemTimeWrite.wDay = day;
	SystemTimeWrite.wHour = hour;
	SystemTimeWrite.wMinute = minute;
	SystemTimeWrite.wSecond = seconds;


	SystemTimeToFileTime(&SystemTimeWrite, &LastWriteLocalTime);
	LocalFileTimeToFileTime(&LastWriteLocalTime, &LastWriteTime);
	

	if (!SetFileTime(hFile, (LPFILETIME)NULL, (LPFILETIME)NULL, &LastWriteTime))
		cout << " ��� " << GetLastError();


	cout << "����� ��������� ������ � ����: " << SystemTimeWrite.wDay << "/" <<
		SystemTimeWrite.wMonth << "/" << SystemTimeWrite.wYear << "     " <<
		SystemTimeWrite.wHour << ":" << SystemTimeWrite.wMinute << ":" << SystemTimeWrite.wSecond << ":" <<
		SystemTimeWrite.wMilliseconds << endl;

	CloseHandle(hFile);
	_getch();
}