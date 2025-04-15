#include <bits/stdc++.h>
using namespace std;
class User
{
private:
    string name;
    int id;

public:
    User(int n, const string &s)
    {
        this->name = s;
        this->id = n;
    }
    string getName()
    {
        return this->name;
    }
    int getId()
    {
        return this->id;
    }
    void printUser()
    {
        cout << "ID: " << this->id << ", Name: " << this->name << endl;
    }
};
class Expense
{
private:
    pair<string, int> PersonWhoPaid;
    int totalPaidAmount;
    vector<pair<string, int>> totalPeopleInvolved;

public:
    // pair<string,int> represents a pair of person with name and its id
    Expense(pair<string, int> p, int t, vector<pair<string, int>> &people)
    {
        this->PersonWhoPaid = p;
        this->totalPaidAmount = t;
        this->totalPeopleInvolved = people;
    }
    void printExpense()
    {
        cout << "Paid by: " << PersonWhoPaid.first
             << " (ID: " << PersonWhoPaid.second << ")"
             << ", Amount: Rs. " << totalPaidAmount << endl;
        cout << "Split among:\n";
        for (auto &p : totalPeopleInvolved)
        {
            cout << "- " << p.first << " (ID: " << p.second << ")\n";
        }
    }
};

class ExpenseManager
{
private:
    vector<User> AllUsers;
    vector<Expense> AllExpenses;
    // first in for id and second for amount
    map<pair<string, int>, int> mp;
    // user m int mtlb id h
    map<string, pair<int, string>> user;
    static int ID;

public:
    void registerNewUser()
    {
        string name, password;
        cout << "Enter the name: " << endl;
        cin >> name;
        cout << "Enter the password: " << endl;
        cin >> password;

        if (user.find(name) == user.end())
        {
            user[name] = {ID, password};
            User newUser(ID, name);
            AllUsers.push_back(newUser);
            mp[{name, ID}] = 0; // initialize balance
            cout << "User is registered Successfully. Proceed with the Login!!" << endl;
            cout << "Name: " << name << " | ID: " << ID << endl;
            ID++;
        }
        else
        {
            cout << "User already registered!!" << endl;
        }
    }

    bool login(string &name, int id, string &pass)
    {
        if (user.find(name) == user.end())
        {
            cout << "User not found" << endl;
            return false;
        }
        else
        {
            if (user[name].first != id || user[name].second != pass)
            {
                cout << "Incorrect ID or Password" << endl;
                return false;
            }
            else
            {
                cout << "User Logged in Successfully" << endl;
                return true;
            }
        }
    }
    void logout(bool &logged)
    {
        if (logged)
        {
            logged = false;
            cout << "Logged out successfully!!" << endl;
        }
        else
        {
            cout << "User is not logged In" << endl;
        }
    }

    void updateBalances(pair<string, int> payer, int totalAmount, vector<pair<string, int>> &people)
    {
        int numPeople = people.size() + 1;
        int share = totalAmount / numPeople;

        for (auto &person : people)
        {
            mp[person] -= share; // each owes this much
            mp[payer] += share;  // payer should get this much
        }
    }

    void addNewExpense(pair<string, int> p, int t, vector<pair<string, int>> &people)
    {
        Expense E(p, t, people);
        AllExpenses.push_back(E);

        updateBalances(p, t, people);
    }
    void printAllUsers()
    {
        for (int i = 0; i < AllUsers.size(); i++)
        {
            AllUsers[i].printUser();
        }
    }
    void printAllExpenses()
    {
        for (int i = 0; i < AllExpenses.size(); i++)
        {
            AllExpenses[i].printExpense();
        }
    }
    void showBalances()
    {
        cout << "\n--- User Balances ---\n";

        for (auto &it : mp)
        {
            string name = it.first.first;
            int id = it.first.second;
            int balance = it.second;

            if (balance == 0)
            {
                cout << name << " (ID: " << id << ") has settled balance" << endl;
            }
            if (balance > 0)
            {
                cout << name << " (ID: " << id << ") will receive Rs. " << balance << endl;
            }
            else
            {
                cout << name << " (ID: " << id << ") owes Rs. " << -balance << endl;
            }
        }
    }
    void settleExpenses()
    {
        queue<pair<pair<string, int>, int>> creditors;
        queue<pair<pair<string, int>, int>> debtors;
        for (auto it : mp)
        {
            if (it.second > 0)
            {
                creditors.push({it.first, it.second});
            }
            else if (it.second < 0)
            {
                debtors.push({it.first, it.second});
            }
        }
        while (!creditors.empty() && !debtors.empty())
        {
            pair<pair<string, int>, int> deb = debtors.front();
            debtors.pop();
            pair<pair<string, int>, int> cred = creditors.front();
            creditors.pop();
            int amountToPay = min(abs(cred.second), abs(deb.second));

            cred.second -= abs(amountToPay);
            deb.second += abs(amountToPay);
            cout << deb.first.first << " pays " << amountToPay << "to " << cred.first.first << endl;
            mp[deb.first] = deb.second;
            mp[cred.first] = cred.second;
            if (cred.second > 0)
            {
                creditors.push(cred);
            }
            if (deb.second < 0)
            {
                debtors.push(deb);
            }
        }
    }
    void balanceSummaryPerUser()
    {
        for (auto it : mp)
        {
            if (it.second > 0)
            {
                cout << "Person with name: " << it.first.first << " and id " << it.first.second << " needs to receive an amount of Rs. " << abs(it.second) << endl;
            }
            else if (it.second < 0)
            {
                cout << "Person with name: " << it.first.first << " and id " << it.first.second << " needs to pay an amount of Rs. " << abs(it.second) << endl;
            }
            else
            {
                cout << "Person with name: " << it.first.first << " and id " << it.first.second << " has settled balance." << endl;
            }
        }
    }

    void partialSettlement(string name2, int id2, int amt, string cUN, int cUI)
    {
        if (user.find(name2) == user.end())
        {
            cout << "No such person found" << endl;
            return;
        }
        if (mp[{name2, id2}] > 0 && mp[{cUN, cUI}] < 0)
        {
            int balanceTosettle = min(amt, mp[{name2, id2}]);
            mp[{name2, id2}] -= abs(balanceTosettle);
            mp[{cUN, cUI}] += abs(balanceTosettle);
            cout << "Person " << name2 << "with id " << id2 << "gave " << balanceTosettle << " amount to person " << cUN << "with id " << cUI << endl;
        }
        else if (mp[{name2, id2}] < 0 && mp[{cUN, cUI}] > 0)
        {
            int balanceTosettle = min(amt, mp[{cUN, cUI}]);
            mp[{name2, id2}] += abs(balanceTosettle);
            mp[{cUN, cUI}] -= abs(balanceTosettle);
            cout << "Person " << cUN << "with id " << cUI << "gave " << balanceTosettle << " amount to person " << name2 << "with id " << id2 << endl;
        }
        else
        {
            cout << " No valid transaction possible between these two users!" << endl;
        }
    }
};
int ExpenseManager::ID = 1;

int main()
{
    int choice;
    string currentUserName;
    int currentUserId;
    bool isLoggedIn = false;
    ExpenseManager manager;
    while (true)
    {
        cout << "1. Register 2.Login 3.Add Expense 4.Show All Users 5.Show All Expenses 6. Show Balances 7. Settle All Expenses 8.Settle with Specific User (Partial Settlement) 9.Show Balance Summary Per User 10.Logout 11.Exit" << endl;
        cout << "Enter your choice: " << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            manager.registerNewUser();

            break;
        }
        case 2:
        {
            string name, pass;
            int id;
            cout << "Enter your name: " << endl;
            cin >> name;
            cout << "Enter the id: " << endl;
            cin >> id;
            cout << "Enter the password:" << endl;
            cin >> pass;
            isLoggedIn = manager.login(name, id, pass);
            if (isLoggedIn)
            {
                currentUserName = name;
                currentUserId = id;
            }
            break;
        }
        case 3:
        {
            if (isLoggedIn)
            {
                pair<string, int> payer;
                int total;
                int n;
                pair<string, int> person;
                vector<pair<string, int>> people;
                cout << "Enter the payer name and the id of that person : " << endl;
                cin >> payer.first >> payer.second;
                cout << "Enter the total amount" << endl;
                cin >> total;
                cout << "Enter the total number of people who all are required to give the share" << endl;
                cin >> n;
                for (int i = 0; i < n; i++)
                {
                    cout << "Enter the name of person " << i << endl;
                    cin >> person.first;
                    cout << "Enter the id of the person " << i << endl;
                    cin >> person.second;
                    people.push_back(person);
                }
                manager.addNewExpense(payer, total, people);
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 4:
        {
            if (isLoggedIn)
            {
                manager.printAllUsers();
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 5:
        {
            if (isLoggedIn)
            {
                manager.printAllExpenses();
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 6:
        {
            if (isLoggedIn)
            {
                manager.showBalances();
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 7:
        {
            if (isLoggedIn)
            {
                manager.settleExpenses();
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 8:
        {
            if (isLoggedIn)
            {
                string person;
                int id, amt;
                cout << "Enter the name of the person you want to settle the amount with:" << endl;
                cin >> person;
                cout << "Enter the id of the person you want to settle the amount with:" << endl;
                cin >> id;
                cout << "Enter the amt you want to settle" << endl;
                cin >> amt;

                manager.partialSettlement(person, id, amt, currentUserName, currentUserId);
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 9:
        {
            if (isLoggedIn)
            {
                manager.balanceSummaryPerUser();
                break;
            }
            else
            {
                cout << "Login or Register!!" << endl;
            }
            break;
        }
        case 10:
        {
            manager.logout(isLoggedIn);
            break;
        }

        case 11:
            return 0;
        default:
            cout << "Invalid Choice...Try again!!" << endl;
        }
    }

    return 0;
}
