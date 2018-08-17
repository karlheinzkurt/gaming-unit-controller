#pragma once

#include "gsclib/api/include/IMatcher.h"

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
      Statistics(log4cxx::LoggerPtr logger, boost::filesystem::path const& path);
      
      ~Statistics(); ///< keep to get private impl deleted properly
      
      API::IRatedMatch::SetType rate(API::IMatch::SetType const& matches);
      
   private:
      void load();
      void save() const;
   
   private:
      struct Impl;
      std::unique_ptr< Impl > m_impl;     
   };

}}
