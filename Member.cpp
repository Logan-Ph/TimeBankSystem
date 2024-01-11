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
    userInfo["city"] = readData<std::string>(ifs);
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
    writeData<std::string>(ofs, userInfo["city"]);
    writeData<std::vector<std::string>>(ofs, skills);
    writeData<std::vector<std::string>>(ofs, blockedMembers);
    writeVector<Activity>(ofs, activities);
    writeData<std::vector<std::string>>(ofs, comments);
}

Member::Member(std::string _userName, std::string _pasword) : User(_userName, _pasword)
{
    creditPoints = 20;
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
    std::cout << "Please enter your city: ";
    std::getline(std::cin, userInfo["city"]);
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

    if (blockedMembers.empty() || blockedMembers.size() == 0)
    {
        std::cout << "You have not blocked any member" << std::endl;
        return false;
    }

    std::unordered_map<std::string, Member *> blockedMembersMap;
    for (std::string blockedMember : blockedMembers)
    {
        auto users = TimeBankSystem::getUsers(); // get all users
        auto it = std::find_if(users.begin(), users.end(), [blockedMember](User *user)
                               { return user->getId() == blockedMember; });
        if (it != users.end())
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
            if (activity->getHostId().empty() && (!(startTime > activity->getEndTime() || endTime < activity->getStartTime()))) // condition to check overlap time
            {
                throw std::invalid_argument("You have another activity at this time");
            }
        }
        Activity *activity = new Activity(startTime, endTime, getId(), _miminumHostRatingScore, consumingPoints, getSkills(), userInfo["city"]);
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
    if (activities.empty())
    {
        std::cout << "You have not listed any activity" << std::endl;
        return false;
    }
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
    for (User *user : TimeBankSystem::getUsers())
    {
        if (dynamic_cast<Admin *>(user) != nullptr)
        {
            continue;
        }
        Member *_member = dynamic_cast<Member *>(user);
        if (getId() == _member->getId())
        {
            continue;
        }
        auto it = std::find(getBlockedMembers().begin(), getBlockedMembers().end(), _member->getId());
        if (it != getBlockedMembers().end())
        {
            continue;
        }

        auto activities = _member->getActivities();
        for (Activity *activity : activities)
        {
            std::vector<std::string> requesters = activity->getRequesters();

            if (activity->getHostId().empty() && std::find(requesters.begin(), requesters.end(), getId()) == requesters.end() && activity->getMinimumHostRatingScore() <= getHostRatingScore())
            {
                std::cout << *activity << std::endl;
            }
        }
    }
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

    if (activity->getConsumingPoint() > getCreditPoints())
    {
        std::cout << "You do not have enough credit points to book this activity" << std::endl;
        std::cout << "Book activity failed" << std::endl;
        return false;
    }

    if (activity->getMinimumHostRatingScore() > getHostRatingScore())
    {
        std::cout << "Your host rating score is not enough to book this activity" << std::endl;
        std::cout << "Book activity failed" << std::endl;
        return false;
    }

    std::string requesterId = getId();
    activity->addRequester(requesterId);
    std::cout << "Book activity successful" << std::endl;
    return true;
}

void Member::addActivity(Activity *activity)
{
    activities.push_back(activity);
}

bool Member::confirmActivity()
{
    if (getActivities().empty())
    {
        std::cout << "You have not listed any activity" << std::endl;
        return false;
    }
    for (auto &activity : getActivities())
    {
        if (activity->getHostId().empty())
        {
            std::cout << *activity << std::endl;
        }
    }
    std::string id;
    std::cout << "Please enter the id of the activity you want to confirm: ";
    std::getline(std::cin, id);

    Activity *activity = nullptr;
    auto it = std::find_if(activities.begin(), activities.end(), [&id](Activity *activity)
                           { return activity->getId() == id; });
    if (it == activities.end())
    {
        std::cout << "Activity not found" << std::endl;
        std::cout << "Confirm activity failed" << std::endl;
        return false;
    }

    activity = *it;
    std::cout << "Please enter the id of the member you want to confirm: ";
    std::string requesterId;
    std::getline(std::cin, requesterId);
    if (std::find(activity->getRequesters().begin(), activity->getRequesters().end(), requesterId) == activity->getRequesters().end())
    {
        std::cout << "Requester id not found" << std::endl;
        std::cout << "Confirm activity failed" << std::endl;
        return false;
    }
    activity->acceptRequester(requesterId);
    setCreaditPoints(getCreditPoints() + activity->getConsumingPoint());
    auto host = dynamic_cast<Member *>(findUserById(activity->getHostId(), TimeBankSystem::getUsers()));
    if (host != nullptr)
    {
        host->setCreaditPoints(host->getCreditPoints() - activity->getConsumingPoint());
        host->addActivity(activity);
    }
    std::cout << "Confirm activity successful" << std::endl;
    return true;
}

bool Member::feedbackActivity()
{
    std::vector<Activity *> hostActivities = std::vector<Activity *>();
    std::vector<Activity *> supportActivites = std::vector<Activity *>();
    for (auto *activity : getActivities())
    {
        if (!activity->getHostId().empty() && activity->getHostId() == getId())
        {
            hostActivities.push_back(activity);
        }
        else if (!activity->getSupporterId().empty() && activity->getSupporterId() == getId())
        {
            supportActivites.push_back(activity);
        }
    }

    if (hostActivities.empty() && supportActivites.empty())
    {
        std::cout << "You have not participated in any activity" << std::endl;
        return false;
    }

    std::cout << "Activities as Host: " << std::endl;
    for (auto *activity : hostActivities)
    {
        std::cout << "----------------------" << std::endl;
        std::cout << *activity << std::endl;
    }
    std::cout << "----------------------" << std::endl;

    std::cout << std::endl;

    std::cout << "Activities as Supporter: " << std::endl;
    for (auto *activity : supportActivites)
    {
        std::cout << "----------------------" << std::endl;
        std::cout << *activity << std::endl;
    }
    std::cout << "----------------------" << std::endl;

    std::string id;
    std::cout << "Please enter the id of the activity you want to feedback: ";
    std::getline(std::cin, id);

    Activity *activity = nullptr;
    auto it = std::find_if(activities.begin(), activities.end(), [&id](Activity *activity)
                           { return activity->getId() == id; });
    if (it == activities.end())
    {
        std::cout << "Activity not found" << std::endl;
        std::cout << "Feedback activity failed" << std::endl;
        return false;
    }

    activity = *it;
    Member *member = nullptr;
    if (activity->getHostId() == getId())
    {
        member = dynamic_cast<Member *>(findUserById(activity->getSupporterId(), TimeBankSystem::getUsers()));
    }
    else
    {
        member = dynamic_cast<Member *>(findUserById(activity->getHostId(), TimeBankSystem::getUsers()));
    }

    if (member == nullptr)
    {
        std::cout << "Member not found" << std::endl;
        std::cout << "Feedback activity failed" << std::endl;
        return false;
    }
    std::string comment;
    std::cout << "Please enter your comment: ";
    std::getline(std::cin, comment);
    member->comments.push_back(comment);
    if (member->getId() == activity->getHostId())
    {
        try
        {
            std::cout << "Please enter the rating score for the host: ";
            std::string ratingScore;
            std::getline(std::cin, ratingScore);
            int _ratingScore = std::stoi(ratingScore);
            if (_ratingScore < 0 || _ratingScore > 5)
            {
                throw std::invalid_argument("Invalid rating score");
            }
            member->addHostRatingScore(_ratingScore);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Feedback activity failed" << std::endl;
            return false;
        }
    }
    else
    {
        try
        {
            std::cout << "Please enter the rating supporter rating score for the supporter: ";
            std::string ratingScore;
            std::getline(std::cin, ratingScore);
            int supporterRatingScore = std::stoi(ratingScore);
            if (supporterRatingScore < 0 || supporterRatingScore > 5)
            {
                throw std::invalid_argument("Invalid rating score");
            }
            member->addSupportRatingScore(supporterRatingScore);
            std::cout << "Please enter the skill rating score for the supporter: ";
            std::getline(std::cin, ratingScore);
            int skillRatingScore = std::stoi(ratingScore);
            if (skillRatingScore < 0 || skillRatingScore > 5)
            {
                throw std::invalid_argument("Invalid rating score");
            }
            member->addSkillRatingScore(skillRatingScore);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "Feedback activity failed" << std::endl;
            return false;
        }
    }
    activities.erase(it);
    std::cout << "Feedback activity successful" << std::endl;
    return true;
}

bool Member::addCreditPoints()
{
    std::string points;
    std::cout << "Your current credit points: " << creditPoints << std::endl; // print current credit points
    std::cout << "Please enter the number of credit points you want to add (1 point/$): ";
    std::getline(std::cin, points);
    try
    {
        int _points = std::stoi(points);
        if (_points < 0)
        {
            throw std::invalid_argument("Invalid credit points");
        }
        creditPoints += _points;
        std::cout << "Add credit points successful" << std::endl;
        return true;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Add credit points failed" << std::endl;
        return false;
    }
}

void Member::viewProfile()
{
    std::cout << "Username: " << getUserName() << std::endl;
    std::cout << "Full name: " << userInfo["fullName"] << std::endl;
    std::cout << "Phone number: " << userInfo["phoneNumber"] << std::endl;
    std::cout << "Email: " << userInfo["email"] << std::endl;
    std::cout << "City: " << userInfo["city"] << std::endl;
    std::cout << "Skills: " << std::endl;
    for (auto skill : skills)
    {
        if (!skill.empty())
        {
            std::cout << "\t"
                      << "-" << skill << std::endl;
        }
    }
    std::cout << "Credit points: " << creditPoints << std::endl;
    std::cout << "Skill rating score: " << skillRatingScore->getRatingScore() << std::endl;
    std::cout << "Host rating score: " << hostRatingScore->getRatingScore() << std::endl;
    std::cout << "Support rating score: " << supportRatingScore->getRatingScore() << std::endl;
    std::cout << "Blocked members: " << std::endl;
    for (auto blockedMember : blockedMembers)
    {
        if (!blockedMember.empty())
        {
            std::cout << "\t"
                      << "-" << blockedMember << std::endl;
        }
    }
    std::cout << "Activities: " << std::endl;
    for (auto activity : activities)
    {
        std::cout << "\t" << *activity << std::endl;
    }
    std::cout << "Comments: " << std::endl;
    for (auto comment : comments)
    {
        if (!comment.empty())
        {
            std::cout << "\t"
                      << "-" << comment << std::endl;
        }
    }
}

void Member::viewPendingRequest()
{
    std::vector<Activity *> hostActivities = std::vector<Activity *>();
    std::vector<Activity *> supportActivites = std::vector<Activity *>();
    for (auto *activity : getActivities())
    {
        if (activity->getHostId().empty() && std::find(activity->getRequesters().begin(), activity->getRequesters().end(), getId()) != activity->getRequesters().end())
        {
            hostActivities.push_back(activity);
        }
        else if (activity->getHostId().empty() && activity->getSupporterId() == getId())
        {
            supportActivites.push_back(activity);
        }
    }

    std::cout << "Pending requests activities as Host: " << std::endl;
    for (auto *activity : hostActivities)
    {
        std::cout << "----------------------" << std::endl;
        std::cout << *activity << std::endl;
    }
    std::cout << "----------------------" << std::endl;

    std::cout << std::endl;

    std::cout << "Pending requests activities as Supporter: " << std::endl;
    for (auto *activity : supportActivites)
    {
        std::cout << "----------------------" << std::endl;
        std::cout << *activity << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}

void Member::viewOtherUserProfile()
{
    auto users = TimeBankSystem::getUsers();
    for (auto user : users)
    {
        if (dynamic_cast<Admin *>(user) != nullptr || user->getId() == getId() || std::find(blockedMembers.begin(), blockedMembers.end(), user->getId()) != blockedMembers.end())
        {
            continue;
        }
        Member *member = dynamic_cast<Member *>(user);
        std::cout << "----------------------------------" << std::endl;
        std::cout << *member << std::endl;
        std::cout << "----------------------------------" << std::endl;
    }
}

void Member::searchServices()
{
    try
    {
        std::string city;
        std::cout << "Please enter the start time or 'x' to select all time stamp: ";
        std::time_t startTime = getTime();
        std::cout << "Please enter the end time or 'x' to select all time stamp: ";
        std::time_t endTime = getTime();
        if (startTime != -1 && endTime != -1 && (startTime > endTime))
        {
            throw std::invalid_argument("Start time must be earlier than end time");
        }

        std::cout << "Please enter the city: ";
        std::getline(std::cin, city);
        auto users = TimeBankSystem::getUsers();
        for (auto user : users)
        {
            if (dynamic_cast<Admin *>(user) != nullptr || user->getId() == getId())
            {
                continue;
            }
            Member *member = dynamic_cast<Member *>(user);
            auto activities = member->getActivities();
            auto blockedMembers = member->getBlockedMembers();
            if (std::find(blockedMembers.begin(), blockedMembers.end(), getId()) != blockedMembers.end())
            {
                continue;
            }
            for (auto activity : activities)
            {
                bool isHostEmpty = activity->getHostId().empty();
                bool isWithinTime = (startTime == -1 && endTime == -1) ||
                                    (startTime != -1 && startTime <= activity->getStartTime()) ||
                                    (endTime != -1 && endTime >= activity->getEndTime()) ||
                                    (startTime <= activity->getEndTime() && endTime >= activity->getStartTime());
                bool isCityMatch = city.empty() || activity->getCity() == city;
                if (isHostEmpty && isWithinTime && isCityMatch)
                {
                    std::cout << *activity << std::endl;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "Search services failed" << std::endl;
    }
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

std::map<std::string, std::string> Member::getUserInfo()
{
    return userInfo;
}

std::ostream &operator<<(std::ostream &os, Member &user)
{
    std::cout << "Username: " << user.getUserName() << std::endl;
    std::cout << "Full name: " << user.userInfo["fullName"] << std::endl;
    std::cout << "Phone number: " << user.userInfo["phoneNumber"] << std::endl;
    std::cout << "Email: " << user.userInfo["email"] << std::endl;
    std::cout << "City: " << user.userInfo["city"] << std::endl;
    std::cout << "Skills: " << std::endl;
    for (auto skill : user.getSkills())
    {
        if (!skill.empty())
        {
            std::cout << "\t" << skill << std::endl;
        }
    }
    std::cout << "Skill rating score: " << user.skillRatingScore->getRatingScore() << std::endl;
    std::cout << "Host rating score: " << user.hostRatingScore->getRatingScore() << std::endl;
    std::cout << "Support rating score: " << user.supportRatingScore->getRatingScore() << std::endl;
    std::cout << "Blocked members: " << std::endl;
    std::cout << "Comments: " << std::endl;
    for (auto comment : user.comments)
    {
        if (!comment.empty())
        {
            std::cout << "\t" << comment << std::endl;
        }
    }
    return os;
}
