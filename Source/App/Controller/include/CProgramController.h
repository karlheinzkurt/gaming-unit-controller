#pragma once

#include "Lib/Infrastructure/API/include/ISystem.h"
#include "Lib/Controller/include/IMatcher.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

namespace SL
{
namespace App
{
namespace Utility
{
   
   struct ProgramController
   {
      ProgramController( 
          Lib::Infrastructure::API::ISystem& system
         ,boost::filesystem::path const& configurationFilePath
         ,boost::filesystem::path const& counterFilePath );
      
      void load(boost::filesystem::path const& path);
      
      void save(boost::filesystem::path const& path) const;
      
   private:
      Lib::Infrastructure::API::ISystem& m_system;
      boost::filesystem::path m_configurationFilePath;
      log4cxx::LoggerPtr m_logger;
      std::unique_ptr<Lib::Controller::API::IMatcher> m_matcher;
   };

}}}

