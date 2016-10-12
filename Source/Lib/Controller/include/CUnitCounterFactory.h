#pragma once

#include "IUnitCounter.h"

#include <chrono>
#include <memory>

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

namespace Lib
{
namespace Controller
{
  /** @brief Factory to create counters for different units.
    * @remark This factory has no dependencies to created 
    *         objects and and the other way around.
    * @throw CUnsupportedUnitException when requested unit is not supported.
    */
   struct CUnitCounterFactory
   {
      explicit CUnitCounterFactory(std::chrono::seconds updateCycle = getDefaultUpdateCycle());
      
      std::unique_ptr<IUnitCounter> create(Unit unit, std::chrono::seconds limit);
      std::unique_ptr<IUnitCounter> create(boost::property_tree::ptree const&);
      
   private:
      std::chrono::seconds m_updateCycle;
   };
}}
