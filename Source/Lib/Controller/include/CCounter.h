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
   
   /** @brief Interface for a counter for a specific unit @see Unit
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
      
      virtual boost::property_tree::ptree serialize() const = 0;
   };
   
   /** @return default value for cycle parameter.
    */
   std::chrono::seconds getDefaultCycle();
   
   /** @brief Internally used base class.
    */
   struct AUnitCounterBase : public IUnitCounter
   {
      AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle = getDefaultCycle());
      
      AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle, std::chrono::seconds active, time_point_type previousRun);
      
      virtual boost::property_tree::ptree serialize() const override;
      
   protected:      
      virtual bool exceedsLimit( time_point_type const& point ) const override;
      
      virtual std::chrono::seconds getLimit() const override;
           
      virtual std::chrono::seconds getActive() const override;
      
      virtual void doUpdate(time_point_type const& now) override;

      bool isNewUnit(time_point_type const& pointA, time_point_type const& pointB);
      
      time_point_type getAndUpdate(time_point_type& toUpdate, time_point_type point);
      
      bool isConsecutiveRun(time_point_type const& previous, time_point_type const& now);
      
      virtual time_point_type getUnitBegin(time_point_type const& point) = 0;
      
      std::chrono::seconds const m_limit;
      std::chrono::seconds const m_cycle;
      std::chrono::seconds m_active;
      time_point_type m_previousRun;
   };
   
   /** @brief Counter monitring activities using a week as a unit.
    */
   struct CWeekCounter : public AUnitCounterBase
   {
      using AUnitCounterBase::AUnitCounterBase;
      
      virtual Unit getUnit() const override;
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override;
   };
   
   /** @brief Counter monitring activities using a day as a unit.
    */
   struct CDayCounter : public AUnitCounterBase
   {  
      using AUnitCounterBase::AUnitCounterBase;
    
      virtual Unit getUnit() const override;
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override;
   };
   
   /** @brief Factory to create counters for different units.
    *  @remark This factory has no dependencies to created 
    *          objects and and the other way around.
    *  @throw CUnsupportedUnitException when requested unit is not supported.
    */
   struct CUnitCounterFactory
   {
      std::unique_ptr<IUnitCounter> create(Unit unit, std::chrono::seconds limit, std::chrono::seconds cycle = getDefaultCycle());
      std::unique_ptr<IUnitCounter> create(boost::property_tree::ptree const&);
   };
 
   struct CUnsupportedUnitException : public std::logic_error { using std::logic_error::logic_error; };
  
   std::ostream& operator<<(std::ostream& os, Unit unit);
   std::istream& operator>>(std::istream& is, Unit& unit);

   std::ostream& operator<<(std::ostream& os, IUnitCounter const& counter);
}}
      