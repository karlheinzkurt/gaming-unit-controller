
#include "../include/CProcess.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>

#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <thread>
#include <algorithm>

#include <csignal>
#include <sys/types.h>

namespace Lib
{
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
      
      fs::path determineProcessDirectory( int processId )
      {
         return fs::path( "/proc" ) / std::to_string( processId ); 
      }

      std::string determineCommandLine( fs::path const& processDirectory )
      {
         fs::path const path( processDirectory / "cmdline" );
         if ( !fs::is_regular_file( path ) )
         {  return std::string(); }
         
         std::ifstream stream( path.string(), std::ios::binary );
         if ( !stream.good() )
         {  return std::string(); }
         
         std::string buffer( (std::istreambuf_iterator< std::string::value_type >(stream)), (std::istreambuf_iterator< std::string::value_type >()) );
         /** We have to go through to chars by index,
          *  because it contains multiple 0 chars we want to replace.
          * */
         for ( size_t index( 0 ); index < buffer.size(); ++index )
         {
            if ( buffer[ index ] == 0 ) 
            {  buffer[ index ] = ' '; }
         };
         boost::algorithm::trim( buffer );      
         return buffer;
      }
   }
   
   CProcess::CProcess( int processId ) : 
       m_processDirectory( determineProcessDirectory( processId ) )
      ,m_pid( processId )
      ,m_commandLine( determineCommandLine( m_processDirectory ) ) 
      ,m_readable( !m_commandLine.empty() )
   {}
   
   bool CProcess::isRunning() const
   {  
      return ( -ESRCH != ::kill( m_pid, 0 ) );
   }
   
   bool CProcess::isSignallable() const
   {
      if ( !isRunning() )
      {  return false; }
      
      return ( -EPERM != ::kill( m_pid, 0 ) );      
   }
      
   int CProcess::getProcessId() const
   {  return m_pid; }
   
   std::string CProcess::getCommandLine() const
   {  return m_commandLine; }
   
   std::string CProcess::toString() const
   {  
      std::ostringstream os;
      os << m_pid << ": " << m_commandLine; 
      return os.str(); 
   }
   
   bool CProcess::terminateAndWait( std::chrono::seconds const& timeout )
   {
      if ( 0 != ::kill( m_pid, SIGTERM ) )
      {  return false; }

      std::chrono::seconds const dutyCycle( std::chrono::seconds( 1 ) );
      for ( std::chrono::seconds current( 0 ); isRunning() && current < timeout; current += dutyCycle )
      {  std::this_thread::sleep_for( dutyCycle ); }
         
      return !isRunning();
   }
   
   bool CProcess::killAndWait( std::chrono::seconds const& timeout )
   {
      if ( 0 != ::kill( m_pid, SIGKILL ) )
      {  return false; }
      
      std::chrono::seconds const dutyCycle( std::chrono::seconds( 1 ) );
      for ( std::chrono::seconds current( 0 ); isRunning() && current < timeout; current += dutyCycle )
      {  std::this_thread::sleep_for( dutyCycle ); }
      
      return !isRunning();
   }
   
   CProcessSet::SetType CProcessSet::get()
   {
      std::regex const processPathRegex( "^\\d+$" );
      SetType processes;
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
   
   CProcessSet::SetType CProcessSet::getSignallable( SetType const& processes )
   {
      SetType signallableProcesses;
      for ( auto& process : processes )
      {  
         if ( process->isSignallable() )
         {  signallableProcesses.emplace( std::make_unique< CProcess >( process->getProcessId() ) ); }
      }
      return std::move( signallableProcesses );
   }
   
}}}
