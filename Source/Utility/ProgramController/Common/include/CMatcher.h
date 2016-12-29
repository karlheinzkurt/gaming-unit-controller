#pragma once

#include "IMatcher.h"

#include "Infrastructure/API/include/IProcess.h"

#include <log4cxx/logger.h>

namespace Utility
{
namespace ProgramController
{
   using Infrastructure::API::IProcess;
  
   struct CMatcher : API::IMatcher
   {           
      CMatcher();
      
      virtual IMatcher& add( CMatchingRule rule ) override;
         
      virtual CMatchingRule::SetType const& getRules() const override;
               
      virtual API::IMatch::SetType matches( IProcess const& process ) const override;
      
      virtual API::IMatch::SetType matches( IProcess::SetType const& process ) const override;
      
      virtual boost::property_tree::ptree serialize() const override;
      
      static std::unique_ptr<API::IMatcher> deserialize(boost::property_tree::ptree const&);
      
   private:
      log4cxx::LoggerPtr m_logger;
      CMatchingRule::SetType m_rules;
   };

}}
