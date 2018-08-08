
#include "../include/CSessionController.h"

#include "gsclib/common/include/CMatchingRule.h"
#include "gsclib/common/include/CStatistics.h"
#include "gsclib/common/include/CMatcherFactory.h"
#include "gsclib/common/include/CInfluxAdapter.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

namespace GSC
{
namespace Common
{ 
   boost::filesystem::path CSessionController::getDefaultCounterFilePath()
   {  return boost::filesystem::path("gsc.counts.xml"); }
   
   boost::filesystem::path CSessionController::getDefaultLogFilePath()
   {  return boost::filesystem::path("gsc.log"); }
  
   boost::filesystem::path CSessionController::getDefaultConfigurationFilePath()
   {  return boost::filesystem::path("etc") / "gsc.config.xml"; }
   
   boost::filesystem::path CSessionController::getDefaultLoggerConfigurationFilePath()
   {  return boost::filesystem::path("etc") / "gsc.logger.xml"; }
  
   CSessionController::CSessionController( 
       Infrastructure::API::ISystem& system
      ,API::IRunningStrategy& runningStrategy
      ,boost::filesystem::path const& configurationFilePath
      ,boost::filesystem::path const& counterFilePath ) : 
       m_system(system)
      ,m_runningStrategy(runningStrategy)
      ,m_configurationFilePath(configurationFilePath)
      ,m_logger( log4cxx::Logger::getLogger( "GSC.Common.CSessionController" ) )
   {
      if ( !boost::filesystem::exists( m_configurationFilePath ) )
      {
         m_matcher = CMatcherFactory().create();
         m_matcher->add(std::make_unique<CMatchingRule>(
             "example"
            ,std::chrono::hours(1)
            ,std::list<std::string>{".*match.*", ".*or_match.*"}
            ,std::list<std::string>{".*filter_out.*", ".*filter_also_out.*", ".*filter_out_as_well.*"}));
         auto const examplePath = boost::filesystem::change_extension(m_configurationFilePath, "example.xml");
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
      
      CInfluxAdapter influxAdapter;
      m_runningStrategy.run([this, counterFilePath, &influxAdapter]
      {
         LOG4CXX_INFO( m_logger, "=====> Next Cycle <=====");
         load(m_configurationFilePath);
      
         auto const processes(m_system.getRunningProcesses());
         LOG4CXX_INFO( m_logger, "Processes found: " << processes.size() );
               
         auto const signallableProcesses(m_system.getSignallableProcesses());
         LOG4CXX_INFO( m_logger, "Signallable processes found: " << signallableProcesses.size() );
              
         auto const matches( m_matcher->matches( signallableProcesses ) );
         LOG4CXX_INFO( m_logger, "Matching processes found: " << std::accumulate( 
             matches.begin(), matches.end(), 0
            ,[]( int v, auto const& match ){ return v + match->getProcesses().size(); } ) );
         
         Statistics statistics( m_logger, counterFilePath );
         
         statistics.updateCounters(matches);
         LOG4CXX_INFO( m_logger, "Matching applications found: " << matches.size() );
         for (auto& match : matches) { LOG4CXX_INFO(m_logger, *match); }
         influxAdapter.insertActive(matches);

         auto const exceeds(statistics.filterExceeding(std::move(matches)));
         LOG4CXX_INFO( m_logger, "Exceeding applications found: " << exceeds.size() );
         for (auto& exceed : exceeds) { LOG4CXX_INFO( m_logger, *exceed ); }         
         influxAdapter.insertExceeding(exceeds);
      });
   }
   
   void CSessionController::save(boost::filesystem::path const& path) const
   {
      boost::property_tree::ptree tree;
      tree.add_child("matcher", m_matcher->serialize());
      boost::property_tree::write_xml( path.string(), tree, std::locale(), boost::property_tree::xml_writer_make_settings<std::string>( ' ', 2 ) );
   }
   
   void CSessionController::load(boost::filesystem::path const& path)
   {
      boost::property_tree::ptree tree;
      boost::property_tree::read_xml( path.string(), tree, boost::property_tree::xml_parser::trim_whitespace );
      auto ptMatcher( tree.get_child_optional( "matcher" ) );
      if ( ptMatcher )
      {  m_matcher = CMatcherFactory().create(*ptMatcher); }
   }
}}
