
#include "../include/CMatcher.h"
#include "../include/CMatchingRule.h"

#include <boost/property_tree/ptree.hpp>

#include <sstream>

namespace GSC
{
namespace Common
{  
   struct CMatch : API::IMatch
   {     
      CMatch(std::string const& name, API::IMatchingRule const& rule, std::shared_ptr<Infrastructure::API::IProcess> const& process) 
         :m_name(name)
         ,m_rule(rule)
         ,m_processes({process})
      {}
      
      CMatch(std::string const& name, API::IMatchingRule const& rule, Infrastructure::API::IProcess::SetType const& processes)
         :m_name(name)
         ,m_rule(rule)
         ,m_processes(std::move(processes))
      {}
      
      virtual std::string getName() const override
      {  return m_name; }
      
      virtual std::string toString() const override
      {  
         std::ostringstream os;
         os << getName() << ":" << std::accumulate(m_processes.begin(), m_processes.end(), std::string(), [](std::string v, std::shared_ptr<Infrastructure::API::IProcess> const& process)
         {
            return v + " " + std::to_string(process->getProcessId());
         });
         return os.str();
      }
      
      virtual API::IMatchingRule const& getRule() const override
      {  return m_rule; }
      
      virtual Infrastructure::API::IProcess::SetType const& getProcesses() const override
      {  return m_processes; }
      
   private:   
      std::string m_name;
      API::IMatchingRule const& m_rule;
      Infrastructure::API::IProcess::SetType m_processes;
   };
   
   CMatcher::CMatcher() :
      m_logger( log4cxx::Logger::getLogger( "GSC.Common.CMatcher" ) )
   {}
   
   API::IMatcher& CMatcher::add(std::unique_ptr<API::IMatchingRule> rule )
   {
      m_rules.emplace(std::move(rule));
      return *this;
   }
   
   API::IMatchingRule::SetType const& CMatcher::getRules() const
   {  return m_rules; }
     
   API::IMatch::SetType CMatcher::matches( std::shared_ptr<Infrastructure::API::IProcess> const& process ) const
   {  return matches(Infrastructure::API::IProcess::SetType{process}); }
   
   API::IMatch::SetType CMatcher::matches(Infrastructure::API::IProcess::SetType const& processes) const
   {
      if ( m_rules.empty() )
      {  return API::IMatch::SetType(); }
      
      std::map<API::IMatchingRule const*, Infrastructure::API::IProcess::SetType > temporary;
      for ( auto& process : processes )
      {  
         for ( auto const& rule : m_rules )
         {
            if (rule->matches(process->getCommandLine()))
            {
               LOG4CXX_DEBUG(m_logger, "Rule '" + rule->getName() + "' matches: " + process->toString());
               temporary[rule.get()].emplace(process);
            }  
         }
      }
      API::IMatch::SetType results;
      for ( auto& tuple : temporary ) { results.emplace(std::make_unique<CMatch>(tuple.first->getName(), *tuple.first, tuple.second)); }
      return std::move( results );
   }

   boost::property_tree::ptree CMatcher::serialize() const
   {  
      boost::property_tree::ptree pt;
      std::for_each(m_rules.begin(), m_rules.end(), [&](auto const& rule)
      {  pt.add_child( "rules.rule", rule->serialize()); });
      return std::move(pt);
   }
   
   std::unique_ptr<API::IMatcher> CMatcher::deserialize(boost::property_tree::ptree const& ptMatch)
   {
      auto matcher(std::make_unique<CMatcher>());
      for (auto const& ptRule : ptMatch.get_child( "rules" ).get_child( "" ))
      {  matcher->add(CMatchingRule::deserialize(ptRule.second)); }
      return std::move(matcher);      
   }
}}
