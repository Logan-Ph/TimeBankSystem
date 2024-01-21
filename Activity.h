#ifndef _ACTIVITY_H_
#define _ACTIVITY_H_
#include <string>
#include <ctime>
#include "Serialization.h"
#include "User.h"
#include <vector>

class Activity : public Serialization
{
private:
    std::string id;
    std::string supporterId;
    std::string hostId;
    std::string supporterName;
    std::string hostName;
    std::string city;
    int consumingPoint;
    int minimumHostRatingScore;
    std::time_t startTime;
    std::time_t endTime;
    int isClosed;
    std::vector<std::string> skills;
    std::vector<std::string> requesters; // id of requesters
public:
    Activity();
    ~Activity();
    Activity(std::string _supporterName, std::time_t startTime, std::time_t endTime, std::string supporterId, int _minimumHostRatingScore, int _consumingPoint, std::vector<std::string> _skills, std::string _city);
    std::string getId();
    std::string getSupporterId();
    std::string getHostId();
    std::string getCity();
    std::string getSupporterName();
    std::string getHostName();
    void setHostName(std::string _hostName);
    int isActivityClosed();
    void closeActivity();
    int getConsumingPoint();
    int getMinimumHostRatingScore();
    std::time_t getStartTime();
    std::time_t getEndTime();
    std::vector<std::string> getSkills();
    std::vector<std::string> getRequesters();
    void removeRequester(std::string requesterId);
    bool addRequester(std::string requesterId);
    bool acceptRequester(std::string requesterId);
    void read(std::ifstream &ifs) override;
    void write(std::ofstream &ofs) override;
    friend std::ostream &operator<<(std::ostream &os, Activity &activity);
};

#endif