#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
using namespace std;
void ShowMainMenu();
const string ClientFile = "ClientsList.txt";
enum enMainMenuOptions { ShowClient = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, EmptyList=6, Exit = 7 };
struct stClients {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkClient = false;
};


//show client screen :
vector<string>SplitString(string Txt, string Delim) {
	vector<string>vWords;
	short Pos = 0;
	string word = "";
	while ((Pos = Txt.find(Delim)) != std::string::npos) {
		word = Txt.substr(0, Pos);
		if (word != "") {
			vWords.push_back(word);
		}
		Txt.erase(0, Pos + Delim.length());
	}
	if (Txt != "") {
		vWords.push_back(Txt);
	}
	return vWords;
}
stClients ConvertLineToRecord(string Line, string sperator = "#//#") {
	vector<string>vLine = SplitString(Line, sperator);
	stClients client;
	client.AccountNumber = vLine.at(0);
	client.PinCode = vLine.at(1);
	client.Name = vLine.at(2);
	client.Phone = vLine.at(3);
	client.AccountBalance = stod(vLine.at(4));

	return client;
}
vector<stClients> LoadClientsDataFromFile(string file) {
	vector<stClients>vClients;
	fstream Myfile;
	stClients client;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string Line;
		while (getline(Myfile, Line)) {
			client = ConvertLineToRecord(Line);
			vClients.push_back(client);
		}
		Myfile.close();
	}
	return vClients;
}
void PrintClientForShowClientScreen(stClients& clt) {
	cout << "| " << left << setw(15) << clt.AccountNumber;
	cout << "| " << left << setw(12) << clt.PinCode;
	cout << "| " << left << setw(30) << clt.Name;
	cout << "| " << left << setw(15) << clt.Phone;
	cout << "| " << left << setw(13) << to_string(clt.AccountBalance);
}
void ShowClientListScreen() {
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	cout << "\n\t\t\t\t Client List " << vClient.size() << " Client(s)\n";
	cout << "\n______________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(12) << "Pin Code";
	cout << "| " << left << setw(30) << "           Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(13) << "Account Balance";
	cout << "\n______________________________________________________________________________________________________\n\n";

	if (vClient.size() == 0) {
		cout << "\n\t\t\t\t No Client Available In The System!";
	}
	else

		for (stClients& clt : vClient) {
			PrintClientForShowClientScreen(clt);
			cout << endl;
		}
	cout << "\n\n______________________________________________________________________________________________________\n";

}


//Add client screen :
bool IsAccountNumberExist(string AccountNumber, string file) {
	vector<stClients>vClient;
	fstream Myfile;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string line;
		stClients client;
		while (getline(Myfile, line)) {
			client = ConvertLineToRecord(line);
			if (client.AccountNumber == AccountNumber) {
				Myfile.close();
				return true;
			}
			vClient.push_back(client);
		}
		Myfile.close();
	}
	return false;
}
stClients GetClientInfo() {
	stClients  client;
	cout << "Enter Account Number? ";
	getline(cin >> ws, client.AccountNumber);
	while (IsAccountNumberExist(client.AccountNumber, ClientFile)) {
		cout << "\nClient with [" << client.AccountNumber << "] already exists, Enter another Account Number ";
		getline(cin >> ws, client.AccountNumber);
	}
	cout << "Enter Pin Code : ";
	getline(cin >> ws, client.PinCode);

	cout << "Enter Name : ";
	getline(cin >> ws, client.Name);

	cout << "Enter Phone : ";
	getline(cin >> ws, client.Phone);

	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;

	return client;

}
string ConvertRecordToLine(stClients& clt, string sperator = "#//#") {
	string Line;
	Line += clt.AccountNumber + sperator;
	Line += clt.PinCode + sperator;
	Line += clt.Name + sperator;
	Line += clt.Phone + sperator;
	Line += to_string(clt.AccountBalance);
	return Line;
}
void AddDataLineToFile(string file, string line) {
	fstream Myfile;
	Myfile.open(file, ios::out | ios::app);
	if (Myfile.is_open()) {
		Myfile << line << endl;
		Myfile.close();
	}
}
void AddOneClient() {
	stClients client;
	client = GetClientInfo();
	AddDataLineToFile(ClientFile, ConvertRecordToLine(client));
}
void AddClientsScreen() {
	char AddMore = 'Y';

	do {
		system("cls");
		cout << "\n________________________________________________\n";
		cout << "\t\t Add Client Screen.\n";
		cout << "_________________________________________________\n\n";
		cout << "Adding Client :\n\n";
		AddOneClient();
		cout << "\nClient Added Successfully, do you want to Add more? (Y/N) ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

//Delete client screen :
string AccountNumber() {
	cout << "Please enter Account Number? ";
	string AccountNum = "";
	cin >> AccountNum;
	return AccountNum;
}
void PrintClientCardForEdit(stClients clt) {
	cout << "\nThe Following are Client Details : \n";
	cout << "\nAccount Number : " << clt.AccountNumber << endl;
	cout << "Pin Code : " << clt.PinCode << endl;
	cout << "Name : " << clt.Name << endl;
	cout << "Phone : " << clt.Phone << endl;
	cout << "Account Balance : " << clt.AccountBalance << endl;
}
bool IsAccountNumberInVector(string AccountNum, vector<stClients>& vClient, stClients& client) {
	for (stClients& clt : vClient) {
		if (AccountNum == clt.AccountNumber) {
			client = clt;
			return true;
		}
	}
	return false;
}
bool MarkAccountForDeleteByAccountNumber(string AccountNum, vector<stClients>& vClient) {
	for (stClients& clt : vClient) {
		if (AccountNum == clt.AccountNumber) {
			clt.MarkClient = true;
			return true;
		}
	}
	return false;
}
void SaveClientDataInFileAgain(vector<stClients>& vClient, string file) {
	fstream Myfile;
	Myfile.open(file, ios::out);
	if (Myfile.is_open()) {
		string Line;
		for (stClients& clt : vClient) {
			if (clt.MarkClient == false) {
				Line = ConvertRecordToLine(clt);
				Myfile << Line << endl;
			}

		}
		Myfile.close();
	}


}
void DeleteAccountByAccountNumber(string ReadAccount, vector<stClients>& vClient) {
	stClients client;

	if (IsAccountNumberInVector(ReadAccount, vClient, client)) {
		PrintClientCardForEdit(client);
		char Delete = 'Y';
		cout << "\nAre you sure you want to delete this Account? (Y/N) ";
		cin >> Delete;
		if (toupper(Delete) == 'Y') {
			MarkAccountForDeleteByAccountNumber(ReadAccount, vClient);
			SaveClientDataInFileAgain(vClient, ClientFile);
			vClient = LoadClientsDataFromFile(ClientFile);
			cout << "\nClient Deleted Successfully.";

		}

	}
	else {
		cout << "The Account with N: " << ReadAccount << " Doesn't Exist! ";
	}
}
void DeleteClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Delete Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	DeleteAccountByAccountNumber(ReadAccount, vClient);
}


//Update client screen :

stClients MakeAnUpdateOnAccount(string AccountNum) {
	stClients client;
	client.AccountNumber = AccountNum;
	cout << "\nEnter Pin Code : ";
	getline(cin >> ws, client.PinCode);
	cout << "Enter Name : ";
	getline(cin >> ws, client.Name);
	cout << "Enter Phone : ";
	getline(cin >> ws, client.Phone);
	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;
}
bool UpdateClientByAccountNumber(string AcctNum, vector < stClients >& vClient) {
	stClients client;
	if (IsAccountNumberInVector(AcctNum, vClient, client)) {
		PrintClientCardForEdit(client);
		char Update = 'Y';
		cout << "\nAre you sure you want to Update this account? (Y/N) ";
		cin >> Update;
		if (toupper(Update) == 'Y') {
			for (stClients& clt : vClient) {
				if (clt.AccountNumber == AcctNum) {
					clt = MakeAnUpdateOnAccount(AcctNum);
				}
			}

			SaveClientDataInFileAgain(vClient, ClientFile);
			vClient = LoadClientsDataFromFile(ClientFile);
			cout << "\nClient Updated Successfully.\n";
			return true;
		}
	}
	else {
		cout << "The Account with N: " << AcctNum << " Not Found!\n";
	}
	return false;
}
void UpdateClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Update Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	UpdateClientByAccountNumber(ReadAccount, vClient);
}

//Find client screen :
bool FindClientByAccountNumber(string AcctNum, vector < stClients >& vClient) {

	stClients client;
	if (IsAccountNumberInVector(AcctNum, vClient, client)) {
		PrintClientCardForEdit(client);
		return true;
	}
	else {
		cout << "The Account with N: " << AcctNum << " Not Found!\n";
	}
	return false;
}
void FindClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Find Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	FindClientByAccountNumber(ReadAccount, vClient);
}

//Empty client :
char AskUserIfDeteteAllClients() {
	cout << "Do You want to Delete all Clients? Y/N ";
	char Ask;
	cin >> Ask;
	return Ask;
}
bool RemoveAllClients(char RemoveClient, vector<stClients>& vclient) {
	if (toupper(RemoveClient) == 'Y') {
		if (vclient.size() == 0) {
			cout << "\n Client List is Empty! \n";
			return false;
		}
		else {
			while (!vclient.empty()) {
				vclient.pop_back();
			}
			SaveClientDataInFileAgain(vclient, ClientFile);
			cout << "\nClient Were Removed Successfully.\n";
			vclient = LoadClientsDataFromFile(ClientFile);
			return true;
		}

	}
	else {
		return false;
	}
	return false;
}
void EmptyClientsFromListScreen() {

	cout << "\n________________________________________________\n\n";
	cout << "\t\t Remove All Clients.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	char AskUser = AskUserIfDeteteAllClients();
	RemoveAllClients(AskUser, vClient);
}

//Exit client screen :
void ExitMenu() {
	cout << "\n---------------------------------------\n";
	cout << "\t Program Ends :-)\n";
	cout << "---------------------------------------\n";
}





void GoBackToMainMenu() {
	cout << "\n\nPress any Key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu();
}
short ReadMainMenuOptions() {
	short Choice = 0;
	cout << "Choose what do you want to do? [1-7] ";
	cin >> Choice;
	return Choice;
}
void PerformeMainMenuOptions(enMainMenuOptions MainMenuOptions) {
	switch (MainMenuOptions) {
	case enMainMenuOptions::ShowClient:
	{
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::AddClient:
	{
		system("cls");
		AddClientsScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::DeleteClient:
	{
		system("cls");
		DeleteClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::UpdateClient:
	{
		system("cls");
		UpdateClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::FindClient:
	{
		system("cls");
		FindClientScreen();
		GoBackToMainMenu();
		break;
	}case enMainMenuOptions::EmptyList:
	{
		system("cls");
		EmptyClientsFromListScreen();
		GoBackToMainMenu();
		break;
	}

	case enMainMenuOptions::Exit:
	{
		system("cls");
		ExitMenu();
		break;
	}

	}

}

void Menu() {
	cout << "================================================\n";
	cout << "\t\t Main Menue Screen\n";
	cout << "================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Empty The Client From List.\n";
	cout << "\t[7] Exit.\n";
	cout << "================================================\n";
}
void ShowMainMenu() {
	system("cls");
	Menu();
	PerformeMainMenuOptions((enMainMenuOptions)ReadMainMenuOptions());
}


int main()
{
	ShowMainMenu();


	system("pause>0");
	return 0;
}


