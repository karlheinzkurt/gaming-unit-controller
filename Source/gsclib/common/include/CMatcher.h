#pragma once

#include "gsclib/api/include/IMatcher.h"

#include <log4cxx/logger.h>

namespace GSC
{
namespace Common
{
   struct CMatch : public virtual API::IMatch
   {     
      CMatch(std::string const& name, API::IMatchingRule const& rule, std::shared_ptr<Infrastructure::API::IProcess> const& process);
      
      CMatch(std::string const& name, API::IMatchingRule const& rule, Infrastructure::API::IProcess::SetType const& processes);
      
      CMatch(API::IMatch const&);
      
      virtual std::string getName() const override;
      
      virtual std::string toString() const override;
      
      virtual API::IMatchingRule const& getRule() const override;
      
      virtual Infrastructure::API::IProcess::SetType const& getProcesses() const override;
      
   private:   
      std::string m_name;
      API::IMatchingRule const& m_rule;
      Infrastructure::API::IProcess::SetType m_processes;
   };
   
   struct CRatedMatch : API::IRatedMatch, CMatch
   {
      CRatedMatch(API::IMatch const& m, boost::rational<int> ratio);
      
      virtual std::string toString() const override;
      
      virtual boost::rational<int> getRatio() const override;
      
   private:
      boost::rational<int> m_ratio;
   };
   
   struct CMatcher : API::IMatcher
   {           
      CMatcher();
      
      virtual API::IMatcher& add(std::unique_ptr<API::IMatchingRule> rule) override;
         
      virtual API::IMatchingRule::SetType const& getRules() const override;
               
      virtual API::IMatch::SetType matches(std::shared_ptr<Infrastructure::API::IProcess> const& process) const override;
      
      virtual API::IMatch::SetType matches(Infrastructure::API::IProcess::SetType const& process) const override;
      
      virtual boost::property_tree::ptree serialize() const override;
      
      static std::unique_ptr<API::IMatcher> deserialize(boost::property_tree::ptree const&);
      
   private:
      log4cxx::LoggerPtr m_logger;
      API::IMatchingRule::SetType m_rules;
   };

}}
