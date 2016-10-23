#pragma once

#include "IMatcher.h"

#include "Lib/Infrastructure/API/include/IProcess.h"

namespace Lib
{
namespace Controller
{
   using Infrastructure::API::IProcess;
  
   struct CMatcher : API::IMatcher
   {           
      virtual IMatcher& add( API::CMatchingRule rule ) override;
         
      virtual API::CMatchingRule::SetType const& getRules() const override;
               
      virtual API::IMatch::SetType matches( IProcess const& process ) const override;
      
      virtual API::IMatch::SetType matches( IProcess::SetType const& process ) const override;
      
      virtual boost::property_tree::ptree serialize() const override;
      
      static std::unique_ptr<API::IMatcher> deserialize(boost::property_tree::ptree const&);
      
   private:
      API::CMatchingRule::SetType m_rules;
   };

}}
