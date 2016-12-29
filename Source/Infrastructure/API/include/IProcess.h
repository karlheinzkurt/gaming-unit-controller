#pragma once

#include <string>
#include <iosfwd>
#include <chrono>
#include <set>
#include <memory>

namespace Infrastructure
{
namespace API
{
   struct IProcess
   { 
      typedef std::set< std::shared_ptr< IProcess > > SetType; ///< shared_ptr by intention here
 
      virtual ~IProcess() = default;
      
      virtual bool isRunning() const = 0;
      
      virtual bool isSignallable() const = 0;
      
      virtual int getProcessId() const = 0;
      
      virtual std::string getCommandLine() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual bool terminateAndWait( std::chrono::seconds const& timeout ) = 0;
      
      virtual bool killAndWait( std::chrono::seconds const& timeout ) = 0;
   };
   
   std::ostream& operator<<( std::ostream& os, IProcess const& process );
   
   bool operator==( IProcess const& a, IProcess const& b );
   bool operator<( IProcess const& a, IProcess const& b );

}}
