
#include "../include/Types.h"
#include "../include/CCounter.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace Lib
{
namespace Controller
{
   
   AUnitCounterBase::AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle) : m_limit(limit), m_cycle(cycle), m_active(0), m_previousRun() {}
   
   AUnitCounterBase::AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle, std::chrono::seconds active, time_point_type previousRun) : m_limit(limit), m_cycle(cycle), m_active(active), m_previousRun(previousRun) {}
   
   bool AUnitCounterBase::exceedsLimit( time_point_type const& point ) const
   {
      return m_active >= m_limit;
   }
   
   std::chrono::seconds AUnitCounterBase::getLimit() const
   {
      return m_limit;
   }
   
   std::chrono::seconds AUnitCounterBase::getActive() const
   {
      return m_active;         
   }
   
   void AUnitCounterBase::doUpdate(time_point_type const& now)
   {
      /** Run timestamp gets updated in any case. 
       * */
      auto previousRun(getAndUpdate(m_previousRun, now));
      
      if (isNewUnit(now, previousRun))
      {
         /** First run in this unit, we just
          *  reset the active counter, update
          *  the update timestamp and leave.
          * */
         m_active = std::chrono::seconds( 0 );
         //getAndUpdate(m_previousUpdate, now);
         return;
      }
      
      if (!isConsecutiveRun(previousRun, now))
      {
         /** When this run is not directly
          *  following of a previous run, we
          *  just leave without updating update 
          *  timestamp but we updated run timestamp.
          * */
         return;
      }
      
      /** A normal run updates the update timestamp
       *  and adds the differnce onto the active counter.
       */
      m_active += std::chrono::duration_cast<std::chrono::seconds>( now - previousRun );
   }

   bool AUnitCounterBase::isNewUnit(time_point_type const& pointA, time_point_type const& pointB)
   {
      auto const unitA(getUnitBegin(pointA));
      auto const unitB(getUnitBegin(pointB));
      return unitA > unitB;
   }
   
   IUnitCounter::time_point_type AUnitCounterBase::getAndUpdate(time_point_type& toUpdate, time_point_type point)
   {
      std::swap(toUpdate, point);
      return point;
   }
   
   bool AUnitCounterBase::isConsecutiveRun(time_point_type const& previous, time_point_type const& now)
   {
      return (previous >= (now - 1.5 * m_cycle));
   }
   
   boost::property_tree::ptree AUnitCounterBase::serialize() const
   {
      boost::property_tree::ptree ptCounter;
      std::ostringstream os;
      os << getUnit();
      ptCounter.put( "unit", os.str() );
      ptCounter.put( "limit", m_limit.count() );
      ptCounter.put( "cycle", m_cycle.count() );
      ptCounter.put( "active", m_active.count() );
      ptCounter.put( "previousRun", m_previousRun.time_since_epoch().count() );
      return std::move(ptCounter);
   }
   
   Unit CWeekCounter::getUnit() const
   {
      return Unit::Week;
   }
      
   IUnitCounter::time_point_type CWeekCounter::getUnitBegin(time_point_type const& point)
   {
      namespace bg = boost::gregorian;
   
      auto const startOfWeek(bg::Monday); ///< Define start of the unit   
      
      auto const timeType(std::chrono::system_clock::to_time_t(point));
      tm const localTime(*localtime(&timeType));
      bg::date date(bg::date_from_tm(localTime));
                     
      if (date.day_of_week() != startOfWeek)
      {  date = bg::first_day_of_the_week_before(startOfWeek).get_date(date); }
      
      tm resultLocalTime = bg::to_tm(date);
      resultLocalTime.tm_isdst = localTime.tm_isdst; ///< This is not correct in the week DST switching happens
      resultLocalTime.tm_sec = resultLocalTime.tm_min = 0, resultLocalTime.tm_hour = 0;
      return std::chrono::system_clock::from_time_t(mktime(&resultLocalTime));
   }
   
   Unit CDayCounter::getUnit() const
   {
      return Unit::Day;
   }
   
   IUnitCounter::time_point_type CDayCounter::getUnitBegin(time_point_type const& point)
   {
      auto const timeType(std::chrono::system_clock::to_time_t(point));
      tm resultLocalTime(*localtime(&timeType));
      resultLocalTime.tm_sec = resultLocalTime.tm_min = resultLocalTime.tm_hour = 0;
      resultLocalTime.tm_isdst = resultLocalTime.tm_isdst; ///< This is not correct in the week DST switching happens
      return std::chrono::system_clock::from_time_t(mktime(&resultLocalTime));
   }
   
   std::chrono::seconds getDefaultCycle()
   {
      return std::chrono::seconds(60);
   }
   
   std::unique_ptr<IUnitCounter> CUnitCounterFactory::create(Unit unit, std::chrono::seconds limit, std::chrono::seconds cycle)
   {
      switch(unit)
      {
         case Unit::Day:  return std::make_unique<CDayCounter >(limit, cycle); break;
         case Unit::Week: return std::make_unique<CWeekCounter>(limit, cycle); break;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }
   
   std::unique_ptr<IUnitCounter> CUnitCounterFactory::create(boost::property_tree::ptree const& ptCounter)
   {
      std::istringstream is(ptCounter.get("unit", ""));
      Unit unit;
      is >> unit;
      std::chrono::seconds const limit(ptCounter.get("limit", 0));
      std::chrono::seconds const cycle(ptCounter.get("cycle", getDefaultCycle().count()));
      std::chrono::seconds const active(ptCounter.get("active", 0));
      IUnitCounter::time_point_type const previousRun(CDayCounter::time_point_type::duration(ptCounter.get("previousRun", 0)));
      switch (unit)
      {
         case Unit::Day:  return std::make_unique<CDayCounter>(limit, cycle, active, previousRun); break;
         case Unit::Week: return std::make_unique<CWeekCounter>(limit, cycle, active, previousRun); break;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }  
   
   std::ostream& operator<<(std::ostream& os, Unit unit)
   {
      switch(unit)
      {
         case Unit::Day:  return os << "Day";  break;
         case Unit::Week: return os << "Week"; break;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }
   
   std::istream& operator>>(std::istream& is, Unit& unit)
   {
      std::string u;
      is >> u;
      if (boost::iequals("Day", u))
      {
         unit = Unit::Day;
         return is;
      }
      else
      if (boost::iequals("Week", u))
      {
         unit = Unit::Week;
         return is;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }
   
   std::ostream& operator<<(std::ostream& os, IUnitCounter const& counter)
   {
      return os 
         << "Unit: "   << counter.getUnit()           << ", "
         << "Limit: "  << counter.getLimit().count()  << "s, "
         << "Active: " << counter.getActive().count() << "s";         
   }   
}}
