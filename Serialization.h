#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <fstream>

class Serialization
{
public:
    virtual void read(std::ifstream &ifs) = 0;
    virtual void write(std::ofstream &ofs) = 0;
};

#endif