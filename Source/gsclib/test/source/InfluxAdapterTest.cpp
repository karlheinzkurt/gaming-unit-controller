
#include "gsclib/common/include/CInfluxAdapter.h"

#include <gtest/gtest.h>

using namespace GSC::Common;

TEST(InfluxAdapter, Ping)
{
   auto adapter(std::make_unique<CInfluxAdapter>());
   adapter->ping();
}