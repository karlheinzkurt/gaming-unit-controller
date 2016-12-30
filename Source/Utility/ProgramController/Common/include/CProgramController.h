#pragma once

#include "Infrastructure/API/include/ISystem.h"
#include "Utility/ProgramController/Common/include/IRunningStrategy.h"
#include "Utility/ProgramController/Common/include/IMatcher.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

namespace Utility
{
namespace ProgramController
{
   struct CProgramController
   {
      /** All those paths should be relative to the directory the 
          executable is located.
        */
      
      static boost::filesystem::path getDefaultCounterFilePath();
      
      static boost::filesystem::path getDefaultLogFilePath();
      
      static boost::filesystem::path getDefaultConfigurationFilePath();
      
      static boost::filesystem::path getDefaultLoggerConfigurationFilePath();
      
      CProgramController( 
          Infrastructure::API::ISystem& system
         ,API::IRunningStrategy& runningStrategy
         ,boost::filesystem::path const& configurationFilePath
         ,boost::filesystem::path const& counterFilePath );
      
      void load(boost::filesystem::path const& path);
      
      void save(boost::filesystem::path const& path) const;
      
   private:
      Infrastructure::API::ISystem& m_system;
      API::IRunningStrategy& m_runningStrategy;
      boost::filesystem::path m_configurationFilePath;
      log4cxx::LoggerPtr m_logger;
      std::unique_ptr<Utility::ProgramController::API::IMatcher> m_matcher;
   };
}}
