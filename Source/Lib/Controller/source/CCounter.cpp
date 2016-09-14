
#include "../include/Types.h"
#include "../include/CCounter.h"

#include <boost/algorithm/string/predicate.hpp>

namespace Lib
{
namespace Controller
{
   
   struct AUnitCounterBase : public IUnitCounter
   {
      AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle) : m_limit(limit), m_cycle(cycle), m_active(0), /*m_previousUpdate(), */m_previousRun() {}
      
      virtual bool exceedsLimit( time_point_type const& point ) const override
      {
         return m_active >= m_limit;
      }
      
      virtual std::chrono::seconds getLimit() const override
      {
         return m_limit;
      }
      
      virtual std::chrono::seconds getActive() const override
      {
         return m_active;         
      }
      
      virtual void doUpdate(time_point_type const& now) override
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
         //auto previousUpdate(getAndUpdate(m_previousUpdate, now));         
         m_active += std::chrono::duration_cast<std::chrono::seconds>( now - previousRun );
      }

      /** Internal methods
       */
      bool isNewUnit(time_point_type const& pointA, time_point_type const& pointB)
      {
         return getUnitBegin(pointA) > getUnitBegin(pointB);
      }
      
      time_point_type getAndUpdate(time_point_type& toUpdate, time_point_type point)
      {
         std::swap(toUpdate, point);
         return point;
      }
      
      bool isConsecutiveRun(time_point_type const& previous, time_point_type const& now)
      {
         return (previous >= (now - 1.5 * m_cycle));
      }
      
      virtual time_point_type getUnitBegin(time_point_type const& point) = 0;
      
      std::chrono::seconds const m_limit;
      std::chrono::seconds const m_cycle;
      std::chrono::seconds m_active;
      //time_point_type m_previousUpdate;
      time_point_type m_previousRun;
   };
   
   struct CWeekCounter : public AUnitCounterBase
   {
      using AUnitCounterBase::AUnitCounterBase;
      
      virtual Unit getUnit() const override
      {
         return Unit::Week;
      }
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override
      {
         throw std::logic_error("Not yet implemented");
      }
   };
   
   struct CDayCounter : public AUnitCounterBase
   {      
      using AUnitCounterBase::AUnitCounterBase;
      
      virtual Unit getUnit() const override
      {
         return Unit::Day;
      }
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override
      {
         return time_point_type(std::chrono::duration_cast<days>(point.time_since_epoch()));
      }
   };
   
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
   
   
   std::ostream& operator<<(std::ostream& os, Unit unit)
   {
      switch(unit)
      {
         case Unit::Day:  return os << "Day"; break;
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
   
   boost::property_tree::ptree serialize(IUnitCounter const& counter_)
   {
      auto const& counter(static_cast<AUnitCounterBase const&>(counter_));
      boost::property_tree::ptree ptCounter;
      std::ostringstream os;
      os << counter.getUnit();
      ptCounter.put( "unit", os.str() );
      ptCounter.put( "limit", counter.m_limit.count() );
      ptCounter.put( "cycle", counter.m_cycle.count() );
      ptCounter.put( "active", counter.m_active.count() );
      //ptCounter.put( "previousUpdate", counter.m_previousUpdate.time_since_epoch().count() );
      ptCounter.put( "previousRun", counter.m_previousRun.time_since_epoch().count() );
      return std::move(ptCounter);
   }
   
   std::unique_ptr<IUnitCounter> deserialize(boost::property_tree::ptree const& ptCounter)
   {
      std::istringstream is(ptCounter.get("unit", ""));
      Unit unit;
      is >> unit;
      std::chrono::seconds const limit(ptCounter.get("limit", 0));
      std::chrono::seconds const cycle(ptCounter.get("cycle", getDefaultCycle().count()));
      std::unique_ptr<AUnitCounterBase> counter; ///< We do need the base class type here, so we cannot use the factory!
      switch (unit)
      {
         case Unit::Day:  counter.reset(new CDayCounter(limit, cycle)); break;
         case Unit::Week: counter.reset(new CWeekCounter(limit, cycle)); break;
         default: throw CUnsupportedUnitException("Unsupported unit"); break;
      }
      counter->m_active = std::chrono::seconds(ptCounter.get("active", 0));
      //counter->m_previousUpdate = CDayCounter::time_point_type(CDayCounter::time_point_type::duration(ptCounter.get("previousUpdate", 0)));
      counter->m_previousRun = CDayCounter::time_point_type(CDayCounter::time_point_type::duration(ptCounter.get("previousRun", 0)));
      return std::move(counter);
   }
   
}}
