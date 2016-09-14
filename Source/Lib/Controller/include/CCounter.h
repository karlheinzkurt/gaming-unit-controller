#pragma once

#include <chrono>
#include <iosfwd>

#include <boost/property_tree/ptree.hpp>

namespace Lib
{
namespace Controller
{   
   /** @brief Currently supported units for counters.
    */
   enum class Unit
   {
       Day
      ,Week
   };
   
   /** @brief Counter for a specific unit e.g. a day or a week.
    */
   struct IUnitCounter
   {
      typedef std::chrono::system_clock::time_point time_point_type;
      
      virtual ~IUnitCounter() = default;
      
      virtual Unit getUnit() const = 0;
      
      virtual std::chrono::seconds getLimit() const = 0;
      
      virtual std::chrono::seconds getActive() const = 0;
      
      virtual bool exceedsLimit(time_point_type const& point) const = 0;
      
      virtual void doUpdate(time_point_type const& point) = 0;
   };
   
   std::chrono::seconds getDefaultCycle();
   
   /** @brief Factory to create counters for different units.
    *  @remark This factory has no dependencies to created 
    *          objects and and the other way around.
    *  @throw CUnsupportedUnitException when requested unit is not supported.
    */
   struct CUnitCounterFactory
   {
      std::unique_ptr<IUnitCounter> create(Unit unit, std::chrono::seconds limit, std::chrono::seconds cycle = getDefaultCycle());
   };
 
   struct CUnsupportedUnitException : public std::logic_error { using std::logic_error::logic_error; };
  
   std::ostream& operator<<(std::ostream& os, Unit unit);
   std::istream& operator>>(std::istream& is, Unit& unit);

   std::ostream& operator<<(std::ostream& os, IUnitCounter const& counter);

   boost::property_tree::ptree serialize(IUnitCounter const&);
   std::unique_ptr<IUnitCounter> deserialize(boost::property_tree::ptree const&);
}}
      