#pragma once

#include <boost/filesystem/path.hpp>

namespace Infrastructure
{
namespace Common
{
   struct Logger
   {
      static void configureLog4cxx(boost::filesystem::path configurationFilePath, boost::filesystem::path logFilePath);
   };   
}}
