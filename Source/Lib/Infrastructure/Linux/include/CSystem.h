#pragma once

#include "Lib/Infrastructure/API/include/ISystem.h"
#include "Lib/Infrastructure/API/include/IProcess.h"

namespace Lib
{
namespace Infrastructure
{
namespace Linux
{
   
   struct CSystem : public API::ISystem
   {            
      virtual API::IProcess::SetType getRunningProcesses() override;
      
      virtual API::IProcess::SetType getSignallableProcesses() override;
   };

}}}
