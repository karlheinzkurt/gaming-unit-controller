
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

namespace Infrastructure
{
namespace Linux
{
   namespace fs = boost::filesystem;
   
   namespace
   {
      
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
   
   std::unique_ptr< API::IProcess > CProcess::clone() const
   {
      return std::make_unique<CProcess>(*this);
   }
   
}}
