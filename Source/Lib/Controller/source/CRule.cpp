
#include "../include/CRule.h"

#include <boost/algorithm/string.hpp>

#include <sstream>

namespace Lib
{
namespace Controller
{
namespace API
{
   
   struct CRule::List
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
  
   CRule::CRule(std::string name, std::initializer_list<std::string> whitelist, std::initializer_list<std::string> blacklist) :
       m_name(name)
      ,m_whitelist(std::make_unique<List>(whitelist))
      ,m_blacklist(std::make_unique<List>(blacklist))
   {}
      
   CRule::CRule(std::string name, std::list<std::string> whitelist, std::list<std::string> blacklist) :
       m_name(name)
      ,m_whitelist(std::make_unique<List>(whitelist))
      ,m_blacklist(std::make_unique<List>(blacklist))
   {}
   
   CRule::~CRule() = default;
   CRule::CRule(CRule&&) = default;
   CRule& CRule::operator=(CRule&&) = default;
   
   bool CRule::matches(std::string const& string) const
   {
      if (std::regex_match(string, m_whitelist->m_expression))
      {  
         if (!std::regex_match(string, m_blacklist->m_expression))
         {   return true; }
      }
      return false;
   }
   
   std::string CRule::toString() const
   {
      std::ostringstream os;
      os << m_name
         << " (" << getWhitelistString() << "), "
         << "!(" << getBlacklistString() << ")";
      return os.str();
   }
   
   std::string const& CRule::getName() const
   {
      return m_name;
   }
   
   std::list<std::string> const& CRule::getWhitelist() const
   {
      return m_whitelist->m_list;
   }
      
   std::list<std::string> const& CRule::getBlacklist() const
   {
      return m_blacklist->m_list;
   }
   
   std::string const& CRule::getWhitelistString() const
   {
      return m_whitelist->m_string;
   }
      
   std::string const& CRule::getBlacklistString() const
   {
      return m_blacklist->m_string;
   }
   
   std::regex const& CRule::getWhitelistExpression() const
   {
      return m_whitelist->m_expression;
   }
   
   std::regex const& CRule::getBlacklistExpression() const
   {
      return m_blacklist->m_expression;
   }
   
   bool operator<(CRule const& a, CRule const& b)
   {
      return std::tie(a.getName(), a.getWhitelist(), a.getBlacklist()) < std::tie(b.getName(), b.getWhitelist(), b.getBlacklist());
   }
   
   bool operator==(CRule const& a, CRule const& b)
   {
      return std::tie(a.getName(), a.getWhitelist(), a.getBlacklist()) == std::tie(b.getName(), b.getWhitelist(), b.getBlacklist());
   }
   
   std::ostream& operator<<( std::ostream& os, CRule const& rule )
   {
      return os << rule.toString();
   }
   
}}}
