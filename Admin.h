#ifndef _ADMIN_H_
#define _ADMIN_H_
#include "User.h"
#include <string>

class Admin : public User
{
public:
    Admin();
    Admin(std::string userName, std::string password);
    ~Admin() override;
    bool resetPassword();
};

#endif
