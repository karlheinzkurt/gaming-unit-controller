#pragma once

#include "IMatcher.h"

#include "Lib/Infrastructure/API/include/IProcess.h"

#include <string>
#include <map>
#include <regex>
#include <functional>
#include <iosfwd>

namespace Lib
{
namespace Controller
{
   using Infrastructure::API::IProcess;
   
  
   struct CMatcher : API::IMatcher
   {      
      virtual IMatcher& add( API::CRule rule ) override;
         
      virtual std::string toString() const override;
      
      virtual API::IMatch::SetType matches( IProcess const& process ) const override;
      
      virtual API::IMatch::SetType matches( IProcess::SetType const& process ) const override;
      
   private:
      typedef std::map< std::string, std::tuple< std::regex, std::regex > > MapType;
      MapType m_matcher;
   };

}}
