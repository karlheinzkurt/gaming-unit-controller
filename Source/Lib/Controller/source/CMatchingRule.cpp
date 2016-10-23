
#include "../include/CMatchingRule.h"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>

#include <sstream>

namespace Lib
{
namespace Controller
{
namespace API
{
   
   struct CMatchingRule::List
   {
      List(std::initializer_list<std::string> list) :
          m_list(list)
         ,m_string(boost::algorithm::join(m_list, ", "))
         ,m_expression(std::string("(") + boost::algorithm::join(m_list, "|" ) + ")", std::regex_constants::icase)
      {}
      
      List(std::list<std::string> list) :
          m_list(list)
         ,m_string(boost::algorithm::join(m_list, ", "))
         ,m_expression(std::string("(") + boost::algorithm::join(m_list, "|" ) + ")", std::regex_constants::icase)
      {}
      
      std::list<std::string> m_list;
      std::string m_string; 
      std::regex m_expression;
   };
  
   CMatchingRule::CMatchingRule(std::string name, std::initializer_list<std::string> whitelist, std::initializer_list<std::string> blacklist) :
       m_name(name)
      ,m_whitelist(std::make_unique<List>(whitelist))
      ,m_blacklist(std::make_unique<List>(blacklist))
   {}
      
   CMatchingRule::CMatchingRule(std::string name, std::list<std::string> whitelist, std::list<std::string> blacklist) :
       m_name(name)
      ,m_whitelist(std::make_unique<List>(whitelist))
      ,m_blacklist(std::make_unique<List>(blacklist))
   {}
   
   CMatchingRule::~CMatchingRule() = default;
   CMatchingRule::CMatchingRule(CMatchingRule&&) = default;
   CMatchingRule& CMatchingRule::operator=(CMatchingRule&&) = default;
   
   bool CMatchingRule::matches(std::string const& string) const
   {
      if (std::regex_match(string, m_whitelist->m_expression))
      {  
         if (!std::regex_match(string, m_blacklist->m_expression))
         {   return true; }
      }
      return false;
   }
   
   std::string CMatchingRule::toString() const
   {
      std::ostringstream os;
      os << m_name
         << " (" << getWhitelistString() << "), "
         << "!(" << getBlacklistString() << ")";
      return os.str();
   }
   
   std::string const& CMatchingRule::getName() const
   {
      return m_name;
   }
   
   std::list<std::string> const& CMatchingRule::getWhitelist() const
   {
      return m_whitelist->m_list;
   }
      
   std::list<std::string> const& CMatchingRule::getBlacklist() const
   {
      return m_blacklist->m_list;
   }
   
   std::string const& CMatchingRule::getWhitelistString() const
   {
      return m_whitelist->m_string;
   }
      
   std::string const& CMatchingRule::getBlacklistString() const
   {
      return m_blacklist->m_string;
   }
   
   std::regex const& CMatchingRule::getWhitelistExpression() const
   {
      return m_whitelist->m_expression;
   }
   
   std::regex const& CMatchingRule::getBlacklistExpression() const
   {
      return m_blacklist->m_expression;
   }
   
   bool operator<(CMatchingRule const& a, CMatchingRule const& b)
   {
      return std::tie(a.getName(), a.getWhitelist(), a.getBlacklist()) < std::tie(b.getName(), b.getWhitelist(), b.getBlacklist());
   }
   
   bool operator==(CMatchingRule const& a, CMatchingRule const& b)
   {
      return std::tie(a.getName(), a.getWhitelist(), a.getBlacklist()) == std::tie(b.getName(), b.getWhitelist(), b.getBlacklist());
   }
   
   std::ostream& operator<<( std::ostream& os, CMatchingRule const& rule )
   {
      return os << rule.toString();
   }

   boost::property_tree::ptree CMatchingRule::serialize() const
   {
      boost::property_tree::ptree ptRule;
      ptRule.put("name", getName());
      {
         boost::property_tree::ptree ptWhitelist;
         for (auto const& e : getWhitelist())
         {
            ptWhitelist.add("entry", e);
         }
         ptRule.add_child("whitelist", ptWhitelist);
      }
      {
         boost::property_tree::ptree ptBlacklist;
         for (auto const& e : getBlacklist())
         {
            ptBlacklist.add("entry", e);
         }
         ptRule.add_child("blacklist", ptBlacklist);
      }
      return ptRule;
   }
   CMatchingRule CMatchingRule::deserialize(boost::property_tree::ptree const& ptRule)
   {
      auto name = ptRule.get< std::string >( "name" );
      std::list<std::string> whitelist;
      for (auto const& ptWhitelist : ptRule.get_child( "whitelist" ))
      {
         whitelist.emplace_back(ptWhitelist.second.get< std::string >(""));
      }
      std::list<std::string> blacklist;
      for (auto const& ptBlacklist : ptRule.get_child( "blacklist" ))
      {
         blacklist.emplace_back(ptBlacklist.second.get< std::string >(""));
      }
      return API::CMatchingRule(name, whitelist, blacklist);
   }
   
}}}

