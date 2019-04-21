#pragma once

#include "gsclib/infrastructure/api/include/IProcess.h"

#include <gmock/gmock.h>

namespace Infrastructure
{
namespace Mock
{
   struct CProcess : Infrastructure::API::IProcess
   {
      MOCK_CONST_METHOD0(isRunning, bool());
      MOCK_CONST_METHOD0(isSignallable, bool());
      MOCK_CONST_METHOD0(getProcessId, int());
      MOCK_CONST_METHOD0(getCommandLine, std::string());
      MOCK_CONST_METHOD0(toString, std::string());
      MOCK_METHOD1(terminateAndWait, bool(std::chrono::seconds const& timeout));
      MOCK_METHOD1(killAndWait, bool(std::chrono::seconds const& timeout));
   };
}}
