#include <string>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>


const std::string CREATOR = "Creator.exe";
const std::string REPORTER = "Reporter.exe";


int main()
{
	std::cout << "//Main process//\n\n";

	std::string binaryFileName;
	std::cout << "Enter binary file name: ";
	std::cin >> binaryFileName;

	std::string amountOfNotes;
	std::cout << "Enter a amount of notes in this file: ";
	std::cin >> amountOfNotes;

	std::string messageToCreator = CREATOR + " " + binaryFileName + " " + amountOfNotes;
	std::wstring converterToLPWSTR(messageToCreator.begin(), messageToCreator.end());
	LPWSTR lpszCreatorMessage = &converterToLPWSTR[0];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, lpszCreatorMessage, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		system("cls");
		std::cout << "Something Wrong with Creator process";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	std::string reportFileName;
	std::cout << "Enter report file name: ";
	std::cin >> reportFileName;
	reportFileName += ".txt";
	std::string salaryPerHour;
	std::cout << "Enter salary per hour: ";
	std::cin >> salaryPerHour;

	std::string messageToReporter = REPORTER + " " + binaryFileName + " " + reportFileName + " " + salaryPerHour;
	converterToLPWSTR = std::wstring(messageToReporter.begin(), messageToReporter.end());
	LPWSTR lpszReporterMessage = &converterToLPWSTR[0];

	if (!CreateProcess(NULL, lpszReporterMessage, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		system("cls");
		std::cout << "Something Wrong with Reporter process";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	std::ifstream reportInput(reportFileName);
	if (reportInput.is_open())
	{
		while (!reportInput.eof())
		{
			std::string temp;
			getline(reportInput, temp);
			std::cout << temp << "\n";
		}
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_getch();

	return 0;
}


