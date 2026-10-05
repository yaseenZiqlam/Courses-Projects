# -*- coding: utf-8 -*-
import os
from dataclasses import dataclass
from enum import IntEnum

FileName = "Clients.txt"
Seperator = "#//#"

@dataclass
class stClient:
    AccountNumber:str
    PinCode:str
    Name:str
    Phone:str
    AccountBalance:float
    MarkForDelete:bool = False

class enMainMenueOptions(IntEnum):
    Show = 1
    Add = 2
    Delete = 3
    Update = 4
    Find = 5
    Transactions = 6
    Exit = 7  
    
class enTransactionsMenueOptions(IntEnum):
    Deposit = 1
    WithDraw = 2
    TotalBalances = 3
    MainMenue = 4

def ReadString(Prompt: str) -> str:
    return input(Prompt)
 
 
def ReadAnyNumber(Prompt: str) -> float:
    while True:
        try:
            return float(input(Prompt))
        except ValueError:
            print("Please enter a valid number.")
 
 
def ReadPostiveNumber(Prompt: str) -> float:
    while True:
        Value = ReadAnyNumber(Prompt)
        if Value > 0:
            return Value
        print("Please enter a positive number.")
 
 
def ReadChoiceNumberInRange(From: int, To: int) -> int:
    while True:
        try:
            Choice = int(input(f"Choose [{From} to {To}]? "))
            if From <= Choice <= To:
                return Choice
        except ValueError:
            pass
        print(f"Please enter a number between {From} and {To}.")
 
 
def Clear():
    os.system("cls" if os.name == "nt" else "clear")
 
 
def Pause():
    input("\n\nPress Enter to go back...")


def ReadPerson():
    print("\n")
    Client = stClient(

        input("Enter Your Account Number: "),
        input("Enter Your Pin Code: "),
        input("Enter Your Name: "),
        input("Enter Your Phone: "),
        float(input("Enter Your : Account Balance"))
    )
    return Client
 

def RecordToLine(Client:stClient,Seperator:str):
    
    Line = ""
    
    Line += Client.AccountNumber + Seperator
    Line += Client.PinCode + Seperator
    Line += Client.Name + Seperator
    Line += Client.Phone + Seperator
    Line += str(Client.AccountBalance) 
    
    return Line  

def LineToRecord(Line:str , Delim:str):
    
    ClientInfoList = Line.split(Delim)   
    return stClient(ClientInfoList[0], ClientInfoList[1], ClientInfoList[2], ClientInfoList[3], float(ClientInfoList[4]))
        
    
def PrintClientCard(stClient:stClient):
    
    print("\n\nThe following is the extracted Info from Client:\n")
    print(f"\nAccount Number: {stClient.AccountNumber}\n")
    print(f"Pin Code: {stClient.PinCode}\n")
    print(f"Name: {stClient.Name}\n")
    print(f"Phone:{stClient.Phone}\n")
    print(f"Account Balance: {stClient.AccountBalance}\n")

    
def LoadClientsFromFile(FileName:str,Seperator:str):

    Clients =[]
    if not os.path.exists(FileName):
        return Clients
    
    with open(FileName,"r") as file:
        for Line in file:

            Line = Line.strip()

            if Line:
              Clients.append(LineToRecord(Line.strip(),Seperator))    

    return Clients    
  
def SaveClientsToFile(ClientRecords:list,FileName:str,Seperator:str):
    
    with open(FileName,"w") as file:
        for Client in ClientRecords:
            if(not Client.MarkForDelete):
                file.write(RecordToLine(Client, Seperator)+"\n")
             
                
def SearchByAccountNumber( AccountNumber:str,ClientsRecords:list):

    for Client in ClientsRecords:
        if(Client.AccountNumber==AccountNumber):
            return Client
    return None      

def MarkClientDeleteByAccountNumber(AccountNumber:str,ClientsRecords:list):

    for Client in ClientsRecords:

        if(Client.AccountNumber == AccountNumber):

            Client.MarkForDelete = True
            return True
    return False

def DeleteByAccountNumber(AccountNumber:str, ClientsRecords:list):

    Client = SearchByAccountNumber(AccountNumber, ClientsRecords)
    if Client is None:
        print(f"\nClient with Account Number ( {AccountNumber} ) is Not Found !")
        return False

    PrintClientCard(Client)
    Choice = input("Are you sure for deleting the record?? ")

    if Choice.upper() == 'Y':
        MarkClientDeleteByAccountNumber(AccountNumber, ClientsRecords)
        SaveClientsToFile(ClientsRecords, FileName, Seperator)
        return True
    return False

def ChangeClientRecord(AccountNumber:str):
    return stClient(
        AccountNumber,
        input("Enter PinCode : "),
        input("Enter Name : "),
        input("Enter Phone : "),
        float(input("Account Balance : "))
    )

def UpdateByAccountNumber(AccountNumber:str, ClientsRecords:list):

    Client = SearchByAccountNumber(AccountNumber, ClientsRecords)
    if Client is None:
        print(f"\nClient with Account Number ( {AccountNumber} ) is Not Found!!")
        return False

    PrintClientCard(Client)
    Choice = input("Are You Sure You Want to Update It ? (Y/N)")

    if Choice.upper() == 'Y':
        Updated = ChangeClientRecord(AccountNumber)
        Client.PinCode = Updated.PinCode
        Client.Name = Updated.Name
        Client.Phone = Updated.Phone
        Client.AccountBalance = Updated.AccountBalance
        SaveClientsToFile(ClientsRecords, FileName, Seperator)
        return True
    return False

def IsAccountNumberPrimary(AccountNumber:str,ClientsRecords:str):

    for C in ClientsRecords:

        if(C.AccountNumber == AccountNumber):
            return True
    return False

def ReadClientData():
    Client = stClient(AccountNumber="", PinCode="", Name="", Phone="", AccountBalance=0.0)
 
    Client.AccountNumber = input("Enter Account Number: ")
 
    vClients = LoadClientsFromFile(FileName, Seperator)
    while IsAccountNumberPrimary(Client.AccountNumber, vClients):
        print(f"\nClient with Account Number({Client.AccountNumber}) is already exist !")
        Client.AccountNumber = input("\nEnter Account Number: ")
 
    Client.PinCode = input("Enter PinCode : ")
    Client.Name = input("Enter Name: ")
    Client.Phone = input("Enter Phone: ")
    Client.AccountBalance = float(input("Enter Account Balacne : "))
 
    return Client
 
 
def AddNewClient():
    vClients = LoadClientsFromFile(FileName, Seperator)
    vClients.append(ReadClientData())
    SaveClientsToFile(vClients, FileName, Seperator)
 
 
def AddNewClients():
    while True:
        AddNewClient()
        AddMore = input("\n\nDo you Want To add New Client?? (Y/N)\n")
        if AddMore.upper() != 'Y':
            break
 
 
def PrintClientRecord(Client: stClient):
    print(f"| {Client.AccountNumber:<15}", end="")
    print(f"| {Client.PinCode:<10}", end="")
    print(f"| {Client.Name:<40}", end="")
    print(f"| {Client.Phone:<12}", end="")
    print(f"| {Client.AccountBalance:<12}", end="")
 
 
def PrintAllClientsData(vClients: list):
    print(f"\n\t\t\t\t\tClient List ({len(vClients)}) Client(s).")
    print("\n_______________________________________________________"
          "_________________________________________\n")
    print(f"| {'Accout Number':<15}", end="")
    print(f"| {'Pin Code':<10}", end="")
    print(f"| {'Client Name':<40}", end="")
    print(f"| {'Phone':<12}", end="")
    print(f"| {'Balance':<12}")
    print("\n_______________________________________________________"
          "_________________________________________\n")
    for Client in vClients:
        PrintClientRecord(Client)
        print()
    print("\n_______________________________________________________"
          "_________________________________________\n")
 
 
def ShowClientDataScreen():
    vClients = LoadClientsFromFile(FileName, Seperator)
    PrintAllClientsData(vClients)
 
 
def AddNewClientsScreen():
    Clear()
    print("----------------------------")
    print("    Add New Clients Screen  ")
    print("----------------------------")
    AddNewClients()
 
 
def DeleteClientsScreen():
    Clear()
    print("----------------------------")
    print("    Delete Clients Screen  ")
    print("----------------------------")
    vClients = LoadClientsFromFile(FileName, Seperator)
    DeleteByAccountNumber(ReadString("Enter the Wanted Account Number: "), vClients)
 
 
def UpdateClientScreen():
    Clear()
    print("----------------------------")
    print("    Update Client Screen    ")
    print("----------------------------")
    vClients = LoadClientsFromFile(FileName, Seperator)
    UpdateByAccountNumber(ReadString("Enter The wanted Account Number"), vClients)
 
 
def FindClientScreen():
    Clear()
    print("----------------------------")
    print("    Find Client Screen    ")
    print("----------------------------")
 
    AccountNumber = ReadString("Enter the Wanted Account Number")
    vClients = LoadClientsFromFile(FileName, Seperator)
 
    Client = SearchByAccountNumber(AccountNumber, vClients)
    if Client is not None:
        PrintClientRecord(Client)
    else:
        print("Account Number Not Found!!")
 
 
def GoBackToMainMenue():
    Pause()
    ShowMainMenuScreen()
 
 
def ExitScreen():
    print("----------------------------")
    print("      Program Ends >||<     ")
    print("----------------------------")
 
 

 
def DepositTransaction(Client: stClient, Number: float):
    return Client.AccountBalance + Number
 
 
def DepositByAccountNumber(AccountNumber: str, vClients: list):
    sClient = SearchByAccountNumber(AccountNumber, vClients)
    if sClient is not None:
        PrintClientRecord(sClient)
 
        Value = ReadAnyNumber("Please Enter the Amount :")
        Choice = input("Are You Sure You Want to Update It (Y/N)\n")
 
        if Choice.upper() == 'Y':
            for N in vClients:
                if N.AccountNumber == AccountNumber:
                    N.AccountBalance = DepositTransaction(N, Value)
                    break
            SaveClientsToFile(vClients, FileName, Seperator)
            return True
        return False
    else:
        print(f"\nClient with Account Number({AccountNumber}) is Not Found !")
        return False
 
 
def WithdrawTransaction(Client: stClient, Number: float):
    if Number <= Client.AccountBalance:
        return DepositTransaction(Client, -1 * Number)
    print("The Withdraw Amount Greater than the Account Balance!!")
    return Client.AccountBalance
 
 
def WithdrawByAccountNumber(AccountNumber: str, vClients: list):
    sClient = SearchByAccountNumber(AccountNumber, vClients)
    if sClient is not None:
        PrintClientRecord(sClient)
 
        Value = ReadPostiveNumber("Please Enter Postive Amount :")
        Choice = input("Are You Sure You Want to Update It (Y/N)\n")
 
        if Choice.upper() == 'Y':
            for N in vClients:
                if N.AccountNumber == AccountNumber:
                    N.AccountBalance = WithdrawTransaction(N, Value)
                    break
            SaveClientsToFile(vClients, FileName, Seperator)
            return True
        return False
    else:
        print(f"\nClient with Account Number({AccountNumber}) is Not Found !")
        return False
 
 
def TotalBalancesTransaction(vClients: list):
    Total = 0.0
    for N in vClients:
        Total += N.AccountBalance
    return Total
 
 
def ShowDepositScreen():
    print("----------------------------")
    print("       Deposit Screen       ")
    print("----------------------------")
    vClients = LoadClientsFromFile(FileName, Seperator)
    DepositByAccountNumber(ReadString("Enter Account Number: "), vClients)
 
 
def ShowWithdrawScreen():
    print("----------------------------")
    print("      Withdraw Screen       ")
    print("----------------------------")
    vClients = LoadClientsFromFile(FileName, Seperator)
    WithdrawByAccountNumber(ReadString("Enter Account Number: "), vClients)
 
 
def PrintClientRecordBalance(Client: stClient):
    print(f"| {Client.AccountNumber:<15}", end="")
    print(f"| {Client.Name:<40}", end="")
    print(f"| {Client.AccountBalance:<12}", end="")
 
 
def ShowTotalBalancesScreen():
    vClients = LoadClientsFromFile(FileName, Seperator)
 
    print(f"\n\t\t\t\t\tBalances List ({len(vClients)}) Client(s).")
    print("\n_______________________________________________________"
          "_________________________________________\n")
    print(f"| {'Account Number':<15}", end="")
    print(f"| {'Client Name':<40}", end="")
    print(f"| {'Balance':<12}")
    print("\n_______________________________________________________"
          "_________________________________________\n")
 
    if len(vClients) == 0:
        print("\t\t\t\tNo Clients Available In the System!")
    else:
        for Client in vClients:
            PrintClientRecordBalance(Client)
            print()
 
    TotalBalances = TotalBalancesTransaction(vClients)
    print("\n_______________________________________________________"
          "_________________________________________\n")
    print(f"\t\t\t\t\t   Total Balances = {TotalBalances}")
 
 
def GoBackToTransactionMenue():
    Pause()
    ShowTransactionsMenue()
 
 
def PerfromTranactionsMenueOption(Choice: enTransactionsMenueOptions):
    if Choice == enTransactionsMenueOptions.Deposit:
        Clear()
        ShowDepositScreen()
        GoBackToTransactionMenue()
    elif Choice == enTransactionsMenueOptions.WithDraw:
        Clear()
        ShowWithdrawScreen()
        GoBackToTransactionMenue()
    elif Choice == enTransactionsMenueOptions.TotalBalances:
        Clear()
        ShowTotalBalancesScreen()
        GoBackToTransactionMenue()
    elif Choice == enTransactionsMenueOptions.MainMenue:
        Clear()
        ShowMainMenuScreen()
 
 
def SystemManagement(Choice: enMainMenueOptions):
    if Choice == enMainMenueOptions.Show:
        Clear()
        ShowClientDataScreen()
        GoBackToMainMenue()
    elif Choice == enMainMenueOptions.Add:
        Clear()
        AddNewClientsScreen()
        GoBackToMainMenue()
    elif Choice == enMainMenueOptions.Delete:
        Clear()
        DeleteClientsScreen()
        GoBackToMainMenue()
    elif Choice == enMainMenueOptions.Find:
        Clear()
        FindClientScreen()
        GoBackToMainMenue()
    elif Choice == enMainMenueOptions.Update:
        Clear()
        UpdateClientScreen()
        GoBackToMainMenue()
    elif Choice == enMainMenueOptions.Exit:
        Clear()
        ExitScreen()
    elif Choice == enMainMenueOptions.Transactions:
        Clear()
        ShowTransactionsMenue()
 
 
def ShowTransactionsMenue():
    Clear()
    print("===========================================")
    print("\t\tTransactions Menue Screen")
    print("===========================================")
    print("\t[1] Deposit.")
    print("\t[2] Withdraw.")
    print("\t[3] Total Balances.")
    print("\t[4] Main Menue.")
    print("===========================================")
    PerfromTranactionsMenueOption(enTransactionsMenueOptions(ReadChoiceNumberInRange(1, 4)))
 
 
def ShowMainMenuScreen():
    Clear()
    print("===========================================")
    print("              Main Manue Screen            ")
    print("===========================================")
    print("\t[1] Show Client List.")
    print("\t[2] Add New Clients.")
    print("\t[3] Delete Client.")
    print("\t[4] Update Client Info.")
    print("\t[5] Find Client.")
    print("\t[6] Transactions.")
    print("\t[7] Exit.")
    print("===========================================")
    SystemManagement(enMainMenueOptions(ReadChoiceNumberInRange(1, 7)))
 
 
def main():
    ShowMainMenuScreen()
 
 
if __name__ == "__main__":
    main()
