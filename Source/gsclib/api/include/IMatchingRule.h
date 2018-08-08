#pragma once

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

#include <string>
#include <list>
#include <set>
#include <memory>
#include <chrono>
#include <iosfwd> ///< fwd only

namespace GSC
{
namespace API
{   
   struct IMatchingRule
   {     
      typedef std::set<std::unique_ptr<IMatchingRule>> SetType;
            
      virtual ~IMatchingRule() = default;
      
      virtual bool matches(std::string const& string) const = 0; 
      
      virtual std::string toString() const = 0;
      
      virtual std::string const& getName() const = 0;
      
      virtual std::chrono::seconds getLimit() const = 0;
      
      virtual std::list<std::string> const& getWhitelist() const = 0;
      
      virtual std::list<std::string> const& getBlacklist() const = 0;
            
      virtual boost::property_tree::ptree serialize() const = 0;
   };
   
   bool operator==(IMatchingRule const& a, IMatchingRule const& b);
   bool operator!=(IMatchingRule const& a, IMatchingRule const& b);
   bool operator==(IMatchingRule::SetType const& a, IMatchingRule::SetType const& b);
   bool operator!=(IMatchingRule::SetType const& a, IMatchingRule::SetType const& b);
   std::ostream& operator<<(std::ostream& os, IMatchingRule const& rule);
}}
