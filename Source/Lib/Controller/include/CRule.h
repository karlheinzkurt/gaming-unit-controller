#pragma once

#include "Lib/Infrastructure/API/include/IProcess.h"

#include <string>
#include <list>
#include <set>
#include <initializer_list>
#include <regex>
#include <iosfwd> ///< fwd only

namespace Lib
{
namespace Controller
{
namespace API
{
   
   struct CRule
   {     
      typedef std::set<CRule> SetType;
      
      CRule(std::string name, std::initializer_list<std::string> whiteList, std::initializer_list<std::string> blackList);
      CRule(std::string name, std::list<std::string> whiteList, std::list<std::string> blackList);
      
      ~CRule(); ///< Keep to get forward declared type deleted properly
      
      CRule(CRule const&) = delete;
      CRule& operator=(CRule const&) = delete;
      
      CRule(CRule&&);
      CRule& operator=(CRule&&);
      
      bool matches(std::string const& string) const;
      
      std::string toString() const;
      
      std::string const& getName() const;
      
      std::list<std::string> const& getWhitelist() const;
      
      std::list<std::string> const& getBlacklist() const;
      
      std::string const& getWhitelistString() const;
      
      std::string const& getBlacklistString() const;
      
      std::regex const& getWhitelistExpression() const;
      
      std::regex const& getBlacklistExpression() const;
            
   private:
      std::string m_name;
      struct List;
      std::unique_ptr<List> m_whitelist, m_blacklist;
   };
   
   bool operator<(CRule const& a, CRule const& b);
   bool operator==(CRule const& a, CRule const& b);
   std::ostream& operator<<( std::ostream& os, CRule const& rule );
   
}}}
