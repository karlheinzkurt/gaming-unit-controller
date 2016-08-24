#pragma once

#include "Lib/Infrastructure/API/include/IProcess.h"

#include <boost/filesystem/path.hpp>

#include <set>
#include <string>
#include <iosfwd>
#include <chrono>

namespace Lib
{
namespace Infrastructure
{
namespace Linux
{
   struct CProcess : public API::IProcess
   {       
      explicit CProcess( int processId );
      
      CProcess( CProcess const& ) = default;
      CProcess& operator=( CProcess const& ) = default;
      
      virtual bool isRunning() const override;
      
      virtual bool isSignallable() const override;
      
      virtual int getProcessId() const override;
      
      virtual std::string getCommandLine() const override;
      
      virtual std::string toString() const override;
      
      virtual bool terminateAndWait( std::chrono::seconds const& timeout ) override;
      
      virtual bool killAndWait( std::chrono::seconds const& timeout ) override;
     
   private:   
      boost::filesystem::path const m_processDirectory;
      int const m_pid;
      std::string const m_commandLine;
      bool const m_readable;
   };
   
   struct CProcessSet : public API::IProcessSet
   {
      using API::IProcessSet::SetType;
      
      virtual SetType get() override;
      
      virtual SetType getSignallable( SetType const& processList ) override;
   };

}}}
