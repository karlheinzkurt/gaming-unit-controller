
#include "../include/CProcess.h"
#include "../include/CSystem.h"

#include <boost/filesystem.hpp>

#include <regex>
#include <algorithm>

namespace Infrastructure
{
namespace Linux
{
   
   namespace fs = boost::filesystem;
   
   namespace
   {
      int determineProcessId( fs::path const& processDirectory )
      {  
         static std::regex const pidRegex( "/proc/(\\d+)$" );
         std::string const pidString( std::regex_replace( processDirectory.string(), pidRegex, "$1" ) );
         if ( pidString.empty() )
         {  throw std::domain_error( std::string( "Determination of PID string failed for process directory: " ) + processDirectory.string() ); }
         
         int const pid( std::stoi( pidString ) );
         if ( pid < 1 )
         {  throw std::domain_error( std::string( "PID is 0 or negative for process directory: " ) + processDirectory.string() ); }
         
         return pid;
      }
   }
   
   API::IProcess::SetType CSystem::getRunningProcesses()
   {
      std::regex const processPathRegex( "^\\d+$" );
      API::IProcess::SetType processes;
      std::for_each( fs::directory_iterator( fs::path( "/proc" ) ), fs::directory_iterator(), [ & ]( auto const& entry )
      {
         if ( !fs::is_directory( entry ) )
         {  return; }

         if ( !std::regex_match( entry.path().filename().string(), processPathRegex ) )
         {  return; }

         processes.emplace( std::make_unique< CProcess >( determineProcessId( entry.path() ) ) );
      } );
      return std::move( processes );
   }
   
   API::IProcess::SetType CSystem::getSignallableProcesses()
   {
      API::IProcess::SetType signallableProcesses;
      for ( auto& process : getRunningProcesses() )
      {  
         if ( process->isSignallable() )
         {  signallableProcesses.emplace( std::make_unique< CProcess >( process->getProcessId() ) ); }
      }
      return std::move( signallableProcesses );
   }
   
}}
