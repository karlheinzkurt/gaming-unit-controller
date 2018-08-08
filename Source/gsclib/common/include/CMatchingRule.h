#pragma once

#include "gsclib/api/include/IMatchingRule.h"

#include "gsclib/infrastructure/api/include/IProcess.h"

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

#include <string>
#include <list>
#include <set>
#include <initializer_list>
#include <regex>

namespace GSC
{
namespace Common
{   
   struct CMatchingRule : API::IMatchingRule
   {     
      typedef std::set<CMatchingRule> SetType;
      
      CMatchingRule(std::string name, std::chrono::seconds limit, std::list<std::string> whiteList, std::list<std::string> blackList);
      
      virtual ~CMatchingRule(); ///< Keep to get forward declared type deleted properly
      
      CMatchingRule(CMatchingRule const&) = delete;
      CMatchingRule& operator=(CMatchingRule const&) = delete;
      
      CMatchingRule(CMatchingRule&&);
      CMatchingRule& operator=(CMatchingRule&&);
      
      virtual bool matches(std::string const& string) const override;
      
      virtual std::string toString() const override;
      
      virtual std::string const& getName() const override;
      
      virtual std::chrono::seconds getLimit() const override;
      
      virtual std::list<std::string> const& getWhitelist() const override;
      
      virtual std::list<std::string> const& getBlacklist() const override;
            
      virtual boost::property_tree::ptree serialize() const override;
      
      static std::unique_ptr<API::IMatchingRule> deserialize(boost::property_tree::ptree const& ptRule);
            
   private:
      std::string m_name;
      std::chrono::seconds m_limit;
      struct List;
      std::unique_ptr<List> m_whitelist, m_blacklist;
   };
}}
