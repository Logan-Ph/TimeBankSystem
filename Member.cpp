// remember whenever reading the data from file, you need to remove all the null pointer of the vector after that

#include "Member.h"
#include "TimeBankSystem.h"
#include "Functions.h"
#include "RatingScore.h"
#include <algorithm>
#include <set>
#include <ctime>
#include <unordered_map>

Member::Member()
{
    creditPoints = 20;
    userInfo = std::map<std::string, std::string>();
    blockedMembers = std::vector<std::string>();
    skills = std::vector<std::string>();
    skillRatingScore = new RatingScore();
    hostRatingScore = new RatingScore();
    supportRatingScore = new RatingScore();
    activities = std::vector<Activity *>();
    comments = std::vector<std::string>();
}

Member::~Member() = default;

void Member::read(std::ifstream &ifs)
{
    setUserName(readData<std::string>(ifs));
    setPassword(readData<std::string>(ifs));
    setId(readData<std::string>(ifs));
    skillRatingScore->read(ifs);
    hostRatingScore->read(ifs);
    supportRatingScore->read(ifs);
    setCreaditPoints(readData<int>(ifs));
    userInfo["fullName"] = readData<std::string>(ifs);
    userInfo["phoneNumber"] = readData<std::string>(ifs);
    userInfo["email"] = readData<std::string>(ifs);
    userInfo["address"] = readData<std::string>(ifs);
    skills = readData<std::vector<std::string>>(ifs);
    blockedMembers = readData<std::vector<std::string>>(ifs);
    activities = readVector<Activity>(ifs);
    activities.erase(std::remove(activities.begin(), activities.end(), nullptr), activities.end()); // remove all the null pointer before write to file
    comments = readData<std::vector<std::string>>(ifs);
}

void Member::write(std::ofstream &ofs)
{
    writeData<std::string>(ofs, getUserName());
    writeData<std::string>(ofs, getPassword());
    writeData<std::string>(ofs, getId());
    skillRatingScore->write(ofs);
    hostRatingScore->write(ofs);
    supportRatingScore->write(ofs);
    writeData<int>(ofs, creditPoints);
    writeData<std::string>(ofs, userInfo["fullName"]);
    writeData<std::string>(ofs, userInfo["phoneNumber"]);
    writeData<std::string>(ofs, userInfo["email"]);
    writeData<std::string>(ofs, userInfo["address"]);
    writeData<std::vector<std::string>>(ofs, skills);
    writeData<std::vector<std::string>>(ofs, blockedMembers);
    writeVector<Activity>(ofs, activities);
    writeData<std::vector<std::string>>(ofs, comments);
}

Member::Member(std::string _userName, std::string _pasword) : User(_userName, _pasword)
{
    comments = std::vector<std::string>();
    activities = std::vector<Activity *>();
    userInfo = std::map<std::string, std::string>();
    blockedMembers = std::vector<std::string>();
    skillRatingScore = new RatingScore();
    hostRatingScore = new RatingScore();
    supportRatingScore = new RatingScore();
    id = generateId<User>();
    std::cout << "Please enter your full name: ";
    std::getline(std::cin, userInfo["fullName"]);
    std::cout << "Please enter your phone number: ";
    std::getline(std::cin, userInfo["phoneNumber"]);
    std::cout << "Please enter your email: ";
    std::getline(std::cin, userInfo["email"]);
    std::cout << "Please enter your address: ";
    std::getline(std::cin, userInfo["address"]);
    while (true)
    {
        std::cout << "\nPlease enter your skill or type 'x' to finish: ";
        std::string skill;
        std::getline(std::cin, skill);
        if (skill == "" || skill == "x")
        {
            break;
        }
        skills.push_back(skill);
    }
}

float Member::getSkillRatingScore()
{
    return skillRatingScore->getRatingScore();
}

float Member::getHostRatingScore()
{
    return hostRatingScore->getRatingScore();
}

float Member::getSupportRatingScore()
{
    return supportRatingScore->getRatingScore();
}

int Member::getCreditPoints()
{
    return creditPoints;
}

void Member::setCreaditPoints(int points)
{
    creditPoints = points;
}

bool Member::minusCreditPoints(int points)
{
    if (creditPoints >= points)
    {
        creditPoints -= points;
        return true;
    }

    return false;
}

bool Member::addCreditPoints(int points)
{
    creditPoints += points;
    return true;
}

void Member::addSkillRatingScore(int ratingScore)
{
    skillRatingScore->addRatingScore(ratingScore);
}

void Member::addHostRatingScore(int ratingScore)
{
    hostRatingScore->addRatingScore(ratingScore);
}

void Member::addSupportRatingScore(int ratingScore)
{
    supportRatingScore->addRatingScore(ratingScore);
}

bool Member::blockMember()
{
    for (User *user : TimeBankSystem::getUsers())
    {
        if (dynamic_cast<Admin *>(user) != nullptr)
        {
            continue;
        }
        Member *member = dynamic_cast<Member *>(user);
        if (member->getId() == getId())
        {
            continue;
        }
        auto it = std::find(blockedMembers.begin(), blockedMembers.end(), member->getId());
        if (it != blockedMembers.end())
        {
            continue;
        }
        if (member != nullptr || member->getId() != id)
        {
            std::cout << "Username: " << member->getUserName() << ", ID: " << member->getId() << std::endl;
        }
    }
    std::string id;
    std::cout << "Please enter the id of the member you want to block: ";
    std::getline(std::cin, id);
    User *user = findUserById(id, TimeBankSystem::getUsers());
    if (user == nullptr)
    {
        std::cout << "User not found" << std::endl;
        std::cout << "Block user failed" << std::endl;
        return false;
    }

    if (user->getId() == getId())
    {
        std::cout << "You cannot block yourself" << std::endl;
        std::cout << "Block user failed" << std::endl;
        return false;
    }
    blockedMembers.push_back(user->getId());
    std::cout << "Block user successful" << std::endl;
    return true;
}

bool Member::unBlockMember()
{
    if (blockedMembers.empty())
    {
        std::cout << "You have not blocked any member" << std::endl;
        return false;
    }

    std::unordered_map<std::string, Member *> blockedMembersMap;
    for (auto &blockedMember : blockedMembers)
    {
        auto users = TimeBankSystem::getUsers(); // get all users
        auto it = std::find_if(users.begin(), users.end(), [blockedMember](User *user)
                               { return user->getId() == blockedMember; });
        if (it != TimeBankSystem::getUsers().end())
        {
            Member *member = dynamic_cast<Member *>(*it);
            blockedMembersMap[blockedMember] = member;
        }
    }

    std::cout << "List of blocked members: " << std::endl;
    for (auto &pair : blockedMembersMap)
    {
        std::cout << "Username: " << pair.second->getUserName() << ", ID: " << pair.second->getId() << std::endl;
    }

    std::string unBlockUserId;
    std::cout << "Please enter the id of the member you want to unblock: ";
    std::getline(std::cin, unBlockUserId);
    if (blockedMembersMap.find(unBlockUserId) == blockedMembersMap.end())
    {
        std::cout << "Blocked user id not found" << std::endl;
        std::cout << "Unblock user failed" << std::endl;
        return false;
    }
    blockedMembers.erase(std::remove(blockedMembers.begin(), blockedMembers.end(), unBlockUserId), blockedMembers.end());
    std::cout << "Unblock user successful" << std::endl;
    return true;
}

bool Member::listYourSelf()
{
    try
    {
        int _miminumHostRatingScore;
        int consumingPoints;
        std::cout << "Please enter the minimum host rating score: ";
        std::cin >> _miminumHostRatingScore; // get minimum host rating score
        std::cin.ignore(1, '\n');            // ignore the new line character
        if (_miminumHostRatingScore < 0 || _miminumHostRatingScore > 5)
        {
            throw std::invalid_argument("Invalid rating score");
        }
        std::cout << "Please enter the consuming points for this activity: ";
        std::cin >> consumingPoints;
        std::cin.ignore(1, '\n');
        if (consumingPoints < 0)
        {
            throw std::invalid_argument("Invalid consuming points");
        }
        std::cout << "Please enter the start time: ";
        std::time_t startTime = getTime();
        if (startTime == -1)
        {
            throw std::invalid_argument("Invalid start time");
        }

        std::cout << "Please enter the end time: ";
        std::time_t endTime = getTime();
        if (endTime == -1)
        {
            throw std::invalid_argument("Invalid end time");
        }

        if (startTime >= endTime)
        {
            throw std::invalid_argument("Start time must be earlier than end time");
        }

        if (startTime < getCurrentTime())
        {
            throw std::invalid_argument("Start time must be later than current time");
        }

        for (Activity *activity : activities)
        {
            if (!(startTime > activity->getEndTime() || endTime < activity->getStartTime())) // condition to check overlap time
            {
                throw std::invalid_argument("You have another activity at this time");
            }
        }

        Activity *activity = new Activity(startTime, endTime, getId(), _miminumHostRatingScore, consumingPoints, getSkills());
        activities.push_back(activity);
        return true;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool Member::unListYourSelf()
{
    std::string id;
    std::cout << "Please enter the id of the activity you want to unlist: ";
    std::getline(std::cin, id);
    auto it = std::find_if(activities.begin(), activities.end(), [&id](Activity *activity)
                           { return activity->getId() == id; });
    if (it == activities.end())
    {
        std::cout << "Activity not found" << std::endl;
        std::cout << "Unlist activity failed" << std::endl;
        return false;
    }
    if (!(*it)->getHostId().empty())
    {
        std::cout << "You can not unlist this acctivity" << std::endl;
        std::cout << "Unlist activity failed" << std::endl;
        return false;
    }
    activities.erase(it);
    std::cout << "Unlist activity successful" << std::endl;
    return true;
}

std::vector<std::string> Member::getBlockedMembers()
{
    return blockedMembers;
}

bool Member::bookService()
{
    std::string id;
    std::cout << "Please enter the id of the activity you want to book: ";
    std::getline(std::cin, id);

    Activity *activity = nullptr;
    auto users = TimeBankSystem::getUsers(); // get all users

    for (User *user : users)
    {
        if (dynamic_cast<Admin *>(user) != nullptr || user->getId() == getId())
        {
            continue;
        }

        Member *member = dynamic_cast<Member *>(user);
        auto activities = member->getActivities();
        auto it = std::find_if(activities.begin(), activities.end(), [&id](Activity *activity)
                               { return activity->getId() == id; });

        if (it != activities.end())
        {
            activity = *it;
            break;
        }
    }

    if (activity == nullptr)
    {
        std::cout << "Activity not found" << std::endl;
        std::cout << "Book activity failed" << std::endl;
        return false;
    }

    if (!activity->getHostId().empty() || activity->getHostId() == getId())
    {
        std::cout << "You cannot book this activity" << std::endl;
        std::cout << "Book activity failed" << std::endl;
        return false;
    }

    std::string requesterId = getId();
    activity->addRequester(requesterId);
    std::cout << "Book activity successful" << std::endl;
    return true;
}

std::vector<Activity *> Member::getActivities()
{
    return activities;
}

void Member::setActivities(std::vector<Activity *> _activities)
{
    activities = _activities;
}

std::vector<std::string> Member::getSkills()
{
    return skills;
}

std::ostream &operator<<(std::ostream &os, Member &user)
{
    os << "id: " << user.getId();
    os << " userName: " << user.getUserName();
    os << " skill: " << user.getSkillRatingScore();
    os << " skill: " << user.getHostRatingScore();
    os << " skill: " << user.getSupportRatingScore();
    return os;
}
