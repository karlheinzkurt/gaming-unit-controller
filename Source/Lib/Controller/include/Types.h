
#include <chrono>

typedef std::chrono::duration< std::chrono::hours::rep, std::ratio_multiply< std::chrono::hours::period, std::ratio< 24 > >::type > days;
typedef std::chrono::duration< days::rep, std::ratio_multiply< days::period, std::ratio< 7 > >::type > weeks;
