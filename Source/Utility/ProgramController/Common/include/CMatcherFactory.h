#pragma once

#include "IMatcher.h"

#include <memory>

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

namespace Utility
{
namespace ProgramController
{
  /** @brief Factory to create process matcher.
    * @remark This factory has no dependencies to created 
    *         objects and and the other way around.
    */
   struct CMatcherFactory
   {
      std::unique_ptr<API::IMatcher> create();
      std::unique_ptr<API::IMatcher> create(boost::property_tree::ptree const&);
   };
}}