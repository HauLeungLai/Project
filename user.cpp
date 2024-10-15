#include "user.h"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include "portfolio.h"

extern Portfolio portfolio;

void registerUser(std::map<std::string, std::string>& users) {
    clear();
    printw("Enter a username: ");
    char user[30];
    echo();
    getnstr(user, 30);
    std::string username = user;
    noecho();

    if (users.find(username) != users.end()) {
        printw("Username already exists.\n");
        getch();
        return;
    }

    printw("Enter a password: ");
    char pass[30];
    echo();
    getnstr(pass, 30);
    std::string password = pass;
    noecho();

    users[username] = password;
    saveUsersToFile(users, "users.dat");
    printw("\nRegistration complete.\n");
    getch();
}

bool loginUser(const std::map<std::string, std::string>& users, std::string& currentUser) {
    clear();
    printw("Enter username: ");
    char username[30];
    echo();
    getnstr(username, 30);
    noecho();

    printw("Enter password: ");
    char password[30];
    echo();
    getnstr(password, 30);
    noecho();

    std::string user = username;
    std::string pass = password;

    if (users.find(user) != users.end() && users.at(user) == pass) {
        currentUser = user;
        portfolio.loadPortfolio(currentUser + "_portfolio.dat");  // Load user's portfolio
        printw("\nLogin successful!\n");
        getch();
        return true;
    } else {
        printw("\nInvalid username or password.\n");
        getch();
        return false;
    }
}

void loadUsersFromFile(std::map<std::string, std::string>& users) {
    std::ifstream infile("users.dat");
    std::string username, password;
    while (infile >> username >> password) {
        users[username] = password;
    }
}

void saveUsersToFile(const std::map<std::string, std::string>& users, const std::string& filename) {
    std::ofstream outfile(filename);
    for (const auto& user : users) {
        outfile << user.first << " " << user.second << std::endl;
    }
}
