#pragma once

#include "guclib/infrastructure/api/include/ISystem.h"

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
