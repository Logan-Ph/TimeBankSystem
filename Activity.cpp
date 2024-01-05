#include "Activity.h"
#include "Functions.h"
#include <iostream>
#include <algorithm>

Activity::Activity()
{
    skills = std::vector<std::string>();      // initialize skills
    requresters = std::vector<std::string>(); // initialize requresters
}

Activity::~Activity() = default;

Activity::Activity(std::time_t _startTime, std::time_t _endTime, std::string _supporterId, int _minimumHostRatingScore, int _consumingPoint)
{
    id = generateId<Activity>();
    minimumHostRatingScore = _minimumHostRatingScore;
    consumingPoint = _consumingPoint;
    startTime = _startTime;
    endTime = _endTime;
    supporterId = _supporterId;
    skills = std::vector<std::string>();      // initialize skills
    requresters = std::vector<std::string>(); // initialize requresters
}

bool Activity::addRequester(std::string requesterId)
{
    requresters.push_back(requesterId);
    return true;
}

bool Activity::acceptRequester(std::string requesterId)
{
    requresters.erase(std::remove_if(requresters.begin(), requresters.end(), [&requesterId](std::string &id)
                                     { return id != requesterId; }),
                      requresters.end());
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
    return requresters;
}

std::ostream &operator<<(std::ostream &os, Activity &activity)
{
    os << "Activity id: " << activity.getId() << std::endl;
    os << "Supporter id: " << activity.getSupporterId() << std::endl;
    os << "Host id: " << activity.getHostId() << std::endl;
    os << "City: " << activity.getCity() << std::endl;
    os << "Consuming point: " << activity.getConsumingPoint() << std::endl;
    os << "Minimum host rating score: " << activity.getConsumingPoint() << std::endl;
    os << "Start time: " << formatTime(activity.getStartTime()) << std::endl;
    os << "End time: " << formatTime(activity.getEndTime()) << std::endl;
    os << "Skills: ";
    for (auto it = activity.getSkills().begin(); it != activity.getSkills().end(); ++it)
    {
        os << *it << " ";
    }
    os << std::endl;
    os << "Requesters: ";
    for (auto it = activity.getRequesters().begin(); it != activity.getRequesters().end(); ++it)
    {
        os << *it << " ";
    }
    os << std::endl;
    return os;
}

void Activity::write(std::ofstream &ofs)
{
    writeData<std::string>(ofs, id);
    writeData<std::string>(ofs, supporterId);
    writeData<std::string>(ofs, hostId);
    writeData<std::string>(ofs, city);
    writeData<int>(ofs, consumingPoint);
    writeData<int>(ofs, minimumHostRatingScore);
    writeData<std::time_t>(ofs, startTime);
    writeData<std::time_t>(ofs, endTime);
    writeData<std::vector<std::string>>(ofs, skills);
    writeData<std::vector<std::string>>(ofs, requresters);
}

void Activity::read(std::ifstream &ifs)
{
    id = readData<std::string>(ifs);
    supporterId = readData<std::string>(ifs);
    hostId = readData<std::string>(ifs);
    city = readData<std::string>(ifs);
    consumingPoint = readData<int>(ifs);
    minimumHostRatingScore = readData<int>(ifs);
    startTime = readData<std::time_t>(ifs);
    endTime = readData<std::time_t>(ifs);
    skills = readData<std::vector<std::string>>(ifs);
    requresters = readData<std::vector<std::string>>(ifs);
}