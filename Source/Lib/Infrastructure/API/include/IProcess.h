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
      virtual ~IProcess() = default;
      
      virtual bool isRunning() const = 0;
      
      virtual bool isSignallable() const = 0;
      
      virtual int getProcessId() const = 0;
      
      virtual std::string getCommandLine() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual bool terminateAndWait( std::chrono::seconds const& timeout ) = 0;
      
      virtual bool killAndWait( std::chrono::seconds const& timeout ) = 0;
   };
   
   struct IProcessSet
   {
      typedef std::set< std::unique_ptr< IProcess > > SetType;
      
      virtual ~IProcessSet() = default;
      
      virtual SetType get() = 0;
      
      virtual SetType getSignallable( SetType const& processList ) = 0; 
   };
   
   std::ostream& operator<<( std::ostream& os, IProcess const& process );
   
   bool operator==( IProcess const& a, IProcess const& b );
   bool operator<( IProcess const& a, IProcess const& b );

}}}
