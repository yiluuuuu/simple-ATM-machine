IF Bank Management System
A simple console-based banking system built with C++ that supports both customer and banker operations with file persistence.

Features
Customer Functions
Login with account holder name and password

Transfer money to other accounts

View account details

Banker Functions
Create new accounts (auto-generates 5-digit account number)

Delete existing accounts

Deposit money

Withdraw money (5% fee on withdrawals)

Transfer between accounts

View any account details

Requirements
C++ compiler (C++11 or later)

How to Run
bash
g++ main.cpp -o bank
./bank
File Storage
Account data is automatically saved to accounts.txt in the format:

text
accountNumber name balance password
