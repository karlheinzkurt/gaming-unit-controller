#pragma once

#include "Lib/Infrastructure/API/include/IProcess.h"

#include <string>
#include <list>
#include <iosfwd>

namespace Lib
{
namespace Controller
{
namespace API
{
   using Infrastructure::API::IProcess;

   struct IMatch
   {
      typedef std::set< std::unique_ptr< IMatch > > SetType;
      
      virtual ~IMatch() = default;
      
      virtual std::string getName() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual IProcess::SetType const& getProcesses() const = 0;
   };
   
   std::ostream& operator<<( std::ostream& os, IMatch const& match );
   
   bool operator<( IMatch const& a, IMatch const& b );
   
   struct CRule
   {
      std::string name;
      std::list<std::string> whitelist;
      std::list<std::string> blacklist;      
   };   
      
   struct IMatcher
   {
      virtual ~IMatcher() = default;
      
      virtual IMatcher& add( CRule rule ) = 0;
      
      virtual std::string toString() const = 0;
      
      virtual IMatch::SetType matches( IProcess const& process ) const = 0;
      
      virtual IMatch::SetType matches( IProcess::SetType const& process ) const = 0;
   };
   
   std::ostream& operator<<( std::ostream& os, IMatcher const& matcher );

}}}
