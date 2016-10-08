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

   struct CMatch : API::IMatch
   {     
      CMatch( std::string const& name, IProcess const& process );
      
      CMatch( std::string const& name, IProcess::SetType const& processes );
      
      virtual std::string getName() const override;
      
      virtual std::string toString() const override;
      
      virtual IProcess::SetType const& getProcesses() const override;
      
   private:   
      std::string m_name;
      IProcess::SetType m_processes;
   };
  
   struct CMatcher : API::IMatcher
   {      
      IMatcher& addRule( 
          std::string name
         ,std::initializer_list< std::string > whiteList
         ,std::initializer_list< std::string > blackList = {} );
         
      virtual std::string toString() const override;
      
      virtual API::IMatch::SetType matches( IProcess const& process ) const override;
      
      virtual API::IMatch::SetType matches( IProcess::SetType const& process ) const override;
      
   private:
      typedef std::map< std::string, std::tuple< std::regex, std::regex > > MapType;
      MapType m_matcher;
   };

}}
