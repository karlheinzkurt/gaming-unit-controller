#pragma once

#include "gsclib/api/include/IMatcher.h"

#include <log4cxx/logger.h>

namespace GSC
{
namespace Common
{
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
