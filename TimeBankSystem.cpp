#include "TimeBankSystem.h"
#include "Functions.h"
#include "Admin.h"
#include "Member.h"
#include "User.h"
#include <iostream>
#include <algorithm>
#include <vector>

std::vector<User *> TimeBankSystem::users = std::vector<User *>();

TimeBankSystem::TimeBankSystem()
{
    users = readUsers("users.bin");
}; // constructor

void TimeBankSystem::run()
{
    std::cout << "##################################" << std::endl;
    std::cout << "#                                #" << std::endl;
    std::cout << "#  EEET2482/COSC2082 ASSIGNMENT  #" << std::endl;
    std::cout << "#    \"TIME BANK\" APPLICATION     #"
              << std::endl;
    std::cout << "#                                #" << std::endl;
    std::cout << "# Instructor: Mr. Tran Duc Linh  #" << std::endl;
    std::cout << "# Group 4:                       #" << std::endl;
    std::cout << "# s3978798 Tran Pham Khanh Doan  #" << std::endl;
    std::cout << "# s3975979 Pham Phuoc Sang       #" << std::endl;
    std::cout << "# s3978486 Nguyen Ngoc Thanh Mai #" << std::endl;
    std::cout << "# s3978387 Cao Nguyen Hai Linh   #" << std::endl;
    std::cout << "##################################\n"
              << std::endl;
    loginUI();
}

std::vector<User *> TimeBankSystem::getUsers()
{
    return users; // return the users of the system
}

User *TimeBankSystem::loginUser()
{
    std::string userName, password;
    std::cout << "Please enter your username: ";
    std::getline(std::cin, userName); // get username
    std::cout << "Please enter your password: ";
    std::getline(std::cin, password); // get password
    User *user = findUserByUserName(userName, users);
    if (user != nullptr && user->getPassword() == password) // compare the user password
    {
        return user;
    }
    else // login failed
    {
        return nullptr;
    }
}

bool TimeBankSystem::registerUser()
{
    std::string userName, password;
    std::cout << "Please enter your username: ";
    std::getline(std::cin, userName); // get username
    std::cout << "Please enter your password: ";
    std::getline(std::cin, password); // get password
    User *user = findUserByUserName(userName, users);
    if (user == nullptr) // check if the user not found
    {
        user = new Member(userName, password);
        users.push_back(user);
        std::cout << "User registered successfully" << std::endl;
        return true;
    }
    else // register failed
    {
        std::cout << "User already exists" << std::endl;
        return false;
    }
}

void TimeBankSystem::listUsersInfo()
{
    for (auto *user : users)
    {
        if (dynamic_cast<Admin *>(user) != nullptr)
        {
            continue;
        }
        Member *member = dynamic_cast<Member *>(user);
        std::cout << "----------------------------------" << std::endl;
        std::cout << "ID: " << member->getId() << std::endl;
        std::cout << "Username: " << member->getUserName() << std::endl;
        std::cout << "Full name: " << (member->getUserInfo())["fullName"] << std::endl;
        std::cout << "Phone number: " << (member->getUserInfo())["phoneNumber"] << std::endl;
        std::cout << "Email: " << (member->getUserInfo())["email"] << std::endl;
        std::cout << "City: " << (member->getUserInfo())["city"] << std::endl;
        std::cout << "Skills: " << std::endl;
        for (auto skill : member->getSkills())
        {
            if (!skill.empty())
            {
                std::cout << "\t" << skill << std::endl;
            }
        }
        std::cout << "----------------------------------" << std::endl;
    }
}

void TimeBankSystem::loginUI()
{
    Member *member = nullptr;
    Admin *admin = nullptr;
    std::string choice;
    while (choice != "x")
    {
        std::cout << "Use the app as " << std::endl;
        std::cout << "1. Guest" << std::endl;
        std::cout << "2. Member" << std::endl;
        std::cout << "3. Admin" << std::endl;
        std::cout << "x. Exit" << std::endl;
        std::cout << "Please enter your choice: ";
        std::getline(std::cin, choice);
        if (choice.length() != 1)
        {
            std::cout << std::endl
                      << "Please enter a valid choice" << std::endl;
            continue;
        }
        switch (choice.at(0))
        {
        case '1':
            guestDisplay();
            break;
        case '2':
            member = dynamic_cast<Member *>(loginUser());
            if (member != nullptr)
            {
                std::cout << "Login successful" << std::endl;
                memberDisplay(member);
                break;
            }
            std::cout << "Login failed" << std::endl;
            break;
        case '3':
            admin = dynamic_cast<Admin *>(loginUser());
            if (admin != nullptr)
            {
                std::cout << "Login successful" << std::endl;
                adminDisplay(admin);
                break;
            }
            std::cout << "Login failed" << std::endl;
            break;
        case 'x':
            writeUsers(getUsers(), "users.bin");
            break;
        default:
            break;
        }
    }
}

void TimeBankSystem::guestDisplay()
{
    std::string choice;
    while (choice != "x")
    {
        std::cout << "\n----- Guest Display -----" << std::endl;
        std::cout << "1. Register" << std::endl;
        std::cout << "2. View supporters" << std::endl;
        std::cout << "x. Exit" << std::endl;
        std::cout << "Please enter your choice: ";
        std::getline(std::cin, choice);
        std::cout << std::endl;
        if (choice.length() != 1)
        {
            std::cout << std::endl
                      << "Please enter a valid choice" << std::endl;
            continue;
        }
        switch (choice.at(0))
        {
        case '1':
            registerUser();
            break;
        case '2':
            listUsersInfo();
            break;
        case 'x':
        default:
            break;
        }
    }
}

void TimeBankSystem::adminDisplay(Admin *admin)
{
    std::string choice;
    while (choice != "x")
    {
        std::cout << "\n----- Adnmin Display -----" << std::endl;
        std::cout << "1. List Users" << std::endl;
        std::cout << "2. Reset user password" << std::endl;
        std::cout << "x. Exit" << std::endl;
        std::cout << "Please enter your choice: ";
        std::getline(std::cin, choice);
        std::cout << std::endl;
        if (choice.length() != 1)
        {
            std::cout << std::endl
                      << "Please enter a valid choice" << std::endl;
            continue;
        }
        switch (choice.at(0))
        {
        case '1':
            listUsersInfo();
            break;
        case '2':
            admin->resetPassword();
            break;
        case 'x':
        default:
            break;
        }
    }
}

void TimeBankSystem::memberDisplay(Member *member)
{

    std::string choice;
    while (choice != "x")
    {
        try
        {
            std::cout << "\n----- Member Display -----" << std::endl;
            std::cout << "1. List yourself" << std::endl;
            std::cout << "2. Unlist yourself" << std::endl;
            std::cout << "3. Book a service" << std::endl;
            std::cout << "4. Confirm activity" << std::endl;
            std::cout << "5. Search services" << std::endl;
            std::cout << "6. View your profile" << std::endl;
            std::cout << "7. Feedback on previous activity" << std::endl;
            std::cout << "8. Add more credit points" << std::endl;
            std::cout << "9. Block members" << std::endl;
            std::cout << "10. Unblock members" << std::endl;
            std::cout << "11. View your pending request" << std::endl;
            std::cout << "12. View your previous activity" << std::endl;
            std::cout << "13. View other user profile" << std::endl;
            std::cout << "14. Cancel request" << std::endl;
            std::cout << "x. Exit" << std::endl;
            std::cout << "Please enter your choice: ";
            std::getline(std::cin, choice);
            std::cout << std::endl;
            if (choice == "x")
            {
                break;
            }
            switch (std::stoi(choice))
            {
            case 1:
                if (member->listYourSelf())
                {
                    std::cout << "List yourself successful" << std::endl;
                }
                else
                {
                    std::cout << "List yourself failed" << std::endl;
                }
                break;
            case 2:
                for (Activity *activity : member->getActivities())
                {
                    if (activity->getHostId().empty())
                    {
                        std::cout << *activity << std::endl;
                    }
                }
                member->unListYourSelf();
                break;
            case 3:
                member->bookService();
                break;
            case 4:
                member->confirmActivity();
                break;
            case 5:
                member->searchServices();
                break;
            case 6:
                member->viewProfile();
                break;
            case 7:
                member->feedbackActivity();
                break;
            case 8:
                member->addCreditPoints();
                break;
            case 9:
                member->blockMember();
                break;
            case 10:
                member->unBlockMember();
                break;
            case 11:
                member->viewPendingRequest();
                break;
            case 12:
                member->viewPreviousAcitivity();
                break;
            case 13:
                member->viewOtherUserProfile();
                break;
            case 14:
                member->cancelRequest();
                break;
            default:
                std::cout << "Please enter a valid choice" << std::endl;
                break;
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Please enter a valid choice" << std::endl;
            continue;
        }
    }
}