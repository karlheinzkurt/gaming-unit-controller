
#include "Lib/Controller/include/Types.h"
#include "Lib/Controller/include/CCounter.h"

#include <chrono>
#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>

#include <boost/date_time/gregorian/gregorian.hpp>

class TEST;
using Lib::Controller::CUnitCounterFactory;
using Lib::Controller::CDayCounter;
using Lib::Controller::CWeekCounter;

CUnitCounterFactory counterFactory;

std::chrono::system_clock::time_point getTimePoint(std::string dateTime, bool dst = false)
{
   std::tm tm = {0};
   std::istringstream is(dateTime);
   //is.imbue(std::locale("de_DE.utf-8"));
   std::string const format("%d.%m.%Y %H:%M:%S");
   is >> std::get_time(&tm, format.c_str());
   if (is.fail()) 
   {
      throw std::invalid_argument("Invalid date time string '" + dateTime + "' expected '" + format + "' e.g. '31.12.2016 23:59:59'");
   }
   if (dst)
   {
      tm.tm_isdst = 1;
   }
   return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

TEST(DayCounter, UnitBegin_NewYear)
{
   CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("31.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2015 23:59:59")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 01:00:00")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("02.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("02.01.2016 00:00:00")));
}

TEST(DayCounter, UnitBegin_Spring)
{
   CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("29.02.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 01:02:00")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("02.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("02.03.2016 00:00:00")));
}

TEST(DayCounter, UnitBegin_BeginDST)
{
   CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 00:00:00", false)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 01:59:59", false)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 03:00:00", true)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 23:59:59", true)));
}

TEST(DayCounter, UnitBegin_Summer)
{
   CDayCounter c(std::chrono::hours(10));   
   EXPECT_EQ(getTimePoint("09.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("09.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 03:23:05", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("11.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("11.10.2016 00:00:00", true)));
}

TEST(DayCounter, UnitBegin_EndDST)
{
   CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", true)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", false)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 03:00:00", false)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59", false)));
}

TEST(DayCounter, UnitBegin_Fall)
{
   CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 03:03:59")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("01.11.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.11.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_NewYear)
{     
   CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("28.12.2015 00:00:00")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2015 23:59:59")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("03.01.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("04.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("04.01.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_Spring)
{     
   CWeekCounter c(std::chrono::hours(10));
   ///< Month with 29 days
   EXPECT_EQ(getTimePoint("22.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("28.02.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("29.02.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 00:00:23")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("06.03.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("07.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("07.03.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_BeginDST)
{
   CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 00:00:00", false)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 01:59:59", false)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 03:00:00", true)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 23:59:59", true)));
}

TEST(WeekCounter, UnitBegin_Summer)
{     
   CWeekCounter c(std::chrono::hours(10));
   ///< Month with 30 days | summer time
   EXPECT_EQ(getTimePoint("19.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("25.09.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("26.09.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("01.10.2016 09:23:04", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("02.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("03.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("03.10.2016 00:00:00", true)));
}

TEST(WeekCounter, UnitBegin_EndDST)
{
   CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", true)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", false)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 03:00:00", false)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59", false)));
}

TEST(WeekCounter, UnitBegin_Fall)
{     
   CWeekCounter c(std::chrono::hours(10));
   ///< Month with 31 days
   EXPECT_EQ(getTimePoint("19.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("25.12.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("26.12.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2016 05:00:23")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2017 23:59:59")));
   EXPECT_EQ(getTimePoint("02.01.2017 00:00:00"), c.getUnitBegin(getTimePoint("02.01.2017 00:00:00")));
}

TEST( CounterTest, Init )
{
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(1)));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()+std::chrono::hours(230)));
   EXPECT_FALSE(counter->exceedsLimit(std::chrono::system_clock::now()-std::chrono::hours(230)));
}

TEST( CounterTest, FirstUpdateDoesNotExceed )
{
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::seconds(1)));
   EXPECT_FALSE(counter->exceedsLimit(getTimePoint("26.12.2016 00:03:00")));
   counter->doUpdate(getTimePoint("26.12.2016 00:04:00"));
   EXPECT_FALSE(counter->exceedsLimit(getTimePoint("26.12.2016 00:04:00")));
   counter->doUpdate(getTimePoint("26.12.2016 00:05:00"));
   EXPECT_TRUE(counter->exceedsLimit(getTimePoint("26.12.2016 00:05:00")));
}

TEST( CounterTest, HourPerDay )
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::hours(1)));
   for ( unsigned int i(0); i < 60; ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, HoursPerDay )
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::hours(3)));
   for ( unsigned int i(0); i < (3 * 60); ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, HoursPerWeek )
{
   auto now(getTimePoint("03.03.2016 19:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Week, std::chrono::hours(15)));
   for ( unsigned int i(0); i < (15 * 60); ++i)
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
   auto now(getTimePoint("29.02.2016 23:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::hours(1)));
   for ( unsigned int i(0); i < (47 + (1 * 60)); ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, OverlappingWeek_OverlappingMonth )
{
   auto now(getTimePoint("28.02.2016 20:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Week, std::chrono::hours(15)));
   for ( unsigned int i(0); i < ((4 * 60 - 13) + (15 * 60)); ++i)
   {
      counter->doUpdate(now);
      EXPECT_FALSE(counter->exceedsLimit(now));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_TRUE(counter->exceedsLimit(now));
}

TEST( CounterTest, OverlappingWeek_WithinMonth )
{
   auto now(getTimePoint("06.03.2016 20:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Week, std::chrono::hours(15)));
   for ( unsigned int i(0); i < ((4 * 60 - 13) + (15 * 60)); ++i)
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
   auto now(getTimePoint("29.02.2016 09:13:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::hours(1)));
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
   auto now(getTimePoint("29.02.2016 07:59:00"));
   
   auto counter(counterFactory.create(Lib::Controller::Unit::Day, std::chrono::hours(1)));
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
   
   counter->doUpdate(now); ///< First run after long time goes emptygetTimePoint("29.02.2016 00:00:00")
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
