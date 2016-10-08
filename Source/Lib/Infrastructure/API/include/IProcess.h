#pragma once

#include <string>
#include <iosfwd>
#include <chrono>
#include <set>
#include <memory>

namespace Lib
{
namespace Infrastructure
{
namespace API
{
   struct IProcess
   { 
      typedef std::set< std::unique_ptr< IProcess > > SetType;
 
      virtual ~IProcess() = default;
      
      virtual bool isRunning() const = 0;
      
      virtual bool isSignallable() const = 0;
      
      virtual int getProcessId() const = 0;
      
      virtual std::string getCommandLine() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual bool terminateAndWait( std::chrono::seconds const& timeout ) = 0;
      
      virtual bool killAndWait( std::chrono::seconds const& timeout ) = 0;
      
      virtual std::unique_ptr< API::IProcess > clone() const = 0;
   };
   
   std::ostream& operator<<( std::ostream& os, IProcess const& process );
   
   bool operator==( IProcess const& a, IProcess const& b );
   bool operator<( IProcess const& a, IProcess const& b );

}}}
