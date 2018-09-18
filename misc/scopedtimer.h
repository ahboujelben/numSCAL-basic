/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H

#include <chrono>
#include <unordered_map>
#include <iostream>
#include <fstream>

#define MEASURE_FUNCTION() \
  ScopedTimer timer { __func__ }

class ScopedTimer
{

public:
  using ClockType = std::chrono::steady_clock;

  ScopedTimer(const char *func)
      : function_{func}, start_{ClockType::now()}
  {
  }

  ScopedTimer(const ScopedTimer &) = delete;
  ScopedTimer(ScopedTimer &&) = delete;
  auto operator=(const ScopedTimer &) -> ScopedTimer & = delete;
  auto operator=(ScopedTimer &&) -> ScopedTimer & = delete;

  ~ScopedTimer()
  {
    using namespace std::chrono;
    auto stop = ClockType::now();
    auto duration = (stop - start_);
    auto ns = duration_cast<nanoseconds>(duration).count();
    profile[function_].first += ns;
    profile[function_].second++;
  }

  static void printProfileData()
  {
    std::ofstream profileData("Results/profileData.txt");
    profileData << "Function\tCalls\tT (ms)\tAvg. T per Call (ms)" << std::endl;
    for(auto it : ScopedTimer::profile)
    {
       profileData << it.first;
       profileData << "\t" << it.second.second;
       profileData << "\t" << it.second.first/1e6;
       profileData << "\t" << it.second.first/it.second.second/1e6;
       profileData << std::endl;
    }
  }

private:
  static std::unordered_map<const char *, std::pair<double, int>> profile;
  const char *function_ = {};
  const ClockType::time_point start_ = {};
};

#endif // SCOPEDTIMER_H
