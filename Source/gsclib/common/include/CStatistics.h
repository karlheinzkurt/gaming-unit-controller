#pragma once

#include <boost/filesystem/path.hpp>
#include <log4cxx/logger.h>

#include <set>
#include <chrono>
#include <string>

namespace GSC
{
namespace Common
{
   struct Statistics
   {
      Statistics( log4cxx::LoggerPtr logger, boost::filesystem::path const& path );
      
      ~Statistics(); ///< keep to get private impl deleted properly
      
      void add( std::string const& match );
      std::set< std::string > getCurrentlyExceeding( std::chrono::seconds const& limit );
      
   private:
      void load();
      void save() const;
   
   private:
      struct Impl;
      std::unique_ptr< Impl > m_impl;     
   };

}}
