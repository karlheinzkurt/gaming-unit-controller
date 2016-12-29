#pragma once

#include "Infrastructure/API/include/ISystem.h"

#include <gmock/gmock.h>

namespace Infrastructure
{
namespace Mock
{
   struct CSystem : API::ISystem
   {
      MOCK_METHOD0(getRunningProcesses, API::IProcess::SetType());
      MOCK_METHOD0(getSignallableProcesses, API::IProcess::SetType());
   };
}}
