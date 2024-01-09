#ifndef _MEMBER_H_
#define _MEMBER_H_

#include "User.h"
#include <string>
#include <iostream>
#include <vector>
#include "RatingScore.h"
#include <map>
#include "Activity.h"

class Member : public User
{
private:
    std::string id;
    int creditPoints;
    RatingScore *skillRatingScore;
    RatingScore *hostRatingScore;
    RatingScore *supportRatingScore;
    std::map<std::string, std::string> userInfo;
    std::vector<std::string> skills;
    std::vector<std::string> blockedMembers; // store blocked members' id
    std::vector<Activity *> activities;
    std::vector<std::string> comments;

public:
    Member();
    ~Member() override;
    void read(std::ifstream &ifs) override;
    void write(std::ofstream &ofs) override;
    Member(std::string _userName, std::string _password);
    float getSkillRatingScore();
    float getHostRatingScore();
    float getSupportRatingScore();
    int getCreditPoints();
    std::vector<Activity *> getActivities();
    std::vector<std::string> getBlockedMembers();
    std::vector<std::string> getSkills();
    void addActivity(Activity *activity);
    void setActivities(std::vector<Activity *> _activities);
    void setCreaditPoints(int points);
    bool addCreditPoints(int points);
    bool minusCreditPoints(int points);
    void addSkillRatingScore(int ratingScore);
    void addHostRatingScore(int ratingScore);
    void addSupportRatingScore(int ratingScore);
    bool blockMember();
    bool unBlockMember();
    bool listYourSelf();
    bool unListYourSelf();
    bool bookService();
    bool confirmActivity();
    bool feedbackActivity();
    bool addCreditPoints();
    void viewProfile();
    void viewPendingRequest();
    void viewOtherUserProfile();
    void searchServices();
    std::map<std::string, std::string> getUserInfo();
    friend std::ostream &operator<<(std::ostream &os, Member &user);
};

#endif