
#include "../include/CProgramController.h"

#include "Lib/Infrastructure/Linux/include/CProcess.h"
#include "Lib/Controller/include/CStatistics.h"

namespace SL
{
namespace App
{
namespace Utility
{
   
   using Lib::Infrastructure::Linux::CProcessSet;
   using Lib::Controller::Matcher;
   using Lib::Controller::Match;
   using Lib::Controller::Statistics;
   
   ProgramController::ProgramController( 
       boost::filesystem::path const& absoluteStatisticPath
      ,Matcher matcher ) : 
       m_logger( log4cxx::Logger::getLogger( "App.Utility.Controller" ) )
   {
      CProcessSet processSet;
      auto const processes( processSet.get() );
      LOG4CXX_INFO( m_logger, "Processes found: " << processes.size() );
            
      auto const signallableProcesses( processSet.getSignallable( std::move( processes ) ) );
      LOG4CXX_INFO( m_logger, "Signallable processes found: " << signallableProcesses.size() );
            
      auto const matches( matcher.matches( signallableProcesses ) );
      LOG4CXX_INFO( m_logger, "Matching processes found: " << std::accumulate( 
          matches.begin(), matches.end(), 0
         ,[]( int v, Match const& match ){ return v + match.getProcesses().size(); } ) );
      LOG4CXX_INFO( m_logger, "Matches found: " << matches.size() );
      
      Statistics statistics( m_logger, absoluteStatisticPath );
      for ( auto& match : matches )
      {
         statistics.add( match.getName() );
         LOG4CXX_INFO( m_logger, match ); 
      }
      
      std::set< std::string > const exceeds( statistics.getCurrentlyExceeding( std::chrono::hours( 1 ) ) );
      LOG4CXX_INFO( m_logger, "Exceeding matches found: " << exceeds.size() );
      for ( auto& exceed : exceeds )
      {
         /** kill exceeding processes */
         LOG4CXX_INFO( m_logger, exceed ); 
      }
   }

}}}
