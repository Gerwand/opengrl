#pragma once

#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace grl {
#define PROFILE
#ifdef PROFILE

class TimeInterval
{
public:
    TimeInterval() { _time = std::chrono::high_resolution_clock::now(); }
    void finish()
    {
        _interval = std::chrono::high_resolution_clock::now() - _time;
    }

    long long getUS() { return _interval / std::chrono::microseconds(1); }
private:
    std::chrono::time_point<std::chrono::steady_clock> _time;
    std::chrono::nanoseconds _interval;
};

class Profiler
{
public:
    void addTime(const std::string &name, TimeInterval &time)
    {
        auto interval = time.getUS();
        auto all = _allTimes.insert(std::make_pair(name, std::vector<long long>()));
        all.first->second.push_back(interval);
        auto sum = _sumTimes.insert(std::make_pair(name, 0));
        sum.first->second += interval;
    }

    void report(std::ostream &out)
    {
        auto its = _sumTimes.begin();
        for (auto it1 = _allTimes.begin(); it1 != _allTimes.end(); ++it1, ++its) {
            out<< it1->first << ": " << std::endl;
            auto el = std::minmax_element(it1->second.begin(), it1->second.end());
            out << "\tmin: " << *el.first << std::endl;
            out << "\tmax: " << *el.second << std::endl;
            out << "\tavg: " << its->second/it1->second.size() << std::endl << std::endl;
            out << "\tprobes: " << it1->second.size() << std::endl << std::endl;
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                out << "\t" << *it2 << std::endl;
            }
        }  
        out << "Done" << std::endl;
    }

    void reportShort(std::ostream &out)
    {
        auto its = _sumTimes.begin();
        for (auto it1 = _allTimes.begin(); it1 != _allTimes.end(); ++it1, ++its) {
            out<< it1->first << ": " << std::endl;
            auto el = std::minmax_element(it1->second.begin(), it1->second.end());
            out << "\tmin: " << *el.first << std::endl;
            out << "\tmax: " << *el.second << std::endl;
            out << "\tavg: " << its->second/it1->second.size() << std::endl << std::endl;           
            out << "\tprobes: " << it1->second.size() << std::endl << std::endl;
        }
        out << "Done" << std::endl;
}
private:
    std::map<std::string, std::vector<long long> > _allTimes;
    std::map<std::string, long long> _sumTimes;
};
#else
class TimeInverval
{
public:
    void finish() {}
    long long getUS() { return 0; }
};
#endif
}