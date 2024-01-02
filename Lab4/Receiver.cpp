#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

HANDLE SenderSemaphore;

int main()
{
	char binary_file_name[256];

	cout << "Process Receiver started work." << endl << endl;

	cout << "Enter binary file name (for example, \"file\") : ";
	cin.getline(binary_file_name, 256);

	int records_num = 0;
	cout << "Enter the number of records in that file: ";
	cin >> records_num;

	char binary_file_path[1000];
	sprintf_s(binary_file_path, "%s", binary_file_name);
	ofstream f(binary_file_path, ios::binary);
	f.close();

	int senders_amount = 0;
	cout << "Enter the number of processes Sender: ";
	cin >> senders_amount;

	cin.ignore();

	SenderSemaphore = CreateSemaphore(
		NULL,
		0,
		records_num,
		TEXT("Sender"));

	HANDLE* EventsFromSender;

	EventsFromSender = new HANDLE[senders_amount];
	for (int i = 0; i < senders_amount; i++)
	{
		char EventNum[256];
		sprintf_s(EventNum, "Event of the process %d", (i + 1));
		int len1 = strlen(EventNum) + 1;
		int size1 = MultiByteToWideChar(CP_UTF8, 0, EventNum, len1, NULL, 0);
		wchar_t* wfileName = new wchar_t[size1];
		MultiByteToWideChar(CP_UTF8, 0, EventNum, len1, wfileName, size1);
		EventsFromSender[i] = CreateEvent(
			NULL,
			TRUE,
			FALSE,
			wfileName
		);
	}

	STARTUPINFO* Sender = new STARTUPINFO[senders_amount];
	PROCESS_INFORMATION* Sender_Info = new PROCESS_INFORMATION[senders_amount];

	for (int i = 0; i < senders_amount; i++)
	{
		ZeroMemory(&(Sender[i]), sizeof(Sender[i]));
		(Sender[i]).cb = sizeof(Sender[i]);
		ZeroMemory(&(Sender_Info[i]), sizeof(Sender_Info[i]));

		char buffer[1000];

		sprintf_s(buffer, "%s %s %d", "Sender.exe", binary_file_name, (i + 1));

		int len1 = strlen(buffer) + 1;
		int size1 = MultiByteToWideChar(CP_UTF8, 0, buffer, len1, NULL, 0);
		wchar_t* wfileName = new wchar_t[size1];
		MultiByteToWideChar(CP_UTF8, 0, buffer, len1, wfileName, size1);

		CreateProcess(NULL,
			wfileName,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&(Sender[i]),
			&(Sender_Info[i])
		);
	}

	cout << endl << "Waiting all Sender processes to be ready for next part of work..." << endl;

	WaitForMultipleObjects(senders_amount, EventsFromSender, TRUE, 1000);

	cout << "All the sender processes are ready to resume work..." << endl << endl;

	int current_records_num = 0;

	while (true)
	{
		if (current_records_num >= records_num)
		{
			cout << endl << "Record limit reached, process exits read mode..." << endl;
			break;
		}
		ifstream fin(binary_file_path, ios::binary);
		bool file_empty = false;
		if (fin.peek() == EOF)
			file_empty = true;
		fin.close();
		if (file_empty)
		{
			cout << "Binary file is empty now. Waiting for messages from Sender processes..." << endl << endl;
			while (true)
			{
				ifstream fin(binary_file_path, ios::binary);
				if (fin.peek() != EOF)
					break;
				fin.close();
			}
		}
		cout << "There is some new message from one of the Sender processes..." << endl;
		cout << "Process Receiver: Write \"exit\" to complete work or anything else in order to read some more message from file:" << endl;
		char answer[256];
		cin.getline(answer, 256);
		if (!strncmp(answer, "exit", 4))
			break;
		else
		{
			char message[20];
			ifstream fin(binary_file_path, ios::binary);
			while (fin.peek() != EOF)
			{
				message[0] = '\0';
				fin.getline(message, 20);
				if (message[0] == '\0' )
					cout << "There are no any new messages from Sender processes..." << endl;
				else if(current_records_num < records_num)
				{
					current_records_num++;
					cout << "Reading message " << current_records_num << "/" << records_num << " from binary file:" << endl;
					cout << message << endl;
				}
			}
			fin.close();
		}
		
		cout << endl;
	}

	cout << "Process Receiver completed work." << endl;

	for (int i = 0; i < senders_amount; i++)
	{
		CloseHandle(Sender_Info[i].hProcess);
		CloseHandle(Sender_Info[i].hThread);
	}

	CloseHandle(SenderSemaphore);

	for (int i = 0; i < senders_amount; i++)
		CloseHandle(EventsFromSender[i]);

	delete[] Sender;
	delete[] Sender_Info;
	delete[] EventsFromSender;

	return 0;
}