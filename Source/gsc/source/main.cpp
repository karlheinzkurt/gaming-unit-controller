
#include "gsclib/common/include/CSessionController.h"
#include "gsclib/common/include/CRunningStrategy.h"

#include "gsclib/infrastructure/common/include/System.h"
#include "gsclib/infrastructure/common/include/Logger.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

int main( int argc, char** argv )
{  
   namespace bpo = boost::program_options;
   namespace bfs = boost::filesystem;
   namespace UPC = GSC::Common;
   
   auto const executablePath(bfs::path(argv[0]).parent_path());
   
   bpo::options_description desc("General options");
   desc.add_options()
      ("help,h"
          // no argument
         ,"This help message")
      ("daemon,d"
          // no argument
         ,"Run continuously")
      ("check-interval,i"      
         ,bpo::value< uint64_t >()->default_value( std::chrono::seconds( 60 ).count() )
         ,"Interval to check for matching processes in seconds" )
      ("counter-file,c"
         ,bpo::value<bfs::path>()->default_value(executablePath / UPC::CSessionController::getDefaultCounterFilePath())
         ,"Path to file containing counter information") 
      ("log-file,l"
         ,bpo::value<bfs::path>()->default_value(executablePath / UPC::CSessionController::getDefaultLogFilePath())
         ,"Path to log-file")
      ("config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / UPC::CSessionController::getDefaultConfigurationFilePath())
         ,"Path to configuration file")
      ("logger-config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / UPC::CSessionController::getDefaultLoggerConfigurationFilePath())
         ,"Path to logger configuration file");

   try
   {
      bpo::variables_map vm;
      bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
      bpo::notify(vm);

      if (vm.count("help"))
      {
         std::cout << desc << "\n";
         return 0;
      }

      auto const counterFilePath(                     vm["counter-file"       ].as<bfs::path>());
      auto const logFilePath(                         vm["log-file"           ].as<bfs::path>());
      auto const configurationFilePath(               vm["config-file"        ].as<bfs::path>());
      auto const loggerConfigurationFilePath(         vm["logger-config-file" ].as<bfs::path>());
      auto const checkInterval(std::chrono::seconds(  vm["check-interval"     ].as<size_t>()));
      auto const runningStrategy(                     vm.count("daemon") > 0
         ? std::unique_ptr<UPC::API::IRunningStrategy>(new UPC::CDaemonRunningStrategy(checkInterval))
         : std::unique_ptr<UPC::API::IRunningStrategy>(new UPC::CStraightRunningStrategy));
      
      Infrastructure::Common::Logger::configureLog4cxx(loggerConfigurationFilePath, logFilePath);

      auto& system(Infrastructure::Common::System::get());
      auto controller(std::make_unique<UPC::CSessionController>(
          system
         ,*runningStrategy
         ,configurationFilePath
         ,counterFilePath));
      
      /** \todo Use signal handler stuff to enable daemon to re-read the config or to shutdown properly
          \todo Maybe move this signal handler stuff somewhere
       */
      std::vector<std::unique_ptr<Infrastructure::API::ISignalHandler>> handlers;
      handlers.emplace_back(system.installSignalHandler(Infrastructure::API::Signal::Hup,  [](auto signal){ /* \todo handle */ }));
      handlers.emplace_back(system.installSignalHandler(Infrastructure::API::Signal::Term, [](auto signal){ /* \todo handle */ }));
   }
   catch ( std::exception const& e )
   {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
   }
   return 0;   
}
