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

vector <char*> vDrive; //вектор дисков

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
	system("title = Управление файловой системой");

	int choice, choiceDir, choiceCopy, choiceAttr;

	do {
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "			1. Вывод списка дисков" << endl;
		cout << "			2. Информация о выбранном диске " << endl;
		cout << "			3. Создание и удаление заданного каталога " << endl;
		cout << "			4. Создание файла в новом каталоге " << endl;
		cout << "			5. Копирование и перемещение файла " << endl;
		cout << "			6. Анализ атрибутов " << endl;
		cout << "			7. Изменение атрибутов " << endl;
		cout << "			8. Выход из приложения" << endl;
		cout << "Выберите пункт меню: " << endl;
		cin >> choice;
		if (choice < 0 || choice > 8) {
			cout << "Ошибка. Введите пункт меню заново.";
			_getch();
		}
		switch (choice) {
		case 1:
			system("cls");
			cout << "				Вывод списка дисков: " << endl;
			cout << "С помощью функции GetLogicalDrives(): " << endl;
			Drive();
			cout << "Нажмите Enter для продолжения." << endl;
			_getch();
			cout << "С помощью функции GetLogicalDriveStrings(): " << endl;
			DriveStrings();
			cout << "Нажмите Enter для продолжения.";
			_getch();
			break;
		case 2:
			system("cls");
			DriveInfo();
			break;
		case 3:
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. Создать каталог." << endl;
			cout << "2. Удалить каталог." << endl;
			cout << "Выберите пункт меню: " << endl;
			cin >> choiceDir;
			if (choiceDir < 0 || choiceDir > 2) {
				cout << "Ошибка. Введите пункт меню заново.";
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
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. Скопировать файл." << endl;
			cout << "2. Переместить файл." << endl;
			cout << "Выберите пункт меню: " << endl;
			cin >> choiceCopy;
			if (choiceCopy < 0 || choiceCopy > 2) {
				cout << "Ошибка. Введите пункт меню заново.";
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
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "1. Установить атрибут Только для чтения." << endl;
			cout << "2. Скрыть файл." << endl;
			cout << "3. Изменить время." << endl;
			cout << "Выберите пункт меню: " << endl;
			cin >> choiceAttr;
			if (choiceAttr < 0 || choiceAttr > 3) {
				cout << "Ошибка. Введите пункт меню заново.";
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
	cout << "Список дисков: " << endl;
	for (int i = 0; i < 32; i++) {
		if (drive & mask) {
			printf("%d)%c\n", n, i + 65);
			n++;
		}
		mask <<= 1;
	}
}

void DriveStrings() {
	//вернет длину строки с дисками, поэтому 0
	DWORD dwSize = GetLogicalDriveStrings(0, NULL);  //длина буфера, в который записывается строка с дисками
													 //используем свободную область памяти и выделяем память
	HANDLE hHeap = GetProcessHeap();
	char* sizeDrives = (char*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize); //указатель на буфер для строки
	GetLogicalDriveStrings(dwSize, sizeDrives); //заполняем буфер строками, которые определяют диски
	char* sizeNext = sizeDrives;
	cout << "Список дисков: " << endl;
	//т.к. идёт разделение 0-байтом, достаём из строк только диски
	while (sizeNext[0] != 0)
	{
		cout << sizeNext << endl;
		vDrive.push_back(sizeNext);
		sizeNext = strchr(sizeNext, 0) + 1; //strchr - первое вхождение символа в строку
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
		cout << "			Информация о выбранном диске: " << endl;
		Drive();
		cout << "Выберите диск: " << endl;
		cout << "Пример ввода: C:\\ и т.д. English, please.  ";
		//while (getchar() != '\n');
		//gets_s(string);

		cin >> DriveName;
		cout << "				Результат: " << endl;
		type = GetDriveType(DriveName);
		cout << endl;
		switch (type) {
		case DRIVE_UNKNOWN:
			cout << "Не удалось определить тип." << endl;
			break;
		case DRIVE_NO_ROOT_DIR:
			cout << "Корневой диск не существует." << endl;
			break;
		case DRIVE_REMOVABLE:
			cout << "Гибкий диск." << endl;
			break;
		case DRIVE_FIXED:
			cout << "Жёсткий диск." << endl;
			break;
		case DRIVE_REMOTE:
			cout << "Сетевой диск." << endl;
			break;
		case DRIVE_CDROM:
			cout << "CD/DVD диск." << endl;
			break;
		case DRIVE_RAMDISK:
			cout << "RAM-диск." << endl;
			break;
		}

		bool res = GetVolumeInformation(DriveName, VolumeNameBuffer, sizeof(VolumeNameBuffer), &SerialNumber, &MaximumComponentLength, &FileSF,
			FileSystemNameBuffer, sizeof(FileSystemNameBuffer));
		if (res) {
			cout << endl;
			cout << "Название диска (отображается только английский): " << VolumeNameBuffer << endl;
			cout << "Серийный номер: " << SerialNumber << endl;
			cout << "Файловая система: " << FileSystemNameBuffer << endl;
		}
		bool res2 = GetDiskFreeSpace(DriveName, &dwSectorsPerCluster, &dwBytesPerCluster,
			&dwNumberOfFreeClusters, &dwTotalNumberOfClusters);
		if (res2) {
			cout << endl;
			cout << "Секторов в кластере: " << dwSectorsPerCluster << endl;
			cout << "Байт в кластере: " << dwBytesPerCluster << endl;
			cout << "Всего свободных кластеров на диске: " << dwNumberOfFreeClusters << endl;
			cout << "Всего кластеров на диске: " << dwTotalNumberOfClusters << endl;
			fTotalSpace = (float)dwTotalNumberOfClusters / 1024 * dwSectorsPerCluster*dwBytesPerCluster / 1048576;
			fFreeSpace = (float)dwNumberOfFreeClusters / 1024 * dwSectorsPerCluster*dwBytesPerCluster / 1048576;
			//cout << "посчитано" << endl;
			printf_s("Всего: %3.1f GB, Свободно: %3.1f GB\n\n", fTotalSpace, fFreeSpace);
		}
		do {
			cout << "Хотите повторить? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "Введите Y или N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void NewDirectory() {
	char answer;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	do {
		system("cls");
		cout << "			Создание каталога: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "Ошибка. Невозможно определить каталог." << endl;
		else
			cout << "Текущий каталог: " << info << endl;

		cout << "Введите название нового каталога: " << endl;
		//while (getchar() != '\n');
		//gets_s(directory);
		cin >> DirectoryName;

		if (!CreateDirectory(DirectoryName, lpSecurityAttributes)) {
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				cout << "Такой каталог уже существует." << endl;
		}
		else
			cout << "Каталог создан." << endl;
		if (!SetCurrentDirectory(DirectoryName))
			cout << "Ошибка. Невозможно установить данный каталог." << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "Ошибка. Невозможно определить каталог." << endl;
		else
			cout << "Текущий каталог: " << info << endl;
		do {
			cout << "Хотите повторить? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "Введите Y или N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void DeleteDirectory() {

	char answer;

	do {
		system("cls");
		cout << "			Удаление каталога: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "Ошибка. Невозможно определить каталог." << endl;
		else
			cout << "Текущий каталог: " << info << endl;
		cout << "Введите название удаляемого каталога (Текущий каталог удалить нельзя): " << endl;
		//while (getchar() != '\n');
		//	gets_s(directory);

		cin >> DirectoryName;

		if (RemoveDirectory(DirectoryName))
			cout << "Каталог удалён." << endl;
		else
			cout << "Нельзя удалить каталог. Возможно, он не пуст или не существует." << endl;

		do {
			cout << "Хотите повторить? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "Введите Y или N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void NewFile() {
	char answer;
	do {
		system("cls");
		cout << "			Создание нового файла: " << endl;
		if (!GetCurrentDirectory(buffer, info))
			cout << "Ошибка. Невозможно определить каталог." << endl;
		else
			cout << "Текущий каталог: " << info << endl;

		cout << "Введите название нового файла:" << endl;
		//	while (getchar() != '\n');
		//	gets_s(fileName);
		cin >> FileName;

		HANDLE hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			cout << "Файл не создан." << endl;
		else
			cout << "Файл создан." << endl;
		CloseHandle(hFile);

		do {
			cout << "Хотите повторить? Y/N:	";
			cin >> answer;
			if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
				cout << "Введите Y или N" << endl;
			}
		} while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
	} while (answer != 'N' && answer != 'n');
}

void CopyFiles() {

	system("cls");
	cout << "			Копирование файла: " << endl;
	cout << "Введите полный путь к копируемому файлу:" << endl;
	//	while (getchar() != '\n');
	//	gets_s(fileName);

	cin >> FileName;
	cout << "Введите полный путь к новому файлу:" << endl;
	cout << "Каталог должен быть предварительно создан." << endl;
	//while (getchar() != '\n');
	//gets_s(fileNameNew);
	cin >> FileNameNew;

	if (CopyFile(FileName, FileNameNew, false)) {
		//if (GetLastError() == ERROR_FILE_NOT_FOUND)
		//	cout << "Файл не найден." << endl;
		//else
		cout << "Скопировано." << endl;
	}
	else
		cout << "Невозможно выполнить копирование." << endl;
	_getch();
}

void MovingFile() {

	system("cls");
	cout << "			Перемещение файла: " << endl;
	cout << "Введите полный путь к перемещаемому файлу:" << endl;
	//	while (getchar() != '\n');
	//gets_s(fileName);

	cin >> FileName;
	cout << "Введите полный путь к новому файлу:" << endl;
	cout << "Каталог должен быть предварительно создан." << endl;
	//while (getchar() != '\n');
	//gets_s(fileNameNew);
	cin >> FileNameNew;

	if (MoveFile(FileName, FileNameNew)) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			cout << "Файл не найден." << endl;
		else
			cout << "Перемещено." << endl;
	}
	else
		cout << "Невозможно переместить файл." << endl;
	_getch();
}

void FileAttr() {

	system("cls");
	cout << "			Анализ атрибутов: " << endl;
	cout << "Введите полный путь к файлу:" << endl;
	cin >> FileName;
	hFile = CreateFile(FileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Ошибка. Невозможно открыть файл." << endl <<
			"Код: " << GetLastError() << endl;
		_getch();
	}
	if (!GetFileInformationByHandle(hFile, &lpFileInformation)) {
		cout << "Ошибка." << GetLastError() << endl;
		_getch();
	}
	attr = lpFileInformation.dwFileAttributes;
	//if (attr = INVALID_FILE_ATTRIBUTES)
	//	cout << "Ошибка. Невозможно определить атрибуты." << endl;
	//else {
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		cout << "Это - каталог." << endl;
	if (attr & FILE_ATTRIBUTE_ARCHIVE)
		cout << "Archive. Подлежит резервному копированию." << endl;
	if (attr & FILE_ATTRIBUTE_COMPRESSED)
		cout << "Сжатый." << endl;
	if (attr & FILE_ATTRIBUTE_DEVICE)
		cout << "Зарезервирован." << endl;
	if (attr & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "Зашифрован." << endl;
	if (attr & FILE_ATTRIBUTE_HIDDEN)
		cout << "Скрыт." << endl;
	if (attr & FILE_ATTRIBUTE_NORMAL)
		cout << "Нет других установленных атрибутов." << endl;
	if (attr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
		cout << "Не индесирован." << endl;
	if (attr & FILE_ATTRIBUTE_OFFLINE)
		cout << "Данные доступны не сразу." << endl;
	if (attr & FILE_ATTRIBUTE_READONLY)
		cout << "Только для чтения." << endl;
	if (attr & FILE_ATTRIBUTE_REPARSE_POINT)
		cout << "Имеет связанную точку повторной обработки." << endl;
	if (attr & FILE_ATTRIBUTE_SPARSE_FILE)
		cout << "Разреженный файл." << endl;
	if (attr & FILE_ATTRIBUTE_SYSTEM)
		cout << "Используется ОС." << endl;
	if (attr & FILE_ATTRIBUTE_TEMPORARY)
		cout << "Используется для временного хранения." << endl;

	if (FileTimeToLocalFileTime(&lpFileInformation.ftCreationTime, &CreationTime) &&
		FileTimeToSystemTime(&CreationTime, &SystemTimeCreation)) {
		cout << "Время создания: " << SystemTimeCreation.wDay << "/" << SystemTimeCreation.wMonth <<
			"/" << SystemTimeCreation.wYear << "     " << SystemTimeCreation.wHour << ":" <<
			SystemTimeCreation.wMinute << ":" << SystemTimeCreation.wSecond << ":" <<
			SystemTimeCreation.wMilliseconds << endl;
	}
	else
		cout << "Невозможно определить время создания.";

	if (FileTimeToLocalFileTime(&lpFileInformation.ftLastAccessTime, &LastAccessTime) &&
		FileTimeToSystemTime(&LastAccessTime, &SystemTimeAccess)) {
		cout << "Время последнего доступа к файлу: " << SystemTimeAccess.wDay << "/" <<
			SystemTimeAccess.wMonth << "/" << SystemTimeAccess.wYear << "     " <<
			SystemTimeAccess.wHour << ":" << SystemTimeAccess.wMinute << ":" << SystemTimeAccess.wSecond << ":" <<
			SystemTimeAccess.wMilliseconds << endl;
	}
	else
		cout << "Невозможно определить время последнего доступа к файлу.";

	if (FileTimeToLocalFileTime(&lpFileInformation.ftLastWriteTime, &LastWriteTime) &&
		FileTimeToSystemTime(&LastWriteTime, &SystemTimeWrite)) {
		cout << "Время последней записи в файл: " << SystemTimeWrite.wDay << "/" <<
			SystemTimeWrite.wMonth << "/" << SystemTimeWrite.wYear << "     " <<
			SystemTimeWrite.wHour << ":" << SystemTimeWrite.wMinute << ":" << SystemTimeWrite.wSecond << ":" <<
			SystemTimeWrite.wMilliseconds << endl;
	}
	else
		cout << "Невозможно определить время последней записи в файл.";

	CloseHandle(hFile);
	_getch();
}

void ReadOnly() {

	system("cls");
	cout << "			Установка атрибута только для чтения: " << endl;
	cout << "Введите полный путь к файлу: " << endl;
	cin >> FileName;
	attr = GetFileAttributes(FileName);
	if (attr == INVALID_FILE_ATTRIBUTES)
		cout << "Невозможно определить атрибуты." << endl;
	else
	{
		if (SetFileAttributes(FileName, attr | FILE_ATTRIBUTE_READONLY))
			cout << "Атрибут только для чтения установлен." << endl;
		else
			cout << "Невозможно установить атрибут." << endl;
	}
	_getch();
}

void HideFile() {

	system("cls");
	cout << "			Скрытие файла: " << endl;
	cout << "Введите полный путь к файлу: " << endl;
	cin >> FileName;
	attr = GetFileAttributes(FileName);
	if (attr == INVALID_FILE_ATTRIBUTES)
		cout << "Невозможно определить атрибуты." << endl;
	else
	{
		if (SetFileAttributes(FileName, attr | FILE_ATTRIBUTE_HIDDEN))
			cout << "Файл скрыт." << endl;
		else
			cout << "Невозможно установить атрибут." << endl;
	}
	_getch();
}

void SetFileTimeToCurrentTime() {

	system("cls");

	cout << "Введите полный путь к файлу:" << endl;
	cin >> FileName;
	hFile = CreateFile(FileName, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Ошибка. Невозможно открыть файл." << endl <<
			"Код: " << GetLastError() << endl;
		_getch();
	}

	WORD year, month, day, hour, minute, seconds;
	cout << "Введите год: ";
	cin >> year;
	cout << "Введите месяц: ";
	cin >> month;
	cout << "Введите день: ";
	cin >> day;
	cout << "Введите час: ";
	cin >> hour;
	cout << "Введите минуты: ";
	cin >> minute;
	cout << "Введите секунды: ";
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
		cout << " код " << GetLastError();


	cout << "Время последней записи в файл: " << SystemTimeWrite.wDay << "/" <<
		SystemTimeWrite.wMonth << "/" << SystemTimeWrite.wYear << "     " <<
		SystemTimeWrite.wHour << ":" << SystemTimeWrite.wMinute << ":" << SystemTimeWrite.wSecond << ":" <<
		SystemTimeWrite.wMilliseconds << endl;

	CloseHandle(hFile);
	_getch();
}