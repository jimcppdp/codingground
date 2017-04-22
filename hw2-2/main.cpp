#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define YEAR1900 70+24*17
#define YEAR2000 31+24*7

#define TEST 1
#if TEST
  #define ITEM_COUNT 10  // for test only
#else
  #define ITEM_COUNT 100000
#endif


class TRADE_DATE {
  public:
    int ID;
    tuple <int, int, int> begin_date;
    tuple <int, int, int> end_date;
    int duration;
};

// this class template is got from others
template <class T>
class uniform_time_distribution{
public:
  uniform_time_distribution(T start, T end)
    : m_start(start), m_end(end),
    m_seconds(std::chrono::duration_cast<std::chrono::seconds>(end - start))
  {}

  template <class Generator>
  T operator()(Generator && g){
    std::uniform_int_distribution<std::chrono::seconds::rep> d(0, m_seconds.count());

    return m_start + std::chrono::seconds(d(g));
  }

private:
  T m_start;
  T m_end;
  std::chrono::seconds m_seconds;
};

// this class template is got from others
template <class T>
T randomTime(T start, T end){
  static std::random_device rd;
  static std::mt19937 gen(rd());

  uniform_time_distribution<T> t(start, end);

  return t(gen);
}

void randomGenerator(std::vector<TRADE_DATE>& tds)
{
  
  time_point <system_clock,duration<int>> tp_seconds (duration<int>(0));
  system_clock::time_point tp (tp_seconds);
  system_clock::time_point rand_begin = tp - hours(24 * 365 * YEAR1900);
  system_clock::time_point rand_end = tp + hours(24 * 365 * YEAR2000);

  std::random_device rd;  
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<int> dis1(1, 10);

  for (int n = 0; n < ITEM_COUNT; ++n) {
    TRADE_DATE td;
    auto time = randomTime(rand_begin, rand_end);
    auto ttime = system_clock::to_time_t(time);
    tm utc_tm = *gmtime(&ttime); 

    td.duration = dis1(gen);
    td.ID = n;
    int begin_year = utc_tm.tm_year + 1900;
    int begin_month = utc_tm.tm_mon + 1;
    int begin_day = utc_tm.tm_mday;
    auto end_time = time + hours(24*td.duration);
    auto tend_time = system_clock::to_time_t(end_time);
    utc_tm = *gmtime(&tend_time);     
    int end_year = utc_tm.tm_year + 1900;
    int end_month = utc_tm.tm_mon + 1;
    int end_day = utc_tm.tm_mday;
    td.begin_date = make_tuple(begin_year, begin_month, begin_day);
    td.end_date = make_tuple(end_year, end_month, end_day);
    tds.push_back(td); 

  }
  
}


int main(int argc, char const *argv[])
{
  std::vector<TRADE_DATE> tds;
  
  randomGenerator(tds);

  for (auto td : tds) {
      std::cout << td.ID << " : " << td.duration << " : " << 
        std::get<0>(td.begin_date) << " : " << 
        std::get<1>(td.begin_date) << " : " << 
        std::get<2>(td.begin_date) << " : " <<
        std::get<0>(td.end_date) << " : " << 
        std::get<1>(td.end_date) << " : " << 
        std::get<2>(td.end_date) << '\n';
  }
    
  
}

