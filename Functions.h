#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "User.h"
#include "Activity.h"
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>

std::vector<User *> readUsers(std::string fileName); // read all users from file

template <typename T> // read all vector of pointers of complex objects from file
std::vector<T *> readVector(std::ifstream &ifs)
{
    size_t dataVecSize;                                                    // size of the vector
    ifs.read(reinterpret_cast<char *>(&dataVecSize), sizeof(dataVecSize)); // read the size of the vector
    std::vector<T *> dataVec(dataVecSize);                                 // vector of pointers to T objects
    for (size_t i = 0; i < dataVecSize; i++)
    {
        T *data = new T();       // create new T object
        data->read(ifs);         // read data from file
        dataVec.push_back(data); // add data to vector
    }
    if (ifs.fail() && !ifs.eof())
    {
        std::cerr << "Error occurred before reaching end of file" << std::endl;
    }
    return dataVec;
};

template <typename T> // write all vector of pointers of complex objects from file
bool writeVector(std::ofstream &ofs, std::vector<T *> dataVec)
{
    try
    {
        size_t dataVecSize = dataVec.size();                                          // size of the vector
        ofs.write(reinterpret_cast<const char *>(&dataVecSize), sizeof(dataVecSize)); // write the size of vector to file
        for (auto it = dataVec.begin(); it != dataVec.end(); ++it)
        {
            (*it)->write(ofs); // write data to file
        }
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "Error writing to file" << std::endl;
        return false;
    }
};

bool writeUsers(std::vector<User *> users, std::string fileName); // write alll users from file

User *findUserById(std::string id, std::vector<User *>); // find users by id

User *findUserByUserName(std::string userName, std::vector<User *>); // find users by username

std::time_t getTime(); // get time for user

std::time_t getCurrentTime(); // get current time

std::string formatTime(std::time_t time); // format time

template <typename T>
std::string generateId() // generate id for user
{
    static int counter = 0;
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    if constexpr (std::is_same<T, User>::value)
    {
        ss << 'U';
    }
    else if constexpr (std::is_same<T, Activity>::value)
    {
        ss << 'A';
    }
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d%H%M%S") << std::setfill('0') << std::setw(3) << ms.count() << std::setfill('0') << std::setw(3) << counter++;
    return ss.str();
}

template <typename T>
T readData(std::ifstream &ifs) // read data from file (used only for POD and non-POD types)
{
    if constexpr (std::is_same<T, std::string>::value) // check string type
    {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        std::vector<char> buffer(size);
        ifs.read(buffer.data(), size);
        return std::string(buffer.begin(), buffer.end());
    }
    else if constexpr ((std::is_same<T, int>::value) || (std::is_same<T, float>::value) || (std::is_same<T, double>::value) || (std::is_same<T, time_t>::value)) // check int, float, double type
    {
        T number;
        ifs.read(reinterpret_cast<char *>(&number), sizeof(number));
        return number;
    }
    else if constexpr (std::is_same<T, std::vector<std::string>>::value) // check vector of string type
    {
        int size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        std::vector<std::string> buffer(size);
        for (size_t i = 0; i < size; i++)
        {
            std::string data = readData<std::string>(ifs);
            buffer.push_back(data);
        }
        return buffer;
    }
}

template <typename T>
void writeData(std::ofstream &ofs, T data) // write data to file (used only for POD and non-POD types)
{
    if constexpr (std::is_same<T, std::string>::value) // check string type
    {
        size_t size = data.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        ofs.write(data.c_str(), size);
    }
    else if constexpr ((std::is_same<T, int>::value) || (std::is_same<T, float>::value) || (std::is_same<T, double>::value) || (std::is_same<T, time_t>::value)) // check int, float, double type
    {
        ofs.write(reinterpret_cast<const char *>(&data), sizeof(data));
    }
    else if constexpr (std::is_same<T, std::vector<std::string>>::value) // check vector of string type
    {
        int size = data.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (auto it : data)
        {
            writeData<std::string>(ofs, it);
        }
    }
}

#endif