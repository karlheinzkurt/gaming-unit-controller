
#include "../include/CMatcher.h"

#include <boost/algorithm/string.hpp>

#include <regex>
#include <sstream>

namespace Lib
{
namespace Controller
{
      
   CMatch::CMatch( std::string const& name, IProcess const& process ) : m_name( name ), m_processes() 
   {
      m_processes.emplace(process.clone());
   }
   
   CMatch::CMatch( std::string const& name, IProcess::SetType const& processes ) : m_name( name ), m_processes() 
   {  
      for ( auto& process : processes )
      {  
         m_processes.emplace(process->clone());
      }      
   }
   
   std::string CMatch::getName() const
   {  return m_name; }
   
   std::string CMatch::toString() const
   {  
      std::ostringstream os;
      os << getName() << ":" << std::accumulate(m_processes.begin(), m_processes.end(), std::string(), [](std::string v, std::unique_ptr<IProcess> const& p)
      {
         return v + " " + std::to_string(p->getProcessId());
      });
      return os.str();
   }
   
   API::IProcess::SetType const& CMatch::getProcesses() const
   {  return m_processes; }
   
   API::IMatcher& CMatcher::addRule( 
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
   
   std::string CMatcher::toString() const
   {
      std::ostringstream os;
      for (auto matcher : m_matcher)
      {
         os << matcher.first << "(" << /*std::get<0>(matcher.second)*/"whitelist" << ", !" << "blacklist"/*std::get<1>(matcher.second)*/ << ") ";
      }
      return os.str();
   }
   
   API::IMatch::SetType CMatcher::matches( IProcess const& process ) const
   {
      std::string const commandLine( process.getCommandLine() );      
      if ( m_matcher.empty() )
      {  return API::IMatch::SetType(); }
      
      std::map< std::string, IProcess::SetType > temporary;
      for ( auto& entry : m_matcher )
      {
         std::regex whiteList, blackList;
         std::tie( whiteList, blackList ) = entry.second;
         if ( std::regex_match( commandLine, whiteList ) )
         {  
            if ( !std::regex_match( commandLine, blackList ) )
            {  temporary[ entry.first ].emplace(process.clone()); }
         }  
      }
      
      API::IMatch::SetType results;
      for ( auto& tuple : temporary ) { results.emplace(std::make_unique<CMatch>(tuple.first, tuple.second)); }
      return std::move( results );
   }
   
   API::IMatch::SetType CMatcher::matches( IProcess::SetType const& processes ) const
   {
      std::map< std::string, IProcess::SetType > temporary;
      for ( auto& process : processes )
      {  
         for ( auto const& match : matches( *process ) )
         {
            temporary[ match->getName() ].emplace(process->clone());
         }
      }
      API::IMatch::SetType results;
      for ( auto& tuple : temporary ) { results.emplace(std::make_unique<CMatch>(tuple.first, tuple.second)); }
      return std::move( results );
   }
   
}}

