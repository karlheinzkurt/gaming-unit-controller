#pragma once

#include "gsclib/infrastructure/api/include/ISystem.h"
#include "gsclib/common/include/IRunningStrategy.h"
#include "gsclib/common/include/IMatcher.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

namespace GSC
{
namespace Common
{
   struct CSessionController
   {
      /** All those paths should be relative to the directory the 
          executable is located.
        */
      
      static boost::filesystem::path getDefaultCounterFilePath();
      
      static boost::filesystem::path getDefaultLogFilePath();
      
      static boost::filesystem::path getDefaultConfigurationFilePath();
      
      static boost::filesystem::path getDefaultLoggerConfigurationFilePath();
      
      CSessionController( 
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
      std::unique_ptr<GSC::Common::API::IMatcher> m_matcher;
   };
}}
