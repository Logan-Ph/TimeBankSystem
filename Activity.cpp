#include "Activity.h"
#include "Functions.h"
#include <iostream>
#include <algorithm>

Activity::Activity()
{
    skills = std::vector<std::string>();     // initialize skills
    requesters = std::vector<std::string>(); // initialize requresters
}

Activity::~Activity() = default;

Activity::Activity(std::string _supporterName, std::time_t _startTime, std::time_t _endTime, std::string _supporterId, int _minimumHostRatingScore, int _consumingPoint, std::vector<std::string> _skills, std::string _city)
{
    id = generateId<Activity>();
    supporterName = _supporterName;
    minimumHostRatingScore = _minimumHostRatingScore;
    consumingPoint = _consumingPoint;
    startTime = _startTime;
    endTime = _endTime;
    supporterId = _supporterId;
    skills = _skills;                        // initialize skills
    requesters = std::vector<std::string>(); // initialize requresters
    city = _city;
    isClosed = 0;
}

bool Activity::addRequester(std::string requesterId)
{
    requesters.push_back(requesterId);
    return true;
}

bool Activity::acceptRequester(std::string requesterId)
{
    requesters.erase(std::remove_if(requesters.begin(), requesters.end(), [&requesterId](std::string &id)
                                    { return id != requesterId; }),
                     requesters.end());
    hostId = requesterId;
    return true;
}

std::string Activity::getId()
{
    return id;
}

std::string Activity::getSupporterId()
{
    return supporterId;
}

std::string Activity::getHostId()
{
    return hostId;
}

std::string Activity::getCity()
{
    return city;
}

int Activity::getConsumingPoint()
{
    return consumingPoint;
}

int Activity::getMinimumHostRatingScore()
{
    return minimumHostRatingScore;
}

std::time_t Activity::getStartTime()
{
    return startTime;
}

std::time_t Activity::getEndTime()
{
    return endTime;
}

std::vector<std::string> Activity::getSkills()
{
    return skills;
}

std::vector<std::string> Activity::getRequesters()
{
    return requesters;
}

int Activity::isActivityClosed()
{
    return isClosed;
}

void Activity::closeActivity()
{
    isClosed = 1;
}

std::string Activity::getSupporterName()
{
    return supporterName;
}

std::string Activity::getHostName()
{
    return hostName;
}

void Activity::setHostName(std::string _hostName)
{
    hostName = _hostName;
}

void Activity::removeRequester(std::string requesterId)
{
    requesters.erase(std::remove_if(requesters.begin(), requesters.end(), [&requesterId](std::string &id)
                                    { return id == requesterId; }),
                     requesters.end());
}

std::ostream &operator<<(std::ostream &os, Activity &activity)
{
    os << "Activity id: " << activity.getId() << std::endl;
    os << "Supporter info: " << std::endl;
    os << "\t-Supporter id: " << activity.getSupporterId() << std::endl;
    os << "\t-Supporter name: " << activity.getSupporterName() << std::endl;
    os << "Host info: " << std::endl;
    os << "\t-Host id: " << activity.getHostId() << std::endl;
    os << "\t-Host name: " << activity.getHostName() << std::endl;
    os << "City: " << activity.getCity() << std::endl;
    os << "Consuming point: " << activity.getConsumingPoint() << std::endl;
    os << "Minimum host rating score: " << activity.getMinimumHostRatingScore() << std::endl;
    os << "Start time: " << formatTime(activity.getStartTime()) << std::endl;
    os << "End time: " << formatTime(activity.getEndTime()) << std::endl;
    os << "Skills: " << std::endl;
    auto skills = activity.getSkills();
    for (std::string skill : skills)
    {
        std::cout << "\t" << skill << std::endl;
    }
    os << "Requesters: " << std::endl;
    auto requesters = activity.getRequesters();
    for (std::string requesterId : requesters)
    {
        std::cout << "\t" << requesterId << std::endl;
    }
    return os;
}

void Activity::write(std::ofstream &ofs)
{
    writeData<std::string>(ofs, id);
    writeData<std::string>(ofs, supporterId);
    writeData<std::string>(ofs, hostId);
    writeData<std::string>(ofs, supporterName);
    writeData<std::string>(ofs, hostName);
    writeData<std::string>(ofs, city);
    writeData<int>(ofs, consumingPoint);
    writeData<int>(ofs, minimumHostRatingScore);
    writeData<std::time_t>(ofs, startTime);
    writeData<std::time_t>(ofs, endTime);
    writeData<int>(ofs, isClosed);
    writeData<std::vector<std::string>>(ofs, skills);
    writeData<std::vector<std::string>>(ofs, requesters);
}

void Activity::read(std::ifstream &ifs)
{
    id = readData<std::string>(ifs);
    supporterId = readData<std::string>(ifs);
    hostId = readData<std::string>(ifs);
    supporterName = readData<std::string>(ifs);
    hostName = readData<std::string>(ifs);
    city = readData<std::string>(ifs);
    consumingPoint = readData<int>(ifs);
    minimumHostRatingScore = readData<int>(ifs);
    startTime = readData<std::time_t>(ifs);
    endTime = readData<std::time_t>(ifs);
    isClosed = readData<int>(ifs);
    skills = readData<std::vector<std::string>>(ifs);
    skills.erase(std::remove_if(skills.begin(), skills.end(), [](std::string skill)
                                { return skill.empty(); }),
                 skills.end());
    requesters = readData<std::vector<std::string>>(ifs);
    requesters.erase(std::remove_if(requesters.begin(), requesters.end(), [](std::string id)
                                    { return id.empty(); }),
                     requesters.end()); // remove empty data
}
