#pragma once

#include "gsclib/api/include/IUnitCounter.h"

#include <chrono>
#include <memory>

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

namespace GSC
{
namespace Common
{
  /** @brief Factory to create counters for different units.
    * @remark This factory has no dependencies to created 
    *         objects and and the other way around.
    * @throw CUnsupportedUnitException when requested unit is not supported.
    */
   struct CUnitCounterFactory
   {
      explicit CUnitCounterFactory(std::chrono::seconds updateCycle = API::getDefaultUpdateCycle());
      
      std::unique_ptr<API::IUnitCounter> create(API::Unit unit);
      std::unique_ptr<API::IUnitCounter> create(boost::property_tree::ptree const&);
      
   private:
      std::chrono::seconds m_updateCycle;
   };
}}
