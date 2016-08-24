#pragma once

#include "Lib/Controller/include/CMatcher.h"

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
      ProgramController( boost::filesystem::path const& absoluteStatisticPath
                        ,Lib::Controller::Matcher matcher );
      
   private:
      log4cxx::LoggerPtr m_logger;
   };

}}}

