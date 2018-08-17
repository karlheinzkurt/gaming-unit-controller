#pragma once

#include <memory>

#include "gsclib/api/include/IMatcher.h"

namespace GSC { namespace Common {

struct CInfluxAdapter
{
   CInfluxAdapter();
   CInfluxAdapter(CInfluxAdapter const&) = delete;
   CInfluxAdapter(CInfluxAdapter&&) = delete;
   CInfluxAdapter& operator=(CInfluxAdapter const&) = delete;
   CInfluxAdapter& operator=(CInfluxAdapter&&) = delete;
   ~CInfluxAdapter();
   
   void insertActive(API::IMatch::SetType const& active);
   void insertExceeding(API::IExceedingMatch::SetType const& exceeding);
   
   void ping();
   
private:
   struct Impl;
   std::unique_ptr<Impl> m_impl;
};

}}