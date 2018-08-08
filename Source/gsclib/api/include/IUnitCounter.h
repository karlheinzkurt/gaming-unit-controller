#pragma once

#include <chrono>
#include <stdexcept>
#include <iosfwd> ///< fwd only

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

namespace GSC
{
namespace API
{   
   /** @brief Currently supported units for counters.
    *  @todo Add support for monthly units.
    */
   enum class Unit
   {
       Day
      ,Week
   };
   
   std::ostream& operator<<(std::ostream& os, Unit unit);
   std::istream& operator>>(std::istream& is, Unit& unit);
   
   /** @return default value for update cycle parameter.
    */
   std::chrono::seconds getDefaultUpdateCycle();
   
   /** @brief Interface for a counter for a specific amount of time
    *         within a given unit @see Unit
    */
   struct IUnitCounter
   {
      typedef std::chrono::system_clock::time_point time_point_type;
      
      virtual ~IUnitCounter() = default;
      
      virtual std::string toString() const = 0;
      
      /** @return Unit used to calculate whether the limit gets exceeded.
       */
      virtual Unit getUnit() const = 0;
            
      /** @return Seconds marked as active within the current unit.
       */
      virtual std::chrono::seconds getActive() const = 0;
      
      /** @param limit to be checked if exceeded
          @return true when the given limit is exceeded for 
                  the current unit, otherwise false.
       */
      virtual bool exceedsLimit(std::chrono::seconds limit) const = 0;
      
      /** @param point in time to be used to update the internal counters. 
       */
      virtual void doUpdate(time_point_type const& point) = 0;
      
      /** @return ptree object containing the serialized state of the 
                  object for storage.
       */
      virtual boost::property_tree::ptree serialize() const = 0;
   };
   
   bool operator==(IUnitCounter const& a, IUnitCounter const& b);
   std::ostream& operator<<(std::ostream& os, IUnitCounter const& counter);
   
   struct CUnsupportedUnitException : public std::logic_error { using std::logic_error::logic_error; };
}}
