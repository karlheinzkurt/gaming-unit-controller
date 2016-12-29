
#include "Utility/ProgramController/Common/include/CMatcher.h"
#include "Utility/ProgramController/Common/include/CMatcherFactory.h"

#include <boost/property_tree/ptree.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace Utility::ProgramController;

struct CProcessMock: Infrastructure::API::IProcess
{
   MOCK_CONST_METHOD0(isRunning, bool());
   MOCK_CONST_METHOD0(isSignallable, bool());
   MOCK_CONST_METHOD0(getProcessId, int());
   MOCK_CONST_METHOD0(getCommandLine, std::string());
   MOCK_CONST_METHOD0(toString, std::string());
   MOCK_METHOD1(terminateAndWait, bool(std::chrono::seconds const& timeout));
   MOCK_METHOD1(killAndWait, bool(std::chrono::seconds const& timeout));
   MOCK_CONST_METHOD0(clone, std::unique_ptr< IProcess >());
};

TEST(Matcher, NoRule)
{
   ::testing::StrictMock<CProcessMock> processMock;
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
