#include "User.h"
#include "Functions.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

User::User() = default;

User::User(std::string _userName, std::string _password)
{
    id = generateId<User>();
    userName = _userName;
    password = _password;
};

User::~User() = default;

std::string User::getUserName() { return userName; };

std::string User::getPassword() { return password; };

std::string User::getId() { return id; };

void User::setUserName(std::string _userName) { userName = _userName; };

void User::setPassword(std::string _password) { password = _password; };

void User::setId(std::string _id) { id = _id; };

void User::read(std::ifstream &ifs)
{
    setUserName(readData<std::string>(ifs));
    setPassword(readData<std::string>(ifs));
    setId(readData<std::string>(ifs));
}

void User::write(std::ofstream &ofs)
{
    writeData<std::string>(ofs, getUserName());
    writeData<std::string>(ofs, getPassword());
    writeData<std::string>(ofs, getId());
}

std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "id: " << user.id;
    os << " username: " << user.userName;
    os << " password: " << user.password;
    return os;
}
