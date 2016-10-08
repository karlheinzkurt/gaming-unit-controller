#pragma once

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
          boost::filesystem::path const& configurationFilePath
         ,boost::filesystem::path const& counterFilePath
         ,Lib::Controller::API::IMatcher const& matcher );
      
   private:
      log4cxx::LoggerPtr m_logger;
   };

}}}

