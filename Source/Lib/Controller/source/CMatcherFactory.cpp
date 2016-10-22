
#include "../include/CMatcherFactory.h"
#include "../include/CMatcher.h"

#include <boost/property_tree/ptree.hpp>

#include <list>

namespace Lib
{
namespace Controller
{
     
   /** We might have different matcher types in future,
    *  so we can select then here between them.
    */
   
   std::unique_ptr<API::IMatcher> CMatcherFactory::create()
   {
      return std::move(std::make_unique<CMatcher>());
   }
   
   std::unique_ptr<API::IMatcher> CMatcherFactory::create(boost::property_tree::ptree const& ptMatch)
   {
      return CMatcher::deserialize(ptMatch);
   }
   
}}
