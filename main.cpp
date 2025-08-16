                                    // BANK MANAGEMENT SYSTEM
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include <bits/stdc++.h>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <algorithm>


using namespace std;

// Function to generate account number
int account_generator(MYSQL* conn, const string& email) {
    stringstream ss;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;
    ss << "SELECT account_no,email FROM users";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if (qstate == 0) {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res))) {
            if (row[1] == email) {
                return atoi(row[0]);
            }
        }
    }
    return 0;
}
// Function to validate account number
bool account_validator(MYSQL* conn, int acc) {
    stringstream ss;
    int qstate = 0;
    MYSQL_RES* res;
    MYSQL_ROW row;
    ss << "SELECT account_no FROM users";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if (qstate == 0) {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res))) {
            int acc_num = atoi(row[0]);
            if (acc_num == acc) {
                return true;
            }
        }
    }
    return false;
}

//Function to create an account
void createaccount(MYSQL* conn)
{
    stringstream ss;
    string namedb,address;
    string email,pass;
    char phonenum[20];
    int qstate = 0,balance;

    cout<<"\nEnter Your Name: ";
    cin.ignore();
    getline(cin,namedb);

    cout<<"\nEnter Mobile Number: "<<endl;
    cin>>phonenum;


    cout<<"\nEnter Your Address: ";
    string text;
    cin>> text;

    cout<<"\nEnter Amount To Deposit: "<<endl;
    cin>>balance;
    if(balance < 0) balance *= -1;

    int flag_email = 0;
    cout<<"\nEnter Email Address: "<<endl;
    cin >> email;

    transform(email.begin(), email.end(), email.begin(), ::tolower);

    cout<<"\nEnter Password:";
       cin>>pass;

    ss << "INSERT INTO users (name, phoneno, address, balance, email, password) VALUES ('" << namedb << "', '" << phonenum <<"', '" << address <<"', "<< balance <<", '"<< email <<"', '"<< pass <<"')";

    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if(qstate == 0)
        cout<<"Account Created Successfully"<<endl;
}

int loginaccount(MYSQL* conn, const string& emaildb, const string& passdb)
{
  stringstream ss;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int qstate = 0;
  int flag = 0;
  ss << "SELECT account_no,email,password FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if(qstate == 0){
    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res))){
        if((strcmp(row[1], emaildb.c_str()) == 0) && (strcmp(row[2], passdb.c_str()) == 0))
        {
            flag = 1;
            cout<<"Login Successful"<<endl;
            char *ch = row[0];
            return atoi(ch);
        }
    }
    if(flag == 0)
    {
        cout<<"Invalid Username/password"<<endl;
        return 0;
    }
      }
  return 0;
}

void display_details(MYSQL* conn,int acc)
{
  stringstream ss;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int qstate;
  ss << "SELECT account_no, name FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if(qstate == 0){
    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res))){
        char *ch = row[0];
        if(atoi(ch) == acc)
        {
            cout<<"\t\t\t\t<<<-----------------WELCOME "<<row[1] <<"------------------>>>"<<endl;
            cout<<"\t \t \t \t \t \t Your ACCOUNT NUMBER: "<<atoi(ch)<<endl;
            cout<<"\t \t \t \tFirst create a new pin, If your account is new"<<endl;
        }
    }

  }
}
int pin_generate(MYSQL* conn, int acc, int pin)
{
    stringstream ss;
    int qstate = 0;
    MYSQL_RES* res;
    MYSQL_ROW row;
    int p,p1,flag = 0;

    ss<<"SELECT pin,account_no FROM mpin";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    int flag1 = 0;
    if(qstate == 0)
    {
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)))
        {
        char *ch = row[0];
        p = atoi(ch);
        char *ch1 = row[1];
        p1 = atoi(ch1);
        if(p1 == acc)
        {
          flag1 = 1;
          if(p == pin){flag = 1;return 1;}
          else {flag = 1;return 2;};
        }
        }
        if(flag1 == 0) return 0;
        if(flag == 0) return 3;
    }
    return 0;
}

int create_pin(MYSQL* conn,int acc)
{
      stringstream ss;
    int qstate = 0;
    int pin,flag = 0;
    int p;

    while(flag == 0)
    {
        cout<<"Enter a 4 digit pin: ";
        cin>>pin;
        if((pin >= 1000) && (pin <= 9999))
        {
            flag = 1;
            continue;
        }
        else
        cout<<"\nEnter Valid Input"<<endl;

    }
    p = pin_generate(conn,acc,pin);
    if(p == 0)
    {
        ss<<"INSERT INTO mpin (pin, account_no) VALUES ("<< pin <<", "<< acc <<")";
        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);
        if(qstate == 0 && flag == 1)
        {
            return 1;
        }
    }
    else cout<<"Pin already exists"<<acc<<endl;
    return 0;
}

int bal_validate(MYSQL* conn, int acc, int transfer)
{
    stringstream ss;
    MYSQL_ROW row;
       MYSQL_RES* res;
    int qstate = 0;

    ss<<"select balance from users where account_no = "<< acc <<"";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    if(qstate == 0)
    {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        const char* ch = row[0];
        int bal = atoi(ch);
        if(bal >= transfer)
            return 1;
        else
            return 2;
    }
    else
    {
        cout<<"Balance query error: "<<mysql_error(conn)<<endl;
        return 0;
    }

}

void transfer_money(MYSQL* conn, int acc)
{
  stringstream ss,ss1,ss2,ss3;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int recipient_amt = 0,sender_amt = 0,transfer_amt,account_num;
  int qstate = 0,qstate1 = 0,qstate2 = 0,qstate3 = 0;
  int p = 0;

   cout<<"Enter Account Number of the recipient: ";
  cin>>account_num;
  int acc_val = account_validator(conn,account_num);
  if(acc_val == 0)
    {
        cout<<"Recipient account does not exist"<<endl;
        return;
    }

  cout<<"Enter Amount to transfer: ";
  cin>>transfer_amt;

  int bal_valid = bal_validate(conn,acc,transfer_amt);
  if(bal_valid == 2)
  {
      cout<<"Insufficient Balance"<<endl;
      return;
  }
  sender_amt = sender_amt - transfer_amt;
  recipient_amt = recipient_amt + transfer_amt;
  cout<<"enter pin: "<<endl;
  cin>>p;

  int spin = pin_generate(conn,acc,p);
  if(spin == 2){cout<<"Wrong PIN number"<<endl;return;}
  else if((spin == 3) || (spin == 0)){cout<<"PIN Not Exists for this account number."<<endl;return;}


  ss<<"SELECT account_no,balance FROM users";
  string query = ss.str();
  const char* q = query.c_str();
    qstate = mysql_query(conn, q);

  if(spin == 1 && acc_val != 0)
 {
  if(qstate == 0){
    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res)))
    {
        char *ch = row[0];
        int ac = atoi(ch);
        char *ch1 = row[1];
        int bala = atoi(ch1);
        if(acc == ac)
        {
            if(bala > transfer_amt)
            {
              sender_amt = bala - transfer_amt;
              ss1 << "UPDATE users SET balance = "<< sender_amt <<" WHERE account_no = "<< acc <<"";
              string query = ss1.str();
              const char* q1 = query.c_str();
              qstate1 = mysql_query(conn, q1);
              if(qstate1 == 0)
              {
                  cout<<"Amount Of Rupees "<<transfer_amt<<" Transfered Successfully to Account Number: "<<account_num<<endl;
              }
            }
            else
            {
                cout<<"Insufficient Balance......"<<endl;
            }
        }
        char *ch2 = row[0];
        int ac1 = atoi(ch2);
        char *ch3 = row[1];
        int bala1 = atoi(ch3);
             if(account_num == ac1)
        {
              recipient_amt = bala1 + transfer_amt;
              ss2 << "UPDATE users SET balance = "<< recipient_amt <<" WHERE account_no = "<< account_num <<"";
              string query2 = ss2.str();
              const char* q2 = query2.c_str();
              qstate2 = mysql_query(conn, q2);
              if(qstate2 == 0)
              {
                  cout<<"Amount Of Rupees "<<transfer_amt<<" Transfered Successfully From Account Number: "<<acc<<endl;
              }
              else
              {
                  cout<<"Transfer Error at recipient: "<<mysql_error(conn);
              }
        }
      }
      time_t now = time(0);
      char* dt = ctime(&now);
      string d = dt;
      ss3<<"INSERT INTO transfer(date, account_no, balance, transamt, to_acc_no) VALUES ('"<< d <<"', "<< acc <<", "<< sender_amt <<", "<< transfer_amt <<", "<< account_num <<")";
      string query3 = ss3.str();
      const char* q3 = query3.c_str();
      qstate3 = mysql_query(conn, q3);
      if(qstate3 == 0)
      {
          cout<<"\nTransaction Successful without any discrepancies....."<<endl;
          cout<<"Current balance in your account is: "<<sender_amt<<endl;
      }
      else cout<<"Transfer Error is: "<<mysql_error(conn)<<endl;
   }
    else cout<<"query Error is: "<<mysql_error(conn)<<endl;
  }
  else cout<<"Pin not created"<<endl;
}

void deposit(MYSQL* conn, int acc_no)
{
    int qstate = 0,qstate1 = 0,qstate2 = 0;
    stringstream ss,ss1,ss2;
    int amount;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int p = 0;

    cout<<"Enter Amount to deposit in your account: ";
    cin>>amount;

    cout<<"enter pin: "<<endl;
    cin>>p;

    int spin = pin_generate(conn,acc_no,p);
    if((spin == 3) || (spin == 0))
    {
        cout<<"PIN Not Exists for this account number."<<endl;
        return;
    }

    if(spin == 1)
    {
        ss1<<"select balance FROM users WHERE account_no = "<< acc_no <<"";
        string query1 = ss1.str();
        const char *q1 = query1.c_str();
        qstate1 = mysql_query(conn,q1);
        if(qstate1 == 0)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            char *ch = row[0];
            int bal = atoi(ch);
            bal = bal + amount;

            time_t now = time(0);
            char* dt = ctime(&now);
                   string d = dt;

            ss2<<"UPDATE users SET balance = "<< bal <<" WHERE account_no = "<< acc_no <<"";
            string query2 = ss2.str();
            const char *q2 = query2.c_str();
            qstate2 = mysql_query(conn,q2);
            if(qstate2 == 0)
            {
                cout<<"Updated"<<endl;
            }
            ss<<"INSERT INTO deposit (date, account_no, balance, transamt) VALUES ('"<< d <<"', "<< acc_no <<", "<< bal <<","<< amount <<")";
            string query = ss.str();
            const char *q = query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate == 0)
            {
                cout<<"Amount of rupees "<<amount<<" has been deposited into account "<<acc_no<<endl;
                cout<<"Current balance in your account is: "<<bal<<endl;
            }
        }
    }
    else cout<<"Pin not created"<<endl;
}



void withdraw(MYSQL* conn, int acc_no)
{
    int qstate = 0,qstate1 = 0,qstate2 = 0;
    stringstream ss,ss1,ss2;
    int amount;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int p = 0;
      cout<<"Enter Amount to withdraw: ";
    cin>>amount;
    int bal_valid = bal_validate(conn,acc_no,amount);
    if(bal_valid == 2)
    {
      cout<<"Not enough balance"<<endl;
      return;
    }


    cout<<"enter pin: "<<endl;
    cin>>p;

    int spin = pin_generate(conn,acc_no,p);
    if(spin == 1)
    {
        ss1<<"select balance FROM users WHERE account_no = "<< acc_no <<"";
        string query1 = ss1.str();
        const char *q1 = query1.c_str();
        qstate1 = mysql_query(conn,q1);
        if(qstate1 == 0)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            char *ch = row[0];
            int bal = atoi(ch);
            bal = bal - amount;

            time_t now = time(0);
            char* dt = ctime(&now);
            string d = dt;

            ss2<<"UPDATE users SET balance = "<< bal <<" WHERE account_no = "<< acc_no <<"";
            string query2 = ss2.str();
            const char *q2 = query2.c_str();
                    qstate2 = mysql_query(conn,q2);
            if(qstate2 == 0)
            {
                cout<<"Balance Updated..."<<endl;
            }
            else cout<<"Balance Not Updated Error is : "<<mysql_error(conn)<<endl;

            ss<<"INSERT INTO withdraw (date, account_no, balance, transamt) VALUES ('"<< d <<"', "<< acc_no <<", "<< bal <<","<< amount <<")";
            string query = ss.str();
            const char *q = query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate == 0)
            {
                cout<<"Amount of rupees "<<amount<<" has been debited from account "<<acc_no<<" Successfully"<<endl;
                cout<<"Current balance in your account is: "<<bal<<endl;
            }
            else cout<<"Withdraw error occurred: "<<mysql_error(conn);

        }
        else{cout<<"Account Does not exist... error is: "<<mysql_error(conn)<<endl;return;}
    }
    else cout<<"Pin not created"<<endl;
}
void show_transaction(MYSQL* conn, int acc_no)
{
    stringstream ss,ss1,ss2;
    MYSQL_ROW row,row1,row2;
    MYSQL_RES *res,*res1,*res2;
    int qstate = 0,qstate1 = 0,qstate2 = 0;

    ss<<"SELECT sno, date, account_no, transamt, to_acc_no FROM transfer WHERE account_no = "<< acc_no <<"";
    string query = ss.str();
    const char *q = query.c_str();
    qstate = mysql_query(conn,q);
    if(qstate == 0)
    {
        cout<<"\t\t\t\t-----------------TRANSACTION DETAILS------------------"<<endl;
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)))
        {
        char *ch = row[0];
        char *ch1 = row[2];
        char *ch2 = row[3];
        char *ch3 = row[4];
        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row[1] <<"From Account Number: " << atoi(ch1) << endl;
        cout<<"To Account Number: "<< atoi(ch3) << endl <<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Transaction details Error: "<<mysql_error(conn)<<endl;

    ss1<<"SELECT sno, date, account_no, transamt FROM deposit WHERE account_no = "<< acc_no <<"";
    string query1 = ss1.str();
    const char *q1 = query1.c_str();
    qstate1 = mysql_query(conn,q1);
    if(qstate1 == 0)
    {
        cout<<"\t\t\t\t-------------------DEPOSIT DETAILS---------------------"<<endl;
        res1 = mysql_store_result(conn);
        while((row1 = mysql_fetch_row(res1)))
        {
        char *ch = row1[0];
        char *ch1 = row1[2];
        char *ch2 = row1[3];

        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row1[1] << "Account Number: " << atoi(ch1) << endl;
        cout<<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Deposit details Error: "<<mysql_error(conn)<<endl;

    ss2<<"SELECT sno, date, account_no, transamt FROM withdraw WHERE account_no = "<< acc_no <<"";
    string query2 = ss2.str();
    const char *q2 = query2.c_str();
    qstate2 = mysql_query(conn,q2);
    if(qstate2 == 0)
    {
        cout<<"\t\t\t\t-------------------WITHDRAW DETAILS--------------------"<<endl;
        res2 = mysql_store_result(conn);
        while((row2 = mysql_fetch_row(res2)))
        {
        char *ch = row2[0];
        char *ch1 = row2[2];
        char *ch2 = row2[3];

        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row2[1] << "Account Number: " << atoi(ch1) << endl;
        cout<<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Deposit details Error: "<<mysql_error(conn)<<endl;

}

int main()
{
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "Payal@09", "bankmanagement", 3306, NULL, 0);
    int acc_no = 0,pin = 0;
    string email,pass;
    system("Color E0");
    cout<<"\t\t\t=================================================================="<<endl;
    cout<<"\t\t\t                        WELCOME TO BANK                           "<<endl;
    cout<<"\t\t\t=================================================================="<<endl;

    if(conn){
        char ch;
        cout<<"Database Connected Successfully"<<endl;
        do
        {
            cout<<"Enter\n\n\t 1 - Create Account\n\t 2 - Login\n\t 3 - Transfer\n\t 4 - Deposit\n\t 5 - Withdraw\n\t 6 - create pin\n\t 7 - show Transaction Details\n\t 0 - exit"<<endl;
            cin>>ch;
            system("cls");
            switch(ch)
            {
            case '1':
                createaccount(conn);
                break;
            case '2':
                 cout<<"Enter email address: ";
                 cin.ignore();
                 getline(cin,email);
                 transform(email.begin(), email.end(), email.begin(), ::tolower);

                 cout<<"Enter password: ";
                 getline(cin,pass);

                 acc_no = loginaccount(conn,email,pass);
                 if(acc_no != 0) display_details(conn,acc_no);
                 else cout<<"Please Login..."<<endl;
                 break;
            case '3':
                if(acc_no!=0) transfer_money(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '4':
                if(acc_no!=0) deposit(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '5':
                if(acc_no!=0) withdraw(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '6':
                if(acc_no!=0){
                  pin = create_pin(conn,acc_no);
                  if(!pin) cout<<"Pin Not created"<<endl;
                  else if(pin == 1) cout<<"Pin Created Successfully"<<endl;
                  else continue;
                }
                else cout<<"Please Login..."<<endl;
                break;
            case '7':
                 if(acc_no!=0) show_transaction(conn,acc_no);
                 else cout<<"Please Login..."<<endl;
                 break;
            case '0':
                cout<<"Thank You!!!!!"<<endl;
                break;
            default:
                cout<<"Please Enter a valid input"<<endl;
                break;
            }
        }while(ch != '0');
    }
    else
    {
        cout << "Database Connection Error: " << mysql_error(conn) << endl;
    }
    return 0;
}


