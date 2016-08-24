
#include "../include/CMatcher.h"

#include <boost/algorithm/string.hpp>

#include <regex>

namespace Lib
{
namespace Controller
{
      
   Match::Match( std::string const& name ) : m_name( name ), m_processes() 
   {}
   
   Match& Match::add( IProcess const& process )
   {  
      m_processes.emplace( std::cref< IProcess >( process ) ); 
      return *this;
   }
   
   Match& Match::add( std::set< std::reference_wrapper< IProcess const > > const& processes )
   {
      for ( auto& process : processes )
      {  add( process ); }
      return *this;
   }
   
   std::string const& Match::getName() const
   {  return m_name; }
   
   std::set< std::reference_wrapper< IProcess const > > Match::getProcesses() const
   {  return m_processes; }
   
   std::ostream& operator<<( std::ostream& os, Match const& match )
   {
      std::list< std::string > pids;
      auto const processes( match.getProcesses() );
      std::transform( processes.begin(), processes.end(), std::back_inserter( pids ), []( IProcess const& process )
      {  return std::to_string( process.getProcessId() ); } );
      
      return ( os << match.getName() << ": with processes " << boost::algorithm::join( pids, ", " ) );
   }
   
   Matcher& Matcher::add( 
       std::string name
      ,std::initializer_list< std::string > whiteList
      ,std::initializer_list< std::string > blackList /*= {}*/ )
   {
      m_matcher.emplace( MapType::value_type( name, std::make_tuple( 
          std::regex( std::string( "(" ) + boost::algorithm::join( whiteList, "|" ) + ")", std::regex_constants::icase )
         ,std::regex( std::string( "(" ) + boost::algorithm::join( blackList, "|" ) + ")", std::regex_constants::icase )
      ) ) );
      return *this;
   }
   
   Matcher::ResultSet Matcher::matches( IProcess const& process )
   {
      std::string const commandLine( process.getCommandLine() );      
      if ( m_matcher.empty() )
      {  return ResultSet(); }
      
      std::map< std::string, std::set< std::reference_wrapper< IProcess const > > > temporary;
      for ( auto& entry : m_matcher )
      {
         std::regex whiteList, blackList;
         std::tie( whiteList, blackList ) = entry.second;
         if ( std::regex_match( commandLine, whiteList ) )
         {  
            if ( !std::regex_match( commandLine, blackList ) )
            {  temporary[ entry.first ].insert( process ); }
         }  
      }
      
      ResultSet results;
      for ( auto& tuple : temporary ) { results.emplace( Match( tuple.first ).add( tuple.second ) ); }
      return std::move( results );
   }
   
   Matcher::ResultSet Matcher::matches( IProcessSet::SetType const& processes )
   {
      std::map< std::string, std::set< std::reference_wrapper< IProcess const > > > temporary;
      for ( auto& process : processes )
      {  
         for ( auto const& match : matches( *process ) )
         {
            auto processes( match.getProcesses() );
            temporary[ match.getName() ].insert( processes.begin(), processes.end() );
         }
      }
      ResultSet results;
      for ( auto& tuple : temporary ) { results.emplace( Match( tuple.first ).add( tuple.second ) ); }
      return std::move( results );
   }
   
}}

