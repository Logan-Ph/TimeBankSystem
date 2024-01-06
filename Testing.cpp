
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // ------------------ This file for creating dummy user ----------------------------------------------------// // //
// // ------------------ To run this file you need to comment all of the "main.cpp" file ----------------------// // //
// // ------------------ Then uncomment all the followings below this section ---------------------------------// // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

#if 0
#include <iostream>
#include "User.h"
#include "Functions.h"
#include "Admin.h"
#include "Member.h"
#include <vector>

int main()
{
    Admin *admin = new Admin("admin", "123");
    Member *member1 = new Member("user1", "123");
    Member *member2 = new Member("user2", "123");
    Member *member3 = new Member("user3", "123");
    Member *member4 = new Member("user4", "123");
    std::vector<User *> users = {admin, member1, member2, member3, member4};
    writeUsers(users, "users.bin");

    std::vector<User *> newUsers = readUsers("users.bin");
    for (User *user : newUsers)
    {

        if (dynamic_cast<Admin *>(user) != nullptr)
        {
            std::cout << "Admin: ";
        }
        else if (dynamic_cast<Member *>(user) != nullptr)
        {
            std::cout << "Member: ";
        }
        std::cout << *user << std::endl;
    }
}
#endif
