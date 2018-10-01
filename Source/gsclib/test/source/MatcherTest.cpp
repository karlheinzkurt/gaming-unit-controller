
#include "gsclib/common/include/CMatcher.h"
#include "gsclib/common/include/CMatchingRule.h"
#include "gsclib/common/include/CMatcherFactory.h"

#include "gsclib/infrastructure/mock/include/CProcess.h"

#include <boost/property_tree/ptree.hpp>

#include <gtest/gtest.h>

using namespace GSC::Common;

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
   matcher.add(std::make_unique<CMatchingRule>("thatRule", std::chrono::seconds(), CMatchingRule::Implication::Warn, std::list<std::string>{".*java.*jenkins.*"}, std::list<std::string>{}));
   EXPECT_EQ(size_t(0), matcher.matches(process).size());
}

TEST(Matcher, MatchingRule)
{
   auto process(std::make_shared<::testing::NiceMock<Infrastructure::Mock::CProcess>>());
   EXPECT_CALL(*process, getCommandLine()).WillRepeatedly(::testing::Return("/usr/bin/java -Djava.awt.headless=true -jar /usr/share/jenkins/jenkins.war --webroot=/var/cache/jenkins/war --httpPort=9090"));
   
   CMatcher matcher;
   matcher.add(std::make_unique<CMatchingRule>("thatRule", std::chrono::seconds(), CMatchingRule::Implication::Warn, std::list<std::string>{".*java.*jenkins.*"}, std::list<std::string>{}));
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
   matcher.add(std::make_unique<CMatchingRule>("thatRule", std::chrono::seconds(), CMatchingRule::Implication::Warn, std::list<std::string>{".*java.*"}, std::list<std::string>{".*jenkins.*"}));
   auto matches(matcher.matches(process));
   EXPECT_EQ(size_t(0), matches.size());
}

TEST(Matcher, SerializeRoundTrip)
{ 
   CMatcherFactory factory;
   auto source = factory.create();
   source->add(std::make_unique<CMatchingRule>("bli", std::chrono::seconds(), CMatchingRule::Implication::Warn, std::list<std::string>{"java.+", "blub$"}, std::list<std::string>{".*eclipse.*"}));
   source->add(std::make_unique<CMatchingRule>("bla", std::chrono::seconds(), CMatchingRule::Implication::Warn, std::list<std::string>{"java.+"}, std::list<std::string>{".*jenkins.*", "^blabla.*"}));
   boost::property_tree::ptree pt = source->serialize();
   auto destination = factory.create(pt);
   
   EXPECT_EQ(source->getRules(), destination->getRules());
}
