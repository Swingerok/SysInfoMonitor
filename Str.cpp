#include <algorithm>
#include "Str.h"

std::string Str::ltrim (std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
};

uint64_t Str::getint (std::string str) {
    str.erase(std::remove_if(
                      str.begin(),
                      str.end(),
                      [](char ch) {
                          return !std::isdigit(ch);
                      }),
              str.end()
    );
    return std::stoi(str);
};

double Str::getdouble (std::string str) {
    str.erase(std::remove_if(
                      str.begin(),
                      str.end(),
                      [](char ch) {
                          if (ch == '.') {
                              return false;
                          }
                          return !std::isdigit(ch);
                      }),
              str.end()
    );
    return std::stod(str);
};

std::string Str::getval (std::string str) {

    int i = 0;
    do {
        i++;
    } while (str[i] != ':');
    return ltrim(str.substr(i + 1, str.size()));
};

std::vector<std::string> Str::split(std::string str) {
    str = ltrim(str);
    std::vector<std::string> vec;
    auto it = str.begin();

    std::string temp = "";

    while (it != str.end()) {
        if (*it != ' ') {
            temp.push_back(*it);
        } else {
            vec.push_back(temp);
            temp.clear();
        }
        it++;
    }

    return std::move(vec);
}

bool Str::starts_with(const std::string &line, const std::string &sub) {
    auto it1 = line.begin();
    auto it2 = sub.begin();

    if (line.size() < sub.size()) {
        return false;
    }

    while (it2 != sub.end()) {
        if ((*it1) != (*it2)) {
            return false;
        }
        it1++;
        it2++;
    }

    return true;
}

void Str::erase(std::string& str, const char &ch) {
    auto it = str.begin();
    while (it != str.end()) {
        if ((*it) == ch) {
            break;
        }
        it++;
    }
    str.erase(it);
}

