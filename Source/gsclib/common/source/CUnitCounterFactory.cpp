
#include "../include/CUnitCounterFactory.h"
#include "../include/CUnitCounter.h"

#include <sstream>

#include <boost/property_tree/ptree.hpp>

namespace GSC
{
namespace Common
{
   CUnitCounterFactory::CUnitCounterFactory(std::chrono::seconds updateCycle) :
      m_updateCycle(updateCycle)
   {}
   
   std::unique_ptr<API::IUnitCounter> CUnitCounterFactory::create(API::Unit unit, std::chrono::seconds limit)
   {
      switch(unit)
      {
         case API::Unit::Day:  return std::make_unique<Internal::CDayCounter >(limit, m_updateCycle); break;
         case API::Unit::Week: return std::make_unique<Internal::CWeekCounter>(limit, m_updateCycle); break;
      }
      throw API::CUnsupportedUnitException("Unsupported unit");
   }
   
   std::unique_ptr<API::IUnitCounter> CUnitCounterFactory::create(boost::property_tree::ptree const& ptCounter)
   {
      std::istringstream is(ptCounter.get("unit", ""));
      API::Unit unit;
      is >> unit;
      std::chrono::seconds const limit(ptCounter.get("limit", 0));
      std::chrono::seconds const active(ptCounter.get("active", 0));
      API::IUnitCounter::time_point_type const previousRun(API::IUnitCounter::time_point_type::duration(ptCounter.get<API::IUnitCounter::time_point_type::rep>("previousRun")));
      
      /** Update cycle has to be used from parameter, otherwise it could 
       *  lead to not properly working counters, because it has to match the cycle the update
       *  methods for counters get normally called and this can change, so we should not store it. */
      
      switch (unit)
      {
         case API::Unit::Day:  return std::make_unique<Internal::CDayCounter>(limit, m_updateCycle, active, previousRun); break;
         case API::Unit::Week: return std::make_unique<Internal::CWeekCounter>(limit, m_updateCycle, active, previousRun); break;
      }
      throw API::CUnsupportedUnitException("Unsupported unit");
   }
}}
