
#include "../include/Logger.h"

#include <log4cxx/xml/domconfigurator.h>

#include <cstdlib>

namespace Infrastructure
{
namespace Common
{
   void Logger::configureLog4cxx(boost::filesystem::path configurationFilePath, boost::filesystem::path logFilePath)
   {
      setenv("logfile.path", logFilePath.string().c_str(), 1);
      log4cxx::xml::DOMConfigurator::configure(configurationFilePath.string());
   }   
}}
