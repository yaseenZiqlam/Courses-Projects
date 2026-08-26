#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

const string FileName = "Clients.txt";
const string UsersFileName = "Users.txt";


void GoBackToMainMenue();
void ShowMainMenuScreen();
void ShowManageUsersOption();
void ShowTransactionsMenue();
void ShowAccessDeniedMessage();
void Login();


enum enMainMenueOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6, ManagementOptions = 7,Logout = 8 };
enum enTransactionsMenueOptions { Deposit = 1, WithDraw = 2, TotalBalances = 3, MainMenue = 4 };
enum enUserManagementOptions{ListUsers=1,AddUser=2,DeleteUser=3,UpdateUser=4,FindUsers=5,MainMenues=6};
enum enMainMenuePermissions {eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,	pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64};

bool CheckAccessPermission(enMainMenuePermissions Permission);

float ReadPostiveNumber(const string& Message)
{
	float Number;
	do
	{
		cout << Message << endl;
		cin >> Number;

	} while (Number < 0);
	return Number;
}

float ReadAnyNumber(const string& Message)
{
	float Number;

	cout << Message << endl;
	cin >> Number;

	return Number;
}

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkforDelete = false;
};

struct stUsers {
	string AccountName="";
	string AccountPassword="";
	short AccountPremissions = 0;
	bool MarkForDelete = false;
};

stUsers CurrentUser;

string FromUserRecordToLine(stUsers User,string Delim = "#//#")
{
	string Line = "";

	Line += User.AccountName+Delim;
	Line += User.AccountPassword + Delim;
	Line += to_string(User.AccountPremissions);

	return Line;
}

vector <string> SplitLine(string Line, string Seperator);

stUsers FromLineToUserRecord(string Line,string Delim="#//#")
{
	stUsers User;
	vector <string>vString = SplitLine(Line, Delim);

	User.AccountName = vString[0];
	User.AccountPassword = vString[1];
	User.AccountPremissions = stoi(vString[2]);

	return User;
}

void PrintUserCard(stUsers User)
{
	cout << "\n\nThe following is the extracted User record:";
	cout << "\nAccount Name                  : " << User.AccountName;
	cout << "\nAccount Password              : " << User.AccountPassword;
	cout << "\nAccount Premissions           : " << User.AccountPremissions;
	cout << "\n\n";
}

vector <stUsers> LoadUsersFromFile(string FileNames, string Seperator)
{
	vector <stUsers> vUsers;
	string Line = "";
	fstream MyFile;

	MyFile.open(FileNames, ios::in);

	while (getline(MyFile, Line))
	{
		if (Line != "")
		{
			vUsers.push_back(FromLineToUserRecord(Line));
		}
	}
	MyFile.close();
	return vUsers;
}

void SaveUsersToFile(vector <stUsers>& vUsers,string FileNames,string Delim = "#//#")
{
	fstream MyFile;
	MyFile.open(FileNames, ios::out);
	string Line = "";

	if (MyFile.is_open())
	{
		for (stUsers& U : vUsers)
		{
			if (U.MarkForDelete == false)
			{

				Line = FromUserRecordToLine(U, Delim);

				if (Line != "")
				{
					MyFile << Line << endl;
				}
			}

		}

		MyFile.close();
	}
}

bool SearchByAccountNameAndPassword(stUsers& User, string AccountName,string Password)
{
	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	for (stUsers& N : vUsers)
	{
		if (N.AccountName == AccountName && N.AccountPassword == Password)
		{
			User = N;
			return true;
		}
	}
	return false;


}

bool SearchByAccountName(stUsers& User, string AccountName, vector<stUsers>& vUsers)
{
	for (stUsers& N : vUsers)
	{
		if (N.AccountName == AccountName )
		{
			User = N;
			return true;
		}
	}
	return false;
}

bool MarkUserForDeleteByAccountName(string AccountName, vector<stUsers>& vUsers)
{
	for (stUsers& N : vUsers)
	{
		if (N.AccountName == AccountName)
		{
			N.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteByAccountName(string AccountName,vector<stUsers>& vUsers)
{
	stUsers sUser;
	char Choice = 'n';

	if (AccountName == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;

	}


	if (SearchByAccountName(sUser, AccountName,vUsers))
	{
		PrintUserCard(sUser);


		cout << "Are You Sure You Want to delete It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			MarkUserForDeleteByAccountName(AccountName, vUsers);
			SaveUsersToFile(vUsers, UsersFileName,"#//#");
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Name(" << AccountName << ") is Not Found !";
		return false;
	}
}

stUsers ReadUserPermission(stUsers User)
{
	char Choice = 'n';

	cout << "Do You Want To Give Full Permission?? (Y/N) ";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = enMainMenuePermissions::eAll;
		return User;
	}

	cout << "Is this have accsess to the Client List? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pListClients;
	}

	cout << "Is this user have Premission to Add New Clients ? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pAddNewClient;
	}

	cout << "Is this user have Premission to Delete Clients ? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pDeleteClient;
	}

	cout << "Is this user have Premission to Update Clients ? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pUpdateClients;
	}

	cout << "Is this user have Premission to Find Clients ? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pFindClient;
	}

	cout << "Is this user have Premission to Manage Users ? (Y/N)";
	cin >> Choice;

	if (toupper(Choice) == 'Y')
	{
		User.AccountPremissions = User.AccountPremissions | enMainMenuePermissions::pManageUsers;
	}

	return User;
}

stUsers ChangeUserRecord(string AccountName)
{
	stUsers User;

	User.AccountName = AccountName;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.AccountPassword);

	User = ReadUserPermission(User);

	return User;
}

bool UpdateByAccountName(string AccountName,vector<stUsers>& vUsers)
{
	stUsers sUser;
	char Choice = 'n';

	if (SearchByAccountName(sUser, AccountName,vUsers))
	{
		PrintUserCard(sUser);

		cout << "Are You Sure You Want to Update It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			for (stUsers& N : vUsers)
			{
				if (N.AccountName == AccountName)
				{
					N = ChangeUserRecord(AccountName);
					break;
				}
			}
			SaveUsersToFile(vUsers, UsersFileName, "#//#");
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountName << ") is Not Found !";
		return false;
	}
}

bool IsAccountNamePrimary(string AccountName, vector<stUsers>& vUsers)
{
	for (stUsers& N : vUsers)
	{
		if (N.AccountName == AccountName)
		{
			return true;
		}
	}
	return false;
}

stUsers ReadUserData()
{
	stUsers User;

	cout << "Enter Account Name: ";
	getline(cin >> ws, User.AccountName);

	vector <stUsers>vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	while (IsAccountNamePrimary(User.AccountName, vUsers))
	{
		cout << "\nClient with Account Name(" << User.AccountName << ") is already exiest !";
		cout << "\n\nEnter Account Number: ";
		getline(cin >> ws, User.AccountName);
	}

	cout << "Enter Password : ";
	getline(cin, User.AccountPassword);

	User = ReadUserPermission(User);


	return User;
}

void AddNewUser()
{
	vector <stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	vUsers.push_back(ReadUserData());
	SaveUsersToFile(vUsers, UsersFileName ,"#//#");
}

void AddNewUsers()
{
	char AddMore = 'n';

	do
	{
		AddNewUser();

		cout << "\n\nDo you Want To add New User?? (Y/N)" << endl;
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');


}
void PrintUserRecord(stUsers User)
{
	cout << "| " << setw(40) << left << User.AccountName;
	cout << "| " << setw(10) << left << User.AccountPassword;
	cout << "| " << setw(4) << left << User.AccountPremissions;
}

void PrintAllUsersData(vector <stUsers>& vUsers)
{
	cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(40) << "Account Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(4) << "Account Permissions";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stUsers& User : vUsers)
	{
		PrintUserRecord(User);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

short ReadChoiceNumberInRange(short From, short To)
{
	short Number;
	do
	{
		printf("Enter Your Choice, Number Between %d and %d\n", From, To);
		cin >> Number;

	} while (Number < From || Number >To);
	return Number;
}

string ReadString(const string& Message)
{
	string Line;

	cout << endl << Message << endl;
	getline(cin >> ws, Line);

	return Line;
}

string RecordToLine(stClient sClient, string Seperator)
{
	string Line = "";

	Line += sClient.AccountNumber + Seperator;
	Line += sClient.PinCode + Seperator;
	Line += sClient.Name + Seperator;
	Line += sClient.Phone + Seperator;
	Line += to_string(sClient.AccountBalance);

	return Line;
}

vector <string> SplitLine(string Line, string Seperator)
{
	short pos = 0;
	string sWord = "";
	vector <string> vString;

	while ((pos = short(Line.find(Seperator))) != string::npos)
	{

		sWord = Line.substr(0, pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		Line.erase(0, pos + Seperator.length());
	}
	if (Line != "")
	{
		vString.push_back(Line);
	}
	return vString;
}

stClient LineToRecord(string Line, string Seperator)
{
	vector <string> vString = SplitLine(Line, Seperator);
	stClient Client;

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

void PrintClientRecord1(stClient Client)
{
	cout << "\n\nThe following is the extracted client record:";
	cout << "\nAccout Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n\n";
}

vector <stClient> LoadClientsFromFile(string FileName, string Seperator)
{
	vector <stClient> vClients;
	fstream MyFile;
	string Line;

	MyFile.open(FileName, ios::in);

	while (getline(MyFile, Line))
	{
		if (Line != "")
		{
			vClients.push_back(LineToRecord(Line, Seperator));
		}
	}
	MyFile.close();

	return vClients;
}

void SaveClientToFile(string FileName, string Seperator, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Line = "";

	if (MyFile.is_open())
	{
		for (stClient& C : vClients)
		{
			if (C.MarkforDelete == false)
			{

				Line = RecordToLine(C, Seperator);

				if (Line != "")
				{
					MyFile << Line << endl;
				}
			}

		}

		MyFile.close();
	}
}

bool SearchByAccountNumber(stClient& Client, string AccountNumber, vector<stClient>& vClients)
{

	for (stClient& N : vClients)
	{
		if (N.AccountNumber == AccountNumber)
		{
			Client = N;
			return true;
		}
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& N : vClients)
	{
		if (N.AccountNumber == AccountNumber)
		{
			N.MarkforDelete = true;
			return true;
		}

	}
	return false;
}

bool DeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient sClient;
	char Choice = 'n';

	if (SearchByAccountNumber(sClient, AccountNumber, vClients))
	{
		PrintClientRecord1(sClient);


		cout << "Are You Sure You Want to delete It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientToFile(FileName, "#//#", vClients);
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Not Found !";
		return false;
	}
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient sClient;
	char Choice = 'n';

	if (SearchByAccountNumber(sClient, AccountNumber, vClients))
	{
		PrintClientRecord1(sClient);

		cout << "Are You Sure You Want to Update It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			for (stClient& N : vClients)
			{
				if (N.AccountNumber == AccountNumber)
				{
					N = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientToFile(FileName, "#//#", vClients);
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Not Found !";
		return false;
	}
}

bool IsAccountNumberPrimary(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& N : vClients)
	{
		if (N.AccountNumber == AccountNumber)
		{
			return true;
		}
	}
	return false;
}

stClient ReadClientData()
{
	stClient Client;

	cout << "Enter Account Number: ";
	getline(cin >> ws, Client.AccountNumber);

	vector <stClient>vClients = LoadClientsFromFile(FileName, "#//#");
	while (IsAccountNumberPrimary(Client.AccountNumber, vClients))
	{
		cout << "\nClient with Account Number(" << Client.AccountNumber << ") is already exiest !";
		cout << "\n\nEnter Account Number: ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode : ";
	getline(cin, Client.PinCode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balacne : ";
	cin >> Client.AccountBalance;


	return Client;
}

void AddNewClient()
{
	vector <stClient>vClients = LoadClientsFromFile(FileName, "#//#");
	vClients.push_back(ReadClientData());
	SaveClientToFile(FileName, "#//#", vClients);
}

void AddNewClients()
{
	char AddMore = 'n';

	do
	{
		AddNewClient();

		cout << "\n\nDo you Want To add New Client?? (Y/N)" << endl;
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');


}

void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector <stClient>& vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClient& Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ShowClientDataScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	vector <stClient> vClients = LoadClientsFromFile(FileName, "#//#");
	PrintAllClientsData(vClients);
}

void AddNewClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "----------------------------\n";
	cout << "    Add New Clients Screen  \n";
	cout << "----------------------------\n";

	AddNewClients();
}

void DeleteClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

	cout << "----------------------------\n";
	cout << "    Delete Clients Screen  \n";
	cout << "----------------------------\n";

	vector<stClient> vClients = LoadClientsFromFile(FileName, "#//#");
	DeleteByAccountNumber(ReadString("Enter the Wanted Account Number: "), vClients);

}

void UpdateClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	cout << "----------------------------\n";
	cout << "    Update Client Screen    \n";
	cout << "----------------------------\n";
	vector<stClient> vClients = LoadClientsFromFile(FileName, "#//#");
	UpdateByAccountNumber(ReadString("Enter The wanted Account Number"), vClients);
}

void FindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

	cout << "----------------------------\n";
	cout << "    Find Client Screen    \n";
	cout << "----------------------------\n";

	stClient Client;
	string AccountNumber = ReadString("Enter the Wanted Account Number");
	vector<stClient> vClients = LoadClientsFromFile(FileName, "#//#");

	if (SearchByAccountNumber(Client, AccountNumber, vClients))
	{
		PrintClientRecord1(Client);
	}
	else
		cout << "Account Number Not Found!!" << endl;
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenuScreen();
}

void ExitScreen()
{
	cout << "----------------------------\n";
	cout << "      Program Ends >||<     \n";
	cout << "----------------------------\n";
}

float DepositTransaction(stClient& Client, float Number)
{
	return Client.AccountBalance + Number;
}

bool DepositByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient sClient;
	double Value = 0.0;
	char Choice = 'n';

	if (SearchByAccountNumber(sClient, AccountNumber, vClients))
	{
		PrintClientRecord1(sClient);


		Value = ReadAnyNumber("Please Enter the Amount :");

		cout << "Are You Sure You Want to Update It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			for (stClient& N : vClients)
			{
				if (N.AccountNumber == AccountNumber)
				{
					N.AccountBalance = DepositTransaction(N, Value);
					break;
				}
			}
			SaveClientToFile(FileName, "#//#", vClients);
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Not Found !";
		return false;
	}
}

float WithdrawTransaction(stClient& Client, float Number)
{
	if (Number <= Client.AccountBalance)
	{
		return DepositTransaction(Client, (-1 * Number));
	}
	else
		cout << "The Withdraw Amount Greater than the Account Balance!!" << endl;
}

bool WithdrawByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient sClient;
	double Value = 0.0;
	char Choice = 'n';

	if (SearchByAccountNumber(sClient, AccountNumber, vClients))
	{
		PrintClientRecord1(sClient);


		Value = ReadPostiveNumber("Please Enter Postive Amount :");

		cout << "Are You Sure You Want to Update It (Y/N)" << endl;
		cin >> Choice;

		if (toupper(Choice) == 'Y')
		{
			for (stClient& N : vClients)
			{
				if (N.AccountNumber == AccountNumber)
				{
					N.AccountBalance = WithdrawTransaction(N, Value);
					break;
				}
			}
			SaveClientToFile(FileName, "#//#", vClients);
			return true;

		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Not Found !";
		return false;
	}
}

double TotalBalancesTransaction(vector<stClient>& vClients)
{
	double Total = 0.0;
	for (stClient& N : vClients)
	{
		Total += N.AccountBalance;

	}
	return Total;
}

void ShowDepositScreen()
{
	cout << "----------------------------\n";
	cout << "       Deposit Screen       \n";
	cout << "----------------------------\n";

	vector<stClient>vClients = LoadClientsFromFile(FileName, "#//#");
	DepositByAccountNumber(ReadString("Enter Account Number: "), vClients);

}

void ShowWithdrawScreen()
{
	cout << "----------------------------\n";
	cout << "      Withdraw Screen       \n";
	cout << "----------------------------\n";

	vector<stClient>vClients = LoadClientsFromFile(FileName, "#//#");
	WithdrawByAccountNumber(ReadString("Enter Account Number: "), vClients);
}

void PrintClientRecordBalance(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalancesScreen()
{

	vector <stClient> vClients = LoadClientsFromFile(FileName, "#//#");

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = 0;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientRecordBalance(Client);
			cout << endl;
		}
	}
	TotalBalances = TotalBalancesTransaction(vClients);
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void GoBackToTransactionMenue()
{
	cout << "\n\nPress any key to go back to Transaction Menue...";
	system("pause>0");
	ShowTransactionsMenue();
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions Choice)
{
	switch (Choice)
	{
	case enTransactionsMenueOptions::Deposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenue();
		break;
	}
	case enTransactionsMenueOptions::WithDraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionMenue();
		break;
	}
	case enTransactionsMenueOptions::TotalBalances:
	{
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionMenue();
		break;
	}
	case enTransactionsMenueOptions::MainMenue:
	{
		system("cls");
		ShowMainMenuScreen();
		break;
	}
	}
}

void SystemManagement(enMainMenueOptions Choice)
{
	switch (Choice)
	{
	case enMainMenueOptions::Show:
	{
		system("cls");
		ShowClientDataScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::Add:
	{
		system("cls");
		AddNewClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::Delete:
	{
		system("cls");
		DeleteClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::Find:
	{
		system("cls");
		FindClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::Update:
	{
		system("cls");
		UpdateClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::ManagementOptions:
	{
		system("cls");
		ShowManageUsersOption();
		break;
	}
	case enMainMenueOptions::Transactions:
	{
		system("cls");
		ShowTransactionsMenue();
		break;
	}
	case enMainMenueOptions::Logout:
		system("cls");
		Login();
		break;
	}
}

void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this, \nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}

void ShowUsersListScreen()
{
	vector <stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	PrintAllUsersData(vUsers);
}

void AddNewUsersScreen()
{
	cout << "----------------------------\n";
	cout << "    Add New Users Screen    \n";
	cout << "----------------------------\n";

	AddNewUsers();

}

void DeleteUsersScreen()
{

	cout << "----------------------------\n";
	cout << "    Delete Users Screen     \n";
	cout << "----------------------------\n";
	vector <stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	DeleteByAccountName(ReadString("Enter Account Name: "), vUsers);

}

void UpdateUsersScreen()
{

	cout << "----------------------------\n";
	cout << "    Update Users Screen    \n";
	cout << "----------------------------\n";
	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");
	UpdateByAccountName(ReadString("Enter The wanted Account Number"), vUsers);
}

void FindUsersScreen()
{

	system("cls");
	cout << "----------------------------\n";
	cout << "    Find Users Screen    \n";
	cout << "----------------------------\n";

	stUsers User;
	string AccountName = ReadString("Enter the Wanted Account Name");
	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName, "#//#");

	if (SearchByAccountName(User, AccountName, vUsers))
	{
		PrintUserCard(User);
	}
	else
		cout << "Account Name Not Found!!" << endl;

}

void GoBackToUsersOptions()
{
	system("pause");
	cout << "Press enter to get back to Users Manue......." << endl;
	ShowManageUsersOption();
}

void UserManagmentSystem(enUserManagementOptions Choice)
{
	switch (Choice)
	{
	case enUserManagementOptions::ListUsers:
	{
		system("cls");
		ShowUsersListScreen();
		GoBackToUsersOptions();
		break;
	}
	case enUserManagementOptions::AddUser:
	{
		system("cls");
		AddNewUsersScreen();
		GoBackToUsersOptions();
		break;
	}
	case enUserManagementOptions::DeleteUser:
	{
		system("cls");
		DeleteUsersScreen();
		GoBackToUsersOptions();
		break;
	}
	case enUserManagementOptions::UpdateUser:
	{
		system("cls");
		UpdateUsersScreen();
		GoBackToUsersOptions();
		break;
	}
	case enUserManagementOptions::FindUsers:
	{
		system("cls");
		FindUsersScreen();
		GoBackToUsersOptions();
		break;
	}
	case enUserManagementOptions::MainMenues:
	{
		system("cls");
		ShowMainMenuScreen();
		break;
	}
	}
}

void ShowManageUsersOption()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "           Users Management Screen         \n";
	cout << "===========================================\n";
	cout << "\t[1] Show Users List.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===========================================\n";
	UserManagmentSystem(enUserManagementOptions(ReadChoiceNumberInRange(1, 6)));
}

void ShowTransactionsMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
	PerfromTranactionsMenueOption(enTransactionsMenueOptions(ReadChoiceNumberInRange(1, 4)));
}

void ShowMainMenuScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "              Main Manue Screen            \n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Clients.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";
	SystemManagement(enMainMenueOptions(ReadChoiceNumberInRange(1, 8)));
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentUser.AccountPremissions == enMainMenuePermissions::eAll)
		return true;

	if ((Permission & CurrentUser.AccountPremissions) == Permission)
		return true;
	else
		return false;

}

bool LoadUserInfo(string Username, string Password)
{

	if (SearchByAccountNameAndPassword(CurrentUser,Username,Password))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string Username, Password;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter Username? ";
		cin >> Username;
		cout << "Enter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenuScreen();
}

int main()
{
	Login();
	return 0;
}