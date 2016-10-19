
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Lib/Controller/include/CMatcher.h"

using Lib::Controller::CMatcher;
using Lib::Controller::API::IMatch;
using Lib::Controller::API::CRule;

struct CProcessMock: Lib::Infrastructure::API::IProcess
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

TEST(MatcherTest, Match)
{
   CMatcher matcher;
   ::testing::StrictMock<CProcessMock> processMock;
   EXPECT_CALL(processMock, getCommandLine()).WillRepeatedly(::testing::Return("running --while --done"));;
   
   matcher.add(CRule()).add(CRule());
   EXPECT_EQ(size_t(0), matcher.matches(processMock).size());
}
