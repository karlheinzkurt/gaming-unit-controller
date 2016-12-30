
#include "../include/CProgramController.h"

#include "Utility/ProgramController/Common/include/CMatchingRule.h"
#include "Utility/ProgramController/Common/include/CStatistics.h"
#include "Utility/ProgramController/Common/include/CMatcherFactory.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

namespace Utility
{
namespace ProgramController
{ 
   boost::filesystem::path CProgramController::getDefaultCounterFilePath()
   {  return boost::filesystem::path("Utility.ProgramController.Counter.xml"); }
   
   boost::filesystem::path CProgramController::getDefaultLogFilePath()
   {  return boost::filesystem::path("Utility.ProgramController.log"); }
  
   boost::filesystem::path CProgramController::getDefaultConfigurationFilePath()
   {  return boost::filesystem::path("etc") / "Utility.ProgramController.Config.xml"; }
   
   boost::filesystem::path CProgramController::getDefaultLoggerConfigurationFilePath()
   {  return boost::filesystem::path("etc") / "Utility.ProgramController.LoggerConfig.xml"; }
  
   CProgramController::CProgramController( 
       Infrastructure::API::ISystem& system
      ,boost::filesystem::path const& configurationFilePath
      ,boost::filesystem::path const& counterFilePath ) : 
       m_system(system)
      ,m_configurationFilePath(configurationFilePath)
      ,m_logger( log4cxx::Logger::getLogger( "Utility.ProgramController.CProgramController" ) )
   {
      if ( !boost::filesystem::exists( m_configurationFilePath ) )
      {
         m_matcher = CMatcherFactory().create();
         m_matcher->add(CMatchingRule("example", {".*match.*", ".*or_match.*"}, {".*filter_out.*", ".*filter_also_out.*", ".*filter_out_as_well.*"}));
         auto examplePath = boost::filesystem::change_extension(m_configurationFilePath, "Example.xml");
         save(examplePath);
         
         std::ostringstream os;
         os << "Configuration file " << m_configurationFilePath << " does not exist, see " << examplePath << " as an example to create one";
         throw std::invalid_argument(os.str());
      }
      else
      {
         if ( !boost::filesystem::is_regular_file( m_configurationFilePath ) )
         {  
            std::ostringstream os;
            os << "Path exists but is not a file: " << m_configurationFilePath; 
            throw std::invalid_argument(os.str());
         }
      }
      load(m_configurationFilePath);
      
      auto const processes(system.getRunningProcesses());
      LOG4CXX_INFO( m_logger, "Processes found: " << processes.size() );
            
      auto const signallableProcesses(system.getSignallableProcesses());
      LOG4CXX_INFO( m_logger, "Signallable processes found: " << signallableProcesses.size() );
           
      auto const matches( m_matcher->matches( signallableProcesses ) );
      LOG4CXX_INFO( m_logger, "Matching processes found: " << std::accumulate( 
          matches.begin(), matches.end(), 0
         ,[]( int v, auto const& match ){ return v + match->getProcesses().size(); } ) );
      LOG4CXX_INFO( m_logger, "Matches found: " << matches.size() );
      
      Statistics statistics( m_logger, counterFilePath );
      for ( auto& match : matches )
      {
         statistics.add( match->getName() );
         LOG4CXX_INFO( m_logger, *match ); 
      }
      
      std::set< std::string > const exceeds( statistics.getCurrentlyExceeding( /*matches, */std::chrono::hours( 1 ) ) );
      LOG4CXX_INFO( m_logger, "Exceeding matches found: " << exceeds.size() );
      for ( auto& exceed : exceeds )
      {
         /** kill exceeding processes */
         LOG4CXX_INFO( m_logger, exceed ); 
      }
   }
   
   void CProgramController::save(boost::filesystem::path const& path) const
   {
      boost::property_tree::ptree tree;
      tree.add_child("matcher", m_matcher->serialize());
      boost::property_tree::write_xml( path.string(), tree, std::locale(), boost::property_tree::xml_writer_make_settings<std::string>( ' ', 2 ) );
   }
   
   void CProgramController::load(boost::filesystem::path const& path)
   {
      boost::property_tree::ptree tree;
      boost::property_tree::read_xml( path.string(), tree, boost::property_tree::xml_parser::trim_whitespace );
      auto ptMatcher( tree.get_child_optional( "matcher" ) );
      if ( ptMatcher )
      {
         CMatcherFactory matcherFactory;
         m_matcher = matcherFactory.create(*ptMatcher);
      }
   }
}}
