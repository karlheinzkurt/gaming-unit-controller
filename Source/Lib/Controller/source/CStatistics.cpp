
#include "../include/CStatistics.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

#include <exception>
#include <sstream>
#include <chrono>
#include <map>
#include <string>

namespace Lib
{
namespace Controller
{
   namespace pt = boost::property_tree;
   namespace fs = boost::filesystem;
   namespace ch = std::chrono;
   
   typedef ch::duration< ch::hours::rep, std::ratio_multiply< ch::hours::period, std::ratio< 24 > >::type > days;
   typedef ch::duration< days::rep, std::ratio_multiply< days::period, std::ratio< 7 > >::type > weeks;
   
   namespace
   {
      struct Counter
      {
         std::chrono::seconds m_active;
         std::chrono::system_clock::time_point m_lastChange;
      };
      
      typedef std::map< std::string, Counter > CounterMapType;
   }
   
   struct Statistics::Impl
   {
      Impl( log4cxx::LoggerPtr logger, boost::filesystem::path const& path ) : 
          m_logger( logger )
         ,m_path( path )
         ,m_now( ch::system_clock::now() )
         ,m_lastRun()
         ,m_counter()
      {}
      
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_path;
      std::chrono::system_clock::time_point m_now;
      std::chrono::system_clock::time_point m_lastRun;
      CounterMapType m_counter;      
   };
   
   Statistics::Statistics( log4cxx::LoggerPtr logger, boost::filesystem::path const& path ) : 
       m_impl( new Impl( logger, path ) )
   {
      if ( !fs::exists( m_impl->m_path ) )
      {
         fs::create_directories( m_impl->m_path.parent_path() );
         save(); ///< Save initial statistics file
      }
      else
      {
         if ( !fs::is_regular_file( m_impl->m_path ) )
         {  throw std::invalid_argument( std::string( "Path exists but is not a file: " ) + m_impl->m_path.string() ); }
      }
      load();      
   }
   
   Statistics::~Statistics()
   {
      try
      {  save(); }
      catch ( std::exception const& e )
      {  LOG4CXX_ERROR( m_impl->m_logger, e.what() ); }
   }
   
   void Statistics::add( std::string const& match )
   {
      auto& counter( m_impl->m_counter[ match ] );
      
      /** We reset the counter for the match when a new 
       *  counting unit has been archieved.
       * */
      days::rep const unitNow( ch::duration_cast< days >( m_impl->m_now.time_since_epoch() ).count() );
      days::rep const unitLastChange( ch::duration_cast< days >( counter.m_lastChange.time_since_epoch() ).count() );
      if ( unitLastChange < unitNow )
      {
         LOG4CXX_INFO( m_impl->m_logger, "New unit, resetting counter for: " + match );
         counter.m_active = ch::seconds( 0 );
      }
      
      /** Only when the last run has updated the counter
       *  do we add the delta to the active counter.
       *  When the entry update was earlier, we just
       *  set the update date to now, next run will 
       *  then increase the active counter.
       * */ 
      if ( counter.m_lastChange >= m_impl->m_lastRun )
      {  counter.m_active += ch::duration_cast< ch::seconds >( m_impl->m_now - m_impl->m_lastRun ); }
      
      /** When there was a match, we update the change stamp
       *  in any case. 
       * */
      counter.m_lastChange = m_impl->m_now;
   }
   
   std::set< std::string > Statistics::getCurrentlyExceeding( std::chrono::seconds const& limit )
   {
      std::set< std::string > results;
      for ( auto& tuple : m_impl->m_counter )
      {
         if (    tuple.second.m_lastChange >= m_impl->m_lastRun 
              && tuple.second.m_active     >= limit )
         {  results.insert( tuple.first ); }
      }
      return results;
   }
   
   void Statistics::save()
   {
      pt::ptree tree;
      tree.put( "statistics.lastRun", m_impl->m_now.time_since_epoch().count() );
      for ( auto const& counter : m_impl->m_counter )
      {
         pt::ptree ptCounter;
         ptCounter.put( "name", counter.first );
         ptCounter.put( "active", counter.second.m_active.count() );
         ptCounter.put( "lastChange", counter.second.m_lastChange.time_since_epoch().count() );
         tree.add_child( "statistics.counters.counter", ptCounter );
      }
      pt::write_xml( m_impl->m_path.string(), tree, std::locale(), pt::xml_writer_make_settings<std::string>( ' ', 2 ) );
   }
   
   void Statistics::load()
   {
      pt::ptree tree;
      pt::read_xml( m_impl->m_path.string(), tree, pt::xml_parser::trim_whitespace );
      m_impl->m_lastRun = ch::system_clock::time_point( ch::system_clock::time_point::duration( tree.get( "statistics.lastRun", m_impl->m_now.time_since_epoch().count() ) ) );
      boost::optional< pt::ptree& > ptCounters( tree.get_child_optional( "statistics.counters" ) );
      if ( ptCounters )
      {
         for ( auto const& child : ptCounters->get_child( "" ) )
         {
            pt::ptree const& ptCounter( child.second );
            std::string const name( ptCounter.get< std::string >( "name" ) );
            Counter counter;
            counter.m_active = ch::seconds( ptCounter.get( "active", 0 ) );
            counter.m_lastChange = ch::system_clock::time_point( ch::system_clock::time_point::duration( ptCounter.get( "lastChange", m_impl->m_now.time_since_epoch().count() ) ) );
            m_impl->m_counter.emplace( CounterMapType::value_type( name, counter ) );
         }
      }
   }

}}
