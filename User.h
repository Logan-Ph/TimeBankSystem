#ifndef _USER_H_
#define _USER_H_
#include "Serialization.h"
#include <string>
#include <fstream>

class User : public Serialization
{
private:
    std::string userName;
    std::string password;
    std::string id;

public:
    User();
    User(std::string userName, std::string password);
    virtual ~User();
    std::string getUserName();
    std::string getPassword();
    std::string getId();
    void setUserName(std::string userName);
    void setPassword(std::string password);
    void setId(std::string id);
    virtual void read(std::ifstream &ifs);
    virtual void write(std::ofstream &ofs);
    friend std::ostream &operator<<(std::ostream &os, const User &user);
};

#endif