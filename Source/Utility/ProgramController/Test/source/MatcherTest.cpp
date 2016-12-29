
#include "Utility/ProgramController/Common/include/CMatcher.h"
#include "Utility/ProgramController/Common/include/CMatcherFactory.h"

#include "Infrastructure/Mock/include/CProcess.h"

#include <boost/property_tree/ptree.hpp>

#include <gtest/gtest.h>

using namespace Utility::ProgramController;

TEST(Matcher, NoRule)
{
   ::testing::StrictMock<Infrastructure::Mock::CProcess> processMock;
   EXPECT_CALL(processMock, getCommandLine()).WillRepeatedly(::testing::Return(""));;   
   
   CMatcher matcher;
   EXPECT_EQ(size_t(0), matcher.matches(processMock).size());
}

TEST(Matcher, SerializeRoundTrip)
{ 
   CMatcherFactory factory;
   auto source = factory.create();
   source->add(CMatchingRule("bli", {"java.+", "blub$"}, {".*eclipse.*"}));
   source->add(CMatchingRule("bla", {"java.+"}, {".*jenkins.*", "^blabla.*"}));
   boost::property_tree::ptree pt = source->serialize();
   auto destination = factory.create(pt);
   
   EXPECT_EQ(source->getRules(), destination->getRules());
}
