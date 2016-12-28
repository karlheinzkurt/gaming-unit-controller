
#include "../include/CUnitCounterFactory.h"
#include "../include/CUnitCounter.h"

#include <sstream>

#include <boost/property_tree/ptree.hpp>

namespace Lib
{
namespace Controller
{
   CUnitCounterFactory::CUnitCounterFactory(std::chrono::seconds updateCycle) :
      m_updateCycle(updateCycle)
   {}
   
   std::unique_ptr<IUnitCounter> CUnitCounterFactory::create(Unit unit, std::chrono::seconds limit)
   {
      switch(unit)
      {
         case Unit::Day:  return std::make_unique<Internal::CDayCounter >(limit, m_updateCycle); break;
         case Unit::Week: return std::make_unique<Internal::CWeekCounter>(limit, m_updateCycle); break;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }
   
   std::unique_ptr<IUnitCounter> CUnitCounterFactory::create(boost::property_tree::ptree const& ptCounter)
   {
      std::istringstream is(ptCounter.get("unit", ""));
      Unit unit;
      is >> unit;
      std::chrono::seconds const limit(ptCounter.get("limit", 0));
      std::chrono::seconds const active(ptCounter.get("active", 0));
      IUnitCounter::time_point_type const previousRun(IUnitCounter::time_point_type::duration(ptCounter.get<int>("previousRun")));
      
      /** Update cycle has to be used from parameter, otherwise it could 
       *  lead to not properly working counters, because it has to match the cycle the update
       *  methods for counters get normally called and this can change, so we should not store it. */
      
      switch (unit)
      {
         case Unit::Day:  return std::make_unique<Internal::CDayCounter>(limit, m_updateCycle, active, previousRun); break;
         case Unit::Week: return std::make_unique<Internal::CWeekCounter>(limit, m_updateCycle, active, previousRun); break;
      }
      throw CUnsupportedUnitException("Unsupported unit");
   }
}}
