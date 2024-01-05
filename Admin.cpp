#include "Admin.h"
#include "Functions.h"
#include "TimeBankSystem.h"
#include <string>
#include <iostream>

Admin::Admin() = default;

Admin::~Admin() = default;

Admin::Admin(std::string userName, std::string password) : User(userName, password){};

bool Admin::resetPassword()
{
    std::string id;
    TimeBankSystem::listUsersInfo(); // list all the users info mation for admin
    std::cout << "Please enter the id of the user you want to reset password: ";
    std::getline(std::cin, id); // get id from admin
    User *user = findUserById(id, TimeBankSystem::getUsers()); // find user by id
    if (user == nullptr) // check if the user not found
    {
        std::cout << "User not found" << std::endl;
        std::cout << "Reset user password failed" << std::endl;
        return false;
    }
    std::cout << "Please enter a new password for this account: ";
    std::string newPassword;
    std::getline(std::cin, newPassword);
    user->setPassword(newPassword);
    std::cout << "Reset user successful" << std::endl;
    return true;
}
