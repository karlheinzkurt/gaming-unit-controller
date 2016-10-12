#pragma once

/** Do not include this file directly, use CUnitCounterFactory to create instances.
    This is an internal header file.
*/

#include "IUnitCounter.h"

#include <chrono>
#include <iosfwd>

#include <boost/property_tree/ptree_fwd.hpp>

namespace Lib
{
namespace Controller
{   
namespace Internal ///< Internal
{
   /** @brief Base class for counters for any unit.
    */
   struct AUnitCounterBase : public IUnitCounter
   {
      AUnitCounterBase(std::chrono::seconds limit, std::chrono::seconds cycle = getDefaultUpdateCycle());
      
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
   
   /** @brief Counter monitoring activities using a week as a unit.
    *  @remark Use the factory to create an instance.
    */
   struct CWeekCounter : public AUnitCounterBase
   {
      using AUnitCounterBase::AUnitCounterBase;
      
      virtual Unit getUnit() const override;
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override;
   };
   
   /** @brief Counter monitoring activities using a day as a unit.
    *  @remark Use the factory to create an instance.
    */
   struct CDayCounter : public AUnitCounterBase
   {  
      using AUnitCounterBase::AUnitCounterBase;
    
      virtual Unit getUnit() const override;
      
      virtual time_point_type getUnitBegin(time_point_type const& point) override;
   };
}}}
      