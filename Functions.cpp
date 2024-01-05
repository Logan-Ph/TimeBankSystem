#include "Functions.h"
#include "Admin.h"
#include "User.h"
#include "Member.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <chrono>

std::vector<User *> readUsers(std::string fileName)
{
    std::vector<User *> users;                                    // vector of pointers to User objects
    std::ifstream ifs(fileName, std::ios::binary | std::ios::in); // open file for reading in binary mode
    size_t size;                                                  // size of the string
    int userType;                                                 // 1 for Admin, 2 for Member

    while (ifs.read(reinterpret_cast<char *>(&userType), sizeof(userType))) // read the user type
    {
        User *user;
        if (userType == 1)
        {
            user = new Admin();
        }
        else
        {
            user = new Member();
        }
        user->read(ifs);
        users.push_back(user);
    }

    if (ifs.fail() && !ifs.eof())
    {
        std::cerr << "Error occurred before reaching end of file" << std::endl;
    }

    ifs.close();
    return users;
}

bool writeUsers(std::vector<User *> users, std::string fileName)
{
    try
    {
        std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
        for (auto it = users.begin(); it != users.end(); ++it)
        {
            User *user = *it;                                              // this is a pointer of pointer to a User object
            int userType = dynamic_cast<Admin *>(user) != nullptr ? 1 : 2; // cast to Admin or Member
            ofs.write(reinterpret_cast<const char *>(&userType), sizeof(userType));
            user->write(ofs);
        }
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "Error writing to file" << std::endl;
        return false;
    }
}

User *findUserByUserName(std::string userName, std::vector<User *> users)
{
    auto it = std::find_if(users.begin(), users.end(), [userName](User *user)
                           { return user->getUserName() == userName; });
    if (it != users.end())
    {
        return *it;
    }

    // Handle the situation when the user is not found
    return nullptr;
}

User *findUserById(std::string id, std::vector<User *> users)
{
    auto it = std::find_if(users.begin(), users.end(), [id](User *user)
                           { return user->getId() == id; });
    if (it != users.end())
    {
        return *it;
    }

    // Handle the situation when the user is not found
    return nullptr;
}

std::time_t getTime()
{
    std::string input;
    std::cout << "Enter date and time in the following format: YYYY-MM-DD HH:MM:SS" << std::endl;
    std::getline(std::cin, input);
    std::tm t = {};
    std::stringstream ss(input);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    if (ss.fail())
    {
        std::cout << "Failed to parse date and time.\n";
        return std::time_t(-1);
    }
    return std::mktime(&t);
}

std::time_t getCurrentTime()
{
    return std::time(nullptr);
}

std::string formatTime(std::time_t time)
{
    std::tm *localTime = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}