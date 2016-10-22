
#include "../include/CMatcher.h"

#include <boost/property_tree/ptree.hpp>

#include <sstream>

namespace Lib
{
namespace Controller
{
   
   struct CMatch : API::IMatch
   {     
      CMatch( std::string const& name, IProcess const& process )
      {
         m_processes.emplace(process.clone());
      }
      
      CMatch( std::string const& name, IProcess::SetType const& processes )
      {  
         for ( auto& process : processes )
         {  
            m_processes.emplace(process->clone());
         }      
      }
      
      virtual std::string getName() const override
      {  return m_name; }
      
      virtual std::string toString() const override
      {  
         std::ostringstream os;
         os << getName() << ":" << std::accumulate(m_processes.begin(), m_processes.end(), std::string(), [](std::string v, std::unique_ptr<IProcess> const& p)
         {
            return v + " " + std::to_string(p->getProcessId());
         });
         return os.str();
      }
      
      virtual IProcess::SetType const& getProcesses() const override
      {  return m_processes; }
      
   private:   
      std::string m_name;
      IProcess::SetType m_processes;
   };
   
   API::IMatcher& CMatcher::add( API::CRule rule )
   {
      m_rules.emplace(std::move(rule));
      return *this;
   }
   
   API::CRule::SetType const& CMatcher::getRules() const
   {
      return m_rules;
   }
     
   API::IMatch::SetType CMatcher::matches( IProcess const& process ) const
   {
      if ( m_rules.empty() )
      {  return API::IMatch::SetType(); }
      
      std::map< std::string, IProcess::SetType > temporary;
      for ( auto const& rule : m_rules )
      {
         if (rule.matches(process.getCommandLine()))
         {
            temporary[rule.getName()].emplace(process.clone());
         }  
      }
      
      API::IMatch::SetType results;
      for ( auto& tuple : temporary ) { results.emplace(std::make_unique<CMatch>(tuple.first, tuple.second)); }
      return std::move( results );
   }
   
   API::IMatch::SetType CMatcher::matches( IProcess::SetType const& processes ) const
   {
      std::map< std::string, IProcess::SetType > temporary;
      for ( auto& process : processes )
      {  
         for ( auto const& match : matches( *process ) )
         {
            temporary[ match->getName() ].emplace(process->clone());
         }
      }
      API::IMatch::SetType results;
      for ( auto& tuple : temporary ) { results.emplace(std::make_unique<CMatch>(tuple.first, tuple.second)); }
      return std::move( results );
   }

   boost::property_tree::ptree CMatcher::serialize() const
   {  
      boost::property_tree::ptree pt;
      std::for_each(m_rules.begin(), m_rules.end(), [&](auto const& rule)
      {
         boost::property_tree::ptree ptRule;
         ptRule.put("name", rule.getName());
         {
            boost::property_tree::ptree ptWhitelist;
            for (auto const& e : rule.getWhitelist())
            {
               ptWhitelist.add("entry", e);
            }
            ptRule.add_child("whitelist", ptWhitelist);
         }
         {
            boost::property_tree::ptree ptBlacklist;
            for (auto const& e : rule.getBlacklist())
            {
               ptBlacklist.add("entry", e);
            }
            ptRule.add_child("blacklist", ptBlacklist);
         }
         pt.add_child( "rules.rule", ptRule);
      });
      return std::move(pt);
   }
   
   std::unique_ptr<API::IMatcher> CMatcher::deserialize(boost::property_tree::ptree const& ptMatch)
   {
      auto matcher(std::make_unique<CMatcher>());
      for (auto const& ptRule : ptMatch.get_child( "rules" ).get_child( "" ))
      {
         auto name = ptRule.second.get< std::string >( "name" );
         std::list<std::string> whitelist;
         for (auto const& ptWhitelist : ptRule.second.get_child( "whitelist" ))
         {
            whitelist.emplace_back(ptWhitelist.second.get< std::string >(""));
         }
         std::list<std::string> blacklist;
         for (auto const& ptBlacklist : ptRule.second.get_child( "blacklist" ))
         {
            blacklist.emplace_back(ptBlacklist.second.get< std::string >(""));
         }
         matcher->add(API::CRule(name, whitelist, blacklist));
      }
      return std::move(matcher);      
   }
 
}}
