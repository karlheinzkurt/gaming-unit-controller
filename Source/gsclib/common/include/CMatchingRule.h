#pragma once

#include "gsclib/infrastructure/api/include/IProcess.h"

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

#include <string>
#include <list>
#include <set>
#include <initializer_list>
#include <regex>
#include <iosfwd> ///< fwd only

namespace GSC
{
namespace Common
{   
   struct CMatchingRule
   {     
      typedef std::set<CMatchingRule> SetType;
      
      CMatchingRule(std::string name, std::initializer_list<std::string> whiteList, std::initializer_list<std::string> blackList);
      CMatchingRule(std::string name, std::list<std::string> whiteList, std::list<std::string> blackList);
      
      ~CMatchingRule(); ///< Keep to get forward declared type deleted properly
      
      CMatchingRule(CMatchingRule const&) = delete;
      CMatchingRule& operator=(CMatchingRule const&) = delete;
      
      CMatchingRule(CMatchingRule&&);
      CMatchingRule& operator=(CMatchingRule&&);
      
      bool matches(std::string const& string) const;
      
      std::string toString() const;
      
      std::string const& getName() const;
      
      std::list<std::string> const& getWhitelist() const;
      
      std::list<std::string> const& getBlacklist() const;
            
      boost::property_tree::ptree serialize() const;
      
      static CMatchingRule deserialize(boost::property_tree::ptree const& ptRule);
            
   private:
      std::string m_name;
      struct List;
      std::unique_ptr<List> m_whitelist, m_blacklist;
   };
   
   bool operator<(CMatchingRule const& a, CMatchingRule const& b);
   bool operator==(CMatchingRule const& a, CMatchingRule const& b);
   std::ostream& operator<<( std::ostream& os, CMatchingRule const& rule );
   
}}
