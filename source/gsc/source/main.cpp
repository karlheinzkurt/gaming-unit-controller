
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
   
   using GSC::Common::CSessionController;
   
   auto const executablePath(bfs::path(argv[0]).parent_path());
   
   bpo::options_description desc("General options");
   desc.add_options()
      ("help,h",   "This help message")
      ("daemon,d", "Run continuously")
      ("check-interval,i"      
         ,bpo::value< uint64_t >()->default_value( std::chrono::seconds( 60 ).count() )
         ,"Interval to check for matching processes in seconds" )
      ("counter-file,c"
         ,bpo::value<bfs::path>()->default_value(executablePath / CSessionController::getDefaultCounterFilePath())
         ,"Path to file containing counter information") 
      ("log-file,l"
         ,bpo::value<bfs::path>()->default_value(executablePath / CSessionController::getDefaultLogFilePath())
         ,"Path to log-file")
      ("config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / CSessionController::getDefaultConfigurationFilePath())
         ,"Path to configuration file")
      ("logger-config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / CSessionController::getDefaultLoggerConfigurationFilePath())
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
      
      auto& system(Infrastructure::Common::System::get());
      std::unique_ptr<GSC::API::IRunningStrategy> runningStrategy;
      std::vector<std::unique_ptr<Infrastructure::API::ISignalHandler>> handlers;
      if (vm.count("daemon"))
      {
         runningStrategy = std::make_unique<GSC::Common::CDaemonRunningStrategy>(checkInterval);
         handlers.emplace_back(system.installSignalHandler(Infrastructure::API::Signal::Term, [&runningStrategy](auto signal)
         {  runningStrategy->cancel(); }));
      }
      else
      {  runningStrategy = std::make_unique<GSC::Common::CStraightRunningStrategy>(); }
      
      Infrastructure::Common::Logger::configureLog4cxx(loggerConfigurationFilePath, logFilePath);
      auto controller(std::make_unique<GSC::Common::CSessionController>(
          system
         ,*runningStrategy
         ,configurationFilePath
         ,counterFilePath));
   }
   catch ( std::exception const& e )
   {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
   }
   return 0;   
}
