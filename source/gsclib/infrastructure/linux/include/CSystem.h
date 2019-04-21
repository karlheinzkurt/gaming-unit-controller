#pragma once

#include "gsclib/infrastructure/api/include/ISystem.h"
#include "gsclib/infrastructure/api/include/IProcess.h"

namespace Infrastructure
{
namespace Linux
{
   
   struct CSystem : public API::ISystem
   {            
      virtual API::IProcess::SetType getRunningProcesses() override;
      
      virtual API::IProcess::SetType getSignallableProcesses() override;
      
      virtual std::unique_ptr<API::ISignalHandler> installSignalHandler(API::Signal signal, std::function<void(API::Signal)> function) override;
   };

}}
