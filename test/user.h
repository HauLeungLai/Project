#ifndef USER_H
#define USER_H

#include <string>
#include <map>

void registerUser(std::map<std::string, std::string>& users);
bool loginUser(const std::map<std::string, std::string>& users, std::string& currentUser);
void loadUsersFromFile(std::map<std::string, std::string>& users);
void saveUsersToFile(const std::map<std::string, std::string>& users, const std::string& filename);

#endif
