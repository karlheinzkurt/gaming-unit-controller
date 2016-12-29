#pragma once

#include "Infrastructure/API/include/ISystem.h"
#include "Utility/ProgramController/Common/include/IMatcher.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

namespace Utility
{
namespace ProgramController
{
   struct ProgramController
   {
      ProgramController( 
          Infrastructure::API::ISystem& system
         ,boost::filesystem::path const& configurationFilePath
         ,boost::filesystem::path const& counterFilePath );
      
      void load(boost::filesystem::path const& path);
      
      void save(boost::filesystem::path const& path) const;
      
   private:
      Infrastructure::API::ISystem& m_system;
      boost::filesystem::path m_configurationFilePath;
      log4cxx::LoggerPtr m_logger;
      std::unique_ptr<Utility::ProgramController::API::IMatcher> m_matcher;
   };
}}
