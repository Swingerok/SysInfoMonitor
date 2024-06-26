#ifndef SYSINFOMONITOR_STR_H
#define SYSINFOMONITOR_STR_H

#include <iostream>
#include <string>
#include <vector>


class Str {
public:
    static std::string ltrim(std::string s);

    static uint64_t getint(std::string str);

    static double getdouble(std::string str);

    static std::string getval(std::string str);

    static std::vector<std::string> split(std::string str);

    static bool starts_with(const std::string& line, const std::string& sub);

    static void erase(std::string &str, const char &ch);
};


#endif
