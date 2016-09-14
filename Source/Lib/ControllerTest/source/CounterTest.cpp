
#include "Lib/Controller/include/Types.h"
#include "Lib/Controller/include/CCounter.h"

#include <chrono>

#include <gtest/gtest.h>

using Lib::Controller::IUnitCounter;
using Lib::Controller::CUnitCounterFactory;

CUnitCounterFactory counterFactory;
IUnitCounter::time_point_type const someDay(days(17076)); ///< 01.10.2016 -> 17076 days since 1.1.1970

TEST( CounterTest, Init )
{
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(1)));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()+std::chrono::hours(230)));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()-std::chrono::hours(230)));
}

TEST( CounterTest, HourPerDay )
{
   IUnitCounter::time_point_type now(someDay + std::chrono::hours(9) + std::chrono::minutes(13));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(3600)));
   for ( unsigned int i(0); i < 60; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, OverlappingDay )
{
   IUnitCounter::time_point_type now(someDay - std::chrono::minutes(47));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(3600)));
   for ( unsigned int i(0); i < (47 + 60); ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, HourPerDay_Split2 )
{
   IUnitCounter::time_point_type now(someDay + std::chrono::hours(9) + std::chrono::minutes(13));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(3600)));
   for ( unsigned int i(0); i < 30; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   
   now += std::chrono::hours(2);
   
   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_FALSE(counter->exceedsLimit(now));
   now += std::chrono::minutes(1);
   
   for ( unsigned int i(0); i < 30; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, HourPerDay_Split4 )
{
   IUnitCounter::time_point_type now(someDay + std::chrono::hours(7) + std::chrono::minutes(59));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(3600)));
   for ( unsigned int i(0); i < 10; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   
   now += std::chrono::hours(1);
   
   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_FALSE(counter->exceedsLimit(now));
   now += std::chrono::minutes(1);
   
   for ( unsigned int i(0); i < 15; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   
   now += std::chrono::minutes(15);
   
   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_FALSE(counter->exceedsLimit(now));
   now += std::chrono::minutes(1);
   
   for ( unsigned int i(0); i < 25; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   
   now += std::chrono::hours(3);
   
   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_FALSE(counter->exceedsLimit(now));
   now += std::chrono::minutes(1);
   
   for ( unsigned int i(0); i < 10; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}
