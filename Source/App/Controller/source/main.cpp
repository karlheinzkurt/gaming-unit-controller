
#include "../include/CProgramController.h"

#include "Lib/Controller/include/CMatcher.h"

#include <log4cxx/xml/domconfigurator.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <chrono>

#include <csignal>

void signalHandler( int sig )
{
   switch(sig)
   {
      case SIGHUP:  {} break;
      case SIGTERM: {  exit(0); } break;
   }	
}

int main( int argc, char** argv )
{
   signal( SIGHUP, signalHandler );  /* hangup signal */
   signal( SIGTERM, signalHandler ); /* software termination signal from kill */
   
   namespace po = boost::program_options;
   namespace fs = boost::filesystem;
   
   fs::path const executablePath( /*fs::canonical( fs::absolute(*/ fs::path( argv[0] ).parent_path() /*) )*/ );
   
   po::options_description desc("General options");
   desc.add_options()
       ("help",                  "This help message")
       ("check-interval,i"      ,po::value< uint64_t >()->default_value( std::chrono::seconds( 60 ).count() )
                                ,"Interval to check for matching processes in seconds" )
       ("counter-file,c"        ,po::value< fs::path >()->default_value( executablePath / "App.Utility.Controller.Counter.xml" )
                                ,"Path to config file") 
       ("log-file,l"            ,po::value< fs::path >()->default_value( executablePath / "App.Utility.Controller.log" )
                                ,"Path to logger config file")
       ("config-file"           ,po::value< fs::path >()->default_value( executablePath / "etc" / "App.Utility.Controller.Config.xml" )
                                ,"Path to configuration file")
       ("logger-config-file"    ,po::value< fs::path >()->default_value( executablePath / "etc" / "App.Utility.Controller.Logger.xml" )
                                ,"Path to logger configuration file");

   try
   {
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);

      if (vm.count("help"))
      {
         std::cout << desc << "\n";
         return 1;
      }

      fs::path const counterFilePath( vm[ "counter-file" ].as< fs::path >() );
      fs::path const logFilePath( vm[ "log-file" ].as< fs::path >() );
      fs::path const configurationFilePath( vm[ "config-file" ].as< fs::path >() );
      fs::path const loggerConfigurationFilePath( vm[ "logger-config-file" ].as< fs::path >() );
      
      setenv("logfile.path", logFilePath.string().c_str(), 1);
      log4cxx::xml::DOMConfigurator::configure( loggerConfigurationFilePath.string() );

      Lib::Controller::CMatcher matcher;
      matcher.addRule( "browser", { ".*browser.*" } );
      matcher.addRule( "gvfs", { ".*gvfs.*" }, { ".*volume[-]monitor.*" } );
      SL::App::Utility::ProgramController controller( 
          configurationFilePath
         ,counterFilePath
         ,std::move( matcher ) );
   }
   catch ( std::exception const& e )
   {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
   }
   return 0;   
}
