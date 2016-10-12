
#include "../include/IUnitCounter.h"

#include <iostream>

#include <boost/algorithm/string/predicate.hpp>

namespace Lib
{
namespace Controller
{
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
   
   std::chrono::seconds getDefaultUpdateCycle()
   {
      return std::chrono::seconds(60);
   }
}}
