
#include "../include/CMatchingRule.h"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>

#include <sstream>

namespace GSC
{
namespace Common
{  
   struct CMatchingRule::List
   {     
      List(std::list<std::string> list) :
          m_list(list)
         ,m_string(boost::algorithm::join(m_list, ", "))
         ,m_expression(std::string("(") + boost::algorithm::join(m_list, "|" ) + ")", std::regex_constants::icase)
      {}
      
      std::list<std::string> m_list;
      std::string m_string; 
      std::regex m_expression;
   };
      
   CMatchingRule::CMatchingRule(std::string name, std::chrono::seconds limit, std::list<std::string> whitelist, std::list<std::string> blacklist) 
      :m_name(name)
      ,m_limit(limit)
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
         << " (" << m_whitelist->m_string << "), "
         << "!(" << m_blacklist->m_string << ")";
      return os.str();
   }
   
   std::string const& CMatchingRule::getName() const
   {  return m_name; }
   
   std::chrono::seconds CMatchingRule::getLimit() const
   {  return m_limit; }
   
   std::list<std::string> const& CMatchingRule::getWhitelist() const
   {  return m_whitelist->m_list; }
      
   std::list<std::string> const& CMatchingRule::getBlacklist() const
   {  return m_blacklist->m_list; }
   
   boost::property_tree::ptree CMatchingRule::serialize() const
   {
      boost::property_tree::ptree ptRule;
      ptRule.put("name", getName());
      ptRule.put("limit", getLimit().count());
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
   
   std::unique_ptr<API::IMatchingRule> CMatchingRule::deserialize(boost::property_tree::ptree const& ptRule)
   {
      auto name = ptRule.get<std::string>("name");
      std::chrono::seconds limit(ptRule.get("limit", 0));
      std::list<std::string> whitelist;
      for (auto const& ptWhitelist : ptRule.get_child( "whitelist" ))
      {  whitelist.emplace_back(ptWhitelist.second.get< std::string >("")); }
      std::list<std::string> blacklist;
      for (auto const& ptBlacklist : ptRule.get_child( "blacklist" ))
      {  blacklist.emplace_back(ptBlacklist.second.get< std::string >("")); }
      return std::make_unique<CMatchingRule>(name, limit, whitelist, blacklist);
   }
   
}}
