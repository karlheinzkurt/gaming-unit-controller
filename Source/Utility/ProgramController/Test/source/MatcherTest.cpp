
#include "Utility/ProgramController/Common/include/CMatcher.h"
#include "Utility/ProgramController/Common/include/CMatcherFactory.h"

#include "Infrastructure/Mock/include/CProcess.h"

#include <boost/property_tree/ptree.hpp>

#include <gtest/gtest.h>

using namespace Utility::ProgramController;

TEST(Matcher, NoRuleNoCommandLineNoMatch)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return(""));
   
   CMatcher matcher;
   EXPECT_EQ(size_t(0), matcher.matches(process).size());
}

TEST(Matcher, NoRuleNoMatch)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return("/usr/bin/java -Djava.awt.headless=true -jar /usr/share/jenkins/jenkins.war --webroot=/var/cache/jenkins/war --httpPort=9090"));
   
   CMatcher matcher;
   EXPECT_EQ(size_t(0), matcher.matches(process).size());
}

TEST(Matcher, NoCommandLineNoMatch)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return(""));
   
   CMatcher matcher;
   matcher.add(CMatchingRule("thatRule", {".*java.*jenkins.*"}, {}));
   EXPECT_EQ(size_t(0), matcher.matches(process).size());
}

TEST(Matcher, MatchingRule)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return("/usr/bin/java -Djava.awt.headless=true -jar /usr/share/jenkins/jenkins.war --webroot=/var/cache/jenkins/war --httpPort=9090"));
   
   CMatcher matcher;
   matcher.add(CMatchingRule("thatRule", {".*java.*jenkins.*"}, {}));
   auto matches(matcher.matches(process));
   EXPECT_EQ(size_t(1), matches.size());
   EXPECT_EQ("thatRule", (*matches.begin())->getName());
   EXPECT_EQ(process->getCommandLine(), (*(*matches.begin())->getProcesses().begin())->getCommandLine());
}

TEST(Matcher, NotMatchingRule)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return("/usr/bin/java -Djava.awt.headless=true -jar /usr/share/jenkins/jenkins.war --webroot=/var/cache/jenkins/war --httpPort=9090"));
   
   CMatcher matcher;
   matcher.add(CMatchingRule("thatRule", {".*java.*"}, {".*jenkins.*"}));
   auto matches(matcher.matches(process));
   EXPECT_EQ(size_t(0), matches.size());
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
