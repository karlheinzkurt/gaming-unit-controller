
#include "../include/CMatcherFactory.h"
#include "../include/CMatcher.h"

#include <boost/property_tree/ptree.hpp>

#include <list>

namespace Lib
{
namespace Controller
{
   
   namespace 
   {
   }
   
   std::unique_ptr<API::IMatcher> CMatcherFactory::create(boost::property_tree::ptree const& ptMatch)
   {
      auto matcher(std::make_unique<CMatcher>());      
      for (auto const& ptRule : ptMatch.get_child( "rules" ))
      {
         API::CRule rule;
         rule.name = ptRule.second.get< std::string >( "name" );
         for (auto const& ptWhitelist : ptRule.second.get_child( "whitelist" ))
         {
            rule.whitelist.emplace_back(ptWhitelist.second.get< std::string >("entry"));
         }
         auto optionalBlacklist(ptRule.second.get_child_optional( "blacklist" ));
         if (optionalBlacklist)
         {
            for (auto const& ptBlacklist : *optionalBlacklist)
            {
               rule.blacklist.emplace_back(ptBlacklist.second.get< std::string >("entry"));
            }
         }
         matcher->add(rule);
      }
      return matcher;
   }
   
}}
