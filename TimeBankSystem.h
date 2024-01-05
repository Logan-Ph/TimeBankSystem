#ifndef _TIMEBANKSYSTEM_H_
#define _TIMEBANKSYSTEM_H_

#include <string>
#include <vector>
#include "Admin.h"
#include "Member.h"
#include "User.h"

class TimeBankSystem
{
private:
    static std::vector<User *> users;

public:
    TimeBankSystem();
    static std::vector<User *> getUsers();
    static void listUsersInfo();
    User *loginUser();
    void run();
    bool registerUser();
    void loginUI();
    void adminDisplay(Admin *admin);
    void memberDisplay(Member *member);
    void guestDisplay();
};

#endif
