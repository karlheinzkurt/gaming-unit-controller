
#include "gsclib/common/include/CUnitCounter.h"
#include "gsclib/common/include/CUnitCounterFactory.h"

#include <chrono>
#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time/local_time.hpp>

using namespace GSC::Common;

CUnitCounterFactory counterFactory;
static auto const one(boost::rational<int>(1, 1));

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

// TODO This test is not working in arbitrary time zones, since
// it breaks when executed in azure somewhere else.
//
// TEST(local_date_time, time_point_convertion)
// {
//    auto const point(getTimePoint("27.03.2016 03:00:00", true));
//    auto const time(boost::posix_time::from_time_t(std::chrono::system_clock::to_time_t(point)));
//    boost::local_time::time_zone_ptr const zone(new boost::local_time::posix_time_zone("CET+1CEST01:00:00,M3.5.0/2,M10.5.0/3"));
//    boost::local_time::local_date_time localTime(time, zone);

//    auto resultTm(to_tm(localTime));
//    auto resultPoint(std::chrono::system_clock::from_time_t(mktime(&resultTm)));

//    EXPECT_EQ(point, resultPoint);
// }

TEST(DayCounter, UnitBegin_NewYear)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("31.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2015 23:59:59")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 01:00:00")));
   EXPECT_EQ(getTimePoint("01.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("02.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("02.01.2016 00:00:00")));
}

TEST(DayCounter, UnitBegin_Spring)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("29.02.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 01:02:00")));
   EXPECT_EQ(getTimePoint("01.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("02.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("02.03.2016 00:00:00")));
}

TEST(DayCounter, UnitBegin_BeginDST)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 00:00:00", false)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 01:59:59", false)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 03:00:00", true)));
   EXPECT_EQ(getTimePoint("27.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 23:59:59", true)));
}

TEST(DayCounter, UnitBegin_Summer)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("09.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("09.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 03:23:05", true)));
   EXPECT_EQ(getTimePoint("10.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("10.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("11.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("11.10.2016 00:00:00", true)));
}

TEST(DayCounter, UnitBegin_EndDST)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", true)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", false)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 03:00:00", false)));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59", false)));
}

TEST(DayCounter, UnitBegin_Fall)
{
   Internal::CDayCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("30.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 03:03:59")));
   EXPECT_EQ(getTimePoint("31.10.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.10.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("01.11.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.11.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_NewYear)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("28.12.2015 00:00:00")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2015 23:59:59")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("28.12.2015 00:00:00"), c.getUnitBegin(getTimePoint("03.01.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("04.01.2016 00:00:00"), c.getUnitBegin(getTimePoint("04.01.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_Spring)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   ///< Month with 29 days
   EXPECT_EQ(getTimePoint("22.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("28.02.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("29.02.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.03.2016 00:00:23")));
   EXPECT_EQ(getTimePoint("29.02.2016 00:00:00"), c.getUnitBegin(getTimePoint("06.03.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("07.03.2016 00:00:00"), c.getUnitBegin(getTimePoint("07.03.2016 00:00:00")));
}

TEST(WeekCounter, UnitBegin_BeginDST)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 00:00:00", false)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 01:59:59", false)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 03:00:00", true)));
   EXPECT_EQ(getTimePoint("21.03.2016 00:00:00", false), c.getUnitBegin(getTimePoint("27.03.2016 23:59:59", true)));
}

TEST(WeekCounter, UnitBegin_Summer)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   ///< Month with 30 days | summer time
   EXPECT_EQ(getTimePoint("19.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("25.09.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("26.09.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("01.10.2016 09:23:04", true)));
   EXPECT_EQ(getTimePoint("26.09.2016 00:00:00", true), c.getUnitBegin(getTimePoint("02.10.2016 23:59:59", true)));
   EXPECT_EQ(getTimePoint("03.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("03.10.2016 00:00:00", true)));
}

TEST(WeekCounter, UnitBegin_EndDST)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 00:00:00", true)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", true)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 02:00:00", false)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 03:00:00", false)));
   EXPECT_EQ(getTimePoint("24.10.2016 00:00:00", true), c.getUnitBegin(getTimePoint("30.10.2016 23:59:59", false)));
}

TEST(WeekCounter, UnitBegin_Fall)
{
   Internal::CWeekCounter c(std::chrono::hours(10));
   ///< Month with 31 days
   EXPECT_EQ(getTimePoint("19.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("25.12.2016 23:59:59")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("26.12.2016 00:00:00")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("31.12.2016 05:00:23")));
   EXPECT_EQ(getTimePoint("26.12.2016 00:00:00"), c.getUnitBegin(getTimePoint("01.01.2017 23:59:59")));
   EXPECT_EQ(getTimePoint("02.01.2017 00:00:00"), c.getUnitBegin(getTimePoint("02.01.2017 00:00:00")));
}

TEST(CounterTest, Init)
{
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   EXPECT_GT(one, counter->exceedsLimit(-std::chrono::seconds(1)));
   EXPECT_GT(one, counter->exceedsLimit(std::chrono::seconds(0)));
   EXPECT_GT(one, counter->exceedsLimit(std::chrono::seconds(1)));
   EXPECT_GT(one, counter->exceedsLimit(std::chrono::hours(230)));
}

TEST(CounterTest, FirstUpdateDoesNotExceed)
{
   auto const limit(std::chrono::seconds(1));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   EXPECT_GT(one, counter->exceedsLimit(limit));
   counter->doUpdate(getTimePoint("26.12.2016 00:04:00"));
   EXPECT_GT(one, counter->exceedsLimit(limit));
   counter->doUpdate(getTimePoint("26.12.2016 00:05:00"));
   EXPECT_LE(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, HourPerDay)
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   auto const limit(std::chrono::hours(1));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   for (unsigned int i(0); i < 60; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, HoursPerDay)
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   auto const limit(std::chrono::hours(3));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   for (unsigned int i(0); i < (3 * 60); ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_LE(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, HoursPerWeek)
{
   auto now(getTimePoint("03.03.2016 19:13:00"));
   auto const limit(std::chrono::hours(15));
   auto counter(counterFactory.create(GSC::API::Unit::Week));
   for (unsigned int i(0); i < (15 * 60); ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, OverlappingDay)
{
   auto now(getTimePoint("29.02.2016 23:13:00"));
   auto const limit(std::chrono::hours(1));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   for (unsigned int i(0); i < (47 + (1 * 60)); ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, OverlappingWeek_OverlappingMonth)
{
   auto now(getTimePoint("28.02.2016 20:13:00"));
   auto const limit(std::chrono::hours(15));
   auto counter(counterFactory.create(GSC::API::Unit::Week));
   for (unsigned int i(0); i < ((4 * 60 - 13) + (15 * 60)); ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, OverlappingWeek_WithinMonth)
{
   auto now(getTimePoint("06.03.2016 20:13:00"));
   auto const limit(std::chrono::hours(15));
   auto counter(counterFactory.create(GSC::API::Unit::Week));
   for (unsigned int i(0); i < ((4 * 60 - 13) + (15 * 60)); ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);
   counter->doUpdate(now);
   EXPECT_LT(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, HourPerDay_Split2)
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   auto const limit(std::chrono::hours(1));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   for (unsigned int i(0); i < 30; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }

   now += std::chrono::hours(2);

   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_GT(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);

   for (unsigned int i(0); i < 30; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }
   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
}

TEST(CounterTest, HourPerDay_Split4)
{
   auto now(getTimePoint("29.02.2016 07:59:00"));
   auto const limit(std::chrono::hours(1));
   auto counter(counterFactory.create(GSC::API::Unit::Day));
   for (unsigned int i(0); i < 10; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }

   now += std::chrono::hours(1);

   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_GT(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);

   for (unsigned int i(0); i < 15; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }

   now += std::chrono::minutes(15);

   counter->doUpdate(now); ///< First run after long time goes empty
   EXPECT_GT(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);

   for (unsigned int i(0); i < 25; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }

   now += std::chrono::hours(3);

   counter->doUpdate(now); ///< First run after long time goes emptygetTimePoint("29.02.2016 00:00:00")
   EXPECT_GT(one, counter->exceedsLimit(limit));
   now += std::chrono::minutes(1);

   for (unsigned int i(0); i < 10; ++i)
   {
      counter->doUpdate(now);
      EXPECT_GT(one, counter->exceedsLimit(limit));
      now += std::chrono::minutes(1);
   }

   counter->doUpdate(now);
   EXPECT_EQ(one, counter->exceedsLimit(limit));
}

TEST(Counter, SerializeRoundTrip)
{
   auto now(getTimePoint("29.02.2016 09:13:00"));
   auto source(counterFactory.create(GSC::API::Unit::Day));
   source->doUpdate(now);
   source->doUpdate(now + std::chrono::minutes(1));
   source->doUpdate(now + std::chrono::minutes(2));
   boost::property_tree::ptree pt = source->serialize();
   auto destination = counterFactory.create(pt);
   EXPECT_TRUE(source->getActive().count() > 0);
   EXPECT_EQ(*source, *destination);
}
